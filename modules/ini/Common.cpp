// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void IniResult::Check() const
{
    // Identify the error type
    switch (m_Result)
    {
        case SI_FAIL:
            _SqMod->SqThrow("Unable to %s. Probably invalid", m_Action.c_str());
        break;
        case SI_NOMEM:
            _SqMod->SqThrow("Unable to %s. Ran out of memory", m_Action.c_str());
        break;
        case SI_FILE:
            _SqMod->SqThrow("Unable to %s. %s", strerror(errno));
        break;
        case SI_BADREF:
            _SqMod->SqThrow("Unable to %s. Invalid document reference", m_Action.c_str());
        break;
        case SI_OK:
        case SI_UPDATED:
        case SI_INSERTED:
            break; /* These are not error messahes. */
        default:
            _SqMod->SqThrow("Unable to %s for some unforeseen reason", m_Action.c_str());
    }
}

} // Namespace:: SqMod
