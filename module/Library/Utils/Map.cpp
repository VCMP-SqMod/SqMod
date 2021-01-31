// ------------------------------------------------------------------------------------------------
#include "Library/Utils/Map.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqMapInteger, _SC("SqMapInteger"))
SQMOD_DECL_TYPENAME(SqMapString, _SC("SqMapString"))

// ------------------------------------------------------------------------------------------------
template < class T, class U >
static void Register_Map(HSQUIRRELVM vm, Table & ns, const SQChar * name)
{
	using Container = SqMap< T >;
    // --------------------------------------------------------------------------------------------
    ns.Bind(name,
        Class< Container, NoCopy< Container > >(vm, U::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &U::Fn)
        // Properties
        .Prop(_SC("Null"), &Container::IsNull)
        .Prop(_SC("Empty"), &Container::Empty)
        .Prop(_SC("Size"), &Container::Size)
        // Member Methods
        .Func(_SC("Get"), &Container::Get)
        .Func(_SC("Set"), &Container::Set)
        .Func(_SC("Clear"), &Container::Clear)
        .Func(_SC("Erase"), &Container::Erase)
    );
}

// ================================================================================================
void Register_Map(HSQUIRRELVM vm, Table & ns)
{
    Register_Map< SQInteger, SqMapInteger >(vm, ns, _SC("IntMap"));
    Register_Map< String, SqMapString >(vm, ns, _SC("StrMap"));
}

} // Namespace:: SqMod
