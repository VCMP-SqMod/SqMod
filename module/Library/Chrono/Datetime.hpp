#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/Chrono.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Helper class used to represent a certain date and time.
*/
class Datetime
{
public:

    // ------------------------------------------------------------------------------------------------
    static SQChar       Delimiter;
    static SQChar       DateDelim;
    static SQChar       TimeDelim;

private:

    // ------------------------------------------------------------------------------------------------
    Uint16  m_Year; // Year
    Uint8   m_Month; // Month
    Uint8   m_Day; // Day

    // ------------------------------------------------------------------------------------------------
    Uint8   m_Hour; // Hour
    Uint8   m_Minute; // Minute
    Uint8   m_Second; // Second
    Uint16  m_Millisecond; // Millisecond

    // ------------------------------------------------------------------------------------------------
    SQChar  m_Delimiter; // Date and time delimiter when generating strings.
    SQChar  m_DateDelim; // Date component delimiter when generating strings.
    SQChar  m_TimeDelim; // Time component delimiter when generating strings.

protected:

    /* ------------------------------------------------------------------------------------------------
     * Compare the values of two instances.
    */
    Int32 Compare(const Datetime & o) const;

public:

    /* ------------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Datetime()
        : m_Year(1970)
        , m_Month(1)
        , m_Day(1)
        , m_Hour(0)
        , m_Minute(0)
        , m_Second(0)
        , m_Millisecond(0)
        , m_Delimiter(Delimiter)
        , m_DateDelim(DateDelim)
        , m_TimeDelim(TimeDelim)
    {
        /* ... */
    }

    /* ------------------------------------------------------------------------------------------------
     * Speciffic year constructor.
    */
    Datetime(Uint16 year)
        : m_Delimiter(Delimiter)
        , m_DateDelim(DateDelim)
        , m_TimeDelim(TimeDelim)
    {
        Set(year, 1, 1, 0, 0, 0, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Speciffic year and month constructor.
    */
    Datetime(Uint16 year, Uint8 month)
        : m_Delimiter(Delimiter)
        , m_DateDelim(DateDelim)
        , m_TimeDelim(TimeDelim)
    {
        Set(year, month, 1, 0, 0, 0, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Speciffic date constructor.
    */
    Datetime(Uint16 year, Uint8 month, Uint8 day)
        : m_Delimiter(Delimiter)
        , m_DateDelim(DateDelim)
        , m_TimeDelim(TimeDelim)
    {
        Set(year, month, day, 0, 0, 0, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Speciffic date and hour constructor.
    */
    Datetime(Uint16 year, Uint8 month, Uint8 day, Uint8 hour)
        : m_Delimiter(Delimiter)
        , m_DateDelim(DateDelim)
        , m_TimeDelim(TimeDelim)
    {
        Set(year, month, day, hour, 0, 0, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Speciffic date, hour and minute constructor.
    */
    Datetime(Uint16 year, Uint8 month, Uint8 day, Uint8 hour, Uint8 minute)
        : m_Delimiter(Delimiter)
        , m_DateDelim(DateDelim)
        , m_TimeDelim(TimeDelim)
    {
        Set(year, month, day, hour, minute, 0, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Speciffic date and time constructor.
    */
    Datetime(Uint16 year, Uint8 month, Uint8 day, Uint8 hour, Uint8 minute, Uint8 second)
        : m_Delimiter(Delimiter)
        , m_DateDelim(DateDelim)
        , m_TimeDelim(TimeDelim)
    {
        Set(year, month, day, hour, minute, second, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Speciffic date and precise time constructor.
    */
    Datetime(Uint16 year, Uint8 month, Uint8 day, Uint8 hour, Uint8 minute, Uint8 second, Uint16 millisecond)
        : m_Delimiter(Delimiter)
        , m_DateDelim(DateDelim)
        , m_TimeDelim(TimeDelim)
    {
        Set(year, month, day, hour, minute, second, millisecond);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Datetime(const Datetime & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Datetime(Datetime && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Datetime() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Datetime & operator = (const Datetime & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Datetime & operator = (Datetime && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const Datetime & o) const
    {
        return Compare(o) == 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const Datetime & o) const
    {
        return Compare(o) != 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const Datetime & o) const
    {
        return Compare(o) < 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const Datetime & o) const
    {
        return Compare(o) > 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const Datetime & o) const
    {
        return Compare(o) <= 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const Datetime & o) const
    {
        return Compare(o) >= 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    Datetime operator + (const Datetime & o) const;

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    Datetime operator - (const Datetime & o) const;

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    Datetime operator * (const Datetime & o) const;

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    Datetime operator / (const Datetime & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Datetime & o) const
    {
        return Compare(o);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(Uint16 year)
    {
        Set(year, m_Month, m_Day, m_Hour, m_Minute, m_Second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(Uint16 year, Uint8 month)
    {
        Set(year, month, m_Day, m_Hour, m_Minute, m_Second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(Uint16 year, Uint8 month, Uint8 day)
    {
        Set(year, month, day, m_Hour, m_Minute, m_Second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(Uint16 year, Uint8 month, Uint8 day, Uint8 hour)
    {
        Set(year, month, day, hour, m_Minute, m_Second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(Uint16 year, Uint8 month, Uint8 day, Uint8 hour, Uint8 minute)
    {
        Set(year, month, day, hour, minute, m_Second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(Uint16 year, Uint8 month, Uint8 day, Uint8 hour, Uint8 minute, Uint8 second)
    {
        Set(year, month, day, hour, minute, second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(Uint16 year, Uint8 month, Uint8 day, Uint8 hour, Uint8 minute, Uint8 second, Uint16 millisecond);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the local delimiter character.
    */
    SQChar GetDelimiter() const
    {
        return m_Delimiter;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the local delimiter character.
    */
    void SetDelimiter(SQChar c)
    {
        m_Delimiter = c;
    }

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the local date delimiter character.
    */
    SQChar GetDateDelim() const
    {
        return m_DateDelim;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the local date delimiter character.
    */
    void SetDateDelim(SQChar c)
    {
        m_DateDelim = c;
    }

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the local time delimiter character.
    */
    SQChar GetTimeDelim() const
    {
        return m_TimeDelim;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the local time delimiter character.
    */
    void SetTimeDelim(SQChar c)
    {
        m_TimeDelim = c;
    }

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the values as a string.
    */
    CSStr GetStr() const
    {
        return ToString();
    }

    /* ------------------------------------------------------------------------------------------------
     * Extract the values from a string.
    */
    void SetStr(CSStr str);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the day component.
    */
    Uint16 GetDayOfYear() const
    {
        return Chrono::DayOfYear(m_Year, m_Month, m_Day);
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the day component.
    */
    void SetDayOfYear(Uint16 doy);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the year component.
    */
    Uint16 GetYear() const
    {
        return m_Year;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the year component.
    */
    void SetYear(Uint16 year);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the month component.
    */
    Uint8 GetMonth() const
    {
        return m_Month;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the month component.
    */
    void SetMonth(Uint8 month);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the day component.
    */
    Uint8 GetDay() const
    {
        return m_Day;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the day component.
    */
    void SetDay(Uint8 day);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the hour component.
    */
    Uint8 GetHour() const
    {
        return m_Hour;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the hour component.
    */
    void SetHour(Uint8 hour);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the minute component.
    */
    Uint8 GetMinute() const
    {
        return m_Minute;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the minute component.
    */
    void SetMinute(Uint8 minute);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the second component.
    */
    Uint8 GetSecond() const
    {
        return m_Second;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the second component.
    */
    void SetSecond(Uint8 second);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the millisecond component.
    */
    Uint16 GetMillisecond() const
    {
        return m_Millisecond;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the millisecond component.
    */
    void SetMillisecond(Uint16 millisecond);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of years to the current date.
    */
    Datetime & AddYears(Int32 years);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of months to the current date.
    */
    Datetime & AddMonths(Int32 months);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of days to the current date.
    */
    Datetime & AddDays(Int32 days);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of hours to the current time.
    */
    Datetime & AddHours(Int32 hours);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of minutes to the current time.
    */
    Datetime & AddMinutes(Int32 minutes);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of seconds to the current time.
    */
    Datetime & AddSeconds(Int32 seconds);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of milliseconds to the current time.
    */
    Datetime & AddMilliseconds(Int32 milliseconds);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of years to obtain a new date.
    */
    Datetime AndYears(Int32 years);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of months to obtain a new date.
    */
    Datetime AndMonths(Int32 months);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of days to obtain a new date.
    */
    Datetime AndDays(Int32 days);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of hours to obtain a new time.
    */
    Datetime AndHours(Int32 hours);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of minutes to obtain a new time.
    */
    Datetime AndMinutes(Int32 minutes);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of seconds to obtain a new time.
    */
    Datetime AndSeconds(Int32 seconds);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of milliseconds to obtain a new time.
    */
    Datetime AndMilliseconds(Int32 milliseconds);

    /* ------------------------------------------------------------------------------------------------
     * See whether the associated year is a leap year.
    */
    bool IsThisLeapYear() const
    {
        return Chrono::IsLeapYear(m_Year);
    }

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the number of days in the associated year.
    */
    Uint16 GetYearDays() const
    {
        return Chrono::DaysInYear(m_Year);
    }

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the number of days in the associated month.
    */
    Uint8 GetMonthDays() const
    {
        return Chrono::DaysInMonth(m_Year, m_Month);
    }

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the date from this date-time instance.
    */
    Date GetDate() const;

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the time from this date-time instance.
    */
    Time GetTime() const;

    /* ------------------------------------------------------------------------------------------------
     * Convert this date-time instance to a time-stamp.
    */
    Timestamp GetTimestamp() const;
};

} // Namespace:: SqMod
