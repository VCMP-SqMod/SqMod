// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "JValue.hpp"
#include "JArray.hpp"
#include "JObject.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CSStr JSONTypeToStr(json_type type)
{
    switch (type)
    {
        case JSON_OBJECT:   return _SC("object");
        case JSON_ARRAY:    return _SC("array");
        case JSON_STRING:   return _SC("string");
        case JSON_INTEGER:  return _SC("integer");
        case JSON_REAL:     return _SC("real");
        case JSON_TRUE:     return _SC("true");
        case JSON_FALSE:    return _SC("false");
        case JSON_NULL:     return _SC("null");
        default:            return _SC("unknown");
    }
}

// ------------------------------------------------------------------------------------------------
void SqThrowLast(HSQUIRRELVM vm, CSStr msg)
{
    // Remember the current stack size
    const StackGuard sg(vm);
    // Push the last error on the stack
    sq_getlasterror(vm);
    // Attempt to obtained the error as a string
    StackStrF val(vm, -1, false);
    // Did the retrieval failed?
    if (SQ_FAILED(val.mRes))
    {
        STHROWF("%s [Unknown error]", msg);
    }
    // Throw the resulting error message
    STHROWF("%s [%s]", msg, val.mPtr);
}

// ------------------------------------------------------------------------------------------------
Object SqFromJSON(HSQUIRRELVM /*vm*/, json_t * jval)
{
    switch (json_typeof(jval))
    {
        case JSON_OBJECT:
        {

        } break;
        case JSON_ARRAY:
        {

        } break;
        case JSON_STRING:
        {

        } break;
        case JSON_INTEGER:
        {

        } break;
        case JSON_REAL:
        {

        } break;
        case JSON_TRUE:
        {

        } break;
        case JSON_FALSE:
        {

        } break;
        case JSON_NULL:
        {

        } break;
        default: STHROWF("Unknown JSON value type");
    }

    return Object();
}

// ------------------------------------------------------------------------------------------------
json_t * SqToJSON(HSQUIRRELVM vm, SQInteger idx)
{
    switch (sq_gettype(vm, idx))
    {
        case OT_NULL:
        {
            return json_null();
        } break;
        case OT_INTEGER:
        {
            SQInteger i;
            // Retrieve the integer value
            if (SQ_FAILED(sq_getinteger(vm, idx, &i)))
            {
                SqThrowLast(vm, "Cannot retrieve integer value");
            }
            // Return the JSON value
            return json_integer(i);
        } break;
        case OT_FLOAT:
        {
            SQFloat f;
            // Retrieve the float value
            if (SQ_FAILED(sq_getfloat(vm, idx, &f)))
            {
                SqThrowLast(vm, "Cannot retrieve float value");
            }
            // Return the JSON value
            return json_real(f);
        } break;
        case OT_BOOL:
        {
            SQBool b;
            // Retrieve the boolean value
            if (SQ_FAILED(sq_getbool(vm, idx, &b)))
            {
                SqThrowLast(vm, "Cannot retrieve boolean value");
            }
            // Return the JSON value
            return json_boolean(b);
        } break;
        case OT_STRING:
        {
            CSStr s = nullptr;
            SQInteger n;
            // Retrieve the string value
            if (SQ_FAILED(sq_getstringandsize(vm, idx, &s, &n)))
            {
                SqThrowLast(vm, "Cannot retrieve string value");
            }
            // Return the JSON value
            return json_stringn(s, ConvTo< std::size_t >::From(n));
        } break;
        case OT_TABLE:
        {
            // Create an object wrapper to release automatically in case of failure
            JObject obj;
            // Remember the current stack size
            const StackGuard sg(vm);
            // Push null to begin table iteration
            sq_pushnull(vm);
            // Compute the new table index on the stack if necessary
            if (idx < 0)
            {
                --idx;
            }
            // Start iterating table elements
            while(SQ_SUCCEEDED(sq_next(vm, idx)))
            {
                // Attempt to convert the key to a string
                StackStrF val(vm, -2, false);
                // Did the conversion failed?
                if (SQ_FAILED(val.mRes))
                {
                    SqThrowLast(vm, "Invalid table key");
                }
                // Assign the value with further recursive scanning
                if (json_object_set_new(obj, val.mPtr, SqToJSON(vm, -1)) < 0)
                {
                    STHROWF("Unable to set table element (&s)", val.mPtr);
                }
                // Pop the key and value before the new iteration
                sq_pop(vm, 2);
            }
            // Increase the reference count so that we don't destroy the object
            json_incref(obj);
            // Return the resulted object
            return obj;
        } break;
        case OT_ARRAY:
        {
            // Create an array wrapper to release automatically in case of failure
            JArray arr;
            // Remember the current stack size
            const StackGuard sg(vm);
            // Obtain the total size of the array
            const SQInteger size = sq_getsize(vm, idx);
            // Push null to begin array iteration
            sq_pushnull(vm);
            // Compute the new array index on the stack if necessary
            if (idx < 0)
            {
                --idx;
            }
            // Currently processed element
            SQInteger pos;
            // Start iterating array elements
            while(SQ_SUCCEEDED(sq_next(vm, idx)))
            {
                // Retrieve the currently processed array element index
                if (SQ_FAILED(sq_getinteger(vm, -2, &pos)))
                {
                    SqThrowLast(vm, "Unable to retrieve iterator position");
                }
                // Are we still within the array bounds?
                else if (pos >= size)
                {
                    break; // Stop iterating
                }
                // Assign the value with further recursive scanning
                if (json_array_append_new(arr, SqToJSON(vm, -1)) < 0)
                {
                    STHROWF("Unable to set array element: " _PRINT_INT_FMT, pos);
                }
                // Pop the key and value before the new iteration
                sq_pop(vm, 2);
            }
            // Increase the reference count so that we don't destroy the array
            json_incref(arr);
            // Return the resulted array
            return arr;
        } break;
        default:
        {
            // Grab the type name of the object on the stack
            const String tn(SqTypeName(vm, idx));
            // Now throw the error with the obtained name
            STHROWF("Unknown conversion for type: %s", tn.c_str());
        }
    }
    // Should not reach this point
    return nullptr;
}

// ------------------------------------------------------------------------------------------------
JObject SqTableToJSONObject(Table & tbl)
{
    // Make sure that the given table is not null
    if (tbl.IsNull())
    {
        return JObject(); // Nothing to add
    }
    // Remember the current stack size
    const StackGuard sg(tbl.GetVM());
    // Push our table onto the stack
    Var< Table & >::push(tbl.GetVM(), tbl);
    // Attempt to extract the values from the given table
    return JObject(SqToJSON(tbl.GetVM(), -1), false);
}

// ------------------------------------------------------------------------------------------------
JArray SqArrayToJSONArray(Array & arr)
{
    // Make sure that the given array is not null
    if (arr.IsNull())
    {
        return JArray(); // Nothing to add
    }
    // Remember the current stack size
    const StackGuard sg(arr.GetVM());
    // Push our array onto the stack
    Var< Array & >::push(arr.GetVM(), arr);
    // Attempt to extract the values from the given array
    return JArray(SqToJSON(arr.GetVM(), -1), false);
}

// ------------------------------------------------------------------------------------------------
JObject SqObjectToJSONObject(Object & obj)
{
    // Make sure that the given object is not null
    if (obj.IsNull())
    {
        return JObject(); // Nothing to add
    }
    // Remember the current stack size
    const StackGuard sg(obj.GetVM());
    // Push our object onto the stack
    Var< Object & >::push(obj.GetVM(), obj);
    // Attempt to extract the values from the given object
    return JObject(SqToJSON(obj.GetVM(), -1), false);
}

// ------------------------------------------------------------------------------------------------
JValue SqValueToJSONValue(Object & obj)
{
    // Make sure that the given object is not null
    if (obj.IsNull())
    {
        return JValue(); // Nothing to add
    }
    // Remember the current stack size
    const StackGuard sg(obj.GetVM());
    // Push our object onto the stack
    Var< Object & >::push(obj.GetVM(), obj);
    // Attempt to extract the values from the given object
    return JValue(SqToJSON(obj.GetVM(), -1), false);
}

// ================================================================================================
void Register_Common(Table & jns)
{
    jns.Func(_SC("FromTable"), &SqTableToJSONObject);
    jns.Func(_SC("FromArray"), &SqArrayToJSONArray);
    jns.Func(_SC("ToObject"), &SqObjectToJSONObject);
    jns.Func(_SC("ToValue"), &SqValueToJSONValue);
}

} // Namespace:: SqMod
