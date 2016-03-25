// ------------------------------------------------------------------------------------------------
#include "Library/Chrono/Time.hpp"
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Datetime.hpp"
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Time::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqChronoTime");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Time Time::operator + (const Time & o) const
{
    return Time(o);
}

// ------------------------------------------------------------------------------------------------
Time Time::operator - (const Time & o) const
{
    return Time(o);
}

// ------------------------------------------------------------------------------------------------
Time Time::operator * (const Time & o) const
{
    return Time(o);
}

// ------------------------------------------------------------------------------------------------
Time Time::operator / (const Time & o) const
{
    return Time(o);
}

// ------------------------------------------------------------------------------------------------
void Time::Set(Uint8 hour, Uint8 minute, Uint8 second, Uint16 millisecond)
{
    // Is the specified hour within range?
    if (hour >= 24)
    {
        STHROWF("Hour value is out of range: %u >= 24", hour);
    }
    // Is the specified minute within range?
    else if (minute >= 60)
    {
        STHROWF("Minute value is out of range: %u >= 60", minute);
    }
    // Is the specified second within range?
    else if (second >= 60)
    {
        STHROWF("Second value is out of range: %u >= 60", second);
    }
    // Is the specified millisecond within range?
    else if (millisecond >= 1000)
    {
        STHROWF("Millisecond value is out of range: %u >= 1000", millisecond);
    }
    // Now it's safe to assign the values
    m_Hour = hour;
    m_Minute = minute;
    m_Second = second;
    m_Millisecond = millisecond;
}

// ------------------------------------------------------------------------------------------------
CSStr Time::GetStr() const
{
    return ToString();
}

// ------------------------------------------------------------------------------------------------
void Time::SetStr(CSStr /*str*/)
{

}

// ------------------------------------------------------------------------------------------------
Int32 Time::Compare(const Time & o) const
{
    if (m_Hour < o.m_Hour)
    {
        return -1;
    }
    else if (m_Hour > o.m_Hour)
    {
        return 1;
    }
    else if (m_Minute < o.m_Minute)
    {
        return -1;
    }
    else if (m_Minute > o.m_Minute)
    {
        return 1;
    }
    else if (m_Second < o.m_Second)
    {
        return -1;
    }
    else if (m_Second > o.m_Second)
    {
        return 1;
    }
    else if (m_Millisecond < o.m_Millisecond)
    {
        return -1;
    }
    else if (m_Millisecond == o.m_Millisecond)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// ------------------------------------------------------------------------------------------------
CSStr Time::ToString() const
{
    return ToStrF("%02u:%02u:%02u:%u", m_Hour, m_Minute, m_Second, m_Millisecond);
}

// ------------------------------------------------------------------------------------------------
void Time::SetHour(Uint8 hour)
{
    // Is the specified hour within range?
    if (hour >= 24)
    {
        STHROWF("Hour value is out of range: %u >= 24", hour);
    }
    // Now it's safe to assign the value
    m_Hour = hour;
}

// ------------------------------------------------------------------------------------------------
void Time::SetMinute(Uint8 minute)
{
    // Is the specified minute within range?
    if (minute >= 60)
    {
        STHROWF("Minute value is out of range: %u >= 60", minute);
    }
    // Now it's safe to assign the value
    m_Minute = minute;
}

// ------------------------------------------------------------------------------------------------
void Time::SetSecond(Uint8 second)
{
    // Is the specified second within range?
    if (second >= 60)
    {
        STHROWF("Second value is out of range: %u >= 60", second);
    }
    // Now it's safe to assign the value
    m_Second = second;
}

// ------------------------------------------------------------------------------------------------
void Time::SetMillisecond(Uint16 millisecond)
{
    // Is the specified millisecond within range?
    if (millisecond >= 1000)
    {
        STHROWF("Millisecond value is out of range: %u >= 1000", millisecond);
    }
    // Now it's safe to assign the value
    m_Millisecond = millisecond;
}

// ------------------------------------------------------------------------------------------------
Time Time::AddHours(Int32 hours)
{
    // Replicate the current time
    Time tm(*this);
    // Did we even added any hours?
    if (!hours)
    {
        return tm; // Return the time as is
    }
    // Add the specified amount of hours
    tm.m_Hour = ((hours % 24) + m_Hour);
    // Make sure the value is within range
    tm.m_Hour %= 24;
    // Return the result
    return tm;
}

// ------------------------------------------------------------------------------------------------
Time Time::AddMinutes(Int32 minutes)
{
    // Replicate the current time
    Time tm(*this);
    // Did we even added any minutes?
    if (!minutes)
    {
        return tm; // Return the time as is
    }
    // Extract the number of hours
    Int32 hours = minutes / 60;
    // Extract the number of minutes
    minutes = (minutes % 60) + m_Minute;
    // Make sure the value is within range
    if (minutes >= 60)
    {
        // Increase hours
        ++hours;
        // Subtract from minutes
        minutes %= 60;
    }
    // Do we have any hours?
    if (hours > 0)
    {
        // Add the hours
        tm = tm.AddHours(hours);
    }
    // Assign the resulted minutes
    tm.m_Minute = minutes;
    // Return the result
    return tm;
}

// ------------------------------------------------------------------------------------------------
Time Time::AddSeconds(Int32 seconds)
{
    // Replicate the current time
    Time tm(*this);
    // Did we even added any seconds?
    if (!seconds)
    {
        return tm; // Return the time as is
    }
    // Extract the number of minutes
    Int32 minutes = seconds / 60;
    // Extract the number of seconds
    seconds = (seconds % 60) + m_Second;
    // Make sure the value is within range
    if (seconds >= 60)
    {
        // Increase minutes
        ++minutes;
        // Subtract from seconds
        seconds %= 60;
    }
    // Do we have any minutes?
    if (minutes > 0)
    {
        // Add the minutes
        tm = tm.AddMinutes(minutes);
    }
    // Assign the resulted seconds
    tm.m_Second = seconds;
    // Return the result
    return tm;
}

// ------------------------------------------------------------------------------------------------
Time Time::AddMilliseconds(Int32 milliseconds)
{
    // Replicate the current time
    Time tm(*this);
    // Did we even added any milliseconds?
    if (!milliseconds)
    {
        return tm; // Return the time as is
    }
    // Extract the number of seconds
    Int32 seconds = milliseconds / 1000;
    // Extract the number of milliseconds
    milliseconds = (milliseconds % 1000) + m_Millisecond;
    // Make sure the value is within range
    if (milliseconds >= 1000)
    {
        // Increase seconds
        ++seconds;
        // Subtract from milliseconds
        milliseconds %= 1000;
    }
    // Do we have any seconds?
    if (seconds > 0)
    {
        // Add the seconds
        tm = tm.AddSeconds(seconds);
    }
    // Assign the resulted milliseconds
    tm.m_Millisecond = milliseconds;
    // Return the result
    return tm;
}

// ================================================================================================
void Register_ChronoTime(HSQUIRRELVM vm, Table & cns)
{
    RootTable(vm).Bind(_SC("SqTime"), Class< Time >(vm, _SC("SqChronoTime"))
        // Constructors
        .Ctor()
        .Ctor< Uint8 >()
        .Ctor< Uint8, Uint8 >()
        .Ctor< Uint8, Uint8, Uint8 >()
        .Ctor< Uint8, Uint8, Uint8, Uint16 >()
        // Core Metamethods
        .Func(_SC("_tostring"), &Time::ToString)
        .SquirrelFunc(_SC("_typename"), &Time::Typename)
        .Func(_SC("_cmp"), &Time::Cmp)
        // Metamethods
        .Func< Time (Time::*)(const Time &) const >(_SC("_add"), &Time::operator +)
        .Func< Time (Time::*)(const Time &) const >(_SC("_sub"), &Time::operator -)
        .Func< Time (Time::*)(const Time &) const >(_SC("_mul"), &Time::operator *)
        .Func< Time (Time::*)(const Time &) const >(_SC("_div"), &Time::operator /)
        // Properties
        .Prop(_SC("Hour"), &Time::GetHour, &Time::SetHour)
        .Prop(_SC("Minute"), &Time::GetMinute, &Time::SetMinute)
        .Prop(_SC("Second"), &Time::GetSecond, &Time::SetSecond)
        .Prop(_SC("Millisecond"), &Time::GetMillisecond, &Time::SetMillisecond)
        .Prop(_SC("Str"), &Time::GetStr, &Time::SetStr)
        // Member Methods
        .Func(_SC("AddHours"), &Time::AddHours)
        .Func(_SC("AddMinutes"), &Time::AddMinutes)
        .Func(_SC("AddSeconds"), &Time::AddSeconds)
        .Func(_SC("AddMillis"), &Time::AddMilliseconds)
        .Func(_SC("AddMilliseconds"), &Time::AddMilliseconds)
        // Overloaded Methods
        .Overload< void (Time::*)(Uint8) >(_SC("Set"), &Time::Set)
        .Overload< void (Time::*)(Uint8, Uint8) >(_SC("Set"), &Time::Set)
        .Overload< void (Time::*)(Uint8, Uint8, Uint8) >(_SC("Set"), &Time::Set)
        .Overload< void (Time::*)(Uint8, Uint8, Uint8, Uint16) >(_SC("Set"), &Time::Set)
    );
}

} // Namespace:: SqMod
