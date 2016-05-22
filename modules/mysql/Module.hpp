#ifndef _SQMYSQL_MODULE_HPP_
#define _SQMYSQL_MODULE_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqMod.h"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Proxies to comunicate with the server.
*/
extern PluginFuncs*         _Func;
extern PluginCallbacks*     _Clbk;
extern PluginInfo*          _Info;

/* ------------------------------------------------------------------------------------------------
 * Proxies to comunicate with the Squirrel plugin.
*/
extern HSQAPI               _SqAPI;
extern HSQEXPORTS           _SqMod;
extern HSQUIRRELVM          _SqVM;

/* ------------------------------------------------------------------------------------------------
 * Output a message only if the _DEBUG was defined.
*/
void OutputDebug(const char * msg, ...);

/* ------------------------------------------------------------------------------------------------
 * Output a formatted user message to the console.
*/
void OutputMessage(const char * msg, ...);

/* ------------------------------------------------------------------------------------------------
 * Output a formatted error message to the console.
*/
void OutputError(const char * msg, ...);

} // Namespace:: SqMod

#endif // _SQMYSQL_MODULE_HPP_
