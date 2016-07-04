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
    #define SQMOD_RELEASED_CMD      0xDEADBEAF
    #define SQMOD_API_VER           1

    //primitive functions
    typedef HSQAPI          (*SqEx_GetSquirrelAPI) (void);
    typedef HSQUIRRELVM     (*SqEx_GetSquirrelVM) (void);
    //logging utilities
    typedef void            (*SqEx_LogMessage) (const SQChar * fmt, ...);
    //script loading
    typedef SQRESULT        (*SqEx_LoadScript) (const SQChar * filepath, SQBool delay);
    //numeric utilities
    typedef SQRESULT        (*SqEx_GetSLongValue) (HSQUIRRELVM vm, SQInteger idx, SqInt64 * num);
    typedef SQRESULT        (*SqEx_PushSLongObject) (HSQUIRRELVM vm, SqInt64 num);
    typedef SQRESULT        (*SqEx_GetULongValue) (HSQUIRRELVM vm, SQInteger idx, SqUint64 * num);
    typedef SQRESULT        (*SqEx_PushULongObject) (HSQUIRRELVM vm, SqUint64 num);
    //time utilities
    typedef SqInt64         (*SqEx_GetCurrentSysTime) (void);
    typedef SqInt64         (*SqEx_GetEpochTimeMicro) (void);
    typedef SqInt64         (*SqEx_GetEpochTimeMilli) (void);
    typedef SQBool          (*SqEx_ValidDate) (uint16_t year, uint8_t month, uint8_t day);
    typedef SQBool          (*SqEx_IsLeapYear) (uint16_t year);
    typedef uint16_t        (*SqEx_DaysInYear) (uint16_t year);
    typedef uint8_t         (*SqEx_DaysInMonth) (uint16_t year, uint8_t month);
    typedef uint16_t        (*SqEx_DayOfYear) (uint16_t year, uint8_t month, uint8_t day);
    typedef SqInt64         (*SqEx_DateRangeToSeconds) (uint16_t lyear, uint8_t lmonth, uint8_t lday, uint16_t ryear, uint8_t rmonth, uint8_t rday);
    typedef SQRESULT        (*SqEx_GetTimestamp) (HSQUIRRELVM vm, SQInteger idx, SqInt64 * num);
    typedef SQRESULT        (*SqEx_PushTimestamp) (HSQUIRRELVM vm, SqInt64 num);
    typedef SQRESULT        (*SqEx_GetDate) (HSQUIRRELVM vm, SQInteger idx, uint16_t * year, uint8_t * month, uint8_t * day);
    typedef SQRESULT        (*SqEx_PushDate) (HSQUIRRELVM vm, uint16_t year, uint8_t month, uint8_t day);
    typedef SQRESULT        (*SqEx_GetTime) (HSQUIRRELVM vm, SQInteger idx, uint8_t * hour, uint8_t * minute, uint8_t * second, uint16_t * millisecond);
    typedef SQRESULT        (*SqEx_PushTime) (HSQUIRRELVM vm, uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond);
    typedef SQRESULT        (*SqEx_GetDatetime) (HSQUIRRELVM vm, SQInteger idx, uint16_t * year, uint8_t * month, uint8_t * day, uint8_t * hour, uint8_t * minute, uint8_t * second, uint16_t * millisecond);
    typedef SQRESULT        (*SqEx_PushDatetime) (HSQUIRRELVM vm, uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond);
    //stack utilities
    typedef SQInteger       (*SqEx_PopStackInteger) (HSQUIRRELVM vm, SQInteger idx);
    typedef SQFloat         (*SqEx_PopStackFloat) (HSQUIRRELVM vm, SQInteger idx);
    typedef SqInt64         (*SqEx_PopStackSLong) (HSQUIRRELVM vm, SQInteger idx);
    typedef SqUint64        (*SqEx_PopStackULong) (HSQUIRRELVM vm, SQInteger idx);
    //buffer utilities
    typedef SQRESULT        (*SqEx_PushBuffer) (HSQUIRRELVM vm, SQInteger size, SQInteger cursor);
    typedef SQRESULT        (*SqEx_PushBufferData) (HSQUIRRELVM vm, const char * data, SQInteger size, SQInteger cursor);

    /* --------------------------------------------------------------------------------------------
     * Allows modules to interface with the plug-in API without linking of any sorts
    */
    typedef struct
    {
        unsigned int                        StructSize;
        //primitive functions
        SqEx_GetSquirrelAPI                 GetSquirrelAPI;
        SqEx_GetSquirrelVM                  GetSquirrelVM;
        //logging utilities
        SqEx_LogMessage                     LogDbg;
        SqEx_LogMessage                     LogUsr;
        SqEx_LogMessage                     LogScs;
        SqEx_LogMessage                     LogInf;
        SqEx_LogMessage                     LogWrn;
        SqEx_LogMessage                     LogErr;
        SqEx_LogMessage                     LogFtl;
        SqEx_LogMessage                     LogSDbg;
        SqEx_LogMessage                     LogSUsr;
        SqEx_LogMessage                     LogSScs;
        SqEx_LogMessage                     LogSInf;
        SqEx_LogMessage                     LogSWrn;
        SqEx_LogMessage                     LogSErr;
        SqEx_LogMessage                     LogSFtl;
        //script loading
        SqEx_LoadScript                     LoadScript;
        //numeric utilities
        SqEx_GetSLongValue                  GetSLongValue;
        SqEx_PushSLongObject                PushSLongObject;
        SqEx_GetULongValue                  GetULongValue;
        SqEx_PushULongObject                PushULongObject;
        //time utilities
        SqEx_GetCurrentSysTime              GetCurrentSysTime;
        SqEx_GetEpochTimeMicro              GetEpochTimeMicro;
        SqEx_GetEpochTimeMilli              GetEpochTimeMilli;
        SqEx_ValidDate                      ValidDate;
        SqEx_IsLeapYear                     IsLeapYear;
        SqEx_DaysInYear                     DaysInYear;
        SqEx_DaysInMonth                    DaysInMonth;
        SqEx_DayOfYear                      DayOfYear;
        SqEx_DateRangeToSeconds             DateRangeToSeconds;
        SqEx_GetTimestamp                   GetTimestamp;
        SqEx_PushTimestamp                  PushTimestamp;
        SqEx_GetDate                        GetDate;
        SqEx_PushDate                       PushDate;
        SqEx_GetTime                        GetTime;
        SqEx_PushTime                       PushTime;
        SqEx_GetDatetime                    GetDatetime;
        SqEx_PushDatetime                   PushDatetime;
        //stack utilities
        SqEx_PopStackInteger                PopStackInteger;
        SqEx_PopStackFloat                  PopStackFloat;
        SqEx_PopStackSLong                  PopStackSLong;
        SqEx_PopStackULong                  PopStackULong;
        //buffer utilities
        SqEx_PushBuffer                     PushBuffer;
        SqEx_PushBufferData                 PushBufferData;
    } sq_exports, SQEXPORTS, *HSQEXPORTS;

#ifdef SQMOD_PLUGIN_API

    //primitive functions
    extern SqEx_GetSquirrelAPI                  SqMod_GetSquirrelAPI;
    extern SqEx_GetSquirrelVM                   SqMod_GetSquirrelVM;

    //logging utilities
    extern SqEx_LogMessage                      SqMod_LogDbg;
    extern SqEx_LogMessage                      SqMod_LogUsr;
    extern SqEx_LogMessage                      SqMod_LogScs;
    extern SqEx_LogMessage                      SqMod_LogInf;
    extern SqEx_LogMessage                      SqMod_LogWrn;
    extern SqEx_LogMessage                      SqMod_LogErr;
    extern SqEx_LogMessage                      SqMod_LogFtl;
    extern SqEx_LogMessage                      SqMod_LogSDbg;
    extern SqEx_LogMessage                      SqMod_LogSUsr;
    extern SqEx_LogMessage                      SqMod_LogSScs;
    extern SqEx_LogMessage                      SqMod_LogSInf;
    extern SqEx_LogMessage                      SqMod_LogSWrn;
    extern SqEx_LogMessage                      SqMod_LogSErr;
    extern SqEx_LogMessage                      SqMod_LogSFtl;

    //script loading
    extern SqEx_LoadScript                      SqMod_LoadScript;

    //numeric utilities
    extern SqEx_GetSLongValue                   SqMod_GetSLongValue;
    extern SqEx_PushSLongObject                 SqMod_PushSLongObject;
    extern SqEx_GetULongValue                   SqMod_GetULongValue;
    extern SqEx_PushULongObject                 SqMod_PushULongObject;

    //time utilities
    extern SqEx_GetCurrentSysTime               SqMod_GetCurrentSysTime;
    extern SqEx_GetEpochTimeMicro               SqMod_GetEpochTimeMicro;
    extern SqEx_GetEpochTimeMilli               SqMod_GetEpochTimeMilli;
    extern SqEx_ValidDate                       SqMod_ValidDate;
    extern SqEx_IsLeapYear                      SqMod_IsLeapYear;
    extern SqEx_DaysInYear                      SqMod_DaysInYear;
    extern SqEx_DaysInMonth                     SqMod_DaysInMonth;
    extern SqEx_DayOfYear                       SqMod_DayOfYear;
    extern SqEx_DateRangeToSeconds              SqMod_DateRangeToSeconds;
    extern SqEx_GetTimestamp                    SqMod_GetTimestamp;
    extern SqEx_PushTimestamp                   SqMod_PushTimestamp;
    extern SqEx_GetDate                         SqMod_GetDate;
    extern SqEx_PushDate                        SqMod_PushDate;
    extern SqEx_GetTime                         SqMod_GetTime;
    extern SqEx_PushTime                        SqMod_PushTime;
    extern SqEx_GetDatetime                     SqMod_GetDatetime;
    extern SqEx_PushDatetime                    SqMod_PushDatetime;

    //stack utilities
    extern SqEx_PopStackInteger                 SqMod_PopStackInteger;
    extern SqEx_PopStackFloat                   SqMod_PopStackFloat;
    extern SqEx_PopStackSLong                   SqMod_PopStackSLong;
    extern SqEx_PopStackULong                   SqMod_PopStackULong;

    //buffer utilities
    extern SqEx_PushBuffer                      SqMod_PushBuffer;
    extern SqEx_PushBufferData                  SqMod_PushBufferData;

#endif // SQMOD_PLUGIN_API

    /* --------------------------------------------------------------------------------------------
     * Import the functions from the main squirrel plug-in.
    */
    SQUIRREL_API HSQEXPORTS sq_api_import(PluginFuncs * vcapi);

    /* --------------------------------------------------------------------------------------------
     * Assign the functions from the specified API structure into the global functions.
    */
    SQUIRREL_API SQRESULT sq_api_expand(HSQAPI sqapi);

    /* --------------------------------------------------------------------------------------------
     * Assign the functions from the specified API structure into the global functions.
    */
    SQUIRREL_API SQRESULT sqmod_api_expand(HSQEXPORTS sqmodapi);

    /* --------------------------------------------------------------------------------------------
     * Undo changes done by sq_api_expand.
    */
    SQUIRREL_API void sq_api_collapse();

    /* --------------------------------------------------------------------------------------------
     * Undo changes done by sqmod_api_expand.
    */
    SQUIRREL_API void sqmod_api_collapse();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_SQ_API_H_*/
