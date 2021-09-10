// ------------------------------------------------------------------------------------------------
#include "Library/Chrono.hpp"
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Timer.hpp"
#include "Library/Chrono/Timestamp.hpp"
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_OS_WINDOWS

#if defined(_SQ64) && (_WIN32_WINNT < 0x0600)
    // We need this for the GetTickCount64() function
    #undef _WIN32_WINNT
    #define _WIN32_WINNT 0x0600
#endif // _SQ64

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
const uint8_t Chrono::MonthLengths[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

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
int64_t Chrono::GetCurrentSysTime()
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
    return int64_t(1000000LL * time.QuadPart / frequency.QuadPart);
}

// ------------------------------------------------------------------------------------------------
int64_t Chrono::GetEpochTimeMicro()
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    // Extract the nanoseconds from the resulted timestamp
    uint64_t time = ft.dwHighDateTime;
    time <<= 32;
    time |= ft.dwLowDateTime;
    time /= 10;
    time -= 11644473600000000ULL;
    // Return the resulted timestamp
    return int64_t(time);
}

// ------------------------------------------------------------------------------------------------
#ifndef _SQ64
int64_t GetTickCount64()
{
    return static_cast< int64_t >(GetTickCount()); // Fall-back to 32 bit?
}
#endif // _SQ64

#else

// ------------------------------------------------------------------------------------------------
int64_t Chrono::GetCurrentSysTime()
{
    // POSIX implementation
    timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return int64_t(uint64_t(time.tv_sec) * 1000000 + time.tv_nsec / 1000);
}

// ------------------------------------------------------------------------------------------------
int64_t Chrono::GetEpochTimeMicro()
{
    // POSIX implementation
    timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return int64_t(uint64_t(time.tv_sec) * 1000000 + time.tv_nsec / 1000);
}

// ------------------------------------------------------------------------------------------------
uint32_t GetTickCount()
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
int64_t GetTickCount64()
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
int64_t Chrono::GetEpochTimeMilli()
{
    return (GetEpochTimeMicro() / 1000L);
}

// ------------------------------------------------------------------------------------------------
bool Chrono::ValidDate(uint16_t year, uint8_t month, uint8_t day)
{
    // Is this a valid date? & Is the month within range?
    if (year == 0 || month == 0 || day == 0 || month > 12)
    {
        return false;
    }
    // Return whether the day inside the month
    return day <= DaysInMonth(year, month);
}

// ------------------------------------------------------------------------------------------------
uint8_t Chrono::DaysInMonth(uint16_t year, uint8_t month)
{
    // Is the specified month within range?
    if (month > 12)
    {
        STHROWF("Month value is out of range: {} > 12", month);
    }
    // Obtain the days in this month
    uint8_t days = *(MonthLengths + month);
    // Should we account for January?
    if (month == 2 && IsLeapYear(year))
    {
        ++days;
    }
    // Return the resulted days
    return days;
}

// ------------------------------------------------------------------------------------------------
uint16_t Chrono::DayOfYear(uint16_t year, uint8_t month, uint8_t day)
{
    // Start with 0 days
    uint16_t doy = 0;
    // Cumulate the days in months
    for (uint8_t m = 1; m < month; ++month)
    {
        doy += DaysInMonth(year, m);
    }
    // Add the specified days
    doy += day;
    // Return the result
    return doy;
}

// ------------------------------------------------------------------------------------------------
Date Chrono::ReverseDayOfYear(uint16_t year, uint16_t doy)
{
    // The resulted month
    uint8_t month = 1;
    // Calculate the months till the specified day of year
    for (; month < 12; ++month)
    {
        // Get the number of days in the current month
        uint32_t days = DaysInMonth(year, month);
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
int64_t Chrono::DateRangeToSeconds(uint16_t _year, uint8_t _month, uint8_t _day, uint16_t year_, uint8_t month_, uint8_t day_)
{
    // Are we within the same year?
    if (_year == year_)
    {
        return std::abs((DayOfYear(_year, _month, _day) - DayOfYear(year_, month_, day_)) * 86400LL);
    }
    // Is the start year greater than the end year?
    else if (_year > year_)
    {
        std::swap(_year, year_);
        std::swap(_month, month_);
        std::swap(_day, day_);
    }
    // Calculate the remaining days from the first year
    int64_t num = DaysInYear(_year) - DayOfYear(_year, _month, _day);
    // Calculate the days withing the years range
    while (++_year < year_)
    {
        num += DaysInYear(_year);
    }
    // Calculate the days up to the last day
    num += DayOfYear(year_, month_, day_);
    // Convert the obtained days in seconds
    num *= 86400ULL;
    // Return the result
    return std::abs(num);
}

// ------------------------------------------------------------------------------------------------
static SQRESULT SqGetEpochTimeMicro(HSQUIRRELVM vm)
{
    sq_pushinteger(vm, Chrono::GetEpochTimeMicro());
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQRESULT SqGetEpochTimeMilli(HSQUIRRELVM vm)
{
    sq_pushinteger(vm, Chrono::GetEpochTimeMilli());
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQRESULT SqGetCurrentSysTime(HSQUIRRELVM vm)
{
    sq_pushinteger(vm, Chrono::GetCurrentSysTime());
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQRESULT SqGetTickCount(HSQUIRRELVM vm)
{
    sq_pushinteger(vm, ConvTo< SQInteger >::From(GetTickCount()));
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQRESULT SqGetTickCount64(HSQUIRRELVM vm)
{
    sq_pushinteger(vm, ConvTo< SQInteger >::From(GetTickCount64()));
    return 1;
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
    .SquirrelFunc(_SC("EpochMicro"), &SqGetEpochTimeMicro)
    .SquirrelFunc(_SC("EpochMilli"), &SqGetEpochTimeMilli)
    .SquirrelFunc(_SC("Current"), &SqGetCurrentSysTime)
    .SquirrelFunc(_SC("TickCount"), &SqGetTickCount)
    .SquirrelFunc(_SC("TickCount64"), &SqGetTickCount64)
    .Func(_SC("IsLeapYear"), &Chrono::IsLeapYear)
    .Func(_SC("IsDateValid"), &Chrono::ValidDate)
    .Func(_SC("DaysInYear"), &Chrono::DaysInYear)
    .Func(_SC("DaysInMonth"), &Chrono::DaysInMonth)
    .Func(_SC("DayOfYear"), &Chrono::DayOfYear)
    .Func(_SC("ReverseDayOfYear"), &Chrono::ReverseDayOfYear);

    RootTable(vm).Bind(_SC("SqChrono"), cns);
}

} // Namespace:: SqMod
