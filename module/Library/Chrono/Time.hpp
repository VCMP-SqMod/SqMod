#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/Chrono.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Helper class used to represent a certain time.
*/
class Time
{
public:

    // ------------------------------------------------------------------------------------------------
    static SQChar   Delimiter;

protected:

    /* ------------------------------------------------------------------------------------------------
     * Compare the values of two instances.
    */
    SQMOD_NODISCARD int32_t Compare(const Time & o) const;

private:

    // ------------------------------------------------------------------------------------------------
    uint8_t   m_Hour{0}; // Hour
    uint8_t   m_Minute{0}; // Minute
    uint8_t   m_Second{0}; // Second
    uint16_t  m_Millisecond{0}; // Millisecond

    // ------------------------------------------------------------------------------------------------
    SQChar  m_Delimiter; // Component delimiter when generating strings.

public:

    /* ------------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Time()
        : m_Hour(0)
        , m_Minute(0)
        , m_Second(0)
        , m_Millisecond(0)
        , m_Delimiter(Delimiter)
    {
        /* ... */
    }

    /* ------------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit Time(uint8_t hour)
        : m_Delimiter(Delimiter)
    {
        Set(hour, 0, 0, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Time(uint8_t hour, uint8_t minute)
        : m_Delimiter(Delimiter)
    {
        Set(hour, minute, 0, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Time(uint8_t hour, uint8_t minute, uint8_t second)
        : m_Delimiter(Delimiter)
    {
        Set(hour, minute, second, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Time(uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond)
        : m_Delimiter(Delimiter)
    {
        Set(hour, minute, second, millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * String constructor.
    */
    explicit Time(const SQChar * str)
        : m_Delimiter(Delimiter)
    {
        SetStr(str);
    }

    /* ------------------------------------------------------------------------------------------------
     * Time-stamp constructor.
    */
    explicit Time(int64_t ts);

    /* ------------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Time(const Time & o) = default;

    /* ------------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Time(Time && o) = default;

    /* ------------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Time() = default;

    /* ------------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Time & operator = (const Time & o) = default;

    /* ------------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Time & operator = (Time && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const Time & o) const
    {
        return Compare(o) == 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const Time & o) const
    {
        return Compare(o) != 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const Time & o) const
    {
        return Compare(o) < 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const Time & o) const
    {
        return Compare(o) > 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const Time & o) const
    {
        return Compare(o) <= 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const Time & o) const
    {
        return Compare(o) >= 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    Time operator + (const Time & o) const;

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    Time operator - (const Time & o) const;

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    Time operator * (const Time & o) const;

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    Time operator / (const Time & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    SQMOD_NODISCARD int32_t Cmp(const Time & o) const
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
    void Set(uint8_t hour)
    {
        Set(hour, m_Minute, m_Second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(uint8_t hour, uint8_t minute)
    {
        Set(hour, minute, m_Second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(uint8_t hour, uint8_t minute, uint8_t second)
    {
        Set(hour, minute, second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond);

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
     * Add the specified amount of hours to the current time.
    */
    Time & AddHours(int32_t hours);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of minutes to the current time.
    */
    Time & AddMinutes(int32_t minutes);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of seconds to the current time.
    */
    Time & AddSeconds(int32_t seconds);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of milliseconds to the current time.
    */
    Time & AddMilliseconds(int32_t milliseconds);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of hours to obtain a new time.
    */
    SQMOD_NODISCARD Time AndHours(int32_t hours);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of minutes to obtain a new time.
    */
    SQMOD_NODISCARD Time AndMinutes(int32_t minutes);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of seconds to obtain a new time.
    */
    SQMOD_NODISCARD Time AndSeconds(int32_t seconds);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of milliseconds to obtain a new time.
    */
    SQMOD_NODISCARD Time AndMilliseconds(int32_t milliseconds);

    /* ------------------------------------------------------------------------------------------------
     * Convert this time instance to a time-stamp.
    */
    SQMOD_NODISCARD Timestamp GetTimestamp() const;

    /* --------------------------------------------------------------------------------------------
     *
    */
    std::time_t ToTimeT() const;
};

} // Namespace:: SqMod
