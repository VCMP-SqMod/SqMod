// ------------------------------------------------------------------------------------------------
#include "Library/Random.hpp"
#include "Base/Shared.hpp"
#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <RandomLib/Random.hpp>

// ------------------------------------------------------------------------------------------------
#include <time.h>
#include <stdlib.h>
#ifdef SQMOD_OS_WINDOWS
    #include <process.h>
#else
    #include <sys/types.h>
    #include <unistd.h>
#endif

// ------------------------------------------------------------------------------------------------
RandomLib::Random g_RGen;

// ------------------------------------------------------------------------------------------------
namespace SqMod {

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
    g_RGen.Reseed();
}

void ReseedRandom(Uint32 n)
{
    g_RGen.Reseed(n);
}

// ------------------------------------------------------------------------------------------------
Int8 GetRandomInt8()
{
    return g_RGen.Integer< Int8 >();
}

Int8 GetRandomInt8(Int8 n)
{
    return g_RGen.Integer< Int8 >(n);
}

Int8 GetRandomInt8(Int8 m, Int8 n)
{
    return g_RGen.IntegerC< Int8 >(m, n);
}

// ------------------------------------------------------------------------------------------------
Uint8 GetRandomUint8()
{
    return g_RGen.Integer< Uint8 >();
}

Uint8 GetRandomUint8(Uint8 n)
{
    return g_RGen.Integer< Uint8 >(n);
}

Uint8 GetRandomUint8(Uint8 m, Uint8 n)
{
    return g_RGen.IntegerC< Uint8 >(m, n);
}


// ------------------------------------------------------------------------------------------------
Int16 GetRandomInt16()
{
    return g_RGen.Integer< Int16 >();
}

Int16 GetRandomInt16(Int16 n)
{
    return g_RGen.Integer< Int16 >(n);
}

Int16 GetRandomInt16(Int16 m, Int16 n)
{
    return g_RGen.IntegerC< Int16 >(m, n);
}


// ------------------------------------------------------------------------------------------------
Uint16 GetRandomUint16()
{
    return g_RGen.Integer< Uint16 >();
}

Uint16 GetRandomUint16(Uint16 n)
{
    return g_RGen.Integer< Uint16 >(n);
}

Uint16 GetRandomUint16(Uint16 m, Uint16 n)
{
    return g_RGen.IntegerC< Uint16 >(m, n);
}


// ------------------------------------------------------------------------------------------------
Int32 GetRandomInt32()
{
    return g_RGen.Integer< Int32 >();
}

Int32 GetRandomInt32(Int32 n)
{
    return g_RGen.Integer< Int32 >(n);
}

Int32 GetRandomInt32(Int32 m, Int32 n)
{
    return g_RGen.IntegerC< Int32 >(m, n);
}


// ------------------------------------------------------------------------------------------------
Uint32 GetRandomUint32()
{
    return g_RGen.Integer< Uint32 >();
}

Uint32 GetRandomUint32(Uint32 n)
{
    return g_RGen.Integer< Uint32 >(n);
}

Uint32 GetRandomUint32(Uint32 m, Uint32 n)
{
    return g_RGen.IntegerC< Uint32 >(m, n);
}


// ------------------------------------------------------------------------------------------------
Int64 GetRandomInt64()
{
    return g_RGen.Integer< Int64 >();
}

Int64 GetRandomInt64(Int64 n)
{
    return g_RGen.Integer< Int64 >(n);
}

Int64 GetRandomInt64(Int64 m, Int64 n)
{
    return g_RGen.IntegerC< Int64 >(m, n);
}


// ------------------------------------------------------------------------------------------------
Uint64 GetRandomUint64()
{
    return g_RGen.Integer< Uint64 >();
}

Uint64 GetRandomUint64(Uint64 n)
{
    return g_RGen.Integer< Uint64 >(n);
}

Uint64 GetRandomUint64(Uint64 m, Uint64 n)
{
    return g_RGen.IntegerC< Uint64 >(m, n);
}

// ------------------------------------------------------------------------------------------------
Float32 GetRandomFloat32()
{
    static const Float32 m = NumLimit<Int32>::Min, n = NumLimit<Int32>::Max;
    return (n - m) * (Float32(g_RGen.IntegerC< Int16 >(0, NumLimit< Int16 >::Max)) / Float32(RAND_MAX)) + m;
}

Float32 GetRandomFloat32(Float32 n)
{
    return Float32(g_RGen.IntegerC< Int16 >(0, NumLimit< Int16 >::Max)) / Float32(RAND_MAX/n);
}

Float32 GetRandomFloat32(Float32 m, Float32 n)
{
    return (n - m) * (Float32(g_RGen.IntegerC< Int16 >(0, NumLimit< Int16 >::Max)) / Float32(RAND_MAX)) + m;
}

// ------------------------------------------------------------------------------------------------
Float64 GetRandomFloat64()
{
    static const Float64 m = NumLimit<Int32>::Min, n = NumLimit<Int32>::Max;
    return (n - m) * (Float64(g_RGen.IntegerC< Int16 >(0, NumLimit< Int16 >::Max)) / Float64(RAND_MAX)) + m;
}

Float64 GetRandomFloat64(Float64 n)
{
    return Float64(g_RGen.IntegerC< Int16 >(0, NumLimit< Int16 >::Max)) / Float64(RAND_MAX/n);
}

Float64 GetRandomFloat64(Float64 m, Float64 n)
{
    return (n - m) * (Float64(g_RGen.IntegerC< Int16 >(0, NumLimit< Int16 >::Max)) / Float64(RAND_MAX)) + m;
}

// ------------------------------------------------------------------------------------------------
void GetRandomString(String & str, String::size_type len)
{
    str.reserve(len+1);
    str.resize(len);
    for (String::iterator itr = str.begin(); itr != str.end(); ++itr)
    {
        *itr = g_RGen.Integer< String::value_type >();
    }
    str.push_back(0);
}

void GetRandomString(String & str, String::size_type len, String::value_type n)
{
    str.reserve(len+1);
    str.resize(len);
    for (String::iterator itr = str.begin(); itr != str.end(); ++itr)
    {
        *itr = g_RGen.Integer< String::value_type >(n);
    }
    str.push_back(0);
}

void GetRandomString(String & str, String::size_type len, String::value_type m, String::value_type n)
{
    str.reserve(len+1);
    str.resize(len);
    for (String::iterator itr = str.begin(); itr != str.end(); ++itr)
    {
        *itr = g_RGen.IntegerC< String::value_type >(m, n);
    }
    str.push_back(0);
}

// ------------------------------------------------------------------------------------------------
bool GetRandomBool()
{
    return g_RGen.Boolean();
}

// ------------------------------------------------------------------------------------------------
template < typename T > static T RandomValue()
{
    return RandomVal< T >::Get();
}

template < typename T > static T RandomValue(T n)
{
    return RandomVal< T >::Get(n);
}

template < typename T > static T RandomValue(T m, T n)
{
    return RandomVal< T >::Get(m, n);
}

// ------------------------------------------------------------------------------------------------
static CSStr RandomString(Int32 len)
{
    if (len <= 0)
    {
        return _SC("");
    }
    Buffer b(len+1);
    SStr s = b.Get< SQChar >();
    for (Int32 n = 0; n <= len; ++n, ++s)
    {
        *s = g_RGen.Integer< SQChar >();
    }
    b.At< SQChar >(len) = 0;
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static CSStr RandomString(Int32 len, SQChar n)
{
    if (len <= 0)
    {
        return _SC("");
    }
    Buffer b(len+1);
    SStr s = b.Get< SQChar >();
    for (Int32 n = 0; n <= len; ++n, ++s)
    {
        *s = g_RGen.Integer< SQChar >(n);
    }
    b.At< SQChar >(len) = 0;
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static CSStr RandomString(Int32 len, SQChar m, SQChar n)
{
    if (len <= 0)
    {
        return _SC("");
    }
    Buffer b(len+1);
    SStr s = b.Get< SQChar >();
    for (Int32 n = 0; n <= len; ++n, ++s)
    {
        *s = g_RGen.IntegerC< SQChar >(m, n);
    }
    b.At< SQChar >(len) = 0;
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static bool RandomProbI(SQInteger p)
{
    return g_RGen.Prob(p);
}

static bool RandomProbI(SQInteger m, SQInteger n)
{
    return g_RGen.Prob(m, n);
}

// ------------------------------------------------------------------------------------------------
static bool RandomProbF(SQFloat p)
{
    return g_RGen.Prob(p);
}

static bool RandomProbF(SQFloat m, SQFloat n)
{
    return g_RGen.Prob(m, n);
}

// ------------------------------------------------------------------------------------------------
void Register_Random(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("Random"), Table(vm)
        .Func(_SC("GenSeed"), &GenerateSeed)
        .Overload< void (*)(void) >(_SC("Reseed"), &ReseedRandom)
        .Overload< void (*)(Uint32) >(_SC("Reseed"), &ReseedRandom)
        .Overload< SQInteger (*)(void) >(_SC("Integer"), &RandomValue< SQInteger >)
        .Overload< SQInteger (*)(SQInteger) >(_SC("Integer"), &RandomValue< SQInteger >)
        .Overload< SQInteger (*)(SQInteger, SQInteger) >(_SC("Integer"), &RandomValue< SQInteger >)
        .Overload< SQFloat (*)(void) >(_SC("Float"), &RandomValue< SQFloat >)
        .Overload< SQFloat (*)(SQFloat) >(_SC("Float"), &RandomValue< SQFloat >)
        .Overload< SQFloat (*)(SQFloat, SQFloat) >(_SC("Float"), &RandomValue< SQFloat >)
        .Overload< CSStr (*)(Int32) >(_SC("String"), &RandomString)
        .Overload< CSStr (*)(Int32, SQChar) >(_SC("String"), &RandomString)
        .Overload< CSStr (*)(Int32, SQChar, SQChar) >(_SC("String"), &RandomString)
        .Func(_SC("Bool"), &GetRandomBool)
        .Overload< bool (*)(SQInteger) >(_SC("ProbI"), &RandomProbI)
        .Overload< bool (*)(SQInteger, SQInteger) >(_SC("ProbI"), &RandomProbI)
        .Overload< bool (*)(SQFloat) >(_SC("ProbF"), &RandomProbF)
        .Overload< bool (*)(SQFloat, SQFloat) >(_SC("ProbF"), &RandomProbF)
    );
}

} // Namespace:: SqMod
