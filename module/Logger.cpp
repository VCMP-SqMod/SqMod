// ------------------------------------------------------------------------------------------------
#include "Logger.hpp"
#include "Core.hpp"
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <ctime>
#include <cerrno>
#include <cstring>
#include <cstdarg>
#include <memory>

// ------------------------------------------------------------------------------------------------
#include <sqratUtil.h>

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_OS_WINDOWS

// ------------------------------------------------------------------------------------------------
#include <windows.h>

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
static inline const char * GetLevelTag(uint8_t level)
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

/* ------------------------------------------------------------------------------------------------
 * Logging level to callback index.
*/
static inline uint8_t GetLevelIdx(uint8_t level)
{
    switch (level)
    {
        case LOGL_DBG:  return 0;
        case LOGL_USR:  return 1;
        case LOGL_SCS:  return 2;
        case LOGL_INF:  return 3;
        case LOGL_WRN:  return 4;
        case LOGL_ERR:  return 5;
        case LOGL_FTL:  return 6;
        default:        return 0xFF;
    }
}

#ifndef SQMOD_OS_WINDOWS

/* ------------------------------------------------------------------------------------------------
 * Identify the message prefix and color.
*/
static inline const char * GetColoredLevelTag(uint8_t level)
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
static inline const char * GetColoredLevelTagDim(uint8_t level)
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
static inline void OutputConsoleMessage(const Logger::MsgPtr & msg)
{
#ifdef SQMOD_OS_WINDOWS
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csb_state;
    GetConsoleScreenBufferInfo(hstdout, &csb_state);
    SetConsoleTextAttribute(hstdout, GetLevelColor(msg->mLvl));
    if (Logger::Get().ConsoleHasTime())
    {
        std::printf("%s %s ", GetLevelTag(msg->mLvl), msg->mBuf);
    }
    else
    {
        std::printf("%s ", GetLevelTag(msg->mLvl));
    }
    SetConsoleTextAttribute(hstdout, msg->mSub ? LC_NORMAL : LC_WHITE);
    std::printf("%s\n", msg->mStr.c_str());
    SetConsoleTextAttribute(hstdout, csb_state.wAttributes);
#else
    if (Logger::Get().ConsoleHasTime())
    {
        std::printf("%s %s %s\033[0m\n",
                    msg->mSub ? GetColoredLevelTagDim(msg->mLvl) : GetColoredLevelTag(msg->mLvl), msg->mBuf, msg->mStr.c_str());
    }
    else
    {
        std::printf("%s %s\033[0m\n",
                    msg->mSub ? GetColoredLevelTagDim(msg->mLvl) : GetColoredLevelTag(msg->mLvl), msg->mStr.c_str());
    }
#endif // SQMOD_OS_WINDOWS
}

// ------------------------------------------------------------------------------------------------
Logger Logger::s_Inst;

// ------------------------------------------------------------------------------------------------
void Logger::Message::Stamp()
{
    std::time_t t = std::time(nullptr);
    mTms = true; // Mark this message as being time-stamped
    std::strftime(mBuf, sizeof(mBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
}
// ------------------------------------------------------------------------------------------------
uint32_t Logger::Message::Append(const SQChar * str)
{
    // Validate string
    if (str)
    {
        return Append(str, std::strlen(str));
    }
    // No string to append
    return 0;
}
// ------------------------------------------------------------------------------------------------
uint32_t Logger::Message::Append(const SQChar * str, size_t len)
{
    // Discard trailing characters
    mStr.resize(mLen);
    // Append the given string
    mStr.append(str, len);
    // Update message length
    mLen += static_cast< uint32_t >(len);
    // Return back the written character count
    return static_cast< uint32_t >(len);
}
// ------------------------------------------------------------------------------------------------
uint32_t Logger::Message::AppendF(const SQChar * str, ...)
{
    // Initialize the variable argument list
    va_list args;
    va_start(args, str);
    // Forward to the actual implementation
    const size_t r = AppendFv(str, args);
    // Finalize the variable argument list
    va_end(args);
    // Return result
    return r;
}
// ------------------------------------------------------------------------------------------------
uint32_t Logger::Message::AppendFv(const SQChar * str, va_list vl)
{
    va_list args;
    // The estimated buffer required
    int len = static_cast< int >(mInc);
begin:
    // Do not modify the original va_list
    va_copy(args, vl);
    // Do we need to reserve space?
    if (static_cast< size_t >(mLen) + len > mStr.size())
    {
        mStr.resize(mLen + len, '\0'); // Reserve the necessary space
    }
    // Attempt to generate the specified string
    int res = std::vsnprintf((&mStr.front()) + mLen, static_cast< size_t >(len), str, args);
    // Do we need more space?
    if (res >= len)
    {
        // Adjust to required size
        len = res + 1;
        // Try again
        goto begin;
    }
    else if (res > 0)
    {
        mLen += static_cast< uint32_t >(res); // Update message size
    }
    else
    {
        res = 0; // Format failed
    }
    // Return the number of written characters
    return static_cast< uint32_t >(res);
}
// ------------------------------------------------------------------------------------------------
Logger::Logger() noexcept
    : m_ThreadID(std::this_thread::get_id())
    , m_Message()
    , m_Queue(4096)
    , m_ConsoleLevels(LOGL_ANY)
    , m_LogFileLevels(~LOGL_DBG)
    , m_ConsoleTime(false)
    , m_LogFileTime(true)
    , m_CyclicLock(false)
    , m_StringTruncate(32)
    , m_File(nullptr)
    , m_Filename()
    , m_LogCb{}
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
void Logger::SetLogFilename(const char * filename)
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
    // This should be enough for any kind of path
    char buffer[1024];
    // Obtain the current time for generating the filename
    const std::time_t t = std::time(nullptr);
    // Generate the filename using the current time-stamp
    if (std::strftime(buffer, sizeof(buffer), filename, std::localtime(&t)) > 0)
    {
        m_Filename.assign(buffer);
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
void Logger::BindCb(uint8_t level, Function & func)
{
    // Get the index of this log level
    const uint8_t idx = GetLevelIdx(level);
    // Is the log level valid?
    if (idx > 6)
    {
        STHROWF("Out of range log level index: {} > 4", int(idx));
    }
    // Obtain the function instance called for this log level
    Function & cb = m_LogCb[idx];
    // Assign the specified environment and function
    cb = std::move(func);
}

// ------------------------------------------------------------------------------------------------
void Logger::Initialize(const char * filename)
{
    // Set which thread is allowed to output directly
    m_ThreadID = std::this_thread::get_id();
    // Close the logging file, if any
    Close();
    // Set the log file name and open the file if necessary
    SetLogFilename(filename);
}

// ------------------------------------------------------------------------------------------------
void Logger::Terminate()
{
    // Process whatever is in the queue
    ProcessQueue();
    // Close the stream, if any
    Close();
}

// ------------------------------------------------------------------------------------------------
void Logger::Release()
{
    for (auto & f : m_LogCb)
    {
        f.Release();
    }
}

// ------------------------------------------------------------------------------------------------
void Logger::ProcessQueue()
{
    // Process only what's currently in the queue
    const size_t count = m_Queue.size_approx();
    // Retrieve each message individually and process it
    for (size_t n = 0; n <= count; ++n)
    {
        // Try to get a message from the queue
        if (m_Queue.try_dequeue(m_Message))
        {
            ProcessMessage(); // Process it
        }
    }
}

// ------------------------------------------------------------------------------------------------
SQBool Logger::ProcessCb()
{
    // Get the index of this log level
    const uint8_t idx = GetLevelIdx(m_Message->mLvl);
    // Is the log level valid and is there a callback associated?
    if (idx > 6 || m_LogCb[idx].IsNull())
    {
        return SQFalse;
    }
    // Grab the associated function
    Function & fn = m_LogCb[idx];
    // Grab the default VM
    HSQUIRRELVM vm = SqVM();
    // Gram the top of the stack
    SQInteger top = sq_gettop(vm);
    // Push the function followed by the environment
    sq_pushobject(vm, fn.GetFunc());
    sq_pushobject(vm, fn.GetEnv());
    // Push the log message
    sq_pushstring(vm, m_Message->mStr.c_str(), static_cast< SQInteger >(m_Message->mStr.size()));
    // Specify whether this is a sub message
    sq_pushbool(vm, static_cast< SQBool >(m_Message->mSub));
    // Make the function call and store the result
    SQRESULT res = sq_call(vm, 3, static_cast< SQBool >(true),
                           static_cast< SQBool >(ErrorHandling::IsEnabled()));
    // Default to non greedy callback
    SQBool ret = SQFalse;
    // Did the function succeeded and is the returned value not null?
    if (SQ_SUCCEEDED(res) && sq_gettype(vm, -1) != OT_NULL) {
        // Obtain the returned value
        sq_tobool(vm, -1, &ret);
    }
    // Pop the callback object and return value from the stack
    sq_settop(vm, top);
    // Return the outcome of this callback
    return ret;
}

// ------------------------------------------------------------------------------------------------
void Logger::PushMessage(MsgPtr & msg)
{
    // Are we in the main thread?
    if (m_ThreadID == std::this_thread::get_id())
    {
        // Take ownership of this message and mark it as current/last message
        m_Message = std::move(msg);
        // Finish the message
        m_Message->Finish();
        // Time-stamp the message, if necessary
        if (m_ConsoleTime || m_LogFileTime)
        {
            m_Message->Stamp();
        }
        // Deal with it now
        ProcessMessage();
    }
    else
    {
        // Finish the message
        msg->Finish();
        // Time-stamp the message
        msg->Stamp();
        // Queue it for later
        m_Queue.enqueue(std::move(msg));
    }
}
// ------------------------------------------------------------------------------------------------
void Logger::PushMessage(MsgPtr * msg, size_t len)
{
    // Are we in the main thread?
    if (std::this_thread::get_id() == m_ThreadID)
    {
        for (size_t i = 0; i < len; ++i)
        {
            // Take ownership of this message and mark it as current/last message
            m_Message = std::move(msg[i]);
            // Finish the message
            m_Message->Finish();
            // Time-stamp the message, if necessary
            if (m_ConsoleTime || m_LogFileTime)
            {
                m_Message->Stamp();
            }
            // Deal with it now
            ProcessMessage();
        }
    }
    else
    {
        for (size_t i = 0; i < len; ++i)
        {
            // Finish the message
            msg[i]->Finish();
            // Time-stamp the messages
            msg[i]->Stamp();
        }
        // Queue it for later
        m_Queue.enqueue_bulk(std::make_move_iterator(msg), len);
    }
}

// ------------------------------------------------------------------------------------------------
void Logger::ProcessMessage()
{
    // Is there a message to process
    if (!m_Message)
    {
        return; // Nothing to process
    }
    // Is there a cyclic lock on the logger?
    if (!m_CyclicLock)
    {
        // Lock the logger to prevent a cyclic dependency
        m_CyclicLock = true;
        // Attempt to process the script callback first
        const bool greedy = static_cast< bool >(ProcessCb());
        // Unlock the logger after the callback was invoked
        m_CyclicLock = false;
        // Is the callback for this level greedy?
        if (greedy)
        {
            return;
        }
    }
    // Are we allowed to send this message level to console?
    if (m_ConsoleLevels & m_Message->mLvl)
    {
        OutputConsoleMessage(m_Message);
    }
    // Are we allowed to write it to a file?
    if (m_File && (m_LogFileLevels & m_Message->mLvl))
    {
        // Write the level tag
        std::fputs(GetLevelTag(m_Message->mLvl), m_File);
        std::fputc(' ', m_File);
        // Should we include the time-stamp?
        if (m_LogFileTime)
        {
            std::fputs(m_Message->mBuf, m_File);
            std::fputc(' ', m_File);
        }
        // Write the message
        std::fputs(m_Message->mStr.data(), m_File);
        // Append a new line
        std::fputc('\n', m_File);
    }
}

// ------------------------------------------------------------------------------------------------
void Logger::Send(uint8_t level, bool sub, const char * msg, size_t len)
{
    // Is this level even allowed?
    if ((m_ConsoleLevels & level) || (m_LogFileLevels & level))
    {
        // Create a new message builder
        MsgPtr message(new Message(level, sub));
        // Generate the log message
        message->Append(msg, len);
        // Process the message in the buffer
        PushMessage(message);
    }
}

// ------------------------------------------------------------------------------------------------
void Logger::SendFv(uint8_t level, bool sub, const char * fmt, va_list args)
{
    // Is this level even allowed?
    if ((m_ConsoleLevels & level) || (m_LogFileLevels & level))
    {
        // Create a new message builder
        MsgPtr message(new Message(level, sub));
        // Generate the log message
        message->AppendFv(fmt, args);
        // Process the message in the buffer
        PushMessage(message);
    }
}

// ------------------------------------------------------------------------------------------------
void Logger::WriteF(uint8_t level, bool sub, const char * fmt, ...)
{
    if ((m_ConsoleLevels & level) || (m_LogFileLevels & level))
    {
        // Initialize the variable argument list
        va_list args;
        va_start(args, fmt);
        // Create a new message builder
        MsgPtr message(new Message(level, sub));
        // Generate the log message
        message->AppendFv(fmt, args);
        // Finalize the variable argument list
        va_end(args);
        // Process the message in the buffer
        PushMessage(message);
    }
}

// ------------------------------------------------------------------------------------------------
void Logger::DebugF(HSQUIRRELVM vm, const char * fmt, ...)
{
    // Initialize the variable argument list
    va_list args;
    va_start(args, fmt);
    // Forward the call to the actual debug function
    DebugFv(vm, fmt, args);
    // Finalize the variable argument list
    va_end(args);
}

void Logger::DebugFv(HSQUIRRELVM vm, const char * fmt, va_list args)
{
    using namespace Sqrat;
    // We want to make sure these messages appear in succession
    // So we will push them in bulk after generating them
    std::array< MsgPtr, 3 > messages{};
    // Create a new message builder
    MsgPtr message(new Message(LOGL_ERR, true));
    // Used to acquire stack information
    SQStackInfos si;
    // Write the given error message
    message->AppendFv(fmt, args);
    // Obtain information about the current stack level
    if (SQ_SUCCEEDED(sq_stackinfos(vm, 1, &si)))
    {
        // Whether we should fall back to normal message
        bool fall_back = true;
        // Should (can) we include a snippet of code in the traceback?
        // This feature is currently available only for main thread!
        if ((m_ThreadID == std::this_thread::get_id()) && Core::Get().IsDebugging() && si.source && (si.line > 0)) {
            // Grab the associated line of code
            String code = Core::Get().FetchCodeLine(si.source, si.line-1, true);
            // Valid line of code?
            if (!code.empty())
            {
                message->AppendF("\n[\n=>Location: %s\n=>Line: %d\n=>Function: %s\n=>Code: %s\n]"
                                    , si.source, si.line, si.funcname ? si.funcname : _SC("unknown"), code.c_str());
                fall_back = false; // No need to fall back to normal message!
            }
        }
        // Should the regular message be shown instead?
        if (fall_back)
        {
            message->AppendF("\n[\n=>Location: %s\n=>Line: %d\n=>Function: %s\n]"
                    , si.source ? si.source : _SC("unknown")
                    , si.line
                    , si.funcname ? si.funcname : _SC("unknown"));
        }
    }
    else
    {
        message->AppendF("\n[\n=> Location: unknown\n=> Line: unknown\n=> Function: unknown\n]");
    }
    // Assign the error message
    messages[0] = std::move(message);
    // Create a new message builder
    message = std::make_unique<Message>(LOGL_INF, true);
    // Trace list (so it can be reused later in locals)
    std::vector< std::string > locations;
    std::vector< std::string > closures;
    // Obtain traces from the associated function call
    for (int32_t level = 1; SQ_SUCCEEDED(sq_stackinfos(vm, level, &si)); ++level)
    {
        // Store source location
        locations.emplace_back(fmt::format("{} : {}", si.source ? si.source : _SC("unknown"), si.line));
        // Store closure name
        closures.emplace_back(si.funcname ? si.funcname : _SC("unknown"));
    }
    // Begin the traceback process
    message->Append("Traceback:\n[\n");
    // Traceback the function call
    for (int32_t level = 0; level < locations.size(); ++level)
    {
        message->AppendF("=> [%d] %s in %s\n", level + 1, locations[level].c_str(), closures[level].c_str());
    }
    // End the function call traceback
    message->Append("]");
    // Assign the error message
    messages[1] = std::move(message);
    // Create a new message builder
    message = std::make_unique<Message>(LOGL_INF, true);
    // Temporary variables to retrieve stack information
    const SQChar * s_ = nullptr, * name;
    SQInteger i_;
    SQFloat f_;
    SQUserPointer p_;
    StackStrF ssf_;
    // Begin the local variables information
    message->Append("Locals:\n[\n");
    // Indentation string
    std::string indent;
    // Whether current level includes trace
    bool traced = false;
    // Process each stack level
    for (int32_t level = 0; level < 10; ++level)
    {
        SQInteger seq = 0;
        // Display all locals in the current stack level
        while((name = sq_getlocal(vm, static_cast< SQUnsignedInteger >(level), static_cast< SQUnsignedInteger >(seq))))
        {
            ++seq;
            // This first loop?
            if (!traced)
            {
                // Can we trace it? (current level trace is actually level - 1)
                if (level > 0 && static_cast< size_t >(level) <= locations.size())
                {
                    message->AppendF("%s=> [%d] %s (%s)\n", indent.c_str(), level, closures[level - 1].c_str(), locations[level - 1].c_str());
                }
                else
                {
                    message->AppendF("%s=> [%d]\n", indent.c_str(), level);
                }
                // Mark as traced
                traced = true;
            }
            // Identify type
            switch(sq_gettype(vm, -1))
            {
                case OT_NULL:
                    message->AppendF("%s|- %-10s[%s]\n", indent.c_str(), "NULL", name);
                    break;
                case OT_INTEGER:
                    sq_getinteger(vm, -1, &i_);
                    message->AppendF("%s|- %-10s[%s] with value: %" PRINT_INT_FMT "\n", indent.c_str(), "INTEGER", name, i_);
                    break;
                case OT_FLOAT:
                    sq_getfloat(vm, -1, &f_);
                    message->AppendF("%s|- %-10s[%s] with value: %f\n", indent.c_str(), "FLOAT", name, f_);
                    break;
                case OT_USERPOINTER:
                    sq_getuserpointer(vm, -1, &p_);
                    message->AppendF("%s|- %-10s[%s] pointing at: %p\n", indent.c_str(), "POINTER", name, p_);
                    break;
                case OT_STRING:
                    sq_getstringandsize(vm, -1, &s_, &i_);
                    if (i_ > 0) {
                        message->AppendF("%s|- %-10s[%s] of %" PRINT_INT_FMT " characters: %.*s\n", indent.c_str(), "STRING", name, i_, m_StringTruncate, s_);
                    } else {
                        message->AppendF("%s|- %-10s[%s] empty\n", indent.c_str(), "STRING", level, name);
                    }
                    break;
                case OT_TABLE:
                    i_ = sq_getsize(vm, -1);
                    message->AppendF("%s|- %-10s[%s] with %" PRINT_INT_FMT " elements\n", indent.c_str(), "TABLE", name, i_);
                    break;
                case OT_ARRAY:
                    i_ = sq_getsize(vm, -1);
                    message->AppendF("%s|- %-10s[%s] with %" PRINT_INT_FMT " elements\n", indent.c_str(), "ARRAY", name, i_);
                    break;
                case OT_CLOSURE:
                    s_ = _SC("@anonymous");
                    if (SQ_SUCCEEDED(sq_getclosurename(vm, -1))) {
                        if (sq_gettype(vm, -1) != OT_NULL && SQ_SUCCEEDED(ssf_.Release(vm).Proc())) {
                            s_ = ssf_.mPtr;
                        }
                        sq_poptop(vm);
                    }
                    message->AppendF("%s|- %-10s[%s] with name: %s\n", indent.c_str(), "CLOSURE", name, s_);
                    break;
                case OT_NATIVECLOSURE:
                    s_ = _SC("@unknown");
                    if (SQ_SUCCEEDED(sq_getclosurename(vm, -1))) {
                        if (sq_gettype(vm, -1) != OT_NULL && SQ_SUCCEEDED(ssf_.Release(vm).Proc())) {
                            s_ = ssf_.mPtr;
                        }
                        sq_poptop(vm);
                    }
                    message->AppendF("%s|- %-10s[%s] with name: %s\n", indent.c_str(), "NCLOSURE", name, s_);
                    break;
                case OT_GENERATOR:
                    message->AppendF("%s|- %-10s[%s]\n", indent.c_str(), "GENERATOR", name);
                    break;
                case OT_USERDATA:
                    message->AppendF("%s|- %-10s[%s]\n", indent.c_str(), "USERDATA", name);
                    break;
                case OT_THREAD:
                    message->AppendF("%s|- %-10s[%s]\n", indent.c_str(), "THREAD", name);
                    break;
                case OT_CLASS:
                    // Brute force our way into getting the name of this class without blowing up
                    s_ = _SC("@unknown");
                    // Create a dummy, non-constructed instance and hope `_typeof` doesn't rely on member variables
                    if (SQ_SUCCEEDED(sq_createinstance(vm, -1))) {
                        // Attempt a `_typeof` on that instance
                        if (SQ_SUCCEEDED(sq_typeof(vm, -1))) {
                            if (SQ_SUCCEEDED(ssf_.Release(vm).Proc())) {
                                s_ = ssf_.mPtr;
                            }
                            // Pop the name object
                            sq_poptop(vm);
                        }
                        // Pop the dummy instance
                        sq_poptop(vm);
                    }
                    message->AppendF("%s|- %-10s[%s] of type: %s\n", indent.c_str(), "CLASS", name, s_);
                    break;
                case OT_INSTANCE:
                    s_ = _SC("@unknown");
                    if (SQ_SUCCEEDED(sq_typeof(vm, -1))) {
                        if (SQ_SUCCEEDED(ssf_.Release(vm).Proc())) {
                            s_ = ssf_.mPtr;
                        }
                        sq_poptop(vm);
                    }
                    message->AppendF("%s|- %-10s[%s] of type: %s\n", indent.c_str(), "INSTANCE", name, s_);
                    break;
                case OT_WEAKREF:
                    s_ = _SC("@unknown");
                    // Attempt to grab the value pointed by the weak reference
                    if (SQ_SUCCEEDED(sq_getweakrefval(vm, -1))) {
                        // Attempt a `_typeof` on that instance
                        if (SQ_SUCCEEDED(sq_typeof(vm, -1))) {
                            if (SQ_SUCCEEDED(ssf_.Release(vm).Proc())) {
                                s_ = ssf_.mPtr;
                            }
                            // Pop the name object
                            sq_poptop(vm);
                        }
                        // Pop the referenced value
                        sq_poptop(vm);
                    }
                    message->AppendF("%s|- %-10s[%s] of type: %s\n", indent.c_str(), "WEAKREF", name, s_);
                    break;
                case OT_BOOL:
                    sq_getinteger(vm, -1, &i_);
                    message->AppendF("%s|- %-10s[%s] with value: %s\n", indent.c_str(), "BOOL", name, i_ ? _SC("true") : _SC("false"));
                    break;
                default:
                    message->AppendF("%s|- %-10s[%s]\n", indent.c_str(), "UNKNOWN", name);
                break;
            }
            sq_pop(vm, 1);
        }
        // Mark next level as untraced
        traced = false;
        // Indent for next level
        if (level != 0)
        {
            indent.push_back(' ');
        }
    }
    // End the variables information
    message->Append("]");
    // Assign the error message
    messages[2] = std::move(message);
    // Submit the error messages in bulk
    PushMessage(messages.data(), messages.size());
}

// ------------------------------------------------------------------------------------------------
#define SQMOD_LOG(N_, L_, S_) /*
*/ void N_(const char * fmt, ...) /*
*/ { /*
*/  va_list args; /*
*/  va_start(args, fmt); /*
*/  Logger::Get().SendFv(L_, S_, fmt, args); /*
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
#define SQMOD_VLOG(N_, L_, S_) /*
*/ void N_(const char * fmt, va_list vlist) /*
*/ { /*
*/  Logger::Get().SendFv(L_, S_, fmt, vlist); /*
*/ } /*
*/

// ------------------------------------------------------------------------------------------------
SQMOD_VLOG(LogDbgV, LOGL_DBG, false)
SQMOD_VLOG(LogUsrV, LOGL_USR, false)
SQMOD_VLOG(LogScsV, LOGL_SCS, false)
SQMOD_VLOG(LogInfV, LOGL_INF, false)
SQMOD_VLOG(LogWrnV, LOGL_WRN, false)
SQMOD_VLOG(LogErrV, LOGL_ERR, false)
SQMOD_VLOG(LogFtlV, LOGL_FTL, false)

// ------------------------------------------------------------------------------------------------
SQMOD_VLOG(LogSDbgV, LOGL_DBG, true)
SQMOD_VLOG(LogSUsrV, LOGL_USR, true)
SQMOD_VLOG(LogSScsV, LOGL_SCS, true)
SQMOD_VLOG(LogSInfV, LOGL_INF, true)
SQMOD_VLOG(LogSWrnV, LOGL_WRN, true)
SQMOD_VLOG(LogSErrV, LOGL_ERR, true)
SQMOD_VLOG(LogSFtlV, LOGL_FTL, true)

// ------------------------------------------------------------------------------------------------
#define SQMOD_CLOG(N_, L_, S_) /*
*/bool N_(bool c, const char * fmt, ...) /*
*/ { /*
*/  if (!c) /*
*/  { /*
*/      return c; /*
*/  } /*
*/  va_list args; /*
*/  va_start(args, fmt); /*
*/  Logger::Get().SendFv(L_, S_, fmt, args); /*
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
template < uint8_t L, bool S > static SQInteger LogBasicMessage(HSQUIRRELVM vm)
{
    const auto top = sq_gettop(vm);
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
    Logger::Get().WriteF(L, S, "%s", val.mPtr);
    // This function does not return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
template < uint8_t L > static void BindLogCallback(Function & func)
{
    Logger::Get().BindCb(L, func);
}

// ------------------------------------------------------------------------------------------------
static void SqLogClose()
{
    Logger::Get().Close();
}

// ------------------------------------------------------------------------------------------------
static void SqLogInitialize(const SQChar * filename)
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
    Logger::Get().SetConsoleLevels(ConvTo< uint8_t >::From(level));
}

// ------------------------------------------------------------------------------------------------
static bool SqLogGetConsoleLevels()
{
    return Logger::Get().GetConsoleLevels();
}

// ------------------------------------------------------------------------------------------------
static void SqLogSetLogFileLevels(SQInteger level)
{
    Logger::Get().SetLogFileLevels(ConvTo< uint8_t >::From(level));
}

// ------------------------------------------------------------------------------------------------
static bool SqLogGetLogFileLevels()
{
    return Logger::Get().GetLogFileLevels();
}

// ------------------------------------------------------------------------------------------------
static void SqLogEnableConsoleLevel(SQInteger level)
{
    Logger::Get().EnableConsoleLevel(ConvTo< uint8_t >::From(level));
}

// ------------------------------------------------------------------------------------------------
static void SqLogDisableConsoleLevel(SQInteger level)
{
    Logger::Get().DisableConsoleLevel(ConvTo< uint8_t >::From(level));
}

// ------------------------------------------------------------------------------------------------
static void SqLogToggleConsoleLevel(SQInteger level, bool toggle)
{
    Logger::Get().ToggleConsoleLevel(ConvTo< uint8_t >::From(level), toggle);
}

// ------------------------------------------------------------------------------------------------
static void SqLogEnableLogFileLevel(SQInteger level)
{
    Logger::Get().EnableLogFileLevel(ConvTo< uint8_t >::From(level));
}

// ------------------------------------------------------------------------------------------------
static void SqLogDisableLogFileLevel(SQInteger level)
{
    Logger::Get().DisableLogFileLevel(ConvTo< uint8_t >::From(level));
}

// ------------------------------------------------------------------------------------------------
static void SqLogToggleLogFileLevel(SQInteger level, bool toggle)
{
    Logger::Get().ToggleLogFileLevel(ConvTo< uint8_t >::From(level), toggle);
}

// ------------------------------------------------------------------------------------------------
static const String & SqLogGetLogFilename()
{
    return Logger::Get().GetLogFilename();
}

// ------------------------------------------------------------------------------------------------
static void SqLogSetLogFilename(const SQChar * filename)
{
    Logger::Get().SetLogFilename(filename);
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqLogGetStringTruncate()
{
    return static_cast< SQInteger >(Logger::Get().GetStringTruncate());
}

// ------------------------------------------------------------------------------------------------
static void SqLogSetStringTruncate(SQInteger nc)
{
    Logger::Get().SetStringTruncate(ConvTo< uint32_t >::From(nc));
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
        .Func(_SC("BindDbg"), &BindLogCallback< LOGL_DBG >)
        .Func(_SC("BindUsr"), &BindLogCallback< LOGL_USR >)
        .Func(_SC("BindScs"), &BindLogCallback< LOGL_SCS >)
        .Func(_SC("BindInf"), &BindLogCallback< LOGL_INF >)
        .Func(_SC("BindWrn"), &BindLogCallback< LOGL_WRN >)
        .Func(_SC("BindErr"), &BindLogCallback< LOGL_ERR >)
        .Func(_SC("BindFtl"), &BindLogCallback< LOGL_FTL >)
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
        .Func(_SC("GetStringTruncate"), &SqLogGetStringTruncate)
        .Func(_SC("SetStringTruncate"), &SqLogSetStringTruncate)
    );
}

} // Namespace:: SqMod
