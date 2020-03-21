// ------------------------------------------------------------------------------------------------
#include "Library/XML/Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void DocumentRef::Validate() const
{
    if (!m_Ptr)
    {
        STHROWF("Invalid XML document reference");
    }
}

// ------------------------------------------------------------------------------------------------
SQInteger XmlParseResult::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqXmlParseResult");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
void XmlParseResult::Validate() const
{
    // Is the documen handle valid?
    if (!m_Doc)
    {
        STHROWF("Invalid XML document reference");
    }
}

// ------------------------------------------------------------------------------------------------
void XmlParseResult::Check() const
{
    if (m_Result.status != status_ok)
    {
        STHROWF("XML parse error [%s]", m_Result.description());
    }
}

} // Namespace:: SqMod
