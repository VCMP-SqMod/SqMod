// ------------------------------------------------------------------------------------------------
#include "ResultSet.hpp"

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
Int32 ResultSet::Cmp(const ResultSet & o) const
{
    if (m_Handle == o.m_Handle)
    {
        return 0;
    }
    else if (m_Handle.HndPtr() > o.m_Handle.HndPtr())
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
CSStr ResultSet::ToString() const
{
    // Do we have a valid handle?
    if (m_Handle)
    {
        ToStrF("%u", m_Handle->mFieldCount);
    }
    // Default to an empty string
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::GetInt8(Uint32 idx) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Int8 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return ConvTo< Int8 >::From(*reinterpret_cast< Int8 ** >(m_Handle->mRow)[idx]);
}

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::GetUint8(Uint32 idx) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Uint8 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return ConvTo< Uint8 >::From(*reinterpret_cast< Uint8 ** >(m_Handle->mRow)[idx]);
}

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::GetInt16(Uint32 idx) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Int16 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return ConvTo< Int16 >::From(*reinterpret_cast< Int16 ** >(m_Handle->mRow)[idx]);
}

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::GetUint16(Uint32 idx) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Uint16 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return ConvTo< Uint16 >::From(*reinterpret_cast< Uint16 ** >(m_Handle->mRow)[idx]);
}

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::GetInt32(Uint32 idx) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Int32 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return ConvTo< Int32 >::From(*reinterpret_cast< Int32 ** >(m_Handle->mRow)[idx]);
}

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::GetUint32(Uint32 idx) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Uint32 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return ConvTo< Uint32 >::From(*reinterpret_cast< Uint32 ** >(m_Handle->mRow)[idx]);
}

// ------------------------------------------------------------------------------------------------
Int64 ResultSet::GetInt64(Uint32 idx) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Int64 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return ConvTo< Int64 >::From(*reinterpret_cast< Int64 ** >(m_Handle->mRow)[idx]);
}

// ------------------------------------------------------------------------------------------------
Uint64 ResultSet::GetUint64(Uint32 idx) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Uint64 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return ConvTo< Uint64 >::From(*reinterpret_cast< Uint64 ** >(m_Handle->mRow)[idx]);
}

// ------------------------------------------------------------------------------------------------
SQFloat ResultSet::GetFloat32(Uint32 idx) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Float32 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return ConvTo< Float32 >::From(*reinterpret_cast< Float32 ** >(m_Handle->mRow)[idx]);
}

// ------------------------------------------------------------------------------------------------
SQFloat ResultSet::GetFloat64(Uint32 idx) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< Float64 >::From(m_Handle->mBinds[idx].mInt64);
    }
    // Retrieve the value directly from the row
    return ConvTo< Float64 >::From(*reinterpret_cast< Float64 ** >(m_Handle->mRow)[idx]);
}

// ------------------------------------------------------------------------------------------------
bool ResultSet::GetBoolean(Uint32 idx) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< bool >::From(m_Handle->mBinds[idx].mUint64);
    }
    // Retrieve the value directly from the row
    return ConvTo< bool >::From(*reinterpret_cast< Uint8 ** >(m_Handle->mRow)[idx]);
}


} // Namespace:: SqMod
