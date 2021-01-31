// ------------------------------------------------------------------------------------------------
#include "Library/Numeric/Random.hpp"
#include "Core/Common.hpp"
#include "Core/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <ctime>
#include <memory>
#include <random>

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
static std::unique_ptr< std::mt19937 >          RG32_MT19937 = // NOLINT(cert-err58-cpp)
                            std::make_unique< std::mt19937 >(GenerateSeed());

static std::unique_ptr< std::mt19937_64 >       RG64_MT19937 = // NOLINT(cert-err58-cpp)
                            std::make_unique< std::mt19937_64 >(GenerateSeed());

// ------------------------------------------------------------------------------------------------
static std::uniform_int_distribution< int8_t >    Int8_Dist(std::numeric_limits< int8_t >::min(), // NOLINT(cert-err58-cpp)
                                                            std::numeric_limits< int8_t >::max());
static std::uniform_int_distribution< uint8_t >   uint8_t_Dist(std::numeric_limits< uint8_t >::min(), // NOLINT(cert-err58-cpp)
                                                            std::numeric_limits< uint8_t >::max());

static std::uniform_int_distribution< int16_t >   Int16_Dist(std::numeric_limits< int16_t >::min(), // NOLINT(cert-err58-cpp)
                                                            std::numeric_limits< int16_t >::max());
static std::uniform_int_distribution< uint16_t >  Uint16_Dist(std::numeric_limits< uint16_t >::min(), // NOLINT(cert-err58-cpp)
                                                            std::numeric_limits< uint16_t >::max());

static std::uniform_int_distribution< int32_t >   Int32_Dist(std::numeric_limits< int32_t >::min(), // NOLINT(cert-err58-cpp)
                                                            std::numeric_limits< int32_t >::max());
static std::uniform_int_distribution< uint32_t >  Uint32_Dist(std::numeric_limits< uint32_t >::min(), // NOLINT(cert-err58-cpp)
                                                            std::numeric_limits< uint32_t >::max());

static std::uniform_int_distribution< int64_t >   Int64_Dist(std::numeric_limits< int64_t >::min(), // NOLINT(cert-err58-cpp)
                                                            std::numeric_limits< int64_t >::max());
static std::uniform_int_distribution< uint64_t >  Uint64_Dist(std::numeric_limits< uint64_t >::min(), // NOLINT(cert-err58-cpp)
                                                            std::numeric_limits< uint64_t >::max());

static std::uniform_real_distribution<float>  Float32_Dist(std::numeric_limits< float >::min(), // NOLINT(cert-err58-cpp)
                                                            std::numeric_limits< float >::max());
static std::uniform_real_distribution<double>  Float64_Dist(std::numeric_limits< double >::min(), // NOLINT(cert-err58-cpp)
                                                            std::numeric_limits< double >::max());

// ------------------------------------------------------------------------------------------------
static std::uniform_int_distribution< String::value_type >
                                    String_Dist(std::numeric_limits< String::value_type >::min(), // NOLINT(cert-err58-cpp)
                                                std::numeric_limits< String::value_type >::max());

// ------------------------------------------------------------------------------------------------
uint32_t GenerateSeed()
{
    unsigned long a = clock();
    unsigned long b = time(nullptr);
#ifdef SQMOD_OS_WINDOWS
    unsigned long c = _getpid();
#else
    unsigned long c = getpid();
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
size_t GenerateSeed2()
{
    struct { // NOLINT(cppcoreguidelines-pro-type-member-init)
        std::clock_t    c;
        std::time_t     t;
#ifdef SQMOD_OS_WINDOWS
        int             p;
#else
        pid_t           p;
#endif
    } data;
    data.c = std::clock();
    data.t = std::time(nullptr);
#ifdef SQMOD_OS_WINDOWS
    data.p = _getpid();
#else
    data.p = getpid();
#endif
    // Mangle and return result
    return FnvHash(reinterpret_cast< const uint8_t * >(&data), sizeof(data));
}

// ------------------------------------------------------------------------------------------------
void ReseedRandom()
{
    RG32_MT19937 = std::make_unique<std::mt19937>(GenerateSeed());
    RG64_MT19937 = std::make_unique<std::mt19937_64>(GenerateSeed());
}

void ReseedRandom(uint32_t n)
{
    RG32_MT19937 = std::make_unique<std::mt19937>(n);
    RG64_MT19937 = std::make_unique<std::mt19937_64>(n);
}

// ------------------------------------------------------------------------------------------------
void ReseedRandom32()
{
    RG32_MT19937 = std::make_unique<std::mt19937>(GenerateSeed());
}

void ReseedRandom32(uint32_t n)
{
    RG32_MT19937 = std::make_unique<std::mt19937>(n);
}

// ------------------------------------------------------------------------------------------------
void ReseedRandom64()
{
    RG64_MT19937 = std::make_unique<std::mt19937_64>(GenerateSeed());
}

void ReseedRandom64(uint32_t n)
{
    RG64_MT19937 = std::make_unique<std::mt19937_64>(n);
}

// ------------------------------------------------------------------------------------------------
int8_t GetRandomInt8()
{
    return Int8_Dist(*RG32_MT19937);
}

int8_t GetRandomInt8(int8_t n)
{
    return std::uniform_int_distribution< int8_t >(0, n)(*RG32_MT19937);
}

int8_t GetRandomInt8(int8_t m, int8_t n)
{
    return std::uniform_int_distribution< int8_t >(m, n)(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
uint8_t GetRandomUint8()
{
    return uint8_t_Dist(*RG32_MT19937);
}

uint8_t GetRandomUint8(uint8_t n)
{
    return std::uniform_int_distribution< uint8_t >(0, n)(*RG32_MT19937);
}

uint8_t GetRandomUint8(uint8_t m, uint8_t n)
{
    return std::uniform_int_distribution< uint8_t >(m, n)(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
int16_t GetRandomInt16()
{
    return Int16_Dist(*RG32_MT19937);
}

int16_t GetRandomInt16(int16_t n)
{
    return std::uniform_int_distribution< int16_t >(0, n)(*RG32_MT19937);
}

int16_t GetRandomInt16(int16_t m, int16_t n)
{
    return std::uniform_int_distribution< int16_t >(m, n)(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
uint16_t GetRandomUint16()
{
    return Uint16_Dist(*RG32_MT19937);
}

uint16_t GetRandomUint16(uint16_t n)
{
    return std::uniform_int_distribution< uint16_t >(0, n)(*RG32_MT19937);
}

uint16_t GetRandomUint16(uint16_t m, uint16_t n)
{
    return std::uniform_int_distribution< uint16_t >(m, n)(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
int32_t GetRandomInt32()
{
    return Int32_Dist(*RG32_MT19937);
}

int32_t GetRandomInt32(int32_t n)
{
    return std::uniform_int_distribution< int32_t >(0, n)(*RG32_MT19937);
}

int32_t GetRandomInt32(int32_t m, int32_t n)
{
    return std::uniform_int_distribution< int32_t >(m, n)(*RG32_MT19937);
}


// ------------------------------------------------------------------------------------------------
uint32_t GetRandomUint32()
{
    return Uint32_Dist(*RG32_MT19937);
}

uint32_t GetRandomUint32(uint32_t n)
{
    return std::uniform_int_distribution< uint32_t >(0, n)(*RG32_MT19937);
}

uint32_t GetRandomUint32(uint32_t m, uint32_t n)
{
    return std::uniform_int_distribution< uint32_t >(m, n)(*RG32_MT19937);
}


// ------------------------------------------------------------------------------------------------
int64_t GetRandomInt64()
{
    return Int64_Dist(*RG64_MT19937);
}

int64_t GetRandomInt64(int64_t n)
{
    return std::uniform_int_distribution< int64_t >(0, n)(*RG64_MT19937);
}

int64_t GetRandomInt64(int64_t m, int64_t n)
{
    return std::uniform_int_distribution< int64_t >(m, n)(*RG64_MT19937);
}


// ------------------------------------------------------------------------------------------------
uint64_t GetRandomUint64()
{
    return Uint64_Dist(*RG64_MT19937);
}

uint64_t GetRandomUint64(uint64_t n)
{
    return std::uniform_int_distribution< uint64_t >(0, n)(*RG64_MT19937);
}

uint64_t GetRandomUint64(uint64_t m, uint64_t n)
{
    return std::uniform_int_distribution< uint64_t >(m, n)(*RG64_MT19937);
}

// ------------------------------------------------------------------------------------------------
float GetRandomFloat32()
{
    return Float32_Dist(*RG32_MT19937);
}

float GetRandomFloat32(float n)
{
    return std::uniform_real_distribution< float >(0, n)(*RG32_MT19937);
}

float GetRandomFloat32(float m, float n)
{
    return std::uniform_real_distribution< float >(m, n)(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
double GetRandomFloat64()
{
    return Float64_Dist(*RG64_MT19937);
}

double GetRandomFloat64(double n)
{
    return std::uniform_real_distribution< double >(0, n)(*RG64_MT19937);
}

double GetRandomFloat64(double m, double n)
{
    return std::uniform_real_distribution< double >(m, n)(*RG64_MT19937);
}

// ------------------------------------------------------------------------------------------------
void GetRandomString(String & str, String::size_type len)
{
    // Resize to the requested size and fill with 0
    str.resize(len);
    // Generate the requested amount of characters
    for (auto & c : str)
    {
        c = String_Dist(*RG32_MT19937);
    }
}

void GetRandomString(String & str, String::size_type len, String::value_type n)
{
    // Resize to the requested size and fill with 0
    str.resize(len);
    // Create the custom distribution
    std::uniform_int_distribution< String::value_type > dist(1, n);
    // Generate the requested amount of characters
    for (auto & c : str)
    {
        c = dist(*RG32_MT19937);
    }
}

void GetRandomString(String & str, String::size_type len, String::value_type m, String::value_type n)
{
    // Resize to the requested size and fill with 0
    str.resize(len);
    // Create the custom distribution
    std::uniform_int_distribution< String::value_type > dist(m, n);
    // Generate the requested amount of characters
    for (auto & c : str)
    {
        c = dist(*RG32_MT19937);
    }
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
#ifdef _SQ64
SQMOD_NODISCARD SQInteger RandomInt() { return GetRandomInt64(); }
SQMOD_NODISCARD SQInteger RandomIntUpto(SQInteger n) { return GetRandomInt64(n); }
SQMOD_NODISCARD SQInteger RandomIntBetween(SQInteger m, SQInteger n) { return GetRandomInt64(m, n); }
#else
SQMOD_NODISCARD SQInteger RandomInt() { return GetRandomInt32(); }
SQMOD_NODISCARD SQInteger RandomIntUpto(SQInteger n) { return GetRandomInt32(n); }
SQMOD_NODISCARD SQInteger RandomIntBetween(SQInteger m, SQInteger n) { return GetRandomInt32(m, n); }
#endif // _SQ64

// ------------------------------------------------------------------------------------------------
#ifdef SQUSEDOUBLE
SQMOD_NODISCARD SQFloat RandomFloat() { return GetRandomFloat32(); }
SQMOD_NODISCARD SQFloat RandomFloatUpto(float n) { return GetRandomFloat32(n); }
SQMOD_NODISCARD SQFloat RandomFloatBetween(float m, float n) { return GetRandomFloat32(m, n); }
#else
SQMOD_NODISCARD SQFloat RandomFloat() { return GetRandomFloat64(); }
SQMOD_NODISCARD SQFloat RandomFloatUpto(double n) { return GetRandomFloat64(n); }
SQMOD_NODISCARD SQFloat RandomFloatBetween(double m, double n) { return GetRandomFloat64(m, n); }
#endif // SQUSEDOUBLE

// ------------------------------------------------------------------------------------------------
bool RandomBool()
{
    return std::bernoulli_distribution()(*RG32_MT19937);
}

bool RandomBoolProb(SQFloat p)
{
    return std::bernoulli_distribution(p)(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
static String RandomString(int32_t len)
{
    // Is there anything to generate?
    if (len <= 0)
        return _SC("");
    // Prepare the string buffer
    String str;
    // Request the random fill
    GetRandomString(str, len);
    // Return ownership of the string
    return str;
}

// ------------------------------------------------------------------------------------------------
static String RandomStringUpto(int32_t len, SQChar n)
{
    // Is there anything to generate?
    if (len <= 0)
        return _SC("");
    // Prepare the string buffer
    String str;
    // Request the random fill
    GetRandomString(str, len, n);
    // Return ownership of the string
    return str;
}

// ------------------------------------------------------------------------------------------------
static String RandomStringBetween(int32_t len, SQChar m, SQChar n)
{
    // Is there anything to generate?
    if (len <= 0)
        return _SC("");
    // Prepare the string buffer
    String str;
    // Request the random fill
    GetRandomString(str, len, m, n);
    // Return ownership of the string
    return str;
}

// ------------------------------------------------------------------------------------------------
void Register_Random(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqRand"), Table(vm)
        .Func(_SC("GenSeed"), &GenerateSeed)
        .Func(_SC("GenSeed2"), &GenerateSeed2)
        .Overload< void (*)(void) >(_SC("Reseed"), &ReseedRandom)
        .Overload< void (*)(uint32_t) >(_SC("Reseed"), &ReseedRandom)
        .Overload< void (*)(void) >(_SC("Reseed32"), &ReseedRandom32)
        .Overload< void (*)(uint32_t) >(_SC("Reseed32"), &ReseedRandom32)
        .Overload< void (*)(void) >(_SC("Reseed64"), &ReseedRandom64)
        .Overload< void (*)(uint32_t) >(_SC("Reseed64"), &ReseedRandom64)
        .Overload(_SC("Integer"), &RandomInt)
        .Overload(_SC("Integer"), &RandomIntUpto)
        .Overload(_SC("Integer"), &RandomIntBetween)
        .Overload(_SC("Float"), &RandomFloat)
        .Overload(_SC("Float"), &RandomFloatUpto)
        .Overload(_SC("Float"), &RandomFloatBetween)
        .Overload(_SC("String"), &RandomString)
        .Overload(_SC("String"), &RandomStringUpto)
        .Overload(_SC("String"), &RandomStringBetween)
        .Overload(_SC("Bool"), &RandomBool)
        .Overload(_SC("Bool"), &RandomBoolProb)
    );
}

} // Namespace:: SqMod
