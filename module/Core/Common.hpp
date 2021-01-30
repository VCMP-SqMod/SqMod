#pragma once

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstddef>
#include <climits>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <cfloat>
#include <cmath>
#include <cinttypes>

// ------------------------------------------------------------------------------------------------
#include <new>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <typeinfo>
#include <typeindex>
#include <exception>
#include <stdexcept>
#include <functional>
#include <type_traits>

// ------------------------------------------------------------------------------------------------
#include <vcmp.h>
#include <squirrelex.h>
#include <sqratAllocator.h>
#include <sqratArray.h>
#include <sqratClass.h>
#include <sqratClassType.h>
#include <sqratFunction.h>
#include <sqratLightObj.h>
#include <sqratObject.h>
#include <sqratTable.h>
#include <sqratUtil.h>
#include <fmt/core.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Proxies to communicate with the server.
*/
extern PluginFuncs *        _Func; //NOLINT(bugprone-reserved-identifier)
extern PluginCallbacks *    _Clbk; //NOLINT(bugprone-reserved-identifier)
extern PluginInfo *         _Info; //NOLINT(bugprone-reserved-identifier)

/* ------------------------------------------------------------------------------------------------
 * Helper to register pure typename functions for better performance.
*/
#define SQMOD_DECL_TYPENAME(t, s) /*
*/ namespace { /*
*/  struct t { /*
*/   static const SQChar Str[]; /*
*/   static SQInteger Fn(HSQUIRRELVM vm); /*
*/  }; /*
*/  const SQChar t::Str[] = s; /*
*/  SQInteger t::Fn(HSQUIRRELVM vm) { /*
*/   sq_pushstring(vm, Str, sizeof(Str) / sizeof(SQChar)); /*
*/   return 1; /*
*/  } /*
*/ } /*
*/

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Primary logging functions.
*/
extern void LogDbg(const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogUsr(const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogScs(const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogInf(const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogWrn(const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogErr(const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogFtl(const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Secondary logging functions.
*/
extern void LogSDbg(const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogSUsr(const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogSScs(const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogSInf(const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogSWrn(const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogSErr(const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogSFtl(const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Primary logging functions.
*/
extern void LogDbgV(const char * fmt, va_list vlist);
extern void LogUsrV(const char * fmt, va_list vlist);
extern void LogScsV(const char * fmt, va_list vlist);
extern void LogInfV(const char * fmt, va_list vlist);
extern void LogWrnV(const char * fmt, va_list vlist);
extern void LogErrV(const char * fmt, va_list vlist);
extern void LogFtlV(const char * fmt, va_list vlist);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Secondary logging functions.
*/
extern void LogSDbgV(const char * fmt, va_list vlist);
extern void LogSUsrV(const char * fmt, va_list vlist);
extern void LogSScsV(const char * fmt, va_list vlist);
extern void LogSInfV(const char * fmt, va_list vlist);
extern void LogSWrnV(const char * fmt, va_list vlist);
extern void LogSErrV(const char * fmt, va_list vlist);
extern void LogSFtlV(const char * fmt, va_list vlist);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Primary conditional logging functions.
*/
extern bool cLogDbg(bool exp, const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogUsr(bool exp, const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogScs(bool exp, const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogInf(bool exp, const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogWrn(bool exp, const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogErr(bool exp, const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogFtl(bool exp, const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Secondary conditional logging functions.
*/
extern bool cLogSDbg(bool exp, const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogSUsr(bool exp, const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogSScs(bool exp, const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogSInf(bool exp, const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogSWrn(bool exp, const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogSErr(bool exp, const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogSFtl(bool exp, const char * fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);

/* ------------------------------------------------------------------------------------------------
 * Helper used to reference and keep track of signal instances.
*/
typedef std::pair< Signal *, LightObj > SignalPair;

/* ------------------------------------------------------------------------------------------------
 * Initialize a signal instance into the specified pair.
*/
extern void InitSignalPair(SignalPair & sp, LightObj & et, const char * name);

/* ------------------------------------------------------------------------------------------------
 * Reset/release the specified signal pair.
*/
extern void ResetSignalPair(SignalPair & sp, bool clear = true);

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

/* ------------------------------------------------------------------------------------------------
 * Generate a formatted string and throw it as a sqrat exception.
*/
template < class... Args > void SqThrowF(Args &&... args)
{
    throw Sqrat::Exception(fmt::format(std::forward< Args >(args)...));
}

/* ------------------------------------------------------------------------------------------------
 * Generate a formatted string and throw it as a squirrel exception.
*/
template < class... Args > SQRESULT SqThrowErrorF(HSQUIRRELVM vm, Args &&... args)
{
    String msg;
    try
    {
    	msg = fmt::format(std::forward< Args >(args)...);
    }
    catch(const std::exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
	return sq_throwerror(vm, msg.c_str());
}


/* ------------------------------------------------------------------------------------------------
 * Throw the last system error as an exception.
*/
void SqThrowLastF(const SQChar * msg, ...);

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null script object.
*/
SQMOD_NODISCARD Object & NullObject();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null script object.
*/
SQMOD_NODISCARD LightObj & NullLightObj();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null/empty script table.
*/
SQMOD_NODISCARD Table & NullTable();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null/empty script array.
*/
SQMOD_NODISCARD Array & NullArray();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null script function.
*/
SQMOD_NODISCARD Function & NullFunction();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null string.
*/
SQMOD_NODISCARD String & NullString();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the string representation of a certain type.
*/
SQMOD_NODISCARD const SQChar * SqTypeName(SQObjectType type);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the string representation of a certain type from a value on the stack.
*/
SQMOD_NODISCARD String SqTypeName(HSQUIRRELVM vm, SQInteger idx);

/* ------------------------------------------------------------------------------------------------
 * Create a script string instance from a buffer.
*/
SQMOD_NODISCARD LightObj BufferToStrObj(const Buffer & b);

/* ------------------------------------------------------------------------------------------------
 * Create a script string instance from a portion of a buffer.
*/
SQMOD_NODISCARD LightObj BufferToStrObj(const Buffer & b, uint32_t size);

/* ------------------------------------------------------------------------------------------------
 * Attempt to pop the value at the specified index on the stack as a native integer.
*/
SQMOD_NODISCARD SQInteger PopStackInteger(HSQUIRRELVM vm, SQInteger idx);

/* ------------------------------------------------------------------------------------------------
 * Attempt to pop the value at the specified index on the stack as a native float.
*/
SQMOD_NODISCARD SQFloat PopStackFloat(HSQUIRRELVM vm, SQInteger idx);

/* ------------------------------------------------------------------------------------------------
 * Simple function to check whether the specified string can be considered as a boolean value
*/
SQMOD_NODISCARD bool SToB(const SQChar * str);

} // Namespace:: SqMod
