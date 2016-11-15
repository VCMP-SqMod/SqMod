// ------------------------------------------------------------------------------------------------
#include "Library/Chrono/Datetime.hpp"
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Time.hpp"
#include "Library/Chrono/Timestamp.hpp"
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("SqDatetime"))

// ------------------------------------------------------------------------------------------------
SQChar Datetime::Delimiter = ' ';
SQChar Datetime::DateDelim = '-';
SQChar Datetime::TimeDelim = ':';

// ------------------------------------------------------------------------------------------------
Int32 Datetime::Compare(const Datetime & o) const
{
    if (m_Year < o.m_Year)
    {
        return -1;
    }
    else if (m_Year > o.m_Year)
    {
        return 1;
    }
    else if (m_Month < o.m_Month)
    {
        return -1;
    }
    else if (m_Month > o.m_Month)
    {
        return 1;
    }
    else if (m_Day < o.m_Day)
    {
        return -1;
    }
    else if (m_Day > o.m_Day)
    {
        return 1;
    }
    else if (m_Hour < o.m_Hour)
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
Datetime Datetime::operator + (const Datetime & o) const
{
    // Add the components individually
    return Datetime(o);
}

// ------------------------------------------------------------------------------------------------
Datetime Datetime::operator - (const Datetime & o) const
{
    return Datetime(o);
}

// ------------------------------------------------------------------------------------------------
Datetime Datetime::operator * (const Datetime & o) const
{
    return Datetime(o);
}

// ------------------------------------------------------------------------------------------------
Datetime Datetime::operator / (const Datetime & o) const
{
    return Datetime(o);
}

// ------------------------------------------------------------------------------------------------
CSStr Datetime::ToString() const
{
    return ToStrF("%04u%c%02u%c%02u%c%02u%c%02u%c%02u%c%u"
        , m_Year, m_DateDelim, m_Month, m_DateDelim, m_Day
        , m_Delimiter
        , m_Hour, m_TimeDelim, m_Minute, m_TimeDelim, m_Second , m_TimeDelim, m_Millisecond
    );
}

// ------------------------------------------------------------------------------------------------
void Datetime::Set(Uint16 year, Uint8 month, Uint8 day, Uint8 hour, Uint8 minute, Uint8 second, Uint16 millisecond)
{
    // Validate the specified date
    if (!Chrono::ValidDate(year, month, day))
    {
        STHROWF("Invalid date: %04u%c%02u%c%02u%c%u"
            , m_Delimiter, m_Year
            , m_Delimiter, m_Month
            , m_Delimiter, m_Day
        );
    }
    // Is the specified hour within range?
    else if (hour >= 24)
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
    // Assign the specified values
    m_Year = year;
    m_Month = month;
    m_Day = day;
    m_Hour = hour;
    m_Minute = minute;
    m_Second = second;
    m_Millisecond = millisecond;
}

// ------------------------------------------------------------------------------------------------
void Datetime::SetStr(CSStr str)
{
    // The format specifications that will be used to scan the string
    static SQChar fs[] = _SC(" %u - %u - %u   %u : %u : %u : %u ");
    // Is the specified string empty?
    if (!str || *str == '\0')
    {
        // Clear the values
        m_Year = 0;
        m_Month = 0;
        m_Day = 0;
        m_Hour = 0;
        m_Minute = 0;
        m_Second = 0;
        m_Millisecond = 0;
        // We're done here
        return;
    }
    // Assign the specified delimiter
    fs[4] = m_DateDelim;
    fs[9] = m_DateDelim;
    fs[14] = m_Delimiter;
    fs[19] = m_TimeDelim;
    fs[24] = m_TimeDelim;
    fs[29] = m_TimeDelim;
    // The sscanf function requires at least 32 bit integers
    Uint32 year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, milli = 0;
    // Attempt to extract the component values from the specified string
    sscanf(str, fs, &year, &month, &day, &hour, &minute, &second, &milli);
    // Clamp the extracted values to the boundaries of associated type and assign them
    Set(ClampL< Uint32, Uint8 >(year),
        ClampL< Uint32, Uint8 >(month),
        ClampL< Uint32, Uint8 >(day),
        ClampL< Uint32, Uint8 >(hour),
        ClampL< Uint32, Uint8 >(minute),
        ClampL< Uint32, Uint8 >(second),
        ClampL< Uint32, Uint16 >(milli)
    );
}

// ------------------------------------------------------------------------------------------------
void Datetime::SetDayOfYear(Uint16 doy)
{
    // Reverse the given day of year to a full date
    Date d = Chrono::ReverseDayOfyear(m_Year, doy);
    // Set the obtained month
    SetMonth(d.GetMonth());
    // Set the obtained day
    SetDay(d.GetDay());
}

// ------------------------------------------------------------------------------------------------
void Datetime::SetYear(Uint16 year)
{
    // Make sure the year is valid
    if (!year)
    {
        STHROWF("Invalid year: %u", year);
    }
    // Assign the value
    m_Year = year;
    // Make sure the new date is valid
    if (!Chrono::ValidDate(m_Year, m_Month, m_Day))
    {
        m_Month = 1;
        m_Day = 1;
    }
}

// ------------------------------------------------------------------------------------------------
void Datetime::SetMonth(Uint8 month)
{
    // Make sure the month is valid
    if (month == 0 || month > 12)
    {
        STHROWF("Invalid month: %u", month);
    }
    // Assign the value
    m_Month = month;
    // Make sure the month days are in range
    if (m_Day > Chrono::DaysInMonth(m_Year, m_Month))
    {
        m_Month = 1; // Fall back to the beginning of the month
    }
}

// ------------------------------------------------------------------------------------------------
void Datetime::SetDay(Uint8 day)
{
    // Grab the amount of days in the current month
    const Uint8 dim = Chrono::DaysInMonth(m_Year, m_Month);
    // Make sure the day is valid
    if (day == 0)
    {
        STHROWF("Invalid day: %u", day);
    }
    else if (day > dim)
    {
        STHROWF("Day is out of range: %u > %u", day, dim);
    }
    // Assign the value
    m_Day = day;
}

// ------------------------------------------------------------------------------------------------
void Datetime::SetHour(Uint8 hour)
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
void Datetime::SetMinute(Uint8 minute)
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
void Datetime::SetSecond(Uint8 second)
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
void Datetime::SetMillisecond(Uint16 millisecond)
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
Datetime & Datetime::AddYears(Int32 years)
{
    // Do we have a valid amount of years?
    if (years)
    {
        // Add the specified amount of years
        SetYear(ConvTo< Uint16 >::From(static_cast< Int32 >(m_Year) + years));
    }
    // Allow chaining operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Datetime & Datetime::AddMonths(Int32 months)
{
    // Do we have a valid amount of months?
    if (months)
    {
        // Extract the number of years
        Int32 years = static_cast< Int32 >(months / 12);
        // Extract the number of months
        months = (months % 12) + m_Month;
        // Do we have extra months?
        if (months >= 12)
        {
            // Increase the years
            ++years;
            // Subtract one year from months
            months %= 12;
        }
        else if (months < 0)
        {
            // Decrease the years
            --years;
            // Add one year to months
            months = 12 - months;
        }
        // Are there any years to add?
        if (years)
        {
            SetYear(ConvTo< Uint16 >::From(static_cast< Int32 >(m_Year) + years));
        }
        // Add the months
        SetMonth(months);
    }
    // Allow chaining operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Datetime & Datetime::AddDays(Int32 days)
{
    // Do we have a valid amount of days?
    if (days)
    {
        // Whether the number of days is positive or negative
        const Int32 dir = days > 0 ? 1 : -1;
        // Grab current year
        Int32 year = m_Year;
        // Calculate the days in the current year
        Int32 diy = Chrono::DaysInYear(year);
        // Calculate the day of year
        Int32 doy = GetDayOfYear() + days;
        // Calculate the resulting years
        while (doy > diy || doy < 0)
        {
            doy -= diy * dir;
            year += dir;
            diy = Chrono::DaysInYear(year);
        }
        // Set the obtained year
        SetYear(year);
        // Set the obtained day of year
        SetDayOfYear(doy);
    }
    // Allow chaining operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Datetime & Datetime::AddHours(Int32 hours)
{
    // Did we even add any hours?
    if (hours)
    {
        // Extract the number of days
        Int32 days = static_cast< Int32 >(hours / 24);
        // Extract the number of hours
        m_Hour += (hours % 24);
        // Are the hours overlapping with the next day?
        if (m_Hour >= 24)
        {
            // Increase the days
            ++days;
            // Subtract one day from hours
            m_Hour %= 24;
        }
        // Should we add any days?
        if (days)
        {
            AddDays(days);
        }
    }
    // Allow chaining operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Datetime & Datetime::AddMinutes(Int32 minutes)
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
Datetime & Datetime::AddSeconds(Int32 seconds)
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
Datetime & Datetime::AddMilliseconds(Int32 milliseconds)
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
Datetime Datetime::AndYears(Int32 years)
{
    // Do we have a valid amount of years?
    if (!years)
    {
        return Datetime(*this); // Return the date-time as is
    }
    // Replicate the current date
    Datetime dt(*this);
    // Add the specified amount of years
    dt.SetYear(ConvTo< Uint16 >::From(static_cast< Int32 >(m_Year) + years));
    // Return the resulted date
    return dt;
}

// ------------------------------------------------------------------------------------------------
Datetime Datetime::AndMonths(Int32 months)
{
    // Do we have a valid amount of months?
    if (!months)
    {
        return Datetime(*this); // Return the date-time as is
    }
    // Extract the number of years
    Int32 years = static_cast< Int32 >(months / 12);
    // Extract the number of months
    months = (months % 12) + m_Month;
    // Do we have extra months?
    if (months >= 12)
    {
        // Increase the years
        ++years;
        // Subtract one year from months
        months %= 12;
    }
    else if (months < 0)
    {
        // Decrease the years
        --years;
        // Add one year to months
        months = 12 - months;
    }
    // Replicate the current date
    Datetime dt(*this);
    // Are there any years to add?
    if (years)
    {
        dt.SetYear(ConvTo< Uint16 >::From(static_cast< Int32 >(m_Year) + years));
    }
    // Add the months
    dt.SetMonth(months);
    // Return the resulted date
    return dt;
}

// ------------------------------------------------------------------------------------------------
Datetime Datetime::AndDays(Int32 days)
{
    // Do we have a valid amount of days?
    if (!days)
    {
        return Datetime(*this); // Return the date-time as is
    }
    // Whether the number of days is positive or negative
    const Int32 dir = days > 0 ? 1 : -1;
    // Grab current year
    Int32 year = m_Year;
    // Calculate the days in the current year
    Int32 diy = Chrono::DaysInYear(year);
    // Calculate the day of year
    Int32 doy = GetDayOfYear() + days;
    // Calculate the resulting years
    while (doy > diy || doy < 0)
    {
        doy -= diy * dir;
        year += dir;
        diy = Chrono::DaysInYear(year);
    }
    // Replicate the current date
    Datetime dt(*this);
    // Set the obtained year
    dt.SetYear(year);
    // Set the obtained day of year
    dt.SetDayOfYear(doy);
    // Return the resulted date
    return dt;
}

// ------------------------------------------------------------------------------------------------
Datetime Datetime::AndHours(Int32 hours)
{
    // Did we even add any hours?
    if (!hours)
    {
        return Datetime(*this); // Return the date-time as is
    }
    // Extract the number of days
    Int32 days = static_cast< Int32 >(hours / 24);
    // Extract the number of hours
    hours = m_Hour + (hours % 24);
    // Are the hours overlapping with the next day?
    if (hours >= 24)
    {
        ++days; // Increase the days
    }
    // Replicate the current time
    Datetime dt(*this);
    // Should we add any days?
    if (days)
    {
        dt.AddDays(days);
    }
    // Assign the resulted hours
    dt.m_Hour = (hours % 24);
    // Return the result
    return dt;
}

// ------------------------------------------------------------------------------------------------
Datetime Datetime::AndMinutes(Int32 minutes)
{
    // Did we even added any minutes?
    if (!minutes)
    {
        return Datetime(*this); // Return the date-time as is
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
    Datetime dt(*this);
    // Should we add any hours?
    if (hours)
    {
        dt.AddHours(hours);
    }
    // Assign the resulted minutes
    dt.m_Minute = (minutes % 60);
    // Return the result
    return dt;
}

// ------------------------------------------------------------------------------------------------
Datetime Datetime::AndSeconds(Int32 seconds)
{
    // Did we even added any seconds?
    if (!seconds)
    {
        return Datetime(*this); // Return the date-time as is
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
    Datetime dt(*this);
    // Should we add any minutes?
    if (minutes)
    {
        dt.AddMinutes(minutes);
    }
    // Assign the resulted seconds
    dt.m_Second = (seconds % 60);
    // Return the result
    return dt;
}

// ------------------------------------------------------------------------------------------------
Datetime Datetime::AndMilliseconds(Int32 milliseconds)
{
    // Did we even added any milliseconds?
    if (!milliseconds)
    {
        return Datetime(*this); // Return the date-time as is
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
    Datetime dt(*this);
    // Should we add any seconds?
    if (seconds)
    {
        dt.AddSeconds(seconds);
    }
    // Assign the resulted milliseconds
    dt.m_Millisecond = (milliseconds % 1000);
    // Return the result
    return dt;
}

// ------------------------------------------------------------------------------------------------
Date Datetime::GetDate() const
{
    return Date(m_Year, m_Month, m_Day);
}

// ------------------------------------------------------------------------------------------------
Time Datetime::GetTime() const
{
    return Time(m_Hour, m_Minute, m_Second, m_Millisecond);
}

// ------------------------------------------------------------------------------------------------
Timestamp Datetime::GetTimestamp() const
{
    // Calculate the current day of the year
    Int32 days = Chrono::DayOfYear(m_Year, m_Month, m_Day);
    // Calculate all days till the current year
    for (Int32 year = 0; year < m_Year; --year)
    {
        days += Chrono::DaysInYear(year);
    }
    // Calculate the microseconds in the resulted days
    Int64 ms = static_cast< Int64 >(days * 86400000000LL);
    // Calculate the microseconds in the current time
    ms += static_cast< Int64 >(m_Hour * 3600000000LL);
    ms += static_cast< Int64 >(m_Minute * 60000000L);
    ms += static_cast< Int64 >(m_Second * 1000000L);
    ms += static_cast< Int64 >(m_Millisecond * 1000L);
    // Return the resulted timestamp
    return Timestamp(ms);
}

// ================================================================================================
void Register_ChronoDatetime(HSQUIRRELVM vm, Table & /*cns*/)
{
    RootTable(vm).Bind(Typename::Str, Class< Datetime >(vm, Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< Uint16 >()
        .Ctor< Uint16, Uint8 >()
        .Ctor< Uint16, Uint8, Uint8 >()
        .Ctor< Uint16, Uint8, Uint8, Uint8 >()
        .Ctor< Uint16, Uint8, Uint8, Uint8, Uint8 >()
        .Ctor< Uint16, Uint8, Uint8, Uint8, Uint8, Uint8 >()
        .Ctor< Uint16, Uint8, Uint8, Uint8, Uint8, Uint8, Uint16 >()
        // Static Properties
        .SetStaticValue(_SC("GlobalDelimiter"), &Datetime::Delimiter)
        .SetStaticValue(_SC("GlobalDateDelim"), &Datetime::DateDelim)
        .SetStaticValue(_SC("GlobalTimeDelim"), &Datetime::TimeDelim)
        // Core Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Datetime::ToString)
        .Func(_SC("cmp"), &Datetime::Cmp)
        // Meta-methods
        .Func< Datetime (Datetime::*)(const Datetime &) const >(_SC("_add"), &Datetime::operator +)
        .Func< Datetime (Datetime::*)(const Datetime &) const >(_SC("_sub"), &Datetime::operator -)
        .Func< Datetime (Datetime::*)(const Datetime &) const >(_SC("_mul"), &Datetime::operator *)
        .Func< Datetime (Datetime::*)(const Datetime &) const >(_SC("_div"), &Datetime::operator /)
        // Properties
        .Prop(_SC("Delimiter"), &Datetime::GetDelimiter, &Datetime::SetDelimiter)
        .Prop(_SC("DateDelim"), &Datetime::GetDateDelim, &Datetime::SetDateDelim)
        .Prop(_SC("TimeDelim"), &Datetime::GetTimeDelim, &Datetime::SetTimeDelim)
        .Prop(_SC("Str"), &Datetime::GetStr, &Datetime::SetStr)
        .Prop(_SC("DayOfYear"), &Datetime::GetDayOfYear, &Datetime::SetDayOfYear)
        .Prop(_SC("Year"), &Datetime::GetYear, &Datetime::SetYear)
        .Prop(_SC("Month"), &Datetime::GetMonth, &Datetime::SetMonth)
        .Prop(_SC("Day"), &Datetime::GetDay, &Datetime::SetDay)
        .Prop(_SC("Hour"), &Datetime::GetHour, &Datetime::SetHour)
        .Prop(_SC("Minute"), &Datetime::GetMinute, &Datetime::SetMinute)
        .Prop(_SC("Second"), &Datetime::GetSecond, &Datetime::SetSecond)
        .Prop(_SC("Millisecond"), &Datetime::GetMillisecond, &Datetime::SetMillisecond)
        .Prop(_SC("LeapYear"), &Datetime::IsThisLeapYear)
        .Prop(_SC("YearDays"), &Datetime::GetYearDays)
        .Prop(_SC("MonthDays"), &Datetime::GetMonthDays)
        .Prop(_SC("Date"), &Datetime::GetDate)
        .Prop(_SC("Time"), &Datetime::GetTime)
        .Prop(_SC("Timestamp"), &Datetime::GetTimestamp)
        // Member Methods
        .Func(_SC("AddYears"), &Datetime::AddYears)
        .Func(_SC("AddMonths"), &Datetime::AddMonths)
        .Func(_SC("AddDays"), &Datetime::AddDays)
        .Func(_SC("AddHours"), &Datetime::AddHours)
        .Func(_SC("AddMinutes"), &Datetime::AddMinutes)
        .Func(_SC("AddSeconds"), &Datetime::AddSeconds)
        .Func(_SC("AddMillis"), &Datetime::AddMilliseconds)
        .Func(_SC("AddMilliseconds"), &Datetime::AddMilliseconds)
        .Func(_SC("AndYears"), &Datetime::AndYears)
        .Func(_SC("AndMonths"), &Datetime::AndMonths)
        .Func(_SC("AndDays"), &Datetime::AndDays)
        .Func(_SC("AndHours"), &Datetime::AndHours)
        .Func(_SC("AndMinutes"), &Datetime::AndMinutes)
        .Func(_SC("AndSeconds"), &Datetime::AndSeconds)
        .Func(_SC("AndMillis"), &Datetime::AndMilliseconds)
        .Func(_SC("AndMilliseconds"), &Datetime::AndMilliseconds)
        // Overloaded Methods
        .Overload< void (Datetime::*)(Uint16) >(_SC("Set"), &Datetime::Set)
        .Overload< void (Datetime::*)(Uint16, Uint8) >(_SC("Set"), &Datetime::Set)
        .Overload< void (Datetime::*)(Uint16, Uint8, Uint8) >(_SC("Set"), &Datetime::Set)
        .Overload< void (Datetime::*)(Uint16, Uint8, Uint8, Uint8) >(_SC("Set"), &Datetime::Set)
        .Overload< void (Datetime::*)(Uint16, Uint8, Uint8, Uint8, Uint8) >(_SC("Set"), &Datetime::Set)
        .Overload< void (Datetime::*)(Uint16, Uint8, Uint8, Uint8, Uint8, Uint8) >(_SC("Set"), &Datetime::Set)
        .Overload< void (Datetime::*)(Uint16, Uint8, Uint8, Uint8, Uint8, Uint8, Uint16) >(_SC("Set"), &Datetime::Set)
    );
}

} // Namespace:: SqMod
