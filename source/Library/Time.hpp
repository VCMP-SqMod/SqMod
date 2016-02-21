#ifndef _LIBRARY_TIMER_HPP_
#define _LIBRARY_TIMER_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
class Timer;

/* ------------------------------------------------------------------------------------------------
 * Retrieve the current time as microseconds.
*/
Int64 GetCurrentTime();

/* ------------------------------------------------------------------------------------------------
 *
*/
Int64 GetEpochTimeMicro();

/* ------------------------------------------------------------------------------------------------
 *
*/
Int64 GetEpochTimeMilli();

/* ------------------------------------------------------------------------------------------------
 *
*/
class Timestamp
{
    // --------------------------------------------------------------------------------------------
    friend class Timer;

public:

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp()
        : m_Timestamp(0)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp(Int64 t)
        : m_Timestamp(t)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    explicit Timestamp(const SLongInt & t);

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp(const Timestamp & o)
        : m_Timestamp(o.m_Timestamp)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    ~Timestamp()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & operator = (const Timestamp o)
    {
        m_Timestamp = o.m_Timestamp;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Timestamp operator + (const Timestamp & o) const
    {
        return Timestamp(m_Timestamp + o.m_Timestamp);
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Timestamp operator - (const Timestamp & o) const
    {
        return Timestamp(m_Timestamp - o.m_Timestamp);
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Timestamp operator * (const Timestamp & o) const
    {
        return Timestamp(m_Timestamp * o.m_Timestamp);
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Timestamp operator / (const Timestamp & o) const
    {
        return Timestamp(m_Timestamp / o.m_Timestamp);
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Int32 Cmp(const Timestamp & b) const;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    CSStr ToString() const;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetNow();

    /* --------------------------------------------------------------------------------------------
     *
    */
    SLongInt GetMicroseconds() const;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetMicroseconds(const SLongInt & ammount);

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQInteger GetMicrosecondsRaw() const
    {
        return SQInteger(m_Timestamp);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetMicrosecondsRaw(SQInteger ammount)
    {
        m_Timestamp = ammount;
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SLongInt GetMilliseconds() const;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetMilliseconds(const SLongInt & ammount);

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQInteger GetMillisecondsRaw() const
    {
        return SQInteger(m_Timestamp / 1000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetMillisecondsRaw(SQInteger ammount)
    {
        m_Timestamp = Int64(Int64(ammount) * 1000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQFloat GetSecondsF() const
    {
        return SQFloat(m_Timestamp / 1000000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetSecondsF(SQFloat ammount)
    {
        m_Timestamp = Int64(Float64(ammount) * 1000000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQInteger GetSecondsI() const
    {
        return SQInteger(m_Timestamp / 1000000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetSecondsI(SQInteger ammount)
    {
        m_Timestamp = Int64(Int64(ammount) * 1000000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQFloat GetMinutesF() const
    {
        return SQFloat(m_Timestamp / 60000000.0f);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetMinutesF(SQFloat ammount)
    {
        m_Timestamp = Int64(Float64(ammount) * 60000000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQInteger GetMinutesI() const
    {
        return SQInteger(m_Timestamp / 60000000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetMinutesI(SQInteger ammount)
    {
        m_Timestamp = Int64(Int64(ammount) * 60000000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQFloat GetHoursF() const
    {
        return SQFloat(m_Timestamp / 3600000000.0d);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetHoursF(SQFloat ammount)
    {
        m_Timestamp = Int64(Float64(ammount) * 3600000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQInteger GetHoursI() const
    {
        return SQInteger(m_Timestamp / 3600000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetHoursI(SQInteger ammount)
    {
        m_Timestamp = Int64(Float64(ammount) * 3600000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQFloat GetDaysF() const
    {
        return SQFloat(m_Timestamp / 86400000000.0d);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetDaysF(SQFloat ammount)
    {
        m_Timestamp = Int64(Float64(ammount) * 86400000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQInteger GetDaysI() const
    {
        return SQInteger(m_Timestamp / 86400000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetDaysI(SQInteger ammount)
    {
        m_Timestamp = Int64(Float64(ammount) * 86400000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQFloat GetYearsF() const
    {
        return SQFloat(m_Timestamp / 31557600000000.0d);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetYearsF(SQFloat ammount)
    {
        m_Timestamp = Int64(Float64(ammount) * 31557600000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQInteger GetYearsI() const
    {
        return SQInteger(m_Timestamp / 31557600000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetYearsI(SQInteger ammount)
    {
        m_Timestamp = Int64(Float64(ammount) * 31557600000000LL);
    }

private:

    // --------------------------------------------------------------------------------------------
    Int64   m_Timestamp;
};

/* ------------------------------------------------------------------------------------------------
 *
*/
class Timer
{
    /* --------------------------------------------------------------------------------------------
     *
    */
    Timer(Int64 t)
        : m_Timestamp(t)
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timer();

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timer(const Timer & o)
        : m_Timestamp(o.m_Timestamp)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    ~Timer()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timer & operator = (const Timer o)
    {
        m_Timestamp = o.m_Timestamp;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Int32 Cmp(const Timer & b) const;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    CSStr ToString() const;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void Reset();

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp Restart();

    /* --------------------------------------------------------------------------------------------
     *
    */
    Int64 RestartRaw();

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp GetElapsedTime() const;

    /* --------------------------------------------------------------------------------------------
     *
    */
    Int64 GetElapsedTimeRaw() const;

private:

    // --------------------------------------------------------------------------------------------
    Int64   m_Timestamp;
};

} // Namespace:: SqMod

#endif // _LIBRARY_TIMER_HPP_
