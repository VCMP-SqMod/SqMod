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

}

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::GetInt16(Uint32 idx) const
{

}

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::GetUint16(Uint32 idx) const
{

}

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::GetInt32(Uint32 idx) const
{

}

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::GetUint32(Uint32 idx) const
{

}

// ------------------------------------------------------------------------------------------------
Object ResultSet::GetInt64(Uint32 idx) const
{

}

// ------------------------------------------------------------------------------------------------
Object ResultSet::GetUint64(Uint32 idx) const
{

}

// ------------------------------------------------------------------------------------------------
SQFloat ResultSet::GetFloat32(Uint32 idx) const
{

}

// ------------------------------------------------------------------------------------------------
SQFloat ResultSet::GetFloat64(Uint32 idx) const
{

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
