// ------------------------------------------------------------------------------------------------
#include "Library/Utils/Vector.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqVectorInteger, _SC("SqVectorInteger"))
SQMOD_DECL_TYPENAME(SqVectorString, _SC("SqVectorString"))
SQMOD_DECL_TYPENAME(SqVectorFloat, _SC("SqVectorFloat"))
SQMOD_DECL_TYPENAME(SqVectorByte, _SC("SqVectorByte"))
SQMOD_DECL_TYPENAME(SqVectorBool, _SC("SqVectorBool"))

// ------------------------------------------------------------------------------------------------
template < class T, class U >
static void Register_Vector(HSQUIRRELVM vm, Table & ns, const SQChar * name)
{
	using Vector = SqVector< T >;
    // --------------------------------------------------------------------------------------------
    Class< Vector, NoCopy< Vector > > cls(vm, U::Str);
    // Constructors
    cls.Ctor()
    .template Ctor< SQInteger >()
    .template Ctor< SQInteger, typename Vector::OptimalArg >()
    // Meta-methods
    .SquirrelFunc(_SC("_typename"), &U::Fn)
    // Properties
    .Prop(_SC("Null"), &Vector::IsNull)
    .Prop(_SC("Front"), &Vector::Front)
    .Prop(_SC("Back"), &Vector::Back)
    .Prop(_SC("Empty"), &Vector::Empty)
    .Prop(_SC("Size"), &Vector::Size, &Vector::Resize)
    .Prop(_SC("Capacity"), &Vector::Capacity, &Vector::Reserve)
    // Member Methods
    .Func(_SC("Get"), &Vector::Get)
    .Func(_SC("Set"), &Vector::Set)
    .Func(_SC("Resize"), &Vector::ResizeEx)
    .Func(_SC("Reserve"), &Vector::Reserve)
    .Func(_SC("Compact"), &Vector::Compact)
    .Func(_SC("Clear"), &Vector::Clear)
    .Func(_SC("Push"), &Vector::Push)
    .Func(_SC("Pop"), &Vector::Pop)
    .Func(_SC("EraseAt"), &Vector::EraseAt)
    .Func(_SC("EraseFrom"), &Vector::EraseFrom)
    .Func(_SC("EraseValue"), &Vector::EraseValue)
    .Func(_SC("InsertAt"), &Vector::InsertAt)
    .Func(_SC("Insert"), &Vector::Insert)
    .Func(_SC("Locate"), &Vector::Locate)
    .Func(_SC("LocateFrom"), &Vector::LocateFrom)
    .Func(_SC("Count"), &Vector::Count)
    .Func(_SC("Equal"), &Vector::Equal)
    .Func(_SC("Each"), &Vector::Each)
    .Func(_SC("EachRange"), &Vector::EachRange)
    .Func(_SC("While"), &Vector::While)
    .Func(_SC("WhileRange"), &Vector::WhileRange)
    .Func(_SC("AsArray"), &Vector::AsArray)
    .Func(_SC("Reverse"), &Vector::Reverse)
    .Func(_SC("Generate"), &Vector::Generate)
    .Func(_SC("GenerateSome"), &Vector::GenerateSome)
    .Func(_SC("GenerateFrom"), &Vector::GenerateFrom);
    // --------------------------------------------------------------------------------------------
    ns.Bind(name, cls);
    // Bind it to the root table with a `Sq` prefix
    RootTable(vm).Bind(fmt::format("Sq{}", name).c_str(), cls);
}

// ================================================================================================
void Register_Vector(HSQUIRRELVM vm, Table & ns)
{
    Register_Vector< SQInteger, SqVectorInteger >(vm, ns, _SC("IntVec"));
    Register_Vector< String, SqVectorString >(vm, ns, _SC("StrVec"));
    Register_Vector< SQFloat, SqVectorFloat >(vm, ns, _SC("FloatVec"));
    Register_Vector< uint8_t, SqVectorByte >(vm, ns, _SC("ByteVec"));
    Register_Vector< bool, SqVectorBool >(vm, ns, _SC("BoolVec"));
}

} // Namespace:: SqMod
