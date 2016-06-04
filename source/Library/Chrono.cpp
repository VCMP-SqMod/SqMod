// ------------------------------------------------------------------------------------------------
#include "Library/Chrono.hpp"
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Datetime.hpp"
#include "Library/Chrono/Time.hpp"
#include "Library/Chrono/Timer.hpp"
#include "Library/Chrono/Timestamp.hpp"
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
extern void Register_ChronoDate(HSQUIRRELVM vm, Table & cns);
extern void Register_ChronoDatetime(HSQUIRRELVM vm, Table & cns);
extern void Register_ChronoTime(HSQUIRRELVM vm, Table & cns);
extern void Register_ChronoTimer(HSQUIRRELVM vm, Table & cns);
extern void Register_ChronoTimestamp(HSQUIRRELVM vm, Table & cns);

// ------------------------------------------------------------------------------------------------
const Uint8 Chrono::MonthLengths[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_OS_WINDOWS

/* ------------------------------------------------------------------------------------------------
 * Used by GetCurrentSysTime to obtain the system frequency on initial call.
*/
inline LARGE_INTEGER GetFrequency()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return frequency;
}

// ------------------------------------------------------------------------------------------------
Int64 Chrono::GetCurrentSysTime()
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
Int64 Chrono::GetEpochTimeMicro()
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
#ifndef _SQ64
Int64 GetTickCount64()
{
    return 0ULL; // Should we fallback to 32 bit?
}
#endif // _SQ64

#else

// ------------------------------------------------------------------------------------------------
Int64 Chrono::GetCurrentSysTime()
{
    // POSIX implementation
    timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return Int64(Uint64(time.tv_sec) * 1000000 + time.tv_nsec / 1000);
}

// ------------------------------------------------------------------------------------------------
Int64 Chrono::GetEpochTimeMicro()
{
    // POSIX implementation
    timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return Int64(Uint64(time.tv_sec) * 1000000 + time.tv_nsec / 1000);
}

// ------------------------------------------------------------------------------------------------
Uint32 Chrono::GetTickCount()
{
    // POSIX implementation
    struct timespec time;
    if (clock_gettime(CLOCK_MONOTONIC, &time))
    {
        return 0;
    }
    return time.tv_sec * 1000.0 + time.tv_nsec / 1000000.0;
}

// ------------------------------------------------------------------------------------------------
Int64 Chrono::GetTickCount64()
{
    struct timespec time;
    if (clock_gettime(CLOCK_MONOTONIC, &time))
    {
        return 0;
    }
    return time.tv_sec * 1000.0 + time.tv_nsec / 1000000.0;
}

#endif // SQMOD_OS_WINDOWS

// ------------------------------------------------------------------------------------------------
Int64 Chrono::GetEpochTimeMilli()
{
    return (GetEpochTimeMicro() / 1000L);
}

// ------------------------------------------------------------------------------------------------
bool Chrono::ValidDate(Uint16 year, Uint8 month, Uint8 day)
{
    // Is this a valid date?
    if (year == 0 || month == 0 || day == 0)
    {
        return false;
    }
    // Is the month within range?
    else if (month > 12)
    {
        return false;
    }
    // Return whether the day inside the month
    return day <= DaysInMonth(year, month);
}

// ------------------------------------------------------------------------------------------------
Uint8 Chrono::DaysInMonth(Uint16 year, Uint8 month)
{
    // Is the specified month within range?
    if (month > 12)
    {
        STHROWF("Month value is out of range: %u > 12", month);
    }
    // Obtain the days in this month
    Uint8 days = *(MonthLengths + month);
    // Should we account for January?
    if (month == 2 && IsLeapYear(year))
    {
        ++days;
    }
    // Return the resulted days
    return days;
}

// ------------------------------------------------------------------------------------------------
Uint16 Chrono::DayOfYear(Uint16 year, Uint8 month, Uint8 day)
{
    // Start with 0 days
    Uint16 doy = 0;
    // Cumulate the days in months
    for (Uint8 m = 1; m < month; ++month)
    {
        doy += DaysInMonth(year, m);
    }
    // Add the specified days
    doy += day;
    // Return the result
    return doy;
}

// ------------------------------------------------------------------------------------------------
Date Chrono::ReverseDayOfyear(Uint16 year, Uint16 doy)
{
    // The resulted month
    Uint8 month = 1;
    // Calculate the months till the specified day of year
    for (; month < 12; ++month)
    {
        // Get the number of days in the current month
        Uint32 days = DaysInMonth(year, month);
        // Can this month fit in the remaining days?
        if (days >= doy)
        {
            break; // The search is complete
        }
        // Subtract the month days from days of year
        doy -= days;
    }
    // Return the resulted date
    return Date(year, month, doy);
}

// ------------------------------------------------------------------------------------------------
static SLongInt SqGetEpochTimeMicro()
{
    return SLongInt(Chrono::GetEpochTimeMicro());
}

// ------------------------------------------------------------------------------------------------
static SLongInt SqGetEpochTimeMilli()
{
    return SLongInt(Chrono::GetEpochTimeMilli());
}

// ------------------------------------------------------------------------------------------------
static SLongInt SqGetCurrentSysTime()
{
    return SLongInt(Chrono::GetCurrentSysTime());
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqGetTickCount()
{
    return ConvTo< SQInteger >::From(GetTickCount());
}

// ------------------------------------------------------------------------------------------------
static SLongInt SqGetTickCount64()
{
    return SLongInt(GetTickCount64());
}

// ================================================================================================
void Register_Chrono(HSQUIRRELVM vm)
{
    Table cns(vm);

    Register_ChronoDate(vm, cns);
    Register_ChronoDatetime(vm, cns);
    Register_ChronoTime(vm, cns);
    Register_ChronoTimer(vm, cns);
    Register_ChronoTimestamp(vm, cns);

    cns
    .Func(_SC("EpochMicro"), &SqGetEpochTimeMicro)
    .Func(_SC("EpochMilli"), &SqGetEpochTimeMilli)
    .Func(_SC("Current"), &SqGetCurrentSysTime)
    .Func(_SC("TickCount"), &SqGetTickCount)
    .Func(_SC("TickCount64"), &SqGetTickCount64)
    .Func(_SC("IsLeapYear"), &Chrono::IsLeapYear)
    .Func(_SC("IsDateValid"), &Chrono::ValidDate)
    .Func(_SC("DaysInYear"), &Chrono::DaysInYear)
    .Func(_SC("DaysInMonth"), &Chrono::DaysInMonth)
    .Func(_SC("DayOfYear"), &Chrono::DayOfYear)
    .Func(_SC("ReverseDayOfyear"), &Chrono::ReverseDayOfyear);

    RootTable(vm).Bind(_SC("SqChrono"), cns);
}

} // Namespace:: SqMod
