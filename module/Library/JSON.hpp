#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"
#include "Library/IO/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <sajson.h>
#include <fmt/args.h>
#include <fmt/format.h>
#include <fmt/xchar.h>


// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * JSON serializer. The generated JSON output is always minified for efficiency reasons.
*/
struct CtxJSON
{
    /* --------------------------------------------------------------------------------------------
     * Output string.
    */
    String mOutput{};

    /* --------------------------------------------------------------------------------------------
     * Prefer a table with named members even when a simple array would do the job.
     * Take a Vector3 for example. Compact array [x, y, z] or named object {x: #.#, y: #.#, z: #.#}
    */
    bool mObjectOverArray{true};

    /* --------------------------------------------------------------------------------------------
     * How nested are we currently.
    */
    uint32_t mDepth{0};

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CtxJSON() noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit CtxJSON(bool ooa) noexcept
        : CtxJSON()
    {
        mObjectOverArray = ooa;
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CtxJSON(const CtxJSON &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CtxJSON(CtxJSON &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CtxJSON() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CtxJSON & operator = (const CtxJSON &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CtxJSON & operator = (CtxJSON &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current depth.
    */
    SQMOD_NODISCARD const String & GetOutput() const noexcept
    {
        return mOutput;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current depth.
    */
    SQMOD_NODISCARD SQInteger GetDepth() const noexcept
    {
        return mDepth;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve whether objects are preferred over arrays.
    */
    SQMOD_NODISCARD bool GetObjectOverArray() const noexcept
    {
        return mObjectOverArray;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve whether objects are preferred over arrays.
    */
    CtxJSON & SetObjectOverArray(bool toggle) noexcept
    {
        mObjectOverArray = toggle;
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Increase indentation by one level.
    */
    void Advance() noexcept
    {
        ++mDepth;
    }

    /* --------------------------------------------------------------------------------------------
     * Decrease indentation by one level.
    */
    void Retreat() noexcept
    {
        assert(mDepth > 0);
        if (mDepth) --mDepth;
    }

    /* --------------------------------------------------------------------------------------------
     * Begin writing an array.
    */
    CtxJSON & OpenArray()
    {
        // Add the array-begin character
        mOutput.push_back('[');
        // Go forward one level
        Advance();
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Stop writing an array.
    */
    CtxJSON & CloseArray()
    {
        // If the last character is a comma then replace it
        if (mOutput.back() == ',')
        {
            mOutput.back() = ']';
        }
        // Append the array-end character
        else
        {
            mOutput.push_back(']');
        }
        // Go back one level
        Retreat();
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Begin writing an object.
    */
    CtxJSON & OpenObject()
    {
        // Add the object-begin character
        mOutput.push_back('{');
        // Go forward one level
        Advance();
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Stop writing an object.
    */
    CtxJSON & CloseObject()
    {
        // If the last character is a comma then replace it
        if (mOutput.back() == ',')
        {
            mOutput.back() = '}';
        }
        // Append the object-end character
        else
        {
            mOutput.push_back('}');
        }
        // Go back one level
        Retreat();
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Begin writing a key value.
    */
    CtxJSON & MakeKey()
    {
        // If the last character is a comma then replace it
        if (mOutput.back() == ',')
        {
            mOutput.back() = ':';
        }
        // Append the array-end character
        else
        {
            mOutput.push_back(':');
        }
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Check whether the specified weak-ref points to a type of value that must be wrapped.
    */
    SQMOD_NODISCARD static bool CheckWeakRefWrap(HSQUIRRELVM vm, SQInteger idx) noexcept
    {
        SQRESULT r = sq_getweakrefval(vm, idx);
        // Whether the type doesn't have to be wrapped
        bool w = true;
        // Attempt to grab the value pointed by the weak reference
        if (SQ_SUCCEEDED(r))
        {
            // Attempt to serialize the actual value
            w = sq_gettype(vm, -1) != OT_TABLE && sq_gettype(vm, -1) != OT_ARRAY && sq_gettype(vm, -1) == OT_INSTANCE;
            // Pop the referenced value
            sq_poptop(vm);
        }
        // Wrap the value by default
        return w;
    }

    /* --------------------------------------------------------------------------------------------
     * Serialize given arguments.
    */
    SQRESULT SerializeParams(HSQUIRRELVM vm)
    {
        bool wrap_everything_in_array = false;
        // Clear the output buffer if necessary
        mOutput.clear();
        mDepth = 0;
        // Fetch the number of objects on the stack
        const auto top = sq_gettop(vm);
        // If there's more than one argument then they all get wrapped inside an array
        // If there is one argument and is not an array, table or instance then do the same
        if (top > 2 || (sq_gettype(vm, 2) != OT_TABLE &&
                        sq_gettype(vm, 2) != OT_ARRAY &&
                        sq_gettype(vm, 2) != OT_INSTANCE &&
                        CheckWeakRefWrap(vm, 2)))
        {
            wrap_everything_in_array = true;
            // Open an array
            OpenArray();
        }
        // Serialize every specified argument
        for (SQInteger i = 2; i <= top; ++i)
        {
            if (SQRESULT r = SerializeAt(vm, i); SQ_FAILED(r))
            {
                return r; // Propagate the error
            }
        }
        // Was everything wrapped inside an array?
        if (wrap_everything_in_array)
        {
            CloseArray();
        }
        // Remove trailing separator, if any
        else if (mOutput.back() == ',')
        {
            mOutput.pop_back();
        }
        // Push the output string on the stack
        sq_pushstring(vm, mOutput.c_str(), static_cast< SQInteger >(mOutput.size()));
        // Specify that we have a value on the stack
        return 1;
    }

    /* --------------------------------------------------------------------------------------------
     * Serialize the value a specific position in the stack.
    */
    SQRESULT SerializeAt(HSQUIRRELVM vm, SQInteger idx) // NOLINT(misc-no-recursion)
    {
        // Identify object type
        switch (sq_gettype(vm, idx))
        {
            case OT_NULL: {
                PushNull();
            } break;
            case OT_INTEGER: {
                SQInteger v;
                // Attempt to retrieve the value from the stack
                if (SQRESULT r = sq_getinteger(vm, idx, &v); SQ_FAILED(r))
                {
                    return r; // Propagate the error
                }
                // Write the value in the output
                PushInteger(v);
            } break;
            case OT_FLOAT: {
                SQFloat v;
                // Attempt to retrieve the value from the stack
                if (SQRESULT r = sq_getfloat(vm, idx, &v); SQ_FAILED(r))
                {
                    return r; // Propagate the error
                }
                // Write the value in the output
            #ifdef SQUSEDOUBLE
                PushDouble(v);
            #else
                PushFloat(v);
            #endif
            } break;
            case OT_BOOL: {
                SQBool v;
                // Attempt to retrieve the value from the stack
                if (SQRESULT r = sq_getbool(vm, idx, &v); SQ_FAILED(r))
                {
                    return r; // Propagate the error
                }
                // Write the value in the output
                PushBool(v != SQFalse);
            } break;
            case OT_STRING: {
                const SQChar * v = nullptr;
                SQInteger n = 0;
                // Attempt to retrieve and convert the string
                if (SQRESULT r = sq_getstringandsize(vm, idx, &v, &n); SQ_FAILED(r))
                {
                    return r; // Propagate the error
                }
                // Write the value in the output
                PushString(v, static_cast< size_t >(n));
            } break;
            case OT_TABLE: {
                if (SQRESULT r = SerializeTable(vm, idx); SQ_FAILED(r))
                {
                    return r; // Propagate the error
                }
                // Include the separator manually
                mOutput.push_back(',');
            } break;
            case OT_ARRAY: {
                if (SQRESULT r = SerializeArray(vm, idx); SQ_FAILED(r))
                {
                    return r; // Propagate the error
                }
                // Include the separator manually
                mOutput.push_back(',');
            } break;
            case OT_INSTANCE: {
                if (SQRESULT r = SerializeInstance(vm, idx); SQ_FAILED(r))
                {
                    return r; // Propagate the error
                }
            } break;
            case OT_WEAKREF: {
                if (SQRESULT r = SerializeWeakRef(vm, idx); SQ_FAILED(r))
                {
                    return r; // Propagate the error
                }
            } break;
            case OT_USERDATA:
            case OT_CLOSURE:
            case OT_NATIVECLOSURE:
            case OT_GENERATOR:
            case OT_USERPOINTER:
            case OT_THREAD:
            case OT_FUNCPROTO:
            case OT_CLASS:
            case OT_OUTER:
                return sq_throwerrorf(vm, _SC("Type (%s) is not serializable"), SqTypeName(sq_gettype(vm, 2)));
        }
        // Serialization was successful
        return SQ_OK;
    }

    /* --------------------------------------------------------------------------------------------
     * Serialize the array a specific position in the stack. Stack index must be absolute!
    */
    SQRESULT SerializeArray(HSQUIRRELVM vm, SQInteger idx) // NOLINT(misc-no-recursion)
    {
        // Begin array scope
        OpenArray();
        // Push null to initiate iteration
        sq_pushnull(vm);
        // So we can use absolute stack indexes to avoid errors
        const auto top = sq_gettop(vm);
        // Start iterating the array at the specified position in the stack
        for(SQRESULT r = SQ_OK; SQ_SUCCEEDED(sq_next(vm, idx));)
        {
            // Attempt serialization of the currently iterated value
            r = SerializeAt(vm, top + 2);
            // Check for failures
            if (SQ_FAILED(r))
            {
                // Pop the null iterator, key and value from the stack
                sq_pop(vm, 3);
                // Propagate the error
                return r;
            }
            // Pop the key and value from the stack (i.e. cleanup after `sq_next`)
            sq_pop(vm, 2);
        }
        // Pop the null iterator
        sq_poptop(vm);
        // Close array scope
        CloseArray();
        // Serialization was successful
        return SQ_OK;
    }

    /* --------------------------------------------------------------------------------------------
     * Serialize the table a specific position in the stack. Stack index must be absolute!
    */
    SQRESULT SerializeTable(HSQUIRRELVM vm, SQInteger idx) // NOLINT(misc-no-recursion)
    {
        // Begin object scope
        OpenObject();
        // Push null to initiate iteration
        sq_pushnull(vm);
        // So we can use absolute stack indexes to avoid errors
        const auto top = sq_gettop(vm);
        // Start iterating the object at the specified position in the stack
        for(SQRESULT r = SQ_OK; SQ_SUCCEEDED(sq_next(vm, idx));)
        {
            if (sq_gettype(vm, -2) == OT_STRING)
            {
                // Attempt serialization of the currently iterated element key
                r = SerializeAt(vm, top + 1);
                // Can we proceed with the value?
                if (SQ_SUCCEEDED(r))
                {
                    // Mark the value above as the key of this element and
                    // attempt serialization of the currently iterated element value
                    r = MakeKey().SerializeAt(vm, top + 2);
                }
            }
            else
            {
                r = sq_throwerror(vm, _SC("Only string values are accepted as object keys"));
            }
            // Check for failures
            if (SQ_FAILED(r))
            {
                // Pop the null iterator, key and value from the stack
                sq_pop(vm, 3);
                // Propagate the error
                return r;
            }
            // Pop the key and value from the stack (i.e. cleanup after `sq_next`)
            sq_pop(vm, 2);
        }
        // Pop the null iterator
        sq_poptop(vm);
        // Close object scope
        CloseObject();
        // Serialization was successful
        return SQ_OK;
    }

    /* --------------------------------------------------------------------------------------------
     * Serialize the instance a specific position in the stack. Stack index must be absolute!
    */
    SQRESULT SerializeInstance(HSQUIRRELVM vm, SQInteger idx)
    {
        return sq_throwerror(vm, _SC("Instance serialization is not yet implemented"));
    }

    /* --------------------------------------------------------------------------------------------
     * Serialize the weak-ref a specific position in the stack. Stack index must be absolute!
    */
    SQRESULT SerializeWeakRef(HSQUIRRELVM vm, SQInteger idx)
    {
        SQRESULT r = sq_getweakrefval(vm, idx);
        // Attempt to grab the value pointed by the weak reference
        if (SQ_SUCCEEDED(r))
        {
            // Attempt to serialize the actual value
            r = SerializeAt(vm, sq_gettop(vm));
            // Pop the referenced value
            sq_poptop(vm);
        }
        // Propagate the error, if any
        return r;
    }

    /* --------------------------------------------------------------------------------------------
     * Serialize a value to the current container. It assumes an array or object is currently open.
    */
    SQRESULT PushValues(HSQUIRRELVM vm)
    {
        // Fetch the number of objects on the stack
        const auto top = sq_gettop(vm);
        // Do we have a value?
        if (top < 2)
        {
            return sq_throwerror(vm, _SC("Must specify at least one value to be pushed"));
        }
        // Serialize every specified argument
        for (SQInteger i = 2; i <= top; ++i)
        {
            if (SQRESULT r = SerializeAt(vm, i); SQ_FAILED(r))
            {
                return r; // Propagate the error
            }
        }
        // Allow chaining
        sq_push(vm, 1);
        // Specify that a value was returned
        return 1;
    }

    /* --------------------------------------------------------------------------------------------
     * Serialize a key/value pair to the current object. It assumes an object is currently open.
    */
    SQRESULT PushElement(HSQUIRRELVM vm)
    {
        // Fetch the number of objects on the stack
        const auto top = sq_gettop(vm);
        // Do we have a value?
        if (top < 3)
        {
            return sq_throwerrorf(vm, _SC("Not enough parameters. Received %lld but %lld needed"), top-1, 2);
        }
        else if (sq_gettype(vm, 2) != OT_STRING)
        {
            return sq_throwerrorf(vm, _SC("Element key must be a string"));
        }
        // Attempt serialization of the currently iterated element key
        if (SQRESULT r = SerializeAt(vm, 2); SQ_SUCCEEDED(r))
        {
            // Mark the value above as the key of this element and
            // attempt serialization of the currently iterated element value
            r = MakeKey().SerializeAt(vm, 3);
            // Check for failures
            if (SQ_FAILED(r))
            {
                return r; // Propagate the error
            }
        }
        // Allow chaining
        sq_push(vm, 1);
        // Specify that a value was returned
        return 1;
    }

    /* --------------------------------------------------------------------------------------------
     * Push a key in the output. It assumes an object was open and previous element closed properly.
    */
    CtxJSON & PushKey(StackStrF & key)
    {
        // Validate the string value
        if (key.mLen >= 0 && SQ_SUCCEEDED(key.mRes))
        {
            PushString(key.mPtr, static_cast< size_t >(key.mLen));
            MakeKey();
        }
        else
        {
            STHROWF("Invalid object key");
        }
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Write a null value to the output.
    */
    void PushNull()
    {
        mOutput.append("null,");
    }

    /* --------------------------------------------------------------------------------------------
     * Write an integer value to the output.
    */
    void PushInteger(SQInteger value)
    {
        fmt::format_int f(value);
        // Append the formatted integer to the buffer
        mOutput.append(f.data(), f.size());
        mOutput.push_back(',');
    }

    /* --------------------------------------------------------------------------------------------
     * Write a single precision floating point value to the output.
    */
    void PushFloat(float value)
    {
        fmt::format_to(std::back_inserter(mOutput), "{},", value);
    }

    /* --------------------------------------------------------------------------------------------
     * Write a double precision floating point value to the output.
    */
    void PushDouble(double value)
    {
        fmt::format_to(std::back_inserter(mOutput), "{},", value);
    }

    /* --------------------------------------------------------------------------------------------
     * Write a boolean value to the output.
    */
    void PushBool(bool value)
    {
        if (value)
        {
            mOutput.append("true,", 5);
        }
        else
        {
            mOutput.append("false,", 6);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Write a string value to the output.
    */
    void PushString(const SQChar * str)
    {
        mOutput.push_back('"');
        mOutput.append(str);
        mOutput.push_back('"');
        mOutput.push_back(',');
    }

    /* --------------------------------------------------------------------------------------------
     * Write a string value to the output.
    */
    void PushString(const SQChar * str, size_t length)
    {
        mOutput.push_back('"');
        mOutput.append(str, length);
        mOutput.push_back('"');
        mOutput.push_back(',');
    }
};

} // Namespace:: SqMod
