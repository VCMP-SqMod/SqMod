// ------------------------------------------------------------------------------------------------
#include "Library/SQLite.hpp"

// ------------------------------------------------------------------------------------------------
#include <ctime>

// ------------------------------------------------------------------------------------------------
#include <sqstdblob.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
// Error message when failed to bind value to parameter index.
#define SQMOD_BINDFAILED "Unable to bind (%s) parameter (%d) because [%s]"

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(SQLiteConnectionTypename, _SC("SQLiteConnection"))
SQMODE_DECL_TYPENAME(SQLiteParameterTypename, _SC("SQLiteParameter"))
SQMODE_DECL_TYPENAME(SQLiteColumnTypename, _SC("SQLiteColumn"))
SQMODE_DECL_TYPENAME(SQLiteStatementTypename, _SC("SQLiteStatement"))
SQMODE_DECL_TYPENAME(SQLiteTransactionTypename, _SC("SQLiteTransaction"))

// ------------------------------------------------------------------------------------------------
#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

/* ------------------------------------------------------------------------------------------------
 * Helper class that represents an integral enumeration value. Used to reduce compilation times.
*/
struct SEnumElement
{
    CSStr Name;
    Int32 Value;
};

// ------------------------------------------------------------------------------------------------
static const SEnumElement g_MainEnum[] = {
    {_SC("ABORT"),                                SQLITE_ABORT},
    {_SC("ABORT_ROLLBACK"),                       SQLITE_ABORT_ROLLBACK},
    {_SC("ACCESS_EXISTS"),                        SQLITE_ACCESS_EXISTS},
    {_SC("ACCESS_READ"),                          SQLITE_ACCESS_READ},
    {_SC("ACCESS_READWRITE"),                     SQLITE_ACCESS_READWRITE},
    {_SC("ALTER_TABLE"),                          SQLITE_ALTER_TABLE},
    {_SC("ANALYZE"),                              SQLITE_ANALYZE},
    {_SC("ANY"),                                  SQLITE_ANY},
    {_SC("ATTACH"),                               SQLITE_ATTACH},
    {_SC("AUTH"),                                 SQLITE_AUTH},
    {_SC("AUTH_USER"),                            SQLITE_AUTH_USER},
    {_SC("BLOB"),                                 SQLITE_BLOB},
    {_SC("BUSY"),                                 SQLITE_BUSY},
    {_SC("BUSY_RECOVERY"),                        SQLITE_BUSY_RECOVERY},
    {_SC("BUSY_SNAPSHOT"),                        SQLITE_BUSY_SNAPSHOT},
    {_SC("CANTOPEN"),                             SQLITE_CANTOPEN},
    {_SC("CANTOPEN_CONVPATH"),                    SQLITE_CANTOPEN_CONVPATH},
    {_SC("CANTOPEN_FULLPATH"),                    SQLITE_CANTOPEN_FULLPATH},
    {_SC("CANTOPEN_ISDIR"),                       SQLITE_CANTOPEN_ISDIR},
    {_SC("CANTOPEN_NOTEMPDIR"),                   SQLITE_CANTOPEN_NOTEMPDIR},
    {_SC("CHECKPOINT_FULL"),                      SQLITE_CHECKPOINT_FULL},
    {_SC("CHECKPOINT_PASSIVE"),                   SQLITE_CHECKPOINT_PASSIVE},
    {_SC("CHECKPOINT_RESTART"),                   SQLITE_CHECKPOINT_RESTART},
    {_SC("CHECKPOINT_TRUNCATE"),                  SQLITE_CHECKPOINT_TRUNCATE},
    {_SC("CONFIG_COVERING_INDEX_SCAN"),           SQLITE_CONFIG_COVERING_INDEX_SCAN},
    {_SC("CONFIG_GETMALLOC"),                     SQLITE_CONFIG_GETMALLOC},
    {_SC("CONFIG_GETMUTEX"),                      SQLITE_CONFIG_GETMUTEX},
    {_SC("CONFIG_GETPCACHE"),                     SQLITE_CONFIG_GETPCACHE},
    {_SC("CONFIG_GETPCACHE2"),                    SQLITE_CONFIG_GETPCACHE2},
    {_SC("CONFIG_HEAP"),                          SQLITE_CONFIG_HEAP},
    {_SC("CONFIG_LOG"),                           SQLITE_CONFIG_LOG},
    {_SC("CONFIG_LOOKASIDE"),                     SQLITE_CONFIG_LOOKASIDE},
    {_SC("CONFIG_MALLOC"),                        SQLITE_CONFIG_MALLOC},
    {_SC("CONFIG_MEMSTATUS"),                     SQLITE_CONFIG_MEMSTATUS},
    {_SC("CONFIG_MMAP_SIZE"),                     SQLITE_CONFIG_MMAP_SIZE},
    {_SC("CONFIG_MULTITHREAD"),                   SQLITE_CONFIG_MULTITHREAD},
    {_SC("CONFIG_MUTEX"),                         SQLITE_CONFIG_MUTEX},
    {_SC("CONFIG_PAGECACHE"),                     SQLITE_CONFIG_PAGECACHE},
    {_SC("CONFIG_PCACHE"),                        SQLITE_CONFIG_PCACHE},
    {_SC("CONFIG_PCACHE2"),                       SQLITE_CONFIG_PCACHE2},
    {_SC("CONFIG_PCACHE_HDRSZ"),                  SQLITE_CONFIG_PCACHE_HDRSZ},
    {_SC("CONFIG_PMASZ"),                         SQLITE_CONFIG_PMASZ},
    {_SC("CONFIG_SCRATCH"),                       SQLITE_CONFIG_SCRATCH},
    {_SC("CONFIG_SERIALIZED"),                    SQLITE_CONFIG_SERIALIZED},
    {_SC("CONFIG_SINGLETHREAD"),                  SQLITE_CONFIG_SINGLETHREAD},
    {_SC("CONFIG_SQLLOG"),                        SQLITE_CONFIG_SQLLOG},
    {_SC("CONFIG_URI"),                           SQLITE_CONFIG_URI},
    {_SC("CONFIG_WIN32_HEAPSIZE"),                SQLITE_CONFIG_WIN32_HEAPSIZE},
    {_SC("CONSTRAINT"),                           SQLITE_CONSTRAINT},
    {_SC("CONSTRAINT_CHECK"),                     SQLITE_CONSTRAINT_CHECK},
    {_SC("CONSTRAINT_COMMITHOOK"),                SQLITE_CONSTRAINT_COMMITHOOK},
    {_SC("CONSTRAINT_FOREIGNKEY"),                SQLITE_CONSTRAINT_FOREIGNKEY},
    {_SC("CONSTRAINT_FUNCTION"),                  SQLITE_CONSTRAINT_FUNCTION},
    {_SC("CONSTRAINT_NOTNULL"),                   SQLITE_CONSTRAINT_NOTNULL},
    {_SC("CONSTRAINT_PRIMARYKEY"),                SQLITE_CONSTRAINT_PRIMARYKEY},
    {_SC("CONSTRAINT_ROWID"),                     SQLITE_CONSTRAINT_ROWID},
    {_SC("CONSTRAINT_TRIGGER"),                   SQLITE_CONSTRAINT_TRIGGER},
    {_SC("CONSTRAINT_UNIQUE"),                    SQLITE_CONSTRAINT_UNIQUE},
    {_SC("CONSTRAINT_VTAB"),                      SQLITE_CONSTRAINT_VTAB},
    {_SC("COPY"),                                 SQLITE_COPY},
    {_SC("CORRUPT"),                              SQLITE_CORRUPT},
    {_SC("CORRUPT_VTAB"),                         SQLITE_CORRUPT_VTAB},
    {_SC("CREATE_INDEX"),                         SQLITE_CREATE_INDEX},
    {_SC("CREATE_TABLE"),                         SQLITE_CREATE_TABLE},
    {_SC("CREATE_TEMP_INDEX"),                    SQLITE_CREATE_TEMP_INDEX},
    {_SC("CREATE_TEMP_TABLE"),                    SQLITE_CREATE_TEMP_TABLE},
    {_SC("CREATE_TEMP_TRIGGER"),                  SQLITE_CREATE_TEMP_TRIGGER},
    {_SC("CREATE_TEMP_VIEW"),                     SQLITE_CREATE_TEMP_VIEW},
    {_SC("CREATE_TRIGGER"),                       SQLITE_CREATE_TRIGGER},
    {_SC("CREATE_VIEW"),                          SQLITE_CREATE_VIEW},
    {_SC("CREATE_VTABLE"),                        SQLITE_CREATE_VTABLE},
    {_SC("DBCONFIG_ENABLE_FKEY"),                 SQLITE_DBCONFIG_ENABLE_FKEY},
    {_SC("DBCONFIG_ENABLE_TRIGGER"),              SQLITE_DBCONFIG_ENABLE_TRIGGER},
    {_SC("DBCONFIG_LOOKASIDE"),                   SQLITE_DBCONFIG_LOOKASIDE},
    {_SC("DBSTATUS_CACHE_HIT"),                   SQLITE_DBSTATUS_CACHE_HIT},
    {_SC("DBSTATUS_CACHE_MISS"),                  SQLITE_DBSTATUS_CACHE_MISS},
    {_SC("DBSTATUS_CACHE_USED"),                  SQLITE_DBSTATUS_CACHE_USED},
    {_SC("DBSTATUS_CACHE_WRITE"),                 SQLITE_DBSTATUS_CACHE_WRITE},
    {_SC("DBSTATUS_DEFERRED_FKS"),                SQLITE_DBSTATUS_DEFERRED_FKS},
    {_SC("DBSTATUS_LOOKASIDE_HIT"),               SQLITE_DBSTATUS_LOOKASIDE_HIT},
    {_SC("DBSTATUS_LOOKASIDE_MISS_FULL"),         SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL},
    {_SC("DBSTATUS_LOOKASIDE_MISS_SIZE"),         SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE},
    {_SC("DBSTATUS_LOOKASIDE_USED"),              SQLITE_DBSTATUS_LOOKASIDE_USED},
    {_SC("DBSTATUS_MAX"),                         SQLITE_DBSTATUS_MAX},
    {_SC("DBSTATUS_SCHEMA_USED"),                 SQLITE_DBSTATUS_SCHEMA_USED},
    {_SC("DBSTATUS_STMT_USED"),                   SQLITE_DBSTATUS_STMT_USED},
    {_SC("DELETE"),                               SQLITE_DELETE},
    {_SC("DENY"),                                 SQLITE_DENY},
    {_SC("DETACH"),                               SQLITE_DETACH},
    {_SC("DETERMINISTIC"),                        SQLITE_DETERMINISTIC},
    {_SC("DONE"),                                 SQLITE_DONE},
    {_SC("DROP_INDEX"),                           SQLITE_DROP_INDEX},
    {_SC("DROP_TABLE"),                           SQLITE_DROP_TABLE},
    {_SC("DROP_TEMP_INDEX"),                      SQLITE_DROP_TEMP_INDEX},
    {_SC("DROP_TEMP_TABLE"),                      SQLITE_DROP_TEMP_TABLE},
    {_SC("DROP_TEMP_TRIGGER"),                    SQLITE_DROP_TEMP_TRIGGER},
    {_SC("DROP_TEMP_VIEW"),                       SQLITE_DROP_TEMP_VIEW},
    {_SC("DROP_TRIGGER"),                         SQLITE_DROP_TRIGGER},
    {_SC("DROP_VIEW"),                            SQLITE_DROP_VIEW},
    {_SC("DROP_VTABLE"),                          SQLITE_DROP_VTABLE},
    {_SC("EMPTY"),                                SQLITE_EMPTY},
    {_SC("ERROR"),                                SQLITE_ERROR},
    {_SC("FAIL"),                                 SQLITE_FAIL},
    {_SC("FCNTL_BUSYHANDLER"),                    SQLITE_FCNTL_BUSYHANDLER},
    {_SC("FCNTL_CHUNK_SIZE"),                     SQLITE_FCNTL_CHUNK_SIZE},
    {_SC("FCNTL_COMMIT_PHASETWO"),                SQLITE_FCNTL_COMMIT_PHASETWO},
    {_SC("FCNTL_FILE_POINTER"),                   SQLITE_FCNTL_FILE_POINTER},
    {_SC("FCNTL_GET_LOCKPROXYFILE"),              SQLITE_FCNTL_GET_LOCKPROXYFILE},
    {_SC("FCNTL_HAS_MOVED"),                      SQLITE_FCNTL_HAS_MOVED},
    {_SC("FCNTL_LAST_ERRNO"),                     SQLITE_FCNTL_LAST_ERRNO},
    {_SC("FCNTL_LOCKSTATE"),                      SQLITE_FCNTL_LOCKSTATE},
    {_SC("FCNTL_MMAP_SIZE"),                      SQLITE_FCNTL_MMAP_SIZE},
    {_SC("FCNTL_OVERWRITE"),                      SQLITE_FCNTL_OVERWRITE},
    {_SC("FCNTL_PERSIST_WAL"),                    SQLITE_FCNTL_PERSIST_WAL},
    {_SC("FCNTL_POWERSAFE_OVERWRITE"),            SQLITE_FCNTL_POWERSAFE_OVERWRITE},
    {_SC("FCNTL_PRAGMA"),                         SQLITE_FCNTL_PRAGMA},
    {_SC("FCNTL_RBU"),                            SQLITE_FCNTL_RBU},
    {_SC("FCNTL_SET_LOCKPROXYFILE"),              SQLITE_FCNTL_SET_LOCKPROXYFILE},
    {_SC("FCNTL_SIZE_HINT"),                      SQLITE_FCNTL_SIZE_HINT},
    {_SC("FCNTL_SYNC"),                           SQLITE_FCNTL_SYNC},
    {_SC("FCNTL_SYNC_OMITTED"),                   SQLITE_FCNTL_SYNC_OMITTED},
    {_SC("FCNTL_TEMPFILENAME"),                   SQLITE_FCNTL_TEMPFILENAME},
    {_SC("FCNTL_TRACE"),                          SQLITE_FCNTL_TRACE},
    {_SC("FCNTL_VFSNAME"),                        SQLITE_FCNTL_VFSNAME},
    {_SC("FCNTL_WAL_BLOCK"),                      SQLITE_FCNTL_WAL_BLOCK},
    {_SC("FCNTL_WIN32_AV_RETRY"),                 SQLITE_FCNTL_WIN32_AV_RETRY},
    {_SC("FCNTL_WIN32_SET_HANDLE"),               SQLITE_FCNTL_WIN32_SET_HANDLE},
    {_SC("FCNTL_ZIPVFS"),                         SQLITE_FCNTL_ZIPVFS},
    {_SC("FLOAT"),                                SQLITE_FLOAT},
    {_SC("FORMAT"),                               SQLITE_FORMAT},
    {_SC("FULL"),                                 SQLITE_FULL},
    {_SC("FUNCTION"),                             SQLITE_FUNCTION},
    {_SC("IGNORE"),                               SQLITE_IGNORE},
    {_SC("INDEX_CONSTRAINT_EQ"),                  SQLITE_INDEX_CONSTRAINT_EQ},
    {_SC("INDEX_CONSTRAINT_GE"),                  SQLITE_INDEX_CONSTRAINT_GE},
    {_SC("INDEX_CONSTRAINT_GT"),                  SQLITE_INDEX_CONSTRAINT_GT},
    {_SC("INDEX_CONSTRAINT_LE"),                  SQLITE_INDEX_CONSTRAINT_LE},
    {_SC("INDEX_CONSTRAINT_LT"),                  SQLITE_INDEX_CONSTRAINT_LT},
    {_SC("INDEX_CONSTRAINT_MATCH"),               SQLITE_INDEX_CONSTRAINT_MATCH},
    {_SC("INDEX_SCAN_UNIQUE"),                    SQLITE_INDEX_SCAN_UNIQUE},
    {_SC("INSERT"),                               SQLITE_INSERT},
    {_SC("INTEGER"),                              SQLITE_INTEGER},
    {_SC("INTERNAL"),                             SQLITE_INTERNAL},
    {_SC("INTERRUPT"),                            SQLITE_INTERRUPT},
    {_SC("IOCAP_ATOMIC"),                         SQLITE_IOCAP_ATOMIC},
    {_SC("IOCAP_ATOMIC16K"),                      SQLITE_IOCAP_ATOMIC16K},
    {_SC("IOCAP_ATOMIC1K"),                       SQLITE_IOCAP_ATOMIC1K},
    {_SC("IOCAP_ATOMIC2K"),                       SQLITE_IOCAP_ATOMIC2K},
    {_SC("IOCAP_ATOMIC32K"),                      SQLITE_IOCAP_ATOMIC32K},
    {_SC("IOCAP_ATOMIC4K"),                       SQLITE_IOCAP_ATOMIC4K},
    {_SC("IOCAP_ATOMIC512"),                      SQLITE_IOCAP_ATOMIC512},
    {_SC("IOCAP_ATOMIC64K"),                      SQLITE_IOCAP_ATOMIC64K},
    {_SC("IOCAP_ATOMIC8K"),                       SQLITE_IOCAP_ATOMIC8K},
    {_SC("IOCAP_IMMUTABLE"),                      SQLITE_IOCAP_IMMUTABLE},
    {_SC("IOCAP_POWERSAFE_OVERWRITE"),            SQLITE_IOCAP_POWERSAFE_OVERWRITE},
    {_SC("IOCAP_SAFE_APPEND"),                    SQLITE_IOCAP_SAFE_APPEND},
    {_SC("IOCAP_SEQUENTIAL"),                     SQLITE_IOCAP_SEQUENTIAL},
    {_SC("IOCAP_UNDELETABLE_WHEN_OPEN"),          SQLITE_IOCAP_UNDELETABLE_WHEN_OPEN},
    {_SC("IOERR"),                                SQLITE_IOERR},
    {_SC("IOERR_ACCESS"),                         SQLITE_IOERR_ACCESS},
    {_SC("IOERR_BLOCKED"),                        SQLITE_IOERR_BLOCKED},
    {_SC("IOERR_CHECKRESERVEDLOCK"),              SQLITE_IOERR_CHECKRESERVEDLOCK},
    {_SC("IOERR_CLOSE"),                          SQLITE_IOERR_CLOSE},
    {_SC("IOERR_CONVPATH"),                       SQLITE_IOERR_CONVPATH},
    {_SC("IOERR_DELETE"),                         SQLITE_IOERR_DELETE},
    {_SC("IOERR_DELETE_NOENT"),                   SQLITE_IOERR_DELETE_NOENT},
    {_SC("IOERR_DIR_CLOSE"),                      SQLITE_IOERR_DIR_CLOSE},
    {_SC("IOERR_DIR_FSYNC"),                      SQLITE_IOERR_DIR_FSYNC},
    {_SC("IOERR_FSTAT"),                          SQLITE_IOERR_FSTAT},
    {_SC("IOERR_FSYNC"),                          SQLITE_IOERR_FSYNC},
    {_SC("IOERR_GETTEMPPATH"),                    SQLITE_IOERR_GETTEMPPATH},
    {_SC("IOERR_LOCK"),                           SQLITE_IOERR_LOCK},
    {_SC("IOERR_MMAP"),                           SQLITE_IOERR_MMAP},
    {_SC("IOERR_NOMEM"),                          SQLITE_IOERR_NOMEM},
    {_SC("IOERR_RDLOCK"),                         SQLITE_IOERR_RDLOCK},
    {_SC("IOERR_READ"),                           SQLITE_IOERR_READ},
    {_SC("IOERR_SEEK"),                           SQLITE_IOERR_SEEK},
    {_SC("IOERR_SHMLOCK"),                        SQLITE_IOERR_SHMLOCK},
    {_SC("IOERR_SHMMAP"),                         SQLITE_IOERR_SHMMAP},
    {_SC("IOERR_SHMOPEN"),                        SQLITE_IOERR_SHMOPEN},
    {_SC("IOERR_SHMSIZE"),                        SQLITE_IOERR_SHMSIZE},
    {_SC("IOERR_SHORT_READ"),                     SQLITE_IOERR_SHORT_READ},
    {_SC("IOERR_TRUNCATE"),                       SQLITE_IOERR_TRUNCATE},
    {_SC("IOERR_UNLOCK"),                         SQLITE_IOERR_UNLOCK},
    {_SC("IOERR_VNODE"),                          SQLITE_IOERR_VNODE},
    {_SC("IOERR_WRITE"),                          SQLITE_IOERR_WRITE},
    {_SC("LIMIT_ATTACHED"),                       SQLITE_LIMIT_ATTACHED},
    {_SC("LIMIT_COLUMN"),                         SQLITE_LIMIT_COLUMN},
    {_SC("LIMIT_COMPOUND_SELECT"),                SQLITE_LIMIT_COMPOUND_SELECT},
    {_SC("LIMIT_EXPR_DEPTH"),                     SQLITE_LIMIT_EXPR_DEPTH},
    {_SC("LIMIT_FUNCTION_ARG"),                   SQLITE_LIMIT_FUNCTION_ARG},
    {_SC("LIMIT_LENGTH"),                         SQLITE_LIMIT_LENGTH},
    {_SC("LIMIT_LIKE_PATTERN_LENGTH"),            SQLITE_LIMIT_LIKE_PATTERN_LENGTH},
    {_SC("LIMIT_SQL_LENGTH"),                     SQLITE_LIMIT_SQL_LENGTH},
    {_SC("LIMIT_TRIGGER_DEPTH"),                  SQLITE_LIMIT_TRIGGER_DEPTH},
    {_SC("LIMIT_VARIABLE_NUMBER"),                SQLITE_LIMIT_VARIABLE_NUMBER},
    {_SC("LIMIT_VDBE_OP"),                        SQLITE_LIMIT_VDBE_OP},
    {_SC("LIMIT_WORKER_THREADS"),                 SQLITE_LIMIT_WORKER_THREADS},
    {_SC("LOCKED"),                               SQLITE_LOCKED},
    {_SC("LOCKED_SHAREDCACHE"),                   SQLITE_LOCKED_SHAREDCACHE},
    {_SC("LOCK_EXCLUSIVE"),                       SQLITE_LOCK_EXCLUSIVE},
    {_SC("LOCK_NONE"),                            SQLITE_LOCK_NONE},
    {_SC("LOCK_PENDING"),                         SQLITE_LOCK_PENDING},
    {_SC("LOCK_RESERVED"),                        SQLITE_LOCK_RESERVED},
    {_SC("LOCK_SHARED"),                          SQLITE_LOCK_SHARED},
    {_SC("MISMATCH"),                             SQLITE_MISMATCH},
    {_SC("MISUSE"),                               SQLITE_MISUSE},
    {_SC("MUTEX_FAST"),                           SQLITE_MUTEX_FAST},
    {_SC("MUTEX_RECURSIVE"),                      SQLITE_MUTEX_RECURSIVE},
    {_SC("MUTEX_STATIC_APP1"),                    SQLITE_MUTEX_STATIC_APP1},
    {_SC("MUTEX_STATIC_APP2"),                    SQLITE_MUTEX_STATIC_APP2},
    {_SC("MUTEX_STATIC_APP3"),                    SQLITE_MUTEX_STATIC_APP3},
    {_SC("MUTEX_STATIC_LRU"),                     SQLITE_MUTEX_STATIC_LRU},
    {_SC("MUTEX_STATIC_LRU2"),                    SQLITE_MUTEX_STATIC_LRU2},
    {_SC("MUTEX_STATIC_MASTER"),                  SQLITE_MUTEX_STATIC_MASTER},
    {_SC("MUTEX_STATIC_MEM"),                     SQLITE_MUTEX_STATIC_MEM},
    {_SC("MUTEX_STATIC_MEM2"),                    SQLITE_MUTEX_STATIC_MEM2},
    {_SC("MUTEX_STATIC_OPEN"),                    SQLITE_MUTEX_STATIC_OPEN},
    {_SC("MUTEX_STATIC_PMEM"),                    SQLITE_MUTEX_STATIC_PMEM},
    {_SC("MUTEX_STATIC_PRNG"),                    SQLITE_MUTEX_STATIC_PRNG},
    {_SC("MUTEX_STATIC_VFS1"),                    SQLITE_MUTEX_STATIC_VFS1},
    {_SC("MUTEX_STATIC_VFS2"),                    SQLITE_MUTEX_STATIC_VFS2},
    {_SC("MUTEX_STATIC_VFS3"),                    SQLITE_MUTEX_STATIC_VFS3},
    {_SC("NOLFS"),                                SQLITE_NOLFS},
    {_SC("NOMEM"),                                SQLITE_NOMEM},
    {_SC("NOTADB"),                               SQLITE_NOTADB},
    {_SC("NOTFOUND"),                             SQLITE_NOTFOUND},
    {_SC("NOTICE"),                               SQLITE_NOTICE},
    {_SC("NOTICE_RECOVER_ROLLBACK"),              SQLITE_NOTICE_RECOVER_ROLLBACK},
    {_SC("NOTICE_RECOVER_WAL"),                   SQLITE_NOTICE_RECOVER_WAL},
    {_SC("NULL"),                                 SQLITE_NULL},
    {_SC("OK"),                                   SQLITE_OK},
    {_SC("OPEN_AUTOPROXY"),                       SQLITE_OPEN_AUTOPROXY},
    {_SC("OPEN_CREATE"),                          SQLITE_OPEN_CREATE},
    {_SC("OPEN_DELETEONCLOSE"),                   SQLITE_OPEN_DELETEONCLOSE},
    {_SC("OPEN_EXCLUSIVE"),                       SQLITE_OPEN_EXCLUSIVE},
    {_SC("OPEN_FULLMUTEX"),                       SQLITE_OPEN_FULLMUTEX},
    {_SC("OPEN_MAIN_DB"),                         SQLITE_OPEN_MAIN_DB},
    {_SC("OPEN_MAIN_JOURNAL"),                    SQLITE_OPEN_MAIN_JOURNAL},
    {_SC("OPEN_MASTER_JOURNAL"),                  SQLITE_OPEN_MASTER_JOURNAL},
    {_SC("OPEN_MEMORY"),                          SQLITE_OPEN_MEMORY},
    {_SC("OPEN_NOMUTEX"),                         SQLITE_OPEN_NOMUTEX},
    {_SC("OPEN_PRIVATECACHE"),                    SQLITE_OPEN_PRIVATECACHE},
    {_SC("OPEN_READONLY"),                        SQLITE_OPEN_READONLY},
    {_SC("OPEN_READWRITE"),                       SQLITE_OPEN_READWRITE},
    {_SC("OPEN_SHAREDCACHE"),                     SQLITE_OPEN_SHAREDCACHE},
    {_SC("OPEN_SUBJOURNAL"),                      SQLITE_OPEN_SUBJOURNAL},
    {_SC("OPEN_TEMP_DB"),                         SQLITE_OPEN_TEMP_DB},
    {_SC("OPEN_TEMP_JOURNAL"),                    SQLITE_OPEN_TEMP_JOURNAL},
    {_SC("OPEN_TRANSIENT_DB"),                    SQLITE_OPEN_TRANSIENT_DB},
    {_SC("OPEN_URI"),                             SQLITE_OPEN_URI},
    {_SC("OPEN_WAL"),                             SQLITE_OPEN_WAL},
    {_SC("PERM"),                                 SQLITE_PERM},
    {_SC("PRAGMA"),                               SQLITE_PRAGMA},
    {_SC("PROTOCOL"),                             SQLITE_PROTOCOL},
    {_SC("RANGE"),                                SQLITE_RANGE},
    {_SC("READ"),                                 SQLITE_READ},
    {_SC("READONLY"),                             SQLITE_READONLY},
    {_SC("READONLY_CANTLOCK"),                    SQLITE_READONLY_CANTLOCK},
    {_SC("READONLY_DBMOVED"),                     SQLITE_READONLY_DBMOVED},
    {_SC("READONLY_RECOVERY"),                    SQLITE_READONLY_RECOVERY},
    {_SC("READONLY_ROLLBACK"),                    SQLITE_READONLY_ROLLBACK},
    {_SC("RECURSIVE"),                            SQLITE_RECURSIVE},
    {_SC("REINDEX"),                              SQLITE_REINDEX},
    {_SC("REPLACE"),                              SQLITE_REPLACE},
    {_SC("ROLLBACK"),                             SQLITE_ROLLBACK},
    {_SC("ROW"),                                  SQLITE_ROW},
    {_SC("SAVEPOINT"),                            SQLITE_SAVEPOINT},
    {_SC("SCANSTAT_EST"),                         SQLITE_SCANSTAT_EST},
    {_SC("SCANSTAT_EXPLAIN"),                     SQLITE_SCANSTAT_EXPLAIN},
    {_SC("SCANSTAT_NAME"),                        SQLITE_SCANSTAT_NAME},
    {_SC("SCANSTAT_NLOOP"),                       SQLITE_SCANSTAT_NLOOP},
    {_SC("SCANSTAT_NVISIT"),                      SQLITE_SCANSTAT_NVISIT},
    {_SC("SCANSTAT_SELECTID"),                    SQLITE_SCANSTAT_SELECTID},
    {_SC("SCHEMA"),                               SQLITE_SCHEMA},
    {_SC("SELECT"),                               SQLITE_SELECT},
    {_SC("SHM_EXCLUSIVE"),                        SQLITE_SHM_EXCLUSIVE},
    {_SC("SHM_LOCK"),                             SQLITE_SHM_LOCK},
    {_SC("SHM_NLOCK"),                            SQLITE_SHM_NLOCK},
    {_SC("SHM_SHARED"),                           SQLITE_SHM_SHARED},
    {_SC("SHM_UNLOCK"),                           SQLITE_SHM_UNLOCK},
    {_SC("STATUS_MALLOC_COUNT"),                  SQLITE_STATUS_MALLOC_COUNT},
    {_SC("STATUS_MALLOC_SIZE"),                   SQLITE_STATUS_MALLOC_SIZE},
    {_SC("STATUS_MEMORY_USED"),                   SQLITE_STATUS_MEMORY_USED},
    {_SC("STATUS_PAGECACHE_OVERFLOW"),            SQLITE_STATUS_PAGECACHE_OVERFLOW},
    {_SC("STATUS_PAGECACHE_SIZE"),                SQLITE_STATUS_PAGECACHE_SIZE},
    {_SC("STATUS_PAGECACHE_USED"),                SQLITE_STATUS_PAGECACHE_USED},
    {_SC("STATUS_PARSER_STACK"),                  SQLITE_STATUS_PARSER_STACK},
    {_SC("STATUS_SCRATCH_OVERFLOW"),              SQLITE_STATUS_SCRATCH_OVERFLOW},
    {_SC("STATUS_SCRATCH_SIZE"),                  SQLITE_STATUS_SCRATCH_SIZE},
    {_SC("STATUS_SCRATCH_USED"),                  SQLITE_STATUS_SCRATCH_USED},
    {_SC("STMTSTATUS_AUTOINDEX"),                 SQLITE_STMTSTATUS_AUTOINDEX},
    {_SC("STMTSTATUS_FULLSCAN_STEP"),             SQLITE_STMTSTATUS_FULLSCAN_STEP},
    {_SC("STMTSTATUS_SORT"),                      SQLITE_STMTSTATUS_SORT},
    {_SC("STMTSTATUS_VM_STEP"),                   SQLITE_STMTSTATUS_VM_STEP},
    {_SC("SYNC_DATAONLY"),                        SQLITE_SYNC_DATAONLY},
    {_SC("SYNC_FULL"),                            SQLITE_SYNC_FULL},
    {_SC("SYNC_NORMAL"),                          SQLITE_SYNC_NORMAL},
    {_SC("TESTCTRL_ALWAYS"),                      SQLITE_TESTCTRL_ALWAYS},
    {_SC("TESTCTRL_ASSERT"),                      SQLITE_TESTCTRL_ASSERT},
    {_SC("TESTCTRL_BENIGN_MALLOC_HOOKS"),         SQLITE_TESTCTRL_BENIGN_MALLOC_HOOKS},
    {_SC("TESTCTRL_BITVEC_TEST"),                 SQLITE_TESTCTRL_BITVEC_TEST},
    {_SC("TESTCTRL_BYTEORDER"),                   SQLITE_TESTCTRL_BYTEORDER},
    {_SC("TESTCTRL_EXPLAIN_STMT"),                SQLITE_TESTCTRL_EXPLAIN_STMT},
    {_SC("TESTCTRL_FAULT_INSTALL"),               SQLITE_TESTCTRL_FAULT_INSTALL},
    {_SC("TESTCTRL_FIRST"),                       SQLITE_TESTCTRL_FIRST},
    {_SC("TESTCTRL_IMPOSTER"),                    SQLITE_TESTCTRL_IMPOSTER},
    {_SC("TESTCTRL_ISINIT"),                      SQLITE_TESTCTRL_ISINIT},
    {_SC("TESTCTRL_ISKEYWORD"),                   SQLITE_TESTCTRL_ISKEYWORD},
    {_SC("TESTCTRL_LAST"),                        SQLITE_TESTCTRL_LAST},
    {_SC("TESTCTRL_LOCALTIME_FAULT"),             SQLITE_TESTCTRL_LOCALTIME_FAULT},
    {_SC("TESTCTRL_NEVER_CORRUPT"),               SQLITE_TESTCTRL_NEVER_CORRUPT},
    {_SC("TESTCTRL_OPTIMIZATIONS"),               SQLITE_TESTCTRL_OPTIMIZATIONS},
    {_SC("TESTCTRL_PENDING_BYTE"),                SQLITE_TESTCTRL_PENDING_BYTE},
    {_SC("TESTCTRL_PRNG_RESET"),                  SQLITE_TESTCTRL_PRNG_RESET},
    {_SC("TESTCTRL_PRNG_RESTORE"),                SQLITE_TESTCTRL_PRNG_RESTORE},
    {_SC("TESTCTRL_PRNG_SAVE"),                   SQLITE_TESTCTRL_PRNG_SAVE},
    {_SC("TESTCTRL_RESERVE"),                     SQLITE_TESTCTRL_RESERVE},
    {_SC("TESTCTRL_SCRATCHMALLOC"),               SQLITE_TESTCTRL_SCRATCHMALLOC},
    {_SC("TESTCTRL_SORTER_MMAP"),                 SQLITE_TESTCTRL_SORTER_MMAP},
    {_SC("TESTCTRL_VDBE_COVERAGE"),               SQLITE_TESTCTRL_VDBE_COVERAGE},
    {_SC("TEXT"),                                 SQLITE_TEXT},
    {_SC("TOOBIG"),                               SQLITE_TOOBIG},
    {_SC("TRANSACTION"),                          SQLITE_TRANSACTION},
    {_SC("UPDATE"),                               SQLITE_UPDATE},
    {_SC("UTF16"),                                SQLITE_UTF16},
    {_SC("UTF16BE"),                              SQLITE_UTF16BE},
    {_SC("UTF16LE"),                              SQLITE_UTF16LE},
    {_SC("UTF16_ALIGNED"),                        SQLITE_UTF16_ALIGNED},
    {_SC("UTF8"),                                 SQLITE_UTF8},
    {_SC("VERSION_NUMBER"),                       SQLITE_VERSION_NUMBER},
    {_SC("VTAB_CONSTRAINT_SUPPORT"),              SQLITE_VTAB_CONSTRAINT_SUPPORT},
    {_SC("WARNING"),                              SQLITE_WARNING},
    {_SC("WARNING_AUTOINDEX"),                    SQLITE_WARNING_AUTOINDEX}
};
#pragma clang diagnostic pop

// ------------------------------------------------------------------------------------------------
static inline bool IsDigitsOnly(CSStr str)
{
    while (std::isdigit(*str) || std::isspace(*str))
    {
        ++str;
    }
    // Return whether we reached the end while searching
    return *str == '\0';
}

// ------------------------------------------------------------------------------------------------
Object GetConnectionObj(const ConnRef & conn)
{
    return Object(new SQLiteConnection(conn));
}

// ------------------------------------------------------------------------------------------------
Object GetStatementObj(const StmtRef & stmt)
{
    return Object(new SQLiteStatement(stmt));
}

// ------------------------------------------------------------------------------------------------
bool IsQueryEmpty(CSStr str)
{
    // Is the pointer valid?
    if (!str || *str == '\0')
    {
        return true;
    }
    // Currently processed character
    SQChar c = 0;
    // See if the query contains any alpha numeric characters
    while ((c = *str) != 0)
    {
        if (std::isalnum(c) != 0)
        {
            return false;
        }
        ++str;
    }
    // At this point we consider the query empty
    return true;
}

// ------------------------------------------------------------------------------------------------
CSStr GetErrStr(Int32 status)
{
    return sqlite3_errstr(status);
}

// ------------------------------------------------------------------------------------------------
void SetSoftHeapLimit(Int32 limit)
{
    sqlite3_soft_heap_limit(limit);
}

// ------------------------------------------------------------------------------------------------
Int32 ReleaseMemory(Int32 bytes)
{
    return sqlite3_release_memory(bytes);
}

// ------------------------------------------------------------------------------------------------
Object GetMemoryUsage()
{
    // Obtain the initial stack size
    const StackGuard sg;
    // Push a long integer instance with the requested value on the stack
    return Object(new SLongInt(sqlite3_memory_used()));
}

// ------------------------------------------------------------------------------------------------
Object GetMemoryHighwaterMark(bool reset)
{
    // Obtain the initial stack size
    const StackGuard sg;
    // Push a long integer instance with the requested value on the stack
    return Object(new SLongInt(sqlite3_memory_highwater(reset)));
}

// ------------------------------------------------------------------------------------------------
CSStr EscapeString(StackStrF & str)
{
    // Is there even a string to escape?
    if (str.mLen <= 0)
    {
        return _SC(""); // Default to empty string
    }
    // Attempt to escape the specified string
    sqlite3_snprintf(GetTempBuffSize(), GetTempBuff(), "%q", str.mPtr);
    // Return the resulted string
    return GetTempBuff();
}

// ------------------------------------------------------------------------------------------------
CCStr EscapeStringEx(SQChar spec, StackStrF & str)
{
    // Utility that allows changing the format specifier temporarily
    static SQChar fs[] = _SC("%q");
    // Validate the specified format specifier
    if ((spec != 'q') && (spec != 'Q') && (spec != 'w') && (spec != 's'))
    {
        STHROWF("Unknown format specifier: '%c'", spec);
    }
    // Is there even a string to escape?
    else if (!str.mLen)
    {
        return _SC(""); // Default to empty string
    }
    // Apply the format specifier
    fs[1] = spec;
    // Attempt to escape the specified string
    sqlite3_snprintf(GetTempBuffSize(), GetTempBuff(), fs, str.mPtr);
    // Restore the format specifier
    fs[1] = 'q';
    // Return the resulted string
    return GetTempBuff();
}

// ------------------------------------------------------------------------------------------------
CCStr ArrayToQueryColumns(Array & arr)
{
    // Do we even have any elements to process?
    if (arr.Length() <= 0)
    {
        return _SC(""); // Default to empty string
    }
    // Allocate a vector with the required amount of column names
    std::vector< String > values(arr.Length());
    // Attempt to extract the array elements as strings
    arr.GetArray< String >(&values[0], values.size());
    // Used to know the position of the next column name
    Uint32 offset = 0;
    // Obtain the start of the array
    auto itr = values.begin();
    // Process all elements within range
    for (; itr != values.end() && offset < GetTempBuffSize(); ++itr)
    {
        // Is the name valid?
        if (itr->empty())
        {
            STHROWF("Invalid column name");
        }
        // Attempt to append the column name to the buffer
        sqlite3_snprintf(static_cast< int >(GetTempBuffSize() - offset), GetTempBuff() + offset, "[%q], ", itr->c_str());
        // Add the column name size to the offset
        offset += itr->size();
        // Also include the comma and space in the offset
        offset += 2;
    }
    // Trim the last coma and space
    if (offset >= 2)
    {
        GetTempBuff()[offset-2] = '\0';
    }
    else
    {
        GetTempBuff()[0] = '\0';
    }
    // Return the resulted string
    return GetTempBuff();
}

// ------------------------------------------------------------------------------------------------
CCStr TableToQueryColumns(Table & tbl)
{
    // Used to know the position of the next column name
    Uint32 offset = 0;
    // Used to obtain the column name temporarily
    String name;
    // Obtain the start of the table
    Table::iterator itr;
    // Process all elements within range
    while (tbl.Next(itr))
    {
        // Use the element key as the column name
        name.assign(itr.getName());
        // Is the name valid?
        if (name.empty())
        {
            STHROWF("Invalid or empty column name");
        }
        // Attempt to append the column name to the buffer
        sqlite3_snprintf(static_cast< int >(GetTempBuffSize() - offset), GetTempBuff() + offset, "[%q], ", name.c_str());
        // Add the column name size to the offset
        offset += name.size();
        // Also include the comma and space in the offset
        offset += 2;
    }
    // Trim the last coma and space
    if (offset >= 2)
    {
        GetTempBuff()[offset-2] = '\0';
    }
    else
    {
        GetTempBuff()[0] = '\0';
    }
    // Return the resulted string
    return GetTempBuff();
}

// ------------------------------------------------------------------------------------------------
SQLiteConnHnd::SQLiteConnHnd()
    : mPtr(nullptr)
    , mStatus(SQLITE_OK)
    , mQueue()
    , mFlags(0)
    , mName()
    , mVFS()
    , mMemory(false)
    , mTrace(false)
    , mProfile(false)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
SQLiteConnHnd::~SQLiteConnHnd()
{
    // Is there anything to close?
    if (mPtr != nullptr)
    {
        // Flush remaining queries in the queue and ignore the result
        Flush(mQueue.size(), NullObject(), NullFunction());
        // NOTE: Should we call sqlite3_interrupt(...) before closing?
        // Attempt to close the database
        if ((sqlite3_close(mPtr)) != SQLITE_OK)
        {
            LogErr("Unable to close SQLite connection [%s]", sqlite3_errmsg(mPtr));
        }
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteConnHnd::Create(CSStr name, Int32 flags, CSStr vfs)
{
    // Make sure a previous connection doesn't exist
    if (mPtr)
    {
        STHROWF("Unable to connect to database. Database already connected");
    }
    // Make sure the name is valid
    else if (!name || *name == '\0')
    {
        STHROWF("Unable to connect to database. The name is invalid");
    }
    // Attempt to create the database connection
    else if ((mStatus = sqlite3_open_v2(name, &mPtr, flags, vfs)) != SQLITE_OK)
    {
        // Grab the error message before destroying the handle
        String msg(sqlite3_errmsg(mPtr) ? sqlite3_errmsg(mPtr) : _SC("Unknown reason"));
        // Must be destroyed regardless of result
        sqlite3_close(mPtr);
        // Prevent further use of this handle
        mPtr = nullptr;
        // Now its safe to throw the error
        STHROWF("Unable to connect to database [%s]", msg.c_str());
    }
    // Let's save the specified information
    mName.assign(name);
    mFlags = flags;
    mVFS.assign(vfs ? vfs : _SC(""));
    // Optional check if database is initially stored in memory
    mMemory = (mName == _SC(":memory:"));
}

// ------------------------------------------------------------------------------------------------
Int32 SQLiteConnHnd::Flush(Uint32 num, Object & env, Function & func)
{
    // Do we even have a valid connection?
    if (!mPtr)
    {
        return -1; // No connection!
    }
    // Is there anything to flush?
    else if (!num || mQueue.empty())
    {
        return 0; // Nothing to process!
    }
    // Can we even flush that many?
    else if (num > mQueue.size())
    {
        num = mQueue.size();
    }
    // Generate the function that should be called upon error
    Function callback = Function(env.GetVM(), env.GetObject(), func.GetFunc());
    // Obtain iterators to the range of queries that should be flushed
    auto itr = mQueue.begin();
    auto end = mQueue.begin() + num;
    // Attempt to begin the flush transaction
    if ((mStatus = sqlite3_exec(mPtr, "BEGIN", nullptr, nullptr, nullptr)) != SQLITE_OK)
    {
        STHROWF("Unable to begin flush transaction [%s]", sqlite3_errmsg(mPtr));
    }
    // Process all queries within range of selection
    for (; itr != end; ++itr)
    {
        // Should we manually terminate this query?
        /*
        if (*(*itr).rbegin() != ';')
        {
            itr->push_back(';');
        }
        */
        // Attempt to execute the currently processed query string
        if ((mStatus = sqlite3_exec(mPtr, itr->c_str(), nullptr, nullptr, nullptr)) == SQLITE_OK)
        {
            continue;
        }
        // Do we have to execute any callback to resolve our issue?
        else if (!callback.IsNull())
        {
            try
            {
                // Ask the callback whether the query processing should end here
                SharedPtr< bool > ret = callback.Evaluate< bool >(mStatus, *itr);
                // Should we break here?
                if (!!ret && !(*ret))
                {
                    break;
                }
            }
            catch (const Sqrat::Exception & e)
            {
                LogErr("Squirrel error caught in flush handler [%s]", e.what());
            }
            catch (const std::exception & e)
            {
                LogErr("Program error caught in flush handler [%s]", e.what());
            }
            catch (...)
            {
                LogErr("Unknown error caught in flush handler");
            }
        }
    }
    // Erase all queries till end or till the point of failure (if any occurred)
    mQueue.erase(mQueue.begin(), itr);
    // Attempt to commit changes requested during transaction
    if ((mStatus = sqlite3_exec(mPtr, "COMMIT", nullptr, nullptr, nullptr)) == SQLITE_OK)
    {
        return sqlite3_changes(mPtr);
    }
    // Attempt to roll back erroneous changes
    else if ((mStatus = sqlite3_exec(mPtr, "ROLLBACK", nullptr, nullptr, nullptr)) != SQLITE_OK)
    {
        STHROWF("Unable to rollback flush transaction [%s]", sqlite3_errmsg(mPtr));
    }
    // The transaction failed somehow but we managed to rollback
    else
    {
        STHROWF("Unable to commit flush transaction [%s]", sqlite3_errmsg(mPtr));
    }
    // Operation failed
    return -1;
}

// ------------------------------------------------------------------------------------------------
SQLiteStmtHnd::SQLiteStmtHnd(ConnRef conn)
    : mPtr(nullptr)
    , mStatus(SQLITE_OK)
    , mConn(std::move(conn))
    , mQuery()
    , mColumns(0)
    , mParameters(0)
    , mIndexes()
    , mGood(false)
    , mDone(false)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
SQLiteStmtHnd::~SQLiteStmtHnd()
{
    // Is there anything to finalize?
    if (mPtr != nullptr)
    {
        // Attempt to finalize the statement
        if ((sqlite3_finalize(mPtr)) != SQLITE_OK)
        {
            LogErr("Unable to finalize SQLite statement [%s]", mConn->ErrMsg());
        }
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteStmtHnd::Create(CSStr query, SQInteger length)
{
    // Make sure a previous statement doesn't exist
    if (mPtr)
    {
        STHROWF("Unable to prepare statement. Statement already prepared");
    }
    // Is the specified database connection is valid?
    else if (!mConn)
    {
        STHROWF("Unable to prepare statement. Invalid connection handle");
    }
    // Is the specified query string valid?
    else if (!query || !length || *query == '\0')
    {
        STHROWF("Unable to prepare statement. Invalid or empty query string");
    }
    // Save the query string
    mQuery.assign(query, length);
    // Attempt to prepare a statement with the specified query string
    if ((mStatus = sqlite3_prepare_v2(mConn->mPtr, mQuery.c_str(), ConvTo< Int32 >::From(mQuery.size()),
                                            &mPtr, nullptr)) != SQLITE_OK)
    {
        // Clear the query string since it failed
        mQuery.clear();
        // Explicitly make sure the handle is null
        mPtr = nullptr;
        // Now it's safe to throw the error
        STHROWF("Unable to prepare statement [%s]", mConn->ErrMsg());
    }
    else
    {
        // Obtain the number of available columns
        mColumns = sqlite3_column_count(mPtr);
        // Obtain the number of available parameters
        mParameters = sqlite3_bind_parameter_count(mPtr);
    }
}

// ------------------------------------------------------------------------------------------------
Int32 SQLiteStmtHnd::GetColumnIndex(CSStr name, SQInteger length)
{
    // Validate the handle
    if (!mPtr)
    {
        STHROWF("Invalid SQLite statement");
    }
    // Are the names cached?
    else if (mIndexes.empty())
    {
        for (Int32 i = 0; i < mColumns; ++i)
        {
            // Get the column name at the current index
            auto column_name = static_cast< CSStr >(sqlite3_column_name(mPtr, i));
            // Validate the name
            if (!column_name)
            {
                STHROWF("Unable to retrieve column name for index (%d)", i);
            }
            // Save it to guarantee the same lifetime as this instance
            else
            {
                mIndexes[name] = i;
            }
        }
    }
    const String str(name, length < 0 ? std::strlen(name) : length);
    // Attempt to find the specified column
    const Indexes::iterator itr = mIndexes.find(str);
    // Was there a column with the specified name?
    if (itr != mIndexes.end())
    {
        return itr->second;
    }
    // No such column exists (expecting the invoker to validate the result)
    return -1;
}

// ------------------------------------------------------------------------------------------------
CCStr SQLiteStmtHnd::ErrStr() const
{
    return mConn ? sqlite3_errstr(sqlite3_errcode(mConn->mPtr)) : _SC("");
}

// ------------------------------------------------------------------------------------------------
CCStr SQLiteStmtHnd::ErrMsg() const
{
    return mConn ? sqlite3_errmsg(mConn->mPtr) : _SC("");
}

// ------------------------------------------------------------------------------------------------
Int32 SQLiteStmtHnd::ErrNo() const
{
    return mConn ? sqlite3_errcode(mConn->mPtr) : SQLITE_NOMEM;
}

// ------------------------------------------------------------------------------------------------
Int32 SQLiteStmtHnd::ExErrNo() const
{
    return mConn ? sqlite3_extended_errcode(mConn->mPtr) : SQLITE_NOMEM;
}

// ------------------------------------------------------------------------------------------------
void SQLiteConnection::TraceOutput(void * /*ptr*/, CCStr sql)
{
    LogInf("SQLite Trace: %s", sql);
}

// ------------------------------------------------------------------------------------------------
void SQLiteConnection::ProfileOutput(void * /*ptr*/, CCStr sql, sqlite3_uint64 time)
{
    LogInf("SQLite profile (time: %llu): %s", time, sql);
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SQLiteConnection::Validate(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite connection reference =>[%s:%d]", file, line);
    }
}
#else
void SQLiteConnection::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite connection reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SQLiteConnection::ValidateCreated(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite connection reference =>[%s:%d]", file, line);
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid SQLite connection =>[%s:%d]", file, line);
    }
}
#else
void SQLiteConnection::ValidateCreated() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite connection reference");
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid SQLite connection");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const ConnRef & SQLiteConnection::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const ConnRef & SQLiteConnection::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const ConnRef & SQLiteConnection::GetCreated(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    return m_Handle;
}
#else
const ConnRef & SQLiteConnection::GetCreated() const
{
    ValidateCreated();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
void SQLiteConnection::Open(StackStrF & name)
{
    // Should we create a connection handle?
    if (!m_Handle)
    {
        m_Handle = ConnRef(new SQLiteConnHnd());
    }
    // Make sure another database isn't opened
    if (SQMOD_GET_VALID(*this)->mPtr != nullptr)
    {
        STHROWF("Already referencing a valid database connection");
    }
    // Effing signed/unsigned warnings everywhere. I just need it to shut up.
    constexpr unsigned OPEN_READWRITE_F = SQLITE_OPEN_READWRITE;
    constexpr unsigned OPEN_CREATE_F = SQLITE_OPEN_CREATE;
    // Perform the requested operation
    m_Handle->Create(name.mPtr, OPEN_READWRITE_F | OPEN_CREATE_F, nullptr);
}

// ------------------------------------------------------------------------------------------------
void SQLiteConnection::Open(StackStrF & name, Int32 flags)
{
    // Should we create a connection handle?
    if (!m_Handle)
    {
        m_Handle = ConnRef(new SQLiteConnHnd());
    }
    // Make sure another database isn't opened
    if (SQMOD_GET_VALID(*this)->mPtr != nullptr)
    {
        STHROWF("Already referencing a valid database connection");
    }
    // Perform the requested operation
    m_Handle->Create(name.mPtr, flags, nullptr);
}

// ------------------------------------------------------------------------------------------------
void SQLiteConnection::Open(StackStrF & name, Int32 flags, StackStrF & vfs)
{
    // Should we create a connection handle?
    if (!m_Handle)
    {
        m_Handle = ConnRef(new SQLiteConnHnd());
    }
    // Make sure another database isn't opened
    if (SQMOD_GET_VALID(*this)->mPtr != nullptr)
    {
        STHROWF("Already referencing a valid database connection");
    }
    // Perform the requested operation
    m_Handle->Create(name.mPtr, flags, vfs.mPtr);
}

// ------------------------------------------------------------------------------------------------
Int32 SQLiteConnection::Exec(StackStrF & str)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to execute the specified query
    m_Handle->mStatus = sqlite3_exec(m_Handle->mPtr, str.mPtr, nullptr, nullptr, nullptr);
    // Validate the execution result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF("Unable to execute query [%s]", m_Handle->ErrMsg());
    }
    // Return rows affected by this query
    return sqlite3_changes(m_Handle->mPtr);
}

// ------------------------------------------------------------------------------------------------
Object SQLiteConnection::Query(StackStrF & str) const
{
    SQMOD_VALIDATE_CREATED(*this);
    // Return the requested information
    return Object(new SQLiteStatement(m_Handle, str));
}

// ------------------------------------------------------------------------------------------------
void SQLiteConnection::Queue(StackStrF & str)
{
    SQMOD_VALIDATE(*this);
    // Is there a query to commit?
    if (!str.mLen || IsQueryEmpty(str.mPtr))
    {
        STHROWF("No query string to queue");
    }
    // Add the specified string to the queue
    m_Handle->mQueue.emplace_back(str.mPtr, str.mLen);
}

// ------------------------------------------------------------------------------------------------
bool SQLiteConnection::IsReadOnly() const
{
    // Request the desired information
    const int result = sqlite3_db_readonly(SQMOD_GET_CREATED(*this)->mPtr, "main");
    // Verify the result
    if (result == -1)
    {
        STHROWF("'main' is not the name of a database on connection");
    }
    // Return the requested information
    return (result != 1);
}

// ------------------------------------------------------------------------------------------------
bool SQLiteConnection::TableExists(StackStrF & name) const
{
    StackStrF query("SELECT count(*) FROM [sqlite_master] WHERE [type]='table' AND [name]=?");
    // Prepare a statement to inspect the master table
    SQLiteStatement stmt(SQMOD_GET_CREATED(*this), query);
    // Could the statement be created?
    if (stmt.IsValid())
    {
        // Bind the specified name onto the statement parameter
        SQLiteParameter(stmt.GetHandle(), 1).SetString(name);
        // Attempt to step the statement and obtain a value
        if (stmt.Step())
        {
            return (sqlite3_column_int(stmt, 0) == 1);
        }
    }
    // Assume it doesn't exist
    return false;
}

// ------------------------------------------------------------------------------------------------
void SQLiteConnection::SetTracing(bool toggle)
{
    // Check whether changes are necessary
    if (SQMOD_GET_CREATED(*this)->mTrace == toggle)
    {
        return; // No point in proceeding
    }
    // Do we have to disable it?
    else if (m_Handle->mTrace)
    {
        sqlite3_trace(m_Handle->mPtr, nullptr, nullptr);
    }
    // Go ahead and enable tracing
    else
    {
        sqlite3_trace(m_Handle->mPtr, &SQLiteConnection::TraceOutput, nullptr);
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteConnection::SetProfiling(bool toggle)
{
    // Check whether changes are necessary
    if (SQMOD_GET_CREATED(*this)->mProfile == toggle)
    {
        return; // No point in proceeding
    }
    // Do we have to disable it?
    else if (m_Handle->mProfile)
    {
        sqlite3_profile(m_Handle->mPtr, nullptr, nullptr);
    }
    // Go ahead and enable profiling
    else
    {
        sqlite3_profile(m_Handle->mPtr, &SQLiteConnection::ProfileOutput, nullptr);
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteConnection::SetBusyTimeout(Int32 millis)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Apply the requested timeout
    if ((m_Handle->mStatus = sqlite3_busy_timeout(m_Handle->mPtr, millis)) != SQLITE_OK)
    {
        STHROWF("Unable to set busy timeout [%s]", m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
Int32 SQLiteConnection::GetInfo(Int32 operation, bool highwater, bool reset)
{
    // Where to retrieve the information
    Int32 cur_value;
    Int32 hiwtr_value;
    // Attempt to retrieve the specified information
    if ((m_Handle->mStatus = sqlite3_db_status(SQMOD_GET_CREATED(*this)->mPtr, operation, &cur_value, &hiwtr_value, reset)) != SQLITE_OK)
    {
        STHROWF("Unable to get runtime status information", m_Handle->ErrMsg());
    }
    // Return the high-water value if requested
    else if (highwater)
    {
        return hiwtr_value;
    }
    // Return the requested information
    return cur_value;
}

// ------------------------------------------------------------------------------------------------
void SQLiteConnection::ReserveQueue(Uint32 num)
{
    SQMOD_VALIDATE(*this);
    // Perform the requested operation
    m_Handle->mQueue.reserve(m_Handle->mQueue.size() + num);
}

// ------------------------------------------------------------------------------------------------
void SQLiteConnection::PopQueue()
{
    SQMOD_VALIDATE(*this);
    // Perform the requested operation
    if (!SQMOD_GET_VALID(*this)->mQueue.empty())
    {
        m_Handle->mQueue.pop_back();
    }
}

// ------------------------------------------------------------------------------------------------
Int32 SQLiteConnection::Flush()
{
    SQMOD_VALIDATE_CREATED(*this);
    // Perform the requested operation
    return m_Handle->Flush(m_Handle->mQueue.size(), NullObject(), NullFunction());
}

// ------------------------------------------------------------------------------------------------
Int32 SQLiteConnection::Flush(SQInteger num)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Perform the requested operation
    return m_Handle->Flush(ConvTo< Uint32 >::From(num), NullObject(), NullFunction());
}

// ------------------------------------------------------------------------------------------------
Int32 SQLiteConnection::Flush(Object & env, Function & func)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Perform the requested operation
    return m_Handle->Flush(m_Handle->mQueue.size(), env, func);
}

// ------------------------------------------------------------------------------------------------
Int32 SQLiteConnection::Flush(SQInteger num, Object & env, Function & func)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Perform the requested operation
    return m_Handle->Flush(ConvTo< Uint32 >::From(num), env, func);
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SQLiteParameter::Validate(CCStr file, Int32 line) const
{
    // Are we pointing to a valid index?
    if (m_Index < 0)
    {
        SqThrowF("Invalid column index: %d < 0 =>[%s:%d]", m_Index, file, line);
    }
    // Do we have a valid statement handle?
    else if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference =>[%s:%d]", file, line);
    }
}
#else
void SQLiteParameter::Validate() const
{
    // Are we pointing to a valid index?
    if (m_Index < 0)
    {
        SqThrowF("Invalid column index: %d < 0", m_Index);
    }
    // Do we have a valid statement handle?
    else if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SQLiteParameter::ValidateCreated(CCStr file, Int32 line) const
{
    // Are we pointing to a valid index?
    if (m_Index < 0)
    {
        SqThrowF("Invalid column index: %d < 0 =>[%s:%d]", m_Index, file, line);
    }
    else if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference =>[%s:%d]", file, line);
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid SQLite statement =>[%s:%d]", file, line);
    }
}
#else
void SQLiteParameter::ValidateCreated() const
{
    // Are we pointing to a valid index?
    if (m_Index < 0)
    {
        SqThrowF("Invalid column index: %d < 0", m_Index);
    }
    else if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference");
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid SQLite statement");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const StmtRef & SQLiteParameter::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const StmtRef & SQLiteParameter::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const StmtRef & SQLiteParameter::GetCreated(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    return m_Handle;
}
#else
const StmtRef & SQLiteParameter::GetCreated() const
{
    ValidateCreated();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SQLiteParameter::ValidateParam(Int32 idx, CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    // Is the specified index in range?
    if (!m_Handle->CheckParameter(idx))
    {
        SqThrowF("Parameter index is out of range (%d:%d) =>[%s:%d]", idx, m_Handle->mParameters,
                    file, line);
    }
}
#else
void SQLiteParameter::ValidateParam(Int32 idx) const
{
    ValidateCreated();
    // Is the specified index in range?
    if (!m_Handle->CheckParameter(idx))
    {
        SqThrowF("SQLiteParameter index is out of range (%d:%d)", idx, m_Handle->mParameters);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetIndex(const Object & param)
{
    // Where the index will be extracted
    Int32 idx = 0;
    // Grab the object virtual machine
    HSQUIRRELVM vm = param.GetVM();
    // Remember the current stack size
    const StackGuard sg(vm);
    // Push the specified object onto the stack
    Var< const Object & >::push(vm, param);
    // Identify the type of parameter was given
    switch  (param.GetType())
    {
        // Is this a string value?
        case OT_STRING:
        {
            // Obtain the object from the stack as a string
            StackStrF val(vm, -1);
            // Validate the result
            if (SQ_FAILED(val.Proc(false)))
            {
                STHROWF("%s", LastErrorString(vm).c_str());
            }
            // Is the obtained string empty?
            else if (val.mLen <= 0)
            {
                STHROWF("Cannot use an empty parameter name");
            }
            // Attempt to find a parameter with the specified name
            idx = sqlite3_bind_parameter_index(SQMOD_GET_CREATED(*this)->mPtr, val.mPtr);
        } break;
        // Is this an integer value? (or at least can be easily converted to one)
        case OT_INTEGER:
        case OT_FLOAT:
        case OT_BOOL:
        {
            idx = ConvTo< Int32 >::From(PopStackInteger(vm, -1));
        } break;
        // Is this an instance that we can extract either a string or integer from it?
        case OT_INSTANCE:
        {
            // Obtain the object from the stack as a string
            StackStrF val(vm, -1);
            // Validate the result
            if (SQ_FAILED(val.Proc(false)))
            {
                STHROWF("%s", LastErrorString(vm).c_str());
            }
            // Is the obtained string empty?
            else if (val.mLen <= 0)
            {
                STHROWF("Cannot use an empty parameter name");
            }
            // Check if this value is made only of digits
            else if (IsDigitsOnly(val.mPtr))
            {
                idx = ConvNum< Int32 >::FromStr(val.mPtr);
            }
            // Attempt to find a parameter with the specified name
            else
            {
                idx = sqlite3_bind_parameter_index(SQMOD_GET_CREATED(*this)->mPtr, val.mPtr);
            }
        } break;
        // We don't recognize this kind of value!
        default: STHROWF("Unknown parameter index of type (%s)", SqTypeName(param.GetType()));
    }
    // Assign the index with a failsafe to invalid on error
    AutoAssign< Int32 > aa(m_Index, 0, idx);
    // Validate the obtained parameter index
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Don't fall back to the invalid index anymore
    aa.Set(idx);
}

// ------------------------------------------------------------------------------------------------
Object SQLiteParameter::GetStatement() const
{
    return GetStatementObj(m_Handle);
}

// ------------------------------------------------------------------------------------------------
Object SQLiteParameter::GetConnection() const
{
    return GetConnectionObj(SQMOD_GET_VALID(*this)->mConn);
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetValue(const Object & value)
{
    switch (value.GetType())
    {
        case OT_NULL:
        {
            SetNull();
        } break;
        case OT_INTEGER:
        {
            SetInteger(value.Cast< SQInteger >());
        } break;
        case OT_FLOAT:
        {
            SetFloat(value.Cast< SQFloat >());
        } break;
        case OT_BOOL:
        {
            SetBool(value.Cast< bool >());
        } break;
        case OT_STRING:
        {
            SQMOD_VALIDATE_CREATED(*this);
            // Remember the current stack size
            const StackGuard sg;
            // Push the object onto the stack
            Var< Object >::push(DefaultVM::Get(), value);
            // Pop the object from the stack as a string
            const Var< CSStr > str(DefaultVM::Get(), -1);
            // Attempt to bind the specified value
            SetStringRaw(str.value, ConvTo< SQInteger >::From(str.size));
        } break;
        default: STHROWF("No known conversion for the specified value type");
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetBool(bool value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, value);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "bool", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetChar(SQInteger value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, ConvTo< SQChar >::From(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "char", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetInteger(SQInteger value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_integer(m_Handle->mPtr, m_Index, value);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "integer", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetInt8(SQInteger value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, ConvTo< Int8 >::From(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "int8", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetUint8(SQInteger value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, ConvTo< Uint8 >::From(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "uint8", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetInt16(SQInteger value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, ConvTo< Int16 >::From(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "int16", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetUint16(SQInteger value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, ConvTo< Uint16 >::From(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "uint16", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetInt32(SQInteger value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, ConvTo< Int32 >::From(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "int32", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetUint32(SQInteger value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, ConvTo< Uint32 >::From(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "uint32", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetInt64(const Object & value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int64(m_Handle->mPtr, m_Index, value.Cast< const SLongInt & >().GetNum());
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "int64", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetUint64(const Object & value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int64(m_Handle->mPtr, m_Index, value.Cast< const ULongInt & >().GetNum());
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "uint64", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetFloat(SQFloat value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_double(m_Handle->mPtr, m_Index, value);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "float", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetFloat32(SQFloat value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_double(m_Handle->mPtr, m_Index, ConvTo< Float32 >::From(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "float32", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetFloat64(SQFloat value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_double(m_Handle->mPtr, m_Index, value);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "float64", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetString(StackStrF & value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_text(m_Handle->mPtr, m_Index, value.mPtr, value.mLen, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "string", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetStringRaw(CSStr value, SQInteger length)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_text(m_Handle->mPtr, m_Index, value, length, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "string", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetZeroBlob(SQInteger size)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_zeroblob(m_Handle->mPtr, m_Index, ConvTo< Int32 >::From(size));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "blob", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetBlob(const Object & value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // The blob data pointer and size
    SQUserPointer ptr = nullptr;
    SQInteger len = 0;
    // Grab the associated object virtual machine
    HSQUIRRELVM vm = value.GetVM();
    // Extract the blob data from the specified object
    {
        // Remember the current stack size
        const StackGuard sg(vm);
        // Push the specified object onto the stack
        Var< const Object & >::push(vm, value);
        // Grab the blob data pointer
        if (SQ_FAILED(sqstd_getblob(vm, -1, &ptr)))
        {
            STHROWF("Unable to obtain the blob data");
        }
        // Grab the blob data size
        len = sqstd_getblobsize(vm, -1);
    }
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_blob(m_Handle->mPtr, m_Index, ptr, len, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "blob", m_Index, m_Handle->ErrMsg());
    }
}
// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetData(const SqBuffer & value)
{
    // Grab the internal buffer
    Buffer & buff = *value.GetRef();
    // Attempt to bind the specified value
#ifdef _SQ64
    m_Handle->mStatus = sqlite3_bind_blob64(m_Handle->mPtr, m_Index, buff.Data(), buff.Position(), SQLITE_TRANSIENT);
#else
    m_Handle->mStatus = sqlite3_bind_blob(m_Handle->mPtr, m_Index, buff.Data(), buff.Position(), SQLITE_TRANSIENT);
#endif
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "buffer", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetDataEx(const SqBuffer & value, SQInteger offset, SQInteger length)
{
    // Grab the internal buffer
    Buffer & buff = *value.GetRef();
    // Make sure we are within range
    if (offset < 0)
    {
        STHROWF("Negative buffer offset is not allowed: %d", offset);
    }
    else if (offset < 0)
    {
        STHROWF("Negative buffer length is not allowed: %d", length);
    }
    else if (offset + length > buff.Capacity())
    {
        STHROWF("Buffer range is outside boundaries: (%d+%d) > %u", offset, length, buff.Capacity());
    }
    // Attempt to bind the specified value
#ifdef _SQ64
    m_Handle->mStatus = sqlite3_bind_blob64(m_Handle->mPtr, m_Index, (buff.Data() + offset), static_cast< int >(offset + length), SQLITE_TRANSIENT);
#else
        m_Handle->mStatus = sqlite3_bind_blob(m_Handle->mPtr, m_Index, (buff.Data() + offset), static_cast< int >(offset + length), SQLITE_TRANSIENT);
#endif
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "buffer", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetDate(const Date & value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to generate the specified date string
    CSStr ptr = FmtStr(_SC("%s 00:00:00"), value.ToString());
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_text(m_Handle->mPtr, m_Index, ptr, -1, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "date", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetDateEx(SQInteger year, SQInteger month, SQInteger day)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Convert the specified values within the proper ranges
    const uint16_t y = ConvTo< uint16_t >::From(year);
    const uint8_t m = ConvTo< uint8_t >::From(month), d = ConvTo< uint8_t >::From(day);
    // Validate the specified date
    if (!Chrono::ValidDate(y, m, d))
    {
        STHROWF("Invalid date (%u-%u-%u)", y, m, d);
    }
    // Attempt to generate the specified date string
    CSStr ptr = FmtStr(_SC("%u-%u-%u 00:00:00"), y, m, d);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_text(m_Handle->mPtr, m_Index, ptr, -1, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "date", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetTime(const Time & value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, value.GetTimestamp().GetSecondsI());
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "time", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetTimeEx(SQInteger hour, SQInteger minute, SQInteger second)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Convert the specified values within the proper ranges
    const uint8_t h = ConvTo< uint8_t >::From(hour)
                , m = ConvTo< uint8_t >::From(minute)
                , s = ConvTo< uint8_t >::From(second);
    // Is the specified hour within range?
    if (h >= 24)
    {
        STHROWF("Hour value is out of range: %u >= 24", h);
    }
    // Is the specified minute within range?
    else if (m >= 60)
    {
        STHROWF("Minute value is out of range: %u >= 60", m);
    }
    // Is the specified second within range?
    else if (s >= 60)
    {
        STHROWF("Second value is out of range: %u >= 60", s);
    }
    // Calculate the number of seconds in the specified time and bind the resulted value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, (h * (60 * 60)) + (m * 60) + s);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "time", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetDatetime(const Datetime & value)
{
    SetDatetimeEx(value.GetYear(), value.GetMonth(), value.GetDay(), value.GetHour(), value.GetMinute(), value.GetSecond());
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetDatetimeEx(SQInteger year, SQInteger month, SQInteger day, SQInteger hour, SQInteger minute, SQInteger second)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Convert the specified values within the proper ranges
    const uint16_t y = ConvTo< uint16_t >::From(year);
    const uint8_t mo = ConvTo< uint8_t >::From(month)
                , d = ConvTo< uint8_t >::From(day)
                , h = ConvTo< uint8_t >::From(hour)
                , mi = ConvTo< uint8_t >::From(minute)
                , s = ConvTo< uint8_t >::From(second);
    // Validate the specified date
    if (!Chrono::ValidDate(y, mo, d))
    {
        STHROWF("Invalid date (%u-%u-%u)", y, mo, d);
    }
    // Is the specified hour within range?
    else if (h >= 24)
    {
        STHROWF("Hour value is out of range: %u >= 24", h);
    }
    // Is the specified minute within range?
    else if (mi >= 60)
    {
        STHROWF("Minute value is out of range: %u >= 60", mi);
    }
    // Is the specified second within range?
    else if (s >= 60)
    {
        STHROWF("Second value is out of range: %u >= 60", s);
    }
    // Attempt to generate the specified date string
    CSStr ptr = FmtStr(_SC("%04u-%02u-%02u %02u:%02u:%02u"), y, mo, d, h, mi, s);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_text(m_Handle->mPtr, m_Index, ptr, -1, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "date-time", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetNow()
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index,
                                            static_cast< Int32 >(std::time(nullptr)));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "time-stamp", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void SQLiteParameter::SetNull()
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_null(m_Handle->mPtr, m_Index);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "null", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SQLiteColumn::Validate(CCStr file, Int32 line) const
{
    // Are we pointing to a valid index?
    if (m_Index < 0)
    {
        SqThrowF("Invalid column index: %d < 0 =>[%s:%d]", m_Index, file, line);
    }
    // Do we have a valid statement handle?
    else if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference =>[%s:%d]", file, line);
    }
}
#else
void SQLiteColumn::Validate() const
{
    // Are we pointing to a valid index?
    if (m_Index < 0)
    {
        SqThrowF("Invalid column index: %d < 0", m_Index);
    }
    // Do we have a valid statement handle?
    else if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SQLiteColumn::ValidateCreated(CCStr file, Int32 line) const
{
    // Are we pointing to a valid index?
    if (m_Index < 0)
    {
        SqThrowF("Invalid column index: %d < 0 =>[%s:%d]", m_Index, file, line);
    }
    else if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference =>[%s:%d]", file, line);
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid SQLite statement =>[%s:%d]", file, line);
    }
}
#else
void SQLiteColumn::ValidateCreated() const
{
    // Are we pointing to a valid index?
    if (m_Index < 0)
    {
        SqThrowF("Invalid column index: %d < 0", m_Index);
    }
    else if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference");
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid SQLite statement");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const StmtRef & SQLiteColumn::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const StmtRef & SQLiteColumn::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const StmtRef & SQLiteColumn::GetCreated(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    return m_Handle;
}
#else
const StmtRef & SQLiteColumn::GetCreated() const
{
    ValidateCreated();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SQLiteColumn::ValidateColumn(Int32 idx, CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    // Is the specified index in range?
    if (!m_Handle->CheckColumn(idx))
    {
        SqThrowF("Column index is out of range: %d:%d =>[%s:%d]", idx, m_Handle->mColumns,
                    file, line);
    }
}
#else
void SQLiteColumn::ValidateColumn(Int32 idx) const
{
    ValidateCreated();
    // Is the specified index in range?
    if (!m_Handle->CheckColumn(idx))
    {
        SqThrowF("Column index is out of range: %d:%d", idx, m_Handle->mColumns);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SQLiteColumn::ValidateRow(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    // Do we have any rows available?
    if (!m_Handle->mGood)
    {
        STHROWF("No row available =>[%s:%d]", file, line);
    }
}
#else
void SQLiteColumn::ValidateRow() const
{
    ValidateCreated();
    // Do we have any rows available?
    if (!m_Handle->mGood)
    {
        STHROWF("No row available");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
void SQLiteColumn::SetIndex(const Object & column)
{
    // Where the index will be extracted
    Int32 idx = -1;
    // Grab the object virtual machine
    HSQUIRRELVM vm = column.GetVM();
    // Remember the current stack size
    const StackGuard sg(vm);
    // Push the specified object onto the stack
    Var< const Object & >::push(vm, column);
    // Identify the type of column was given
    switch  (column.GetType())
    {
        // Is this a string value?
        case OT_STRING:
        {
            // Obtain the object from the stack as a string
            StackStrF val(vm, -1);
            // Validate the result
            if (SQ_FAILED(val.Proc(false)))
            {
                STHROWF("%s", LastErrorString(vm).c_str());
            }
            // Is the obtained string empty?
            else if (val.mLen <= 0)
            {
                STHROWF("Cannot use an empty column name");
            }
            // Attempt to find a column with the specified name
            idx = m_Handle->GetColumnIndex(val.mPtr);
        } break;
        // Is this an integer value? (or at least can be easily converted to one)
        case OT_INTEGER:
        case OT_FLOAT:
        case OT_BOOL:
        {
            idx = ConvTo< Int32 >::From(PopStackInteger(vm, -1));
        } break;
        // Is this an instance that we can extract either a string or integer from it?
        case OT_INSTANCE:
        {
            // Obtain the object from the stack as a string
            StackStrF val(vm, -1);
            // Validate the result
            if (SQ_FAILED(val.Proc(false)))
            {
                STHROWF("%s", LastErrorString(vm).c_str());
            }
            // Is the obtained string empty?
            else if (val.mLen <= 0)
            {
                STHROWF("Cannot use an empty column name");
            }
            // Check if this value is made only of digits
            else if (IsDigitsOnly(val.mPtr))
            {
                idx = ConvNum< Int32 >::FromStr(val.mPtr);
            }
            // Attempt to find a column with the specified name
            else
            {
                idx = m_Handle->GetColumnIndex(val.mPtr);
            }
        } break;
        // We don't recognize this kind of value!
        default: STHROWF("Unknown column index of type (%s)", SqTypeName(column.GetType()));
    }
    // Assign the index with a failsafe to invalid on error
    AutoAssign< Int32 > aa(m_Index, -1, idx);
    // Validate the obtained column index
    SQMOD_VALIDATE_COLUMN(*this, idx);
    // Don't fall back to the invalid index anymore
    aa.Set(idx);
}

// ------------------------------------------------------------------------------------------------
Object SQLiteColumn::GetStatement() const
{
    return GetStatementObj(m_Handle);
}

// ------------------------------------------------------------------------------------------------
Object SQLiteColumn::GetConnection() const
{
    return GetConnectionObj(SQMOD_GET_VALID(*this)->mConn);
}

// ------------------------------------------------------------------------------------------------
bool SQLiteColumn::IsNull() const
{
    return (sqlite3_column_type(SQMOD_GET_CREATED(*this)->mPtr, m_Index) == SQLITE_NULL);
}

// ------------------------------------------------------------------------------------------------
CSStr SQLiteColumn::GetName() const
{
    return sqlite3_column_name(SQMOD_GET_CREATED(*this)->mPtr, m_Index);
}

// ------------------------------------------------------------------------------------------------
CSStr SQLiteColumn::GetOriginName() const
{
#ifdef SQLITE_ENABLE_COLUMN_METADATA
    return sqlite3_column_origin_name(SQMOD_GET_CREATED(*this)->mPtr, m_Index);
#else
    STHROWF("The module was compiled without this feature");
    // Request failed
    return _SC("");
#endif
}

// ------------------------------------------------------------------------------------------------
Int32 SQLiteColumn::GetType() const
{
    return sqlite3_column_type(SQMOD_GET_CREATED(*this)->mPtr, m_Index);
}

// ------------------------------------------------------------------------------------------------
Int32 SQLiteColumn::GetBytes() const
{
    return sqlite3_column_bytes(SQMOD_GET_CREATED(*this)->mPtr, m_Index);
}

// ------------------------------------------------------------------------------------------------
Object SQLiteColumn::GetValue() const
{
    SQMOD_VALIDATE_ROW(*this);
    // Obtain the initial stack size
    const StackGuard sg;
    // Identify which type of value must be pushed on the stack
    switch (sqlite3_column_type(m_Handle->mPtr, m_Index))
    {
        // Is this a null value?
        case SQLITE_NULL:
        {
            sq_pushnull(DefaultVM::Get());
        } break;
        // Is this an integer?
        case SQLITE_INTEGER:
        {
            sq_pushinteger(DefaultVM::Get(), sqlite3_column_integer(m_Handle->mPtr, m_Index));
        } break;
        // Is this a floating point?
        case SQLITE_FLOAT:
        {
            sq_pushfloat(DefaultVM::Get(),
                            ConvTo< SQFloat >::From(sqlite3_column_double(m_Handle->mPtr, m_Index)));
        } break;
        // Is this a string?
        case SQLITE_TEXT:
        {
            sq_pushstring(DefaultVM::Get(),
                            reinterpret_cast< CSStr >(sqlite3_column_text(m_Handle->mPtr, m_Index)),
                            sqlite3_column_bytes(m_Handle->mPtr, m_Index));
        } break;
        // Is this raw data?
        case SQLITE_BLOB:
        {
            // Retrieve the size of the blob that must be allocated
            const Int32 size = sqlite3_column_bytes(m_Handle->mPtr, m_Index);
            // Retrieve the the actual blob data that must be returned
            auto data = reinterpret_cast< CCStr >(sqlite3_column_blob(m_Handle->mPtr, m_Index));
            // Attempt to create a buffer with the blob data on the stack
            Var< const SqBuffer & >::push(DefaultVM::Get(), SqBuffer(data, size, 0));
        } break;
        // Unknown type
        default: STHROWF("Unknown value to fetch at index: %d", m_Index);
    }
    // Obtain the object with the value from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
Object SQLiteColumn::GetNumber() const
{
    SQMOD_VALIDATE_ROW(*this);
    // Obtain the initial stack size
    const StackGuard sg;
    // Identify which type of value must be pushed on the stack
    switch (sqlite3_column_type(m_Handle->mPtr, m_Index))
    {
        // Is this a null value?
        case SQLITE_NULL:
        {
            sq_pushinteger(DefaultVM::Get(), 0);
        } break;
        // Is this an integer?
        case SQLITE_INTEGER:
        {
            sq_pushinteger(DefaultVM::Get(), sqlite3_column_integer(m_Handle->mPtr, m_Index));
        } break;
        // Is this a floating point?
        case SQLITE_FLOAT:
        {
            sq_pushfloat(DefaultVM::Get(),
                            ConvTo< SQFloat >::From(sqlite3_column_double(m_Handle->mPtr, m_Index)));
        } break;
        // Is this a string?
        case SQLITE_TEXT:
        {
            auto str = reinterpret_cast< CSStr >(sqlite3_column_text(m_Handle->mPtr, m_Index));
            // Is there even a string to parse?
            if (!str || *str == '\0')
            {
                sq_pushinteger(DefaultVM::Get(), 0);
            }
            // Can we treat this string as a float?
            else if (!std::strchr(str, '.'))
            {
                sq_pushfloat(DefaultVM::Get(),
                                ConvTo< SQFloat >::From(std::strtod(str, nullptr)));
            }
            else
            {
                sq_pushinteger(DefaultVM::Get(),
                                ConvTo< SQInteger >::From(std::strtoll(str, nullptr, 10)));

            }
        } break;
        // Unknown type
        default: STHROWF("Unknown number to fetch at index: %d", m_Index);
    }
    // Obtain the object with the value from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
SQInteger SQLiteColumn::GetInteger() const
{
    SQMOD_VALIDATE_ROW(*this);
    // Return the requested information
    return sqlite3_column_integer(m_Handle->mPtr, m_Index);
}

// ------------------------------------------------------------------------------------------------
SQFloat SQLiteColumn::GetFloat() const
{
    SQMOD_VALIDATE_ROW(*this);
    // Return the requested information
    return ConvTo< SQFloat >::From(sqlite3_column_double(m_Handle->mPtr, m_Index));
}

// ------------------------------------------------------------------------------------------------
Object SQLiteColumn::GetLong() const
{
    SQMOD_VALIDATE_ROW(*this);
    // Return the requested information
    return Object(new SLongInt(sqlite3_column_int64(m_Handle->mPtr, m_Index)));
}

// ------------------------------------------------------------------------------------------------
Object SQLiteColumn::GetString() const
{
    SQMOD_VALIDATE_ROW(*this);
    // Obtain the initial stack size
    const StackGuard sg;
    // Push the column text on the stack
    sq_pushstring(DefaultVM::Get(), reinterpret_cast< CSStr >(sqlite3_column_text(m_Handle->mPtr, m_Index)),
                            sqlite3_column_bytes(m_Handle->mPtr, m_Index));
    // Get the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
bool SQLiteColumn::GetBoolean() const
{
    SQMOD_VALIDATE_ROW(*this);
    // Return the requested information
    return sqlite3_column_int(m_Handle->mPtr, m_Index) > 0;
}

// ------------------------------------------------------------------------------------------------
SQChar SQLiteColumn::GetChar() const
{
    SQMOD_VALIDATE_ROW(*this);
    // Return the requested information
    return (SQChar)sqlite3_column_int(m_Handle->mPtr, m_Index);
}

// ------------------------------------------------------------------------------------------------
Object SQLiteColumn::GetBuffer() const
{
    SQMOD_VALIDATE_ROW(*this);
    // Remember the current stack size
    const StackGuard sg;
    // Retrieve the size of the blob that must be allocated
    const Int32 size = sqlite3_column_bytes(m_Handle->mPtr, m_Index);
    // Retrieve the the actual blob data that must be returned
    auto data = reinterpret_cast< CCStr >(sqlite3_column_blob(m_Handle->mPtr, m_Index));
    // Attempt to create a buffer with the blob data on the stack
    Var< const SqBuffer & >::push(DefaultVM::Get(), SqBuffer(data, size, 0));
    // Get the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
Object SQLiteColumn::GetBlob() const
{
    SQMOD_VALIDATE_ROW(*this);
    // Obtain the initial stack size
    const StackGuard sg;
    // Obtain the size of the data
    const Int32 sz = sqlite3_column_bytes(m_Handle->mPtr, m_Index);
    // Allocate a blob of the same size
    SQUserPointer p = sqstd_createblob(DefaultVM::Get(), sz);
    // Obtain a pointer to the data
    const void * b = sqlite3_column_blob(m_Handle->mPtr, m_Index);
    // Could the memory blob be allocated?
    if (!p)
    {
        STHROWF("Unable to allocate space for column blob value");
    }
    // Is there any data to read?
    else if (!b)
    {
        // Pop the memory blob from the stack
        sq_pop(DefaultVM::Get(), 1);
        // Push a null value instead
        sq_pushnull(DefaultVM::Get());
    }
    // Copy the data into the memory blob
    else
    {
        std::memcpy(p, b, sz);
    }
    // Get the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SQLiteStatement::Validate(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference =>[%s:%d]", file, line);
    }
}
#else
void SQLiteStatement::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SQLiteStatement::ValidateCreated(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference =>[%s:%d]", file, line);
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid SQLite statement =>[%s:%d]", file, line);
    }
}
#else
void SQLiteStatement::ValidateCreated() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference");
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid SQLite statement");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const StmtRef & SQLiteStatement::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const StmtRef & SQLiteStatement::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const StmtRef & SQLiteStatement::GetCreated(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    return m_Handle;
}
#else
const StmtRef & SQLiteStatement::GetCreated() const
{
    ValidateCreated();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SQLiteStatement::ValidateColumn(Int32 idx, CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    // Is the specified index in range?
    if (!m_Handle->CheckColumn(idx))
    {
        SqThrowF("Column index is out of range: %d:%d =>[%s:%d]", idx, m_Handle->mColumns,
                    file, line);
    }
}
#else
void SQLiteStatement::ValidateColumn(Int32 idx) const
{
    ValidateCreated();
    // Is the specified index in range?
    if (!m_Handle->CheckColumn(idx))
    {
        SqThrowF("Column index is out of range: %d:%d", idx, m_Handle->mColumns);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SQLiteStatement::ValidateParam(Int32 idx, CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    // Is the specified index in range?
    if (!m_Handle->CheckParameter(idx))
    {
        SqThrowF("Parameter index is out of range: %d:%d =>[%s:%d]", idx, m_Handle->mParameters,
                    file, line);
    }
}
#else
void SQLiteStatement::ValidateParam(Int32 idx) const
{
    ValidateCreated();
    // Is the specified index in range?
    if (!m_Handle->CheckParameter(idx))
    {
        SqThrowF("Parameter index is out of range: %d:%d", idx, m_Handle->mParameters);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SQLiteStatement::ValidateRow(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    // Do we have any rows available?
    if (!m_Handle->mGood)
    {
        SqThrowF("No row available =>[%s:%d]", file, line);
    }
}
#else
void SQLiteStatement::ValidateRow() const
{
    ValidateCreated();
    // Do we have any rows available?
    if (!m_Handle->mGood)
    {
        SqThrowF("No row available");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
SQLiteStatement::SQLiteStatement(const SQLiteConnection & connection, StackStrF & query)
    : m_Handle(new SQLiteStmtHnd(connection.GetHandle()))
{
    SQMOD_GET_VALID(*this)->Create(query.mPtr, query.mLen);
}

// ------------------------------------------------------------------------------------------------
Object SQLiteStatement::GetConnection() const
{
    return Object(new SQLiteConnection(SQMOD_GET_VALID(*this)->mConn));
}

// ------------------------------------------------------------------------------------------------
SQLiteStatement & SQLiteStatement::Reset()
{
    SQMOD_VALIDATE_CREATED(*this);
    // Specify that we don't have a row available and we haven't finished stepping
    m_Handle->mGood = false;
    m_Handle->mDone = false;
    // Attempt to reset the statement to it's initial state
    m_Handle->mStatus = sqlite3_reset(m_Handle->mPtr);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF("Unable to reset statement [%s]", m_Handle->ErrStr());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
SQLiteStatement & SQLiteStatement::Clear()
{
    SQMOD_VALIDATE_CREATED(*this);
    // Specify that we don't have a row available and we haven't finished stepping
    m_Handle->mGood = false;
    m_Handle->mDone = false;
    // Attempt to clear the statement
    m_Handle->mStatus = sqlite3_clear_bindings(m_Handle->mPtr);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF("Unable to clear statement [%s]", m_Handle->ErrStr());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Int32 SQLiteStatement::Exec()
{
    SQMOD_VALIDATE_CREATED(*this);
    // Did we reset first?
    if (m_Handle->mDone)
    {
        STHROWF("Executed without resetting first");
    }
    // Attempt to step the statement
    m_Handle->mStatus = sqlite3_step(m_Handle->mPtr);
    // Have we finished stepping?
    if (m_Handle->mStatus == SQLITE_DONE)
    {
        // Specify that we don't have row available and we finished stepping
        m_Handle->mGood = false;
        m_Handle->mDone = true;
        // Return the changes made by this statement
        return sqlite3_changes(m_Handle->mConn->mPtr);
    }
    // Specify that we don't have any row and we haven't finished stepping
    m_Handle->mGood = false;
    m_Handle->mDone = false;
    // Inspect the result
    switch (m_Handle->mStatus)
    {
        // We don't expect any rows to be returned in this case!
        case SQLITE_ROW:    STHROWF("Results were found");
        // fall through
        case SQLITE_BUSY:   STHROWF("Database was busy");
        // fall through
        case SQLITE_ERROR:  STHROWF("Runtime error occurred");
        // fall through
        case SQLITE_MISUSE: STHROWF("Statement misuse");
        // fall through
        default:            STHROWF("Unknown failure [%s]", m_Handle->ErrStr());
    }
    // Operation failed (shouldn't reach this point!)
    return -1;
}

// ------------------------------------------------------------------------------------------------
bool SQLiteStatement::Step()
{
    SQMOD_VALIDATE_CREATED(*this);
    // Did we reset first?
    if (m_Handle->mDone)
    {
        STHROWF("Stepped without resetting first");
    }
    // Attempt to step the statement
    m_Handle->mStatus = sqlite3_step(m_Handle->mPtr);
    // Do we have a row available?
    if (m_Handle->mStatus == SQLITE_ROW)
    {
        // Specify that we have a row available
        return (m_Handle->mGood = true);
    }
    // Have we finished stepping?
    else if (m_Handle->mStatus == SQLITE_DONE)
    {
        // Specify that we finished stepping
        m_Handle->mDone = true;
        // Specify that we don't have a row available
        return (m_Handle->mGood = false);
    }
    // Specify that we don't have any row and we haven't finished stepping
    m_Handle->mGood = false;
    m_Handle->mDone = false;
    // Inspect the result
    switch (m_Handle->mStatus)
    {
        case SQLITE_BUSY:   STHROWF("Database was busy");
        // fall through
        case SQLITE_ERROR:  STHROWF("Runtime error occurred");
        // fall through
        case SQLITE_MISUSE: STHROWF("Statement misuse");
        // fall through
        default:            STHROWF("Unknown failure [%s]", m_Handle->ErrStr());
    }
    // Operation failed (shouldn't reach this point!)
    return false;
}

// ------------------------------------------------------------------------------------------------
SQLiteStatement & SQLiteStatement::SetArray(Int32 idx, const Array & arr)
{
    // Obtain a script iterator
    Array::iterator itr;
    // Create a parameter instance to bind the values
    SQLiteParameter param(m_Handle);
    // Process each element until _next returns null
    while (idx <= m_Handle->mParameters && arr.Next(itr))
    {
        // Update the parameter index
        param.SetIndex(idx++);
        // Bind it to the current index
        param.SetValue(Object(itr.getValue()));
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
SQLiteStatement & SQLiteStatement::SetTable(const Table & tbl)
{
    // Is there anything to bind?
    if (tbl.GetSize() <= 0)
    {
        return *this; // Nothing to bind!
    }
    // Obtain a table iterator
    Table::iterator itr;
    // Create a parameter instance to bind the values
    SQLiteParameter param(m_Handle);
    // Process each element until _next returns null
    while (tbl.Next(itr))
    {
        // Update the parameter index
        param.SetIndex(itr.getName());
        // Bind it to the current index
        param.SetValue(Object(itr.getValue()));
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Array SQLiteStatement::GetArray(Int32 min, Int32 max) const
{
    SQMOD_VALIDATE_ROW(*this);
    // Is the specified minimum index valid?
    if (min < 0)
    {
        STHROWF("Minimum is bellow zero: %d", min);
    }
    // Is the minimum actually the minimum?
    else if (min > max)
    {
        STHROWF("Minimum is higher than maximum: %d > %d", min, max);
    }
    // Is the minimum in range?
    else if (!m_Handle->CheckColumn(min))
    {
        STHROWF("Minimum is out of range: %d:%d", min, m_Handle->mColumns);
    }
    // Is the maximum in range?
    else if (!m_Handle->CheckColumn(max))
    {
        STHROWF("Maximum is out of range: %d:%d", max, m_Handle->mColumns);
    }
    // Allocate an array large enough to hold the values from selected columns
    Array arr(DefaultVM::Get(), max-min);
    // Create a column instance to retrieve the values
    SQLiteColumn column(m_Handle);
    // Array element counter
    Int32 elem = 0;
    // Process the range of selected columns
    while (min <= max)
    {
        // Update the column index
        column.SetIndex(min++);
        // Retrieve the column value and bind it to the array
        arr.SetValue(elem++, column.GetValue());
    }
    // Return the resulted array
    return arr;
}

// ------------------------------------------------------------------------------------------------
Table SQLiteStatement::GetTable(Int32 min, Int32 max) const
{
    SQMOD_VALIDATE_ROW(*this);
    // Is the specified minimum index valid?
    if (min < 0)
    {
        STHROWF("Minimum is bellow zero: %d", min);
    }
    // Is the minimum actually the minimum?
    else if (min > max)
    {
        STHROWF("Minimum is higher than maximum: %d > %d", min, max);
    }
    // Is the minimum in range>
    else if (!m_Handle->CheckColumn(min))
    {
        STHROWF("Minimum is out of range: %d:%d", min, m_Handle->mColumns);
    }
    // Is the maximum in range?
    else if (!m_Handle->CheckColumn(max))
    {
        STHROWF("Maximum is out of range: %d:%d", max, m_Handle->mColumns);
    }
    // Create a table to hold the selected column values
    Table tbl(DefaultVM::Get());
    // Create a column instance to retrieve the values
    SQLiteColumn column(m_Handle);
    // Process the range of selected columns
    while (min <= max)
    {
        // Attempt to obtain the column name
        CSStr name = sqlite3_column_name(m_Handle->mPtr, min);
        // Validate the obtained name
        if (!name)
        {
            STHROWF("Unable to retrieve name of column (%d)", min);
        }
        // Update the column index
        column.SetIndex(min++);
        // Retrieve the column value and bind it to the table
        tbl.SetValue(name, column.GetValue());
    }
    // Return the resulted table
    return tbl;
}

// ------------------------------------------------------------------------------------------------
SQLiteTransaction::SQLiteTransaction(const SQLiteConnection & db)
    : SQLiteTransaction(db.GetHandle())
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
SQLiteTransaction::SQLiteTransaction(ConnRef  db)
    : m_Handle(std::move(db)), m_Committed(false)
{
    // Was the specified database connection valid?
    if (!m_Handle)
    {
        STHROWF("Invalid connection handle");
    }
    // Attempt to begin transaction
    m_Handle->mStatus = sqlite3_exec(m_Handle->mPtr, "BEGIN", nullptr, nullptr, nullptr);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF("Unable to begin transaction [%s]", m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
SQLiteTransaction::~SQLiteTransaction()
{
    // Was this transaction successfully committed?
    if (m_Committed)
    {
        return; // We're done here!
    }
    // Attempt to roll back changes because this failed to commit
    m_Handle->mStatus = sqlite3_exec(m_Handle->mPtr, "ROLLBACK", nullptr, nullptr, nullptr);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        // We cannot throw exceptions in destructor
        LogErr("Unable to rollback transaction [%s]", m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
bool SQLiteTransaction::Commit()
{
    // We shouldn't even be here if there wasn't a valid connection but let's be sure
    if (!m_Handle)
    {
        STHROWF("Invalid database connection");
    }
    // Was this transaction already committed?
    else if (m_Committed)
    {
        STHROWF("Transaction was already committed");
    }
    // Attempt to commit the change during this transaction
    m_Handle->mStatus = sqlite3_exec(m_Handle->mPtr, "COMMIT", nullptr, nullptr, nullptr);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF("Unable to commit transaction [%s]", m_Handle->ErrMsg());
    }
    else
    {
        m_Committed = true; // Everything was committed successfully
    }
    // Return the result
    return m_Committed;
}

// ================================================================================================
void Register_SQLite(HSQUIRRELVM vm)
{
    Enumeration e(vm);

    for (auto n : g_MainEnum)
    {
        e.Const(n.Name, n.Value);
    }

    ConstTable(vm).Enum(_SC("SQLiteOpt"), e);

    Table sqlns(vm);

	sqlns.Func(_SC("IsQueryEmpty"), &IsQueryEmpty)
	    .Func(_SC("GetErrStr"), &GetErrStr)
	    .Func(_SC("SetSoftHeapLimit"), &SetSoftHeapLimit)
	    .Func(_SC("ReleaseMemory"), &ReleaseMemory)
	    .Func(_SC("MemoryUsage"), &GetMemoryUsage)
	    .Func(_SC("MemoryHighwaterMark"), &GetMemoryHighwaterMark)
	    .Func(_SC("ArrayToQueryColumns"), &ArrayToQueryColumns)
	    .Func(_SC("TableToQueryColumns"), &TableToQueryColumns)
	    .FmtFunc(_SC("EscapeString"), &EscapeString)
	    .FmtFunc(_SC("EscapeStringEx"), &EscapeStringEx)
	    .FmtFunc(_SC("Escape"), &EscapeString)
	    .FmtFunc(_SC("EscapeEx"), &EscapeStringEx);

    sqlns.Bind(_SC("Connection"),
        Class< SQLiteConnection >(sqlns.GetVM(), SQLiteConnectionTypename::Str)
        // Constructors
        .Ctor()
        .Ctor< StackStrF & >()
        .Ctor< StackStrF &, Int32 >()
        .Ctor< StackStrF &, Int32, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SQLiteConnectionTypename::Fn)
        .Func(_SC("_tostring"), &SQLiteConnection::ToString)
        // Properties
        .Prop(_SC("IsValid"), &SQLiteConnection::IsValid)
        .Prop(_SC("Connected"), &SQLiteConnection::IsConnected)
        .Prop(_SC("References"), &SQLiteConnection::GetRefCount)
        .Prop(_SC("Status"), &SQLiteConnection::GetStatus)
        .Prop(_SC("Flags"), &SQLiteConnection::GetFlags)
        .Prop(_SC("Name"), &SQLiteConnection::GetName)
        .Prop(_SC("VFS"), &SQLiteConnection::GetVFS)
        .Prop(_SC("ErrCode"), &SQLiteConnection::GetErrorCode)
        .Prop(_SC("ExErrCode"), &SQLiteConnection::GetExtendedErrorCode)
        .Prop(_SC("ExtendedErrCode"), &SQLiteConnection::GetExtendedErrorCode)
        .Prop(_SC("ErrStr"), &SQLiteConnection::GetErrStr)
        .Prop(_SC("ErrMsg"), &SQLiteConnection::GetErrMsg)
        .Prop(_SC("ReadOnly"), &SQLiteConnection::IsReadOnly)
        .Prop(_SC("Autocommit"), &SQLiteConnection::GetAutoCommit)
        .Prop(_SC("LastInsertRowId"), &SQLiteConnection::GetLastInsertRowID)
        .Prop(_SC("Changes"), &SQLiteConnection::GetChanges)
        .Prop(_SC("TotalChanges"), &SQLiteConnection::GetTotalChanges)
        .Prop(_SC("Trace"), &SQLiteConnection::GetTracing, &SQLiteConnection::SetTracing)
        .Prop(_SC("Profile"), &SQLiteConnection::GetProfiling, &SQLiteConnection::SetProfiling)
        .Prop(_SC("QueueSize"), &SQLiteConnection::QueueSize)
        // Member Methods
        .Func(_SC("Release"), &SQLiteConnection::Release)
        .FmtFunc(_SC("Exec"), &SQLiteConnection::Exec)
        .FmtFunc(_SC("Queue"), &SQLiteConnection::Queue)
        .FmtFunc(_SC("Query"), &SQLiteConnection::Query)
        .FmtFunc(_SC("TableExists"), &SQLiteConnection::TableExists)
        .Func(_SC("InterruptOperation"), &SQLiteConnection::InterruptOperation)
        .Func(_SC("SetBusyTimeout"), &SQLiteConnection::SetBusyTimeout)
        .Func(_SC("ReleaseMemory"), &SQLiteConnection::ReleaseMemory)
        .Func(_SC("ReserveQueue"), &SQLiteConnection::ReserveQueue)
        .Func(_SC("CompactQueue"), &SQLiteConnection::CompactQueue)
        .Func(_SC("ClearQueue"), &SQLiteConnection::ClearQueue)
        .Func(_SC("PopQueue"), &SQLiteConnection::PopQueue)
        // Member Overloads
        .Overload< void (SQLiteConnection::*)(StackStrF &) >(_SC("Open"), &SQLiteConnection::Open)
        .Overload< void (SQLiteConnection::*)(StackStrF &, Int32) >(_SC("Open"), &SQLiteConnection::Open)
        .Overload< void (SQLiteConnection::*)(StackStrF &, Int32, StackStrF &) >(_SC("Open"), &SQLiteConnection::Open)
        .Overload< Int32 (SQLiteConnection::*)(Int32) >(_SC("GetInfo"), &SQLiteConnection::GetInfo)
        .Overload< Int32 (SQLiteConnection::*)(Int32, bool) >(_SC("GetInfo"), &SQLiteConnection::GetInfo)
        .Overload< Int32 (SQLiteConnection::*)(Int32, bool, bool) >(_SC("GetInfo"), &SQLiteConnection::GetInfo)
        .Overload< Int32 (SQLiteConnection::*)(void) >(_SC("Flush"), &SQLiteConnection::Flush)
        .Overload< Int32 (SQLiteConnection::*)(SQInteger) >(_SC("Flush"), &SQLiteConnection::Flush)
        .Overload< Int32 (SQLiteConnection::*)(Object &, Function &) >(_SC("Flush"), &SQLiteConnection::Flush)
        .Overload< Int32 (SQLiteConnection::*)(SQInteger, Object &, Function &) >(_SC("Flush"), &SQLiteConnection::Flush)
    );

    sqlns.Bind(_SC("Parameter"),
        Class< SQLiteParameter >(sqlns.GetVM(), SQLiteParameterTypename::Str)
        // Constructors
        .Ctor()
        .Ctor< const SQLiteParameter & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SQLiteParameterTypename::Fn)
        .Func(_SC("_tostring"), &SQLiteParameter::ToString)
        // Properties
        .Prop(_SC("IsValid"), &SQLiteParameter::IsValid)
        .Prop(_SC("References"), &SQLiteParameter::GetRefCount)
        .Prop(_SC("Index"), &SQLiteParameter::GetIndex)
        .Prop(_SC("Statement"), &SQLiteParameter::GetStatement)
        .Prop(_SC("Connection"), &SQLiteParameter::GetConnection)
        .Prop(_SC("References"), &SQLiteParameter::GetRefCount)
        .Prop(_SC("Name"), &SQLiteParameter::GetName)
        // Member Methods
        .Func(_SC("Release"), &SQLiteParameter::Release)
        .Func(_SC("SetValue"), &SQLiteParameter::SetValue)
        .Func(_SC("SetBool"), &SQLiteParameter::SetBool)
        .Func(_SC("SetChar"), &SQLiteParameter::SetChar)
        .Func(_SC("SetInteger"), &SQLiteParameter::SetInteger)
        .Func(_SC("SetInt8"), &SQLiteParameter::SetInt8)
        .Func(_SC("SetUint8"), &SQLiteParameter::SetUint8)
        .Func(_SC("SetInt16"), &SQLiteParameter::SetInt16)
        .Func(_SC("SetUint16"), &SQLiteParameter::SetUint16)
        .Func(_SC("SetInt32"), &SQLiteParameter::SetInt32)
        .Func(_SC("SetUint32"), &SQLiteParameter::SetUint32)
        .Func(_SC("SetInt64"), &SQLiteParameter::SetInt64)
        .Func(_SC("SetUint64"), &SQLiteParameter::SetUint64)
        .Func(_SC("SetFloat"), &SQLiteParameter::SetFloat)
        .Func(_SC("SetFloat32"), &SQLiteParameter::SetFloat32)
        .Func(_SC("SetFloat64"), &SQLiteParameter::SetFloat64)
        .FmtFunc(_SC("SetString"), &SQLiteParameter::SetString)
        .Func(_SC("SetZeroBlob"), &SQLiteParameter::SetZeroBlob)
        .Func(_SC("SetBlob"), &SQLiteParameter::SetBlob)
        .Func(_SC("SetData"), &SQLiteParameter::SetData)
        .Func(_SC("SetDataEx"), &SQLiteParameter::SetDataEx)
        .Func(_SC("SetDate"), &SQLiteParameter::SetDate)
        .Func(_SC("SetDateEx"), &SQLiteParameter::SetDateEx)
        .Func(_SC("SetTime"), &SQLiteParameter::SetTime)
        .Func(_SC("SetTimeEx"), &SQLiteParameter::SetTimeEx)
        .Func(_SC("SetDatetime"), &SQLiteParameter::SetDatetime)
        .Func(_SC("SetDatetimeEx"), &SQLiteParameter::SetDatetimeEx)
        .Func(_SC("SetNow"), &SQLiteParameter::SetNow)
        .Func(_SC("SetNull"), &SQLiteParameter::SetNull)
    );

    sqlns.Bind(_SC("Column"),
        Class< SQLiteColumn >(sqlns.GetVM(), SQLiteColumnTypename::Str)
        // Constructors
        .Ctor()
        .Ctor< const SQLiteColumn & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SQLiteColumnTypename::Fn)
        .Func(_SC("_tostring"), &SQLiteColumn::ToString)
        // Properties
        .Prop(_SC("IsValid"), &SQLiteColumn::IsValid)
        .Prop(_SC("References"), &SQLiteColumn::GetRefCount)
        .Prop(_SC("Index"), &SQLiteColumn::GetIndex)
        .Prop(_SC("Statement"), &SQLiteColumn::GetStatement)
        .Prop(_SC("Connection"), &SQLiteColumn::GetConnection)
        .Prop(_SC("IsNull"), &SQLiteColumn::IsNull)
        .Prop(_SC("Name"), &SQLiteColumn::GetName)
        .Prop(_SC("OriginName"), &SQLiteColumn::GetOriginName)
        .Prop(_SC("Type"), &SQLiteColumn::GetType)
        .Prop(_SC("Bytes"), &SQLiteColumn::GetBytes)
        .Prop(_SC("Value"), &SQLiteColumn::GetValue)
        .Prop(_SC("Number"), &SQLiteColumn::GetNumber)
        .Prop(_SC("Integer"), &SQLiteColumn::GetInteger)
        .Prop(_SC("Float"), &SQLiteColumn::GetFloat)
        .Prop(_SC("Long"), &SQLiteColumn::GetLong)
        .Prop(_SC("String"), &SQLiteColumn::GetString)
        .Prop(_SC("Boolean"), &SQLiteColumn::GetBoolean)
        .Prop(_SC("Char"), &SQLiteColumn::GetChar)
        .Prop(_SC("Buffer"), &SQLiteColumn::GetBuffer)
        .Prop(_SC("Blob"), &SQLiteColumn::GetBlob)
        // Member Methods
        .Func(_SC("Release"), &SQLiteColumn::Release)
    );

    sqlns.Bind(_SC("Statement"),
        Class< SQLiteStatement >(sqlns.GetVM(), SQLiteStatementTypename::Str)
        // Constructors
        .Ctor()
        .Ctor< const SQLiteStatement & >()
        .Ctor< const SQLiteConnection &, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SQLiteStatementTypename::Fn)
        .Func(_SC("_tostring"), &SQLiteStatement::ToString)
        // Properties
        .Prop(_SC("IsValid"), &SQLiteStatement::IsValid)
        .Prop(_SC("Prepared"), &SQLiteStatement::IsPrepared)
        .Prop(_SC("References"), &SQLiteStatement::GetReferences)
        .Prop(_SC("Connection"), &SQLiteStatement::GetConnection)
        .Prop(_SC("Status"), &SQLiteStatement::GetStatus)
        .Prop(_SC("ErrCode"), &SQLiteStatement::GetErrorCode)
        .Prop(_SC("ExErrCode"), &SQLiteStatement::GetExtendedErrorCode)
        .Prop(_SC("ExtendedErrCode"), &SQLiteStatement::GetExtendedErrorCode)
        .Prop(_SC("ErrStr"), &SQLiteStatement::GetErrStr)
        .Prop(_SC("ErrMsg"), &SQLiteStatement::GetErrMsg)
        .Prop(_SC("Columns"), &SQLiteStatement::GetColumns)
        .Prop(_SC("Parameters"), &SQLiteStatement::GetParameters)
        .Prop(_SC("Query"), &SQLiteStatement::GetQuery)
        .Prop(_SC("Good"), &SQLiteStatement::GetGood)
        .Prop(_SC("Done"), &SQLiteStatement::GetDone)
        .Prop(_SC("DataCount"), &SQLiteStatement::GetDataCount)
        // Member Methods
        .Func(_SC("Release"), &SQLiteStatement::Release)
        .Func(_SC("CheckParameter"), &SQLiteStatement::CheckParameter)
        .FmtFunc(_SC("GetParameterIndex"), &SQLiteStatement::GetParameterIndex)
        .Func(_SC("GetParameterName"), &SQLiteStatement::GetParameterName)
        .Func(_SC("CheckColumn"), &SQLiteStatement::CheckColumn)
        .Func(_SC("IsColumnNull"), &SQLiteStatement::IsColumnNull)
        .FmtFunc(_SC("ColumnIndex"), &SQLiteStatement::GetColumnIndex)
        .Func(_SC("ColumnName"), &SQLiteStatement::GetColumnName)
        .Func(_SC("ColumnOriginName"), &SQLiteStatement::GetColumnOriginName)
        .Func(_SC("ColumnType"), &SQLiteStatement::GetColumnType)
        .Func(_SC("ColumnBytes"), &SQLiteStatement::GetColumnBytes)
        .Func(_SC("Reset"), &SQLiteStatement::Reset)
        .Func(_SC("Clear"), &SQLiteStatement::Clear)
        .Func(_SC("Exec"), &SQLiteStatement::Exec)
        .Func(_SC("Step"), &SQLiteStatement::Step)
        .Func(_SC("Param"), &SQLiteStatement::GetParameter)
        .Func(_SC("Parameter"), &SQLiteStatement::GetParameter)
        .Func(_SC("SetValue"), &SQLiteStatement::SetValue)
        .Func(_SC("SetBool"), &SQLiteStatement::SetBool)
        .Func(_SC("SetChar"), &SQLiteStatement::SetChar)
        .Func(_SC("SetInteger"), &SQLiteStatement::SetInteger)
        .Func(_SC("SetInt8"), &SQLiteStatement::SetInt8)
        .Func(_SC("SetUint8"), &SQLiteStatement::SetUint8)
        .Func(_SC("SetInt16"), &SQLiteStatement::SetInt16)
        .Func(_SC("SetUint16"), &SQLiteStatement::SetUint16)
        .Func(_SC("SetInt32"), &SQLiteStatement::SetInt32)
        .Func(_SC("SetUint32"), &SQLiteStatement::SetUint32)
        .Func(_SC("SetInt64"), &SQLiteStatement::SetInt64)
        .Func(_SC("SetUint64"), &SQLiteStatement::SetUint64)
        .Func(_SC("SetFloat"), &SQLiteStatement::SetFloat)
        .Func(_SC("SetFloat32"), &SQLiteStatement::SetFloat32)
        .Func(_SC("SetFloat64"), &SQLiteStatement::SetFloat64)
        .FmtFunc(_SC("SetString"), &SQLiteStatement::SetString)
        .Func(_SC("SetZeroBlob"), &SQLiteStatement::SetZeroBlob)
        .Func(_SC("SetBlob"), &SQLiteStatement::SetBlob)
        .Func(_SC("SetData"), &SQLiteStatement::SetData)
        .Func(_SC("SetDate"), &SQLiteStatement::SetDate)
        .Func(_SC("SetDateEx"), &SQLiteStatement::SetDateEx)
        .Func(_SC("SetTime"), &SQLiteStatement::SetTime)
        .Func(_SC("SetTimeEx"), &SQLiteStatement::SetTimeEx)
        .Func(_SC("SetDatetime"), &SQLiteStatement::SetDatetime)
        .Func(_SC("SetDatetimeEx"), &SQLiteStatement::SetDatetimeEx)
        .Func(_SC("SetNow"), &SQLiteStatement::SetNow)
        .Func(_SC("SetNull"), &SQLiteStatement::SetNull)
        .Func(_SC("SetArray"), &SQLiteStatement::SetArray)
        .Func(_SC("SetTable"), &SQLiteStatement::SetTable)
        .Func(_SC("Field"), &SQLiteStatement::GetColumn)
        .Func(_SC("Column"), &SQLiteStatement::GetColumn)
        .Func(_SC("GetValue"), &SQLiteStatement::GetValue)
        .Func(_SC("GetNumber"), &SQLiteStatement::GetNumber)
        .Func(_SC("GetInteger"), &SQLiteStatement::GetInteger)
        .Func(_SC("GetFloat"), &SQLiteStatement::GetFloat)
        .Func(_SC("GetLong"), &SQLiteStatement::GetLong)
        .Func(_SC("GetString"), &SQLiteStatement::GetString)
        .Func(_SC("GetBoolean"), &SQLiteStatement::GetBoolean)
        .Func(_SC("GetChar"), &SQLiteStatement::GetChar)
        .Func(_SC("GetBuffer"), &SQLiteStatement::GetBuffer)
        .Func(_SC("GetBlob"), &SQLiteStatement::GetBlob)
        // Member overloads
        .Overload< Array (SQLiteStatement::*)(void) const >(_SC("GetArray"), &SQLiteStatement::GetArray)
        .Overload< Array (SQLiteStatement::*)(Int32) const >(_SC("GetArray"), &SQLiteStatement::GetArray)
        .Overload< Array (SQLiteStatement::*)(Int32, Int32) const >(_SC("GetArray"), &SQLiteStatement::GetArray)
        .Overload< Table (SQLiteStatement::*)(void) const >(_SC("GetTable"), &SQLiteStatement::GetTable)
        .Overload< Table (SQLiteStatement::*)(Int32) const >(_SC("GetTable"), &SQLiteStatement::GetTable)
        .Overload< Table (SQLiteStatement::*)(Int32, Int32) const >(_SC("GetTable"), &SQLiteStatement::GetTable)
    );

    sqlns.Bind(_SC("Transaction"),
        Class< SQLiteTransaction, NoCopy< SQLiteTransaction > >(sqlns.GetVM(), SQLiteTransactionTypename::Str)
        // Constructors
        .Ctor< const SQLiteConnection & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SQLiteTransactionTypename::Fn)
        .Func(_SC("_tostring"), &SQLiteTransaction::ToString)
        // Properties
        .Prop(_SC("IsValid"), &SQLiteTransaction::IsValid)
        .Prop(_SC("Committed"), &SQLiteTransaction::Commited)
        // Member Methods
        .Func(_SC("Commit"), &SQLiteTransaction::Commit)
    );

    RootTable(vm).Bind(_SC("SQLite"), sqlns);
}

} // Namespace:: SqMod
