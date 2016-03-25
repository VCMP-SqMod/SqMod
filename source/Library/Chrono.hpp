#ifndef _LIBRARY_CHRONO_HPP_
#define _LIBRARY_CHRONO_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Retrieve the current time as microseconds.
*/
Int64 GetCurrentSysTime();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the epoch time as microseconds.
*/
Int64 GetEpochTimeMicro();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the epoch time as milliseconds.
*/
Int64 GetEpochTimeMilli();

} // Namespace:: SqMod

#endif // _LIBRARY_CHRONO_HPP_