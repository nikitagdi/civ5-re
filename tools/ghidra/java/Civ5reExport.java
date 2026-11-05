// civ5-re function list export (ghidra-functions.txt v2) with the ECX/EDX entry-read detector.
// Args: [--out=<file>] [--cache-key=<key12>]. Defaults: <project dir>/<project name>.functions.txt,
// key from a project name "pristine-<key12>".
//@category civ5re
import ghidra.app.script.GhidraScript;
import ghidra.framework.Application;
import ghidra.framework.model.ProjectLocator;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressRange;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.pcode.PcodeOp;
import ghidra.program.model.pcode.Varnode;
import ghidra.program.model.symbol.FlowType;
import ghidra.program.model.symbol.SymbolTable;

import java.io.File;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.List;

public class Civ5reExport extends GhidraScript {

    static final String VERSION = "1";
    static final int MAX_INSNS = 64;

    private Register[] regs;

    @Override
    public void run() throws Exception {
        String out = null;
        String key = null;
        String[] args = getScriptArgs();
        for (int i = 0; i < args.length; i++) {
            String a = args[i];
            if (a.startsWith("--out=")) {
                out = a.substring(6);
            } else if (a.startsWith("--cache-key=")) {
                key = a.substring(12);
            } else if (a.equals("--out") && i + 1 < args.length) {
                out = args[++i]; // analyzeHeadless.bat splits "--out=X" at '='
            } else if (a.equals("--cache-key") && i + 1 < args.length) {
                key = args[++i];
            }
        }
        ProjectLocator loc = state.getProject() != null ? state.getProject().getProjectLocator() : null;
        if (key == null) {
            key = loc != null && loc.getName().startsWith("pristine-") ? loc.getName().substring(9) : "-";
        }
        if (out == null) {
            if (loc == null) {
                throw new IllegalArgumentException("civ5re: --out= is required without a project");
            }
            out = new File(loc.getLocation(), loc.getName() + ".functions.txt").getPath();
        }
        regs = new Register[] { currentProgram.getRegister("ECX"), currentProgram.getRegister("EDX") };
        SymbolTable st = currentProgram.getSymbolTable();

        List<String> lines = new ArrayList<>();
        for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
            monitor.checkCancelled();
            Address entry = f.getEntryPoint();
            AddressRange r = f.getBody().getRangeContaining(entry);
            Address end = r != null ? r.getMaxAddress().add(1) : entry.add(1);
            String flags = "-";
            if (!st.isExternalEntryPoint(entry)) {
                flags = detect(entry);
            }
            lines.add(String.format("0x%08x\t0x%08x\t%s\t%s", entry.getOffset(), end.getOffset(),
                f.getName(true).replace('\t', ' '), flags));
        }

        File dst = new File(out);
        File tmp = new File(dst.getPath() + ".tmp");
        try (PrintWriter w = new PrintWriter(Files.newBufferedWriter(tmp.toPath(), StandardCharsets.UTF_8))) {
            w.print("# civ5re ghidra-functions 2\n");
            w.print("# binary_sha256=" + currentProgram.getExecutableSHA256() + " ghidra=" +
                Application.getApplicationVersion() + " cache_key=" + key + " export=" + VERSION +
                " functions=" + lines.size() + "\n");
            w.print("# start\tend\tname\tflags\n");
            for (String l : lines) {
                w.print(l);
                w.print('\n');
            }
        }
        Files.move(tmp.toPath(), dst.toPath(), StandardCopyOption.REPLACE_EXISTING);
        println("civ5re: " + lines.size() + " functions -> " + dst);
    }

    private int regIndex(Varnode v) {
        if (v == null || !v.isRegister()) {
            return -1;
        }
        for (int i = 0; i < regs.length; i++) {
            Address a = regs[i].getAddress();
            long lo = a.getOffset(), hi = lo + regs[i].getMinimumByteSize() - 1;
            long vlo = v.getOffset(), vhi = vlo + v.getSize() - 1;
            if (v.getAddress().getAddressSpace().equals(a.getAddressSpace()) && vlo <= hi && vhi >= lo) {
                return i;
            }
        }
        return -1;
    }

    // Registers read on entry before being written, within the first block (up to the first jump or call).
    String detect(Address entry) {
        boolean[] written = new boolean[regs.length];
        boolean[] read = new boolean[regs.length];
        Instruction ins = getInstructionAt(entry);
        for (int n = 0; ins != null && n < MAX_INSNS; n++) {
            String m = ins.getMnemonicString().toUpperCase();
            boolean push = m.startsWith("PUSH");
            if ((m.equals("XOR") || m.equals("SUB")) && ins.getNumOperands() == 2) {
                Object[] a = ins.getOpObjects(0), b = ins.getOpObjects(1);
                if (a.length == 1 && b.length == 1 && a[0] instanceof Register && a[0].equals(b[0])) {
                    Varnode v = new Varnode(((Register) a[0]).getAddress(), ((Register) a[0]).getMinimumByteSize());
                    int i = regIndex(v);
                    if (i >= 0) {
                        written[i] = true;
                    }
                    ins = ins.getNext();
                    continue;
                }
            }
            for (PcodeOp op : ins.getPcode()) {
                if (!push) {
                    for (Varnode in : op.getInputs()) {
                        int i = regIndex(in);
                        if (i >= 0 && !written[i]) {
                            read[i] = true;
                        }
                    }
                }
                int o = regIndex(op.getOutput());
                if (o >= 0) {
                    written[o] = true;
                }
            }
            FlowType ft = ins.getFlowType();
            if (ft.isJump() || ft.isCall() || ft.isTerminal() || !ft.hasFallthrough()) {
                break;
            }
            ins = ins.getNext();
        }
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < regs.length; i++) {
            if (read[i]) {
                sb.append(sb.length() == 0 ? "r=" : ",").append(regs[i].getName());
            }
        }
        return sb.length() == 0 ? "-" : sb.toString();
    }
}
