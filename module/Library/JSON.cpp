// ------------------------------------------------------------------------------------------------
#include "Library/JSON.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
#include <cstdio>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static SQInteger SqToJSON(HSQUIRRELVM vm) noexcept
{
    return sq_throwerror(vm, _SC("Not implemented yet!"));
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqFromJson_Push(HSQUIRRELVM vm, const char * str, jsmntok * t, size_t count) noexcept
{
    // Are there any elements to process?
    if (count == 0)
    {
        // We still need something on the stack
        sq_pushnull(vm);
        // No token consumed
        return 0;
    }
    // Is this a primitive type?
    else if (t->type & JSMN_PRIMITIVE)
    {
        // Primitive length (in characters)
        const jsmnint l = (t->end - t->start);
        // Primitive start (character offset)
        const char * v = (str + t->start);
        // Is this a floating point?
        if (memchr(v, '.', l) || memchr(v, 'e', l) || memchr(v, 'E', l))
        {
            sq_pushfloat(vm, ConvNum< SQFloat >::FromStr(v));
        }
        // Is this an integer?
        else if (((v[0] >= '0') && (v[0] <= '9')) || (v[0] == '-') || (v[0] == '+'))
        {
            sq_pushinteger(vm, ConvNum< SQInteger >::FromStr(v));
        }
        // Is this a boolean true?
        else if (v[0] == 't')
        {
            sq_pushbool(vm, SQTrue);
        }
        // Is this a boolean false?
        else if (v[0] == 'f')
        {
            sq_pushbool(vm, SQFalse);
        }
        // Is this null?
        else if (v[0] == 'n')
        {
            sq_pushnull(vm);
        }
        // Should never really get here because it should be sanitized by the JSON parser
        // But doesn't hurt to have it here in case something out of our scope goes wrong
        else
        {
            return sq_throwerrorf(vm, _SC("Unrecognized JSON primitive: '%.*s'"), l, v);
        }
        // One token was consumed
        return 1;
    }
    // Is this a string?
    else if (t->type & JSMN_STRING)
    {
        sq_pushstring(vm, (str + t->start), static_cast< SQInteger >(t->end - t->start));
        // One token was consumed
        return 1;
    }
    // Is this an object?
    else if (t->type & JSMN_OBJECT)
    {
        // Number of tokens consumed by this object
        SQInteger c = 0, r = SQ_OK;
        // Create a new table on the stack
        sq_newtableex(vm, static_cast< SQInteger >(t->size));
        // Process object elements
        for (jsmnint i = 0; i < t->size; i++)
        {
            // Locate key token relative to the current token
            jsmntok * k = (t + 1 + c);
            // Transform the key into a script object on the stack
            r = SqFromJson_Push(vm, str, k, count - c);
            // Did we fail?
            if (SQ_FAILED(r))
            {
                break; // Abort
            }
            // Update consumed tokens
            c += r;
            // Does the key have an associated value?
            if (k->size > 0)
            {
                // Transform the value into a script object on the stack
                r = SqFromJson_Push(vm, str, (t + 1 + c), count - c);
                // Did we fail?
                if (SQ_FAILED(r))
                {
                    // Discard the key
                    sq_poptop(vm);
                    // Abort
                    break;
                }
                // Update consumed tokens
                c += r;
            }
            else
            {
                sq_pushnull(vm); // Default to null because a value must exist
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
            // Discard the table
            sq_poptop(vm);
            // Propagate the error
            return r;
        }
        // Return consumed tokens
        return c + 1;
    }
    // Is this an array?
    else if (t->type & JSMN_ARRAY)
    {
        // Number of tokens consumed by this array
        SQInteger c = 0, r = SQ_OK;
        // Create a new array on the stack
        sq_newarrayex(vm, static_cast< SQInteger >(t->size));
        // Process array elements
        for (jsmnint i = 0; i < t->size; i++)
        {
            // Transform the value into a script object on the stack
            r = SqFromJson_Push(vm, str, (t + 1 + c), count - c);
            // Did we fail?
            if (SQ_FAILED(r))
            {
                break; // Abort
            }
            // Update consumed tokens
            c += r;
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
            // Discard the array
            sq_poptop(vm);
            // Propagate the error
            return r;
        }
        // Return consumed tokens
        return c + 1;
    }
    // Should never really get here because it should be sanitized by the JSON parser
    // But doesn't hurt to have it here in case something out of our scope goes wrong
    return sq_throwerror(vm, _SC("Unrecognized JSON type"));
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
    // Parser context
    jsmnparser p;
    // Initialize parser
    jsmn_init(&p);
    // Estimate the number of tokens necessary to parse the specified JSON string
    jsmnint r = jsmn_parse(&p, s.mPtr, static_cast< size_t >(s.mLen), nullptr, 0);
    // Is there anything to parse?
    if (r == 0)
    {
        // Default to null
        sq_pushnull(vm);
        // A value was returned
        return 1;
    }
    // See if there was an error
    switch (r)
    {
        case jsmnint(JSMN_ERROR_NOMEM):
            return sq_throwerror(vm, _SC("Not enough token memory was provided"));
        case jsmnint(JSMN_ERROR_LEN):
            return sq_throwerror(vm, _SC("Input data too long"));
        case jsmnint(JSMN_ERROR_INVAL):
            return sq_throwerror(vm, _SC("Invalid character inside JSON string"));
        case jsmnint(JSMN_ERROR_PART):
            return sq_throwerror(vm, _SC("The string is not a full JSON packet, more bytes expected"));
        case jsmnint(JSMN_ERROR_UNMATCHED_BRACKETS):
            return sq_throwerror(vm, _SC("The JSON string has unmatched brackets"));
        default: break; // Nothing bad happened
    }
    // Initialize the token array
    std::vector< jsmntok > tks(static_cast< size_t >(r) + 16);
    // Initialize parser
    jsmn_init(&p);
    // Attempt to parse the specified JSON string
    r = jsmn_parse(&p, s.mPtr, static_cast< size_t >(s.mLen), tks.data(), tks.size());
    // Process the tokens that were parsed from the string
    SQInteger res = SqFromJson_Push(vm, s.mPtr, tks.data(), p.toknext);
    // We either have a value to return or we propagate some error
    return SQ_SUCCEEDED(res) ? 1 : res;
}

// ================================================================================================
void Register_JSON(HSQUIRRELVM vm)
{
    RootTable(vm).SquirrelFunc(_SC("SqToJSON"), SqToJSON);
    RootTable(vm).SquirrelFunc(_SC("SqFromJSON"), SqFromJSON);
}

} // Namespace:: SqMod
