// ------------------------------------------------------------------------------------------------
#include "Statement.hpp"
#include "Connection.hpp"
#include "Column.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
// Error message when failed to bind value to parameter index.
#define SQMOD_BADPARAMI "Unable to bind [%s] parameter at (%d) because [%s]"
// Error message when failed to bind value to parameter name.
#define SQMOD_BADPARAMS "Unable to bind [%s] parameter at (%s:%d) because [%s]"

// ------------------------------------------------------------------------------------------------
#define FETCH_PARAMETER_NAME(V, N) /*
*/ const Int32 V = sqlite3_bind_parameter_index(m_Handle->mPtr, N); /*
*/ if (!V) /*
*/ { /*
*/  STHROWF("Unknown parameter named (%s)", N); /*
*/ } /*
*/

// ------------------------------------------------------------------------------------------------
SQInteger Statement::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqSQLiteStatement");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Statement::Validate(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference =>[%s:%d]", file, line);
    }
}
#else
void Statement::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Statement::ValidateCreated(CCStr file, Int32 line) const
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
void Statement::ValidateCreated() const
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
const StmtRef & Statement::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const StmtRef & Statement::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const StmtRef & Statement::GetCreated(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    return m_Handle;
}
#else
const StmtRef & Statement::GetCreated() const
{
    ValidateCreated();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Statement::ValidateColumn(Int32 idx, CCStr file, Int32 line) const
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
void Statement::ValidateColumn(Int32 idx) const
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
void Statement::ValidateParameter(Int32 idx, CCStr file, Int32 line) const
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
void Statement::ValidateParameter(Int32 idx) const
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
void Statement::ValidateRow(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    // Do we have any rows available?
    if (!m_Handle->mGood)
    {
        SqThrowF("No row available =>[%s:%d]", file, line);
    }
}
#else
void Statement::ValidateRow() const
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
Statement::Statement(const Connection & connection, CSStr query)
    : m_Handle(new StmtHnd(connection.GetHandle()))
{
    GET_VALID_HND(*this)->Create(query);
}

// ------------------------------------------------------------------------------------------------
Object Statement::GetConnection() const
{
    return Object(new Connection(GET_VALID_HND(*this)->mConn));
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::Reset()
{
    VALIDATE_CREATED_HND(*this);
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
Statement & Statement::Clear()
{
    VALIDATE_CREATED_HND(*this);
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
Int32 Statement::Exec()
{
    VALIDATE_CREATED_HND(*this);
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
        case SQLITE_BUSY:   STHROWF("Database was busy");
        case SQLITE_ERROR:  STHROWF("Runtime error occurred");
        case SQLITE_MISUSE: STHROWF("Statement misuse");
        default:            STHROWF("Unknown failure");
    }
    // Operation failed (shouldn't reach this point!)
    return -1;
}

// ------------------------------------------------------------------------------------------------
bool Statement::Step()
{
    VALIDATE_CREATED_HND(*this);
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
        case SQLITE_ERROR:  STHROWF("Runtime error occurred");
        case SQLITE_MISUSE: STHROWF("Statement misuse");
        default:            STHROWF("Unknown failure");
    }
    // Operation failed (shouldn't reach this point!)
    return false;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::IndexBindArray(Int32 idx, const Array & arr)
{
    VALIDATE_PARAMETER_HND(*this, idx);
    // Obtain a script iterator
    Array::iterator itr;
    // Process each element until _next returns null
    while (idx <= m_Handle->mParameters && arr.Next(itr))
    {
        IndexBind(idx++, Object(itr.getValue()));
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::IndexBindValue(Int32 idx, Int32 value)
{
    VALIDATE_PARAMETER_HND(*this, idx);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, idx, value);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMI, "value", idx, m_Handle->ErrMsg());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::IndexBindLong(Int32 idx, const Object & value)
{
    VALIDATE_PARAMETER_HND(*this, idx);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int64(m_Handle->mPtr, idx, FetchSLongObjVal(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMI, "long", idx, m_Handle->ErrMsg());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::IndexBindInteger(Int32 idx, SQInteger value)
{
    VALIDATE_PARAMETER_HND(*this, idx);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_integer(m_Handle->mPtr, idx, value);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMI, "integer", idx, m_Handle->ErrMsg());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::IndexBindFloat(Int32 idx, SQFloat value)
{
    VALIDATE_PARAMETER_HND(*this, idx);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_double(m_Handle->mPtr, idx, value);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMI, "float", idx, m_Handle->ErrMsg());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::IndexBindString(Int32 idx, CSStr value)
{
    VALIDATE_PARAMETER_HND(*this, idx);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_text(m_Handle->mPtr, idx, value, -1, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMI, "string", idx, m_Handle->ErrMsg());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::IndexBindBool(Int32 idx, bool value)
{
    VALIDATE_PARAMETER_HND(*this, idx);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, idx, value);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMI, "boolean", idx, m_Handle->ErrMsg());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::IndexBindNull(Int32 idx)
{
    VALIDATE_PARAMETER_HND(*this, idx);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_null(m_Handle->mPtr, idx);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMI, "null", idx, m_Handle->ErrMsg());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::NameBindTable(const Table & tbl)
{
    VALIDATE_CREATED_HND(*this);
    // Is there anything to bind?
    if (tbl.GetSize() <= 0)
    {
        return *this; // Nothing to bind!
    }
    // Obtain a table iterator
    Table::iterator itr;
    // Process each element until _next returns null
    while (tbl.Next(itr))
    {
        // Bind it to the associated key
        NameBind(itr.getName(), Object(itr.getValue()));
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::NameBindValue(CSStr name, Int32 value)
{
    VALIDATE_CREATED_HND(*this);
    // Attempt to obtain the index of the specified parameter name
    FETCH_PARAMETER_NAME(idx, name)
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, idx, value);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMS, "value", name, idx, m_Handle->ErrMsg());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::NameBindLong(CSStr name, const Object & value)
{
    VALIDATE_CREATED_HND(*this);
    // Attempt to obtain the index of the specified parameter name
    FETCH_PARAMETER_NAME(idx, name)
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, idx, FetchSLongObjVal(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMS, "long", name, idx, m_Handle->ErrMsg());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::NameBindInteger(CSStr name, SQInteger value)
{
    VALIDATE_CREATED_HND(*this);
    // Attempt to obtain the index of the specified parameter name
    FETCH_PARAMETER_NAME(idx, name)
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_integer(m_Handle->mPtr, idx, value);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMS, "integer", name, idx, m_Handle->ErrMsg());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::NameBindFloat(CSStr name, SQFloat value)
{
    VALIDATE_CREATED_HND(*this);
    // Attempt to obtain the index of the specified parameter name
    FETCH_PARAMETER_NAME(idx, name)
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_double(m_Handle->mPtr, idx, value);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMS, "float", name, idx, m_Handle->ErrMsg());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::NameBindString(CSStr name, CSStr value)
{
    VALIDATE_CREATED_HND(*this);
    // Attempt to obtain the index of the specified parameter name
    FETCH_PARAMETER_NAME(idx, name)
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_text(m_Handle->mPtr, idx, value, -1, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMS, "string", name, idx, m_Handle->ErrMsg());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::NameBindBool(CSStr name, bool value)
{
    VALIDATE_CREATED_HND(*this);
    // Attempt to obtain the index of the specified parameter name
    FETCH_PARAMETER_NAME(idx, name)
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, idx, value);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMS, "boolean", name, idx, m_Handle->ErrMsg());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::NameBindNull(CSStr name)
{
    VALIDATE_CREATED_HND(*this);
    // Attempt to obtain the index of the specified parameter name
    FETCH_PARAMETER_NAME(idx, name)
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_null(m_Handle->mPtr, idx);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMS, "null", name, idx, m_Handle->ErrMsg());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::IndexBind(Int32 idx, const Object & value)
{
    VALIDATE_PARAMETER_HND(*this, idx);
    // Attempt to identify the specified type
    switch (value.GetType())
    {
        // Is this a native integer value?
        case OT_INTEGER:
        {
            m_Handle->mStatus = sqlite3_bind_integer(m_Handle->mPtr, idx, value.Cast< SQInteger >());
        } break;
        // Is this a native float value?
        case OT_FLOAT:
        {
            m_Handle->mStatus = sqlite3_bind_double(m_Handle->mPtr, idx, value.Cast< SQFloat >());
        } break;
        // Is this a boolean value?
        case OT_BOOL:
        {
            m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, idx, value.Cast< bool >());
        } break;
        // Is this a string value?
        case OT_STRING:
        {
            m_Handle->mStatus = sqlite3_bind_text(m_Handle->mPtr, idx,
                                                    value.Cast< CSStr >(), -1, SQLITE_TRANSIENT);
        } break;
        // Is this a null value?
        case OT_NULL:
        {
            m_Handle->mStatus = sqlite3_bind_null(m_Handle->mPtr, idx);
        } break;
        // Is this an instance value? Attempt to treat it as a long integer
        case OT_INSTANCE:
        {
            m_Handle->mStatus = sqlite3_bind_int64(m_Handle->mPtr, idx, FetchSLongObjVal(value));
        } break;
        // Is this an array value?
        case OT_ARRAY:
        {
            IndexBindArray(idx, Array(value));
            // No point in checking the status here
            return *this;
        } break;
        // We don't recognize this kind of value!
        default: STHROWF("Attempting to bind unknown value type (%d)", idx);
    }
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMI, "auto", idx, m_Handle->ErrMsg());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::NameBind(CSStr name, const Object & value)
{
    VALIDATE_CREATED_HND(*this);
    // Attempt to obtain the index of the specified parameter name
    FETCH_PARAMETER_NAME(idx, name)
    // Attempt to identify the specified type
    switch (value.GetType())
    {
        // Is this a native integer value?
        case OT_INTEGER:
        {
            m_Handle->mStatus = sqlite3_bind_integer(m_Handle->mPtr, idx, value.Cast< SQInteger >());
        } break;
        // Is this a native float value?
        case OT_FLOAT:
        {
            m_Handle->mStatus = sqlite3_bind_double(m_Handle->mPtr, idx, value.Cast< SQFloat >());
        } break;
        // Is this a boolean value?
        case OT_BOOL:
        {
            m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, idx, value.Cast< bool >());
        } break;
        // Is this a string value?
        case OT_STRING:
        {
            m_Handle->mStatus = sqlite3_bind_text(m_Handle->mPtr, idx,
                                                    value.Cast< CSStr >(), -1, SQLITE_TRANSIENT);
        } break;
        // Is this a null value?
        case OT_NULL:
        {
            m_Handle->mStatus = sqlite3_bind_null(m_Handle->mPtr, idx);
        } break;
        // Is this an instance value? Attempt to treat it as a long integer
        case OT_INSTANCE:
        {
            m_Handle->mStatus = sqlite3_bind_int64(m_Handle->mPtr, idx, FetchSLongObjVal(value));
        } break;
        // Is this an array value?
        case OT_ARRAY:
        {
            IndexBindArray(idx, Array(value));
            // No point in checking the status here
            return *this;
        } break;
        // We don't recognize this kind of value!
        default: STHROWF("Attempting to bind unknown value type (%s:%d)", name, idx);
    }
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMS, "auto", name, idx, m_Handle->ErrMsg());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::Bind(const Object & param, const Object & value)
{
    // Identify the type of parameter was given
    switch  (param.GetType())
    {
        // Is this a string value?
        case OT_STRING:
        {
            NameBind(param.Cast< CSStr >(), value);
        } break;
        // Is this an integer value? (or at least can be easily converted to one)
        case OT_INTEGER:
        case OT_FLOAT:
        case OT_BOOL:
        {
            IndexBind(param.Cast< SQInteger >(), value);
        } break;
        // We don't recognize this kind of value!
        default: STHROWF("Unknown parameter index type");
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Object Statement::FetchColumnIndex(Int32 idx) const
{
    VALIDATE_CREATED_HND(*this);
    // Do we have any rows available?
    if (!m_Handle->mGood)
    {
        STHROWF("No row available");
    }
    // Is the specified index valid?
    else if (!m_Handle->CheckColumn(idx))
    {
        STHROWF("Column index is out of range: %d:%d", idx, m_Handle->mColumns);
    }
    // Obtain the initial stack size
    const StackGuard sg;
    // Identify which type of value must be pushed on the stack
    switch (sqlite3_column_type(m_Handle->mPtr, idx))
    {
        // Is this a null value?
        case SQLITE_NULL:
        {
            sq_pushnull(DefaultVM::Get());
        } break;
        // Is this an integer?
        case SQLITE_INTEGER:
        {
            sq_pushinteger(DefaultVM::Get(), sqlite3_column_integer(m_Handle->mPtr, idx));
        } break;
        // Is this a floating point?
        case SQLITE_FLOAT:
        {
            sq_pushfloat(DefaultVM::Get(),
                            ConvTo< SQFloat >::From(sqlite3_column_double(m_Handle->mPtr, idx)));
        } break;
        // Is this a string?
        case SQLITE_TEXT:
        {
            sq_pushstring(DefaultVM::Get(),
                            reinterpret_cast< CSStr >(sqlite3_column_text(m_Handle->mPtr, idx)),
                            sqlite3_column_bytes(m_Handle->mPtr, idx));
        } break;
        // Is this raw data?
        case SQLITE_BLOB:
        {
            // Retrieve the size of the blob that must be allocated
            const Int32 size = sqlite3_column_bytes(m_Handle->mPtr, idx);
            // Retrieve the the actual blob data that must be returned
            CCStr data = reinterpret_cast< CCStr >(sqlite3_column_blob(m_Handle->mPtr, idx));
            // Attempt to create a buffer with the blob data on the stack
            if (SQ_FAILED(SqMod_PushBufferData(DefaultVM::Get(), data, size, size)))
            {
                STHROWF("Unable to allocate buffer of at least (%d) bytes", size);
            }
        } break;
        // Unknown type
        default: STHROWF("Unknown value to fetch at index: %d", idx);
    }
    // Obtain the object with the value from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
Object Statement::FetchColumnName(CSStr name) const
{
    VALIDATE_CREATED_HND(*this);
    // Return the requested information
    return FetchColumnIndex(m_Handle->GetColumnIndex(name));
}

// ------------------------------------------------------------------------------------------------
Object Statement::FetchColumn(const Object & column) const
{
    // Identify the type of parameter was given
    switch  (column.GetType())
    {
        // Is this a string value?
        case OT_STRING:
            return FetchColumnName(column.Cast< CSStr >());
        // Is this an integer value? (or at least can be easily converted to one)
        case OT_INTEGER:
        case OT_FLOAT:
        case OT_BOOL:
            return FetchColumnIndex(column.Cast< SQInteger >());
        // We don't recognize this kind of value!
        default: STHROWF("Unknown column identifier type");
    }
    // We have to return something even if we don't reach this point!
    return Object();
}

// ------------------------------------------------------------------------------------------------
Array Statement::FetchArray() const
{
    VALIDATE_CREATED_HND(*this);
    // Return the requested information
    return FetchArray(0, m_Handle->mColumns);
}

// ------------------------------------------------------------------------------------------------
Array Statement::FetchArray(Int32 min) const
{
    VALIDATE_CREATED_HND(*this);
    // Return the requested information
    return FetchArray(min, m_Handle->mColumns);
}

// ------------------------------------------------------------------------------------------------
Array Statement::FetchArray(Int32 min, Int32 max) const
{
    VALIDATE_ROW_HND(*this);
    // Was there anything selected?
    if (min == max)
    {
        return Array(); // Nothing to retrieve
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
    // Allocate an array large enough to hold the values from selected columns
    Array arr(DefaultVM::Get(), max-min);
    // Process the range of selected columns
    for (Int32 elem = 0, idx = min; idx < max; ++elem, ++idx)
    {
        // Identify the type of value that must be assigned
        switch (sqlite3_column_type(m_Handle->mPtr, idx))
        {
            // Is this a null value?
            case SQLITE_NULL:
            // Array elements are initialized as null by default so we just leave it like that
            break;
            // Is this an integer?
            case SQLITE_INTEGER:
            {
                arr.SetValue(elem, sqlite3_column_integer(m_Handle->mPtr, idx));
            } break;
            // Is this a floating point?
            case SQLITE_FLOAT:
            {
                arr.SetValue(elem, ConvTo< SQFloat >::From(sqlite3_column_double(m_Handle->mPtr, idx)));
            } break;
            // Is this a string?
            case SQLITE_TEXT:
            {
                arr.SetValue(elem, reinterpret_cast< CCStr >(sqlite3_column_text(m_Handle->mPtr, idx)));
            } break;
            // Is this raw data?
            case SQLITE_BLOB:
            {
                // Remember the current stack size
                const StackGuard sg;
                // Retrieve the size of the blob that must be allocated
                const Int32 size = sqlite3_column_bytes(m_Handle->mPtr, idx);
                // Retrieve the the actual blob data that must be returned
                CCStr data = reinterpret_cast< CCStr >(sqlite3_column_blob(m_Handle->mPtr, idx));
                // Attempt to create a buffer with the blob data on the stack
                if (SQ_FAILED(SqMod_PushBufferData(DefaultVM::Get(), data, size, size)))
                {
                    STHROWF("Unable to allocate buffer of at least (%d) bytes", size);
                }
                // Obtain the object from the stack
                Var< Object > obj(DefaultVM::Get(), -1);
                // Now bind it as an array element
                arr.Bind(elem, obj.value);
            } break;
            // Unknown type
            default: STHROWF("Unknown value to fetch at index: %d", idx);
        }
    }
    // Return the resulted array
    return arr;
}

// ------------------------------------------------------------------------------------------------
Table Statement::FetchTable() const
{
    VALIDATE_CREATED_HND(*this);
    // Return the requested information
    return FetchTable(0, m_Handle->mColumns);
}

// ------------------------------------------------------------------------------------------------
Table Statement::FetchTable(Int32 min) const
{
    VALIDATE_CREATED_HND(*this);
    // Return the requested information
    return FetchTable(min, m_Handle->mColumns);
}

// ------------------------------------------------------------------------------------------------
Table Statement::FetchTable(Int32 min, Int32 max) const
{
    VALIDATE_ROW_HND(*this);
    // Was there anything selected?
    if (min == max)
    {
        return Table(); // Nothing to retrieve
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
    // Used to bind null values
    Object obj;
    // Process the range of selected columns
    for (Int32 elem = 0, idx = min; idx < max; ++elem, ++idx)
    {
        //Attempt to obtain the column name
        CSStr name = sqlite3_column_name(m_Handle->mPtr, idx);
        // Validate the obtained name
        if (!name)
        {
            STHROWF("Unable to retrieve name of column (%d)", idx);
        }
        // Identify the type of value that must be assigned
        switch (sqlite3_column_type(m_Handle->mPtr, idx))
        {
            // Is this a null value?
            case SQLITE_NULL:
            {
                // Make sure we don't insert the same instance
                obj.Release();
                // Now it's safe to insert the value
                tbl.SetValue(name, obj);
            } break;
            // Is this an integer?
            case SQLITE_INTEGER:
            {
                tbl.SetValue(name, sqlite3_column_integer(m_Handle->mPtr, idx));
            } break;
            // Is this a floating point?
            case SQLITE_FLOAT:
            {
                tbl.SetValue(name, ConvTo< SQFloat >::From(sqlite3_column_double(m_Handle->mPtr, idx)));
            } break;
            // Is this a string?
            case SQLITE_TEXT:
            {
                tbl.SetValue(name, reinterpret_cast< CCStr >(sqlite3_column_text(m_Handle->mPtr, idx)));
            } break;
            // Is this raw data?
            case SQLITE_BLOB:
            {
                // Remember the current stack size
                const StackGuard sg;
                // Retrieve the size of the blob that must be allocated
                const Int32 size = sqlite3_column_bytes(m_Handle->mPtr, idx);
                // Retrieve the the actual blob data that must be returned
                CCStr data = reinterpret_cast< CCStr >(sqlite3_column_blob(m_Handle->mPtr, idx));
                // Attempt to create a buffer with the blob data on the stack
                if (SQ_FAILED(SqMod_PushBufferData(DefaultVM::Get(), data, size, size)))
                {
                    STHROWF("Unable to allocate buffer of at least (%d) bytes", size);
                }
                // Obtain the object from the stack
                Var< Object > obj(DefaultVM::Get(), -1);
                // Now bind it as a table element
                tbl.Bind(name, obj.value);
            } break;
            // Unknown type
            default: STHROWF("Unknown value to fetch at index: %d", idx);
        }
    }
    // Return the resulted table
    return tbl;
}

// ------------------------------------------------------------------------------------------------
bool Statement::CheckColumn(Int32 idx) const
{
    VALIDATE_CREATED_HND(*this);
    // Return the requested information
    return m_Handle->CheckColumn(idx);
}

// ------------------------------------------------------------------------------------------------
bool Statement::CheckParameter(Int32 idx) const
{
    VALIDATE_CREATED_HND(*this);
    // Return the requested information
    return m_Handle->CheckParameter(idx);
}

// ------------------------------------------------------------------------------------------------
bool Statement::IsColumnNull(Int32 idx) const
{
    VALIDATE_COLUMN_HND(*this, idx);
    // Return the requested information
    return (sqlite3_column_type(m_Handle->mPtr, idx) == SQLITE_NULL);
}

// ------------------------------------------------------------------------------------------------
Int32 Statement::GetColumnIndex(CSStr name) const
{
    VALIDATE_CREATED_HND(*this);
    // Return the requested information
    return m_Handle->GetColumnIndex(name);
}

// ------------------------------------------------------------------------------------------------
CSStr Statement::GetColumnName(Int32 idx) const
{
    VALIDATE_COLUMN_HND(*this, idx);
    // Return the requested information
    return sqlite3_column_name(m_Handle->mPtr, idx);
}

// ------------------------------------------------------------------------------------------------
CSStr Statement::GetColumnOriginName(Int32 idx) const
{
#ifdef SQLITE_ENABLE_COLUMN_METADATA
    VALIDATE_COLUMN_HND(*this, idx);
    // Return the requested information
    return sqlite3_column_origin_name(m_Handle->mPtr, idx);
#else
    // The compiler moans when extra warnings are enabled
    SQMOD_UNUSED_VAR(idx);
    // Stop the execution here!
    STHROWF("The module was compiled without this feature");
    // We have to return something
    return _SC("");
#endif
}

// ------------------------------------------------------------------------------------------------
Int32 Statement::GetColumnType(Int32 idx) const
{
    VALIDATE_COLUMN_HND(*this, idx);
    // Return the requested information
    return sqlite3_column_type(m_Handle->mPtr, idx);
}

// ------------------------------------------------------------------------------------------------
Int32 Statement::GetColumnBytes(Int32 idx) const
{
    VALIDATE_COLUMN_HND(*this, idx);
    // Return the requested information
    return sqlite3_column_bytes(m_Handle->mPtr, idx);
}

// ------------------------------------------------------------------------------------------------
Object Statement::GetColumnByIndex(Int32 idx) const
{
    VALIDATE_COLUMN_HND(*this, idx);
    // Return the requested information
    return Object(new Column(m_Handle, idx));
}

// ------------------------------------------------------------------------------------------------
Object Statement::GetColumnByName(CSStr name) const
{
    VALIDATE_CREATED_HND(*this);
    // Attempt to obtain the requested column index
    const Int32 idx = m_Handle->GetColumnIndex(name);
    // Validate the obtained index
    if (idx < 0)
    {
        STHROWF("Unknown column named (%s)", name);
    }
    // Return the requested column
    return Object(new Column(m_Handle, idx));
}

// ------------------------------------------------------------------------------------------------
Object Statement::GetColumn(const Object & column) const
{
    // Identify the type of parameter was given
    switch  (column.GetType())
    {
        // Is this a string value?
        case OT_STRING:
            return GetColumnByName(column.Cast< CSStr >());
        // Is this an integer value? (or at least can be easily converted to one)
        case OT_INTEGER:
        case OT_FLOAT:
        case OT_BOOL:
            return GetColumnByIndex(column.Cast< SQInteger >());
        // We don't recognize this kind of value!
        default: STHROWF("Unknown column identifier type");
    }
    // We have to return something even if we don't reach this point!
    return Object();
}

// ------------------------------------------------------------------------------------------------
Int32 Statement::GetDataCount() const
{
    VALIDATE_ROW_HND(*this);
    // Return the requested information
    return sqlite3_data_count(m_Handle->mPtr);
}

// ------------------------------------------------------------------------------------------------
Int32 Statement::GetParameterIndex(CSStr name) const
{
    VALIDATE_CREATED_HND(*this);
    // Return the requested information
    return sqlite3_bind_parameter_index(m_Handle->mPtr, name);
}

// ------------------------------------------------------------------------------------------------
CSStr Statement::GetParameterName(Int32 idx) const
{
    VALIDATE_PARAMETER_HND(*this, idx);
    // Return the requested information
    return sqlite3_bind_parameter_name(m_Handle->mPtr, idx);
}

// ================================================================================================
void Register_Statement(Table & sqlns)
{
    sqlns.Bind(_SC("Statement"),
        Class< Statement >(sqlns.GetVM(), _SC("SqSQLiteStatement"))
        // Constructors
        .Ctor()
        .Ctor< const Connection &, CCStr >()
        .Ctor< const Statement & >()
        // Meta-methods
        .Func(_SC("_cmp"), &Statement::Cmp)
        .SquirrelFunc(_SC("_typename"), &Statement::Typename)
        .Func(_SC("_tostring"), &Statement::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Statement::IsValid)
        .Prop(_SC("Prepared"), &Statement::IsPrepared)
        .Prop(_SC("References"), &Statement::GetReferences)
        .Prop(_SC("Connection"), &Statement::GetConnection)
        .Prop(_SC("Status"), &Statement::GetStatus)
        .Prop(_SC("ErrCode"), &Statement::GetErrorCode)
        .Prop(_SC("ExErrCode"), &Statement::GetExtendedErrorCode)
        .Prop(_SC("ExtendedErrCode"), &Statement::GetExtendedErrorCode)
        .Prop(_SC("ErrStr"), &Statement::GetErrStr)
        .Prop(_SC("ErrMsg"), &Statement::GetErrMsg)
        .Prop(_SC("Columns"), &Statement::GetColumns)
        .Prop(_SC("Parameters"), &Statement::GetParameters)
        .Prop(_SC("Query"), &Statement::GetQuery)
        .Prop(_SC("Good"), &Statement::GetGood)
        .Prop(_SC("Done"), &Statement::GetDone)
        .Prop(_SC("DataCount"), &Statement::GetDataCount)
        // Member Methods
        .Func(_SC("Release"), &Statement::Release)
        .Func(_SC("Reset"), &Statement::Reset)
        .Func(_SC("Clear"), &Statement::Clear)
        .Func(_SC("Exec"), &Statement::Exec)
        .Func(_SC("Step"), &Statement::Step)
        .Func(_SC("BindArrayI"), &Statement::IndexBindArray)
        .Func(_SC("BindValueI"), &Statement::IndexBindValue)
        .Func(_SC("BindLongI"), &Statement::IndexBindLong)
        .Func(_SC("BindIntegerI"), &Statement::IndexBindInteger)
        .Func(_SC("BindFloatI"), &Statement::IndexBindFloat)
        .Func(_SC("BindStringI"), &Statement::IndexBindString)
        .Func(_SC("BindBoolI"), &Statement::IndexBindBool)
        .Func(_SC("BindNullI"), &Statement::IndexBindNull)
        .Func(_SC("BindTableN"), &Statement::NameBindTable)
        .Func(_SC("BindValueN"), &Statement::NameBindValue)
        .Func(_SC("BindLongN"), &Statement::NameBindLong)
        .Func(_SC("BindIntegerN"), &Statement::NameBindInteger)
        .Func(_SC("BindFloatN"), &Statement::NameBindFloat)
        .Func(_SC("BindStringN"), &Statement::NameBindString)
        .Func(_SC("BindBoolN"), &Statement::NameBindBool)
        .Func(_SC("BindNullN"), &Statement::NameBindNull)
        .Func(_SC("BindI"), &Statement::IndexBind)
        .Func(_SC("BindN"), &Statement::NameBind)
        .Func(_SC("Bind"), &Statement::Bind)
        .Func(_SC("FetchI"), &Statement::FetchColumnIndex)
        .Func(_SC("FetchN"), &Statement::FetchColumnName)
        .Func(_SC("Fetch"), &Statement::FetchColumn)
        .Func(_SC("CheckColumn"), &Statement::CheckColumn)
        .Func(_SC("CheckParameter"), &Statement::CheckParameter)
        .Func(_SC("IsColumnNull"), &Statement::IsColumnNull)
        .Func(_SC("ColumnIndex"), &Statement::GetColumnIndex)
        .Func(_SC("ColumnName"), &Statement::GetColumnName)
        .Func(_SC("ColumnOriginName"), &Statement::GetColumnOriginName)
        .Func(_SC("ColumnType"), &Statement::GetColumnType)
        .Func(_SC("ColumnBytes"), &Statement::GetColumnBytes)
        .Func(_SC("GetI"), &Statement::GetColumnByIndex)
        .Func(_SC("GetN"), &Statement::GetColumnByName)
        .Func(_SC("Get"), &Statement::GetColumn)
        .Func(_SC("GetParameterIndex"), &Statement::GetParameterIndex)
        .Func(_SC("GetParameterName"), &Statement::GetParameterName)
        // Member overloads
        .Overload< Array (Statement::*)(void) const >(_SC("FetchArray"), &Statement::FetchArray)
        .Overload< Array (Statement::*)(Int32) const >(_SC("FetchArray"), &Statement::FetchArray)
        .Overload< Array (Statement::*)(Int32, Int32) const >(_SC("FetchArray"), &Statement::FetchArray)
        .Overload< Table (Statement::*)(void) const >(_SC("FetchTable"), &Statement::FetchTable)
        .Overload< Table (Statement::*)(Int32) const >(_SC("FetchTable"), &Statement::FetchTable)
        .Overload< Table (Statement::*)(Int32, Int32) const >(_SC("FetchTable"), &Statement::FetchTable)
    );
}

} // Namespace:: SqMod
