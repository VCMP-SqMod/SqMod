#include "Library/Format.hpp"
#include "Register.hpp"
#include "Logger.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include <format.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static const SQChar * GetTypeName(SQObjectType type_id)
{
	switch (type_id)
	{
		case OT_INTEGER:
			return _SC("integer");
		case OT_FLOAT:
			return _SC("float");
		case OT_STRING:
			return _SC("string");
		case OT_BOOL:
			return _SC("boolean");
		case OT_USERPOINTER:
			return _SC("user pointer");
		case OT_NULL:
			return _SC("null");
		case OT_TABLE:
			return _SC("table");
		case OT_ARRAY:
			return _SC("array");
		case OT_CLOSURE:
			return _SC("closure");
		case OT_NATIVECLOSURE:
			return _SC("native closure");
		case OT_GENERATOR:
			return _SC("generator");
		case OT_USERDATA:
			return _SC("user data");
		case OT_THREAD:
			return _SC("thread");
		case OT_CLASS:
			return _SC("class");
		case OT_INSTANCE:
			return _SC("instance");
		case OT_WEAKREF:
			return _SC("weak reference");
		default:
			return _SC("unknown");
	}
}

// ------------------------------------------------------------------------------------------------
String GetFormatStr(HSQUIRRELVM vm, SQInteger arg, SQInteger args)
{
	if (sq_gettype(vm, arg) == OT_STRING)
    {
		const SQChar * str = 0;
		sq_getstring(vm, arg, &str);
		return GetFormatStr(vm, (str == NULL ? "" : str), ++arg, args);
	}
    else
    {
		LogErr("Expecting format string but got: %s", GetTypeName(sq_gettype(vm, arg)));
	}
	return String();
}

// ------------------------------------------------------------------------------------------------
String GetFormatStr(HSQUIRRELVM vm, const String & fstr,  SQInteger arg, SQInteger args)
{
	using namespace fmt::internal;

	String str;

	if (args-arg > 15)
    {
		LogErr("Too many arguments to format");
		return str;
	}
    else if (fstr.empty())
    {
        return str;
    }
	else if (arg == args)
    {
		// Unnecessary but should throw an error when trying to format something without arguments
		try
        {
			str = fmt::format(fstr);
		}
        catch (const fmt::SystemError & e)
        {
			LogErr(e.what());
		}
		return str;
	}

	const SQChar * sval = 0;
	SQInteger ival;
	SQFloat fval;
	SQUserPointer pval;

	std::vector< MakeValue< SQChar > > vlist;
	vlist.reserve(args-arg);
	std::uint64_t vtype = 0x0;

	for (unsigned sh = 0;arg <= args; arg++, sh += 4)
	{
		switch(sq_gettype(vm, arg))
		{
			case OT_INTEGER:
				sq_getinteger(vm, arg, &ival);
				vlist.emplace_back(ival);
				vtype |= (MakeValue< SQChar >::type(ival) << sh);
			break;
			case OT_FLOAT:
				sq_getfloat(vm, arg, &fval);
				vlist.emplace_back(fval);
				vtype |= (MakeValue< SQChar >::type(fval) << sh);
			break;
			case OT_STRING:
				sq_getstring(vm, arg, &sval);
				vlist.emplace_back(sval);
				vtype |= (MakeValue< SQChar >::type(sval) << sh);
			break;
			case OT_BOOL:
				sq_getinteger(vm, arg, &ival);
				vlist.emplace_back(static_cast<unsigned char>(ival));
				vtype |= (MakeValue< SQChar >::type(static_cast<unsigned char>(ival)) << sh);
			break;
			case OT_USERPOINTER:
				sq_getuserpointer(vm, arg, &pval);
				vlist.emplace_back(static_cast<void *>(pval));
				vtype |= (MakeValue< SQChar >::type(static_cast<void *>(pval)) << sh);
			break;
			default:
				LogErr("Cannot format (%s) data type", GetTypeName(sq_gettype(vm, arg)));
				return str;
		}
	}

	vlist.shrink_to_fit();

	try
    {
		fmt::BasicMemoryWriter<SQChar> w;
		w.write(fstr, fmt::ArgList(vtype, vlist.data()));
		str = w.c_str();
	}
    catch (const fmt::SystemError & e)
    {
		LogErr("%s", e.what());
	}

	return str;
}

// ------------------------------------------------------------------------------------------------
bool Register_Format(HSQUIRRELVM vm)
{
    SQMOD_UNUSED_VAR(vm);
    return true;
}

} // Namespace:: SqMod
