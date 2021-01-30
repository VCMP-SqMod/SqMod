#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/Chrono.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Helper class used to represent a certain date.
*/
class Date
{
public:

    // ------------------------------------------------------------------------------------------------
    static SQChar       Delimiter;

private:

    // ------------------------------------------------------------------------------------------------
    uint16_t  m_Year{}; // Year
    uint8_t   m_Month{}; // Month
    uint8_t   m_Day{}; // Day

    // ------------------------------------------------------------------------------------------------
    SQChar  m_Delimiter; // Component delimiter when generating strings.

protected:

    /* ------------------------------------------------------------------------------------------------
     * Compare the values of two instances.
    */
    SQMOD_NODISCARD int32_t Compare(const Date & o) const;

public:

    /* ------------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Date()
        : m_Year(1970)
        , m_Month(1)
        , m_Day(1)
        , m_Delimiter(Delimiter)
    {
        /* ... */
    }

    /* ------------------------------------------------------------------------------------------------
     * Specific year constructor.
    */
    explicit Date(uint16_t year)
        : m_Delimiter(Delimiter)
    {
        Set(year, 1, 1);
    }

    /* ------------------------------------------------------------------------------------------------
     * Specific year and month constructor.
    */
    Date(uint16_t year, uint8_t month)
        : m_Delimiter(Delimiter)
    {
        Set(year, month, 1);
    }

    /* ------------------------------------------------------------------------------------------------
     * Specific date constructor.
    */
    Date(uint16_t year, uint8_t month, uint8_t day)
        : m_Delimiter(Delimiter)
    {
        Set(year, month, day);
    }

    /* ------------------------------------------------------------------------------------------------
     * String constructor.
    */
    explicit Date(const SQChar * str)
        : m_Delimiter(Delimiter)
    {
        SetStr(str);
    }

    /* ------------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Date(const Date & o) = default;

    /* ------------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Date(Date && o) = default;

    /* ------------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Date() = default;

    /* ------------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Date & operator = (const Date & o) = default;

    /* ------------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Date & operator = (Date && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const Date & o) const
    {
        return Compare(o) == 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const Date & o) const
    {
        return Compare(o) != 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const Date & o) const
    {
        return Compare(o) < 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const Date & o) const
    {
        return Compare(o) > 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const Date & o) const
    {
        return Compare(o) <= 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const Date & o) const
    {
        return Compare(o) >= 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    Date operator + (const Date & o) const;

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    Date operator - (const Date & o) const;

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    Date operator * (const Date & o) const;

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    Date operator / (const Date & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    SQMOD_NODISCARD int32_t Cmp(const Date & o) const
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
        Set(year, m_Month, m_Day);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(uint16_t year, uint8_t month)
    {
        Set(year, month, m_Day);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(uint16_t year, uint8_t month, uint8_t day);

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
     * Add the specified amount of years to the current date.
    */
    Date & AddYears(int32_t years);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of months to the current date.
    */
    Date & AddMonths(int32_t months);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of days to the current date.
    */
    Date & AddDays(int32_t days);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of years to obtain a new date.
    */
    SQMOD_NODISCARD Date AndYears(int32_t years);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of months to obtain a new date.
    */
    SQMOD_NODISCARD Date AndMonths(int32_t months);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of days to obtain a new date.
    */
    SQMOD_NODISCARD Date AndDays(int32_t days);

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
     * Convert this date instance to a time-stamp.
    */
    SQMOD_NODISCARD Timestamp GetTimestamp() const;
};

} // Namespace:: SqMod
