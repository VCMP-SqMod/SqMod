// ------------------------------------------------------------------------------------------------
#include "PocoLib/Time.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqClock, _SC("SqClock"))
SQMOD_DECL_TYPENAME(SqDateTime, _SC("SqDateTime"))
SQMOD_DECL_TYPENAME(SqDateTimeFormatter, _SC("SqDateTimeFormatter"))
SQMOD_DECL_TYPENAME(SqDateTimeParser, _SC("SqDateTimeParser"))
SQMOD_DECL_TYPENAME(SqLocalDateTime, _SC("SqLocalDateTime"))
SQMOD_DECL_TYPENAME(SqStopwatch, _SC("SqStopwatch"))
SQMOD_DECL_TYPENAME(SqTimespan, _SC("SqTimespan"))
SQMOD_DECL_TYPENAME(SqTimestamp, _SC("SqTimestamp"))
SQMOD_DECL_TYPENAME(SqTimezone, _SC("SqTimezone"))

// ================================================================================================
void Register_POCO_Time(HSQUIRRELVM vm, Table & ns)
{
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("DateTime"),
        Class< DateTime >(vm, SqDateTime::Str)
        // Constructors
        .Ctor()
        .Ctor< const Timestamp & >()
        .Ctor< int, int, int, int, int, int, int, int >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDateTime::Fn)
        // Properties
        .Prop(_SC("Year"), &DateTime::year)
        .Prop(_SC("Month"), &DateTime::month)
        .Prop(_SC("Week"), &DateTime::week)
        .Prop(_SC("Day"), &DateTime::day)
        .Prop(_SC("DayOfWeek"), &DateTime::dayOfWeek)
        .Prop(_SC("DayOfYear"), &DateTime::dayOfYear)
        .Prop(_SC("Hour"), &DateTime::hour)
        .Prop(_SC("HourAMPM"), &DateTime::hourAMPM)
        .Prop(_SC("IsAM"), &DateTime::isAM)
        .Prop(_SC("IsPM"), &DateTime::isPM)
        .Prop(_SC("Minute"), &DateTime::minute)
        .Prop(_SC("Second"), &DateTime::second)
        .Prop(_SC("Millisecond"), &DateTime::millisecond)
        .Prop(_SC("Microsecond"), &DateTime::microsecond)
        .Prop(_SC("JulianDay"), &DateTime::julianDay)
        .Prop(_SC("TimeStamp"), &DateTime::timestamp)
        .Prop(_SC("UtcTime"), &DateTime::utcTime)
        // Member Methods
        .Func(_SC("Assign"), &DateTime::assign)
        .Func(_SC("Swap"), &DateTime::swap)
        .Func(_SC("GetWeek"), &DateTime::week)
        .Func(_SC("MakeUTC"), &DateTime::makeUTC)
        .Func(_SC("MakeLocal"), &DateTime::makeLocal)
        // Static method
        .StaticFunc(_SC("IsLeapYear"), &DateTime::isLeapYear)
        .StaticFunc(_SC("DaysOfMonth"), &DateTime::daysOfMonth)
        .StaticFunc(_SC("IsValid"), &DateTime::isValid)
    );

    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Timespan"),
        Class< Timespan >(vm, SqTimespan::Str)
        // Constructors
        .Ctor()
        .Ctor< const Timespan & >()
        .Ctor< long, long >()
        .Ctor< int, int, int, int, int >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqTimespan::Fn)
        // Properties
        .Prop(_SC("Days"), &Timespan::days)
        .Prop(_SC("Hours"), &Timespan::hours)
        .Prop(_SC("TotalHours"), &Timespan::totalHours)
        .Prop(_SC("Minutes"), &Timespan::minutes)
        .Prop(_SC("TotalMinutes"), &Timespan::totalMinutes)
        .Prop(_SC("Seconds"), &Timespan::seconds)
        .Prop(_SC("TotalSeconds"), &Timespan::totalSeconds)
        .Prop(_SC("Milliseconds"), &Timespan::milliseconds)
        .Prop(_SC("TotalMilliseconds"), &Timespan::totalMilliseconds)
        .Prop(_SC("Microseconds"), &Timespan::microseconds)
        .Prop(_SC("Useconds"), &Timespan::useconds)
        .Prop(_SC("TotalMicroseconds"), &Timespan::totalMicroseconds)
        // Member Methods
        .FmtFunc(_SC("Swap"), &Timespan::swap)
        // Member Overloads
        .Overload< Timespan & (Timespan::*)(long, long) >
            (_SC("Assign"), &Timespan::assign)
        .Overload< Timespan & (Timespan::*)(int, int, int, int, int) >
            (_SC("Assign"), &Timespan::assign)
        // Static Values
        .SetStaticValue(_SC("MILLISECONDS"), static_cast< SQInteger >(Timespan::MILLISECONDS))
        .SetStaticValue(_SC("SECONDS"), static_cast< SQInteger >(Timespan::SECONDS))
        .SetStaticValue(_SC("MINUTES"), static_cast< SQInteger >(Timespan::MINUTES))
        .SetStaticValue(_SC("HOURS"), static_cast< SQInteger >(Timespan::HOURS))
        .SetStaticValue(_SC("DAYS"), static_cast< SQInteger >(Timespan::DAYS))
    );

    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqMonth"), Enumeration(vm)
        .Const(_SC("January"),      static_cast< SQInteger >(Poco::DateTime::JANUARY))
        .Const(_SC("February"),     static_cast< SQInteger >(Poco::DateTime::FEBRUARY))
        .Const(_SC("March"),        static_cast< SQInteger >(Poco::DateTime::MARCH))
        .Const(_SC("April"),        static_cast< SQInteger >(Poco::DateTime::APRIL))
        .Const(_SC("May"),          static_cast< SQInteger >(Poco::DateTime::MAY))
        .Const(_SC("June"),         static_cast< SQInteger >(Poco::DateTime::JUNE))
        .Const(_SC("July"),         static_cast< SQInteger >(Poco::DateTime::JULY))
        .Const(_SC("August"),       static_cast< SQInteger >(Poco::DateTime::AUGUST))
        .Const(_SC("September"),    static_cast< SQInteger >(Poco::DateTime::SEPTEMBER))
        .Const(_SC("October"),      static_cast< SQInteger >(Poco::DateTime::OCTOBER))
        .Const(_SC("November"),     static_cast< SQInteger >(Poco::DateTime::NOVEMBER))
        .Const(_SC("December"),     static_cast< SQInteger >(Poco::DateTime::DECEMBER))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqDayOfWeek"), Enumeration(vm)
        .Const(_SC("Sunday"),       static_cast< SQInteger >(Poco::DateTime::SUNDAY))
        .Const(_SC("Monday"),       static_cast< SQInteger >(Poco::DateTime::MONDAY))
        .Const(_SC("Tuesday"),      static_cast< SQInteger >(Poco::DateTime::TUESDAY))
        .Const(_SC("Wednesday"),    static_cast< SQInteger >(Poco::DateTime::WEDNESDAY))
        .Const(_SC("Thursday"),     static_cast< SQInteger >(Poco::DateTime::THURSDAY))
        .Const(_SC("Friday"),       static_cast< SQInteger >(Poco::DateTime::FRIDAY))
        .Const(_SC("Saturday"),     static_cast< SQInteger >(Poco::DateTime::SATURDAY))
    );
}

} // Namespace:: SqMod
