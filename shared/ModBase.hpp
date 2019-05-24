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

#define VALID_ENTITY(e) (e >= 0)
#define INVALID_ENTITY(e) (e < 0)

#define VALID_ENTITYEX(e, m) ((e >= 0) && (e < m))
#define INVALID_ENTITYEX(e, m) ((e < 0) || (e >= m))

#define VALID_ENTITYGET(e) ((e >= 0) ? e : -1)
#define VALID_ENTITYGETEX(e, m) ((e >= 0) && (e < m) ? e : -1)

#define VALID_VEHCOL(e) ((e >= 0) && (e <= 94))
#define INVALID_VEHCOL(e) ((e < 0) && (e > 94))

/* ------------------------------------------------------------------------------------------------
 * COLOR PACKING
*/

#define SQMOD_PACK_RGB(r, g, b) static_cast< Uint32 >(r << 16 | g << 8 | b)
#define SQMOD_PACK_RGBA(r, g, b, a) static_cast< Uint32 >(r << 24 | g << 16 | b << 8 | a)
#define SQMOD_PACK_ARGB(a, r, g, b) static_cast< Uint32 >(a << 24 | r << 16 | g << 8 | b)

#define SQMOD_PACK_RGB_TO_RGBA(r, g, b) static_cast< Uint32 >(r << 24 | g << 16 | b << 8 | 0)
#define SQMOD_PACK_RGB_TO_ARGB(r, g, b) static_cast< Uint32 >(0 << 24 | r << 16 | g << 8 | b)

/* ------------------------------------------------------------------------------------------------
 * GENERAL RESPONSES
*/

#define SQMOD_SUCCESS     1
#define SQMOD_FAILURE     0
#define SQMOD_UNKNOWN     -1
#define SQMOD_TRUE        1
#define SQMOD_FALSE       0
#define SQMOD_NULL        NULL
#define SQMOD_BLANK       0

/* ------------------------------------------------------------------------------------------------
 * GENERAL SERVER LIMITS
*/

#define SQMOD_NAMELENGTH    64
#define SQMOD_NAMELENGTH_T  65
#define SQMOD_UIDLENGTH     40
#define SQMOD_UIDLENGTH_T   41
#define SQMOD_IPADDRESS     16
#define SQMOD_IPADDRESS_T   17
#define SQMOD_SVNAMELENGTH  128
#define SQMOD_PASSWDLENGTH  128
#define SQMOD_GMNAMELENGTH  96
#define SQMOD_MSGLENGTH     512
#define SQMOD_CMDLENGTH     512

/* ------------------------------------------------------------------------------------------------
 * INITIAL ENTITY POOLS
*/

#define SQMOD_BLIP_POOL         128
#define SQMOD_CHECKPOINT_POOL   2000
#define SQMOD_KEYBIND_POOL      256
#define SQMOD_OBJECT_POOL       3000
#define SQMOD_PICKUP_POOL       2000
#define SQMOD_PLAYER_POOL       100
#define SQMOD_VEHICLE_POOL      1000

/* ------------------------------------------------------------------------------------------------
 * ENTITY CREATE/DESTROY
*/

#define SQMOD_CREATE_DEFAULT    0
#define SQMOD_CREATE_MANUAL     -3
#define SQMOD_CREATE_POOL       -4
#define SQMOD_CREATE_AUTOMATIC  -5
#define SQMOD_CREATE_OVERWRITE  -6
#define SQMOD_CREATE_IMPORT     -7

#define SQMOD_DESTROY_DEFAULT   0
#define SQMOD_DESTROY_MANUAL    -3
#define SQMOD_DESTROY_POOL      -4
#define SQMOD_DESTROY_AUTOMATIC -5
#define SQMOD_DESTROY_OVERWRITE -6
#define SQMOD_DESTROY_CLEANUP   -7

/* ------------------------------------------------------------------------------------------------
 * MODEL ID LIMITS
*/

#define SQMOD_WEAPONID_MAX      71
#define SQMOD_WEAPONID_CAP      256
#define SQMOD_VEHICLEID_MAX     237
#define SQMOD_VEHICLEID_CAP     512
#define SQMOD_SKINID_MAX        160
#define SQMOD_SKINID_CAP        256
#define SQMOD_MODELID_MAX       640
#define SQMOD_MODELID_CAP       2048

/* ------------------------------------------------------------------------------------------------
 * SCRIPT OPTIONS
*/

/* ------------------------------------------------------------------------------------------------
 * PLUGIN OPTIONS
*/

#define SQMOD_STACK_SIZE            2048
#define SQMOD_MAX_ROUTINES          1024
#define SQMOD_MAX_CMD_ARGS          12
#define SQMOD_PLAYER_MSG_PREFIXES   16
#define SQMOD_PLAYER_TMP_BUFFER     128

/* ------------------------------------------------------------------------------------------------
 * PLAYER ACTION IDENTIFIERS
*/
#define SQMOD_PLAYER_ACTION_NONE                    0
#define SQMOD_PLAYER_ACTION_NORMAL                  1
#define SQMOD_PLAYER_ACTION_AIMING                  12
#define SQMOD_PLAYER_ACTION_SHOOTING                16
#define SQMOD_PLAYER_ACTION_JUMPING                 41
#define SQMOD_PLAYER_ACTION_LYING_ON_GROUND         42
#define SQMOD_PLAYER_ACTION_GETTING_UP              43
#define SQMOD_PLAYER_ACTION_JUMPING_FROM_VEHICLE    44
#define SQMOD_PLAYER_ACTION_DRIVING                 50
#define SQMOD_PLAYER_ACTION_DYING                   54
#define SQMOD_PLAYER_ACTION_WASTED                  55
#define SQMOD_PLAYER_ACTION_ENTERING_VEHICLE        58
#define SQMOD_PLAYER_ACTION_EXITING_VEHICLE         60
#define SQMOD_PLAYER_ACTION_MAX                     61

/* ------------------------------------------------------------------------------------------------
 * WEATHER IDENTIFIERS
*/
#define SQMOD_WEATHER_MOSTLYCLEAR       0
#define SQMOD_WEATHER_OVERCAST          1
#define SQMOD_WEATHER_THUNDERSTORM      2
#define SQMOD_WEATHER_STORM             2
#define SQMOD_WEATHER_STORMY            2
#define SQMOD_WEATHER_FOGGY             3
#define SQMOD_WEATHER_FOG               3
#define SQMOD_WEATHER_CLEAR             4
#define SQMOD_WEATHER_SUNNY             4
#define SQMOD_WEATHER_RAIN              5
#define SQMOD_WEATHER_RAINY             5
#define SQMOD_WEATHER_DARKCLOUDY        6
#define SQMOD_WEATHER_LIGHTCLOUDY       7
#define SQMOD_WEATHER_OVERCASTCLOUDY    8
#define SQMOD_WEATHER_BLACKCLOUDS       9
#define SQMOD_WEATHER_MAX               10


/* ------------------------------------------------------------------------------------------------
 * WEAPON IDENTIFIERS
*/
#define SQMOD_WEAPON_UNARMED            0
#define SQMOD_WEAPON_BRASSKNUCKLES      1
#define SQMOD_WEAPON_SCREWDRIVER        2
#define SQMOD_WEAPON_GOLFCLUB           3
#define SQMOD_WEAPON_NIGHTSTICK         4
#define SQMOD_WEAPON_KNIFE              5
#define SQMOD_WEAPON_BASEBALLBAT        6
#define SQMOD_WEAPON_HAMMER             7
#define SQMOD_WEAPON_MEATCLEAVER        8
#define SQMOD_WEAPON_MACHETE            9
#define SQMOD_WEAPON_KATANA             10
#define SQMOD_WEAPON_CHAINSAW           11
#define SQMOD_WEAPON_GRENADE            12
#define SQMOD_WEAPON_REMOTE             13
#define SQMOD_WEAPON_TEARGAS            14
#define SQMOD_WEAPON_MOLOTOV            15
#define SQMOD_WEAPON_ROCKET             16
#define SQMOD_WEAPON_COLT45             17
#define SQMOD_WEAPON_PYTHON             18
#define SQMOD_WEAPON_SHOTGUN            19
#define SQMOD_WEAPON_SPAS12             20
#define SQMOD_WEAPON_STUBBY             21
#define SQMOD_WEAPON_TEC9               22
#define SQMOD_WEAPON_UZI                23
#define SQMOD_WEAPON_INGRAM             24
#define SQMOD_WEAPON_MP5                25
#define SQMOD_WEAPON_M4                 26
#define SQMOD_WEAPON_RUGER              27
#define SQMOD_WEAPON_SNIPER             28
#define SQMOD_WEAPON_LASERSCOPE         29
#define SQMOD_WEAPON_ROCKETLAUNCHER     30
#define SQMOD_WEAPON_FLAMETHROWER       31
#define SQMOD_WEAPON_M60                32
#define SQMOD_WEAPON_MINIGUN            33
#define SQMOD_WEAPON_BOMB               34
#define SQMOD_WEAPON_HELICANNON         35
#define SQMOD_WEAPON_CAMERA             36
#define SQMOD_WEAPON_VEHICLE            39
#define SQMOD_WEAPON_EXPLOSION1         41
#define SQMOD_WEAPON_DRIVEBY            42
#define SQMOD_WEAPON_DROWNED            43
#define SQMOD_WEAPON_FALL               44
#define SQMOD_WEAPON_EXPLOSION2         51
#define SQMOD_WEAPON_SUICIDE            70
#define SQMOD_WEAPON_MAX                71

/* ------------------------------------------------------------------------------------------------
 * VEHICLE IDENTIFIERS
*/
#define SQMOD_VEHICLE_LANDSTALKER       130
#define SQMOD_VEHICLE_IDAHO             131
#define SQMOD_VEHICLE_STINGER           132
#define SQMOD_VEHICLE_LINERUNNER        133
#define SQMOD_VEHICLE_PERENNIAL         134
#define SQMOD_VEHICLE_SENTINEL          135
#define SQMOD_VEHICLE_RIO               136
#define SQMOD_VEHICLE_FIRETRUCK         137
#define SQMOD_VEHICLE_TRASHMASTER       138
#define SQMOD_VEHICLE_STRETCH           139
#define SQMOD_VEHICLE_MANANA            140
#define SQMOD_VEHICLE_INFERNUS          141
#define SQMOD_VEHICLE_VOODOO            142
#define SQMOD_VEHICLE_PONY              143
#define SQMOD_VEHICLE_MULE              144
#define SQMOD_VEHICLE_CHEETAH           145
#define SQMOD_VEHICLE_AMBULANCE         146
#define SQMOD_VEHICLE_FBIWASHINGTON     147
#define SQMOD_VEHICLE_MOONBEAM          148
#define SQMOD_VEHICLE_ESPERANTO         149
#define SQMOD_VEHICLE_TAXI              150
#define SQMOD_VEHICLE_WASHINGTON        151
#define SQMOD_VEHICLE_BOBCAT            152
#define SQMOD_VEHICLE_MRWHOOPEE         153
#define SQMOD_VEHICLE_BFINJECTION       154
#define SQMOD_VEHICLE_HUNTER            155
#define SQMOD_VEHICLE_POLICE            156
#define SQMOD_VEHICLE_ENFORCER          157
#define SQMOD_VEHICLE_SECURICAR         158
#define SQMOD_VEHICLE_BANSHEE           159
#define SQMOD_VEHICLE_PREDATOR          160
#define SQMOD_VEHICLE_BUS               161
#define SQMOD_VEHICLE_RHINO             162
#define SQMOD_VEHICLE_BARRACKSOL        163
#define SQMOD_VEHICLE_BARRACKS          163
#define SQMOD_VEHICLE_CUBANHERMES       164
#define SQMOD_VEHICLE_HELICOPTER        165
#define SQMOD_VEHICLE_ANGEL             166
#define SQMOD_VEHICLE_COACH             167
#define SQMOD_VEHICLE_CABBIE            168
#define SQMOD_VEHICLE_STALLION          169
#define SQMOD_VEHICLE_RUMPO             170
#define SQMOD_VEHICLE_RCBANDIT          171
#define SQMOD_VEHICLE_HEARSE            172
#define SQMOD_VEHICLE_PACKER            173
#define SQMOD_VEHICLE_SENTINELXS        174
#define SQMOD_VEHICLE_ADMIRAL           175
#define SQMOD_VEHICLE_SQUALO            176
#define SQMOD_VEHICLE_SEASPARROW        177
#define SQMOD_VEHICLE_PIZZABOY          178
#define SQMOD_VEHICLE_GANGBURRITO       179
#define SQMOD_VEHICLE_AIRTRAIN          180
#define SQMOD_VEHICLE_DEADDODO          181
#define SQMOD_VEHICLE_SPEEDER           182
#define SQMOD_VEHICLE_REEFER            183
#define SQMOD_VEHICLE_TROPIC            184
#define SQMOD_VEHICLE_FLATBED           185
#define SQMOD_VEHICLE_YANKEE            186
#define SQMOD_VEHICLE_CADDY             187
#define SQMOD_VEHICLE_ZEBRA             188
#define SQMOD_VEHICLE_ZEBRACAB          188
#define SQMOD_VEHICLE_TOPFUN            189
#define SQMOD_VEHICLE_SKIMMER           190
#define SQMOD_VEHICLE_PCJ600            191
#define SQMOD_VEHICLE_PCJ               191
#define SQMOD_VEHICLE_FAGGIO            192
#define SQMOD_VEHICLE_FREEWAY           193
#define SQMOD_VEHICLE_RCBARON           194
#define SQMOD_VEHICLE_RCRAIDER          195
#define SQMOD_VEHICLE_GLENDALE          196
#define SQMOD_VEHICLE_OCEANIC           197
#define SQMOD_VEHICLE_SANCHEZ           198
#define SQMOD_VEHICLE_SPARROW           199
#define SQMOD_VEHICLE_PATRIOT           200
#define SQMOD_VEHICLE_LOVEFIST          201
#define SQMOD_VEHICLE_COASTGUARD        202
#define SQMOD_VEHICLE_DINGHY            203
#define SQMOD_VEHICLE_HERMES            204
#define SQMOD_VEHICLE_SABRE             205
#define SQMOD_VEHICLE_SABRETURBO        206
#define SQMOD_VEHICLE_PHOENIX           207
#define SQMOD_VEHICLE_WALTON            208
#define SQMOD_VEHICLE_REGINA            209
#define SQMOD_VEHICLE_COMET             210
#define SQMOD_VEHICLE_DELUXO            211
#define SQMOD_VEHICLE_BURRITO           212
#define SQMOD_VEHICLE_SPANDEX           213
#define SQMOD_VEHICLE_SPANDEXPRESS      213
#define SQMOD_VEHICLE_MARQUIS           214
#define SQMOD_VEHICLE_BAGGAGE           215
#define SQMOD_VEHICLE_BAGGAGEHANDLER    215
#define SQMOD_VEHICLE_KAUFMAN           216
#define SQMOD_VEHICLE_KAUFMANCAB        216
#define SQMOD_VEHICLE_MAVERICK          217
#define SQMOD_VEHICLE_VCNMAVERICK       218
#define SQMOD_VEHICLE_RANCHER           219
#define SQMOD_VEHICLE_FBIRANCHER        220
#define SQMOD_VEHICLE_VIRGO             221
#define SQMOD_VEHICLE_GREENWOOD         222
#define SQMOD_VEHICLE_CUBANJETMAX       223
#define SQMOD_VEHICLE_HOTRING1          224
#define SQMOD_VEHICLE_HOTRINGRACER1     224
#define SQMOD_VEHICLE_SANDKING          225
#define SQMOD_VEHICLE_BLISTA            226
#define SQMOD_VEHICLE_BLISTAC           226
#define SQMOD_VEHICLE_BLISTACOMPACT     226
#define SQMOD_VEHICLE_COMPACT           226
#define SQMOD_VEHICLE_POLICEMAV         227
#define SQMOD_VEHICLE_POLICEMAVERICK    227
#define SQMOD_VEHICLE_BOXVILLE          228
#define SQMOD_VEHICLE_BENSON            229
#define SQMOD_VEHICLE_MESA              230
#define SQMOD_VEHICLE_MESAGRANDE        230
#define SQMOD_VEHICLE_RCGOBLIN          231
#define SQMOD_VEHICLE_HOTRING2          232
#define SQMOD_VEHICLE_HOTRINGRACER2     232
#define SQMOD_VEHICLE_HOTRING3          233
#define SQMOD_VEHICLE_HOTRINGRACER3     233
#define SQMOD_VEHICLE_BLOODRING1        234
#define SQMOD_VEHICLE_BLOODRINGBANGER1  234
#define SQMOD_VEHICLE_BLOODRING2        235
#define SQMOD_VEHICLE_BLOODRINGBANGER2  235
#define SQMOD_VEHICLE_VICECHEE          236
#define SQMOD_VEHICLE_POLICECHEETAH     236
#define SQMOD_VEHICLE_FBICHEETAH        236
#define SQMOD_VEHICLE_CHEETAH2          236
#define SQMOD_VEHICLE_MAX               237

/* ------------------------------------------------------------------------------------------------
 * SKIN IDENTIFIERS
*/
#define SQMOD_SKIN_TOMMY_VERCETTI           0
#define SQMOD_SKIN_COP                      1
#define SQMOD_SKIN_SWAT                     2
#define SQMOD_SKIN_FBI                      3
#define SQMOD_SKIN_ARMY                     4
#define SQMOD_SKIN_PARAMEDIC                5
#define SQMOD_SKIN_FIREMAN                  6
#define SQMOD_SKIN_GOLF_GUY_A               7
#define SQMOD_SKIN_BUM_LADY_A               9
#define SQMOD_SKIN_BUM_LADY_B               10
#define SQMOD_SKIN_PUNK_A                   11
#define SQMOD_SKIN_LAWYER                   12
#define SQMOD_SKIN_SPANISH_LADY_A           13
#define SQMOD_SKIN_SPANISH_LADY_B           14
#define SQMOD_SKIN_COOL_GUY_A               15
#define SQMOD_SKIN_ARABIC_GUY               16
#define SQMOD_SKIN_BEACH_LADY_A             17
#define SQMOD_SKIN_BEACH_LADY_B             18
#define SQMOD_SKIN_BEACH_GUY_A              19
#define SQMOD_SKIN_BEACH_GUY_B              20
#define SQMOD_SKIN_OFFICE_LADY_A            21
#define SQMOD_SKIN_WAITRESS_A               22
#define SQMOD_SKIN_FOOD_LADY                23
#define SQMOD_SKIN_PROSTITUTE_A             24
#define SQMOD_SKIN_BUM_LADY_C               25
#define SQMOD_SKIN_BUM_GUY_A                26
#define SQMOD_SKIN_GARBAGEMAN_A             27
#define SQMOD_SKIN_TAXI_DRIVER_A            28
#define SQMOD_SKIN_HATIAN_A                 29
#define SQMOD_SKIN_CRIMINAL_A               30
#define SQMOD_SKIN_HOOD_LADY                31
#define SQMOD_SKIN_GRANNY_A                 32
#define SQMOD_SKIN_BUSINESS_MAN_A           33
#define SQMOD_SKIN_CHURCH_GUY               34
#define SQMOD_SKIN_CLUB_LADY                35
#define SQMOD_SKIN_CHURCH_LADY              36
#define SQMOD_SKIN_PIMP                     37
#define SQMOD_SKIN_BEACH_LADY_C             38
#define SQMOD_SKIN_BEACH_GUY_C              39
#define SQMOD_SKIN_BEACH_LADY_D             40
#define SQMOD_SKIN_BEACH_GUY_D              41
#define SQMOD_SKIN_BUSINESS_MAN_B           42
#define SQMOD_SKIN_PROSTITUTE_B             43
#define SQMOD_SKIN_BUM_LADY_D               44
#define SQMOD_SKIN_BUM_GUY_B                45
#define SQMOD_SKIN_HATIAN_B                 46
#define SQMOD_SKIN_CONSTRUCTION_WORKER_A    47
#define SQMOD_SKIN_PUNK_B                   48
#define SQMOD_SKIN_PROSTITUTE_C             49
#define SQMOD_SKIN_GRANNY_B                 50
#define SQMOD_SKIN_PUNK_C                   51
#define SQMOD_SKIN_BUSINESS_MAN_C           52
#define SQMOD_SKIN_SPANISH_LADY_C           53
#define SQMOD_SKIN_SPANISH_LADY_D           54
#define SQMOD_SKIN_COOL_GUY_B               55
#define SQMOD_SKIN_BUSINESS_MAN_D           56
#define SQMOD_SKIN_BEACH_LADY_E             57
#define SQMOD_SKIN_BEACH_GUY_E              58
#define SQMOD_SKIN_BEACH_LADY_F             59
#define SQMOD_SKIN_BEACH_GUY_F              60
#define SQMOD_SKIN_CONSTRUCTION_WORKER_B    61
#define SQMOD_SKIN_GOLF_GUY_B               62
#define SQMOD_SKIN_GOLF_LADY                63
#define SQMOD_SKIN_GOLF_GUY_C               64
#define SQMOD_SKIN_BEACH_LADY_G             65
#define SQMOD_SKIN_BEACH_GUY_G              66
#define SQMOD_SKIN_OFFICE_LADY_B            67
#define SQMOD_SKIN_BUSINESS_MAN_E           68
#define SQMOD_SKIN_BUSINESS_MAN_F           69
#define SQMOD_SKIN_PROSTITUTE_D             70
#define SQMOD_SKIN_BUM_LADY_E               71
#define SQMOD_SKIN_BUM_GUY_C                72
#define SQMOD_SKIN_SPANISH_GUY              73
#define SQMOD_SKIN_TAXI_DRIVER_B            74
#define SQMOD_SKIN_GYM_LADY                 75
#define SQMOD_SKIN_GYM_GUY                  76
#define SQMOD_SKIN_SKATE_LADY               77
#define SQMOD_SKIN_SKATE_GUY                78
#define SQMOD_SKIN_SHOPPER_A                79
#define SQMOD_SKIN_SHOPPER_B                80
#define SQMOD_SKIN_TOURIST_A                81
#define SQMOD_SKIN_TOURIST_B                82
#define SQMOD_SKIN_CUBAN_A                  83
#define SQMOD_SKIN_CUBAN_B                  84
#define SQMOD_SKIN_HATIAN_C                 85
#define SQMOD_SKIN_HATIAN_D                 86
#define SQMOD_SKIN_SHARK_A                  87
#define SQMOD_SKIN_SHARK_B                  88
#define SQMOD_SKIN_DIAZ_GUY_A               89
#define SQMOD_SKIN_DIAZ_GUY_B               90
#define SQMOD_SKIN_DBP_SECURITY_A           91
#define SQMOD_SKIN_DBP_SECURITY_B           92
#define SQMOD_SKIN_BIKER_A                  93
#define SQMOD_SKIN_BIKER_B                  94
#define SQMOD_SKIN_VERCETTI_GUY_A           95
#define SQMOD_SKIN_VERCETTI_GUY_B           96
#define SQMOD_SKIN_UNDERCOVER_COP_A         97
#define SQMOD_SKIN_UNDERCOVER_COP_B         98
#define SQMOD_SKIN_UNDERCOVER_COP_C         99
#define SQMOD_SKIN_UNDERCOVER_COP_D         100
#define SQMOD_SKIN_UNDERCOVER_COP_E         101
#define SQMOD_SKIN_UNDERCOVER_COP_F         102
#define SQMOD_SKIN_RICH_GUY                 103
#define SQMOD_SKIN_COOL_GUY_C               104
#define SQMOD_SKIN_PROSTITUTE_E             105
#define SQMOD_SKIN_PROSTITUTE_F             106
#define SQMOD_SKIN_LOVE_FIST_A              107
#define SQMOD_SKIN_KEN_ROSENBURG            108
#define SQMOD_SKIN_CANDY_SUXX               109
#define SQMOD_SKIN_HILARY                   110
#define SQMOD_SKIN_LOVE_FIST_B              111
#define SQMOD_SKIN_PHIL                     112
#define SQMOD_SKIN_ROCKSTAR_GUY             113
#define SQMOD_SKIN_SONNY                    114
#define SQMOD_SKIN_LANCE_A                  115
#define SQMOD_SKIN_MERCADES_A               116
#define SQMOD_SKIN_LOVE_FIST_C              117
#define SQMOD_SKIN_ALEX_SRUB                118
#define SQMOD_SKIN_LANCE_COP                119
#define SQMOD_SKIN_LANCE_B                  120
#define SQMOD_SKIN_CORTEZ                   121
#define SQMOD_SKIN_LOVE_FIST_D              122
#define SQMOD_SKIN_COLUMBIAN_GUY_A          123
#define SQMOD_SKIN_HILARY_ROBBER            124
#define SQMOD_SKIN_MERCADES_B               125
#define SQMOD_SKIN_CAM                      126
#define SQMOD_SKIN_CAM_ROBBER               127
#define SQMOD_SKIN_PHIL_ONE_ARM             128
#define SQMOD_SKIN_PHIL_ROBBER              129
#define SQMOD_SKIN_COOL_GUY_D               130
#define SQMOD_SKIN_PIZZAMAN                 131
#define SQMOD_SKIN_TAXI_DRIVER_C            132
#define SQMOD_SKIN_TAXI_DRIVER_D            133
#define SQMOD_SKIN_SAILOR_A                 134
#define SQMOD_SKIN_SAILOR_B                 135
#define SQMOD_SKIN_SAILOR_C                 136
#define SQMOD_SKIN_CHEF                     137
#define SQMOD_SKIN_CRIMINAL_B               138
#define SQMOD_SKIN_FRENCH_GUY               139
#define SQMOD_SKIN_GARBAGEMAN_B             140
#define SQMOD_SKIN_HATIAN_E                 141
#define SQMOD_SKIN_WAITRESS_B               142
#define SQMOD_SKIN_SONNY_GUY_A              143
#define SQMOD_SKIN_SONNY_GUY_B              144
#define SQMOD_SKIN_SONNY_GUY_C              145
#define SQMOD_SKIN_COLUMBIAN_GUY_B          146
#define SQMOD_SKIN_THUG_A                   147
#define SQMOD_SKIN_BEACH_GUY_H              148
#define SQMOD_SKIN_GARBAGEMAN_C             149
#define SQMOD_SKIN_GARBAGEMAN_D             150
#define SQMOD_SKIN_GARBAGEMAN_E             151
#define SQMOD_SKIN_TRANNY                   152
#define SQMOD_SKIN_THUG_B                   153
#define SQMOD_SKIN_SPANDEX_GUY_A            154
#define SQMOD_SKIN_SPANDEX_GUY_B            155
#define SQMOD_SKIN_STRIPPER_A               156
#define SQMOD_SKIN_STRIPPER_B               157
#define SQMOD_SKIN_STRIPPER_C               158
#define SQMOD_SKIN_STORE_CLERK              159
#define SQMOD_SKIN_MAX                      160

/* ------------------------------------------------------------------------------------------------
 * KEYBOARD CODES
*/
#define SQMOD_KEYCODE_ABNT_C1               0xC1
#define SQMOD_KEYCODE_ABNT_C2               0xC2
#define SQMOD_KEYCODE_ADD                   0x6B
#define SQMOD_KEYCODE_ATTN                  0xF6
#define SQMOD_KEYCODE_BACK                  0x08
#define SQMOD_KEYCODE_CANCEL                0x03
#define SQMOD_KEYCODE_CLEAR                 0x0C
#define SQMOD_KEYCODE_CRSEL                 0xF7
#define SQMOD_KEYCODE_DECIMAL               0x6E
#define SQMOD_KEYCODE_DIVIDE                0x6F
#define SQMOD_KEYCODE_EREOF                 0xF9
#define SQMOD_KEYCODE_ESCAPE                0x1B
#define SQMOD_KEYCODE_EXECUTE               0x2B
#define SQMOD_KEYCODE_EXSEL                 0xF8
#define SQMOD_KEYCODE_ICO_CLEAR             0xE6
#define SQMOD_KEYCODE_ICO_HELP              0xE3
#define SQMOD_KEYCODE_KEY_0                 0x30
#define SQMOD_KEYCODE_KEY_1                 0x31
#define SQMOD_KEYCODE_KEY_2                 0x32
#define SQMOD_KEYCODE_KEY_3                 0x33
#define SQMOD_KEYCODE_KEY_4                 0x34
#define SQMOD_KEYCODE_KEY_5                 0x35
#define SQMOD_KEYCODE_KEY_6                 0x36
#define SQMOD_KEYCODE_KEY_7                 0x37
#define SQMOD_KEYCODE_KEY_8                 0x38
#define SQMOD_KEYCODE_KEY_9                 0x39
#define SQMOD_KEYCODE_KEY_A                 0x41
#define SQMOD_KEYCODE_KEY_B                 0x42
#define SQMOD_KEYCODE_KEY_C                 0x43
#define SQMOD_KEYCODE_KEY_D                 0x44
#define SQMOD_KEYCODE_KEY_E                 0x45
#define SQMOD_KEYCODE_KEY_F                 0x46
#define SQMOD_KEYCODE_KEY_G                 0x47
#define SQMOD_KEYCODE_KEY_H                 0x48
#define SQMOD_KEYCODE_KEY_I                 0x49
#define SQMOD_KEYCODE_KEY_J                 0x4A
#define SQMOD_KEYCODE_KEY_K                 0x4B
#define SQMOD_KEYCODE_KEY_L                 0x4C
#define SQMOD_KEYCODE_KEY_M                 0x4D
#define SQMOD_KEYCODE_KEY_N                 0x4E
#define SQMOD_KEYCODE_KEY_O                 0x4F
#define SQMOD_KEYCODE_KEY_P                 0x50
#define SQMOD_KEYCODE_KEY_Q                 0x51
#define SQMOD_KEYCODE_KEY_R                 0x52
#define SQMOD_KEYCODE_KEY_S                 0x53
#define SQMOD_KEYCODE_KEY_T                 0x54
#define SQMOD_KEYCODE_KEY_U                 0x55
#define SQMOD_KEYCODE_KEY_V                 0x56
#define SQMOD_KEYCODE_KEY_W                 0x57
#define SQMOD_KEYCODE_KEY_X                 0x58
#define SQMOD_KEYCODE_KEY_Y                 0x59
#define SQMOD_KEYCODE_KEY_Z                 0x5A
#define SQMOD_KEYCODE_MULTIPLY              0x6A
#define SQMOD_KEYCODE_NONAME                0xFC
#define SQMOD_KEYCODE_NUMPAD0               0x60
#define SQMOD_KEYCODE_NUMPAD1               0x61
#define SQMOD_KEYCODE_NUMPAD2               0x62
#define SQMOD_KEYCODE_NUMPAD3               0x63
#define SQMOD_KEYCODE_NUMPAD4               0x64
#define SQMOD_KEYCODE_NUMPAD5               0x65
#define SQMOD_KEYCODE_NUMPAD6               0x66
#define SQMOD_KEYCODE_NUMPAD7               0x67
#define SQMOD_KEYCODE_NUMPAD8               0x68
#define SQMOD_KEYCODE_NUMPAD9               0x69
#define SQMOD_KEYCODE_OEM_1                 0xBA
#define SQMOD_KEYCODE_OEM_102               0xE2
#define SQMOD_KEYCODE_OEM_2                 0xBF
#define SQMOD_KEYCODE_OEM_3                 0xC0
#define SQMOD_KEYCODE_OEM_4                 0xDB
#define SQMOD_KEYCODE_OEM_5                 0xDC
#define SQMOD_KEYCODE_OEM_6                 0xDD
#define SQMOD_KEYCODE_OEM_7                 0xDE
#define SQMOD_KEYCODE_OEM_8                 0xDF
#define SQMOD_KEYCODE_OEM_ATTN              0xF0
#define SQMOD_KEYCODE_OEM_AUTO              0xF3
#define SQMOD_KEYCODE_OEM_AX                0xE1
#define SQMOD_KEYCODE_OEM_BACKTAB           0xF5
#define SQMOD_KEYCODE_OEM_CLEAR             0xFE
#define SQMOD_KEYCODE_OEM_COMMA             0xBC
#define SQMOD_KEYCODE_OEM_COPY              0xF2
#define SQMOD_KEYCODE_OEM_CUSEL             0xEF
#define SQMOD_KEYCODE_OEM_ENLW              0xF4
#define SQMOD_KEYCODE_OEM_FINISH            0xF1
#define SQMOD_KEYCODE_OEM_FJ_LOYA           0x95
#define SQMOD_KEYCODE_OEM_FJ_MASSHOU        0x93
#define SQMOD_KEYCODE_OEM_FJ_ROYA           0x96
#define SQMOD_KEYCODE_OEM_FJ_TOUROKU        0x94
#define SQMOD_KEYCODE_OEM_JUMP              0xEA
#define SQMOD_KEYCODE_OEM_MINUS             0xBD
#define SQMOD_KEYCODE_OEM_PA1               0xEB
#define SQMOD_KEYCODE_OEM_PA2               0xEC
#define SQMOD_KEYCODE_OEM_PA3               0xED
#define SQMOD_KEYCODE_OEM_PERIOD            0xBE
#define SQMOD_KEYCODE_OEM_PLUS              0xBB
#define SQMOD_KEYCODE_OEM_RESET             0xE9
#define SQMOD_KEYCODE_OEM_WSCTRL            0xEE
#define SQMOD_KEYCODE_PA1                   0xFD
#define SQMOD_KEYCODE_PACKET                0xE7
#define SQMOD_KEYCODE_PLAY                  0xFA
#define SQMOD_KEYCODE_PROCESSKEY            0xE5
#define SQMOD_KEYCODE_RETURN                0x0D
#define SQMOD_KEYCODE_SELECT                0x29
#define SQMOD_KEYCODE_SEPARATOR             0x6C
#define SQMOD_KEYCODE_SPACE                 0x20
#define SQMOD_KEYCODE_SUBTRACT              0x6D
#define SQMOD_KEYCODE_TAB                   0x09
#define SQMOD_KEYCODE_ZOOM                  0xFB
#define SQMOD_KEYCODE_ACCEPT                0x1E
#define SQMOD_KEYCODE_APPS                  0x5D
#define SQMOD_KEYCODE_BROWSER_BACK          0xA6
#define SQMOD_KEYCODE_BROWSER_FAVORITES     0xAB
#define SQMOD_KEYCODE_BROWSER_FORWARD       0xA7
#define SQMOD_KEYCODE_BROWSER_HOME          0xAC
#define SQMOD_KEYCODE_BROWSER_REFRESH       0xA8
#define SQMOD_KEYCODE_BROWSER_SEARCH        0xAA
#define SQMOD_KEYCODE_BROWSER_STOP          0xA9
#define SQMOD_KEYCODE_CAPITAL               0x14
#define SQMOD_KEYCODE_CONVERT               0x1C
#define SQMOD_KEYCODE_DELETE                0x2E
#define SQMOD_KEYCODE_DOWN                  0x28
#define SQMOD_KEYCODE_END                   0x23
#define SQMOD_KEYCODE_F1                    0x70
#define SQMOD_KEYCODE_F10                   0x79
#define SQMOD_KEYCODE_F11                   0x7A
#define SQMOD_KEYCODE_F12                   0x7B
#define SQMOD_KEYCODE_F13                   0x7C
#define SQMOD_KEYCODE_F14                   0x7D
#define SQMOD_KEYCODE_F15                   0x7E
#define SQMOD_KEYCODE_F16                   0x7F
#define SQMOD_KEYCODE_F17                   0x80
#define SQMOD_KEYCODE_F18                   0x81
#define SQMOD_KEYCODE_F19                   0x82
#define SQMOD_KEYCODE_F2                    0x71
#define SQMOD_KEYCODE_F20                   0x83
#define SQMOD_KEYCODE_F21                   0x84
#define SQMOD_KEYCODE_F22                   0x85
#define SQMOD_KEYCODE_F23                   0x86
#define SQMOD_KEYCODE_F24                   0x87
#define SQMOD_KEYCODE_F3                    0x72
#define SQMOD_KEYCODE_F4                    0x73
#define SQMOD_KEYCODE_F5                    0x74
#define SQMOD_KEYCODE_F6                    0x75
#define SQMOD_KEYCODE_F7                    0x76
#define SQMOD_KEYCODE_F8                    0x77
#define SQMOD_KEYCODE_F9                    0x78
#define SQMOD_KEYCODE_FINAL                 0x18
#define SQMOD_KEYCODE_HELP                  0x2F
#define SQMOD_KEYCODE_HOME                  0x24
#define SQMOD_KEYCODE_ICO_00                0xE4
#define SQMOD_KEYCODE_INSERT                0x2D
#define SQMOD_KEYCODE_JUNJA                 0x17
#define SQMOD_KEYCODE_KANA                  0x15
#define SQMOD_KEYCODE_KANJI                 0x19
#define SQMOD_KEYCODE_LAUNCH_APP1           0xB6
#define SQMOD_KEYCODE_LAUNCH_APP2           0xB7
#define SQMOD_KEYCODE_LAUNCH_MAIL           0xB4
#define SQMOD_KEYCODE_LAUNCH_MEDIA_SELECT   0xB5
#define SQMOD_KEYCODE_LBUTTON               0x01
#define SQMOD_KEYCODE_LCONTROL              0xA2
#define SQMOD_KEYCODE_LEFT                  0x25
#define SQMOD_KEYCODE_LMENU                 0xA4
#define SQMOD_KEYCODE_LSHIFT                0xA0
#define SQMOD_KEYCODE_LWIN                  0x5B
#define SQMOD_KEYCODE_MBUTTON               0x04
#define SQMOD_KEYCODE_MEDIA_NEXT_TRACK      0xB0
#define SQMOD_KEYCODE_MEDIA_PLAY_PAUSE      0xB3
#define SQMOD_KEYCODE_MEDIA_PREV_TRACK      0xB1
#define SQMOD_KEYCODE_MEDIA_STOP            0xB2
#define SQMOD_KEYCODE_MODECHANGE            0x1F
#define SQMOD_KEYCODE_NEXT                  0x22
#define SQMOD_KEYCODE_NONCONVERT            0x1D
#define SQMOD_KEYCODE_NUMLOCK               0x90
#define SQMOD_KEYCODE_OEM_FJ_JISHO          0x92
#define SQMOD_KEYCODE_PAUSE                 0x13
#define SQMOD_KEYCODE_PRINT                 0x2A
#define SQMOD_KEYCODE_PRIOR                 0x21
#define SQMOD_KEYCODE_RBUTTON               0x02
#define SQMOD_KEYCODE_RCONTROL              0xA3
#define SQMOD_KEYCODE_RIGHT                 0x27
#define SQMOD_KEYCODE_RMENU                 0xA5
#define SQMOD_KEYCODE_RSHIFT                0xA1
#define SQMOD_KEYCODE_RWIN                  0x5C
#define SQMOD_KEYCODE_SCROLL                0x91
#define SQMOD_KEYCODE_SLEEP                 0x5F
#define SQMOD_KEYCODE_SNAPSHOT              0x2C
#define SQMOD_KEYCODE_UP                    0x26
#define SQMOD_KEYCODE_VOLUME_DOWN           0xAE
#define SQMOD_KEYCODE_VOLUME_MUTE           0xAD
#define SQMOD_KEYCODE_VOLUME_UP             0xAF
#define SQMOD_KEYCODE_XBUTTON1              0x05
#define SQMOD_KEYCODE_XBUTTON2              0x06
#define SQMOD_KEYCODE_NONE                  0xFF
#define SQMOD_KEYCODE_MAX                   0x100

/* ------------------------------------------------------------------------------------------------
 * ASCII CHARACTER CODES
*/
#define SQMOD_ASCII_NUL                     0
#define SQMOD_ASCII_SOH                     1
#define SQMOD_ASCII_STX                     2
#define SQMOD_ASCII_ETX                     3
#define SQMOD_ASCII_EOT                     4
#define SQMOD_ASCII_ENQ                     5
#define SQMOD_ASCII_ACK                     6
#define SQMOD_ASCII_BEL                     7
#define SQMOD_ASCII_BS                      8
#define SQMOD_ASCII_TAB                     9
#define SQMOD_ASCII_LF                      10
#define SQMOD_ASCII_VT                      11
#define SQMOD_ASCII_FF                      12
#define SQMOD_ASCII_CR                      13
#define SQMOD_ASCII_SO                      14
#define SQMOD_ASCII_SI                      15
#define SQMOD_ASCII_DLE                     16
#define SQMOD_ASCII_DC1                     17
#define SQMOD_ASCII_DC2                     18
#define SQMOD_ASCII_DC3                     19
#define SQMOD_ASCII_DC4                     20
#define SQMOD_ASCII_NAK                     21
#define SQMOD_ASCII_SYN                     22
#define SQMOD_ASCII_ETB                     23
#define SQMOD_ASCII_CAN                     24
#define SQMOD_ASCII_EM                      25
#define SQMOD_ASCII_SUB                     26
#define SQMOD_ASCII_ESC                     27
#define SQMOD_ASCII_FS                      28
#define SQMOD_ASCII_GS                      29
#define SQMOD_ASCII_RS                      30
#define SQMOD_ASCII_US                      31
#define SQMOD_ASCII_SPACE                   32
#define SQMOD_ASCII_EXCLAMATION_POINT       33
#define SQMOD_ASCII_DOUBLE_QUOTES           34
#define SQMOD_ASCII_NUMBER_SIGN             35
#define SQMOD_ASCII_DOLLAR_SIGN             36
#define SQMOD_ASCII_PERCENT_SIGN            37
#define SQMOD_ASCII_AMPERSAND               38
#define SQMOD_ASCII_SINGLE_QUOTE            39
#define SQMOD_ASCII_OPENING_PARENTHESIS     40
#define SQMOD_ASCII_CLOSING_PARENTHESIS     41
#define SQMOD_ASCII_ASTERISK                42
#define SQMOD_ASCII_PLUS                    43
#define SQMOD_ASCII_COMMA                   44
#define SQMOD_ASCII_MINUS                   45
#define SQMOD_ASCII_PERIOD                  46
#define SQMOD_ASCII_SLASH                   47
#define SQMOD_ASCII_ZERO                    48
#define SQMOD_ASCII_ONE                     49
#define SQMOD_ASCII_TWO                     50
#define SQMOD_ASCII_THREE                   51
#define SQMOD_ASCII_FOUR                    52
#define SQMOD_ASCII_FIVE                    53
#define SQMOD_ASCII_SIX                     54
#define SQMOD_ASCII_SEVEN                   55
#define SQMOD_ASCII_EIGHT                   56
#define SQMOD_ASCII_NINE                    57
#define SQMOD_ASCII_COLON                   58
#define SQMOD_ASCII_EMICOLON                59
#define SQMOD_ASCII_LESS_THAN_SIGN          60
#define SQMOD_ASCII_EQUAL_SIGN              61
#define SQMOD_ASCII_GREATER_THAN_SIGN       62
#define SQMOD_ASCII_QUESTION_MARK           63
#define SQMOD_ASCII_AT                      64
#define SQMOD_ASCII_UPPER_A                 65
#define SQMOD_ASCII_UPPER_B                 66
#define SQMOD_ASCII_UPPER_C                 67
#define SQMOD_ASCII_UPPER_D                 68
#define SQMOD_ASCII_UPPER_E                 69
#define SQMOD_ASCII_UPPER_F                 70
#define SQMOD_ASCII_UPPER_G                 71
#define SQMOD_ASCII_UPPER_H                 72
#define SQMOD_ASCII_UPPER_I                 73
#define SQMOD_ASCII_UPPER_J                 74
#define SQMOD_ASCII_UPPER_K                 75
#define SQMOD_ASCII_UPPER_L                 76
#define SQMOD_ASCII_UPPER_M                 77
#define SQMOD_ASCII_UPPER_N                 78
#define SQMOD_ASCII_UPPER_O                 79
#define SQMOD_ASCII_UPPER_P                 80
#define SQMOD_ASCII_UPPER_Q                 81
#define SQMOD_ASCII_UPPER_R                 82
#define SQMOD_ASCII_UPPER_S                 83
#define SQMOD_ASCII_UPPER_T                 84
#define SQMOD_ASCII_UPPER_U                 85
#define SQMOD_ASCII_UPPER_V                 86
#define SQMOD_ASCII_UPPER_W                 87
#define SQMOD_ASCII_UPPER_X                 88
#define SQMOD_ASCII_UPPER_Y                 89
#define SQMOD_ASCII_UPPER_Z                 90
#define SQMOD_ASCII_OPENING_BRACKET         91
#define SQMOD_ASCII_BACKSLASH               92
#define SQMOD_ASCII_CLOSING_BRACKET         93
#define SQMOD_ASCII_CARET                   94
#define SQMOD_ASCII_UNDERSCORE              95
#define SQMOD_ASCII_GRAVE_ACCENT            96
#define SQMOD_ASCII_LOWER_A                 97
#define SQMOD_ASCII_LOWER_B                 98
#define SQMOD_ASCII_LOWER_C                 99
#define SQMOD_ASCII_LOWER_D                 100
#define SQMOD_ASCII_LOWER_E                 101
#define SQMOD_ASCII_LOWER_F                 102
#define SQMOD_ASCII_LOWER_G                 103
#define SQMOD_ASCII_LOWER_H                 104
#define SQMOD_ASCII_LOWER_I                 105
#define SQMOD_ASCII_LOWER_J                 106
#define SQMOD_ASCII_LOWER_K                 107
#define SQMOD_ASCII_LOWER_L                 108
#define SQMOD_ASCII_LOWER_M                 109
#define SQMOD_ASCII_LOWER_N                 110
#define SQMOD_ASCII_LOWER_O                 111
#define SQMOD_ASCII_LOWER_P                 112
#define SQMOD_ASCII_LOWER_Q                 113
#define SQMOD_ASCII_LOWER_R                 114
#define SQMOD_ASCII_LOWER_S                 115
#define SQMOD_ASCII_LOWER_T                 116
#define SQMOD_ASCII_LOWER_U                 117
#define SQMOD_ASCII_LOWER_V                 118
#define SQMOD_ASCII_LOWER_W                 119
#define SQMOD_ASCII_LOWER_X                 120
#define SQMOD_ASCII_LOWER_Y                 121
#define SQMOD_ASCII_LOWER_Z                 122
#define SQMOD_ASCII_OPENING_BRACE           123
#define SQMOD_ASCII_VERTICAL_BAR            124
#define SQMOD_ASCII_CLOSING_BRACE           125
#define SQMOD_ASCII_TILDE                   126
#define SQMOD_ASCII_UNDEFINED               127
#define SQMOD_ASCII_MAX                     128

#endif // _SQMODBASE_HPP_
