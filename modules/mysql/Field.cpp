// ------------------------------------------------------------------------------------------------
#include "Field.hpp"

// ------------------------------------------------------------------------------------------------
#include <cerrno>
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

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
const Uint32 Field::INVALID_INDEX = std::numeric_limits< Uint32 >::max();

// ------------------------------------------------------------------------------------------------
SQInteger Field::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMySQLField");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Field::Validate(CCStr file, Int32 line) const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference =>[%s:%d]", file, line);
    }
    // Are we pointing to a valid index?
    else if (m_Index >= m_Handle->mFieldCount)
    {
        SqThrowF("Field index is out of range: %u >= %lu =>[%s:%d]",
                    m_Index, m_Handle->mFieldCount, file, line);
    }
}
#else
void Field::Validate() const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference");
    }
    // Are we pointing to a valid index?
    else if (m_Index >= m_Handle->mFieldCount)
    {
        SqThrowF("Field index is out of range: %u >= %lu", m_Index, m_Handle->mFieldCount);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Field::ValidateCreated(CCStr file, Int32 line) const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference =>[%s:%d]", file, line);
    }
    // Are we pointing to a valid index?
    m_Handle->ValidateField(m_Index, file, line);
}
#else
void Field::ValidateCreated() const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference");
    }
    // Are we pointing to a valid index?
    m_Handle->ValidateField(m_Index);
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Field::ValidateStepped(CCStr file, Int32 line) const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference =>[%s:%d]", file, line);
    }
    // Do we have a valid row available?
    else if (m_Handle->mRow == nullptr)
    {
        SqThrowF("No row available in MySQL result-set =>[%s:%d]", file, line);
    }
    // Are we pointing to a valid index?
    m_Handle->ValidateField(m_Index, file, line);
}
#else
void Field::ValidateStepped() const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference");
    }
    // Do we have a valid row available?
    else if (m_Handle->mRow == nullptr)
    {
        SqThrowF("No row available in MySQL result-set");
    }
    // Are we pointing to a valid index?
    m_Handle->ValidateField(m_Index);
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const ResRef & Field::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const ResRef & Field::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const ResRef & Field::GetCreated(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    return m_Handle;
}
#else
const ResRef & Field::GetCreated() const
{
    ValidateCreated();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const ResRef & Field::GetStepped(CCStr file, Int32 line) const
{
    ValidateStepped(file, line);
    return m_Handle;
}
#else
const ResRef & Field::GetStepped() const
{
    ValidateStepped();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Field::ValidateField(Uint32 idx, CCStr file, Int32 line) const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference =>[%s:%d]", file, line);
    }
    // Validate the specified field index
    m_Handle->ValidateField(idx, file, line);
}
#else
void Field::ValidateField(Uint32 idx) const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference");
    }
    // Validate the specified field index
    m_Handle->ValidateField(idx);
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
void Field::SetIndex(const Object & field)
{
    // Where the index will be extracted
    Uint32 idx = INVALID_INDEX;
    // Grab the object virtual machine
    HSQUIRRELVM vm = field.GetVM();
    // Remember the current stack size
    const StackGuard sg(vm);
    // Push the specified object onto the stack
    Var< const Object & >::push(vm, field);
    // Identify the type of field was given
    switch  (field.GetType())
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
                STHROWF("Cannot use an empty field name");
            }
            // Attempt to find a field with the specified name
            idx = m_Handle->GetFieldIndex(val.mPtr);
        } break;
        // Is this an integer value? (or at least can be easily converted to one)
        case OT_INTEGER:
        case OT_FLOAT:
        case OT_BOOL:
        {
            idx = ConvTo< Uint32 >::From(SqMod_PopStackInteger(vm, -1));
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
                STHROWF("Cannot use an empty field name");
            }
            // Check if this value is made only of digits
            else if (IsDigitsOnly(val.mPtr))
            {
                idx = ConvNum< Uint32 >::FromStr(val.mPtr);
            }
            // Attempt to find a field with the specified name
            else
            {
                idx = m_Handle->GetFieldIndex(val.mPtr);
            }
        } break;
        // We don't recognize this kind of value!
        default: STHROWF("Unknown field index of type (%s)", SqTypeName(field.GetType()));
    }
    // Validate the obtained field index
    SQMOD_VALIDATE_FIELD(*this, idx);
    // Assign the new index
    m_Index = idx;
}

// ------------------------------------------------------------------------------------------------
Object Field::GetResultSet() const
{
    return Object();
}

// ------------------------------------------------------------------------------------------------
Object Field::GetConnection() const
{
    return Object();
}

// ------------------------------------------------------------------------------------------------
bool Field::GetBoolean() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< bool >::From(m_Handle->mBinds[m_Index].mUint64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< bool >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQChar Field::GetChar() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< SQChar >::From(m_Handle->mBinds[m_Index].mInt32[0]);
    }
    // Retrieve the value directly from the row
    return DbConvTo< SQChar >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQInteger Field::GetInteger() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
#ifdef _SQ64
        return ConvTo< SQInteger >::From(m_Handle->mBinds[m_Index].mInt64);
#else
        return ConvTo< SQInteger >::From(m_Handle->mBinds[m_Index].mInt32[0]);
#endif  // _SQ64
    }
    // Retrieve the value directly from the row
    return DbConvTo< SQInteger >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQFloat Field::GetFloat() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
#ifdef SQUSEDOUBLE
        return ConvTo< SQFloat >::From(m_Handle->mBinds[m_Index].mFloat64);
#else
        return ConvTo< SQFloat >::From(m_Handle->mBinds[m_Index].mFloat32[0]);
#endif // SQUSEDOUBLE
    }
    // Retrieve the value directly from the row
    return DbConvTo< SQFloat >::From(m_Handle->mRow[m_Index],
                                        m_Handle->mLengths[m_Index],
                                        m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQInteger Field::GetInt8() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Int8 >::From(m_Handle->mBinds[m_Index].mInt64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< Int8 >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQInteger Field::GetUint8() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Uint8 >::From(m_Handle->mBinds[m_Index].mInt64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< Uint8 >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQInteger Field::GetInt16() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Int16 >::From(m_Handle->mBinds[m_Index].mInt64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< Int16 >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQInteger Field::GetUint16() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Uint16 >::From(m_Handle->mBinds[m_Index].mInt64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< Uint16 >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQInteger Field::GetInt32() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Int32 >::From(m_Handle->mBinds[m_Index].mInt64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< Int32 >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQInteger Field::GetUint32() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Uint32 >::From(m_Handle->mBinds[m_Index].mInt64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< Uint32 >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
Object Field::GetInt64() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Obtain the initial stack size
    const StackGuard sg;
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        SqMod_PushSLongObject(DefaultVM::Get(),
                                ConvTo< Int64 >::From(m_Handle->mBinds[m_Index].mInt64));
    }
    // Retrieve the value directly from the row
    else
    {
        SqMod_PushSLongObject(DefaultVM::Get(),
            DbConvTo< Int64 >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type));
    }
    // Obtain the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
Object Field::GetUint64() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Obtain the initial stack size
    const StackGuard sg;
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        SqMod_PushULongObject(DefaultVM::Get(),
                                ConvTo< Uint64 >::From(m_Handle->mBinds[m_Index].mUint64));
    }
    // Retrieve the value directly from the row
    else
    {
        SqMod_PushULongObject(DefaultVM::Get(),
            DbConvTo< Uint64 >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type));
    }
    // Obtain the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
SQFloat Field::GetFloat32() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Float32 >::From(m_Handle->mBinds[m_Index].mFloat32[0]);
    }
    // Retrieve the value directly from the row
    return DbConvTo< Float32 >::From(m_Handle->mRow[m_Index],
                                        m_Handle->mLengths[m_Index],
                                        m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQFloat Field::GetFloat64() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Float64 >::From(m_Handle->mBinds[m_Index].mFloat64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< Float64 >::From(m_Handle->mRow[m_Index],
                                        m_Handle->mLengths[m_Index],
                                        m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
Object Field::GetString() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Obtain the initial stack size
    const StackGuard sg;
    // Retrieve the value directly from the row and push it on the stack
    sq_pushstring(DefaultVM::Get(), m_Handle->mRow[m_Index], m_Handle->mLengths[m_Index]);
    // Obtain the object from the stack
    Object stro(-1, DefaultVM::Get());
    // Restore the stack
    sg.Restore();
    // Return it the string object
    return stro;
}

// ------------------------------------------------------------------------------------------------
Object Field::GetBuffer() const
{
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
Object Field::GetBlob() const
{
    return NullObject();
}

// ================================================================================================
void Register_Field(Table & sqlns)
{
    sqlns.Bind(_SC("Field"),
        Class< Field >(sqlns.GetVM(), _SC("SqMySQLField"))
        // Constructors
        .Ctor()
        .Ctor< const Field & >()
        // Meta-methods
        .Func(_SC("_cmp"), &Field::Cmp)
        .SquirrelFunc(_SC("_typename"), &Field::Typename)
        .Func(_SC("_tostring"), &Field::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Field::IsValid)
        .Prop(_SC("References"), &Field::GetRefCount)
        .Prop(_SC("Index"), &Field::GetIndex)
        .Prop(_SC("ResultSet"), &Field::GetResultSet)
        .Prop(_SC("Connection"), &Field::GetConnection)
        .Prop(_SC("Bool"), &Field::GetBoolean)
        .Prop(_SC("Boolean"), &Field::GetBoolean)
        .Prop(_SC("Char"), &Field::GetChar)
        .Prop(_SC("Integer"), &Field::GetInteger)
        .Prop(_SC("Float"), &Field::GetFloat)
        .Prop(_SC("Int8"), &Field::GetInt8)
        .Prop(_SC("Uint8"), &Field::GetUint8)
        .Prop(_SC("Int16"), &Field::GetInt16)
        .Prop(_SC("Uint16"), &Field::GetUint16)
        .Prop(_SC("Int32"), &Field::GetInt32)
        .Prop(_SC("Uint32"), &Field::GetUint32)
        .Prop(_SC("Int64"), &Field::GetInt64)
        .Prop(_SC("Uint64"), &Field::GetUint64)
        .Prop(_SC("Float32"), &Field::GetFloat32)
        .Prop(_SC("Float64"), &Field::GetFloat64)
        .Prop(_SC("String"), &Field::GetString)
        .Prop(_SC("Buffer"), &Field::GetBuffer)
        .Prop(_SC("Blob"), &Field::GetBlob)
        // Member Methods
        .Func(_SC("Release"), &Field::Release)
    );
}

} // Namespace:: SqMod
