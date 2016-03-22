// ------------------------------------------------------------------------------------------------
#include "Statement.hpp"
#include "Connection.hpp"
#include "Column.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
// Error message when failed to bind value to parameter index.
#define SQMOD_BADPARAMI "Unable to bind [%s] parameter at (%d) because [%s]"
// Error message when failed to bind value to parameter name.
#define SQMOD_BADPARAMS "Unable to bind [%s] parameter at (%s:%d) because [%s]"

// ------------------------------------------------------------------------------------------------
SQInteger Statement::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqSQLiteStatement");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
void Statement::Validate() const
{
    // Is the handle valid?
    if (!m_Handle)
    {
        STHROWF("Invalid SQLite statement reference");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::ValidateIndex(Int32 idx) const
{
    // Is the handle valid?
    if (!m_Handle)
    {
        STHROWF("Invalid SQLite statement reference");
    }
    // Is the specified index in range?
    else if (!m_Handle->CheckIndex(idx))
    {
        STHROWF("Column index is out of range: %d", idx);
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::ValidateRow() const
{
    // Is the handle valid?
    if (!m_Handle)
    {
        STHROWF("Invalid SQLite statement reference");
    }
    // Do we have any rows available?
    else if (!m_Handle->mGood)
    {
        STHROWF("No row available");
    }
}

// ------------------------------------------------------------------------------------------------
Statement::Statement()
    : m_Handle()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Statement::Statement(const ConnHnd & connection, CSStr query)
    : m_Handle(connection)
{
    // Validate the statement handle
    if (m_Handle.m_Hnd)
    {
        m_Handle->Create(query);
    }
    // We just failed to obtain a valid handle
    else
    {
        STHROWF("Unable to create the statement reference");
    }
}

// ------------------------------------------------------------------------------------------------
Statement::Statement(const Connection & connection, CSStr query)
    : m_Handle(connection.GetHandle())
{
    // Validate the statement handle
    if (m_Handle.m_Hnd)
    {
        m_Handle->Create(query);
    }
    // We just failed to obtain a valid handle
    else
    {
        STHROWF("Unable to create the statement reference");
    }
}

// ------------------------------------------------------------------------------------------------
Object Statement::GetConnection() const
{
    // Validate the handle
    Validate();
    // Return the requested information
    return Object(new Connection(m_Handle->mConn));
}

// ------------------------------------------------------------------------------------------------
void Statement::Reset()
{
    // Validate the handle
    Validate();
    // Specify that we don't have a row available and we haven't finished stepping
    m_Handle->mGood = false;
    m_Handle->mDone = false;
    // Attempt to reset the statement to it's initial state
    m_Handle = sqlite3_reset(m_Handle);
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF("Unable to reset statement [%s]", m_Handle.ErrStr());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::Clear()
{
    // Validate the handle
    Validate();
    // Specify that we don't have a row available and we haven't finished stepping
    m_Handle->mGood = false;
    m_Handle->mDone = false;
    // Attempt to clear the statement
    m_Handle = sqlite3_clear_bindings(m_Handle);
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF("Unable to clear statement [%s]", m_Handle.ErrStr());
    }
}

// ------------------------------------------------------------------------------------------------
Int32 Statement::Exec()
{
    // Validate the handle
    Validate();
    // Did we reset first?
    if (m_Handle->mDone)
    {
        STHROWF("Executed without resetting first");
    }
    // Attempt to step the statement
    m_Handle = sqlite3_step(m_Handle);
    // Have we finished stepping?
    if (m_Handle == SQLITE_DONE)
    {
        // Specify that we don't have row available and we finished stepping
        m_Handle->mGood = false;
        m_Handle->mDone = true;
        // Return the changes made by this statement
        return sqlite3_changes(m_Handle->mConn);
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
    // Validate the handle
    Validate();
    // Did we reset first?
    if (m_Handle->mDone)
    {
        STHROWF("Stepped without resetting first");
    }
    // Attempt to step the statement
    m_Handle = sqlite3_step(m_Handle);
    // Do we have a row available?
    if (m_Handle == SQLITE_ROW)
    {
        // Specify that we have a row available
        return (m_Handle->mGood = true);
    }
    // Have we finished stepping?
    else if (m_Handle == SQLITE_DONE)
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
void Statement::IndexBindA(Int32 idx, const Array & arr)
{
    // Validate the handle
    Validate();
    // Know when to stop trying to bind values
    const Int32 max = sqlite3_bind_parameter_count(m_Handle);
    // Make sure that we are at least in bounds
    if (idx >= max)
    {
        STHROWF("Parameter index out of range: %d >= %d", idx, max);
    }
    // Should we clear onward parameters?
    else if (arr.Length() <= 0)
    {
        // Clear all parameters after the specified index
        for (Int32 idx = arr.Length(); idx < max; ++idx)
        {
            sqlite3_bind_null(m_Handle, idx);
        }
    }
    // Attempt to retrieve and bind the specified values
    else
    {
        // Allocated memory for the specified amount of values
        std::vector< Object > values(arr.Length());
        // Attempt to retrieve the specified values
        arr.GetArray< Object >(&values[0], values.size());
        // Obtain the start of the object list
        std::vector< Object >::iterator itr = values.begin();
        // Attempt to bind each specified element individually
        for (; itr != values.end() && idx < max; ++itr)
        {
            IndexBind(idx++, *itr);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindI(Int32 idx, Int32 value)
{
    // Validate the handle
    Validate();
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_int(m_Handle, idx, value);
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMI, "int", idx, m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindL(Int32 idx, const Object & value)
{
    // Validate the handle
    Validate();
    // Obtain the initial stack size
    const StackGuard sg(DefaultVM::Get());
    // Push the specified object onto the stack
    Var< const Object & >::push(DefaultVM::Get(), value);
    // The resulted long integer value
    Int64 longint = 0;
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(_SqMod->GetSLongValue(DefaultVM::Get(), -1, &longint)))
    {
        STHROWF("Invalid long integer specified");
    }
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_int(m_Handle, idx, longint);
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMI, "long", idx, m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindV(Int32 idx, SQInteger value)
{
    // Validate the handle
    Validate();
    // Attempt to bind the specified value
#ifdef _SQ64
    m_Handle = sqlite3_bind_int64(m_Handle, idx, value);
#else
    m_Handle = sqlite3_bind_int(m_Handle, idx, value);
#endif // _SQ64
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMI, "value", idx, m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindF(Int32 idx, SQFloat value)
{
    // Validate the handle
    Validate();
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_double(m_Handle, idx, value);
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMI, "float", idx, m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindS(Int32 idx, CSStr value)
{
    // Validate the handle
    Validate();
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_text(m_Handle, idx, value, -1, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMI, "string", idx, m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindB(Int32 idx, bool value)
{
    // Validate the handle
    Validate();
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_int(m_Handle, idx, value);
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMI, "boolean", idx, m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindN(Int32 idx)
{
    // Validate the handle
    Validate();
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_null(m_Handle, idx);
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMI, "null", idx, m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindT(const Table & tbl)
{
    // Validate the handle
    Validate();
    // Should we clear the all the parameters?
    if (tbl.GetSize() <= 0)
    {
        Clear();
    }
    // Attempt to assign the specified parameter values
    else
    {
        // Obtain a table iterator
        Table::iterator itr;
        // Used to retrieve element values
        Object val;
        // Process each element until _next returns null
        while (tbl.Next(itr))
        {
            // Get the value object
            val = Object(itr.getValue());
            // Bind it to the associated key
            NameBind(itr.getName(), val);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindI(CSStr name, Int32 value)
{
    // Validate the handle
    Validate();
    // Attempt to obtain the index of the specified parameter name
    const Int32 idx = sqlite3_bind_parameter_index(m_Handle, name);
    // Validate the obtained index
    if (!idx)
    {
        STHROWF("Unknown parameter named (%s)", name);
    }
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_int(m_Handle, idx, value);
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMS, "int", name, idx, m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindL(CSStr name, const Object & value)
{
    // Validate the handle
    Validate();
    // Attempt to obtain the index of the specified parameter name
    const Int32 idx = sqlite3_bind_parameter_index(m_Handle, name);
    // Validate the obtained index
    if (!idx)
    {
        STHROWF("Unknown parameter named (%s)", name);
    }
    // Obtain the initial stack size
    const StackGuard sg(DefaultVM::Get());
    // Push the specified object onto the stack
    Var< const Object & >::push(DefaultVM::Get(), value);
    // The resulted long integer value
    Uint64 longint = 0;
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(_SqMod->GetULongValue(DefaultVM::Get(), -1, &longint)))
    {
        STHROWF("Invalid long integer specified");
    }
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_int(m_Handle, idx, longint);
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMS, "long", name, idx, m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindV(CSStr name, SQInteger value)
{
    // Validate the handle
    Validate();
    // Attempt to obtain the index of the specified parameter name
    const Int32 idx = sqlite3_bind_parameter_index(m_Handle, name);
    // Validate the obtained index
    if (!idx)
    {
        STHROWF("Unknown parameter named (%s)", name);
    }
    // Attempt to bind the specified value
#ifdef _SQ64
    m_Handle = sqlite3_bind_int64(m_Handle, idx, value);
#else
    m_Handle = sqlite3_bind_int(m_Handle, idx, value);
#endif // _SQ64
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMS, "value", name, idx, m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindF(CSStr name, SQFloat value)
{
    // Validate the handle
    Validate();
    // Attempt to obtain the index of the specified parameter name
    const Int32 idx = sqlite3_bind_parameter_index(m_Handle, name);
    // Validate the obtained index
    if (!idx)
    {
        STHROWF("Unknown parameter named (%s)", name);
    }
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_double(m_Handle, idx, value);
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMS, "float", name, idx, m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindS(CSStr name, CSStr value)
{
    // Validate the handle
    Validate();
    // Attempt to obtain the index of the specified parameter name
    const Int32 idx = sqlite3_bind_parameter_index(m_Handle, name);
    // Validate the obtained index
    if (!idx)
    {
        STHROWF("Unknown parameter named (%s)", name);
    }
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_text(m_Handle, idx, value, -1, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMS, "string", name, idx, m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindB(CSStr name, bool value)
{
    // Validate the handle
    Validate();
    // Attempt to obtain the index of the specified parameter name
    const Int32 idx = sqlite3_bind_parameter_index(m_Handle, name);
    // Validate the obtained index
    if (!idx)
    {
        STHROWF("Unknown parameter named (%s)", name);
    }
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_int(m_Handle, idx, value);
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMS, "boolean", name, idx, m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindN(CSStr name)
{
    // Validate the handle
    Validate();
    // Attempt to obtain the index of the specified parameter name
    const Int32 idx = sqlite3_bind_parameter_index(m_Handle, name);
    // Validate the obtained index
    if (!idx)
    {
        STHROWF("Unknown parameter named (%s)", name);
    }
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_null(m_Handle, idx);
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMS, "null", name, idx, m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBind(Int32 idx, const Object & value)
{
    // Validate the handle
    Validate();
    // Attempt to identify the specified type
    switch (value.GetType())
    {
        // Is this a native integer value?
        case OT_INTEGER:
#ifdef _SQ64
            m_Handle = sqlite3_bind_int64(m_Handle, idx, value.Cast< SQInteger >());
#else
            m_Handle = sqlite3_bind_int(m_Handle, idx, value.Cast< SQInteger >());
#endif
        break;
        // Is this a native float value?
        case OT_FLOAT:
            m_Handle = sqlite3_bind_double(m_Handle, idx, value.Cast< SQFloat >());
        break;
        // Is this a boolean value?
        case OT_BOOL:
            m_Handle = sqlite3_bind_int(m_Handle, idx, value.Cast< bool >());
        break;
        // Is this a string value?
        case OT_STRING:
            m_Handle = sqlite3_bind_text(m_Handle, idx, value.Cast< CSStr >(), -1, SQLITE_TRANSIENT);
        break;
        // Is this a null value?
        case OT_NULL:
            m_Handle = sqlite3_bind_null(m_Handle, idx);
        break;
        // Is this an array value?
        case OT_ARRAY:
            IndexBindA(idx, value.Cast< const Array & >());
        break;
        // Is this an instance value?
        case OT_INSTANCE:
        {
            // Obtain the initial stack size
            const StackGuard sg(DefaultVM::Get());
            // Push the specified object onto the stack
            Var< const Object & >::push(DefaultVM::Get(), value);
            // The resulted long integer value
            Int64 longint = 0;
            // Attempt to get the numeric value inside the specified object
            if (SQ_FAILED(_SqMod->GetSLongValue(DefaultVM::Get(), -1, &longint)))
            {
                STHROWF("Invalid long integer specified (%d)", idx);
            }
            // Now bind the resulted long integer
            m_Handle = sqlite3_bind_int64(m_Handle, idx, longint);
        } break;
        // We don't recognize this kind of value!
        default:
            STHROWF("Attempting to bind unknown value type (%d)", idx);
    }
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMI, "auto", idx, m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBind(CSStr name, const Object & value)
{
    // Validate the handle
    Validate();
    // Attempt to obtain the index of the specified parameter name
    const Int32 idx = sqlite3_bind_parameter_index(m_Handle, name);
    // Validate the obtained index
    if (!idx)
    {
        STHROWF("Unknown parameter named (%s)", name);
    }
    // Attempt to identify the specified type
    switch (value.GetType())
    {
        // Is this a native integer value?
        case OT_INTEGER:
#ifdef _SQ64
            m_Handle = sqlite3_bind_int64(m_Handle, idx, value.Cast< SQInteger >());
#else
            m_Handle = sqlite3_bind_int(m_Handle, idx, value.Cast< SQInteger >());
#endif
        break;
        // Is this a native float value?
        case OT_FLOAT:
            m_Handle = sqlite3_bind_double(m_Handle, idx, value.Cast< SQFloat >());
        break;
        // Is this a boolean value?
        case OT_BOOL:
            m_Handle = sqlite3_bind_int(m_Handle, idx, value.Cast< bool >());
        break;
        // Is this a string value?
        case OT_STRING:
            m_Handle = sqlite3_bind_text(m_Handle, idx, value.Cast< CSStr >(), -1, SQLITE_TRANSIENT);
        break;
        // Is this a null value?
        case OT_NULL:
            m_Handle = sqlite3_bind_null(m_Handle, idx);
        break;
        // Is this an array value?
        case OT_TABLE:
            NameBindT(value.Cast< const Table & >());
        break;
        // Is this an instance value?
        case OT_INSTANCE:
        {
            // Obtain the initial stack size
            const StackGuard sg(DefaultVM::Get());
            // Push the specified object onto the stack
            Var< const Object & >::push(DefaultVM::Get(), value);
            // The resulted long integer value
            Int64 longint = 0;
            // Attempt to get the numeric value inside the specified object
            if (SQ_FAILED(_SqMod->GetSLongValue(DefaultVM::Get(), -1, &longint)))
            {
                STHROWF("Invalid long integer specified (%s:%d)", name, idx);
            }
            // Now bind the resulted long integer
            m_Handle = sqlite3_bind_int64(m_Handle, idx, longint);
        } break;
        // We don't recognize this kind of value!
        default:
            STHROWF("Attempting to bind unknown value type (%s:%d)", name, idx);
    }
    // Validate the result
    if (m_Handle != SQLITE_OK)
    {
        STHROWF(SQMOD_BADPARAMS, "auto", name, idx, m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::Bind(const Object & param, const Object & value)
{
    // Is this a string value?
    if (param.GetType() == OT_STRING)
    {
        NameBind(param.Cast< CSStr >(), value);
    }
    // Is this an integer value?
    else if (param.GetType() == OT_INTEGER)
    {
        IndexBind(param.Cast< SQInteger >(), value);
    }
    // We don't recognize this kind of value!
    else
    {
        STHROWF("Unknown parameter index type");
    }
}

// ------------------------------------------------------------------------------------------------
Object Statement::FetchColumnIndex(Int32 idx) const
{
    // Validate the handle and row
    ValidateRow();
    // Is the specified index valid?
    if (!m_Handle->CheckIndex(idx))
        STHROWF("Column index is out of range: %d", idx);
    // Obtain the initial stack size
    const StackGuard sg(DefaultVM::Get());
    // Identify which type of value must be pushed on the stack
    switch (sqlite3_column_type(m_Handle, idx))
    {
        // Is this a null value?
        case SQLITE_NULL:
            sq_pushnull(DefaultVM::Get());
        break;
        // Is this an integer?
        case SQLITE_INTEGER:
#ifdef _SQ64
            sq_pushinteger(DefaultVM::Get(), sqlite3_column_int64(m_Handle, idx));
#else
            sq_pushinteger(DefaultVM::Get(), sqlite3_column_int(m_Handle, idx));
#endif
        break;
        // Is this a floating point?
        case SQLITE_FLOAT:
            sq_pushfloat(DefaultVM::Get(), sqlite3_column_double(m_Handle, idx));
        break;
        // Is this a string?
        case SQLITE_TEXT:
            sq_pushstring(DefaultVM::Get(), (CSStr)sqlite3_column_text(m_Handle, idx),
                                        sqlite3_column_bytes(m_Handle, idx));
        break;
        // Is this raw data?
        case SQLITE_BLOB:
        {
            // Obtain the size of the data
            const Int32 sz = sqlite3_column_bytes(m_Handle, idx);
            // Allocate a blob of the same size
            SQUserPointer p = sqstd_createblob(DefaultVM::Get(), sz);
            // Obtain a pointer to the data
            const void * b = sqlite3_column_blob(m_Handle, idx);
            // Could the memory blob be allocated?
            if (!p)
                STHROWF("Unable to allocate space for column blob value");
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
                memcpy(p, b, sz);
        } break;
        // Unknown type
        default:
            STHROWF("Unknown value to fetch at index: %d", idx);
    }
    // Obtain the object with the value from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
Object Statement::FetchColumnName(CSStr name) const
{
    // Validate the handle and row
    ValidateRow();
    // Return the requested information
    return FetchColumnIndex(m_Handle->GetColumnIndex(name));
}

// ------------------------------------------------------------------------------------------------
Object Statement::FetchColumn(const Object & column) const
{
    // Is this a string value?
    if (column.GetType() == OT_STRING)
    {
        return FetchColumnName(column.Cast< CSStr >());
    }
    // Is this an integer value?
    else if (column.GetType() == OT_INTEGER)
    {
        return FetchColumnIndex(column.Cast< SQInteger >());
    }
    // We don't recognize this kind of value!
    STHROWF("Unknown column identifier type");
    // We have to return something!
    return Object();
}

// ------------------------------------------------------------------------------------------------
Array Statement::FetchArray() const
{
    // Validate the handle
    Validate();
    // Return the requested information
    return FetchArray(0, m_Handle->mColumns);
}

// ------------------------------------------------------------------------------------------------
Array Statement::FetchArray(Int32 min) const
{
    // Validate the handle
    Validate();
    // Return the requested information
    return FetchArray(min, m_Handle->mColumns);
}

// ------------------------------------------------------------------------------------------------
Array Statement::FetchArray(Int32 min, Int32 max) const
{
    // Validate the handle
    ValidateRow();
    // Was there anything selected?
    if (min == max)
    {
        return Array(); // Nothing to retrieve
    }
    // Is the minimum actually the minimum?
    else if (min > max)
    {
        STHROWF("Minimum is higher than maximum");
    }
    // Is the minimum in range>
    else if (!m_Handle->CheckIndex(min))
    {
        STHROWF("Minimum is out of range");
    }
    // Is the maximum in range?
    else if (!m_Handle->CheckIndex(max))
    {
        STHROWF("Maximum is out of range");
    }
    // Allocate an array large enough to hold the values from selected columns
    Array arr(DefaultVM::Get(), max-min);
    // Process the range of selected columns
    for (Int32 elem = 0, idx = min; idx < max; ++elem, ++idx)
    {
        // Identify the type of value that must be assigned
        switch (sqlite3_column_type(m_Handle, idx))
        {
            // Is this a null value?
            case SQLITE_NULL:
            // Array elements are initialized as null by default so we just leave it like that
            break;
            // Is this an integer?
            case SQLITE_INTEGER:
#ifdef _SQ64
                arr.SetValue(elem, sqlite3_column_int64(m_Handle, idx));
#else
                arr.SetValue(elem, sqlite3_column_int(m_Handle, idx));
#endif
            break;
            // Is this a floating point?
            case SQLITE_FLOAT:
                arr.SetValue(elem, sqlite3_column_double(m_Handle, idx));
            break;
            // Is this a string?
            case SQLITE_TEXT:
                arr.SetValue(elem, reinterpret_cast< CCStr >(sqlite3_column_text(m_Handle, idx)));
            break;
            // Is this raw data?
            case SQLITE_BLOB:
            {
                // Obtain the initial stack size
                const StackGuard sg(DefaultVM::Get());
                // Obtain the size of the data
                const Int32 sz = sqlite3_column_bytes(m_Handle, idx);
                // Allocate a blob of the same size
                SQUserPointer p = sqstd_createblob(DefaultVM::Get(), sz);
                // Obtain a pointer to the data
                const void * b = sqlite3_column_blob(m_Handle, idx);
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
                    memcpy(p, b, sz);
                }
                // Obtain the object from the stack
                Var< Object > obj(DefaultVM::Get(), -1);
                // Bind it as an array element
                arr.Bind(elem, obj.value);
            } break;
            // Unknown type
            default:
                STHROWF("Unknown value to fetch at index: %d", idx);
        }
    }
    // Return the resulted array
    return arr;
}

// ------------------------------------------------------------------------------------------------
Table Statement::FetchTable() const
{
    // Validate the handle
    Validate();
    // Return the requested information
    return FetchTable(0, m_Handle->mColumns);
}

// ------------------------------------------------------------------------------------------------
Table Statement::FetchTable(Int32 min) const
{
    // Validate the handle
    Validate();
    // Return the requested information
    return FetchTable(min, m_Handle->mColumns);
}

// ------------------------------------------------------------------------------------------------
Table Statement::FetchTable(Int32 min, Int32 max) const
{
    // Validate the handle
    ValidateRow();
    // Was there anything selected?
    if (min == max)
    {
        return Table(); // Nothing to retrieve
    }
    // Is the minimum actually the minimum?
    else if (min > max)
    {
        STHROWF("Minimum is higher than maximum");
    }
    // Is the minimum in range>
    else if (!m_Handle->CheckIndex(min))
    {
        STHROWF("Minimum is out of range");
    }
    // Is the maximum in range?
    else if (!m_Handle->CheckIndex(max))
    {
        STHROWF("Maximum is out of range");
    }
    // Create a table to hold the selected column values
    Table tbl(DefaultVM::Get());
    // Used to bind null values
    Object obj;
    // Process the range of selected columns
    for (Int32 elem = 0, idx = min; idx < max; ++elem, ++idx)
    {
        //Attempt to obtain the column name
        CSStr name = sqlite3_column_name(m_Handle, idx);
        // Validate the obtained name
        if (!name)
            STHROWF("Unable to retrieve name of column (%d)", idx);
        // Identify the type of value that must be assigned
        switch (sqlite3_column_type(m_Handle, idx))
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
#ifdef _SQ64
                tbl.SetValue(name, sqlite3_column_int64(m_Handle, idx));
#else
                tbl.SetValue(name, sqlite3_column_int(m_Handle, idx));
#endif
            break;
            // Is this a floating point?
            case SQLITE_FLOAT:
                tbl.SetValue(name, sqlite3_column_double(m_Handle, idx));
            break;
            // Is this a string?
            case SQLITE_TEXT:
                tbl.SetValue(name, reinterpret_cast< CCStr >(sqlite3_column_text(m_Handle, idx)));
            break;
            // Is this raw data?
            case SQLITE_BLOB:
            {
                // Obtain the initial stack size
                const StackGuard sg(DefaultVM::Get());
                // Obtain the size of the data
                const Int32 sz = sqlite3_column_bytes(m_Handle, idx);
                // Allocate a blob of the same size
                SQUserPointer p = sqstd_createblob(DefaultVM::Get(), sz);
                // Obtain a pointer to the data
                const void * b = sqlite3_column_blob(m_Handle, idx);
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
                    memcpy(p, b, sz);
                }
                // Obtain the object from the stack
                Var< Object > obj(DefaultVM::Get(), -1);
                // Bind it as a table element
                tbl.Bind(name, obj.value);
            } break;
            // Unknown type
            default:
                STHROWF("Unknown value to fetch at index: %d", idx);
        }
    }
    // Return the resulted table
    return tbl;
}

// ------------------------------------------------------------------------------------------------
bool Statement::CheckIndex(Int32 idx) const
{
    // Validate the handle
    Validate();
    // Return the requested information
    return m_Handle->CheckIndex(idx);
}

// ------------------------------------------------------------------------------------------------
bool Statement::IsColumnNull(Int32 idx) const
{
    // Can we make the request?
    ValidateIndex(idx);
    // Return the requested information
    return (sqlite3_column_type(m_Handle, idx) == SQLITE_NULL);
}

// ------------------------------------------------------------------------------------------------
Int32 Statement::GetColumnIndex(CSStr name) const
{
    // Validate the handle
    Validate();
    // Return the requested information
    return m_Handle->GetColumnIndex(name);
}

// ------------------------------------------------------------------------------------------------
CSStr Statement::GetColumnName(Int32 idx) const
{
    // Can we make the request?
    ValidateIndex(idx);
    // Return the requested information
    return sqlite3_column_name(m_Handle, idx);
}

// ------------------------------------------------------------------------------------------------
CSStr Statement::GetColumnOriginName(Int32 idx) const
{
#ifdef SQLITE_ENABLE_COLUMN_METADATA
    // Can we make the request?
    ValidateIndex(idx);
    // Return the requested information
    return sqlite3_column_origin_name(m_Handle, idx);
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
    // Can we make the request?
    ValidateIndex(idx);
    // Return the requested information
    return sqlite3_column_type(m_Handle, idx);
}

// ------------------------------------------------------------------------------------------------
Int32 Statement::GetColumnBytes(Int32 idx) const
{
    // Can we make the request?
    ValidateIndex(idx);
    // Return the requested information
    return sqlite3_column_bytes(m_Handle, idx);
}

// ------------------------------------------------------------------------------------------------
Object Statement::GetColumnByIndex(Int32 idx) const
{
    // Can we make the request?
    ValidateIndex(idx);
    // Return the requested information
    return Object(new Column(m_Handle, idx));
}

// ------------------------------------------------------------------------------------------------
Object Statement::GetColumnByName(CSStr name) const
{
    // Validate the handle
    Validate();
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
    // Is this a string value?
    if (column.GetType() == OT_STRING)
    {
        return GetColumnByName(column.Cast< CSStr >());
    }
    // Is this an integer value?
    else if (column.GetType() == OT_INTEGER)
    {
        return GetColumnByIndex(column.Cast< SQInteger >());
    }
    // We don't recognize this kind of value!
    STHROWF("Unknown column identifier type");
    // We have to return something!
    return Object();
}

} // Namespace:: SqMod
