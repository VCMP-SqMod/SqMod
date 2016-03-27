// ------------------------------------------------------------------------------------------------
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Datetime.hpp"
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQChar Date::Delimiter = '-';

// ------------------------------------------------------------------------------------------------
const Uint8 Date::MonthLengths[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

// ------------------------------------------------------------------------------------------------
SQInteger Date::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqChronoDate");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Date Date::operator + (const Date & o) const
{
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
void Date::Set(Uint16 year, Uint8 month, Uint8 day)
{
    if (!ValidDate(year, month, day))
    {
        STHROWF("Invalid date: %04u%c%02u%c%02u%c%u"
            , m_Delimiter, m_Year
            , m_Delimiter, m_Month
            , m_Delimiter, m_Day
        );
    }
}

// ------------------------------------------------------------------------------------------------
CSStr Date::GetStr() const
{
    return ToString();
}

// ------------------------------------------------------------------------------------------------
void Date::SetStr(CSStr str)
{
    // The format specifications that will be used to scan the string
    static SQChar fs[] = _SC(" %u , %u , %u,");
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
CSStr Date::ToString() const
{
    return ToStrF("%04u%c%02u%c%02u%c%u"
        , m_Delimiter, m_Year
        , m_Delimiter, m_Month
        , m_Delimiter, m_Day
    );
}

// ------------------------------------------------------------------------------------------------
void Date::SetYear(Uint16 year)
{

}

// ------------------------------------------------------------------------------------------------
void Date::SetMonth(Uint8 month)
{

}

// ------------------------------------------------------------------------------------------------
void Date::SetDay(Uint8 day)
{

}

// ------------------------------------------------------------------------------------------------
Date Date::AddYears(Int32 years)
{
    return Date(*this);
}

// ------------------------------------------------------------------------------------------------
Date Date::AddMonths(Int32 months)
{
    return Date(*this);
}

// ------------------------------------------------------------------------------------------------
Date Date::AddDays(Int32 days)
{
    return Date(*this);
}

// ------------------------------------------------------------------------------------------------
bool Date::ValidDate(Uint16 year, Uint8 month, Uint8 day)
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
Uint8 Date::DaysInMonth(Uint16 year, Uint8 month)
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
Uint16 Date::DayOfYear(Uint16 year, Uint8 month, Uint8 day)
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

// ================================================================================================
void Register_ChronoDate(HSQUIRRELVM vm, Table & /*cns*/)
{
    RootTable(vm).Bind(_SC("SqDate"), Class< Date >(vm, _SC("SqChronoDate"))
        // Constructors
        .Ctor()
        .Ctor< Uint16 >()
        .Ctor< Uint16, Uint8 >()
        .Ctor< Uint16, Uint8, Uint8 >()
        // Static Properties
        .SetStaticValue(_SC("GlobalDelimiter"), &Date::Delimiter)
        // Core Metamethods
        .Func(_SC("_tostring"), &Date::ToString)
        .SquirrelFunc(_SC("_typename"), &Date::Typename)
        .Func(_SC("_cmp"), &Date::Cmp)
        // Metamethods
        .Func< Date (Date::*)(const Date &) const >(_SC("_add"), &Date::operator +)
        .Func< Date (Date::*)(const Date &) const >(_SC("_sub"), &Date::operator -)
        .Func< Date (Date::*)(const Date &) const >(_SC("_mul"), &Date::operator *)
        .Func< Date (Date::*)(const Date &) const >(_SC("_div"), &Date::operator /)
        // Properties
        .Prop(_SC("Year"), &Date::GetYear, &Date::SetYear)
        .Prop(_SC("Month"), &Date::GetMonth, &Date::SetMonth)
        .Prop(_SC("Day"), &Date::GetDay, &Date::SetDay)
        .Prop(_SC("Str"), &Date::GetStr, &Date::SetStr)
        .Prop(_SC("Delimiter"), &Date::GetDelimiter, &Date::SetDelimiter)
        .Prop(_SC("LeapYear"), &Date::IsThisLeapYear)
        .Prop(_SC("YearDays"), &Date::GetYearDays)
        .Prop(_SC("MonthDays"), &Date::GetMonthDays)
        // Member Methods
        .Func(_SC("AddYears"), &Date::AddYears)
        .Func(_SC("AddMonths"), &Date::AddMonths)
        .Func(_SC("AddDays"), &Date::AddDays)
        // Overloaded Methods
        .Overload< void (Date::*)(Uint16) >(_SC("Set"), &Date::Set)
        .Overload< void (Date::*)(Uint16, Uint8) >(_SC("Set"), &Date::Set)
        .Overload< void (Date::*)(Uint16, Uint8, Uint8) >(_SC("Set"), &Date::Set)
    );
}

} // Namespace:: SqMod
