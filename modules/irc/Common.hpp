#ifndef _SQIRC_COMMON_HPP_
#define _SQIRC_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <libircclient.h>
#include <libirc_rfcnumeric.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * SOFTWARE INFORMATION
*/
#define SQIRC_NAME "Squirrel IRC Module"
#define SQIRC_AUTHOR "Sandu Liviu Catalin (S.L.C)"
#define SQIRC_COPYRIGHT "Copyright (C) 2016 Sandu Liviu Catalin"
#define SQIRC_HOST_NAME "SqModIRCHost"
#define SQIRC_VERSION 001
#define SQIRC_VERSION_STR "0.0.1"
#define SQIRC_VERSION_MAJOR 0
#define SQIRC_VERSION_MINOR 0
#define SQIRC_VERSION_PATCH 1

/* ------------------------------------------------------------------------------------------------
 * Types of events that the session emits.
*/
enum SessionEvent
{
    SET_CONNECT = 0,
    SET_NICK,
    SET_QUIT,
    SET_JOIN,
    SET_PART,
    SET_MODE,
    SET_UMODE,
    SET_TOPIC,
    SET_KICK,
    SET_CHANNEL,
    SET_PRIVMSG,
    SET_NOTICE,
    SET_CHANNELNOTICE,
    SET_INVITE,
    SET_CTCPREQ,
    SET_CTCPREP,
    SET_CTCPACTION,
    SET_UNKNOWN,
    SET_NUMERIC,
    SET_DCCCHATREQ,
    SET_DCCSENDREQ,
    SET_MAX
};

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
class Session;

/* ------------------------------------------------------------------------------------------------
 * Extract the name from the specified origin.
*/
SQInteger GetNick(HSQUIRRELVM vm);

/* ------------------------------------------------------------------------------------------------
 * Extract the host from the specified origin.
*/
SQInteger GetHost(HSQUIRRELVM vm);

/* ------------------------------------------------------------------------------------------------
 * Returns a new plain text message with stripped mIRC color codes.
*/
SQInteger StripColorFromMIRC(HSQUIRRELVM vm);

/* ------------------------------------------------------------------------------------------------
 * Returns a new message with converted mIRC color codes and format options.
*/
SQInteger ConvertColorFromMIRC(HSQUIRRELVM vm);

/* ------------------------------------------------------------------------------------------------
 * Returns a new message with converted color codes and format options.
*/
SQInteger ConvertColorToMIRC(HSQUIRRELVM vm);

} // Namespace:: SqMod

#endif // _SQIRC_COMMON_HPP_
