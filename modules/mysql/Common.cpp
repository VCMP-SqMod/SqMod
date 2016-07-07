// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void SqDateToMySQLTime(Object & obj, MYSQL_TIME & t)
{
    // The resulted date values
    uint16_t year = 0;
    uint8_t month = 0, day = 0;
    {
        // Obtain the initial stack size
        const StackGuard sg(_SqVM);
        // Push the specified object onto the stack
        Var< Object >::push(_SqVM, obj);
        // Attempt to get the values inside the specified object
        if (SQ_FAILED(SqMod_GetDate(_SqVM, -1, &year, &month, &day)))
        {
            STHROWF("Invalid date specified");
        }
    }
    // Populate the given structure
    t.year = year;
    t.month = month;
    t.day = day;
    t.hour = 0;
    t.minute = 0;
    t.second = 0;
    t.neg = false;
    t.second_part = 0;
    t.time_type = MYSQL_TIMESTAMP_DATE;
}

// ------------------------------------------------------------------------------------------------
void SqTimeToMySQLTime(Object & obj, MYSQL_TIME & t)
{
    // The resulted time values
    uint8_t hour = 0, minute = 0, second = 0;
    uint16_t milli = 0;
    {
        // Obtain the initial stack size
        const StackGuard sg(_SqVM);
        // Push the specified object onto the stack
        Var< Object >::push(_SqVM, obj);
        // Attempt to get the values inside the specified object
        if (SQ_FAILED(SqMod_GetTime(_SqVM, -1, &hour, &minute, &second, &milli)))
        {
            STHROWF("Invalid time specified");
        }
    }
    // Populate the given structure
    t.year = 0;
    t.month = 0;
    t.day = 0;
    t.neg = false;
    t.second_part = (milli * 1000L);
    t.time_type = MYSQL_TIMESTAMP_TIME;
}

// ------------------------------------------------------------------------------------------------
void SqDatetimeToMySQLTime(Object & obj, MYSQL_TIME & t)
{
    // The resulted date values
    uint16_t year = 0, milli = 0;
    uint8_t month = 0, day = 0,  hour = 0, minute = 0, second = 0;
    {
        // Obtain the initial stack size
        const StackGuard sg(_SqVM);
        // Push the specified object onto the stack
        Var< Object >::push(_SqVM, obj);
        // Attempt to get the values inside the specified object
        if (SQ_FAILED(SqMod_GetDatetime(_SqVM, -1, &year, &month, &day, &hour, &minute, &second, &milli)))
        {
            STHROWF("Invalid date and time specified");
        }
    }
    // Populate the given structure
    t.year = year;
    t.month = month;
    t.day = day;
    t.hour = hour;
    t.minute = minute;
    t.second = second;
    t.neg = false;
    t.second_part = (milli * 1000L);
    t.time_type = MYSQL_TIMESTAMP_DATETIME;
}

} // Namespace:: SqMod
