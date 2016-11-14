// ------------------------------------------------------------------------------------------------
#include "Description.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Description::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMDescription");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Description::Validate(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid Maxmind database reference =>[%s:%d]", file, line);
    }
}
#else
void Description::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid Maxmind database reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
Description::Pointer Description::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    // Validate the referenced description
    if (!m_Description)
    {
        SqThrowF("Invalid Maxmind meta-data description reference =>[%s:%d]", file, line);
    }
    // Return the description pointer
    return m_Description;
}
#else
Description::Pointer Description::GetValid() const
{
    Validate();
    // Validate the referenced description
    if (!m_Description)
    {
        SqThrowF("Invalid Maxmind meta-data description reference");
    }
    // Return the description pointer
    return m_Description;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------



// ================================================================================================
void Register_Description(Table & mmns)
{
    mmns.Bind(_SC("Description"),
        Class< Description >(mmns.GetVM(), _SC("SqMMDescription"))
        // Constructors
        .Ctor()
        .Ctor< const Description & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Description::Typename)
        .Func(_SC("_tostring"), &Description::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Description::IsValid)
        .Prop(_SC("Value"), &Description::GetDescriptionValue)
        .Prop(_SC("Language"), &Description::GetDescriptionLanguage)
    );
}

} // Namespace:: SqMod
