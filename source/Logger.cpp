#include "Logger.hpp"

// ------------------------------------------------------------------------------------------------
#include <ctime>
#include <cstdio>
#include <cstdarg>

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
inline const char * GetLevelTag(Uint8 type) noexcept
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
inline Uint16 GetLevelColor(Uint8 type) noexcept
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
inline const char * GetLevelColor(Uint8 type) noexcept
{
    return "";
}

#endif // SQMOD_OS_WINDOWS

// ------------------------------------------------------------------------------------------------
Logger::Logger() noexcept
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
void Logger::_Finalizer(Logger * ptr) noexcept
{
    if (ptr) delete ptr;
}

// ------------------------------------------------------------------------------------------------
Logger::Pointer Logger::Inst() noexcept
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
void Logger::ToggleConsoleTime(bool enabled) noexcept
{
    m_ConsoleTime = enabled;
}

void Logger::ToggleFileTime(bool enabled) noexcept
{
    m_FileTime = enabled;
}

// ------------------------------------------------------------------------------------------------
bool Logger::HasConsoleTime() const noexcept
{
    return m_ConsoleTime;
}

bool Logger::HasFileTime()const noexcept
{
    return m_FileTime;
}

// ------------------------------------------------------------------------------------------------
void Logger::SetConsoleLevels(Uint8 levels) noexcept
{
    m_ConsoleLevels = levels;
}

void Logger::SetFileLevels(Uint8 levels) noexcept
{
    m_FileLevels = levels;
}

// ------------------------------------------------------------------------------------------------
Uint8 Logger::GetConsoleLevels() const noexcept
{
    return m_ConsoleLevels;
}

Uint8 Logger::GetFileLevels() const noexcept
{
    return m_FileLevels;
}

// ------------------------------------------------------------------------------------------------
void Logger::EnableConsoleLevel(Uint8 level) noexcept
{
    m_ConsoleLevels |= level;
}

void Logger::EnableFileLevel(Uint8 level) noexcept
{
    m_FileLevels |= level;
}

// ------------------------------------------------------------------------------------------------
void Logger::DisableConsoleLevel(Uint8 level) noexcept
{
    if (m_ConsoleLevels & level) m_ConsoleLevels ^= level;
}

void Logger::DisableFileLevel(Uint8 level) noexcept
{
    if (m_FileLevels & level) m_FileLevels ^= level;
}

// ------------------------------------------------------------------------------------------------
Uint8 Logger::GetDebugLevel() const noexcept
{
    return m_DebugLevel;
}

void Logger::SetDebugLevel(Uint8 level) noexcept
{
    m_DebugLevel = level;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Logger::GetVerbosity() const noexcept
{
    return m_Verbosity;
}

void Logger::SetVerbosity(SQInt32 level) noexcept
{
    m_Verbosity = level;
}

// ------------------------------------------------------------------------------------------------
void Logger::Send(Uint8 type, bool sub, const char * fmt, va_list args)  noexcept
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
#define SQMOD_LOG(N_, L_, S_) /*
*/ void Logger::N_(const char * fmt, ...) noexcept /*
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
*/bool Logger::N_(bool c, const char * fmt, ...) noexcept /*
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

} // Namespace:: SqMod
