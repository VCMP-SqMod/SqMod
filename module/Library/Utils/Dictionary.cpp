// ------------------------------------------------------------------------------------------------
#include "Library/Utils/Dictionary.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDictionaryTn, _SC("SqDictionary"))

// ================================================================================================
void Register_Dictionary(HSQUIRRELVM vm, Table & ns)
{
    ns.Bind(_SC("Dictionary"),
        Class< SqDictionary >(vm, SqDictionaryTn::Str)
        // Constructors
        .Ctor()
        .Ctor< SQInteger >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDictionaryTn::Fn)
        // Properties
        .Prop(_SC("Empty"), &SqDictionary::Empty)
        .Prop(_SC("Size"), &SqDictionary::Size)
        // Member Methods
        .Func(_SC("Get"), &SqDictionary::Get)
        .Func(_SC("Set"), &SqDictionary::Set)
        .Func(_SC("Clear"), &SqDictionary::Clear)
        .Func(_SC("Erase"), &SqDictionary::Erase)
        .Func(_SC("Exists"), &SqDictionary::Exists)
        .CbFunc(_SC("Each"), &SqDictionary::Each)
        .CbFunc(_SC("EachWith"), &SqDictionary::EachWith)
        .CbFunc(_SC("While"), &SqDictionary::While)
        .CbFunc(_SC("WhileWith"), &SqDictionary::WhileWith)
    );
}

} // Namespace:: SqMod
