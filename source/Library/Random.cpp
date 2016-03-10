// ------------------------------------------------------------------------------------------------
#include "Library/Random.hpp"
#include "Base/Shared.hpp"
#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <ctime>
#include <memory>
#include <random>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_OS_WINDOWS
    #include <process.h>
#else
    #include <sys/types.h>
    #include <unistd.h>
#endif

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static std::unique_ptr< std::mt19937 >          RG32_MT19937 =
                            std::unique_ptr< std::mt19937 >(new std::mt19937(GenerateSeed()));

static std::unique_ptr< std::mt19937_64 >       RG64_MT19937 =
                            std::unique_ptr< std::mt19937_64 >(new std::mt19937_64(GenerateSeed()));

// ------------------------------------------------------------------------------------------------
static std::uniform_int_distribution< Int8 >    Int8_Dist(std::numeric_limits< Int8 >::min(),
                                                            std::numeric_limits< Int8 >::max());
static std::uniform_int_distribution< Uint8 >   Uint8_Dist(std::numeric_limits< Uint8 >::min(),
                                                            std::numeric_limits< Uint8 >::max());

static std::uniform_int_distribution< Int16 >   Int16_Dist(std::numeric_limits< Int16 >::min(),
                                                            std::numeric_limits< Int16 >::max());
static std::uniform_int_distribution< Uint16 >  Uint16_Dist(std::numeric_limits< Uint16 >::min(),
                                                            std::numeric_limits< Uint16 >::max());

static std::uniform_int_distribution< Int32 >   Int32_Dist(std::numeric_limits< Int32 >::min(),
                                                            std::numeric_limits< Int32 >::max());
static std::uniform_int_distribution< Uint32 >  Uint32_Dist(std::numeric_limits< Uint32 >::min(),
                                                            std::numeric_limits< Uint32 >::max());

static std::uniform_int_distribution< Int64 >   Int64_Dist(std::numeric_limits< Int64 >::min(),
                                                            std::numeric_limits< Int64 >::max());
static std::uniform_int_distribution< Uint64 >  Uint64_Dist(std::numeric_limits< Uint64 >::min(),
                                                            std::numeric_limits< Uint64 >::max());

static std::uniform_real_distribution<Float32>  Float32_Dist(std::numeric_limits< Float32 >::min(),
                                                            std::numeric_limits< Float32 >::max());
static std::uniform_real_distribution<Float64>  Float64_Dist(std::numeric_limits< Float64 >::min(),
                                                            std::numeric_limits< Float64 >::max());

// ------------------------------------------------------------------------------------------------
static std::uniform_int_distribution< String::value_type >
                                    String_Dist(std::numeric_limits< String::value_type >::min(),
                                                std::numeric_limits< String::value_type >::max());

// ------------------------------------------------------------------------------------------------
SizeT GenerateSeed()
{
    Ulong a = clock();
    Ulong b = time(NULL);
#ifdef SQMOD_OS_WINDOWS
    Ulong c = _getpid();
#else
    Ulong c = getpid();
#endif
    // Mangle
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    // Return result
    return c;
}

// ------------------------------------------------------------------------------------------------
void ReseedRandom()
{
    RG32_MT19937.reset(new std::mt19937(GenerateSeed()));
    RG64_MT19937.reset(new std::mt19937_64(GenerateSeed()));
}

void ReseedRandom(Uint32 n)
{
    RG32_MT19937.reset(new std::mt19937(n));
    RG64_MT19937.reset(new std::mt19937_64(n));
}

// ------------------------------------------------------------------------------------------------
void ReseedRandom32()
{
    RG32_MT19937.reset(new std::mt19937(GenerateSeed()));
}

void ReseedRandom32(Uint32 n)
{
    RG32_MT19937.reset(new std::mt19937(n));
}

// ------------------------------------------------------------------------------------------------
void ReseedRandom64()
{
    RG64_MT19937.reset(new std::mt19937_64(GenerateSeed()));
}

void ReseedRandom64(Uint32 n)
{
    RG64_MT19937.reset(new std::mt19937_64(n));
}

// ------------------------------------------------------------------------------------------------
Int8 GetRandomInt8()
{
    return Int8_Dist(*RG32_MT19937);
}

Int8 GetRandomInt8(Int8 n)
{
    return std::uniform_int_distribution< Int8 >(0, n)(*RG32_MT19937);
}

Int8 GetRandomInt8(Int8 m, Int8 n)
{
    return std::uniform_int_distribution< Int8 >(m, n)(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
Uint8 GetRandomUint8()
{
    return Uint8_Dist(*RG32_MT19937);
}

Uint8 GetRandomUint8(Uint8 n)
{
    return std::uniform_int_distribution< Uint8 >(0, n)(*RG32_MT19937);
}

Uint8 GetRandomUint8(Uint8 m, Uint8 n)
{
    return std::uniform_int_distribution< Uint8 >(m, n)(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
Int16 GetRandomInt16()
{
    return Int16_Dist(*RG32_MT19937);
}

Int16 GetRandomInt16(Int16 n)
{
    return std::uniform_int_distribution< Int16 >(0, n)(*RG32_MT19937);
}

Int16 GetRandomInt16(Int16 m, Int16 n)
{
    return std::uniform_int_distribution< Int16 >(m, n)(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
Uint16 GetRandomUint16()
{
    return Uint16_Dist(*RG32_MT19937);
}

Uint16 GetRandomUint16(Uint16 n)
{
    return std::uniform_int_distribution< Uint16 >(0, n)(*RG32_MT19937);
}

Uint16 GetRandomUint16(Uint16 m, Uint16 n)
{
    return std::uniform_int_distribution< Uint16 >(m, n)(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
Int32 GetRandomInt32()
{
    return Int32_Dist(*RG32_MT19937);
}

Int32 GetRandomInt32(Int32 n)
{
    return std::uniform_int_distribution< Int32 >(0, n)(*RG32_MT19937);
}

Int32 GetRandomInt32(Int32 m, Int32 n)
{
    return std::uniform_int_distribution< Int32 >(m, n)(*RG32_MT19937);
}


// ------------------------------------------------------------------------------------------------
Uint32 GetRandomUint32()
{
    return Int32_Dist(*RG32_MT19937);
}

Uint32 GetRandomUint32(Uint32 n)
{
    return std::uniform_int_distribution< Int32 >(0, n)(*RG32_MT19937);
}

Uint32 GetRandomUint32(Uint32 m, Uint32 n)
{
    return std::uniform_int_distribution< Int32 >(m, n)(*RG32_MT19937);
}


// ------------------------------------------------------------------------------------------------
Int64 GetRandomInt64()
{
    return Int64_Dist(*RG64_MT19937);
}

Int64 GetRandomInt64(Int64 n)
{
    return std::uniform_int_distribution< Int64 >(0, n)(*RG64_MT19937);
}

Int64 GetRandomInt64(Int64 m, Int64 n)
{
    return std::uniform_int_distribution< Int64 >(m, n)(*RG64_MT19937);
}


// ------------------------------------------------------------------------------------------------
Uint64 GetRandomUint64()
{
    return Uint64_Dist(*RG64_MT19937);
}

Uint64 GetRandomUint64(Uint64 n)
{
    return std::uniform_int_distribution< Uint64 >(0, n)(*RG64_MT19937);
}

Uint64 GetRandomUint64(Uint64 m, Uint64 n)
{
    return std::uniform_int_distribution< Uint64 >(m, n)(*RG64_MT19937);
}

// ------------------------------------------------------------------------------------------------
Float32 GetRandomFloat32()
{
    return Float32_Dist(*RG32_MT19937);
}

Float32 GetRandomFloat32(Float32 n)
{
    return std::uniform_real_distribution< Float32 >(0, n)(*RG32_MT19937);
}

Float32 GetRandomFloat32(Float32 m, Float32 n)
{
    return std::uniform_real_distribution< Float32 >(m, n)(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
Float64 GetRandomFloat64()
{
    return Float64_Dist(*RG64_MT19937);
}

Float64 GetRandomFloat64(Float64 n)
{
    return std::uniform_real_distribution< Float64 >(0, n)(*RG64_MT19937);
}

Float64 GetRandomFloat64(Float64 m, Float64 n)
{
    return std::uniform_real_distribution< Float64 >(m, n)(*RG64_MT19937);
}

// ------------------------------------------------------------------------------------------------
void GetRandomString(String & str, String::size_type len)
{
    // Reserve the requested size + the null terminator
    str.reserve(len+1);
    // Resize to the requested size and fill with 0
    str.resize(len);
    // Generate the requested amount of characters
    for (auto & c : str)
        c = String_Dist(*RG32_MT19937);
    // Append the null terminator
    str.push_back(0);
}

void GetRandomString(String & str, String::size_type len, String::value_type n)
{
    // Reserve the requested size + the null terminator
    str.reserve(len+1);
    // Resize to the requested size and fill with 0
    str.resize(len);
    // Create the custom distribution
    std::uniform_int_distribution< String::value_type > dist(1, n);
    // Generate the requested amount of characters
    for (auto & c : str)
        c = dist(*RG32_MT19937);
    // Append the null terminator
    str.push_back(0);
}

void GetRandomString(String & str, String::size_type len, String::value_type m, String::value_type n)
{
    // Reserve the requested size + the null terminator
    str.reserve(len+1);
    // Resize to the requested size and fill with 0
    str.resize(len);
    // Create the custom distribution
    std::uniform_int_distribution< String::value_type > dist(m, n);
    // Generate the requested amount of characters
    for (auto & c : str)
        c = dist(*RG32_MT19937);
    // Append the null terminator
    str.push_back(0);
}

// ------------------------------------------------------------------------------------------------
bool GetRandomBool()
{
    return std::bernoulli_distribution()(*RG32_MT19937);
}

bool GetRandomBool(SQFloat p)
{
    return std::bernoulli_distribution(p)(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
static String RandomString(Int32 len)
{
    // Is there anything to generate?
    if (len <= 0)
        return _SC("");
    // Prepare the string buffer
    String str;
    // Request the random fill
    GetRandomString(str, len);
    // Return ownership of the string
    return std::move(str);
}

// ------------------------------------------------------------------------------------------------
static String RandomString(Int32 len, SQChar n)
{
    // Is there anything to generate?
    if (len <= 0)
        return _SC("");
    // Prepare the string buffer
    String str;
    // Request the random fill
    GetRandomString(str, len, n);
    // Return ownership of the string
    return std::move(str);
}

// ------------------------------------------------------------------------------------------------
static String RandomString(Int32 len, SQChar m, SQChar n)
{
    // Is there anything to generate?
    if (len <= 0)
        return _SC("");
    // Prepare the string buffer
    String str;
    // Request the random fill
    GetRandomString(str, len, m, n);
    // Return ownership of the string
    return std::move(str);
}

// ------------------------------------------------------------------------------------------------
void Register_Random(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqRand"), Table(vm)
        .Func(_SC("GenSeed"), &GenerateSeed)
        .Overload< void (*)(void) >(_SC("Reseed"), &ReseedRandom)
        .Overload< void (*)(Uint32) >(_SC("Reseed"), &ReseedRandom)
        .Overload< void (*)(void) >(_SC("Reseed32"), &ReseedRandom32)
        .Overload< void (*)(Uint32) >(_SC("Reseed32"), &ReseedRandom32)
        .Overload< void (*)(void) >(_SC("Reseed64"), &ReseedRandom64)
        .Overload< void (*)(Uint32) >(_SC("Reseed64"), &ReseedRandom64)

#ifdef _SQ64
        .Overload< SQInteger (*)(void) >(_SC("Integer"), &GetRandomInt64)
        .Overload< SQInteger (*)(SQInteger) >(_SC("Integer"), &GetRandomInt64)
        .Overload< SQInteger (*)(SQInteger, SQInteger) >(_SC("Integer"), &GetRandomInt64)
#else
        .Overload< SQInteger (*)(void) >(_SC("Integer"), &GetRandomInt32)
        .Overload< SQInteger (*)(SQInteger) >(_SC("Integer"), &GetRandomInt32)
        .Overload< SQInteger (*)(SQInteger, SQInteger) >(_SC("Integer"), &GetRandomInt32)
#endif // _SQ64

#ifdef SQUSEDOUBLE
        .Overload< SQFloat (*)(void) >(_SC("Float"), &GetRandomFloat64)
        .Overload< SQFloat (*)(SQFloat) >(_SC("Float"), &GetRandomFloat64)
        .Overload< SQFloat (*)(SQFloat, SQFloat) >(_SC("Float"), &GetRandomFloat64)
#else
        .Overload< SQFloat (*)(void) >(_SC("Float"), &GetRandomFloat32)
        .Overload< SQFloat (*)(SQFloat) >(_SC("Float"), &GetRandomFloat32)
        .Overload< SQFloat (*)(SQFloat, SQFloat) >(_SC("Float"), &GetRandomFloat32)
#endif // SQUSEDOUBLE

        .Overload< String (*)(Int32) >(_SC("String"), &RandomString)
        .Overload< String (*)(Int32, SQChar) >(_SC("String"), &RandomString)
        .Overload< String (*)(Int32, SQChar, SQChar) >(_SC("String"), &RandomString)
        .Overload< bool (*)(void) >(_SC("Bool"), &GetRandomBool)
        .Overload< bool (*)(SQFloat) >(_SC("Bool"), &GetRandomBool)
    );
}

} // Namespace:: SqMod
