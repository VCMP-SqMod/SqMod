// ------------------------------------------------------------------------------------------------
#include "Column.hpp"
#include "Connection.hpp"
#include "Statement.hpp"

// ------------------------------------------------------------------------------------------------
#include <cerrno>
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Column::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqSQLiteColumn");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Column::Validate(CCStr file, Int32 line) const
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
void Column::Validate() const
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
void Column::ValidateCreated(CCStr file, Int32 line) const
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
void Column::ValidateCreated() const
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
const StmtRef & Column::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const StmtRef & Column::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const StmtRef & Column::GetCreated(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    return m_Handle;
}
#else
const StmtRef & Column::GetCreated() const
{
    ValidateCreated();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Column::ValidateRow(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    // Do we have any rows available?
    if (!m_Handle->mGood)
    {
        STHROWF("No row available =>[%s:%d]", file, line);
    }
}
#else
void Column::ValidateRow() const
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
Object Column::GetStatement() const
{
    // Return the requested information
    return Object(new Statement(m_Handle));
}

// ------------------------------------------------------------------------------------------------
Object Column::GetConnection() const
{
    VALIDATE_HND(*this);
    // Return the requested information
    return Object(new Connection(m_Handle->mConn));
}

// ------------------------------------------------------------------------------------------------
Object Column::GetValue() const
{
    VALIDATE_ROW_HND(*this);
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
            CCStr data = reinterpret_cast< CCStr >(sqlite3_column_blob(m_Handle->mPtr, m_Index));
            // Attempt to create a buffer with the blob data on the stack
            if (SQ_FAILED(SqMod_PushBufferData(DefaultVM::Get(), data, size, size)))
            {
                STHROWF("Unable to allocate buffer of at least (%d) bytes", size);
            }
        } break;
        // Unknown type
        default: STHROWF("Unknown value to fetch at index: %d", m_Index);
    }
    // Obtain the object with the value from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
Object Column::GetNumber() const
{
    VALIDATE_ROW_HND(*this);
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
            CSStr str = reinterpret_cast< CSStr >(sqlite3_column_text(m_Handle->mPtr, m_Index));
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
SQInteger Column::GetInteger() const
{
    VALIDATE_ROW_HND(*this);
    // Return the requested information
    return sqlite3_column_integer(m_Handle->mPtr, m_Index);
}

// ------------------------------------------------------------------------------------------------
SQFloat Column::GetFloat() const
{
    VALIDATE_ROW_HND(*this);
    // Return the requested information
    return ConvTo< SQFloat >::From(sqlite3_column_double(m_Handle->mPtr, m_Index));
}

// ------------------------------------------------------------------------------------------------
Object Column::GetLong() const
{
    VALIDATE_ROW_HND(*this);
    // Return the requested information
    return MakeSLongObj(sqlite3_column_int64(m_Handle->mPtr, m_Index));
}

// ------------------------------------------------------------------------------------------------
Object Column::GetString() const
{
    VALIDATE_ROW_HND(*this);
    // Obtain the initial stack size
    const StackGuard sg(_SqVM);
    // Push the column text on the stack
    sq_pushstring(_SqVM, reinterpret_cast< CSStr >(sqlite3_column_text(m_Handle->mPtr, m_Index)),
                            sqlite3_column_bytes(m_Handle->mPtr, m_Index));
    // Get the object from the stack and return it
    return Var< Object >(_SqVM, -1).value;
}

// ------------------------------------------------------------------------------------------------
bool Column::GetBoolean() const
{
    VALIDATE_ROW_HND(*this);
    // Return the requested information
    return sqlite3_column_int(m_Handle->mPtr, m_Index) > 0;
}

// ------------------------------------------------------------------------------------------------
SQChar Column::GetChar() const
{
    VALIDATE_ROW_HND(*this);
    // Return the requested information
    return (SQChar)sqlite3_column_int(m_Handle->mPtr, m_Index);
}

// ------------------------------------------------------------------------------------------------
Object Column::GetBuffer() const
{
    VALIDATE_ROW_HND(*this);
    // Remember the current stack size
    const StackGuard sg;
    // Retrieve the size of the blob that must be allocated
    const Int32 size = sqlite3_column_bytes(m_Handle->mPtr, m_Index);
    // Retrieve the the actual blob data that must be returned
    CCStr data = reinterpret_cast< CCStr >(sqlite3_column_blob(m_Handle->mPtr, m_Index));
    // Attempt to create a buffer with the blob data on the stack
    if (SQ_FAILED(SqMod_PushBufferData(DefaultVM::Get(), data, size, size)))
    {
        STHROWF("Unable to allocate buffer of at least (%d) bytes", size);
    }
    // Get the object from the stack and return it
    return Var< Object >(_SqVM, -1).value;
}

// ------------------------------------------------------------------------------------------------
Object Column::GetBlob() const
{
    VALIDATE_ROW_HND(*this);
    // Obtain the initial stack size
    const StackGuard sg(_SqVM);
    // Obtain the size of the data
    const Int32 sz = sqlite3_column_bytes(m_Handle->mPtr, m_Index);
    // Allocate a blob of the same size
    SQUserPointer p = sqstd_createblob(_SqVM, sz);
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
        sq_pop(_SqVM, 1);
        // Push a null value instead
        sq_pushnull(_SqVM);
    }
    // Copy the data into the memory blob
    else
    {
        std::memcpy(p, b, sz);
    }
    // Get the object from the stack and return it
    return Var< Object >(_SqVM, -1).value;
}

// ------------------------------------------------------------------------------------------------
bool Column::IsNull() const
{
    VALIDATE_CREATED_HND(*this);
    // Return the requested information
    return (sqlite3_column_type(m_Handle->mPtr, m_Index) == SQLITE_NULL);
}

// ------------------------------------------------------------------------------------------------
CSStr Column::GetName() const
{
    VALIDATE_CREATED_HND(*this);
    // Return the requested information
    return sqlite3_column_name(m_Handle->mPtr, m_Index);
}

// ------------------------------------------------------------------------------------------------
CSStr Column::GetOriginName() const
{
#ifdef SQLITE_ENABLE_COLUMN_METADATA
    VALIDATE_CREATED_HND(*this);
    // Return the requested information
    return sqlite3_column_origin_name(m_Handle->mPtr, m_Index);
#else
    STHROWF("The module was compiled without this feature");
    // Request failed
    return _SC("");
#endif
}

// ------------------------------------------------------------------------------------------------
Int32 Column::GetType() const
{
    VALIDATE_CREATED_HND(*this);
    // Return the requested information
    return sqlite3_column_type(m_Handle->mPtr, m_Index);
}

// ------------------------------------------------------------------------------------------------
Int32 Column::GetBytes() const
{
    VALIDATE_CREATED_HND(*this);
    // Return the requested information
    return sqlite3_column_bytes(m_Handle->mPtr, m_Index);
}

// ================================================================================================
void Register_Column(Table & sqlns)
{
    sqlns.Bind(_SC("Column"),
        Class< Column >(sqlns.GetVM(), _SC("SqSQLiteColumn"))
        // Constructors
        .Ctor()
        .Ctor< const Column & >()
        // Meta-methods
        .Func(_SC("_cmp"), &Column::Cmp)
        .SquirrelFunc(_SC("_typename"), &Column::Typename)
        .Func(_SC("_tostring"), &Column::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Column::IsValid)
        .Prop(_SC("References"), &Column::GetRefCount)
        .Prop(_SC("Index"), &Column::GetIndex)
        .Prop(_SC("Statement"), &Column::GetNumber)
        .Prop(_SC("Connection"), &Column::GetConnection)
        .Prop(_SC("Value"), &Column::GetValue)
        .Prop(_SC("Number"), &Column::GetNumber)
        .Prop(_SC("Integer"), &Column::GetInteger)
        .Prop(_SC("Float"), &Column::GetFloat)
        .Prop(_SC("Long"), &Column::GetLong)
        .Prop(_SC("String"), &Column::GetString)
        .Prop(_SC("Boolean"), &Column::GetBoolean)
        .Prop(_SC("Char"), &Column::GetChar)
        .Prop(_SC("Buffer"), &Column::GetBuffer)
        .Prop(_SC("Blob"), &Column::GetBlob)
        .Prop(_SC("IsNull"), &Column::IsNull)
        .Prop(_SC("Name"), &Column::GetName)
        .Prop(_SC("OriginName"), &Column::GetOriginName)
        .Prop(_SC("Type"), &Column::GetType)
        .Prop(_SC("Bytes"), &Column::GetBytes)
        // Member Methods
        .Func(_SC("Release"), &Column::Release)
    );
}

} // Namespace:: SqMod
