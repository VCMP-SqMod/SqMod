// ------------------------------------------------------------------------------------------------
#include "Library/Chrono/Timestamp.hpp"
#include "Library/Chrono/Timer.hpp"
#include "Library/Chrono/Time.hpp"
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Datetime.hpp"

// ------------------------------------------------------------------------------------------------
#include <chrono>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("SqTimestamp"))

// ------------------------------------------------------------------------------------------------
int32_t Timestamp::Cmp(const Timestamp & o) const
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
String Timestamp::ToString() const
{
    return fmt::format("{}", m_Timestamp);
}

// ------------------------------------------------------------------------------------------------
void Timestamp::SetNow()
{
    m_Timestamp = Chrono::GetCurrentSysTime();
}

// ------------------------------------------------------------------------------------------------
SQInteger Timestamp::GetMicroseconds() const
{
    return m_Timestamp;
}

// ------------------------------------------------------------------------------------------------
void Timestamp::SetMicroseconds(SQInteger amount)
{
    m_Timestamp = amount;
}

// ------------------------------------------------------------------------------------------------
Timestamp & Timestamp::AddMicroseconds(SQInteger amount) { m_Timestamp += amount; return *this; }
Timestamp & Timestamp::SubMicroseconds(SQInteger amount) { m_Timestamp -= amount; return *this; }

// ------------------------------------------------------------------------------------------------
    SQInteger Timestamp::GetMilliseconds() const
{
    return m_Timestamp / 1000L;
}

// ------------------------------------------------------------------------------------------------
void Timestamp::SetMilliseconds(SQInteger amount)
{
    m_Timestamp = (amount * 1000L);
}

// ------------------------------------------------------------------------------------------------
Timestamp & Timestamp::AddMilliseconds(SQInteger amount) { m_Timestamp += (amount * 1000L); return *this; }
Timestamp & Timestamp::SubMilliseconds(SQInteger amount) { m_Timestamp -= (amount * 1000L); return *this; }

// ------------------------------------------------------------------------------------------------
Time Timestamp::GetTime() const
{
    return Time(m_Timestamp);
}

// ------------------------------------------------------------------------------------------------
void Timestamp::SetTime(const Time & t)
{
    SetHoursI(t.GetHour());
    AddMinutesI(t.GetMinute());
    AddSecondsI(t.GetSecond());
    AddMillisecondsRaw(t.GetMillisecond());
}

// ------------------------------------------------------------------------------------------------
Timestamp & Timestamp::AddTime(const Time & t)
{
    AddHoursI(t.GetHour());
    AddMinutesI(t.GetMinute());
    AddSecondsI(t.GetSecond());
    AddMillisecondsRaw(t.GetMillisecond());
    return *this;
}

// ------------------------------------------------------------------------------------------------
Timestamp & Timestamp::SubTime(const Time & t)
{
    SubMillisecondsRaw(t.GetMillisecond());
    SubSecondsI(t.GetSecond());
    SubMinutesI(t.GetMinute());
    SubHoursI(t.GetHour());
    return *this;
}

// ------------------------------------------------------------------------------------------------
Date Timestamp::GetDate() const
{
    return Date(m_Timestamp);
}

// ------------------------------------------------------------------------------------------------
void Timestamp::SetDate(const Date & d)
{
    SetYearsI(d.GetYear());
    AddDaysF(d.GetMonth() * 30.4167);
    AddDaysI(d.GetDay());
}

// ------------------------------------------------------------------------------------------------
Timestamp & Timestamp::AddDate(const Date & d)
{
    AddYearsI(d.GetYear());
    AddDaysF(d.GetMonth() * 30.4167);
    AddDaysI(d.GetDay());
    return *this;
}

// ------------------------------------------------------------------------------------------------
Timestamp & Timestamp::SubDate(const Date & d)
{
    SubDaysI(d.GetDay());
    SubDaysF(d.GetMonth() * 30.4167);
    SubYearsI(d.GetYear());
    return *this;
}

// ------------------------------------------------------------------------------------------------
Datetime Timestamp::GetDatetime() const
{
    return Datetime(m_Timestamp);
}

// ------------------------------------------------------------------------------------------------
void Timestamp::SetDatetime(const Datetime & dt)
{
    SetYearsI(dt.GetYear());
    AddDaysF(dt.GetMonth() * 30.4167);
    AddDaysI(dt.GetDay());
    AddHoursI(dt.GetHour());
    AddMinutesI(dt.GetMinute());
    AddSecondsI(dt.GetSecond());
    AddMillisecondsRaw(dt.GetMillisecond());
}

// ------------------------------------------------------------------------------------------------
Timestamp & Timestamp::AddDatetime(const Datetime & dt)
{
    AddYearsI(dt.GetYear());
    AddDaysF(dt.GetMonth() * 30.4167);
    AddDaysI(dt.GetDay());
    AddHoursI(dt.GetHour());
    AddMinutesI(dt.GetMinute());
    AddSecondsI(dt.GetSecond());
    AddMillisecondsRaw(dt.GetMillisecond());
    return *this;
}

// ------------------------------------------------------------------------------------------------
Timestamp & Timestamp::SubDatetime(const Datetime & dt)
{
    SubMillisecondsRaw(dt.GetMillisecond());
    SubSecondsI(dt.GetSecond());
    SubMinutesI(dt.GetMinute());
    SubHoursI(dt.GetHour());
    SubDaysI(dt.GetDay());
    SubDaysF(dt.GetMonth() * 30.4167);
    SubYearsI(dt.GetYear());
    return *this;
}

// ------------------------------------------------------------------------------------------------
std::time_t Timestamp::ToTimeT() const
{
    return std::chrono::system_clock::to_time_t(std::chrono::time_point< std::chrono::system_clock >{std::chrono::microseconds{m_Timestamp}});
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetEpochTimeNow()
{
    return Timestamp(Chrono::GetEpochTimeMicro());
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetMicrosecondsRaw(int64_t amount)
{
    return Timestamp(amount);
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetMicroseconds(SQInteger amount)
{
    return Timestamp(amount);
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetMilliseconds(SQInteger amount)
{
    return Timestamp(int64_t(int64_t(amount) * 1000L));
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetSeconds(SQFloat amount)
{
    return Timestamp(int64_t(double(amount) * 1000000L));
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetMinutes(SQFloat amount)
{
    return Timestamp(int64_t((double(amount) * 60000000L)));
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetHours(SQFloat amount)
{
    return Timestamp(int64_t(double(amount) * 3600000000LL));
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetDays(SQFloat amount)
{
    return Timestamp(int64_t(double(amount) * 86400000000LL));
}

// ------------------------------------------------------------------------------------------------
static Timestamp SqGetYears(SQFloat amount)
{
    return Timestamp(int64_t(double(amount) * 31557600000000LL));
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
        .Prop(_SC("Time"), &Timestamp::GetTime, &Timestamp::SetTime)
        .Prop(_SC("Date"), &Timestamp::GetDate, &Timestamp::SetDate)
        .Prop(_SC("Datetime"), &Timestamp::GetDatetime, &Timestamp::SetDatetime)
        // Member Methods
        .Func(_SC("SetNow"), &Timestamp::SetNow)
        // Properties
        .Func(_SC("AddMicroseconds"), &Timestamp::AddMicroseconds)
        .Func(_SC("SubMicroseconds"), &Timestamp::SubMicroseconds)
        .Func(_SC("AddMicrosecondsRaw"), &Timestamp::AddMicrosecondsRaw)
        .Func(_SC("SubMicrosecondsRaw"), &Timestamp::SubMicrosecondsRaw)
        .Func(_SC("AddMilliseconds"), &Timestamp::AddMilliseconds)
        .Func(_SC("SubMilliseconds"), &Timestamp::SubMilliseconds)
        .Func(_SC("AddMillisecondsRaw"), &Timestamp::AddMillisecondsRaw)
        .Func(_SC("SubMillisecondsRaw"), &Timestamp::SubMillisecondsRaw)
        .Func(_SC("AddSecondsF"), &Timestamp::AddSecondsF)
        .Func(_SC("SubSecondsF"), &Timestamp::SubSecondsF)
        .Func(_SC("AddSecondsI"), &Timestamp::AddSecondsI)
        .Func(_SC("SubSecondsI"), &Timestamp::SubSecondsI)
        .Func(_SC("AddMinutesF"), &Timestamp::AddMinutesF)
        .Func(_SC("SubMinutesF"), &Timestamp::SubMinutesF)
        .Func(_SC("AddMinutesI"), &Timestamp::AddMinutesI)
        .Func(_SC("SubMinutesI"), &Timestamp::SubMinutesI)
        .Func(_SC("AddHoursF"), &Timestamp::AddHoursF)
        .Func(_SC("SubHoursF"), &Timestamp::SubHoursF)
        .Func(_SC("AddHoursI"), &Timestamp::AddHoursI)
        .Func(_SC("SubHoursI"), &Timestamp::SubHoursI)
        .Func(_SC("AddDaysF"), &Timestamp::AddDaysF)
        .Func(_SC("SubDaysF"), &Timestamp::SubDaysF)
        .Func(_SC("AddDaysI"), &Timestamp::AddDaysI)
        .Func(_SC("SubDaysI"), &Timestamp::SubDaysI)
        .Func(_SC("AddYearsF"), &Timestamp::AddYearsF)
        .Func(_SC("SubYearsF"), &Timestamp::SubYearsF)
        .Func(_SC("AddYearsI"), &Timestamp::AddYearsI)
        .Func(_SC("SubYearsI"), &Timestamp::SubYearsI)
        .Func(_SC("AddTime"), &Timestamp::AddTime)
        .Func(_SC("SubTime"), &Timestamp::SubTime)
        .Func(_SC("AddDate"), &Timestamp::AddDate)
        .Func(_SC("SubDate"), &Timestamp::SubDate)
        .Func(_SC("AddDatetime"), &Timestamp::AddDatetime)
        .Func(_SC("SubDatetime"), &Timestamp::SubDatetime)
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
}

} // Namespace:: SqMod
