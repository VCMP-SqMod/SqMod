// ------------------------------------------------------------------------------------------------
#include "Library/Chrono/Timer.hpp"
#include "Library/Chrono/Timestamp.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("SqTimer"))

// ------------------------------------------------------------------------------------------------
Timer::Timer()
    : m_Timestamp(Chrono::GetCurrentSysTime())
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
int32_t Timer::Cmp(const Timer & o) const
{
    if (m_Timestamp == o.m_Timestamp)
        return 0;
    else if (m_Timestamp > o.m_Timestamp)
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
String Timer::ToString() const
{
    return fmt::format("{}", m_Timestamp);
}

// ------------------------------------------------------------------------------------------------
void Timer::Reset()
{
    m_Timestamp = Chrono::GetCurrentSysTime();
}

// ------------------------------------------------------------------------------------------------
Timestamp Timer::Restart()
{
    const int64_t now = Chrono::GetCurrentSysTime(), elapsed = now - m_Timestamp;
    m_Timestamp = now;
    return Timestamp(elapsed);
}

// ------------------------------------------------------------------------------------------------
int64_t Timer::RestartRaw()
{
    const int64_t now = Chrono::GetCurrentSysTime(), elapsed = now - m_Timestamp;
    m_Timestamp = now;
    return elapsed;
}

// ------------------------------------------------------------------------------------------------
Timestamp Timer::GetElapsedTime() const
{
    return Timestamp(Chrono::GetCurrentSysTime() - m_Timestamp);
}

// ------------------------------------------------------------------------------------------------
int64_t Timer::GetElapsedTimeRaw() const
{
    return (Chrono::GetCurrentSysTime() - m_Timestamp);
}

// ================================================================================================
void Register_ChronoTimer(HSQUIRRELVM vm, Table & /*cns*/)
{
    RootTable(vm).Bind(Typename::Str,
        Class< Timer >(vm, Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< const Timer & >()
        // Core Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Timer::ToString)
        .Func(_SC("cmp"), &Timer::Cmp)
        // Properties
        .Prop(_SC("Elapsed"), &Timer::GetElapsedTime)
        .Prop(_SC("ElapsedRaw"), &Timer::GetElapsedTimeRaw)
        // Functions
        .Func(_SC("Reset"), &Timer::Reset)
        .Func(_SC("Restart"), &Timer::Restart)
        .Func(_SC("RestartRaw"), &Timer::RestartRaw)
    );
}

} // Namespace:: SqMod
