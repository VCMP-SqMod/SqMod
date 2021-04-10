// ------------------------------------------------------------------------------------------------
#include "Library/Utils/String.hpp"
#include "Library/Format.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("SqString"))

// ------------------------------------------------------------------------------------------------
SQInteger SqString::GetLevenshtein(SqString & o) const
{
    if(mS.empty()) return o.mS.size();
    if(o.mS.empty()) return mS.size();

    std::vector< size_t > costs(o.mS.size() + 1);
    std::iota(costs.begin(), costs.end(), 0);
    size_t i = 0;

    for (const auto & c1 : mS)
    {
        costs[0] = i + 1;
        size_t corner = i;
        size_t j = 0;
        for (const auto & c2 : o.mS)
        {
            size_t upper = costs[j + 1];
            costs[j + 1] = (c1 == c2) ? corner : 1 + std::min(std::min(upper, corner), costs[j]);
            corner = upper;
            ++j;
        }
        ++i;
    }

    return costs[o.mS.size()];
}

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
        .Prop(_SC("Trimmed"), &SqString::Trimmed)
        .Prop(_SC("Lower"), &SqString::GetLower)
        .Prop(_SC("Upper"), &SqString::GetUpper)
        .Prop(_SC("Fnv1a32"), &SqString::GetFnv1a32)
        .Prop(_SC("Fnv1a64"), &SqString::GetFnv1a64)
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
        .Func(_SC("TrimLeft"), &SqString::TrimLeft)
        .Func(_SC("TrimRight"), &SqString::TrimRight)
        .Func(_SC("Trim"), &SqString::Trim)
        .Func(_SC("ToLower"), &SqString::ToLower)
        .Func(_SC("ToUpper"), &SqString::ToUpper)
        .Func(_SC("SwapCase"), &SqString::SwapCase)
        .Func(_SC("Compare"), &SqString::Compare)
        .Func(_SC("CompareI"), &SqString::CompareI)
        .FmtFunc(_SC("Contains"), &SqString::Contains)
        .FmtFunc(_SC("StartsWith"), &SqString::StartsWith)
        .FmtFunc(_SC("EndsWith"), &SqString::EndsWith)
        .FmtFunc(_SC("Find"), &SqString::Find)
        .FmtFunc(_SC("FindFrom"), &SqString::FindFrom)
        .FmtFunc(_SC("RFind"), &SqString::RFind)
        .FmtFunc(_SC("RFindFrom"), &SqString::RFindFrom)
        .FmtFunc(_SC("FindFirstOf"), &SqString::FindFirstOf)
        .FmtFunc(_SC("FindFirstOfFrom"), &SqString::FindFirstOfFrom)
        .FmtFunc(_SC("FindFirstNotOf"), &SqString::FindFirstNotOf)
        .FmtFunc(_SC("FindFirstNotOfFrom"), &SqString::FindFirstNotOfFrom)
        .FmtFunc(_SC("FindLastOf"), &SqString::FindLastOf)
        .FmtFunc(_SC("FindLastOfFrom"), &SqString::FindLastOfFrom)
        .FmtFunc(_SC("FindLastNotOf"), &SqString::FindLastNotOf)
        .FmtFunc(_SC("FindLastNotOfFrom"), &SqString::FindLastNotOfFrom)
        .FmtFunc(_SC("Levenshtein"), &SqString::GetLevenshtein)
        .FmtFunc(_SC("Remove"), &SqString::Remove)
        .FmtFunc(_SC("Eliminate"), &SqString::Eliminate)
        // Member Overloads
        .Overload(_SC("Locate"), &SqString::Locate)
        .Overload(_SC("Locate"), &SqString::LocateFrom)
        .Overload(_SC("RLocate"), &SqString::RLocate)
        .Overload(_SC("RLocate"), &SqString::RLocateFrom)
        .Overload(_SC("LocateFirstOf"), &SqString::LocateFirstOf)
        .Overload(_SC("LocateFirstOf"), &SqString::LocateFirstOfFrom)
        .Overload(_SC("LocateFirstNotOf"), &SqString::LocateFirstNotOf)
        .Overload(_SC("LocateFirstNotOf"), &SqString::LocateFirstNotOfFrom)
        .Overload(_SC("LocateLastOf"), &SqString::LocateLastOf)
        .Overload(_SC("LocateLastOf"), &SqString::LocateLastOfFrom)
        .Overload(_SC("LocateLastNotOf"), &SqString::LocateLastNotOf)
        .Overload(_SC("LocateLastNotOf"), &SqString::LocateLastNotOfFrom)
        .Overload(_SC("Repeat"), &SqString::Repeat)
        .Overload(_SC("Repeat"), &SqString::Repeat_)
        .Overload(_SC("Replace"), &SqString::Replace)
        .Overload(_SC("Replace"), &SqString::Replace_)
        .Overload(_SC("Change"), &SqString::Change)
        .Overload(_SC("Change"), &SqString::Change_)
    );
    // --------------------------------------------------------------------------------------------
    RootTable(vm).SquirrelFunc(_SC("SqStringF"), SqStringFormat);
}

} // Namespace:: SqMod
