// ------------------------------------------------------------------------------------------------
#include "Library/Format.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Common implementation of formattable script types.
*/
struct SqObjectFmt
{
    /* --------------------------------------------------------------------------------------------
     * Object VM.
    */
    HSQUIRRELVM mVM{};

    /* --------------------------------------------------------------------------------------------
     * Script table object.
    */
    HSQOBJECT   mObj{};

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    SqObjectFmt(HSQUIRRELVM vm, SQInteger idx, SQInteger & res)
        : mVM(vm), mObj()
    {
        res = sq_getstackobj(vm, idx, &mObj);
        // Not checking if this is the correct type because it shouldn't get here if it wasn't
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SqObjectFmt(const SqObjectFmt & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SqObjectFmt(SqObjectFmt && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~SqObjectFmt() =  default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    SqObjectFmt & operator = (const SqObjectFmt & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    SqObjectFmt & operator = (SqObjectFmt && o) noexcept = default;
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of SqObjectFmt for array type.
*/
struct SqArrayFmt : public SqObjectFmt
{
    using SqObjectFmt::SqObjectFmt;
    using SqObjectFmt::operator =;
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of SqObjectFmt for table type.
*/
struct SqTableFmt : public SqObjectFmt
{
    using SqObjectFmt::SqObjectFmt;
    using SqObjectFmt::operator =;
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of SqObjectFmt for class type.
*/
struct SqClassFmt : public SqObjectFmt
{
    using SqObjectFmt::SqObjectFmt;
    using SqObjectFmt::operator =;
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of SqObjectFmt for instance type.
*/
struct SqInstanceFmt : public SqObjectFmt
{
    using SqObjectFmt::SqObjectFmt;
    using SqObjectFmt::operator =;
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of SqObjectFmt for closure type.
*/
struct SqClosureFmt : public SqObjectFmt
{
    using SqObjectFmt::SqObjectFmt;
    using SqObjectFmt::operator =;
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of SqObjectFmt for generator type.
*/
struct SqGeneratorFmt : public SqObjectFmt
{
    using SqObjectFmt::SqObjectFmt;
    using SqObjectFmt::operator =;
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of SqObjectFmt for weak reference type.
*/
struct SqWeakReferenceFmt : public SqObjectFmt
{
    using SqObjectFmt::SqObjectFmt;
    using SqObjectFmt::operator =;
};

// ------------------------------------------------------------------------------------------------
SQInteger FormatContext::Proc(HSQUIRRELVM vm, SQInteger text, SQInteger args, SQInteger end)
{
    // Is there any argument limit?
    if (end < 0) end = sq_gettop(vm);
    // Common value storage
    SQInteger i = 0;
    SQBool b = 0;
    SQFloat f = 0;
    const SQChar * s = nullptr;
    // Attempt to retrieve the string value from the stack
    mRes = sq_getstringandsize(vm, text, &s, &i);
    // By any chance we failed to retrieve string?
    if (SQ_FAILED(mRes))
    {
        return mRes;
    // Drop previous arguments, if any
    } else mArgs.clear();
    // Remember the format string and assume it lives on the stack
    fmt::basic_string_view< SQChar > f_str(s, static_cast< size_t >(i));
    // Process the arguments in the specified range
    for(SQInteger idx = args; SQ_SUCCEEDED(mRes) && idx <= end; ++idx)
    {
        switch(sq_gettype(vm, idx))
        {
            // Null?
            case OT_NULL: {
                mArgs.push_back(nullptr);
            } break;
            // Integer?
            case OT_INTEGER: {
                mRes = sq_getinteger(vm, idx, &i);
                mArgs.push_back(i);
            } break;
            // Float?
            case OT_FLOAT: {
                mRes = sq_getfloat(vm, idx, &f);
                mArgs.push_back(f);
            } break;
            // Boolean?
            case OT_BOOL: {
                mRes = sq_getbool(vm, idx, &b);
                mArgs.push_back(static_cast< bool >(b));
            } break;
            // String?
            case OT_STRING: {
                mRes = sq_getstringandsize(vm, idx, &s, &i);
                if (SQ_SUCCEEDED(mRes)) mArgs.push_back(fmt::basic_string_view< SQChar >(s, static_cast< size_t >(i)));
            } break;
            // Table?
            case OT_TABLE: {
                SqTableFmt o(vm, idx, mRes);
                if (SQ_SUCCEEDED(mRes)) mArgs.push_back(o);
            } break;
            // Array?
            case OT_ARRAY: {
                SqArrayFmt o(vm, idx, mRes);
                if (SQ_SUCCEEDED(mRes)) mArgs.push_back(o);
            } break;
            // User data?
            case OT_USERDATA: {
                SQUserPointer p = nullptr;
                mRes = sq_getuserdata(vm, idx, &p, nullptr);
                if (SQ_SUCCEEDED(mRes)) mArgs.push_back(static_cast< void * >(p));
            } break;
            // Closure?
            case OT_CLOSURE:
            case OT_NATIVECLOSURE: {
                SqClosureFmt o(vm, idx, mRes);
                if (SQ_SUCCEEDED(mRes)) mArgs.push_back(o);
            } break;
            // Generator?
            case OT_GENERATOR: {
                SqGeneratorFmt o(vm, idx, mRes);
                if (SQ_SUCCEEDED(mRes)) mArgs.push_back(o);
            } break;
            // User pointer?
            case OT_USERPOINTER: {
                SQUserPointer p = nullptr;
                mRes = sq_getuserpointer(vm, idx, &p);
                if (SQ_SUCCEEDED(mRes)) mArgs.push_back(static_cast< void * >(p));
            } break;
            // Class?
            case OT_CLASS: {
                SqClassFmt o(vm, idx, mRes);
                if (SQ_SUCCEEDED(mRes)) mArgs.push_back(o);
            } break;
            // Instance?
            case OT_INSTANCE: {
                SqInstanceFmt o(vm, idx, mRes);
                if (SQ_SUCCEEDED(mRes)) mArgs.push_back(o);
            } break;
            // Weak reference?
            case OT_WEAKREF: {
                SqWeakReferenceFmt o(vm, idx, mRes);
                if (SQ_SUCCEEDED(mRes)) mArgs.push_back(o);
            } break;
            // Unknown!
            default: mRes = sq_throwerror(vm, "Unknown or unsupported object type");
        }
    }
    // Attempt to generate the format string
    if (SQ_SUCCEEDED(mRes))
    {
        try
        {
            mOut = fmt::vformat(f_str, mArgs);
        }
        catch (const std::exception & e)
        {
            mRes = sq_throwerror(vm, e.what());
        }
    }
    // Return result
    return mRes;
}

// ------------------------------------------------------------------------------------------------
template < class FormatContext >
inline auto FormatObjectInContext(HSQUIRRELVM vm, SQInteger idx, SQInteger src, FormatContext & ctx) -> decltype(ctx.out())
{
    // Identify what type of value we should format
    switch(sq_gettype(vm, idx))
    {
        // Null?
        case OT_NULL: {
            return fmt::format_to(ctx.out(), "0x0");
        }
        // Integer?
        case OT_INTEGER: {
            SQInteger i = 0;
            sq_getinteger(vm, idx, &i);
            return fmt::format_to(ctx.out(), "{}", i);
        }
        // Float?
        case OT_FLOAT: {
            SQFloat f = 0;
            sq_getfloat(vm, idx, &f);
            return fmt::format_to(ctx.out(), "{}", f);
        }
        // Boolean?
        case OT_BOOL: {
            SQBool b = 0;
            sq_getbool(vm, idx, &b);
            return fmt::format_to(ctx.out(), "{}", b ? "true" : "false");
        }
        // String?
        case OT_STRING: {
            SQInteger i = 0;
            const SQChar * s = nullptr;
            sq_getstringandsize(vm, idx, &s, &i);
            return fmt::format_to(ctx.out(), "{}", fmt::basic_string_view< SQChar >(s, static_cast< size_t >(i)));
        }
        // Table? TODO: generate json-style string!
        case OT_TABLE: throw fmt::format_error("no viable conversion from table to string");
        // Array? TODO: generate json-style string!
        case OT_ARRAY: throw fmt::format_error("no viable conversion from array to string");
        case OT_USERDATA: {
            SQUserPointer p = nullptr;
            sq_getuserdata(vm, idx, &p, nullptr);
            return fmt::format_to(ctx.out(), "{:p}", p);
        }
        // Closure?
        case OT_CLOSURE:
        case OT_NATIVECLOSURE: {
            SQInteger n = 0, x;
            // Retrieve the number of parameters that this closure expects
            if (SQ_SUCCEEDED(sq_getclosureinfo(vm, idx, &n, &x)))
            {
                // Does this closure need any parameters?
                if (n <= 1)
                {
                    const SQObjectType st = sq_gettype(vm, src);
                    // is the source is a class or instance?
                    if (st == OT_CLASS || st == OT_INSTANCE)
                    {
                        sq_push(vm, src); // Use that as environment
                    }
                    else
                    {
                        sq_push(vm, 1); // Use the current environment
                    }
                    // Attempt to invoke the function
                    if (SQ_SUCCEEDED(sq_call(vm, 1, SQTrue, SQFalse)))
                    {
                        // At this point we have a new object on the stack that must be removed
                        SqPopTopGuard tpg(vm);
                        // Invoke this function again on the returned value
                        return FormatObjectInContext(vm, -1, src, ctx);
                    } else throw fmt::format_error(LastErrorString(vm));
                }
                else
                {
                    std::string s("closure requires ");
                    fmt::format_int f(n);
                    s.append(f.data(), f.size());
                    s.append(" parameters and cannot be invoked");
                    throw fmt::format_error(s);
                }
            } else throw fmt::format_error("failed to retrieve closure information");
        }
        // Generator?
        case OT_GENERATOR: {
            // Guard the stack size
            StackGuard sg(vm);
            // Is the generator at the top of the stack?
            if (idx != -1 && idx != sq_gettop(vm))
            {
                sq_push(vm, idx); // Then push it
            }
            // Keep resuming the generator until null is returned
            while (true)
            {
                if (SQ_SUCCEEDED(sq_resume(vm, SQTrue, SQFalse)))
                {
                    // At this point we have a new object on the stack that must be removed
                    ::SqMod::SqPopTopGuard rpg(vm);
                    // Is the returned value not null?
                    if (sq_gettype(vm, -1) != OT_NULL)
                    {
                        // Invoke this function again on the returned value
                        return FormatObjectInContext(vm, -1, src, ctx);
                    } else break; // Stop
                } else throw fmt::format_error(LastErrorString(vm));
            }
        }
        // user pointer?
        case OT_USERPOINTER: {
            SQUserPointer p = nullptr;
            sq_getuserpointer(vm, idx, &p);
            return fmt::format_to(ctx.out(), "{:p}", p);
        }
        // Class?
        case OT_CLASS: {
            // Attempt to retrieve the type name of the specified value
            if (SQ_SUCCEEDED(sq_typeof(vm, idx)))
            {
                // At this point we have a new object on the stack that must be removed
                SqPopTopGuard tpg(vm);
                // The value on the stack may not be a string
                if (SQ_SUCCEEDED(sq_tostring(vm, -1)))
                {
                    SQInteger i = 0;
                    const SQChar * s = nullptr;
                    // At this point we have a new object on the stack that must be removed
                    SqPopTopGuard spg(vm);
                    // Attempt to obtain the string pointer
                    if (SQ_SUCCEEDED(sq_getstringandsize(vm, -1, &s, &i)))
                    {
                        return fmt::format_to(ctx.out(), "{}", fmt::basic_string_view< SQChar >(s, static_cast< size_t >(i)));
                    } else throw fmt::format_error("failed to retrieve type name string from stack");
                } else throw fmt::format_error("failed to convert class type name to string");
            } else throw fmt::format_error("no viable conversion from class to string");
        }
        // Instance?
        case OT_INSTANCE: {
            // Attempt to convert the value from the stack to a string
            if (SQ_SUCCEEDED(sq_tostring(vm, idx)))
            {
                SQInteger i = 0;
                const SQChar * s = nullptr;
                // At this point we have a new object on the stack that must be removed
                SqPopTopGuard spg(vm);
                // Attempt to obtain the string pointer
                if (SQ_SUCCEEDED(sq_getstringandsize(vm, -1, &s, &i)))
                {
                    return fmt::format_to(ctx.out(), "{}", fmt::basic_string_view< SQChar >(s, static_cast< size_t >(i)));
                } else throw fmt::format_error("failed to retrieve instance string representation from stack");
            } else throw fmt::format_error("no viable conversion from instance to string");
        }
        // Weak reference?
        case OT_WEAKREF: {
            // Push the referenced object on the stack
            if (SQ_SUCCEEDED(sq_getweakrefval(vm, idx)))
            {
                // At this point we have a new object on the stack that must be removed
                SqPopTopGuard vpg(vm);
                // Invoke this function again on the referenced object
                return FormatObjectInContext(vm, -1, src, ctx);
            } else throw fmt::format_error("failed to retrieve weak reference value");
        }
        // Unknown
        default: break;
    }
    // Unknown type
    throw fmt::format_error("unknown or unsupported value type");
}

// ------------------------------------------------------------------------------------------------
void ExtendedFormatProcess(StackStrF & ss, SQInteger top)
{
    FormatContext ctx;
    // Attempt to perform the format
    ss.mRes = ctx.Proc(ss.mVM, ss.mIdx, ss.mIdx + 1, top);
    // Did format succeed?
    if (SQ_SUCCEEDED(ss.mRes))
    {
        // Transform the string into a script object
        sq_pushstring(ss.mVM, ctx.mOut.data(), static_cast<SQInteger>(ctx.mOut.size()));
        // At this point we have a new object on the stack that must be removed
        SqPopTopGuard spg(ss.mVM);
        // Obtain a reference to the string object
        ss.mRes = sq_getstackobj(ss.mVM, -1, &ss.mObj);
        // Could we retrieve the object from the stack?
        if (SQ_SUCCEEDED(ss.mRes))
        {
            // Keep a strong reference to the object
            sq_addref(ss.mVM, &ss.mObj);
            // Attempt to retrieve the string value from the stack
            ss.mRes = sq_getstringandsize(ss.mVM, -1, &ss.mPtr, &ss.mLen);
        }
        // Did the retrieval succeeded but ended up with a null string pointer?
        if (SQ_SUCCEEDED(ss.mRes) && !ss.mPtr)
        {
            ss.mRes = sq_throwerror(ss.mVM, "Unable to retrieve the string");
        }
    }
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqFormat(HSQUIRRELVM vm)
{
    FormatContext ctx;
    // Attempt to generate the formatted string
    if (SQ_FAILED(ctx.Proc(vm, 2, 3, sq_gettop(vm))))
    {
        return ctx.mRes;
    }
    // Push it on the stack
    sq_pushstring(vm, ctx.mOut.data(), static_cast< SQInteger >(ctx.mOut.size()));
    // Specify that we returned a value
    return 1;
}

// ================================================================================================
void Register_Format(HSQUIRRELVM vm)
{
    RootTable(vm).SquirrelFunc(_SC("fmt"), SqFormat);
}

} // Namespace:: SqMod

// ------------------------------------------------------------------------------------------------
namespace fmt {

/* ------------------------------------------------------------------------------------------------
 * Implements formatting for script arrays.
*/
template < > struct formatter< ::SqMod::SqArrayFmt >
{
    size_t mIndex{};

    /* --------------------------------------------------------------------------------------------
     * Parse the specified string range.
    */
    auto parse(format_parse_context & ctx) -> decltype(ctx.begin())
    {
        // Retrieve the range that to be parsed
        auto it = ctx.begin(), begin = it, end = ctx.end();
        // Find the closing brace
        while (it != end && *it != '}') ++it;
        // Check if closing brace was found
        if (it != end && *it != '}')
        {
            throw format_error("missing curly brace }");
        }
        // Check if an index was specified
        else if (it == begin)
        {
            mIndex = 0; // Default to first element
        }
        else
        {
            char * p_end = nullptr;
            // Transform the value into an index
        #ifdef _SQ64
            mIndex = std::strtoull(&(*begin), &p_end, 10);
        #else
            mIndex = std::strtoul(&(*begin), &p_end, 10);
        #endif
            // Validate index range
            if (errno == ERANGE)
            {
                throw format_error("index out of range");
            }
            // Validate index value
            else if (*p_end != '}')
            {
                throw format_error("invalid integer index");
            }
        }
        // Return an iterator past the end of the parsed range:
        return it;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform the requested format.
    */
    template < class FormatContext >
    auto format(const ::SqMod::SqArrayFmt & o, FormatContext & ctx) -> decltype(ctx.out())
    {
        // Remember the stack size
        SQInteger top = sq_gettop(o.mVM);
        // Push the array object on the stack
        sq_pushobject(o.mVM, o.mObj);
        // At this point we have a new object on the stack that must be removed
        ::SqMod::SqPopTopGuard apg(o.mVM);
        // Push the index on the stack
        sq_pushinteger(o.mVM, static_cast< SQInteger >(mIndex));
        // Attempt to retrieve the index at the specified index
        if (SQ_FAILED(sq_get(o.mVM, -2)))
        {
            std::string s("no such index in array: ");
            fmt::format_int f(mIndex);
            s.append(f.data(), f.size());
            throw format_error(s);
        }
        // At this point we have a new object on the stack that must be removed
        ::SqMod::SqPopTopGuard epg(o.mVM);
        // Run the format on the object that was pushed on the stack
        return ::SqMod::FormatObjectInContext(o.mVM, -1, top + 1, ctx);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Implements formatting for script tables.
*/
template < > struct formatter< ::SqMod::SqTableFmt >
{
    std::string mIndex{};

    /* --------------------------------------------------------------------------------------------
     * Parse the specified string range.
    */
    auto parse(format_parse_context & ctx) -> decltype(ctx.begin())
    {
        // Retrieve the range that to be parsed
        auto it = ctx.begin(), begin = it, end = ctx.end();
        // Find the closing brace
        while (it != end && *it != '}') ++it;
        // Check if closing brace was found
        if (it != end && *it != '}')
        {
            throw format_error("missing curly brace }");
        }
        // Check if a name was specified
        else if (it == begin)
        {
            throw format_error("must specify a table index");
        }
        // Store the index name
        mIndex.assign(begin, it);
        // Return an iterator past the end of the parsed range:
        return it;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform the requested format.
    */
    template < class FormatContext >
    auto format(const ::SqMod::SqTableFmt & o, FormatContext & ctx) -> decltype(ctx.out())
    {
        // Remember the stack size
        SQInteger top = sq_gettop(o.mVM);
        // Push the array object on the stack
        sq_pushobject(o.mVM, o.mObj);
        // At this point we have a new object on the stack that must be removed
        ::SqMod::SqPopTopGuard apg(o.mVM);
        // Push the index on the stack (only works with string keys/elements)
        sq_pushstring(o.mVM, mIndex.data(), static_cast< SQInteger >(mIndex.size()));
        // Attempt to retrieve the index at the specified index
        if (SQ_FAILED(sq_get(o.mVM, -2)))
        {
            std::string s("no such index in table: ");
            s.append(mIndex);
            throw format_error(s);
        }
        // At this point we have a new object on the stack that must be removed
        ::SqMod::SqPopTopGuard epg(o.mVM);
        // Run the format on the object that was pushed on the stack
        return ::SqMod::FormatObjectInContext(o.mVM, -1, top + 1, ctx);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Implements formatting for script classes.
*/
template < > struct formatter< ::SqMod::SqClassFmt >
{
    std::string mIndex{};

    /* --------------------------------------------------------------------------------------------
     * Parse the specified string range.
    */
    auto parse(format_parse_context & ctx) -> decltype(ctx.begin())
    {
        // Retrieve the range that to be parsed
        auto it = ctx.begin(), begin = it, end = ctx.end();
        // Find the closing brace
        while (it != end && *it != '}') ++it;
        // Check if closing brace was found
        if (it != end && *it != '}')
        {
            throw format_error("missing curly brace }");
        }
        // Check if a identifier was specified
        else if (it == begin)
        {
            throw format_error("must specify a member identifier");
        }
        // Validate first character
        else if (std::isalpha(*begin) == 0 && *begin != '_')
        {
            throw format_error("member names must start with an alphabetic or underscore character");
        }
        // Validate member identifier
        else if (!std::all_of(begin, it, [](int c) { return std::isalnum(c) != 0 || c == '_'; }))
        {
            throw format_error("member names can only contain alpha-numeric or underscore characters");
        }
        // Store the member identifier
        mIndex.assign(begin, it);
        // Return an iterator past the end of the parsed range:
        return it;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform the requested format.
    */
    template < class FormatContext >
    auto format(const ::SqMod::SqClassFmt & o, FormatContext & ctx) -> decltype(ctx.out())
    {
        // Remember the stack size
        SQInteger top = sq_gettop(o.mVM);
        // Push the class object on the stack
        sq_pushobject(o.mVM, o.mObj);
        // At this point we have a new object on the stack that must be removed
        ::SqMod::SqPopTopGuard apg(o.mVM);
        // Push the index on the stack (only works with string keys/elements)
        sq_pushstring(o.mVM, mIndex.data(), static_cast< SQInteger >(mIndex.size()));
        // Attempt to retrieve the index at the specified index
        if (SQ_FAILED(sq_get(o.mVM, -2)))
        {
            std::string s("no such identifier in class: ");
            s.append(mIndex);
            throw format_error(s);
        }
        // At this point we have a new object on the stack that must be removed
        ::SqMod::SqPopTopGuard epg(o.mVM);
        // Run the format on the object that was pushed on the stack
        return ::SqMod::FormatObjectInContext(o.mVM, -1, top + 1, ctx);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Implements formatting for script instances.
*/
template < > struct formatter< ::SqMod::SqInstanceFmt >
{
    std::string mIndex{};

    /* --------------------------------------------------------------------------------------------
     * Parse the specified string range.
    */
    auto parse(format_parse_context & ctx) -> decltype(ctx.begin())
    {
        // Retrieve the range that to be parsed
        auto it = ctx.begin(), begin = it, end = ctx.end();
        // Find the closing brace
        while (it != end && *it != '}') ++it;
        // Check if closing brace was found
        if (it != end && *it != '}')
        {
            throw format_error("missing curly brace }");
        }
        // Check if a identifier was specified
        else if (it == begin)
        {
            throw format_error("must specify a member identifier");
        }
        // Validate first character
        else if (std::isalpha(*begin) == 0 && *begin != '_')
        {
            throw format_error("member names must start with an alphabetic or underscore character");
        }
        // Validate member identifier
        else if (!std::all_of(begin, it, [](int c) { return std::isalnum(c) != 0 || c == '_'; }))
        {
            throw format_error("member names can only contain alpha-numeric or underscore characters");
        }
        // Store the member identifier
        mIndex.assign(begin, it);
        // Return an iterator past the end of the parsed range:
        return it;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform the requested format.
    */
    template < class FormatContext >
    auto format(const ::SqMod::SqInstanceFmt & o, FormatContext & ctx) -> decltype(ctx.out())
    {
        // Remember the stack size
        SQInteger top = sq_gettop(o.mVM);
        // Push the class object on the stack
        sq_pushobject(o.mVM, o.mObj);
        // At this point we have a new object on the stack that must be removed
        ::SqMod::SqPopTopGuard apg(o.mVM);
        // Push the index on the stack (only works with string keys/elements)
        sq_pushstring(o.mVM, mIndex.data(), static_cast< SQInteger >(mIndex.size()));
        // Attempt to retrieve the index at the specified index
        if (SQ_FAILED(sq_get(o.mVM, -2)))
        {
            std::string s("no such identifier in instance: ");
            s.append(mIndex);
            throw format_error(s);
        }
        // At this point we have a new object on the stack that must be removed
        ::SqMod::SqPopTopGuard epg(o.mVM);
        // Run the format on the object that was pushed on the stack
        return ::SqMod::FormatObjectInContext(o.mVM, -1, top + 1, ctx);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Implements formatting for script closures.
*/
template < > struct formatter< ::SqMod::SqClosureFmt >
{
    std::string mParameter{};

    /* --------------------------------------------------------------------------------------------
     * Parse the specified string range.
    */
    auto parse(format_parse_context & ctx) -> decltype(ctx.begin())
    {
        // Retrieve the range that to be parsed
        auto it = ctx.begin(), begin = it, end = ctx.end();
        // Find the closing brace
        while (it != end && *it != '}') ++it;
        // Check if closing brace was found
        if (it != end && *it != '}')
        {
            throw format_error("missing curly brace }");
        }
        // Check if a parameter was specified
        else if (it != begin)
        {
            mParameter.assign(begin, it); // Store the parameter
        }
        // Return an iterator past the end of the parsed range:
        return it;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform the requested format.
    */
    template < class FormatContext >
    auto format(const ::SqMod::SqClosureFmt & o, FormatContext & ctx) -> decltype(ctx.out())
    {
        SQInteger n = 0, x;
        // Push the closure object on the stack
        sq_pushobject(o.mVM, o.mObj);
        // At this point we have a new object on the stack that must be removed
        ::SqMod::SqPopTopGuard opg(o.mVM);
        // Retrieve the number of parameters that this closure expects
        if (SQ_SUCCEEDED(sq_getclosureinfo(o.mVM, -1, &n, &x)))
        {
            // Does this closure need any parameters?
            if (n <= 2)
            {
                // Use the current environment
                sq_push(o.mVM, 1);
                // If it requires a parameter then we push the string anyway
                if (n == 2 || !mParameter.empty())
                {
                    sq_pushstring(o.mVM, mParameter.data(), static_cast< SQInteger >(mParameter.size()));
                    x = 2; // Include string in parameter count
                } else x = 1; // Exclude string from parameter count
                // Attempt to invoke the function
                if (SQ_SUCCEEDED(sq_call(o.mVM, x, SQTrue, SQFalse)))
                {
                    // At this point we have a new object on the stack that must be removed
                    ::SqMod::SqPopTopGuard tpg(o.mVM);
                    // Invoke this function again on the returned value
                    ::SqMod::FormatObjectInContext(o.mVM, -1, 1, ctx);
                } else throw fmt::format_error(::SqMod::LastErrorString(o.mVM));
            }
            else
            {
                std::string s("closure requires ");
                fmt::format_int f(n);
                s.append(f.data(), f.size());
                s.append(" parameters and cannot be invoked");
                throw fmt::format_error(s);
            }
        } else throw fmt::format_error("failed to retrieve closure information");
        // Return the output iterator
        return ctx.out();
    }
};

/* ------------------------------------------------------------------------------------------------
 * Implements formatting for script generators.
*/
template < > struct formatter< ::SqMod::SqGeneratorFmt >
{
    std::string mDelimiter{};

    /* --------------------------------------------------------------------------------------------
     * Parse the specified string range.
    */
    auto parse(format_parse_context & ctx) -> decltype(ctx.begin())
    {
        // Retrieve the range that to be parsed
        auto it = ctx.begin(), begin = it, end = ctx.end();
        // Find the closing brace
        while (it != end && *it != '}') ++it;
        // Check if closing brace was found
        if (it != end && *it != '}')
        {
            throw format_error("missing curly brace }");
        }
        // Check if a delimiter was specified
        else if (it != begin)
        {
            mDelimiter.assign(begin, it); // Store the delimiter
        }
        // Return an iterator past the end of the parsed range:
        return it;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform the requested format.
    */
    template < class FormatContext >
    auto format(const ::SqMod::SqGeneratorFmt & o, FormatContext & ctx) -> decltype(ctx.out())
    {
        // Push the generator object on the stack
        sq_pushobject(o.mVM, o.mObj);
        // At this point we have a new object on the stack that must be removed
        ::SqMod::SqPopTopGuard opg(o.mVM);
        // Keep resuming the generator until null is returned
        for (SQInteger n = 0; /*...*/ ; ++n)
        {
            if (SQ_SUCCEEDED(sq_resume(o.mVM, SQTrue, SQFalse)))
            {
                // At this point we have a new object on the stack that must be removed
                ::SqMod::SqPopTopGuard rpg(o.mVM);
                // Is the returned value not null?
                if (sq_gettype(o.mVM, -1) != OT_NULL)
                {
                    // Do not include the delimiter before the first or after last value
                    if (n != 0) format_to(ctx.out(), "{:s}", mDelimiter);
                    // Run the format on the object that was pushed on the stack
                    ::SqMod::FormatObjectInContext(o.mVM, -1, 1, ctx);
                } else break; // Stop
            } else throw format_error(::SqMod::LastErrorString(o.mVM));
        }
        // Return the output iterator
        return ctx.out();
    }
};

/* ------------------------------------------------------------------------------------------------
 * Implements formatting for script generators.
*/
template < > struct formatter< ::SqMod::SqWeakReferenceFmt >
{
    /* --------------------------------------------------------------------------------------------
     * Parse the specified string range.
    */
    auto parse(format_parse_context & ctx) -> decltype(ctx.begin()) // NOLINT(readability-convert-member-functions-to-static)
    {
        // Retrieve the range that to be parsed
        auto it = ctx.begin(), begin = it, end = ctx.end();
        // Find the closing brace
        while (it != end && *it != '}') ++it;
        // Check if closing brace was found
        if (it != end && *it != '}')
        {
            throw format_error("missing curly brace }");
        }
        // Check if a delimiter was specified
        else if (it != begin)
        {
            throw format_error("weak references don't accept format specifiers");
        }
        // Return an iterator past the end of the parsed range:
        return it;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform the requested format.
    */
    template < class FormatContext >
    auto format(const ::SqMod::SqWeakReferenceFmt & o, FormatContext & ctx) -> decltype(ctx.out())
    {
        // Push the reference object on the stack
        sq_pushobject(o.mVM, o.mObj);
        // At this point we have a new object on the stack that must be removed
        ::SqMod::SqPopTopGuard opg(o.mVM);
        // Push the referenced object on the stack
        if (SQ_FAILED(sq_getweakrefval(o.mVM, -1)))
        {
            throw fmt::format_error("failed to retrieve weak reference value");
        }
        // At this point we have a new object on the stack that must be removed
        ::SqMod::SqPopTopGuard vpg(o.mVM);
        // Run the format on the object that was pushed on the stack
        return ::SqMod::FormatObjectInContext(o.mVM, -1, 1, ctx);
    }
};

} // Namespace:: fmt
