// ------------------------------------------------------------------------------------------------
#include "Library/Chrono/Datetime.hpp"
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Time.hpp"
#include "Library/Chrono/Timestamp.hpp"
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("SqDatetime"))

// ------------------------------------------------------------------------------------------------
SQChar Datetime::Delimiter = ' ';
SQChar Datetime::DateDelim = '-';
SQChar Datetime::TimeDelim = ':';

// ------------------------------------------------------------------------------------------------
Datetime::Datetime(int64_t ts)
    : Datetime()
{
    const auto y = static_cast< uint16_t >(std::lround(std::floor(ts / 3.17098e-14)));
    ts -= int64_t{y} * 3.17098e-14;
    const auto mo = static_cast< uint8_t >(std::lround(std::floor(ts / 2.628e+12)));
    ts -= int64_t{mo} * 2.628e+12;
    const auto d = static_cast< uint8_t >(std::lround(std::floor(ts / 8.64e+10)));
    ts -= int64_t{d} * 8.64e+10;
    const auto h = static_cast< uint8_t >(std::lround(std::floor(ts / 3.6e+9)));
    ts -= int64_t{h} * 3.6e+9;
    const auto mi = static_cast< uint8_t >(std::lround(std::floor(ts / 6e+7)));
    ts -= int64_t{mi} * 6e+7;
    const auto s = static_cast< uint8_t >(std::lround(std::floor(ts / 1e+6)));
    ts -= int64_t{s} * 1e+6;
    // Set the specified date-time
    Set(y, mo, d, h, mi, s, static_cast< uint16_t >(ts / 1000LL));
}

// ------------------------------------------------------------------------------------------------
int32_t Datetime::Compare(const Datetime & o) const
{
    if (m_Year < o.m_Year)
    { // NOLINT(bugprone-branch-clone)
        return -1;
    }
    else if (m_Year > o.m_Year)
    { // NOLINT(bugprone-branch-clone)
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
String Datetime::ToString() const
{
    return fmt::format("{:04}{}{:02}{}{:02}{}{:02}{}{:02}{}{:02}{}{}"
        , m_Year, m_DateDelim, m_Month, m_DateDelim, m_Day
        , m_Delimiter
        , m_Hour, m_TimeDelim, m_Minute, m_TimeDelim, m_Second , m_TimeDelim, m_Millisecond
    );
}

// ------------------------------------------------------------------------------------------------
void Datetime::Set(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond)
{
    // Validate the specified date
    if (!Chrono::ValidDate(year, month, day))
    {
        STHROWF("Invalid date: {:04}{:c}{:02}{:c}{:02}{:c}{}"
            , m_DateDelim, m_Year
            , m_DateDelim, m_Month
            , m_DateDelim, m_Day
        );
    }
    // Is the specified hour within range?
    else if (hour >= 24)
    {
        STHROWF("Hour value is out of range: {} >= 24", hour);
    }
    // Is the specified minute within range?
    else if (minute >= 60)
    {
        STHROWF("Minute value is out of range: {} >= 60", minute);
    }
    // Is the specified second within range?
    else if (second >= 60)
    {
        STHROWF("Second value is out of range: {} >= 60", second);
    }
    // Is the specified millisecond within range?
    else if (millisecond >= 1000)
    {
        STHROWF("Millisecond value is out of range: {} >= 1000", millisecond);
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
void Datetime::SetStr(const SQChar * str)
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
    uint32_t year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, milli = 0;
    // Attempt to extract the component values from the specified string
    sscanf(str, fs, &year, &month, &day, &hour, &minute, &second, &milli);
    // Clamp the extracted values to the boundaries of associated type and assign them
    Set(ClampL< uint32_t, uint8_t >(year),
        ClampL< uint32_t, uint8_t >(month),
        ClampL< uint32_t, uint8_t >(day),
        ClampL< uint32_t, uint8_t >(hour),
        ClampL< uint32_t, uint8_t >(minute),
        ClampL< uint32_t, uint8_t >(second),
        ClampL< uint32_t, uint16_t >(milli)
    );
}

// ------------------------------------------------------------------------------------------------
void Datetime::SetDayOfYear(uint16_t doy)
{
    // Reverse the given day of year to a full date
    Date d = Chrono::ReverseDayOfYear(m_Year, doy);
    // Set the obtained month
    SetMonth(d.GetMonth());
    // Set the obtained day
    SetDay(d.GetDay());
}

// ------------------------------------------------------------------------------------------------
void Datetime::SetYear(uint16_t year)
{
    // Make sure the year is valid
    if (!year)
    {
        STHROWF("Invalid year: {}", year);
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
void Datetime::SetMonth(uint8_t month)
{
    // Make sure the month is valid
    if (month == 0 || month > 12)
    {
        STHROWF("Invalid month: {}", month);
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
void Datetime::SetDay(uint8_t day)
{
    // Grab the amount of days in the current month
    const uint8_t dim = Chrono::DaysInMonth(m_Year, m_Month);
    // Make sure the day is valid
    if (day == 0)
    {
        STHROWF("Invalid day: {}", day);
    }
    else if (day > dim)
    {
        STHROWF("Day is out of range: {} > {}", day, dim);
    }
    // Assign the value
    m_Day = day;
}

// ------------------------------------------------------------------------------------------------
void Datetime::SetHour(uint8_t hour)
{
    // Is the specified hour within range?
    if (hour >= 24)
    {
        STHROWF("Hour value is out of range: {} >= 24", hour);
    }
    // Now it's safe to assign the value
    m_Hour = hour;
}

// ------------------------------------------------------------------------------------------------
void Datetime::SetMinute(uint8_t minute)
{
    // Is the specified minute within range?
    if (minute >= 60)
    {
        STHROWF("Minute value is out of range: {} >= 60", minute);
    }
    // Now it's safe to assign the value
    m_Minute = minute;
}

// ------------------------------------------------------------------------------------------------
void Datetime::SetSecond(uint8_t second)
{
    // Is the specified second within range?
    if (second >= 60)
    {
        STHROWF("Second value is out of range: {} >= 60", second);
    }
    // Now it's safe to assign the value
    m_Second = second;
}

// ------------------------------------------------------------------------------------------------
void Datetime::SetMillisecond(uint16_t millisecond)
{
    // Is the specified millisecond within range?
    if (millisecond >= 1000)
    {
        STHROWF("Millisecond value is out of range: {} >= 1000", millisecond);
    }
    // Now it's safe to assign the value
    m_Millisecond = millisecond;
}

// ------------------------------------------------------------------------------------------------
Datetime & Datetime::AddYears(int32_t years)
{
    // Do we have a valid amount of years?
    if (years)
    {
        // Add the specified amount of years
        SetYear(ConvTo< uint16_t >::From(static_cast< int32_t >(m_Year) + years));
    }
    // Allow chaining operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Datetime & Datetime::AddMonths(int32_t months)
{
    // Do we have a valid amount of months?
    if (months)
    {
        // Extract the number of years
        auto years = static_cast< int32_t >(months / 12);
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
            SetYear(ConvTo< uint16_t >::From(static_cast< int32_t >(m_Year) + years));
        }
        // Add the months
        SetMonth(static_cast< uint8_t >(months));
    }
    // Allow chaining operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Datetime & Datetime::AddDays(int32_t days)
{
    // Do we have a valid amount of days?
    if (days)
    {
        // Whether the number of days is positive or negative
        const int32_t dir = days > 0 ? 1 : -1;
        // Grab current year
        int32_t year = m_Year;
        // Calculate the days in the current year
        int32_t diy = Chrono::DaysInYear(static_cast< uint16_t >(year));
        // Calculate the day of year
        int32_t doy = GetDayOfYear() + days;
        // Calculate the resulting years
        while (doy > diy || doy < 0)
        {
            doy -= diy * dir;
            year += dir;
            diy = Chrono::DaysInYear(static_cast< uint16_t >(year));
        }
        // Set the obtained year
        SetYear(static_cast< uint16_t >(year));
        // Set the obtained day of year
        SetDayOfYear(static_cast< uint16_t >(doy));
    }
    // Allow chaining operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Datetime & Datetime::AddHours(int32_t hours)
{
    // Did we even add any hours?
    if (hours)
    {
        // Extract the number of days
        auto days = static_cast< int32_t >(hours / 24);
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
Datetime & Datetime::AddMinutes(int32_t minutes)
{
    // Did we even add any minutes?
    if (minutes)
    {
        // Extract the number of hours
        auto hours = static_cast< int32_t >(minutes / 60);
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
Datetime & Datetime::AddSeconds(int32_t seconds)
{
    // Did we even add any seconds?
    if (seconds)
    {
        // Extract the number of minutes
        auto minutes = static_cast< int32_t >(seconds / 60);
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
Datetime & Datetime::AddMilliseconds(int32_t milliseconds)
{
    // Did we even add any milliseconds?
    if (milliseconds)
    {
        // Extract the number of seconds
        auto seconds = static_cast< int32_t >(milliseconds / 1000);
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
Datetime Datetime::AndYears(int32_t years)
{
    // Do we have a valid amount of years?
    if (!years)
    {
        return Datetime(*this); // Return the date-time as is
    }
    // Replicate the current date
    Datetime dt(*this);
    // Add the specified amount of years
    dt.SetYear(ConvTo< uint16_t >::From(static_cast< int32_t >(m_Year) + years));
    // Return the resulted date
    return dt;
}

// ------------------------------------------------------------------------------------------------
Datetime Datetime::AndMonths(int32_t months)
{
    // Do we have a valid amount of months?
    if (!months)
    {
        return Datetime(*this); // Return the date-time as is
    }
    // Extract the number of years
    auto years = static_cast< int32_t >(months / 12);
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
        dt.SetYear(ConvTo< uint16_t >::From(static_cast< int32_t >(m_Year) + years));
    }
    // Add the months
    dt.SetMonth(static_cast< uint8_t >(months));
    // Return the resulted date
    return dt;
}

// ------------------------------------------------------------------------------------------------
Datetime Datetime::AndDays(int32_t days)
{
    // Do we have a valid amount of days?
    if (!days)
    {
        return Datetime(*this); // Return the date-time as is
    }
    // Whether the number of days is positive or negative
    const int32_t dir = days > 0 ? 1 : -1;
    // Grab current year
    int32_t year = m_Year;
    // Calculate the days in the current year
    int32_t diy = Chrono::DaysInYear(static_cast< uint16_t >(year));
    // Calculate the day of year
    int32_t doy = GetDayOfYear() + days;
    // Calculate the resulting years
    while (doy > diy || doy < 0)
    {
        doy -= diy * dir;
        year += dir;
        diy = Chrono::DaysInYear(static_cast< uint16_t >(year));
    }
    // Replicate the current date
    Datetime dt(*this);
    // Set the obtained year
    dt.SetYear(static_cast< uint16_t >(year));
    // Set the obtained day of year
    dt.SetDayOfYear(static_cast< uint16_t >(doy));
    // Return the resulted date
    return dt;
}

// ------------------------------------------------------------------------------------------------
Datetime Datetime::AndHours(int32_t hours)
{
    // Did we even add any hours?
    if (!hours)
    {
        return Datetime(*this); // Return the date-time as is
    }
    // Extract the number of days
    auto days = static_cast< int32_t >(hours / 24);
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
    dt.m_Hour = static_cast< uint8_t >(hours % 24);
    // Return the result
    return dt;
}

// ------------------------------------------------------------------------------------------------
Datetime Datetime::AndMinutes(int32_t minutes)
{
    // Did we even added any minutes?
    if (!minutes)
    {
        return Datetime(*this); // Return the date-time as is
    }
    // Extract the number of hours
    auto hours =  static_cast< int32_t >(minutes / 60);
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
    dt.m_Minute = static_cast< uint8_t >(minutes % 60);
    // Return the result
    return dt;
}

// ------------------------------------------------------------------------------------------------
Datetime Datetime::AndSeconds(int32_t seconds)
{
    // Did we even added any seconds?
    if (!seconds)
    {
        return Datetime(*this); // Return the date-time as is
    }
    // Extract the number of minutes
    auto minutes = static_cast< int32_t >(seconds / 60);
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
    dt.m_Second = static_cast< uint8_t >(seconds % 60);
    // Return the result
    return dt;
}

// ------------------------------------------------------------------------------------------------
Datetime Datetime::AndMilliseconds(int32_t milliseconds)
{
    // Did we even added any milliseconds?
    if (!milliseconds)
    {
        return Datetime(*this); // Return the date-time as is
    }
    // Extract the number of seconds
    auto seconds = static_cast< int32_t >(milliseconds / 1000);
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
    dt.m_Millisecond = static_cast< uint16_t >(milliseconds % 1000);
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
    int32_t days = Chrono::DayOfYear(m_Year, m_Month, m_Day);
    // Calculate all days till the current year
    for (int32_t year = 0; year < m_Year; --year)
    {
        days += Chrono::DaysInYear(static_cast< uint16_t >(year));
    }
    // Calculate the microseconds in the resulted days
    auto ms = static_cast< int64_t >(days * 86400000000LL);
    // Calculate the microseconds in the current time
    ms += static_cast< int64_t >(m_Hour * 3600000000LL);
    ms += static_cast< int64_t >(m_Minute * 60000000L);
    ms += static_cast< int64_t >(m_Second * 1000000L);
    ms += static_cast< int64_t >(m_Millisecond * 1000L);
    // Return the resulted timestamp
    return Timestamp(ms);
}

// ------------------------------------------------------------------------------------------------
std::time_t Datetime::ToTimeT() const
{
    return GetTimestamp().ToTimeT();
}

// ================================================================================================
void Register_ChronoDatetime(HSQUIRRELVM vm, Table & /*cns*/)
{
    RootTable(vm).Bind(Typename::Str,
        Class< Datetime >(vm, Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< uint16_t >()
        .Ctor< uint16_t, uint8_t >()
        .Ctor< uint16_t, uint8_t, uint8_t >()
        .Ctor< uint16_t, uint8_t, uint8_t, uint8_t >()
        .Ctor< uint16_t, uint8_t, uint8_t, uint8_t, uint8_t >()
        .Ctor< uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t >()
        .Ctor< uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint16_t >()
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
        .Overload< void (Datetime::*)(uint16_t) >(_SC("Set"), &Datetime::Set)
        .Overload< void (Datetime::*)(uint16_t, uint8_t) >(_SC("Set"), &Datetime::Set)
        .Overload< void (Datetime::*)(uint16_t, uint8_t, uint8_t) >(_SC("Set"), &Datetime::Set)
        .Overload< void (Datetime::*)(uint16_t, uint8_t, uint8_t, uint8_t) >(_SC("Set"), &Datetime::Set)
        .Overload< void (Datetime::*)(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t) >(_SC("Set"), &Datetime::Set)
        .Overload< void (Datetime::*)(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t) >(_SC("Set"), &Datetime::Set)
        .Overload< void (Datetime::*)(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint16_t) >(_SC("Set"), &Datetime::Set)
    );
}

} // Namespace:: SqMod
