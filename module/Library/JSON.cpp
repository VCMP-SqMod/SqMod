// ------------------------------------------------------------------------------------------------
#include "Library/JSON.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
#include <cstdio>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqCtxJSON, _SC("SqCtxJSON"))

// ------------------------------------------------------------------------------------------------
static SQInteger SqToJSON(HSQUIRRELVM vm) noexcept
{
    return CtxJSON().SerializeParams(vm);
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqToCompactJSON(HSQUIRRELVM vm) noexcept
{
    return CtxJSON(false).SerializeParams(vm);
}

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
