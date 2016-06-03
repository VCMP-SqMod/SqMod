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
