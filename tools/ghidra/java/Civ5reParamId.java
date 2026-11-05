// civ5-re: Decompiler Parameter ID over all functions with the profile options (civ5re ghidra paramid).
//@category civ5re
import ghidra.app.cmd.function.DecompilerParameterIdCmd;
import ghidra.app.script.GhidraScript;
import ghidra.framework.options.Options;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Program;
import ghidra.program.model.symbol.SourceType;

public class Civ5reParamId extends GhidraScript {

    static final String ANALYZER = "Decompiler Parameter ID";

    @Override
    public void run() throws Exception {
        Options opts = currentProgram.getOptions(Program.ANALYSIS_PROPERTIES).getOptions(ANALYZER);
        SourceType clear = opts.getEnum("Analysis Clear Level", SourceType.ANALYSIS);
        boolean commitTypes = opts.getBoolean("Commit Data Types", true);
        boolean commitVoid = opts.getBoolean("Commit Void Return Values", false);
        int timeout = opts.getInt("Analysis Decompiler Timeout (sec)", 60);

        AddressSet entries = new AddressSet();
        int n = 0;
        for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
            entries.add(f.getEntryPoint());
            n++;
        }
        long t0 = System.currentTimeMillis();
        DecompilerParameterIdCmd cmd = new DecompilerParameterIdCmd("civ5re " + ANALYZER, entries, clear,
            commitTypes, commitVoid, timeout);
        if (!cmd.applyTo(currentProgram, monitor)) {
            throw new IllegalStateException("civ5re: Param ID failed: " + cmd.getStatusMsg());
        }
        println(String.format("civ5re: Param ID over %d functions (clear=%s commitTypes=%s commitVoid=%s timeout=%d) in %.1f s",
            n, clear, commitTypes, commitVoid, timeout, (System.currentTimeMillis() - t0) / 1000.0));
    }
}
