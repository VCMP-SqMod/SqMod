// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
#include <libircclient.h>
#include <libirc_rfcnumeric.h>

// ------------------------------------------------------------------------------------------------
static SQChar g_Buffer[512];

// ------------------------------------------------------------------------------------------------
namespace SqMod {

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
