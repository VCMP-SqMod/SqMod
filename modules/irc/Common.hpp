#ifndef _SQIRC_COMMON_HPP_
#define _SQIRC_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "ModBase.hpp"

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
 * Forward declarations.
*/
class Session;

/* ------------------------------------------------------------------------------------------------
 * Extract the name from the specified origin.
*/
CSStr GetNick(CSStr origin);

/* ------------------------------------------------------------------------------------------------
 * Extract the host from the specified origin.
*/
CSStr GetHost(CSStr target);

} // Namespace:: SqMod

#endif // _SQIRC_COMMON_HPP_
