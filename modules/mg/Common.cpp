// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstring>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static SQChar g_Buffer[4096]; // Common buffer to reduce memory allocations.

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

// ------------------------------------------------------------------------------------------------
void SqThrowF(CSStr str, ...)
{
    // Initialize the argument list
    va_list args;
    va_start (args, str);
    // Write the requested contents
    if (std::vsnprintf(g_Buffer, sizeof(g_Buffer), str, args) < 0)
    {
        std::strcpy(g_Buffer, "Unknown error has occurred");
    }
    // Release the argument list
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
        g_Buffer[0] = '\0'; // Make sure the string is terminated
    }
    // Release the argument list
    va_end(args);
    // Return the data from the buffer
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
StackGuard::StackGuard()
    : m_VM(_SqVM), m_Top(sq_gettop(m_VM))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
StackGuard::StackGuard(HSQUIRRELVM vm)
    : m_VM(vm), m_Top(sq_gettop(vm))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
StackGuard::~StackGuard()
{
    sq_pop(m_VM, sq_gettop(m_VM) - m_Top);
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
            mRes = sq_getstring(vm, idx, &mPtr);
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
                mRes = sq_getstring(vm, -1, &mPtr);
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

} // Namespace:: SqMod
