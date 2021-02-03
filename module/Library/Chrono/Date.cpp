// ------------------------------------------------------------------------------------------------
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Timestamp.hpp"
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("SqDate"))

// ------------------------------------------------------------------------------------------------
SQChar Date::Delimiter = '-';

// ------------------------------------------------------------------------------------------------
int32_t Date::Compare(const Date & o) const
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
    // They're equal
    return 0;
}

// ------------------------------------------------------------------------------------------------
Date Date::operator + (const Date & o) const
{
    // Add the components individually
    return Date(o);
}

// ------------------------------------------------------------------------------------------------
Date Date::operator - (const Date & o) const
{
    return Date(o);
}

// ------------------------------------------------------------------------------------------------
Date Date::operator * (const Date & o) const
{
    return Date(o);
}

// ------------------------------------------------------------------------------------------------
Date Date::operator / (const Date & o) const
{
    return Date(o);
}

// ------------------------------------------------------------------------------------------------
String Date::ToString() const
{
    return fmt::format("{:04}{}{:02}{}{:02}", m_Year, m_Delimiter, m_Month, m_Delimiter, m_Day);
}

// ------------------------------------------------------------------------------------------------
void Date::Set(uint16_t year, uint8_t month, uint8_t day)
{
    if (!Chrono::ValidDate(year, month, day))
    {
        STHROWF("Invalid date: {:04}{:c}{:02}{:c}{:02}" , m_Year, m_Delimiter, m_Month, m_Delimiter, m_Day);
    }
    // Assign the specified values
    m_Year = year;
    m_Month = month;
    m_Day = day;
}

// ------------------------------------------------------------------------------------------------
void Date::SetStr(const SQChar * str)
{
    // The format specifications that will be used to scan the string
    static SQChar fs[] = _SC(" %u - %u - %u ");
    // Is the specified string empty?
    if (!str || *str == '\0')
    {
        // Clear the values
        m_Year = 0;
        m_Month = 0;
        m_Day = 0;
        // We're done here
        return;
    }
    // Assign the specified delimiter
    fs[4] = m_Delimiter;
    fs[9] = m_Delimiter;
    // The sscanf function requires at least 32 bit integers
    uint32_t year = 0, month = 0, day = 0;
    // Attempt to extract the component values from the specified string
    sscanf(str, fs, &year, &month, &day);
    // Clamp the extracted values to the boundaries of associated type and assign them
    Set(ClampL< uint32_t, uint8_t >(year),
        ClampL< uint32_t, uint8_t >(month),
        ClampL< uint32_t, uint8_t >(day)
    );
}

// ------------------------------------------------------------------------------------------------
void Date::SetDayOfYear(uint16_t doy)
{
    // Reverse the given day of year to a full date
    Date d = Chrono::ReverseDayOfYear(m_Year, doy);
    // Set the obtained month
    SetMonth(d.m_Month);
    // Set the obtained day
    SetDay(d.m_Day);
}

// ------------------------------------------------------------------------------------------------
void Date::SetYear(uint16_t year)
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
void Date::SetMonth(uint8_t month)
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
void Date::SetDay(uint8_t day)
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
Date & Date::AddYears(int32_t years)
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
Date & Date::AddMonths(int32_t months)
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
Date & Date::AddDays(int32_t days)
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
Date Date::AndYears(int32_t years)
{
    // Do we have a valid amount of years?
    if (!years)
    {
        return Date(*this); // Return the date as is
    }
    // Replicate the current date
    Date d(*this);
    // Add the specified amount of years
    d.SetYear(ConvTo< uint16_t >::From(static_cast< int32_t >(m_Year) + years));
    // Return the resulted date
    return d;
}

// ------------------------------------------------------------------------------------------------
Date Date::AndMonths(int32_t months)
{
    // Do we have a valid amount of months?
    if (!months)
    {
        return Date(*this); // Return the date as is
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
    Date d(*this);
    // Are there any years to add?
    if (years)
    {
        d.SetYear(ConvTo< uint16_t >::From(static_cast< int32_t >(m_Year) + years));
    }
    // Add the months
    d.SetMonth(static_cast< uint8_t >(months));
    // Return the resulted date
    return d;
}

// ------------------------------------------------------------------------------------------------
Date Date::AndDays(int32_t days)
{
    // Do we have a valid amount of days?
    if (!days)
    {
        return Date(*this); // Return the date as is
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
    Date d(*this);
    // Set the obtained year
    d.SetYear(static_cast< uint16_t >(year));
    // Set the obtained day of year
    d.SetDayOfYear(static_cast< uint16_t >(doy));
    // Return the resulted date
    return d;
}

// ------------------------------------------------------------------------------------------------
Timestamp Date::GetTimestamp() const
{
    // Calculate the current day of the year
    int32_t days = Chrono::DayOfYear(m_Year, m_Month, m_Day);
    // Calculate all days till the current year
    for (int32_t year = 0; year < m_Year; --year)
    {
        days += Chrono::DaysInYear(static_cast< uint16_t >(year));
    }
    // Return the resulted timestamp
    return Timestamp(static_cast< int64_t >(days * 86400000000LL));
}

// ================================================================================================
void Register_ChronoDate(HSQUIRRELVM vm, Table & /*cns*/)
{
    RootTable(vm).Bind(Typename::Str,
        Class< Date >(vm, Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< uint16_t >()
        .Ctor< uint16_t, uint8_t >()
        .Ctor< uint16_t, uint8_t, uint8_t >()
        // Static Properties
        .SetStaticValue(_SC("GlobalDelimiter"), &Date::Delimiter)
        // Core Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Date::ToString)
        .Func(_SC("cmp"), &Date::Cmp)
        // Meta-methods
        .Func< Date (Date::*)(const Date &) const >(_SC("_add"), &Date::operator +)
        .Func< Date (Date::*)(const Date &) const >(_SC("_sub"), &Date::operator -)
        .Func< Date (Date::*)(const Date &) const >(_SC("_mul"), &Date::operator *)
        .Func< Date (Date::*)(const Date &) const >(_SC("_div"), &Date::operator /)
        // Properties
        .Prop(_SC("Delimiter"), &Date::GetDelimiter, &Date::SetDelimiter)
        .Prop(_SC("Str"), &Date::GetStr, &Date::SetStr)
        .Prop(_SC("DayOfYear"), &Date::GetDayOfYear, &Date::SetDayOfYear)
        .Prop(_SC("Year"), &Date::GetYear, &Date::SetYear)
        .Prop(_SC("Month"), &Date::GetMonth, &Date::SetMonth)
        .Prop(_SC("Day"), &Date::GetDay, &Date::SetDay)
        .Prop(_SC("LeapYear"), &Date::IsThisLeapYear)
        .Prop(_SC("YearDays"), &Date::GetYearDays)
        .Prop(_SC("MonthDays"), &Date::GetMonthDays)
        .Prop(_SC("Timestamp"), &Date::GetTimestamp)
        // Member Methods
        .Func(_SC("AddYears"), &Date::AddYears)
        .Func(_SC("AddMonths"), &Date::AddMonths)
        .Func(_SC("AddDays"), &Date::AddDays)
        .Func(_SC("AndYears"), &Date::AndYears)
        .Func(_SC("AndMonths"), &Date::AndMonths)
        .Func(_SC("AndDays"), &Date::AndDays)
        // Overloaded Methods
        .Overload< void (Date::*)(uint16_t) >(_SC("Set"), &Date::Set)
        .Overload< void (Date::*)(uint16_t, uint8_t) >(_SC("Set"), &Date::Set)
        .Overload< void (Date::*)(uint16_t, uint8_t, uint8_t) >(_SC("Set"), &Date::Set)
    );
}

} // Namespace:: SqMod
