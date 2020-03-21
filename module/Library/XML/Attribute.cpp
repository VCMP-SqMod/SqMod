// ------------------------------------------------------------------------------------------------
#include "Library/XML/Attribute.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger XmlAttribute::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqXmlAttribute");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
LightObj XmlAttribute::AsLong(const SLongInt & def) const
{
    return LightObj(SqTypeIdentity< SLongInt >{}, SqVM(), m_Attr.as_llong(def.GetNum()));
}

// ------------------------------------------------------------------------------------------------
LightObj XmlAttribute::AsUlong(const ULongInt & def) const
{
    return LightObj(SqTypeIdentity< ULongInt >{}, SqVM(), m_Attr.as_ullong(def.GetNum()));
}

// ------------------------------------------------------------------------------------------------
bool XmlAttribute::ApplyLong(const SLongInt & value)
{
    return m_Attr.set_value(value.GetNum());
}

// ------------------------------------------------------------------------------------------------
bool XmlAttribute::ApplyUlong(const ULongInt & value)
{
    return m_Attr.set_value(value.GetNum());
}

// ------------------------------------------------------------------------------------------------
LightObj XmlAttribute::GetLong() const
{
    return LightObj(SqTypeIdentity< SLongInt >{}, SqVM(), m_Attr.as_llong());
}

// ------------------------------------------------------------------------------------------------
void XmlAttribute::SetLong(const SLongInt & value)
{
    m_Attr = value.GetNum();
}

// ------------------------------------------------------------------------------------------------
LightObj XmlAttribute::GetUlong() const
{
    return LightObj(SqTypeIdentity< ULongInt >{}, SqVM(), m_Attr.as_ullong());
}

// ------------------------------------------------------------------------------------------------
void XmlAttribute::SetUlong(const ULongInt & value)
{
    m_Attr = value.GetNum();
}

} // Namespace:: SqMod
