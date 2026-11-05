// civ5-re: apply build/ghidra/plan.json to the program in one transaction; read back to state.json.
// Args: [--repo=<repo>] [--dry-run] [--prune-all]. Repo: --repo=, env CIV5RE_REPO, folder with civ5re.cfg
// above this script, GUI askDirectory.
//@category civ5re
//@menupath Tools.civ5re.Apply
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonNull;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.app.script.GhidraScript;
import ghidra.app.util.NamespaceUtils;
import ghidra.framework.options.Options;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.data.*;
import ghidra.program.model.data.Enum;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.pcode.Varnode;
import ghidra.program.model.symbol.*;
import ghidra.program.model.util.StringPropertyMap;
import ghidra.program.model.util.PropertyMapManager;

import java.io.File;
import java.io.Reader;
import java.io.Writer;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;
import java.security.MessageDigest;
import java.time.Instant;
import java.util.*;

public class Civ5reApply extends GhidraScript {

    static final int SCRIPT_VERSION = 2;
    static final int MAX_BODY_STEPS = 64;
    static final String ROOT = "/civ5re";
    static final String MAP = "civ5re";
    static final SourceType SRC = SourceType.IMPORTED;

    static class Abort extends RuntimeException {
        Abort(String m) {
            super(m);
        }
    }

    private final Gson gson = new GsonBuilder().serializeNulls().disableHtmlEscaping().setPrettyPrinting().create();
    private DataTypeManager dtm;
    private SymbolTable st;
    private Listing listing;
    private StringPropertyMap own;
    private boolean dryRun;
    private boolean pruneAll;
    private Map<String, JsonObject> lazyById;
    private final Set<String> lazyActive = new HashSet<>();

    private final Map<Address, String> bodyStatus = new HashMap<>();
    private final Map<String, Integer> counters = new LinkedHashMap<>();
    private final JsonArray entries = new JsonArray();
    private final List<String> lines = new ArrayList<>();

    private static final Map<String, DataType> BUILTIN = new LinkedHashMap<>();
    static {
        BUILTIN.put("void", VoidDataType.dataType);
        BUILTIN.put("bool", BooleanDataType.dataType);
        BUILTIN.put("char", CharDataType.dataType);
        BUILTIN.put("schar", SignedCharDataType.dataType);
        BUILTIN.put("uchar", UnsignedCharDataType.dataType);
        BUILTIN.put("short", ShortDataType.dataType);
        BUILTIN.put("ushort", UnsignedShortDataType.dataType);
        BUILTIN.put("int", IntegerDataType.dataType);
        BUILTIN.put("uint", UnsignedIntegerDataType.dataType);
        BUILTIN.put("long", LongDataType.dataType);
        BUILTIN.put("ulong", UnsignedLongDataType.dataType);
        BUILTIN.put("longlong", LongLongDataType.dataType);
        BUILTIN.put("ulonglong", UnsignedLongLongDataType.dataType);
        BUILTIN.put("float", FloatDataType.dataType);
        BUILTIN.put("double", DoubleDataType.dataType);
        BUILTIN.put("longdouble", LongDoubleDataType.dataType);
        BUILTIN.put("wchar32", WideChar32DataType.dataType);
        BUILTIN.put("wchar16", WideChar16DataType.dataType);
        BUILTIN.put("undefined", DataType.DEFAULT);
        BUILTIN.put("undefined1", Undefined1DataType.dataType);
        BUILTIN.put("undefined2", Undefined2DataType.dataType);
        BUILTIN.put("undefined4", Undefined4DataType.dataType);
        BUILTIN.put("undefined8", Undefined8DataType.dataType);
    }

    // ---------------------------------------------------------------- entry

    @Override
    public void run() throws Exception {
        String repoArg = null;
        String[] args = getScriptArgs();
        for (int i = 0; i < args.length; i++) {
            String a = args[i];
            if (a.startsWith("--repo=")) {
                repoArg = a.substring(7);
            } else if (a.equals("--repo") && i + 1 < args.length) {
                repoArg = args[++i]; // analyzeHeadless.bat splits "--repo=X" at '='
            } else if (a.equals("--dry-run")) {
                dryRun = true;
            } else if (a.equals("--prune-all")) {
                pruneAll = true;
            } else if (!a.isBlank()) {
                throw new IllegalArgumentException("civ5re: unknown argument " + a);
            }
        }
        File repo = findRepo(repoArg);
        File out = new File(repo, "build/ghidra");
        for (String c : new String[] { "changes", "conflicts", "manual_overwrites", "stale", "stale_limit",
            "types_created", "types_changed", "types_deleted", "functions_created", "functions_changed",
            "data_changed", "labels_changed", "bodies_repaired", "bodies_skipped" }) {
            counters.put(c, 0);
        }
        String planSha = null;
        String aborted = null;
        Throwable failure = null;
        JsonObject state = null;
        long t0 = System.currentTimeMillis();
        try {
            JsonObject plan;
            try (Reader r = Files.newBufferedReader(new File(out, "plan.json").toPath(), StandardCharsets.UTF_8)) {
                plan = JsonParser.parseReader(r).getAsJsonObject();
            }
            JsonObject h = plan.getAsJsonObject("header");
            planSha = str(h, "plan_sha256");
            checkFresh(repo, h);
            dtm = currentProgram.getDataTypeManager();
            st = currentProgram.getSymbolTable();
            listing = currentProgram.getListing();
            String plan8 = planSha == null ? "?" : planSha.substring(0, Math.min(8, planSha.length()));
            int tx = currentProgram.startTransaction("civ5re apply " + plan8);
            boolean commit = false;
            try {
                apply(plan, h);
                state = readBack(plan, planSha);
                commit = !dryRun;
            } finally {
                currentProgram.endTransaction(tx, commit);
            }
        } catch (Abort a) {
            aborted = a.getMessage();
        } catch (Throwable e) {
            aborted = "exception: " + e;
            failure = e;
        }
        if (aborted != null) {
            entries.add(entry("conflict", "plan", "-", aborted));
            lines.add("ABORTED: " + aborted);
        }
        if (state != null && aborted == null && !dryRun) {
            writeJson(new File(out, "state.json"), sorted(state));
        }
        writeReport(out, planSha, aborted, (System.currentTimeMillis() - t0) / 1000.0);
        if (failure != null) {
            if (failure instanceof Exception) {
                throw (Exception) failure;
            }
            throw new RuntimeException(failure);
        }
        if (aborted != null) {
            printerr("civ5re: apply aborted: " + aborted);
        }
    }

    private File findRepo(String arg) throws Exception {
        if (arg != null && !arg.isEmpty()) {
            return new File(arg);
        }
        String env = System.getenv("CIV5RE_REPO");
        if (env != null && !env.isEmpty()) {
            return new File(env);
        }
        if (sourceFile != null) {
            for (File d = sourceFile.getFile(false).getAbsoluteFile().getParentFile(); d != null; d = d.getParentFile()) {
                if (new File(d, "civ5re.cfg").isFile()) {
                    return d;
                }
            }
        }
        if (!isRunningHeadless()) {
            return askDirectory("civ5-re repository root", "Use");
        }
        throw new IllegalArgumentException("civ5re: repo not found: pass --repo= or set CIV5RE_REPO");
    }

    private void checkFresh(File repo, JsonObject h) throws Exception {
        String stale = "plan is stale: civ5re ghidra apply";
        if (h.get("script_version") == null || h.get("script_version").getAsInt() != SCRIPT_VERSION) {
            throw new Abort(stale + " (script_version " + h.get("script_version") + " != " + SCRIPT_VERSION + ")");
        }
        String[][] files = { { "typedb_sha256", "build/typedb.json" }, { "binfacts_sha256", "build/binfacts.json" } };
        for (String[] f : files) {
            File p = new File(repo, f[1]);
            String have = p.isFile() ? sha256(p) : null;
            if (have == null || !have.equalsIgnoreCase(String.valueOf(str(h, f[0])))) {
                throw new Abort(stale + " (" + f[1] + " changed)");
            }
        }
        String bin = str(h, "binary_sha256");
        if (bin != null && !bin.equalsIgnoreCase(currentProgram.getExecutableSHA256())) {
            throw new Abort("plan is for another binary: " + bin);
        }
    }

    // ---------------------------------------------------------------- apply

    private void apply(JsonObject plan, JsonObject h) throws Exception {
        PropertyMapManager pm = currentProgram.getUsrPropertyManager();
        own = pm.getStringPropertyMap(MAP);

        JsonArray types = arr(plan, "types");
        JsonArray functions = arr(plan, "functions");
        JsonArray data = arr(plan, "data");
        JsonArray labels = arr(plan, "labels");

        // Stale set first: the safeguard stops before any write.
        Set<String> planTypes = new HashSet<>();
        for (JsonElement e : types) {
            planTypes.add(str(e.getAsJsonObject(), "id"));
        }
        Map<Address, Set<String>> planLabels = new HashMap<>();
        Set<Address> planFns = new HashSet<>();
        Set<Address> planData = new HashSet<>();
        for (JsonElement e : functions) {
            planFns.add(addr(e.getAsJsonObject()));
        }
        for (JsonElement e : data) {
            JsonObject o = e.getAsJsonObject();
            planData.add(addr(o));
            planLabels.computeIfAbsent(addr(o), k -> new HashSet<>()).add(qname(arr(o, "scope"), str(o, "name")));
        }
        for (JsonElement e : labels) {
            JsonObject o = e.getAsJsonObject();
            planLabels.computeIfAbsent(addr(o), k -> new HashSet<>()).add(qname(arr(o, "scope"), str(o, "name")));
        }
        List<DataType> staleTypes = new ArrayList<>();
        int ours = 0;
        for (DataType dt : ourTypes()) {
            ours++;
            if (!planTypes.contains(dt.getPathName())) {
                staleTypes.add(dt);
            }
        }
        List<Address> ownAddrs = new ArrayList<>();
        int staleOwn = 0;
        if (own != null) {
            for (Address a = own.getFirstPropertyAddress(); a != null; a = own.getNextPropertyAddress(a)) {
                ownAddrs.add(a);
                JsonObject o = ownAt(a);
                ours++;
                if (o.has("fn") && !planFns.contains(a)) {
                    staleOwn++;
                }
                if (o.has("data") && !planData.contains(a)) {
                    staleOwn++;
                }
                for (JsonElement l : arrOr(o, "labels")) {
                    if (!planLabels.getOrDefault(a, Set.of()).contains(l.getAsString())) {
                        staleOwn++;
                    }
                }
            }
        }
        int staleCount = staleTypes.size() + staleOwn;
        int min = h.has("stale_min") ? h.get("stale_min").getAsInt() : 50;
        double pct = h.has("stale_pct") ? h.get("stale_pct").getAsDouble() : 2.0;
        int limit = (int) Math.max(min, Math.floor(ours * pct / 100.0));
        counters.put("stale", staleCount);
        counters.put("stale_limit", limit);
        if (staleCount > limit && !pruneAll) {
            throw new Abort("stale items " + staleCount + " > limit " + limit + " (use --prune-all)");
        }

        if (own == null) {
            own = pm.createStringPropertyMap(MAP);
        }
        CategoryPath root = new CategoryPath(ROOT);
        CategoryPath pref = currentProgram.getPreferredRootNamespaceCategoryPath();
        if (pref == null || !pref.equals(root)) {
            currentProgram.setPreferredRootNamespaceCategoryPath(ROOT);
            change("type", ROOT, "preferred root namespace category " + pref + " -> " + ROOT);
        }

        // 1. empty shells of all records
        Map<String, JsonObject> byId = new LinkedHashMap<>();
        for (JsonElement e : types) {
            JsonObject t = e.getAsJsonObject();
            byId.put(str(t, "id"), t);
        }
        for (JsonObject t : byId.values()) {
            String kind = str(t, "kind");
            if (!kind.equals("struct") && !kind.equals("union")) {
                continue;
            }
            DataType have = dtm.getDataType(str(t, "id"));
            boolean ok = kind.equals("struct") ? have instanceof Structure : have instanceof Union;
            if (have != null && !ok) {
                dtm.remove(have);
                have = null;
                change("type", str(t, "id"), "kind changed");
            }
            if (have == null) {
                CategoryPath cp = catOf(str(t, "id"));
                DataType shell = kind.equals("struct") ? new StructureDataType(cp, nameOf(str(t, "id")), 0, dtm)
                        : new UnionDataType(cp, nameOf(str(t, "id")), dtm);
                dtm.addDataType(shell, DataTypeConflictHandler.REPLACE_HANDLER);
                bump("types_created");
                change("type", str(t, "id"), "created");
            }
        }
        lazyById = byId;
        // 2. enums, typedefs, funcdefs (in plan order; typedef/funcdef chains resolve lazily)
        for (JsonObject t : byId.values()) {
            String kind = str(t, "kind");
            if (kind.equals("enum") || kind.equals("typedef") || kind.equals("funcdef")) {
                applySimpleType(t, byId);
            }
        }
        // 3. fill records
        for (JsonObject t : byId.values()) {
            String kind = str(t, "kind");
            if (kind.equals("struct") || kind.equals("union")) {
                fillComposite(t);
            }
        }
        // 4. scopes
        for (JsonElement e : arr(plan, "scopes")) {
            applyScope(e.getAsJsonObject());
        }
        // 4b. function bodies that Ghidra cut at `call next; pop`
        repairBodies(arrOr(plan, "bodies"), planFns);
        // 5. functions
        for (JsonElement e : functions) {
            applyFunction(e.getAsJsonObject());
        }
        // 6. data and labels
        for (JsonElement e : data) {
            applyData(e.getAsJsonObject());
        }
        for (JsonElement e : labels) {
            JsonObject o = e.getAsJsonObject();
            Address a = addr(o);
            if (!currentProgram.getMemory().contains(a)) {
                conflict("label", qname(arr(o, "scope"), str(o, "name")), "address " + hex(a) + " outside memory");
                continue;
            }
            ensureLabel(a, str(o, "name"), scope(arr(o, "scope")), false);
        }
        // 7. stale
        for (DataType dt : staleTypes) {
            String id = dt.getPathName();
            dtm.remove(dt);
            bump("types_deleted");
            change("type", id, "stale: deleted");
            stale("type", id, "deleted");
        }
        for (Address a : ownAddrs) {
            pruneOwn(a, planFns, planData, planLabels);
        }
        if (!dryRun) {
            Options o = currentProgram.getOptions(MAP);
            o.setString("plan_sha256", String.valueOf(str(h, "plan_sha256")));
            o.setInt("script_version", SCRIPT_VERSION);
            o.setString("time", Instant.now().toString());
            o.setString("counters", gson.toJson(counters));
        }
    }

    // ---------------------------------------------------------------- bodies

    // Ghidra ends a function after `call next` (get-PC idiom) and starts a bogus function at the next instruction.
    // plan.bodies lists symbols whose Ghidra body is shorter than the symbol: glue the pieces back together while
    // the piece boundary is exactly such a call and the bogus function is not an address the plan or the symbols
    // name. A repeated run finds the body complete and changes nothing.
    private void repairBodies(JsonArray bodies, Set<Address> planFns) throws Exception {
        FunctionManager fm = currentProgram.getFunctionManager();
        for (JsonElement el : bodies) {
            JsonObject b = el.getAsJsonObject();
            Address a = addr(b);
            Address last = a.add(b.get("size").getAsLong() - 1);
            AddressSet range = new AddressSet(a, last);
            Function f = fm.getFunctionAt(a);
            if (f == null) {
                skipBody(a, "no function");
                continue;
            }
            String why = null;
            int pieces = 0;
            while (pieces < MAX_BODY_STEPS) {
                ghidra.program.model.address.AddressRange r = f.getBody().getRangeContaining(a);
                Address e = r == null ? null : r.getMaxAddress().next();
                if (e == null || e.compareTo(last) > 0) {
                    break;
                }
                Instruction call = listing.getInstructionBefore(e);
                if (call == null || !call.getMnemonicString().equalsIgnoreCase("CALL")
                    || !e.equals(call.getMaxAddress().next()) || call.getFlows().length != 1
                    || !call.getFlows()[0].equals(e)) {
                    why = "the body ends at " + hex(e) + " but not after a call to the next instruction";
                    break;
                }
                Function g = fm.getFunctionAt(e);
                if (g == null) {
                    why = "no function at " + hex(e);
                    break;
                }
                if (planFns.contains(e) || g.getSymbol().getSource() != SourceType.DEFAULT
                    || st.getSymbols(e).length > 1) {
                    why = hex(e) + " is a named function";
                    break;
                }
                if (!range.contains(g.getBody())) {
                    why = "the function at " + hex(e) + " leaves the symbol range";
                    break;
                }
                AddressSet joined = new AddressSet(f.getBody());
                joined.add(g.getBody());
                change("function", hex(a), "body: joined " + g.getName() + " (" + g.getBody().getNumAddresses()
                    + " bytes) after a call to the next instruction");
                if (call.getFlowType().isCall()) {
                    call.setFlowOverride(FlowOverride.BRANCH); // the call is a get-PC, not a call
                }
                try {
                    if (f.isThunk()) {
                        f.setThunkedFunction(null); // `call next` alone looks like a thunk of the next function
                    }
                    fm.removeFunction(e);
                    f.setBody(joined);
                } catch (RuntimeException ex) {
                    throw new IllegalStateException("body repair at " + hex(a) + " piece " + hex(e) + ": " + ex, ex);
                }
                pieces++;
            }
            if (why != null) {
                skipBody(a, why);
            } else if (pieces == MAX_BODY_STEPS) {
                skipBody(a, "more than " + MAX_BODY_STEPS + " pieces");
            } else {
                bodyStatus.put(a, "ok");
                if (pieces > 0) {
                    bump("bodies_repaired");
                }
            }
        }
    }

    private void skipBody(Address a, String why) {
        bodyStatus.put(a, "skipped: " + why);
        bump("bodies_skipped");
        lines.add("skipped  body " + hex(a) + ": " + why);
    }

    private JsonArray bodiesState(JsonObject plan) {
        JsonArray out = new JsonArray();
        for (JsonElement el : arrOr(plan, "bodies")) {
            JsonObject b = el.getAsJsonObject();
            Address a = addr(b);
            Function f = getFunctionAt(a);
            ghidra.program.model.address.AddressRange r = f == null ? null : f.getBody().getRangeContaining(a);
            JsonObject o = new JsonObject();
            o.addProperty("addr", hex(a));
            o.addProperty("size", b.get("size").getAsLong());
            o.add("end", r == null ? JsonNull.INSTANCE : new com.google.gson.JsonPrimitive(hex(r.getMaxAddress().next())));
            o.addProperty("status", bodyStatus.getOrDefault(a, "skipped: not attempted"));
            out.add(o);
        }
        return out;
    }

    // ---------------------------------------------------------------- types

    private List<DataType> ourTypes() {
        List<DataType> outl = new ArrayList<>();
        Category c = dtm.getCategory(new CategoryPath(ROOT));
        if (c != null) {
            collect(c, outl);
        }
        outl.sort(Comparator.comparing(DataType::getPathName));
        return outl;
    }

    private void collect(Category c, List<DataType> outl) {
        for (DataType dt : c.getDataTypes()) {
            if (dt instanceof Composite || dt instanceof Enum || dt instanceof TypeDef ||
                dt instanceof FunctionDefinition) {
                outl.add(dt);
            }
        }
        for (Category sub : c.getCategories()) {
            collect(sub, outl);
        }
    }

    private static CategoryPath catOf(String id) {
        int i = id.lastIndexOf('/');
        return i <= 0 ? CategoryPath.ROOT : new CategoryPath(id.substring(0, i));
    }

    private static String nameOf(String id) {
        return id.substring(id.lastIndexOf('/') + 1);
    }

    private DataType resolve(JsonElement ref) {
        JsonObject o = ref.getAsJsonObject();
        if (o.has("b")) {
            DataType dt = BUILTIN.get(o.get("b").getAsString());
            if (dt == null) {
                throw new IllegalArgumentException("unknown builtin " + o);
            }
            return dt == DataType.DEFAULT ? dt : dtm.resolve(dt, null);
        }
        if (o.has("p")) {
            DataType to = resolve(o.get("p"));
            return dtm.getPointer(to == DataType.DEFAULT ? null : to, 4);
        }
        if (o.has("a")) {
            DataType el = resolve(o.get("a"));
            int n = o.get("n").getAsInt();
            return new ArrayDataType(el, n, el.getLength(), dtm);
        }
        if (o.has("t")) {
            String tid = o.get("t").getAsString();
            DataType dt = dtm.getDataType(tid);
            if (dt == null && lazyById != null && lazyById.containsKey(tid) && lazyActive.add(tid)) {
                try { // forward reference (also through pointers/arrays): create the typedef/funcdef/enum first
                    applySimpleType(lazyById.get(tid), lazyById);
                } catch (RuntimeException e) {
                    throw e;
                } catch (Exception e) {
                    throw new RuntimeException(e);
                } finally {
                    lazyActive.remove(tid);
                }
                dt = dtm.getDataType(tid);
            }
            if (dt == null) {
                throw new IllegalArgumentException("type not defined: " + o);
            }
            return dt;
        }
        throw new IllegalArgumentException("bad type ref " + o);
    }

    private JsonElement refOf(DataType dt) {
        JsonObject o = new JsonObject();
        if (dt == null || dt == DataType.DEFAULT || dt instanceof DefaultDataType) {
            o.addProperty("b", "undefined");
        } else if (dt instanceof Pointer) {
            o.add("p", refOf(((Pointer) dt).getDataType()));
        } else if (dt instanceof Array) {
            o.add("a", refOf(((Array) dt).getDataType()));
            o.addProperty("n", ((Array) dt).getNumElements());
        } else if (dt instanceof BuiltInDataType && BUILTIN.containsKey(dt.getName())) {
            o.addProperty("b", dt.getName());
        } else {
            o.addProperty("t", dt.getPathName());
        }
        return o;
    }

    private void applySimpleType(JsonObject t, Map<String, JsonObject> byId) throws Exception {
        String id = str(t, "id");
        DataType have = dtm.getDataType(id);
        JsonObject want = normType(t);
        if (have != null && typeState(have).equals(want)) {
            return;
        }
        CategoryPath cp = catOf(id);
        String name = nameOf(id);
        DataType nw;
        switch (str(t, "kind")) {
            case "enum": {
                EnumDataType en = new EnumDataType(cp, name, t.get("size").getAsInt(), dtm);
                for (JsonElement v : arr(t, "values")) {
                    JsonArray p = v.getAsJsonArray();
                    en.add(p.get(0).getAsString(), p.get(1).getAsLong());
                }
                nw = en;
                break;
            }
            case "typedef": {
                JsonObject to = t.getAsJsonObject("to");
                if (to.has("t") && dtm.getDataType(to.get("t").getAsString()) == null && byId.containsKey(to.get("t").getAsString())) {
                    applySimpleType(byId.get(to.get("t").getAsString()), byId);
                }
                nw = new TypedefDataType(cp, name, resolve(to), dtm);
                break;
            }
            default: {
                FunctionDefinitionDataType fd = new FunctionDefinitionDataType(cp, name, dtm);
                fd.setReturnType(resolveLazy(t.get("ret"), byId));
                List<ParameterDefinition> ps = new ArrayList<>();
                for (JsonElement pe : arr(t, "params")) {
                    JsonObject p = pe.getAsJsonObject();
                    ps.add(new ParameterDefinitionImpl(str(p, "name"), resolveLazy(p.get("type"), byId), null));
                }
                fd.setArguments(ps.toArray(new ParameterDefinition[0]));
                fd.setVarArgs(t.has("varargs") && t.get("varargs").getAsBoolean());
                fd.setCallingConvention(str(t, "cc") == null ? "__cdecl" : str(t, "cc"));
                nw = fd;
            }
        }
        if (have != null && !sameKind(have, nw)) {
            dtm.remove(have);
            have = null;
        }
        if (have == null) {
            dtm.addDataType(nw, DataTypeConflictHandler.REPLACE_HANDLER);
            bump("types_created");
            change("type", id, "created");
        } else {
            have.replaceWith(nw);
            bump("types_changed");
            change("type", id, "changed");
        }
    }

    // Funcdef/typedef referencing another not-yet-created typedef/funcdef.
    private DataType resolveLazy(JsonElement ref, Map<String, JsonObject> byId) throws Exception {
        JsonObject o = ref.getAsJsonObject();
        JsonObject cur = o;
        while (cur.has("p") || cur.has("a")) {
            cur = cur.getAsJsonObject(cur.has("p") ? "p" : "a");
        }
        if (cur.has("t")) {
            String id = cur.get("t").getAsString();
            if (dtm.getDataType(id) == null && byId.containsKey(id)) {
                applySimpleType(byId.get(id), byId);
            }
        }
        return resolve(o);
    }

    private static boolean sameKind(DataType a, DataType b) {
        return (a instanceof Enum && b instanceof Enum) || (a instanceof TypeDef && b instanceof TypeDef) ||
            (a instanceof FunctionDefinition && b instanceof FunctionDefinition);
    }

    private void fillComposite(JsonObject t) throws Exception {
        String id = str(t, "id");
        Composite have = (Composite) dtm.getDataType(id);
        JsonObject want = normType(t);
        if (typeState(have).equals(want)) {
            return;
        }
        Composite nw;
        if (str(t, "kind").equals("struct")) {
            StructureDataType s = new StructureDataType(catOf(id), nameOf(id), t.get("size").getAsInt(), dtm);
            s.setPackingEnabled(false);
            for (JsonElement fe : arr(t, "fields")) {
                JsonObject f = fe.getAsJsonObject();
                DataType ft = resolve(f.get("type"));
                int off = f.get("off").getAsInt();
                JsonElement bit = f.get("bit");
                if (bit != null && !bit.isJsonNull()) {
                    JsonObject b = bit.getAsJsonObject();
                    s.insertBitFieldAt(off, ft.getLength(), b.get("shift").getAsInt(), ft, b.get("size").getAsInt(),
                        str(f, "name"), str(f, "comment"));
                } else if (ft.getLength() > 0) {
                    s.replaceAtOffset(off, ft, ft.getLength(), str(f, "name"), str(f, "comment"));
                }
            }
            nw = s;
        } else {
            UnionDataType u = new UnionDataType(catOf(id), nameOf(id), dtm);
            for (JsonElement fe : arr(t, "fields")) {
                JsonObject f = fe.getAsJsonObject();
                DataType ft = resolve(f.get("type"));
                JsonElement bit = f.get("bit");
                if (bit != null && !bit.isJsonNull()) {
                    u.addBitField(ft, bit.getAsJsonObject().get("size").getAsInt(), str(f, "name"), str(f, "comment"));
                } else {
                    u.add(ft, ft.getLength(), str(f, "name"), str(f, "comment"));
                }
            }
            nw = u;
        }
        String desc = str(t, "desc");
        nw.setDescription(desc == null ? "" : desc);
        have.replaceWith(nw);
        have.setDescription(desc == null ? "" : desc);
        bump("types_changed");
        change("type", id, "layout written");
    }

    private JsonObject normType(JsonObject t) {
        JsonObject o = new JsonObject();
        String kind = str(t, "kind");
        o.addProperty("id", str(t, "id"));
        o.addProperty("kind", kind);
        switch (kind) {
            case "struct":
            case "union": {
                o.addProperty("size", t.get("size").getAsInt());
                o.add("desc", t.has("desc") ? t.get("desc") : JsonNull.INSTANCE);
                JsonArray fs = new JsonArray();
                for (JsonElement fe : arr(t, "fields")) {
                    JsonObject f = fe.getAsJsonObject();
                    JsonObject g = new JsonObject();
                    g.addProperty("off", f.get("off").getAsInt());
                    g.add("name", f.has("name") ? f.get("name") : JsonNull.INSTANCE);
                    g.add("type", f.get("type"));
                    g.add("comment", f.has("comment") ? f.get("comment") : JsonNull.INSTANCE);
                    g.add("bit", f.has("bit") ? f.get("bit") : JsonNull.INSTANCE);
                    fs.add(g);
                }
                o.add("fields", fs);
                break;
            }
            case "enum": {
                o.addProperty("size", t.get("size").getAsInt());
                List<JsonArray> vs = new ArrayList<>();
                for (JsonElement v : arr(t, "values")) {
                    vs.add(v.getAsJsonArray());
                }
                o.add("values", sortValues(vs));
                break;
            }
            case "typedef":
                o.add("to", t.get("to"));
                break;
            default: {
                o.addProperty("cc", str(t, "cc") == null ? "__cdecl" : str(t, "cc"));
                o.add("ret", t.get("ret"));
                JsonArray ps = new JsonArray();
                for (JsonElement pe : arr(t, "params")) {
                    JsonObject p = pe.getAsJsonObject();
                    JsonObject q = new JsonObject();
                    q.add("name", p.has("name") ? p.get("name") : JsonNull.INSTANCE);
                    q.add("type", p.get("type"));
                    ps.add(q);
                }
                o.add("params", ps);
                o.addProperty("varargs", t.has("varargs") && t.get("varargs").getAsBoolean());
            }
        }
        return o;
    }

    private static JsonArray sortValues(List<JsonArray> vs) {
        vs.sort(Comparator.<JsonArray>comparingLong(v -> v.get(1).getAsLong()).thenComparing(v -> v.get(0).getAsString()));
        JsonArray a = new JsonArray();
        vs.forEach(a::add);
        return a;
    }

    private JsonObject typeState(DataType dt) {
        JsonObject o = new JsonObject();
        o.addProperty("id", dt.getPathName());
        if (dt instanceof Composite) {
            Composite c = (Composite) dt;
            o.addProperty("kind", dt instanceof Union ? "union" : "struct");
            o.addProperty("size", c.isZeroLength() ? 0 : c.getLength());
            String d = c.getDescription();
            o.add("desc", d == null || d.isEmpty() ? JsonNull.INSTANCE : new com.google.gson.JsonPrimitive(d));
            JsonArray fs = new JsonArray();
            for (DataTypeComponent comp : c.getDefinedComponents()) {
                JsonObject g = new JsonObject();
                if (comp.isBitFieldComponent()) {
                    BitFieldDataType bf = (BitFieldDataType) comp.getDataType();
                    DataType base = bf.getBaseDataType();
                    int unit = Math.max(1, base.getLength());
                    int absBit = comp.getOffset() * 8 + bf.getBitOffset();
                    int off = (absBit / (unit * 8)) * unit;
                    g.addProperty("off", off);
                    g.add("name", jstr(comp.getFieldName()));
                    g.add("type", refOf(base));
                    g.add("comment", jstr(comp.getComment()));
                    JsonObject b = new JsonObject();
                    b.addProperty("size", bf.getBitSize());
                    b.addProperty("shift", absBit - off * 8);
                    g.add("bit", b);
                } else {
                    g.addProperty("off", comp.getOffset());
                    g.add("name", jstr(comp.getFieldName()));
                    g.add("type", refOf(comp.getDataType()));
                    g.add("comment", jstr(comp.getComment()));
                    g.add("bit", JsonNull.INSTANCE);
                }
                fs.add(g);
            }
            o.add("fields", fs);
        } else if (dt instanceof Enum) {
            Enum en = (Enum) dt;
            o.addProperty("kind", "enum");
            o.addProperty("size", en.getLength());
            List<JsonArray> vs = new ArrayList<>();
            for (String n : en.getNames()) {
                JsonArray p = new JsonArray();
                p.add(n);
                p.add(en.getValue(n));
                vs.add(p);
            }
            o.add("values", sortValues(vs));
        } else if (dt instanceof TypeDef) {
            o.addProperty("kind", "typedef");
            o.add("to", refOf(((TypeDef) dt).getDataType()));
        } else if (dt instanceof FunctionDefinition) {
            FunctionDefinition fd = (FunctionDefinition) dt;
            o.addProperty("kind", "funcdef");
            o.addProperty("cc", fd.getCallingConventionName());
            o.add("ret", refOf(fd.getReturnType()));
            JsonArray ps = new JsonArray();
            for (ParameterDefinition p : fd.getArguments()) {
                JsonObject q = new JsonObject();
                q.add("name", jstr(p.getName()));
                q.add("type", refOf(p.getDataType()));
                ps.add(q);
            }
            o.add("params", ps);
            o.addProperty("varargs", fd.hasVarArgs());
        } else {
            o.addProperty("kind", "other");
        }
        return o;
    }

    // ---------------------------------------------------------------- scopes

    private Namespace child(Namespace parent, String name) {
        for (Symbol s : st.getSymbols(name, parent)) {
            SymbolType ty = s.getSymbolType();
            if (ty == SymbolType.NAMESPACE || ty == SymbolType.CLASS) {
                return (Namespace) s.getObject();
            }
        }
        return null;
    }

    private void applyScope(JsonObject sc) throws Exception {
        JsonArray path = arr(sc, "path");
        Namespace parent = currentProgram.getGlobalNamespace();
        for (int i = 0; i < path.size(); i++) {
            String part = path.get(i).getAsString();
            boolean last = i == path.size() - 1;
            boolean wantClass = last && "class".equals(str(sc, "kind"));
            Namespace ns = child(parent, part);
            String id = qname(path, null);
            if (ns == null) {
                ns = wantClass ? st.createClass(parent, part, SRC) : st.createNameSpace(parent, part, SRC);
                change("scope", id, "created " + (wantClass ? "class" : "namespace") + " " + ns.getName(true));
            } else if (wantClass && !(ns instanceof GhidraClass)) {
                ns = NamespaceUtils.convertNamespaceToClass(ns);
                change("scope", id, "namespace -> class");
            } else if (last && !wantClass && ns instanceof GhidraClass) {
                conflict("scope", id, "is a class in Ghidra, plan says namespace");
            }
            parent = ns;
        }
    }

    private Namespace scope(JsonArray path) throws Exception {
        Namespace ns = currentProgram.getGlobalNamespace();
        for (JsonElement e : path) {
            Namespace c = child(ns, e.getAsString());
            if (c == null) {
                c = st.createNameSpace(ns, e.getAsString(), SRC);
                change("scope", c.getName(true), "created namespace (missing from scopes[])");
            }
            ns = c;
        }
        return ns;
    }

    private static List<String> scopePath(Namespace ns) {
        List<String> p = new ArrayList<>();
        for (Namespace n = ns; n != null && !n.isGlobal(); n = n.getParentNamespace()) {
            p.add(0, n.getName());
        }
        return p;
    }

    // ---------------------------------------------------------------- functions

    private void applyFunction(JsonObject pf) throws Exception {
        Address a = addr(pf);
        String id = str(pf, "addr") + " " + qname(arr(pf, "scope"), str(pf, "name"));
        MemoryBlock blk = currentProgram.getMemory().getBlock(a);
        if (blk == null) {
            conflict("function", id, "address outside memory");
            return;
        }
        if (!blk.isExecute()) {
            conflict("function", id, "address not executable (" + blk.getName() + ")");
            return;
        }
        Function f = getFunctionAt(a);
        if (f == null) {
            Function outer = getFunctionContaining(a);
            if (outer != null) {
                conflict("function", id, "inside the body of " + outer.getName(true) + " @ " + outer.getEntryPoint());
                return;
            }
            Data d = listing.getDefinedDataContaining(a);
            if (d != null) {
                conflict("function", id, "address is data " + d.getDataType().getName());
                return;
            }
            Instruction ins = listing.getInstructionContaining(a);
            if (ins != null && !ins.getAddress().equals(a)) {
                conflict("function", id, "not an instruction start");
                return;
            }
            if (ins == null) {
                new DisassembleCommand(a, null, true).applyTo(currentProgram, monitor);
            }
            CreateFunctionCmd cmd = new CreateFunctionCmd(a);
            if (!cmd.applyTo(currentProgram, monitor) || (f = getFunctionAt(a)) == null) {
                conflict("function", id, "createFunction failed: " + cmd.getStatusMsg());
                return;
            }
            bump("functions_created");
            change("function", id, "created");
        }
        JsonObject rec = ownAt(a);
        boolean firstTime = !rec.has("fn");
        Symbol sym = f.getSymbol();
        if (firstTime) {
            rec.addProperty("fn", 1);
            rec.addProperty("prev", sym.getName());
            JsonArray pns = new JsonArray();
            scopePath(sym.getParentNamespace()).forEach(pns::add);
            rec.add("prev_ns", pns);
            rec.addProperty("prev_src", sym.getSource().name());
        }
        boolean changed = false;
        // name and scope
        Namespace ns = scope(arr(pf, "scope"));
        String name = str(pf, "name");
        if (!sym.getName().equals(name) || !sym.getParentNamespace().equals(ns)) {
            manualCheck(sym.getSource(), "function", id, "name " + sym.getName(true));
            String old = sym.getName(true);
            sym.setNameAndNamespace(name, ns, SRC);
            change("function", id, "name " + old + " -> " + sym.getName(true));
            changed = true;
        } else if (sym.getSource() != SRC) {
            manualCheck(sym.getSource(), "function", id, "name source");
            change("function", id, "name source " + sym.getSource() + " -> " + SRC);
            sym.setSource(SRC);
            changed = true;
        }
        // signature
        if (pf.has("partial") && pf.get("partial").getAsBoolean()) {
            changed |= applyPartial(f, pf, id);
        } else {
            changed |= applySignature(f, pf, id);
        }
        if (changed) {
            bump("functions_changed");
        }
        putOwn(a, rec);
    }

    private boolean applyPartial(Function f, JsonObject pf, String id) throws Exception {
        boolean changed = false;
        String cc = str(pf, "cc");
        boolean plainWanted = !(pf.has("custom") && pf.get("custom").getAsBoolean());
        // Param ID leaves custom storage (and no typed `this`) on stubs that never use `this`: reset it to the plain convention.
        boolean customLeft = plainWanted && f.hasCustomVariableStorage();
        if (cc != null && !cc.isEmpty() && (!cc.equals(f.getCallingConventionName()) || customLeft)) {
            if (f.hasCustomVariableStorage()) {
                f.setCustomVariableStorage(false);
            }
            if (pf.has("drop_this") && pf.get("drop_this").getAsBoolean()) {
                dropThis(f, id);
            }
            change("function", id, "cc " + f.getCallingConventionName() + (customLeft ? " (custom storage)" : "") + " -> " + cc);
            f.setCallingConvention(cc);
            changed = true;
        } else if (pf.has("drop_this") && pf.get("drop_this").getAsBoolean()) {
            changed |= dropThis(f, id);
        }
        DataType ret = resolve(pf.get("ret"));
        boolean same = refOf(f.getReturn().getFormalDataType()).equals(pf.get("ret"));
        // Ghidra keeps source DEFAULT for an `undefined` return, so its source never settles.
        boolean unknownRet = same && ret == DataType.DEFAULT;
        if (!same || (!unknownRet && f.getReturn().getSource() != SRC)) {
            change("function", id, "return " + f.getReturn().getFormalDataType().getName() + " -> " + ret.getName());
            f.setReturnType(ret, SRC);
            changed = true;
        }
        return changed;
    }

    // Param ID's first parameter of a method is `this`: remove the explicit one sitting where the automatic one goes.
    private boolean dropThis(Function f, String id) {
        for (Parameter p : f.getParameters()) {
            if (p.isAutoParameter()) {
                continue;
            }
            VariableStorage vs = p.getVariableStorage();
            if (vs.isStackStorage() && vs.getStackOffset() == 4) {
                change("function", id, "removed Param ID this parameter " + p.getName());
                f.removeParameter(p.getOrdinal());
                return true;
            }
            return false;
        }
        return false;
    }

    private boolean applySignature(Function f, JsonObject pf, String id) throws Exception {
        JsonObject want = wantSig(pf);
        JsonObject have = sigState(f);
        if (have.equals(want)) {
            return false;
        }
        manualCheck(f.getSignatureSource(), "function", id, "signature " + f.getPrototypeString(true, false));
        StringBuilder diff = new StringBuilder();
        for (String k : want.keySet()) {
            if (!want.get(k).equals(have.get(k))) {
                diff.append(diff.length() == 0 ? "" : "; ").append(k).append(' ').append(gson.toJson(have.get(k)).replaceAll("\\s+", ""));
            }
        }
        boolean custom = pf.has("custom") && pf.get("custom").getAsBoolean();
        String cc = str(pf, "cc");
        DataType retType = resolve(pf.get("ret"));
        ReturnParameterImpl ret;
        if (custom) {
            // An undefined return keeps unassigned storage: Param ID would shrink a register to 1 byte (EAX -> AL).
            VariableStorage rs = retType == DataType.DEFAULT ? null : storage(pf.get("ret_storage"), retType);
            ret = new ReturnParameterImpl(retType, rs == null ? VariableStorage.UNASSIGNED_STORAGE : rs, true, currentProgram);
        } else {
            ret = new ReturnParameterImpl(retType, currentProgram);
        }
        List<Variable> params = new ArrayList<>();
        for (JsonElement pe : arr(pf, "params")) {
            JsonObject p = pe.getAsJsonObject();
            DataType dt = resolve(p.get("type"));
            if (custom) {
                VariableStorage vs = storage(p.get("storage"), dt);
                params.add(new ParameterImpl(str(p, "name"), dt, vs == null ? VariableStorage.UNASSIGNED_STORAGE : vs,
                    currentProgram, SRC));
            } else {
                params.add(new ParameterImpl(str(p, "name"), dt, currentProgram, SRC));
            }
        }
        f.updateFunction(cc, ret, params,
            custom ? Function.FunctionUpdateType.CUSTOM_STORAGE : Function.FunctionUpdateType.DYNAMIC_STORAGE_FORMAL_PARAMS,
            true, SRC);
        boolean va = pf.has("varargs") && pf.get("varargs").getAsBoolean();
        if (f.hasVarArgs() != va) {
            f.setVarArgs(va);
        }
        if (f.getSignatureSource() != SRC) {
            f.setSignatureSource(SRC);
        }
        JsonObject now = sigState(f);
        change("function", id, "signature -> " + f.getPrototypeString(true, false) + " (was: " + diff + ")");
        if (!now.equals(want)) {
            conflict("function", id, "signature did not take: want " + gson.toJson(want) + " have " + gson.toJson(now));
        }
        return true;
    }

    private JsonObject wantSig(JsonObject pf) {
        boolean custom = pf.has("custom") && pf.get("custom").getAsBoolean();
        JsonObject o = new JsonObject();
        o.addProperty("cc", str(pf, "cc"));
        o.addProperty("custom", custom);
        o.add("ret", pf.get("ret"));
        boolean undefRet = pf.get("ret").getAsJsonObject().has("b") && "undefined".equals(pf.get("ret").getAsJsonObject().get("b").getAsString());
        o.add("ret_storage", custom && !undefRet && pf.has("ret_storage") ? pf.get("ret_storage") : JsonNull.INSTANCE);
        JsonArray ps = new JsonArray();
        for (JsonElement pe : arr(pf, "params")) {
            JsonObject p = pe.getAsJsonObject();
            JsonObject q = new JsonObject();
            q.add("name", p.get("name"));
            q.add("type", p.get("type"));
            q.add("storage", custom && p.has("storage") ? p.get("storage") : JsonNull.INSTANCE);
            ps.add(q);
        }
        o.add("params", ps);
        o.addProperty("varargs", pf.has("varargs") && pf.get("varargs").getAsBoolean());
        o.addProperty("sig_source", SRC.name());
        return o;
    }

    private JsonObject sigState(Function f) {
        boolean custom = f.hasCustomVariableStorage();
        JsonObject o = new JsonObject();
        o.addProperty("cc", f.getCallingConventionName());
        o.addProperty("custom", custom);
        o.add("ret", refOf(f.getReturn().getFormalDataType()));
        o.add("ret_storage", custom ? storageRef(f.getReturn().getVariableStorage()) : JsonNull.INSTANCE);
        JsonArray ps = new JsonArray();
        for (Parameter p : f.getParameters()) {
            if (p.isAutoParameter()) {
                continue;
            }
            JsonObject q = new JsonObject();
            q.addProperty("name", p.getName());
            q.add("type", refOf(p.getFormalDataType()));
            q.add("storage", custom ? storageRef(p.getVariableStorage()) : JsonNull.INSTANCE);
            ps.add(q);
        }
        o.add("params", ps);
        o.addProperty("varargs", f.hasVarArgs());
        o.addProperty("sig_source", f.getSignatureSource().name());
        return o;
    }

    private VariableStorage storage(JsonElement e, DataType dt) throws Exception {
        if (e == null || e.isJsonNull()) {
            return null;
        }
        JsonObject o = e.getAsJsonObject();
        if (o.has("none")) {
            return VariableStorage.VOID_STORAGE;
        }
        if (o.has("stack")) {
            return new VariableStorage(currentProgram, o.get("stack").getAsInt(), Math.max(1, dt.getLength()));
        }
        String[] names = o.get("reg").getAsString().split(":");
        Register[] regs = new Register[names.length];
        for (int i = 0; i < names.length; i++) {
            regs[i] = currentProgram.getRegister(names[i]);
            if (regs[i] == null) {
                throw new IllegalArgumentException("unknown register " + names[i]);
            }
        }
        return new VariableStorage(currentProgram, regs);
    }

    private JsonElement storageRef(VariableStorage vs) {
        if (vs == null || vs.isUnassignedStorage() || vs.isBadStorage()) {
            return JsonNull.INSTANCE;
        }
        JsonObject o = new JsonObject();
        if (vs.isVoidStorage()) {
            o.addProperty("none", true);
        } else if (vs.isStackStorage()) {
            o.addProperty("stack", vs.getStackOffset());
        } else if (vs.isRegisterStorage() || vs.getVarnodeCount() > 1) {
            StringBuilder sb = new StringBuilder();
            for (Varnode v : vs.getVarnodes()) {
                Register r = currentProgram.getRegister(v);
                sb.append(sb.length() == 0 ? "" : ":").append(r != null ? r.getName() : v.toString());
            }
            o.addProperty("reg", sb.toString());
        } else {
            o.addProperty("other", vs.toString());
        }
        return o;
    }

    // ---------------------------------------------------------------- data and labels

    private void applyData(JsonObject pd) throws Exception {
        Address a = addr(pd);
        String id = str(pd, "addr") + " " + qname(arr(pd, "scope"), str(pd, "name"));
        int size = pd.get("size").getAsInt();
        if (!currentProgram.getMemory().contains(a) || !currentProgram.getMemory().contains(a.add(Math.max(0, size - 1)))) {
            conflict("data", id, "range outside memory");
            return;
        }
        AddressSet range = new AddressSet(a, a.add(Math.max(0, size - 1)));
        if (listing.getInstructions(range, true).hasNext()) {
            conflict("data", id, "range overlaps instructions");
            return;
        }
        ensureLabel(a, str(pd, "name"), scope(arr(pd, "scope")), true);
        JsonObject rec = ownAt(a);
        Data d = listing.getDefinedDataAt(a);
        if (d == null || d.getLength() != size || !refOf(d.getDataType()).equals(pd.get("type"))) {
            DataType dt = resolve(pd.get("type"));
            listing.clearCodeUnits(a, range.getMaxAddress(), false);
            listing.createData(a, dt, size);
            bump("data_changed");
            change("data", id, "data " + (d == null ? "none" : d.getDataType().getName()) + " -> " + dt.getName());
        }
        rec.addProperty("data", 1);
        putOwn(a, rec);
    }

    private void ensureLabel(Address a, String name, Namespace ns, boolean primary) throws Exception {
        String q = qname(scopePath(ns), name);
        Symbol s = st.getSymbol(name, a, ns);
        boolean changed = false;
        boolean created = false;
        if (s == null) {
            s = st.createLabel(a, name, ns, SRC);
            change("label", hex(a) + " " + q, "created");
            changed = created = true;
        } else if (s.getSource() != SRC) {
            manualCheck(s.getSource(), "label", hex(a) + " " + q, "label source");
            s.setSource(SRC);
            change("label", hex(a) + " " + q, "source -> " + SRC);
            changed = true;
        }
        if (primary && !s.isPrimary() && getFunctionAt(a) == null) {
            s.setPrimary();
            change("label", hex(a) + " " + q, "made primary");
            changed = true;
        }
        if (changed) {
            bump("labels_changed");
        }
        JsonObject rec = ownAt(a);
        JsonArray ls = arrOr(rec, "labels");
        boolean known = false;
        for (JsonElement e : ls) {
            known |= e.getAsString().equals(q);
        }
        if (!known && created) { // a pre-existing symbol (e.g. from .dynsym) is adopted, never deleted as stale
            ls.add(q);
            rec.add("labels", ls);
            putOwn(a, rec);
        }
    }

    private void pruneOwn(Address a, Set<Address> planFns, Set<Address> planData, Map<Address, Set<String>> planLabels)
            throws Exception {
        JsonObject rec = ownAt(a);
        Set<String> keepLabels = planLabels.getOrDefault(a, Set.of());
        JsonArray ls = arrOr(rec, "labels");
        JsonArray keep = new JsonArray();
        for (JsonElement e : ls) {
            String q = e.getAsString();
            if (keepLabels.contains(q)) {
                keep.add(q);
                continue;
            }
            int i = q.lastIndexOf("::");
            String name = i < 0 ? q : q.substring(i + 2);
            Namespace ns = i < 0 ? currentProgram.getGlobalNamespace() : findScope(q.substring(0, i));
            Symbol s = ns == null ? null : st.getSymbol(name, a, ns);
            if (s != null && s.getSymbolType() == SymbolType.LABEL) {
                s.delete();
                bump("labels_changed");
                change("label", hex(a) + " " + q, "stale: deleted");
            }
            stale("label", hex(a) + " " + q, "deleted");
        }
        if (keep.size() > 0) {
            rec.add("labels", keep);
        } else {
            rec.remove("labels");
        }
        if (rec.has("data") && !planData.contains(a)) {
            Data d = listing.getDefinedDataAt(a);
            if (d != null) {
                listing.clearCodeUnits(a, d.getMaxAddress(), false);
                bump("data_changed");
                change("data", hex(a), "stale: cleared " + d.getDataType().getName());
            }
            stale("data", hex(a), "cleared");
            rec.remove("data");
        }
        if (rec.has("fn") && !planFns.contains(a)) {
            Function f = getFunctionAt(a);
            if (f != null) {
                SourceType ps = SourceType.valueOf(str(rec, "prev_src"));
                Namespace pns = findScope(String.join("::", strings(arrOr(rec, "prev_ns"))));
                if (pns == null) {
                    pns = currentProgram.getGlobalNamespace();
                }
                f.getSymbol().setNameAndNamespace(ps == SourceType.DEFAULT ? "" : str(rec, "prev"), pns, ps);
                if (f.getSignatureSource() != SourceType.DEFAULT) {
                    f.setSignatureSource(SourceType.DEFAULT);
                }
                bump("functions_changed");
                change("function", hex(a), "stale: name restored to " + f.getName(true) + ", signature source DEFAULT");
            }
            stale("function", hex(a), "restored");
            rec.remove("fn");
            rec.remove("prev");
            rec.remove("prev_ns");
            rec.remove("prev_src");
        }
        putOwn(a, rec);
    }

    private Namespace findScope(String q) {
        Namespace ns = currentProgram.getGlobalNamespace();
        if (q.isEmpty()) {
            return ns;
        }
        for (String part : q.split("::")) {
            ns = child(ns, part);
            if (ns == null) {
                return null;
            }
        }
        return ns;
    }

    // ---------------------------------------------------------------- ownership map

    private JsonObject ownAt(Address a) {
        String v = own == null ? null : own.getString(a);
        return v == null ? new JsonObject() : JsonParser.parseString(v).getAsJsonObject();
    }

    private void putOwn(Address a, JsonObject rec) {
        String v = rec.size() == 0 ? null : new Gson().toJson(sorted(rec));
        String old = own.getString(a);
        if (Objects.equals(v, old)) {
            return;
        }
        if (v == null) {
            own.remove(a);
        } else {
            own.add(a, v);
        }
    }

    // ---------------------------------------------------------------- read back

    private JsonObject readBack(JsonObject plan, String planSha) {
        JsonObject s = new JsonObject();
        s.addProperty("schema", "civ5re.state/1");
        s.addProperty("script_version", SCRIPT_VERSION);
        s.addProperty("plan_sha256", planSha);
        s.addProperty("binary_sha256", currentProgram.getExecutableSHA256());
        s.addProperty("time", Instant.now().toString());
        JsonArray types = new JsonArray();
        for (DataType dt : ourTypes()) {
            types.add(typeState(dt));
        }
        s.add("types", types);

        TreeSet<Address> fnAddrs = new TreeSet<>();
        TreeSet<Address> dataAddrs = new TreeSet<>();
        TreeMap<Address, TreeSet<String>> labelSet = new TreeMap<>();
        for (JsonElement e : arr(plan, "functions")) {
            fnAddrs.add(addr(e.getAsJsonObject()));
        }
        for (JsonElement e : arr(plan, "data")) {
            dataAddrs.add(addr(e.getAsJsonObject()));
        }
        for (JsonElement e : arr(plan, "labels")) {
            JsonObject o = e.getAsJsonObject();
            labelSet.computeIfAbsent(addr(o), k -> new TreeSet<>()).add(qname(arr(o, "scope"), str(o, "name")));
        }
        if (own != null) {
            for (Address a = own.getFirstPropertyAddress(); a != null; a = own.getNextPropertyAddress(a)) {
                JsonObject rec = ownAt(a);
                if (rec.has("fn")) {
                    fnAddrs.add(a);
                }
                if (rec.has("data")) {
                    dataAddrs.add(a);
                }
                for (JsonElement l : arrOr(rec, "labels")) {
                    if (!isPrimaryLabel(a, l.getAsString())) { // state.labels: non-primary symbols only
                        labelSet.computeIfAbsent(a, k -> new TreeSet<>()).add(l.getAsString());
                    }
                }
            }
        }
        JsonArray funcs = new JsonArray();
        for (Address a : fnAddrs) {
            funcs.add(funcState(a));
        }
        s.add("funcs", funcs);
        JsonArray data = new JsonArray();
        for (Address a : dataAddrs) {
            JsonObject o = new JsonObject();
            o.addProperty("addr", hex(a));
            Data d = listing.getDefinedDataAt(a);
            o.addProperty("exists", d != null);
            o.addProperty("owned", isOwned(a));
            Symbol p = st.getPrimarySymbol(a);
            o.add("name", p == null ? JsonNull.INSTANCE : new com.google.gson.JsonPrimitive(p.getName()));
            o.add("scope", jarr(p == null ? List.of() : scopePath(p.getParentNamespace())));
            o.add("type", d == null ? JsonNull.INSTANCE : refOf(d.getDataType()));
            o.addProperty("size", d == null ? 0 : d.getLength());
            data.add(o);
        }
        s.add("data", data);
        JsonArray labels = new JsonArray();
        for (Map.Entry<Address, TreeSet<String>> e : labelSet.entrySet()) {
            for (String q : e.getValue()) {
                int i = q.lastIndexOf("::");
                String name = i < 0 ? q : q.substring(i + 2);
                String sq = i < 0 ? "" : q.substring(0, i);
                Namespace ns = findScope(sq);
                JsonObject o = new JsonObject();
                o.addProperty("addr", hex(e.getKey()));
                o.addProperty("exists", ns != null && st.getSymbol(name, e.getKey(), ns) != null);
                o.addProperty("owned", isOwned(e.getKey()));
                o.addProperty("name", name);
                o.add("scope", jarr(sq.isEmpty() ? List.of() : Arrays.asList(sq.split("::"))));
                labels.add(o);
            }
        }
        s.add("labels", labels);
        s.add("bodies", bodiesState(plan));
        return s;
    }

    private boolean isPrimaryLabel(Address a, String q) {
        int i = q.lastIndexOf("::");
        Namespace ns = findScope(i < 0 ? "" : q.substring(0, i));
        Symbol s = ns == null ? null : st.getSymbol(i < 0 ? q : q.substring(i + 2), a, ns);
        return s != null && s.isPrimary();
    }

    private boolean isOwned(Address a) {
        return own != null && own.getString(a) != null;
    }

    private JsonObject funcState(Address a) {
        JsonObject o = new JsonObject();
        o.addProperty("addr", hex(a));
        Function f = getFunctionAt(a);
        o.addProperty("exists", f != null);
        o.addProperty("owned", isOwned(a) && ownAt(a).has("fn"));
        if (f == null) {
            return o;
        }
        Symbol sym = f.getSymbol();
        o.addProperty("name", sym.getName());
        List<String> sp = scopePath(sym.getParentNamespace());
        o.add("scope", jarr(sp));
        JsonArray kinds = new JsonArray();
        for (Namespace n = sym.getParentNamespace(); n != null && !n.isGlobal(); n = n.getParentNamespace()) {
            kinds.add(n instanceof GhidraClass ? "class" : "namespace");
        }
        JsonArray kr = new JsonArray();
        for (int i = kinds.size() - 1; i >= 0; i--) {
            kr.add(kinds.get(i));
        }
        o.add("scope_kinds", kr);
        JsonObject sig = sigState(f);
        for (String k : new String[] { "cc", "custom", "ret", "ret_storage", "params", "varargs", "sig_source" }) {
            o.add(k, sig.get(k));
        }
        JsonElement autoThis = JsonNull.INSTANCE;
        boolean retptr = false;
        for (Parameter p : f.getParameters()) {
            if (!p.isAutoParameter()) {
                continue;
            }
            if (p.getAutoParameterType() == AutoParameterType.THIS) {
                autoThis = refOf(p.getDataType());
            } else if (p.getAutoParameterType() == AutoParameterType.RETURN_STORAGE_PTR) {
                retptr = true;
            }
        }
        o.add("auto_this", autoThis);
        o.addProperty("auto_retptr", retptr);
        o.addProperty("name_source", sym.getSource().name());
        return o;
    }

    // ---------------------------------------------------------------- report

    private void bump(String c) {
        counters.merge(c, 1, Integer::sum);
    }

    private void change(String entity, String id, String msg) {
        bump("changes");
        entries.add(entry("change", entity, id, msg));
        lines.add("change   " + entity + " " + id + ": " + msg);
    }

    private void conflict(String entity, String id, String msg) {
        bump("conflicts");
        entries.add(entry("conflict", entity, id, msg));
        lines.add("conflict " + entity + " " + id + ": " + msg);
    }

    private void stale(String entity, String id, String msg) {
        entries.add(entry("stale", entity, id, msg));
        lines.add("stale    " + entity + " " + id + ": " + msg);
    }

    private void manualCheck(SourceType src, String entity, String id, String what) {
        if (src == SourceType.USER_DEFINED) {
            bump("manual_overwrites");
            entries.add(entry("manual", entity, id, "manual edit overwritten: " + what));
            lines.add("manual   " + entity + " " + id + ": manual edit overwritten: " + what);
        }
    }

    private static JsonObject entry(String kind, String entity, String id, String msg) {
        JsonObject o = new JsonObject();
        o.addProperty("kind", kind);
        o.addProperty("entity", entity);
        o.addProperty("id", id);
        o.addProperty("message", msg);
        return o;
    }

    private void writeReport(File out, String planSha, String aborted, double secs) throws Exception {
        JsonObject r = new JsonObject();
        r.addProperty("schema", "civ5re.apply-report/1");
        r.addProperty("script_version", SCRIPT_VERSION);
        r.addProperty("plan_sha256", planSha);
        r.addProperty("dry_run", dryRun);
        r.addProperty("aborted", aborted);
        r.addProperty("seconds", secs);
        JsonObject c = new JsonObject();
        counters.forEach(c::addProperty);
        r.add("counters", c);
        r.add("entries", entries);
        StringBuilder sb = new StringBuilder();
        sb.append("civ5re apply").append(dryRun ? " --dry-run" : "").append(" plan=").append(planSha)
                .append(String.format(" %.1fs", secs)).append('\n');
        counters.forEach((k, v) -> sb.append(k).append('=').append(v).append(' '));
        sb.append('\n');
        for (String l : lines) {
            sb.append(l).append('\n');
        }
        out.mkdirs();
        String base = dryRun ? "apply-report.dry" : "apply-report"; // rebuild runs both in one headless call
        writeText(new File(out, base + ".txt"), sb.toString());
        writeJson(new File(out, base + ".json"), sorted(r));
        println("civ5re: " + (aborted != null ? "ABORTED " + aborted + "; " : "") + "changes=" + counters.get("changes") +
            " conflicts=" + counters.get("conflicts") + " manual=" + counters.get("manual_overwrites") +
            " stale=" + counters.get("stale") + (dryRun ? " (dry run, rolled back)" : ""));
    }

    // ---------------------------------------------------------------- utils

    private static JsonElement sorted(JsonElement e) {
        if (e.isJsonObject()) {
            JsonObject o = new JsonObject();
            new TreeMap<>(e.getAsJsonObject().asMap()).forEach((k, v) -> o.add(k, sorted(v)));
            return o;
        }
        if (e.isJsonArray()) {
            JsonArray a = new JsonArray();
            e.getAsJsonArray().forEach(x -> a.add(sorted(x)));
            return a;
        }
        return e;
    }

    private void writeJson(File f, JsonElement e) throws Exception {
        writeText(f, gson.toJson(e) + "\n");
    }

    private static void writeText(File f, String s) throws Exception {
        f.getParentFile().mkdirs();
        File tmp = new File(f.getPath() + ".tmp");
        try (Writer w = Files.newBufferedWriter(tmp.toPath(), StandardCharsets.UTF_8)) {
            w.write(s);
        }
        Files.move(tmp.toPath(), f.toPath(), StandardCopyOption.REPLACE_EXISTING);
    }

    private static String sha256(File f) throws Exception {
        MessageDigest md = MessageDigest.getInstance("SHA-256");
        byte[] h = md.digest(Files.readAllBytes(f.toPath()));
        StringBuilder sb = new StringBuilder();
        for (byte b : h) {
            sb.append(String.format("%02x", b));
        }
        return sb.toString();
    }

    private Address addr(JsonObject o) {
        return toAddr(Long.decode(str(o, "addr")));
    }

    private static String hex(Address a) {
        return String.format("0x%08x", a.getOffset());
    }

    private static String str(JsonObject o, String k) {
        JsonElement e = o.get(k);
        return e == null || e.isJsonNull() ? null : e.getAsString();
    }

    private static JsonArray arr(JsonObject o, String k) {
        JsonElement e = o.get(k);
        return e == null || e.isJsonNull() ? new JsonArray() : e.getAsJsonArray();
    }

    private static JsonArray arrOr(JsonObject o, String k) {
        return arr(o, k).deepCopy();
    }

    private static List<String> strings(JsonArray a) {
        List<String> l = new ArrayList<>();
        a.forEach(e -> l.add(e.getAsString()));
        return l;
    }

    private static String qname(JsonArray scope, String name) {
        return qname(strings(scope), name);
    }

    private static String qname(List<String> scope, String name) {
        List<String> p = new ArrayList<>(scope);
        if (name != null) {
            p.add(name);
        }
        return String.join("::", p);
    }

    private static JsonArray jarr(List<String> l) {
        JsonArray a = new JsonArray();
        l.forEach(a::add);
        return a;
    }

    private static JsonElement jstr(String s) {
        return s == null ? JsonNull.INSTANCE : new com.google.gson.JsonPrimitive(s);
    }
}
