#ifndef _SQMMDB_COMMON_HPP_
#define _SQMMDB_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <maxminddb.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * SOFTWARE INFORMATION
*/
#define SQMMDB_NAME "Squirrel MaxmindDB Module"
#define SQMMDB_AUTHOR "Sandu Liviu Catalin (S.L.C)"
#define SQMMDB_COPYRIGHT "Copyright (C) 2018 Sandu Liviu Catalin"
#define SQMMDB_HOST_NAME "SqModMMDBHost"
#define SQMMDB_VERSION 001
#define SQMMDB_VERSION_STR "0.0.1"
#define SQMMDB_VERSION_MAJOR 0
#define SQMMDB_VERSION_MINOR 0
#define SQMMDB_VERSION_PATCH 1

/* ------------------------------------------------------------------------------------------------
 * Handle validation.
*/
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    #define SQMOD_VALIDATE(x)               (x).Validate(__FILE__, __LINE__)
    #define SQMOD_GET_VALID(x)              (x).GetValid(__FILE__, __LINE__)
    #define SQMOD_GET_VALID_ELEM(x)         (x).GetValidElem(__FILE__, __LINE__)
    #define SQMOD_GET_VALID_DATA(x)         (x).GetValidData(__FILE__, __LINE__)
#else
    #define SQMOD_VALIDATE(x)               (x).Validate()
    #define SQMOD_GET_VALID(x)              (x).GetValid()
    #define SQMOD_GET_VALID_ELEM(x)         (x).GetValidElem()
    #define SQMOD_GET_VALID_DATA(x)         (x).GetValidData()
#endif // _DEBUG

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
class Database;
class Metadata;
class Description;
class SockAddr;
class EntryData;
class EntryDataList;
class LookupResult;
class SearchNode;

/* ------------------------------------------------------------------------------------------------
 * Forward handle declarations.
*/
struct DbHnd;

/* ------------------------------------------------------------------------------------------------
 * Common typedefs.
*/
typedef SharedPtr< DbHnd > DbRef;

/* ------------------------------------------------------------------------------------------------
 * Used to retrieve the string representation of the specified type identifier.
*/
CSStr AsTypeStr(Uint32 id);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the value from the specified entry data as a boolean.
*/
bool GetEntryAsBool(const MMDB_entry_data_s & ed);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the value from the specified entry data as a native integer.
*/
SQInteger GetEntryAsInteger(const MMDB_entry_data_s & ed);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the value from the specified entry data as a floating point.
*/
SQFloat GetEntryAsFloat(const MMDB_entry_data_s & ed);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the value from the specified entry data as a long integer.
*/
Object GetEntryAsLong(const MMDB_entry_data_s & ed);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the value from the specified entry data as a string.
*/
Object GetEntryAsString(const MMDB_entry_data_s & ed);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the value from the specified entry data as a stream of bytes.
*/
Object GetEntryAsBytes(const MMDB_entry_data_s & ed);

} // Namespace:: SqMod

#endif // _SQMMDB_COMMON_HPP_
