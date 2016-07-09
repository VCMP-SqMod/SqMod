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
#ifndef SQMOD_PLUGIN_API
    #include "Library/Numeric/LongInt.hpp"
    #include <sqstdstring.h>
#endif // SQMOD_PLUGIN_API

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
PluginFuncs*        _Func = nullptr;
PluginCallbacks*    _Clbk = nullptr;
PluginInfo*         _Info = nullptr;

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_PLUGIN_API

    HSQAPI          _SqAPI = nullptr;
    HSQEXPORTS      _SqMod = nullptr;
    HSQUIRRELVM     _SqVM = nullptr;

#endif // SQMOD_PLUGIN_API

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

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
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
    SetConsoleTextAttribute(hstdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
    std::printf("[SQMOD] ");

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
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
    throw Sqrat::Exception(g_Buffer);
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
    return (std::strcmp(buffer, "true") == 0 || std::strcmp(buffer, "yes") == 0 ||
            std::strcmp(buffer, "on") == 0 || std::strcmp(buffer, "1") == 0) ? true : false;
}

// ------------------------------------------------------------------------------------------------
Object & NullObject()
{
    static Object o;
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
    return (std::strcmp(s, "true") == 0) ? true : false;
}

bool ConvNum< bool >::FromStr(CSStr s, Int32 /*base*/)
{
    return (std::strcmp(s, "true") == 0) ? true : false;
}

// --------------------------------------------------------------------------------------------
StackStrF::StackStrF(HSQUIRRELVM vm, SQInteger idx, bool fmt)
    : mPtr(nullptr)
    , mLen(-1)
    , mRes(SQ_OK)
    , mObj()
    , mVM(vm)
{
    const Int32 top = sq_gettop(vm);
    // Reset the converted value object
    sq_resetobject(&mObj);
    // Was the string or value specified?
    if (top <= (idx - 1))
    {
        mRes = sq_throwerror(vm, "Missing string or value");
    }
    // Do we have enough values to call the format function and are we allowed to?
    else if (top > idx && fmt)
    {
        // Pointer to the generated string
        SStr str = nullptr;
        // Attempt to generate the specified string format
        mRes = sqstd_format(vm, idx, &mLen, &str);
        // Did the format succeeded but ended up with a null string pointer?
        if (SQ_SUCCEEDED(mRes) && !str)
        {
            mRes = sq_throwerror(vm, "Unable to generate the string");
        }
        else
        {
            mPtr = const_cast< CSStr >(str);
        }
    }
    // Is the value on the stack an actual string?
    else if (sq_gettype(vm, idx) == OT_STRING)
    {
        // Obtain a reference to the string object
        mRes = sq_getstackobj(vm, idx, &mObj);
        // Could we retrieve the object from the stack?
        if (SQ_SUCCEEDED(mRes))
        {
            // Keep a strong reference to the object
            sq_addref(vm, &mObj);
            // Attempt to retrieve the string value from the stack
            mRes = sq_getstringandsize(vm, idx, &mPtr, &mLen);
        }
        // Did the retrieval succeeded but ended up with a null string pointer?
        if (SQ_SUCCEEDED(mRes) && !mPtr)
        {
            mRes = sq_throwerror(vm, "Unable to retrieve the string");
        }
    }
    // We have to try and convert it to string
    else
    {
        // Attempt to convert the value from the stack to a string
        mRes = sq_tostring(vm, idx);
        // Could we convert the specified value to string?
        if (SQ_SUCCEEDED(mRes))
        {
            // Obtain a reference to the resulted object
            mRes = sq_getstackobj(vm, -1, &mObj);
            // Could we retrieve the object from the stack?
            if (SQ_SUCCEEDED(mRes))
            {
                // Keep a strong reference to the object
                sq_addref(vm, &mObj);
                // Attempt to obtain the string pointer
                mRes = sq_getstringandsize(vm, -1, &mPtr, &mLen);
            }
        }
        // Pop a value from the stack regardless of the result
        sq_pop(vm, 1);
        // Did the retrieval succeeded but ended up with a null string pointer?
        if (SQ_SUCCEEDED(mRes) && !mPtr)
        {
            mRes = sq_throwerror(vm, "Unable to retrieve the value");
        }
    }
}

// ------------------------------------------------------------------------------------------------
StackStrF::~StackStrF()
{
    if (mVM && !sq_isnull(mObj))
    {
        sq_release(mVM, &mObj);
    }
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
    StackStrF val(vm, -1, false);
    // Did the conversion failed?
    if (SQ_FAILED(val.mRes))
    {
        return _SC("unknown");
    }
    // Return the obtained string value
    return String(val.mPtr, val.mLen);
}

// ------------------------------------------------------------------------------------------------
Object BufferToStrObj(const Buffer & b)
{
    // Obtain the initial stack size
    const StackGuard sg(DefaultVM::Get());
    // Push the string onto the stack
    sq_pushstring(DefaultVM::Get(), b.Data(), b.Position());
    // Obtain the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
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
    const StackGuard sg(DefaultVM::Get());
    // Push the string onto the stack
    sq_pushstring(DefaultVM::Get(), b.Data(), size);
    // Obtain the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
Object MakeSLongObj(Int64 value)
{
    // Obtain the default virtual machine
    HSQUIRRELVM vm = DefaultVM::Get();
    // Obtain the initial stack size
    const StackGuard sg(vm);
#ifdef SQMOD_PLUGIN_API
    // Push a long integer instance with the requested value on the stack
    SqMod_PushSLongObject(vm, value);
#else
    // Transform the specified value into a script object
    PushVar< SLongInt >(vm, SLongInt(value));
#endif // SQMOD_PLUGIN_API
    // Obtain the object from the stack and return it
    return Var< Object >(vm, -1).value;
}

// ------------------------------------------------------------------------------------------------
Object MakeULongObj(Uint64 value)
{
    // Obtain the default virtual machine
    HSQUIRRELVM vm = DefaultVM::Get();
    // Obtain the initial stack size
    const StackGuard sg(vm);
#ifdef SQMOD_PLUGIN_API
    // Push a long integer instance with the requested value on the stack
    SqMod_PushULongObject(vm, value);
#else
    // Transform the specified value into a script object
    PushVar< ULongInt >(vm, ULongInt(value));
#endif // SQMOD_PLUGIN_API
    // Obtain the object from the stack and return it
    return Var< Object >(vm, -1).value;
}

// ------------------------------------------------------------------------------------------------
Object MakeSLongObj(HSQUIRRELVM vm, Int64 value)
{
    // Obtain the initial stack size
    const StackGuard sg(vm);
#ifdef SQMOD_PLUGIN_API
    // Push a long integer instance with the requested value on the stack
    SqMod_PushSLongObject(vm, value);
#else
    // Transform the specified value into a script object
    PushVar< SLongInt >(vm, SLongInt(value));
#endif // SQMOD_PLUGIN_API
    // Obtain the object from the stack and return it
    return Var< Object >(vm, -1).value;
}

// ------------------------------------------------------------------------------------------------
Object MakeULongObj(HSQUIRRELVM vm, Uint64 value)
{
    // Obtain the initial stack size
    const StackGuard sg(vm);
#ifdef SQMOD_PLUGIN_API
    // Push a long integer instance with the requested value on the stack
    SqMod_PushULongObject(vm, value);
#else
    // Transform the specified value into a script object
    PushVar< ULongInt >(vm, ULongInt(value));
#endif // SQMOD_PLUGIN_API
    // Obtain the object from the stack and return it
    return Var< Object >(vm, -1).value;
}

// ------------------------------------------------------------------------------------------------
Int64 FetchSLongObjVal(const Object & value)
{
    // Grab the associated object virtual machine
    HSQUIRRELVM vm = value.GetVM();
    // Obtain the initial stack size
    const StackGuard sg(vm);
    // Push the specified object onto the stack
    Var< const Object & >::push(vm, value);
    // Retrieve and return the object value from the stack
    return PopStackSLong(vm, -1);
}

// ------------------------------------------------------------------------------------------------
Uint64 FetchULongObjVal(const Object & value)
{
    // Grab the associated object virtual machine
    HSQUIRRELVM vm = value.GetVM();
    // Obtain the initial stack size
    const StackGuard sg(vm);
    // Push the specified object onto the stack
    Var< const Object & >::push(vm, value);
    // Retrieve and return the object value from the stack
    return PopStackSLong(vm, -1);
}

// ------------------------------------------------------------------------------------------------
SQRESULT FetchDateObjVal(const Object & value, Uint16 & year, Uint8 & month, Uint8 & day)
{
#ifdef SQMOD_PLUGIN_API
    // Grab the associated object virtual machine
    HSQUIRRELVM vm = value.GetVM();
    // Remember the current stack size
    const StackGuard sg(vm);
    // Push the specified object onto the stack
    Var< const Object & >::push(vm, value);
    // Grab the date components from the date instance
    return SqMod_GetDate(vm, -1, &year, &month, &day);
#else
    STHROWF("This method is only available in modules");
    // Should not reach this point
    return SQ_ERROR;
    // Avoid unused parameter warnings
    SQMOD_UNUSED_VAR(value);
    SQMOD_UNUSED_VAR(year);
    SQMOD_UNUSED_VAR(month);
    SQMOD_UNUSED_VAR(day);
#endif // SQMOD_PLUGIN_API
}

// ------------------------------------------------------------------------------------------------
CSStr FetchDateObjStr(const Object & value)
{
#ifdef SQMOD_PLUGIN_API
    static SQChar buffer[32];
    // Grab the associated object virtual machine
    HSQUIRRELVM vm = value.GetVM();
    // Remember the current stack size
    const StackGuard sg(vm);
    // Push the specified object onto the stack
    Var< const Object & >::push(vm, value);
    // Grab the date instance as a string
    const StackStrF val(vm, -1, false);
    // Validate the result
    if (SQ_FAILED(val.mRes))
    {
        return _SC("1000-01-01");
    }
    // Copy the string into the common buffer
    std::strncpy(buffer, val.mPtr, sizeof(buffer));
    // Return the obtained string
    return buffer;
#else
    STHROWF("This method is only available in modules");
    // Should not reach this point
    return nullptr;
    // Avoid unused parameter warnings
    SQMOD_UNUSED_VAR(value);
#endif // SQMOD_PLUGIN_API
}

// ------------------------------------------------------------------------------------------------
SQRESULT FetchTimeObjVal(const Object & value, Uint8 & hour, Uint8 & minute, Uint8 & second)
{
#ifdef SQMOD_PLUGIN_API
    // Grab the associated object virtual machine
    HSQUIRRELVM vm = value.GetVM();
    // Remember the current stack size
    const StackGuard sg(vm);
    // Push the specified object onto the stack
    Var< const Object & >::push(vm, value);
    // Grab the date components from the date instance
    return SqMod_GetTime(vm, -1, &hour, &minute, &second, nullptr);
#else
    STHROWF("This method is only available in modules");
    // Should not reach this point
    return SQ_ERROR;
    // Avoid unused parameter warnings
    SQMOD_UNUSED_VAR(value);
    SQMOD_UNUSED_VAR(hour);
    SQMOD_UNUSED_VAR(minute);
    SQMOD_UNUSED_VAR(second);
#endif // SQMOD_PLUGIN_API
}

// ------------------------------------------------------------------------------------------------
SQRESULT FetchTimeObjVal(const Object & value, Uint8 & hour, Uint8 & minute, Uint8 & second, Uint16 & millisecond)
{
#ifdef SQMOD_PLUGIN_API
    // Grab the associated object virtual machine
    HSQUIRRELVM vm = value.GetVM();
    // Remember the current stack size
    const StackGuard sg(vm);
    // Push the specified object onto the stack
    Var< const Object & >::push(vm, value);
    // Grab the date components from the date instance
    return SqMod_GetTime(vm, -1, &hour, &minute, &second, &millisecond);
#else
    STHROWF("This method is only available in modules");
    // Should not reach this point
    return SQ_ERROR;
    // Avoid unused parameter warnings
    SQMOD_UNUSED_VAR(value);
    SQMOD_UNUSED_VAR(hour);
    SQMOD_UNUSED_VAR(minute);
    SQMOD_UNUSED_VAR(second);
    SQMOD_UNUSED_VAR(millisecond);
#endif // SQMOD_PLUGIN_API
}

// ------------------------------------------------------------------------------------------------
CSStr FetchTimeObjStr(const Object & value)
{
#ifdef SQMOD_PLUGIN_API
    static SQChar buffer[32];
    // Grab the associated object virtual machine
    HSQUIRRELVM vm = value.GetVM();
    // Remember the current stack size
    const StackGuard sg(vm);
    // Push the specified object onto the stack
    Var< const Object & >::push(vm, value);
    // Grab the time instance as a string
    const StackStrF val(vm, -1, false);
    // Validate the result
    if (SQ_FAILED(val.mRes))
    {
        return _SC("00:00:00");
    }
    // Copy the string into the common buffer
    std::strncpy(buffer, val.mPtr, sizeof(buffer));
    // Remove the millisecond part from the string, if any
    buffer[8] = '\0';
    // Return the obtained string
    return buffer;
#else
    STHROWF("This method is only available in modules");
    // Should not reach this point
    return nullptr;
    // Avoid unused parameter warnings
    SQMOD_UNUSED_VAR(value);
#endif // SQMOD_PLUGIN_API
}

// ------------------------------------------------------------------------------------------------
SQRESULT FetchDatetimeObjVal(const Object & value, Uint16 & year, Uint8 & month, Uint8 & day, Uint8 & hour,
                                Uint8 & minute, Uint8 & second)
{
#ifdef SQMOD_PLUGIN_API
    // Grab the associated object virtual machine
    HSQUIRRELVM vm = value.GetVM();
    // Remember the current stack size
    const StackGuard sg(vm);
    // Push the specified object onto the stack
    Var< const Object & >::push(vm, value);
    // Grab the date components from the date instance
    return SqMod_GetDatetime(vm, -1, &year, &month, &day, &hour, &minute, &second, nullptr);
#else
    STHROWF("This method is only available in modules");
    // Should not reach this point
    return SQ_ERROR;
    // Avoid unused parameter warnings
    SQMOD_UNUSED_VAR(value);
    SQMOD_UNUSED_VAR(year);
    SQMOD_UNUSED_VAR(month);
    SQMOD_UNUSED_VAR(day);
    SQMOD_UNUSED_VAR(hour);
    SQMOD_UNUSED_VAR(minute);
    SQMOD_UNUSED_VAR(second);
#endif // SQMOD_PLUGIN_API
}

// ------------------------------------------------------------------------------------------------
SQRESULT FetchDatetimeObjVal(const Object & value, Uint16 & year, Uint8 & month, Uint8 & day, Uint8 & hour,
                                Uint8 & minute, Uint8 & second, Uint16 & millisecond)
{
#ifdef SQMOD_PLUGIN_API
    // Grab the associated object virtual machine
    HSQUIRRELVM vm = value.GetVM();
    // Remember the current stack size
    const StackGuard sg(vm);
    // Push the specified object onto the stack
    Var< const Object & >::push(vm, value);
    // Grab the date components from the date instance
    return SqMod_GetDatetime(vm, -1, &year, &month, &day, &hour, &minute, &second, &millisecond);
#else
    STHROWF("This method is only available in modules");
    // Should not reach this point
    return SQ_ERROR;
    // Avoid unused parameter warnings
    SQMOD_UNUSED_VAR(value);
    SQMOD_UNUSED_VAR(year);
    SQMOD_UNUSED_VAR(month);
    SQMOD_UNUSED_VAR(day);
    SQMOD_UNUSED_VAR(hour);
    SQMOD_UNUSED_VAR(minute);
    SQMOD_UNUSED_VAR(second);
    SQMOD_UNUSED_VAR(millisecond);
#endif // SQMOD_PLUGIN_API
}

// ------------------------------------------------------------------------------------------------
CSStr FetchDatetimeObjStr(const Object & value)
{
#ifdef SQMOD_PLUGIN_API
    static SQChar buffer[32];
    // Grab the associated object virtual machine
    HSQUIRRELVM vm = value.GetVM();
    // Remember the current stack size
    const StackGuard sg(vm);
    // Push the specified object onto the stack
    Var< const Object & >::push(vm, value);
    // Grab the date-time instance as a string
    const StackStrF val(vm, -1, false);
    // Validate the result
    if (SQ_FAILED(val.mRes))
    {
        return _SC("1000-01-01 00:00:00");
    }
    // Copy the string into the common buffer
    std::strncpy(buffer, val.mPtr, sizeof(buffer));
    // Remove the millisecond part from the string, if any
    buffer[19] = '\0';
    // Return the obtained string
    return buffer;
#else
    STHROWF("This method is only available in modules");
    // Should not reach this point
    return nullptr;
    // Avoid unused parameter warnings
    SQMOD_UNUSED_VAR(value);
#endif // SQMOD_PLUGIN_API
}

// ------------------------------------------------------------------------------------------------
SQInteger PopStackInteger(HSQUIRRELVM vm, SQInteger idx)
{
#ifdef SQMOD_PLUGIN_API
    return SqMod_PopStackInteger(vm, idx);
#else
    // Identify which type must be extracted
    switch (sq_gettype(vm, idx))
    {
        case OT_INTEGER:
        {
            SQInteger val;
            sq_getinteger(vm, idx, &val);
            return val;
        } break;
        case OT_FLOAT:
        {
            SQFloat val;
            sq_getfloat(vm, idx, &val);
            return ConvTo< SQInteger >::From(val);
        } break;
        case OT_BOOL:
        {
            SQBool val;
            sq_getbool(vm, idx, &val);
            return static_cast< SQInteger >(val);
        } break;
        case OT_STRING:
        {
            CSStr val = nullptr;
            // Attempt to retrieve and convert the string
            if (SQ_SUCCEEDED(sq_getstring(vm, idx, &val)) && val != nullptr && *val != '\0')
            {
                return ConvTo< SQInteger >::From(std::strtoll(val, nullptr, 10));
            }
        } break;
        case OT_ARRAY:
        case OT_TABLE:
        case OT_CLASS:
        case OT_USERDATA:
        {
            return sq_getsize(vm, idx);
        } break;
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
        } break;
        default: break;
    }
    // Default to 0
    return 0;
#endif // SQMOD_PLUGIN_API
}

// ------------------------------------------------------------------------------------------------
SQFloat PopStackFloat(HSQUIRRELVM vm, SQInteger idx)
{
#ifdef SQMOD_PLUGIN_API
    return SqMod_PopStackFloat(vm, idx);
#else
    // Identify which type must be extracted
    switch (sq_gettype(vm, idx))
    {
        case OT_FLOAT:
        {
            SQFloat val;
            sq_getfloat(vm, idx, &val);
            return val;
        } break;
        case OT_INTEGER:
        {
            SQInteger val;
            sq_getinteger(vm, idx, &val);
            return ConvTo< SQFloat >::From(val);
        } break;
        case OT_BOOL:
        {
            SQBool val;
            sq_getbool(vm, idx, &val);
            return ConvTo< SQFloat >::From(val);
        } break;
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
            }
        } break;
        case OT_ARRAY:
        case OT_TABLE:
        case OT_CLASS:
        case OT_USERDATA:
        {
            return ConvTo< SQFloat >::From(sq_getsize(vm, idx));
        } break;
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
        } break;
        default: break;
    }
    // Default to 0
    return 0.0;
#endif // SQMOD_PLUGIN_API
}

// ------------------------------------------------------------------------------------------------
Int64 PopStackSLong(HSQUIRRELVM vm, SQInteger idx)
{
#ifdef SQMOD_PLUGIN_API
    return SqMod_PopStackSLong(vm, idx);
#else
    // Identify which type must be extracted
    switch (sq_gettype(vm, idx))
    {
        case OT_INTEGER:
        {
            SQInteger val;
            sq_getinteger(vm, idx, &val);
            return static_cast< Int64 >(val);
        } break;
        case OT_FLOAT:
        {
            SQFloat val;
            sq_getfloat(vm, idx, &val);
            return ConvTo< Int64 >::From(val);
        } break;
        case OT_BOOL:
        {
            SQBool val;
            sq_getbool(vm, idx, &val);
            return static_cast< Int64 >(val);
        } break;
        case OT_STRING:
        {
            CSStr val = nullptr;
            // Attempt to retrieve and convert the string
            if (SQ_SUCCEEDED(sq_getstring(vm, idx, &val)) && val != nullptr && *val != '\0')
            {
                return std::strtoll(val, nullptr, 10);
            }
        } break;
        case OT_ARRAY:
        case OT_TABLE:
        case OT_CLASS:
        case OT_USERDATA:
        {
            return static_cast< Int64 >(sq_getsize(vm, idx));
        } break;
        case OT_INSTANCE:
        {
            // Attempt to treat the value as a signed long instance
            try
            {
                return Var< const SLongInt & >(vm, idx).value.GetNum();
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to treat the value as a unsigned long instance
            try
            {
                return ConvTo< Int64 >::From(Var< const ULongInt & >(vm, idx).value.GetNum());
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to get the size of the instance as a fall back
            return static_cast< Int64 >(sq_getsize(vm, idx));
        } break;
        default: break;
    }
    // Default to 0
    return 0;
#endif // SQMOD_PLUGIN_API
}

// ------------------------------------------------------------------------------------------------
Uint64 PopStackULong(HSQUIRRELVM vm, SQInteger idx)
{
#ifdef SQMOD_PLUGIN_API
    return SqMod_PopStackULong(vm, idx);
#else
    // Identify which type must be extracted
    switch (sq_gettype(vm, idx))
    {
        case OT_INTEGER:
        {
            SQInteger val;
            sq_getinteger(vm, idx, &val);
            return ConvTo< Uint64 >::From(val);
        } break;
        case OT_FLOAT:
        {
            SQFloat val;
            sq_getfloat(vm, idx, &val);
            return ConvTo< Uint64 >::From(val);
        } break;
        case OT_BOOL:
        {
            SQBool val;
            sq_getbool(vm, idx, &val);
            return ConvTo< Uint64 >::From(val);
        } break;
        case OT_STRING:
        {
            CSStr val = nullptr;
            // Attempt to retrieve and convert the string
            if (SQ_SUCCEEDED(sq_getstring(vm, idx, &val)) && val != nullptr && *val != '\0')
            {
                return std::strtoull(val, nullptr, 10);
            }
        } break;
        case OT_ARRAY:
        case OT_TABLE:
        case OT_CLASS:
        case OT_USERDATA:
        {
            return ConvTo< Uint64 >::From(sq_getsize(vm, idx));
        } break;
        case OT_INSTANCE:
        {
            // Attempt to treat the value as a signed long instance
            try
            {
                return ConvTo< Uint64 >::From(Var< const SLongInt & >(vm, idx).value.GetNum());
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to treat the value as a unsigned long instance
            try
            {
                return Var< const ULongInt & >(vm, idx).value.GetNum();
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to get the size of the instance as a fall back
            return ConvTo< Uint64 >::From(sq_getsize(vm, idx));
        } break;
        default: break;
    }
    // Default to 0
    return 0;
#endif // SQMOD_PLUGIN_API
}

} // Namespace:: SqMod
