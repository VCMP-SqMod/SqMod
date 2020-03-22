#ifndef _SQMODBASE_HPP_
#define _SQMODBASE_HPP_

// ------------------------------------------------------------------------------------------------
#include <sqconfig.h>

// ------------------------------------------------------------------------------------------------
#include <cstddef>
#include <cassert>
#include <string>

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

#ifndef SQMOD_ARCHITECTURE
     #define SQMOD_ARCHITECTURE 0
#endif

#ifndef SQMOD_PLATFORM
     #define SQMOD_PLATFORM 0
#endif

/* ------------------------------------------------------------------------------------------------
 * SQUIRREL FORWARD DECLARATIONS
*/

extern "C" {
    typedef struct tagSQObject SQObject;
    struct SQVM;
    typedef struct SQVM* HSQUIRRELVM;
    typedef SQObject HSQOBJECT;
} /*extern "C"*/

/* ------------------------------------------------------------------------------------------------
 * SQRAT FORWARD DECLARATIONS
*/

namespace Sqrat {
    class Array;
    class Object;
    class Table;
    class Function;
} // Namespace:: Sqrat

/* ------------------------------------------------------------------------------------------------
 * FUNDAMENTAL DATATYPES
*/

namespace SqMod {

/**< 8 bits integer types */
typedef char                        Int8, I8;
typedef unsigned char               Uint8, U8;

/**< 16 bits integer types */
typedef short                       Int16, I16;
typedef unsigned short              Uint16, U16;

/**< 32 bits integer types */
typedef int                         Int32, I32;
typedef unsigned int                Uint32, U32;

/**< 64 bits integer types */
#if defined(_MSC_VER)
    typedef __int64                 Int64, I64;
    typedef unsigned __int64        Uint64, U64;
#else
    typedef long long               Int64, I64;
    typedef unsigned long long      Uint64, U64;
#endif

/**< integer type */
#ifdef SQMOD_LONG
    typedef Int64                   Int, Integer;
    typedef Uint64                  Uint, Uinteger, UnisgnedInteger;
#else
    typedef Int32                   Int, Integer;
    typedef Uint32                  Uint, Uinteger, UnisgnedInteger;
#endif

/**< long integer type */
typedef long                        LongI;
typedef unsigned long               Ulong;

/**< 32 bits float types */
typedef float                       Float32, Real32, F32;

/**< 64 bits float types */
typedef double                      Float64, Real64, F64;

/**< boolean type */
typedef Uint8                       Boolean;

/**< character type */
typedef bool                        BoolT;

/**< character types */
typedef char                        CharT;

/**< user type */
typedef void *                      VoidP;

/**< size type */
typedef Uint32                      SizeT;

/* ------------------------------------------------------------------------------------------------
 * STRING TYPE
*/

typedef std::basic_string<SQChar>   String;

typedef char *                      CStr;
typedef const char *                CCStr;

typedef SQChar *                    SStr;
typedef const SQChar *              CSStr;

/* ------------------------------------------------------------------------------------------------
 * SHORT SQUIRREL TYPENAMES
*/

typedef SQUnsignedInteger32         SQUint32;
typedef SQUnsignedInteger           SQUint;
typedef SQInteger                   SQInt;

// ------------------------------------------------------------------------------------------------
using namespace Sqrat;

/* ------------------------------------------------------------------------------------------------
 * Squirrel compatible stl string.
*/

typedef std::basic_string< SQChar > String;

/* ------------------------------------------------------------------------------------------------
 * FORWARD DECLARATIONS
*/

} // Namespace:: SqMod

/* ------------------------------------------------------------------------------------------------
 * OS SPECIFFIC OPTIONS
*/

#if defined(SQMOD_OS_WINDOWS)
    #define SQMOD_DIRSEP_CHAR   '\\'
    #define SQMOD_DIRSEP_STR    "\\"
#else
    #define SQMOD_DIRSEP_CHAR   '/'
    #define SQMOD_DIRSEP_STR    "/"
#endif

/* ------------------------------------------------------------------------------------------------
 * SYMBOL EXPORTING
*/

#if defined(_MSC_VER)
    #define SQMOD_EXPORT          __declspec(dllexport)
    #define SQMOD_IMPORT          __declspec(dllimport)
#elif defined(__GNUC__)
    #define SQMOD_EXPORT          __declspec(dllexport)
    #define SQMOD_IMPORT          __declspec(dllimport)
#endif

#if defined(__cplusplus)
    #define SQMOD_EXTERN_C        extern "C"
#else
    #define SQMOD_EXTERN_C        /* */
#endif

#if defined(_MSC_VER)
    #define SQMOD_API_EXPORT      extern "C" __declspec(dllexport)
#elif defined(__GNUC__)
    #define SQMOD_API_EXPORT      extern "C"
#else
    #define SQMOD_API_EXPORT      extern "C"
#endif

/* ------------------------------------------------------------------------------------------------
 * CALLING CONVENTIONS
*/

#if defined(_MSC_VER)
    #define SQMOD_STDCALL         __stdcall
    #define SQMOD_CDECL           __cdecl
    #define SQMOD_FASTCALL        __fastcall
#elif defined(__GNUC__)
    #define SQMOD_STDCALL         __attribute__((stdcall))
    #define SQMOD_CDECL           /* */
    #define SQMOD_FASTCALL        __attribute__((fastcall))
#endif

/* ------------------------------------------------------------------------------------------------
 * FUNCTION INLINING
*/

#if defined(_MSC_VER)
    #define SQMOD_FORCEINLINE    __forceinline
#elif defined(__GNUC__)
    #define SQMOD_FORCEINLINE    inline
#endif

/* ------------------------------------------------------------------------------------------------
 * ATTRIBUTES
*/
#if defined(__GNUC__) && __GNUC__ >= 7
 #define SQ_FALL_THROUGH __attribute__ ((fallthrough))
#else
 #define SQ_FALL_THROUGH ((void)0)
#endif // __GNUC__ >= 7

/* ------------------------------------------------------------------------------------------------
 * LOGGING LOCATION
*/

#define SQMOD_TRUESTRINGIZE(x) #x
#define SQMOD_STRINGIZEWRAP(x) SQMOD_TRUESTRINGIZE(x)

#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    #define SQMOD_MSGLOC(m) (m " =>[" __FILE__ ":" SQMOD_STRINGIZEWRAP(__LINE__) "] ")
#else
    #define SQMOD_MSGLOC(m) (m)
#endif // _DEBUG

/* ------------------------------------------------------------------------------------------------
 * EXCEPTION THROWING
*/

#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    #define STHROW(e, m, ...) throw e(m " =>[" __FILE__ ":" SQMOD_STRINGIZEWRAP(__LINE__) "] ", ##__VA_ARGS__)
    #define STHROWF(m, ...) SqThrowF(m " =>[" __FILE__ ":" SQMOD_STRINGIZEWRAP(__LINE__) "] ", ##__VA_ARGS__)
#else
    #define STHROW(e, m, ...) throw e(m, ##__VA_ARGS__)
    #define STHROWF(m, ...) SqThrowF(m, ##__VA_ARGS__)
#endif // _DEBUG

/* ------------------------------------------------------------------------------------------------
 * VARIOUS DEFINES
*/

#define SQMOD_DECL_UNUSED_VAR(t, n, v) t n = v; (void)(n)
#define SQMOD_UNUSED_VAR(n) (void)(n)

#define VALID_ENTITY(e) ((e) >= 0)
#define INVALID_ENTITY(e) ((e) < 0)

#define VALID_ENTITYEX(e, m) (((e) >= 0) && ((e) < (m)))
#define INVALID_ENTITYEX(e, m) (((e) < 0) || ((e) >= (m)))

#define VALID_ENTITYGET(e) (((e) >= 0) ? (e) : -1)
#define VALID_ENTITYGETEX(e, m) (((e) >= 0) && ((e) < (m)) ? (e) : -1)

#define VALID_VEHCOL(e) (((e) >= 0) && ((e) <= 94))
#define INVALID_VEHCOL(e) (((e) < 0) && ((e) > 94))

/* ------------------------------------------------------------------------------------------------
 * GENERAL RESPONSES
*/

#define SQMOD_SUCCESS     1
#define SQMOD_FAILURE     0
#define SQMOD_UNKNOWN     (-1)
#define SQMOD_TRUE        1
#define SQMOD_FALSE       0
#define SQMOD_NULL        NULL
#define SQMOD_BLANK       0

#endif // _SQMODBASE_HPP_
