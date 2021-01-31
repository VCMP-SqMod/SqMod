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
	using Container = SqVector< T >;
    // --------------------------------------------------------------------------------------------
    ns.Bind(name,
        Class< Container, NoCopy< Container > >(vm, U::Str)
        // Constructors
        .Ctor()
        .template Ctor< SQInteger >()
        .template Ctor< SQInteger, typename Container::OptimalArg >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &U::Fn)
        // Properties
        .Prop(_SC("Null"), &Container::IsNull)
        .Prop(_SC("Front"), &Container::Front)
        .Prop(_SC("Back"), &Container::Back)
        .Prop(_SC("Empty"), &Container::Empty)
        .Prop(_SC("Size"), &Container::Size, &Container::Resize)
        .Prop(_SC("Capacity"), &Container::Capacity, &Container::Reserve)
        .Prop(_SC("Sorted"), &Container::IsSorted)
        // Member Methods
        .Func(_SC("Get"), &Container::Get)
        .Func(_SC("Set"), &Container::Set)
        .Func(_SC("Resize"), &Container::ResizeEx)
        .Func(_SC("Reserve"), &Container::Reserve)
        .Func(_SC("Compact"), &Container::Compact)
        .Func(_SC("Clear"), &Container::Clear)
        .Func(_SC("Push"), &Container::Push)
        .Func(_SC("Append"), &Container::Push)
        .Func(_SC("Extend"), &Container::Extend)
        .Func(_SC("Pop"), &Container::Pop)
        .Func(_SC("EraseAt"), &Container::EraseAt)
        .Func(_SC("EraseFrom"), &Container::EraseFrom)
        .Func(_SC("EraseValue"), &Container::EraseValue)
        .Func(_SC("InsertAt"), &Container::InsertAt)
        .Func(_SC("Insert"), &Container::Insert)
        .Func(_SC("Find"), &Container::Locate)
        .Func(_SC("FindFrom"), &Container::LocateFrom)
        .Func(_SC("Count"), &Container::Count)
        .Func(_SC("Equal"), &Container::Equal)
        .Func(_SC("Slice"), &Container::Slice)
        .Func(_SC("Each"), &Container::Each)
        .Func(_SC("EachRange"), &Container::EachRange)
        .Func(_SC("While"), &Container::While)
        .Func(_SC("WhileRange"), &Container::WhileRange)
        .Func(_SC("AsArray"), &Container::AsArray)
        .Func(_SC("Reverse"), &Container::Reverse)
        .Func(_SC("Generate"), &Container::Generate)
        .Func(_SC("GenerateSome"), &Container::GenerateSome)
        .Func(_SC("GenerateFrom"), &Container::GenerateFrom)
        .Func(_SC("Sort"), &Container::Sort)
        .Func(_SC("Shuffle"), &Container::Shuffle)
    );
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
