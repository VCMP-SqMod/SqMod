// ------------------------------------------------------------------------------------------------
#include "Base/Stack.hpp"
#include "Base/Shared.hpp"
#include "Base/Buffer.hpp"
#include "Library/Numeric.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqstdstring.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger PopStackInteger(HSQUIRRELVM vm, SQInteger idx)
{
    // Identify which type must be extracted
    switch (sq_gettype(vm, idx))
    {
        case OT_INTEGER:
        {
            SQInteger val;
            sq_getinteger(vm, idx, &val);
            return val;
        } break;
        case OT_FLOAT:
        {
            SQFloat val;
            sq_getfloat(vm, idx, &val);
            return ConvTo< SQInteger >::From(val);
        } break;
        case OT_BOOL:
        {
            SQBool val;
            sq_getbool(vm, idx, &val);
            return static_cast< SQInteger >(val);
        } break;
        case OT_STRING:
        {
            CSStr val = nullptr;
            // Attempt to retrieve and convert the string
            if (SQ_SUCCEEDED(sq_getstring(vm, idx, &val)) && val != nullptr && *val != '\0')
            {
                return ConvTo< SQInteger >::From(std::strtoll(val, nullptr, 10));
            }
        } break;
        case OT_ARRAY:
        case OT_TABLE:
        case OT_CLASS:
        case OT_USERDATA:
        {
            return sq_getsize(vm, idx);
        } break;
        case OT_INSTANCE:
        {
            // Attempt to treat the value as a signed long instance
            try
            {
                return ConvTo< SQInteger >::From(Var< const SLongInt & >(vm, idx).value.GetNum());
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to treat the value as a unsigned long instance
            try
            {
                return ConvTo< SQInteger >::From(Var< const ULongInt & >(vm, idx).value.GetNum());
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to get the size of the instance as a fallback
            return sq_getsize(vm, idx);
        } break;
        default: break;
    }
    // Default to 0
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQFloat PopStackFloat(HSQUIRRELVM vm, SQInteger idx)
{
    // Identify which type must be extracted
    switch (sq_gettype(vm, idx))
    {
        case OT_FLOAT:
        {
            SQFloat val;
            sq_getfloat(vm, idx, &val);
            return val;
        } break;
        case OT_INTEGER:
        {
            SQInteger val;
            sq_getinteger(vm, idx, &val);
            return ConvTo< SQFloat >::From(val);
        } break;
        case OT_BOOL:
        {
            SQBool val;
            sq_getbool(vm, idx, &val);
            return ConvTo< SQFloat >::From(val);
        } break;
        case OT_STRING:
        {
            CSStr val = nullptr;
            // Attempt to retrieve and convert the string
            if (SQ_SUCCEEDED(sq_getstring(vm, idx, &val)) && val != nullptr && *val != '\0')
            {
#ifdef SQUSEDOUBLE
                return std::strtod(val, nullptr);
#else
                return std::strtof(val, nullptr);
#endif // SQUSEDOUBLE
            }
        } break;
        case OT_ARRAY:
        case OT_TABLE:
        case OT_CLASS:
        case OT_USERDATA:
        {
            return ConvTo< SQFloat >::From(sq_getsize(vm, idx));
        } break;
        case OT_INSTANCE:
        {
            // Attempt to treat the value as a signed long instance
            try
            {
                return ConvTo< SQFloat >::From(Var< const SLongInt & >(vm, idx).value.GetNum());
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to treat the value as a unsigned long instance
            try
            {
                return ConvTo< SQFloat >::From(Var< const ULongInt & >(vm, idx).value.GetNum());
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to get the size of the instance as a fallback
            return ConvTo< SQFloat >::From(sq_getsize(vm, idx));
        } break;
        default: break;
    }
    // Default to 0
    return 0.0;
}

// ------------------------------------------------------------------------------------------------
Int64 PopStackLong(HSQUIRRELVM vm, SQInteger idx)
{
    // Identify which type must be extracted
    switch (sq_gettype(vm, idx))
    {
        case OT_INTEGER:
        {
            SQInteger val;
            sq_getinteger(vm, idx, &val);
            return static_cast< Int64 >(val);
        } break;
        case OT_FLOAT:
        {
            SQFloat val;
            sq_getfloat(vm, idx, &val);
            return ConvTo< Int64 >::From(val);
        } break;
        case OT_BOOL:
        {
            SQBool val;
            sq_getbool(vm, idx, &val);
            return static_cast< Int64 >(val);
        } break;
        case OT_STRING:
        {
            CSStr val = nullptr;
            // Attempt to retrieve and convert the string
            if (SQ_SUCCEEDED(sq_getstring(vm, idx, &val)) && val != nullptr && *val != '\0')
            {
                return std::strtoll(val, nullptr, 10);
            }
        } break;
        case OT_ARRAY:
        case OT_TABLE:
        case OT_CLASS:
        case OT_USERDATA:
        {
            return static_cast< Int64 >(sq_getsize(vm, idx));
        } break;
        case OT_INSTANCE:
        {
            // Attempt to treat the value as a signed long instance
            try
            {
                return Var< const SLongInt & >(vm, idx).value.GetNum();
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to treat the value as a unsigned long instance
            try
            {
                return ConvTo< Int64 >::From(Var< const ULongInt & >(vm, idx).value.GetNum());
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to get the size of the instance as a fallback
            return static_cast< Int64 >(sq_getsize(vm, idx));
        } break;
        default: break;
    }
    // Default to 0
    return 0;
}

// ------------------------------------------------------------------------------------------------
Uint64 PopStackULong(HSQUIRRELVM vm, SQInteger idx)
{
    // Identify which type must be extracted
    switch (sq_gettype(vm, idx))
    {
        case OT_INTEGER:
        {
            SQInteger val;
            sq_getinteger(vm, idx, &val);
            return ConvTo< Uint64 >::From(val);
        } break;
        case OT_FLOAT:
        {
            SQFloat val;
            sq_getfloat(vm, idx, &val);
            return ConvTo< Uint64 >::From(val);
        } break;
        case OT_BOOL:
        {
            SQBool val;
            sq_getbool(vm, idx, &val);
            return ConvTo< Uint64 >::From(val);
        } break;
        case OT_STRING:
        {
            CSStr val = nullptr;
            // Attempt to retrieve and convert the string
            if (SQ_SUCCEEDED(sq_getstring(vm, idx, &val)) && val != nullptr && *val != '\0')
            {
                return std::strtoull(val, nullptr, 10);
            }
        } break;
        case OT_ARRAY:
        case OT_TABLE:
        case OT_CLASS:
        case OT_USERDATA:
        {
            return ConvTo< Uint64 >::From(sq_getsize(vm, idx));
        } break;
        case OT_INSTANCE:
        {
            // Attempt to treat the value as a signed long instance
            try
            {
                return ConvTo< Uint64 >::From(Var< const SLongInt & >(vm, idx).value.GetNum());
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to treat the value as a unsigned long instance
            try
            {
                return Var< const ULongInt & >(vm, idx).value.GetNum();
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to get the size of the instance as a fallback
            return ConvTo< Uint64 >::From(sq_getsize(vm, idx));
        } break;
        default: break;
    }
    // Default to 0
    return 0;
}

// --------------------------------------------------------------------------------------------
StackStrF::StackStrF(HSQUIRRELVM vm, SQInteger idx, bool fmt)
    : mPtr(nullptr)
    , mLen(-1)
    , mRes(SQ_OK)
    , mObj()
    , mVM(vm)
{
    const Int32 top = sq_gettop(vm);
    // Reset the converted value object
    sq_resetobject(&mObj);
    // Was the string or value specified?
    if (top <= (idx - 1))
    {
        mRes = sq_throwerror(vm, "Missing string or value");
    }
    // Do we have enough values to call the format function and are we allowed to?
    else if (top > idx && fmt)
    {
        // Pointer to the generated string
        SStr str = nullptr;
        // Attempt to generate the specified string format
        mRes = sqstd_format(vm, idx, &mLen, &str);
        // Did the format succeeded but ended up with a null string pointer?
        if (SQ_SUCCEEDED(mRes) && !str)
        {
            mRes = sq_throwerror(vm, "Unable to generate the string");
        }
        else
        {
            mPtr = const_cast< CSStr >(str);
        }
    }
    // Is the value on the stack an actual string?
    else if (sq_gettype(vm, idx) == OT_STRING)
    {
        // Obtain a reference to the string object
        mRes = sq_getstackobj(vm, idx, &mObj);
        // Could we retrieve the object from the stack?
        if (SQ_SUCCEEDED(mRes))
        {
            // Keep a strong reference to the object
            sq_addref(vm, &mObj);
            // Attempt to retrieve the string value from the stack
            mRes = sq_getstring(vm, idx, &mPtr);
        }
        // Did the retrieval succeeded but ended up with a null string pointer?
        if (SQ_SUCCEEDED(mRes) && !mPtr)
        {
            mRes = sq_throwerror(vm, "Unable to retrieve the string");
        }
    }
    // We have to try and convert it to string
    else
    {
        // Attempt to convert the value from the stack to a string
        mRes = sq_tostring(vm, idx);
        // Could we convert the specified value to string?
        if (SQ_SUCCEEDED(mRes))
        {
            // Obtain a reference to the resulted object
            mRes = sq_getstackobj(vm, -1, &mObj);
            // Could we retrieve the object from the stack?
            if (SQ_SUCCEEDED(mRes))
            {
                // Keep a strong reference to the object
                sq_addref(vm, &mObj);
                // Attempt to obtain the string pointer
                mRes = sq_getstring(vm, -1, &mPtr);
            }
        }
        // Pop a value from the stack regardless of the result
        sq_pop(vm, 1);
        // Did the retrieval succeeded but ended up with a null string pointer?
        if (SQ_SUCCEEDED(mRes) && !mPtr)
        {
            mRes = sq_throwerror(vm, "Unable to retrieve the value");
        }
    }
}

// ------------------------------------------------------------------------------------------------
StackStrF::~StackStrF()
{
    if (mVM && !sq_isnull(mObj))
    {
        sq_release(mVM, &mObj);
    }
}

// ------------------------------------------------------------------------------------------------
Object BufferToStrObj(const Buffer & b)
{
    // Obtain the initial stack size
    const StackGuard sg(DefaultVM::Get());
    // Push the string onto the stack
    sq_pushstring(DefaultVM::Get(), b.Data(), b.Position());
    // Obtain the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// --------------------------------------------------------------------------------------------
Object BufferToStrObj(const Buffer & b, Uint32 size)
{
    // Perform a range check on the specified buffer
    if (size > b.Capacity())
    {
        STHROWF("The specified buffer size is out of range: %u >= %u", size, b.Capacity());
    }
    // Obtain the initial stack size
    const StackGuard sg(DefaultVM::Get());
    // Push the string onto the stack
    sq_pushstring(DefaultVM::Get(), b.Data(), size);
    // Obtain the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------


} // Namespace:: SqMod
