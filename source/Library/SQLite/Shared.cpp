#include "Library/SQLite/Shared.hpp"
#include "Library/SQLite/Connection.hpp"
#include "Library/SQLite/Statement.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace SQLite {

// ------------------------------------------------------------------------------------------------
ConnectionHandle::ConnectionHandle()
    : Hnd(nullptr)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
ConnectionHandle::ConnectionHandle(const SQChar * path, SQInt32 flags, const SQChar * vfs)
    : Hnd(new Handle(path, flags, vfs != NULL ? vfs : _SC("")))
{
    // See if a handle could be created
    if (!Hnd)
    {
        LogErr("Attempting to <open database connection> without a valid handle");
        Drop();
    }
    // At least the path must be valid
    else if (Hnd->Path.empty())
    {
        LogErr("Attempting to <open database connection> without a valid path");
        Drop();
    }
    // Attempt to open a connection
    else if ((Hnd->Status = sqlite3_open_v2(path, &(*Hnd).Ptr, flags, vfs)) != SQLITE_OK)
    {
        LogErr("Unable to <open database> because : %d", sqlite3_errmsg(Hnd->Ptr));
        Drop();
    }
}

// ------------------------------------------------------------------------------------------------
ConnectionHandle::ConnectionHandle(const ConnectionHandle & o)
    : Hnd(o.Hnd)
{
    // Grab a reference to the new handle
    Grab();
}

// ------------------------------------------------------------------------------------------------
ConnectionHandle::ConnectionHandle(ConnectionHandle && o)
    : Hnd(o.Hnd)
{
    // Prevent the other instance from releasing the stolen reference
    o.Hnd = nullptr;
}

// ------------------------------------------------------------------------------------------------
ConnectionHandle::~ConnectionHandle()
{
    // Drop the current handle reference if any
    Drop();
}

// ------------------------------------------------------------------------------------------------
ConnectionHandle & ConnectionHandle::operator = (const ConnectionHandle & o)
{
    // Make sure it's not the same handle instance in both
    if (Hnd != o.Hnd)
    {
        // Drop the current handle reference if any
        Drop();
        // Assign the specified handle
        Hnd = o.Hnd;
        // Grab a reference to the new handle
        Grab();
    }

    return *this;
}

// ------------------------------------------------------------------------------------------------
ConnectionHandle & ConnectionHandle::operator = (ConnectionHandle && o)
{
    // Make sure it's not the same handle instance in both
    if (Hnd != o.Hnd)
    {
        // Drop the current handle reference if any
        Drop();
        // Steal the handle reference from the other instance
        Hnd = o.Hnd;
        // Prevent the other instance from releasing the stolen reference
        o.Hnd = nullptr;
    }

    return *this;
}

// ------------------------------------------------------------------------------------------------
Transaction::Transaction(const Connection & db)
    : m_Connection(db), m_Commited(false)
{

}

// ------------------------------------------------------------------------------------------------
Transaction::~Transaction()
{
    if (!m_Commited && m_Connection)
    {
        if ((m_Connection->Status = sqlite3_exec(m_Connection, "ROLLBACK", NULL, NULL, NULL)) != SQLITE_OK)
        {
            LogErr("Unable to <rollback transaction> because: %s", m_Connection.ErrMsg());
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Transaction::Commit()
{
    if (!m_Commited && m_Connection)
    {
        if ((m_Connection->Status = sqlite3_exec(m_Connection, "COMMIT", NULL, NULL, NULL)) != SQLITE_OK)
        {
            LogErr("Unable to <commit transaction> because: %s", m_Connection.ErrMsg());
        }
        else
        {
            m_Commited = true;
        }
    }
    else if (m_Commited)
    {
        LogWrn("Attempting to <commit transaction> which was already committed");
    }
    else
    {
        LogWrn("Attempting to <commit transaction> using an invalid connection");
    }
}

// ------------------------------------------------------------------------------------------------
void SetSoftHeapLimit(SQInt32 limit)
{
    sqlite3_soft_heap_limit(limit);
}

// ------------------------------------------------------------------------------------------------
SQInt32 ReleaseMemory(SQInt32 bytes)
{
    return sqlite3_release_memory(bytes);
}

// ------------------------------------------------------------------------------------------------
SLongInt GetMemoryUsage()
{
    return SLongInt(static_cast< SLongInt::Type >(sqlite3_memory_used()));
}

// ------------------------------------------------------------------------------------------------
SLongInt GetMemoryHighwaterMark(bool reset)
{
    return SLongInt(static_cast< SLongInt::Type >(sqlite3_memory_highwater(reset)));
}

} // Namespace:: SQLite

// ================================================================================================
bool Register_SQLite(HSQUIRRELVM vm)
{
    using namespace SQLite;

    // Attempt to create the SQLite namespace
    Sqrat::Table sqlns(vm);

    // Output debugging information
    LogDbg("Beginning registration of <SQLite Connection> type");
    // Attempt to register the specified type
    sqlns.Bind(_SC("Connection"), Sqrat::Class< Connection >(vm, _SC("Connection"))
        /* Constructors */
        .Ctor()
        .Ctor< const SQChar * >()
        .Ctor< const SQChar *, SQInt32 >()
        .Ctor< const SQChar *, SQInt32, const SQChar * >()
        /* Metamethods */
        .Func(_SC("_cmp"), &Connection::Cmp)
        .Func(_SC("_tostring"), &Connection::ToString)
        /* Properties */
        .Prop(_SC("gtag"), &Connection::GetGlobalTag, &Connection::SetGlobalTag)
        .Prop(_SC("gdata"), &Connection::GetGlobalData, &Connection::SetGlobalData)
        .Prop(_SC("ltag"), &Connection::GetLocalTag, &Connection::SetLocalTag)
        .Prop(_SC("ldata"), &Connection::GetLocalData, &Connection::SetLocalData)
        .Prop(_SC("refs"), &Connection::GetRefs)
        .Prop(_SC("valid"), &Connection::IsValid)
        .Prop(_SC("connected"), &Connection::IsValid)
        .Prop(_SC("status"), &Connection::GetStatus)
        .Prop(_SC("flags"), &Connection::GetFlags)
        .Prop(_SC("path"), &Connection::GetPath)
        .Prop(_SC("vfs"), &Connection::GetVFS)
        .Prop(_SC("err_str"), &Connection::GetErrStr)
        .Prop(_SC("err_msg"), &Connection::GetErrMsg)
        .Prop(_SC("read_only"), &Connection::IsReadOnly)
        .Prop(_SC("auto_commit"), &Connection::GetAutoCommit)
        .Prop(_SC("last_insert_row_id"), &Connection::GetLastInsertRowID)
        .Prop(_SC("changes"), &Connection::GetChanges)
        .Prop(_SC("total_changes"), &Connection::GetTotalChanges)
        .Prop(_SC("err_code"), &Connection::GetErrorCode)
        .Prop(_SC("extended_err_code"), &Connection::GetExtendedErrorCode)
        .Prop(_SC("trace"), &Connection::GetTracing, &Connection::SetTracing)
        .Prop(_SC("profile"), &Connection::GetProfiling, &Connection::SetProfiling)
        .Prop(_SC("busy_timeout"), &Connection::SetBusyTimeout)
        .Prop(_SC("number_of_checked_out_lookaside_memory_slots"), &Connection::GetNumberOfCheckedOutLookasideMemorySlots)
        .Prop(_SC("heap_memory_used_by_pager_caches"), &Connection::GetHeapMemoryUsedByPagerCaches)
        .Prop(_SC("heap_memory_used_to_store_schemas"), &Connection::GetHeapMemoryUsedToStoreSchemas)
        .Prop(_SC("heap_and_lookaside_memory_used_by_prepared_statements"), &Connection::GetHeapAndLookasideMemoryUsedByPreparedStatements)
        .Prop(_SC("pagercache_hit_count"), &Connection::GetPagerCacheHitCount)
        .Prop(_SC("pagercache_miss_count"), &Connection::GetPagerCacheMissCount)
        .Prop(_SC("number_of_dirty_cache_entries"), &Connection::GetNumberOfDirtyCacheEntries)
        .Prop(_SC("number_of_unresolved_foreign_keys"), &Connection::GetNumberOfUnresolvedForeignKeys)
        .Prop(_SC("highest_number_of_checked_out_lookaside_memory_slots"), &Connection::GetHighestNumberOfCheckedOutLookasideMemorySlots)
        .Prop(_SC("lookaside_memory_hit_count"), &Connection::GetLookasideMemoryHitCount)
        .Prop(_SC("lookaside_memory_miss_count_due_to_small_slot_size"), &Connection::GetLookasideMemoryMissCountDueToSmallSlotSize)
        .Prop(_SC("lookaside_memory_miss_count_due_to_full_memory"), &Connection::GetLookasideMemoryMissCountDueToFullMemory)
        /* Functions */
        .Func(_SC("release"), &Connection::Release)
        .Func(_SC("exec"), &Connection::Exec)
        .Func(_SC("queue"), &Connection::Queue)
        .Func(_SC("table_exists"), &Connection::TableExists)
        .Func(_SC("interrupt_operation"), &Connection::InterruptOperation)
        .Func(_SC("release_memory"), &Connection::ReleaseMemory)
        .Func(_SC("copy_to_memory"), &Connection::CopyToMemory)
        .Func(_SC("copy_to_database"), &Connection::CopyToDatabase)
    );
    // Output debugging information
    LogDbg("Registration of <SQLite Connection> type was successful");

    // Output debugging information
    LogDbg("Beginning registration of <SQLite Statement> type");
    // Attempt to register the specified type
    sqlns.Bind(_SC("Statement"), Sqrat::Class< Statement, NoCopy< Statement > >(vm, _SC("Statement"))
        /* Constructors */
        .Ctor()
        .Ctor< const Connection &, const SQChar * >()
        /* Metamethods */
        .Func(_SC("_cmp"), &Statement::Cmp)
        .Func(_SC("_tostring"), &Statement::ToString)
        /* Properties */
        .Prop(_SC("ltag"), &Statement::GetLocalTag, &Statement::SetLocalTag)
        .Prop(_SC("ldata"), &Statement::GetLocalData, &Statement::SetLocalData)
        .Prop(_SC("valid"), &Statement::IsValid)
        .Prop(_SC("status"), &Statement::GetStatus)
        .Prop(_SC("columns"), &Statement::GetColumns)
        .Prop(_SC("query"), &Statement::GetQuery)
        .Prop(_SC("err_str"), &Statement::GetErrStr)
        .Prop(_SC("err_msg"), &Statement::GetErrMsg)
        .Prop(_SC("connection"), &Statement::GetConnection)
        .Prop(_SC("good"), &Statement::GetGood)
        .Prop(_SC("done"), &Statement::GetDone)
        .Prop(_SC("err_code"), &Statement::GetErrorCode)
        .Prop(_SC("extended_err_code"), &Statement::GetExtendedErrorCode)
        /* Functions */
        .Func(_SC("reset"), &Statement::Reset)
        .Func(_SC("clear"), &Statement::Clear)
        .Func(_SC("exec"), &Statement::Exec)
        .Func(_SC("step"), &Statement::Step)
        .Func(_SC("ibind_a"), &Statement::IndexBindA)
        .Func(_SC("ibind_t"), &Statement::IndexBindT)
        .Func(_SC("ibind_i"), &Statement::IndexBindI)
        .Func(_SC("ibind_l"), &Statement::IndexBindL)
        .Func(_SC("ibind_v"), &Statement::IndexBindV)
        .Func(_SC("ibind_f"), &Statement::IndexBindF)
        .Func(_SC("ibind_s"), &Statement::IndexBindS)
        .Func(_SC("ibind_b"), &Statement::IndexBindB)
        .Func(_SC("ibind_n"), &Statement::IndexBindN)
        .Func(_SC("nbind_t"), &Statement::NameBindT)
        .Func(_SC("nbind_i"), &Statement::NameBindI)
        .Func(_SC("nbind_l"), &Statement::NameBindL)
        .Func(_SC("nbind_v"), &Statement::NameBindV)
        .Func(_SC("nbind_f"), &Statement::NameBindF)
        .Func(_SC("nbind_s"), &Statement::NameBindS)
        .Func(_SC("nbind_b"), &Statement::NameBindB)
        .Func(_SC("nbind_n"), &Statement::NameBindN)
        .Func(_SC("ibind"), &Statement::IndexBind)
        .Func(_SC("nbind"), &Statement::NameBind)
        .Func(_SC("fetchi"), &Statement::FetchColumnIndex)
        .Func(_SC("fetchn"), &Statement::FetchColumnName)
        .Func(_SC("fetcha"), &Statement::FetchArray)
        .Func(_SC("fetcht"), &Statement::FetchTable)
        .Func(_SC("is_column_null"), &Statement::IsColumnNull)
        .Func(_SC("column_name"), &Statement::GetColumnName)
        .Func(_SC("column_origin_name"), &Statement::GetColumnOriginName)
        .Func(_SC("column_type"), &Statement::GetColumnType)
        .Func(_SC("column_bytes"), &Statement::GetColumnBytes)
    );
    // Output debugging information
    LogDbg("Registration of <SQLite Statement> type was successful");

    // Output debugging information
    LogDbg("Beginning registration of <SQLite Transaction> type");
    // Attempt to register the specified type
    sqlns.Bind(_SC("Transaction"), Sqrat::Class< Transaction, NoCopy< Transaction > >(vm, _SC("Transaction"))
        /* Constructors */
        .Ctor< const Connection & >()
        /* Properties */
        .Prop(_SC("committed"), &Transaction::Commited)
        /* Functions */
        .Func(_SC("commit"), &Transaction::Commit)
    );
    // Output debugging information
    LogDbg("Registration of <SQLite Transaction> type was successful");

    // Output debugging information
    LogDbg("Beginning registration of <SQLite functions> type");
    // Attempt to register the specified functions
    sqlns.Func(_SC("set_soft_heap_limit"), &SetSoftHeapLimit);
    sqlns.Func(_SC("release_memory"), &ReleaseMemory);
    sqlns.Func(_SC("memory_usage"), &GetMemoryUsage);
    sqlns.Func(_SC("memory_highwater_mark"), &GetMemoryHighwaterMark);
    // Output debugging information
    LogDbg("Registration of <SQLite functions> type was successful");

    // Attempt to bind the namespace to the root table
    Sqrat::RootTable(vm).Bind(_SC("SQLite"), sqlns);

    // Output debugging information
    LogDbg("Beginning registration of <SQLite Constants> type");
/*
    // Attempt to register the SQLite constants enumeration
    Sqrat::ConstTable(vm).Enum(_SC("ESQLITE"), Sqrat::Enumeration(vm)
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
        .Const(_SC("NULL"),                             SQLITE_NULL)
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
*/
    // Output debugging information
    LogDbg("Registration of <SQLite Constants> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
