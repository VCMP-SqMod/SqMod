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
CSStr Statement::s_BadParamI = "Unable to bind [%s] parameter at (%d) because [%s]";
CSStr Statement::s_BadParamS = "Unable to bind [%s] parameter at (%s:%d) because [%s]";

// ------------------------------------------------------------------------------------------------
SQInteger Statement::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqSQLiteStatement");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
bool Statement::Validate() const
{
    if (m_Handle)
        return true;
    // Invalid statement reference
    _SqMod->SqThrow("Invalid SQLite statement reference");
    return false;
}

// ------------------------------------------------------------------------------------------------
bool Statement::ValidateIndex(Int32 idx) const
{
    // Validate the handle
    if (!m_Handle)
        _SqMod->SqThrow("Invalid SQLite statement reference");
    // Is the specified index in range?
    else if (!m_Handle->CheckIndex(idx))
        _SqMod->SqThrow("Index out of range: %d", idx);
    // Requirements satisfied
    else
        return true;
    // Should not proceed further
    return false;
}

// ------------------------------------------------------------------------------------------------
bool Statement::RowAvailable() const
{
    // Validate the handle
    if (!m_Handle)
        _SqMod->SqThrow("Invalid SQLite statement reference");
    // Do we have any rows available?
    else if (!m_Handle->mGood)
        _SqMod->SqThrow("No row available");
    // Requirements satisfied
    else
        return true;
    // Need to reset the statement
    return false;
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
    if (m_Handle)
        m_Handle->Create(query);
    else
        _SqMod->SqThrow("Unable to create the statement reference");
}

// ------------------------------------------------------------------------------------------------
Statement::Statement(const Connection & connection, CSStr query)
    : m_Handle(connection.GetHandle())
{
    if (m_Handle)
        m_Handle->Create(query);
    else
        _SqMod->SqThrow("Unable to create the statement reference");
}

// ------------------------------------------------------------------------------------------------
Connection Statement::GetConnection() const
{
    // Validate the handle
    if (Validate())
        return Connection(m_Handle->mConn);
    // Request failed
    return Connection();
}

// ------------------------------------------------------------------------------------------------
void Statement::Reset()
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    // Specify that we don't have a row available and we haven't finished stepping
    m_Handle->mGood = false;
    m_Handle->mDone = false;
    // Attempt to reset the statement to it's initial state
    m_Handle = sqlite3_reset(m_Handle);
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow("Unable to reset statement [%s]", m_Handle.ErrStr());
}

// ------------------------------------------------------------------------------------------------
void Statement::Clear()
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    // Specify that we don't have a row available and we haven't finished stepping
    m_Handle->mGood = false;
    m_Handle->mDone = false;
    // Attempt to clear the statement
    m_Handle = sqlite3_clear_bindings(m_Handle);
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow("Unable to clear statement [%s]", m_Handle.ErrStr());
}

// ------------------------------------------------------------------------------------------------
Int32 Statement::Exec()
{
    // Validate the handle
    if (!Validate())
        return -1; // Unable to proceed
    // Did we reset first?
    else if (m_Handle->mDone)
    {
        _SqMod->SqThrow("Execute without resetting first");
        // Unable to proceed
        return -1;
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
        case SQLITE_ROW:    _SqMod->SqThrow("Results were found");
        case SQLITE_BUSY:   _SqMod->SqThrow("Database was busy");
        case SQLITE_ERROR:  _SqMod->SqThrow("Runtime error occurred");
        case SQLITE_MISUSE: _SqMod->SqThrow("Statement misuse");
        default:            _SqMod->SqThrow("Unknown failure");
    }
    // Operation failed
    return -1;
}

// ------------------------------------------------------------------------------------------------
bool Statement::Step()
{
    // Validate the handle
    if (!Validate())
        return false; // Unable to proceed
    // Did we reset first?
    else if (m_Handle->mDone)
    {
        _SqMod->SqThrow("Stepping without resetting first");
        // Unable to proceed
        return false;
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
        case SQLITE_BUSY:   _SqMod->SqThrow("Database was busy");
        case SQLITE_ERROR:  _SqMod->SqThrow("Runtime error occurred");
        case SQLITE_MISUSE: _SqMod->SqThrow("Statement misuse");
        default:            _SqMod->SqThrow("Unknown failure");
    }
    // Operation failed
    return false;
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindA(Int32 idx, Array & arr)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    // Know when to stop trying to bind values
    const Int32 max = sqlite3_bind_parameter_count(m_Handle);
    // Make sure that we are at least in bounds
    if (idx >= max)
        _SqMod->SqThrow("Parameter index out of range");
    // Should we clear onward parameters?
    else if (arr.Length() <= 0)
    {
        // Clear all parameters after the specified index
        for (Int32 idx = arr.Length(); idx < max; ++idx)
            sqlite3_bind_null(m_Handle, idx);
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
            IndexBind(idx++, *itr);
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindI(Int32 idx, Int32 value)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_int(m_Handle, idx, value);
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow(s_BadParamI, "int", idx, m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindL(Int32 idx, Object & value)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    else
        m_Handle = SQLITE_OK;
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Push the specified object onto the stack
    Var< Object & >::push(_SqVM, value);
    // The resulted long integer value
    Int64 longint = 0;
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(_SqMod->GetSLongValue(_SqVM, -1, &longint)))
        _SqMod->SqThrow("Invalid long integer specified");
    // Attempt to bind the specified value
    else
        m_Handle = sqlite3_bind_int(m_Handle, idx, longint);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow(s_BadParamI, "long", idx, m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindV(Int32 idx, SQInteger value)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    // Attempt to bind the specified value
#ifdef _SQ64
    m_Handle = sqlite3_bind_int64(m_Handle, idx, value);
#else
    m_Handle = sqlite3_bind_int(m_Handle, idx, value);
#endif // _SQ64
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow(s_BadParamI, "value", idx, m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindF(Int32 idx, SQFloat value)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_double(m_Handle, idx, value);
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow(s_BadParamI, "float", idx, m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindS(Int32 idx, CSStr value)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_text(m_Handle, idx, value, -1, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow(s_BadParamI, "string", idx, m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindB(Int32 idx, bool value)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_int(m_Handle, idx, value);
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow(s_BadParamI, "boolean", idx, m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBindN(Int32 idx)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_null(m_Handle, idx);
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow(s_BadParamI, "null", idx, m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindT(Table & tbl)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    // Should we clear the all the parameters?
    else if (tbl.GetSize() <= 0)
        Clear();
    // Attempt to assign the specified parameter values
    else
    {
        Table::iterator itr;
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
    if (!Validate())
        return; // Unable to proceed
    // Attempt to obtain the index of the specified parameter name
    const Int32 idx = sqlite3_bind_parameter_index(m_Handle, name);
    // Validate the obtained index
    if (!idx)
    {
        _SqMod->SqThrow("Unknown parameter named (%s)", name);
        // Unable to proceed
        return;
    }
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_int(m_Handle, idx, value);
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow(s_BadParamS, "int", name, idx, m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindL(CSStr name, Object & value)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    else
        m_Handle = SQLITE_OK;
    // Attempt to obtain the index of the specified parameter name
    const Int32 idx = sqlite3_bind_parameter_index(m_Handle, name);
    // Validate the obtained index
    if (!idx)
    {
        _SqMod->SqThrow("Unknown parameter named (%s)", name);
        // Unable to proceed
        return;
    }
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Push the specified object onto the stack
    Var< Object & >::push(_SqVM, value);
    // The resulted long integer value
    Uint64 longint = 0;
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(_SqMod->GetULongValue(_SqVM, -1, &longint)))
        _SqMod->SqThrow("Unknown long integer specified");
    // Attempt to bind the specified value
    else
        m_Handle = sqlite3_bind_int(m_Handle, idx, longint);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow(s_BadParamS, "long", name, idx, m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindV(CSStr name, SQInteger value)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    // Attempt to obtain the index of the specified parameter name
    const Int32 idx = sqlite3_bind_parameter_index(m_Handle, name);
    // Validate the obtained index
    if (!idx)
    {
        _SqMod->SqThrow("Unknown parameter named (%s)", name);
        // Unable to proceed
        return;
    }
    // Attempt to bind the specified value
#ifdef _SQ64
    m_Handle = sqlite3_bind_int64(m_Handle, idx, value);
#else
    m_Handle = sqlite3_bind_int(m_Handle, idx, value);
#endif // _SQ64
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow(s_BadParamS, "value", name, idx, m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindF(CSStr name, SQFloat value)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    // Attempt to obtain the index of the specified parameter name
    const Int32 idx = sqlite3_bind_parameter_index(m_Handle, name);
    // Validate the obtained index
    if (!idx)
    {
        _SqMod->SqThrow("Unknown parameter named (%s)", name);
        // Unable to proceed
        return;
    }
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_double(m_Handle, idx, value);
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow(s_BadParamS, "float", name, idx, m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindS(CSStr name, CSStr value)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    // Attempt to obtain the index of the specified parameter name
    const Int32 idx = sqlite3_bind_parameter_index(m_Handle, name);
    // Validate the obtained index
    if (!idx)
    {
        _SqMod->SqThrow("Unknown parameter named (%s)", name);
        // Unable to proceed
        return;
    }
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_text(m_Handle, idx, value, -1, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow(s_BadParamS, "string", name, idx, m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindB(CSStr name, bool value)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    // Attempt to obtain the index of the specified parameter name
    const Int32 idx = sqlite3_bind_parameter_index(m_Handle, name);
    // Validate the obtained index
    if (!idx)
    {
        _SqMod->SqThrow("Unknown parameter named (%s)", name);
        // Unable to proceed
        return;
    }
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_int(m_Handle, idx, value);
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow(s_BadParamS, "boolean", name, idx, m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBindN(CSStr name)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    // Attempt to obtain the index of the specified parameter name
    const Int32 idx = sqlite3_bind_parameter_index(m_Handle, name);
    // Validate the obtained index
    if (!idx)
    {
        _SqMod->SqThrow("Unknown parameter named (%s)", name);
        // Unable to proceed
        return;
    }
    // Attempt to bind the specified value
    m_Handle = sqlite3_bind_null(m_Handle, idx);
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow(s_BadParamS, "null", name, idx, m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
void Statement::IndexBind(Int32 idx, Object & value)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    else
        m_Handle = SQLITE_OK;
    // Attempt to identify the specified type
    switch (value.GetType())
    {
        // Is this an integer?
        case OT_INTEGER:
#ifdef _SQ64
           m_Handle = sqlite3_bind_int64(m_Handle, idx, value.Cast< SQInteger >());
#else
           m_Handle = sqlite3_bind_int(m_Handle, idx, value.Cast< SQInteger >());
#endif
        break;
        // Is this a float?
        case OT_FLOAT:
           m_Handle = sqlite3_bind_double(m_Handle, idx, value.Cast< SQFloat >());
        break;
        // Is this a boolean?
        case OT_BOOL:
           m_Handle = sqlite3_bind_int(m_Handle, idx, value.Cast< bool >());
        break;
        // Is this a string?
        case OT_STRING:
        {
            CSStr str = value.Cast< CSStr >();
            m_Handle = sqlite3_bind_text(m_Handle, idx, str, -1, SQLITE_TRANSIENT);
        } break;
        // Is this a null value?
        case OT_NULL:
           m_Handle = sqlite3_bind_null(m_Handle, idx);
        break;
        // Unsupported type
        default:
        {
            _SqMod->SqThrow("Attempting to bind unknown value type (%d)", idx);
            // Unable to proceed
            return;
        }
    }
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow(s_BadParamI, "auto", idx, m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
void Statement::NameBind(CSStr name, Object & value)
{
    // Validate the handle
    if (!Validate())
        return; // Unable to proceed
    // Attempt to obtain the index of the specified parameter name
    const Int32 idx = sqlite3_bind_parameter_index(m_Handle, name);
    // Validate the obtained index
    if (!idx)
    {
        _SqMod->SqThrow("Unknown parameter named (%s)", name);
        // Unable to proceed
        return;
    }
    // Attempt to identify the specified type
    switch (value.GetType())
    {
        // Is this an integer?
        case OT_INTEGER:
#ifdef _SQ64
            m_Handle = sqlite3_bind_int64(m_Handle, idx, value.Cast< SQInteger >());
#else
            m_Handle = sqlite3_bind_int(m_Handle, idx, value.Cast< SQInteger >());
#endif
        break;
        // Is this a floating point?
        case OT_FLOAT:
            m_Handle = sqlite3_bind_double(m_Handle, idx, value.Cast< SQFloat >());
        break;
        // Is this a boolean?
        case OT_BOOL:
            m_Handle = sqlite3_bind_int(m_Handle, idx, value.Cast< bool >());
        break;
        // Is this a string?
        case OT_STRING:
        {
            CSStr str = value.Cast< CSStr >();
            m_Handle = sqlite3_bind_text(m_Handle, idx, str, -1, SQLITE_TRANSIENT);
        } break;
        // Is this a null value?
        case OT_NULL:
            m_Handle = sqlite3_bind_null(m_Handle, idx);
        break;
        // Unsupported type
        default:
        {
            _SqMod->SqThrow("Attempting to bind unknown value type (%s:%d)", name, idx);
            // Unable to proceed
            return;
        }
    }
    // Validate the result
    if (m_Handle != SQLITE_OK)
        _SqMod->SqThrow(s_BadParamS, "auto", name, idx, m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
Object Statement::FetchColumnIndex(Int32 idx) const
{
    // Validate the handle and row
    if (!RowAvailable())
        return Object(); // Unable to proceed
    // Is the specified index valid?
    else if (!m_Handle->CheckIndex(idx))
    {
        _SqMod->SqThrow("Index out of range: %d", idx);
        // Unable to proceed
        return Object();
    }
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Identify which type of value must be pushed on the stack
    switch (sqlite3_column_type(m_Handle, idx))
    {
        // Is this a null value?
        case SQLITE_NULL:
            sq_pushnull(_SqVM);
        break;
        // Is this an integer?
        case SQLITE_INTEGER:
#ifdef _SQ64
            sq_pushinteger(_SqVM, sqlite3_column_int64(m_Handle, idx));
#else
            sq_pushinteger(_SqVM, sqlite3_column_int(m_Handle, idx));
#endif
        // Is this a floating point?
        case SQLITE_FLOAT:
            sq_pushfloat(_SqVM, sqlite3_column_double(m_Handle, idx));
        // Is this a string?
        case SQLITE_TEXT:
            sq_pushstring(_SqVM, (CSStr)sqlite3_column_text(m_Handle, idx),
                                        sqlite3_column_bytes(m_Handle, idx));
        // Is this raw data?
        case SQLITE_BLOB:
        {
            // Obtain the size of the data
            const Int32 sz = sqlite3_column_bytes(m_Handle, idx);
            // Allocate a blob of the same size
            SQUserPointer p = sqstd_createblob(_SqVM, sz);
            // Obtain a pointer to the data
            const void * b = sqlite3_column_blob(m_Handle, idx);
            // Could the memory blob be allocated?
            if (!p)
            {
                _SqMod->SqThrow("Unable to allocate space for column blob value");
                // Default to null
                sq_pushnull(_SqVM);
            }
            // Is there any data to read?
            else if (!b)
            {
                // Pop the memory blob from the stack
                sq_pop(_SqVM, sq_gettop(_SqVM) - top);
                // Now throw the error
                _SqMod->SqThrow("Unable to read data from column blob value");
                // Default to null
                sq_pushnull(_SqVM);
            }
            // Copy the data into the memory blob
            else
                memcpy(p, b, sz);
        }
        // Unknown type
        default:
        {
            _SqMod->SqThrow("Unknown value to fetch at index: %d", idx);
            // Default to null
            sq_pushnull(_SqVM);
        }
    }
    // Obtain the object with the value from the stack
    Var< Object > obj(_SqVM, -1);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Now return the obtained object
    return obj.value;
}

// ------------------------------------------------------------------------------------------------
Object Statement::FetchColumnName(CSStr name) const
{
    // Validate the handle and row
    if (RowAvailable())
        return FetchColumnIndex(m_Handle->GetColumnIndex(name));
    // Request failed
    return Object();
}

// ------------------------------------------------------------------------------------------------
Array Statement::FetchArray() const
{
    // Validate the handle
    if (Validate())
        return FetchArray(0, m_Handle->mColumns-1);
    // Request failed
    return Array();
}

// ------------------------------------------------------------------------------------------------
Array Statement::FetchArray(Int32 min) const
{
    // Validate the handle
    if (Validate())
        return FetchArray(min, m_Handle->mColumns-1);
    // Request failed
    return Array();
}

// ------------------------------------------------------------------------------------------------
Array Statement::FetchArray(Int32 min, Int32 max) const
{
    // Validate the handle
    if (!RowAvailable())
        return Array(); // Unable to proceed
    // Was there anything selected?
    else if (min == max)
        return Array(); // Nothing to retrieve
    // Is the minimum actually the minimum?
    else if (min > max)
    {
        _SqMod->SqThrow("Minimum is higher than maximum");
        // Unable to proceed
        return Array();
    }
    // Is the minimum in range>
    else if (!m_Handle->CheckIndex(min))
    {
        _SqMod->SqThrow("Minimum is out of range");
        // Unable to proceed
        return Array();
    }
    // Is the maximum in range?
    else if (!m_Handle->CheckIndex(max))
    {
        _SqMod->SqThrow("Maximum is out of range");
        // Unable to proceed
        return Array();
    }
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Allocate an array large enough to hold the values from selected columns
    Array arr(_SqVM, max-min);
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
                arr.SetValue(elem, (CCStr)(sqlite3_column_text(m_Handle, idx)));
            break;
            // Is this raw data?
            case SQLITE_BLOB:
                {
                    // Obtain the size of the data
                    const Int32 sz = sqlite3_column_bytes(m_Handle, idx);
                    // Allocate a blob of the same size
                    SQUserPointer p = sqstd_createblob(_SqVM, sz);
                    // Obtain a pointer to the data
                    const void * b = sqlite3_column_blob(m_Handle, idx);
                    // Could the memory blob be allocated?
                    if (!p)
                    {
                        _SqMod->SqThrow("Unable to allocate space for column blob value");
                        // Unable to proceed
                        return Array();
                    }
                    // Is there any data to read?
                    else if (!b)
                    {
                        // Pop the memory blob from the stack
                        sq_pop(_SqVM, sq_gettop(_SqVM) - top);
                        // Now throw the error
                        _SqMod->SqThrow("Unable to read data from column blob value");
                        // Unable to proceed
                        return Array();
                    }
                    else
                    {
                        // Copy the data into the memory blob
                        memcpy(p, b, sz);
                        // Obtain the object from the stack
                        Var< Object > obj(_SqVM, -1);
                        // Bind it as an array element
                        arr.Bind(elem, obj.value);
                        // Pop the memory blob from the stack
                        sq_pop(_SqVM, sq_gettop(_SqVM) - top);
                    }
                }
            break;
            // Unknown type
            default:
                _SqMod->SqThrow("Unknown value to fetch at index: %d", idx);
                // Unable to proceed
                return Array();
        }
    }
    // Return the resulted array
    return arr;
}

// ------------------------------------------------------------------------------------------------
Table Statement::FetchTable() const
{
    // Validate the handle
    if (Validate())
        return FetchTable(0, m_Handle->mColumns-1);
    // Request failed
    return Table();
}

// ------------------------------------------------------------------------------------------------
Table Statement::FetchTable(Int32 min) const
{
    // Validate the handle
    if (Validate())
        return FetchTable(min, m_Handle->mColumns-1);
    // Request failed
    return Table();
}

// ------------------------------------------------------------------------------------------------
Table Statement::FetchTable(Int32 min, Int32 max) const
{
    // Validate the handle
    if (!RowAvailable())
        return Table(); // Unable to proceed
    // Was there anything selected?
    else if (min == max)
        return Table(); // Nothing to retrieve
    // Is the minimum actually the minimum?
    else if (min > max)
    {
        _SqMod->SqThrow("Minimum is higher than maximum");
        // Unable to proceed
        return Table();
    }
    // Is the minimum in range>
    else if (!m_Handle->CheckIndex(min))
    {
        _SqMod->SqThrow("Minimum is out of range");
        // Unable to proceed
        return Table();
    }
    // Is the maximum in range?
    else if (!m_Handle->CheckIndex(max))
    {
        _SqMod->SqThrow("Maximum is out of range");
        // Unable to proceed
        return Table();
    }
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Allocate an array large enough to hold the values from selected columns
    Table tbl(_SqVM);
    // Used to bind null values
    Object obj;
    // Process the range of selected columns
    for (Int32 elem = 0, idx = min; idx < max; ++elem, ++idx)
    {
        //Attempt to obtain the column name
        CSStr name = sqlite3_column_name(m_Handle, idx);
        // Validate the obtained name
        if (!name)
        {
            _SqMod->SqThrow("Unable to retrieve name of column (%d)", idx);
            // Unable to proceed
            return Table();
        }
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
            }
            break;
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
                tbl.SetValue(name, (CCStr)(sqlite3_column_text(m_Handle, idx)));
            break;
            // Is this raw data?
            case SQLITE_BLOB:
                {
                    // Obtain the size of the data
                    const Int32 sz = sqlite3_column_bytes(m_Handle, idx);
                    // Allocate a blob of the same size
                    SQUserPointer p = sqstd_createblob(_SqVM, sz);
                    // Obtain a pointer to the data
                    const void * b = sqlite3_column_blob(m_Handle, idx);
                    // Could the memory blob be allocated?
                    if (!p)
                    {
                        _SqMod->SqThrow("Unable to allocate space for column blob value");
                        // Unable to proceed
                        return Table();
                    }
                    // Is there any data to read?
                    else if (!b)
                    {
                        // Pop the memory blob from the stack
                        sq_pop(_SqVM, sq_gettop(_SqVM) - top);
                        // Now throw the error
                        _SqMod->SqThrow("Unable to read data from column blob value");
                        // Unable to proceed
                        return Table();
                    }
                    else
                    {
                        // Copy the data into the memory blob
                        memcpy(p, b, sz);
                        // Obtain the object from the stack
                        Var< Object > obj(_SqVM, -1);
                        // Bind it as a table element
                        tbl.Bind(name, obj.value);
                        // Pop the memory blob from the stack
                        sq_pop(_SqVM, sq_gettop(_SqVM) - top);
                    }
                }
            break;
            // Unknown type
            default:
                _SqMod->SqThrow("Unknown value to fetch at index: %d", idx);
                // Unable to proceed
                return Table();
        }
    }
    // Return the resulted table
    return tbl;
}

// ------------------------------------------------------------------------------------------------
bool Statement::CheckIndex(Int32 idx) const
{
    // Validate the handle
    if (Validate())
        return m_Handle->CheckIndex(idx);
    // Invalid statement is same as invalid index
    return false;
}

// ------------------------------------------------------------------------------------------------
bool Statement::IsColumnNull(Int32 idx) const
{
    // Can we make the request?
    if (ValidateIndex(idx))
        return (sqlite3_column_type(m_Handle, idx) == SQLITE_NULL);
    // Request failed
    return true;
}

// ------------------------------------------------------------------------------------------------
Int32 Statement::GetColumnIndex(CSStr name) const
{
    // Validate the handle
    if (Validate())
        return m_Handle->GetColumnIndex(name);
    // Invalid statement is same as invalid index
    return -1;
}

// ------------------------------------------------------------------------------------------------
CSStr Statement::GetColumnName(Int32 idx) const
{
    // Can we make the request?
    if (ValidateIndex(idx))
        return sqlite3_column_name(m_Handle, idx);
    // Request failed
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
CSStr Statement::GetColumnOriginName(Int32 idx) const
{
#ifdef SQLITE_ENABLE_COLUMN_METADATA
    // Can we make the request?
    if (ValidateIndex(idx))
        return sqlite3_column_origin_name(m_Handle, idx);
#else
    SQMOD_UNUSED_VAR(idx);
    _SqMod->SqThrow("The module was compiled without this feature");
#endif
    // Request failed
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
Int32 Statement::GetColumnType(Int32 idx) const
{
    // Can we make the request?
    if (ValidateIndex(idx))
        return sqlite3_column_type(m_Handle, idx);
    // Request failed
    return -1;
}

// ------------------------------------------------------------------------------------------------
Int32 Statement::GetColumnBytes(Int32 idx) const
{
    // Can we make the request?
    if (ValidateIndex(idx))
        return sqlite3_column_bytes(m_Handle, idx);
    // Request failed
    return -1;
}

// ------------------------------------------------------------------------------------------------
Column Statement::GetColumnByIndex(Int32 idx) const
{
    // Can we make the request?
    if (ValidateIndex(idx))
        return Column(m_Handle, idx);
    // Request failed
    return Column();
}

// ------------------------------------------------------------------------------------------------
Column Statement::GetColumnByName(CSStr name) const
{
    // Validate the handle
    if (Validate())
    {
        // Attempt to obtain the requested column index
        const Int32 idx = m_Handle->GetColumnIndex(name);
        // Validate the obtained index
        if (idx < 0)
            _SqMod->SqThrow("Unknown column named (%s)", name);
        // Return the requested column
        else
            return Column(m_Handle, idx);
    }
    // Request failed
    return Column();
}

} // Namespace:: SqMod
