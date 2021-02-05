// ------------------------------------------------------------------------------------------------
#include "Core/Privilege/Unit.hpp"
#include "Core/Privilege/Entry.hpp"
#include "Core/Privilege/Class.hpp"
#include "Core/Privilege.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(UnitTn, _SC("SqPrivilegeUnit"))

// ------------------------------------------------------------------------------------------------
void PvUnit::SetTag(StackStrF & tag)
{
    mTag = std::move(tag);
    // Hash the name and cache it (the name hash is cached in .mRes member variable)
    mTag.CacheHash();
    // Do we still belong to a class? (should always be. but always check)
    if (!mClass.expired())
    {
        auto p = mClass.lock();
        // Propagate this change to the class as well
        p->UpdateUnitHash(mID, static_cast< size_t >(mTag.mRes));
        // Propagate this change to the manager as well
        if (p->mManager)
        {
            p->mManager->UpdateUnitHash(mID, static_cast< size_t >(mTag.mRes));
        }
    }
}

// ------------------------------------------------------------------------------------------------
void PvUnit::Release()
{
    mOnQuery.Release();
    mOnGained.Release();
    mOnLost.Release();
    mTag.Release();
    mData.Release();
}

// ------------------------------------------------------------------------------------------------
void PvUnit::ValidateManager() const
{
    ValidateClass();
    // Validate the manager through the class
    if (!(mClass.lock()->mManager))
    {
        STHROWF("Unit ({} : {}) has invalid manager reference", mID, mTag.mPtr);
    }
}

// ------------------------------------------------------------------------------------------------
PvManager & PvUnit::ValidManager() const
{
    ValidateClass();
    // Retrieve the manager pointer (lock only once)
    PvManager * ptr = mClass.lock()->mManager;
    // Validate the manager through the class
    if (!ptr)
    {
        STHROWF("Unit ({} : {}) has invalid manager reference", mID, mTag.mPtr);
    }
    // Acquire a reference and return it
    return *ptr;
}

// ------------------------------------------------------------------------------------------------
const Function & PvUnit::GetOnQuery(SQInteger id) const
{
    // Should we go for the one in the class?
    if (mOnQuery.IsNull())
    {
        return ValidClass().GetOnQuery(id);
    }
    // We're using our own
    return mOnQuery;
}

// ------------------------------------------------------------------------------------------------
const Function & PvUnit::GetOnGained(SQInteger id) const
{
    // Should we go for the one in the class?
    if (mOnGained.IsNull())
    {
        return ValidClass().GetOnGained(id);
    }
    // We're using our own
    return mOnGained;
}

// ------------------------------------------------------------------------------------------------
const Function & PvUnit::GetOnLost(SQInteger id) const
{
    // Should we go for the one in the class?
    if (mOnLost.IsNull())
    {
        return ValidClass().GetOnLost(id);
    }
    // We're using our own
    return mOnLost;
}

// ------------------------------------------------------------------------------------------------
SQInteger PvUnit::GetEntryValue(SQInteger id) const
{
    // Look for the specified status value
    auto itr = mPrivileges.find(id);
    // Should we go for the one in the parent?
    if (itr == mPrivileges.end())
    {
        return ValidClass().GetEntryValue(id);
    }
    // Return the associated value
    return itr->second;
}

// ------------------------------------------------------------------------------------------------
SQInteger PvUnit::GetInheritedEntryValue(SQInteger id) const
{
    return ValidClass().GetEntryValue(id);
}

// ------------------------------------------------------------------------------------------------
void PvUnit::DoGained(SQInteger id, SQInteger value) const
{
    // Function to be called when a privilege is gained
    const Function & gained = GetOnGained(id);
    // Is there someone interested in this result?
    if (!gained.IsNull())
    {
        gained.Execute(id, value);
    }
}

// ------------------------------------------------------------------------------------------------
void PvUnit::DoLost(SQInteger id, SQInteger value) const
{
    // Function to be called when a privilege is lost
    const Function & lost = GetOnLost(id);
    // Is there someone interested in this result?
    if (!lost.IsNull())
    {
        lost.Execute(id, value);
    }
}

// ------------------------------------------------------------------------------------------------
void PvUnit::DoChanged(SQInteger id, bool status, SQInteger value) const
{
    // Was this considered an upgrade?
    if (status)
    {
        DoGained(id, value);
    }
    else
    {
        DoLost(id, value);
    }
}

// ------------------------------------------------------------------------------------------------
void PvUnit::AssignPrivilege(SQInteger id, SQInteger value)
{
    // Find the current status of this entry
    SQInteger current = GetEntryValue(id);
    // Retrieve the associated entry
    PvEntry & entry = ValidManager().ValidEntry(id);
    // Is there someone that can identify this change?
    if (!entry.mOnModify.IsNull())
    {
        LightObj r = entry.mOnModify.Eval(current, value);
        // Was this considered a change?
        if (!r.IsNull())
        {
            DoChanged(id, r.Cast< bool >(), value);
        }
    }
    // Either way, we are setting this value
    mPrivileges[id] = value;
}

// ------------------------------------------------------------------------------------------------
void PvUnit::RemovePrivilege(SQInteger id)
{
    // Look for the status of this value
    auto itr = mPrivileges.find(id);
    // Do we even have this status?
    if (itr == mPrivileges.end())
    {
        return; // Nothing to remove!
    }
    // Find the inherited status of this entry
    SQInteger inherited = GetInheritedEntryValue(id);
    // Get the current status of this entry
    SQInteger current = itr->second;
    // Erase this status value
    mPrivileges.erase(itr);
    // Retrieve the associated entry
    PvEntry & entry = ValidManager().ValidEntry(id);
    // Is there someone that can identify this change?
    if (!entry.mOnModify.IsNull())
    {
        LightObj r = entry.mOnModify.Eval(current, inherited);
        // Was this considered a change?
        if (!r.IsNull())
        {
            DoChanged(id, r.Cast< bool >(), current);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void PvUnit::ModifyPrivilege(SQInteger id, SQInteger value)
{
    // Find the current status of this entry
    SQInteger current = GetEntryValue(id);
    // If they are exactly the same
    if (value == current)
    {
        return; // Just do nothing
    }
    // Retrieve the associated entry
    PvEntry & entry = ValidManager().ValidEntry(id);
    // Is there someone that can identify this change?
    if (!entry.mOnModify.IsNull())
    {
        LightObj r = entry.mOnModify.Eval(current, value);
        // Was this considered a change?
        if (!r.IsNull())
        {
            DoChanged(id, r.Cast< bool >(), value);
            // Use this value now as well
            mPrivileges[id] = value;
        }
    }
    else
    {
        // By default we use > comparison to decide upgrades
        DoChanged(id, value > current, value);
        // Use this value now
        mPrivileges[id] = value;
    }
}

// ------------------------------------------------------------------------------------------------
void PvUnit::AssignPrivilege(StackStrF & tag, SQInteger value)
{
    AssignPrivilege(ValidManager().GetValidEntryWithTag(tag.CacheHash())->mID, value);
}

// ------------------------------------------------------------------------------------------------
void PvUnit::RemovePrivilege(StackStrF & tag)
{
    RemovePrivilege(ValidManager().GetValidEntryWithTag(tag.CacheHash())->mID);
}

// ------------------------------------------------------------------------------------------------
void PvUnit::ModifyPrivilege(StackStrF & tag, SQInteger value)
{
    ModifyPrivilege(ValidManager().GetValidEntryWithTag(tag.CacheHash())->mID, value);
}

// ------------------------------------------------------------------------------------------------
void PvUnit::RemoveAllPrivileges()
{
    // Discard all privileges but not before gaining ownership of them
    PvStatusList list = std::move(mPrivileges);
    // Go over all entries and see if this unit will gain or loose any privileges from this change
    for (const auto & e : list)
    {
        // Get the value that we have now after the change
        SQInteger current = GetEntryValue(e.first);
        // Were they literally the same?
        if (current == e.second)
        {
            continue; // Don't even bother
        }
        // Retrieve the associated entry
        PvEntry & entry = ValidManager().ValidEntry(e.first);
        // Is there someone that can identify this change?
        if (!entry.mOnModify.IsNull())
        {
            LightObj r = entry.mOnModify.Eval(current, e.second);
            // Was this considered a change?
            if (!r.IsNull())
            {
                DoChanged(e.first, r.Cast< bool >(), e.second);
            }
        }
        else
        {
            // By default we use > comparison to decide upgrades
            DoChanged(e.first, e.second > current, e.second);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void PvUnit::AssignClass(const std::shared_ptr< PvClass > & cls)
{
    // Make sure we have a valid class
    ValidateClass();
    // Get a strong reference to the current class
    PvClass::Ref current = mClass.lock();
    // Are they the same?
    if (current == cls)
    {
        return; // Nothing will change
    }
    // Assign this class
    mClass = cls;
    // Propagate changes
    ValidManager().PropagateClassChange(*this, cls);
}

// ------------------------------------------------------------------------------------------------
bool PvUnit::Can(SQInteger id, SQInteger req) const
{
    // Get the current status of the specified entry
    SQInteger current = GetEntryValue(id);
    // Retrieve the function responsible for the query event
    const Function & query = GetOnQuery(id);
    // Is there someone that can arbitrate this request?
    if (!query.IsNull())
    {
        // Attempt arbitration
        LightObj r = query.Eval(current, req);
        // If NULL or false the request was denied
        if (!r.IsNull() && r.Cast< bool >())
        {
            return true; // Request allowed
        }
    }
    // We use the >= comparison to settle arbitration
    else if (current >= ValidManager().ValidEntry(id).mDefault)
    {
        return true;
    }
    // Request failed, no arbitration
    return false;
}

// ------------------------------------------------------------------------------------------------
void PvUnit::EachEntryID(Object & ctx, Function & func)
{
    // In order to be safe from modifications while iterating, create a copy
    PvStatusList list(mPrivileges);
    // Iterate entries and forward the ID to the callback
    for (const auto & e : list)
    {
        func(ctx, e.first);
    }
}

// ================================================================================================
LightObj SqPvUnit::GetClass() const
{
    return LightObj(SqTypeIdentity< SqPvClass >{}, SqVM(), Valid().mClass);
}

// ------------------------------------------------------------------------------------------------
inline void SqPvUnit::SetClass(const SqPvClass & cls) const
{
    Validate();
    cls.Validate();
    mI.lock()->AssignClass(cls.mI.lock());
}

// ------------------------------------------------------------------------------------------------
LightObj SqPvUnit::GetManager() const { return LightObj(ValidCls().mManager); }

// ------------------------------------------------------------------------------------------------
bool SqPvUnit::Can(LightObj & obj, SQInteger req) const
{
    // Entry ID?
    if (obj.GetType() == OT_INTEGER)
    {
        return Valid().Can(obj.Cast< SQInteger >(), req);
    }
    // Entry tag?
    else if (obj.GetType() == OT_STRING)
    {
        Var< LightObj >::push(SqVM(), obj);
        // Tag string
        StackStrF tag(SqVM(), -1);
        // Attempt extraction
        if (SQ_FAILED(tag.Proc(false)))
        {
            // Restore the stack first
            sq_poptop(SqVM());
            // Now the exception
            STHROWF("Unable to extract tag string");
        }
        // Restore the stack
        sq_poptop(SqVM());
        // Reference the instance
        PvUnit & u = Valid();
        // Generate and cache the hash
        tag.CacheHash();
        // Forward request
        return u.Can(u.ValidManager().GetValidEntryWithTag(tag)->mID, req);
    }
    // Entry instance?
    else if (obj.GetType() == OT_INSTANCE && obj.GetTypeTag() == StaticClassTypeTag< SqPvEntry >::Get())
    {
        return Valid().Can(obj.CastI< SqPvEntry >()->Valid().mID, req);
    }
    STHROWF("Unknown or unsupported entry identification type (%s)", SqTypeName(obj.GetType()));
    SQ_UNREACHABLE
}

// ================================================================================================
void Register_Privilege_Unit(HSQUIRRELVM vm, Table & ns)
{
    ns.Bind(_SC("Unit"),
        Class< SqPvUnit, NoConstructor< SqPvUnit > >(vm, UnitTn::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &UnitTn::Fn)
        .Func(_SC("_tostring"), &SqPvUnit::ToString)
        // Core Properties
        .Prop(_SC("ID"), &SqPvUnit::GetID)
        .Prop(_SC("Tag"), &SqPvUnit::GetTag, &SqPvUnit::SetTag)
        .Prop(_SC("Data"), &SqPvUnit::GetData, &SqPvUnit::SetData)
        .Prop(_SC("Authority"), &SqPvUnit::GetAuthority, &SqPvUnit::SetAuthority)
        .Prop(_SC("Manager"), &SqPvUnit::GetManager)
        .Prop(_SC("Class"), &SqPvUnit::GetClass, &SqPvUnit::SetClass)
        .Prop(_SC("Authority"), &SqPvUnit::GetAuthority, &SqPvUnit::SetAuthority)
        // Core Methods
        .FmtFunc(_SC("SetTag"), &SqPvUnit::ApplyTag)
        .CbFunc(_SC("OnQuery"), &SqPvUnit::SetOnQuery)
        .CbFunc(_SC("OnLost"), &SqPvUnit::SetOnLost)
        .CbFunc(_SC("OnGained"), &SqPvUnit::SetOnGained)
        // Member Methods
        .Func(_SC("Can"), &SqPvUnit::Can)
        .Func(_SC("Assign"), &SqPvUnit::AssignPrivilegeWithID)
        .FmtFunc(_SC("AssignWithTag"), &SqPvUnit::AssignPrivilegeWithTag)
        .Func(_SC("Remove"), &SqPvUnit::RemovePrivilegeWithID)
        .FmtFunc(_SC("RemoveWithTag"), &SqPvUnit::RemovePrivilegeWithTag)
        .Func(_SC("Modify"), &SqPvUnit::ModifyPrivilegeWithID)
        .FmtFunc(_SC("ModifyWithTag"), &SqPvUnit::ModifyPrivilegeWithTag)
        .Func(_SC("RemoveAll"), &SqPvUnit::RemoveAllPrivileges)
        .Func(_SC("EachEntryID"), &SqPvUnit::EachEntryID)
    );
}

} // Namespace:: SqMod
