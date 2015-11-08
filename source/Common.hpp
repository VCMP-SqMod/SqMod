#ifndef _COMMON_HPP_
#define _COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include <vcmp.h>
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
#include <memory>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
using namespace Sqrat;

/* ------------------------------------------------------------------------------------------------
 * ...
*/
typedef ::Sqrat::string SqTag;
typedef ::Sqrat::Object SqObj;

// ------------------------------------------------------------------------------------------------
extern PluginFuncs*         _Func;
extern PluginCallbacks*     _Clbk;
extern PluginInfo*          _Info;

/* ------------------------------------------------------------------------------------------------
 * ...
*/
SqObj & NullData();

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Array & NullArray();

/* ------------------------------------------------------------------------------------------------
 * Utility used to transform values into script objects on the default VM
*/
template < typename T > SqObj MakeSqObj(const T & v)
{
    // Push the specified value on the stack
    Sqrat::PushVar< T >(Sqrat::DefaultVM::Get(), v);
    // Get the object off the stack
    Sqrat::Var< SqObj > var(Sqrat::DefaultVM::Get(), -1);
    // Pop the object from the stack
    sq_pop(Sqrat::DefaultVM::Get(), 1);
    // Return the object
    return var.value;
}

/* ------------------------------------------------------------------------------------------------
 * Utility used to transform values into script objects
*/
template < typename T > SqObj MakeSqObj(HSQUIRRELVM vm, const T & v)
{
    // Push the specified value on the stack
    Sqrat::PushVar< T >(vm, v);
    // Get the object off the stack
    Sqrat::Var< SqObj > var(vm, -1);
    // Pop the object from the stack
    sq_pop(vm, 1);
    // Return the object
    return var.value;
}

} // Namespace:: SqMod

#endif // _COMMON_HPP_
