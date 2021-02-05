// ------------------------------------------------------------------------------------------------
#include "Core/Privilege.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(ManagerTn, _SC("SqPrivilegeManager"))

// ------------------------------------------------------------------------------------------------
void TerminatePrivileges()
{
    // Go over all managers and try to close them
    for (PvManager * inst = PvManager::sHead; inst && inst->mNext != PvManager::sHead; inst = inst->mNext)
    {
        inst->Terminate();
    }
}

// ------------------------------------------------------------------------------------------------
void PvManager::Release()
{
    // Release objects from this instance
    m_OnQuery.Release();
    m_OnGained.Release();
    m_OnLost.Release();
    m_Tag.Release();
    m_Data.Release();
}

// ------------------------------------------------------------------------------------------------
void PvManager::Terminate()
{
    // Release script objects held by classes
    for (auto & c : m_Classes)
    {
        //c->Release();
    }
    // Release script objects held by units
    for (auto & u : m_Units)
    {
        //c->Release();
    }
    // Release script objects held by entries
    for (auto & e : m_Entries)
    {
        //c->Release();
    }
    // Release script objects held by the manager
    Release();
}

// ------------------------------------------------------------------------------------------------
LightObj PvManager::CreateEntry(SQInteger id, StackStrF & name)
{
    ModifyEntries();
    // Look for a similar entry
    auto itr = m_Entries.find(PvIdentity(id));
    // Is this unique?
    if (itr != m_Entries.end())
    {
        STHROWF("Entry ({} : {}) already exists", id, name.mPtr);
    }
    // We backup the name hash now because I don't know if the move will happen before or after i create the identity
    // Compiler optimizations and sh!t. (the name hash is cached in .mRes member variable)
    const auto h = name.CacheHash().GetHash();
    // Create it now
    auto & e = m_Entries.emplace_back(PvIdentity(id, h), std::make_shared< PvEntry >(id, std::move(name), this));
    // Create a wrapper instance and return it
    return LightObj(SqTypeIdentity< SqPvEntry >{}, SqVM(), e);
}

// ------------------------------------------------------------------------------------------------
LightObj PvManager::CreateClass(SQInteger id, StackStrF & name)
{
    ModifyClasses();
    // Look for a similar class
    auto itr = m_Classes.find(PvIdentity(id));
    // Is this unique?
    if (itr != m_Classes.end())
    {
        STHROWF("Class ({} : {}) already exists", id, name.mPtr);
    }
    // We backup the name hash now because I don't know if the move will happen before or after i create the identity
    // Compiler optimizations and sh!t. (the name hash is cached in .mRes member variable)
    const auto h = name.CacheHash().GetHash();
    // Create it now
    auto & e = m_Classes.emplace_back(PvIdentity(id, h), std::make_shared< PvClass >(id, std::move(name), this));
    // Create a wrapper instance and return it
    return LightObj(SqTypeIdentity< SqPvClass >{}, SqVM(), e);
}

// ------------------------------------------------------------------------------------------------
LightObj PvManager::CreateUnit(SQInteger id, const SqPvClass & cls, StackStrF & name)
{
    ModifyUnits();
    // Validate the given class
    cls.Validate();
    // Look for a similar unit
    auto itr = m_Units.find(PvIdentity(id));
    // Is this unique?
    if (itr != m_Units.end())
    {
        STHROWF("Unit ({} : {}) already exists", id, name.mPtr);
    }
    // We backup the name hash now because I don't know if the move will happen before or after i create the identity
    // Compiler optimizations and sh!t. (the name hash is cached in .mRes member variable)
    const auto h = name.CacheHash().GetHash();
    // Create it now
    auto & e = m_Units.emplace_back(PvIdentity(id, h), std::make_shared< PvUnit >(id, std::move(name), cls.mI));
    // Create a wrapper instance and return it
    return LightObj(SqTypeIdentity< SqPvUnit >{}, SqVM(), e);
}

// ------------------------------------------------------------------------------------------------
void PvManager::PropagateParentAssign(const PvClass & cls, const PvClass::Ref & parent)
{
    
}

// ------------------------------------------------------------------------------------------------
void PvManager::PropagateParentChange(const PvClass & cls, const PvClass::Ref & parent)
{
    // Prevent any changes to entries during this operation
    AutoAssign< bool > aag(m_LockEntries, false, true);
    // Go over all entries and see if the specified class will gain or loose any privileges from this change
    for (const auto & e : m_Entries)
    {
        // Owned privileges are not affected regardless of the inherited class
        {
            // See if this class inherits the current entry
            auto itr = cls.mPrivileges.find(e.second->mID);
            // Is this entry inherited or owned?
            if (itr != cls.mPrivileges.end())
            {
                break; // Nothing new will happen here
            }
        }
        // We know we inherit this entry value so let's get that value instead
        SQInteger inherited = cls.GetInheritedEntryValue(e.second->mID);
        // Get the value that will be inherited for this entry
        SQInteger current = parent->GetEntryValue(e.second->mID);
        // Are they literally the same?
        if (inherited == current)
        {
            break; // Don't even bother
        }
        // Find out who can identify this change
        const Function & modify = e.second->mOnModify.IsNull() ? m_OnModify : e.second->mOnModify;
        // Is there someone that can identify this change?
        if (!modify.IsNull())
        {
            LightObj r = modify.Eval(inherited, current);
            // Was this considered a change?
            if (!r.IsNull())
            {
                cls.DoChanged(e.second->mID, r.Cast< bool >(), current);
            }
        }
        else
        {
            // By default we use > comparison to decide upgrades
            cls.DoChanged(e.second->mID, current > inherited, current);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void PvManager::PropagateClassChange(const PvUnit & unit, const PvClass::Ref & cls)
{
    // Prevent any changes to entries during this operation
    AutoAssign< bool > aag(m_LockEntries, false, true);
    // Go over all entries and see if the specified unit will gain or loose any privileges from this change
    for (const auto & e : m_Entries)
    {
        // Owned privileges are not affected regardless of the inherited class
        {
            // See if this unit inherits the current entry
            auto itr = unit.mPrivileges.find(e.second->mID);
            // Is this entry inherited or owned?
            if (itr != unit.mPrivileges.end())
            {
                break; // Nothing new will happen here
            }
        }
        // We know we inherit this entry value so let's get that value instead
        SQInteger inherited = unit.GetInheritedEntryValue(e.second->mID);
        // Get the value that will be inherited for this entry
        SQInteger current = cls->GetEntryValue(e.second->mID);
        // Are they literally the same?
        if (inherited == current)
        {
            break; // Don't even bother
        }
        // Find out who can identify this change
        const Function & modify = e.second->mOnModify.IsNull() ? m_OnModify : e.second->mOnModify;
        // Is there someone that can identify this change?
        if (!modify.IsNull())
        {
            LightObj r = modify.Eval(inherited, current);
            // Was this considered a change?
            if (!r.IsNull())
            {
                unit.DoChanged(e.second->mID, r.Cast< bool >(), current);
            }
        }
        else
        {
            // By default we use > comparison to decide upgrades
            unit.DoChanged(e.second->mID, current > inherited, current);
        }
    }
}

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD LightObj PvManager::GetEntryWithID(SQInteger id)
{
    return LightObj(SqTypeIdentity< SqPvEntry >{}, SqVM(), GetValidEntry(id));
}

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD LightObj PvManager::GetEntryWithTag(StackStrF & tag)
{
    return LightObj(SqTypeIdentity< SqPvEntry >{}, SqVM(), GetValidEntryWithTag(tag.CacheHash()));
}

// ------------------------------------------------------------------------------------------------
bool PvManager::HaveEntryWithID(SQInteger id)
{
    for (const auto & e : m_Entries) // NOLINT(readability-use-anyofallof)
    {
        if (e.first.mID == id)
        {
            return true;
        }
    }
    // Not found
    return false;
}

// ------------------------------------------------------------------------------------------------
bool PvManager::HaveEntryWithTag(StackStrF & tag)
{
    const size_t h = tag.ToHash();
    for (const auto & e : m_Entries) // NOLINT(readability-use-anyofallof)
    {
        if (e.first.mHash == h)
        {
            return true;
        }
    }
    // Not found
    return false;
}

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD LightObj PvManager::GetClassWithID(SQInteger id)
{
    return LightObj(SqTypeIdentity< SqPvClass >{}, SqVM(), GetValidClass(id));
}

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD LightObj PvManager::GetClassWithTag(StackStrF & tag)
{
    return LightObj(SqTypeIdentity< SqPvClass >{}, SqVM(), GetValidClassWithTag(tag.CacheHash()));
}

// ------------------------------------------------------------------------------------------------
bool PvManager::HaveClassWithID(SQInteger id)
{
    for (const auto & c : m_Classes) // NOLINT(readability-use-anyofallof)
    {
        if (c.first.mID == id)
        {
            return true;
        }
    }
    // Not found
    return false;
}

// ------------------------------------------------------------------------------------------------
bool PvManager::HaveClassWithTag(StackStrF & tag)
{
    const size_t h = tag.ToHash();
    for (const auto & c : m_Classes) // NOLINT(readability-use-anyofallof)
    {
        if (c.first.mHash == h)
        {
            return true;
        }
    }
    // Not found
    return false;
}

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD LightObj PvManager::GetUnitWithID(SQInteger id)
{
    return LightObj(SqTypeIdentity< SqPvUnit >{}, SqVM(), GetValidUnit(id));
}

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD LightObj PvManager::GetUnitWithTag(StackStrF & tag)
{
    return LightObj(SqTypeIdentity< SqPvUnit >{}, SqVM(), GetValidUnitWithTag(tag.CacheHash()));
}

// ------------------------------------------------------------------------------------------------
bool PvManager::HaveUnitWithID(SQInteger id)
{
    for (const auto & u : m_Units) // NOLINT(readability-use-anyofallof)
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
bool PvManager::HaveUnitWithTag(StackStrF & tag)
{
    const size_t h = tag.ToHash();
    for (const auto & u : m_Units) // NOLINT(readability-use-anyofallof)
    {
        if (u.first.mHash == h)
        {
            return true;
        }
    }
    // Not found
    return false;
}

// ------------------------------------------------------------------------------------------------
extern void Register_Privilege_Class(HSQUIRRELVM vm, Table & ns);
extern void Register_Privilege_Entry(HSQUIRRELVM vm, Table & ns);
extern void Register_Privilege_Unit(HSQUIRRELVM vm, Table & ns);

// ================================================================================================
void Register_Privilege(HSQUIRRELVM vm)
{
    Table ns(vm);

    // --------------------------------------------------------------------------------------------
    Register_Privilege_Class(vm, ns);
    Register_Privilege_Entry(vm, ns);
    Register_Privilege_Unit(vm, ns);

    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Manager"),
        Class< PvManager, NoCopy< PvManager > >(vm, ManagerTn::Str)
        // Constructors
        .Ctor()
        .Ctor< StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &ManagerTn::Fn)
        .Func(_SC("_tostring"), &PvManager::ToString)
        // Core Properties
        .Prop(_SC("Tag"), &PvManager::GetTag, &PvManager::SetTag)
        .Prop(_SC("Data"), &PvManager::GetData, &PvManager::SetData)
        // Core Methods
        .FmtFunc(_SC("SetTag"), &PvManager::ApplyTag)
        .CbFunc(_SC("OnQuery"), &PvManager::SetOnQuery)
        .CbFunc(_SC("OnLost"), &PvManager::SetOnLost)
        .CbFunc(_SC("OnGained"), &PvManager::SetOnGained)
        // Member Methods
        .CbFunc(_SC("CreateEntry"), &PvManager::CreateEntry)
        .CbFunc(_SC("CreateClass"), &PvManager::CreateClass)
        .CbFunc(_SC("CreateUnit"), &PvManager::CreateUnit)
        .Func(_SC("GetEntry"), &PvManager::GetEntryWithID)
        .FmtFunc(_SC("GetEntryWithTag"), &PvManager::GetEntryWithTag)
        .Func(_SC("HaveEntry"), &PvManager::HaveEntryWithID)
        .FmtFunc(_SC("HaveEntryWithTag"), &PvManager::HaveEntryWithTag)
        .Func(_SC("GetClass"), &PvManager::GetClassWithID)
        .FmtFunc(_SC("GetClassWithTag"), &PvManager::GetClassWithTag)
        .Func(_SC("HaveClass"), &PvManager::HaveClassWithID)
        .FmtFunc(_SC("HaveClassWithTag"), &PvManager::HaveClassWithTag)
        .Func(_SC("GetUnit"), &PvManager::GetUnitWithID)
        .FmtFunc(_SC("GetUnitWithTag"), &PvManager::GetUnitWithTag)
        .Func(_SC("HaveUnit"), &PvManager::HaveUnitWithID)
        .FmtFunc(_SC("HaveUnitWithTag"), &PvManager::HaveUnitWithTag)
    );

    RootTable(vm).Bind(_SC("SqPrivilege"), ns);
}

} // Namespace:: SqMod
