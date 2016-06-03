// ------------------------------------------------------------------------------------------------
#include "Document.hpp"
#include "Node.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Document::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqXmlDocument");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
void Document::CanLoad() const
{
    // Is the document even valid?
    m_Doc.Validate();
    // Are there any other references?
    if (m_Doc.Count() > 1)
    {
        // To load new values now, would mean to cause undefined behavior in existing references
        STHROWF("Loading is disabled while document is referenced");
    }
}

// ------------------------------------------------------------------------------------------------
Node Document::GetNode() const
{
    // Validate the document handle
    m_Doc.Validate();
    // Return the requested information
    return Node(m_Doc, m_Doc->document_element());
}

} // Namespace:: SqMod