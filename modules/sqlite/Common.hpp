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
    #define SQMOD_THROW_CURRENT(x, a)       (x).ThrowCurrent(a, __FILE__, __LINE__)
    #define SQMOD_VALIDATE(x)               (x).Validate(__FILE__, __LINE__)
    #define SQMOD_VALIDATE_CREATED(x)       (x).ValidateCreated(__FILE__, __LINE__)
    #define SQMOD_VALIDATE_PARAM(x, i)      (x).ValidateParam((i), __FILE__, __LINE__)
    #define SQMOD_VALIDATE_COLUMN(x, i)     (x).ValidateColumn((i), __FILE__, __LINE__)
    #define SQMOD_VALIDATE_ROW(x)           (x).ValidateRow(__FILE__, __LINE__)
    #define SQMOD_GET_VALID(x)              (x).GetValid(__FILE__, __LINE__)
    #define SQMOD_GET_CREATED(x)            (x).GetCreated(__FILE__, __LINE__)
#else
    #define SQMOD_THROW_CURRENT(x, a)       (x).ThrowCurrent(a)
    #define SQMOD_VALIDATE(x)               (x).Validate()
    #define SQMOD_VALIDATE_CREATED(x)       (x).ValidateCreated()
    #define SQMOD_VALIDATE_PARAM(x, i)      (x).ValidateParam((i))
    #define SQMOD_VALIDATE_COLUMN(x, i)     (x).ValidateColumn((i))
    #define SQMOD_VALIDATE_ROW(x)           (x).ValidateRow()
    #define SQMOD_GET_VALID(x)              (x).GetValid()
    #define SQMOD_GET_CREATED(x)            (x).GetCreated()
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
 * Obtain a script object from a connection handle. (meant to avoid having to include the header)
*/
Object GetConnectionObj(const ConnRef & conn);

/* ------------------------------------------------------------------------------------------------
 * Obtain a script object from a statement handle. (meant to avoid having to include the header)
*/
Object GetStatementObj(const StmtRef & stmt);

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
CSStr EscapeString(const StackStrF & str);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the escaped version of the specified string using the supplied format specifier.
*/
CCStr EscapeStringEx(SQChar spec, const StackStrF & str);

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
