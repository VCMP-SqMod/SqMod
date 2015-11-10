#include "Base/Shared.hpp"
#include "Register.hpp"
#include "Logger.hpp"
#include "Debug.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include "Base/AABB.hpp"
#include "Base/Circle.hpp"
#include "Base/Color3.hpp"
#include "Base/Color4.hpp"
#include "Base/Quaternion.hpp"
#include "Base/Sphere.hpp"
#include "Base/Vector2i.hpp"
#include "Base/Vector2u.hpp"
#include "Base/Vector2f.hpp"
#include "Base/Vector3.hpp"
#include "Base/Vector4.hpp"

// ------------------------------------------------------------------------------------------------
#include <ctime>
#include <memory>
#include <random>
#include <cstdarg>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static std::unique_ptr<std::mt19937>       RG32_MT19937 = std::unique_ptr<std::mt19937>( \
                                            new std::mt19937(_SCU32(std::time(0))));

static std::unique_ptr<std::mt19937_64>    RG64_MT19937 = std::unique_ptr<std::mt19937_64>( \
                                            new std::mt19937_64(_SCU32(std::time(0))));

// ------------------------------------------------------------------------------------------------
static std::uniform_int_distribution<Int8>      Int8_Dist(std::numeric_limits<Int8>::min(), std::numeric_limits<Int8>::max());
static std::uniform_int_distribution<Uint8>     UInt8_Dist(std::numeric_limits<Uint8>::min(), std::numeric_limits<Uint8>::max());

static std::uniform_int_distribution<Int16>     Int16_Dist(std::numeric_limits<Int16>::min(), std::numeric_limits<Int16>::max());
static std::uniform_int_distribution<Uint16>    UInt16_Dist(std::numeric_limits<Uint16>::min(), std::numeric_limits<Uint16>::max());

static std::uniform_int_distribution<Int32>     Int32_Dist(std::numeric_limits<Int32>::min(), std::numeric_limits<Int32>::max());
static std::uniform_int_distribution<Uint32>    UInt32_Dist(std::numeric_limits<Uint32>::min(), std::numeric_limits<Uint32>::max());

static std::uniform_int_distribution<Int64>     Int64_Dist(std::numeric_limits<Int64>::min(), std::numeric_limits<Int64>::max());
static std::uniform_int_distribution<Uint64>    UInt64_Dist(std::numeric_limits<Uint64>::min(), std::numeric_limits<Uint64>::max());

static std::uniform_real_distribution<Float32>  Float32_Dist(std::numeric_limits<Float32>::min(), std::numeric_limits<Float32>::max());
static std::uniform_real_distribution<Float64>  Float64_Dist(std::numeric_limits<Float64>::min(), std::numeric_limits<Float64>::max());

// ------------------------------------------------------------------------------------------------
static std::uniform_int_distribution<String::size_type> String_Dist(std::numeric_limits<String::value_type>::min(), std::numeric_limits<String::value_type>::max());

// ------------------------------------------------------------------------------------------------
static const std::vector<Color3> SQ_Color_List
{
    Color3(240, 248, 255),
    Color3(250, 235, 215),
    Color3(0, 255, 255),
    Color3(127, 255, 212),
    Color3(240, 255, 255),
    Color3(245, 245, 220),
    Color3(255, 228, 196),
    Color3(0, 0, 0),
    Color3(255, 235, 205),
    Color3(0, 0, 255),
    Color3(138, 43, 226),
    Color3(165, 42, 42),
    Color3(222, 184, 135),
    Color3(95, 158, 160),
    Color3(127, 255, 0),
    Color3(210, 105, 30),
    Color3(255, 127, 80),
    Color3(100, 149, 237),
    Color3(255, 248, 220),
    Color3(220, 20, 60),
    Color3(0, 255, 255),
    Color3(0, 0, 139),
    Color3(0, 139, 139),
    Color3(184, 134, 11),
    Color3(169, 169, 169),
    Color3(0, 100, 0),
    Color3(189, 183, 107),
    Color3(139, 0, 139),
    Color3(85, 107, 47),
    Color3(255, 140, 0),
    Color3(153, 50, 204),
    Color3(139, 0, 0),
    Color3(233, 150, 122),
    Color3(143, 188, 143),
    Color3(72, 61, 139),
    Color3(47, 79, 79),
    Color3(0, 206, 209),
    Color3(148, 0, 211),
    Color3(255, 20, 147),
    Color3(0, 191, 255),
    Color3(105, 105, 105),
    Color3(30, 144, 255),
    Color3(178, 34, 34),
    Color3(255, 250, 240),
    Color3(34, 139, 34),
    Color3(255, 0, 255),
    Color3(220, 220, 220),
    Color3(248, 248, 255),
    Color3(255, 215, 0),
    Color3(218, 165, 32),
    Color3(128, 128, 128),
    Color3(0, 128, 0),
    Color3(173, 255, 47),
    Color3(240, 255, 240),
    Color3(255, 105, 180),
    Color3(205, 92, 92),
    Color3(75, 0, 130),
    Color3(255, 255, 240),
    Color3(240, 230, 140),
    Color3(230, 230, 250),
    Color3(255, 240, 245),
    Color3(124, 252, 0),
    Color3(255, 250, 205),
    Color3(173, 216, 230),
    Color3(240, 128, 128),
    Color3(224, 255, 255),
    Color3(250, 250, 210),
    Color3(211, 211, 211),
    Color3(144, 238, 144),
    Color3(255, 182, 193),
    Color3(255, 160, 122),
    Color3(32, 178, 170),
    Color3(135, 206, 250),
    Color3(119, 136, 153),
    Color3(176, 196, 222),
    Color3(255, 255, 224),
    Color3(0, 255, 0),
    Color3(50, 205, 50),
    Color3(250, 240, 230),
    Color3(255, 0, 255),
    Color3(128, 0, 0),
    Color3(102, 205, 170),
    Color3(0, 0, 205),
    Color3(186, 85, 211),
    Color3(147, 112, 219),
    Color3(60, 179, 113),
    Color3(123, 104, 238),
    Color3(0, 250, 154),
    Color3(72, 209, 204),
    Color3(199, 21, 133),
    Color3(25, 25, 112),
    Color3(245, 255, 250),
    Color3(255, 228, 225),
    Color3(255, 228, 181),
    Color3(255, 222, 173),
    Color3(0, 0, 128),
    Color3(253, 245, 230),
    Color3(128, 128, 0),
    Color3(107, 142, 35),
    Color3(255, 165, 0),
    Color3(255, 69, 0),
    Color3(218, 112, 214),
    Color3(238, 232, 170),
    Color3(152, 251, 152),
    Color3(175, 238, 238),
    Color3(219, 112, 147),
    Color3(255, 239, 213),
    Color3(255, 218, 185),
    Color3(205, 133, 63),
    Color3(255, 192, 203),
    Color3(221, 160, 221),
    Color3(176, 224, 230),
    Color3(128, 0, 128),
    Color3(255, 0, 0),
    Color3(188, 143, 143),
    Color3(65, 105, 225),
    Color3(139, 69, 19),
    Color3(250, 128, 114),
    Color3(244, 164, 96),
    Color3(46, 139, 87),
    Color3(255, 245, 238),
    Color3(160, 82, 45),
    Color3(192, 192, 192),
    Color3(135, 206, 235),
    Color3(106, 90, 205),
    Color3(112, 128, 144),
    Color3(255, 250, 250),
    Color3(0, 255, 127),
    Color3(70, 130, 180),
    Color3(210, 180, 140),
    Color3(0, 128, 128),
    Color3(216, 191, 216),
    Color3(255, 99, 71),
    Color3(64, 224, 208),
    Color3(238, 130, 238),
    Color3(245, 222, 179),
    Color3(255, 255, 255),
    Color3(245, 245, 245),
    Color3(255, 255, 0),
    Color3(154, 205, 50)
};

// ------------------------------------------------------------------------------------------------
void LogDbg(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Log->Send(Logger::LEVEL_DBG, false, fmt, args);
    va_end(args);
}

void LogMsg(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Log->Send(Logger::LEVEL_MSG, false, fmt, args);
    va_end(args);
}

void LogScs(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Log->Send(Logger::LEVEL_SCS, false, fmt, args);
    va_end(args);
}

void LogInf(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Log->Send(Logger::LEVEL_INF, false, fmt, args);
    va_end(args);
}

void LogWrn(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Log->Send(Logger::LEVEL_WRN, false, fmt, args);
    va_end(args);
}

void LogErr(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Log->Send(Logger::LEVEL_ERR, false, fmt, args);
    va_end(args);
}

void LogFtl(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Log->Send(Logger::LEVEL_FTL, false, fmt, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void DbgWrn(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Dbg->Wrn(NULL, NULL, fmt, args);
    va_end(args);
}

void DbgErr(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Dbg->Wrn(NULL, NULL, fmt, args);
    va_end(args);
}

void DbgFtl(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Dbg->Wrn(NULL, NULL, fmt, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void DbgWrn(const char * func, const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Dbg->Wrn(NULL, func, fmt, args);
    va_end(args);
}

void DbgErr(const char * func, const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Dbg->Wrn(NULL, func, fmt, args);
    va_end(args);
}

void DbgFtl(const char * func, const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Dbg->Wrn(NULL, func, fmt, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void DbgWrn(const char * type, const char * func, const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Dbg->Wrn(type, func, fmt, args);
    va_end(args);
}

void DbgErr(const char * type, const char * func, const char * fmt, ...)
{
    _Dbg->SetInf(type, func);
    va_list args;
    va_start(args, fmt);
    _Dbg->Wrn(type, func, fmt, args);
    va_end(args);
}

void DbgFtl(const char * type, const char * func, const char * fmt, ...)
{
    _Dbg->SetInf(type, func);
    va_list args;
    va_start(args, fmt);
    _Dbg->Wrn(type, func, fmt, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
const SQChar * ToStringF(const char * fmt, ...)
{
    // Acquire a buffer from the buffer pool
    Buffer vbuf = _Core->PullBuffer(128);
    // Get direct access to the buffer data
    Buffer::Pointer buf = vbuf.Data();
    // Variable arguments structure
    va_list args;
    // Get the specified arguments
    va_start (args, fmt);
    // Run the specified format
    int ret =  std::vsnprintf(buf, vbuf.Size() * sizeof(Buffer::Value), fmt, args);
    // Check for buffer overflows
    if (_SCU32(ret) >= vbuf.Size())
    {
        // Scale buffer
        vbuf.Reserve(ret);
        // Run the specified format
        ret =  std::vsnprintf(buf, vbuf.Size() * sizeof(Buffer::Value), fmt, args);
    }
    // Check for formatting errors
    if (ret < 0)
    {
        // Throw error
        LogErr("Failed to run the specified string format");
        // Return an empty string
        buf[0] = 0;
    }
    // Return the buffer back to the buffer pool
    _Core->PushBuffer(std::move(vbuf));
    // The buffer data wasn't destroyed and Squirrel should have time to copy it
    return buf;
}

// ------------------------------------------------------------------------------------------------
const SQChar * InsertStr(const SQChar * f, const std::vector< const SQChar * > & a)
{
    // Acquire a buffer from the buffer pool
    Buffer vbuf = _Core->PullBuffer(128);
    // Get direct access to the buffer data
    Buffer::Pointer buf = vbuf.Data();
    // Get the size of the buffer
    const Buffer::SzType sz = vbuf.Size();
    // Size of the resulted string and the number of specified arguments
    unsigned n = 0, s = a.size();
    // See if the format string is valid
    if (f)
    {
        // Temporary variables used for processing characters
        SQChar c = 0, p = 0;
        // Process each character in the format string
        while ((c = *(f++)) && (n < sz))
        {
            // Peek at the next character
            p = *f;
            // Check for argument character
            if (c != '%')
            {
                // Add this character to the buffer
                buf[n++] = c;
            }
            // See if the argument character is escaped
            else if (p == '%')
            {
                // Skip the escaping character
                ++f;
                // Add this character to the buffer
                buf[n++] = '%';
            }
            // See if there's a number to extract
            else if (p >= 48 && p <= 57)
            {
                // Attempt to extract a numeric value
                long int i = strtol(f, nullptr, 10);
                // Skip until the first non digit character
                while (*f >= 48 && *f <= 57)
                {
                    ++f;
                }
                // Force the index to be in range
                i = i <= 0 ? 0 : (i >= (long int)s ? s : i);
                // Don't even bother if the argument isn't valid
                if (a[i])
                {
                    // Get the specified argument
                    const SQChar * v = a[i];
                    // Add the specified argument to the buffer
                    while ((c = *(v++)) && (n < sz))
                    {
                        buf[n++] = c;
                    }
                }
            }
            // Fallback to regular replication
            else
            {
                // Add this character to the buffer
                buf[n++] = c;
            }
        }

    }
    // Insert the null character
    buf[n] = '\0';
    // Return the buffer back to the buffer pool
    _Core->PushBuffer(std::move(vbuf));
    // The buffer data wasn't destroyed and Squirrel should have time to copy it
    return buf;
}

// Utility for the <InsertStr> function
const SQChar * InsStr(const SQChar * f)
{
    return InsertStr(f, std::vector< const SQChar * >());
}

// ------------------------------------------------------------------------------------------------
const SQChar * LeftStr(const SQChar * t, SQChar f, SQUint32 w)
{
    // Acquire a buffer from the buffer pool
    Buffer vbuf = _Core->PullBuffer(w);
    // Get direct access to the buffer data
    Buffer::Pointer buf = vbuf.Data();
    // Get the length of the string
    SQUint32 n = strlen(t);
    // Fill the buffer with the specified fill character
    memset(buf, f, w * sizeof(Buffer::Value));
    // Is the width in bounds?
    if (w >= vbuf.Size())
    {
        LogWrn("Invalid width specified: %d > %d", w, vbuf.Size());
        // Invalidate the width
        w = 0;
    }
    // Is the string empty?
    else if (n == 0)
    {
        LogWrn("Invalid string length: %d < 0", n);
    }
    // Is the string in bounds?
    else if (n > w)
    {
        LogWrn("String is out of bounds: %d > %d", n, w);
    }
    // Insert the string
    else
    {
        strncpy(buf, t, n);
    }
    // Add the terminating character
    buf[w] = '\0';
    // Return the buffer back to the buffer pool
    _Core->PushBuffer(std::move(vbuf));
    // The buffer data wasn't destroyed and Squirrel should have time to copy it
    return buf;
}

const SQChar * LeftStr(const SQChar * t, SQChar f, SQUint32 w, SQUint32 o)
{
    // Acquire a buffer from the buffer pool
    Buffer vbuf = _Core->PullBuffer(w);
    // Get direct access to the buffer data
    Buffer::Pointer buf = vbuf.Data();
    // Get the length of the string
    SQUint32 n = strlen(t);
    // Fill the buffer with the specified fill character
    memset(buf, f, w * sizeof(Buffer::Value));
    // Is the width in bounds?
    if (w >= vbuf.Size())
    {
        LogWrn("Invalid width specified: %d > %d", w, vbuf.Size());
        // Invalidate the width
        w = 0;
    }
    // Is the string empty?
    else if (n == 0)
    {
        LogWrn("Invalid string length: %d < 0", n);
    }
    // Is the string in bounds?
    else if ((o+n) > w)
    {
        LogWrn("String is out of bounds: (%d+%d) > %d", o, n, w);
    }
    // Insert the string
    else
    {
        strncpy(buf + o, t, n);
    }
    // Add the terminating character
    buf[w] = '\0';
    // Return the buffer back to the buffer pool
    _Core->PushBuffer(std::move(vbuf));
    // The buffer data wasn't destroyed and Squirrel should have time to copy it
    return buf;
}

// ------------------------------------------------------------------------------------------------
const SQChar * RightStr(const SQChar * t, SQChar f, SQUint32 w)
{
    // Acquire a buffer from the buffer pool
    Buffer vbuf = _Core->PullBuffer(w);
    // Get direct access to the buffer data
    Buffer::Pointer buf = vbuf.Data();
    // Get the length of the string
    SQUint32 n = strlen(t);
    // Fill the buffer with the specified fill character
    memset(buf, f, w * sizeof(Buffer::Value));
    // Is the width in bounds?
    if (w >= vbuf.Size())
    {
        LogWrn("Invalid width specified: %d > %d", w, vbuf.Size());
        // Invalidate the width
        w = 0;
    }
    // Is the string empty?
    else if (n == 0)
    {
        LogWrn("Invalid string length: %d < 0", n);
    }
    // Is the string in bounds?
    else if (n > w)
    {
        LogWrn("String is out of bounds: %d > %d", n, w);
    }
    // Insert the string
    else
    {
        strncpy(buf + (w-n), t, n);
    }
    // Add the terminating character
    buf[w] = '\0';
    // Return the buffer back to the buffer pool
    _Core->PushBuffer(std::move(vbuf));
    // The buffer data wasn't destroyed and Squirrel should have time to copy it
    return buf;
}

const SQChar * RightStr(const SQChar * t, SQChar f, SQUint32 w, SQUint32 o)
{
    // Acquire a buffer from the buffer pool
    Buffer vbuf = _Core->PullBuffer(w);
    // Get direct access to the buffer data
    Buffer::Pointer buf = vbuf.Data();
    // Get the length of the string
    SQUint32 n = strlen(t);
    // Fill the buffer with the specified fill character
    memset(buf, f, w * sizeof(Buffer::Value));
    // Is the width in bounds?
    if (w >= vbuf.Size())
    {
        LogWrn("Invalid width specified: %d > %d", w, vbuf.Size());
        // Invalidate the width
        w = 0;
    }
    // Is the string empty?
    else if (n == 0)
    {
        LogWrn("Invalid string length: %d < 0", n);
    }
    // Is the string in bounds?
    else if ((n+o) > w)
    {
        LogWrn("String is out of bounds: (%d+%d) > %d", n, o, w);
    }
    // Insert the string
    else
    {
        strncpy(buf + ((w-n)-o), t, n);
    }
    // Add the terminating character
    buf[w] = '\0';
    // Return the buffer back to the buffer pool
    _Core->PushBuffer(std::move(vbuf));
    // The buffer data wasn't destroyed and Squirrel should have time to copy it
    return buf;
}

// ------------------------------------------------------------------------------------------------
const SQChar * CenterStr(const SQChar * t, SQChar f, SQUint32 w)
{
    // Acquire a buffer from the buffer pool
    Buffer vbuf = _Core->PullBuffer(w);
    // Get direct access to the buffer data
    Buffer::Pointer buf = vbuf.Data();
    // Get the length of the string
    SQUint32 n = strlen(t);
    // Fill the buffer with the specified fill character
    memset(buf, f, w * sizeof(Buffer::Value));
    // Is the width in bounds?
    if (w >= vbuf.Size())
    {
        LogWrn("Invalid width specified: %d > %d", w, vbuf.Size());
        // Invalidate the width
        w = 0;
    }
    // Is the string empty?
    else if (n == 0)
    {
        LogWrn("Invalid string length: %d < 0", n);
    }
    // Is the string in bounds?
    else if (n > w)
    {
        LogWrn("String is out of bounds: %d > %d", n, w);
    }
    // Insert the string
    else
    {
        strncpy(buf + (w/2 - n/2), t, n);
    }
    // Add the terminating character
    buf[w] = '\0';
    // Return the buffer back to the buffer pool
    _Core->PushBuffer(std::move(vbuf));
    // The buffer data wasn't destroyed and Squirrel should have time to copy it
    return buf;
}

// ------------------------------------------------------------------------------------------------
void InitMTRG32()
{
    RG32_MT19937.reset(new std::mt19937(_SCU32(std::time(0))));
}

// ------------------------------------------------------------------------------------------------
void InitMTRG64()
{
    RG64_MT19937.reset(new std::mt19937_64(_SCU32(std::time(0))));
}

// ------------------------------------------------------------------------------------------------
Int8 GetRandomInt8()
{
    return Int8_Dist(*RG32_MT19937);
}

Int8 GetRandomInt8(Int8 min, Int8 max)
{
    std::uniform_int_distribution<Int8> dist(min, max);
    return dist(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
Uint8 GetRandomUint8()
{
    return UInt8_Dist(*RG32_MT19937);
}

Uint8 GetRandomUint8(Uint8 min, Uint8 max)
{
    std::uniform_int_distribution<Uint8> dist(min, max);
    return dist(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
Int16 GetRandomInt16()
{
    return Int16_Dist(*RG32_MT19937);
}

Int16 GetRandomInt16(Int16 min, Int16 max)
{
    std::uniform_int_distribution<Int16> dist(min, max);
    return dist(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
Uint16 GetRandomUint16()
{
    return UInt16_Dist(*RG32_MT19937);
}

Uint16 GetRandomUint16(Uint16 min, Uint16 max)
{
    std::uniform_int_distribution<Uint16> dist(min, max);
    return dist(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
Int32 GetRandomInt32()
{
    return Int32_Dist(*RG32_MT19937);
}

Int32 GetRandomInt32(Int32 min, Int32 max)
{
    std::uniform_int_distribution<Int32> dist(min, max);
    return dist(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
Uint32 GetRandomUint32()
{
    return UInt32_Dist(*RG32_MT19937);
}

Uint32 GetRandomUint32(Uint32 min, Uint32 max)
{
    std::uniform_int_distribution<Uint32> dist(min, max);
    return dist(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
Int64 GetRandomInt64()
{
    return Int64_Dist(*RG64_MT19937);
}

Int64 GetRandomInt64(Int64 min, Int64 max)
{
    std::uniform_int_distribution<Int64> dist(min, max);
    return dist(*RG64_MT19937);
}

// ------------------------------------------------------------------------------------------------
Uint64 GetRandomUint64()
{
    return UInt64_Dist(*RG64_MT19937);
}

Uint64 GetRandomUint64(Uint64 min, Uint64 max)
{
    std::uniform_int_distribution<Uint64> dist(min, max);
    return dist(*RG64_MT19937);
}

// ------------------------------------------------------------------------------------------------
Float32 GetRandomFloat32()
{
    return Float32_Dist(*RG32_MT19937);
}

Float32 GetRandomFloat32(Float32 min, Float32 max)
{
    std::uniform_real_distribution<Float32> dist(min, max);
    return dist(*RG32_MT19937);
}

// ------------------------------------------------------------------------------------------------
Float64 GetRandomFloat64()
{
    return Float64_Dist(*RG64_MT19937);
}

Float64 GetRandomFloat64(Float64 min, Float64 max)
{
    std::uniform_real_distribution<Float64> dist(min, max);
    return dist(*RG64_MT19937);
}

// ------------------------------------------------------------------------------------------------
String GetRandomString(String::size_type len)
{
    String str(len, 0);
    std::generate(str.begin(), str.end(), [&] () -> String::value_type { return String_Dist(*RG32_MT19937); });
    return std::move(str);
}

String GetRandomString(String::size_type len, String::value_type min, String::value_type max)
{
    String str(len, 0);
    std::uniform_int_distribution<String::value_type> dist(min, max);
    std::generate(str.begin(), str.end(), [&] () -> String::value_type { return dist(*RG32_MT19937); });
    return std::move(str);
}

// ------------------------------------------------------------------------------------------------
bool GetRandomBool()
{
    return Int8_Dist(*RG32_MT19937) > 0 ? true : false;
}

// ------------------------------------------------------------------------------------------------
const Color3 & GetRandomColor()
{
    return SQ_Color_List.at(GetRandomUint32(0, SQ_Color_List.size()-1));
}

// --------------------------------------------------------------------------------------------
bool SToB(const SQChar * str)
{
    return (strcmp(str, "true") == 0 || \
            strcmp(str, "yes") == 0 || \
            strcmp(str, "on") == 0 || \
            strcmp(str, "1") == 0) ? true : false;
}

// ------------------------------------------------------------------------------------------------
Color3 GetColor(const SQChar * name)
{
    // See if we actually have something to search for
    if(std::strlen(name) <= 0)
    {
        LogErr("Cannot extract values from an empty string");
        return Color3::NIL;
    }
    // Clone the string into an editable version
    String str = String(name);
    // Strip non alphanumeric characters from the name
    str.erase(std::remove_if(str.begin(), str.end(), std::not1(std::ptr_fun(::isalnum))), str.end());
    // Convert the string to lowercase
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    // See if we still have a valid name after the cleanup
    if(str.empty())
    {
        LogErr("Cannot extract values from an invalid string: %s", name);
        return Color3::NIL;
    }
    // Grab the actual length of the string
    std::size_t len = str.length();
    // Get the most significant characters used to identify a weapon
    SQChar a = str[0], b = 0, c = 0, d = str[len-1];
    // Look for deeper specifiers
    if(str.length() >= 3)
    {
        c = str[2];
        b = str[1];
    }
    else if(str.length() >= 2)
    {
        b = str[1];
    }
    // Search for a pattern in the name
    switch(a)
    {
        // [A]liceBlue
        // [A]ntiqueWhite
        // [A]qua
        // [A]quamarine
        // [A]zure
        case 'a':
            switch (b)
            {
                // [Al]iceBlue
                case 'l': return Color3(240, 248, 255);
                // [Aq]ua[m]arine
                case 'm': return Color3(127, 255, 212);
                // [An]tiqueWhite
                case 'n': return Color3(250, 235, 215);
                // [Aq]ua
                // [Aq]uamarine
                case 'q':
                    // [Aq]u[a]
                    if (d == 'a') return Color3(0, 255, 255);
                    // [Aq]ua[m]arin[e]
                    else if (d == 'e' || (len > 4 && str[4] == 'm')) return Color3(127, 255, 212);
                    // Default to blank
                    else return Color3::NIL;
                // [Az]ure
                case 'z': return Color3(240, 255, 255);
                // Default to blank
                default: return Color3::NIL;
            }
        // [B]eige
        // [B]isque
        // [B]lack
        // [B]lanchedAlmond
        // [B]lue
        // [B]lueViolet
        // [B]rown
        // [B]urlyWood
        case 'b':
            switch (b)
            {
                // [B]lanched[A]lmond
                case 'a': return Color3(255, 235, 205);
                // [Be]ige
                case 'e': return Color3(245, 245, 220);
                // [Bi]sque
                case 'i': return Color3(255, 228, 196);
                // [Bl]ack
                // [Bl]anchedAlmond
                // [Bl]ue
                // [Bl]ueViolet
                case 'l':
                    // [Bl]a[ck]
                    if (d == 'k' || d == 'c') return Color3(0, 0, 0);
                    // [Bl]anched[A]lmon[d]
                    else if (d == 'd' || (len > 8 && str[8] == 'a')) return Color3(255, 235, 205);
                    // [Bl]u[e]
                    else if (d == 'e') return Color3(0, 0, 255);
                    // [Bl]ue[V]iole[t]
                    else if (d == 't' || (len > 4 && str[4] == 'v')) return Color3(138, 43, 226);
                    // Default to blank
                    else return Color3::NIL;
                // [Br]own
                case 'r': return Color3(165, 42, 42);
                // [Bu]rlyWood
                case 'u': return Color3(222, 184, 135);
                // [B]lue[V]iolet
                case 'v': return Color3(138, 43, 226);
                // Default to blank
                default: return Color3::NIL;
            }
        // [C]adetBlue
        // [C]hartreuse
        // [C]hocolate
        // [C]oral
        // [C]ornflowerBlue
        // [C]ornsilk
        // [C]rimson
        // [C]yan
        case 'c':
            switch (b)
            {
                // [Ca]detBlue
                case 'a': return Color3(95, 158, 160);
                // [Ch]artreuse
                // [Ch]ocolate
                case 'h':
                    // [Ch]artreuse
                    if (c == 'a') return Color3(127, 255, 0);
                    // [Ch]ocolate
                    else if (c == 'o') return Color3(210, 105, 30);
                    // Default to blank
                    else return Color3::NIL;
                // [Co]ral
                // [Co]rnflowerBlue
                // [Co]rnsilk
                case 'o':
                    // [Co]ra[l]
                    if (d == 'l') return Color3(255, 127, 80);
                    // [Co]rnflower[B]lu[e]
                    else if (d == 'e' || (len > 10 && str[10] == 'b')) return Color3(100, 149, 237);
                    // [Co]rnsil[k]
                    else if (d == 'k') return Color3(255, 248, 220);
                    // Default to blank
                    else return Color3::NIL;
                // [Cr]imson
                case 'r': return Color3(220, 20, 60);
                // [Cy]an
                case 'y': return Color3(0, 255, 255);
                // Default to blank
                default: return Color3::NIL;
            }
        // [D]arkBlue
        // [D]arkCyan
        // [D]arkGoldenRod
        // [D]arkGray
        // [D]arkGreen
        // [D]arkKhaki
        // [D]arkMagenta
        // [D]arkOliveGreen
        // [D]arkOrange
        // [D]arkOrchid
        // [D]arkRed
        // [D]arkSalmon
        // [D]arkSeaGreen
        // [D]arkSlateBlue
        // [D]arkSlateGray
        // [D]arkTurquoise
        // [D]arkViolet
        // [D]eepPink
        // [D]eepSkyBlue
        // [D]imGray
        // [D]odgerBlue
        case 'd':
            // [Di]mGray
            if (b == 'i' || b == 'g') return Color3(105, 105, 105);
            // [Do]dgerBlue
            else if (b == 'o' || b == 'b') return Color3(30, 144, 255);
            // [De]ep[P]in[k]
            else if (b == 'e' && (d == 'k' || (len > 4 && str[4] == 'p'))) return Color3(255, 20, 147);
            // [De]ep[S]kyBlu[e]
            else if (b == 'e' && (d == 'e' || (len > 4 && str[4] == 's'))) return Color3(0, 191, 255);
            // [Da]rkBlue
            // [Da]rkCyan
            // [Da]rkGoldenRod
            // [Da]rkGray
            // [Da]rkGreen
            // [Da]rkKhaki
            // [Da]rkMagenta
            // [Da]rkOliveGreen
            // [Da]rkOrange
            // [Da]rkOrchid
            // [Da]rkRed
            // [Da]rkSalmon
            // [Da]rkSeaGreen
            // [Da]rkSlateBlue
            // [Da]rkSlateGray
            // [Da]rkTurquoise
            // [Da]rkViolet
            else if (b == 'a') {
                // [Da]rk[B]lue
                if (c == 'b' || (len > 4 && str[4] == 'b')) return Color3(0, 0, 139);
                // [Da]rk[C]yan
                else if (c == 'c' || (len > 4 && str[4] == 'c')) return Color3(0, 139, 139);
                // [Da]rk[Go]ldenRo[d]
                else if ((len > 4 && str[4] == 'g') && (d == 'd' || d == 'o')) return Color3(184, 134, 11);
                // [Da]rk[G]r[ay]
                else if ((len > 4 && str[4] == 'g') && (d == 'y' || d == 'a')) return Color3(169, 169, 169);
                // [Da]rk[G]r[een]
                else if ((len > 4 && str[4] == 'g') && (d == 'n' || d == 'e')) return Color3(0, 100, 0);
                // [Da]rk[K]hak[i]
                else if (d == 'i' || c == 'k' || (len > 4 && str[4] == 'k')) return Color3(189, 183, 107);
                // [Da]rk[M]agent[a]
                else if (d == 'a' || c == 'm' || (len > 4 && str[4] == 'm')) return Color3(139, 0, 139);
                // [Da]rk[O]liveGr[een]
                else if ((len > 4 && str[4] == 'o') && (d == 'n' || d == 'e')) return Color3(85, 107, 47);
                // [Da]rk[O]r[a]ng[e]
                else if ((len > 4 && str[4] == 'o') && (d == 'e' || d == 'a')) return Color3(255, 140, 0);
                // [Da]rk[O]r[c]hi[d]
                else if ((len > 4 && str[4] == 'o') && (d == 'd' || d == 'c')) return Color3(153, 50, 204);
                // [Da]rk[R]ed
                else if (len > 4 && str[4] == 'r') return Color3(139, 0, 0);
                // [Da]rk[Sa]lmon
                else if (len > 5 && str[4] == 's' && str[5] == 'a') return Color3(233, 150, 122);
                // [Da]rk[Se]aGreen
                else if (len > 5 && str[4] == 's' && str[5] == 'e') return Color3(143, 188, 143);
                // [Da]rk[S]lateBlu[e]
                else if ((len > 4 && str[4] == 's') && (d == 'e' || d == 'b')) return Color3(72, 61, 139);
                // [Da]rk[S]lateGra[y]
                else if ((len > 4 && str[4] == 's') && (d == 'y' || d == 'g')) return Color3(47, 79, 79);
                // [Da]rk[T]urquoise
                else if (c == 't' || (len > 4 && str[4] == 't')) return Color3(0, 206, 209);
                // [Da]rk[V]iolet
                else if (c == 'v' || (len > 4 && str[4] == 'v')) return Color3(148, 0, 211);
                // Default to blank
                else return Color3::NIL;
            // Default to blank
            } else return Color3::NIL;
        // [F]ireBrick
        // [F]loralWhite
        // [F]orestGreen
        // [F]uchsia
        case 'f':
            switch (b)
            {
                // [Fi]re[B]rick
                case 'i':
                case 'b': return Color3(178, 34, 34);
                // [Fl]oral[W]hite
                case 'l':
                case 'w': return Color3(255, 250, 240);
                // [Fo]rest[G]reen
                case 'o':
                case 'g': return Color3(34, 139, 34);
                // [Fu]chsia
                case 'u': return Color3(255, 0, 255);
                // Default to blank
                default: return Color3::NIL;
            }
        // [G]ainsboro
        // [G]hostWhite
        // [G]old
        // [G]oldenRod
        // [G]ray
        // [G]reen
        // [G]reenYellow
        case 'g':
            // [Ga]insboro
            if (b == 'a') return Color3(220, 220, 220);
            // [Gh]ost[W]hite
            else if (b == 'h' || b == 'w') return Color3(248, 248, 255);
            // [Go]ld[e]n[R]od
            else if (len > 4 && (str[4] == 'e' || str[4] == 'r')) return Color3(218, 165, 32);
            // [Go]l[d]
            else if (b == 'o' && d == 'd') return Color3(255, 215, 0);
            // [Gray]
            else if (b == 'r' && (d == 'y' || d == 'a')) return Color3(128, 128, 128);
            // [Gr]een
            else if (b == 'r' && d == 'n') return Color3(0, 128, 0);
            // [Gr]eenYellow
            else if (b == 'r' && (d == 'w' || (len > 5 && str[5] == 'y'))) return Color3(173, 255, 47);
            // Default to blank
            else return Color3::NIL;
        // [H]oneyDew
        // [H]otPink
        case 'h':
            // [H]o[n]ey[D]e[w]
            if (d == 'w' || c == 'n' || (len > 5 && str[5] == 'd')) return Color3(240, 255, 240);
            // [H]o[tP]in[k]
            else if (d == 'k' || c == 't' || (len > 3 && str[3] == 'p')) return Color3(255, 105, 180);
            // Default to blank
            else return Color3::NIL;
        // [I]ndianRed
        // [I]ndigo
        // [I]vory
        case 'i':
            // [In]dian[R]e[d]
            if (b == 'n' && (d == 'd' || d == 'r')) return Color3(205, 92, 92);
            // [In]di[go]
            else if (b == 'n' && (d == 'o' || d == 'g')) return Color3(75, 0, 130);
            // [I]vory
            else if (b == 'v') return Color3(255, 255, 240);
            // Default to blank
            else return Color3::NIL;
        // [K]haki
        case 'k': return Color3(240, 230, 140);
        // [L]avender
        // [L]avenderBlush
        // [L]awnGreen
        // [L]emonChiffon
        // [L]ightBlue
        // [L]ightCoral
        // [L]ightCyan
        // [L]ightGoldenRodYellow
        // [L]ightGray
        // [L]ightGreen
        // [L]ightPink
        // [L]ightSalmon
        // [L]ightSeaGreen
        // [L]ightSkyBlue
        // [L]ightSlateGray
        // [L]ightSteelBlue
        // [L]ightYellow
        // [L]ime
        // [L]imeGreen
        // [L]inen
        case 'l':
            // [La]vende[r]
            if (b == 'a' && d == 'r') return Color3(230, 230, 250);
            // [La]vender[B]lus[h]
            else if (b == 'a' && (d == 'h' || d == 'b')) return Color3(255, 240, 245);
            // [Law]n[G]ree[n]
            else if (b == 'g' || (b == 'a' && (c == 'w' || d == 'n'))) return Color3(124, 252, 0);
            // [Le]mon[C]hiffon
            else if (b == 'e' || b == 'c') return Color3(255, 250, 205);
            // [Li]me[G]reen
            else if (b == 'g' || (b == 'i' && (len > 4 && str[4] == 'g'))) return Color3(50, 205, 50);
            // [Lime]
            else if (b == 'i' && c == 'm' && d == 'e') return Color3(0, 255, 0);
            // [Lin]e[n]
            else if (b == 'i' && (c == 'n' || d == 'n')) return Color3(250, 240, 230);
            // [Li]ghtBlue
            // [Li]ghtCoral
            // [Li]ghtCyan
            // [Li]ghtGoldenRodYellow
            // [Li]ghtGray
            // [Li]ghtGreen
            // [Li]ghtPink
            // [Li]ghtSalmon
            // [Li]ghtSeaGreen
            // [Li]ghtSkyBlue
            // [Li]ghtSlateGray
            // [Li]ghtSteelBlue
            // [Li]ghtYellow
            else if (b == 'i') {
                // [Li]ght[B]lue
                if (len > 5 && str[5] == 'b') return Color3(173, 216, 230);
                // [Li]ght[Co]ra[l]
                else if ((len > 5 && str[5] == 'c') && (d == 'l' || d == 'o')) return Color3(240, 128, 128);
                // [Li]ght[Cy]a[n]
                else if ((len > 5 && str[5] == 'c') && (d == 'n' || d == 'y')) return Color3(224, 255, 255);
                // [Li]ght[Go]ldenRodYello[w]
                else if ((len > 5 && str[5] == 'g') && (d == 'w' || d == 'o')) return Color3(250, 250, 210);
                // [Li]ght[G]r[ay]
                else if ((len > 5 && str[5] == 'g') && (d == 'y' || d == 'a')) return Color3(211, 211, 211);
                // [Li]ght[G]r[een]
                else if ((len > 5 && str[5] == 'g') && (d == 'n' || d == 'e')) return Color3(144, 238, 144);
                // [Li]ght[P]ink
                else if (len > 5 && str[5] == 'p') return Color3(255, 182, 193);
                // [Li]ght[Sa]lmon
                else if (len > 6 && str[5] == 's' && str[5] == 'a') return Color3(255, 160, 122);
                // [Li]ght[Se]aGreen
                else if (len > 6 && str[5] == 's' && str[5] == 'e') return Color3(32, 178, 170);
                // [Li]ght[Sk]yBlue
                else if (len > 6 && str[5] == 's' && str[5] == 'k') return Color3(135, 206, 250);
                // [Li]ght[Sl]ateGray
                else if (len > 6 && str[5] == 's' && str[5] == 'l') return Color3(119, 136, 153);
                // [Li]ght[St]eelBlue
                else if (len > 6 && str[5] == 's' && str[5] == 't') return Color3(176, 196, 222);
                // [Li]ght[Y]ellow
                else if (len > 5 && str[5] == 'y') return Color3(255, 255, 224);
                // Default to blank
                else return Color3::NIL;
            // Default to blank
            } else return Color3::NIL;
        // [M]agenta
        // [M]aroon
        // [M]ediumAquaMarine
        // [M]ediumBlue
        // [M]ediumOrchid
        // [M]ediumPurple
        // [M]ediumSeaGreen
        // [M]ediumSlateBlue
        // [M]ediumSpringGreen
        // [M]ediumTurquoise
        // [M]ediumVioletRed
        // [M]idnightBlue
        // [M]intCream
        // [M]istyRose
        // [M]occasin
        case 'm':
            // [Ma]genta
            if (b == 'a' && (c == 'a' || d == 'a')) return Color3(255, 0, 255);
            // [Ma]roon
            else if (b == 'a' && (c == 'r' || d == 'n' || d == 'o')) return Color3(128, 0, 0);
            // [Me]diumAquaMarine
            // [Me]diumBlue
            // [Me]diumOrchid
            // [Me]diumPurple
            // [Me]diumSeaGreen
            // [Me]diumSlateBlue
            // [Me]diumSpringGreen
            // [Me]diumTurquoise
            // [Me]diumVioletRed
            else if (b == 'e') {
                // [Me]dium[A]quaMarine
                if (c == 'a' || (len > 6 && str[6] == 'a')) return Color3(102, 205, 170);
                // [Me]dium[B]lue
                else if (c == 'b' || (len > 6 && str[6] == 'b')) return Color3(0, 0, 205);
                // [Me]dium[O]rchid
                else if (c == 'o' || (len > 6 && str[6] == 'o')) return Color3(186, 85, 211);
                // [Me]dium[P]urple
                else if (c == 'p' || (len > 6 && str[6] == 'p')) return Color3(147, 112, 219);
                // [Me]dium[T]urquoise
                else if (c == 't' || (len > 6 && str[6] == 't')) return Color3(72, 209, 204);
                // [Me]dium[V]ioletRed
                else if (c == 'v' || (len > 6 && str[6] == 'v')) return Color3(199, 21, 133);
                // [Me]dium[Se]aGreen
                else if (len > 7 && str[6] == 's' && str[7] == 'e') return Color3(60, 179, 113);
                // [Me]dium[Sl]ateBlue
                else if (len > 7 && str[6] == 's' && str[7] == 'l') return Color3(123, 104, 238);
                // [Me]dium[Sp]ringGreen
                else if (len > 7 && str[6] == 's' && str[7] == 'p') return Color3(0, 250, 154);
                // Default to blank
                else return Color3::NIL;
            }
            // [Mi]dnightBlue
            else if (b == 'i' && c == 'd') return Color3(25, 25, 112);
            // [Mi]ntCream
            else if (b == 'i' && c == 'n') return Color3(245, 255, 250);
            // [Mi]styRose
            else if (b == 'i' && c == 's') return Color3(255, 228, 225);
            // [Mo]ccasin
            else if (b == 'o') return Color3(255, 228, 181);
            // Default to blank
            else return Color3::NIL;
        // [N]avajoWhite
        // [N]avy
        case 'n':
            // [Na]vajo[W]hite
            if (c == 'v' || c == 'w') return Color3(255, 222, 173);
            // [Na]v[y]
            else if (c == 'a' || d == 'y') return Color3(0, 0, 128);
            // Default to blank
            else return Color3::NIL;
        // [O]ldLace
        // [O]live
        // [O]liveDrab
        // [O]range
        // [O]rangeRed
        // [O]rchid
        case 'o':
            // [Old]Lace
            if (b == 'l' && c == 'd') return Color3(253, 245, 230);
            // [Ol]ive[D]ra[b]
            else if (b == 'l' && (d == 'b' || d == 'd')) return Color3(107, 142, 35);
            // [Ol]iv[e]
            else if (b == 'l' && d == 'e') return Color3(128, 128, 0);
            // [Or]ange[R]e[d]
            else if (b == 'r' && (d == 'd' || d == 'r')) return Color3(255, 69, 0);
            // [Or]ang[e]
            else if (b == 'r' && d == 'e') return Color3(255, 165, 0);
            // [Orc]hid
            else if (d == 'c') return Color3(218, 112, 214);
            // Default to blank
            else return Color3::NIL;
        // [P]aleGoldenRod
        // [P]aleGreen
        // [P]aleTurquoise
        // [P]aleVioletRed
        // [P]apayaWhip
        // [P]eachPuff
        // [P]eru
        // [P]ink
        // [P]lum
        // [P]owderBlue
        // [P]urple
        case 'p':
            // [Pu]rple
            if (b == 'u') return Color3(128, 0, 128);
            // [Po]wderBlue
            else if (b == 'o') return Color3(176, 224, 230);
            // [Pi]nk
            else if (b == 'i') return Color3(255, 192, 203);
            // [Pl]um
            else if (b == 'l') return Color3(221, 160, 221);
            // [Pea]chPuff
            else if (b == 'e' && c == 'a') return Color3(255, 218, 185);
            // [Per]u
            else if (b == 'e' && c == 'r') return Color3(205, 133, 63);
            // [Pa]payaWhip
            else if (b == 'a' && c == 'p') return Color3(255, 239, 213);
            // [Pa]le[Go]ldenRod
            else if (b == 'a' && (len > 5 && str[4] == 'g' && str[5] == 'o')) return Color3(238, 232, 170);
            // [Pa]le[Gr]een
            else if (b == 'a' && (len > 5 && str[4] == 'g' && str[5] == 'r')) return Color3(152, 251, 152);
            // [Pa]le[T]urquoise
            else if (b == 'a' && (len > 4 && str[4] == 't')) return Color3(175, 238, 238);
            // [Pa]le[V]ioletRed
            else if (b == 'a' && (len > 4 && str[4] == 'v')) return Color3(219, 112, 147);
            // Default to blank
            else return Color3::NIL;
        // [R]ed
        // [R]osyBrown
        // [R]oyalBlue
        case 'r':
            // [Re]d
            if (b == 'e') return Color3(255, 0, 0);
            // [Ros]yBrown
            else if (b == 'o' && c == 's') return Color3(188, 143, 143);
            // [Roy]alBlue
            else if (b == 'o' && c == 'y') return Color3(65, 105, 225);
            // Default to blank
            else return Color3::NIL;
        // [S]addleBrown
        // [S]almon
        // [S]andyBrown
        // [S]eaGreen
        // [S]eaShell
        // [S]ienna
        // [S]ilver
        // [S]kyBlue
        // [S]lateBlue
        // [S]lateGray
        // [S]now
        // [S]pringGreen
        // [S]teelBlue
        case 's':
            // [Sad]dleBrown
            if (b == 'a' && c == 'd') return Color3(139, 69, 19);
            // [Sal]mon
            else if (b == 'a' && c == 'l') return Color3(250, 128, 114);
            // [San]dyBrown
            else if (b == 'a' && c == 'n') return Color3(244, 164, 96);
            // [Se]a[G]reen
            else if (b == 'e' && d == 'g') return Color3(46, 139, 87);
            // [Se]a[S]hell
            else if (b == 'e' && d == 's') return Color3(255, 245, 238);
            // [Sie]nna
            else if (b == 'i' && c == 'e') return Color3(160, 82, 45);
            // [Sil]ver
            else if (b == 'i' && c == 'l') return Color3(192, 192, 192);
            // [Sk]yBlue
            else if (b == 'k') return Color3(135, 206, 235);
            // [Sl]ateBlue
            else if (b == 'l' && (d == 'e' || (len > 5 && str[5] == 'b'))) return Color3(106, 90, 205);
            // [Sl]ateGray
            else if (b == 'l' && (d == 'y' || (len > 5 && str[5] == 'g'))) return Color3(112, 128, 144);
            // [Sn]ow
            else if (b == 'n') return Color3(255, 250, 250);
            // [Sp]ringGreen
            else if (b == 'p') return Color3(0, 255, 127);
            // [St]eelBlue
            else if (b == 't') return Color3(70, 130, 180);
            // Default to blank
            else return Color3::NIL;
        // [T]an
        // [T]eal
        // [T]histle
        // [T]omato
        // [T]urquoise
        case 't':
            switch(b)
            {
                // [Ta]n
                case 'a': return Color3(210, 180, 140);
                // [Te]al
                case 'e': return Color3(0, 128, 128);
                // [Th]istle
                case 'h': return Color3(216, 191, 216);
                // [To]mato
                case 'o': return Color3(255, 99, 71);
                // [Tu]rquoise
                case 'u': return Color3(64, 224, 208);
                // Default to blank
                default: return Color3::NIL;
            }
        // [V]iolet
        case 'v': return Color3(238, 130, 238);
        // [W]heat
        // [W]hite
        // [W]hiteSmoke
        case 'w':
            // [Wh]eat
            if (b == 'h' && c == 'e') return Color3(245, 222, 179);
            // [Wh]ite[S]moke
            else if (b == 'h' && (len > 5 && str[5] == 's')) return Color3(245, 245, 245);
            // [Whi]te
            else if (b == 'h' && c == 'i') return Color3(255, 255, 255);
            // Default to blank
            else return Color3::NIL;
        // [Y]ellow
        // [Y]ellowGreen
        case 'y':
            // [Ye]llow[G]reen
            if (b == 'e' && (len > 6 && str[6] == 'g')) return Color3(154, 205, 50);
            // [Yel]low
            else if (b == 'e' && c == 'l') return Color3(255, 255, 0);
            // Default to blank
            else return Color3::NIL;
        // Default to blank
        default: return Color3::NIL;
    }
}

// ------------------------------------------------------------------------------------------------
AABB GetAABB(const SQChar * str, SQChar delim)
{
    static SQChar fs[] = _SC(" %f , %f , %f , %f , %f , %f ");
    static AABB box;

    box.Clear();

    if (std::strlen(str) <= 0)
    {
        LogErr("Cannot extract values from an empty string");
        return box;
    }
    else if (delim != AABB::Delim)
    {
        fs[4] = delim;
        fs[9] = delim;
        fs[14] = delim;
        fs[19] = delim;
        fs[24] = delim;
    }
    else
    {
        fs[4] = AABB::Delim;
        fs[9] = AABB::Delim;
        fs[14] = AABB::Delim;
        fs[19] = AABB::Delim;
        fs[24] = AABB::Delim;
    }

    std::sscanf(str, &fs[0], &box.min.x, &box.min.y, &box.min.z, &box.max.x, &box.max.y, &box.max.z);

    return box;
}

// ------------------------------------------------------------------------------------------------
Circle GetCircle(const SQChar * str, SQChar delim)
{
    static SQChar fs[] = _SC(" %f , %f , %f ");
    static Circle circle;

    circle.Clear();

    if (std::strlen(str) <= 0)
    {
        LogErr("Cannot extract values from an empty string");
        return circle;
    }
    else if (delim != Circle::Delim)
    {
        fs[4] = delim;
        fs[9] = delim;
    }
    else
    {
        fs[4] = Circle::Delim;
        fs[9] = Circle::Delim;
    }

    std::sscanf(str, &fs[0], &circle.pos.x, &circle.pos.y, &circle.rad);

    return circle;
}

// ------------------------------------------------------------------------------------------------
Color3 GetColor3(const SQChar * str, SQChar delim)
{
    static SQChar fs[] = _SC(" %u , %u , %u ");
    SQUint32 r = 0, g = 0, b = 0;

    if (std::strlen(str) <= 0)
    {
        LogErr("Cannot extract values from an empty string");
        return Color3();
    }
    else if (delim != Color3::Delim)
    {
        fs[4] = delim;
        fs[9] = delim;
    }
    else
    {
        fs[4] = Color3::Delim;
        fs[9] = Color3::Delim;
    }

    std::sscanf(str, &fs[0], &r, &g, &b);

    return Color3(static_cast<Color3::Value>(r), static_cast<Color3::Value>(g), static_cast<Color3::Value>(b));
}

Color4 GetColor4(const SQChar * str, SQChar delim)
{
    static SQChar fs[] = _SC(" %u , %u , %u , %u ");
    SQUint32 r = 0, g = 0, b = 0, a = 0;

    if (std::strlen(str) <= 0)
    {
        LogErr("Cannot extract values from an empty string");
        return Color4();
    }
    else if (delim != Color4::Delim)
    {
        fs[4] = delim;
        fs[9] = delim;
        fs[14] = delim;
    }
    else
    {
        fs[4] = Color4::Delim;
        fs[9] = Color4::Delim;
        fs[14] = Color4::Delim;
    }

    std::sscanf(str, &fs[0], &r, &g, &b, &a);

    return Color4(static_cast<Color4::Value>(r), static_cast<Color4::Value>(g), static_cast<Color4::Value>(b), static_cast<Color4::Value>(a));
}

// ------------------------------------------------------------------------------------------------
Quaternion GetQuaternion(const SQChar * str, SQChar delim)
{
    static SQChar fs[] = _SC(" %f , %f , %f , %f ");
    static Quaternion quat;

    quat.Clear();

    if (std::strlen(str) <= 0)
    {
        LogErr("Cannot extract values from an empty string");
        return quat;
    }
    else if (delim != Quaternion::Delim)
    {
        fs[4] = delim;
        fs[9] = delim;
        fs[14] = delim;
    }
    else
    {
        fs[4] = Quaternion::Delim;
        fs[9] = Quaternion::Delim;
        fs[14] = Quaternion::Delim;
    }

    std::sscanf(str, &fs[0], &quat.x, &quat.y, &quat.z, &quat.w);

    return quat;
}

Sphere GetSphere(const SQChar * str, SQChar delim)
{
    static SQChar fs[] = _SC(" %f , %f , %f , %f ");
    static Sphere sphere;

    sphere.Clear();

    if (std::strlen(str) <= 0)
    {
        LogErr("Cannot extract values from an empty string");
        return sphere;
    }
    else if (delim != Sphere::Delim)
    {
        fs[4] = delim;
        fs[9] = delim;
        fs[14] = delim;
    }
    else
    {
        fs[4] = Sphere::Delim;
        fs[9] = Sphere::Delim;
        fs[14] = Sphere::Delim;
    }

    std::sscanf(str, &fs[0], &sphere.pos.x, &sphere.pos.y, &sphere.pos.z, &sphere.rad);

    return sphere;
}

// ------------------------------------------------------------------------------------------------
Vector2f GetVector2f(const SQChar * str, SQChar delim)
{
    static SQChar fs[] = _SC(" %f , %f ");
    static Vector2f vec;

    vec.Clear();

    if (std::strlen(str) <= 0)
    {
        LogErr("Cannot extract values from an empty string");
        return vec;
    }
    else if (delim != Vector2f::Delim)
    {
        fs[4] = delim;
    }
    else
    {
        fs[4] = Vector2f::Delim;
    }

    std::sscanf(str, &fs[0], &vec.x, &vec.y);

    return vec;
}

Vector2i GetVector2i(const SQChar * str, SQChar delim)
{
    static SQChar fs[] = _SC(" %d , %d ");
    static Vector2i vec;

    vec.Clear();

    if (std::strlen(str) <= 0)
    {
        LogErr("Cannot extract values from an empty string");
        return vec;
    }
    else if (delim != Vector2i::Delim)
    {
        fs[4] = delim;
    }
    else
    {
        fs[4] = Vector2i::Delim;
    }

    std::sscanf(str, &fs[0], &vec.x, &vec.y);

    return vec;
}

Vector2u GetVector2u(const SQChar * str, SQChar delim)
{
    static SQChar fs[] = _SC(" %u , %u ");
    static Vector2u vec;

    vec.Clear();

    if (std::strlen(str) <= 0)
    {
        LogErr("Cannot extract values from an empty string");
        return vec;
    }
    else if (delim != Vector2u::Delim)
    {
        fs[4] = delim;
    }
    else
    {
        fs[4] = Vector2u::Delim;
    }

    std::sscanf(str, &fs[0], &vec.x, &vec.y);

    return vec;
}

// ------------------------------------------------------------------------------------------------
Vector3 GetVector3(const SQChar * str, SQChar delim)
{
    static SQChar fs[] = _SC(" %f , %f , %f ");
    static Vector3 vec;

    vec.Clear();

    if (std::strlen(str) <= 0)
    {
        LogErr("Cannot extract values from an empty string");
        return vec;
    }
    else if (delim != Vector3::Delim)
    {
        fs[4] = delim;
        fs[9] = delim;
    }
    else
    {
        fs[4] = Vector3::Delim;
        fs[9] = Vector3::Delim;
    }

    std::sscanf(str, &fs[0], &vec.x, &vec.y, &vec.z);

    return vec;
}

Vector4 GetVector4(const SQChar * str, SQChar delim)
{
    static SQChar fs[] = _SC(" %f , %f , %f , %f ");
    static Vector4 vec;

    vec.Clear();

    if (std::strlen(str) <= 0)
    {
        LogErr("Cannot extract values from an empty string");
        return vec;
    }
    else if (delim != Vector4::Delim)
    {
        fs[4] = delim;
        fs[9] = delim;
        fs[14] = delim;
    }
    else
    {
        fs[4] = Vector4::Delim;
        fs[9] = Vector4::Delim;
        fs[14] = Vector4::Delim;
    }

    std::sscanf(str, &fs[0], &vec.x, &vec.y, &vec.z, &vec.w);

    return vec;
}

// ------------------------------------------------------------------------------------------------
template < typename T > T StrToInt(const SQChar * str)
{
    try
    {
        return SToI< T >::Fn(str, nullptr, 10);
    }
    catch (const std::invalid_argument & ia)
    {
        LogErr("Unable to convert string to integer: %s", ia.what());
    }
    catch (const std::out_of_range & oor)
    {
        LogErr("Unable to convert string to integer: %s", oor.what());
    }
    return static_cast< T >(0);
}

// ------------------------------------------------------------------------------------------------
template < typename T > T StrToInt(const SQChar * str, SQInt32 base)
{
    try
    {
        return SToI< T >::Fn(str, nullptr, base);
    }
    catch (const std::invalid_argument & ia)
    {
        LogErr("Unable to convert string to integer: %s", ia.what());
    }
    catch (const std::out_of_range & oor)
    {
        LogErr("Unable to convert string to integer: %s", oor.what());
    }
    return static_cast< T >(0);
}

// ------------------------------------------------------------------------------------------------
template < typename T > T StrToReal(const SQChar * str)
{
    try
    {
        return SToF< T >::Fn(str, nullptr);
    }
    catch (const std::invalid_argument & ia)
    {
        LogErr("Unable to convert string to float: %s", ia.what());
    }
    catch (const std::out_of_range & oor)
    {
        LogErr("Unable to convert string to float: %s", oor.what());
    }
    return static_cast< T >(0.0);
}

// ------------------------------------------------------------------------------------------------
template < typename T > T RandomValue()
{
    return RandomVal< T >::Get();
}

template < typename T > T RandomValue(T min, T max)
{
    return RandomVal< T >::Get(min, max);
}

// ------------------------------------------------------------------------------------------------
bool Register_Base(HSQUIRRELVM vm)
{
    // Typedef squirrel string to simplify code
    typedef const SQChar * SQS;
    // Output debugging information
    LogDbg("Beginning registration of <Base> API");
    // Attempt to register the specified API
    Sqrat::RootTable(vm)

    .Func(_SC("EpsEqI"), &EpsEq<SQInteger>)
    .Func(_SC("EpsEqF"), &EpsEq<SQFloat>)

    .Func(_SC("ClampI"), &Clamp<SQInteger>)
    .Func(_SC("ClampF"), &Clamp<SQFloat>)

    .Func(_SC("IsSpace"), &isspace)
    .Func(_SC("IsPrint"), &isprint)
    .Func(_SC("IsCntrl"), &iscntrl)
    .Func(_SC("IsUpper"), &isupper)
    .Func(_SC("IsLower"), &islower)
    .Func(_SC("IsAlpha"), &isalpha)
    .Func(_SC("IsDigit"), &isdigit)
    .Func(_SC("IsPunct"), &ispunct)
    .Func(_SC("IsXdigit"), &isxdigit)
    .Func(_SC("IsAlnum"), &isalnum)
    .Func(_SC("IsGraph"), &isgraph)
    .Func(_SC("IsBlank"), &isblank)

    .Overload< SQInteger (*)(void) >(_SC("GetRandomInt"), &RandomValue< SQInteger >)
    .Overload< SQInteger (*)(SQInteger, SQInteger) >(_SC("GetRandomInt"), &RandomValue< SQInteger >)

    .Overload< SQFloat (*)(void) >(_SC("GetRandomFloat"), &RandomVal< SQFloat >::Get)
    .Overload< SQFloat (*)(SQFloat, SQFloat) >(_SC("GetRandomFloat"), &RandomVal< SQFloat >::Get)

    .Overload< String (*)(String::size_type) >(_SC("GetRandomString"), &GetRandomString)
    .Overload< String (*)(String::size_type, String::value_type, String::value_type) >(_SC("GetRandomString"), &GetRandomString)

    .Func(_SC("GetRandomBool"), &GetRandomBool)
    .Func(_SC("GetRandomColor"), &GetRandomColor)

    .Overload< SQInteger (*)(const SQChar *) >(_SC("SToI"), &StrToInt<SQInteger>)
    .Overload< SQInteger (*)(const SQChar *, SQInt32) >(_SC("SToI"), &StrToInt<SQInteger>)

    .Func(_SC("SToF"), &StrToReal<SQFloat>)
    .Func(_SC("SToB"), &SToB)

    .Func(_SC("GetColor"), &GetColor)
    .Func(_SC("GetCircle"), &GetCircle)
    .Func(_SC("GetAABB"), &GetAABB)
    .Func(_SC("GetColor3"), &GetColor3)
    .Func(_SC("GetColor4"), &GetColor4)
    .Func(_SC("GetQuaternion"), &GetQuaternion)
    .Func(_SC("GetSphere"), &GetSphere)
    .Func(_SC("GetVector2f"), &GetVector2f)
    .Func(_SC("GetVector2i"), &GetVector2i)
    .Func(_SC("GetVector2u"), &GetVector2u)
    .Func(_SC("GetVector3"), &GetVector3)
    .Func(_SC("GetVector4"), &GetVector4)

    .Bind(_SC("Fmt"), Sqrat::Table(vm)
        .Overload< SQS (*)(SQS) >(_SC("Ins"), \
            &InsStr)
        .Overload< SQS (*)(SQS, SQS) >(_SC("Ins"), \
            &InsStr<SQS>)
        .Overload< SQS (*)(SQS, SQS, SQS) >(_SC("Ins"), \
            &InsStr<SQS, SQS>)
        .Overload< SQS (*)(SQS, SQS, SQS, SQS) >(_SC("Ins"), \
            &InsStr<SQS, SQS, SQS>)
        .Overload< SQS (*)(SQS, SQS, SQS, SQS, SQS) >(_SC("Ins"), \
            &InsStr<SQS, SQS, SQS, SQS>)
        .Overload< SQS (*)(SQS, SQS, SQS, SQS, SQS, SQS) >(_SC("Ins"), \
            &InsStr<SQS, SQS, SQS, SQS, SQS>)
        .Overload< SQS (*)(SQS, SQS, SQS, SQS, SQS, SQS, SQS) >(_SC("Ins"), \
            &InsStr<SQS, SQS, SQS, SQS, SQS, SQS>)
        .Overload< SQS (*)(SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS) >(_SC("Ins"), \
            &InsStr<SQS, SQS, SQS, SQS, SQS, SQS, SQS>)
        .Overload< SQS (*)(SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS) >(_SC("Ins"), \
            &InsStr<SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS>)
        .Overload< SQS (*)(SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS) >(_SC("Ins"), \
            &InsStr<SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS>)
        .Overload< SQS (*)(SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS) >(_SC("Ins"), \
            &InsStr<SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS>)
        .Overload< SQS (*)(SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS) >(_SC("Ins"), \
            &InsStr<SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS>)
        .Overload< SQS (*)(SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS) >(_SC("Ins"), \
            &InsStr<SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS>)
        .Overload< SQS (*)(SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS) >(_SC("Ins"), \
            &InsStr<SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS, SQS>)
        .Overload< SQS (*)(SQS, SQChar, SQUint32) >(_SC("Left"), \
            &LeftStr)
        .Overload< SQS (*)(SQS, SQChar, SQUint32, SQUint32) >(_SC("Left"), \
            &LeftStr)
        .Overload< SQS (*)(SQS, SQChar, SQUint32) >(_SC("Right"), \
            &RightStr)
        .Overload< SQS (*)(SQS, SQChar, SQUint32, SQUint32) >(_SC("Right"), \
            &RightStr)
        .Func(_SC("Center"), &CenterStr)
    )

    /* END REGISTRATION STATEMENT */ ;

    // Output debugging information
    LogDbg("Registration of <Base> API was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
