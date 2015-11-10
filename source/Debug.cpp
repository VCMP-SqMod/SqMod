#include "Debug.hpp"
#include "Logger.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdarg>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Debug::Pointer _Dbg = Debug::Inst();

// ------------------------------------------------------------------------------------------------
Debug::Debug()
    : m_VM(NULL)
    , m_Opts(PRINT_ALL | TRACE_ALL)
    , m_Type()
    , m_Func()
    , m_RootName(_SC("root"))
    , m_UnknownType(_SC("unknown"))
    , m_UnknownFunc(_SC("unknown"))
{

}

// ------------------------------------------------------------------------------------------------
Debug::~Debug()
{

}

// ------------------------------------------------------------------------------------------------
void Debug::_Finalizer(Debug * ptr)
{
    delete ptr; /* Assuming 'delete' checks for NULL */
}

// ------------------------------------------------------------------------------------------------
Debug::Pointer Debug::Inst()
{
    if (!_Dbg)
    {
        return Pointer(new Debug(), &Debug::_Finalizer);
    }

    return Pointer(nullptr, &Debug::_Finalizer);
}

// ------------------------------------------------------------------------------------------------
HSQUIRRELVM Debug::GetVM()
{
    return m_VM;
}

// ------------------------------------------------------------------------------------------------
void Debug::SetVM(HSQUIRRELVM vm)
{
    m_VM = vm;
}

// ------------------------------------------------------------------------------------------------
void Debug::PrintTrace(SQInt32 lvl, SQInt32 end)
{
    SQMOD_UNUSED_VAR(lvl);
    SQMOD_UNUSED_VAR(end);
}

// ------------------------------------------------------------------------------------------------
void Debug::PrintCallstack(SQInt32 lvl, SQInt32 end)
{
    SQMOD_UNUSED_VAR(lvl);
    SQMOD_UNUSED_VAR(end);
}

// ------------------------------------------------------------------------------------------------
void Debug::SetInf(const char * type, const char * func)
{
    if (type != NULL)
    {
        m_Type.assign(type);
    }
    else
    {
        m_Type.assign(m_UnknownType);
    }

    if (func != NULL)
    {
        m_Func.assign(func);
    }
    else
    {
        m_Func.assign(m_UnknownFunc);
    }
}

// ------------------------------------------------------------------------------------------------
void Debug::Wrn(const char * type, const char * func, const char * fmt, va_list args)
{
    // Store the information about the source
    SetInf(type, func);
    // Are warning messages enabled?
    if (!(m_Opts & WRN_PRINT))
    {
        return;
    }
    // Send the message
    _Log->Send(Logger::LEVEL_WRN, false, fmt, args);
    // Is trace enabled for warning messages?
    if (!(m_Opts & WRN_TRACE))
    {
        return;
    }
    // Do a traceback of the function call
    InternalTrace();
}

// ------------------------------------------------------------------------------------------------
void Debug::Err(const char * type, const char * func, const char * fmt, va_list args)
{
    // Store the information about the source
    SetInf(type, func);
    // Are warning messages enabled?
    if (!(m_Opts & ERR_PRINT))
    {
        return;
    }
    // Send the message
    _Log->Send(Logger::LEVEL_ERR, false, fmt, args);
    // Is trace enabled for warning messages?
    if (!(m_Opts & ERR_TRACE))
    {
        return;
    }
    // Do a traceback of the function call
    InternalTrace();
}

// ------------------------------------------------------------------------------------------------
void Debug::Ftl(const char * type, const char * func, const char * fmt, va_list args)
{
    // Store the information about the source
    SetInf(type, func);
    // Are warning messages enabled?
    if (!(m_Opts & FTL_PRINT))
    {
        return;
    }
    // Send the message
    _Log->Send(Logger::LEVEL_FTL, false, fmt, args);
    // Is trace enabled for warning messages?
    if (!(m_Opts & FTL_TRACE))
    {
        return;
    }
    // Do a traceback of the function call
    InternalTrace();
}

// ------------------------------------------------------------------------------------------------
void Debug::Wrn(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Wrn(NULL, NULL, fmt, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void Debug::Err(const char * fmt, ...)
{
    SetInf(NULL, NULL);
    va_list args;
    va_start(args, fmt);
    Err(NULL, NULL, fmt, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void Debug::Ftl(const char * fmt, ...)
{
    SetInf(NULL, NULL);
    va_list args;
    va_start(args, fmt);
    Ftl(NULL, NULL, fmt, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void Debug::Wrn(const char * func, const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Wrn(NULL, func, fmt, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void Debug::Err(const char * func, const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Err(NULL, func, fmt, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void Debug::Ftl(const char * func, const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Ftl(NULL, func, fmt, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void Debug::Wrn(const char * type, const char * func, const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Wrn(type, func, fmt, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void Debug::Err(const char * type, const char * func, const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Err(type, func, fmt, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void Debug::Ftl(const char * type, const char * func, const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Ftl(type, func, fmt, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void Debug::InternalTrace()
{
    // Structure that will contain informations about the queried stack level
    SQStackInfos si;
    // Output the base stack information before the actual trace
    if (SQ_SUCCEEDED(sq_stackinfos(m_VM, 1, &si)))
    {
        if (m_Func.at(0) == '@')
        {
            _Log->SInf(" > 1 [ %s:%d > %s::%s ]", si.source ? si.source : _SC("unknown"),
                        si.line, m_Type.c_str(), m_Func.c_str()+1);
        }
        else
        {
            _Log->SInf(" > 1 [ %s:%d > %s::%s(...) ]", si.source ? si.source : _SC("unknown"),
                        si.line, m_Type.c_str(), m_Func.c_str());
        }
    }
    // Keep outputting traceback information for the rest of the function calls
    for (SQInt32 level = 2; SQ_SUCCEEDED(sq_stackinfos(m_VM, level, &si)); ++level)
    {
        _Log->SInf(" > %d [ %s:%d > %s::%s(...) ]", level, si.source ? si.source : _SC("unknown"),
                    si.line, EnvName(level).c_str(), si.funcname ? si.funcname : _SC("unknown"));
    }
}

// ------------------------------------------------------------------------------------------------
String Debug::EnvName(SQInt32 level)
{
    // Obtain the top of the stack
    const SQInteger top = sq_gettop(m_VM);
    // Name of the retrieved local
    const SQChar * name = NULL;
    // Attempt to find the this environment
    for (SQInt32 seq = 0; (name = sq_getlocal(m_VM, level, seq)); ++seq)
    {
        // Is this the `this` environment?
        if (strcmp(name, "this") == 0)
        {
            // Found it!
            break;
        }
        // Pop this useless local from the stack
        sq_pop(m_VM, 1);
    }
    // Have we found anything?
    if (name == NULL)
    {
        // Unable to find the `this` environment
        return m_UnknownType;
    }
    // Push the root table on the stack
    sq_pushroottable(m_VM);
    // See if the `this` environment in the current level is the root table
    if (sq_cmp(m_VM) == 0)
    {
        // Pop everything pushed onto the stack
        sq_pop(m_VM, sq_gettop(m_VM) - top);
        // Return the name specified for the root table
        return m_RootName;
    }
    // Attempt to manually call the _typeof metamethod on the `this` environment
    else if (SQ_SUCCEEDED(sq_typeof(m_VM, -2)))
    {
        // Treat the value returned by the the _typeof metamethod as a string
        Var< String > str(m_VM, -1);
        // Pop everything pushed onto the stack
        sq_pop(m_VM, sq_gettop(m_VM) - top);
        // If it was a valid string then return it as the name
        if (!str.value.empty())
        {
            return str.value;
        }
    }
    // Just pop everything pushed onto the stack
    else
    {
        sq_pop(m_VM, sq_gettop(m_VM) - top);
    }
    // At this point the `this` environment is not recognized
    return m_UnknownType;
}

// ================================================================================================
bool Register_Dbg(HSQUIRRELVM vm)
{
    SQMOD_UNUSED_VAR(vm);
    return true;
}

} // Namespace:: SqMod
