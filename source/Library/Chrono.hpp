#ifndef _LIBRARY_CHRONO_HPP_
#define _LIBRARY_CHRONO_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

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

    // ------------------------------------------------------------------------------------------------
    static const Uint8  MonthLengths[12];

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
    static Int64 GetCurrentSysTime();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the epoch time as microseconds.
    */
    static Int64 GetEpochTimeMicro();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the epoch time as milliseconds.
    */
    static Int64 GetEpochTimeMilli();

    /* --------------------------------------------------------------------------------------------
     * See whether the specified date is valid.
    */
    static bool ValidDate(Uint16 year, Uint8 month, Uint8 day);

    /* --------------------------------------------------------------------------------------------
     * See whether the specified year is a leap year.
    */
    static bool IsLeapYear(Uint16 year)
    {
        return !(year % 400) || (!(year % 4) && (year % 100));
    }

    /* --------------------------------------------------------------------------------------------
     * retrieve the number of days in the specified year.
    */
    static Uint16 DaysInYear(Uint16 year)
    {
        return IsLeapYear(year) ? 366 : 365;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of days in the specified month.
    */
    static Uint8 DaysInMonth(Uint16 year, Uint8 month);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number/position of the specified day in the specified year and month.
    */
    static Uint16 DayOfYear(Uint16 year, Uint8 month, Uint8 day);

    /* --------------------------------------------------------------------------------------------
     * Convert just the year and day of year to full date.
    */
    static Date ReverseDayOfyear(Uint16 year, Uint16 doy);

    /* --------------------------------------------------------------------------------------------
     * Calculate the number of days in the specified date range.
    */
    static Int64 DateRangeToSeconds(Uint16 _year, Uint8 _month, Uint8 _day, Uint16 year_, Uint8 month_, Uint8 day_);
};

} // Namespace:: SqMod

#endif // _LIBRARY_CHRONO_HPP_