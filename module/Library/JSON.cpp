// ------------------------------------------------------------------------------------------------
#include "Library/JSON.hpp"

// ------------------------------------------------------------------------------------------------
#include <sajson.h>
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
#include <cstdio>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqCtxJSON, _SC("SqCtxJSON"))

// ------------------------------------------------------------------------------------------------
static SQInteger SqFromJson_Push(HSQUIRRELVM vm, const sajson::value & node) noexcept // NOLINT(misc-no-recursion)
{
    // Operation result
    SQInteger r = SQ_OK;
    // Identify element type
    switch (node.get_type())
    {
        case sajson::TYPE_INTEGER: {
            sq_pushinteger(vm, static_cast< SQInteger >(node.get_integer_value()));
        } break;
        case sajson::TYPE_DOUBLE: {
            sq_pushfloat(vm, static_cast< SQFloat >(node.get_double_value()));
        } break;
        case sajson::TYPE_NULL: {
            sq_pushnull(vm);
        } break;
        case sajson::TYPE_FALSE: {
            sq_pushbool(vm, SQFalse);
        } break;
        case sajson::TYPE_TRUE: {
            sq_pushbool(vm, SQTrue);
        } break;
        case sajson::TYPE_STRING: {
            sq_pushstring(vm, node.as_cstring(), static_cast< SQInteger >(node.get_string_length()));
        } break;
        case sajson::TYPE_ARRAY: {
            // Array length
            const size_t n = node.get_length();
            // Create a new array on the stack
            sq_newarrayex(vm, static_cast< SQInteger >(n));
            // Process array elements
            for (size_t i = 0; i < n; ++i)
            {
                // Transform the value into a script object on the stack
                r = SqFromJson_Push(vm, node.get_array_element(i));
                // Did we fail?
                if (SQ_FAILED(r))
                {
                    break; // Abort
                }
                // At this point we have a value on the stack
                r = sq_arrayappend(vm, -2);
                // Did we fail?
                if (SQ_FAILED(r))
                {
                    // Discard the value
                    sq_poptop(vm);
                    // Abort
                    break;
                }
            }
            // Anything bad happened?
            if (SQ_FAILED(r))
            {
                sq_poptop(vm); // Discard the array
            }
        } break;
        case sajson::TYPE_OBJECT: {
            // Object length
            const size_t n = node.get_length();
            // Create a new table on the stack
            sq_newtableex(vm, static_cast< SQInteger >(n));
            //
            for (size_t i = 0; i < n; ++i)
            {
                const auto k = node.get_object_key(i);
                // Transform the key into a script object on the stack
                sq_pushstring(vm, k.data(), static_cast< SQInteger >(k.length()));
                // Transform the value into a script object on the stack
                r = SqFromJson_Push(vm, node.get_object_value(i));
                // Did we fail?
                if (SQ_FAILED(r))
                {
                    // Discard the key
                    sq_poptop(vm);
                    // Abort
                    break;
                }
                // At this point we have a key and a value on the stack
                r = sq_newslot(vm, -3, SQFalse);
                // Did we fail?
                if (SQ_FAILED(r))
                {
                    // Discard the key/value pair
                    sq_pop(vm, 2);
                    // Abort
                    break;
                }
            }
            // Anything bad happened?
            if (SQ_FAILED(r))
            {
                sq_poptop(vm); // Discard the table
            }
        } break;
        default:
            // Should never really get here because it should be sanitized by the JSON parser
            // But doesn't hurt to have it here in case something out of our scope goes wrong
            r = sq_throwerror(vm, _SC("Unrecognized JSON type"));
    }
    // Return the result
    return r;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqFromJSON(HSQUIRRELVM vm) noexcept
{
    // Remember the current stack size
    const SQInteger top = sq_gettop(vm);
    // Was the JSON string specified?
    if (top < 2)
    {
        return sq_throwerror(vm, _SC("Please specify the JSON string to parse"));
    }
    // JSON string storage
    StackStrF s(vm, 2);
    // Attempt to retrieve the specified JSON string
    if (SQ_FAILED(s.Proc(true)))
    {
        return s.mRes; // Propagate the error
    }
    // Attempt to parse the specified JSON string
    const sajson::document & document = sajson::parse(sajson::dynamic_allocation(), sajson::string(s.mPtr, static_cast<size_t>(s.mLen)));
    // See if there was an error
    if (!document.is_valid())
    {
        return sq_throwerror(vm, document.get_error_message_as_cstring());
    }
    // Process the nodes that were parsed from the string
    SQInteger r = SqFromJson_Push(vm, document.get_root());
    // We either have a value to return or we propagate some error
    return SQ_SUCCEEDED(r) ? 1 : r;
}

// ------------------------------------------------------------------------------------------------
CtxJSON & CtxJSON::OpenArray()
{
    // Add the array-begin character
    mOutput.push_back('[');
    // Go forward one level
    Advance();
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
CtxJSON & CtxJSON::CloseArray()
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
    // Move the comma after the closing character
    mOutput.push_back(',');
    // Go back one level
    Retreat();
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
CtxJSON & CtxJSON::ReopenArray()
{
    // If the last character is a comma then remove it
    if (mOutput.back() == ',')
    {
        mOutput.pop_back();
    }
    // If the last character is the array-end character then replace it with a comma
    if (mOutput.back() == ']')
    {
        mOutput.back() = ',';
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
CtxJSON & CtxJSON::OpenObject()
{
    // Add the object-begin character
    mOutput.push_back('{');
    // Go forward one level
    Advance();
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
CtxJSON & CtxJSON::CloseObject()
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
    // Move the comma after the closing character
    mOutput.push_back(',');
    // Go back one level
    Retreat();
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
CtxJSON & CtxJSON::ReopenObject()
{
    // If the last character is a comma then remove it
    if (mOutput.back() == ',')
    {
        mOutput.pop_back();
    }
    // If the last character is the object-end character then replace it with a comma
    if (mOutput.back() == '}')
    {
        mOutput.back() = ',';
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
CtxJSON & CtxJSON::MakeKey()
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
    // Allow the hook to react
    if (mKeyHook)
    {
        mKeyHook(*this);
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
bool CtxJSON::CheckWeakRefWrap(HSQUIRRELVM vm, SQInteger idx) noexcept
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

// ------------------------------------------------------------------------------------------------
SQRESULT CtxJSON::SerializeParams(HSQUIRRELVM vm)
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
    if (mOutput.back() == ',')
    {
        mOutput.pop_back();
    }
    // Push the output string on the stack
    sq_pushstring(vm, mOutput.c_str(), static_cast< SQInteger >(mOutput.size()));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQRESULT CtxJSON::SerializeAt(HSQUIRRELVM vm, SQInteger idx) // NOLINT(misc-no-recursion)
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
        } break;
        case OT_ARRAY: {
            if (SQRESULT r = SerializeArray(vm, idx); SQ_FAILED(r))
            {
                return r; // Propagate the error
            }
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

// ------------------------------------------------------------------------------------------------
SQRESULT CtxJSON::SerializeArray(HSQUIRRELVM vm, SQInteger idx) // NOLINT(misc-no-recursion)
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

// ------------------------------------------------------------------------------------------------
SQRESULT CtxJSON::SerializeTable(HSQUIRRELVM vm, SQInteger idx) // NOLINT(misc-no-recursion)
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

// ------------------------------------------------------------------------------------------------
SQRESULT CtxJSON::SerializeInstance(HSQUIRRELVM vm, SQInteger idx)
{
    sq_pushstring(vm, mMetaMethod.c_str(), static_cast< SQInteger >(mMetaMethod.size()));
    // Attempt to retrieve the meta-method from the instance
    if(SQRESULT r = sq_get(vm, idx); SQ_FAILED(r))
    {
        return r; // Propagate the error
    }
    // Make sure this is actually a closure/function that we can invoke
    else if (const auto t = sq_gettype(vm, -1); t != OT_CLOSURE && t != OT_NATIVECLOSURE)
    {
        // Remove whatever is on the stack
        sq_poptop(vm);
        // Abort the operation as we can't do anything about it
        return sq_throwerrorf(vm, _SC("`_tojson` meta-method is not a closure for type (%s)"), SqTypeName(vm, idx).c_str());
    }
    // Push the instance itself the stack (the environment)
    sq_push(vm, idx);
    // Push this instance on the stack (the json context)
    ClassType< CtxJSON >::PushInstance(vm, this);
    // Invoke the function to perform the conversion in this context
    SQRESULT r = sq_call(vm, 2, SQFalse, SQFalse);
    // Remove the closure from the stack
    sq_poptop(vm);
    // Propagate the result, whatever that is
    return r;
}

// ------------------------------------------------------------------------------------------------
SQRESULT CtxJSON::SerializeWeakRef(HSQUIRRELVM vm, SQInteger idx) // NOLINT(misc-no-recursion)
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

// ------------------------------------------------------------------------------------------------
SQRESULT CtxJSON::PushValues(HSQUIRRELVM vm)
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

// ------------------------------------------------------------------------------------------------
SQRESULT CtxJSON::PushElement(HSQUIRRELVM vm)
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

// ------------------------------------------------------------------------------------------------
CtxJSON & CtxJSON::PushKey(StackStrF & key)
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

// ------------------------------------------------------------------------------------------------
void CtxJSON::PushNull()
{
    mOutput.append("null,");
}

// ------------------------------------------------------------------------------------------------
void CtxJSON::PushInteger(SQInteger value)
{
    fmt::format_int f(value);
    // Append the formatted integer to the buffer
    mOutput.append(f.data(), f.size());
    mOutput.push_back(',');
}

// ------------------------------------------------------------------------------------------------
void CtxJSON::PushFloat(float value)
{
    fmt::format_to(std::back_inserter(mOutput), "{},", value);
}

// ------------------------------------------------------------------------------------------------
void CtxJSON::PushDouble(double value)
{
    fmt::format_to(std::back_inserter(mOutput), "{},", value);
}

// ------------------------------------------------------------------------------------------------
void CtxJSON::PushBool(bool value)
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

// ------------------------------------------------------------------------------------------------
void CtxJSON::PushString(const SQChar * str)
{
    mOutput.push_back('"');
    mOutput.append(str);
    mOutput.push_back('"');
    mOutput.push_back(',');
    // Allow the hook to know
    mString.assign(str);
}

// ------------------------------------------------------------------------------------------------
void CtxJSON::PushString(const SQChar * str, size_t length)
{
    mOutput.push_back('"');
    mOutput.append(str, length);
    mOutput.push_back('"');
    mOutput.push_back(',');
    // Allow the hook to know
    mString.assign(str, length);
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqToJSON(HSQUIRRELVM vm) noexcept
{
    // Make sure the instance is cleaned up even in the case of exceptions
    DeleteGuard< CtxJSON > sq_dg(new CtxJSON());
    // Remember the instance, so we don't have to cast the script object back
    auto ctx = sq_dg.Get();
    // Turn it into a script object because it may be passed as a parameter to `_tojson` meta-methods
    LightObj obj(sq_dg, vm);
    // Proceed with the serialization
    return ctx->SerializeParams(vm);
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqToCompactJSON(HSQUIRRELVM vm) noexcept
{
    // Make sure the instance is cleaned up even in the case of exceptions
    DeleteGuard< CtxJSON > sq_dg(new CtxJSON(false));
    // Remember the instance, so we don't have to cast the script object back
    auto ctx = sq_dg.Get();
    // Turn it into a script object because it may be passed as a parameter to `_tojson` meta-methods
    LightObj obj(sq_dg, vm);
    // Proceed with the serialization
    return ctx->SerializeParams(vm);
}

// ================================================================================================
void Register_JSON(HSQUIRRELVM vm)
{
    RootTable(vm).SquirrelFunc(_SC("SqToJSON"), SqToJSON);
    RootTable(vm).SquirrelFunc(_SC("SqToCompactJSON"), SqToCompactJSON);
    RootTable(vm).SquirrelFunc(_SC("SqFromJSON"), SqFromJSON);
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(_SC("SqCtxJSON"),
        Class< CtxJSON, NoCopy< CtxJSON > >(vm, SqCtxJSON::Str)
        // Constructors
        .Ctor()
        .Ctor< bool >()
        .Ctor< bool, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqCtxJSON::Fn)
        // Properties
        .Prop(_SC("Output"), &CtxJSON::GetOutput)
        .Prop(_SC("Depth"), &CtxJSON::GetDepth)
        .Prop(_SC("OOA"), &CtxJSON::GetObjectOverArray, &CtxJSON::SetObjectOverArray)
        .Prop(_SC("ObjectOverArray"), &CtxJSON::GetObjectOverArray, &CtxJSON::SetObjectOverArray)
        // Member Methods
        .SquirrelMethod< CtxJSON, &CtxJSON::SerializeParams >(_SC("Serialize"))
        .SquirrelMethod< CtxJSON, &CtxJSON::PushValues >(_SC("PushValues"))
        .SquirrelMethod< CtxJSON, &CtxJSON::PushElement >(_SC("PushElement"))
        .Func(_SC("OpenArray"), &CtxJSON::OpenArray)
        .Func(_SC("CloseArray"), &CtxJSON::CloseArray)
        .Func(_SC("OpenObject"), &CtxJSON::OpenObject)
        .Func(_SC("CloseObject"), &CtxJSON::CloseObject)
        .Func(_SC("MakeKey"), &CtxJSON::MakeKey)
        .FmtFunc(_SC("PushKey"), &CtxJSON::PushKey)
        .Func(_SC("SetOOA"), &CtxJSON::SetObjectOverArray)
        .Func(_SC("SetObjectOverArray"), &CtxJSON::SetObjectOverArray)
    );
}

} // Namespace:: SqMod
