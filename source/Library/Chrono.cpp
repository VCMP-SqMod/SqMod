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
    return 0; // Should we fallback to 32 bit?
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
    return time.tv_sec * 1000.0 + time.tv_nsec / 1000000.0;
}

// ------------------------------------------------------------------------------------------------
Int64 GetTickCount64()
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
Int64 GetEpochTimeMilli()
{
    return (GetEpochTimeMicro() / 1000L);
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
static SQInteger SqGetTickCount()
{
    return GetTickCount();
}

// ------------------------------------------------------------------------------------------------
static SLongInt SqGetTickCount64()
{
    return SLongInt(GetTickCount64());
}

// ------------------------------------------------------------------------------------------------
extern void Register_ChronoDate(HSQUIRRELVM vm, Table & cns);
extern void Register_ChronoDatetime(HSQUIRRELVM vm, Table & cns);
extern void Register_ChronoTime(HSQUIRRELVM vm, Table & cns);
extern void Register_ChronoTimer(HSQUIRRELVM vm, Table & cns);
extern void Register_ChronoTimestamp(HSQUIRRELVM vm, Table & cns);

// ------------------------------------------------------------------------------------------------
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
    .Func(_SC("TickCount64"), &SqGetTickCount64);

    RootTable(vm).Bind(_SC("SqChrono"), cns);
}

} // Namespace:: SqMod
