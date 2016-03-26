#ifndef _LIBRARY_CHRONO_TIME_HPP_
#define _LIBRARY_CHRONO_TIME_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

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
    Time(Uint8 hour)
        : m_Delimiter(Delimiter)
    {
        Set(hour, 0, 0, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Time(Uint8 hour, Uint8 minute)
        : m_Delimiter(Delimiter)
    {
        Set(hour, minute, 0, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Time(Uint8 hour, Uint8 minute, Uint8 second)
        : m_Delimiter(Delimiter)
    {
        Set(hour, minute, second, 0);
    }

    /* ------------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Time(Uint8 hour, Uint8 minute, Uint8 second, Uint16 millisecond)
        : m_Delimiter(Delimiter)
    {
        Set(hour, minute, second, millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * String constructor.
    */
    Time(CSStr str)
        : m_Delimiter(Delimiter)
    {
        SetStr(str);
    }

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
    Int32 Cmp(const Time & o) const
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
    void Set(Uint8 hour)
    {
        Set(hour, m_Minute, m_Second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(Uint8 hour, Uint8 minute)
    {
        Set(hour, minute, m_Second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(Uint8 hour, Uint8 minute, Uint8 second)
    {
        Set(hour, minute, second, m_Millisecond);
    }

    /* ------------------------------------------------------------------------------------------------
     * Assign the specified values.
    */
    void Set(Uint8 hour, Uint8 minute, Uint8 second, Uint16 millisecond);

    /* ------------------------------------------------------------------------------------------------
     * Retrieve the values as a string.
    */
    CSStr GetStr() const;

    /* ------------------------------------------------------------------------------------------------
     * Extract the values from a string.
    */
    void SetStr(CSStr str);

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
     * Add the specified amount of hours.
    */
    Time AddHours(Int32 hours);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of minutes.
    */
    Time AddMinutes(Int32 minutes);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of seconds.
    */
    Time AddSeconds(Int32 seconds);

    /* ------------------------------------------------------------------------------------------------
     * Add the specified amount of milliseconds.
    */
    Time AddMilliseconds(Int32 milliseconds);

protected:

    /* ------------------------------------------------------------------------------------------------
     * Compare the values of two instances.
    */
    Int32 Compare(const Time & o) const;

private:

    // ------------------------------------------------------------------------------------------------
    Uint8   m_Hour; // Hour
    Uint8   m_Minute; // Minute
    Uint8   m_Second; // Second
    Uint16  m_Millisecond; // Millisecond

    // ------------------------------------------------------------------------------------------------
    SQChar  m_Delimiter; // Component selimiter when generating strings.
};

} // Namespace:: SqMod

#endif // _LIBRARY_CHRONO_TIME_HPP_