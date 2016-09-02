//
// SqModule: API used to communicate with and register squirrel modules
//

//
// Copyright (c) 2016 Sandu Liviu Catalin (aka. S.L.C)
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source
//  distribution.
//

#if !defined(_SQ_MOD_H_)
#define _SQ_MOD_H_

#include <stddef.h>

#include "SqAPI.h"
#include "vcmp.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**< 64 bits integer types */
    #if defined(_MSC_VER)
        typedef __int64                 SqInt64;
        typedef unsigned __int64        SqUint64;
    #else
        typedef long long               SqInt64;
        typedef unsigned long long      SqUint64;
    #endif

    #if defined(_MSC_VER)
        #define SQMOD_API_EXPORT    extern "C" __declspec(dllexport)
    #elif defined(__GNUC__)
        #define SQMOD_API_EXPORT    extern "C"
    #else
        #define SQMOD_API_EXPORT    extern "C"
    #endif

    #define SQMOD_HOST_NAME         "SqModHost"
    #define SQMOD_INITIALIZE_CMD    0xDABBAD00
    #define SQMOD_LOAD_CMD          0xDEADBABE
    #define SQMOD_TERMINATE_CMD     0xDEADC0DE
    #define SQMOD_CLOSING_CMD       0xBAAAAAAD
    #define SQMOD_RELEASED_CMD      0xDEADBEAF
    #define SQMOD_API_VER           1

    //primitive functions
    typedef HSQUIRRELVM     (*SqModAPI_GetSquirrelVM) (void);
    //logging utilities
    typedef void            (*SqModAPI_LogMessage) (const SQChar * fmt, ...);
    //script loading
    typedef SQRESULT        (*SqModAPI_LoadScript) (const SQChar * filepath, SQBool delay);
    //numeric utilities
    typedef SQRESULT        (*SqModAPI_GetSLongValue) (HSQUIRRELVM vm, SQInteger idx, SqInt64 * num);
    typedef SQRESULT        (*SqModAPI_PushSLongObject) (HSQUIRRELVM vm, SqInt64 num);
    typedef SQRESULT        (*SqModAPI_GetULongValue) (HSQUIRRELVM vm, SQInteger idx, SqUint64 * num);
    typedef SQRESULT        (*SqModAPI_PushULongObject) (HSQUIRRELVM vm, SqUint64 num);
    //time utilities
    typedef SqInt64         (*SqModAPI_GetCurrentSysTime) (void);
    typedef SqInt64         (*SqModAPI_GetEpochTimeMicro) (void);
    typedef SqInt64         (*SqModAPI_GetEpochTimeMilli) (void);
    typedef SQBool          (*SqModAPI_ValidDate) (uint16_t year, uint8_t month, uint8_t day);
    typedef SQBool          (*SqModAPI_IsLeapYear) (uint16_t year);
    typedef uint16_t        (*SqModAPI_DaysInYear) (uint16_t year);
    typedef uint8_t         (*SqModAPI_DaysInMonth) (uint16_t year, uint8_t month);
    typedef uint16_t        (*SqModAPI_DayOfYear) (uint16_t year, uint8_t month, uint8_t day);
    typedef SqInt64         (*SqModAPI_DateRangeToSeconds) (uint16_t lyear, uint8_t lmonth, uint8_t lday, uint16_t ryear, uint8_t rmonth, uint8_t rday);
    typedef SQRESULT        (*SqModAPI_GetTimestamp) (HSQUIRRELVM vm, SQInteger idx, SqInt64 * num);
    typedef SQRESULT        (*SqModAPI_PushTimestamp) (HSQUIRRELVM vm, SqInt64 num);
    typedef SQRESULT        (*SqModAPI_GetDate) (HSQUIRRELVM vm, SQInteger idx, uint16_t * year, uint8_t * month, uint8_t * day);
    typedef SQRESULT        (*SqModAPI_PushDate) (HSQUIRRELVM vm, uint16_t year, uint8_t month, uint8_t day);
    typedef SQRESULT        (*SqModAPI_GetTime) (HSQUIRRELVM vm, SQInteger idx, uint8_t * hour, uint8_t * minute, uint8_t * second, uint16_t * millisecond);
    typedef SQRESULT        (*SqModAPI_PushTime) (HSQUIRRELVM vm, uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond);
    typedef SQRESULT        (*SqModAPI_GetDatetime) (HSQUIRRELVM vm, SQInteger idx, uint16_t * year, uint8_t * month, uint8_t * day, uint8_t * hour, uint8_t * minute, uint8_t * second, uint16_t * millisecond);
    typedef SQRESULT        (*SqModAPI_PushDatetime) (HSQUIRRELVM vm, uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond);
    //stack utilities
    typedef SQInteger       (*SqModAPI_PopStackInteger) (HSQUIRRELVM vm, SQInteger idx);
    typedef SQFloat         (*SqModAPI_PopStackFloat) (HSQUIRRELVM vm, SQInteger idx);
    typedef SqInt64         (*SqModAPI_PopStackSLong) (HSQUIRRELVM vm, SQInteger idx);
    typedef SqUint64        (*SqModAPI_PopStackULong) (HSQUIRRELVM vm, SQInteger idx);
    //buffer utilities
    typedef SQRESULT        (*SqModAPI_PushBuffer) (HSQUIRRELVM vm, SQInteger size, SQInteger cursor);
    typedef SQRESULT        (*SqModAPI_PushBufferData) (HSQUIRRELVM vm, const char * data, SQInteger size, SQInteger cursor);
    typedef SQRESULT        (*SqModAPI_GetBufferInfo) (HSQUIRRELVM vm, SQInteger idx, const char ** ptr, SQInteger * size, SQInteger * cursor);
    typedef const char *    (*SqModAPI_GetBufferData) (HSQUIRRELVM vm, SQInteger idx);
    typedef SQInteger       (*SqModAPI_GetBufferSize) (HSQUIRRELVM vm, SQInteger idx);
    typedef SQInteger       (*SqModAPI_GetBufferCursor) (HSQUIRRELVM vm, SQInteger idx);

    /* --------------------------------------------------------------------------------------------
     * Allows modules to interface with the plug-in API without linking of any sorts
    */
    typedef struct
    {
        //primitive functions
        SqModAPI_GetSquirrelVM                      GetSquirrelVM;
        //logging utilities
        SqModAPI_LogMessage                         LogDbg;
        SqModAPI_LogMessage                         LogUsr;
        SqModAPI_LogMessage                         LogScs;
        SqModAPI_LogMessage                         LogInf;
        SqModAPI_LogMessage                         LogWrn;
        SqModAPI_LogMessage                         LogErr;
        SqModAPI_LogMessage                         LogFtl;
        SqModAPI_LogMessage                         LogSDbg;
        SqModAPI_LogMessage                         LogSUsr;
        SqModAPI_LogMessage                         LogSScs;
        SqModAPI_LogMessage                         LogSInf;
        SqModAPI_LogMessage                         LogSWrn;
        SqModAPI_LogMessage                         LogSErr;
        SqModAPI_LogMessage                         LogSFtl;
        //script loading
        SqModAPI_LoadScript                         LoadScript;
        //numeric utilities
        SqModAPI_GetSLongValue                      GetSLongValue;
        SqModAPI_PushSLongObject                    PushSLongObject;
        SqModAPI_GetULongValue                      GetULongValue;
        SqModAPI_PushULongObject                    PushULongObject;
        //time utilities
        SqModAPI_GetCurrentSysTime                  GetCurrentSysTime;
        SqModAPI_GetEpochTimeMicro                  GetEpochTimeMicro;
        SqModAPI_GetEpochTimeMilli                  GetEpochTimeMilli;
        SqModAPI_ValidDate                          ValidDate;
        SqModAPI_IsLeapYear                         IsLeapYear;
        SqModAPI_DaysInYear                         DaysInYear;
        SqModAPI_DaysInMonth                        DaysInMonth;
        SqModAPI_DayOfYear                          DayOfYear;
        SqModAPI_DateRangeToSeconds                 DateRangeToSeconds;
        SqModAPI_GetTimestamp                       GetTimestamp;
        SqModAPI_PushTimestamp                      PushTimestamp;
        SqModAPI_GetDate                            GetDate;
        SqModAPI_PushDate                           PushDate;
        SqModAPI_GetTime                            GetTime;
        SqModAPI_PushTime                           PushTime;
        SqModAPI_GetDatetime                        GetDatetime;
        SqModAPI_PushDatetime                       PushDatetime;
        //stack utilities
        SqModAPI_PopStackInteger                    PopStackInteger;
        SqModAPI_PopStackFloat                      PopStackFloat;
        SqModAPI_PopStackSLong                      PopStackSLong;
        SqModAPI_PopStackULong                      PopStackULong;
        //buffer utilities
        SqModAPI_PushBuffer                         PushBuffer;
        SqModAPI_PushBufferData                     PushBufferData;
        SqModAPI_GetBufferInfo                      GetBufferInfo;
        SqModAPI_GetBufferData                      GetBufferData;
        SqModAPI_GetBufferSize                      GetBufferSize;
        SqModAPI_GetBufferCursor                    GetBufferCursor;
    } sq_modapi, SQMODAPI, *HSQMODAPI;

#ifdef SQMOD_PLUGIN_API

    //primitive functions
    extern SqModAPI_GetSquirrelVM                   SqMod_GetSquirrelVM;

    //logging utilities
    extern SqModAPI_LogMessage                      SqMod_LogDbg;
    extern SqModAPI_LogMessage                      SqMod_LogUsr;
    extern SqModAPI_LogMessage                      SqMod_LogScs;
    extern SqModAPI_LogMessage                      SqMod_LogInf;
    extern SqModAPI_LogMessage                      SqMod_LogWrn;
    extern SqModAPI_LogMessage                      SqMod_LogErr;
    extern SqModAPI_LogMessage                      SqMod_LogFtl;
    extern SqModAPI_LogMessage                      SqMod_LogSDbg;
    extern SqModAPI_LogMessage                      SqMod_LogSUsr;
    extern SqModAPI_LogMessage                      SqMod_LogSScs;
    extern SqModAPI_LogMessage                      SqMod_LogSInf;
    extern SqModAPI_LogMessage                      SqMod_LogSWrn;
    extern SqModAPI_LogMessage                      SqMod_LogSErr;
    extern SqModAPI_LogMessage                      SqMod_LogSFtl;

    //script loading
    extern SqModAPI_LoadScript                      SqMod_LoadScript;

    //numeric utilities
    extern SqModAPI_GetSLongValue                   SqMod_GetSLongValue;
    extern SqModAPI_PushSLongObject                 SqMod_PushSLongObject;
    extern SqModAPI_GetULongValue                   SqMod_GetULongValue;
    extern SqModAPI_PushULongObject                 SqMod_PushULongObject;

    //time utilities
    extern SqModAPI_GetCurrentSysTime               SqMod_GetCurrentSysTime;
    extern SqModAPI_GetEpochTimeMicro               SqMod_GetEpochTimeMicro;
    extern SqModAPI_GetEpochTimeMilli               SqMod_GetEpochTimeMilli;
    extern SqModAPI_ValidDate                       SqMod_ValidDate;
    extern SqModAPI_IsLeapYear                      SqMod_IsLeapYear;
    extern SqModAPI_DaysInYear                      SqMod_DaysInYear;
    extern SqModAPI_DaysInMonth                     SqMod_DaysInMonth;
    extern SqModAPI_DayOfYear                       SqMod_DayOfYear;
    extern SqModAPI_DateRangeToSeconds              SqMod_DateRangeToSeconds;
    extern SqModAPI_GetTimestamp                    SqMod_GetTimestamp;
    extern SqModAPI_PushTimestamp                   SqMod_PushTimestamp;
    extern SqModAPI_GetDate                         SqMod_GetDate;
    extern SqModAPI_PushDate                        SqMod_PushDate;
    extern SqModAPI_GetTime                         SqMod_GetTime;
    extern SqModAPI_PushTime                        SqMod_PushTime;
    extern SqModAPI_GetDatetime                     SqMod_GetDatetime;
    extern SqModAPI_PushDatetime                    SqMod_PushDatetime;

    //stack utilities
    extern SqModAPI_PopStackInteger                 SqMod_PopStackInteger;
    extern SqModAPI_PopStackFloat                   SqMod_PopStackFloat;
    extern SqModAPI_PopStackSLong                   SqMod_PopStackSLong;
    extern SqModAPI_PopStackULong                   SqMod_PopStackULong;

    //buffer utilities
    extern SqModAPI_PushBuffer                      SqMod_PushBuffer;
    extern SqModAPI_PushBufferData                  SqMod_PushBufferData;
    extern SqModAPI_GetBufferInfo                   SqMod_GetBufferInfo;
    extern SqModAPI_GetBufferData                   SqMod_GetBufferData;
    extern SqModAPI_GetBufferSize                   SqMod_GetBufferSize;
    extern SqModAPI_GetBufferCursor                 SqMod_GetBufferCursor;

#endif // SQMOD_PLUGIN_API

    /* --------------------------------------------------------------------------------------------
     * The structure exported by the host plug-in to import the module and squirrel API.
    */
    typedef struct
    {
        unsigned int StructSize;
        //base functions
        int32_t (*PopulateModuleAPI) (HSQMODAPI api, size_t size);
        int32_t (*PopulateSquirrelAPI) (HSQLIBAPI api, size_t size);
    } sq_modexports, SQMODEXPORTS, *HSQMODEXPORTS;

    /* --------------------------------------------------------------------------------------------
     * Assign the functions from the specified API structure into the global functions.
    */
    SQUIRREL_API uint8_t sqmod_api_expand(HSQMODAPI sqmodapi);

    /* --------------------------------------------------------------------------------------------
     * Assign the functions from the specified API structure into the global functions.
    */
    SQUIRREL_API uint8_t sqlib_api_expand(HSQLIBAPI sqlibapi);

    /* --------------------------------------------------------------------------------------------
     * Undo changes done by sqmod_api_expand.
    */
    SQUIRREL_API void sqmod_api_collapse();

    /* --------------------------------------------------------------------------------------------
     * Undo changes done by sq_api_expand.
    */
    SQUIRREL_API void sqlib_api_collapse();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_SQ_MOD_H_*/
