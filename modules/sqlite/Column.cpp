// ------------------------------------------------------------------------------------------------
#include "Column.hpp"
#include "Connection.hpp"
#include "Statement.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Column::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqSQLiteColumn");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
bool Column::Validate() const
{
    // Are we pointing to a valid index?
    if (m_Index < 0)
        _SqMod->SqThrow("Invalid column index");
    // Do we belong to a valid statement?
    else if (!m_Stmt)
        _SqMod->SqThrow("Invalid SQLite statement reference");
    // Requirements satisfied
    else
        return true;
    // Validation failed
    return false;
}

// ------------------------------------------------------------------------------------------------
bool Column::RowAvailable() const
{
    // Are we pointing to a valid index?
    if (m_Index < 0)
        _SqMod->SqThrow("Invalid column index");
    // Do we belong to a valid statement?
    else if (!m_Stmt)
        _SqMod->SqThrow("Invalid SQLite statement reference");
    // Do we have any rows available?
    else if (!m_Stmt->mGood)
        _SqMod->SqThrow("No row available");
    // Requirements satisfied
    else
        return true;
    // Validation failed
    return false;
}

// ------------------------------------------------------------------------------------------------
Statement Column::GetStatement() const
{
    // Validate the handle
    if (Validate())
        return Statement(m_Stmt);
    // Request failed
    return Statement();
}

// ------------------------------------------------------------------------------------------------
Connection Column::GetConnection() const
{
    // Validate the handle
    if (Validate())
        return Connection(m_Stmt->mConn);
    // Request failed
    return Connection();
}

// ------------------------------------------------------------------------------------------------
Int32 Column::GetNumber() const
{
    // Validate the handle and index
    if (RowAvailable())
        return sqlite3_column_int(m_Stmt, m_Index);
    // Request failed
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger Column::GetInteger() const
{
    // Validate the handle and index
    if (RowAvailable())
#ifdef _SQ64
        return sqlite3_column_int64(m_Stmt, m_Index);
#else
        return sqlite3_column_int(m_Stmt, m_Index);
#endif
    // Request failed
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQFloat Column::GetFloat() const
{
    // Validate the handle and index
    if (RowAvailable())
        return (SQFloat)sqlite3_column_double(m_Stmt, m_Index);
    // Request failed
    return SQFloat(0.0);
}

// ------------------------------------------------------------------------------------------------
Object Column::GetLong() const
{
    // Validate the handle and index
    if (!RowAvailable())
        return Object(); // Request failed
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Push a long integer instance with the requested value on the stack
    _SqMod->PushSLongObject(_SqVM, sqlite3_column_int64(m_Stmt, m_Index));
    // Obtain the object from the stack
    Var< Object > inst(_SqVM, -1);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the long integer instance
    return inst.value;
}

// ------------------------------------------------------------------------------------------------
Object Column::GetString() const
{
    // Validate the handle and index
    if (RowAvailable())
        return Object(); // Request failed
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Push the column text on the stack
    sq_pushstring(_SqVM, (CSStr)sqlite3_column_text(m_Stmt, m_Index),
                                sqlite3_column_bytes(m_Stmt, m_Index));
    // Obtain the object from the stack
    Var< Object > inst(_SqVM, -1);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the long integer instance
    return inst.value;
}

// ------------------------------------------------------------------------------------------------
bool Column::GetBoolean() const
{
    // Validate the handle and index
    if (RowAvailable())
        return sqlite3_column_int(m_Stmt, m_Index) > 0;
    // Request failed
    return false;
}

// ------------------------------------------------------------------------------------------------
Object Column::GetBlob() const
{
    // Validate the handle and index
    if (RowAvailable())
        return Object(); // Request failed
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Obtain the size of the data
    const Int32 sz = sqlite3_column_bytes(m_Stmt, m_Index);
    // Allocate a blob of the same size
    SQUserPointer p = sqstd_createblob(_SqVM, sz);
    // Obtain a pointer to the data
    const void * b = sqlite3_column_blob(m_Stmt, m_Index);
    // Could the memory blob be allocated?
    if (!p)
    {
        _SqMod->SqThrow("Unable to allocate space for column blob value");
        // Request failed
        return Object();
    }
    // Is there any data to read?
    else if (!b)
    {
        // Pop the memory blob from the stack
        sq_pop(_SqVM, sq_gettop(_SqVM) - top);
        // Now throw the error
        _SqMod->SqThrow("Unable to read data from column blob value");
        // Request failed
        return Object();
    }
    // Copy the data into the memory blob
    else
        memcpy(p, b, sz);
    // Obtain the object from the stack
    Var< Object > inst(_SqVM, -1);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the long integer instance
    return inst.value;
}

// ------------------------------------------------------------------------------------------------
SQChar Column::GetChar() const
{
    // Validate the handle and index
    if (RowAvailable())
        return (SQChar)sqlite3_column_int(m_Stmt, m_Index);
    // Request failed
    return 0;
}

// ------------------------------------------------------------------------------------------------
bool Column::IsNull() const
{
    // Can we make the request?
    if (Validate())
        return (sqlite3_column_type(m_Stmt, m_Index) == SQLITE_NULL);
    // Request failed
    return true;
}

// ------------------------------------------------------------------------------------------------
CSStr Column::GetName() const
{
    // Can we make the request?
    if (Validate())
        return sqlite3_column_name(m_Stmt, m_Index);
    // Request failed
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
CSStr Column::GetOriginName() const
{
#ifdef SQLITE_ENABLE_COLUMN_METADATA
    // Can we make the request?
    if (Validate())
        return sqlite3_column_origin_name(m_Stmt, m_Index);
#else
    _SqMod->SqThrow("The module was compiled without this feature");
#endif
    // Request failed
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
Int32 Column::GetType() const
{
    // Can we make the request?
    if (Validate())
        return sqlite3_column_type(m_Stmt, m_Index);
    // Request failed
    return -1;
}

// ------------------------------------------------------------------------------------------------
Int32 Column::GetBytes() const
{
    // Can we make the request?
    if (Validate())
        return sqlite3_column_bytes(m_Stmt, m_Index);
    // Request failed
    return -1;
}


} // Namespace:: SqMod
