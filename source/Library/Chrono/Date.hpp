#ifndef _LIBRARY_CHRONO_DATE_HPP_
#define _LIBRARY_CHRONO_DATE_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

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

    // ------------------------------------------------------------------------------------------------
    static const Uint8  MonthLengths[12];

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
     * Base constructor.
    */
    Date(Uint16 year)
        : m_Delimiter(Delimiter)
    {
        Set(year, 1, 1);
    }

    /* ------------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Date(Uint16 year, Uint8 month)
        : m_Delimiter(Delimiter)
    {
        Set(year, month, 1);
    }

    /* ------------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Date(Uint16 year, Uint8 month, Uint8 day)
        : m_Delimiter(Delimiter)
    {
        Set(year, month, day);
    }

    /* ------------------------------------------------------------------------------------------------
     * String constructor.
    */
    Date(CSStr str)
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
    Int32 Cmp(const Date & o) const
    {
        return Compare(o);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

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
     * Assign the specified values.
    */
    void Set(Uint16 year)
    {
        Set(year, m_Month, m_Day);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(Uint16 year, Uint8 month)
    {
        Set(year, month, m_Day);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(Uint16 year, Uint8 month, Uint8 day);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the values as a string.
    */
    CSStr GetStr() const;

    /* ------------------------------------------------------------------------------------------------
     * Extract the values from a string.
    */
    void SetStr(CSStr str);

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
     * Add the specified amount of years.
    */
    Date AddYears(Int32 years);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of months.
    */
    Date AddMonths(Int32 months);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of days.
    */
    Date AddDays(Int32 days);

    /* ------------------------------------------------------------------------------------------------
     * See whether the associated year is a leap year.
    */
    bool IsThisLeapYear() const
    {
        return IsLeapYear(m_Year);
    }

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the number of days in the associated year.
    */
    Uint16 GetYearDays() const
    {
        return DaysInYear(m_Year);
    }

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the number of days in the associated month.
    */
    Uint8 GetMonthDays() const
    {
        return DaysInMonth(m_Year, m_Month);
    }

protected:

    /* ------------------------------------------------------------------------------------------------
     * Compare the values of two instances.
    */
    Int32 Compare(const Date & o) const;

private:

    // ------------------------------------------------------------------------------------------------
    Uint16          m_Year; // Year
    Uint8           m_Month; // Month
    Uint8           m_Day; // Day

    // ------------------------------------------------------------------------------------------------
    SQChar          m_Delimiter; // Component delimiter when generating strings.

public:

    /* ------------------------------------------------------------------------------------------------
     * See whether the specified year is a leap year.
    */
    static bool IsLeapYear(Uint16 year)
    {
        return !(year % 400) || (!(year % 4) && (year % 100));
    }

    /* ------------------------------------------------------------------------------------------------
     * See whether the specified date is valid.
    */
    static bool ValidDate(Uint16 year, Uint8 month, Uint8 day);

    /* ------------------------------------------------------------------------------------------------
     * retrieve the number of days in the specified year.
    */
    static Uint16 DaysInYear(Uint16 year)
    {
        return IsLeapYear(year) ? 366 : 365;
    }

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the number of days in the specified month.
    */
    static Uint8 DaysInMonth(Uint16 year, Uint8 month);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the number/position of the specified day in the specified year and month.
    */
    static Uint16 DayOfYear(Uint16 year, Uint8 month, Uint8 day);
};

} // Namespace:: SqMod

#endif // _LIBRARY_CHRONO_DATE_HPP_
