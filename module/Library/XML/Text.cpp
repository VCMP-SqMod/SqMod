// ------------------------------------------------------------------------------------------------
#include "Library/XML/Text.hpp"
#include "Library/XML/Node.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger XmlText::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqXmlText");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Int32 XmlText::Cmp(const XmlText & o)
{
    if (strcmp(m_Text.get(), o.m_Text.get()) == 0)
    {
        return 0;
    }
    else if (strlen(m_Text.get()) > strlen(o.m_Text.get()))
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
LightObj XmlText::AsLong(const SLongInt & def) const
{
    return LightObj(SqTypeIdentity< SLongInt >{}, SqVM(), m_Text.as_llong(def.GetNum()));
}

// ------------------------------------------------------------------------------------------------
LightObj XmlText::AsUlong(const ULongInt & def) const
{
    return LightObj(SqTypeIdentity< ULongInt >{}, SqVM(), m_Text.as_ullong(def.GetNum()));
}

// ------------------------------------------------------------------------------------------------
bool XmlText::ApplyLong(const SLongInt & value)
{
    return m_Text.set(value.GetNum());
}

// ------------------------------------------------------------------------------------------------
bool XmlText::ApplyUlong(const ULongInt & value)
{
    return m_Text.set(value.GetNum());
}

// ------------------------------------------------------------------------------------------------
LightObj XmlText::GetLong() const
{
    return LightObj(SqTypeIdentity< SLongInt >{}, SqVM(), m_Text.as_llong());
}

// ------------------------------------------------------------------------------------------------
void XmlText::SetLong(const SLongInt & value)
{
    m_Text = value.GetNum();
}

// ------------------------------------------------------------------------------------------------
LightObj XmlText::GetUlong() const
{
    return LightObj(SqTypeIdentity< SLongInt >{}, SqVM(), m_Text.as_ullong());
}

// ------------------------------------------------------------------------------------------------
void XmlText::SetUlong(const ULongInt & value)
{
    m_Text = value.GetNum();
}

// ------------------------------------------------------------------------------------------------
XmlNode XmlText::GetData() const
{
    return XmlNode(m_Doc, m_Text.data());
}

} // Namespace:: SqMod
