#include "Library/SQLite/Statement.hpp"
#include "Library/SQLite/Connection.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqstdblob.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace SQLite {

// ------------------------------------------------------------------------------------------------
Statement::Statement()
    : m_Handle(NULL), m_Connection(), m_Query(), m_Status(SQLITE_OK), m_Columns(0)
    , m_ColIdx(), m_Good(), m_Done(), m_Tag(), m_Data()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Statement::Statement(const Connection & db, const SQChar * query)
    : m_Handle(NULL), m_Connection(db), m_Query(query), m_Status(SQLITE_OK), m_Columns(0)
    , m_ColIdx(), m_Good(), m_Done(), m_Tag(), m_Data()
{
    // See if a valid connection was specified
    if (!m_Connection)
    {
        LogErr("Attempting to <prepare statement> without a valid connection handle");
    }
    // See if the specified query is valid
    else if (m_Query.empty())
    {
        LogErr("Attempting to <prepare statement> without a valid query string");
    }
    // Attempt to create the statement
    else if ((m_Status = sqlite3_prepare_v2(m_Connection, static_cast< const char * >(query),
                            _SCI32(m_Query.size() * sizeof(char)), &m_Handle, NULL)) != SQLITE_OK)
    {
        LogErr("Unable to <prepare statement> because : %s", GetErrMsg());
        m_Handle = NULL;
    }
    else
    {
        // Get the number of columns in this statement
        m_Columns = sqlite3_column_count(m_Handle);
    }
}

// ------------------------------------------------------------------------------------------------
Statement::~Statement()
{
    // If a valid handle exists then attempt to release it
    if (m_Handle != NULL && (m_Status = sqlite3_finalize(m_Handle)) != SQLITE_OK)
    {
        LogErr("Unable to <finalize statement> because : %s", GetErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
SQInteger Statement::Cmp(const Statement & o) const
{
    if (m_Handle == o.m_Handle)
    {
        return 0;
    }
    else if (m_Handle > o.m_Handle)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * Statement::ToString() const
{
    return m_Query.c_str();
}

// ------------------------------------------------------------------------------------------------
const SQChar * Statement::GetLocalTag() const
{
    return m_Tag.c_str();
}

void Statement::SetLocalTag(const SQChar * tag)
{
    m_Tag = tag;
}

// ------------------------------------------------------------------------------------------------
SqObj & Statement::GetLocalData()
{
    return m_Data;
}

void Statement::SetLocalData(SqObj & data)
{
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
void Statement::Reset()
{
    if (m_Handle != NULL)
    {
        if ((m_Status = sqlite3_reset(m_Handle)) != SQLITE_OK)
        {
            LogErr("Unable to <reset statement> because : %s", GetErrMsg());
        }
    }
    else
    {
        LogWrn("Attempting to <reset statement> using an invalid reference");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::Clear()
{
    if (m_Handle != NULL)
    {
        if ((m_Status = sqlite3_clear_bindings(m_Handle)) != SQLITE_OK)
        {
            LogErr("Unable to <clear statement> because : %s", GetErrMsg());
        }
    }
    else
    {
        LogWrn("Attempting to <clear statement> using an invalid reference");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 Statement::GetStatus() const
{
    return m_Status;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Statement::GetColumns() const
{
    return m_Columns;
}

// ------------------------------------------------------------------------------------------------
const SQChar * Statement::GetQuery() const
{
    return m_Query.c_str();
}

// ------------------------------------------------------------------------------------------------
const SQChar * Statement::GetErrStr() const
{
    return sqlite3_errstr(m_Status);
}

// ------------------------------------------------------------------------------------------------
const SQChar * Statement::GetErrMsg() const
{
    if (m_Connection)
    {
        return m_Connection.ErrMsg();
    }
    else
    {
        LogWrn("Attempting to <get connection error message> using an invalid connection: null");
    }

    return _SC("");
}

// ------------------------------------------------------------------------------------------------
Connection Statement::GetConnection() const
{
    return Connection(m_Connection);
}

// ------------------------------------------------------------------------------------------------
bool Statement::GetGood() const
{
    return m_Good;
}

// ------------------------------------------------------------------------------------------------
bool Statement::GetDone() const
{
    return m_Done;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Statement::GetErrorCode() const
{
    if (m_Connection)
    {
        return sqlite3_errcode(m_Connection);
    }
    else
    {
        LogWrn("Attempting to <get database error code> using an invalid connection: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Statement::GetExtendedErrorCode() const
{
    if (m_Connection)
    {
        return sqlite3_extended_errcode(m_Connection);
    }
    else
    {
        LogWrn("Attempting to <get database extended error code> using an invalid connection: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Statement::Exec()
{
    if (m_Handle != NULL && !m_Done)
    {
        m_Status = sqlite3_step(m_Handle);

        switch (m_Status)
        {
            case SQLITE_DONE:
                m_Good = false;
                m_Done = true;
                return sqlite3_changes(m_Connection);
            case SQLITE_ROW:
                LogErr("Unable to <execute statement> because results were found");
            break;
            case SQLITE_BUSY:
                LogErr("Unable to <execute statement> while database is busy");
            break;
            case SQLITE_ERROR:
                LogErr("Unable to <execute statement> due to a runtime error");
            break;
            case SQLITE_MISUSE:
                LogErr("Unable to <execute statement> due to statement misuse");
            break;
            default:
                LogErr("Unable to <execute statement> for unforeseen reasons");
        }

        m_Good = false;
        m_Done = false;
    }
    else if (m_Handle == NULL)
    {
        LogWrn("Attempting to <execute statement> using an invalid statement: null");
    }
    else
    {
        LogErr("Attempting to <execute statement> without resetting first");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
bool Statement::Step()
{
    if (m_Handle != NULL && !m_Done)
    {
        m_Status = sqlite3_step(m_Handle);

        switch (m_Status)
        {
            case SQLITE_ROW:
                m_Good = true;
                return true;
            case SQLITE_DONE:
                m_Good = false;
                m_Done = true;
                return false;
            case SQLITE_BUSY:
                LogErr("Unable to <step statement> while database is busy");
            break;
            case SQLITE_ERROR:
                LogErr("Unable to <step statement> due to a runtime error");
            break;
            case SQLITE_MISUSE:
                LogErr("Unable to <step statement> due to statement misuse");
            break;
            default:
                LogErr("Unable to <step statement> for unforeseen reasons");
        }

        m_Good = false;
        m_Done = false;
    }
    else if (m_Handle == NULL)
    {
        LogWrn("Attempting to <step statement> using an invalid statement: null");
    }
    else
    {
        LogErr("Attempting to <step statement> without resetting first");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindA(const Array & arr)
{
    SQMOD_UNUSED_VAR(arr);
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindT(const Table & tbl)
{
    SQMOD_UNUSED_VAR(tbl);
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindI(SQInt32 idx, SQInt32 value)
{
    if (m_Handle != NULL)
    {
        m_Status = sqlite3_bind_int(m_Handle, idx, value);
        Status("Unable to bind index (%d) to statement because: %s", idx, GetErrMsg());
    }
    else
    {
        LogWrn("Attempting to <bind statement index integer> using an invalid statement: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindL(SQInt32 idx, const SLongInt & value)
{
    if (m_Handle != NULL)
    {
        m_Status = sqlite3_bind_int64(m_Handle, idx, value);
        Status("Unable to bind index (%d) to statement because: %s", idx, GetErrMsg());
    }
    else
    {
        LogWrn("Attempting to <bind statement index long> using an invalid statement: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindV(SQInt32 idx, SQInteger value)
{
    if (m_Handle != NULL)
    {
#ifdef _SQ64
        m_Status = sqlite3_bind_int64(m_Handle, idx, value);
#else
        m_Status = sqlite3_bind_int(m_Handle, idx, value);
#endif
        Status("Unable to bind index (%d) to statement because: %s", idx, GetErrMsg());
    }
    else
    {
        LogWrn("Attempting to <bind statement index integer> using an invalid statement: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindF(SQInt32 idx, SQFloat value)
{
    if (m_Handle != NULL)
    {
        m_Status = sqlite3_bind_double(m_Handle, idx, value);
        Status("Unable to bind index (%d) to statement because: %s", idx, GetErrMsg());
    }
    else
    {
        LogWrn("Attempting to <bind statement index float> using an invalid statement: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindS(SQInt32 idx, const SQChar * value)
{
    if (m_Handle != NULL)
    {
        m_Status = sqlite3_bind_text(m_Handle, idx, value, _SCI32(strlen(value)), SQLITE_TRANSIENT);
        Status("Unable to bind index (%d) to statement because: %s", idx, GetErrMsg());
    }
    else
    {
        LogWrn("Attempting to <bind statement index string> using an invalid statement: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindB(SQInt32 idx, bool value)
{
    if (m_Handle != NULL)
    {
        m_Status = sqlite3_bind_int(m_Handle, idx, value);
        Status("Unable to bind index (%d) to statement because: %s", idx, GetErrMsg());
    }
    else
    {
        LogWrn("Attempting to <bind statement index boolean> using an invalid statement: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindN(SQInt32 idx)
{
    if (m_Handle != NULL)
    {
        m_Status = sqlite3_bind_null(m_Handle, idx);
        Status("Unable to bind index (%d) to statement because: %s", idx, GetErrMsg());
    }
    else
    {
        LogWrn("Attempting to <bind statement index null> using an invalid statement: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindT(const Table & tbl)
{
    SQMOD_UNUSED_VAR(tbl);
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindI(const SQChar * name, SQInt32 value)
{
    if (m_Handle != NULL)
    {
        const int idx = sqlite3_bind_parameter_index(m_Handle, name);
        m_Status = sqlite3_bind_int(m_Handle, idx, value);
        Status("Unable to bind name (%s) index (%d) to statement because: %s", name, idx, GetErrMsg());
    }
    else
    {
        LogWrn("Attempting to <bind statement name integer> using an invalid statement: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindL(const SQChar * name, const SLongInt & value)
{
    if (m_Handle != NULL)
    {
        const int idx = sqlite3_bind_parameter_index(m_Handle, name);
        m_Status = sqlite3_bind_int64(m_Handle, idx, value);
        Status("Unable to bind name (%s) index (%d) to statement because: %s", name, idx, GetErrMsg());
    }
    else
    {
        LogWrn("Attempting to <bind statement name long> using an invalid statement: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindV(const SQChar * name, SQInteger value)
{
    if (m_Handle != NULL)
    {
        const int idx = sqlite3_bind_parameter_index(m_Handle, name);
#ifdef _SQ64
        m_Status = sqlite3_bind_int64(m_Handle, idx, value);
#else
        m_Status = sqlite3_bind_int(m_Handle, idx, value);
#endif
        Status("Unable to bind name (%s) index (%d) to statement because: %s", name, idx, GetErrMsg());
    }
    else
    {
        LogWrn("Attempting to <bind statement name integer> using an invalid statement: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindF(const SQChar * name, SQFloat value)
{
    if (m_Handle != NULL)
    {
        const int idx = sqlite3_bind_parameter_index(m_Handle, name);
        m_Status = sqlite3_bind_double(m_Handle, idx, value);
        Status("Unable to bind name (%s) index (%d) to statement because: %s", name, idx, GetErrMsg());
    }
    else
    {
        LogWrn("Attempting to <bind statement name float> using an invalid statement: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindS(const SQChar * name, const SQChar * value)
{
    if (m_Handle != NULL)
    {
        const int idx = sqlite3_bind_parameter_index(m_Handle, name);
        m_Status = sqlite3_bind_text(m_Handle, idx, value, _SCI32(strlen(value)), SQLITE_TRANSIENT);
        Status("Unable to bind name (%s) index (%d) to statement because: %s", name, idx, GetErrMsg());
    }
    else
    {
        LogWrn("Attempting to <bind statement name string> using an invalid statement: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindB(const SQChar * name, bool value)
{
    if (m_Handle != NULL)
    {
        const int idx = sqlite3_bind_parameter_index(m_Handle, name);
        m_Status = sqlite3_bind_int(m_Handle, idx, value);
        Status("Unable to bind name (%s) index (%d) to statement because: %s", name, idx, GetErrMsg());
    }
    else
    {
        LogWrn("Attempting to <bind statement name boolean> using an invalid statement: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindN(const SQChar * name)
{
    if (m_Handle != NULL)
    {
        const int idx = sqlite3_bind_parameter_index(m_Handle, name);
        m_Status = sqlite3_bind_null(m_Handle, idx);
        Status("Unable to bind name (%s) index (%d) to statement because: %s", name, idx, GetErrMsg());
    }
    else
    {
        LogWrn("Attempting to <bind statement name null> using an invalid statement: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBind(SQInt32 idx, SqObj & value)
{
    if (m_Handle != NULL)
    {
        switch (value.GetType())
        {
            case OT_INTEGER:
#ifdef _SQ64
                m_Status = sqlite3_bind_int64(m_Handle, idx, value.Cast< SQInteger >());
#else
                m_Status = sqlite3_bind_int(m_Handle, idx, value.Cast< SQInteger >());
#endif
            break;
            case OT_FLOAT:
                m_Status = sqlite3_bind_double(m_Handle, idx, value.Cast< SQFloat >());
            break;
            case OT_BOOL:
                m_Status = sqlite3_bind_int(m_Handle, idx, value.Cast< bool >());
            break;
            case OT_STRING:
                {
                    const SQChar * str = value.Cast< const SQChar * >();
                    m_Status = sqlite3_bind_text(m_Handle, idx, str, _SCI32(strlen(str)), SQLITE_TRANSIENT);
                }
            break;
            case OT_NULL:
                m_Status = sqlite3_bind_null(m_Handle, idx);
            break;
            default:
                LogErr("Attempting to <bind statement index value> using an unknown value");
                return;
            break;
        }

        Status("Unable to bind index (%d) to statement because: %s", idx, GetErrMsg());
    }
    else
    {
        LogWrn("Attempting to <bind statement index value> using an invalid statement: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBind(const SQChar * name, SqObj & value)
{
    if (m_Handle != NULL)
    {
        const int idx = sqlite3_bind_parameter_index(m_Handle, name);

        if (idx == 0)
        {
            LogErr("Attempting to <bind statement name value> using an unknown name: %s", name);
        }

        switch (value.GetType())
        {
            case OT_INTEGER:
#ifdef _SQ64
                m_Status = sqlite3_bind_int64(m_Handle, idx, value.Cast< SQInteger >());
#else
                m_Status = sqlite3_bind_int(m_Handle, idx, value.Cast< SQInteger >());
#endif
            break;
            case OT_FLOAT:
                m_Status = sqlite3_bind_double(m_Handle, idx, value.Cast< SQFloat >());
            break;
            case OT_BOOL:
                m_Status = sqlite3_bind_int(m_Handle, idx, value.Cast< bool >());
            break;
            case OT_STRING:
                {
                    const SQChar * str = value.Cast< const SQChar * >();
                    m_Status = sqlite3_bind_text(m_Handle, idx, str, _SCI32(strlen(str)), SQLITE_TRANSIENT);
                }
            break;
            case OT_NULL:
                m_Status = sqlite3_bind_null(m_Handle, idx);
            break;
            default:
                LogErr("Attempting to <bind statement name value> using an unknown value");
                return;
            break;
        }

        Status("Unable to bind name (%s) index (%d) to statement because: %s", name, idx, GetErrMsg());
    }
    else
    {
        LogWrn("Attempting to <bind statement name value> using an invalid statement: null");
    }
}

// ------------------------------------------------------------------------------------------------
SqObj Statement::FetchColumnIndex(SQInt32 idx)
{
    if (m_Handle != NULL && m_Good && CheckIndex(idx))
    {
        switch (sqlite3_column_type(m_Handle, idx))
        {
            case SQLITE_NULL:
                // Ignore the index and leave it null
                break;
            case SQLITE_INTEGER:
#ifdef _SQ64
                return MakeSqObj< SQInteger >(sqlite3_column_int64(m_Handle, idx));
#else
                return MakeSqObj< SQInteger >(sqlite3_column_int(m_Handle, idx));
#endif
            case SQLITE_FLOAT:
                return MakeSqObj< SQFloat >(sqlite3_column_double(m_Handle, idx));
            case SQLITE_TEXT:
                return MakeSqObj< const SQChar * >(reinterpret_cast< const SQChar * >
                                                    (sqlite3_column_text(m_Handle, idx)));
            case SQLITE_BLOB:
                {
                    // Get the sqlite blob size
                    const SQInt32 sz = sqlite3_column_bytes(m_Handle, idx);
                    // Create another blob in squirrel with the same size and get it's address
                    SQUserPointer p = sqstd_createblob(Sqrat::DefaultVM::Get(), sz);
                    // Get the address of the sqlite blob
                    const void * b = sqlite3_column_blob(m_Handle, idx);
                    // See if the squirrel blob could be created
                    if (p == NULL)
                    {
                        LogErr("Unable to allocate space for column blob value");
                    }
                    // See if the sqlite blob could be retrieved
                    else if (b == NULL)
                    {
                        LogErr("Unable to read data from column blob value");
                    }
                    // Create the squirrel object
                    else
                    {
                        // Copy the data from sqlite blob to squirrel blob
                        std::memcpy(p, b, sz);
                        // Retrieve the blob object from the stack
                        Var< SqObj > var(Sqrat::DefaultVM::Get(), -1);
                        // Pop the blob object from the stack
                        sq_pop(Sqrat::DefaultVM::Get(), 1);
                        // Return the blob object
                        return var.value;
                    }
                }
            default:
                LogErr("Unknown sqlite value to retrieve at index: %d", idx);
        }
    }
    else if (!m_Good)
    {
        LogErr("Attempting to <fetch statement index value> without a valid row available");
    }
    else if (!CheckIndex(idx))
    {
        LogErr("Attempting to <fetch statement index value> using an out of bounds index: %d", idx);
    }
    else
    {
        LogWrn("Attempting to <fetch statement index value> using an invalid statement: null");
    }

    return Object();
}

// ------------------------------------------------------------------------------------------------
SqObj Statement::FetchColumnName(const SQChar * name)
{
    const int idx = GetColumnIndex(name);

    if (m_Handle != NULL && m_Good && CheckIndex(idx))
    {
        switch (sqlite3_column_type(m_Handle, idx))
        {
            case SQLITE_NULL:
                // Ignore the index and leave it null
                break;
            case SQLITE_INTEGER:
#ifdef _SQ64
                return MakeSqObj< SQInteger >(sqlite3_column_int64(m_Handle, idx));
#else
                return MakeSqObj< SQInteger >(sqlite3_column_int(m_Handle, idx));
#endif
            case SQLITE_FLOAT:
                return MakeSqObj< SQFloat >(sqlite3_column_double(m_Handle, idx));
            case SQLITE_TEXT:
                return MakeSqObj< const SQChar * >(reinterpret_cast< const SQChar * >
                                                    (sqlite3_column_text(m_Handle, idx)));
            case SQLITE_BLOB:
                {
                    // Get the sqlite blob size
                    const SQInt32 sz = sqlite3_column_bytes(m_Handle, idx);
                    // Create another blob in squirrel with the same size and get it's address
                    SQUserPointer p = sqstd_createblob(Sqrat::DefaultVM::Get(), sz);
                    // Get the address of the sqlite blob
                    const void * b = sqlite3_column_blob(m_Handle, idx);
                    // See if the squirrel blob could be created
                    if (p == NULL)
                    {
                        LogErr("Unable to allocate space for column blob value");
                    }
                    // See if the sqlite blob could be retrieved
                    else if (b == NULL)
                    {
                        LogErr("Unable to read data from column blob value");
                    }
                    // Create the squirrel object
                    else
                    {
                        // Copy the data from sqlite blob to squirrel blob
                        std::memcpy(p, b, sz);
                        // Retrieve the blob object from the stack
                        Var< SqObj > var(Sqrat::DefaultVM::Get(), -1);
                        // Pop the blob object from the stack
                        sq_pop(Sqrat::DefaultVM::Get(), 1);
                        // Return the blob object
                        return var.value;
                    }
                }
            default:
                LogErr("Unknown sqlite value to retrieve at index: %d", idx);
        }
    }
    else if (!m_Good)
    {
        LogErr("Attempting to <fetch statement name value> without a valid row available");
    }
    else if (!CheckIndex(idx))
    {
        LogErr("Attempting to <fetch statement name value> using an out of bounds index: %d", idx);
    }
    else
    {
        LogWrn("Attempting to <fetch statement name value> using an invalid statement: null");
    }

    return Object();
}

// ------------------------------------------------------------------------------------------------
Array Statement::FetchArray()
{
    if (m_Handle != NULL && m_Good)
    {
        // Create an array with the same size as the number of columns
        Array arr(Sqrat::DefaultVM::Get(), m_Columns);
        // Process each cell in the row
        for (SQInt32 idx = 0; idx < m_Columns; ++idx)
        {
            switch (sqlite3_column_type(m_Handle, idx))
            {
                case SQLITE_NULL:
                    // Ignore the index and leave it null
                break;
                case SQLITE_INTEGER:
    #ifdef _SQ64
                    arr.SetValue(idx, sqlite3_column_int64(m_Handle, idx));
    #else
                    arr.SetValue(idx, sqlite3_column_int(m_Handle, idx));
    #endif
                break;
                case SQLITE_FLOAT:
                    arr.SetValue(idx, sqlite3_column_double(m_Handle, idx));
                break;
                case SQLITE_TEXT:
                    arr.SetValue(idx, reinterpret_cast< const SQChar * >
                                                        (sqlite3_column_text(m_Handle, idx)));
                break;
                case SQLITE_BLOB:
                    {
                        // Get the sqlite blob size
                        const SQInt32 sz = sqlite3_column_bytes(m_Handle, idx);
                        // Create another blob in squirrel with the same size and get it's address
                        SQUserPointer p = sqstd_createblob(Sqrat::DefaultVM::Get(), sz);
                        // Get the address of the sqlite blob
                        const void * b = sqlite3_column_blob(m_Handle, idx);
                        // See if the squirrel blob could be created
                        if (p == NULL)
                        {
                            LogErr("Unable to allocate space for column blob value");
                        }
                        // See if the sqlite blob could be retrieved
                        else if (b == NULL)
                        {
                            LogErr("Unable to read data from column blob value");
                        }
                        // Create the squirrel object
                        else
                        {
                            // Copy the data from sqlite blob to squirrel blob
                            std::memcpy(p, b, sz);
                            // Retrieve the blob object from the stack
                            Var< SqObj > var(Sqrat::DefaultVM::Get(), -1);
                            // Add the blob object to the array
                            arr.Bind(idx, var.value);
                        }
                        // Pop the blob object from the stack
                        sq_pop(Sqrat::DefaultVM::Get(), 1);
                    }
                break;
                default:
                    LogErr("Unknown sqlite value to retrieve at index: %d", idx);
            }
        }
        // Return the resulted array
        return arr;
    }
    else if (!m_Good)
    {
        LogErr("Attempting to <fetch statement array value> without a valid row available");
    }
    else
    {
        LogWrn("Attempting to <fetch statement array value> using an invalid statement: null");
    }

    return Array();
}

// ------------------------------------------------------------------------------------------------
Table Statement::FetchTable()
{
    if (m_Handle != NULL && m_Good)
    {
        // Create an empty table
        Table tbl(Sqrat::DefaultVM::Get());
        // Process each cell in the row
        for (SQInt32 idx = 0; idx < m_Columns; ++idx)
        {
            switch (sqlite3_column_type(m_Handle, idx))
            {
                case SQLITE_NULL:
                    tbl.Bind(sqlite3_column_name(m_Handle, idx), NullData());
                break;
                case SQLITE_INTEGER:
    #ifdef _SQ64
                    tbl.SetValue(sqlite3_column_name(m_Handle, idx), sqlite3_column_int64(m_Handle, idx));
    #else
                    tbl.SetValue(sqlite3_column_name(m_Handle, idx), sqlite3_column_int(m_Handle, idx));
    #endif
                break;
                case SQLITE_FLOAT:
                    tbl.SetValue(sqlite3_column_name(m_Handle, idx), sqlite3_column_double(m_Handle, idx));
                break;
                case SQLITE_TEXT:
                    tbl.SetValue(sqlite3_column_name(m_Handle, idx), reinterpret_cast< const SQChar * >
                                                                        (sqlite3_column_text(m_Handle, idx)));
                break;
                case SQLITE_BLOB:
                    {
                        // Get the sqlite blob size
                        const SQInt32 sz = sqlite3_column_bytes(m_Handle, idx);
                        // Create another blob in squirrel with the same size and get it's address
                        SQUserPointer p = sqstd_createblob(Sqrat::DefaultVM::Get(), sz);
                        // Get the address of the sqlite blob
                        const void * b = sqlite3_column_blob(m_Handle, idx);
                        // See if the squirrel blob could be created
                        if (p == NULL)
                        {
                            LogErr("Unable to allocate space for column blob value");
                        }
                        // See if the sqlite blob could be retrieved
                        else if (b == NULL)
                        {
                            LogErr("Unable to read data from column blob value");
                        }
                        // Create the squirrel object
                        else
                        {
                            // Copy the data from sqlite blob to squirrel blob
                            std::memcpy(p, b, sz);
                            // Retrieve the blob object from the stack
                            Var< SqObj > var(Sqrat::DefaultVM::Get(), -1);
                            // Add the blob object to the table
                            tbl.Bind(sqlite3_column_name(m_Handle, idx), var.value);
                        }
                        // Pop the blob object from the stack
                        sq_pop(Sqrat::DefaultVM::Get(), 1);
                    }
                break;
                default:
                    LogErr("Unknown sqlite value to retrieve at index: %d", idx);
            }
        }
        // Return the resulted table
        return tbl;
    }
    else if (!m_Good)
    {
        LogErr("Attempting to <fetch statement table value> without a valid row available");
    }
    else
    {
        LogWrn("Attempting to <fetch statement table value> using an invalid statement: null");
    }

    return Table();
}

// ------------------------------------------------------------------------------------------------
bool Statement::CheckIndex(SQInt32 idx) const
{
    return (idx >= 0) && (idx < m_Columns);
}

// ------------------------------------------------------------------------------------------------
bool Statement::IsColumnNull(SQInt32 idx) const
{
    if (m_Handle != NULL && CheckIndex(idx))
    {
        return (sqlite3_column_type(m_Handle, idx) == SQLITE_NULL);
    }
    else if (!CheckIndex(idx))
    {
        LogErr("Attempting to <see if statement column is null> using an out of bounds index: %d", idx);
    }
    else
    {
        LogWrn("Attempting to <see if statement column is null> using an invalid statement: null");
    }

    return true;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Statement::GetColumnIndex(const SQChar * name)
{
    if (m_Handle != NULL && m_Good)
    {
        if (m_ColIdx.empty())
        {
            for (SQInt32 i = 0; i < m_Columns; ++i)
            {
                const char * name = sqlite3_column_name(m_Handle, i);
                if (name == NULL)
                {
                    LogErr("Unable to retrieve column name for index: %d", i);
                }
                m_ColIdx.emplace(name, i);
            }
        }
        // Attempt to find the specified key
        const ColumnIndex::const_iterator itr = m_ColIdx.find(name);
        // See if it could be found
        if (itr == m_ColIdx.cend())
        {
            LogErr("Unknown column name: %s", name);
        }
        // Return the associated index
        return itr->second;
    }
    else if (!m_Good)
    {
        LogErr("Attempting to <get statement column index> without a valid row available");
    }
    else
    {
        LogWrn("Attempting to <get statement column index> using an invalid statement: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
const SQChar * Statement::GetColumnName(SQInt32 idx)
{
    if (m_Handle != NULL && CheckIndex(idx))
    {
        return sqlite3_column_name(m_Handle, idx);
    }
    else if (!CheckIndex(idx))
    {
        LogErr("Attempting to <get statement column name> using an out of bounds index: %d", idx);
    }
    else
    {
        LogWrn("Attempting to <get statement column name> using an invalid statement: null");
    }

    return _SC("");
}

// ------------------------------------------------------------------------------------------------
const SQChar * Statement::GetColumnOriginName(SQInt32 idx)
{
#ifdef SQLITE_ENABLE_COLUMN_METADATA
    if (m_Handle != NULL && CheckIndex(idx))
    {
        return sqlite3_column_origin_name(m_Handle, idx);
    }
    else if (!CheckIndex(idx))
    {
        LogErr("Attempting to <get statement column origin name> using an out of bounds index: %d", idx);
    }
    else
    {
        LogWrn("Attempting to <get statement column origin name> using an invalid statement: null");
    }
#else
    SQMOD_UNUSED_VAR(idx);
    LogErr("The module was compiled without this feature");
#endif
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
SQInteger Statement::GetColumnType(SQInt32 idx) const
{
    if (m_Handle != NULL && CheckIndex(idx))
    {
        return sqlite3_column_type(m_Handle, idx);
    }
    else if (!CheckIndex(idx))
    {
        LogErr("Attempting to <get statement column type> using an out of bounds index: %d", idx);
    }
    else
    {
        LogWrn("Attempting to <get statement column type> using an invalid statement: null");
    }

    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger Statement::GetColumnBytes(SQInt32 idx) const
{
    if (m_Handle != NULL && CheckIndex(idx))
    {
        return sqlite3_column_bytes(m_Handle, idx);
    }
    else if (!CheckIndex(idx))
    {
        LogErr("Attempting to <get statement column bytes> using an out of bounds index: %d", idx);
    }
    else
    {
        LogWrn("Attempting to <get statement column bytes> using an invalid statement: null");
    }

    return 0;
}

} // Namespace:: SQLite
} // Namespace:: SqMod
