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
    uint16_t  m_Year{}; // Year
    uint8_t   m_Month{}; // Month
    uint8_t   m_Day{}; // Day

    // ------------------------------------------------------------------------------------------------
    uint8_t   m_Hour{}; // Hour
    uint8_t   m_Minute{}; // Minute
    uint8_t   m_Second{}; // Second
    uint16_t  m_Millisecond{}; // Millisecond

    // ------------------------------------------------------------------------------------------------
    SQChar  m_Delimiter; // Date and time delimiter when generating strings.
    SQChar  m_DateDelim; // Date component delimiter when generating strings.
    SQChar  m_TimeDelim; // Time component delimiter when generating strings.

protected:

    /* ------------------------------------------------------------------------------------------------
     * Compare the values of two instances.
    */
    SQMOD_NODISCARD int32_t Compare(const Datetime & o) const;

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
     * Specific year constructor.
    */
    explicit Datetime(uint16_t year)
        : m_Delimiter(Delimiter)
        , m_DateDelim(DateDelim)
        , m_TimeDelim(TimeDelim)
    {
        Set(year, 1, 1, 0, 0, 0, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Specific year and month constructor.
    */
    Datetime(uint16_t year, uint8_t month)
        : m_Delimiter(Delimiter)
        , m_DateDelim(DateDelim)
        , m_TimeDelim(TimeDelim)
    {
        Set(year, month, 1, 0, 0, 0, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Specific date constructor.
    */
    Datetime(uint16_t year, uint8_t month, uint8_t day)
        : m_Delimiter(Delimiter)
        , m_DateDelim(DateDelim)
        , m_TimeDelim(TimeDelim)
    {
        Set(year, month, day, 0, 0, 0, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Specific date and hour constructor.
    */
    Datetime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour)
        : m_Delimiter(Delimiter)
        , m_DateDelim(DateDelim)
        , m_TimeDelim(TimeDelim)
    {
        Set(year, month, day, hour, 0, 0, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Specific date, hour and minute constructor.
    */
    Datetime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute)
        : m_Delimiter(Delimiter)
        , m_DateDelim(DateDelim)
        , m_TimeDelim(TimeDelim)
    {
        Set(year, month, day, hour, minute, 0, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Specific date and time constructor.
    */
    Datetime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
        : m_Delimiter(Delimiter)
        , m_DateDelim(DateDelim)
        , m_TimeDelim(TimeDelim)
    {
        Set(year, month, day, hour, minute, second, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Specific date and precise time constructor.
    */
    Datetime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond)
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
    SQMOD_NODISCARD int32_t Cmp(const Datetime & o) const
    {
        return Compare(o);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    SQMOD_NODISCARD String ToString() const;

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(uint16_t year)
    {
        Set(year, m_Month, m_Day, m_Hour, m_Minute, m_Second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(uint16_t year, uint8_t month)
    {
        Set(year, month, m_Day, m_Hour, m_Minute, m_Second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(uint16_t year, uint8_t month, uint8_t day)
    {
        Set(year, month, day, m_Hour, m_Minute, m_Second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(uint16_t year, uint8_t month, uint8_t day, uint8_t hour)
    {
        Set(year, month, day, hour, m_Minute, m_Second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute)
    {
        Set(year, month, day, hour, minute, m_Second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
    {
        Set(year, month, day, hour, minute, second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the local delimiter character.
    */
    SQMOD_NODISCARD SQChar GetDelimiter() const
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
    SQMOD_NODISCARD SQChar GetDateDelim() const
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
    SQMOD_NODISCARD SQChar GetTimeDelim() const
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
    SQMOD_NODISCARD String GetStr() const
    {
        return ToString();
    }

    /* ------------------------------------------------------------------------------------------------
     * Extract the values from a string.
    */
    void SetStr(const SQChar * str);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the day component.
    */
    SQMOD_NODISCARD uint16_t GetDayOfYear() const
    {
        return Chrono::DayOfYear(m_Year, m_Month, m_Day);
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the day component.
    */
    void SetDayOfYear(uint16_t doy);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the year component.
    */
    SQMOD_NODISCARD uint16_t GetYear() const
    {
        return m_Year;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the year component.
    */
    void SetYear(uint16_t year);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the month component.
    */
    SQMOD_NODISCARD uint8_t GetMonth() const
    {
        return m_Month;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the month component.
    */
    void SetMonth(uint8_t month);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the day component.
    */
    SQMOD_NODISCARD uint8_t GetDay() const
    {
        return m_Day;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the day component.
    */
    void SetDay(uint8_t day);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the hour component.
    */
    SQMOD_NODISCARD uint8_t GetHour() const
    {
        return m_Hour;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the hour component.
    */
    void SetHour(uint8_t hour);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the minute component.
    */
    SQMOD_NODISCARD uint8_t GetMinute() const
    {
        return m_Minute;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the minute component.
    */
    void SetMinute(uint8_t minute);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the second component.
    */
    SQMOD_NODISCARD uint8_t GetSecond() const
    {
        return m_Second;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the second component.
    */
    void SetSecond(uint8_t second);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the millisecond component.
    */
    SQMOD_NODISCARD uint16_t GetMillisecond() const
    {
        return m_Millisecond;
    }

    /* ------------------------------------------------------------------------------------------------
     * Modify the millisecond component.
    */
    void SetMillisecond(uint16_t millisecond);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of years to the current date.
    */
    Datetime & AddYears(int32_t years);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of months to the current date.
    */
    Datetime & AddMonths(int32_t months);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of days to the current date.
    */
    Datetime & AddDays(int32_t days);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of hours to the current time.
    */
    Datetime & AddHours(int32_t hours);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of minutes to the current time.
    */
    Datetime & AddMinutes(int32_t minutes);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of seconds to the current time.
    */
    Datetime & AddSeconds(int32_t seconds);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of milliseconds to the current time.
    */
    Datetime & AddMilliseconds(int32_t milliseconds);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of years to obtain a new date.
    */
    SQMOD_NODISCARD Datetime AndYears(int32_t years);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of months to obtain a new date.
    */
    SQMOD_NODISCARD Datetime AndMonths(int32_t months);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of days to obtain a new date.
    */
    SQMOD_NODISCARD Datetime AndDays(int32_t days);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of hours to obtain a new time.
    */
    SQMOD_NODISCARD Datetime AndHours(int32_t hours);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of minutes to obtain a new time.
    */
    SQMOD_NODISCARD Datetime AndMinutes(int32_t minutes);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of seconds to obtain a new time.
    */
    SQMOD_NODISCARD Datetime AndSeconds(int32_t seconds);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of milliseconds to obtain a new time.
    */
    SQMOD_NODISCARD Datetime AndMilliseconds(int32_t milliseconds);

    /* ------------------------------------------------------------------------------------------------
     * See whether the associated year is a leap year.
    */
    SQMOD_NODISCARD bool IsThisLeapYear() const
    {
        return Chrono::IsLeapYear(m_Year);
    }

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the number of days in the associated year.
    */
    SQMOD_NODISCARD uint16_t GetYearDays() const
    {
        return Chrono::DaysInYear(m_Year);
    }

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the number of days in the associated month.
    */
    SQMOD_NODISCARD uint8_t GetMonthDays() const
    {
        return Chrono::DaysInMonth(m_Year, m_Month);
    }

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the date from this date-time instance.
    */
    SQMOD_NODISCARD Date GetDate() const;

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the time from this date-time instance.
    */
    SQMOD_NODISCARD Time GetTime() const;

    /* ------------------------------------------------------------------------------------------------
     * Convert this date-time instance to a time-stamp.
    */
    SQMOD_NODISCARD Timestamp GetTimestamp() const;
};

} // Namespace:: SqMod
