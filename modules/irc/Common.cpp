// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger GetNick(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
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
SQInteger GetHost(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
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
SQInteger StripColorFromMIRC(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
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
SQInteger ConvertColorFromMIRC(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
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
SQInteger ConvertColorToMIRC(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Attempt to convert the colors
    CStr str = irc_color_convert_to_mirc(val.mPtr);
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

} // Namespace:: SqMod
