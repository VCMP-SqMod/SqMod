// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"
#include "Core/Buffer.hpp"
#include "Core/Utility.hpp"
#include "Library/Numeric/Long.hpp"

// ------------------------------------------------------------------------------------------------
#include <cerrno>
#include <cstdarg>

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_OS_WINDOWS
    #include <windows.h>
#endif // SQMOD_OS_WINDOWS

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
PluginFuncs *       _Func = nullptr; //NOLINT(bugprone-reserved-identifier)
PluginCallbacks *   _Clbk = nullptr; //NOLINT(bugprone-reserved-identifier)
PluginInfo *        _Info = nullptr; //NOLINT(bugprone-reserved-identifier)

/* --------------------------------------------------------------------------------------------
 * Raw console message output.
*/
static inline void OutputMessageImpl(const char * msg, va_list args)
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
static inline void OutputErrorImpl(const char * msg, va_list args)
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
void OutputDebug(const char * msg, ...)
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
void OutputMessage(const char * msg, ...)
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
void OutputError(const char * msg, ...)
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
void SqThrowLastF(const SQChar * msg, ...)
{
    // Acquire a moderately sized buffer
    Buffer b(128);
    // Prepare the arguments list
    va_list args;
    va_start (args, msg);
    // Attempt to run the specified format
    if (b.WriteF(0, msg, args) == 0)
    {
        b.At(0) = '\0'; // Make sure the string is null terminated
    }
    // Finalize the argument list
    va_end(args);
#ifdef SQMOD_OS_WINDOWS
    // Get the error message, if any.
    const DWORD error_num = ::GetLastError();
    // Was there an error recorded?
    if(error_num == 0)
    {
        // Invoker is responsible for making sure this doesn't happen!
        SqThrowF(fmt::runtime("{} [Unknown error]"), b.Data());
    }
    // The resulted message buffer
    LPSTR msg_buff = nullptr;
    // Attempt to obtain the error message
    const std::size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, // NOLINT(hicpp-signed-bitwise)
        nullptr, error_num, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // NOLINT(hicpp-signed-bitwise)
        reinterpret_cast< LPSTR >(&msg_buff), 0, nullptr);
    // Copy the message buffer before freeing it
    std::string message(msg_buff, size);
    //Free the message buffer
    LocalFree(msg_buff);
    // Now it's safe to throw the error
    SqThrowF(fmt::runtime("{} [{}]"), b.Data(), message);
#else
    SqThrowF(fmt::runtime("{} [{}]"), b.Data(), std::strerror(errno));
#endif // SQMOD_OS_WINDOWS
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
const SQChar * SqTypeName(SQObjectType type)
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
LightObj BufferToStrObj(const Buffer & b)
{
    // Obtain the initial stack size
    const StackGuard sg(SqVM());
    // Push the string onto the stack
    sq_pushstring(SqVM(), b.Data(), b.Position());
    // Obtain the object from the stack and return it
    return Var< LightObj >(SqVM(), -1).value;
}

// --------------------------------------------------------------------------------------------
LightObj BufferToStrObj(const Buffer & b, uint32_t size)
{
    // Perform a range check on the specified buffer
    if (size > b.Capacity())
    {
        STHROWF("The specified buffer size is out of range: {} >= {}", size, b.Capacity());
    }
    // Obtain the initial stack size
    const StackGuard sg(SqVM());
    // Push the string onto the stack
    sq_pushstring(SqVM(), b.Data(), size);
    // Obtain the object from the stack and return it
    return Var< LightObj >(SqVM(), -1).value;
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
            const SQChar * val = nullptr;
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
            SQUserPointer tag;
            // Attempt to retrieve the type tag
            if (SQ_FAILED(sq_gettypetag(vm, -1, &tag)))
            {
                break;
            }
            // Is the instance SLongInt? (signed long)
            else if (static_cast< AbstractStaticClassData * >(tag) == StaticClassTypeTag< SLongInt >::Get())
            {
                try
                {
                    return ConvTo< SQInteger >::From(Var< const SLongInt & >(vm, idx).value.GetNum());
                }
                catch (...)
                {
                    // Just ignore it...
                }
            }
            // Is the instance ULongInt? (unsigned long)
            else if (static_cast< AbstractStaticClassData * >(tag) == StaticClassTypeTag< ULongInt >::Get())
            {
                try
                {
                    return ConvTo< SQInteger >::From(Var< const ULongInt & >(vm, idx).value.GetNum());
                }
                catch (...)
                {
                    // Just ignore it...
                }
            }
            else
            {
                // Attempt to get the size of the instance as a fall back
                return sq_getsize(vm, idx);
            }
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
            const SQChar * val = nullptr;
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
            SQUserPointer tag;
            // Attempt to retrieve the type tag
            if (SQ_FAILED(sq_gettypetag(vm, -1, &tag)))
            {
                break;
            }
            // Is the instance SLongInt? (signed long)
            else if (static_cast< AbstractStaticClassData * >(tag) == StaticClassTypeTag< SLongInt >::Get())
            {
                try
                {
                    return ConvTo< SQFloat >::From(Var< const SLongInt & >(vm, idx).value.GetNum());
                }
                catch (...)
                {
                    // Just ignore it...
                }
            }
            // Is the instance ULongInt? (unsigned long)
            else if (static_cast< AbstractStaticClassData * >(tag) == StaticClassTypeTag< ULongInt >::Get())
            {
                try
                {
                    return ConvTo< SQFloat >::From(Var< const ULongInt & >(vm, idx).value.GetNum());
                }
                catch (...)
                {
                    // Just ignore it...
                }
            }
            else
            {
                // Attempt to get the size of the instance as a fall back
                return ConvTo< SQFloat >::From(sq_getsize(vm, idx));
            }
        }
        default: break;
    }
    // Default to 0
    return 0.0;
}

// ------------------------------------------------------------------------------------------------
bool SToB(const SQChar * str)
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

} // Namespace:: SqMod
