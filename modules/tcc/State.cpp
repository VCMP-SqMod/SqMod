// ------------------------------------------------------------------------------------------------
#include "State.hpp"
#include "Symbol.hpp"

// ------------------------------------------------------------------------------------------------
#include <libtcc.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger State::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqTccState");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
void State::AddSource(CSStr filename) const
{
    m_Handle.AddFile(filename, TCC_FILETYPE_C);
}

// ------------------------------------------------------------------------------------------------
void State::AddBinary(CSStr filename) const
{
    m_Handle.AddFile(filename, TCC_FILETYPE_BINARY);
}

// ------------------------------------------------------------------------------------------------
void State::AddAsm(CSStr filename) const
{
    m_Handle.AddFile(filename, TCC_FILETYPE_ASM);
}

// ------------------------------------------------------------------------------------------------
void State::AddAsmPP(CSStr filename) const
{
    m_Handle.AddFile(filename, TCC_FILETYPE_ASM_PP);
}

// ------------------------------------------------------------------------------------------------
Symbol State::GetSymbol(CSStr name) const
{
    // Is the specified name even valid?
    if (!name || *name == '\0')
    {
        STHROWF("Invalid or empty symbol name: null");
    }
    // Return the requested symbol
    return Symbol(m_Handle, name, m_Handle.GetSymbol< Symbol::SymPtr >(name));
}

} // Namespace:: SqMod
