// ------------------------------------------------------------------------------------------------
#include "Library/Chrono/Timestamp.hpp"
#include "Library/Chrono/Timer.hpp"
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Time.hpp"
#include "Library/Chrono/Datetime.hpp"
#include "Library/Numeric/LongInt.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("SqTimestamp"))

// ------------------------------------------------------------------------------------------------
Timestamp::Timestamp(const SLongInt & t)
    : m_Timestamp(t.GetNum())
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Int32 Timestamp::Cmp(const Timestamp & o) const
{
    if (m_Timestamp == o.m_Timestamp)
    {
        return 0;
    }
    else if (m_Timestamp > o.m_Timestamp)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
CSStr Timestamp::ToString() const
{
    return ToStrF("%lld", m_Timestamp);
}

// ------------------------------------------------------------------------------------------------
void Timestamp::SetNow()
{
    m_Timestamp = Chrono::GetCurrentSysTime();
}

// ------------------------------------------------------------------------------------------------
SLongInt Timestamp::GetMicroseconds() const
{
    return SLongInt(m_Timestamp);
}

// ------------------------------------------------------------------------------------------------
void Timestamp::SetMicroseconds(const SLongInt & ammount)
{
    m_Timestamp = ammount.GetNum();
}

// ------------------------------------------------------------------------------------------------
SLongInt Timestamp::GetMilliseconds() const
{
    return SLongInt(m_Timestamp / 1000L);
}

// ------------------------------------------------------------------------------------------------
void Timestamp::SetMilliseconds(const SLongInt & ammount)
{
    m_Timestamp = (ammount.GetNum() * 1000L);
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetEpochTimeNow()
{
    return Timestamp(Chrono::GetEpochTimeMicro());
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetMicrosecondsRaw(Int64 ammount)
{
    return Timestamp(ammount);
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetMicroseconds(const SLongInt & ammount)
{
    return Timestamp(ammount);
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetMilliseconds(SQInteger ammount)
{
    return Timestamp(Int64(Int64(ammount) * 1000L));
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetSeconds(SQFloat ammount)
{
    return Timestamp(Int64(Float64(ammount) * 1000000L));
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetMinutes(SQFloat ammount)
{
    return Timestamp(Int64((Float64(ammount) * 60000000L)));
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetHours(SQFloat ammount)
{
    return Timestamp(Int64(Float64(ammount) * 3600000000LL));
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetDays(SQFloat ammount)
{
    return Timestamp(Int64(Float64(ammount) * 86400000000LL));
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetYears(SQFloat ammount)
{
    return Timestamp(Int64(Float64(ammount) * 31557600000000LL));
}

// ================================================================================================
void Register_ChronoTimestamp(HSQUIRRELVM vm, Table & /*cns*/)
{
    RootTable(vm).Bind(Typename::Str,
        Class< Timestamp >(vm, Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< const Timestamp & >()
        // Core Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Timestamp::ToString)
        .Func(_SC("cmp"), &Timestamp::Cmp)
        // Meta-methods
        .Func< Timestamp (Timestamp::*)(const Timestamp &) const >(_SC("_add"), &Timestamp::operator +)
        .Func< Timestamp (Timestamp::*)(const Timestamp &) const >(_SC("_sub"), &Timestamp::operator -)
        .Func< Timestamp (Timestamp::*)(const Timestamp &) const >(_SC("_mul"), &Timestamp::operator *)
        .Func< Timestamp (Timestamp::*)(const Timestamp &) const >(_SC("_div"), &Timestamp::operator /)
        // Properties
        .Prop(_SC("Microseconds"), &Timestamp::GetMicroseconds, &Timestamp::SetMicroseconds)
        .Prop(_SC("MicrosecondsRaw"), &Timestamp::GetMicrosecondsRaw, &Timestamp::SetMicrosecondsRaw)
        .Prop(_SC("Milliseconds"), &Timestamp::GetMilliseconds, &Timestamp::SetMilliseconds)
        .Prop(_SC("MillisecondsRaw"), &Timestamp::GetMillisecondsRaw, &Timestamp::SetMillisecondsRaw)
        .Prop(_SC("SecondsF"), &Timestamp::GetSecondsF, &Timestamp::SetSecondsF)
        .Prop(_SC("SecondsI"), &Timestamp::GetSecondsI, &Timestamp::SetSecondsI)
        .Prop(_SC("MinutesF"), &Timestamp::GetMinutesF, &Timestamp::SetMinutesF)
        .Prop(_SC("MinutesI"), &Timestamp::GetMinutesI, &Timestamp::SetMinutesI)
        .Prop(_SC("HoursF"), &Timestamp::GetHoursF, &Timestamp::SetHoursF)
        .Prop(_SC("HoursI"), &Timestamp::GetHoursI, &Timestamp::SetHoursI)
        .Prop(_SC("DaysF"), &Timestamp::GetDaysF, &Timestamp::SetDaysF)
        .Prop(_SC("DaysI"), &Timestamp::GetDaysI, &Timestamp::SetDaysI)
        .Prop(_SC("YearsF"), &Timestamp::GetYearsF, &Timestamp::SetYearsF)
        .Prop(_SC("YearsI"), &Timestamp::GetYearsI, &Timestamp::SetYearsI)
        // Member Methods
        .Func(_SC("SetNow"), &Timestamp::SetNow)
        // Static Functions
        .StaticFunc(_SC("GetNow"), &SqGetEpochTimeNow)
        .StaticFunc(_SC("GetMicrosRaw"), &SqGetMicrosecondsRaw)
        .StaticFunc(_SC("GetMicrosecondsRaw"), &SqGetMicrosecondsRaw)
        .StaticFunc(_SC("GetMicros"), &SqGetMicroseconds)
        .StaticFunc(_SC("GetMicroseconds"), &SqGetMicroseconds)
        .StaticFunc(_SC("GetMillis"), &SqGetMilliseconds)
        .StaticFunc(_SC("GetMilliseconds"), &SqGetMilliseconds)
        .StaticFunc(_SC("GetSeconds"), &SqGetSeconds)
        .StaticFunc(_SC("GetMinutes"), &SqGetMinutes)
        .StaticFunc(_SC("GetHours"), &SqGetHours)
        .StaticFunc(_SC("GetDays"), &SqGetDays)
        .StaticFunc(_SC("GetYears"), &SqGetYears)
    );
;
}

} // Namespace:: SqMod
