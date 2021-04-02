// ------------------------------------------------------------------------------------------------
#include "Library/Utils/String.hpp"
#include "Library/Format.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("SqString"))

// ------------------------------------------------------------------------------------------------
static SQInteger SqStringFormat(HSQUIRRELVM vm)
{
    FormatContext ctx;
    // Attempt to generate the formatted string
    if (SQ_FAILED(ctx.Proc(vm, 2, 3, sq_gettop(vm))))
    {
        return ctx.mRes;
    }
    // Create the instance and guard it to make sure it gets deleted in case of exceptions
    DeleteGuard< SqString > instance(new SqString(std::move(ctx.mOut)));
    // Push the instance on the stack
    ClassType< SqString >::PushInstance(vm, instance);
    // Stop guarding the instance
    instance.Release();
    // Specify that we returned a value
    return 1;
}

// ================================================================================================
void Register_Native_String(HSQUIRRELVM vm, Table & ns)
{
    RootTable(vm).Bind(Typename::Str,
        Class< SqString >(vm, Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &SqString::GetString)
        // Properties
        .Prop(_SC("Str"), &SqString::GetString, &SqString::SetString)
        .Prop(_SC("Front"), &SqString::Front)
        .Prop(_SC("Back"), &SqString::Back)
        .Prop(_SC("Empty"), &SqString::Empty)
        .Prop(_SC("Size"), &SqString::Size, &SqString::Resize)
        .Prop(_SC("Capacity"), &SqString::Capacity, &SqString::Reserve)
        .Prop(_SC("Sorted"), &SqString::IsSorted)
        // Member Methods
        .Func(_SC("Get"), &SqString::Get)
        .Func(_SC("Set"), &SqString::Set)
        .Func(_SC("Resize"), &SqString::ResizeEx)
        .Func(_SC("Reserve"), &SqString::Reserve)
        .Func(_SC("Compact"), &SqString::Compact)
        .Func(_SC("Clear"), &SqString::Clear)
        .Func(_SC("Push"), &SqString::Push)
        .Func(_SC("Append"), &SqString::Append)
        .Func(_SC("Extend"), &SqString::Extend)
        .Func(_SC("Pop"), &SqString::Pop)
        .Func(_SC("EraseAt"), &SqString::EraseAt)
        .Func(_SC("EraseFrom"), &SqString::EraseFrom)
        .Func(_SC("EraseValue"), &SqString::EraseValue)
        .Func(_SC("InsertAt"), &SqString::InsertAt)
        .Func(_SC("Insert"), &SqString::Insert)
        .Func(_SC("Locate"), &SqString::Locate)
        .Func(_SC("LocateFrom"), &SqString::LocateFrom)
        .Func(_SC("Find"), &SqString::Find)
        .Func(_SC("FindFrom"), &SqString::FindFrom)
        .Func(_SC("Count"), &SqString::Count)
        .Func(_SC("Equal"), &SqString::Equal)
        .Func(_SC("Slice"), &SqString::Slice)
        .Func(_SC("Each"), &SqString::Each)
        .Func(_SC("EachRange"), &SqString::EachRange)
        .Func(_SC("While"), &SqString::While)
        .Func(_SC("WhileRange"), &SqString::WhileRange)
        .Func(_SC("Reverse"), &SqString::Reverse)
        .Func(_SC("Generate"), &SqString::Generate)
        .Func(_SC("GenerateSome"), &SqString::GenerateSome)
        .Func(_SC("GenerateFrom"), &SqString::GenerateFrom)
        .Func(_SC("GenerateBetween"), &SqString::GenerateBetween)
        .Func(_SC("Sort"), &SqString::Sort)
        .Func(_SC("Shuffle"), &SqString::Shuffle)
        .Func(_SC("Assign"), &SqString::SetString)
    );
    // --------------------------------------------------------------------------------------------
    RootTable(vm).SquirrelFunc(_SC("SqStringF"), SqStringFormat);
}

} // Namespace:: SqMod
