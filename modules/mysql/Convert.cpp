// ------------------------------------------------------------------------------------------------
#include "Convert.hpp"

// ------------------------------------------------------------------------------------------------
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CSStr SqMySQLTypename(enum_field_types type)
{
    switch(type)
    {
        case MYSQL_TYPE_DECIMAL:        return _SC("decimal");
        case MYSQL_TYPE_TINY:           return _SC("tiny");
        case MYSQL_TYPE_SHORT:          return _SC("short");
        case MYSQL_TYPE_LONG:           return _SC("long");
        case MYSQL_TYPE_FLOAT:          return _SC("float");
        case MYSQL_TYPE_DOUBLE:         return _SC("double");
        case MYSQL_TYPE_NULL:           return _SC("null");
        case MYSQL_TYPE_TIMESTAMP:      return _SC("time-stamp");
        case MYSQL_TYPE_LONGLONG:       return _SC("long-long");
        case MYSQL_TYPE_INT24:          return _SC("int24");
        case MYSQL_TYPE_DATE:           return _SC("date");
        case MYSQL_TYPE_TIME:           return _SC("time");
        case MYSQL_TYPE_DATETIME:       return _SC("date-time");
        case MYSQL_TYPE_YEAR:           return _SC("year");
        case MYSQL_TYPE_NEWDATE:        return _SC("new-date");
        case MYSQL_TYPE_VARCHAR:        return _SC("var-char");
        case MYSQL_TYPE_BIT:            return _SC("bit");
#ifdef MYSQL_TYPE_TIMESTAMP2
        case MYSQL_TYPE_TIMESTAMP2:     return _SC("time-stamp2");
#endif // MYSQL_TYPE_TIMESTAMP2
#ifdef MYSQL_TYPE_DATETIME2
        case MYSQL_TYPE_DATETIME2:      return _SC("date-time2");
#endif // MYSQL_TYPE_DATETIME2
#ifdef MYSQL_TYPE_TIME2
        case MYSQL_TYPE_TIME2:          return _SC("time2");
#endif // MYSQL_TYPE_TIME2
        case MYSQL_TYPE_NEWDECIMAL:     return _SC("new-decimal");
        case MYSQL_TYPE_ENUM:           return _SC("enum");
        case MYSQL_TYPE_SET:            return _SC("set");
        case MYSQL_TYPE_TINY_BLOB:      return _SC("tiny-blob");
        case MYSQL_TYPE_MEDIUM_BLOB:    return _SC("medium-blob");
        case MYSQL_TYPE_LONG_BLOB:      return _SC("long-blob");
        case MYSQL_TYPE_BLOB:           return _SC("blob");
        case MYSQL_TYPE_VAR_STRING:     return _SC("var-string");
        case MYSQL_TYPE_STRING:         return _SC("string");
        case MYSQL_TYPE_GEOMETRY:       return _SC("geometry");
        default:                        return _SC("unknown");
    }
}

// ------------------------------------------------------------------------------------------------
CSStr SqMySQLTypenameC(enum_field_types type)
{
    switch(type)
    {
        case MYSQL_TYPE_DECIMAL:        return _SC("Decimal");
        case MYSQL_TYPE_TINY:           return _SC("Tiny");
        case MYSQL_TYPE_SHORT:          return _SC("Short");
        case MYSQL_TYPE_LONG:           return _SC("Long");
        case MYSQL_TYPE_FLOAT:          return _SC("Float");
        case MYSQL_TYPE_DOUBLE:         return _SC("Double");
        case MYSQL_TYPE_NULL:           return _SC("Null");
        case MYSQL_TYPE_TIMESTAMP:      return _SC("Time-Stamp");
        case MYSQL_TYPE_LONGLONG:       return _SC("Long-Long");
        case MYSQL_TYPE_INT24:          return _SC("Int24");
        case MYSQL_TYPE_DATE:           return _SC("Date");
        case MYSQL_TYPE_TIME:           return _SC("Time");
        case MYSQL_TYPE_DATETIME:       return _SC("Date-Time");
        case MYSQL_TYPE_YEAR:           return _SC("Year");
        case MYSQL_TYPE_NEWDATE:        return _SC("New-Date");
        case MYSQL_TYPE_VARCHAR:        return _SC("Var-Char");
        case MYSQL_TYPE_BIT:            return _SC("Bit");
#ifdef MYSQL_TYPE_TIMESTAMP2
        case MYSQL_TYPE_TIMESTAMP2:     return _SC("Time-Stamp2");
#endif // MYSQL_TYPE_TIMESTAMP2
#ifdef MYSQL_TYPE_DATETIME2
        case MYSQL_TYPE_DATETIME2:      return _SC("Date-Time2");
#endif // MYSQL_TYPE_DATETIME2
#ifdef MYSQL_TYPE_TIME2
        case MYSQL_TYPE_TIME2:          return _SC("Time2");
#endif // MYSQL_TYPE_TIME2
        case MYSQL_TYPE_NEWDECIMAL:     return _SC("New-Decimal");
        case MYSQL_TYPE_ENUM:           return _SC("Enum");
        case MYSQL_TYPE_SET:            return _SC("Set");
        case MYSQL_TYPE_TINY_BLOB:      return _SC("Tiny-Blob");
        case MYSQL_TYPE_MEDIUM_BLOB:    return _SC("Medium-Blob");
        case MYSQL_TYPE_LONG_BLOB:      return _SC("Long-Blob");
        case MYSQL_TYPE_BLOB:           return _SC("Blob");
        case MYSQL_TYPE_VAR_STRING:     return _SC("Var-String");
        case MYSQL_TYPE_STRING:         return _SC("String");
        case MYSQL_TYPE_GEOMETRY:       return _SC("Geometry");
        default:                        return _SC("Unknown");
    }
}

// ------------------------------------------------------------------------------------------------
static inline Int64 MySQLDateStrToSeconds(CSStr value)
{
    Uint32 y = 1000, m = 1, d = 1;
    // Attempt to extract the numeric values from the string
    std::sscanf(value, "%u - %u - %u", &y, &m, &d);
    // Calculate the number of seconds and return it
    return SqMod_DateRangeToSeconds(1000, 1, 1, y, m, d);
}

// ------------------------------------------------------------------------------------------------
static inline Int64 MySQLTimeStrToSeconds(CSStr value)
{
    Int32 h = 0, m = 0, s = 0;
    // Attempt to extract the numeric values from the string
    std::sscanf(value, "%d : %d : %d", &h, &m, &s);
    // Convert the hours to seconds
    h *= (60 * 60);
    // Add the remaining minutes and seconds and return the result
    return (h < 0) ? (h - ((m * 60) + s)) : (h + ((m * 60) + s));
}

// ------------------------------------------------------------------------------------------------
static inline Int64 MySQLDatetimeStrToSeconds(CSStr value)
{
    Uint32 y = 1000, mo = 1, d = 1, h = 0, mi = 0, s = 0;
    // Attempt to extract the numeric values from the string
    std::sscanf(value, "%u - %u - %u %u : %u : %u", &y, &mo, &d, &h, &mi, &s);
    // Calculate the number of seconds and return it
    return SqMod_DateRangeToSeconds(1000, 1, 1, y, mo, d) + (h * (60 * 60)) + ((mi * 60) + s);
}

// ------------------------------------------------------------------------------------------------
static inline Int64 MySQLTimestampStrToSeconds(CSStr value)
{
    Uint32 y = 1000, mo = 1, d = 1, h = 0, mi = 0, s = 0;
    // Attempt to extract the numeric values from the string
    std::sscanf(value, "%u - %u - %u %u : %u : %u", &y, &mo, &d, &h, &mi, &s);
    // Detect if this was time-stamp 0
    if (!y && !mo && !d && !h && !mi && !s)
    {
        return 0;
    }
    // Calculate the number of seconds and return it
    return SqMod_DateRangeToSeconds(1970, 1, 1, y, mo, d) + (h * (60 * 60)) + ((mi * 60) + s);
}

// ------------------------------------------------------------------------------------------------
template < typename T > static inline T MemToNum(const Uint8 * b, Ulong l)
{
    union
    {
        Uint8   a[8];
        T       n;
    };

    n = 0ULL;

    switch (l)
    {
        default:
        case 8:
        {
            a[7] = b[0];
            a[6] = b[1];
            a[5] = b[2];
            a[4] = b[3];
            a[3] = b[4];
            a[2] = b[5];
            a[1] = b[6];
            a[0] = b[7];
        } break;
        case 7:
        {
            a[6] = b[0];
            a[5] = b[1];
            a[4] = b[2];
            a[3] = b[3];
            a[2] = b[4];
            a[1] = b[5];
            a[0] = b[6];
        } break;
        case 6:
        {
            a[5] = b[0];
            a[4] = b[1];
            a[3] = b[2];
            a[2] = b[3];
            a[1] = b[4];
            a[0] = b[5];
        } break;
        case 5:
        {
            a[4] = b[0];
            a[3] = b[1];
            a[2] = b[2];
            a[1] = b[3];
            a[0] = b[4];
        } break;
        case 4:
        {
            a[3] = b[0];
            a[2] = b[1];
            a[1] = b[2];
            a[0] = b[3];
        } break;
        case 3:
        {
            a[2] = b[0];
            a[1] = b[1];
            a[0] = b[2];
        } break;
        case 2:
        {
            a[1] = b[0];
            a[0] = b[1];
        } break;
        case 1:
        {
            a[0] = b[0];
        } break;
        case 0: break;
    }

    return n;
}

// ------------------------------------------------------------------------------------------------
template < typename T >
static inline T ConvertToSInt(CSStr value, Ulong length, enum_field_types type, CSStr tn)
{
    // Is there even a value to attempt to extract?
    if (!value || *value == '\0')
    {
        return 0;
    }
    // Identify the type of value that must be converted and try to at least approximate something
    switch(type)
    {
        case MYSQL_TYPE_NULL:
        case MYSQL_TYPE_GEOMETRY:       return static_cast< T >(0);
        case MYSQL_TYPE_BIT:
        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_TINY:
        case MYSQL_TYPE_SHORT:
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:           return ConvTo< T >::From(std::strtol(value, nullptr, 10));
        case MYSQL_TYPE_LONGLONG:
        case MYSQL_TYPE_VARCHAR:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_ENUM:
        case MYSQL_TYPE_SET:            return ConvTo< T >::From(std::strtoll(value, nullptr, 10));
        case MYSQL_TYPE_FLOAT:          return ConvTo< T >::From(std::strtof(value, nullptr));
        case MYSQL_TYPE_DOUBLE:
        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL:     return ConvTo< T >::From(std::strtod(value, nullptr));
        case MYSQL_TYPE_DATE:
        case MYSQL_TYPE_NEWDATE:        return ConvTo< T >::From(MySQLDateStrToSeconds(value));
        case MYSQL_TYPE_TIME:           return ConvTo< T >::From(MySQLTimeStrToSeconds(value));
        case MYSQL_TYPE_TIMESTAMP:      return ConvTo< T >::From(MySQLTimestampStrToSeconds(value));
        case MYSQL_TYPE_DATETIME:       return ConvTo< T >::From(MySQLDatetimeStrToSeconds(value));
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_BLOB:           return MemToNum< T >(reinterpret_cast< const Uint8 * >(value), length);
        default:
        {
            STHROWF("Unknown conversion from (%s) to (%s)", SqMySQLTypenameC(type), tn);
        } break;
    }
    // Should not reach this point!
    return 0;
}

// ------------------------------------------------------------------------------------------------
template < typename T >
static inline T ConvertToUInt(CSStr value, Ulong length, enum_field_types type, CSStr tn)
{
    // Is there even a value to attempt to extract?
    if (!value || *value == '\0')
    {
        return 0;
    }
    // Identify the type of value that must be converted and try to at least approximate something
    switch(type)
    {
        case MYSQL_TYPE_NULL:
        case MYSQL_TYPE_GEOMETRY:       return static_cast< T >(0);
        case MYSQL_TYPE_BIT:
        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_TINY:
        case MYSQL_TYPE_SHORT:
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:           return ConvTo< T >::From(std::strtoul(value, nullptr, 10));
        case MYSQL_TYPE_LONGLONG:
        case MYSQL_TYPE_VARCHAR:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_ENUM:
        case MYSQL_TYPE_SET:            return ConvTo< T >::From(std::strtoull(value, nullptr, 10));
        case MYSQL_TYPE_FLOAT:          return ConvTo< T >::From(std::strtof(value, nullptr));
        case MYSQL_TYPE_DOUBLE:
        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL:     return ConvTo< T >::From(std::strtod(value, nullptr));
        case MYSQL_TYPE_DATE:
        case MYSQL_TYPE_NEWDATE:        return ConvTo< T >::From(MySQLDateStrToSeconds(value));
        case MYSQL_TYPE_TIME:           return ConvTo< T >::From(MySQLTimeStrToSeconds(value));
        case MYSQL_TYPE_TIMESTAMP:      return ConvTo< T >::From(MySQLTimestampStrToSeconds(value));
        case MYSQL_TYPE_DATETIME:       return ConvTo< T >::From(MySQLDatetimeStrToSeconds(value));
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_BLOB:           return MemToNum< T >(reinterpret_cast< const Uint8 * >(value), length);
        default:
        {
            STHROWF("Unknown conversion from (%s) to (%s)", SqMySQLTypenameC(type), tn);
        } break;
    }
    // Should not reach this point!
    return 0;
}

// ------------------------------------------------------------------------------------------------
Int8 DbConvTo< Int8 >::From(CSStr value, Ulong length, enum_field_types type, CSStr tn)
{
    return ConvertToSInt< Int8 >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
Uint8 DbConvTo< Uint8 >::From(CSStr value, Ulong length, enum_field_types type, CSStr tn)
{
    return ConvertToUInt< Uint8 >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
Int16 DbConvTo< Int16 >::From(CSStr value, Ulong length, enum_field_types type, CSStr tn)
{
    return ConvertToSInt< Int16 >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
Uint16 DbConvTo< Uint16 >::From(CSStr value, Ulong length, enum_field_types type, CSStr tn)
{
    return ConvertToUInt< Uint16 >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
Int32 DbConvTo< Int32 >::From(CSStr value, Ulong length, enum_field_types type, CSStr tn)
{
    return ConvertToSInt< Int32 >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
Uint32 DbConvTo< Uint32 >::From(CSStr value, Ulong length, enum_field_types type, CSStr tn)
{
    return ConvertToUInt< Uint32 >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
Int64 DbConvTo< Int64 >::From(CSStr value, Ulong length, enum_field_types type, CSStr tn)
{
    return ConvertToSInt< Int64 >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
Uint64 DbConvTo< Uint64 >::From(CSStr value, Ulong length, enum_field_types type, CSStr tn)
{
    return ConvertToUInt< Uint64 >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
Float32 DbConvTo< Float32 >::From(CSStr value, Ulong length, enum_field_types type, CSStr tn)
{
    // Is there even a value to attempt to extract?
    if (!value || *value == '\0')
    {
        return 0;
    }
    // Identify the type of value that must be converted and try to at least approximate something
    switch(type)
    {
        case MYSQL_TYPE_NULL:
        case MYSQL_TYPE_GEOMETRY:       return static_cast< Float32 >(0);
        case MYSQL_TYPE_BIT:
        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_TINY:
        case MYSQL_TYPE_SHORT:
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:           return ConvTo< Float32 >::From(std::strtol(value, nullptr, 10));
        case MYSQL_TYPE_LONGLONG:       return ConvTo< Float32 >::From(std::strtoll(value, nullptr, 10));
        case MYSQL_TYPE_ENUM:
        case MYSQL_TYPE_SET:
        case MYSQL_TYPE_VARCHAR:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_FLOAT:          return std::strtof(value, nullptr);
        case MYSQL_TYPE_DOUBLE:         return ConvTo< Float32 >::From(std::strtod(value, nullptr));
        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL:     return std::strtof(value, nullptr);
        case MYSQL_TYPE_DATE:
        case MYSQL_TYPE_NEWDATE:        return ConvTo< Float32 >::From(MySQLDateStrToSeconds(value));
        case MYSQL_TYPE_TIME:           return ConvTo< Float32 >::From(MySQLTimeStrToSeconds(value));
        case MYSQL_TYPE_TIMESTAMP:      return ConvTo< Float32 >::From(MySQLTimestampStrToSeconds(value));
        case MYSQL_TYPE_DATETIME:       return ConvTo< Float32 >::From(MySQLDatetimeStrToSeconds(value));
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_BLOB:           return MemToNum< Float32 >(reinterpret_cast< const Uint8 * >(value), length);
        default:
        {
            STHROWF("Unknown conversion from (%s) to (%s)", SqMySQLTypenameC(type), tn);
        } break;
    }
    // Should not reach this point!
    return 0;
}

// ------------------------------------------------------------------------------------------------
Float64 DbConvTo< Float64 >::From(CSStr value, Ulong length, enum_field_types type, CSStr tn)
{
    // Is there even a value to attempt to extract?
    if (!value || *value == '\0')
    {
        return 0;
    }
    // Identify the type of value that must be converted and try to at least approximate something
    switch(type)
    {
        case MYSQL_TYPE_NULL:
        case MYSQL_TYPE_GEOMETRY:       return static_cast< Float64 >(0);
        case MYSQL_TYPE_BIT:
        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_TINY:
        case MYSQL_TYPE_SHORT:
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:           return ConvTo< Float64 >::From(std::strtol(value, nullptr, 10));
        case MYSQL_TYPE_LONGLONG:       return ConvTo< Float64 >::From(std::strtoll(value, nullptr, 10));
        case MYSQL_TYPE_ENUM:
        case MYSQL_TYPE_SET:
        case MYSQL_TYPE_VARCHAR:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_FLOAT:
        case MYSQL_TYPE_DOUBLE:
        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL:     return std::strtod(value, nullptr);
        case MYSQL_TYPE_DATE:
        case MYSQL_TYPE_NEWDATE:        return ConvTo< Float64 >::From(MySQLDateStrToSeconds(value));
        case MYSQL_TYPE_TIME:           return ConvTo< Float64 >::From(MySQLTimeStrToSeconds(value));
        case MYSQL_TYPE_TIMESTAMP:      return ConvTo< Float64 >::From(MySQLTimestampStrToSeconds(value));
        case MYSQL_TYPE_DATETIME:       return ConvTo< Float64 >::From(MySQLDatetimeStrToSeconds(value));
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_BLOB:           return MemToNum< Float64 >(reinterpret_cast< const Uint8 * >(value), length);
        default:
        {
            STHROWF("Unknown conversion from (%s) to (%s)", SqMySQLTypenameC(type), tn);
        } break;
    }
    // Should not reach this point!
    return 0;
}

// ------------------------------------------------------------------------------------------------
bool DbConvTo< bool >::From(CSStr value, Ulong length, enum_field_types type, CSStr tn)
{
    return ConvertToSInt< Int64 >(value, length, type, tn) > 0;
}

} // Namespace:: SqMod
