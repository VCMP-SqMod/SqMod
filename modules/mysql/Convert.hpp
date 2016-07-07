#ifndef _SQMYSQL_CONVERT_HPP_
#define _SQMYSQL_CONVERT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <mysql.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 *
*/
struct DbDecimal;
struct DbDate;
struct DbTime;
struct DbDatetime;

/* ------------------------------------------------------------------------------------------------
 * Retrieve the lowercase name of a MySQL data-type.
*/
CSStr SqMySQLTypename(enum_field_types type);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the capitalized name of a MySQL data-type.
*/
CSStr SqMySQLTypenameC(enum_field_types type);

/* ------------------------------------------------------------------------------------------------
 * Utility used to convert from database types to known types.
*/
template < typename T > struct DbConvTo;

/* ------------------------------------------------------------------------------------------------
 * Specialization for signed 8 bit integer.
*/
template<> struct DbConvTo< Int8 >
{
    static Int8 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Int8"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for unsigned 8 bit integer.
*/
template<> struct DbConvTo< Uint8 >
{
    static Uint8 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Uint8"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for signed 16 bit integer.
*/
template<> struct DbConvTo< Int16 >
{
    static Int16 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Int16"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for unsigned 16 bit integer.
*/
template<> struct DbConvTo< Uint16 >
{
    static Uint16 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Uint16"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for signed 32 bit integer.
*/
template<> struct DbConvTo< Int32 >
{
    static Int32 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Int32"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for unsigned 32 bit integer.
*/
template<> struct DbConvTo< Uint32 >
{
    static Uint32 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Uint32"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for signed 64 bit integer.
*/
template<> struct DbConvTo< Int64 >
{
    static Int64 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Int64"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for unsigned 64 bit integer.
*/
template<> struct DbConvTo< Uint64 >
{
    static Uint64 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Uint64"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for 32 floating point.
*/
template<> struct DbConvTo< Float32 >
{
    static Float32 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Float32"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for 64 floating point.
*/
template<> struct DbConvTo< Float64 >
{
    static Float64 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Float64"));
};

} // Namespace:: SqMod

#endif // _SQMYSQL_CONVERT_HPP_
