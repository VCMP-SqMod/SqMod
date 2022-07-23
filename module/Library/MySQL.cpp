// ------------------------------------------------------------------------------------------------
#include "Library/MySQL.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
#include <limits>
#include <vector>
#include <unordered_map>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
LightObj GetMySQLFromSession(Poco::Data::SessionImpl * session)
{
    // Create a reference counted connection handle instance
    MySQLConnRef ref(new MySQLConnHnd(session));
    // Transform it into a connection instance and yield it as a script object
    return LightObj(SqTypeIdentity< MySQLConnection >{}, SqVM(), ref);
}

// ------------------------------------------------------------------------------------------------
static inline bool IsDigitsOnly(const SQChar * str)
{
    while (std::isdigit(*str) || std::isspace(*str))
    {
        ++str;
    }
    // Return whether we reached the end while searching
    return *str == '\0';
}

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
    t.second_part = static_cast< decltype(t.second_part) >(value.GetTimestamp().GetSecondsI());
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
    t.second_part = static_cast<decltype(t.second_part)>(value.GetMillisecond() * 1000L);
    t.time_type = MYSQL_TIMESTAMP_DATETIME;
}

// ------------------------------------------------------------------------------------------------
const SQChar * SqMySQLTypename(enum_field_types type)
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
const SQChar * SqMySQLTypenameC(enum_field_types type)
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
static inline int64_t MySQLDateStrToSeconds(const SQChar * value)
{
    uint32_t y = 1000, m = 1, d = 1;
    // Attempt to extract the numeric values from the string
    std::sscanf(value, "%u - %u - %u", &y, &m, &d); // NOLINT(cert-err34-c)
    // Calculate the number of seconds and return it
    return Chrono::DateRangeToSeconds(1000, 1, 1, static_cast<uint16_t>(y), static_cast<uint8_t>(m), static_cast<uint8_t>(d));
}

// ------------------------------------------------------------------------------------------------
static inline int64_t MySQLTimeStrToSeconds(const SQChar * value)
{
    int32_t h = 0, m = 0, s = 0;
    // Attempt to extract the numeric values from the string
    std::sscanf(value, "%d : %d : %d", &h, &m, &s); // NOLINT(cert-err34-c)
    // Convert the hours to seconds
    h *= (60 * 60);
    // Add the remaining minutes and seconds and return the result
    return (h < 0) ? (h - ((m * 60) + s)) : (h + ((m * 60) + s));
}

// ------------------------------------------------------------------------------------------------
static inline int64_t MySQLDatetimeStrToSeconds(const SQChar * value)
{
    uint32_t y = 1000, mo = 1, d = 1, h = 0, mi = 0, s = 0;
    // Attempt to extract the numeric values from the string
    std::sscanf(value, "%u - %u - %u %u : %u : %u", &y, &mo, &d, &h, &mi, &s); // NOLINT(cert-err34-c)
    // Calculate the number of seconds and return it
    return Chrono::DateRangeToSeconds(1000, 1, 1, static_cast<uint16_t>(y), static_cast<uint8_t>(mo),
                                      static_cast<uint8_t>(d)) + (h * (60 * 60)) + ((mi * 60) + s);
}

// ------------------------------------------------------------------------------------------------
static inline int64_t MySQLTimestampStrToSeconds(const SQChar * value)
{
    uint32_t y = 1000, mo = 1, d = 1, h = 0, mi = 0, s = 0;
    // Attempt to extract the numeric values from the string
    std::sscanf(value, "%u - %u - %u %u : %u : %u", &y, &mo, &d, &h, &mi, &s); // NOLINT(cert-err34-c)
    // Detect if this was time-stamp 0
    if (!y && !mo && !d && !h && !mi && !s)
    {
        return 0;
    }
    // Calculate the number of seconds and return it
    return Chrono::DateRangeToSeconds(1970, 1, 1, static_cast<uint16_t>(y), static_cast<uint8_t>(mo),
                                      static_cast<uint8_t>(d)) + (h * (60 * 60)) + ((mi * 60) + s);
}

// ------------------------------------------------------------------------------------------------
template < typename T > static inline T MemToNum(const uint8_t * b, unsigned long l)
{
    union
    {
        uint8_t a[8];
        T       n{};
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
static inline T ConvertToSInt(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn)
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
        case MYSQL_TYPE_BLOB:           return MemToNum< T >(reinterpret_cast< const uint8_t * >(value), length);
        default:
        {
            STHROWF("Unknown conversion from ({}) to ({})", SqMySQLTypenameC(type), tn);
        } break;
    }
    // Should not reach this point!
    return 0;
}

// ------------------------------------------------------------------------------------------------
template < typename T >
static inline T ConvertToUInt(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn)
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
        case MYSQL_TYPE_BLOB:           return MemToNum< T >(reinterpret_cast< const uint8_t * >(value), length);
        default:
        {
            STHROWF("Unknown conversion from ({}) to ({})", SqMySQLTypenameC(type), tn);
        } break;
    }
    // Should not reach this point!
    return 0;
}

// ------------------------------------------------------------------------------------------------
int8_t DbConvTo< int8_t >::From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn)
{
    return ConvertToSInt< int8_t >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
uint8_t DbConvTo< uint8_t >::From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn)
{
    return ConvertToUInt< uint8_t >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
int16_t DbConvTo< int16_t >::From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn)
{
    return ConvertToSInt< int16_t >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
uint16_t DbConvTo< uint16_t >::From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn)
{
    return ConvertToUInt< uint16_t >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
int32_t DbConvTo< int32_t >::From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn)
{
    return ConvertToSInt< int32_t >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
uint32_t DbConvTo< uint32_t >::From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn)
{
    return ConvertToUInt< uint32_t >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
int64_t DbConvTo< int64_t >::From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn)
{
    return ConvertToSInt< int64_t >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
uint64_t DbConvTo< uint64_t >::From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn)
{
    return ConvertToUInt< uint64_t >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
float DbConvTo< float >::From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn)
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
        case MYSQL_TYPE_GEOMETRY:       return static_cast< float >(0);
        case MYSQL_TYPE_BIT:
        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_TINY:
        case MYSQL_TYPE_SHORT:
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:           return ConvTo< float >::From(std::strtol(value, nullptr, 10));
        case MYSQL_TYPE_LONGLONG:       return ConvTo< float >::From(std::strtoll(value, nullptr, 10));
        case MYSQL_TYPE_ENUM:
        case MYSQL_TYPE_SET:
        case MYSQL_TYPE_VARCHAR:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_FLOAT:          return std::strtof(value, nullptr);
        case MYSQL_TYPE_DOUBLE:         return ConvTo< float >::From(std::strtod(value, nullptr));
        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL:     return std::strtof(value, nullptr);
        case MYSQL_TYPE_DATE:
        case MYSQL_TYPE_NEWDATE:        return ConvTo< float >::From(MySQLDateStrToSeconds(value));
        case MYSQL_TYPE_TIME:           return ConvTo< float >::From(MySQLTimeStrToSeconds(value));
        case MYSQL_TYPE_TIMESTAMP:      return ConvTo< float >::From(MySQLTimestampStrToSeconds(value));
        case MYSQL_TYPE_DATETIME:       return ConvTo< float >::From(MySQLDatetimeStrToSeconds(value));
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_BLOB:           return MemToNum< float >(reinterpret_cast< const uint8_t * >(value), length);
        default:
        {
            STHROWF("Unknown conversion from ({}) to ({})", SqMySQLTypenameC(type), tn);
        } break;
    }
    // Should not reach this point!
    return 0;
}

// ------------------------------------------------------------------------------------------------
double DbConvTo< double >::From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn)
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
        case MYSQL_TYPE_GEOMETRY:       return static_cast< double >(0);
        case MYSQL_TYPE_BIT:
        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_TINY:
        case MYSQL_TYPE_SHORT:
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:           return ConvTo< double >::From(std::strtol(value, nullptr, 10));
        case MYSQL_TYPE_LONGLONG:       return ConvTo< double >::From(std::strtoll(value, nullptr, 10));
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
        case MYSQL_TYPE_NEWDATE:        return ConvTo< double >::From(MySQLDateStrToSeconds(value));
        case MYSQL_TYPE_TIME:           return ConvTo< double >::From(MySQLTimeStrToSeconds(value));
        case MYSQL_TYPE_TIMESTAMP:      return ConvTo< double >::From(MySQLTimestampStrToSeconds(value));
        case MYSQL_TYPE_DATETIME:       return ConvTo< double >::From(MySQLDatetimeStrToSeconds(value));
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_BLOB:           return MemToNum< double >(reinterpret_cast< const uint8_t * >(value), length);
        default:
        {
            STHROWF("Unknown conversion from ({}) to ({})", SqMySQLTypenameC(type), tn);
        } break;
    }
    // Should not reach this point!
    return 0;
}

// ------------------------------------------------------------------------------------------------
bool DbConvTo< bool >::From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn)
{
    return ConvertToSInt< int64_t >(value, length, type, tn) > 0;
}

// ------------------------------------------------------------------------------------------------
char DbConvTo< char >::From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn)
{
    return ConvertToSInt< char >(value, length, type, tn);
}

// ------------------------------------------------------------------------------------------------
void MySQLConnHnd::GrabCurrent()
{
    mErrNo = mysql_errno(Access());
    mErrStr.assign(mysql_error(mPtr));
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void MySQLConnHnd::ThrowCurrent(const char * act, const char * file, int32_t line)
{
    GrabCurrent();
    // Throw the exception with the resulted message
    throw Sqrat::Exception(fmt::format("{} ({}) : {} =>[{}:{}]", act, mErrNo, mErrStr, file, line));
}
#else
void MySQLConnHnd::ThrowCurrent(const char * act)
{
    GrabCurrent();
    // Throw the exception with the resulted message
    throw Sqrat::Exception(fmt::format("{} ({}) : {}", act, mErrNo, mErrStr));
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
MySQLConnHnd::MySQLConnHnd()
    : mPtr(nullptr)
    , mErrNo(0)
    , mErrStr()
    , mSession()
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
MySQLConnHnd::MySQLConnHnd(Poco::Data::SessionImpl * session)
    : MySQLConnHnd()
{
    mSession.assign(session, true);
    // Retrieve the internal handle property
    mPtr = Poco::AnyCast< MYSQL * >(session->getProperty("handle"));
}

// ------------------------------------------------------------------------------------------------
MySQLConnHnd::MySQLConnHnd(Poco::AutoPtr< Poco::Data::SessionImpl > && session)
    : MySQLConnHnd()
{
    mSession = std::move(session);
    // Retrieve the internal handle property
    mPtr = Poco::AnyCast< MYSQL * >(session->getProperty("handle"));
}

// ------------------------------------------------------------------------------------------------
MySQLConnHnd::~MySQLConnHnd()
{
    Disconnect();
}

// ------------------------------------------------------------------------------------------------
void MySQLConnHnd::Create(const MySQLAccount & acc)
{
    // Is this connection already created?
    if (mPtr != nullptr)
    {
        STHROWF("MySQL connection was already created");
    }
    // Attempt to initialize a connection handle
    mPtr = mysql_init(nullptr);
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
    mFlags = static_cast<unsigned long>(acc.GetFlags());
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
    else if (mysql_autocommit(mPtr, static_cast< MySQLStmtBind::BoolType >(mAutoCommit)) != 0)
    {
        SQMOD_THROW_CURRENT(*this, "Cannot configure auto-commit");
    }
    // Get iterators to the options container
    auto itr = acc.GetOptions().cbegin();
    auto end = acc.GetOptions().cend();
    // Process each option in the container
    for (String sql(128, 0); itr != end; ++itr)
    {
        // Prepare the SQL query that applies the option
        sql.assign("SET OPTION ");
        sql.append(itr->first);
        sql.append("=");
        sql.append(itr->second);
        // Execute the resulted query
        if (Execute(sql.c_str(), static_cast< unsigned long >(sql.size())) != 1)
        {
            SQMOD_THROW_CURRENT(*this, "Unable to apply option");
        }
    }
    MY_CHARSET_INFO charset_info;
    // Grab the information about the current character set
    mysql_get_character_set_info(mPtr, &charset_info);
    // We only need the character set name
    if (charset_info.name != nullptr)
    {
        mCharset.assign(charset_info.name);
    }
}

// ------------------------------------------------------------------------------------------------
void MySQLConnHnd::Disconnect()
{
    if (mPtr != nullptr)
    {
        // If this connection is a pooled session then let it clean itself up
        if (mSession.isNull())
        {
            mysql_close(mPtr);
            // mysql_init() called mysql_thread_init() therefore it needs to clear memory
            // when the MYSQL handle is closed
            mysql_thread_end();
        }
        else
        {
            mSession.reset();
        }
        // Prevent further use of this handle
        mPtr = nullptr;
    }
}

// ------------------------------------------------------------------------------------------------
uint64_t MySQLConnHnd::Execute(const SQChar * query, unsigned long size)
{
    // Make sure that we are connected
    if (!Access())
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
        size = static_cast<unsigned long>(std::strlen(query));
    }
    // Attempt to execute the specified query
    if (mysql_real_query(mPtr, query, size))
    {
        SQMOD_THROW_CURRENT(*this, "Unable to execute query");
    }

    // Where the number of affected rows will be stored
    uint64_t affected = 0UL;
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
        // Non SELECT queries should have a field count of 0
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
        const int32_t status = mysql_next_result(mPtr);

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
void MySQLStmtBind::SetInput(enum_field_types type, BindType * bind, const char * buffer, unsigned long length)
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
            mBind->buffer_length = sizeof(int16_t);
        } break;
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:
        {
            mBind->buffer_length = sizeof(int32_t);
        } break;
        case MYSQL_TYPE_LONGLONG:
        {
            mBind->buffer_length = sizeof(int64_t);
        } break;
        case MYSQL_TYPE_FLOAT:
        {
            mBind->buffer_length = sizeof(float);
        } break;
        case MYSQL_TYPE_DOUBLE:
        {
            mBind->buffer_length = sizeof(double);
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
void MySQLStmtHnd::GrabCurrent()
{
    mErrNo = mysql_stmt_errno(Access());
    mErrStr.assign(mysql_stmt_error(mPtr));
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void MySQLStmtHnd::ThrowCurrent(const char * act, const char * file, int32_t line)
{
    GrabCurrent();
    // Throw the exception with the resulted message
    throw Sqrat::Exception(fmt::format("{} ({}) : {} =>[{}:{}]", act, mErrNo, mErrStr, file, line));
}
#else
void MySQLStmtHnd::ThrowCurrent(const char * act)
{
    GrabCurrent();
    // Throw the exception with the resulted message
    throw Sqrat::Exception(fmt::format("{} ({}) : {}", act, mErrNo, mErrStr));
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void MySQLStmtHnd::ValidateParam(uint32_t idx, const char * file, int32_t line) const
{
    // Is the handle valid?
    if (Access() == nullptr)
    {
        STHROWF("Invalid MySQL statement reference =>[{}:{}]", file, line);
    }
    else if (idx >= mParams)
    {
        STHROWF("Parameter index is out of range: {} >= {} =>[{}:{}]", idx, mParams, file, line);
    }
}
#else
void MySQLStmtHnd::ValidateParam(uint32_t idx) const
{
    // Is the handle valid?
    if (Access() == nullptr)
    {
        STHROWF("Invalid MySQL statement reference");
    }
    else if (idx >= mParams)
    {
        STHROWF("Parameter index is out of range: {} >= {}", idx, mParams);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
MySQLStmtHnd::MySQLStmtHnd()
    : mPtr(nullptr)
    , mErrNo(0)
    , mErrStr()
    , mParams(0)
    , mBinds(nullptr)
    , mMyBinds(nullptr)
    , mConnection()
    , mQuery()
{

}

// ------------------------------------------------------------------------------------------------
MySQLStmtHnd::~MySQLStmtHnd()
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
void MySQLStmtHnd::Create(const MySQLConnRef & conn, const SQChar * query)
{
    // Is this statement already created?
    if (Access() != nullptr)
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
    else if (mysql_stmt_prepare(mPtr, mQuery.c_str(), static_cast<unsigned long>(mQuery.size())))
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
    mBinds = new MySQLStmtBind[mParams];
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
void MySQLResBind::SetOutput(const FieldType & field, BindType * bind)
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
            mBind->buffer_length = sizeof(int16_t);
        } break;
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:
        {
            mBind->buffer_length = sizeof(int32_t);
        } break;
        case MYSQL_TYPE_LONGLONG:
        {
            mBind->buffer_length = sizeof(int64_t);
        } break;
        case MYSQL_TYPE_FLOAT:
        {
            mBind->buffer_length = sizeof(float);
        } break;
        case MYSQL_TYPE_DOUBLE:
        {
            mBind->buffer_length = sizeof(double);
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
MySQLResHnd::MySQLResHnd()
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
MySQLResHnd::~MySQLResHnd()
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
void MySQLResHnd::GrabCurrent() const
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
void MySQLResHnd::ThrowCurrent(const char * act, const char * file, int32_t line) const
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
void MySQLResHnd::ThrowCurrent(const char * act) const
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
void MySQLResHnd::ValidateField(uint32_t idx, const char * file, int32_t line) const
{
    // Is the handle valid?
    if (Access() == nullptr)
    {
        STHROWF("Invalid MySQL result-set =>[{}:{}]", file, line);
    }
    else if (idx >= mFieldCount)
    {
        STHROWF("Field index is out of range: {} >= {} =>[{}:{}]", idx, mFieldCount, file, line);
    }
}
#else
void MySQLResHnd::ValidateField(uint32_t idx) const
{
    // Is the handle valid?
    if (Access() == nullptr)
    {
        STHROWF("Invalid MySQL result-set");
    }
    else if (idx >= mFieldCount)
    {
        STHROWF("Field index is out of range: {} >= {}", idx, mFieldCount);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
uint32_t MySQLResHnd::GetFieldIndex(const SQChar * name)
{
    // Validate the handle
    if (!Access())
    {
        STHROWF("Invalid MySQL result-set");
    }
    // Attempt to find the specified field
    const auto itr = mIndexes.find(name);
    // Was there a field with the specified name?
    if (itr != mIndexes.end())
    {
        return itr->second;
    }
    // No such field exists (expecting the invoker to validate the result)
    return std::numeric_limits< uint32_t >::max();
}

// ------------------------------------------------------------------------------------------------
void MySQLResHnd::Create(const MySQLConnRef & conn)
{
    // Is this result-set already created?
    if (Access() != nullptr)
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
    for (uint32_t i = 0; i < mFieldCount; ++i)
    {
        // Associate the current field name with the current index
        mIndexes[mFields[i].name] = i;
        // Include table names if available
        if (mFields[i].table_length > 0)
        {
            mIndexes[fmt::format("{}.{}", mFields[i].table, mFields[i].name)] = i;
        }
        else if (mFields[i].org_table_length > 0)
        {
            mIndexes[fmt::format("{}.{}", mFields[i].org_table, mFields[i].name)] = i;
        }
    }
}

// ------------------------------------------------------------------------------------------------
void MySQLResHnd::Create(const MySQLStmtRef & stmt)
{
    // Is this result-set already created?
    if (Access() != nullptr)
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
        mBinds = new MySQLResBind[mFieldCount];
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
        mRow = new char *[mFieldCount];
        // Validate the allocated structures
        if (!mRow)
        {
            STHROWF("Unable to allocate MySQL row pointers");
        }
        // Initialize the bind points to null
        std::memset(mMyBinds, 0, sizeof(BindType) * mFieldCount);
    }
    // Configure bind points and make associations
    for (uint32_t i = 0; i < mFieldCount; ++i)
    {
        // Associate the current field name with the current index
        mIndexes[mFields[i].name] = i;
        // Include table names if available
        if (mFields[i].table_length > 0)
        {
            mIndexes[fmt::format("{}.{}", mFields[i].table, mFields[i].name)] = i;
        }
        else if (mFields[i].org_table_length > 0)
        {
            mIndexes[fmt::format("{}.{}", mFields[i].org_table, mFields[i].name)] = i;
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
uint64_t MySQLResHnd::RowIndex() const
{
    // Is this result-set even valid?
    if (!Access())
    {
        STHROWF("Invalid MySQL result-set");
    }
    // Did we come from a statement?
    else if (mStatement)
    {
        return (uint64_t)mysql_stmt_row_tell(mStatement->mPtr);
    }
    // Just retrieve it from the result set
    return (uint64_t)mysql_row_tell(mPtr);
}

// ------------------------------------------------------------------------------------------------
uint64_t MySQLResHnd::RowCount() const
{
    // Is this result-set even valid?
    if (!Access())
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
bool MySQLResHnd::Next()
{
    // Is this result-set even valid?
    if (!Access())
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
    return (mRow != nullptr);
}

// ------------------------------------------------------------------------------------------------
bool MySQLResHnd::SetRowIndex(uint64_t index)
{
    // Is this result-set even valid?
    if (!Access())
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

// ------------------------------------------------------------------------------------------------
const String MySQLAccount::s_String{}; // NOLINT(cert-err58-cpp)

// ------------------------------------------------------------------------------------------------
SQInteger MySQLAccount::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMySQLAccount");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
MySQLAccount::MySQLAccount(const SQChar * host, const SQChar * user, const SQChar * pass, const SQChar * name, SQInteger port, const SQChar * socket)
    : m_Port(0)
    , m_Host()
    , m_User()
    , m_Pass()
    , m_Name()
    , m_Socket()
    , m_Flags(CLIENT_MULTI_STATEMENTS)
    , m_SSL_Key()
    , m_SSL_Cert()
    , m_SSL_CA()
    , m_SSL_CA_Path()
    , m_SSL_Cipher()
    , m_Options()
    , m_AutoCommit(true)
{
    // Validate the specified port number
    if (port >= 0xFFFF)
    {
        STHROWF("Port number out of range: " _PRINT_INT_FMT, port);
    }
    // Assign the specified port
    else
    {
        m_Port = ConvTo< uint16_t >::From(port);
    }
    // Assign the remaining values
    SetHost(host);
    SetUser(user);
    SetPass(pass);
    SetName(name);
    SetSocket(socket);
}

// ------------------------------------------------------------------------------------------------
int32_t MySQLAccount::Cmp(const MySQLAccount & o) const
{
    if (m_User == o.m_User && m_Pass == o.m_Pass)
    {
        return 0;
    }
    else if ((m_User > o.m_User) && (m_Pass > o.m_Pass)) // NOLINT(modernize-use-nullptr)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
String MySQLAccount::ToString() const
{
    return fmt::format("{}:{}@{}:{}", m_User, m_Pass, m_Host, m_Port);
}

// ------------------------------------------------------------------------------------------------
void MySQLAccount::SetHost(const SQChar * addr)
{
    // Clear the current host address
    m_Host.assign(_SC(""));
    // Validate the given address
    if (!addr || *addr == '\0')
    {
        return; // Nothing to set.
    }
    // See if there's a colon in the address
    const SQChar * pos = std::strchr(addr, ':');
    // Should we look for a port number?
    if (pos != nullptr)
    {
        // Assign the address portion
        m_Host.assign(addr, pos - addr);
        // Attempt to extract the numeric value
        const unsigned long num = std::strtoul(++pos, nullptr, 10);
        // Is the port number withing range?
        if (num > 0xFFFF)
        {
            STHROWF("Port number out of range: {}", num);
        }
        // Assign the extracted port number
        m_Port = ConvTo< uint16_t >::From(num);
    }
    // Assign the address as is
    else
    {
        m_Host.assign(addr);
    }
}

// ------------------------------------------------------------------------------------------------
void MySQLAccount::SetPortNum(SQInteger port)
{
    // Validate the specified port number
    if (port >= 0xFFFF)
    {
        STHROWF("Port number out of range: " _PRINT_INT_FMT, port);
    }
    // Assign the specified port number
    m_Port = ConvTo< uint16_t >::From(port);
}

// ------------------------------------------------------------------------------------------------
void MySQLAccount::SetSSL(const SQChar * key, const SQChar * cert, const SQChar * ca, const SQChar * ca_path, const SQChar * cipher)
{
    if (!key || *key == '\0')
    {
        m_SSL_Key.clear();
        m_SSL_Cert.clear();
        m_SSL_CA.clear();
        m_SSL_CA_Path.clear();
        m_SSL_Cipher.clear();
    }
    else
    {
        m_SSL_Key.assign(key ? key : _SC(""));
        m_SSL_Cert.assign(cert ? cert : _SC(""));
        m_SSL_CA.assign(ca ? ca : _SC(""));
        m_SSL_CA_Path.assign(ca_path ? ca_path : _SC(""));
        m_SSL_Cipher.assign(cipher ? cipher : _SC(""));
    }
}

// ------------------------------------------------------------------------------------------------
Table MySQLAccount::GetOptionsTable() const
{
    // Allocate an empty table
    Table tbl(SqVM(), static_cast< SQInteger >(m_Options.size()));
    // Insert every option into the table
    for (const auto & opt : m_Options)
    {
        tbl.SetValue(opt.first.c_str(), opt.second);
    }
    // Return the resulted table
    return tbl;
}

// ------------------------------------------------------------------------------------------------
const String & MySQLAccount::GetOption(const SQChar * name) const
{
    // Make sure the specified name is valid
    if (!name || *name == '\0')
    {
        STHROWF("Invalid or empty option name");
    }
    // Attempt to find the requested option
    const auto itr = m_Options.find(name);
    // Return the result
    return (itr == m_Options.cend()) ? s_String : itr->second;
}

// ------------------------------------------------------------------------------------------------
void MySQLAccount::SetOption(const SQChar * name, const SQChar * value)
{
    // Make sure the specified name is valid
    if (!name || *name == '\0')
    {
        STHROWF("Invalid or empty option name");
    }
    // Assign the specified value
    m_Options[name] = value ? value : _SC("");
}

// ------------------------------------------------------------------------------------------------
void MySQLAccount::RemoveOption(const SQChar * name)
{
    // Make sure the specified name is valid
    if (!name || *name == '\0')
    {
        STHROWF("Invalid or empty option name");
    }
    // Erase the specified value
    m_Options.erase(name);
}

// ------------------------------------------------------------------------------------------------
MySQLConnection MySQLAccount::Connect() const
{
    return MySQLConnection(*this);
}

// ------------------------------------------------------------------------------------------------
SQInteger MySQLConnection::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMySQLConnection");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void MySQLConnection::Validate(const char * file, int32_t line) const
{
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid MySQL connection reference =>[{}:{}]"), file, line);
    }
}
#else
void MySQLConnection::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid MySQL connection reference"));
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void MySQLConnection::ValidateCreated(const char * file, int32_t line) const
{
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid MySQL connection reference =>[{}:{}]"), file, line);
    }
    else if (m_Handle->Access() == nullptr)
    {
        SqThrowF(SQMOD_RTFMT("Invalid MySQL connection =>[{}:{}]"), file, line);
    }
}
#else
void MySQLConnection::ValidateCreated() const
{
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid MySQL connection reference"));
    }
    else if (m_Handle->Access() == nullptr)
    {
        SqThrowF(fmt::runtime("Invalid MySQL connection"));
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const MySQLConnRef & MySQLConnection::GetValid(const char * file, int32_t line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const MySQLConnRef & MySQLConnection::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const MySQLConnRef & MySQLConnection::GetCreated(const char * file, int32_t line) const
{
    ValidateCreated(file, line);
    return m_Handle;
}
#else
const MySQLConnRef & MySQLConnection::GetCreated() const
{
    ValidateCreated();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
SQInteger MySQLConnection::Insert(const SQChar * query)
{
    // Make sure the specified query is valid
    if (!query || *query == '\0')
    {
        STHROWF("Invalid or empty MySQL query");
    }
    // Attempt to execute the specified query
    else if (mysql_real_query(SQMOD_GET_CREATED(*this)->mPtr, query, static_cast<unsigned long>(std::strlen(query))) != 0)
    {
        SQMOD_THROW_CURRENT(*m_Handle, "Unable to execute MySQL query");
    }
    // Return the identifier of the inserted row
    return static_cast< SQInteger >(mysql_insert_id(m_Handle->Access()));
}

// ------------------------------------------------------------------------------------------------
MySQLResultSet MySQLConnection::Query(const SQChar * query)
{
    // Make sure the specified query is valid
    if (!query || *query == '\0')
    {
        STHROWF("Invalid or empty MySQL query");
    }
    // Attempt to execute the specified query
    else if (mysql_real_query(SQMOD_GET_CREATED(*this)->mPtr, query, static_cast<unsigned long>(std::strlen(query))) != 0)
    {
        SQMOD_THROW_CURRENT(*m_Handle, "Unable to execute MySQL query");
    }
    // Return the identifier of the inserted row
    return MySQLResultSet(m_Handle);
}

// ------------------------------------------------------------------------------------------------
MySQLStatement MySQLConnection::GetStatement(const SQChar * query)
{
    return MySQLStatement(SQMOD_GET_CREATED(*this), query);
}

// ------------------------------------------------------------------------------------------------
SQInteger MySQLConnection::ExecuteF(HSQUIRRELVM vm)
{
    const auto top = static_cast<const int32_t>(sq_gettop(vm));
    // Was the query value specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing query value");
    }
    // The connection instance
    MySQLConnection * conn = nullptr;
    // Attempt to extract the argument values
    try
    {
        conn = Var< MySQLConnection * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.what());
    }
    // Do we have a valid connection instance?
    if (!conn)
    {
        return sq_throwerror(vm, "Invalid MySQL connection instance");
    }
    // Validate the connection info
    try
    {
        SQMOD_VALIDATE_CREATED(*conn);
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.what());
    }
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Make sure the query string is valid
    else if (!(val.mPtr) || *(val.mPtr) == '\0')
    {
        STHROWF("Invalid or empty MySQL query");
    }
    // Attempt to execute the specified query
    try
    {
        sq_pushinteger(vm, static_cast< SQInteger >(conn->m_Handle->Execute(val.mPtr, static_cast< unsigned long >(val.mLen))));
    }
    catch (const std::exception & e)
    {
        return sq_throwerror(vm, e.what()); // Propagate the error
    }
    // This function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger MySQLConnection::InsertF(HSQUIRRELVM vm)
{
    const auto top = static_cast<const int32_t>(sq_gettop(vm));
    // Was the query value specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing query value");
    }
    // The connection instance
    MySQLConnection * conn = nullptr;
    // Attempt to extract the argument values
    try
    {
        conn = Var< MySQLConnection * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.what());
    }
    // Do we have a valid connection instance?
    if (!conn)
    {
        return sq_throwerror(vm, "Invalid MySQL connection instance");
    }
    // Validate the connection info
    try
    {
        SQMOD_VALIDATE_CREATED(*conn);
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.what());
    }
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Make sure the query string is valid
    else if (!(val.mPtr) || *(val.mPtr) == '\0')
    {
        return sq_throwerror(vm, "Invalid or empty MySQL query");
    }
    // Attempt to execute the specified query
    try
    {
        if (mysql_real_query(conn->m_Handle->Access(), val.mPtr, static_cast<unsigned long>(val.mLen)) != 0)
        {
            SQMOD_THROW_CURRENT(*(conn->m_Handle), "Unable to execute MySQL query");
        }
        // Return the identifier of the inserted row
        sq_pushinteger(vm, static_cast< SQInteger >(mysql_insert_id(conn->m_Handle->Access())));
    }
    catch (const std::exception & e)
    {
        return sq_throwerror(vm, e.what()); // Propagate the error
    }
    // This function returned a value
    return 1;
}


// ------------------------------------------------------------------------------------------------
SQInteger MySQLConnection::QueryF(HSQUIRRELVM vm)
{
    const auto top = static_cast<const int32_t>(sq_gettop(vm));
    // Was the query value specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing query value");
    }
    // The connection instance
    MySQLConnection * conn = nullptr;
    // Attempt to extract the argument values
    try
    {
        conn = Var< MySQLConnection * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.what());
    }
    // Do we have a valid connection instance?
    if (!conn)
    {
        return sq_throwerror(vm, "Invalid MySQL connection instance");
    }
    // Validate the connection info
    try
    {
        SQMOD_VALIDATE_CREATED(*conn);
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.what());
    }
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Make sure the query string is valid
    else if (!(val.mPtr) || *(val.mPtr) == '\0')
    {
        return sq_throwerror(vm, "Invalid or empty MySQL query");
    }
    // Attempt to execute the specified query
    try
    {
        if (mysql_real_query(conn->m_Handle->Access(), val.mPtr, static_cast<unsigned long>(val.mLen)) != 0)
        {
            SQMOD_THROW_CURRENT(*(conn->m_Handle), "Unable to execute MySQL query");
        }
        // Return a new instance with the obtained result set
        Var< MySQLResultSet * >::push(vm, new MySQLResultSet(conn->m_Handle));
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.what());
    }
    // This function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
LightObj MySQLConnection::EscapeString(StackStrF & str)
{
    // Is there even a string to escape?
    if (str.mLen <= 0)
    {
        return LightObj(_SC(""), 0, str.mVM); // Default to empty string
    }
    // Allocate a buffer for the given string
    std::vector< SQChar > buffer(static_cast< size_t >(str.mLen * 2 + 1));
    // Attempt to escape the specified string
    const unsigned long len = mysql_real_escape_string(m_Handle->Access(), buffer.data(), str.mPtr,
                                                       static_cast<unsigned long>(str.mLen));
    // Return the resulted string
    return LightObj(buffer.data(), static_cast< SQInteger >(len), str.mVM);
}

// ------------------------------------------------------------------------------------------------
SQInteger MySQLField::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMySQLField");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void MySQLField::Validate(const char * file, int32_t line) const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid MySQL result-set reference =>[{}:{}]"), file, line);
    }
    // Are we pointing to a valid index?
    else if (m_Index >= m_Handle->mFieldCount)
    {
        SqThrowF(SQMOD_RTFMT("Field index is out of range: {} >= {} =>[{}:{}]"), m_Index, m_Handle->mFieldCount, file, line);
    }
}
#else
void MySQLField::Validate() const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid MySQL result-set reference"));
    }
    // Are we pointing to a valid index?
    else if (m_Index >= m_Handle->mFieldCount)
    {
        SqThrowF(fmt::runtime("Field index is out of range: {} >= {}"), m_Index, m_Handle->mFieldCount);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void MySQLField::ValidateCreated(const char * file, int32_t line) const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid MySQL result-set reference =>[{}:{}]"), file, line);
    }
    // Are we pointing to a valid index?
    m_Handle->ValidateField(m_Index, file, line);
}
#else
void MySQLField::ValidateCreated() const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid MySQL result-set reference"));
    }
    // Are we pointing to a valid index?
    m_Handle->ValidateField(m_Index);
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void MySQLField::ValidateStepped(const char * file, int32_t line) const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid MySQL result-set reference =>[{}:{}]"), file, line);
    }
    // Do we have a valid row available?
    else if (m_Handle->mRow == nullptr)
    {
        SqThrowF(SQMOD_RTFMT("No row available in MySQL result-set =>[{}:{}]"), file, line);
    }
    // Are we pointing to a valid index?
    m_Handle->ValidateField(m_Index, file, line);
}
#else
void MySQLField::ValidateStepped() const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid MySQL result-set reference"));
    }
    // Do we have a valid row available?
    else if (m_Handle->mRow == nullptr)
    {
        SqThrowF(fmt::runtime("No row available in MySQL result-set"));
    }
    // Are we pointing to a valid index?
    m_Handle->ValidateField(m_Index);
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const MySQLResRef & MySQLField::GetValid(const char * file, int32_t line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const MySQLResRef & MySQLField::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const MySQLResRef & MySQLField::GetCreated(const char * file, int32_t line) const
{
    ValidateCreated(file, line);
    return m_Handle;
}
#else
const MySQLResRef & MySQLField::GetCreated() const
{
    ValidateCreated();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const MySQLResRef & MySQLField::GetStepped(const char * file, int32_t line) const
{
    ValidateStepped(file, line);
    return m_Handle;
}
#else
const MySQLResRef & MySQLField::GetStepped() const
{
    ValidateStepped();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void MySQLField::ValidateField(uint32_t idx, const char * file, int32_t line) const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid MySQL result-set reference =>[{}:{}]"), file, line);
    }
    // Validate the specified field index
    m_Handle->ValidateField(idx, file, line);
}
#else
void MySQLField::ValidateField(uint32_t idx) const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid MySQL result-set reference"));
    }
    // Validate the specified field index
    m_Handle->ValidateField(idx);
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
void MySQLField::SetIndex(const Object & field)
{
    // Where the index will be extracted
    uint32_t idx = INVALID_INDEX;
    // Grab the object virtual machine
    HSQUIRRELVM vm = field.GetVM();
    // Remember the current stack size
    const StackGuard sg(vm);
    // Push the specified object onto the stack
    Var< const Object & >::push(vm, field);
    // Identify the type of field was given
    switch  (field.GetType())
    {
        // Is this a string value?
        case OT_STRING:
        {
            // Obtain the object from the stack as a string
            StackStrF val(vm, -1);
            // Validate the result
            if (SQ_FAILED(val.Proc(false)))
            {
                STHROWF("{}", LastErrorString(vm));
            }
            // Is the obtained string empty?
            else if (val.mLen <= 0)
            {
                STHROWF("Cannot use an empty field name");
            }
            // Attempt to find a field with the specified name
            idx = m_Handle->GetFieldIndex(val.mPtr);
        } break;
        // Is this an integer value? (or at least can be easily converted to one)
        case OT_INTEGER:
        case OT_FLOAT:
        case OT_BOOL:
        {
            idx = ConvTo< uint32_t >::From(PopStackInteger(vm, -1));
        } break;
        // Is this an instance that we can extract either a string or integer from it?
        case OT_INSTANCE:
        {
            // Obtain the object from the stack as a string
            StackStrF val(vm, -1);
            // Validate the result
            if (SQ_FAILED(val.Proc(false)))
            {
                STHROWF("{}", LastErrorString(vm).c_str());
            }
            // Is the obtained string empty?
            else if (val.mLen <= 0)
            {
                STHROWF("Cannot use an empty field name");
            }
            // Check if this value is made only of digits
            else if (IsDigitsOnly(val.mPtr))
            {
                idx = ConvNum< uint32_t >::FromStr(val.mPtr);
            }
            // Attempt to find a field with the specified name
            else
            {
                idx = m_Handle->GetFieldIndex(val.mPtr);
            }
        } break;
        // We don't recognize this kind of value!
        default: STHROWF("Unknown field index of type ({})", SqTypeName(field.GetType()));
    }
    // Validate the obtained field index
    SQMOD_VALIDATE_FIELD(*this, idx);
    // Assign the new index
    m_Index = idx;
}

// ------------------------------------------------------------------------------------------------
Object MySQLField::GetResultSet() const // NOLINT(readability-convert-member-functions-to-static)
{
    return Object();
}

// ------------------------------------------------------------------------------------------------
Object MySQLField::GetConnection() const // NOLINT(readability-convert-member-functions-to-static)
{
    return Object();
}

// ------------------------------------------------------------------------------------------------
bool MySQLField::GetBoolean() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< bool >::From(m_Handle->mBinds[m_Index].mUint64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< bool >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQChar MySQLField::GetChar() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< SQChar >::From(m_Handle->mBinds[m_Index].mInt32[0]);
    }
    // Retrieve the value directly from the row
    return DbConvTo< SQChar >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQInteger MySQLField::GetInteger() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
#ifdef _SQ64
        return ConvTo< SQInteger >::From(m_Handle->mBinds[m_Index].mInt64);
#else
        return ConvTo< SQInteger >::From(m_Handle->mBinds[m_Index].mInt32[0]);
#endif  // _SQ64
    }
    // Retrieve the value directly from the row
    return DbConvTo< SQInteger >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQFloat MySQLField::GetFloat() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
#ifdef SQUSEDOUBLE
        return ConvTo< SQFloat >::From(m_Handle->mBinds[m_Index].mFloat64);
#else
        return ConvTo< SQFloat >::From(m_Handle->mBinds[m_Index].mFloat32[0]);
#endif // SQUSEDOUBLE
    }
    // Retrieve the value directly from the row
    return DbConvTo< SQFloat >::From(m_Handle->mRow[m_Index],
                                        m_Handle->mLengths[m_Index],
                                        m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQInteger MySQLField::GetInt8() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< int8_t >::From(m_Handle->mBinds[m_Index].mInt64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< int8_t >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQInteger MySQLField::GetUint8() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< uint8_t >::From(m_Handle->mBinds[m_Index].mInt64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< uint8_t >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQInteger MySQLField::GetInt16() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< int16_t >::From(m_Handle->mBinds[m_Index].mInt64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< int16_t >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQInteger MySQLField::GetUint16() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< uint16_t >::From(m_Handle->mBinds[m_Index].mInt64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< uint16_t >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQInteger MySQLField::GetInt32() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< int32_t >::From(m_Handle->mBinds[m_Index].mInt64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< int32_t >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQInteger MySQLField::GetUint32() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< uint32_t >::From(m_Handle->mBinds[m_Index].mInt64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< uint32_t >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQInteger MySQLField::GetInt64() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Obtain the initial stack size
    const StackGuard sg;
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< SQInteger >::From(m_Handle->mBinds[m_Index].mInt64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< SQInteger >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQInteger MySQLField::GetUint64() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Obtain the initial stack size
    const StackGuard sg;
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
      return ConvTo< SQInteger >::From(m_Handle->mBinds[m_Index].mUint64);
    }
    // Retrieve the value directly from the row
    return DbConvTo< SQInteger >::From(m_Handle->mRow[m_Index],
                                    m_Handle->mLengths[m_Index],
                                    m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQFloat MySQLField::GetFloat32() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return ConvTo< float >::From(m_Handle->mBinds[m_Index].mFloat32[0]);
    }
    // Retrieve the value directly from the row
    return DbConvTo< float >::From(m_Handle->mRow[m_Index],
                                        m_Handle->mLengths[m_Index],
                                        m_Handle->mFields[m_Index].type);
}

// ------------------------------------------------------------------------------------------------
SQFloat MySQLField::GetFloat64() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Should we retrieve the value from the bind wrapper?
    if (m_Handle->mStatement)
    {
        return static_cast<SQFloat>(ConvTo<double>::From(m_Handle->mBinds[m_Index].mFloat64));
    }
    // Retrieve the value directly from the row
    return static_cast<SQFloat>(DbConvTo<double>::From(m_Handle->mRow[m_Index],
                                                       m_Handle->mLengths[m_Index],
                                                       m_Handle->mFields[m_Index].type));
}

// ------------------------------------------------------------------------------------------------
Object MySQLField::GetString() const
{
    SQMOD_VALIDATE_STEPPED(*this);
    // Obtain the initial stack size
    const StackGuard sg;
    // Retrieve the value directly from the row and push it on the stack
    sq_pushstring(SqVM(), m_Handle->mRow[m_Index], m_Handle->mLengths[m_Index]);
    // Obtain the object from the stack
    Object str_o(-1, SqVM());
    // Restore the stack
    sg.Restore();
    // Return it the string object
    return str_o;
}

// ------------------------------------------------------------------------------------------------
Object MySQLField::GetBuffer() const // NOLINT(readability-convert-member-functions-to-static)
{
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
Object MySQLField::GetBlob() const // NOLINT(readability-convert-member-functions-to-static)
{
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
SQInteger MySQLResultSet::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMySQLResultSet");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void MySQLResultSet::Validate(const char * file, int32_t line) const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid MySQL result-set reference =>[{}:{}]"), file, line);
    }
}
#else
void MySQLResultSet::Validate() const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid MySQL result-set reference"));
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void MySQLResultSet::ValidateCreated(const char * file, int32_t line) const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid MySQL result-set reference =>[{}:{}]"), file, line);
    }
    else if (m_Handle->Access() == nullptr)
    {
        SqThrowF(SQMOD_RTFMT("Invalid MySQL result-set =>[{}:{}]"), file, line);
    }
}
#else
void MySQLResultSet::ValidateCreated() const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid MySQL result-set reference"));
    }
    else if (m_Handle->Access() == nullptr)
    {
        SqThrowF(fmt::runtime("Invalid MySQL result-set"));
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void MySQLResultSet::ValidateStepped(const char * file, int32_t line) const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid MySQL result-set reference =>[{}:{}]"), file, line);
    }
    // Do we have a valid row available?
    else if (m_Handle->mRow == nullptr)
    {
        SqThrowF(SQMOD_RTFMT("No row available in MySQL result-set =>[{}:{}]"), file, line);
    }
}
#else
void MySQLResultSet::ValidateStepped() const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid MySQL result-set reference"));
    }
    // Do we have a valid row available?
    else if (m_Handle->mRow == nullptr)
    {
        SqThrowF(fmt::runtime("No row available in MySQL result-set"));
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const MySQLResRef & MySQLResultSet::GetValid(const char * file, int32_t line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const MySQLResRef & MySQLResultSet::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const MySQLResRef & MySQLResultSet::GetCreated(const char * file, int32_t line) const
{
    ValidateCreated(file, line);
    return m_Handle;
}
#else
const MySQLResRef & MySQLResultSet::GetCreated() const
{
    ValidateCreated();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const MySQLResRef & MySQLResultSet::GetStepped(const char * file, int32_t line) const
{
    ValidateStepped(file, line);
    return m_Handle;
}
#else
const MySQLResRef & MySQLResultSet::GetStepped() const
{
    ValidateStepped();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void MySQLResultSet::ValidateField(int32_t idx, const char * file, int32_t line) const
{
    ValidateCreated(file, line);
    m_Handle->ValidateField(idx, file, line);
}
#else
void MySQLResultSet::ValidateField(int32_t idx) const
{
    ValidateCreated();
    m_Handle->ValidateField(static_cast<uint32_t>(idx));
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
Array MySQLResultSet::GetFieldNames() const
{
    SQMOD_VALIDATE_CREATED(*this);
    // Grab the number of available fields
    const SQInteger field_count = ConvTo< SQInteger >::From(m_Handle->mFieldCount);
    // Grab the array with field instances
    const MySQLResHnd::FieldType * fields = m_Handle->mFields;
    // Is there even something to process?
    if (!field_count || !fields)
    {
        return Array(SqVM(), 0);
    }
    // Allocate an array with the same amount of elements as the number of fields
    Array arr(SqVM(), field_count);
    // Iterate over all the available fields and insert them into the created array
    for (SQInteger n = 0; n < field_count; ++n)
    {
        arr.SetValue(n, (fields[n].name == nullptr) ? fmt::format("<field_{}>", n).c_str() : fields[n].name);
    }
    // Return the resulted array
    return arr;
}

// ------------------------------------------------------------------------------------------------
Array MySQLResultSet::GetFieldsArray() const
{
    SQMOD_VALIDATE_CREATED(*this);
    // Grab the number of available fields
    const SQInteger field_count = ConvTo< SQInteger >::From(m_Handle->mFieldCount);
    // Is there even something to process?
    if (!field_count)
    {
        return Array(SqVM(), 0);
    }
    // Create a field instance to insert as copy
    MySQLField field(m_Handle);
    // Allocate an array with the same amount of elements as the number of fields
    Array arr(SqVM(), field_count);
    // Iterate over all the available fields and insert them into the created array
    for (SQInteger n = 0; n < field_count; ++n)
    {
        // Update the field index
        field.SetIndex(ConvTo<uint32_t>::From(n));
        // Insert a copy of the field instance into the array
        arr.SetValue(n, field);
    }
    // Return the resulted array
    return arr;
}

// ------------------------------------------------------------------------------------------------
Array MySQLResultSet::FetchFieldsArray(Array & fields) const
{
    SQMOD_VALIDATE_CREATED(*this);
    // Is there even something to process?
    if (!m_Handle->mFieldCount || fields.Length() == 0)
    {
        return Array(SqVM(), 0);
    }
    // Create a field instance to insert as copy
    MySQLField field(m_Handle);
    // Allocate an array with the same amount of elements as the number of fields
    Array arr(SqVM(), fields.Length());
    // Iterate the specified fields array
    fields.Foreach([&field, &arr](HSQUIRRELVM vm, SQInteger i) -> SQRESULT {
        // Update the field index
        field.SetIndex(Object(-1, vm));
        // Insert a copy of the field instance into the array
        arr.SetValue(i, field);
        // We're good
        return SQ_OK;
    });
    // Return the resulted array
    return arr;
}

// ------------------------------------------------------------------------------------------------
Table MySQLResultSet::GetFieldsTable() const
{
    SQMOD_VALIDATE_CREATED(*this);
    // Grab the number of available fields
    const SQInteger field_count = ConvTo< SQInteger >::From(m_Handle->mFieldCount);
    // Grab the array with field instances
    const MySQLResHnd::FieldType * fields = m_Handle->mFields;
    // Is there even something to process?
    if (!field_count || !fields)
    {
        return Table();
    }
    // Create a field instance to insert as copy
    MySQLField field(m_Handle);
    // Allocate a table to be populated with field instances
    Table tbl(SqVM(), field_count);
    // Iterate over all the available fields and insert them into the created table
    for (SQInteger n = 0; n < field_count; ++n)
    {
        // Update the field index
        field.SetIndex(static_cast< uint32_t >(ConvTo<int32_t>::From(n)));
        // Insert a copy of the field instance into the table
        tbl.SetValue((fields[n].name == nullptr) ? fmt::format("<field_{}>", n).c_str() : fields[n].name, field);
    }
    // Return the resulted table
    return tbl;
}

// ------------------------------------------------------------------------------------------------
Table MySQLResultSet::FetchFieldsTable(Array & fields) const
{
    SQMOD_VALIDATE_CREATED(*this);
    // Is there even something to process?
    if (!m_Handle->mFieldCount || fields.Length() == 0)
    {
        return Table();
    }
    // Create a field instance to insert as copy
    MySQLField field(m_Handle);
    // Allocate a table to be populated with field instances
    Table tbl(SqVM(), fields.Length());
    // Grab the array with field instances
    const MySQLResHnd::FieldType * fields_ptr = m_Handle->mFields;
    // Iterate the specified fields array
    fields.Foreach([&field, &tbl, fields_ptr](HSQUIRRELVM vm, SQInteger i) -> SQRESULT {
        // Update the field index
        field.SetIndex(Object(-1, vm));
        // Insert a copy of the field instance into the table
        tbl.SetValue((fields_ptr[field.GetIndex()].name == nullptr) ? fmt::format("<field_{}>", field.GetIndex()).c_str() : fields_ptr[field.GetIndex()].name, field);
        // We're good
        return SQ_OK;
    });
    // Return the resulted array
    return tbl;
}

// ------------------------------------------------------------------------------------------------
SQInteger MySQLStatement::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMySQLStatement");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void MySQLStatement::Validate(const char * file, int32_t line) const
{
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid MySQL statement reference =>[{}:{}]"), file, line);
    }
}
#else
void MySQLStatement::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid MySQL statement reference"));
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void MySQLStatement::ValidateCreated(const char * file, int32_t line) const
{
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid MySQL statement reference =>[{}:{}]"), file, line);
    }
    else if (m_Handle->Access() == nullptr)
    {
        SqThrowF(SQMOD_RTFMT("Invalid MySQL statement =>[{}:{}]"), file, line);
    }
}
#else
void MySQLStatement::ValidateCreated() const
{
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid MySQL statement reference"));
    }
    else if (m_Handle->Access() == nullptr)
    {
        SqThrowF(fmt::runtime("Invalid MySQL statement"));
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const MySQLStmtRef & MySQLStatement::GetValid(const char * file, int32_t line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const MySQLStmtRef & MySQLStatement::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const MySQLStmtRef & MySQLStatement::GetCreated(const char * file, int32_t line) const
{
    ValidateCreated(file, line);
    return m_Handle;
}
#else
const MySQLStmtRef & MySQLStatement::GetCreated() const
{
    ValidateCreated();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void MySQLStatement::ValidateParam(int32_t idx, const char * file, int32_t line) const
{
    ValidateCreated(file, line);
    m_Handle->ValidateParam(idx, file, line);
}
#else
void MySQLStatement::ValidateParam(int32_t idx) const
{
    ValidateCreated();
    m_Handle->ValidateParam(static_cast< uint32_t >(idx));
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
MySQLStatement::MySQLStatement(const MySQLConnection & connection, const SQChar * query)
    : MySQLStatement(connection.GetHandle(), query)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
MySQLConnection MySQLStatement::GetConnection() const
{
    return MySQLConnection(SQMOD_GET_VALID(*this)->mConnection);
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetConnection(const MySQLConnection & conn)
{
    SQMOD_GET_VALID(*this)->mConnection = conn.GetHandle();
}

// ------------------------------------------------------------------------------------------------
int32_t MySQLStatement::Execute()
{
    // Attempt to bind the parameters
    if (mysql_stmt_bind_param(SQMOD_GET_CREATED(*this)->Access(), m_Handle->mMyBinds))
    {
        SQMOD_THROW_CURRENT(*m_Handle, "Cannot bind MySQL statement parameters");
    }
    // Attempt to execute the statement
    else if (mysql_stmt_execute(m_Handle->Access()))
    {
        SQMOD_THROW_CURRENT(*m_Handle, "Cannot execute MySQL statement");
    }
    // Return the number of rows affected by this query
    return static_cast< int32_t >(mysql_stmt_affected_rows(m_Handle->Access()));
}

// ------------------------------------------------------------------------------------------------
uint32_t MySQLStatement::Insert()
{
    // Attempt to bind the parameters
    if (mysql_stmt_bind_param(SQMOD_GET_CREATED(*this)->Access(), m_Handle->mMyBinds))
    {
        SQMOD_THROW_CURRENT(*m_Handle, "Cannot bind MySQL statement parameters");
    }
    // Attempt to execute the statement
    else if (mysql_stmt_execute(m_Handle->Access()))
    {
        SQMOD_THROW_CURRENT(*m_Handle, "Cannot execute MySQL statement");
    }
    // Return the identifier of the inserted row
    return static_cast< uint32_t >(mysql_stmt_insert_id(m_Handle->Access()));
}

// ------------------------------------------------------------------------------------------------
MySQLResultSet MySQLStatement::Query()
{
    // Attempt to bind the parameters
    if (mysql_stmt_bind_param(SQMOD_GET_CREATED(*this)->Access(), m_Handle->mMyBinds))
    {
        SQMOD_THROW_CURRENT(*m_Handle, "Cannot bind MySQL statement parameters");
    }
    // Attempt to execute the statement
    else if (mysql_stmt_execute(m_Handle->Access()))
    {
        SQMOD_THROW_CURRENT(*m_Handle, "Cannot execute MySQL statement");
    }
    // Return the results of this query
    return MySQLResultSet(m_Handle);
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetInt8(uint32_t idx, SQInteger val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_TINY, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mInt64 = ConvTo< int8_t >::From(val); // NOLINT(cert-str34-c)
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetUint8(uint32_t idx, SQInteger val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_TINY, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mUint64 = ConvTo< uint8_t >::From(val);
    // Specify that this value is unsigned
    m_Handle->mMyBinds[idx].is_unsigned = true;
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetInt16(uint32_t idx, SQInteger val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_SHORT, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mInt64 = ConvTo< int16_t >::From(val);
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetUint16(uint32_t idx, SQInteger val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_SHORT, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mUint64 = ConvTo< uint16_t >::From(val);
    // Specify that this value is unsigned
    m_Handle->mMyBinds[idx].is_unsigned = true;
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetInt32(uint32_t idx, SQInteger val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_LONG, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mInt64 = ConvTo< int32_t >::From(val);
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetUint32(uint32_t idx, SQInteger val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_LONG, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mUint64 = ConvTo< uint32_t >::From(val);
    // Specify that this value is unsigned
    m_Handle->mMyBinds[idx].is_unsigned = true;
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetInt64(uint32_t idx, SQInteger val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_LONGLONG, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mInt64 = ConvTo< int64_t >::From(val);
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetUint64(uint32_t idx, SQInteger val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_LONGLONG, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mUint64 = ConvTo< uint64_t >::From(val);
    // Specify that this value is unsigned
    m_Handle->mMyBinds[idx].is_unsigned = true;
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetSLongInt(uint32_t idx, SQInteger val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_LONGLONG, &(m_Handle->mMyBinds[idx]));
    // Attempt to assign the numeric value inside the specified object
    m_Handle->mBinds[idx].mInt64 = val;
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetULongInt(uint32_t idx, SQInteger val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_LONGLONG, &(m_Handle->mMyBinds[idx]));
    // Attempt to assign the numeric value inside the specified object
    m_Handle->mBinds[idx].mUint64 = static_cast< uint64_t >(val);
    // Specify that this value is unsigned
    m_Handle->mMyBinds[idx].is_unsigned = true;
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetInteger(uint32_t idx, SQInteger val) const
{
#ifdef _SQ64
    SetInt64(idx, val);
#else
    SetInt32(idx, val);
#endif // _SQ64
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetFloat32(uint32_t idx, SQFloat val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_FLOAT, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mFloat32[0] = ConvTo< float >::From(val);
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetFloat64(uint32_t idx, SQFloat val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_DOUBLE, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mFloat64 = ConvTo< double >::From(val);
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetFloat(uint32_t idx, SQFloat val) const
{
#ifdef SQUSEDOUBLE
    SetFloat64(idx, val);
#else
    SetFloat32(idx, val);
#endif // SQUSEDOUBLE
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetBoolean(uint32_t idx, bool val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_TINY, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mUint64 = static_cast< uint64_t >(val);
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetDate(uint32_t idx, const Date & val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_DATE, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    SqDateToMySQLTime(val, m_Handle->mBinds[idx].mTime);
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetTime(uint32_t idx, const Time & val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_TIME, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    SqTimeToMySQLTime(val, m_Handle->mBinds[idx].mTime);
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetDatetime(uint32_t idx, const Datetime & val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_DATETIME, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    SqDatetimeToMySQLTime(val, m_Handle->mBinds[idx].mTime);
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetString(uint32_t idx, const SQChar * val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_STRING, &(m_Handle->mMyBinds[idx]), val,
                                   static_cast<unsigned long>(std::strlen(val)));
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetEnum(uint32_t idx, const SQChar * val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_ENUM, &(m_Handle->mMyBinds[idx]), val,
                                   static_cast<unsigned long>(std::strlen(val)));
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetSet(uint32_t idx, const SQChar * val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_SET, &(m_Handle->mMyBinds[idx]), val,
                                   static_cast<unsigned long>(std::strlen(val)));
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetBlob(uint32_t /*idx*/, Object & /*val*/) const
{
    // TODO: implement
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetData(uint32_t /*idx*/, Object & /*val*/) const
{
    // TODO: implement
}

// ------------------------------------------------------------------------------------------------
void MySQLStatement::SetNull(uint32_t idx) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_NULL, &(m_Handle->mMyBinds[idx]));
}

// ================================================================================================
void Register_MySQL(HSQUIRRELVM vm)
{
    Table sqlns(vm);

    sqlns.Bind(_SC("MySQLAccount")
        , Class< MySQLAccount >(sqlns.GetVM(), _SC("SqMySQLAccount"))
        // Constructors
        .Ctor< const MySQLAccount & >()
        .Ctor< const SQChar *, const SQChar * >()
        .Ctor< const SQChar *, const SQChar *, const SQChar * >()
        .Ctor< const SQChar *, const SQChar *, const SQChar *, const SQChar * >()
        .Ctor< const SQChar *, const SQChar *, const SQChar *, const SQChar *, SQInteger >()
        .Ctor< const SQChar *, const SQChar *, const SQChar *, const SQChar *, SQInteger, const SQChar * >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &MySQLAccount::Cmp)
        .SquirrelFunc(_SC("_typename"), &MySQLAccount::Typename)
        .Func(_SC("_tostring"), &MySQLAccount::ToString)
        // Properties
        .Prop(_SC("Port"), &MySQLAccount::GetPortNum, &MySQLAccount::SetPortNum)
        .Prop(_SC("Host"), &MySQLAccount::GetHost, &MySQLAccount::SetHost)
        .Prop(_SC("User"), &MySQLAccount::GetUser, &MySQLAccount::SetUser)
        .Prop(_SC("Pass"), &MySQLAccount::GetPass, &MySQLAccount::SetPass)
        .Prop(_SC("Socket"), &MySQLAccount::GetSocket, &MySQLAccount::SetSocket)
        .Prop(_SC("Flags"), &MySQLAccount::GetFlags, &MySQLAccount::SetFlags)
        .Prop(_SC("SSL_Key"), &MySQLAccount::GetSSL_Key, &MySQLAccount::SetSSL_Key)
        .Prop(_SC("SSL_Cert"), &MySQLAccount::GetSSL_Cert, &MySQLAccount::SetSSL_Cert)
        .Prop(_SC("SSL_CA"), &MySQLAccount::GetSSL_CA, &MySQLAccount::SetSSL_CA)
        .Prop(_SC("SSL_CA_Path"), &MySQLAccount::GetSSL_CA_Path, &MySQLAccount::SetSSL_CA_Path)
        .Prop(_SC("SSL_Cipher"), &MySQLAccount::GetSSL_Cipher, &MySQLAccount::SetSSL_Cipher)
        .Prop(_SC("AutoCommit"), &MySQLAccount::GetAutoCommit, &MySQLAccount::SetAutoCommit)
        .Prop(_SC("Options"), &MySQLAccount::GetOptionsTable)
        .Prop(_SC("OptionsCount"), &MySQLAccount::OptionsCount)
        .Prop(_SC("OptionsEmpty"), &MySQLAccount::OptionsEmpty)
        // Member Methods
        .Func(_SC("EnableFlags"), &MySQLAccount::EnableFlags)
        .Func(_SC("DisableFlags"), &MySQLAccount::DisableFlags)
        .Func(_SC("SetSSL"), &MySQLAccount::SetSSL)
        .Func(_SC("GetOption"), &MySQLAccount::GetOption)
        .Func(_SC("SetOption"), &MySQLAccount::SetOption)
        .Func(_SC("RemoveOption"), &MySQLAccount::RemoveOption)
        .Func(_SC("OptionsClear"), &MySQLAccount::OptionsClear)
        .Func(_SC("Connect"), &MySQLAccount::Connect)
    );

    sqlns.Bind(_SC("Connection")
        , Class< MySQLConnection >(sqlns.GetVM(), _SC("SqMySQLConnection"))
        // Constructors
        .Ctor()
        .Ctor< const MySQLAccount & >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &MySQLConnection::Cmp)
        .SquirrelFunc(_SC("_typename"), &MySQLConnection::Typename)
        .Func(_SC("_tostring"), &MySQLConnection::ToString)
        // Properties
        .Prop(_SC("IsValid"), &MySQLConnection::IsValid)
        .Prop(_SC("Connected"), &MySQLConnection::IsConnected)
        .Prop(_SC("References"), &MySQLConnection::GetRefCount)
        .Prop(_SC("ErrNo"), &MySQLConnection::GetErrNo)
        .Prop(_SC("ErrStr"), &MySQLConnection::GetErrStr)
        .Prop(_SC("LastErrNo"), &MySQLConnection::GetLastErrNo)
        .Prop(_SC("LastErrStr"), &MySQLConnection::GetLastErrStr)
        .Prop(_SC("Port"), &MySQLConnection::GetPortNum)
        .Prop(_SC("Host"), &MySQLConnection::GetHost)
        .Prop(_SC("User"), &MySQLConnection::GetUser)
        .Prop(_SC("Pass"), &MySQLConnection::GetPass)
        .Prop(_SC("Name"), &MySQLConnection::GetName, &MySQLConnection::SetName)
        .Prop(_SC("Socket"), &MySQLConnection::GetSocket)
        .Prop(_SC("Flags"), &MySQLConnection::GetFlags)
        .Prop(_SC("SSL_Key"), &MySQLConnection::GetSSL_Key)
        .Prop(_SC("SSL_Cert"), &MySQLConnection::GetSSL_Cert)
        .Prop(_SC("SSL_CA"), &MySQLConnection::GetSSL_CA)
        .Prop(_SC("SSL_CA_Path"), &MySQLConnection::GetSSL_CA_Path)
        .Prop(_SC("SSL_Cipher"), &MySQLConnection::GetSSL_Cipher)
        .Prop(_SC("Charset"), &MySQLConnection::GetCharset, &MySQLConnection::SetCharset)
        .Prop(_SC("AutoCommit"), &MySQLConnection::GetAutoCommit, &MySQLConnection::SetAutoCommit)
        .Prop(_SC("InTransaction"), &MySQLConnection::GetInTransaction)
        // Member Methods
        .Func(_SC("Disconnect"), &MySQLConnection::Disconnect)
        .Func(_SC("SelectDb"), &MySQLConnection::SetName)
        .Func(_SC("Execute"), &MySQLConnection::Execute)
        .Func(_SC("Insert"), &MySQLConnection::Insert)
        .Func(_SC("Query"), &MySQLConnection::Query)
        .Func(_SC("Statement"), &MySQLConnection::GetStatement)
        //.Func(_SC("Transaction"), &MySQLConnection::GetTransaction)
        .FmtFunc(_SC("EscapeString"), &MySQLConnection::EscapeString)
        // Squirrel Methods
        .SquirrelFunc(_SC("ExecuteF"), &MySQLConnection::ExecuteF)
        .SquirrelFunc(_SC("InsertF"), &MySQLConnection::InsertF)
        .SquirrelFunc(_SC("QueryF"), &MySQLConnection::QueryF)
    );

    sqlns.Bind(_SC("Field"),
        Class< MySQLField >(sqlns.GetVM(), _SC("SqMySQLField"))
        // Constructors
        .Ctor()
        .Ctor< const MySQLField & >()
        // Meta-methods
        .Func(_SC("_cmp"), &MySQLField::Cmp)
        .SquirrelFunc(_SC("_typename"), &MySQLField::Typename)
        .Func(_SC("_tostring"), &MySQLField::ToString)
        // Properties
        .Prop(_SC("IsValid"), &MySQLField::IsValid)
        .Prop(_SC("References"), &MySQLField::GetRefCount)
        .Prop(_SC("Index"), &MySQLField::GetIndex)
        .Prop(_SC("ResultSet"), &MySQLField::GetResultSet)
        .Prop(_SC("Connection"), &MySQLField::GetConnection)
        .Prop(_SC("Bool"), &MySQLField::GetBoolean)
        .Prop(_SC("Boolean"), &MySQLField::GetBoolean)
        .Prop(_SC("Char"), &MySQLField::GetChar)
        .Prop(_SC("Integer"), &MySQLField::GetInteger)
        .Prop(_SC("Float"), &MySQLField::GetFloat)
        .Prop(_SC("int8_t"), &MySQLField::GetInt8)
        .Prop(_SC("uint8_t"), &MySQLField::GetUint8)
        .Prop(_SC("int16_t"), &MySQLField::GetInt16)
        .Prop(_SC("uint16_t"), &MySQLField::GetUint16)
        .Prop(_SC("int32_t"), &MySQLField::GetInt32)
        .Prop(_SC("uint32_t"), &MySQLField::GetUint32)
        .Prop(_SC("int64_t"), &MySQLField::GetInt64)
        .Prop(_SC("uint64_t"), &MySQLField::GetUint64)
        .Prop(_SC("float"), &MySQLField::GetFloat32)
        .Prop(_SC("double"), &MySQLField::GetFloat64)
        .Prop(_SC("String"), &MySQLField::GetString)
        .Prop(_SC("Buffer"), &MySQLField::GetBuffer)
        .Prop(_SC("Blob"), &MySQLField::GetBlob)
        // Member Methods
        .Func(_SC("Release"), &MySQLField::Release)
    );

    sqlns.Bind(_SC("ResultSet")
        , Class< MySQLResultSet >(sqlns.GetVM(), _SC("SqMySQLResultSet"))
        // Constructors
        .Ctor()
        .Ctor< const MySQLResultSet & >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &MySQLResultSet::Cmp)
        .SquirrelFunc(_SC("_typename"), &MySQLResultSet::Typename)
        .Func(_SC("_tostring"), &MySQLResultSet::ToString)
        // Properties
        .Prop(_SC("IsValid"), &MySQLResultSet::IsValid)
        .Prop(_SC("FieldNames"), &MySQLResultSet::GetFieldNames)
        .Prop(_SC("FieldsArray"), &MySQLResultSet::GetFieldsArray)
        .Prop(_SC("FieldsTable"), &MySQLResultSet::GetFieldsTable)
        .Prop(_SC("RowIndex"), &MySQLResultSet::RowIndex)
        .Prop(_SC("RowCount"), &MySQLResultSet::RowCount)
        // Member Methods
        .Func(_SC("Next"), &MySQLResultSet::Next)
        .Func(_SC("Step"), &MySQLResultSet::Next)
        .Func(_SC("SetRowIndex"), &MySQLResultSet::SetRowIndex)
        .Func(_SC("SetLongRowIndex"), &MySQLResultSet::SetLongRowIndex)
        .Func(_SC("Get"), &MySQLResultSet::GetField)
        .Func(_SC("GetField"), &MySQLResultSet::GetField)
        .Func(_SC("GetBool"), &MySQLResultSet::GetBoolean)
        .Func(_SC("GetBoolean"), &MySQLResultSet::GetBoolean)
        .Func(_SC("GetChar"), &MySQLResultSet::GetChar)
        .Func(_SC("GetInteger"), &MySQLResultSet::GetInteger)
        .Func(_SC("GetFloat"), &MySQLResultSet::GetFloat)
        .Func(_SC("GetInt8"), &MySQLResultSet::GetInt8)
        .Func(_SC("GetUint8"), &MySQLResultSet::GetUint8)
        .Func(_SC("GetInt16"), &MySQLResultSet::GetInt16)
        .Func(_SC("GetUint16"), &MySQLResultSet::GetUint16)
        .Func(_SC("GetInt32"), &MySQLResultSet::GetInt32)
        .Func(_SC("GetUint32"), &MySQLResultSet::GetUint32)
        .Func(_SC("GetInt64"), &MySQLResultSet::GetInt64)
        .Func(_SC("GetUint64"), &MySQLResultSet::GetUint64)
        .Func(_SC("GetFloat32"), &MySQLResultSet::GetFloat32)
        .Func(_SC("GetFloat64"), &MySQLResultSet::GetFloat64)
        .Func(_SC("GetString"), &MySQLResultSet::GetString)
        .Func(_SC("GetBuffer"), &MySQLResultSet::GetBuffer)
        .Func(_SC("GetBlob"), &MySQLResultSet::GetBlob)
        .Func(_SC("GetFieldsArray"), &MySQLResultSet::FetchFieldsArray)
        .Func(_SC("GetFieldsTable"), &MySQLResultSet::FetchFieldsTable)
    );

    sqlns.Bind(_SC("Statement")
        , Class< MySQLStatement >(sqlns.GetVM(), _SC("SqMySQLStatement"))
        // Constructors
        .Ctor()
        .Ctor< const MySQLStatement & >()
        .Ctor< const MySQLConnection &, const SQChar * >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &MySQLStatement::Cmp)
        .SquirrelFunc(_SC("_typename"), &MySQLStatement::Typename)
        .Func(_SC("_tostring"), &MySQLStatement::ToString)
        // Properties
        .Prop(_SC("IsValid"), &MySQLStatement::IsValid)
        .Prop(_SC("Connection"), &MySQLStatement::GetConnection, &MySQLStatement::SetConnection)
        // Member Methods
        .Func(_SC("Execute"), &MySQLStatement::Execute)
        .Func(_SC("Insert"), &MySQLStatement::Insert)
        .Func(_SC("Query"), &MySQLStatement::Query)
        .Func(_SC("SetInt8"), &MySQLStatement::SetInt8)
        .Func(_SC("SetUint8"), &MySQLStatement::SetUint8)
        .Func(_SC("SetInt16"), &MySQLStatement::SetInt16)
        .Func(_SC("SetUint16"), &MySQLStatement::SetUint16)
        .Func(_SC("SetInt32"), &MySQLStatement::SetInt32)
        .Func(_SC("SetUint32"), &MySQLStatement::SetUint32)
        .Func(_SC("SetInt64"), &MySQLStatement::SetInt64)
        .Func(_SC("SetUint64"), &MySQLStatement::SetUint64)
        .Func(_SC("SetSLongInt"), &MySQLStatement::SetSLongInt)
        .Func(_SC("SetULongInt"), &MySQLStatement::SetULongInt)
        .Func(_SC("SetInteger"), &MySQLStatement::SetInteger)
        .Func(_SC("SetFloat32"), &MySQLStatement::SetFloat32)
        .Func(_SC("SetFloat64"), &MySQLStatement::SetFloat64)
        .Func(_SC("SetFloat"), &MySQLStatement::SetFloat)
        .Func(_SC("SetBoolean"), &MySQLStatement::SetBoolean)
        .Func(_SC("SetDate"), &MySQLStatement::SetDate)
        .Func(_SC("SetTime"), &MySQLStatement::SetTime)
        .Func(_SC("SetDatetime"), &MySQLStatement::SetDatetime)
        .Func(_SC("SetString"), &MySQLStatement::SetString)
        .Func(_SC("SetEnum"), &MySQLStatement::SetEnum)
        .Func(_SC("SetSet"), &MySQLStatement::SetSet)
        .Func(_SC("SetBlob"), &MySQLStatement::SetBlob)
        .Func(_SC("SetData"), &MySQLStatement::SetData)
        .Func(_SC("SetBuffer"), &MySQLStatement::SetData)
        .Func(_SC("SetNull"), &MySQLStatement::SetNull)
    );

    RootTable(vm).Bind(_SC("MySQL"), sqlns);
}

} // Namespace:: SqMod
