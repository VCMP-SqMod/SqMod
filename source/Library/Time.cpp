// ------------------------------------------------------------------------------------------------
#include "Library/Time.hpp"
#include "Library/Numeric.hpp"
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_OS_WINDOWS
    #include <windows.h>
#else
    #include <time.h>
#endif // SQMOD_OS_WINDOWS

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_OS_WINDOWS

/* ------------------------------------------------------------------------------------------------
 * Used by GetCurrentSysTime to obtain the system frequency on initial call.
*/
LARGE_INTEGER GetFrequency()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return frequency;
}

// ------------------------------------------------------------------------------------------------
Int64 GetCurrentSysTime()
{
    // Force the following code to run on first core
    // (see http://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx)
    HANDLE current_thread = GetCurrentThread();
    DWORD_PTR previous_mask = SetThreadAffinityMask(current_thread, 1);

    // Get the frequency of the performance counter
    // (it is constant across the program lifetime)
    static const LARGE_INTEGER frequency = GetFrequency();

    // Get the current time
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);

    // Restore the thread affinity
    SetThreadAffinityMask(current_thread, previous_mask);

    // Return the current time as microseconds
    return Int64(1000000LL * time.QuadPart / frequency.QuadPart);
}

// ------------------------------------------------------------------------------------------------
Int64 GetEpochTimeMicro()
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    // Extract the nanoseconds from the resulted timestamp
    Uint64 time = ft.dwHighDateTime;
    time <<= 32;
    time |= ft.dwLowDateTime;
    time /= 10;
    time -= 11644473600000000ULL;
    // Return the resulted timestamp
    return Int64(time);
}

// ------------------------------------------------------------------------------------------------
Uint32 GetTickCount()
{
    return GetTickCount();
}

// ------------------------------------------------------------------------------------------------
Int64 GetTickCount64()
{
#ifdef _SQ64
    return GetTickCount64();
#else
    return 0;
#endif // _SQ64
}

#else

// ------------------------------------------------------------------------------------------------
Int64 GetCurrentSysTime()
{
    // POSIX implementation
    timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return Int64(Uint64(time.tv_sec) * 1000000 + time.tv_nsec / 1000);
}

// ------------------------------------------------------------------------------------------------
Int64 GetEpochTimeMicro()
{
    // POSIX implementation
    timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return Int64(Uint64(time.tv_sec) * 1000000 + time.tv_nsec / 1000);
}

// ------------------------------------------------------------------------------------------------
Uint32 GetTickCount()
{
    // POSIX implementation
    struct timespec time;
    if (clock_gettime(CLOCK_MONOTONIC, &time))
    {
        return 0;
    }
    return now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0;
}

// ------------------------------------------------------------------------------------------------
Int64 GetTickCount64()
{
    struct timespec time;
    if (clock_gettime(CLOCK_MONOTONIC, &time))
    {
        return 0;
    }
    return now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0;
}

#endif // SQMOD_OS_WINDOWS

// ------------------------------------------------------------------------------------------------
Int64 GetEpochTimeMilli()
{
    return (GetEpochTimeMicro() / 1000L);
}

// ------------------------------------------------------------------------------------------------
Timestamp::Timestamp(const SLongInt & t)
    : m_Timestamp(t.GetNum())
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Int32 Timestamp::Cmp(const Timestamp & o) const
{
    if (m_Timestamp == o.m_Timestamp)
        return 0;
    else if (m_Timestamp > o.m_Timestamp)
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
CSStr Timestamp::ToString() const
{
    return ToStrF("%lld", m_Timestamp);
}

// ------------------------------------------------------------------------------------------------
void Timestamp::SetNow()
{
    m_Timestamp = GetCurrentSysTime();
}

// ------------------------------------------------------------------------------------------------
SLongInt Timestamp::GetMicroseconds() const
{
    return SLongInt(m_Timestamp);
}

// ------------------------------------------------------------------------------------------------
void Timestamp::SetMicroseconds(const SLongInt & ammount)
{
    m_Timestamp = ammount.GetNum();
}

// ------------------------------------------------------------------------------------------------
SLongInt Timestamp::GetMilliseconds() const
{
    return SLongInt(m_Timestamp / 1000L);
}

// ------------------------------------------------------------------------------------------------
void Timestamp::SetMilliseconds(const SLongInt & ammount)
{
    m_Timestamp = (ammount.GetNum() * 1000L);
}

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

// ------------------------------------------------------------------------------------------------
static SLongInt SqGetEpochTimeMicro()
{
    return SLongInt(GetEpochTimeMicro());
}

// ------------------------------------------------------------------------------------------------
static SLongInt SqGetEpochTimeMilli()
{
    return SLongInt(GetEpochTimeMilli());
}

// ------------------------------------------------------------------------------------------------
static SLongInt SqGetCurrentSysTime()
{
    return SLongInt(GetCurrentSysTime());
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetEpochTimeNow()
{
    return Timestamp(GetEpochTimeMicro());
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetMicrosecondsRaw(Int64 ammount)
{
    return Timestamp(ammount);
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetMicroseconds(const SLongInt & ammount)
{
    return Timestamp(ammount);
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetMilliseconds(SQInteger ammount)
{
    return Timestamp(Int64(Int64(ammount) * 1000L));
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetSeconds(SQFloat ammount)
{
    return Timestamp(Int64(Float64(ammount) * 1000000L));
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetMinutes(SQFloat ammount)
{
    return Timestamp(Int64((Float64(ammount) * 60000000L)));
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetHours(SQFloat ammount)
{
    return Timestamp(Int64(Float64(ammount) * 3600000000LL));
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetDays(SQFloat ammount)
{
    return Timestamp(Int64(Float64(ammount) * 86400000000LL));
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetYears(SQFloat ammount)
{
    return Timestamp(Int64(Float64(ammount) * 31557600000000LL));
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqGetTickCount()
{
    return GetTickCount();
}

// ------------------------------------------------------------------------------------------------
static SLongInt SqGetTickCount64()
{
    return SLongInt(GetTickCount64());
}

// ================================================================================================
void Register_Time(HSQUIRRELVM vm)
{
    Table timens(vm);

    timens.Bind(_SC("Stamp"), Class< Timestamp >(vm, _SC("SqTimestamp"))
        /* Constructors */
        .Ctor()
        .Ctor< const Timestamp & >()
        /* Core Metamethods */
        .Func(_SC("_tostring"), &Timestamp::ToString)
        .Func(_SC("_cmp"), &Timestamp::Cmp)
        /* Metamethods */
        .Func< Timestamp (Timestamp::*)(const Timestamp &) const >(_SC("_add"), &Timestamp::operator +)
        .Func< Timestamp (Timestamp::*)(const Timestamp &) const >(_SC("_sub"), &Timestamp::operator -)
        .Func< Timestamp (Timestamp::*)(const Timestamp &) const >(_SC("_mul"), &Timestamp::operator *)
        .Func< Timestamp (Timestamp::*)(const Timestamp &) const >(_SC("_div"), &Timestamp::operator /)
        /* Properties */
        .Prop(_SC("Microseconds"), &Timestamp::GetMicroseconds, &Timestamp::SetMicroseconds)
        .Prop(_SC("MicrosecondsRaw"), &Timestamp::GetMicrosecondsRaw, &Timestamp::SetMicrosecondsRaw)
        .Prop(_SC("Milliseconds"), &Timestamp::GetMilliseconds, &Timestamp::SetMilliseconds)
        .Prop(_SC("MillisecondsRaw"), &Timestamp::GetMillisecondsRaw, &Timestamp::SetMillisecondsRaw)
        .Prop(_SC("SecondsF"), &Timestamp::GetSecondsF, &Timestamp::SetSecondsF)
        .Prop(_SC("SecondsI"), &Timestamp::GetSecondsI, &Timestamp::SetSecondsI)
        .Prop(_SC("MinutesF"), &Timestamp::GetMinutesF, &Timestamp::SetMinutesF)
        .Prop(_SC("MinutesI"), &Timestamp::GetMinutesI, &Timestamp::SetMinutesI)
        .Prop(_SC("HoursF"), &Timestamp::GetHoursF, &Timestamp::SetHoursF)
        .Prop(_SC("HoursI"), &Timestamp::GetHoursI, &Timestamp::SetHoursI)
        .Prop(_SC("DaysF"), &Timestamp::GetDaysF, &Timestamp::SetDaysF)
        .Prop(_SC("DaysI"), &Timestamp::GetDaysI, &Timestamp::SetDaysI)
        .Prop(_SC("YearsF"), &Timestamp::GetYearsF, &Timestamp::SetYearsF)
        .Prop(_SC("YearsI"), &Timestamp::GetYearsI, &Timestamp::SetYearsI)
        /* Functions */
        .Func(_SC("SetNow"), &Timestamp::SetNow)
    );

    timens.Bind(_SC("Timer"), Class< Timer >(vm, _SC("SqTimer"))
        /* Constructors */
        .Ctor()
        .Ctor< const Timer & >()
        /* Core Metamethods */
        .Func(_SC("_tostring"), &Timer::ToString)
        .Func(_SC("_cmp"), &Timer::Cmp)
        /* Properties */
        .Prop(_SC("Elapsed"), &Timer::GetElapsedTime)
        .Prop(_SC("ElapsedRaw"), &Timer::GetElapsedTimeRaw)
        /* Functions */
        .Func(_SC("Reset"), &Timer::Reset)
        .Func(_SC("Restart"), &Timer::Restart)
        .Func(_SC("RestartRaw"), &Timer::RestartRaw)
    );

    timens
    .Func(_SC("EpochMicro"), &SqGetEpochTimeMicro)
    .Func(_SC("EpochMilli"), &SqGetEpochTimeMilli)
    .Func(_SC("Current"), &SqGetCurrentSysTime)
    .Func(_SC("Now"), &SqGetEpochTimeNow)
    .Func(_SC("MicrosecondsRaw"), &SqGetMicrosecondsRaw)
    .Func(_SC("Microseconds"), &SqGetMicroseconds)
    .Func(_SC("Milliseconds"), &SqGetMilliseconds)
    .Func(_SC("Seconds"), &SqGetSeconds)
    .Func(_SC("Minutes"), &SqGetMinutes)
    .Func(_SC("Hours"), &SqGetHours)
    .Func(_SC("Days"), &SqGetDays)
    .Func(_SC("Years"), &SqGetYears)
    .Func(_SC("TickCount"), &SqGetTickCount)
    .Func(_SC("TickCount64"), &SqGetTickCount64);

    RootTable(vm).Bind(_SC("SqTime"), timens);
}

} // Namespace:: SqMod
