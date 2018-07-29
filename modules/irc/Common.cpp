// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void * IrcAllocMem(size_t n)
{
    // Does the requested size exceed the common shared buffer size?
    if (n > GetTempBuffSize())
    {
        return std::malloc(n);
    }
    // Return the common shared buffer
    return GetTempBuff();
}

// ------------------------------------------------------------------------------------------------
void IrcFreeMem(void * p)
{
    // Only release if it's not the temporary buffer
    if ((p < GetTempBuff()) || (p > (GetTempBuff() + GetTempBuffSize())))
    {
        std::free(p);
    }
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqGetNick(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Attempt to retrieve the nickname
    irc_target_get_nick(val.mPtr, GetTempBuff(), GetTempBuffSize());
    // Push the resulted value on the stack
    sq_pushstring(vm, GetTempBuff(), -1);
    // Specify that this function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqGetHost(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Attempt to retrieve the host
    irc_target_get_host(val.mPtr, GetTempBuff(), GetTempBuffSize());
    // Push the resulted value on the stack
    sq_pushstring(vm, GetTempBuff(), -1);
    // Specify that this function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqStripColorFromMIRC(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Attempt to strip the colors
    CStr str = irc_color_strip_from_mirc(val.mPtr);
    // Could the IRC library allocate memory?
    if (!str)
    {
        return sq_throwerror(vm, _SC("Unable to allocate memory"));
    }
    // Push the resulted value on the stack
    sq_pushstring(vm, str, -1);
    // Free the memory allocated by the IRC library
    std::free(str);
    // Specify that this function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqConvertColorFromMIRC(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Attempt to convert the colors
    CStr str = irc_color_convert_from_mirc(val.mPtr);
    // Could the IRC library allocate memory?
    if (!str)
    {
        return sq_throwerror(vm, _SC("Unable to allocate memory"));
    }
    // Push the resulted value on the stack
    sq_pushstring(vm, str, -1);
    // Free the memory allocated by the IRC library
    std::free(str);
    // Specify that this function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqConvertColorToMIRC(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Attempt to convert the colors
    CStr str = irc_color_convert_to_mirc(val.mPtr, IrcAllocMem);
    // Could the IRC library allocate memory?
    if (!str)
    {
        return sq_throwerror(vm, _SC("Unable to allocate memory"));
    }
    // Push the resulted value on the stack
    sq_pushstring(vm, str, -1);
    // Free the memory allocated by the IRC library
    IrcFreeMem(str);
    // Specify that this function returned a value
    return 1;
}

// ================================================================================================
void Register_Common(Table & ircns)
{
    ircns.Func(_SC("GetErrStr"), &irc_strerror);
    ircns.SquirrelFunc(_SC("GetNick"), &SqGetNick);
    ircns.SquirrelFunc(_SC("GetHost"), &SqGetHost);
    ircns.SquirrelFunc(_SC("StripColorFromMIRC"), &SqStripColorFromMIRC);
    ircns.SquirrelFunc(_SC("ConvertColorFromMIRC"), &SqConvertColorFromMIRC);
    ircns.SquirrelFunc(_SC("ConvertColorToMIRC"), &SqConvertColorToMIRC);
}

} // Namespace:: SqMod
