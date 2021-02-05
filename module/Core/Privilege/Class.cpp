// ------------------------------------------------------------------------------------------------
#include "Core/Privilege/Class.hpp"
#include "Core/Privilege.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(ClassTn, _SC("SqPrivilegeClass"))

// ------------------------------------------------------------------------------------------------
void PvClass::SetTag(StackStrF & tag)
{
    mTag = std::move(tag);
    // Hash the name and cache it (the name hash is cached in .mRes member variable)
    mTag.CacheHash();
    // Propagate this change to the manager as well
    if (mManager)
    {
        mManager->UpdateClassHash(mID, static_cast< size_t >(mTag.mRes));
    }
}

// ------------------------------------------------------------------------------------------------
void PvClass::Release()
{
    mOnQuery.Release();
    mOnGained.Release();
    mOnLost.Release();
    mTag.Release();
    mData.Release();
}

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD PvManager & PvClass::ValidManager() const
{
    ValidateManager();
    // Return a reference it
    return *mManager;
}

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD const Function & PvClass::GetOnQuery(SQInteger id) const
{
    // Should we go for the one in the manager?
    if (mOnQuery.IsNull())
    {
        return ValidManager().GetOnQuery(id);
    }
    // We're using our own
    return mOnQuery;
}

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD const Function & PvClass::GetOnGained(SQInteger id) const
{
    // Should we go for the one in the manager?
    if (mOnGained.IsNull())
    {
        return ValidManager().GetOnGained(id);
    }
    // We're using our own
    return mOnGained;
}

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD const Function & PvClass::GetOnLost(SQInteger id) const
{
    // Should we go for the one in the manager?
    if (mOnLost.IsNull())
    {
        return ValidManager().GetOnLost(id);
    }
    // We're using our own
    return mOnLost;
}

// ------------------------------------------------------------------------------------------------
SQInteger PvClass::GetEntryValue(SQInteger id) const
{
    // Look for the specified status value
    auto itr = mPrivileges.find(id);
    // Should we go for the one in the parent?
    if (itr == mPrivileges.end())
    {
        // Should we go for the default one?
        if (mParent.expired())
        {
            return ValidManager().GetEntryValue(id);
        }
        // We have a valid parent
        return ValidParent().GetEntryValue(id);
    }
    // Return the associated value
    return itr->second;
}

// ------------------------------------------------------------------------------------------------
SQInteger PvClass::GetInheritedEntryValue(SQInteger id) const
{
    // Should we go for the default one?
    if (mParent.expired())
    {
        return ValidManager().GetEntryValue(id);
    }
    // We have a valid parent
    return ValidParent().GetEntryValue(id);
}

// ------------------------------------------------------------------------------------------------
void PvClass::DoGained(SQInteger id, SQInteger value) const
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
void PvClass::DoLost(SQInteger id, SQInteger value) const
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
void PvClass::DoChanged(SQInteger id, bool status, SQInteger value) const
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
void PvClass::AssignStatus(SQInteger id, SQInteger value)
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
    // Either waiy, we are setting this value
    mPrivileges[id] = value;
}

// ------------------------------------------------------------------------------------------------
void PvClass::RemoveStatus(SQInteger id)
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
void PvClass::ModifyStatus(SQInteger id, SQInteger value)
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
void PvClass::AssignParent(const Ref & parent)
{
    // Do we have a parent?
    if (mParent.expired())
    {
        // Assign the specified class
        mParent = parent;
        // Propagate changes
        ValidManager().PropagateParentAssign(*this, parent);
    }
    // Are they the same?
    else if (mParent.lock() != parent)
    {
        // Assign the specified class
        mParent = parent;
        // Propagate changes
        ValidManager().PropagateParentChange(*this, parent);
    }
}

// ------------------------------------------------------------------------------------------------
bool PvClass::Can(SQInteger id) const
{
    // Retrieve the function responsible for the query event
    const Function & query = GetOnQuery(id);
    // Is there someone that can arbitrate this request?
    if (!query.IsNull())
    {
        // Get the current status of the specified entry
        SQInteger current = GetEntryValue(id);
        // Attempt arbitration
        LightObj r = query.Eval(current);
        // If NULL or false the request was denied
        if (!r.IsNull() && r.Cast< bool >())
        {
            return true; // Request allowed
        }
    }
    // Request failed, no arbitration
    return false;
}

// ------------------------------------------------------------------------------------------------
LightObj PvClass::GetUnitWithID(SQInteger id)
{
    for (const auto & u : mUnits)
    {
        if (u.first.mID == id)
        {
            return LightObj(SqTypeIdentity< SqPvUnit >{}, SqVM(), u.second);
        }
    }
    // Not found
    STHROWF("Unit ({}) does not inherit from this class", id);
    SQ_UNREACHABLE
}

// ------------------------------------------------------------------------------------------------
LightObj PvClass::GetUnitWithTag(StackStrF & tag)
{
    const size_t h = tag.ToHash();
    for (const auto & u : mUnits)
    {
        if (u.first.mHash == h)
        {
            return LightObj(SqTypeIdentity< SqPvUnit >{}, SqVM(), u.second);
        }
    }
    // Not found
    STHROWF("Unit ({}) does not inherit from this class", tag.mPtr);
    SQ_UNREACHABLE
}

// ------------------------------------------------------------------------------------------------
bool PvClass::HaveUnitWithID(SQInteger id)
{
    for (const auto & u : mUnits) // NOLINT(readability-use-anyofallof)
    {
        if (u.first.mID == id)
        {
            return true;
        }
    }
    // Not found
    return false;
}

// ------------------------------------------------------------------------------------------------
bool PvClass::HaveUnitWithTag(StackStrF & tag)
{
    const size_t h = tag.ToHash();
    for (const auto & u : mUnits) // NOLINT(readability-use-anyofallof)
    {
        if (u.first.mHash == h)
        {
            return true;
        }
    }
    // Not found
    return false;
}

// ================================================================================================
bool SqPvClass::Can(LightObj & obj) const
{
    // Entry ID?
    if (obj.GetType() == OT_INTEGER)
    {
        return Valid().Can(obj.Cast< SQInteger >());
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
        PvClass & c = Valid();
        // Generate and cache the hash
        tag.CacheHash();
        // Forward request
        return c.Can(c.ValidManager().GetValidEntryWithTag(tag)->mID);
    }
    // Entry instance?
    else if (obj.GetType() == OT_INSTANCE && obj.GetTypeTag() == StaticClassTypeTag< SqPvEntry >::Get())
    {
        return Valid().Can(obj.CastI< SqPvEntry >()->Valid().mID);
    }
    STHROWF("Unknown or unsupported entry identification type (%s)", SqTypeName(obj.GetType()));
    SQ_UNREACHABLE
}

// ================================================================================================
void Register_Privilege_Class(HSQUIRRELVM vm, Table & ns)
{
    ns.Bind(_SC("Class"),
        Class< SqPvClass, NoConstructor< SqPvClass > >(vm, ClassTn::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &ClassTn::Fn)
        .Func(_SC("_tostring"), &SqPvClass::ToString)
        // Core Properties
        .Prop(_SC("ID"), &SqPvClass::GetID)
        .Prop(_SC("Tag"), &SqPvClass::GetTag, &SqPvClass::SetTag)
        .Prop(_SC("Data"), &SqPvClass::GetData, &SqPvClass::SetData)
        .Prop(_SC("Parent"), &SqPvClass::GetParent, &SqPvClass::SetParent)
        .Prop(_SC("Manager"), &SqPvClass::GetManager)
        // Core Methods
        .FmtFunc(_SC("SetTag"), &SqPvClass::ApplyTag)
        .CbFunc(_SC("OnQuery"), &SqPvClass::SetOnQuery)
        .CbFunc(_SC("OnLost"), &SqPvClass::SetOnLost)
        .CbFunc(_SC("OnGained"), &SqPvClass::SetOnGained)
        // Member Methods
        .Func(_SC("Can"), &SqPvUnit::Can)
        .Func(_SC("GetUnit"), &SqPvClass::GetUnitWithID)
        .FmtFunc(_SC("GetUnitWithTag"), &SqPvClass::GetUnitWithTag)
        .Func(_SC("HaveUnit"), &SqPvClass::HaveUnitWithID)
        .FmtFunc(_SC("HaveUnitWithTag"), &SqPvClass::HaveUnitWithTag)
        // Member Overloads
    );
}

} // Namespace:: SqMod
