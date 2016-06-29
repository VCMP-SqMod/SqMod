// ------------------------------------------------------------------------------------------------
#include "ResultSet.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMySQLResultSet");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void ResultSet::Validate(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference =>[%s:%d]", file, line);
    }
}
#else
void ResultSet::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void ResultSet::ValidateCreated(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference =>[%s:%d]", file, line);
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid MySQL result-set =>[%s:%d]", file, line);
    }
}
#else
void ResultSet::ValidateCreated() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference");
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid MySQL result-set");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const ResRef & ResultSet::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const ResRef & ResultSet::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const ResRef & ResultSet::GetCreated(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    return m_Handle;
}
#else
const ResRef & ResultSet::GetCreated() const
{
    ValidateCreated();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void ResultSet::ValidateField(Int32 idx, CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    m_Handle->ValidateField(idx, file, line);
}
#else
void ResultSet::ValidateField(Int32 idx) const
{
    ValidateCreated();
    m_Handle->ValidateField(idx);
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::GetInt8(Uint32 idx) const
{
    SQMOD_VALIDATE_FIELD(*this, idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Int8 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return ConvTo< Int8 >::From(std::strtol(m_Handle->mRow[idx], nullptr, 10));
}

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::GetUint8(Uint32 idx) const
{
    SQMOD_VALIDATE_FIELD(*this, idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Uint8 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return ConvTo< Uint8 >::From(std::strtoul(m_Handle->mRow[idx], nullptr, 10));
}

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::GetInt16(Uint32 idx) const
{
    SQMOD_VALIDATE_FIELD(*this, idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Int16 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return ConvTo< Int16 >::From(std::strtol(m_Handle->mRow[idx], nullptr, 10));
}

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::GetUint16(Uint32 idx) const
{
    SQMOD_VALIDATE_FIELD(*this, idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Uint16 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return ConvTo< Uint16 >::From(std::strtoul(m_Handle->mRow[idx], nullptr, 10));
}

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::GetInt32(Uint32 idx) const
{
    SQMOD_VALIDATE_FIELD(*this, idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Int32 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return ConvTo< Int32 >::From(std::strtol(m_Handle->mRow[idx], nullptr, 10));
}

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::GetUint32(Uint32 idx) const
{
    SQMOD_VALIDATE_FIELD(*this, idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Uint32 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return ConvTo< Uint32 >::From(std::strtoul(m_Handle->mRow[idx], nullptr, 10));
}

// ------------------------------------------------------------------------------------------------
Int64 ResultSet::GetInt64(Uint32 idx) const
{
    SQMOD_VALIDATE_FIELD(*this, idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Int64 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return std::strtoll(m_Handle->mRow[idx], nullptr, 10);
}

// ------------------------------------------------------------------------------------------------
Uint64 ResultSet::GetUint64(Uint32 idx) const
{
    SQMOD_VALIDATE_FIELD(*this, idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Uint64 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return std::strtoull(m_Handle->mRow[idx], nullptr, 10);
}

// ------------------------------------------------------------------------------------------------
SQFloat ResultSet::GetFloat32(Uint32 idx) const
{
    SQMOD_VALIDATE_FIELD(*this, idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Float32 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return std::strtof(m_Handle->mRow[idx], nullptr);
}

// ------------------------------------------------------------------------------------------------
SQFloat ResultSet::GetFloat64(Uint32 idx) const
{
    SQMOD_VALIDATE_FIELD(*this, idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Float64 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return std::strtod(m_Handle->mRow[idx], nullptr);
}

// ------------------------------------------------------------------------------------------------
bool ResultSet::GetBoolean(Uint32 idx) const
{
    SQMOD_VALIDATE_FIELD(*this, idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< bool >::From(m_Handle->mBinds[idx].mUint64);
    }
    // Retrieve the value directly from the row
    return ConvTo< bool >::From(std::strtol(m_Handle->mRow[idx], nullptr, 10));
}

// ================================================================================================
void Register_ResultSet(Table & sqlns)
{
    sqlns.Bind(_SC("ResultSet")
        , Class< ResultSet >(sqlns.GetVM(), _SC("SqMySQLResultSet"))
        // Constructors
        .Ctor()
        .Ctor< const ResultSet & >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &ResultSet::Cmp)
        .SquirrelFunc(_SC("_typename"), &ResultSet::Typename)
        .Func(_SC("_tostring"), &ResultSet::ToString)
        // Properties
        .Prop(_SC("IsValid"), &ResultSet::IsValid)
        .Prop(_SC("RowIndex"), &ResultSet::RowIndex)
        .Prop(_SC("RowCount"), &ResultSet::RowCount)
        // Member Methods
        .Func(_SC("Next"), &ResultSet::Next)
        .Func(_SC("SetRowIndex"), &ResultSet::SetRowIndex)
        .Func(_SC("SetLongRowIndex"), &ResultSet::SetLongRowIndex)
        .Func(_SC("GetInt8"), &ResultSet::GetInt8)
        .Func(_SC("GetUint8"), &ResultSet::GetUint8)
        .Func(_SC("GetInt16"), &ResultSet::GetInt16)
        .Func(_SC("GetUint16"), &ResultSet::GetUint16)
        .Func(_SC("GetInt32"), &ResultSet::GetInt32)
        .Func(_SC("GetUint32"), &ResultSet::GetUint32)
        .Func(_SC("GetInt64"), &ResultSet::GetInt64)
        .Func(_SC("GetUint64"), &ResultSet::GetUint64)
        .Func(_SC("GetFloat32"), &ResultSet::GetFloat32)
        .Func(_SC("GetFloat64"), &ResultSet::GetFloat64)
        .Func(_SC("GetBool"), &ResultSet::GetBoolean)
        .Func(_SC("GetBoolean"), &ResultSet::GetBoolean)
    );
}

} // Namespace:: SqMod
