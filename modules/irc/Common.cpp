// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
#include <stdarg.h>

// ------------------------------------------------------------------------------------------------
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
#include <libircclient.h>
#include <libirc_rfcnumeric.h>

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
CSStr GetNick(CSStr origin)
{
    // Attempt to retrieve the nickname
    irc_target_get_nick(origin, g_Buffer, sizeof(g_Buffer));
    // Return the nickname that could be retrieved
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
CSStr GetHost(CSStr target)
{
    // Attempt to retrieve the host
    irc_target_get_host(target, g_Buffer, sizeof(g_Buffer));
    // Return the host that could be retrieved
    return g_Buffer;
}

} // Namespace:: SqMod
