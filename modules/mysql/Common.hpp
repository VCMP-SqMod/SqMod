#ifndef _SQMYSQL_COMMON_HPP_
#define _SQMYSQL_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <mysql.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * SOFTWARE INFORMATION
*/
#define SQMYSQL_NAME "Squirrel MySQL Module"
#define SQMYSQL_AUTHOR "Sandu Liviu Catalin (S.L.C)"
#define SQMYSQL_COPYRIGHT "Copyright (C) 2016 Sandu Liviu Catalin"
#define SQMYSQL_HOST_NAME "SqModMySQLHost"
#define SQMYSQL_VERSION 001
#define SQMYSQL_VERSION_STR "0.0.1"
#define SQMYSQL_VERSION_MAJOR 0
#define SQMYSQL_VERSION_MINOR 0
#define SQMYSQL_VERSION_PATCH 1

/* ------------------------------------------------------------------------------------------------
 * Handle validation.
*/
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    #define THROW_CURRENT(x, a) x.GetHnd().ThrowCurrent(a, __FILE__, __LINE__)
    #define THROW_CURRENT_HND(x, a) x.ThrowCurrent(a, __FILE__, __LINE__)
#else
    #define THROW_CURRENT(x, a) x.GetHnd().ThrowCurrent(a)
    #define THROW_CURRENT_HND(x, a) x.ThrowCurrent(a)
#endif // _DEBUG

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
class Account;
class Column;
class Connection;
class ResultSet;
class Statement;
class Transaction;

} // Namespace:: SqMod

#endif // _SQMYSQL_COMMON_HPP_
