#ifndef _SQMOD_MODULE_HPP_
#define _SQMOD_MODULE_HPP_

// ------------------------------------------------------------------------------------------------
#include "sq_api.h"
#include "vcmp.h"

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
extern HSQUIRRELVM          _SqVm;

/* ------------------------------------------------------------------------------------------------
 * Output a message only if the _DEBUG was defined.
*/
void OutputDebug(const char * msg, ...);

/* ------------------------------------------------------------------------------------------------
 * Output a formatted message to the console.
*/
void OutputMessage(const char * msg, ...);

#endif // _SQMOD_MODULE_HPP_
