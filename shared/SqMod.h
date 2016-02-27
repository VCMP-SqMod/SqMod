//
// SqModule: API used to communicate with and register squirrel modules
//

//
// Copyright (c) 2009 Sandu Liviu Catalin (aka. S.L.C)
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
    #define SQMOD_API_VER           1

    /*primitive functions*/
    typedef HSQAPI          (*SqEx_GetSquirrelAPI) (void);
    typedef HSQUIRRELVM     (*SqEx_GetSquirrelVM) (void);
    /*logging utilities*/
    typedef void            (*SqEx_LogMessage) (const SQChar * fmt, ...);
    typedef void            (*SqEx_SqThrow) (const SQChar * fmt, ...);
    /*long numbers*/
    typedef SQRESULT        (*SqEx_GetSLongValue) (HSQUIRRELVM vm, SQInteger idx, SqInt64 * num);
    typedef void            (*SqEx_PushSLongObject) (HSQUIRRELVM vm, SqInt64 num);
    typedef SQRESULT        (*SqEx_GetULongValue) (HSQUIRRELVM vm, SQInteger idx, SqUint64 * num);
    typedef void            (*SqEx_PushULongObject) (HSQUIRRELVM vm, SqUint64 num);
    /*time utilities*/
    typedef SqInt64         (*SqEx_GetCurrentSysTime) (void);
    typedef SqInt64         (*SqEx_GetEpochTimeMicro) (void);
    typedef SqInt64         (*SqEx_GetEpochTimeMilli) (void);
    typedef SQRESULT        (*SqEx_GetTimestamp) (HSQUIRRELVM vm, SQInteger idx, SqInt64 * num);
    typedef void            (*SqEx_PushTimestamp) (HSQUIRRELVM vm, SqInt64 num);

    /* --------------------------------------------------------------------------------------------
     * Allows modules to interface with the plugin API without linking of any sorts
    */
    typedef struct
    {
        unsigned int    uStructSize;
        /*primitive functions*/
        SqEx_GetSquirrelAPI                 GetSquirrelAPI;
        SqEx_GetSquirrelVM                  GetSquirrelVM;
        /*logging utilities*/
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
        SqEx_SqThrow                        SqThrow;
        /*long numbers*/
        SqEx_GetSLongValue                  GetSLongValue;
        SqEx_PushSLongObject                PushSLongObject;
        SqEx_GetULongValue                  GetULongValue;
        SqEx_PushULongObject                PushULongObject;
        /*time utilities*/
        SqEx_GetCurrentSysTime              GetCurrentSysTime;
        SqEx_GetEpochTimeMicro              GetEpochTimeMicro;
        SqEx_GetEpochTimeMilli              GetEpochTimeMilli;
        SqEx_GetTimestamp                   GetTimestamp;
        SqEx_PushTimestamp                  PushTimestamp;
    } sq_exports, SQEXPORTS, *HSQEXPORTS;

    /* --------------------------------------------------------------------------------------------
     * Import the functions from the main squirrel plugin.
    */
    SQUIRREL_API HSQEXPORTS sq_api_import(PluginFuncs * vcapi);

    /* --------------------------------------------------------------------------------------------
     * Assign the functions from the specified API structure into the global functions.
    */
    SQUIRREL_API SQRESULT sq_api_expand(HSQAPI sqapi);

    /* --------------------------------------------------------------------------------------------
     * Undo changes done by sq_api_expand.
    */
    SQUIRREL_API void sq_api_collapse();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_SQ_API_H_*/
