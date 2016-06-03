// ------------------------------------------------------------------------------------------------
#include "Connection.hpp"
#include "Manager.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Connection::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMgConnection");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Int32 Connection::Cmp(const Connection & o) const
{
    if (m_Handle == o.m_Handle)
    {
        return 0;
    }
    else if (m_Handle.HndPtr() > o.m_Handle.HndPtr())
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
void Connection::Bind(Int32 ev, Object & env, Function & func) const
{
    
}

// ------------------------------------------------------------------------------------------------


} // Namespace:: SqMod
