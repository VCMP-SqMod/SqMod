// ------------------------------------------------------------------------------------------------
#include "Library/Chrono/Timer.hpp"
#include "Library/Chrono/Timestamp.hpp"
#include "Library/Chrono.hpp"
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Timer::Timer()
    : m_Timestamp(GetCurrentSysTime())
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Int32 Timer::Cmp(const Timer & o) const
{
    if (m_Timestamp == o.m_Timestamp)
        return 0;
    else if (m_Timestamp > o.m_Timestamp)
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
CSStr Timer::ToString() const
{
    return ToStrF("%lld", m_Timestamp);
}

// ------------------------------------------------------------------------------------------------
void Timer::Reset()
{
    m_Timestamp = GetCurrentSysTime();
}

// ------------------------------------------------------------------------------------------------
Timestamp Timer::Restart()
{
    const Int64 now = GetCurrentSysTime(), elapsed = now - m_Timestamp;
    m_Timestamp = now;
    return Timestamp(elapsed);
}

// ------------------------------------------------------------------------------------------------
Int64 Timer::RestartRaw()
{
    const Int64 now = GetCurrentSysTime(), elapsed = now - m_Timestamp;
    m_Timestamp = now;
    return elapsed;
}

// ------------------------------------------------------------------------------------------------
Timestamp Timer::GetElapsedTime() const
{
    return Timestamp(GetCurrentSysTime() - m_Timestamp);
}

// ------------------------------------------------------------------------------------------------
Int64 Timer::GetElapsedTimeRaw() const
{
    return (GetCurrentSysTime() - m_Timestamp);
}

// ================================================================================================
void Register_ChronoTimer(HSQUIRRELVM vm, Table & cns)
{
    RootTable(vm).Bind(_SC("SqTimer"), Class< Timer >(vm, _SC("SqChronoTimer"))
        // Constructors
        .Ctor()
        .Ctor< const Timer & >()
        // Core Metamethods
        .Func(_SC("_tostring"), &Timer::ToString)
        .Func(_SC("_cmp"), &Timer::Cmp)
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
