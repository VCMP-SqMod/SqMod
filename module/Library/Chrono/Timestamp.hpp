#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/Chrono.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
class Date;
class Time;
class Timer;
class Datetime;

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
    explicit Timestamp(int64_t t)
        : m_Timestamp(t)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp(const Timestamp & o) = default;

    /* --------------------------------------------------------------------------------------------
     *
    */
    ~Timestamp() = default;

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
    SQMOD_NODISCARD int32_t Cmp(const Timestamp & b) const;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQMOD_NODISCARD String ToString() const;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetNow();

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD int64_t GetNum() const
    {
        return m_Timestamp;
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD SQInteger GetMicroseconds() const;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetMicroseconds(SQInteger amount);

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddMicroseconds(SQInteger amount);
    Timestamp & SubMicroseconds(SQInteger amount);

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD SQInteger GetMicrosecondsRaw() const
    {
        return SQInteger(m_Timestamp);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetMicrosecondsRaw(SQInteger amount)
    {
        m_Timestamp = amount;
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddMicrosecondsRaw(SQInteger amount) { m_Timestamp += amount; return *this; }
    Timestamp & SubMicrosecondsRaw(SQInteger amount) { m_Timestamp -= amount; return *this; }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD SQInteger GetMilliseconds() const;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetMilliseconds(SQInteger amount);

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddMilliseconds(SQInteger amount);
    Timestamp & SubMilliseconds(SQInteger amount);

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD SQInteger GetMillisecondsRaw() const
    {
        return SQInteger(m_Timestamp / 1000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetMillisecondsRaw(SQInteger amount)
    {
        m_Timestamp = int64_t(int64_t(amount) * 1000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddMillisecondsRaw(SQInteger amount) { m_Timestamp += int64_t(int64_t(amount) * 1000L); return *this; }
    Timestamp & SubMillisecondsRaw(SQInteger amount) { m_Timestamp -= int64_t(int64_t(amount) * 1000L); return *this; }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD SQFloat GetSecondsF() const
    {
        return static_cast< SQFloat >(static_cast< int64_t >(m_Timestamp / 1000000LL));
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetSecondsF(SQFloat amount)
    {
        m_Timestamp = int64_t(double(amount) * 1000000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddSecondsF(SQFloat amount) { m_Timestamp += int64_t(double(amount) * 1000000L); return *this; }
    Timestamp & SubSecondsF(SQFloat amount) { m_Timestamp -= int64_t(double(amount) * 1000000L); return *this; }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD SQInteger GetSecondsI() const
    {
        return SQInteger(m_Timestamp / 1000000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetSecondsI(SQInteger amount)
    {
        m_Timestamp = int64_t(int64_t(amount) * 1000000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddSecondsI(SQInteger amount) { m_Timestamp += int64_t(int64_t(amount) * 1000000L); return *this; }
    Timestamp & SubSecondsI(SQInteger amount) { m_Timestamp -= int64_t(int64_t(amount) * 1000000L); return *this; }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD SQFloat GetMinutesF() const
    {
        return SQFloat(m_Timestamp) / 60000000.0;
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetMinutesF(SQFloat amount)
    {
        m_Timestamp = int64_t(double(amount) * 60000000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddMinutesF(SQFloat amount) { m_Timestamp += int64_t(double(amount) * 60000000L); return *this; }
    Timestamp & SubMinutesF(SQFloat amount) { m_Timestamp -= int64_t(double(amount) * 60000000L); return *this; }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD SQInteger GetMinutesI() const
    {
        return SQInteger(m_Timestamp / 60000000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetMinutesI(SQInteger amount)
    {
        m_Timestamp = int64_t(int64_t(amount) * 60000000L);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddMinutesI(SQInteger amount) { m_Timestamp += int64_t(int64_t(amount) * 60000000L); return *this; }
    Timestamp & SubMinutesI(SQInteger amount) { m_Timestamp -= int64_t(int64_t(amount) * 60000000L); return *this; }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD SQFloat GetHoursF() const
    {
        return SQFloat(m_Timestamp) / 3600000000.0;
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetHoursF(SQFloat amount)
    {
        m_Timestamp = int64_t(double(amount) * 3600000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddHoursF(SQFloat amount) { m_Timestamp += int64_t(double(amount) * 3600000000LL); return *this; }
    Timestamp & SubHoursF(SQFloat amount) { m_Timestamp -= int64_t(double(amount) * 3600000000LL); return *this; }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD SQInteger GetHoursI() const
    {
        return SQInteger(m_Timestamp / 3600000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetHoursI(SQInteger amount)
    {
        m_Timestamp = int64_t(double(amount) * 3600000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddHoursI(SQInteger amount) { m_Timestamp += int64_t(double(amount) * 3600000000LL); return *this; }
    Timestamp & SubHoursI(SQInteger amount) { m_Timestamp -= int64_t(double(amount) * 3600000000LL); return *this; }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD SQFloat GetDaysF() const
    {
        return SQFloat(m_Timestamp) / 86400000000.0;
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetDaysF(SQFloat amount)
    {
        m_Timestamp = int64_t(double(amount) * 86400000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddDaysF(SQFloat amount) { m_Timestamp += int64_t(double(amount) * 86400000000LL); return *this; }
    Timestamp & SubDaysF(SQFloat amount) { m_Timestamp -= int64_t(double(amount) * 86400000000LL); return *this; }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD SQInteger GetDaysI() const
    {
        return SQInteger(m_Timestamp / 86400000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetDaysI(SQInteger amount)
    {
        m_Timestamp = int64_t(double(amount) * 86400000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddDaysI(SQInteger amount) { m_Timestamp += int64_t(double(amount) * 86400000000LL); return *this; }
    Timestamp & SubDaysI(SQInteger amount) { m_Timestamp -= int64_t(double(amount) * 86400000000LL); return *this; }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD SQFloat GetYearsF() const
    {
        return SQFloat(m_Timestamp) / 31557600000000.0;
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetYearsF(SQFloat amount)
    {
        m_Timestamp = int64_t(double(amount) * 31557600000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddYearsF(SQFloat amount) { m_Timestamp += int64_t(double(amount) * 31557600000000LL); return *this; }
    Timestamp & SubYearsF(SQFloat amount) { m_Timestamp -= int64_t(double(amount) * 31557600000000LL); return *this; }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD SQInteger GetYearsI() const
    {
        return SQInteger(m_Timestamp / 31557600000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetYearsI(SQInteger amount)
    {
        m_Timestamp = int64_t(double(amount) * 31557600000000LL);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddYearsI(SQInteger amount) { m_Timestamp += int64_t(double(amount) * 31557600000000LL); return *this; }
    Timestamp & SubYearsI(SQInteger amount) { m_Timestamp -= int64_t(double(amount) * 31557600000000LL); return *this; }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD Time GetTime() const;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetTime(const Time & t);

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddTime(const Time & t);
    Timestamp & SubTime(const Time & t);

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD Date GetDate() const;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetDate(const Date & d);

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddDate(const Date & d);
    Timestamp & SubDate(const Date & d);

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD Datetime GetDatetime() const;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetDatetime(const Datetime & dt);

    /* --------------------------------------------------------------------------------------------
     *
    */
    Timestamp & AddDatetime(const Datetime & dt);
    Timestamp & SubDatetime(const Datetime & dt);

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQMOD_NODISCARD std::time_t ToTimeT() const;

private:

    // --------------------------------------------------------------------------------------------
    int64_t   m_Timestamp;
};

} // Namespace:: SqMod
