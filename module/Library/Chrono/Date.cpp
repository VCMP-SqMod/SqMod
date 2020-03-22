// ------------------------------------------------------------------------------------------------
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Time.hpp"
#include "Library/Chrono/Datetime.hpp"
#include "Library/Chrono/Timestamp.hpp"
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("SqDate"))

// ------------------------------------------------------------------------------------------------
SQChar Date::Delimiter = '-';

// ------------------------------------------------------------------------------------------------
Int32 Date::Compare(const Date & o) const
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
CSStr Date::ToString() const
{
    return ToStrF("%04u%c%02u%c%02u", m_Year, m_Delimiter, m_Month, m_Delimiter, m_Day);
}

// ------------------------------------------------------------------------------------------------
void Date::Set(Uint16 year, Uint8 month, Uint8 day)
{
    if (!Chrono::ValidDate(year, month, day))
    {
        STHROWF("Invalid date: %04u%c%02u%c%02u" , m_Year, m_Delimiter, m_Month, m_Delimiter, m_Day);
    }
    // Assign the specified values
    m_Year = year;
    m_Month = month;
    m_Day = day;
}

// ------------------------------------------------------------------------------------------------
void Date::SetStr(CSStr str)
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
    Uint32 year = 0, month = 0, day = 0;
    // Attempt to extract the component values from the specified string
    sscanf(str, fs, &year, &month, &day);
    // Clamp the extracted values to the boundaries of associated type and assign them
    Set(ClampL< Uint32, Uint8 >(year),
        ClampL< Uint32, Uint8 >(month),
        ClampL< Uint32, Uint8 >(day)
    );
}

// ------------------------------------------------------------------------------------------------
void Date::SetDayOfYear(Uint16 doy)
{
    // Reverse the given day of year to a full date
    Date d = Chrono::ReverseDayOfyear(m_Year, doy);
    // Set the obtained month
    SetMonth(d.m_Month);
    // Set the obtained day
    SetDay(d.m_Day);
}

// ------------------------------------------------------------------------------------------------
void Date::SetYear(Uint16 year)
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
void Date::SetMonth(Uint8 month)
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
void Date::SetDay(Uint8 day)
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
Date & Date::AddYears(Int32 years)
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
Date & Date::AddMonths(Int32 months)
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
Date & Date::AddDays(Int32 days)
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
Date Date::AndYears(Int32 years)
{
    // Do we have a valid amount of years?
    if (!years)
    {
        return Date(*this); // Return the date as is
    }
    // Replicate the current date
    Date d(*this);
    // Add the specified amount of years
    d.SetYear(ConvTo< Uint16 >::From(static_cast< Int32 >(m_Year) + years));
    // Return the resulted date
    return d;
}

// ------------------------------------------------------------------------------------------------
Date Date::AndMonths(Int32 months)
{
    // Do we have a valid amount of months?
    if (!months)
    {
        return Date(*this); // Return the date as is
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
    Date d(*this);
    // Are there any years to add?
    if (years)
    {
        d.SetYear(ConvTo< Uint16 >::From(static_cast< Int32 >(m_Year) + years));
    }
    // Add the months
    d.SetMonth(months);
    // Return the resulted date
    return d;
}

// ------------------------------------------------------------------------------------------------
Date Date::AndDays(Int32 days)
{
    // Do we have a valid amount of days?
    if (!days)
    {
        return Date(*this); // Return the date as is
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
    Date d(*this);
    // Set the obtained year
    d.SetYear(year);
    // Set the obtained day of year
    d.SetDayOfYear(doy);
    // Return the resulted date
    return d;
}

// ------------------------------------------------------------------------------------------------
Timestamp Date::GetTimestamp() const
{
    // Calculate the current day of the year
    Int32 days = Chrono::DayOfYear(m_Year, m_Month, m_Day);
    // Calculate all days till the current year
    for (Int32 year = 0; year < m_Year; --year)
    {
        days += Chrono::DaysInYear(year);
    }
    // Return the resulted timestamp
    return Timestamp(static_cast< Int64 >(days * 86400000000LL));
}

// ================================================================================================
void Register_ChronoDate(HSQUIRRELVM vm, Table & /*cns*/)
{
    RootTable(vm).Bind(Typename::Str,
        Class< Date >(vm, Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< Uint16 >()
        .Ctor< Uint16, Uint8 >()
        .Ctor< Uint16, Uint8, Uint8 >()
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
        .Overload< void (Date::*)(Uint16) >(_SC("Set"), &Date::Set)
        .Overload< void (Date::*)(Uint16, Uint8) >(_SC("Set"), &Date::Set)
        .Overload< void (Date::*)(Uint16, Uint8, Uint8) >(_SC("Set"), &Date::Set)
    );
}

} // Namespace:: SqMod
