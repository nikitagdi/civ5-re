// civ5-re analysis profile prescript: applies tools/ghidra/analysis.properties before auto-analysis.
// Args: [--profile=<path>] [--seeds=<file>]. Default profile: ../analysis.properties next to this script's folder.
// --seeds: one 0x%08x address per line (# comments); a function is created at each one that has none yet,
// before auto-analysis, so analyzers and Param ID see them (vtable slot targets, exported FUNC symbols).
//@category civ5re
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.FlowOverride;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.mem.MemoryBlock;

import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.LinkedHashMap;
import java.util.Map;

public class Civ5reOptions extends GhidraScript {

    @Override
    public void run() throws Exception {
        File profile = null;
        File seeds = null;
        String[] args = getScriptArgs();
        for (int i = 0; i < args.length; i++) {
            if (args[i].startsWith("--seeds=")) {
                seeds = new File(args[i].substring("--seeds=".length()));
            } else if (args[i].equals("--seeds") && i + 1 < args.length) {
                seeds = new File(args[++i]);
            } else if (args[i].startsWith("--profile=")) {
                profile = new File(args[i].substring("--profile=".length()));
            } else if (args[i].equals("--profile") && i + 1 < args.length) {
                profile = new File(args[++i]); // analyzeHeadless.bat splits "--profile=X" at '='
            }
        }
        if (profile == null) {
            profile = new File(sourceFile.getParentFile().getParentFile().getFile(false), "analysis.properties");
        }
        Map<String, String> opts = new LinkedHashMap<>();
        Map<String, String> pins = new LinkedHashMap<>();
        int n = 0;
        for (String raw : Files.readAllLines(profile.toPath(), StandardCharsets.UTF_8)) {
            n++;
            String line = raw.strip();
            if (line.isEmpty() || line.startsWith("#")) {
                continue;
            }
            int eq = line.indexOf('=');
            if (eq <= 0) {
                throw new IllegalArgumentException(profile + ":" + n + ": expected '<option> = <value>'");
            }
            String key = line.substring(0, eq).strip();
            String value = line.substring(eq + 1).strip();
            (key.startsWith("@") ? pins : opts).put(key, value);
        }

        String lang = currentProgram.getLanguageID().getIdAsString();
        String cspec = currentProgram.getCompilerSpec().getCompilerSpecID().getIdAsString();
        if (pins.containsKey("@processor") && !pins.get("@processor").equals(lang)) {
            throw new IllegalStateException("civ5re: language " + lang + " != profile " + pins.get("@processor"));
        }
        if (pins.containsKey("@cspec") && !pins.get("@cspec").equals(cspec)) {
            throw new IllegalStateException("civ5re: cspec " + cspec + " != profile " + pins.get("@cspec"));
        }

        Map<String, String> known = getCurrentAnalysisOptionsAndValues(currentProgram);
        Map<String, String> apply = new LinkedHashMap<>();
        for (Map.Entry<String, String> e : opts.entrySet()) {
            if (known.containsKey(e.getKey())) {
                apply.put(e.getKey(), e.getValue());
            } else {
                println("civ5re: option not applicable, skipped: " + e.getKey());
            }
        }
        setAnalysisOptions(currentProgram, apply);
        Map<String, String> now = getCurrentAnalysisOptionsAndValues(currentProgram);
        for (Map.Entry<String, String> e : apply.entrySet()) {
            String got = now.get(e.getKey());
            if (!e.getValue().equals(got)) {
                throw new IllegalStateException("civ5re: option " + e.getKey() + " = " + got + ", wanted " + e.getValue());
            }
        }
        println("civ5re: profile " + profile + ": " + apply.size() + " options, " + lang + " " + cspec);
        picCalls(); // before the seeds are disassembled
        if (seeds != null) {
            seedFunctions(seeds);
        }
    }

    // `call next; pop reg` (E8 00 00 00 00 58..5F) is a get-PC, not a call: Ghidra would start a function at the
    // next instruction and cut the real one there. The call is disassembled alone and overridden to a branch, so
    // the flow simply continues. Keep the pattern equal to headless.PIC_CALL.
    private void picCalls() throws Exception {
        int found = 0, done = 0;
        for (MemoryBlock b : currentProgram.getMemory().getBlocks()) {
            if (!b.isInitialized() || !b.isExecute()) {
                continue;
            }
            byte[] data = new byte[(int) b.getSize()];
            b.getBytes(b.getStart(), data);
            for (int i = 0; i + 5 < data.length; i++) {
                if ((data[i] & 0xff) != 0xe8 || data[i + 1] != 0 || data[i + 2] != 0 || data[i + 3] != 0
                    || data[i + 4] != 0 || (data[i + 5] & 0xf8) != 0x58) {
                    continue;
                }
                found++;
                Address a = b.getStart().add(i);
                if (getInstructionAt(a) == null) {
                    new DisassembleCommand(a, new AddressSet(a, a.add(4)), false).applyTo(currentProgram, monitor);
                }
                Instruction ins = getInstructionAt(a);
                if (ins != null && ins.getLength() == 5 && ins.getMnemonicString().equalsIgnoreCase("CALL")) {
                    ins.setFlowOverride(FlowOverride.BRANCH);
                    done++;
                }
            }
        }
        println("civ5re: get-PC calls (call next; pop): " + found + " found, " + done + " overridden to branches");
    }

    private void seedFunctions(File seeds) throws Exception {
        FunctionManager fm = currentProgram.getFunctionManager();
        int total = 0, existed = 0, created = 0, failed = 0;
        for (String raw : Files.readAllLines(seeds.toPath(), StandardCharsets.UTF_8)) {
            String line = raw.strip();
            if (line.isEmpty() || line.startsWith("#")) {
                continue;
            }
            monitor.checkCancelled();
            total++;
            Address a = toAddr(Long.parseUnsignedLong(line.substring(2), 16));
            if (fm.getFunctionAt(a) != null) {
                existed++;
                continue;
            }
            boolean ok = false;
            try {
                disassemble(a);
                ok = createFunction(a, null) != null;
            } catch (Exception e) {
                // counted below
            }
            if (ok) {
                created++;
            } else {
                failed++;
                println("civ5re: seed not created: " + line);
            }
        }
        println("civ5re: seeds " + seeds + ": " + total + " addresses, " + existed + " existed, " + created +
            " created, " + failed + " failed");
    }
}
