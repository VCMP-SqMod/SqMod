// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool ParseResult::Validate() const
{
    if (m_Doc)
        return true;
    // Invalid document reference
    _SqMod->SqThrow("Invalid XML document reference");
    return false;
}

// ------------------------------------------------------------------------------------------------
void ParseResult::Check() const
{
    if (m_Result.status != status_ok)
        _SqMod->SqThrow("XML parse error [%s]", m_Result.description());
}

} // Namespace:: SqMod
