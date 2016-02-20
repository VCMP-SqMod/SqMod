// ------------------------------------------------------------------------------------------------
#include "Logger.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include <time.h>
#include <stdio.h>
#include <stdarg.h>

// ------------------------------------------------------------------------------------------------
namespace {

#ifdef SQMOD_OS_WINDOWS

// ------------------------------------------------------------------------------------------------
#include <Windows.h>

/* ------------------------------------------------------------------------------------------------
 * ...
*/
enum
{
    LC_NORMAL           = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    LC_WHITE            = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    LC_GRAY             = FOREGROUND_INTENSITY,
    LC_LIGHT_BLUE       = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    LC_DARK_BLUE        = FOREGROUND_BLUE,
    LC_LIGHT_GREEN      = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    LC_DARK_GREEN       = FOREGROUND_GREEN,
    LC_LIGHT_CYAN       = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    LC_DARK_CYAN        = FOREGROUND_GREEN | FOREGROUND_BLUE,
    LC_LIGHT_YELLOW     = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    LC_DARK_YELLOW      = FOREGROUND_RED | FOREGROUND_GREEN,
    LC_LIGHT_RED        = FOREGROUND_INTENSITY | FOREGROUND_RED,
    LC_DARK_RED         = FOREGROUND_RED,
    LC_LIGHT_MAGENTA    = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    LC_DARK_MAGENTA     = FOREGROUND_RED | FOREGROUND_BLUE
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
enum
{
    LC_DBG  = LC_LIGHT_BLUE,
    LC_USER  = LC_GRAY,
    LC_SCS  = LC_LIGHT_GREEN,
    LC_INF  = LC_LIGHT_CYAN,
    LC_WRN  = LC_LIGHT_YELLOW,
    LC_ERR  = LC_LIGHT_MAGENTA,
    LC_FTL  = LC_LIGHT_RED
};

#endif // SQMOD_OS_WINDOWS

} // Namespace::

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// --------------------------------------------------------------------------------------------
Logger * _Log = NULL;

/* ------------------------------------------------------------------------------------------------
 * ...
*/
inline CCStr GetLevelTag(Uint8 type)
{
    switch (type)
    {
        case LL_DBG: return "[DBG]";
        case LL_USR: return "[USR]";
        case LL_SCS: return "[SCS]";
        case LL_INF: return "[INF]";
        case LL_WRN: return "[WRN]";
        case LL_ERR: return "[ERR]";
        case LL_FTL: return "[FTL]";
        default: return "[UNK]";
    }
}

#ifdef SQMOD_OS_WINDOWS

/* ------------------------------------------------------------------------------------------------
 * ...
*/
inline Uint16 GetLevelColor(Uint8 type)
{
    switch (type)
    {
        case LL_DBG: return LC_DBG;
        case LL_USR: return LC_USER;
        case LL_SCS: return LC_SCS;
        case LL_INF: return LC_INF;
        case LL_WRN: return LC_WRN;
        case LL_ERR: return LC_ERR;
        case LL_FTL: return LC_FTL;
        default: return LC_NORMAL;
    }
}
#else

/* ------------------------------------------------------------------------------------------------
 * ...
*/
inline CCStr GetLevelColor(Uint8 type)
{
    return g_EmptyStr;
}

#endif // SQMOD_OS_WINDOWS

// ------------------------------------------------------------------------------------------------
Logger::Logger()
    : m_Buffer(4096)
    , m_TmBuff()
    , m_Levels(LL_ANY)
    , m_Time(false)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Logger::~Logger()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
void Logger::Send(Uint8 type, bool sub, CCStr fmt, va_list args)
{
    if (!(m_Levels & type))
        return;
    m_Buffer.WriteF(0, fmt, args);
    Proccess(type, sub);
}

// ------------------------------------------------------------------------------------------------
void Logger::Message(Uint8 type, bool sub, CCStr fmt, ...)
{
    if (!(m_Levels & type))
        return;
    va_list args;
    va_start(args, fmt);
    m_Buffer.WriteF(0, fmt, args);
    Proccess(type, sub);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void Logger::Throw(CCStr fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    m_Buffer.WriteF(0, fmt, args);
    va_end(args);
    Error::Throw(DefaultVM::Get(), m_Buffer.Data());
}

void Logger::Throw(CCStr fmt, va_list args)
{
    m_Buffer.WriteF(0, fmt, args);
    Error::Throw(DefaultVM::Get(), m_Buffer.Data());
}

// ------------------------------------------------------------------------------------------------
void Logger::Debug(CCStr fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Debug(fmt, args);
    va_end(args);
}

void Logger::Debug(CCStr fmt, va_list args)
{
    HSQUIRRELVM vm = DefaultVM::Get();
    SQStackInfos si;
    Int32 ret = m_Buffer.WriteF(0, fmt, args);

    if (SQ_SUCCEEDED(sq_stackinfos(vm, 1, &si)))
        m_Buffer.WriteF(ret, "\n[\n=>Location: %s\n=>Line: %d\n=>Function: %s\n]"
                , si.source ? si.source : _SC("unknown")
                , si.line
                , si.funcname ? si.funcname : _SC("unknown"));
    else
        m_Buffer.WriteF(ret, "\n[\n=>Location: unknown\n=>Line: unknown\n=>Function: unknown\n]");

    Proccess(LL_ERR, true);

    ret = m_Buffer.WriteF(0, "Traceback:\n[\n");

    for (Int32 level = 1; SQ_SUCCEEDED(sq_stackinfos(vm, level, &si)); ++level)
    {
        ret += m_Buffer.WriteF(ret, "=> [%d] %s (%d) [%s]\n", level
                                    , si.source ? si.source : _SC("unknown")
                                    , si.line
                                    , si.funcname ? si.funcname : _SC("unknown"));
    }

    m_Buffer.WriteF(ret, "]");
    Proccess(LL_INF, true);

    CCStr s_ = 0, name = 0;
    SQInteger i_, seq = 0;
    SQFloat f_;
    SQUserPointer p_;

    ret = m_Buffer.WriteF(0, "Locals:\n[\n");

    for (Int32 level = 0; level < 10; level++)
    {
        seq = 0;
        while((name = sq_getlocal(vm, level, seq)))
        {
            ++seq;
            switch(sq_gettype(vm, -1))
            {
                case OT_NULL:
                    ret += m_Buffer.WriteF(ret, "=> [%d] NULL [%s] : ...\n", level, name);
                    break;
                case OT_INTEGER:
                    sq_getinteger(vm, -1, &i_);
                    ret += m_Buffer.WriteF(ret, "=> [%d] INTEGER [%s] : %d\n", level, name, i_);
                    break;
                case OT_FLOAT:
                    sq_getfloat(vm, -1, &f_);
                    ret += m_Buffer.WriteF(ret, "=> [%d] FLOAT [%s] : %f\n", level, name, f_);
                    break;
                case OT_USERPOINTER:
                    sq_getuserpointer(vm, -1, &p_);
                    ret += m_Buffer.WriteF(ret, "=> [%d] USERPOINTER [%s] : %p\n", level, name, p_);
                    break;
                case OT_STRING:
                    sq_getstring(vm, -1, &s_);
                    ret += m_Buffer.WriteF(ret, "=> [%d] STRING [%s] : %s\n", level, name, s_);
                    break;
                case OT_TABLE:
                    ret += m_Buffer.WriteF(ret, "=> [%d] TABLE [%s] : ...\n", level, name);
                    break;
                case OT_ARRAY:
                    ret += m_Buffer.WriteF(ret, "=> [%d] ARRAY [%s] : ...\n", level, name);
                    break;
                case OT_CLOSURE:
                    ret += m_Buffer.WriteF(ret, "=> [%d] CLOSURE [%s] : ...\n", level, name);
                    break;
                case OT_NATIVECLOSURE:
                    ret += m_Buffer.WriteF(ret, "=> [%d] NATIVECLOSURE [%s] : ...\n", level, name);
                    break;
                case OT_GENERATOR:
                    ret += m_Buffer.WriteF(ret, "=> [%d] GENERATOR [%s] : ...\n", level, name);
                    break;
                case OT_USERDATA:
                    ret += m_Buffer.WriteF(ret, "=> [%d] USERDATA [%s] : ...\n", level, name);
                    break;
                case OT_THREAD:
                    ret += m_Buffer.WriteF(ret, "=> [%d] THREAD [%s] : ...\n", level, name);
                    break;
                case OT_CLASS:
                    ret += m_Buffer.WriteF(ret, "=> [%d] CLASS [%s] : ...\n", level, name);
                    break;
                case OT_INSTANCE:
                    ret += m_Buffer.WriteF(ret, "=> [%d] INSTANCE [%s] : ...\n", level, name);
                    break;
                case OT_WEAKREF:
                    ret += m_Buffer.WriteF(ret, "=> [%d] WEAKREF [%s] : ...\n", level, name);
                    break;
                case OT_BOOL:
                    sq_getinteger(vm, -1, &i_);
                    ret += m_Buffer.WriteF(ret, "=> [%d] BOOL [%s] : %s\n", level, name, i_ ? _SC("true") : _SC("false"));
                    break;
                default:
                    ret += m_Buffer.WriteF(ret, "=> [%d] UNKNOWN [%s] : ...\n", level, name);
                break;
            }
            sq_pop(vm, 1);
        }
    }

    m_Buffer.WriteF(ret, "]");
    Proccess(LL_INF, true);
}

// ------------------------------------------------------------------------------------------------
void Logger::Proccess(Uint8 type, bool sub)
{
    if (m_Time)
    {
        time_t rawtime;
        struct tm * timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(m_TmBuff, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    }
    else
        m_TmBuff[0] = 0;
#ifdef SQMOD_OS_WINDOWS
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csb_state;
    GetConsoleScreenBufferInfo(hstdout, &csb_state);
    SetConsoleTextAttribute(hstdout, GetLevelColor(type));
    if (m_Time)
        printf("%s %s ", GetLevelTag(type), m_TmBuff);
    else
        printf("%s ", GetLevelTag(type));
    SetConsoleTextAttribute(hstdout, sub ? LC_NORMAL : LC_WHITE);
    puts(m_Buffer.Data());
    SetConsoleTextAttribute(hstdout, csb_state.wAttributes);
#else
    if (m_Time)
        printf("%s %s ", GetLevelTag(type), m_TmBuff);
    else
        printf("%s ", GetLevelTag(type));
    puts(m_Buffer.Data());
#endif // SQMOD_OS_WINDOWS
}

// ------------------------------------------------------------------------------------------------
#define SQMOD_LOG(N_, L_, S_) /*
*/ void N_(CCStr fmt, ...) /*
*/ { /*
*/  va_list args; /*
*/  va_start(args, fmt); /*
*/  if (_Log) /*
*/   _Log->Send(L_, S_, fmt, args); /*
*/  else /*
*/   vprintf(fmt, args); /*
*/  va_end(args); /*
*/ } /*
*/

// ------------------------------------------------------------------------------------------------
SQMOD_LOG(LogDbg, LL_DBG, false)
SQMOD_LOG(LogUsr, LL_USR, false)
SQMOD_LOG(LogScs, LL_SCS, false)
SQMOD_LOG(LogInf, LL_INF, false)
SQMOD_LOG(LogWrn, LL_WRN, false)
SQMOD_LOG(LogErr, LL_ERR, false)
SQMOD_LOG(LogFtl, LL_FTL, false)

// ------------------------------------------------------------------------------------------------
SQMOD_LOG(LogSDbg, LL_DBG, true)
SQMOD_LOG(LogSUsr, LL_USR, true)
SQMOD_LOG(LogSScs, LL_SCS, true)
SQMOD_LOG(LogSInf, LL_INF, true)
SQMOD_LOG(LogSWrn, LL_WRN, true)
SQMOD_LOG(LogSErr, LL_ERR, true)
SQMOD_LOG(LogSFtl, LL_FTL, true)

// ------------------------------------------------------------------------------------------------
#define SQMOD_CLOG(N_, L_, S_) /*
*/bool N_(bool c, CCStr fmt, ...) /*
*/ { /*
*/  if (!c) /*
*/   return c; /*
*/  va_list args; /*
*/  va_start(args, fmt); /*
*/  if (_Log) /*
*/   _Log->Send(L_, S_, fmt, args); /*
*/  else /*
*/   vprintf(fmt, args); /*
*/  va_end(args); /*
*/  return c; /*
*/ } /*
*/

// ------------------------------------------------------------------------------------------------
SQMOD_CLOG(cLogDbg, LL_DBG, false)
SQMOD_CLOG(cLogUsr, LL_USR, false)
SQMOD_CLOG(cLogScs, LL_SCS, false)
SQMOD_CLOG(cLogInf, LL_INF, false)
SQMOD_CLOG(cLogWrn, LL_WRN, false)
SQMOD_CLOG(cLogErr, LL_ERR, false)
SQMOD_CLOG(cLogFtl, LL_FTL, false)

// ------------------------------------------------------------------------------------------------
SQMOD_CLOG(cLogSDbg, LL_DBG, true)
SQMOD_CLOG(cLogSUsr, LL_USR, true)
SQMOD_CLOG(cLogSScs, LL_SCS, true)
SQMOD_CLOG(cLogSInf, LL_INF, true)
SQMOD_CLOG(cLogSWrn, LL_WRN, true)
SQMOD_CLOG(cLogSErr, LL_ERR, true)
SQMOD_CLOG(cLogSFtl, LL_FTL, true)

// ------------------------------------------------------------------------------------------------
void SqThrow(CCStr fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    if (_Log)
        _Log->Throw(fmt, args);
    else
        vprintf(fmt, args);
    va_end(args);
}

} // Namespace:: SqMod
