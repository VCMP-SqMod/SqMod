#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/Chrono.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
class Timer;

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
    explicit Timestamp(const SLongInt & t);

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
    SQMOD_NODISCARD SLongInt GetMicroseconds() const;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetMicroseconds(const SLongInt & amount);

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
    SQMOD_NODISCARD SLongInt GetMilliseconds() const;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetMilliseconds(const SLongInt & amount);

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
    SQMOD_NODISCARD SQFloat GetMinutesF() const
    {
        return SQFloat(m_Timestamp / 60000000.0);
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
    SQMOD_NODISCARD SQFloat GetHoursF() const
    {
        return SQFloat(m_Timestamp / 3600000000.0);
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
    SQMOD_NODISCARD SQFloat GetDaysF() const
    {
        return SQFloat(m_Timestamp / 86400000000.0);
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
    SQMOD_NODISCARD SQFloat GetYearsF() const
    {
        return SQFloat(m_Timestamp / 31557600000000.0);
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

private:

    // --------------------------------------------------------------------------------------------
    int64_t   m_Timestamp;
};

} // Namespace:: SqMod
