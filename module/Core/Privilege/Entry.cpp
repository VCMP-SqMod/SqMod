// ------------------------------------------------------------------------------------------------
#include "Core/Privilege/Entry.hpp"
#include "Core/Privilege.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(EntryTn, _SC("SqPrivilegeEntry"))

// ------------------------------------------------------------------------------------------------
void PvEntry::SetTag(StackStrF & tag)
{
	mTag = std::move(tag);
    // Hash the name and cache it (the name hash is cached in .mRes member variable)
    mTag.CacheHash();
    // Propagate this change to the manager as well
    if (mManager)
    {
        mManager->UpdateEntryHash(mID, static_cast< size_t >(mTag.mRes));
    }
}

// ------------------------------------------------------------------------------------------------
void PvEntry::Release()
{
    mTag.Release();
    mOnQuery.Release();
    mOnModify.Release();
    mOnGained.Release();
    mOnLost.Release();
    mData.Release();
    mBrief.Release();
    mInfo.Release();
}

// ================================================================================================
void Register_Privilege_Entry(HSQUIRRELVM vm, Table & ns)
{
    ns.Bind(_SC("Entry"),
        Class< SqPvEntry, NoConstructor< SqPvEntry > >(vm, EntryTn::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &EntryTn::Fn)
        .Func(_SC("_tostring"), &SqPvEntry::ToString)
        // Core Properties
        .Prop(_SC("ID"), &SqPvEntry::GetID)
        .Prop(_SC("Tag"), &SqPvEntry::GetTag, &SqPvEntry::SetTag)
        .Prop(_SC("Data"), &SqPvEntry::GetData, &SqPvEntry::SetData)
        .Prop(_SC("Manager"), &SqPvEntry::GetManager)
        .Prop(_SC("Brief"), &SqPvEntry::GetBrief, &SqPvEntry::SetBrief)
        .Prop(_SC("Info"), &SqPvEntry::GetInfo, &SqPvEntry::SetInfo)
        .Prop(_SC("Default"), &SqPvEntry::GetDefault, &SqPvEntry::SetDefault)
        // Core Methods
        .FmtFunc(_SC("SetTag"), &SqPvEntry::ApplyTag)
        .CbFunc(_SC("OnQuery"), &SqPvEntry::SetOnQuery)
        .CbFunc(_SC("OnModify"), &SqPvEntry::SetOnModify)
        .CbFunc(_SC("OnLost"), &SqPvEntry::SetOnLost)
        .CbFunc(_SC("OnGained"), &SqPvEntry::SetOnGained)
        // Member Methods
        .FmtFunc(_SC("SetBrief"), &SqPvEntry::ApplyBrief)
        .FmtFunc(_SC("SetInfo"), &SqPvEntry::ApplyInfo)
        // Raw functions
    );
}

} // Namespace:: SqMod
