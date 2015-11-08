#include "Logger.hpp"
#include "Register.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include <ctime>
#include <cstdio>
#include <cstdarg>

// ------------------------------------------------------------------------------------------------
#include <sqstdstring.h>

// ------------------------------------------------------------------------------------------------
namespace {

#ifdef SQMOD_OS_WINDOWS
    #include <Windows.h>

    enum
    {
        LOG_COLOR_NORMAL            = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        LOG_COLOR_WHITE             = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        LOG_COLOR_GRAY              = FOREGROUND_INTENSITY,
        LOG_COLOR_LIGHT_BLUE        = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
        LOG_COLOR_DARK_BLUE         = FOREGROUND_BLUE,
        LOG_COLOR_LIGHT_GREEN       = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
        LOG_COLOR_DARK_GREEN        = FOREGROUND_GREEN,
        LOG_COLOR_LIGHT_CYAN        = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
        LOG_COLOR_DARK_CYAN         = FOREGROUND_GREEN | FOREGROUND_BLUE,
        LOG_COLOR_LIGHT_YELLOW      = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
        LOG_COLOR_DARK_YELLOW       = FOREGROUND_RED | FOREGROUND_GREEN,
        LOG_COLOR_LIGHT_RED         = FOREGROUND_INTENSITY | FOREGROUND_RED,
        LOG_COLOR_DARK_RED          = FOREGROUND_RED,
        LOG_COLOR_LIGHT_MAGENTA     = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
        LOG_COLOR_DARK_MAGENTA      = FOREGROUND_RED | FOREGROUND_BLUE
    };

    enum
    {
        LOG_COLOR_DBG               = LOG_COLOR_LIGHT_BLUE,
        LOG_COLOR_MSG               = LOG_COLOR_GRAY,
        LOG_COLOR_SCS               = LOG_COLOR_LIGHT_GREEN,
        LOG_COLOR_INF               = LOG_COLOR_LIGHT_CYAN,
        LOG_COLOR_WRN               = LOG_COLOR_LIGHT_YELLOW,
        LOG_COLOR_ERR               = LOG_COLOR_LIGHT_MAGENTA,
        LOG_COLOR_FTL               = LOG_COLOR_LIGHT_RED
    };

#endif // SQMOD_OS_WINDOWS

} // Namespace::

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Logger::Pointer _Log = Logger::Inst();

// ------------------------------------------------------------------------------------------------
inline const char * GetLevelTag(Uint8 type)
{
    switch (type)
    {
        case Logger::LEVEL_DBG: return "[DBG]";
        case Logger::LEVEL_MSG: return "[MSG]";
        case Logger::LEVEL_SCS: return "[SCS]";
        case Logger::LEVEL_INF: return "[INF]";
        case Logger::LEVEL_WRN: return "[WRN]";
        case Logger::LEVEL_ERR: return "[ERR]";
        case Logger::LEVEL_FTL: return "[FTL]";
        default: return "[UNK]";
    }
}

#ifdef SQMOD_OS_WINDOWS

// ------------------------------------------------------------------------------------------------
inline Uint16 GetLevelColor(Uint8 type)
{
    switch (type)
    {
        case Logger::LEVEL_DBG: return LOG_COLOR_DBG;
        case Logger::LEVEL_MSG: return LOG_COLOR_MSG;
        case Logger::LEVEL_SCS: return LOG_COLOR_SCS;
        case Logger::LEVEL_INF: return LOG_COLOR_INF;
        case Logger::LEVEL_WRN: return LOG_COLOR_WRN;
        case Logger::LEVEL_ERR: return LOG_COLOR_ERR;
        case Logger::LEVEL_FTL: return LOG_COLOR_FTL;
        default: return LOG_COLOR_NORMAL;
    }
}
#else

// ------------------------------------------------------------------------------------------------
inline const char * GetLevelColor(Uint8 type)
{
    return "";
}

#endif // SQMOD_OS_WINDOWS

// ------------------------------------------------------------------------------------------------
Logger::Logger()
    : m_ConsoleTime(false), m_FileTime(true)
    , m_ConsoleLevels(Logger::LEVEL_ANY), m_FileLevels(Logger::LEVEL_ANY)
    , m_LogPath("./logs/"), m_DebugLevel(SQMOD_DEBUG_LEVEL), m_Verbosity(0)
{

}

// ------------------------------------------------------------------------------------------------
Logger::~Logger()
{

}

// ------------------------------------------------------------------------------------------------
void Logger::_Finalizer(Logger * ptr)
{
    if (ptr) delete ptr;
}

// ------------------------------------------------------------------------------------------------
Logger::Pointer Logger::Inst()
{
    if (!_Log) return Pointer(new Logger(), &Logger::_Finalizer);
    return Pointer(nullptr, &Logger::_Finalizer);
}

// ------------------------------------------------------------------------------------------------
bool Logger::Init()
{

    return true;
}

bool Logger::Load()
{

    return true;
}

// ------------------------------------------------------------------------------------------------
void Logger::Deinit()
{

}

void Logger::Unload()
{

}

// ------------------------------------------------------------------------------------------------
void Logger::Terminate()
{

}

// ------------------------------------------------------------------------------------------------
void Logger::ToggleConsoleTime(bool enabled)
{
    m_ConsoleTime = enabled;
}

void Logger::ToggleFileTime(bool enabled)
{
    m_FileTime = enabled;
}

// ------------------------------------------------------------------------------------------------
bool Logger::HasConsoleTime() const
{
    return m_ConsoleTime;
}

bool Logger::HasFileTime()const
{
    return m_FileTime;
}

// ------------------------------------------------------------------------------------------------
void Logger::SetConsoleLevels(Uint8 levels)
{
    m_ConsoleLevels = levels;
}

void Logger::SetFileLevels(Uint8 levels)
{
    m_FileLevels = levels;
}

// ------------------------------------------------------------------------------------------------
Uint8 Logger::GetConsoleLevels() const
{
    return m_ConsoleLevels;
}

Uint8 Logger::GetFileLevels() const
{
    return m_FileLevels;
}

// ------------------------------------------------------------------------------------------------
void Logger::EnableConsoleLevel(Uint8 level)
{
    m_ConsoleLevels |= level;
}

void Logger::EnableFileLevel(Uint8 level)
{
    m_FileLevels |= level;
}

// ------------------------------------------------------------------------------------------------
void Logger::DisableConsoleLevel(Uint8 level)
{
    if (m_ConsoleLevels & level) m_ConsoleLevels ^= level;
}

void Logger::DisableFileLevel(Uint8 level)
{
    if (m_FileLevels & level) m_FileLevels ^= level;
}

// ------------------------------------------------------------------------------------------------
Uint8 Logger::GetDebugLevel() const
{
    return m_DebugLevel;
}

void Logger::SetDebugLevel(Uint8 level)
{
    m_DebugLevel = level;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Logger::GetVerbosity() const
{
    return m_Verbosity;
}

void Logger::SetVerbosity(SQInt32 level)
{
    m_Verbosity = level;
}

// ------------------------------------------------------------------------------------------------
void Logger::Send(Uint8 type, bool sub, const char * fmt, va_list args)
{
    // Verify that this level is allowed to be streamed
    if (!(m_ConsoleLevels & type) && !(m_FileLevels & type)) return;

    // Allocate space for the time stamp string
    char timebuff[80];

    // Grab the local time into a string if required
    if (m_ConsoleTime || m_FileTime)
    {
        // Create the variables required to retrieve the time.
        time_t rawtime;
        struct tm * timeinfo;

        // Retrieve the local time.
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        // Format the retrieved time.
        strftime(timebuff, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    }

    // Output to console if streaming to console is enabled
    if (m_ConsoleLevels & type)
    {
#ifdef SQMOD_OS_WINDOWS
        // Get the handle to the output stream
        HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

        // Save the current state of the console screen buffer
        CONSOLE_SCREEN_BUFFER_INFO csb_state;
        GetConsoleScreenBufferInfo(hstdout, &csb_state);

        // Set the attributes for the message information
        SetConsoleTextAttribute(hstdout, GetLevelColor(type));

        // Output the message information
        if (m_ConsoleTime)
        {
            printf("%s %s ", GetLevelTag(type), timebuff);
        }
        else
        {
            printf("%s ", GetLevelTag(type));
        }

        // Set the attributes for the message content
        SetConsoleTextAttribute(hstdout, sub ? LOG_COLOR_NORMAL : LOG_COLOR_WHITE);

        // Print the specified message
        vprintf(fmt, args);

        // Restore the state of the console screen buffer
        SetConsoleTextAttribute(hstdout, csb_state.wAttributes);
#else
        // Output the message information
        if (m_ConsoleTime)
        {
            printf("%s %s ", GetLevelTag(type), timebuff);
        }
        else
        {
            printf("%s ", GetLevelTag(type));
        }

        // Print the specified message
        vprintf(fmt, args);
#endif // SQMOD_OS_WINDOWS

        // Terminate the line
        puts("");
    }
}

// ------------------------------------------------------------------------------------------------
void Logger::Message(Uint8 type, bool sub, const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Send(type, sub, fmt, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
#define SQMOD_LOG(N_, L_, S_) /*
*/ void Logger::N_(const char * fmt, ...) /*
*/ { /*
*/  va_list args; /*
*/  va_start(args, fmt); /*
*/  Send(L_, S_, fmt, args); /*
*/  va_end(args); /*
*/ } /*
*/

// ------------------------------------------------------------------------------------------------
SQMOD_LOG(Dbg, LEVEL_DBG, false)
SQMOD_LOG(Msg, LEVEL_MSG, false)
SQMOD_LOG(Scs, LEVEL_SCS, false)
SQMOD_LOG(Inf, LEVEL_INF, false)
SQMOD_LOG(Wrn, LEVEL_WRN, false)
SQMOD_LOG(Err, LEVEL_ERR, false)
SQMOD_LOG(Ftl, LEVEL_FTL, false)

// ------------------------------------------------------------------------------------------------
SQMOD_LOG(SDbg, LEVEL_DBG, true)
SQMOD_LOG(SMsg, LEVEL_MSG, true)
SQMOD_LOG(SScs, LEVEL_SCS, true)
SQMOD_LOG(SInf, LEVEL_INF, true)
SQMOD_LOG(SWrn, LEVEL_WRN, true)
SQMOD_LOG(SErr, LEVEL_ERR, true)
SQMOD_LOG(SFtl, LEVEL_FTL, true)

// ------------------------------------------------------------------------------------------------
#define SQMOD_CLOG(N_, L_, S_) /*
*/bool Logger::N_(bool c, const char * fmt, ...) /*
*/ { /*
*/  if (c) /*
*/  { /*
*/      va_list args; /*
*/      va_start(args, fmt); /*
*/      Send(L_, S_, fmt, args); /*
*/      va_end(args); /*
*/  } /*
*/  return c; /*
*/ } /*
*/

// ------------------------------------------------------------------------------------------------
SQMOD_CLOG(cDbg, LEVEL_DBG, false)
SQMOD_CLOG(cMsg, LEVEL_MSG, false)
SQMOD_CLOG(cScs, LEVEL_SCS, false)
SQMOD_CLOG(cInf, LEVEL_INF, false)
SQMOD_CLOG(cWrn, LEVEL_WRN, false)
SQMOD_CLOG(cErr, LEVEL_ERR, false)
SQMOD_CLOG(cFtl, LEVEL_FTL, false)

// ------------------------------------------------------------------------------------------------
SQMOD_CLOG(cSDbg, LEVEL_DBG, true)
SQMOD_CLOG(cSMsg, LEVEL_MSG, true)
SQMOD_CLOG(cSScs, LEVEL_SCS, true)
SQMOD_CLOG(cSInf, LEVEL_INF, true)
SQMOD_CLOG(cSWrn, LEVEL_WRN, true)
SQMOD_CLOG(cSErr, LEVEL_ERR, true)
SQMOD_CLOG(cSFtl, LEVEL_FTL, true)

// ------------------------------------------------------------------------------------------------
template < Uint8 L, bool S > static SQInteger LogBasicMessage(HSQUIRRELVM vm)
{
    const SQInteger top = sq_gettop(vm);
    // Are there any arguments on the stack?
    if (top <= 1)
    {
        _Log->Err("Attempting to <log message> without specifying a value");
    }
    // Is there a single string or at least something that can convert to a string on the stack?
    else if (top == 2 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        // Variable where the resulted string will be retrieved
        const SQChar * msg = 0;
        // Attempt to retrieve the specified message from the stack
        if (SQ_FAILED(sq_getstring(vm, -1, &msg)))
        {
            _Log->Err("Unable to <retrieve the log message> from the stack");
            // Failed to log the value
            return 0;
        }
        // Log the specified string
        _Log->Message(L, S, "%s", msg);
    }
    else if (top > 2)
    {
        // Variables containing the resulted string
        SQChar * msg = NULL;
        SQInteger len = 0;
        // Attempt to call the format function with the passed arguments
        if (SQ_FAILED(sqstd_format(vm, 2, &len, &msg)))
        {
            _Log->Err("Unable to <generate the log message> because : %s", Error::Message(vm).c_str());
            // Failed to log the value
            return 0;
        }
        // Log the resulted string
        _Log->Message(L, S, "%s", msg);
    }
    else
    {
        _Log->Err("Unable to <extract the log message> from the specified value");
    }
    // At this point everything went correctly
    return 0;
}

// ================================================================================================
bool Register_Log(HSQUIRRELVM vm)
{

    // // Attempt to create the Log namespace
    Sqrat::Table logns(vm);

    // Output debugging information
    LogDbg("Beginning registration of <Log functions> type");
    // Attempt to register the free functions
    logns.SquirrelFunc(_SC("Dbg"), &LogBasicMessage< Logger::LEVEL_DBG, false >);
    logns.SquirrelFunc(_SC("Msg"), &LogBasicMessage< Logger::LEVEL_MSG, false >);
    logns.SquirrelFunc(_SC("Scs"), &LogBasicMessage< Logger::LEVEL_SCS, false >);
    logns.SquirrelFunc(_SC("Inf"), &LogBasicMessage< Logger::LEVEL_INF, false >);
    logns.SquirrelFunc(_SC("Wrn"), &LogBasicMessage< Logger::LEVEL_WRN, false >);
    logns.SquirrelFunc(_SC("Err"), &LogBasicMessage< Logger::LEVEL_ERR, false >);
    logns.SquirrelFunc(_SC("Ftl"), &LogBasicMessage< Logger::LEVEL_FTL, false >);
    logns.SquirrelFunc(_SC("SDbg"), &LogBasicMessage< Logger::LEVEL_DBG, true >);
    logns.SquirrelFunc(_SC("SMsg"), &LogBasicMessage< Logger::LEVEL_MSG, true >);
    logns.SquirrelFunc(_SC("SScs"), &LogBasicMessage< Logger::LEVEL_SCS, true >);
    logns.SquirrelFunc(_SC("SInf"), &LogBasicMessage< Logger::LEVEL_INF, true >);
    logns.SquirrelFunc(_SC("SWrn"), &LogBasicMessage< Logger::LEVEL_WRN, true >);
    logns.SquirrelFunc(_SC("SErr"), &LogBasicMessage< Logger::LEVEL_ERR, true >);
    logns.SquirrelFunc(_SC("SFtl"), &LogBasicMessage< Logger::LEVEL_FTL, true >);
    // Output debugging information
    LogDbg("Registration of <Log functions> type was successful");

    // Attempt to bind the namespace to the root table
    Sqrat::RootTable(vm).Bind(_SC("Log"), logns);

    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
