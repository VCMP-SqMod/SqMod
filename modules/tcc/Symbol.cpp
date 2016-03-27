// ------------------------------------------------------------------------------------------------
#include "Symbol.hpp"
#include "State.hpp"

// ------------------------------------------------------------------------------------------------
#include <libtcc.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Symbol::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqTccSymbol");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Symbol::Symbol(const State & state, CSStr name)
    : Symbol(state.GetSymbol(name))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Symbol::Symbol(const StateHnd & state, CSStr name, SymPtr sym)
    : m_Handle(sym), m_State(state), m_Name(name ? name : _SC(""))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
State Symbol::GetStateRef() const
{
    return State(m_State);
}

// ------------------------------------------------------------------------------------------------
SQInteger Symbol::Forward(HSQUIRRELVM vm)
{
    // The symbol instance
    Symbol * symbol = nullptr;
    // Attempt to retrieve the symbol instance
    try
    {
        symbol = Var< Symbol * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.Message().c_str());
    }
    // Do we have a valid symbol instance?
    if (!symbol)
    {
        return sq_throwerror(vm, "Invalid symbol instance");
    }
    // Do we have a valid symbol identifier?
    else if (!symbol->IsValid())
    {
        return sq_throwerror(vm, "Invalid symbol reference");
    }
    // Cast the symbol to a squirrel function, forward the call and return the result
    return reinterpret_cast< SQFUNCTION >((*symbol).m_Handle)(vm);
}

} // Namespace:: SqMod
