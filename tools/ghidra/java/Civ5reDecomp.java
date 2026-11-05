// civ5-re decompile dump (civ5re ghidra decomp): compact asm + decompiler C per function, read-only.
// Args: --in=<file with one 0x address per line> --out=<dir> --plan=<plan sha256 recorded in the dumps>.
// Writes <out>/<addr8>.txt and <out>/.run.tsv (addr, name, size, status).
//@category civ5re
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.disassemble.Disassembler;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.Symbol;

import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

public class Civ5reDecomp extends GhidraScript {

    static final int MAX_CALLERS = 15;
    static final int DECOMPILE_TIMEOUT = 60;
    static final int MAX_STRING = 60;
    static final int MAX_GLOBALS = 60;
    static final int MAX_DATA_BYTES = 64;
    static final String FORMAT = "6"; // keep equal to FORMAT in decomp.py

    private Listing listing;

    @Override
    public void run() throws Exception {
        String in = null, out = null, plan = "-";
        String[] args = getScriptArgs();
        for (int i = 0; i < args.length; i++) {
            String a = args[i];
            String next = i + 1 < args.length ? args[i + 1] : null;
            if (a.startsWith("--in=")) {
                in = a.substring(5);
            } else if (a.startsWith("--out=")) {
                out = a.substring(6);
            } else if (a.startsWith("--plan=")) {
                plan = a.substring(7);
            } else if (a.equals("--in") && next != null) { // analyzeHeadless.bat splits "--x=Y" at '='
                in = args[++i];
            } else if (a.equals("--out") && next != null) {
                out = args[++i];
            } else if (a.equals("--plan") && next != null) {
                plan = args[++i];
            }
        }
        if (in == null || out == null) {
            throw new IllegalArgumentException("civ5re: --in= and --out= are required");
        }
        File outDir = new File(out);
        outDir.mkdirs();
        listing = currentProgram.getListing();

        DecompInterface di = new DecompInterface();
        if (!di.openProgram(currentProgram)) {
            throw new IllegalStateException("civ5re: decompiler failed to open the program: " + di.getLastMessage());
        }
        StringBuilder run = new StringBuilder();
        int ok = 0, missing = 0;
        long t0 = System.currentTimeMillis();
        try {
            for (String line : Files.readAllLines(new File(in).toPath(), StandardCharsets.UTF_8)) {
                monitor.checkCancelled();
                line = line.trim();
                if (line.isEmpty() || line.startsWith("#")) {
                    continue;
                }
                String[] tok = line.split("\\s+"); // "0xADDR [0xSYMBOL_SIZE]"
                long off = Long.parseUnsignedLong(tok[0].startsWith("0x") ? tok[0].substring(2) : tok[0], 16);
                long symSize = tok.length > 1 ? Long.decode(tok[1]) : 0;
                Address entry = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(off);
                Function f = getFunctionAt(entry);
                String key = String.format("%08x", off);
                if (f == null) {
                    run.append(key).append("\t-\t0\tno function\n");
                    missing++;
                    continue;
                }
                String text = dump(f, di, plan, symSize);
                writeAtomic(new File(outDir, key + ".txt"), text);
                run.append(key).append('\t').append(f.getName(true).replace('\t', ' ')).append('\t')
                    .append(f.getBody().getNumAddresses()).append("\tok\n");
                ok++;
            }
        } finally {
            di.dispose();
        }
        writeAtomic(new File(outDir, ".run.tsv"), run.toString());
        println(String.format("civ5re: decomp %d functions, %d missing in %.1f s", ok, missing,
            (System.currentTimeMillis() - t0) / 1000.0));
    }

    private void writeAtomic(File dst, String text) throws Exception {
        File tmp = new File(dst.getPath() + ".tmp");
        Files.write(tmp.toPath(), text.getBytes(StandardCharsets.UTF_8));
        Files.move(tmp.toPath(), dst.toPath(), StandardCopyOption.REPLACE_EXISTING);
    }

    private String dump(Function f, DecompInterface di, String plan, long symSize) throws Exception {
        Address entry = f.getEntryPoint();
        StringBuilder sb = new StringBuilder();
        sb.append(String.format("# 0x%08x %s  size 0x%x  cc %s\n", entry.getOffset(), f.getName(true),
            f.getBody().getNumAddresses(), f.getCallingConventionName()));
        sb.append("# signature: ").append(f.getPrototypeString(false, false)).append('\n');
        sb.append("# plan_sha256: ").append(plan).append("  format ").append(FORMAT).append('\n');
        sb.append("# callers: ").append(callers(f)).append('\n');
        sb.append("# callees: ").append(callees(f)).append('\n');
        if (symSize > f.getBody().getNumAddresses()) {
            sb.append(String.format("# body: ghidra 0x%x, symbol 0x%x\n", f.getBody().getNumAddresses(), symSize));
        }
        sb.append("## asm\n");
        asm(f, sb, symSize);
        sb.append("## c\n");
        DecompileResults r = di.decompileFunction(f, DECOMPILE_TIMEOUT, monitor);
        if (r != null && r.decompileCompleted() && r.getDecompiledFunction() != null) {
            String c = r.getDecompiledFunction().getC();
            sb.append(c.endsWith("\n") ? c : c + "\n");
        } else {
            sb.append("// decompile failed: ").append(r == null ? "no result" : r.getErrorMessage()).append('\n');
        }
        dataSection(sb);
        return sb.toString();
    }

    private String name(Function f) {
        return String.format("0x%08x %s", f.getEntryPoint().getOffset(), f.getName(true));
    }

    private String callers(Function f) {
        TreeMap<Long, String> found = new TreeMap<>();
        ReferenceIterator it = currentProgram.getReferenceManager().getReferencesTo(f.getEntryPoint());
        while (it.hasNext()) {
            Reference ref = it.next();
            if (!ref.getReferenceType().isCall()) {
                continue;
            }
            Function c = getFunctionContaining(ref.getFromAddress());
            if (c != null && !c.equals(f)) {
                found.put(c.getEntryPoint().getOffset(), name(c));
            }
        }
        List<String> all = new ArrayList<>(found.values());
        String s = String.join(", ", all.subList(0, Math.min(MAX_CALLERS, all.size())));
        if (all.size() > MAX_CALLERS) {
            s += ", ... (" + all.size() + " total)";
        }
        return all.isEmpty() ? "-" : s;
    }

    private String callees(Function f) {
        TreeMap<Long, String> found = new TreeMap<>();
        for (Function c : f.getCalledFunctions(monitor)) {
            found.put(c.getEntryPoint().getOffset(), name(c));
        }
        // getCalledFunctions can miss calls of a function whose body was changed after analysis: read the references
        for (Instruction ins : listing.getInstructions(f.getBody(), true)) {
            for (Reference ref : ins.getReferencesFrom()) {
                Function c = ref.getReferenceType().isCall() ? getFunctionAt(ref.getToAddress()) : null;
                if (c != null && !c.equals(f)) {
                    found.put(c.getEntryPoint().getOffset(), name(c));
                }
            }
        }
        return found.isEmpty() ? "-" : String.join(", ", found.values());
    }

    // A global the function reads or writes: width 0 when only its address is taken.
    private static class Global {
        final long addr;
        final int width;
        final char kind; // f float, d double, i int, ' ' unknown

        Global(long addr, int width, char kind) {
            this.addr = addr;
            this.width = width;
            this.kind = kind;
        }
    }

    private final TreeMap<Long, Global> globals = new TreeMap<>();

    // The instructions of the symbol range that lie outside Ghidra's body (Ghidra split the function at a PIC
    // `call next; pop` or a non-returning call). Bytes Ghidra left undefined are disassembled inside a transaction
    // that is rolled back by the caller; the project is opened read-only and never saved.
    private List<Instruction> beyondBody(Function f, long symSize) throws Exception {
        List<Instruction> extra = new ArrayList<>();
        AddressSetView body = f.getBody();
        Address pc = f.getEntryPoint();
        Address end = pc.add(symSize - 1);
        Disassembler dis = Disassembler.getDisassembler(currentProgram, monitor, null);
        while (pc != null && pc.compareTo(end) <= 0) {
            ghidra.program.model.address.AddressRange r = body.getRangeContaining(pc);
            if (r != null) {
                pc = r.getMaxAddress().next();
                continue;
            }
            Instruction i = listing.getInstructionAt(pc);
            if (i == null) {
                dis.disassemble(pc, new AddressSet(pc, end), false);
                i = listing.getInstructionAt(pc);
            }
            if (i == null) {
                break;
            }
            extra.add(i);
            pc = i.getMaxAddress().next();
        }
        return extra;
    }

    private void asm(Function f, StringBuilder sb, long symSize) throws Exception {
        AddressSetView body = f.getBody();
        boolean split = symSize > body.getNumAddresses();
        int tx = split ? currentProgram.startTransaction("civ5re decomp: read the symbol range") : -1;
        try {
            asmRange(f, sb, split ? symSize : 0);
        } finally {
            if (split) {
                currentProgram.endTransaction(tx, false); // roll back: nothing is kept
            }
        }
    }

    private void asmRange(Function f, StringBuilder sb, long symSize) throws Exception {
        AddressSetView body = f.getBody();
        List<Instruction> insns = new ArrayList<>();
        for (Instruction ins : listing.getInstructions(body, true)) {
            insns.add(ins);
        }
        AddressSet universe = new AddressSet(body);
        if (symSize > 0) {
            List<Instruction> extra = beyondBody(f, symSize);
            for (Instruction ins : extra) {
                universe.add(ins.getMinAddress(), ins.getMaxAddress());
            }
            insns.addAll(extra);
            insns.sort((x, y) -> x.getAddress().compareTo(y.getAddress()));
        }
        Set<Address> targets = new TreeSet<>();
        for (Instruction ins : insns) {
            if (!ins.getFlowType().isJump()) {
                continue;
            }
            for (Reference ref : ins.getReferencesFrom()) {
                if (ref.getReferenceType().isFlow() && !ref.getReferenceType().isCall()
                    && universe.contains(ref.getToAddress())) {
                    targets.add(ref.getToAddress());
                }
            }
        }
        globals.clear();
        Map<String, Long> known = new HashMap<>(); // registers holding a PIC base
        Address pendingPop = null;
        boolean marked = false;
        for (Instruction ins : insns) {
            Address a = ins.getAddress();
            if (!marked && !body.contains(a)) {
                sb.append("# -- beyond the Ghidra body, inside the symbol size --\n");
                marked = true;
            }
            if (targets.contains(a)) {
                sb.append(String.format("L%08x:\n", a.getOffset()));
            }
            sb.append(String.format("%08x  ", a.getOffset())).append(ins.toString());
            String note = note(ins, known);
            if (!note.isEmpty()) {
                sb.append("  ; ").append(note);
            }
            sb.append('\n');
            pendingPop = track(ins, known, pendingPop);
        }
    }

    // call-next / pop reg / add reg, imm gives a register a known value (the PIC base).
    private Address track(Instruction ins, Map<String, Long> known, Address pendingPop) {
        String m = ins.getMnemonicString().toUpperCase();
        if (m.equals("CALL")) {
            Address next = ins.getFallThrough();
            for (Address fl : ins.getFlows()) {
                if (next != null && fl.equals(next)) {
                    return next;
                }
            }
            return null;
        }
        Register r = ins.getNumOperands() > 0 ? singleRegister(ins, 0) : null;
        if (r == null) {
            return null;
        }
        String rn = r.getName();
        if (m.equals("POP") && ins.getAddress().equals(pendingPop)) {
            known.put(rn, ins.getAddress().getOffset());
        } else if ((m.equals("ADD") || m.equals("SUB")) && known.containsKey(rn) && ins.getNumOperands() == 2) {
            Object[] b = ins.getOpObjects(1);
            if (b.length == 1 && b[0] instanceof Scalar) {
                long v = ((Scalar) b[0]).getValue();
                known.put(rn, (known.get(rn) + (m.equals("ADD") ? v : -v)) & 0xffffffffL);
            } else {
                known.remove(rn);
            }
        } else if (!m.equals("CMP") && !m.equals("TEST") && !m.startsWith("PUSH")) {
            known.remove(rn);
        }
        return null;
    }

    private Register singleRegister(Instruction ins, int op) {
        Object[] o = ins.getOpObjects(op);
        return o.length == 1 && o[0] instanceof Register ? (Register) o[0] : null;
    }

    // The memory operand's effective address when it can be resolved, -1 otherwise.
    private long effective(Instruction ins, int op, Map<String, Long> known) {
        long sum = 0;
        boolean any = false;
        for (Object o : ins.getOpObjects(op)) {
            if (o instanceof Register) {
                Long v = known.get(((Register) o).getName());
                if (v == null) {
                    return -1;
                }
                sum += v;
            } else if (o instanceof Scalar) {
                sum += ((Scalar) o).getValue();
                any = true;
            } else if (o instanceof Address) {
                sum += ((Address) o).getOffset();
                any = true;
            } else {
                return -1;
            }
        }
        return any ? sum & 0xffffffffL : -1;
    }

    private static int widthOf(String rep) {
        if (rep.contains("tword ptr") || rep.contains("xword ptr")) return 10;
        if (rep.contains("qword ptr")) return 8;
        if (rep.contains("dword ptr")) return 4;
        if (rep.contains("word ptr")) return 2;
        if (rep.contains("byte ptr")) return 1;
        return 0;
    }

    private static char kindOf(String m, int w) {
        if (m.startsWith("LEA")) return ' ';
        if (m.startsWith("F") && !m.startsWith("FI") && !m.equals("FXCH")) {
            return w == 4 ? 'f' : w == 8 ? 'd' : ' ';
        }
        if (m.endsWith("SS")) return w == 4 ? 'f' : ' ';
        if (m.endsWith("SD") || m.endsWith("PD")) return w == 8 ? 'd' : ' ';
        return w == 1 || w == 2 || w == 4 || w == 8 ? 'i' : ' ';
    }

    private byte[] bytesAt(Address at, int n) {
        try {
            MemoryBlock b = currentProgram.getMemory().getBlock(at);
            if (b == null || !b.isInitialized()) {
                return null;
            }
            byte[] buf = new byte[n];
            return currentProgram.getMemory().getBytes(at, buf) == n ? buf : null;
        } catch (Exception e) {
            return null;
        }
    }

    private static String hex(byte[] b) {
        StringBuilder s = new StringBuilder();
        for (byte x : b) {
            s.append(String.format("%02x", x & 0xff));
        }
        return s.toString();
    }

    private static String value(byte[] b, char kind) {
        long v = 0;
        for (int i = Math.min(b.length, 8) - 1; i >= 0; i--) {
            v = (v << 8) | (b[i] & 0xff);
        }
        switch (kind) {
            case 'f':
                return "f32 " + Float.intBitsToFloat((int) v);
            case 'd':
                return "f64 " + Double.longBitsToDouble(v);
            default:
                return b.length <= 8 ? "0x" + Long.toHexString(v) : hex(b);
        }
    }

    // Printable ASCII up to a NUL as an escaped, quoted string; null when the bytes are not text.
    private static String cstr(byte[] b) {
        StringBuilder s = new StringBuilder("\"");
        int i = 0;
        for (; i < b.length && b[i] != 0; i++) {
            int c = b[i] & 0xff;
            if (c == '\n') s.append("\\n");
            else if (c == '\t') s.append("\\t");
            else if (c == '\r') s.append("\\r");
            else if (c == '"' || c == '\\') s.append('\\').append((char) c);
            else if (c >= 0x20 && c < 0x7f) s.append((char) c);
            else s.append(String.format("\\x%02x", c));
        }
        if (i == 0 || (i == b.length && b.length < MAX_DATA_BYTES)) {
            return null; // empty, or no terminator inside the known size
        }
        int printable = 0;
        for (int j = 0; j < i; j++) {
            int c = b[j] & 0xff;
            if ((c >= 0x20 && c < 0x7f) || c == '\n' || c == '\t' || c == '\r') printable++;
        }
        if (printable < i) {
            return null;
        }
        return s.append(i == b.length ? "\"..." : "\"").toString();
    }

    // Bytes from `at` to the next symbol, defined data or reference anywhere in the program (max 64).
    private long runLimit(Address at) {
        long limit = MAX_DATA_BYTES;
        Address from = at.next();
        if (from == null) {
            return 1;
        }
        Symbol s = currentProgram.getSymbolTable().getSymbolIterator(from, true).hasNext()
            ? currentProgram.getSymbolTable().getSymbolIterator(from, true).next() : null;
        if (s != null && s.getAddress().isMemoryAddress()) {
            limit = Math.min(limit, s.getAddress().getOffset() - at.getOffset());
        }
        Data nd = listing.getDefinedDataAfter(at);
        if (nd != null) {
            limit = Math.min(limit, nd.getAddress().getOffset() - at.getOffset());
        }
        java.util.Iterator<Address> refs = currentProgram.getReferenceManager()
            .getReferenceDestinationIterator(from, true);
        if (refs.hasNext()) {
            limit = Math.min(limit, refs.next().getOffset() - at.getOffset());
        }
        return Math.max(1, limit);
    }

    // Length in bytes of the float/double table starting at g: its referenced elements are consecutive; the table
    // ends where the next group of references starts, else it runs on over untyped or float data (max 64 bytes).
    private int tableLength(Global g, long room) {
        int w = g.width;
        long last = g.addr;
        Long next = globals.higherKey(last);
        while (next != null && next - last == w && globals.get(next).width == w && next - g.addr < MAX_DATA_BYTES) {
            last = next;
            next = globals.higherKey(last);
        }
        long cap = Math.min(MAX_DATA_BYTES, room);
        if (next != null && next - g.addr <= cap) {
            return (int) Math.max(w, (next - g.addr) / w * w);
        }
        Address at = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(g.addr);
        long len = last - g.addr + w;
        while (len + w <= cap) {
            Address a = at.add(len);
            Symbol s = currentProgram.getSymbolTable().getPrimarySymbol(a);
            if (s != null && s.getSource() != SourceType.DEFAULT) {
                break;
            }
            Data dd = listing.getDataAt(a);
            if (dd != null && dd.isDefined()) {
                String t = dd.getDataType().getName();
                if (dd.getLength() != w || !(t.startsWith("float") || t.startsWith("double")
                    || t.startsWith("undefined"))) {
                    break;
                }
            }
            len += w;
        }
        return (int) len;
    }

    private static String array(byte[] b, char kind, int w) {
        int cnt = b.length / w;
        StringBuilder s = new StringBuilder(kind == 'f' ? "f32" : "f64");
        if (cnt > 1) {
            s.append('[').append(cnt).append(']');
        }
        for (int i = 0; i < cnt; i++) {
            long v = 0;
            for (int j = w - 1; j >= 0; j--) {
                v = (v << 8) | (b[i * w + j] & 0xff);
            }
            s.append(' ').append(kind == 'f' ? Float.toString(Float.intBitsToFloat((int) v))
                : Double.toString(Double.longBitsToDouble(v)));
        }
        return s.toString();
    }

    private String symbolName(Address to) {
        Symbol s = currentProgram.getSymbolTable().getPrimarySymbol(to);
        return s == null ? null : s.getName(true);
    }

    // Call targets; for data: address, name, typed value of initialized bytes, strings.
    private String note(Instruction ins, Map<String, Long> known) {
        Set<String> notes = new LinkedHashSet<>();
        String m = ins.getMnemonicString().toUpperCase();
        Set<Long> seen = new HashSet<>();
        int memOp = -1, width = 0;
        for (int i = 0; i < ins.getNumOperands(); i++) {
            String rep = ins.getDefaultOperandRepresentation(i);
            if (rep.contains("[")) {
                memOp = i;
                width = widthOf(rep);
                break;
            }
        }
        long eff = memOp >= 0 ? effective(ins, memOp, known) : -1;
        Address effAddr = null;
        if (eff >= 0) {
            effAddr = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(eff);
            if (!currentProgram.getMemory().contains(effAddr)) {
                effAddr = null;
            }
        }
        int w = memOp >= 0 ? width : 0;
        char k = memOp >= 0 ? kindOf(m, width) : ' ';
        for (Reference ref : ins.getReferencesFrom()) {
            RefType t = ref.getReferenceType();
            Address to = ref.getToAddress();
            if (!to.isMemoryAddress() || (t.isFlow() && !t.isCall())) {
                continue;
            }
            if (t.isCall()) {
                Function c = getFunctionAt(to);
                if (c != null) {
                    notes.add(c.getName(true));
                }
                continue;
            }
            if (effAddr != null && to.equals(effAddr)) {
                continue; // described below with its value
            }
            boolean viaMem = memOp >= 0 && ref.getOperandIndex() == memOp;
            data(to, notes, seen, viaMem ? w : 0, viaMem ? k : ' ', false);
        }
        if (effAddr != null) {
            data(effAddr, notes, seen, w, k, true);
        }
        return String.join(", ", notes);
    }

    private void data(Address to, Set<String> notes, Set<Long> seen, int width, char kind, boolean showAddr) {
        if (!seen.add(to.getOffset()) || getFunctionAt(to) != null || listing.getInstructionAt(to) != null) {
            return;
        }
        Data d = listing.getDataAt(to);
        String name = symbolName(to);
        Global g = globals.get(to.getOffset());
        if (g == null || (g.width == 0 && width > 0)) {
            globals.put(to.getOffset(), new Global(to.getOffset(), width, kind));
        }
        if (d != null && d.hasStringValue()) {
            String v = d.getDefaultValueRepresentation();
            notes.add(v.length() > MAX_STRING ? v.substring(0, MAX_STRING) + "..." : v);
            return;
        }
        StringBuilder s = new StringBuilder();
        if (showAddr) {
            s.append(String.format("0x%08x", to.getOffset()));
        }
        if (name != null && !(name.startsWith("DAT_") && showAddr)) {
            s.append(s.length() > 0 ? " " : "").append(name);
        }
        if (width > 0) {
            byte[] b = bytesAt(to, width);
            if (b != null && kind != ' ') {
                s.append(s.length() > 0 ? " " : "").append("= ").append(value(b, kind));
            } else if (b == null && currentProgram.getMemory().getBlock(to) != null) {
                s.append(s.length() > 0 ? " " : "").append("(bss)");
            }
        }
        if (s.length() > 0) {
            notes.add(s.toString());
        }
    }

    private void dataSection(StringBuilder sb) {
        sb.append("## data\n");
        if (globals.isEmpty()) {
            sb.append("-\n");
            return;
        }
        int n = 0;
        long skipUntil = 0;
        for (Global g : globals.values()) {
            if (g.addr < skipUntil) {
                continue; // an element of the table printed before
            }
            if (++n > MAX_GLOBALS) {
                sb.append("... ").append(globals.size() - MAX_GLOBALS).append(" more\n");
                break;
            }
            Address at = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(g.addr);
            Data d = listing.getDataAt(at);
            String name = symbolName(at);
            int size = d != null && d.isDefined() ? d.getLength() : g.width;
            sb.append(String.format("%08x  %s", g.addr, name == null ? "-" : name));
            sb.append(size > 0 ? "  size " + size : "  size ?");
            MemoryBlock blk = currentProgram.getMemory().getBlock(at);
            if (blk == null) {
                sb.append("  unmapped");
            } else if (!blk.isInitialized()) {
                sb.append("  bss");
            } else if (d != null && d.hasStringValue()) {
                String v = d.getDefaultValueRepresentation();
                sb.append("  ").append(v.length() > MAX_STRING ? v.substring(0, MAX_STRING) + "..." : v);
            } else {
                long room = blk.getEnd().getOffset() - g.addr + 1;
                boolean defined = d != null && d.isDefined();
                int take = (int) Math.min(MAX_DATA_BYTES, defined ? d.getLength() : Math.min(room, runLimit(at)));
                boolean fp = (g.kind == 'f' || g.kind == 'd') && g.width > 0;
                if (fp) {
                    take = tableLength(g, room);
                    skipUntil = g.addr + take;
                }
                take = (int) Math.min(take, room);
                byte[] b = bytesAt(at, take);
                if (b != null) {
                    if (take > size) {
                        sb.append("  run ").append(take);
                    }
                    sb.append("  bytes ").append(hex(b));
                    if (fp && b.length >= g.width) {
                        sb.append("  ").append(array(b, g.kind, g.width));
                    } else if (g.kind != ' ' && b.length == g.width) {
                        sb.append("  ").append(value(b, g.kind));
                    } else {
                        String s = cstr(b);
                        if (s != null) {
                            sb.append("  str ").append(s);
                        }
                    }
                }
            }
            sb.append('\n');
        }
    }
}
