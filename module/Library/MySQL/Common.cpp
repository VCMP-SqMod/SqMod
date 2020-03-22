// ------------------------------------------------------------------------------------------------
#include "Library/MySQL/Common.hpp"
#include "Library/MySQL/Account.hpp"

// ------------------------------------------------------------------------------------------------
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void SqDateToMySQLTime(const Date & value, MYSQL_TIME & t)
{
    // Populate the given structure
    t.year = value.GetYear();
    t.month = value.GetMonth();
    t.day = value.GetDay();
    t.hour = 0;
    t.minute = 0;
    t.second = 0;
    t.neg = false;
    t.second_part = 0;
    t.time_type = MYSQL_TIMESTAMP_DATE;
}

// ------------------------------------------------------------------------------------------------
void SqTimeToMySQLTime(const Time & value, MYSQL_TIME & t)
{
    // Populate the given structure
    t.year = 0;
    t.month = 0;
    t.day = 0;
    t.neg = false;
    t.second_part = value.GetTimestamp().GetSecondsI();
    t.time_type = MYSQL_TIMESTAMP_TIME;
}

// ------------------------------------------------------------------------------------------------
void SqDatetimeToMySQLTime(const Datetime & value, MYSQL_TIME & t)
{
    // Populate the given structure
    t.year = value.GetYear();
    t.month = value.GetMonth();
    t.day = value.GetDay();
    t.hour = value.GetHour();
    t.minute = value.GetMinute();
    t.second = value.GetSecond();
    t.neg = false;
    t.second_part = (value.GetMillisecond() * 1000L);
    t.time_type = MYSQL_TIMESTAMP_DATETIME;
}

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
    return Chrono::DateRangeToSeconds(1000, 1, 1, y, m, d);
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
    return Chrono::DateRangeToSeconds(1000, 1, 1, y, mo, d) + (h * (60 * 60)) + ((mi * 60) + s);
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
    return Chrono::DateRangeToSeconds(1970, 1, 1, y, mo, d) + (h * (60 * 60)) + ((mi * 60) + s);
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

// ------------------------------------------------------------------------------------------------
void ConnHnd::GrabCurrent()
{
    mErrNo = mysql_errno(mPtr);
    mErrStr.assign(mysql_error(mPtr));
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void ConnHnd::ThrowCurrent(CCStr act, CCStr file, Int32 line)
{
    GrabCurrent();
    // Throw the exception with the resulted message
    throw Sqrat::Exception(FmtStr("%s (%u) : %s =>[%s:%d]", act,
                            mErrNo, mErrStr.c_str(), file, line));
}
#else
void ConnHnd::ThrowCurrent(CCStr act)
{
    GrabCurrent();
    // Throw the exception with the resulted message
    throw Sqrat::Exception(FmtStr("%s (%u) : %s", act,
                            mErrNo, mErrStr.c_str()));
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
ConnHnd::ConnHnd()
    : mPtr(nullptr)
    , mErrNo(0)
    , mErrStr(_SC(""))
    , mPort()
    , mHost()
    , mUser()
    , mPass()
    , mName()
    , mSocket()
    , mFlags()
    , mSSL_Key()
    , mSSL_Cert()
    , mSSL_CA()
    , mSSL_CA_Path()
    , mSSL_Cipher()
    , mCharset()
    , mAutoCommit()
    , mInTransaction(false)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
ConnHnd::~ConnHnd()
{
    Disconnect();
}

// ------------------------------------------------------------------------------------------------
void ConnHnd::Create(const Account & acc)
{
    // Is this connection already created?
    if (mPtr != nullptr)
    {
        STHROWF("MySQL connection was already created");
    }
    // Attempt to initialize a connection handle
    mPtr = mysql_init(NULL);
    // See if a connection handle could be initialized
    if (!mPtr)
    {
        STHROWF("Cannot initialize MYSQL connection structure");
    }
    // Store all the account information
    mPort = acc.GetPortNum();
    mHost = acc.GetHost();
    mUser = acc.GetUser();
    mPass = acc.GetPass();
    mName = acc.GetName();
    mSocket = acc.GetSocket();
    mFlags = acc.GetFlags();
    mSSL_Key = acc.GetSSL_Key();
    mSSL_Cert = acc.GetSSL_Cert();
    mSSL_CA = acc.GetSSL_CA();
    mSSL_CA_Path = acc.GetSSL_CA_Path();
    mSSL_Cipher = acc.GetSSL_Cipher();
    mAutoCommit = acc.GetAutoCommit();
    // Attempt to configure SSL if specified
    if (!mSSL_Key.empty() && mysql_ssl_set(mPtr, mSSL_Key.c_str(), mSSL_Cert.c_str(), mSSL_CA.c_str(),
                                                mSSL_CA_Path.c_str(), mSSL_Cipher.c_str()) != 0)
    {
        SQMOD_THROW_CURRENT(*this, "Cannot configure SSL");
    }
    // Attempt to connect to the specified server
    else if (!mysql_real_connect(mPtr, mHost.c_str(), mUser.c_str(), mPass.c_str(),
                            (mName.empty() ? nullptr : mName.c_str()), mPort,
                            (mSocket.empty() ? nullptr : mSocket.c_str()), mFlags))
    {
        SQMOD_THROW_CURRENT(*this, "Cannot connect to database");
    }
    // Attempt configure the auto-commit option
    else if (mysql_autocommit(mPtr, mAutoCommit) != 0)
    {
        SQMOD_THROW_CURRENT(*this, "Cannot configure auto-commit");
    }
    // Get iterators to the options container
    Account::Options::const_iterator itr = acc.GetOptions().cbegin();
    Account::Options::const_iterator end = acc.GetOptions().cend();
    // Process each option in the container
    for (String sql(128, 0); itr != end; ++itr)
    {
        // Prepare the SQL query that applies the option
        sql.assign("SET OPTION ");
        sql.append(itr->first);
        sql.append("=");
        sql.append(itr->second);
        // Execute the resulted query
        if (Execute(sql.c_str(), static_cast< Ulong >(sql.size())) != 1)
        {
            SQMOD_THROW_CURRENT(*this, "Unable to apply option");
        }
    }
    MY_CHARSET_INFO charsetinfo;
    // Grab the information about the current character set
    mysql_get_character_set_info(mPtr, &charsetinfo);
    // We only need the character set name
    if (charsetinfo.name != nullptr)
    {
        mCharset.assign(charsetinfo.name);
    }
}

// ------------------------------------------------------------------------------------------------
void ConnHnd::Disconnect()
{
    if (mPtr != nullptr)
    {
        mysql_close(mPtr);
        // mysql_init() called mysql_thread_init() therefore it needs to clear memory
        // when the MYSQL handle is closed
        mysql_thread_end();
        // Prevent further use of this handle
        mPtr = nullptr;
    }
}

// ------------------------------------------------------------------------------------------------
Uint64 ConnHnd::Execute(CSStr query, Ulong size)
{
    // Make sure that we are connected
    if (!mPtr)
    {
        STHROWF("Invalid MySQL connection");
    }
    // Make sure the specified query is valid
    else if (!query || *query == '\0')
    {
        STHROWF("Invalid or empty MySQL query");
    }
    // Are we supposed to compute the size?
    else if (!size)
    {
        size = std::strlen(query);
    }
    // Attempt to execute the specified query
    if (mysql_real_query(mPtr, query, size))
    {
        SQMOD_THROW_CURRENT(*this, "Unable to execute query");
    }

    // Where the number of affected rows will be stored
    Uint64 affected = 0UL;
    // Count the number of affected rows by any "upsert" statement
    while (true)
    {
        // Attempt to retrieve a buffered result set from the executed query
        ResType * result = mysql_store_result(mPtr);

        // If we have a result, then this was a SELECT statement and we should not count it
        // because it returns the number of selected rows and not modified/affected
        if (result)
        {
            // Just, free the memory associated with the obtained result set
            mysql_free_result(result);
        }
        // Non SELCT queries should have a field count of 0
        else if (mysql_field_count(mPtr) == 0)
        {
            // Sum the number of affected rows by this statement
            affected += mysql_affected_rows(mPtr);
        }
        else if (mysql_errno(mPtr) != 0)
        {
            SQMOD_THROW_CURRENT(*this, "Unable to count affected rows");
        }
        // Prepare the next result from the executed query
        // If return code is 0 then we have a result ready to process
        const Int32 status = mysql_next_result(mPtr);

        // If return code is higher than 0 then an error occurred
        if (status > 0)
        {
            SQMOD_THROW_CURRENT(*this, "Unable to prepare next result");
        }
        // If return code is less than 0 then there are no results left
        else if (status < 0)
        {
            break;
        }
    }
    // Return the number of affected rows
    return affected;
}

// ------------------------------------------------------------------------------------------------
void StmtBind::SetInput(enum_field_types type, BindType * bind, CCStr buffer, Ulong length)
{
    // Associate the library bind point with our bind wrapper
    mBind = bind;
    // Assign the 64 bit unsigned integer as the default buffer
    mBind->buffer = &mUint64;
    // Match the bind point type to the one from the field
    mBind->buffer_type = type;
    // Default to n empty buffer until type identification
    mBind->buffer_length = 0;
    // Allow the library to specify whether the value is null
    mBind->is_null = &mIsNull;
    // Allow the library to specify if errors occurred
    mBind->error = &mError;
    // Tell the library where to read the buffer size
    mBind->length = &(mBind->buffer_length);
    // Identify and configure the parameter type
    switch (type)
    {
        case MYSQL_TYPE_NULL:
        case MYSQL_TYPE_TINY:
        case MYSQL_TYPE_BIT:
        {
            mBind->buffer_length = 1;
        } break;
        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_SHORT:
        {
            mBind->buffer_length = sizeof(Int16);
        } break;
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:
        {
            mBind->buffer_length = sizeof(Int32);
        } break;
        case MYSQL_TYPE_LONGLONG:
        {
            mBind->buffer_length = sizeof(Int64);
        } break;
        case MYSQL_TYPE_FLOAT:
        {
            mBind->buffer_length = sizeof(Float32);
        } break;
        case MYSQL_TYPE_DOUBLE:
        {
            mBind->buffer_length = sizeof(Float64);
        } break;
        case MYSQL_TYPE_NEWDATE:
        case MYSQL_TYPE_DATE:
        case MYSQL_TYPE_TIME:
        case MYSQL_TYPE_DATETIME:
        case MYSQL_TYPE_TIMESTAMP:
        {
            mBind->buffer = &mTime;
        } break;
        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL:
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_BLOB:
        case MYSQL_TYPE_ENUM:
        case MYSQL_TYPE_SET:
        case MYSQL_TYPE_VARCHAR:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_STRING:
        {
            // Allocate a buffer to match the specified size
            mData.Adjust(length);
            // Should we copy anything into the buffer?
            if (buffer)
            {
                mData.Append(buffer, length);
            }
            // Assign the buffer as the new bind point buffer
            mBind->buffer = mData.Data();
            // Assign the buffer cursor position as the new bind point size
            mBind->buffer_length = mData.Position();
        } break;
        default: STHROWF("Unknown MySQL parameter type");
    }
}

// ------------------------------------------------------------------------------------------------
void StmtHnd::GrabCurrent()
{
    mErrNo = mysql_stmt_errno(mPtr);
    mErrStr.assign(mysql_stmt_error(mPtr));
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void StmtHnd::ThrowCurrent(CCStr act, CCStr file, Int32 line)
{
    GrabCurrent();
    // Throw the exception with the resulted message
    throw Sqrat::Exception(FmtStr("%s (%u) : %s =>[%s:%d]", act,
                            mErrNo, mErrStr.c_str(), file, line));
}
#else
void StmtHnd::ThrowCurrent(CCStr act)
{
    GrabCurrent();
    // Throw the exception with the resulted message
    throw Sqrat::Exception(FmtStr("%s (%u) : %s", act,
                            mErrNo, mErrStr.c_str()));
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void StmtHnd::ValidateParam(Uint32 idx, CCStr file, Int32 line) const
{
    // Is the handle valid?
    if (mPtr == nullptr)
    {
        STHROWF("Invalid MySQL statement reference =>[%s:%d]", file, line);
    }
    else if (idx >= mParams)
    {
        STHROWF("Parameter index is out of range: %u >= %lu =>[%s:%d]", idx, mParams, file, line);
    }
}
#else
void StmtHnd::ValidateParam(Uint32 idx) const
{
    // Is the handle valid?
    if (mPtr == nullptr)
    {
        STHROWF("Invalid MySQL statement reference");
    }
    else if (idx >= mParams)
    {
        STHROWF("Parameter index is out of range: %u >= %lu", idx, mParams);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
StmtHnd::StmtHnd()
    : mPtr(nullptr)
    , mErrNo(0)
    , mErrStr(_SC(""))
    , mParams(0)
    , mBinds(nullptr)
    , mMyBinds(nullptr)
    , mConnection()
    , mQuery(_SC(""))
{

}

// ------------------------------------------------------------------------------------------------
StmtHnd::~StmtHnd()
{
    // Should delete native bindings?
    if (mMyBinds)
    {
        delete [] (mMyBinds);
    }
    // Should we delete binding wrappers?
    if (mBinds)
    {
        delete [] (mBinds);
    }
    // Should we release any statement?
    if (mPtr)
    {
        mysql_stmt_close(mPtr);
    }
}

// ------------------------------------------------------------------------------------------------
void StmtHnd::Create(const ConnRef & conn, CSStr query)
{
    // Is this statement already created?
    if (mPtr != nullptr)
    {
        STHROWF("MySQL statement was already created");
    }
    // Validate the specified connection handle
    else if (!conn)
    {
        STHROWF("Invalid MySQL connection reference");
    }
    else if (conn->mPtr == nullptr)
    {
        STHROWF("Invalid MySQL connection");
    }
    // Validate the specified query string
    else if (!query || *query == '\0')
    {
        STHROWF("Invalid or empty MySQL query");
    }
    // Store the connection handle and query string
    mConnection = conn;
    mQuery.assign(query);
    // Attempt to initialize the statement handle
    mPtr = mysql_stmt_init(mConnection->mPtr);
    // Validate the obtained statement handle
    if (!mPtr)
    {
        SQMOD_THROW_CURRENT(*mConnection, "Cannot initialize MySQL statement");
    }
    // Attempt to prepare the statement with the given query
    else if (mysql_stmt_prepare(mPtr, mQuery.c_str(), mQuery.size()))
    {
        SQMOD_THROW_CURRENT(*this, "Cannot prepare MySQL statement");
    }
    // Retrieve the amount of parameters supported by this statement
    mParams = mysql_stmt_param_count(mPtr);
    // Are there any parameters to allocate?
    if (mParams <= 0)
    {
        // We're done here!
        return;
    }
    // Allocate the binding wrappers
    mBinds = new StmtBind[mParams];
    // Validate the allocated memory
    if (!mBinds)
    {
        STHROWF("Unable to allocate MySQL bind point wrappers");
    }
    // Allocate the binding points
    mMyBinds = new BindType[mParams];
    // Validate the allocated memory
    if (!mMyBinds)
    {
        STHROWF("Unable to allocate MySQL bind point structures");
    }
    // Reset the allocated points
    std::memset(mMyBinds, 0, sizeof(BindType) * mParams);
}

// ------------------------------------------------------------------------------------------------
void ResBind::SetOutput(const FieldType & field, BindType * bind)
{
    // Associate the library bind point with our bind wrapper
    mBind = bind;
    // Assign the 64 bit unsigned integer as the default buffer
    mBind->buffer = &mUint64;
    // Match the bind point type to the one from the field
    mBind->buffer_type = field.type;
    // Default to n empty buffer until type identification
    mBind->buffer_length = 0;
    // Allow the library to specify whether the value is null
    mBind->is_null = &mIsNull;
    // Allow the library to specify if errors occurred
    mBind->error = &mError;
    // Tell the library where to read the buffer size
    mBind->length = &(mBind->buffer_length);
    // Identify and configure the field type
    switch (field.type)
    {
        case MYSQL_TYPE_NULL:
        case MYSQL_TYPE_TINY:
        case MYSQL_TYPE_BIT:
        {
            mBind->buffer_length = 1;
        } break;
        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_SHORT:
        {
            mBind->buffer_length = sizeof(Int16);
        } break;
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:
        {
            mBind->buffer_length = sizeof(Int32);
        } break;
        case MYSQL_TYPE_LONGLONG:
        {
            mBind->buffer_length = sizeof(Int64);
        } break;
        case MYSQL_TYPE_FLOAT:
        {
            mBind->buffer_length = sizeof(Float32);
        } break;
        case MYSQL_TYPE_DOUBLE:
        {
            mBind->buffer_length = sizeof(Float64);
        } break;
        case MYSQL_TYPE_NEWDATE:
        case MYSQL_TYPE_DATE:
        case MYSQL_TYPE_TIME:
        case MYSQL_TYPE_DATETIME:
        case MYSQL_TYPE_TIMESTAMP:
        {
            mBind->buffer = &mTime;
        } break;
        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL:
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_BLOB:
        case MYSQL_TYPE_ENUM:
        case MYSQL_TYPE_SET:
        case MYSQL_TYPE_VARCHAR:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_STRING:
        {
            // Allocate a buffer to match the field size
            mData.Adjust(field.max_length);
            // Assign the buffer as the new bind point buffer
            mBind->buffer = mData.Data();
            // Assign the buffer size as the new bind point size
            mBind->buffer_length = mData.Capacity();
        } break;
        default: STHROWF("Unknown MySQL field type");
    }
}

// ------------------------------------------------------------------------------------------------
ResHnd::ResHnd()
    : mPtr(nullptr)
    , mFieldCount(0)
    , mLengths(nullptr)
    , mFields(nullptr)
    , mBinds(nullptr)
    , mMyBinds(nullptr)
    , mRow(nullptr)
    , mConnection()
    , mStatement()
    , mIndexes()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
ResHnd::~ResHnd()
{
    // Is there a result-set that we should free?
    if (mPtr)
    {
        mysql_free_result(mPtr);
    }
    // Are there any bind points that we should free?
    if (mMyBinds)
    {
        delete [] (mMyBinds);
    }
    // Was this result-set from a statement?
    if (mStatement)
    {
        // Are there any rows pointers we should free?
        if (mRow)
        {
            delete [] (mRow);
        }
        // Free the result-set in the statement
        mysql_stmt_free_result(mStatement->mPtr);
    }
    // Are there any bind wrappers that we should free?
    if (mBinds)
    {
        delete [] (mBinds);
    }
}

// ------------------------------------------------------------------------------------------------
void ResHnd::GrabCurrent()
{
    if (mConnection)
    {
        mConnection->GrabCurrent();
    }
    else if (mStatement)
    {
        mStatement->GrabCurrent();
    }
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void ResHnd::ThrowCurrent(CCStr act, CCStr file, Int32 line)
{
    GrabCurrent();
    // Throw the exception with the resulted message
    if (mConnection)
    {
        mConnection->ThrowCurrent(act, file, line);
    }
    else if (mStatement)
    {
        mStatement->ThrowCurrent(act, file, line);
    }
}
#else
void ResHnd::ThrowCurrent(CCStr act)
{
    GrabCurrent();
    // Throw the exception with the resulted message
    if (mConnection)
    {
        mConnection->ThrowCurrent(act);
    }
    else if (mStatement)
    {
        mStatement->ThrowCurrent(act);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void ResHnd::ValidateField(Uint32 idx, CCStr file, Int32 line) const
{
    // Is the handle valid?
    if (mPtr == nullptr)
    {
        STHROWF("Invalid MySQL result-set =>[%s:%d]", file, line);
    }
    else if (idx >= mFieldCount)
    {
        STHROWF("Field index is out of range: %u >= %lu =>[%s:%d]", idx, mFieldCount, file, line);
    }
}
#else
void ResHnd::ValidateField(Uint32 idx) const
{
    // Is the handle valid?
    if (mPtr == nullptr)
    {
        STHROWF("Invalid MySQL result-set");
    }
    else if (idx >= mFieldCount)
    {
        STHROWF("Field index is out of range: %u >= %lu", idx, mFieldCount);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
Uint32 ResHnd::GetFieldIndex(CSStr name)
{
    // Validate the handle
    if (!mPtr)
    {
        STHROWF("Invalid MySQL result-set");
    }
    // Attempt to find the specified field
    const IndexMap::iterator itr = mIndexes.find(name);
    // Was there a field with the specified name?
    if (itr != mIndexes.end())
    {
        return itr->second;
    }
    // No such field exists (expecting the invoker to validate the result)
    return std::numeric_limits< Uint32 >::max();
}

// ------------------------------------------------------------------------------------------------
void ResHnd::Create(const ConnRef & conn)
{
    // Is this result-set already created?
    if (mPtr != nullptr)
    {
        STHROWF("MySQL result-set was already created");
    }
    // Validate the specified connection handle
    else if (!conn)
    {
        STHROWF("Invalid MySQL connection reference");
    }
    else if (conn->mPtr == nullptr)
    {
        STHROWF("Invalid MySQL connection");
    }
    // Store the connection handle
    mConnection = conn;
    // Retrieve the complete result-set to the client, if any
    mPtr = mysql_store_result(mConnection->mPtr);
    // Did this query return any results?
    if (!mPtr)
    {
        return; // We're done here!
    }
    // Obtain the number of fields in the result-set
    mFieldCount = mysql_num_fields(mPtr);
    // Obtain an array representing the fields in the result-set
    mFields = mysql_fetch_fields(mPtr);
    // Associate the field names with their index
    for (Uint32 i = 0; i < mFieldCount; ++i)
    {
        // Associate the current field name with the current index
        mIndexes[mFields[i].name] = i;
        // Include table names if available
        if (mFields[i].table_length > 0)
        {
            mIndexes[ToStrF("%s.%s", mFields[i].table, mFields[i].name)] = i;
        }
        else if (mFields[i].org_table_length > 0)
        {
            mIndexes[ToStrF("%s.%s", mFields[i].org_table, mFields[i].name)] = i;
        }
    }
}

// ------------------------------------------------------------------------------------------------
void ResHnd::Create(const StmtRef & stmt)
{
    // Is this result-set already created?
    if (mPtr != nullptr)
    {
        STHROWF("MySQL result-set was already created");
    }
    // Validate the given statement handle
    else if (!stmt)
    {
        STHROWF("Invalid MySQL statement reference");
    }
    else if (stmt->mPtr == nullptr)
    {
        STHROWF("Invalid MySQL statement");
    }
    // Store the statement handle
    mStatement = stmt;
    // Set the parameter value for the next operation
    int max_length = 1;
    // Force mysql_stmt_store_result() to update the meta-data MYSQL_FIELD->max_length value
    if (mysql_stmt_attr_set(mStatement->mPtr, STMT_ATTR_UPDATE_MAX_LENGTH, &max_length) != 0)
    {
        SQMOD_THROW_CURRENT(*mStatement, "Cannot apply MySQL statement attribute");
    }
    //  Attempt to buffer the complete result-set on the client
    if (mysql_stmt_store_result(mStatement->mPtr))
    {
        SQMOD_THROW_CURRENT(*mStatement, "Cannot buffer MySQL result-set");
    }
    // Obtain the number of fields in the result-set
    mFieldCount = mysql_stmt_field_count(mStatement->mPtr);
    // Obtain the result-set meta-data
    mPtr = mysql_stmt_result_metadata(mStatement->mPtr);
    // Obtain an array representing the fields in the result-set
    mFields = mysql_fetch_fields(mPtr);
    // Are there any fields to allocate
    if (mFieldCount > 0)
    {
        // Allocate the bind wrappers
        mBinds = new ResBind[mFieldCount];
        // Validate the allocated structures
        if (!mBinds)
        {
            STHROWF("Unable to allocate MySQL bind point wrappers");
        }
        // Allocate the bind points
        mMyBinds = new BindType[mFieldCount];
        // Validate the allocated structures
        if (!mMyBinds)
        {
            STHROWF("Unable to allocate MySQL bind point structures");
        }
        // Allocate the row pointers
        mRow = new CStr[mFieldCount];
        // Validate the allocated structures
        if (!mRow)
        {
            STHROWF("Unable to allocate MySQL row pointers");
        }
        // Initialize the bind points to null
        std::memset(mMyBinds, 0, sizeof(BindType) * mFieldCount);
    }
    // Configure bind points and make associations
    for (Uint32 i = 0; i < mFieldCount; ++i)
    {
        // Associate the current field name with the current index
        mIndexes[mFields[i].name] = i;
        // Include table names if available
        if (mFields[i].table_length > 0)
        {
            mIndexes[ToStrF("%s.%s", mFields[i].table, mFields[i].name)] = i;
        }
        else if (mFields[i].org_table_length > 0)
        {
            mIndexes[ToStrF("%s.%s", mFields[i].org_table, mFields[i].name)] = i;
        }
        // Configure the current bind point according to the associated field
        mBinds[i].SetOutput(mFields[i], &mMyBinds[i]);
        // Store the bind point buffer into the associated row
        mRow[i] = mBinds[i].GetBuffer();
    }
    // Associate our bind points with the statement for result storage
    if (mFieldCount > 0 && mysql_stmt_bind_result(mStatement->mPtr, mMyBinds) != 0)
    {
        SQMOD_THROW_CURRENT(*mStatement, "Cannot bind MySQL variables to statement");
    }
}

// ------------------------------------------------------------------------------------------------
Uint64 ResHnd::RowIndex() const
{
    // Is this result-set even valid?
    if (!mPtr)
    {
        STHROWF("Invalid MySQL result-set");
    }
    // Did we come from a statement?
    else if (mStatement)
    {
        return (Uint64)mysql_stmt_row_tell(mStatement->mPtr);
    }
    // Just retrieve it from the result set
    return (Uint64)mysql_row_tell(mPtr);
}

// ------------------------------------------------------------------------------------------------
Uint64 ResHnd::RowCount() const
{
    // Is this result-set even valid?
    if (!mPtr)
    {
        STHROWF("Invalid MySQL result-set");
    }
    // Did we come from a statement?
    else if (mStatement)
    {
        return mysql_stmt_num_rows(mStatement->mPtr);
    }
    // Just retrieve it from the result set
    return mysql_num_rows(mPtr);
}

// ------------------------------------------------------------------------------------------------
bool ResHnd::Next()
{
    // Is this result-set even valid?
    if (!mPtr)
    {
        STHROWF("Invalid MySQL result-set");
    }
    // Did we come from a statement?
    if (mStatement)
    {
        // Step the statement
        return !mysql_stmt_fetch(mStatement->mPtr);
    }
    // Fetch another row from the result set
    mRow = mysql_fetch_row(mPtr);
    // Fetch the data lengths
    mLengths = mysql_fetch_lengths(mPtr);
    // Return whether the fetched row is valid
    return (mRow != NULL);
}

// ------------------------------------------------------------------------------------------------
bool ResHnd::SetRowIndex(Uint64 index)
{
    // Is this result-set even valid?
    if (!mPtr)
    {
        STHROWF("Invalid MySQL result-set");
    }
    // Did we come from a statement?
    else if (mStatement)
    {
        mysql_stmt_data_seek(mStatement->mPtr, index);
    }
    else
    {
        mysql_data_seek(mPtr, index);
    }
    // Step the result set
    return Next();
}

} // Namespace:: SqMod
