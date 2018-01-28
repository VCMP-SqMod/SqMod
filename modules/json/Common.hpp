#ifndef _SQJSON_COMMON_HPP_
#define _SQJSON_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
#include <jansson.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * SOFTWARE INFORMATION
*/
#define SQJSON_NAME "Squirrel JSON Module"
#define SQJSON_AUTHOR "Sandu Liviu Catalin (S.L.C)"
#define SQJSON_COPYRIGHT "Copyright (C) 2018 Sandu Liviu Catalin"
#define SQJSON_HOST_NAME "SqModJSONHost"
#define SQJSON_VERSION 001
#define SQJSON_VERSION_STR "0.0.1"
#define SQJSON_VERSION_MAJOR 0
#define SQJSON_VERSION_MINOR 0
#define SQJSON_VERSION_PATCH 1

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
class JValue;
class JArray;
class JObject;

/* ------------------------------------------------------------------------------------------------
 * Implements RAII to free the strings obtained from dumps even after exceptions.
*/
struct CStrGuard
{
    // --------------------------------------------------------------------------------------------
    CStr mPtr; // The managed pointer

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    CStrGuard(CStr p)
        : mPtr(p)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CStrGuard()
    {
        if (mPtr)
        {
            std::free(mPtr);
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Class that represents an error occurred while parsing JSON data.
*/
struct JError
{
public:

    // --------------------------------------------------------------------------------------------
    json_error_t mErr; // The managed error instance.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    JError();
};

/* ------------------------------------------------------------------------------------------------
 * Retrieve the string representation of JSON value type.
*/
CSStr JSONTypeToStr(json_type type);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the string representation of JSON value type.
*/
inline CSStr JSONTypeStr(json_t * ptr)
{
    return (ptr == nullptr) ? _SC("unknown") : JSONTypeToStr(json_typeof(ptr));
}

/* ------------------------------------------------------------------------------------------------
 * Convert a script value from the stack to a JSON object.
*/
Object SqFromJSON(HSQUIRRELVM vm, json_t * jval);

/* ------------------------------------------------------------------------------------------------
 * Convert a script value from the stack to a JSON object.
*/
json_t * SqToJSON(HSQUIRRELVM vm, SQInteger idx);

/* ------------------------------------------------------------------------------------------------
 * Convert a script table to a JSON object.
*/
JObject SqTableToJSONObject(Table & obj);

/* ------------------------------------------------------------------------------------------------
 * Convert a script array to a JSON array.
*/
JArray SqArrayToJSONArray(Array & obj);

/* ------------------------------------------------------------------------------------------------
 * Convert a script object to a JSON object.
*/
JObject SqObjectToJSONObject(Object & obj);

/* ------------------------------------------------------------------------------------------------
 * Convert a script value to a JSON value.
*/
JValue SqValueToJSONValue(Object & obj);

} // Namespace:: SqMod

#endif // _SQJSON_COMMON_HPP_
