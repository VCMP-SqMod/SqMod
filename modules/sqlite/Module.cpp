// --------------------------------------------------------------------------------------------
#include "Module.hpp"
#include "Common.hpp"
#include "Connection.hpp"
#include "Statement.hpp"
#include "Column.hpp"

// --------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

// --------------------------------------------------------------------------------------------
#include <sqrat.h>

// --------------------------------------------------------------------------------------------
#if defined(WIN32) || defined(_WIN32)
    #include <Windows.h>
#endif

namespace SqMod {

// --------------------------------------------------------------------------------------------
PluginFuncs*        _Func = nullptr;
PluginCallbacks*    _Clbk = nullptr;
PluginInfo*         _Info = nullptr;

// --------------------------------------------------------------------------------------------
HSQAPI              _SqAPI = nullptr;
HSQEXPORTS          _SqMod = nullptr;
HSQUIRRELVM         _SqVM = nullptr;

/* ------------------------------------------------------------------------------------------------
 * Bind speciffic functions to certain server events.
*/
void BindCallbacks();

/* ------------------------------------------------------------------------------------------------
 * Undo changes made with BindCallbacks().
*/
void UnbindCallbacks();

/* --------------------------------------------------------------------------------------------
 * Register the module API under the specified virtual machine.
*/
void RegisterAPI(HSQUIRRELVM vm);

/* --------------------------------------------------------------------------------------------
 * Initialize the plugin by obtaining the API provided by the host plugin.
*/
void OnSquirrelInitialize()
{
    // Attempt to import the plugin API exported by the host plugin
    _SqMod = sq_api_import(_Func);
    // Did we failed to obtain the plugin exports?
    if(!_SqMod)
    {
        OutputError("Failed to attach [%s] on host plugin.", SQSQLITE_NAME);
    }
    else
    {
        // Obtain the Squirrel API
        _SqAPI = _SqMod->GetSquirrelAPI();
        // Expand the Squirrel API into global functions
        sq_api_expand(_SqAPI);
    }
}

/* --------------------------------------------------------------------------------------------
 * Load the module on the virtual machine provided by the host module.
*/
void OnSquirrelLoad()
{
    // Make sure that we have a valid plugin API
    if (!_SqMod)
    {
        return; // Unable to proceed!
    }
    // Obtain the Squirrel API and VM
    _SqVM = _SqMod->GetSquirrelVM();
    // Make sure that a valid virtual machine exists
    if (!_SqVM)
    {
        return; // Unable to proceed!
    }
    // Set this as the default database
    DefaultVM::Set(_SqVM);
    // Register the module API
    RegisterAPI(_SqVM);
    // Notify about the current status
    OutputMessage("Registered: %s", SQSQLITE_NAME);
}

/* --------------------------------------------------------------------------------------------
 * The virtual machine is about to be terminated and script resources should be released.
*/
void OnSquirrelTerminate()
{
    OutputMessage("Terminating: %s", SQSQLITE_NAME);
    // Release the current database (if any)
    DefaultVM::Set(nullptr);
}

/* --------------------------------------------------------------------------------------------
 * Validate the module API to make sure we don't run into issues.
*/
bool CheckAPIVer(CCStr ver)
{
    // Obtain the numeric representation of the API version
    long vernum = std::strtol(ver, nullptr, 10);
    // Check against version mismatch
    if (vernum == SQMOD_API_VER)
    {
        return true;
    }
    // Log the incident
    OutputError("API version mismatch on %s", SQSQLITE_NAME);
    OutputMessage("=> Requested: %ld Have: %ld", vernum, SQMOD_API_VER);
    // Invoker should not attempt to communicate through the module API
    return false;
}

/* --------------------------------------------------------------------------------------------
 * React to command sent by other plugins.
*/
static int OnInternalCommand(unsigned int type, const char * text)
{
    switch(type)
    {
        case SQMOD_INITIALIZE_CMD:
            if (CheckAPIVer(text))
            {
                OnSquirrelInitialize();
            }
        break;
        case SQMOD_LOAD_CMD:
            OnSquirrelLoad();
        break;
        case SQMOD_TERMINATE_CMD:
            OnSquirrelTerminate();
        break;
        default: break;
    }
    return 1;
}

/* --------------------------------------------------------------------------------------------
 * The server was initialized and this plugin was loaded successfully.
*/
static int OnInitServer()
{
    return 1;
}

static void OnShutdownServer(void)
{
    // The server may still send callbacks
    UnbindCallbacks();
}

// ------------------------------------------------------------------------------------------------
void BindCallbacks()
{
    _Clbk->OnInitServer             = OnInitServer;
    _Clbk->OnInternalCommand        = OnInternalCommand;
    _Clbk->OnShutdownServer         = OnShutdownServer;
}

// ------------------------------------------------------------------------------------------------
void UnbindCallbacks()
{
    _Clbk->OnInitServer             = nullptr;
    _Clbk->OnInternalCommand        = nullptr;
    _Clbk->OnShutdownServer         = nullptr;
}

// --------------------------------------------------------------------------------------------
void RegisterAPI(HSQUIRRELVM vm)
{
    Table sqlns(vm);

    sqlns.Bind(_SC("Connection"), Class< Connection >(vm, _SC("SqSQLiteConnection"))
        // Constructors
        .Ctor()
        .Ctor< CCStr >()
        .Ctor< CCStr, Int32 >()
        .Ctor< CCStr, Int32, CCStr >()
        // Metamethods
        .Func(_SC("_cmp"), &Connection::Cmp)
        .SquirrelFunc(_SC("_typename"), &Connection::Typename)
        .Func(_SC("_tostring"), &Connection::ToString)
        // Properties
        .Prop(_SC("Valid"), &Connection::IsValid)
        .Prop(_SC("Refs"), &Connection::GetRefCount)
        .Prop(_SC("Connected"), &Connection::IsValid)
        .Prop(_SC("Status"), &Connection::GetStatus)
        .Prop(_SC("Flags"), &Connection::GetFlags)
        .Prop(_SC("Name"), &Connection::GetName)
        .Prop(_SC("VFS"), &Connection::GetVFS)
        .Prop(_SC("ErrCode"), &Connection::GetErrorCode)
        .Prop(_SC("ExErrCode"), &Connection::GetExtendedErrorCode)
        .Prop(_SC("ExtendedErrCode"), &Connection::GetExtendedErrorCode)
        .Prop(_SC("ErrStr"), &Connection::GetErrStr)
        .Prop(_SC("ErrMsg"), &Connection::GetErrMsg)
        .Prop(_SC("ReadOnly"), &Connection::IsReadOnly)
        .Prop(_SC("Autocommit"), &Connection::GetAutoCommit)
        .Prop(_SC("LastInsertRowId"), &Connection::GetLastInsertRowID)
        .Prop(_SC("Changes"), &Connection::GetChanges)
        .Prop(_SC("TotalChanges"), &Connection::GetTotalChanges)
        .Prop(_SC("Trace"), &Connection::GetTracing, &Connection::SetTracing)
        .Prop(_SC("Profile"), &Connection::GetProfiling, &Connection::SetProfiling)
        .Prop(_SC("BusyTimeout"), (Int32 (Connection::*)(void) const)(nullptr), &Connection::SetBusyTimeout)
        .Prop(_SC("QueueSize"), &Connection::QueueSize)
        // Member Methods
        .Func(_SC("Release"), &Connection::Release)
        .Overload< void (Connection::*)(CSStr) >(_SC("Open"), &Connection::Open)
        .Overload< void (Connection::*)(CSStr, Int32) >(_SC("Open"), &Connection::Open)
        .Overload< void (Connection::*)(CSStr, Int32, CSStr) >(_SC("Open"), &Connection::Open)
        .Func(_SC("Exec"), &Connection::Exec)
        .Func(_SC("Queue"), &Connection::Queue)
        .Func(_SC("Query"), &Connection::Query)
        .Func(_SC("TableExists"), &Connection::TableExists)
        .Func(_SC("InterruptOperation"), &Connection::InterruptOperation)
        .Func(_SC("ReleaseMemory"), &Connection::ReleaseMemory)
        .Overload< Int32 (Connection::*)(Int32) >(_SC("GetInfo"), &Connection::GetInfo)
        .Overload< Int32 (Connection::*)(Int32, bool) >(_SC("GetInfo"), &Connection::GetInfo)
        .Overload< Int32 (Connection::*)(Int32, bool, bool) >(_SC("GetInfo"), &Connection::GetInfo)
        .Func(_SC("ReserveQueue"), &Connection::ReserveQueue)
        .Func(_SC("CompactQueue"), &Connection::CompactQueue)
        .Func(_SC("ClearQueue"), &Connection::ClearQueue)
        .Func(_SC("PopQueue"), &Connection::PopQueue)
        .Overload< Int32 (Connection::*)(void) >(_SC("Flush"), &Connection::Flush)
        .Overload< Int32 (Connection::*)(Uint32) >(_SC("Flush"), &Connection::Flush)
        .Overload< Int32 (Connection::*)(Object &, Function &) >(_SC("Flush"), &Connection::Flush)
        .Overload< Int32 (Connection::*)(Uint32, Object &, Function &) >(_SC("Flush"), &Connection::Flush)
        .SquirrelFunc(_SC("ExecF"), &Connection::ExecF)
        .SquirrelFunc(_SC("QueueF"), &Connection::QueueF)
        .SquirrelFunc(_SC("QueryF"), &Connection::QueryF)
    );

    sqlns.Bind(_SC("Statement"), Class< Statement >(vm, _SC("SqSQLiteStatement"))
        // Constructors
        .Ctor()
        .Ctor< const Connection &, CCStr >()
        .Ctor< const Statement & >()
        // Metamethods
        .Func(_SC("_cmp"), &Statement::Cmp)
        .SquirrelFunc(_SC("_typename"), &Statement::Typename)
        .Func(_SC("_tostring"), &Statement::ToString)
        // Properties
        .Prop(_SC("Valid"), &Statement::IsValid)
        .Prop(_SC("Refs"), &Statement::GetRefCount)
        .Prop(_SC("Conn"), &Statement::GetConnection)
        .Prop(_SC("Prepared"), &Statement::IsValid)
        .Prop(_SC("Status"), &Statement::GetStatus)
        .Prop(_SC("ErrCode"), &Statement::GetErrorCode)
        .Prop(_SC("ExErrCode"), &Statement::GetExtendedErrorCode)
        .Prop(_SC("ExtendedErrCode"), &Statement::GetExtendedErrorCode)
        .Prop(_SC("ErrStr"), &Statement::GetErrStr)
        .Prop(_SC("ErrMsg"), &Statement::GetErrMsg)
        .Prop(_SC("Columns"), &Statement::GetColumns)
        .Prop(_SC("Query"), &Statement::GetQuery)
        .Prop(_SC("Good"), &Statement::GetGood)
        .Prop(_SC("Done"), &Statement::GetDone)
        // Member Methods
        .Func(_SC("Release"), &Statement::Release)
        .Func(_SC("Reset"), &Statement::Reset)
        .Func(_SC("Clear"), &Statement::Clear)
        .Func(_SC("Exec"), &Statement::Exec)
        .Func(_SC("Step"), &Statement::Step)
        .Func(_SC("IBindA"), &Statement::IndexBindA)
        .Func(_SC("IBindI"), &Statement::IndexBindI)
        .Func(_SC("IBindL"), &Statement::IndexBindL)
        .Func(_SC("IBindV"), &Statement::IndexBindV)
        .Func(_SC("IBindF"), &Statement::IndexBindF)
        .Func(_SC("IBindS"), &Statement::IndexBindS)
        .Func(_SC("IBindB"), &Statement::IndexBindB)
        .Func(_SC("IBindN"), &Statement::IndexBindN)
        .Func(_SC("NBindT"), &Statement::NameBindT)
        .Func(_SC("NBindI"), &Statement::NameBindI)
        .Func(_SC("NBindL"), &Statement::NameBindL)
        .Func(_SC("NBindV"), &Statement::NameBindV)
        .Func(_SC("NBindF"), &Statement::NameBindF)
        .Func(_SC("NBindS"), &Statement::NameBindS)
        .Func(_SC("NBindB"), &Statement::NameBindB)
        .Func(_SC("NBindN"), &Statement::NameBindN)
        .Func(_SC("IBind"), &Statement::IndexBind)
        .Func(_SC("NBind"), &Statement::NameBind)
        .Func(_SC("Bind"), &Statement::Bind)
        .Func(_SC("FetchI"), &Statement::FetchColumnIndex)
        .Func(_SC("FetchN"), &Statement::FetchColumnName)
        .Func(_SC("Fetch"), &Statement::FetchColumn)
        .Overload< Array (Statement::*)(void) const >(_SC("FetchA"), &Statement::FetchArray)
        .Overload< Array (Statement::*)(Int32) const >(_SC("FetchA"), &Statement::FetchArray)
        .Overload< Array (Statement::*)(Int32, Int32) const >(_SC("FetchA"), &Statement::FetchArray)
        .Overload< Table (Statement::*)(void) const >(_SC("FetchT"), &Statement::FetchTable)
        .Overload< Table (Statement::*)(Int32) const >(_SC("FetchT"), &Statement::FetchTable)
        .Overload< Table (Statement::*)(Int32, Int32) const >(_SC("FetchT"), &Statement::FetchTable)
        .Func(_SC("CheckIndex"), &Statement::CheckIndex)
        .Func(_SC("IsColumnNull"), &Statement::IsColumnNull)
        .Func(_SC("ColumnIndex"), &Statement::GetColumnIndex)
        .Func(_SC("ColumnName"), &Statement::GetColumnName)
        .Func(_SC("ColumnOriginName"), &Statement::GetColumnOriginName)
        .Func(_SC("ColumnType"), &Statement::GetColumnType)
        .Func(_SC("ColumnBytes"), &Statement::GetColumnBytes)
        .Func(_SC("GetI"), &Statement::GetColumnByIndex)
        .Func(_SC("GetN"), &Statement::GetColumnByName)
        .Func(_SC("Get"), &Statement::GetColumn)
    );

    sqlns.Bind(_SC("Column"), Class< Column >(vm, _SC("SqSQLiteColumn"))
        // Constructors
        .Ctor()
        .Ctor< const Column & >()
        // Metamethods
        .Func(_SC("_cmp"), &Column::Cmp)
        .SquirrelFunc(_SC("_typename"), &Column::Typename)
        .Func(_SC("_tostring"), &Column::ToString)
        // Properties
        .Prop(_SC("Valid"), &Column::IsValid)
        .Prop(_SC("Refs"), &Column::GetRefCount)
        .Prop(_SC("Index"), &Column::GetIndex)
        .Prop(_SC("Stmt"), &Column::GetNumber)
        .Prop(_SC("Conn"), &Column::GetConnection)
        .Prop(_SC("Number"), &Column::GetNumber)
        .Prop(_SC("Integer"), &Column::GetInteger)
        .Prop(_SC("Float"), &Column::GetFloat)
        .Prop(_SC("Long"), &Column::GetLong)
        .Prop(_SC("String"), &Column::GetString)
        .Prop(_SC("Boolean"), &Column::GetBoolean)
        .Prop(_SC("Blob"), &Column::GetBlob)
        .Prop(_SC("Char"), &Column::GetChar)
        .Prop(_SC("IsNull"), &Column::IsNull)
        .Prop(_SC("Name"), &Column::GetName)
        .Prop(_SC("OriginName"), &Column::GetOriginName)
        .Prop(_SC("Type"), &Column::GetType)
        .Prop(_SC("Bytes"), &Column::GetBytes)
        // Member Methods
        .Func(_SC("Release"), &Column::Release)
    );

    sqlns.Bind(_SC("Transaction"), Class< Transaction, NoCopy< Transaction > >(vm, _SC("SqSQLiteTransaction"))
        // Constructors
        .Ctor< const Connection & >()
        // Properties
        .Prop(_SC("Committed"), &Transaction::Commited)
        // Member Methods
        .Func(_SC("Commit"), &Transaction::Commit)
    );

    sqlns.Func(_SC("IsQueryEmpty"), &IsQueryEmpty);
    sqlns.Func(_SC("GetErrStr"), &GetErrStr);
    sqlns.Func(_SC("SetSoftHeapLimit"), &SetSoftHeapLimit);
    sqlns.Func(_SC("ReleaseMemory"), &ReleaseMemory);
    sqlns.Func(_SC("MemoryUsage"), &GetMemoryUsage);
    sqlns.Func(_SC("EscapeString"), &EscapeString);
    sqlns.Func(_SC("EscapeStringEx"), &EscapeStringEx);
    sqlns.Func(_SC("Escape"), &EscapeString);
    sqlns.Func(_SC("EscapeEx"), &EscapeStringEx);
    sqlns.Func(_SC("ArrayToQueryColumns"), &ArrayToQueryColumns);
    sqlns.Func(_SC("TableToQueryColumns"), &TableToQueryColumns);

    RootTable(vm).Bind(_SC("SQLite"), sqlns);


    ConstTable(vm).Enum(_SC("ESQLite"), Enumeration(vm)
        .Const(_SC("ABORT"),                            SQLITE_ABORT)
        .Const(_SC("ABORT_ROLLBACK"),                   SQLITE_ABORT_ROLLBACK)
        .Const(_SC("ACCESS_EXISTS"),                    SQLITE_ACCESS_EXISTS)
        .Const(_SC("ACCESS_READ"),                      SQLITE_ACCESS_READ)
        .Const(_SC("ACCESS_READWRITE"),                 SQLITE_ACCESS_READWRITE)
        .Const(_SC("ALTER_TABLE"),                      SQLITE_ALTER_TABLE)
        .Const(_SC("ANALYZE"),                          SQLITE_ANALYZE)
        .Const(_SC("ANY"),                              SQLITE_ANY)
        .Const(_SC("ATTACH"),                           SQLITE_ATTACH)
        .Const(_SC("AUTH"),                             SQLITE_AUTH)
        .Const(_SC("AUTH_USER"),                        SQLITE_AUTH_USER)
        .Const(_SC("BLOB"),                             SQLITE_BLOB)
        .Const(_SC("BUSY"),                             SQLITE_BUSY)
        .Const(_SC("BUSY_RECOVERY"),                    SQLITE_BUSY_RECOVERY)
        .Const(_SC("BUSY_SNAPSHOT"),                    SQLITE_BUSY_SNAPSHOT)
        .Const(_SC("CANTOPEN"),                         SQLITE_CANTOPEN)
        .Const(_SC("CANTOPEN_CONVPATH"),                SQLITE_CANTOPEN_CONVPATH)
        .Const(_SC("CANTOPEN_FULLPATH"),                SQLITE_CANTOPEN_FULLPATH)
        .Const(_SC("CANTOPEN_ISDIR"),                   SQLITE_CANTOPEN_ISDIR)
        .Const(_SC("CANTOPEN_NOTEMPDIR"),               SQLITE_CANTOPEN_NOTEMPDIR)
        .Const(_SC("CHECKPOINT_FULL"),                  SQLITE_CHECKPOINT_FULL)
        .Const(_SC("CHECKPOINT_PASSIVE"),               SQLITE_CHECKPOINT_PASSIVE)
        .Const(_SC("CHECKPOINT_RESTART"),               SQLITE_CHECKPOINT_RESTART)
        .Const(_SC("CHECKPOINT_TRUNCATE"),              SQLITE_CHECKPOINT_TRUNCATE)
        .Const(_SC("CONFIG_COVERING_INDEX_SCAN"),       SQLITE_CONFIG_COVERING_INDEX_SCAN)
        .Const(_SC("CONFIG_GETMALLOC"),                 SQLITE_CONFIG_GETMALLOC)
        .Const(_SC("CONFIG_GETMUTEX"),                  SQLITE_CONFIG_GETMUTEX)
        .Const(_SC("CONFIG_GETPCACHE"),                 SQLITE_CONFIG_GETPCACHE)
        .Const(_SC("CONFIG_GETPCACHE2"),                SQLITE_CONFIG_GETPCACHE2)
        .Const(_SC("CONFIG_HEAP"),                      SQLITE_CONFIG_HEAP)
        .Const(_SC("CONFIG_LOG"),                       SQLITE_CONFIG_LOG)
        .Const(_SC("CONFIG_LOOKASIDE"),                 SQLITE_CONFIG_LOOKASIDE)
        .Const(_SC("CONFIG_MALLOC"),                    SQLITE_CONFIG_MALLOC)
        .Const(_SC("CONFIG_MEMSTATUS"),                 SQLITE_CONFIG_MEMSTATUS)
        .Const(_SC("CONFIG_MMAP_SIZE"),                 SQLITE_CONFIG_MMAP_SIZE)
        .Const(_SC("CONFIG_MULTITHREAD"),               SQLITE_CONFIG_MULTITHREAD)
        .Const(_SC("CONFIG_MUTEX"),                     SQLITE_CONFIG_MUTEX)
        .Const(_SC("CONFIG_PAGECACHE"),                 SQLITE_CONFIG_PAGECACHE)
        .Const(_SC("CONFIG_PCACHE"),                    SQLITE_CONFIG_PCACHE)
        .Const(_SC("CONFIG_PCACHE2"),                   SQLITE_CONFIG_PCACHE2)
        .Const(_SC("CONFIG_PCACHE_HDRSZ"),              SQLITE_CONFIG_PCACHE_HDRSZ)
        .Const(_SC("CONFIG_PMASZ"),                     SQLITE_CONFIG_PMASZ)
        .Const(_SC("CONFIG_SCRATCH"),                   SQLITE_CONFIG_SCRATCH)
        .Const(_SC("CONFIG_SERIALIZED"),                SQLITE_CONFIG_SERIALIZED)
        .Const(_SC("CONFIG_SINGLETHREAD"),              SQLITE_CONFIG_SINGLETHREAD)
        .Const(_SC("CONFIG_SQLLOG"),                    SQLITE_CONFIG_SQLLOG)
        .Const(_SC("CONFIG_URI"),                       SQLITE_CONFIG_URI)
        .Const(_SC("CONFIG_WIN32_HEAPSIZE"),            SQLITE_CONFIG_WIN32_HEAPSIZE)
        .Const(_SC("CONSTRAINT"),                       SQLITE_CONSTRAINT)
        .Const(_SC("CONSTRAINT_CHECK"),                 SQLITE_CONSTRAINT_CHECK)
        .Const(_SC("CONSTRAINT_COMMITHOOK"),            SQLITE_CONSTRAINT_COMMITHOOK)
        .Const(_SC("CONSTRAINT_FOREIGNKEY"),            SQLITE_CONSTRAINT_FOREIGNKEY)
        .Const(_SC("CONSTRAINT_FUNCTION"),              SQLITE_CONSTRAINT_FUNCTION)
        .Const(_SC("CONSTRAINT_NOTNULL"),               SQLITE_CONSTRAINT_NOTNULL)
        .Const(_SC("CONSTRAINT_PRIMARYKEY"),            SQLITE_CONSTRAINT_PRIMARYKEY)
        .Const(_SC("CONSTRAINT_ROWID"),                 SQLITE_CONSTRAINT_ROWID)
        .Const(_SC("CONSTRAINT_TRIGGER"),               SQLITE_CONSTRAINT_TRIGGER)
        .Const(_SC("CONSTRAINT_UNIQUE"),                SQLITE_CONSTRAINT_UNIQUE)
        .Const(_SC("CONSTRAINT_VTAB"),                  SQLITE_CONSTRAINT_VTAB)
        .Const(_SC("COPY"),                             SQLITE_COPY)
        .Const(_SC("CORRUPT"),                          SQLITE_CORRUPT)
        .Const(_SC("CORRUPT_VTAB"),                     SQLITE_CORRUPT_VTAB)
        .Const(_SC("CREATE_INDEX"),                     SQLITE_CREATE_INDEX)
        .Const(_SC("CREATE_TABLE"),                     SQLITE_CREATE_TABLE)
        .Const(_SC("CREATE_TEMP_INDEX"),                SQLITE_CREATE_TEMP_INDEX)
        .Const(_SC("CREATE_TEMP_TABLE"),                SQLITE_CREATE_TEMP_TABLE)
        .Const(_SC("CREATE_TEMP_TRIGGER"),              SQLITE_CREATE_TEMP_TRIGGER)
        .Const(_SC("CREATE_TEMP_VIEW"),                 SQLITE_CREATE_TEMP_VIEW)
        .Const(_SC("CREATE_TRIGGER"),                   SQLITE_CREATE_TRIGGER)
        .Const(_SC("CREATE_VIEW"),                      SQLITE_CREATE_VIEW)
        .Const(_SC("CREATE_VTABLE"),                    SQLITE_CREATE_VTABLE)
        .Const(_SC("DBCONFIG_ENABLE_FKEY"),             SQLITE_DBCONFIG_ENABLE_FKEY)
        .Const(_SC("DBCONFIG_ENABLE_TRIGGER"),          SQLITE_DBCONFIG_ENABLE_TRIGGER)
        .Const(_SC("DBCONFIG_LOOKASIDE"),               SQLITE_DBCONFIG_LOOKASIDE)
        .Const(_SC("DBSTATUS_CACHE_HIT"),               SQLITE_DBSTATUS_CACHE_HIT)
        .Const(_SC("DBSTATUS_CACHE_MISS"),              SQLITE_DBSTATUS_CACHE_MISS)
        .Const(_SC("DBSTATUS_CACHE_USED"),              SQLITE_DBSTATUS_CACHE_USED)
        .Const(_SC("DBSTATUS_CACHE_WRITE"),             SQLITE_DBSTATUS_CACHE_WRITE)
        .Const(_SC("DBSTATUS_DEFERRED_FKS"),            SQLITE_DBSTATUS_DEFERRED_FKS)
        .Const(_SC("DBSTATUS_LOOKASIDE_HIT"),           SQLITE_DBSTATUS_LOOKASIDE_HIT)
        .Const(_SC("DBSTATUS_LOOKASIDE_MISS_FULL"),     SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL)
        .Const(_SC("DBSTATUS_LOOKASIDE_MISS_SIZE"),     SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE)
        .Const(_SC("DBSTATUS_LOOKASIDE_USED"),          SQLITE_DBSTATUS_LOOKASIDE_USED)
        .Const(_SC("DBSTATUS_MAX"),                     SQLITE_DBSTATUS_MAX)
        .Const(_SC("DBSTATUS_SCHEMA_USED"),             SQLITE_DBSTATUS_SCHEMA_USED)
        .Const(_SC("DBSTATUS_STMT_USED"),               SQLITE_DBSTATUS_STMT_USED)
        .Const(_SC("DELETE"),                           SQLITE_DELETE)
        .Const(_SC("DENY"),                             SQLITE_DENY)
        .Const(_SC("DETACH"),                           SQLITE_DETACH)
        .Const(_SC("DETERMINISTIC"),                    SQLITE_DETERMINISTIC)
        .Const(_SC("DONE"),                             SQLITE_DONE)
        .Const(_SC("DROP_INDEX"),                       SQLITE_DROP_INDEX)
        .Const(_SC("DROP_TABLE"),                       SQLITE_DROP_TABLE)
        .Const(_SC("DROP_TEMP_INDEX"),                  SQLITE_DROP_TEMP_INDEX)
        .Const(_SC("DROP_TEMP_TABLE"),                  SQLITE_DROP_TEMP_TABLE)
        .Const(_SC("DROP_TEMP_TRIGGER"),                SQLITE_DROP_TEMP_TRIGGER)
        .Const(_SC("DROP_TEMP_VIEW"),                   SQLITE_DROP_TEMP_VIEW)
        .Const(_SC("DROP_TRIGGER"),                     SQLITE_DROP_TRIGGER)
        .Const(_SC("DROP_VIEW"),                        SQLITE_DROP_VIEW)
        .Const(_SC("DROP_VTABLE"),                      SQLITE_DROP_VTABLE)
        .Const(_SC("EMPTY"),                            SQLITE_EMPTY)
        .Const(_SC("ERROR"),                            SQLITE_ERROR)
        .Const(_SC("FAIL"),                             SQLITE_FAIL)
        .Const(_SC("FCNTL_BUSYHANDLER"),                SQLITE_FCNTL_BUSYHANDLER)
        .Const(_SC("FCNTL_CHUNK_SIZE"),                 SQLITE_FCNTL_CHUNK_SIZE)
        .Const(_SC("FCNTL_COMMIT_PHASETWO"),            SQLITE_FCNTL_COMMIT_PHASETWO)
        .Const(_SC("FCNTL_FILE_POINTER"),               SQLITE_FCNTL_FILE_POINTER)
        .Const(_SC("FCNTL_GET_LOCKPROXYFILE"),          SQLITE_FCNTL_GET_LOCKPROXYFILE)
        .Const(_SC("FCNTL_HAS_MOVED"),                  SQLITE_FCNTL_HAS_MOVED)
        .Const(_SC("FCNTL_LAST_ERRNO"),                 SQLITE_FCNTL_LAST_ERRNO)
        .Const(_SC("FCNTL_LOCKSTATE"),                  SQLITE_FCNTL_LOCKSTATE)
        .Const(_SC("FCNTL_MMAP_SIZE"),                  SQLITE_FCNTL_MMAP_SIZE)
        .Const(_SC("FCNTL_OVERWRITE"),                  SQLITE_FCNTL_OVERWRITE)
        .Const(_SC("FCNTL_PERSIST_WAL"),                SQLITE_FCNTL_PERSIST_WAL)
        .Const(_SC("FCNTL_POWERSAFE_OVERWRITE"),        SQLITE_FCNTL_POWERSAFE_OVERWRITE)
        .Const(_SC("FCNTL_PRAGMA"),                     SQLITE_FCNTL_PRAGMA)
        .Const(_SC("FCNTL_RBU"),                        SQLITE_FCNTL_RBU)
        .Const(_SC("FCNTL_SET_LOCKPROXYFILE"),          SQLITE_FCNTL_SET_LOCKPROXYFILE)
        .Const(_SC("FCNTL_SIZE_HINT"),                  SQLITE_FCNTL_SIZE_HINT)
        .Const(_SC("FCNTL_SYNC"),                       SQLITE_FCNTL_SYNC)
        .Const(_SC("FCNTL_SYNC_OMITTED"),               SQLITE_FCNTL_SYNC_OMITTED)
        .Const(_SC("FCNTL_TEMPFILENAME"),               SQLITE_FCNTL_TEMPFILENAME)
        .Const(_SC("FCNTL_TRACE"),                      SQLITE_FCNTL_TRACE)
        .Const(_SC("FCNTL_VFSNAME"),                    SQLITE_FCNTL_VFSNAME)
        .Const(_SC("FCNTL_WAL_BLOCK"),                  SQLITE_FCNTL_WAL_BLOCK)
        .Const(_SC("FCNTL_WIN32_AV_RETRY"),             SQLITE_FCNTL_WIN32_AV_RETRY)
        .Const(_SC("FCNTL_WIN32_SET_HANDLE"),           SQLITE_FCNTL_WIN32_SET_HANDLE)
        .Const(_SC("FCNTL_ZIPVFS"),                     SQLITE_FCNTL_ZIPVFS)
        .Const(_SC("FLOAT"),                            SQLITE_FLOAT)
        .Const(_SC("FORMAT"),                           SQLITE_FORMAT)
        .Const(_SC("FULL"),                             SQLITE_FULL)
        .Const(_SC("FUNCTION"),                         SQLITE_FUNCTION)
        .Const(_SC("IGNORE"),                           SQLITE_IGNORE)
        .Const(_SC("INDEX_CONSTRAINT_EQ"),              SQLITE_INDEX_CONSTRAINT_EQ)
        .Const(_SC("INDEX_CONSTRAINT_GE"),              SQLITE_INDEX_CONSTRAINT_GE)
        .Const(_SC("INDEX_CONSTRAINT_GT"),              SQLITE_INDEX_CONSTRAINT_GT)
        .Const(_SC("INDEX_CONSTRAINT_LE"),              SQLITE_INDEX_CONSTRAINT_LE)
        .Const(_SC("INDEX_CONSTRAINT_LT"),              SQLITE_INDEX_CONSTRAINT_LT)
        .Const(_SC("INDEX_CONSTRAINT_MATCH"),           SQLITE_INDEX_CONSTRAINT_MATCH)
        .Const(_SC("INDEX_SCAN_UNIQUE"),                SQLITE_INDEX_SCAN_UNIQUE)
        .Const(_SC("INSERT"),                           SQLITE_INSERT)
        .Const(_SC("INTEGER"),                          SQLITE_INTEGER)
        .Const(_SC("INTERNAL"),                         SQLITE_INTERNAL)
        .Const(_SC("INTERRUPT"),                        SQLITE_INTERRUPT)
        .Const(_SC("IOCAP_ATOMIC"),                     SQLITE_IOCAP_ATOMIC)
        .Const(_SC("IOCAP_ATOMIC16K"),                  SQLITE_IOCAP_ATOMIC16K)
        .Const(_SC("IOCAP_ATOMIC1K"),                   SQLITE_IOCAP_ATOMIC1K)
        .Const(_SC("IOCAP_ATOMIC2K"),                   SQLITE_IOCAP_ATOMIC2K)
        .Const(_SC("IOCAP_ATOMIC32K"),                  SQLITE_IOCAP_ATOMIC32K)
        .Const(_SC("IOCAP_ATOMIC4K"),                   SQLITE_IOCAP_ATOMIC4K)
        .Const(_SC("IOCAP_ATOMIC512"),                  SQLITE_IOCAP_ATOMIC512)
        .Const(_SC("IOCAP_ATOMIC64K"),                  SQLITE_IOCAP_ATOMIC64K)
        .Const(_SC("IOCAP_ATOMIC8K"),                   SQLITE_IOCAP_ATOMIC8K)
        .Const(_SC("IOCAP_IMMUTABLE"),                  SQLITE_IOCAP_IMMUTABLE)
        .Const(_SC("IOCAP_POWERSAFE_OVERWRITE"),        SQLITE_IOCAP_POWERSAFE_OVERWRITE)
        .Const(_SC("IOCAP_SAFE_APPEND"),                SQLITE_IOCAP_SAFE_APPEND)
        .Const(_SC("IOCAP_SEQUENTIAL"),                 SQLITE_IOCAP_SEQUENTIAL)
        .Const(_SC("IOCAP_UNDELETABLE_WHEN_OPEN"),      SQLITE_IOCAP_UNDELETABLE_WHEN_OPEN)
        .Const(_SC("IOERR"),                            SQLITE_IOERR)
        .Const(_SC("IOERR_ACCESS"),                     SQLITE_IOERR_ACCESS)
        .Const(_SC("IOERR_BLOCKED"),                    SQLITE_IOERR_BLOCKED)
        .Const(_SC("IOERR_CHECKRESERVEDLOCK"),          SQLITE_IOERR_CHECKRESERVEDLOCK)
        .Const(_SC("IOERR_CLOSE"),                      SQLITE_IOERR_CLOSE)
        .Const(_SC("IOERR_CONVPATH"),                   SQLITE_IOERR_CONVPATH)
        .Const(_SC("IOERR_DELETE"),                     SQLITE_IOERR_DELETE)
        .Const(_SC("IOERR_DELETE_NOENT"),               SQLITE_IOERR_DELETE_NOENT)
        .Const(_SC("IOERR_DIR_CLOSE"),                  SQLITE_IOERR_DIR_CLOSE)
        .Const(_SC("IOERR_DIR_FSYNC"),                  SQLITE_IOERR_DIR_FSYNC)
        .Const(_SC("IOERR_FSTAT"),                      SQLITE_IOERR_FSTAT)
        .Const(_SC("IOERR_FSYNC"),                      SQLITE_IOERR_FSYNC)
        .Const(_SC("IOERR_GETTEMPPATH"),                SQLITE_IOERR_GETTEMPPATH)
        .Const(_SC("IOERR_LOCK"),                       SQLITE_IOERR_LOCK)
        .Const(_SC("IOERR_MMAP"),                       SQLITE_IOERR_MMAP)
        .Const(_SC("IOERR_NOMEM"),                      SQLITE_IOERR_NOMEM)
        .Const(_SC("IOERR_RDLOCK"),                     SQLITE_IOERR_RDLOCK)
        .Const(_SC("IOERR_READ"),                       SQLITE_IOERR_READ)
        .Const(_SC("IOERR_SEEK"),                       SQLITE_IOERR_SEEK)
        .Const(_SC("IOERR_SHMLOCK"),                    SQLITE_IOERR_SHMLOCK)
        .Const(_SC("IOERR_SHMMAP"),                     SQLITE_IOERR_SHMMAP)
        .Const(_SC("IOERR_SHMOPEN"),                    SQLITE_IOERR_SHMOPEN)
        .Const(_SC("IOERR_SHMSIZE"),                    SQLITE_IOERR_SHMSIZE)
        .Const(_SC("IOERR_SHORT_READ"),                 SQLITE_IOERR_SHORT_READ)
        .Const(_SC("IOERR_TRUNCATE"),                   SQLITE_IOERR_TRUNCATE)
        .Const(_SC("IOERR_UNLOCK"),                     SQLITE_IOERR_UNLOCK)
        .Const(_SC("IOERR_VNODE"),                      SQLITE_IOERR_VNODE)
        .Const(_SC("IOERR_WRITE"),                      SQLITE_IOERR_WRITE)
        .Const(_SC("LIMIT_ATTACHED"),                   SQLITE_LIMIT_ATTACHED)
        .Const(_SC("LIMIT_COLUMN"),                     SQLITE_LIMIT_COLUMN)
        .Const(_SC("LIMIT_COMPOUND_SELECT"),            SQLITE_LIMIT_COMPOUND_SELECT)
        .Const(_SC("LIMIT_EXPR_DEPTH"),                 SQLITE_LIMIT_EXPR_DEPTH)
        .Const(_SC("LIMIT_FUNCTION_ARG"),               SQLITE_LIMIT_FUNCTION_ARG)
        .Const(_SC("LIMIT_LENGTH"),                     SQLITE_LIMIT_LENGTH)
        .Const(_SC("LIMIT_LIKE_PATTERN_LENGTH"),        SQLITE_LIMIT_LIKE_PATTERN_LENGTH)
        .Const(_SC("LIMIT_SQL_LENGTH"),                 SQLITE_LIMIT_SQL_LENGTH)
        .Const(_SC("LIMIT_TRIGGER_DEPTH"),              SQLITE_LIMIT_TRIGGER_DEPTH)
        .Const(_SC("LIMIT_VARIABLE_NUMBER"),            SQLITE_LIMIT_VARIABLE_NUMBER)
        .Const(_SC("LIMIT_VDBE_OP"),                    SQLITE_LIMIT_VDBE_OP)
        .Const(_SC("LIMIT_WORKER_THREADS"),             SQLITE_LIMIT_WORKER_THREADS)
        .Const(_SC("LOCKED"),                           SQLITE_LOCKED)
        .Const(_SC("LOCKED_SHAREDCACHE"),               SQLITE_LOCKED_SHAREDCACHE)
        .Const(_SC("LOCK_EXCLUSIVE"),                   SQLITE_LOCK_EXCLUSIVE)
        .Const(_SC("LOCK_NONE"),                        SQLITE_LOCK_NONE)
        .Const(_SC("LOCK_PENDING"),                     SQLITE_LOCK_PENDING)
        .Const(_SC("LOCK_RESERVED"),                    SQLITE_LOCK_RESERVED)
        .Const(_SC("LOCK_SHARED"),                      SQLITE_LOCK_SHARED)
        .Const(_SC("MISMATCH"),                         SQLITE_MISMATCH)
        .Const(_SC("MISUSE"),                           SQLITE_MISUSE)
        .Const(_SC("MUTEX_FAST"),                       SQLITE_MUTEX_FAST)
        .Const(_SC("MUTEX_RECURSIVE"),                  SQLITE_MUTEX_RECURSIVE)
        .Const(_SC("MUTEX_STATIC_APP1"),                SQLITE_MUTEX_STATIC_APP1)
        .Const(_SC("MUTEX_STATIC_APP2"),                SQLITE_MUTEX_STATIC_APP2)
        .Const(_SC("MUTEX_STATIC_APP3"),                SQLITE_MUTEX_STATIC_APP3)
        .Const(_SC("MUTEX_STATIC_LRU"),                 SQLITE_MUTEX_STATIC_LRU)
        .Const(_SC("MUTEX_STATIC_LRU2"),                SQLITE_MUTEX_STATIC_LRU2)
        .Const(_SC("MUTEX_STATIC_MASTER"),              SQLITE_MUTEX_STATIC_MASTER)
        .Const(_SC("MUTEX_STATIC_MEM"),                 SQLITE_MUTEX_STATIC_MEM)
        .Const(_SC("MUTEX_STATIC_MEM2"),                SQLITE_MUTEX_STATIC_MEM2)
        .Const(_SC("MUTEX_STATIC_OPEN"),                SQLITE_MUTEX_STATIC_OPEN)
        .Const(_SC("MUTEX_STATIC_PMEM"),                SQLITE_MUTEX_STATIC_PMEM)
        .Const(_SC("MUTEX_STATIC_PRNG"),                SQLITE_MUTEX_STATIC_PRNG)
        .Const(_SC("MUTEX_STATIC_VFS1"),                SQLITE_MUTEX_STATIC_VFS1)
        .Const(_SC("MUTEX_STATIC_VFS2"),                SQLITE_MUTEX_STATIC_VFS2)
        .Const(_SC("MUTEX_STATIC_VFS3"),                SQLITE_MUTEX_STATIC_VFS3)
        .Const(_SC("NOLFS"),                            SQLITE_NOLFS)
        .Const(_SC("NOMEM"),                            SQLITE_NOMEM)
        .Const(_SC("NOTADB"),                           SQLITE_NOTADB)
        .Const(_SC("NOTFOUND"),                         SQLITE_NOTFOUND)
        .Const(_SC("NOTICE"),                           SQLITE_NOTICE)
        .Const(_SC("NOTICE_RECOVER_ROLLBACK"),          SQLITE_NOTICE_RECOVER_ROLLBACK)
        .Const(_SC("NOTICE_RECOVER_WAL"),               SQLITE_NOTICE_RECOVER_WAL)
        .Const(_SC("nullptr"),                             SQLITE_NULL)
        .Const(_SC("OK"),                               SQLITE_OK)
        .Const(_SC("OPEN_AUTOPROXY"),                   SQLITE_OPEN_AUTOPROXY)
        .Const(_SC("OPEN_CREATE"),                      SQLITE_OPEN_CREATE)
        .Const(_SC("OPEN_DELETEONCLOSE"),               SQLITE_OPEN_DELETEONCLOSE)
        .Const(_SC("OPEN_EXCLUSIVE"),                   SQLITE_OPEN_EXCLUSIVE)
        .Const(_SC("OPEN_FULLMUTEX"),                   SQLITE_OPEN_FULLMUTEX)
        .Const(_SC("OPEN_MAIN_DB"),                     SQLITE_OPEN_MAIN_DB)
        .Const(_SC("OPEN_MAIN_JOURNAL"),                SQLITE_OPEN_MAIN_JOURNAL)
        .Const(_SC("OPEN_MASTER_JOURNAL"),              SQLITE_OPEN_MASTER_JOURNAL)
        .Const(_SC("OPEN_MEMORY"),                      SQLITE_OPEN_MEMORY)
        .Const(_SC("OPEN_NOMUTEX"),                     SQLITE_OPEN_NOMUTEX)
        .Const(_SC("OPEN_PRIVATECACHE"),                SQLITE_OPEN_PRIVATECACHE)
        .Const(_SC("OPEN_READONLY"),                    SQLITE_OPEN_READONLY)
        .Const(_SC("OPEN_READWRITE"),                   SQLITE_OPEN_READWRITE)
        .Const(_SC("OPEN_SHAREDCACHE"),                 SQLITE_OPEN_SHAREDCACHE)
        .Const(_SC("OPEN_SUBJOURNAL"),                  SQLITE_OPEN_SUBJOURNAL)
        .Const(_SC("OPEN_TEMP_DB"),                     SQLITE_OPEN_TEMP_DB)
        .Const(_SC("OPEN_TEMP_JOURNAL"),                SQLITE_OPEN_TEMP_JOURNAL)
        .Const(_SC("OPEN_TRANSIENT_DB"),                SQLITE_OPEN_TRANSIENT_DB)
        .Const(_SC("OPEN_URI"),                         SQLITE_OPEN_URI)
        .Const(_SC("OPEN_WAL"),                         SQLITE_OPEN_WAL)
        .Const(_SC("PERM"),                             SQLITE_PERM)
        .Const(_SC("PRAGMA"),                           SQLITE_PRAGMA)
        .Const(_SC("PROTOCOL"),                         SQLITE_PROTOCOL)
        .Const(_SC("RANGE"),                            SQLITE_RANGE)
        .Const(_SC("READ"),                             SQLITE_READ)
        .Const(_SC("READONLY"),                         SQLITE_READONLY)
        .Const(_SC("READONLY_CANTLOCK"),                SQLITE_READONLY_CANTLOCK)
        .Const(_SC("READONLY_DBMOVED"),                 SQLITE_READONLY_DBMOVED)
        .Const(_SC("READONLY_RECOVERY"),                SQLITE_READONLY_RECOVERY)
        .Const(_SC("READONLY_ROLLBACK"),                SQLITE_READONLY_ROLLBACK)
        .Const(_SC("RECURSIVE"),                        SQLITE_RECURSIVE)
        .Const(_SC("REINDEX"),                          SQLITE_REINDEX)
        .Const(_SC("REPLACE"),                          SQLITE_REPLACE)
        .Const(_SC("ROLLBACK"),                         SQLITE_ROLLBACK)
        .Const(_SC("ROW"),                              SQLITE_ROW)
        .Const(_SC("SAVEPOINT"),                        SQLITE_SAVEPOINT)
        .Const(_SC("SCANSTAT_EST"),                     SQLITE_SCANSTAT_EST)
        .Const(_SC("SCANSTAT_EXPLAIN"),                 SQLITE_SCANSTAT_EXPLAIN)
        .Const(_SC("SCANSTAT_NAME"),                    SQLITE_SCANSTAT_NAME)
        .Const(_SC("SCANSTAT_NLOOP"),                   SQLITE_SCANSTAT_NLOOP)
        .Const(_SC("SCANSTAT_NVISIT"),                  SQLITE_SCANSTAT_NVISIT)
        .Const(_SC("SCANSTAT_SELECTID"),                SQLITE_SCANSTAT_SELECTID)
        .Const(_SC("SCHEMA"),                           SQLITE_SCHEMA)
        .Const(_SC("SELECT"),                           SQLITE_SELECT)
        .Const(_SC("SHM_EXCLUSIVE"),                    SQLITE_SHM_EXCLUSIVE)
        .Const(_SC("SHM_LOCK"),                         SQLITE_SHM_LOCK)
        .Const(_SC("SHM_NLOCK"),                        SQLITE_SHM_NLOCK)
        .Const(_SC("SHM_SHARED"),                       SQLITE_SHM_SHARED)
        .Const(_SC("SHM_UNLOCK"),                       SQLITE_SHM_UNLOCK)
        .Const(_SC("SOURCE_ID"),                        SQLITE_SOURCE_ID)
        .Const(_SC("STATUS_MALLOC_COUNT"),              SQLITE_STATUS_MALLOC_COUNT)
        .Const(_SC("STATUS_MALLOC_SIZE"),               SQLITE_STATUS_MALLOC_SIZE)
        .Const(_SC("STATUS_MEMORY_USED"),               SQLITE_STATUS_MEMORY_USED)
        .Const(_SC("STATUS_PAGECACHE_OVERFLOW"),        SQLITE_STATUS_PAGECACHE_OVERFLOW)
        .Const(_SC("STATUS_PAGECACHE_SIZE"),            SQLITE_STATUS_PAGECACHE_SIZE)
        .Const(_SC("STATUS_PAGECACHE_USED"),            SQLITE_STATUS_PAGECACHE_USED)
        .Const(_SC("STATUS_PARSER_STACK"),              SQLITE_STATUS_PARSER_STACK)
        .Const(_SC("STATUS_SCRATCH_OVERFLOW"),          SQLITE_STATUS_SCRATCH_OVERFLOW)
        .Const(_SC("STATUS_SCRATCH_SIZE"),              SQLITE_STATUS_SCRATCH_SIZE)
        .Const(_SC("STATUS_SCRATCH_USED"),              SQLITE_STATUS_SCRATCH_USED)
        .Const(_SC("STMTSTATUS_AUTOINDEX"),             SQLITE_STMTSTATUS_AUTOINDEX)
        .Const(_SC("STMTSTATUS_FULLSCAN_STEP"),         SQLITE_STMTSTATUS_FULLSCAN_STEP)
        .Const(_SC("STMTSTATUS_SORT"),                  SQLITE_STMTSTATUS_SORT)
        .Const(_SC("STMTSTATUS_VM_STEP"),               SQLITE_STMTSTATUS_VM_STEP)
        .Const(_SC("SYNC_DATAONLY"),                    SQLITE_SYNC_DATAONLY)
        .Const(_SC("SYNC_FULL"),                        SQLITE_SYNC_FULL)
        .Const(_SC("SYNC_NORMAL"),                      SQLITE_SYNC_NORMAL)
        .Const(_SC("TESTCTRL_ALWAYS"),                  SQLITE_TESTCTRL_ALWAYS)
        .Const(_SC("TESTCTRL_ASSERT"),                  SQLITE_TESTCTRL_ASSERT)
        .Const(_SC("TESTCTRL_BENIGN_MALLOC_HOOKS"),     SQLITE_TESTCTRL_BENIGN_MALLOC_HOOKS)
        .Const(_SC("TESTCTRL_BITVEC_TEST"),             SQLITE_TESTCTRL_BITVEC_TEST)
        .Const(_SC("TESTCTRL_BYTEORDER"),               SQLITE_TESTCTRL_BYTEORDER)
        .Const(_SC("TESTCTRL_EXPLAIN_STMT"),            SQLITE_TESTCTRL_EXPLAIN_STMT)
        .Const(_SC("TESTCTRL_FAULT_INSTALL"),           SQLITE_TESTCTRL_FAULT_INSTALL)
        .Const(_SC("TESTCTRL_FIRST"),                   SQLITE_TESTCTRL_FIRST)
        .Const(_SC("TESTCTRL_IMPOSTER"),                SQLITE_TESTCTRL_IMPOSTER)
        .Const(_SC("TESTCTRL_ISINIT"),                  SQLITE_TESTCTRL_ISINIT)
        .Const(_SC("TESTCTRL_ISKEYWORD"),               SQLITE_TESTCTRL_ISKEYWORD)
        .Const(_SC("TESTCTRL_LAST"),                    SQLITE_TESTCTRL_LAST)
        .Const(_SC("TESTCTRL_LOCALTIME_FAULT"),         SQLITE_TESTCTRL_LOCALTIME_FAULT)
        .Const(_SC("TESTCTRL_NEVER_CORRUPT"),           SQLITE_TESTCTRL_NEVER_CORRUPT)
        .Const(_SC("TESTCTRL_OPTIMIZATIONS"),           SQLITE_TESTCTRL_OPTIMIZATIONS)
        .Const(_SC("TESTCTRL_PENDING_BYTE"),            SQLITE_TESTCTRL_PENDING_BYTE)
        .Const(_SC("TESTCTRL_PRNG_RESET"),              SQLITE_TESTCTRL_PRNG_RESET)
        .Const(_SC("TESTCTRL_PRNG_RESTORE"),            SQLITE_TESTCTRL_PRNG_RESTORE)
        .Const(_SC("TESTCTRL_PRNG_SAVE"),               SQLITE_TESTCTRL_PRNG_SAVE)
        .Const(_SC("TESTCTRL_RESERVE"),                 SQLITE_TESTCTRL_RESERVE)
        .Const(_SC("TESTCTRL_SCRATCHMALLOC"),           SQLITE_TESTCTRL_SCRATCHMALLOC)
        .Const(_SC("TESTCTRL_SORTER_MMAP"),             SQLITE_TESTCTRL_SORTER_MMAP)
        .Const(_SC("TESTCTRL_VDBE_COVERAGE"),           SQLITE_TESTCTRL_VDBE_COVERAGE)
        .Const(_SC("TEXT"),                             SQLITE_TEXT)
        .Const(_SC("TOOBIG"),                           SQLITE_TOOBIG)
        .Const(_SC("TRANSACTION"),                      SQLITE_TRANSACTION)
        .Const(_SC("UPDATE"),                           SQLITE_UPDATE)
        .Const(_SC("UTF16"),                            SQLITE_UTF16)
        .Const(_SC("UTF16BE"),                          SQLITE_UTF16BE)
        .Const(_SC("UTF16LE"),                          SQLITE_UTF16LE)
        .Const(_SC("UTF16_ALIGNED"),                    SQLITE_UTF16_ALIGNED)
        .Const(_SC("UTF8"),                             SQLITE_UTF8)
        .Const(_SC("VERSION"),                          SQLITE_VERSION)
        .Const(_SC("VERSION_NUMBER"),                   SQLITE_VERSION_NUMBER)
        .Const(_SC("VTAB_CONSTRAINT_SUPPORT"),          SQLITE_VTAB_CONSTRAINT_SUPPORT)
        .Const(_SC("WARNING"),                          SQLITE_WARNING)
        .Const(_SC("WARNING_AUTOINDEX"),                SQLITE_WARNING_AUTOINDEX)
    );
}

// --------------------------------------------------------------------------------------------
void OutputMessageImpl(const char * msg, va_list args)
{
#if defined(WIN32) || defined(_WIN32)
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csb_before;
    GetConsoleScreenBufferInfo( hstdout, &csb_before);
    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN);
    printf("[SQMOD] ");

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    vprintf(msg, args);
    puts("");

    SetConsoleTextAttribute(hstdout, csb_before.wAttributes);
#else
    printf("%c[0;32m[SQMOD]%c[0;37m", 27, 27);
    vprintf(msg, args);
    puts("");
#endif
}

// --------------------------------------------------------------------------------------------
void OutputErrorImpl(const char * msg, va_list args)
{
#if defined(WIN32) || defined(_WIN32)
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csb_before;
    GetConsoleScreenBufferInfo( hstdout, &csb_before);
    SetConsoleTextAttribute(hstdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("[SQMOD] ");

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    vprintf(msg, args);
    puts("");

    SetConsoleTextAttribute(hstdout, csb_before.wAttributes);
#else
    printf("%c[0;32m[SQMOD]%c[0;37m", 27, 27);
    vprintf(msg, args);
    puts("");
#endif
}

// --------------------------------------------------------------------------------------------
void OutputDebug(const char * msg, ...)
{
#ifdef _DEBUG
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputMessageImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
#else
    SQMOD_UNUSED_VAR(msg);
#endif
}

// --------------------------------------------------------------------------------------------
void OutputMessage(const char * msg, ...)
{
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputMessageImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
}

// --------------------------------------------------------------------------------------------
void OutputError(const char * msg, ...)
{
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputErrorImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
}

} // Namespace:: SqMod

// --------------------------------------------------------------------------------------------
SQMOD_API_EXPORT unsigned int VcmpPluginInit(PluginFuncs* functions, PluginCallbacks* callbacks, PluginInfo* info)
{
    using namespace SqMod;
    // Output plugin header
    puts("");
    OutputMessage("--------------------------------------------------------------------");
    OutputMessage("Plugin: %s", SQSQLITE_NAME);
    OutputMessage("Author: %s", SQSQLITE_AUTHOR);
    OutputMessage("Legal: %s", SQSQLITE_COPYRIGHT);
    OutputMessage("--------------------------------------------------------------------");
    puts("");
    // Attempt to find the host plugin ID
    int host_plugin_id = functions->FindPlugin((char *)(SQMOD_HOST_NAME));
    // See if our plugin was loaded after the host plugin
    if (host_plugin_id < 0)
    {
        OutputError("%s could find the host plugin", SQSQLITE_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
    // Should never reach this point but just in case
    else if (host_plugin_id > (info->nPluginId))
    {
        OutputError("%s loaded after the host plugin", SQSQLITE_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
    // Store server proxies
    _Func = functions;
    _Clbk = callbacks;
    _Info = info;
    // Assign plugin information
    _Info->uPluginVer = SQSQLITE_VERSION;
    strcpy(_Info->szName, SQSQLITE_HOST_NAME);
    // Bind callbacks
    BindCallbacks();
    // Notify that the plugin was successfully loaded
    OutputMessage("Successfully loaded %s", SQSQLITE_NAME);
    // Dummy spacing
    puts("");
    // Done!
    return SQMOD_SUCCESS;
}
