#ifndef _SQMYSQL_COMMON_HPP_
#define _SQMYSQL_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "Convert.hpp"

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
    #define SQMOD_THROW_CURRENT(x, a)       (x).ThrowCurrent(a, __FILE__, __LINE__)
    #define SQMOD_VALIDATE(x)               (x).Validate(__FILE__, __LINE__)
    #define SQMOD_VALIDATE_CREATED(x)       (x).ValidateCreated(__FILE__, __LINE__)
    #define SQMOD_VALIDATE_PARAM(x, i)      (x).ValidateParam((i), __FILE__, __LINE__)
    #define SQMOD_VALIDATE_FIELD(x, i)      (x).ValidateField((i), __FILE__, __LINE__)
    #define SQMOD_VALIDATE_STEPPED(x)       (x).ValidateStepped(__FILE__, __LINE__)
    #define SQMOD_GET_VALID(x)              (x).GetValid(__FILE__, __LINE__)
    #define SQMOD_GET_CREATED(x)            (x).GetCreated(__FILE__, __LINE__)
    #define SQMOD_GET_STEPPED(x)            (x).GetStepped(__FILE__, __LINE__)
#else
    #define SQMOD_THROW_CURRENT(x, a)       (x).ThrowCurrent(a)
    #define SQMOD_VALIDATE(x)               (x).Validate()
    #define SQMOD_VALIDATE_CREATED(x)       (x).ValidateCreated()
    #define SQMOD_VALIDATE_PARAM(x, i)      (x).ValidateParam((i))
    #define SQMOD_VALIDATE_FIELD(x, i)      (x).ValidateField((i))
    #define SQMOD_VALIDATE_STEPPED(x)       (x).ValidateStepped()
    #define SQMOD_GET_VALID(x)              (x).GetValid()
    #define SQMOD_GET_CREATED(x)            (x).GetCreated()
    #define SQMOD_GET_STEPPED(x)            (x).GetStepped()
#endif // _DEBUG

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
struct ConnHnd;
struct StmtHnd;
struct ResHnd;

// ------------------------------------------------------------------------------------------------
class Account;
class Column;
class Connection;
class ResultSet;
class Statement;
class Transaction;

/* ------------------------------------------------------------------------------------------------
 * Common typedefs.
*/
typedef SharedPtr< ConnHnd > ConnRef;
typedef SharedPtr< StmtHnd > StmtRef;
typedef SharedPtr< ResHnd > ResRef;

/* ------------------------------------------------------------------------------------------------
 * Replicate the values of a script Date type to a database time type.
*/
void SqDateToMySQLTime(Object & obj, MYSQL_TIME & t);

/* ------------------------------------------------------------------------------------------------
 * Replicate the values of a script Date type to a database time type.
*/
void SqTimeToMySQLTime(Object & obj, MYSQL_TIME & t);

/* ------------------------------------------------------------------------------------------------
 * Replicate the values of a script Date type to a database time type.
*/
void SqDatetimeToMySQLTime(Object & obj, MYSQL_TIME & t);

} // Namespace:: SqMod

#endif // _SQMYSQL_COMMON_HPP_
