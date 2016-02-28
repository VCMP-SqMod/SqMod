// ------------------------------------------------------------------------------------------------
#include "Document.hpp"
#include "Node.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Document::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqXmlDocument");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
void Document::Validate() const
{
    // Validate the document handle
    if (!m_Doc)
        SqThrowF("Invalid XML document reference");
}

// ------------------------------------------------------------------------------------------------
void Document::CanLoad() const
{
    // Is the document even valid?
    if (!m_Doc)
        SqThrowF("Invalid XML document reference");
    // Are there any other references?
    else if (m_Doc.Count() > 1)
        // To load new values now, would mean to cause undefined behavior in existing references
        SqThrowF("Loading is disabled while document is referenced");
}

// ------------------------------------------------------------------------------------------------
Node Document::GetNode() const
{
    if (m_Doc)
        return Node(m_Doc, m_Doc->document_element());
    return Node();
}

} // Namespace:: SqMod