// ------------------------------------------------------------------------------------------------
#include "PocoLib/Time.hpp"

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
}

} // Namespace:: SqMod
