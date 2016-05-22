#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"
#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <string>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Supported levels of logging.
*/
enum LogLvl
{
    LOGL_NIL = (1 << 0),
    LOGL_DBG = (1 << 1),
    LOGL_USR = (1 << 2),
    LOGL_SCS = (1 << 3),
    LOGL_INF = (1 << 4),
    LOGL_WRN = (1 << 5),
    LOGL_ERR = (1 << 6),
    LOGL_FTL = (1 << 7),
    LOGL_ANY = 0xFF
};

/* ------------------------------------------------------------------------------------------------
 * Class responsible for logging output.
*/
class Logger
{
private:

    // --------------------------------------------------------------------------------------------
    static Logger s_Inst; // Logger instance.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Logger();

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Logger(const Logger & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    Logger(Logger && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Logger();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Logger & operator = (const Logger & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Logger & operator = (Logger && o) = delete;

private:

    // --------------------------------------------------------------------------------------------
    Buffer      m_Buffer; // Common buffer where the message is written.

    // --------------------------------------------------------------------------------------------
    Uint8       m_ConsoleLevels; // The levels allowed to be outputted to console.
    Uint8       m_LogFileLevels; // The levels allowed to be outputted to log file.

    // --------------------------------------------------------------------------------------------
    bool        m_ConsoleTime; // Whether console messages should be timestamped.
    bool        m_LogFileTime; // Whether log file messages should be timestamped.

    // --------------------------------------------------------------------------------------------
    std::FILE*  m_File; // Handle to the file where the logs should be saved.
    std::string m_Filename; // The name of the file where the logs are saved.

protected:

    /* --------------------------------------------------------------------------------------------
     * Process the message in the internal buffer.
    */
    void Proccess(Uint8 level, bool sub);

public:

    /* --------------------------------------------------------------------------------------------
     * Retrieve the logger instance.
    */
    static Logger & Get()
    {
        return s_Inst;
    }

    /* --------------------------------------------------------------------------------------------
     * Flush buffered data and close the logging file.
    */
    void Close();

    /* --------------------------------------------------------------------------------------------
     * Initialize the logging utility.
    */
    void Initialize(CCStr filename);

    /* --------------------------------------------------------------------------------------------
     * Terminate the logging utility.
    */
    void Terminate();

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
    bool ConsoleHasTime() const
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
    bool LogFileHasTime() const
    {
        return m_LogFileTime;
    }

    /* --------------------------------------------------------------------------------------------
     * Set the console level flags.
    */
    void SetConsoleLevels(Uint8 levels)
    {
        m_ConsoleLevels = levels;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the console level flags.
    */
    Uint8 GetConsoleLevels() const
    {
        return m_ConsoleLevels;
    }

    /* --------------------------------------------------------------------------------------------
     * Set the log file level flags.
    */
    void SetLogFileLevels(Uint8 levels)
    {
        m_LogFileLevels = levels;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the log file level flags.
    */
    Uint8 GetLogFileLevels() const
    {
        return m_LogFileLevels;
    }

    /* --------------------------------------------------------------------------------------------
     * Enable a certain console logging level.
    */
    void EnableConsoleLevel(Uint8 level)
    {
        m_ConsoleLevels |= level;
    }

    /* --------------------------------------------------------------------------------------------
     * Disable a certain console logging level.
    */
    void DisableConsoleLevel(Uint8 level)
    {
        if (m_ConsoleLevels & level)
        {
            m_ConsoleLevels ^= level;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Toggle a certain console logging level.
    */
    void ToggleConsoleLevel(Uint8 level, bool toggle)
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
    void EnableLogFileLevel(Uint8 level)
    {
        m_LogFileLevels |= level;
    }

    /* --------------------------------------------------------------------------------------------
     * Disable a certain log file logging level.
    */
    void DisableLogFileLevel(Uint8 level)
    {
        m_LogFileLevels |= level;
        m_LogFileLevels ^= level;
    }

    /* --------------------------------------------------------------------------------------------
     * Toggle a certain log file logging level.
    */
    void ToggleLogFileLevel(Uint8 level, bool toggle)
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
     * Retrieve the log file name.
    */
    const std::string & GetLogFilename() const
    {
        return m_Filename;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the log file name.
    */
    void SetLogFilename(CCStr filename);

    /* --------------------------------------------------------------------------------------------
     * Send a log message.
    */
    void Send(Uint8 level, bool sub, CCStr fmt, va_list args);

    /* --------------------------------------------------------------------------------------------
     * Write a log message.
    */
    void Write(Uint8 level, bool sub, CCStr fmt, ...);

    /* --------------------------------------------------------------------------------------------
     * Generate a debug message.
    */
    void Debug(CCStr fmt, ...);

    /* --------------------------------------------------------------------------------------------
     * Generate a debug message.
    */
    void Debug(CCStr fmt, va_list args);

};

/* ------------------------------------------------------------------------------------------------
 * Raw console message output.
*/
void OutputDebug(CCStr msg, ...);

/* ------------------------------------------------------------------------------------------------
 * Raw console message output.
*/
void OutputMessage(CCStr msg, ...);

/* ------------------------------------------------------------------------------------------------
 * Raw console message output.
*/
void OutputError(CCStr msg, ...);

} // Namespace:: SqMod

#endif // _LOGGER_HPP_
