// Integration-test helper: --decompile=<out.txt> writes the decompiler output of all functions;
// --rename=<0xaddr>:<name> renames a function as a manual (USER_DEFINED) edit.
//@category civ5re
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;

import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.List;

public class Civ5reTestProbe extends GhidraScript {

    @Override
    public void run() throws Exception {
        // analyzeHeadless.bat splits "--key=value" at '=': rejoin.
        List<String> args = new ArrayList<>();
        String[] raw = getScriptArgs();
        for (int i = 0; i < raw.length; i++) {
            if ((raw[i].equals("--rename") || raw[i].equals("--decompile")) && i + 1 < raw.length) {
                args.add(raw[i] + "=" + raw[++i]);
            } else {
                args.add(raw[i]);
            }
        }
        for (String a : args) {
            if (a.startsWith("--rename=")) {
                String[] p = a.substring(9).split(":", 2);
                Function f = getFunctionAt(toAddr(Long.decode(p[0])));
                int tx = currentProgram.startTransaction("probe rename");
                try {
                    f.getSymbol().setName(p[1], SourceType.USER_DEFINED);
                } finally {
                    currentProgram.endTransaction(tx, true);
                }
            } else if (a.startsWith("--decompile=")) {
                DecompInterface di = new DecompInterface();
                di.openProgram(currentProgram);
                StringBuilder sb = new StringBuilder();
                for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
                    DecompileResults r = di.decompileFunction(f, 60, monitor);
                    sb.append("=== ").append(f.getEntryPoint()).append(' ').append(f.getName(true)).append('\n');
                    sb.append("sig: ").append(f.getSignature().getPrototypeString(true)).append('\n');
                    sb.append(r.decompileCompleted() ? r.getDecompiledFunction().getC() : "FAILED " + r.getErrorMessage());
                    sb.append('\n');
                }
                di.dispose();
                Files.writeString(new File(a.substring(12)).toPath(), sb.toString(), StandardCharsets.UTF_8);
            }
        }
    }
}
