#pragma once
// The sqlite3 API statically linked into Civ5XP (binfacts group thirdparty, sqlite3_*), with the real
// sqlite3.h signatures and constants. Include it from the .cpp that calls sqlite3_*.

#ifdef __cplusplus
extern "C" {
#endif

typedef long long sqlite3_int64;
typedef unsigned long long sqlite3_uint64;
typedef sqlite3_int64 sqlite_int64;
typedef sqlite3_uint64 sqlite_uint64;

typedef struct sqlite3 sqlite3;
typedef struct sqlite3_stmt sqlite3_stmt;
typedef struct sqlite3_value sqlite3_value;
typedef struct sqlite3_context sqlite3_context;
typedef struct sqlite3_blob sqlite3_blob;
typedef struct sqlite3_backup sqlite3_backup;
typedef struct sqlite3_mutex sqlite3_mutex;
typedef struct sqlite3_vfs sqlite3_vfs;
typedef struct sqlite3_module sqlite3_module;
typedef struct sqlite3_file sqlite3_file;

extern const char sqlite3_version[];
extern char* sqlite3_temp_directory;
extern char* sqlite3_data_directory;

#define SQLITE_OK           0
#define SQLITE_ERROR        1
#define SQLITE_INTERNAL     2
#define SQLITE_PERM         3
#define SQLITE_ABORT        4
#define SQLITE_BUSY         5
#define SQLITE_LOCKED       6
#define SQLITE_NOMEM        7
#define SQLITE_READONLY     8
#define SQLITE_INTERRUPT    9
#define SQLITE_IOERR       10
#define SQLITE_CORRUPT     11
#define SQLITE_NOTFOUND    12
#define SQLITE_FULL        13
#define SQLITE_CANTOPEN    14
#define SQLITE_PROTOCOL    15
#define SQLITE_EMPTY       16
#define SQLITE_SCHEMA      17
#define SQLITE_TOOBIG      18
#define SQLITE_CONSTRAINT  19
#define SQLITE_MISMATCH    20
#define SQLITE_MISUSE      21
#define SQLITE_NOLFS       22
#define SQLITE_AUTH        23
#define SQLITE_FORMAT      24
#define SQLITE_RANGE       25
#define SQLITE_NOTADB      26
#define SQLITE_ROW         100
#define SQLITE_DONE        101

#define SQLITE_OPEN_READONLY         0x00000001
#define SQLITE_OPEN_READWRITE        0x00000002
#define SQLITE_OPEN_CREATE           0x00000004
#define SQLITE_OPEN_DELETEONCLOSE    0x00000008
#define SQLITE_OPEN_EXCLUSIVE        0x00000010
#define SQLITE_OPEN_AUTOPROXY        0x00000020
#define SQLITE_OPEN_URI              0x00000040
#define SQLITE_OPEN_MAIN_DB          0x00000100
#define SQLITE_OPEN_TEMP_DB          0x00000200
#define SQLITE_OPEN_TRANSIENT_DB     0x00000400
#define SQLITE_OPEN_MAIN_JOURNAL     0x00000800
#define SQLITE_OPEN_TEMP_JOURNAL     0x00001000
#define SQLITE_OPEN_SUBJOURNAL       0x00002000
#define SQLITE_OPEN_MASTER_JOURNAL   0x00004000
#define SQLITE_OPEN_NOMUTEX          0x00008000
#define SQLITE_OPEN_FULLMUTEX        0x00010000
#define SQLITE_OPEN_SHAREDCACHE      0x00020000
#define SQLITE_OPEN_PRIVATECACHE     0x00040000
#define SQLITE_OPEN_WAL              0x00080000

#define SQLITE_INTEGER  1
#define SQLITE_FLOAT    2
#define SQLITE_TEXT     3
#define SQLITE_BLOB     4
#define SQLITE_NULL     5
#define SQLITE3_TEXT    3

#define SQLITE_UTF8     1
#define SQLITE_UTF16LE  2
#define SQLITE_UTF16BE  3
#define SQLITE_UTF16    4
#define SQLITE_ANY      5

#define SQLITE_STATIC      ((sqlite3_destructor_type)0)
#define SQLITE_TRANSIENT   ((sqlite3_destructor_type)-1)

#define SQLITE_STATUS_MEMORY_USED          0
#define SQLITE_STATUS_PAGECACHE_USED       1
#define SQLITE_STATUS_PAGECACHE_OVERFLOW   2
#define SQLITE_STATUS_SCRATCH_USED         3
#define SQLITE_STATUS_SCRATCH_OVERFLOW     4
#define SQLITE_STATUS_MALLOC_SIZE          5
#define SQLITE_STATUS_PARSER_STACK         6
#define SQLITE_STATUS_PAGECACHE_SIZE       7
#define SQLITE_STATUS_SCRATCH_SIZE         8
#define SQLITE_STATUS_MALLOC_COUNT         9

#define SQLITE_DBSTATUS_LOOKASIDE_USED       0
#define SQLITE_DBSTATUS_CACHE_USED           1
#define SQLITE_DBSTATUS_SCHEMA_USED          2
#define SQLITE_DBSTATUS_STMT_USED            3
#define SQLITE_DBSTATUS_LOOKASIDE_HIT        4
#define SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE  5
#define SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL  6
#define SQLITE_DBSTATUS_CACHE_HIT            7
#define SQLITE_DBSTATUS_CACHE_MISS           8
#define SQLITE_DBSTATUS_CACHE_WRITE          9

#define SQLITE_STMTSTATUS_FULLSCAN_STEP     1
#define SQLITE_STMTSTATUS_SORT              2
#define SQLITE_STMTSTATUS_AUTOINDEX         3

#define SQLITE_LIMIT_LENGTH                    0
#define SQLITE_LIMIT_SQL_LENGTH                1
#define SQLITE_LIMIT_COLUMN                    2
#define SQLITE_LIMIT_EXPR_DEPTH                3
#define SQLITE_LIMIT_COMPOUND_SELECT           4
#define SQLITE_LIMIT_VDBE_OP                   5
#define SQLITE_LIMIT_FUNCTION_ARG              6
#define SQLITE_LIMIT_ATTACHED                  7
#define SQLITE_LIMIT_LIKE_PATTERN_LENGTH       8
#define SQLITE_LIMIT_VARIABLE_NUMBER           9
#define SQLITE_LIMIT_TRIGGER_DEPTH            10

#define SQLITE_CHECKPOINT_PASSIVE  0
#define SQLITE_CHECKPOINT_FULL     1
#define SQLITE_CHECKPOINT_RESTART  2

typedef void (*sqlite3_destructor_type)(void*);

typedef int (*sqlite3_callback)(void*, int, char**, char**);

const char* sqlite3_libversion(void);
const char* sqlite3_sourceid(void);
int sqlite3_libversion_number(void);
int sqlite3_threadsafe(void);
int sqlite3_compileoption_used(const char* zOptName);
const char* sqlite3_compileoption_get(int N);

int sqlite3_initialize(void);
int sqlite3_shutdown(void);
int sqlite3_os_init(void);
int sqlite3_os_end(void);
int sqlite3_config(int, ...);
int sqlite3_db_config(sqlite3*, int op, ...);

int sqlite3_open(const char* filename, sqlite3** ppDb);
int sqlite3_open16(const void* filename, sqlite3** ppDb);
int sqlite3_open_v2(const char* filename, sqlite3** ppDb, int flags, const char* zVfs);
int sqlite3_close(sqlite3*);
int sqlite3_close_v2(sqlite3*);
int sqlite3_exec(sqlite3*, const char* sql, int (*callback)(void*, int, char**, char**), void*, char** errmsg);
int sqlite3_extended_result_codes(sqlite3*, int onoff);
sqlite3_int64 sqlite3_last_insert_rowid(sqlite3*);
int sqlite3_changes(sqlite3*);
int sqlite3_total_changes(sqlite3*);
void sqlite3_interrupt(sqlite3*);
int sqlite3_complete(const char* sql);
int sqlite3_complete16(const void* sql);
int sqlite3_busy_handler(sqlite3*, int (*)(void*, int), void*);
int sqlite3_busy_timeout(sqlite3*, int ms);
int sqlite3_get_table(sqlite3* db, const char* zSql, char*** pazResult, int* pnRow, int* pnColumn, char** pzErrmsg);
void sqlite3_free_table(char** result);
int sqlite3_get_autocommit(sqlite3*);
sqlite3* sqlite3_db_handle(sqlite3_stmt*);
const char* sqlite3_db_filename(sqlite3* db, const char* zDbName);
int sqlite3_db_readonly(sqlite3* db, const char* zDbName);
sqlite3_stmt* sqlite3_next_stmt(sqlite3* pDb, sqlite3_stmt* pStmt);
int sqlite3_limit(sqlite3*, int id, int newVal);
int sqlite3_enable_shared_cache(int);
int sqlite3_release_memory(int);
int sqlite3_db_release_memory(sqlite3*);
void sqlite3_soft_heap_limit(int N);
sqlite3_int64 sqlite3_soft_heap_limit64(sqlite3_int64 N);
int sqlite3_sleep(int);
int sqlite3_file_control(sqlite3*, const char* zDbName, int op, void*);
int sqlite3_test_control(int op, ...);
sqlite3_mutex* sqlite3_db_mutex(sqlite3*);

int sqlite3_errcode(sqlite3* db);
int sqlite3_extended_errcode(sqlite3* db);
const char* sqlite3_errmsg(sqlite3*);
const void* sqlite3_errmsg16(sqlite3*);
const char* sqlite3_errstr(int);

int sqlite3_prepare(sqlite3* db, const char* zSql, int nByte, sqlite3_stmt** ppStmt, const char** pzTail);
int sqlite3_prepare_v2(sqlite3* db, const char* zSql, int nByte, sqlite3_stmt** ppStmt, const char** pzTail);
int sqlite3_prepare16(sqlite3* db, const void* zSql, int nByte, sqlite3_stmt** ppStmt, const void** pzTail);
int sqlite3_prepare16_v2(sqlite3* db, const void* zSql, int nByte, sqlite3_stmt** ppStmt, const void** pzTail);
const char* sqlite3_sql(sqlite3_stmt* pStmt);
int sqlite3_stmt_readonly(sqlite3_stmt* pStmt);
int sqlite3_stmt_busy(sqlite3_stmt*);
int sqlite3_stmt_status(sqlite3_stmt*, int op, int resetFlg);
int sqlite3_step(sqlite3_stmt*);
int sqlite3_reset(sqlite3_stmt* pStmt);
int sqlite3_clear_bindings(sqlite3_stmt*);
int sqlite3_finalize(sqlite3_stmt* pStmt);

int sqlite3_bind_blob(sqlite3_stmt*, int, const void*, int n, void (*)(void*));
int sqlite3_bind_double(sqlite3_stmt*, int, double);
int sqlite3_bind_int(sqlite3_stmt*, int, int);
int sqlite3_bind_int64(sqlite3_stmt*, int, sqlite3_int64);
int sqlite3_bind_null(sqlite3_stmt*, int);
int sqlite3_bind_text(sqlite3_stmt*, int, const char*, int n, void (*)(void*));
int sqlite3_bind_text16(sqlite3_stmt*, int, const void*, int, void (*)(void*));
int sqlite3_bind_value(sqlite3_stmt*, int, const sqlite3_value*);
int sqlite3_bind_zeroblob(sqlite3_stmt*, int, int n);
int sqlite3_bind_parameter_count(sqlite3_stmt*);
const char* sqlite3_bind_parameter_name(sqlite3_stmt*, int);
int sqlite3_bind_parameter_index(sqlite3_stmt*, const char* zName);

int sqlite3_column_count(sqlite3_stmt* pStmt);
int sqlite3_data_count(sqlite3_stmt* pStmt);
const char* sqlite3_column_name(sqlite3_stmt*, int N);
const void* sqlite3_column_name16(sqlite3_stmt*, int N);
const char* sqlite3_column_decltype(sqlite3_stmt*, int);
const void* sqlite3_column_decltype16(sqlite3_stmt*, int);
const void* sqlite3_column_blob(sqlite3_stmt*, int iCol);
int sqlite3_column_bytes(sqlite3_stmt*, int iCol);
int sqlite3_column_bytes16(sqlite3_stmt*, int iCol);
double sqlite3_column_double(sqlite3_stmt*, int iCol);
int sqlite3_column_int(sqlite3_stmt*, int iCol);
sqlite3_int64 sqlite3_column_int64(sqlite3_stmt*, int iCol);
const unsigned char* sqlite3_column_text(sqlite3_stmt*, int iCol);
const void* sqlite3_column_text16(sqlite3_stmt*, int iCol);
int sqlite3_column_type(sqlite3_stmt*, int iCol);
sqlite3_value* sqlite3_column_value(sqlite3_stmt*, int iCol);

const void* sqlite3_value_blob(sqlite3_value*);
int sqlite3_value_bytes(sqlite3_value*);
int sqlite3_value_bytes16(sqlite3_value*);
double sqlite3_value_double(sqlite3_value*);
int sqlite3_value_int(sqlite3_value*);
sqlite3_int64 sqlite3_value_int64(sqlite3_value*);
const unsigned char* sqlite3_value_text(sqlite3_value*);
const void* sqlite3_value_text16(sqlite3_value*);
const void* sqlite3_value_text16le(sqlite3_value*);
const void* sqlite3_value_text16be(sqlite3_value*);
int sqlite3_value_type(sqlite3_value*);
int sqlite3_value_numeric_type(sqlite3_value*);

int sqlite3_create_function(sqlite3* db, const char* zFunctionName, int nArg, int eTextRep, void* pApp,
                            void (*xFunc)(sqlite3_context*, int, sqlite3_value**),
                            void (*xStep)(sqlite3_context*, int, sqlite3_value**),
                            void (*xFinal)(sqlite3_context*));
int sqlite3_create_function16(sqlite3* db, const void* zFunctionName, int nArg, int eTextRep, void* pApp,
                              void (*xFunc)(sqlite3_context*, int, sqlite3_value**),
                              void (*xStep)(sqlite3_context*, int, sqlite3_value**),
                              void (*xFinal)(sqlite3_context*));
int sqlite3_create_function_v2(sqlite3* db, const char* zFunctionName, int nArg, int eTextRep, void* pApp,
                               void (*xFunc)(sqlite3_context*, int, sqlite3_value**),
                               void (*xStep)(sqlite3_context*, int, sqlite3_value**),
                               void (*xFinal)(sqlite3_context*), void (*xDestroy)(void*));
int sqlite3_overload_function(sqlite3*, const char* zFuncName, int nArg);
void* sqlite3_aggregate_context(sqlite3_context*, int nBytes);
void* sqlite3_user_data(sqlite3_context*);
sqlite3* sqlite3_context_db_handle(sqlite3_context*);
void* sqlite3_get_auxdata(sqlite3_context*, int N);
void sqlite3_set_auxdata(sqlite3_context*, int N, void*, void (*)(void*));

void sqlite3_result_blob(sqlite3_context*, const void*, int, void (*)(void*));
void sqlite3_result_double(sqlite3_context*, double);
void sqlite3_result_error(sqlite3_context*, const char*, int);
void sqlite3_result_error16(sqlite3_context*, const void*, int);
void sqlite3_result_error_toobig(sqlite3_context*);
void sqlite3_result_error_nomem(sqlite3_context*);
void sqlite3_result_error_code(sqlite3_context*, int);
void sqlite3_result_int(sqlite3_context*, int);
void sqlite3_result_int64(sqlite3_context*, sqlite3_int64);
void sqlite3_result_null(sqlite3_context*);
void sqlite3_result_text(sqlite3_context*, const char*, int, void (*)(void*));
void sqlite3_result_text16(sqlite3_context*, const void*, int, void (*)(void*));
void sqlite3_result_text16le(sqlite3_context*, const void*, int, void (*)(void*));
void sqlite3_result_text16be(sqlite3_context*, const void*, int, void (*)(void*));
void sqlite3_result_value(sqlite3_context*, sqlite3_value*);
void sqlite3_result_zeroblob(sqlite3_context*, int n);

int sqlite3_create_collation(sqlite3*, const char* zName, int eTextRep, void* pArg,
                             int (*xCompare)(void*, int, const void*, int, const void*));
int sqlite3_create_collation_v2(sqlite3*, const char* zName, int eTextRep, void* pArg,
                                int (*xCompare)(void*, int, const void*, int, const void*), void (*xDestroy)(void*));
int sqlite3_create_collation16(sqlite3*, const void* zName, int eTextRep, void* pArg,
                               int (*xCompare)(void*, int, const void*, int, const void*));
int sqlite3_collation_needed(sqlite3*, void*, void (*)(void*, sqlite3*, int eTextRep, const char*));
int sqlite3_collation_needed16(sqlite3*, void*, void (*)(void*, sqlite3*, int eTextRep, const void*));

void* sqlite3_trace(sqlite3*, void (*xTrace)(void*, const char*), void*);
void* sqlite3_profile(sqlite3*, void (*xProfile)(void*, const char*, sqlite3_uint64), void*);
void* sqlite3_commit_hook(sqlite3*, int (*)(void*), void*);
void* sqlite3_rollback_hook(sqlite3*, void (*)(void*), void*);
void* sqlite3_update_hook(sqlite3*, void (*)(void*, int, const char*, const char*, sqlite3_int64), void*);
void* sqlite3_wal_hook(sqlite3*, int (*)(void*, sqlite3*, const char*, int), void*);
int sqlite3_wal_autocheckpoint(sqlite3* db, int N);
int sqlite3_wal_checkpoint(sqlite3* db, const char* zDb);
int sqlite3_wal_checkpoint_v2(sqlite3* db, const char* zDb, int eMode, int* pnLog, int* pnCkpt);
int sqlite3_auto_extension(void (*xEntryPoint)(void));
void sqlite3_reset_auto_extension(void);
int sqlite3_enable_load_extension(sqlite3* db, int onoff);
int sqlite3_load_extension(sqlite3* db, const char* zFile, const char* zProc, char** pzErrMsg);

int sqlite3_blob_open(sqlite3*, const char* zDb, const char* zTable, const char* zColumn, sqlite3_int64 iRow,
                      int flags, sqlite3_blob** ppBlob);
int sqlite3_blob_reopen(sqlite3_blob*, sqlite3_int64);
int sqlite3_blob_close(sqlite3_blob*);
int sqlite3_blob_bytes(sqlite3_blob*);
int sqlite3_blob_read(sqlite3_blob*, void* Z, int N, int iOffset);
int sqlite3_blob_write(sqlite3_blob*, const void* z, int n, int iOffset);

sqlite3_backup* sqlite3_backup_init(sqlite3* pDest, const char* zDestName, sqlite3* pSource, const char* zSourceName);
int sqlite3_backup_step(sqlite3_backup* p, int nPage);
int sqlite3_backup_finish(sqlite3_backup* p);
int sqlite3_backup_remaining(sqlite3_backup* p);
int sqlite3_backup_pagecount(sqlite3_backup* p);

void* sqlite3_malloc(int);
void* sqlite3_realloc(void*, int);
void sqlite3_free(void*);
sqlite3_int64 sqlite3_memory_used(void);
sqlite3_int64 sqlite3_memory_highwater(int resetFlag);
void sqlite3_randomness(int N, void* P);
int sqlite3_status(int op, int* pCurrent, int* pHighwater, int resetFlag);
int sqlite3_db_status(sqlite3*, int op, int* pCur, int* pHiwtr, int resetFlg);

char* sqlite3_mprintf(const char*, ...);
char* sqlite3_vmprintf(const char*, __builtin_va_list);
char* sqlite3_snprintf(int, char*, const char*, ...);
char* sqlite3_vsnprintf(int, char*, const char*, __builtin_va_list);
void sqlite3_log(int iErrCode, const char* zFormat, ...);

int sqlite3_stricmp(const char*, const char*);
int sqlite3_strnicmp(const char*, const char*, int);
int sqlite3_strglob(const char* zGlob, const char* zStr);

const char* sqlite3_uri_parameter(const char* zFilename, const char* zParam);
int sqlite3_uri_boolean(const char* zFile, const char* zParam, int bDefault);
sqlite3_int64 sqlite3_uri_int64(const char*, const char*, sqlite3_int64);

sqlite3_mutex* sqlite3_mutex_alloc(int);
void sqlite3_mutex_free(sqlite3_mutex*);
void sqlite3_mutex_enter(sqlite3_mutex*);
int sqlite3_mutex_try(sqlite3_mutex*);
void sqlite3_mutex_leave(sqlite3_mutex*);

#ifdef __cplusplus
}
#endif
