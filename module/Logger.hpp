#pragma once

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <string>

// ------------------------------------------------------------------------------------------------
#include <thread>

// ------------------------------------------------------------------------------------------------
#include <sqratFunction.h>
#include <concurrentqueue.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Supported levels of logging.
*/
enum LogLvl
{
    LOGL_NIL = (1u << 0u),
    LOGL_DBG = (1u << 1u),
    LOGL_USR = (1u << 2u),
    LOGL_SCS = (1u << 3u),
    LOGL_INF = (1u << 4u),
    LOGL_WRN = (1u << 5u),
    LOGL_ERR = (1u << 6u),
    LOGL_FTL = (1u << 7u),
    LOGL_ANY = 0xFFu
};

/* ------------------------------------------------------------------------------------------------
 * Class responsible for logging output.
*/
class Logger
{
protected:
    /* --------------------------------------------------------------------------------------------
     * Message storage and builder.
    */
    struct Message
    {
        // ----------------------------------------------------------------------------------------
        static constexpr size_t TMS_LEN = (128-sizeof(String)-6-sizeof(bool)-sizeof(bool));
        // ----------------------------------------------------------------------------------------
        String      mStr; // Message string.
        uint32_t    mLen; // Message length.
        uint8_t     mLvl; // Message level.
        uint8_t     mInc; // Message increments.
        bool        mSub; // Message hierarchy.
        bool        mTms; // Message time-stamp.
        char        mBuf[TMS_LEN]; // Message time-stamp.
        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        Message()
            : mStr(), mLen(0), mLvl(LOGL_NIL)
            , mInc(std::numeric_limits< uint8_t >::max()), mSub(false), mTms(false), mBuf{'\0'}
        {
        }
        /* ----------------------------------------------------------------------------------------
         * Explicit type constructor.
        */
        Message(uint8_t lvl, bool sub)
            : mStr(), mLen(0), mLvl(lvl)
            , mInc(std::numeric_limits< uint8_t >::max()), mSub(sub), mTms(false), mBuf{'\0'}
        {
        }
        /* ----------------------------------------------------------------------------------------
         * Explicit constructor.
        */
        Message(uint8_t lvl, bool sub, uint32_t len)
            : Message(lvl, sub, len, std::numeric_limits< uint8_t >::max())
        {
        }
        /* ----------------------------------------------------------------------------------------
         * Explicit constructor.
        */
        Message(uint8_t lvl, bool sub, uint32_t len, uint8_t inc)
            : mStr(), mLen(0), mLvl(lvl), mInc(inc), mSub(sub), mTms(false), mBuf{'\0'}
        {
            mStr.resize(len, '\0');
        }
        /* ----------------------------------------------------------------------------------------
         * Copy constructor (disabled).
        */
        Message(const Message & o) = delete;
        /* ----------------------------------------------------------------------------------------
         * Stamp the log message.
        */
        void Stamp();
        /* ----------------------------------------------------------------------------------------
         * Finished the string message.
        */
        void Finish()
        {
            mStr.resize(mLen); // Discard trailing characters
        }
        /* ----------------------------------------------------------------------------------------
         * Append a C string to the message.
        */
        uint32_t Append(const SQChar * str);
        /* ----------------------------------------------------------------------------------------
         * Append a fixed width string to the message.
        */
        uint32_t Append(const SQChar * str, size_t len);
        /* ----------------------------------------------------------------------------------------
         * Append a formatted string to the message.
        */
        uint32_t AppendF(const SQChar * str, ...);
        /* ----------------------------------------------------------------------------------------
         * Append a formatted string to the message.
        */
        uint32_t AppendFv(const SQChar * str, va_list vl);
    };

public:

    /* --------------------------------------------------------------------------------------------
     * Smart message pointer.
    */
    using MsgPtr = std::unique_ptr< Message >;

private:

    // --------------------------------------------------------------------------------------------
    static Logger s_Inst; // Logger instance.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Logger() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Logger();

    /* --------------------------------------------------------------------------------------------
     * Queue of messages written from other threads.
    */
    using MsgQueue = moodycamel::ConcurrentQueue< MsgPtr >;

private:

    // --------------------------------------------------------------------------------------------
    std::thread::id m_ThreadID; // ID of the thread in which the logger was initialized.

    // --------------------------------------------------------------------------------------------
    MsgPtr          m_Message; // Last and/or currently processed log message.
    MsgQueue        m_Queue; // Queue of messages outside of main thread.

    // --------------------------------------------------------------------------------------------
    uint8_t         m_ConsoleLevels; // The levels allowed to be outputted to console.
    uint8_t         m_LogFileLevels; // The levels allowed to be outputted to log file.

    // --------------------------------------------------------------------------------------------
    bool            m_ConsoleTime; // Whether console messages should be timestamped.
    bool            m_LogFileTime; // Whether log file messages should be timestamped.
    bool            m_CyclicLock; // Prevent the script callback from entering a loop.

    // --------------------------------------------------------------------------------------------
    uint32_t        m_StringTruncate; // The length at which to truncate strings in debug.

    // --------------------------------------------------------------------------------------------
    std::FILE*      m_File; // Handle to the file where the logs should be saved.
    std::string     m_Filename; // The name of the file where the logs are saved.

    // --------------------------------------------------------------------------------------------
    Function        m_LogCb[7]; //Callback to receive debug information instead of console.

protected:

    /* --------------------------------------------------------------------------------------------
     * Push the given message either to the screen or queue depending on the calling thread.
    */
    void PushMessage(MsgPtr & msg);

    /* --------------------------------------------------------------------------------------------
     * Push the given messages either to the screen or queue depending on the calling thread.
    */
    void PushMessage(MsgPtr * msg, size_t len);

    /* --------------------------------------------------------------------------------------------
     * Process the message in the internal buffer.
    */
    void ProcessMessage();

public:

    /* --------------------------------------------------------------------------------------------
     * Retrieve the logger instance.
    */
    static Logger & Get()
    {
        return s_Inst;
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Logger(const Logger & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    Logger(Logger && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Logger & operator = (const Logger & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Logger & operator = (Logger && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Flush buffered data and close the logging file.
    */
    void Close();

    /* --------------------------------------------------------------------------------------------
     * Initialize the logging utility.
    */
    void Initialize(const char * filename);

    /* --------------------------------------------------------------------------------------------
     * Terminate the logging utility.
    */
    void Terminate();

    /* --------------------------------------------------------------------------------------------
     * Release the script associated resources.
    */
    void Release();

    /* --------------------------------------------------------------------------------------------
     * Processes the messages that have gathered in the queue.
    */
    void ProcessQueue();

    /* --------------------------------------------------------------------------------------------
     * Enable or disable console message time stamping.
    */
    void ToggleConsoleTime(bool enabled)
    {
        m_ConsoleTime = enabled;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether console message time stamping is enabled.
    */
    SQMOD_NODISCARD bool ConsoleHasTime() const
    {
        return m_ConsoleTime;
    }

    /* --------------------------------------------------------------------------------------------
     * Enable or disable log file message time stamping.
    */
    void ToggleLogFileTime(bool enabled)
    {
        m_LogFileTime = enabled;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether log file message time stamping is enabled.
    */
    SQMOD_NODISCARD bool LogFileHasTime() const
    {
        return m_LogFileTime;
    }

    /* --------------------------------------------------------------------------------------------
     * Set the console level flags.
    */
    void SetConsoleLevels(uint8_t levels)
    {
        m_ConsoleLevels = levels;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the console level flags.
    */
    SQMOD_NODISCARD uint8_t GetConsoleLevels() const
    {
        return m_ConsoleLevels;
    }

    /* --------------------------------------------------------------------------------------------
     * Set the log file level flags.
    */
    void SetLogFileLevels(uint8_t levels)
    {
        m_LogFileLevels = levels;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the log file level flags.
    */
    SQMOD_NODISCARD uint8_t GetLogFileLevels() const
    {
        return m_LogFileLevels;
    }

    /* --------------------------------------------------------------------------------------------
     * Enable a certain console logging level.
    */
    void EnableConsoleLevel(uint8_t level)
    {
        m_ConsoleLevels |= level;
    }

    /* --------------------------------------------------------------------------------------------
     * Disable a certain console logging level.
    */
    void DisableConsoleLevel(uint8_t level)
    {
        if (m_ConsoleLevels & level)
        {
            m_ConsoleLevels ^= level;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Toggle a certain console logging level.
    */
    void ToggleConsoleLevel(uint8_t level, bool toggle)
    {
        if (toggle)
        {
            EnableConsoleLevel(level);
        }
        else
        {
            DisableConsoleLevel(level);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Enable a certain log file logging level.
    */
    void EnableLogFileLevel(uint8_t level)
    {
        m_LogFileLevels |= level;
    }

    /* --------------------------------------------------------------------------------------------
     * Disable a certain log file logging level.
    */
    void DisableLogFileLevel(uint8_t level)
    {
        m_LogFileLevels |= level;
        m_LogFileLevels ^= level;
    }

    /* --------------------------------------------------------------------------------------------
     * Toggle a certain log file logging level.
    */
    void ToggleLogFileLevel(uint8_t level, bool toggle)
    {
        if (toggle)
        {
            EnableLogFileLevel(level);
        }
        else
        {
            DisableLogFileLevel(level);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of characters that strings will be truncated in debug output.
    */
    SQMOD_NODISCARD uint32_t GetStringTruncate() const
    {
        return m_StringTruncate;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of characters that strings will be truncated in debug output.
    */
    void SetStringTruncate(uint32_t nc)
    {
        m_StringTruncate = nc;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the log file name.
    */
    SQMOD_NODISCARD const std::string & GetLogFilename() const
    {
        return m_Filename;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the log file name.
    */
    void SetLogFilename(const char * filename);

    /* --------------------------------------------------------------------------------------------
     * Bind a script callback to a log level.
    */
    void BindCb(uint8_t level, Function & func);

    /* --------------------------------------------------------------------------------------------
     * Send a log message.
    */
    void Send(uint8_t level, bool sub, const char * msg, size_t len);

    /* --------------------------------------------------------------------------------------------
     * Send a log message.
    */
    void SendFv(uint8_t level, bool sub, const char * fmt, va_list args);

    /* --------------------------------------------------------------------------------------------
     * Write a log message.
    */
    void WriteF(uint8_t level, bool sub, const char * fmt, ...);

    /* --------------------------------------------------------------------------------------------
     * Generate a debug message.
    */
    void DebugF(HSQUIRRELVM vm, const char * fmt, ...);

    /* --------------------------------------------------------------------------------------------
     * Generate a debug message.
    */
    void DebugFv(HSQUIRRELVM vm, const char * fmt, va_list args);

private:

    /* --------------------------------------------------------------------------------------------
     * Forward the log message to a callback.
    */
    SQBool ProcessCb();
};

/* ------------------------------------------------------------------------------------------------
 * Raw console message output.
*/
void OutputDebug(const char * msg, ...);

/* ------------------------------------------------------------------------------------------------
 * Raw console message output.
*/
void OutputMessage(const char * msg, ...);

/* ------------------------------------------------------------------------------------------------
 * Raw console message output.
*/
void OutputError(const char * msg, ...);

} // Namespace:: SqMod
