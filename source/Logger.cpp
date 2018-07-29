// ------------------------------------------------------------------------------------------------
#include "Logger.hpp"
#include "Base/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <ctime>
#include <cerrno>
#include <cstring>
#include <cstdarg>

// ------------------------------------------------------------------------------------------------
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_OS_WINDOWS

// ------------------------------------------------------------------------------------------------
#include <Windows.h>

// ------------------------------------------------------------------------------------------------
namespace {

/* ------------------------------------------------------------------------------------------------
 * Common windows colors.
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
 * Logging colors.
*/
enum
{
    LC_DBG  = LC_LIGHT_BLUE,
    LC_USR  = LC_GRAY,
    LC_SCS  = LC_LIGHT_GREEN,
    LC_INF  = LC_LIGHT_CYAN,
    LC_WRN  = LC_LIGHT_YELLOW,
    LC_ERR  = LC_LIGHT_RED,
    LC_FTL  = LC_LIGHT_MAGENTA
};

/* ------------------------------------------------------------------------------------------------
 * Identify the associated message color.
*/
inline WORD GetLevelColor(BYTE level)
{
    switch (level)
    {
        case SqMod::LOGL_DBG: return LC_DBG;
        case SqMod::LOGL_USR: return LC_USR;
        case SqMod::LOGL_SCS: return LC_SCS;
        case SqMod::LOGL_INF: return LC_INF;
        case SqMod::LOGL_WRN: return LC_WRN;
        case SqMod::LOGL_ERR: return LC_ERR;
        case SqMod::LOGL_FTL: return LC_FTL;
        default: return LC_NORMAL;
    }
}

} // Namespace::

#endif // SQMOD_OS_WINDOWS

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Identify the message prefix.
*/
static inline CCStr GetLevelTag(Uint8 level)
{
    switch (level)
    {
        case LOGL_DBG:  return "[DBG]";
        case LOGL_USR:  return "[USR]";
        case LOGL_SCS:  return "[SCS]";
        case LOGL_INF:  return "[INF]";
        case LOGL_WRN:  return "[WRN]";
        case LOGL_ERR:  return "[ERR]";
        case LOGL_FTL:  return "[FTL]";
        default:        return "[UNK]";
    }
}

#ifndef SQMOD_OS_WINDOWS

/* ------------------------------------------------------------------------------------------------
 * Identify the message prefix and color.
*/
static inline CCStr GetColoredLevelTag(Uint8 level)
{
    switch (level)
    {
        case LOGL_DBG:  return "\033[21;94m[DBG]\033[0m";
        case LOGL_USR:  return "\033[21;37m[USR]\033[0m";
        case LOGL_SCS:  return "\033[21;92m[SCS]\033[0m";
        case LOGL_INF:  return "\033[21;96m[INF]\033[0m";
        case LOGL_WRN:  return "\033[21;93m[WRN]\033[0m";
        case LOGL_ERR:  return "\033[21;91m[ERR]\033[0m";
        case LOGL_FTL:  return "\033[21;95m[FTL]\033[0m";
        default:        return "\033[21;0m[UNK]\033[0m";
    }
}

/* ------------------------------------------------------------------------------------------------
 * Identify the message prefix and color.
*/
static inline CCStr GetColoredLevelTagDim(Uint8 level)
{
    switch (level)
    {
        case LOGL_DBG:  return "\033[21;94m[DBG]\033[2m";
        case LOGL_USR:  return "\033[21;37m[USR]\033[2m";
        case LOGL_SCS:  return "\033[21;92m[SCS]\033[2m";
        case LOGL_INF:  return "\033[21;96m[INF]\033[2m";
        case LOGL_WRN:  return "\033[21;93m[WRN]\033[2m";
        case LOGL_ERR:  return "\033[21;91m[ERR]\033[2m";
        case LOGL_FTL:  return "\033[21;95m[FTL]\033[2m";
        default:        return "\033[21;0m[UNK]\033[0m";
    }
}

#endif // SQMOD_OS_WINDOWS

/* ------------------------------------------------------------------------------------------------
 * Output a logging message to the console window.
*/
static inline void OutputConsoleMessage(Uint8 level, bool sub, CCStr tms, CCStr msg)
{
#ifdef SQMOD_OS_WINDOWS
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csb_state;
    GetConsoleScreenBufferInfo(hstdout, &csb_state);
    SetConsoleTextAttribute(hstdout, GetLevelColor(level));
    if (tms)
    {
        std::printf("%s %s ", GetLevelTag(level), tms);
    }
    else
    {
        std::printf("%s ", GetLevelTag(level));
    }
    SetConsoleTextAttribute(hstdout, sub ? LC_NORMAL : LC_WHITE);
    std::printf("%s\n", msg);
    SetConsoleTextAttribute(hstdout, csb_state.wAttributes);
#else
    if (tms)
    {
        std::printf("%s %s %s\033[0m\n",
            sub ? GetColoredLevelTagDim(level) : GetColoredLevelTag(level), tms, msg);
    }
    else
    {
        std::printf("%s %s\033[0m\n",
            sub ? GetColoredLevelTagDim(level) : GetColoredLevelTag(level), msg);
    }
#endif // SQMOD_OS_WINDOWS
}

/* ------------------------------------------------------------------------------------------------
 * Identify the associated message color.
*/
static inline CCStr GetTimeStampStr()
{
    static CharT tmbuff[80];
    std::time_t t = std::time(nullptr);
    std::strftime(tmbuff, sizeof(tmbuff), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    // Return the resulted buffer
    return tmbuff;
}

// ------------------------------------------------------------------------------------------------
Logger Logger::s_Inst;

// ------------------------------------------------------------------------------------------------
Logger::Logger()
    : m_Buffer()
    , m_ConsoleLevels(LOGL_ANY)
    , m_LogFileLevels(~LOGL_DBG)
    , m_ConsoleTime(false)
    , m_LogFileTime(true)
    , m_File(nullptr)
    , m_Filename()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Logger::~Logger()
{
    Close();
}

// ------------------------------------------------------------------------------------------------
void Logger::Close()
{
    // Is there a file handle to close?
    if (m_File)
    {
        // Flush buffered data
        std::fflush(m_File);
        // Close the file handle
        std::fclose(m_File);
        // Prevent further use of this file handle
        m_File = nullptr;
    }
}

// ------------------------------------------------------------------------------------------------
void Logger::SetLogFilename(CCStr filename)
{
    // Close the current logging file, if any
    Close();
    // Clear the current name
    m_Filename.clear();
    // Was there a name specified?
    if (!filename || *filename == '\0')
    {
        return; // We're done here!
    }
    // Make sure the internal buffer has some memory
    m_Buffer.Adjust(1024);
    // Obtain the current time for generating the filename
    const std::time_t t = std::time(nullptr);
    // Generate the filename using the current time-stamp
    if (std::strftime(m_Buffer.Data(), m_Buffer.Size(), filename, std::localtime(&t)) > 0)
    {
        m_Filename.assign(m_Buffer.Data());
    }
    else
    {
        return; // We're done here!
    }
    // Attempt to open the file for writing
    m_File = std::fopen(m_Filename.c_str(), "w");
    // See if the file could be opened
    if (!m_File)
    {
        OutputError("Unable to open the log file (%s) : %s", m_Filename.c_str(), std::strerror(errno));
    }
}

// ------------------------------------------------------------------------------------------------
void Logger::Initialize(CCStr filename)
{
    // Close the logging file
    Close();
    // Allocate some memory in the buffer
    m_Buffer.Adjust(1024);
    // Set the log file name and open the file if necessary
    SetLogFilename(filename);
}

// ------------------------------------------------------------------------------------------------
void Logger::Terminate()
{
    // Release all the buffer resources and references
    m_Buffer.ResetAll();
}

// ------------------------------------------------------------------------------------------------
void Logger::Proccess(Uint8 level, bool sub)
{
    // Obtain the time-stamp if necessary
    CCStr tms = (m_ConsoleTime || m_LogFileTime) ? GetTimeStampStr() : nullptr;
    // Are we allowed to send this message level to console?
    if (m_ConsoleLevels & level)
    {
        OutputConsoleMessage(level, sub, (m_ConsoleTime ? tms : nullptr), m_Buffer.Get());
    }
    // Are we allowed to write it to a file?
    if (m_File && (m_LogFileLevels & level))
    {
        // Write the level tag
        std::fputs(GetLevelTag(level), m_File);
        std::fputc(' ', m_File);
        // Should we include the time-stamp?
        if (m_LogFileTime && tms)
        {
            std::fputs(tms, m_File);
            std::fputc(' ', m_File);
        }
        // Write the message
        std::fputs(m_Buffer.Get(), m_File);
        // Append a new line
        std::fputc('\n', m_File);
    }
}

// ------------------------------------------------------------------------------------------------
void Logger::Send(Uint8 level, bool sub, CCStr fmt, va_list args)
{
    // Is this level even allowed?
    if ((m_ConsoleLevels & level) || (m_LogFileLevels & level))
    {
        // Generate the message in the buffer
        m_Buffer.WriteF(0, fmt, args);
        // Process the message in the buffer
        Proccess(level, sub);
    }
}

// ------------------------------------------------------------------------------------------------
void Logger::Write(Uint8 level, bool sub, CCStr fmt, ...)
{
    if ((m_ConsoleLevels & level) || (m_LogFileLevels & level))
    {
        // Initialize the variable argument list
        va_list args;
        va_start(args, fmt);
        // Generate the message in the buffer
        m_Buffer.WriteF(0, fmt, args);
        // Finalize the variable argument list
        va_end(args);
        // Process the message in the buffer
        Proccess(level, sub);
    }
}

// ------------------------------------------------------------------------------------------------
void Logger::Debug(CCStr fmt, ...)
{
    // Initialize the variable argument list
    va_list args;
    va_start(args, fmt);
    // Forward the call to the actual debug function
    Debug(fmt, args);
    // Finalize the variable argument list
    va_end(args);
}

void Logger::Debug(CCStr fmt, va_list args)
{
    using namespace Sqrat;
    // Retrieve the default Squirrel VM
    HSQUIRRELVM vm = DefaultVM::Get();
    // Used to acquire
    SQStackInfos si;
    // Write the message to the buffer
    Int32 ret = m_Buffer.WriteF(0, fmt, args);
    // Obtain information about the current stack level
    if (SQ_SUCCEEDED(sq_stackinfos(vm, 1, &si)))
    {
        m_Buffer.WriteF(ret, "\n[\n=>Location: %s\n=>Line: %d\n=>Function: %s\n]"
                , si.source ? si.source : _SC("unknown")
                , si.line
                , si.funcname ? si.funcname : _SC("unknown"));
    }
    else
    {
        m_Buffer.WriteF(ret, "\n[\n=>Location: unknown\n=>Line: unknown\n=>Function: unknown\n]");
    }
    // Process the message in the buffer
    Proccess(LOGL_ERR, true);
    // Begin the traceback process
    ret = m_Buffer.WriteF(0, "Traceback:\n[\n");
    // Traceback the function call
    for (Int32 level = 1; SQ_SUCCEEDED(sq_stackinfos(vm, level, &si)); ++level)
    {
        ret += m_Buffer.WriteF(ret, "=> [%d] %s (%d) [%s]\n", level
                                    , si.source ? si.source : _SC("unknown")
                                    , si.line
                                    , si.funcname ? si.funcname : _SC("unknown"));
    }
    // End the function call traceback
    m_Buffer.WriteF(ret, "]");
    // Process the message in the buffer
    Proccess(LOGL_INF, true);
    // Temporary variables to retrieve stack information
    CSStr s_ = 0, name = 0;
    SQInteger i_, seq = 0;
    SQFloat f_;
    SQUserPointer p_;
    // Begin the local variables information
    ret = m_Buffer.WriteF(0, "Locals:\n[\n");
    // Process each stack level
    for (Int32 level = 0; level < 10; level++)
    {
        seq = 0;
        // Display all locals in the current stack level
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
    // End the variables information
    m_Buffer.WriteF(ret, "]");
    // Process the message in the buffer
    Proccess(LOGL_INF, true);
}

// ------------------------------------------------------------------------------------------------
#define SQMOD_LOG(N_, L_, S_) /*
*/ void N_(CCStr fmt, ...) /*
*/ { /*
*/  va_list args; /*
*/  va_start(args, fmt); /*
*/  Logger::Get().Send(L_, S_, fmt, args); /*
*/  va_end(args); /*
*/ } /*
*/

// ------------------------------------------------------------------------------------------------
SQMOD_LOG(LogDbg, LOGL_DBG, false)
SQMOD_LOG(LogUsr, LOGL_USR, false)
SQMOD_LOG(LogScs, LOGL_SCS, false)
SQMOD_LOG(LogInf, LOGL_INF, false)
SQMOD_LOG(LogWrn, LOGL_WRN, false)
SQMOD_LOG(LogErr, LOGL_ERR, false)
SQMOD_LOG(LogFtl, LOGL_FTL, false)

// ------------------------------------------------------------------------------------------------
SQMOD_LOG(LogSDbg, LOGL_DBG, true)
SQMOD_LOG(LogSUsr, LOGL_USR, true)
SQMOD_LOG(LogSScs, LOGL_SCS, true)
SQMOD_LOG(LogSInf, LOGL_INF, true)
SQMOD_LOG(LogSWrn, LOGL_WRN, true)
SQMOD_LOG(LogSErr, LOGL_ERR, true)
SQMOD_LOG(LogSFtl, LOGL_FTL, true)

// ------------------------------------------------------------------------------------------------
#define SQMOD_CLOG(N_, L_, S_) /*
*/bool N_(bool c, CCStr fmt, ...) /*
*/ { /*
*/  if (!c) /*
*/  { /*
*/      return c; /*
*/  } /*
*/  va_list args; /*
*/  va_start(args, fmt); /*
*/  Logger::Get().Send(L_, S_, fmt, args); /*
*/  va_end(args); /*
*/  return c; /*
*/ } /*
*/

// ------------------------------------------------------------------------------------------------
SQMOD_CLOG(cLogDbg, LOGL_DBG, false)
SQMOD_CLOG(cLogUsr, LOGL_USR, false)
SQMOD_CLOG(cLogScs, LOGL_SCS, false)
SQMOD_CLOG(cLogInf, LOGL_INF, false)
SQMOD_CLOG(cLogWrn, LOGL_WRN, false)
SQMOD_CLOG(cLogErr, LOGL_ERR, false)
SQMOD_CLOG(cLogFtl, LOGL_FTL, false)

// ------------------------------------------------------------------------------------------------
SQMOD_CLOG(cLogSDbg, LOGL_DBG, true)
SQMOD_CLOG(cLogSUsr, LOGL_USR, true)
SQMOD_CLOG(cLogSScs, LOGL_SCS, true)
SQMOD_CLOG(cLogSInf, LOGL_INF, true)
SQMOD_CLOG(cLogSWrn, LOGL_WRN, true)
SQMOD_CLOG(cLogSErr, LOGL_ERR, true)
SQMOD_CLOG(cLogSFtl, LOGL_FTL, true)

// ------------------------------------------------------------------------------------------------
template < Uint8 L, bool S > static SQInteger LogBasicMessage(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the message value specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing message value");
    }
    // Attempt to generate the string value
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Forward the resulted string value to the logger
    Logger::Get().Write(L, S, "%s", val.mPtr);
    // This function does not return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
static void SqLogClose()
{
    Logger::Get().Close();
}

// ------------------------------------------------------------------------------------------------
static void SqLogInitialize(CSStr filename)
{
    Logger::Get().Initialize(filename);
}

// ------------------------------------------------------------------------------------------------
static void SqLogToggleConsoleTime(bool toggle)
{
    Logger::Get().ToggleConsoleTime(toggle);
}

// ------------------------------------------------------------------------------------------------
static bool SqLogConsoleHasTime()
{
    return Logger::Get().ConsoleHasTime();
}

// ------------------------------------------------------------------------------------------------
static void SqLogToggleLogFileTime(bool toggle)
{
    Logger::Get().ToggleLogFileTime(toggle);
}

// ------------------------------------------------------------------------------------------------
static bool SqLogLogFileHasTime()
{
    return Logger::Get().LogFileHasTime();
}

// ------------------------------------------------------------------------------------------------
static void SqLogSetConsoleLevels(SQInteger level)
{
    Logger::Get().SetConsoleLevels(ConvTo< Uint8 >::From(level));
}

// ------------------------------------------------------------------------------------------------
static bool SqLogGetConsoleLevels()
{
    return Logger::Get().GetConsoleLevels();
}

// ------------------------------------------------------------------------------------------------
static void SqLogSetLogFileLevels(SQInteger level)
{
    Logger::Get().SetLogFileLevels(ConvTo< Uint8 >::From(level));
}

// ------------------------------------------------------------------------------------------------
static bool SqLogGetLogFileLevels()
{
    return Logger::Get().GetLogFileLevels();
}

// ------------------------------------------------------------------------------------------------
static void SqLogEnableConsoleLevel(SQInteger level)
{
    Logger::Get().EnableConsoleLevel(ConvTo< Uint8 >::From(level));
}

// ------------------------------------------------------------------------------------------------
static void SqLogDisableConsoleLevel(SQInteger level)
{
    Logger::Get().DisableConsoleLevel(ConvTo< Uint8 >::From(level));
}

// ------------------------------------------------------------------------------------------------
static void SqLogToggleConsoleLevel(SQInteger level, bool toggle)
{
    Logger::Get().ToggleConsoleLevel(ConvTo< Uint8 >::From(level), toggle);
}

// ------------------------------------------------------------------------------------------------
static void SqLogEnableLogFileLevel(SQInteger level)
{
    Logger::Get().EnableLogFileLevel(ConvTo< Uint8 >::From(level));
}

// ------------------------------------------------------------------------------------------------
static void SqLogDisableLogFileLevel(SQInteger level)
{
    Logger::Get().DisableLogFileLevel(ConvTo< Uint8 >::From(level));
}

// ------------------------------------------------------------------------------------------------
static void SqLogToggleLogFileLevel(SQInteger level, bool toggle)
{
    Logger::Get().ToggleLogFileLevel(ConvTo< Uint8 >::From(level), toggle);
}

// ------------------------------------------------------------------------------------------------
static const String & SqLogGetLogFilename()
{
    return Logger::Get().GetLogFilename();
}

// ------------------------------------------------------------------------------------------------
static void SqLogSetLogFilename(CSStr filename)
{
    Logger::Get().SetLogFilename(filename);
}

// ================================================================================================
void Register_Log(HSQUIRRELVM vm)
{
    RootTable(vm)
    .Bind(_SC("SqLog"), Table(vm)
        .SquirrelFunc(_SC("Dbg"), &LogBasicMessage< LOGL_DBG, false >)
        .SquirrelFunc(_SC("Usr"), &LogBasicMessage< LOGL_USR, false >)
        .SquirrelFunc(_SC("Scs"), &LogBasicMessage< LOGL_SCS, false >)
        .SquirrelFunc(_SC("Inf"), &LogBasicMessage< LOGL_INF, false >)
        .SquirrelFunc(_SC("Wrn"), &LogBasicMessage< LOGL_WRN, false >)
        .SquirrelFunc(_SC("Err"), &LogBasicMessage< LOGL_ERR, false >)
        .SquirrelFunc(_SC("Ftl"), &LogBasicMessage< LOGL_FTL, false >)
        .SquirrelFunc(_SC("SDbg"), &LogBasicMessage< LOGL_DBG, true >)
        .SquirrelFunc(_SC("SUsr"), &LogBasicMessage< LOGL_USR, true >)
        .SquirrelFunc(_SC("SScs"), &LogBasicMessage< LOGL_SCS, true >)
        .SquirrelFunc(_SC("SInf"), &LogBasicMessage< LOGL_INF, true >)
        .SquirrelFunc(_SC("SWrn"), &LogBasicMessage< LOGL_WRN, true >)
        .SquirrelFunc(_SC("SErr"), &LogBasicMessage< LOGL_ERR, true >)
        .SquirrelFunc(_SC("SFtl"), &LogBasicMessage< LOGL_FTL, true >)
        .Func(_SC("Close"), &SqLogClose)
        .Func(_SC("Initialize"), &SqLogInitialize)
        .Func(_SC("ToggleConsoleTime"), &SqLogToggleConsoleTime)
        .Func(_SC("ConsoleHasTime"), &SqLogConsoleHasTime)
        .Func(_SC("ToggleLogFileTime"), &SqLogToggleLogFileTime)
        .Func(_SC("LogFileHasTime"), &SqLogLogFileHasTime)
        .Func(_SC("SetConsoleLevels"), &SqLogSetConsoleLevels)
        .Func(_SC("GetConsoleLevels"), &SqLogGetConsoleLevels)
        .Func(_SC("SetLogFileLevels"), &SqLogSetLogFileLevels)
        .Func(_SC("GetLogFileLevels"), &SqLogGetLogFileLevels)
        .Func(_SC("EnableConsoleLevel"), &SqLogEnableConsoleLevel)
        .Func(_SC("DisableConsoleLevel"), &SqLogDisableConsoleLevel)
        .Func(_SC("ToggleConsoleLevel"), &SqLogToggleConsoleLevel)
        .Func(_SC("EnableLogFileLevel"), &SqLogEnableLogFileLevel)
        .Func(_SC("DisableLogFileLevel"), &SqLogDisableLogFileLevel)
        .Func(_SC("ToggleLogFileLevel"), &SqLogToggleLogFileLevel)
        .Func(_SC("GetLogFilename"), &SqLogGetLogFilename)
        .Func(_SC("SetLogFilename"), &SqLogSetLogFilename)
    );
}

} // Namespace:: SqMod
