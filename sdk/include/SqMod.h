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

    /* ------------------------------------------------------------------------------------------------
     * ARCHITECTURE IDENTIFIERS
    */

    #define SQMOD_ARCH_ID_UNKNOWN     0
    #define SQMOD_ARCH_ID_32_BIT      1
    #define SQMOD_ARCH_ID_64_BIT      2

    /* ------------------------------------------------------------------------------------------------
     * PLATFORM IDENTIFIERS
    */

    #define SQMOD_PLAT_ID_UNKNOWN     0
    #define SQMOD_PLAT_ID_WINDOWS     1
    #define SQMOD_PLAT_ID_LINUX       2
    #define SQMOD_PLAT_ID_MACOS       3
    #define SQMOD_PLAT_ID_UNIX        4

    /* ------------------------------------------------------------------------------------------------
     * OS IDENTIFICATION
    */

    #if defined(_WIN32) || defined(__WIN32__) || defined(_WIN) || defined(__WIN__)
        // Windows x32
        #define SQMOD_OS_WINDOWS
        #define SQMOD_OS_32
        #define SQMOD_OS_WINDOWS32
        #define SQMOD_ARCHITECTURE 1
        #define SQMOD_PLATFORM 1
    #elif defined(_WIN64) || defined(__WIN64__)
        // Windows x64
        #define SQMOD_OS_WINDOWS
        #define SQMOD_OS_64
        #define SQMOD_OS_WINDOWS64
        #define SQMOD_ARCHITECTURE 2
        #define SQMOD_PLATFORM 1
    #elif defined(linux) || defined(__linux) || defined(__linux__)
        // Linux
        #define SQMOD_OS_LINUX
        #if __GNUC__
            #if __x86_64__ || __ppc64__
                #define SQMOD_OS_64
                #define SQMOD_OS_LINUX64
                 #define SQMOD_ARCHITECTURE 2
                #define SQMOD_PLATFORM 2
            #else
                #define SQMOD_OS_32
                #define SQMOD_OS_LINUX32
                #define SQMOD_ARCHITECTURE 1
                #define SQMOD_PLATFORM 2
            #endif
        #endif
    #elif defined(__APPLE__) || defined(__MACH__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)
        // MacOS
        #define SQMOD_OS_MACOS
        #if __GNUC__
            #if __x86_64__ || __ppc64__
                #define SQMOD_OS_64
                #define SQMOD_OS_MACOS64
                #define SQMOD_ARCHITECTURE 2
                #define SQMOD_PLATFORM 3
            #else
                #define SQMOD_OS_32
                #define SQMOD_OS_MACOS32
                #define SQMOD_ARCHITECTURE 1
                #define SQMOD_PLATFORM 3
            #endif
        #endif
    #elif defined(__unix) || defined(__unix__)
        // Unix
        #define SQMOD_OS_UNIX
        #if __GNUC__
            #if __x86_64__ || __ppc64__
                #define SQMOD_OS_64
                #define SQMOD_OS_UNIX64
                #define SQMOD_ARCHITECTURE 2
                #define SQMOD_PLATFORM 4
            #else
                #define SQMOD_OS_32
                #define SQMOD_OS_UNIX32
                #define SQMOD_ARCHITECTURE 1
                #define SQMOD_PLATFORM 4
            #endif
        #endif
    #else
        // Unsupported system
        #error This operating system is not supported by the Squirrel Module
    #endif

    /* ------------------------------------------------------------------------------------------------
     * VCMP SDK VERSION CHECK.
    */
    #define SQMOD_SDK_MATCH(MJR, MNR) ((PLUGIN_API_MAJOR == (MJR)) && (PLUGIN_API_MINOR == (MNR)))
    #define SQMOD_SDK_LEAST(MJR, MNR) ((PLUGIN_API_MAJOR >= (MJR)) && (PLUGIN_API_MINOR >= (MNR)))
    #define SQMOD_SDK_PRIOR(MJR, MNR) ((PLUGIN_API_MAJOR < (MJR)) && (PLUGIN_API_MINOR < (MNR)))

    #define SQMOD_DECL_UNUSED_VAR(t, n, v) t n = v; (void)(n)
    #define SQMOD_UNUSED_VAR(n) (void)(n)

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
