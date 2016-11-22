// ------------------------------------------------------------------------------------------------
#include "Column.hpp"

// ------------------------------------------------------------------------------------------------
#include <cerrno>
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("SqLiteColumn"))

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
void Column::ValidateColumn(Int32 idx, CCStr file, Int32 line) const
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
void Column::ValidateColumn(Int32 idx) const
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
void Column::SetIndex(const Object & column)
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
            const StackStrF val(vm, -1, false);
            // Validate the result
            if (SQ_FAILED(val.mRes))
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
            idx = ConvTo< Int32 >::From(SqMod_PopStackInteger(vm, -1));
        } break;
        // Is this an instance that we can extract either a string or integer from it?
        case OT_INSTANCE:
        {
            // Obtain the object from the stack as a string
            const StackStrF val(vm, -1, false);
            // Validate the result
            if (SQ_FAILED(val.mRes))
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
Object Column::GetStatement() const
{
    return GetStatementObj(m_Handle);
}

// ------------------------------------------------------------------------------------------------
Object Column::GetConnection() const
{
    return GetConnectionObj(SQMOD_GET_VALID(*this)->mConn);
}

// ------------------------------------------------------------------------------------------------
bool Column::IsNull() const
{
    return (sqlite3_column_type(SQMOD_GET_CREATED(*this)->mPtr, m_Index) == SQLITE_NULL);
}

// ------------------------------------------------------------------------------------------------
CSStr Column::GetName() const
{
    return sqlite3_column_name(SQMOD_GET_CREATED(*this)->mPtr, m_Index);
}

// ------------------------------------------------------------------------------------------------
CSStr Column::GetOriginName() const
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
Int32 Column::GetType() const
{
    return sqlite3_column_type(SQMOD_GET_CREATED(*this)->mPtr, m_Index);
}

// ------------------------------------------------------------------------------------------------
Int32 Column::GetBytes() const
{
    return sqlite3_column_bytes(SQMOD_GET_CREATED(*this)->mPtr, m_Index);
}

// ------------------------------------------------------------------------------------------------
Object Column::GetValue() const
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
            CCStr data = reinterpret_cast< CCStr >(sqlite3_column_blob(m_Handle->mPtr, m_Index));
            // Attempt to create a buffer with the blob data on the stack
            if (SQ_FAILED(SqMod_PushBufferData(DefaultVM::Get(), data, size, 0)))
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
    SQMOD_VALIDATE_ROW(*this);
    // Return the requested information
    return sqlite3_column_integer(m_Handle->mPtr, m_Index);
}

// ------------------------------------------------------------------------------------------------
SQFloat Column::GetFloat() const
{
    SQMOD_VALIDATE_ROW(*this);
    // Return the requested information
    return ConvTo< SQFloat >::From(sqlite3_column_double(m_Handle->mPtr, m_Index));
}

// ------------------------------------------------------------------------------------------------
Object Column::GetLong() const
{
    SQMOD_VALIDATE_ROW(*this);
    // Return the requested information
    return MakeSLongObj(sqlite3_column_int64(m_Handle->mPtr, m_Index));
}

// ------------------------------------------------------------------------------------------------
Object Column::GetString() const
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
bool Column::GetBoolean() const
{
    SQMOD_VALIDATE_ROW(*this);
    // Return the requested information
    return sqlite3_column_int(m_Handle->mPtr, m_Index) > 0;
}

// ------------------------------------------------------------------------------------------------
SQChar Column::GetChar() const
{
    SQMOD_VALIDATE_ROW(*this);
    // Return the requested information
    return (SQChar)sqlite3_column_int(m_Handle->mPtr, m_Index);
}

// ------------------------------------------------------------------------------------------------
Object Column::GetBuffer() const
{
    SQMOD_VALIDATE_ROW(*this);
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
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
Object Column::GetBlob() const
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

// ================================================================================================
void Register_Column(Table & sqlns)
{
    sqlns.Bind(_SC("Column"),
        Class< Column >(sqlns.GetVM(), Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< const Column & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Column::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Column::IsValid)
        .Prop(_SC("References"), &Column::GetRefCount)
        .Prop(_SC("Index"), &Column::GetIndex)
        .Prop(_SC("Statement"), &Column::GetStatement)
        .Prop(_SC("Connection"), &Column::GetConnection)
        .Prop(_SC("IsNull"), &Column::IsNull)
        .Prop(_SC("Name"), &Column::GetName)
        .Prop(_SC("OriginName"), &Column::GetOriginName)
        .Prop(_SC("Type"), &Column::GetType)
        .Prop(_SC("Bytes"), &Column::GetBytes)
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
        // Member Methods
        .Func(_SC("Release"), &Column::Release)
    );
}

} // Namespace:: SqMod
