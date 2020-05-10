// ------------------------------------------------------------------------------------------------
#include "Base/Utility.hpp"
#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <ctime>
#include <cfloat>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_OS_WINDOWS
    #include <windows.h>
#endif // SQMOD_OS_WINDOWS

// ------------------------------------------------------------------------------------------------
#include <sqstdstring.h>
#include "Library/Numeric/LongInt.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
PluginFuncs*        _Func = nullptr;
PluginCallbacks*    _Clbk = nullptr;
PluginInfo*         _Info = nullptr;

/* ------------------------------------------------------------------------------------------------
 * Common buffers to reduce memory allocations. To be immediately copied upon return!
*/
static SQChar g_Buffer[4096];
static SQChar g_NumBuf[1024];

// ------------------------------------------------------------------------------------------------
SStr GetTempBuff()
{
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
Uint32 GetTempBuffSize()
{
    return sizeof(g_Buffer);
}

/* --------------------------------------------------------------------------------------------
 * Raw console message output.
*/
static inline void OutputMessageImpl(CCStr msg, va_list args)
{
#ifdef SQMOD_OS_WINDOWS
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csb_before;
    GetConsoleScreenBufferInfo( hstdout, &csb_before);
    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN);
    std::printf("[SQMOD] ");

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); // NOLINT(hicpp-signed-bitwise)
    std::vprintf(msg, args);
    std::puts("");

    SetConsoleTextAttribute(hstdout, csb_before.wAttributes);
#else
    std::printf("\033[21;32m[SQMOD]\033[0m");
    std::vprintf(msg, args);
    std::puts("");
#endif // SQMOD_OS_WINDOWS
}

/* --------------------------------------------------------------------------------------------
 * Raw console error output.
*/
static inline void OutputErrorImpl(CCStr msg, va_list args)
{
#ifdef SQMOD_OS_WINDOWS
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csb_before;
    GetConsoleScreenBufferInfo( hstdout, &csb_before);
    SetConsoleTextAttribute(hstdout, FOREGROUND_RED | FOREGROUND_INTENSITY); // NOLINT(hicpp-signed-bitwise)
    std::printf("[SQMOD] ");

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); // NOLINT(hicpp-signed-bitwise)
    std::vprintf(msg, args);
    std::puts("");

    SetConsoleTextAttribute(hstdout, csb_before.wAttributes);
#else
    std::printf("\033[21;91m[SQMOD]\033[0m");
    std::vprintf(msg, args);
    std::puts("");
#endif // SQMOD_OS_WINDOWS
}

// --------------------------------------------------------------------------------------------
void OutputDebug(CCStr msg, ...)
{
#ifdef _DEBUG
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputMessageImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
#else
    SQMOD_UNUSED_VAR(msg);
#endif
}

// --------------------------------------------------------------------------------------------
void OutputMessage(CCStr msg, ...)
{
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputMessageImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
}

// --------------------------------------------------------------------------------------------
void OutputError(CCStr msg, ...)
{
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputErrorImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void SqThrowF(CSStr str, ...)
{
    // Initialize the argument list
    va_list args;
    va_start (args, str);
    // Write the requested contents
    if (std::vsnprintf(g_Buffer, sizeof(g_Buffer), str, args) < 0)
    {
        // Write a generic message at least
        std::strcpy(g_Buffer, "Unknown error has occurred");
    }
    // Finalize the argument list
    va_end(args);
    // Throw the exception with the resulted message
    throw Sqrat::Exception(g_Buffer); // NOLINT(hicpp-exception-baseclass,cert-err60-cpp)
}

// ------------------------------------------------------------------------------------------------
CSStr FmtStr(CSStr str, ...)
{
    // Initialize the argument list
    va_list args;
    va_start (args, str);
    // Write the requested contents
    if (std::vsnprintf(g_Buffer, sizeof(g_Buffer), str, args) < 0)
    {
        STHROWF("Failed to run the specified string format");
    }
    // Finalize the argument list
    va_end(args);
    // Return the data from the buffer
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
CSStr ToStrF(CSStr str, ...)
{
    // Prepare the arguments list
    va_list args;
    va_start(args, str);
    // Write the requested contents
    if (std::vsnprintf(g_Buffer, sizeof(g_Buffer), str, args) < 0)
    {
        g_Buffer[0] = '\0'; // Make sure the string is null terminated
    }
    // Finalize the argument list
    va_end(args);
    // Return the resulted string
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
CSStr ToStringF(CSStr str, ...)
{
    // Acquire a moderately sized buffer
    Buffer b(128);
    // Prepare the arguments list
    va_list args;
    va_start (args, str);
    // Attempt to run the specified format
    if (b.WriteF(0, str, args) == 0)
    {
        b.At(0) = '\0'; // Make sure the string is null terminated
    }
    // Finalize the argument list
    va_end(args);
    // Return the resulted string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
SQRESULT SqThrowErrorF(HSQUIRRELVM vm, CCStr str, ...)
{
    // Prepare the arguments list
    va_list args;
    va_start(args, str);
    // Write the requested contents
    if (std::vsnprintf(g_Buffer, sizeof(g_Buffer), str, args) < 0)
    {
        return sq_throwerror(vm, _SC("Formatting error occurred while throwing a script error"));
    }
    // Finalize the argument list
    va_end(args);
    // Throw the resulted string
    return sq_throwerror(vm, g_Buffer);
}

// ------------------------------------------------------------------------------------------------
bool SToB(CSStr str)
{
    // Temporary buffer to store the lowercase string
    SQChar buffer[8];
    // The currently processed character
    unsigned i = 0;
    // Convert only the necessary characters to lowercase
    while (i < 7 && *str != '\0')
    {
        buffer[i++] = static_cast< SQChar >(std::tolower(*(str++)));
    }
    // Add the null terminator
    buffer[i] = '\0';
  // Compare the lowercase string and return the result
  return std::strcmp(buffer, "true") == 0 || std::strcmp(buffer, "yes") == 0 ||
         std::strcmp(buffer, "on") == 0 || std::strcmp(buffer, "1") == 0;
}

// ------------------------------------------------------------------------------------------------
StackStrF & DummyStackStrF()
{
    static StackStrF s;
    s.Release();
    return s;
}

// ------------------------------------------------------------------------------------------------
Object & NullObject()
{
    static Object o;
    o.Release();
    return o;
}

// ------------------------------------------------------------------------------------------------
LightObj & NullLightObj()
{
    static LightObj o;
    o.Release();
    return o;
}

// ------------------------------------------------------------------------------------------------
Table & NullTable()
{
    static Table t;
    t.Release();
    return t;
}

// ------------------------------------------------------------------------------------------------
Array & NullArray()
{
    static Array a;
    a.Release();
    return a;
}

// ------------------------------------------------------------------------------------------------
Function & NullFunction()
{
    static Function f;
    f.Release();
    return f;
}

// ------------------------------------------------------------------------------------------------
String & NullString()
{
    static String s;
    s.resize(0);
    return s;
}

// ------------------------------------------------------------------------------------------------
String & StringRef(const SQChar * str)
{
    static String s;
    s.assign(str);
    return s;
}

// ------------------------------------------------------------------------------------------------
CSStr ConvNum< Int8 >::ToStr(Int8 v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%d", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

Int8 ConvNum< Int8 >::FromStr(CSStr s)
{
    return ConvTo< Int8 >::From(std::strtol(s, nullptr, 10));
}

Int8 ConvNum< Int8 >::FromStr(CSStr s, Int32 base)
{
    return ConvTo< Int8 >::From(std::strtol(s, nullptr, base));
}

// ------------------------------------------------------------------------------------------------
CSStr ConvNum< Uint8 >::ToStr(Uint8 v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%u", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

Uint8 ConvNum< Uint8 >::FromStr(CSStr s)
{
    return ConvTo< Uint8 >::From(std::strtoul(s, nullptr, 10));
}

Uint8 ConvNum< Uint8 >::FromStr(CSStr s, Int32 base)
{
    return ConvTo< Uint8 >::From(std::strtoul(s, nullptr, base));
}

// ------------------------------------------------------------------------------------------------
CSStr ConvNum< Int16 >::ToStr(Int16 v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%d", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

Int16 ConvNum< Int16 >::FromStr(CSStr s)
{
    return ConvTo< Int16 >::From(std::strtol(s, nullptr, 10));
}

Int16 ConvNum< Int16 >::FromStr(CSStr s, Int32 base)
{
    return ConvTo< Int16 >::From(std::strtol(s, nullptr, base));
}

// ------------------------------------------------------------------------------------------------
CSStr ConvNum< Uint16 >::ToStr(Uint16 v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%u", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

Uint16 ConvNum< Uint16 >::FromStr(CSStr s)
{
    return ConvTo< Uint16 >::From(std::strtoul(s, nullptr, 10));
}

Uint16 ConvNum< Uint16 >::FromStr(CSStr s, Int32 base)
{
    return ConvTo< Uint16 >::From(std::strtoul(s, nullptr, base));
}

// ------------------------------------------------------------------------------------------------
CSStr ConvNum< Int32 >::ToStr(Int32 v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%d", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

Int32 ConvNum< Int32 >::FromStr(CSStr s)
{
    return ConvTo< Int32 >::From(std::strtol(s, nullptr, 10));
}

Int32 ConvNum< Int32 >::FromStr(CSStr s, Int32 base)
{
    return ConvTo< Int32 >::From(std::strtol(s, nullptr, base));
}

// ------------------------------------------------------------------------------------------------
CSStr ConvNum< Uint32 >::ToStr(Uint32 v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%u", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

Uint32 ConvNum< Uint32 >::FromStr(CSStr s)
{
    return ConvTo< Uint32 >::From(std::strtoul(s, nullptr, 10));
}

Uint32 ConvNum< Uint32 >::FromStr(CSStr s, Int32 base)
{
    return ConvTo< Uint32 >::From(std::strtoul(s, nullptr, base));
}

// ------------------------------------------------------------------------------------------------
CSStr ConvNum< Int64 >::ToStr(Int64 v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%lld", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

Int64 ConvNum< Int64 >::FromStr(CSStr s)
{
    return std::strtoll(s, nullptr, 10);
}

Int64 ConvNum< Int64 >::FromStr(CSStr s, Int32 base)
{
    return std::strtoll(s, nullptr, base);
}

// ------------------------------------------------------------------------------------------------
CSStr ConvNum< Uint64 >::ToStr(Uint64 v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%llu", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

Uint64 ConvNum< Uint64 >::FromStr(CSStr s)
{
    return std::strtoull(s, nullptr, 10);
}

Uint64 ConvNum< Uint64 >::FromStr(CSStr s, Int32 base)
{
    return std::strtoull(s, nullptr, base);
}

// ------------------------------------------------------------------------------------------------
CSStr ConvNum< LongI >::ToStr(LongI v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%ld", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

LongI ConvNum< LongI >::FromStr(CSStr s)
{
    return std::strtol(s, nullptr, 10);
}

LongI ConvNum< LongI >::FromStr(CSStr s, Int32 base)
{
    return std::strtol(s, nullptr, base);
}

// ------------------------------------------------------------------------------------------------
CSStr ConvNum< Ulong >::ToStr(Ulong v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%lu", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

Ulong ConvNum< Ulong >::FromStr(CSStr s)
{
    return std::strtoul(s, nullptr, 10);
}

Ulong ConvNum< Ulong >::FromStr(CSStr s, Int32 base)
{
    return std::strtoul(s, nullptr, base);
}

// ------------------------------------------------------------------------------------------------
CSStr ConvNum< Float32 >::ToStr(Float32 v)
{
    // Attempt to convert the value to a string
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%f", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the data from the buffer
    return g_NumBuf;
}

Float32 ConvNum< Float32 >::FromStr(CSStr s)
{
    return std::strtof(s, nullptr);
}

Float32 ConvNum< Float32 >::FromStr(CSStr s, Int32 /*base*/)
{
    return std::strtof(s, nullptr);
}

// ------------------------------------------------------------------------------------------------
CSStr ConvNum< Float64 >::ToStr(Float64 v)
{
    // Attempt to convert the value to a string
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%f", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the data from the buffer
    return g_NumBuf;
}

Float64 ConvNum< Float64 >::FromStr(CSStr s)
{
    return std::strtod(s, nullptr);
}

Float64 ConvNum< Float64 >::FromStr(CSStr s, Int32 /*base*/)
{
    return std::strtod(s, nullptr);
}

// ------------------------------------------------------------------------------------------------
CSStr ConvNum< bool >::ToStr(bool v)
{
    if (v)
    {
        g_NumBuf[0] = 't';
        g_NumBuf[1] = 'r';
        g_NumBuf[2] = 'u';
        g_NumBuf[3] = 'e';
        g_NumBuf[4] = '\0';
    }
    else
    {
        g_NumBuf[0] = 'f';
        g_NumBuf[1] = 'a';
        g_NumBuf[2] = 'l';
        g_NumBuf[3] = 's';
        g_NumBuf[4] = 'e';
        g_NumBuf[5] = '\0';
    }
    return g_NumBuf;
}

bool ConvNum< bool >::FromStr(CSStr s)
{
    return std::strcmp(s, "true") == 0;
}

bool ConvNum< bool >::FromStr(CSStr s, Int32 /*base*/)
{
    return std::strcmp(s, "true") == 0;
}

// ------------------------------------------------------------------------------------------------
CSStr SqTypeName(SQObjectType type)
{
    switch (type)
    {
        case OT_NULL:           return _SC("null");
        case OT_INTEGER:        return _SC("integer");
        case OT_FLOAT:          return _SC("float");
        case OT_BOOL:           return _SC("bool");
        case OT_STRING:         return _SC("string");
        case OT_TABLE:          return _SC("table");
        case OT_ARRAY:          return _SC("array");
        case OT_USERDATA:       return _SC("userdata");
        case OT_CLOSURE:        return _SC("closure");
        case OT_NATIVECLOSURE:  return _SC("nativeclosure");
        case OT_GENERATOR:      return _SC("generator");
        case OT_USERPOINTER:    return _SC("userpointer");
        case OT_THREAD:         return _SC("thread");
        case OT_FUNCPROTO:      return _SC("funcproto");
        case OT_CLASS:          return _SC("class");
        case OT_INSTANCE:       return _SC("instance");
        case OT_WEAKREF:        return _SC("weakref");
        case OT_OUTER:          return _SC("outer");
        default:                return _SC("unknown");
    }
}

// ------------------------------------------------------------------------------------------------
String SqTypeName(HSQUIRRELVM vm, SQInteger idx)
{
    // Remember the current stack size
    const StackGuard sg(vm);
    // Attempt to retrieve the type name of the specified value
    if (SQ_FAILED(sq_typeof(vm, idx)))
    {
        return _SC("unknown");
    }
    // Attempt to convert the obtained value to a string
    StackStrF val(vm, -1);
    // Did the conversion failed?
    if (SQ_FAILED(val.Proc(false)))
    {
        return _SC("unknown");
    }
    // Return the obtained string value
    return String(val.mPtr, static_cast< size_t >(val.mLen));
}

// ------------------------------------------------------------------------------------------------
Object BufferToStrObj(const Buffer & b)
{
    // Obtain the initial stack size
    const StackGuard sg(SqVM());
    // Push the string onto the stack
    sq_pushstring(SqVM(), b.Data(), b.Position());
    // Obtain the object from the stack and return it
    return Var< Object >(SqVM(), -1).value;
}

// --------------------------------------------------------------------------------------------
Object BufferToStrObj(const Buffer & b, Uint32 size)
{
    // Perform a range check on the specified buffer
    if (size > b.Capacity())
    {
        STHROWF("The specified buffer size is out of range: %u >= %u", size, b.Capacity());
    }
    // Obtain the initial stack size
    const StackGuard sg(SqVM());
    // Push the string onto the stack
    sq_pushstring(SqVM(), b.Data(), size);
    // Obtain the object from the stack and return it
    return Var< Object >(SqVM(), -1).value;
}

// ------------------------------------------------------------------------------------------------
SQInteger PopStackInteger(HSQUIRRELVM vm, SQInteger idx)
{
    // Identify which type must be extracted
    switch (sq_gettype(vm, idx))
    {
        case OT_INTEGER:
        {
            SQInteger val;
            sq_getinteger(vm, idx, &val);
            return val;
        }
        case OT_FLOAT:
        {
            SQFloat val;
            sq_getfloat(vm, idx, &val);
            return ConvTo< SQInteger >::From(val);
        }
        case OT_BOOL:
        {
            SQBool val;
            sq_getbool(vm, idx, &val);
            return static_cast< SQInteger >(val);
        }
        case OT_STRING:
        {
            CSStr val = nullptr;
            // Attempt to retrieve and convert the string
            if (SQ_SUCCEEDED(sq_getstring(vm, idx, &val)) && val != nullptr && *val != '\0')
            {
                return ConvTo< SQInteger >::From(std::strtoll(val, nullptr, 10));
            } else break;
        }
        case OT_ARRAY:
        case OT_TABLE:
        case OT_CLASS:
        case OT_USERDATA:
        {
            return sq_getsize(vm, idx);
        }
        case OT_INSTANCE:
        {
            // Attempt to treat the value as a signed long instance
            try
            {
                return ConvTo< SQInteger >::From(Var< const SLongInt & >(vm, idx).value.GetNum());
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to treat the value as a unsigned long instance
            try
            {
                return ConvTo< SQInteger >::From(Var< const ULongInt & >(vm, idx).value.GetNum());
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to get the size of the instance as a fall back
            return sq_getsize(vm, idx);
        }
        default: break;
    }
    // Default to 0
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQFloat PopStackFloat(HSQUIRRELVM vm, SQInteger idx)
{
    // Identify which type must be extracted
    switch (sq_gettype(vm, idx))
    {
        case OT_FLOAT:
        {
            SQFloat val;
            sq_getfloat(vm, idx, &val);
            return val;
        }
        case OT_INTEGER:
        {
            SQInteger val;
            sq_getinteger(vm, idx, &val);
            return ConvTo< SQFloat >::From(val);
        }
        case OT_BOOL:
        {
            SQBool val;
            sq_getbool(vm, idx, &val);
            return ConvTo< SQFloat >::From(val);
        }
        case OT_STRING:
        {
            CSStr val = nullptr;
            // Attempt to retrieve and convert the string
            if (SQ_SUCCEEDED(sq_getstring(vm, idx, &val)) && val != nullptr && *val != '\0')
            {
#ifdef SQUSEDOUBLE
                return std::strtod(val, nullptr);
#else
                return std::strtof(val, nullptr);
#endif // SQUSEDOUBLE
            } else break;
        }
        case OT_ARRAY:
        case OT_TABLE:
        case OT_CLASS:
        case OT_USERDATA:
        {
            return ConvTo< SQFloat >::From(sq_getsize(vm, idx));
        }
        case OT_INSTANCE:
        {
            // Attempt to treat the value as a signed long instance
            try
            {
                return ConvTo< SQFloat >::From(Var< const SLongInt & >(vm, idx).value.GetNum());
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to treat the value as a unsigned long instance
            try
            {
                return ConvTo< SQFloat >::From(Var< const ULongInt & >(vm, idx).value.GetNum());
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to get the size of the instance as a fall back
            return ConvTo< SQFloat >::From(sq_getsize(vm, idx));
        }
        default: break;
    }
    // Default to 0
    return 0.0;
}

} // Namespace:: SqMod
