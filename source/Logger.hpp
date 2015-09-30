#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
class Logger
{
public:
    // --------------------------------------------------------------------------------------------
    static constexpr Uint8 LEVEL_NIL = (1 << 0);
    static constexpr Uint8 LEVEL_DBG = (1 << 1);
    static constexpr Uint8 LEVEL_MSG = (1 << 2);
    static constexpr Uint8 LEVEL_SCS = (1 << 3);
    static constexpr Uint8 LEVEL_INF = (1 << 4);
    static constexpr Uint8 LEVEL_WRN = (1 << 5);
    static constexpr Uint8 LEVEL_ERR = (1 << 6);
    static constexpr Uint8 LEVEL_FTL = (1 << 7);
    static constexpr Uint8 LEVEL_ANY = 0xFF;

private:
    // --------------------------------------------------------------------------------------------
    friend class std::unique_ptr<Logger, void(*)(Logger *)>;

    // --------------------------------------------------------------------------------------------
    Logger() noexcept;

    // --------------------------------------------------------------------------------------------
    ~Logger();

    // --------------------------------------------------------------------------------------------
    static void _Finalizer(Logger * ptr) noexcept;

    // --------------------------------------------------------------------------------------------
public:
    // --------------------------------------------------------------------------------------------
    typedef std::unique_ptr<Logger, void(*)(Logger *)>    Pointer;

    // --------------------------------------------------------------------------------------------
    static Pointer Inst() noexcept;

    // --------------------------------------------------------------------------------------------
    bool Init();
    bool Load();

    // --------------------------------------------------------------------------------------------
    void Deinit();
    void Unload();

    // --------------------------------------------------------------------------------------------
    void Terminate();

    // --------------------------------------------------------------------------------------------
    void ToggleConsoleTime(bool enabled) noexcept;
    void ToggleFileTime(bool enabled) noexcept;
    bool HasConsoleTime() const noexcept;
    bool HasFileTime() const noexcept;

    // --------------------------------------------------------------------------------------------
    void SetConsoleLevels(Uint8 levels) noexcept;
    void SetFileLevels(Uint8 levels) noexcept;
    Uint8 GetConsoleLevels() const noexcept;
    Uint8 GetFileLevels() const noexcept;

    // --------------------------------------------------------------------------------------------
    void EnableConsoleLevel(Uint8 level) noexcept;
    void EnableFileLevel(Uint8 level) noexcept;
    void DisableConsoleLevel(Uint8 level) noexcept;
    void DisableFileLevel(Uint8 level) noexcept;

    // --------------------------------------------------------------------------------------------
    Uint8 GetDebugLevel() const noexcept;
    void SetDebugLevel(Uint8 level) noexcept;

    // --------------------------------------------------------------------------------------------
    SQInt32 GetVerbosity() const noexcept;
    void SetVerbosity(SQInt32 level) noexcept;

public:
    // --------------------------------------------------------------------------------------------
    void Send(Uint8 type, bool sub, const char * fmt, va_list args) noexcept;

    // --------------------------------------------------------------------------------------------
    void Dbg(const char * fmt, ...) noexcept;
    void Msg(const char * fmt, ...) noexcept;
    void Scs(const char * fmt, ...) noexcept;
    void Inf(const char * fmt, ...) noexcept;
    void Wrn(const char * fmt, ...) noexcept;
    void Err(const char * fmt, ...) noexcept;
    void Ftl(const char * fmt, ...) noexcept;

    // --------------------------------------------------------------------------------------------
    void SDbg(const char * fmt, ...) noexcept;
    void SMsg(const char * fmt, ...) noexcept;
    void SScs(const char * fmt, ...) noexcept;
    void SInf(const char * fmt, ...) noexcept;
    void SWrn(const char * fmt, ...) noexcept;
    void SErr(const char * fmt, ...) noexcept;
    void SFtl(const char * fmt, ...) noexcept;

    // --------------------------------------------------------------------------------------------
    bool cDbg(bool cond, const char * fmt, ...) noexcept;
    bool cMsg(bool cond, const char * fmt, ...) noexcept;
    bool cScs(bool cond, const char * fmt, ...) noexcept;
    bool cInf(bool cond, const char * fmt, ...) noexcept;
    bool cWrn(bool cond, const char * fmt, ...) noexcept;
    bool cErr(bool cond, const char * fmt, ...) noexcept;
    bool cFtl(bool cond, const char * fmt, ...) noexcept;

    // --------------------------------------------------------------------------------------------
    bool cSDbg(bool cond, const char * fmt, ...) noexcept;
    bool cSMsg(bool cond, const char * fmt, ...) noexcept;
    bool cSScs(bool cond, const char * fmt, ...) noexcept;
    bool cSInf(bool cond, const char * fmt, ...) noexcept;
    bool cSWrn(bool cond, const char * fmt, ...) noexcept;
    bool cSErr(bool cond, const char * fmt, ...) noexcept;
    bool cSFtl(bool cond, const char * fmt, ...) noexcept;

private:
    // --------------------------------------------------------------------------------------------
    bool        m_ConsoleTime;
    bool        m_FileTime;

    // --------------------------------------------------------------------------------------------
    Uint8       m_ConsoleLevels;
    Uint8       m_FileLevels;

    // --------------------------------------------------------------------------------------------
    String      m_LogPath;
    
    // --------------------------------------------------------------------------------------------
    Uint8       m_DebugLevel;

    // --------------------------------------------------------------------------------------------
    SQInt32     m_Verbosity;
};

// --------------------------------------------------------------------------------------------
extern const Logger::Pointer _Log;

} // Namespace:: SqMod

#endif // _LOGGER_HPP_
