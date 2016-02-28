// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
#include <stdarg.h>

// ------------------------------------------------------------------------------------------------
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static SQChar g_Buffer[4096]; // Common buffer to reduce memory allocations.

// ------------------------------------------------------------------------------------------------
SStr GetTempBuff()
{
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
Uint32 GetTempBuffSize()
{
    return sizeof(g_Buffer);
}

// ------------------------------------------------------------------------------------------------
void SqThrowF(CSStr str, ...)
{
    // Initialize the argument list
    va_list args;
    va_start (args, str);
    // Write the requested contents
    if (snprintf(g_Buffer, sizeof(g_Buffer), str, args) < 0)
        strcpy(g_Buffer, "Unknown error has occurred");
    // Release the argument list
    va_end(args);
    // Throw the exception with the resulted message
    throw Sqrat::Exception(g_Buffer);
}

// ------------------------------------------------------------------------------------------------
CSStr FmtStr(CSStr str, ...)
{
    // Initialize the argument list
    va_list args;
    va_start (args, str);
    // Write the requested contents
    if (snprintf(g_Buffer, sizeof(g_Buffer), str, args) < 0)
        g_Buffer[0] = 0; /* make sure the string is terminated */
    // Release the argument list
    va_end(args);
    // Return the data from the buffer
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
StackGuard::StackGuard(HSQUIRRELVM vm)
    : m_Top(sq_gettop(vm)), m_VM(vm)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
StackGuard::~StackGuard()
{
    sq_pop(m_VM, sq_gettop(m_VM) - m_Top);
}

// ------------------------------------------------------------------------------------------------
SQInteger IniResult::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqIniResult");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
void IniResult::Check() const
{
    // Identify the error type
    switch (m_Result)
    {
        case SI_FAIL:
            SqThrowF("Unable to %s. Probably invalid", m_Action.c_str());
        break;
        case SI_NOMEM:
            SqThrowF("Unable to %s. Ran out of memory", m_Action.c_str());
        break;
        case SI_FILE:
            SqThrowF("Unable to %s. %s", strerror(errno));
        break;
        case SI_OK:
        case SI_UPDATED:
        case SI_INSERTED:
            break; /* These are not error messahes. */
        default:
            SqThrowF("Unable to %s for some unforeseen reason", m_Action.c_str());
    }
}

} // Namespace:: SqMod
