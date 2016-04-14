// ------------------------------------------------------------------------------------------------
#include "Library/Math.hpp"
#include "Library/Numeric.hpp"
#include "Base/Stack.hpp"

// ------------------------------------------------------------------------------------------------
#include <cmath>
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static SQInteger SqDiv(HSQUIRRELVM vm)
{
    // The return type of the function
#ifdef _SQ64
    typedef std::lldiv_t DivT;
#else
    typedef std::div_t DivT;
#endif // _SQ64
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    const DivT r = std::div(PopStackInteger(vm, 2), PopStackInteger(vm, 3));
    // Create a new table on the stack
    sq_newtable(vm);
    // Push the quotient index
    sq_pushstring(vm, _SC("quot"), -1);
    // Push the quotient value
    sq_pushinteger(vm, r.quot);
    // Attempt to create the array element
    if (SQ_FAILED(sq_rawset(vm, -3)))
    {
        return sq_throwerror(vm, "Unable to insert the quotient element");
    }
    // Push the remainder index
    sq_pushstring(vm, _SC("rem"), -1);
    // Push the remainder value
    sq_pushinteger(vm, r.rem);
    // Attempt to create the array element
    if (SQ_FAILED(sq_rawset(vm, -3)))
    {
        return sq_throwerror(vm, "Unable to insert the remainder element");
    }
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqRemainder(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Are we both arguments floats?
    if ((sq_gettype(vm, 2) == OT_FLOAT) && sq_gettype(vm, 3) == OT_FLOAT)
    {
        sq_pushfloat(vm, std::remainder(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    }
    // Are we both arguments integers?
    else if ((sq_gettype(vm, 2) == OT_INTEGER) && sq_gettype(vm, 3) == OT_INTEGER)
    {
        sq_pushinteger(vm, std::remainder(PopStackInteger(vm, 2), PopStackInteger(vm, 3)));
    }
    // Is the first argument float?
    else if ((sq_gettype(vm, 2) == OT_FLOAT))
    {
        sq_pushfloat(vm, std::remainder(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    }
    // Is the first argument integer?
    else if ((sq_gettype(vm, 2) == OT_INTEGER))
    {
        sq_pushinteger(vm, std::remainder(PopStackInteger(vm, 2), PopStackInteger(vm, 3)));
    }
    // Default to both arhuments as float so we don't loos precision from the float one
    else
    {
        sq_pushfloat(vm, std::remainder(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    }
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqFma(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 3)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::fma(PopStackFloat(vm, 2), PopStackFloat(vm, 3), PopStackFloat(vm, 4)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqMax(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::fmax(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqMin(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::fmin(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqDim(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::fdim(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqNan(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Attempt to generate the string value
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Fetch the arguments from the stack and perform the requested operation
#ifdef _SQ64
    sq_pushfloat(vm, std::nan(val.mPtr));
#else
    sq_pushfloat(vm, std::nanf(val.mPtr));
#endif // _SQ64
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqNanL(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Attempt to generate the string value
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Fetch the arguments from the stack and perform the requested operation
    try
    {
        Var< SLongInt * >::push(vm, new SLongInt(std::nanl(val.mPtr)));
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.Message().c_str());
    }
    catch (const std::exception e)
    {
        return sq_throwerror(vm, e.what());
    }
    catch (...)
    {
        return sq_throwerror(vm, _SC("Failed to create a long integer instance"));
    }
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqExp(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::exp(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqExp2(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::exp2(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqExpm1(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::expm1(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqLog(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::log(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqLog10(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::log10(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqLog2(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::log2(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqLog1p(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::log1p(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqPow(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::pow(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqSqrt(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::sqrt(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqCbrt(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::cbrt(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqHypot(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::hypot(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqSin(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::sin(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqCos(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::cos(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqTan(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::tan(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqAsin(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::asin(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqAcos(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::acos(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqAtan(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::atan(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqAtan2(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::atan2(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqSinh(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::sinh(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqCosh(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::cosh(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqTanh(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::tanh(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqAsinh(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::asinh(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqAcosh(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::acosh(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqAtanh(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::atanh(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqErf(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::erf(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqErfc(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::erfc(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqTgamma(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::tgamma(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqLgamma(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::lgamma(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqCeil(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::ceil(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqFloor(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::floor(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqTrunc(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::trunc(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqRound(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::round(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqRoundI(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    if (sq_gettype(vm, 2) == OT_FLOAT)
    {
        sq_pushinteger(vm, ConvTo< SQInteger >::From(std::llround(PopStackFloat(vm, 2))));
    }
    else
    {
        sq_pushinteger(vm, ConvTo< SQInteger >::From(std::llround(PopStackInteger(vm, 2))));
    }
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqRoundL(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    try
    {
        if (sq_gettype(vm, 2) == OT_FLOAT)
        {
            Var< SLongInt * >::push(vm, new SLongInt(std::llround(PopStackFloat(vm, 2))));
        }
        else
        {
            Var< SLongInt * >::push(vm, new SLongInt(std::llround(PopStackInteger(vm, 2))));
        }
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.Message().c_str());
    }
    catch (const std::exception e)
    {
        return sq_throwerror(vm, e.what());
    }
    catch (...)
    {
        return sq_throwerror(vm, _SC("Failed to create a long integer instance"));
    }
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqNearByInt(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::nearbyint(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqFrexp(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Where the exponent is retrieved
    Int32 expv = 0;
    // Fetch the arguments from the stack and perform the requested operation
    const SQFloat sigv = std::frexp(PopStackFloat(vm, 2), &expv);
    // Create a new table on the stack
    sq_newtable(vm);
    // Push the significand index
    sq_pushstring(vm, _SC("sig"), -1);
    // Push the significand value
    sq_pushfloat(vm, sigv);
    // Attempt to create the array element
    if (SQ_FAILED(sq_rawset(vm, -3)))
    {
        return sq_throwerror(vm, "Unable to insert the significand element");
    }
    // Push the exponent index
    sq_pushstring(vm, _SC("exp"), -1);
    // Push the exponent value
    sq_pushinteger(vm, expv);
    // Attempt to create the array element
    if (SQ_FAILED(sq_rawset(vm, -3)))
    {
        return sq_throwerror(vm, "Unable to insert the exponent element");
    }
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqLdexp(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::ldexp(PopStackFloat(vm, 2), PopStackInteger(vm, 3)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqModF(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Where the fractional part is retrieved
    SQFloat intv = 0;
    // Fetch the arguments from the stack and perform the requested operation
    const SQFloat fracv = std::modf(PopStackFloat(vm, 2), &intv);
    // Create a new table on the stack
    sq_newtable(vm);
    // Push the integral index
    sq_pushstring(vm, _SC("integral"), -1);
    // Push the integral value
    sq_pushfloat(vm, intv);
    // Attempt to create the array element
    if (SQ_FAILED(sq_rawset(vm, -3)))
    {
        return sq_throwerror(vm, "Unable to insert the integral element");
    }
    // Push the fractional index
    sq_pushstring(vm, _SC("fractional"), -1);
    // Push the fractional value
    sq_pushfloat(vm, fracv);
    // Attempt to create the array element
    if (SQ_FAILED(sq_rawset(vm, -3)))
    {
        return sq_throwerror(vm, "Unable to insert the fractional element");
    }
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqScalbn(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
#ifdef _SQ64
    sq_pushfloat(vm, std::scalbln(PopStackFloat(vm, 2), PopStackInteger(vm, 3)));
#else
    sq_pushfloat(vm, std::scalbn(PopStackFloat(vm, 2), PopStackInteger(vm, 3)));
#endif // _SQ64
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqIlogb(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushinteger(vm, std::ilogb(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqLogb(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::logb(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqNextAfter(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::nextafter(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqNextForward(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::nexttoward(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqCopySign(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushfloat(vm, std::copysign(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqFpClassify(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushinteger(vm, std::fpclassify(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqIsFinite(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushbool(vm, std::isfinite(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqIsInf(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushbool(vm, std::isinf(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqIsNan(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushbool(vm, std::isnan(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqIsNormal(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushbool(vm, std::isnormal(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqSignBit(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushbool(vm, std::signbit(PopStackFloat(vm, 2)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqIsGreater(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushbool(vm, std::isgreater(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqIsGreaterEqual(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushbool(vm, std::isgreaterequal(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqIsLess(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushbool(vm, std::isless(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqIsLessEqual(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushbool(vm, std::islessequal(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqIsLessGreater(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushbool(vm, std::islessgreater(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqIsUnordered(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 2)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the arguments from the stack and perform the requested operation
    sq_pushbool(vm, std::isunordered(PopStackFloat(vm, 2), PopStackFloat(vm, 3)));
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqDigits1(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the integer value from the stack
    Int64 n = std::llabs(PopStackLong(vm, 2));
    // Start with 0 digits
    Uint8 d = 0;
    // Identify the number of digits
    while (n != 0)
    {
        n /= 10;
        ++d;
    }
    // Push the number of digits on the stack
    sq_pushinteger(vm, d);
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqDigits0(HSQUIRRELVM vm)
{
    // Do we have the correct number of arguments?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Wrong number of arguments");
    }
    // Fetch the integer value from the stack
    Int64 n = std::llabs(PopStackLong(vm, 2));
    // Start with 0 digits
    Uint8 d = 0;
    // Identify the number of digits
    do
    {
        n /= 10;
        ++d;
    } while (n != 0);
    // Push the number of digits on the stack
    sq_pushinteger(vm, d);
    // Specify that we have a value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQFloat SqIToF(Int64 sigv, Int64 expv, Int32 padn, Int32 padc, bool negf)
{
    // The number of characters to add before the exponent
    static CharT padb[64];
    // Is the number of pad characters out of range?
    if (padn >= 64)
    {
        STHROWF("Pad characters out of range: %d >= 64", padn);
    }
    // Is the pad character invalid?
    else if (padc > 9 || padc < 0)
    {
        STHROWF("Invalid padding value: %d", padc);
    }
    // Write the padding characters
    std::memset(padb, 48 + padc, padn);
    // Add the null terminator
    padb[padn] = '\0';
    // The obtained string containing the floating point
    CSStr fstr = nullptr;
    // Generate the floating point value
    if (negf)
    {
        fstr = ToStrF("-%lld.%s%lld", sigv, padb, expv);
    }
    else
    {
        fstr = ToStrF("%lld.%s%lld", sigv, padb, expv);
    }
    // Now transform the resulted string to a floating point value
#ifdef SQUSEDOUBLE
    return std::strtod(fstr, nullptr);
#else
    return std::strtof(fstr, nullptr);
#endif // SQUSEDOUBLE
}

// ================================================================================================
void Register_Math(HSQUIRRELVM vm)
{
    Table mns(vm);

    mns
    .SquirrelFunc(_SC("Div"), &SqDiv)
    .SquirrelFunc(_SC("Remainder"), &SqRemainder)
    .SquirrelFunc(_SC("Fma"), &SqFma)
    .SquirrelFunc(_SC("Max"), &SqMax)
    .SquirrelFunc(_SC("Min"), &SqMin)
    .SquirrelFunc(_SC("Dim"), &SqDim)
    .SquirrelFunc(_SC("Nan"), &SqNan)
    .SquirrelFunc(_SC("NanL"), &SqNanL)
    .SquirrelFunc(_SC("Exp"), &SqExp)
    .SquirrelFunc(_SC("Exp2"), &SqExp2)
    .SquirrelFunc(_SC("Expm1"), &SqExpm1)
    .SquirrelFunc(_SC("Log"), &SqLog)
    .SquirrelFunc(_SC("Log10"), &SqLog10)
    .SquirrelFunc(_SC("Log2"), &SqLog2)
    .SquirrelFunc(_SC("Log1p"), &SqLog1p)
    .SquirrelFunc(_SC("Pow"), &SqPow)
    .SquirrelFunc(_SC("Sqrt"), &SqSqrt)
    .SquirrelFunc(_SC("Cbrt"), &SqCbrt)
    .SquirrelFunc(_SC("Hypot"), &SqHypot)
    .SquirrelFunc(_SC("Sin"), &SqSin)
    .SquirrelFunc(_SC("Cos"), &SqCos)
    .SquirrelFunc(_SC("Tan"), &SqTan)
    .SquirrelFunc(_SC("Asin"), &SqAsin)
    .SquirrelFunc(_SC("Acos"), &SqAcos)
    .SquirrelFunc(_SC("Atan"), &SqAtan)
    .SquirrelFunc(_SC("Atan2"), &SqAtan2)
    .SquirrelFunc(_SC("Sinh"), &SqSinh)
    .SquirrelFunc(_SC("Cosh"), &SqCosh)
    .SquirrelFunc(_SC("Tanh"), &SqTanh)
    .SquirrelFunc(_SC("Asinh"), &SqAsinh)
    .SquirrelFunc(_SC("Acosh"), &SqAcosh)
    .SquirrelFunc(_SC("Atanh"), &SqAtanh)
    .SquirrelFunc(_SC("Erf"), &SqErf)
    .SquirrelFunc(_SC("Erfc"), &SqErfc)
    .SquirrelFunc(_SC("Tgamma"), &SqTgamma)
    .SquirrelFunc(_SC("Lgamma"), &SqLgamma)
    .SquirrelFunc(_SC("Ceil"), &SqCeil)
    .SquirrelFunc(_SC("Floor"), &SqFloor)
    .SquirrelFunc(_SC("Trunc"), &SqTrunc)
    .SquirrelFunc(_SC("Round"), &SqRound)
    .SquirrelFunc(_SC("RoundI"), &SqRoundI)
    .SquirrelFunc(_SC("RoundL"), &SqRoundL)
    .SquirrelFunc(_SC("NearByInt"), &SqNearByInt)
    .SquirrelFunc(_SC("Frexp"), &SqFrexp)
    .SquirrelFunc(_SC("Ldexp"), &SqLdexp)
    .SquirrelFunc(_SC("ModF"), &SqModF)
    .SquirrelFunc(_SC("Scalbn"), &SqScalbn)
    .SquirrelFunc(_SC("Ilogb"), &SqIlogb)
    .SquirrelFunc(_SC("Logb"), &SqLogb)
    .SquirrelFunc(_SC("NextAfter"), &SqNextAfter)
    .SquirrelFunc(_SC("NextForward"), &SqNextForward)
    .SquirrelFunc(_SC("CopySign"), &SqCopySign)
    .SquirrelFunc(_SC("FpClassify"), &SqFpClassify)
    .SquirrelFunc(_SC("IsFinite"), &SqIsFinite)
    .SquirrelFunc(_SC("IsInf"), &SqIsInf)
    .SquirrelFunc(_SC("IsNan"), &SqIsNan)
    .SquirrelFunc(_SC("IsNormal"), &SqIsNormal)
    .SquirrelFunc(_SC("SignBit"), &SqSignBit)
    .SquirrelFunc(_SC("IsGreater"), &SqIsGreater)
    .SquirrelFunc(_SC("IsGreaterEqual"), &SqIsGreaterEqual)
    .SquirrelFunc(_SC("IsLess"), &SqIsLess)
    .SquirrelFunc(_SC("IsLessEqual"), &SqIsLessEqual)
    .SquirrelFunc(_SC("IsLessGreater"), &SqIsLessGreater)
    .SquirrelFunc(_SC("IsUnordered"), &SqIsUnordered)
    .SquirrelFunc(_SC("Digits1"), &SqDigits1)
    .SquirrelFunc(_SC("Digits0"), &SqDigits0);

    RootTable(vm).Bind(_SC("SqMath"), mns);

    RootTable(vm).Func(_SC("IToF"), &SqIToF);
}


} // Namespace:: SqMod
