// ------------------------------------------------------------------------------------------------
#include "Document.hpp"
#include "Node.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool Document::Validate() const
{
    if (m_Doc)
        return true;
    // Invalid document reference
    _SqMod->SqThrow("Invalid XML document reference");
    return false;
}

// ------------------------------------------------------------------------------------------------
bool Document::CanLoad() const
{
    // Is the document even valid?
    if (!Validate())
        return false;
    // Are there any other references?
    else if (m_Doc.Count() == 1)
        return true;
    // To load new values now, would mean to cause undefined behavior in existing references
    _SqMod->SqThrow("Loading is disabled while document is referenced");
    return false;
}

// ------------------------------------------------------------------------------------------------
Node Document::GetNode() const
{
    if (m_Doc)
        return Node(m_Doc, m_Doc->document_element());
    return Node();
}

} // Namespace:: SqMod