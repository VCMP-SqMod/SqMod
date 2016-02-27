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
void Column::Validate() const
{
    // Are we pointing to a valid index?
    if (m_Index < 0)
        SqThrowF("Invalid column index");
    // Do we belong to a valid statement?
    else if (!m_Stmt)
        SqThrowF("Invalid SQLite statement reference");
}

// ------------------------------------------------------------------------------------------------
void Column::ValidateRow() const
{
    // Are we pointing to a valid index?
    if (m_Index < 0)
        SqThrowF("Invalid column index");
    // Do we belong to a valid statement?
    else if (!m_Stmt)
        SqThrowF("Invalid SQLite statement reference");
    // Do we have any rows available?
    else if (!m_Stmt->mGood)
        SqThrowF("No row available");
}

// ------------------------------------------------------------------------------------------------
Object Column::GetStatement() const
{
    // Validate the column
    Validate();
    // Return the requested information
    return Object(new Statement(m_Stmt));
}

// ------------------------------------------------------------------------------------------------
Object Column::GetConnection() const
{
    // Validate the column
    Validate();
    // Return the requested information
    return Object(new Connection(m_Stmt->mConn));
}

// ------------------------------------------------------------------------------------------------
Int32 Column::GetNumber() const
{
    // Validate the column and statement row
    ValidateRow();
    // Return the requested information
    return sqlite3_column_int(m_Stmt, m_Index);
}

// ------------------------------------------------------------------------------------------------
SQInteger Column::GetInteger() const
{
    // Validate the column and statement row
    ValidateRow();
    // Return the requested information
#ifdef _SQ64
    return sqlite3_column_int64(m_Stmt, m_Index);
#else
    return sqlite3_column_int(m_Stmt, m_Index);
#endif
}

// ------------------------------------------------------------------------------------------------
SQFloat Column::GetFloat() const
{
    // Validate the column and statement row
    ValidateRow();
    // Return the requested information
    return (SQFloat)sqlite3_column_double(m_Stmt, m_Index);
}

// ------------------------------------------------------------------------------------------------
Object Column::GetLong() const
{
    // Validate the column and statement row
    ValidateRow();
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Push a long integer instance with the requested value on the stack
    _SqMod->PushSLongObject(_SqVM, sqlite3_column_int64(m_Stmt, m_Index));
    // Obtain the object from the stack
    Var< Object > inst(_SqVM, -1);
    // Remove any pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the long integer instance
    return inst.value;
}

// ------------------------------------------------------------------------------------------------
Object Column::GetString() const
{
    // Validate the column and statement row
    ValidateRow();
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Push the column text on the stack
    sq_pushstring(_SqVM, (CSStr)sqlite3_column_text(m_Stmt, m_Index),
                                sqlite3_column_bytes(m_Stmt, m_Index));
    // Obtain the object from the stack
    Var< Object > inst(_SqVM, -1);
    // Remove any pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the long integer instance
    return inst.value;
}

// ------------------------------------------------------------------------------------------------
bool Column::GetBoolean() const
{
    // Validate the column and statement row
    ValidateRow();
    // Return the requested information
    return sqlite3_column_int(m_Stmt, m_Index) > 0;
}

// ------------------------------------------------------------------------------------------------
Object Column::GetBlob() const
{
    // Validate the column and statement row
    ValidateRow();
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
        SqThrowF("Unable to allocate space for column blob value");
    // Is there any data to read?
    else if (!b)
    {
        // Pop the memory blob from the stack
        sq_pop(_SqVM, sq_gettop(_SqVM) - top);
        // Push a null value instead
        sq_pushnull(_SqVM);
    }
    // Copy the data into the memory blob
    else
        memcpy(p, b, sz);
    // Obtain the object from the stack
    Var< Object > inst(_SqVM, -1);
    // Remove any pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the blob instance
    return inst.value;
}

// ------------------------------------------------------------------------------------------------
SQChar Column::GetChar() const
{
    // Validate the column and statement row
    ValidateRow();
    // Return the requested information
    return (SQChar)sqlite3_column_int(m_Stmt, m_Index);
}

// ------------------------------------------------------------------------------------------------
bool Column::IsNull() const
{
    // Validate the column
    Validate();
    // Return the requested information
    return (sqlite3_column_type(m_Stmt, m_Index) == SQLITE_NULL);
}

// ------------------------------------------------------------------------------------------------
CSStr Column::GetName() const
{
    // Validate the column
    Validate();
    // Return the requested information
    return sqlite3_column_name(m_Stmt, m_Index);
}

// ------------------------------------------------------------------------------------------------
CSStr Column::GetOriginName() const
{
#ifdef SQLITE_ENABLE_COLUMN_METADATA
    // Validate the column
    Validate();
    // Return the requested information
    return sqlite3_column_origin_name(m_Stmt, m_Index);
#else
    SqThrowF("The module was compiled without this feature");
#endif
    // Request failed
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
Int32 Column::GetType() const
{
    // Validate the column
    Validate();
    // Return the requested information
    return sqlite3_column_type(m_Stmt, m_Index);
}

// ------------------------------------------------------------------------------------------------
Int32 Column::GetBytes() const
{
    // Validate the column
    Validate();
    // Return the requested information
    return sqlite3_column_bytes(m_Stmt, m_Index);
}

} // Namespace:: SqMod
