#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
class Date;
class Time;
class Datetime;
class Timer;
class Timestamp;

/* ------------------------------------------------------------------------------------------------
 * Class that offers helpers to work with time related information.
*/
class Chrono
{
public:

    // --------------------------------------------------------------------------------------------
    static const uint8_t  MonthLengths[12];

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (disabled)
    */
    Chrono() = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Chrono(const Chrono & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    Chrono(Chrono && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor. (disabled)
    */
    ~Chrono() = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Chrono & operator = (const Chrono & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Chrono & operator = (Chrono && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current time as microseconds.
    */
    static int64_t GetCurrentSysTime();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the epoch time as microseconds.
    */
    static int64_t GetEpochTimeMicro();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the epoch time as milliseconds.
    */
    static int64_t GetEpochTimeMilli();

    /* --------------------------------------------------------------------------------------------
     * See whether the specified date is valid.
    */
    static bool ValidDate(uint16_t year, uint8_t month, uint8_t day);

    /* --------------------------------------------------------------------------------------------
     * See whether the specified year is a leap year.
    */
    static bool IsLeapYear(uint16_t year)
    {
        return !(year % 400) || (!(year % 4) && (year % 100));
    }

    /* --------------------------------------------------------------------------------------------
     * retrieve the number of days in the specified year.
    */
    static uint16_t DaysInYear(uint16_t year)
    {
        return IsLeapYear(year) ? 366 : 365;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of days in the specified month.
    */
    static uint8_t DaysInMonth(uint16_t year, uint8_t month);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number/position of the specified day in the specified year and month.
    */
    static uint16_t DayOfYear(uint16_t year, uint8_t month, uint8_t day);

    /* --------------------------------------------------------------------------------------------
     * Convert just the year and day of year to full date.
    */
    static Date ReverseDayOfYear(uint16_t year, uint16_t doy);

    /* --------------------------------------------------------------------------------------------
     * Calculate the number of days in the specified date range.
    */
    static int64_t DateRangeToSeconds(uint16_t _year, uint8_t _month, uint8_t _day, uint16_t year_, uint8_t month_, uint8_t day_);
};

} // Namespace:: SqMod
