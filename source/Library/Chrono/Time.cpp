// ------------------------------------------------------------------------------------------------
#include "Library/Chrono/Time.hpp"
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Datetime.hpp"
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQChar Time::Delimiter = ':';

// ------------------------------------------------------------------------------------------------
SQInteger Time::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqTime");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
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
CSStr Time::ToString() const
{
    return ToStrF("%02u%c%02u%c%02u%c%u",
                    m_Hour, m_Delimiter,
                    m_Minute, m_Delimiter,
                    m_Second, m_Delimiter,
                    m_Millisecond);
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
void Time::SetStr(CSStr str)
{
    // The format specifications that will be used to scan the string
    static SQChar fs[] = _SC(" %u : %u : %u : %u ");
    // Is the specified string empty?
    if (!str || *str == '\0')
    {
        // Clear the values
        m_Hour = 0;
        m_Minute = 0;
        m_Second = 0;
        m_Millisecond = 0;
        // We're done here
        return;
    }
    // Assign the specified delimiter
    fs[4] = m_Delimiter;
    fs[9] = m_Delimiter;
    fs[14] = m_Delimiter;
    // The sscanf function requires at least 32 bit integers
    Uint32 hour = 0, minute = 0, second = 0, milli = 0;
    // Attempt to extract the component values from the specified string
    sscanf(str, fs, &hour, &minute, &second, &milli);
    // Clamp the extracted values to the boundaries of associated type and assign them
    Set(ClampL< Uint32, Uint8 >(hour),
        ClampL< Uint32, Uint8 >(minute),
        ClampL< Uint32, Uint8 >(second),
        ClampL< Uint32, Uint16 >(milli)
    );
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
Time & Time::AddHours(Int32 hours)
{
    // Did we even add any hours?
    if (hours)
    {
        // Add the specified amount of hours
        m_Hour += (hours % 24);
        // Make sure the value is within range
        m_Hour %= 24;
    }
    // Allow chaining operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Time & Time::AddMinutes(Int32 minutes)
{
    // Did we even add any minutes?
    if (minutes)
    {
        // Extract the number of hours
        Int32 hours = static_cast< Int32 >(minutes / 60);
        // Extract the number of minutes
        m_Minute += (minutes % 60);
        // Are the minutes overlapping with the next hour?
        if (m_Minute >= 60)
        {
            // Increase the hours
            ++hours;
            // Subtract one hour from minutes
            m_Minute %= 60;
        }
        // Should we add any hours?
        if (hours)
        {
            AddHours(hours);
        }
    }
    // Allow chaining operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Time & Time::AddSeconds(Int32 seconds)
{
    // Did we even add any seconds?
    if (seconds)
    {
        // Extract the number of minutes
        Int32 minutes = static_cast< Int32 >(seconds / 60);
        // Extract the number of seconds
        m_Second += (seconds % 60);
        // Are the seconds overlapping with the next minute?
        if (m_Second >= 60)
        {
            // Increase the minutes
            ++minutes;
            // Subtract one minute from seconds
            m_Second %= 60;
        }
        // Should we add any minutes?
        if (minutes)
        {
            AddMinutes(minutes);
        }
    }
    // Allow chaining operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Time & Time::AddMilliseconds(Int32 milliseconds)
{
    // Did we even add any milliseconds?
    if (milliseconds)
    {
        // Extract the number of seconds
        Int32 seconds = static_cast< Int32 >(milliseconds / 1000);
        // Extract the number of milliseconds
        m_Millisecond += (milliseconds / 1000);
        // Are the milliseconds overlapping with the next second?
        if (m_Millisecond >= 1000)
        {
            // Increase the seconds
            ++seconds;
            // Subtract one second from milliseconds
            m_Millisecond %= 1000;
        }
        // Should we add any seconds?
        if (seconds)
        {
            AddSeconds(seconds);
        }
    }
    // Allow chaining operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Time Time::AndHours(Int32 hours)
{
    // Did we even add any hours?
    if (hours)
    {
        return Time((m_Hour + (hours % 24)) % 24, m_Minute, m_Second, m_Millisecond);
    }
    // Return the time as is
    return Time(*this);
}

// ------------------------------------------------------------------------------------------------
Time Time::AndMinutes(Int32 minutes)
{
    // Did we even added any minutes?
    if (!minutes)
    {
        return Time(*this); // Return the time as is
    }
    // Extract the number of hours
    Int32 hours =  static_cast< Int32 >(minutes / 60);
    // Extract the number of minutes
    minutes = m_Minute + (minutes % 60);
    // Are the minutes overlapping with the next hour?
    if (minutes >= 60)
    {
        ++hours; // Increase hours
    }
    // Replicate the current time
    Time t(*this);
    // Should we add any hours?
    if (hours)
    {
        t.AddHours(hours);
    }
    // Assign the resulted minutes
    t.m_Minute = (minutes % 60);
    // Return the result
    return t;
}

// ------------------------------------------------------------------------------------------------
Time Time::AndSeconds(Int32 seconds)
{
    // Did we even added any seconds?
    if (!seconds)
    {
        return Time(*this); // Return the time as is
    }
    // Extract the number of minutes
    Int32 minutes = static_cast< Int32 >(seconds / 60);
    // Extract the number of seconds
    seconds = m_Second + (seconds % 60);
    // Are the seconds overlapping with the next minute?
    if (seconds >= 60)
    {
        ++minutes; // Increase minutes
    }
    // Replicate the current time
    Time t(*this);
    // Should we add any minutes?
    if (minutes)
    {
        t.AddMinutes(minutes);
    }
    // Assign the resulted seconds
    t.m_Second = (seconds % 60);
    // Return the result
    return t;
}

// ------------------------------------------------------------------------------------------------
Time Time::AndMilliseconds(Int32 milliseconds)
{
    // Did we even added any milliseconds?
    if (!milliseconds)
    {
        return Time(*this); // Return the time as is
    }
    // Extract the number of seconds
    Int32 seconds = static_cast< Int32 >(milliseconds / 1000);
    // Extract the number of milliseconds
    milliseconds = m_Millisecond + (milliseconds % 1000);
    // Are the milliseconds overlapping with the next second?
    if (milliseconds >= 1000)
    {
        ++seconds; // Increase seconds
    }
    // Replicate the current time
    Time t(*this);
    // Should we add any seconds?
    if (seconds)
    {
        t.AddSeconds(seconds);
    }
    // Assign the resulted milliseconds
    t.m_Millisecond = (milliseconds % 1000);
    // Return the result
    return t;
}

// ================================================================================================
void Register_ChronoTime(HSQUIRRELVM vm, Table & /*cns*/)
{
    RootTable(vm).Bind(_SC("SqTime"), Class< Time >(vm, _SC("SqTime"))
        // Constructors
        .Ctor()
        .Ctor< Uint8 >()
        .Ctor< Uint8, Uint8 >()
        .Ctor< Uint8, Uint8, Uint8 >()
        .Ctor< Uint8, Uint8, Uint8, Uint16 >()
        // Static Properties
        .SetStaticValue(_SC("GlobalDelimiter"), &Time::Delimiter)
        // Core Meta-methods
        .Func(_SC("_tostring"), &Time::ToString)
        .SquirrelFunc(_SC("_typename"), &Time::Typename)
        .Func(_SC("_cmp"), &Time::Cmp)
        // Meta-methods
        .Func< Time (Time::*)(const Time &) const >(_SC("_add"), &Time::operator +)
        .Func< Time (Time::*)(const Time &) const >(_SC("_sub"), &Time::operator -)
        .Func< Time (Time::*)(const Time &) const >(_SC("_mul"), &Time::operator *)
        .Func< Time (Time::*)(const Time &) const >(_SC("_div"), &Time::operator /)
        // Properties
        .Prop(_SC("Delimiter"), &Time::GetDelimiter, &Time::SetDelimiter)
        .Prop(_SC("Str"), &Time::GetStr, &Time::SetStr)
        .Prop(_SC("Hour"), &Time::GetHour, &Time::SetHour)
        .Prop(_SC("Minute"), &Time::GetMinute, &Time::SetMinute)
        .Prop(_SC("Second"), &Time::GetSecond, &Time::SetSecond)
        .Prop(_SC("Millisecond"), &Time::GetMillisecond, &Time::SetMillisecond)
        // Member Methods
        .Func(_SC("AddHours"), &Time::AddHours)
        .Func(_SC("AddMinutes"), &Time::AddMinutes)
        .Func(_SC("AddSeconds"), &Time::AddSeconds)
        .Func(_SC("AddMillis"), &Time::AddMilliseconds)
        .Func(_SC("AddMilliseconds"), &Time::AddMilliseconds)
        .Func(_SC("AndHours"), &Time::AndHours)
        .Func(_SC("AndMinutes"), &Time::AndMinutes)
        .Func(_SC("AndSeconds"), &Time::AndSeconds)
        .Func(_SC("AndMillis"), &Time::AndMilliseconds)
        .Func(_SC("AndMilliseconds"), &Time::AndMilliseconds)
        // Overloaded Methods
        .Overload< void (Time::*)(Uint8) >(_SC("Set"), &Time::Set)
        .Overload< void (Time::*)(Uint8, Uint8) >(_SC("Set"), &Time::Set)
        .Overload< void (Time::*)(Uint8, Uint8, Uint8) >(_SC("Set"), &Time::Set)
        .Overload< void (Time::*)(Uint8, Uint8, Uint8, Uint16) >(_SC("Set"), &Time::Set)
    );
}

} // Namespace:: SqMod
