#ifndef _SQSQLITE_COMMON_HPP_
#define _SQSQLITE_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqlite3.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
class Connection;
class Statement;
class Column;
class Transaction;

/* ------------------------------------------------------------------------------------------------
 * SOFTWARE INFORMATION
*/
#define SQSQLITE_NAME "Squirrel SQLite Module"
#define SQSQLITE_AUTHOR "Sandu Liviu Catalin (S.L.C)"
#define SQSQLITE_COPYRIGHT "Copyright (C) 2016 Sandu Liviu Catalin"
#define SQSQLITE_HOST_NAME "SqModSQLiteHost"
#define SQSQLITE_VERSION 001
#define SQSQLITE_VERSION_STR "0.0.1"
#define SQSQLITE_VERSION_MAJOR 0
#define SQSQLITE_VERSION_MINOR 0
#define SQSQLITE_VERSION_PATCH 1

/* ------------------------------------------------------------------------------------------------
 * Handle validation.
*/
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    #define VALIDATE_HND(x) (x).Validate(__FILE__, __LINE__)
    #define VALIDATE_OPENED_HND(x) (x).ValidateOpened(__FILE__, __LINE__)
    #define VALIDATE_CREATED_HND(x) (x).ValidateCreated(__FILE__, __LINE__)
    #define VALIDATE_COLUMN_HND(x, i) (x).ValidateColumn((i), __FILE__, __LINE__)
    #define VALIDATE_PARAMETER_HND(x, i) (x).ValidateParameter((i), __FILE__, __LINE__)
    #define VALIDATE_ROW_HND(x) (x).ValidateRow(__FILE__, __LINE__)
    #define GET_VALID_HND(x) (x).GetValid(__FILE__, __LINE__)
    #define GET_OPENED_HND(x) (x).GetOpened(__FILE__, __LINE__)
    #define GET_CREATED_HND(x) (x).GetCreated(__FILE__, __LINE__)
#else
    #define VALIDATE_HND(x) (x).Validate()
    #define VALIDATE_OPENED_HND(x) (x).ValidateOpened()
    #define VALIDATE_CREATED_HND(x) (x).ValidateCreated()
    #define VALIDATE_COLUMN_HND(x, i) (x).ValidateColumn((i))
    #define VALIDATE_PARAMETER_HND(x, i) (x).ValidateParameter((i))
    #define VALIDATE_ROW_HND(x) (x).ValidateRow()
    #define GET_VALID_HND(x) (x).GetValid()
    #define GET_OPENED_HND(x) (x).GetOpened()
    #define GET_CREATED_HND(x) (x).GetCreated()
#endif // _DEBUG

/* ------------------------------------------------------------------------------------------------
 * Helper macros for architecture differences.
*/

#ifdef _SQ64
    #define sqlite3_bind_integer    sqlite3_bind_int64
    #define sqlite3_column_integer  sqlite3_column_int64
#else
    #define sqlite3_bind_integer    sqlite3_bind_int
    #define sqlite3_column_integer  sqlite3_column_int
#endif

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
struct ConnHnd;
struct StmtHnd;

/* ------------------------------------------------------------------------------------------------
 * Common typedefs.
*/
typedef SharedPtr< ConnHnd > ConnRef;
typedef SharedPtr< StmtHnd > StmtRef;

/* ------------------------------------------------------------------------------------------------
 * Generate a formatted query.
*/
CSStr QFmtStr(CSStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Tests if a certain query string is empty.
*/
bool IsQueryEmpty(CSStr str);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the string representation of a certain status code.
*/
CSStr GetErrStr(Int32 status);

/* ------------------------------------------------------------------------------------------------
 * Set a specific heap limit.
*/
void SetSoftHeapLimit(Int32 limit);

/* ------------------------------------------------------------------------------------------------
 * Release the specified amount of memory.
*/
Int32 ReleaseMemory(Int32 bytes);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the current memory usage.
*/
Object GetMemoryUsage();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the memory high watermark.
*/
Object GetMemoryHighwaterMark(bool reset);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the escaped version of the specified string.
*/
CSStr EscapeString(CSStr str);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the escaped version of the specified string using the supplied format specifier.
*/
CCStr EscapeStringEx(SQChar spec, CCStr str);

/* ------------------------------------------------------------------------------------------------
 * Convert the values from the specified array to a list of column names string.
*/
CCStr ArrayToQueryColumns(Array & arr);

/* ------------------------------------------------------------------------------------------------
 * Convert the keys from the specified array to a list of column names string.
*/
CCStr TableToQueryColumns(Table & tbl);

} // Namespace:: SqMod

#endif // _SQSQLITE_COMMON_HPP_
