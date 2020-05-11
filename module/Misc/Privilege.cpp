// ------------------------------------------------------------------------------------------------
#include "Misc/Privilege.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(UnitTn, _SC("SqPrivilegeUnit"))
SQMODE_DECL_TYPENAME(ClassTn, _SC("SqPrivilegeClass"))
SQMODE_DECL_TYPENAME(EntryTn, _SC("SqPrivilegeEntry"))
SQMODE_DECL_TYPENAME(ManagerTn, _SC("SqPrivilegeManager"))

// ------------------------------------------------------------------------------------------------
// Helper value used to identify an index that doesn't exist.
static constexpr size_t BAD_POS = ~static_cast< size_t >(0);

// ------------------------------------------------------------------------------------------------
PvManagers PvManager::s_Managers;

// ------------------------------------------------------------------------------------------------
PvManager & PvUnit::GetManager() const
{
    return m_Class->GetManager(); // Classes must always have a manager!
}
// ------------------------------------------------------------------------------------------------
PvClass & PvUnit::GetClass() const
{
    return *m_Class; // Units must always have a class!
}
// ------------------------------------------------------------------------------------------------
void PvUnit::SetClass(PvClass & cls)
{
    cls.AddUnit(*this);
}
// ------------------------------------------------------------------------------------------------
bool PvUnit::Can(const PvEntry & entry) const
{
    return false;
}
// ------------------------------------------------------------------------------------------------
bool PvUnit::Can(const PvEntry & entry, const PvUnit & unit) const
{
    return false;
}

// ------------------------------------------------------------------------------------------------
PvUnit * PvClass::GetUnitByID(SQInteger id) const
{
    // Locate the requested unit
    for (const auto & e : m_Units)
    {
        if (e.first == id)
        {
            return e.second;
        }
    }
    // No such unit
    return nullptr;
}

// ------------------------------------------------------------------------------------------------
PvUnit * PvClass::GetUnitByTag(StackStrF & name) const
{
    // Retrieve the hash
    auto hash = name.HashIt();
    // Locate the requested unit
    for (const auto & e : m_Units)
    {
        // Does the string hash match?
        if (e.second->GetTagW().GetHash() == hash &&
            // Check if the whole string matches as well
            strncmp(name.mPtr, e.second->GetTagW().mPtr,
                    std::min(name.mLen, e.second->GetTagW().mLen)) == 0)
        {
            return e.second; // This is what we're looking for
        }
    }
    // No such unit
    return nullptr;
}

// ------------------------------------------------------------------------------------------------
LightObj PvClass::CreateUnit(SQInteger id, StackStrF & name)
{
    return m_Manager->CreateUnitImpl(id, *this, name).second;
}

// ------------------------------------------------------------------------------------------------
void PvClass::AddUnit(PvUnit & unit)
{
    // Is this unit already of this class?
    if (&unit.GetClass() == this)
    {
        return; // Nothing to change!
    }
    // Remove it from current class
    unit.GetClass().UnlistUnit(unit);
    // Enlist it with us
    EnlistUnit(unit);
    // Who's your daddy now!
    unit.m_Class = this;
}

// ------------------------------------------------------------------------------------------------
bool PvClass::EnlistUnit(PvUnit & unit)
{
    // Does it exist already?
    for (const auto & e : m_Units)
    {
        if (e.first == unit.GetID())
        {
            return false; // Already enlisted here!
        }
    }
    // Enlist it now
    m_Units.emplace_back(unit.GetID(), &unit);
    // Enlisted now
    return true;
}

// ------------------------------------------------------------------------------------------------
bool PvClass::UnlistUnit(PvUnit & unit)
{
    // Locate the entry in our list
    auto it = std::find_if(m_Units.cbegin(), m_Units.cend(),
        [=, id = unit.GetID()](PvUnitList::const_reference e) -> bool {
        return e.first == id;
    });
    // Is this unit enlisted here?
    if (it == m_Units.end())
    {
        return false; // Not enlisted here!
    }
    // Remove the unit from out container
    m_Units.erase(it);
    // Unlisted now
    return true;
}

// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
PvEntry * PvManager::GetEntryByID(SQInteger id) const
{
    // Locate the requested entry
    for (const auto & e : m_Entries)
    {
        if (e.first == id)
        {
            return e.second;
        }
    }
    // No such entry
    return nullptr;
}
// ------------------------------------------------------------------------------------------------
PvEntry * PvManager::GetEntryByTag(StackStrF & name) const
{
    // Retrieve the hash
    auto hash = name.HashIt();
    // Locate the requested entry
    for (const auto & e : m_Entries)
    {
        // Does the string hash match?
        if (e.second->GetTagW().GetHash() == hash &&
            // Check if the whole string matches as well
            strncmp(name.mPtr, e.second->GetTagW().mPtr,
                    std::min(name.mLen, e.second->GetTagW().mLen)) == 0)
        {
            return e.second; // This is what we're looking for
        }
    }
    // No such entry
    return nullptr;
}
// ------------------------------------------------------------------------------------------------
LightObj PvManager::CreateEntry(SQInteger id, StackStrF & name)
{
    // See if this entry exists already
    PvEntry * e = GetEntryByID(id);
    // Return existing entry if already exists
    if (e != nullptr)
    {
        return LightObj(e);
    }
    // Create an entry instance
    LightObj o(DeleteGuard< PvEntry >(new PvEntry(id, std::move(name), this)));
    // Get a pointer to the new instance
    e = o.CastI< PvEntry >();
    // Append the new entry
    m_Entries.emplace_back(id, e);
    // Return the resulted object
    return o;
}

// ------------------------------------------------------------------------------------------------
PvClass *PvManager::GetClassByID(SQInteger id) const
{
    // Locate the requested class
    for (const auto & e : m_Classes)
    {
        if (e.first == id)
        {
            return e.second;
        }
    }
    // No such class
    return nullptr;
}

// ------------------------------------------------------------------------------------------------
PvClass * PvManager::GetClassByTag(StackStrF & name) const
{
    // Retrieve the hash
    auto hash = name.HashIt();
    // Locate the requested class
    for (const auto & e : m_Classes)
    {
        // Does the string hash match?
        if (e.second->GetTagW().GetHash() == hash &&
            // Check if the whole string matches as well
            strncmp(name.mPtr, e.second->GetTagW().mPtr,
                    std::min(name.mLen, e.second->GetTagW().mLen)) == 0)
        {
            return e.second; // This is what we're looking for
        }
    }
    // No such class
    return nullptr;
}

// ------------------------------------------------------------------------------------------------
LightObj PvManager::CreateClass(SQInteger id, StackStrF & name)
{
    // See if this class exists already
    PvClass * e = GetClassByID(id);
    // Return existing class if already exists
    if (e != nullptr)
    {
        return LightObj(e);
    }
    // Create an class instance
    LightObj o(DeleteGuard< PvClass >(new PvClass(id, std::move(name), this)));
    // Get a pointer to the new instance
    e = o.CastI< PvClass >();
    // Append the new class
    m_Classes.emplace_back(id, e);
    // Return the resulted object
    return o;
}

// ------------------------------------------------------------------------------------------------
PvUnit * PvManager::GetUnitByID(SQInteger id) const
{
    // Locate the requested unit
    for (const auto & e : m_Units)
    {
        if (e.first == id)
        {
            return e.second;
        }
    }
    // No such unit
    return nullptr;
}

// ------------------------------------------------------------------------------------------------
PvUnit * PvManager::GetUnitByTag(StackStrF & name) const
{
    // Retrieve the hash
    auto hash = name.HashIt();
    // Locate the requested unit
    for (const auto & e : m_Units)
    {
        // Does the string hash match?
        if (e.second->GetTagW().GetHash() == hash &&
            // Check if the whole string matches as well
            strncmp(name.mPtr, e.second->GetTagW().mPtr,
                    std::min(name.mLen, e.second->GetTagW().mLen)) == 0)
        {
            return e.second; // This is what we're looking for
        }
    }
    // No such unit
    return nullptr;
}

// ------------------------------------------------------------------------------------------------
LightObj PvManager::CreateUnit(SQInteger id, PvClass & cls, StackStrF & name)
{
    // Return the resulted object
    return CreateUnitImpl(id, cls, name).second;
}

// ------------------------------------------------------------------------------------------------
std::pair< PvUnit *, LightObj > PvManager::CreateUnitImpl(SQInteger id, PvClass & cls, StackStrF & name)
{
    // See if this unit exists already
    PvUnit * unit = GetUnitByID(id);
    // Return existing unit if already exists
    if (unit != nullptr)
    {
        // Return the existing unit
        return std::make_pair(unit, LightObj{unit});
    }
    // Create a unit instance
    LightObj o{DeleteGuard< PvUnit >(new PvUnit(id, std::move(name), &cls))};
    // Grab the pointer from the object
    unit = o.CastI< PvUnit >();
    // Enlist the new unit with this manager
    m_Units.emplace_back(id, unit);
    // Enlist the new unit on the specified class
    cls.EnlistUnit(*unit);
    // Return the created unit
    return std::make_pair(unit, std::move(o));
}

// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
void TerminatePrivileges()
{
    PvManager::Terminate();
}

// ================================================================================================
void Register_Privilege(HSQUIRRELVM vm)
{
    Table pns(vm);

    pns.Bind(_SC("Unit"),
        Class< PvUnit, NoConstructor< PvUnit > >(vm, UnitTn::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &UnitTn::Fn)
        .Func(_SC("_tostring"), &PvUnit::ToString)
        // Core Properties
        .Prop(_SC("ID"), &PvUnit::GetID)
        .Prop(_SC("Tag"), &PvUnit::GetTag, &PvUnit::SetTag)
        .Prop(_SC("Data"), &PvUnit::GetData, &PvUnit::SetData)
        .Prop(_SC("Manager"), &PvUnit::GetManager)
        .Prop(_SC("Class"), &PvUnit::GetClass, &PvUnit::SetClass)
        .Prop(_SC("Authority"), &PvUnit::GetAuthority, &PvUnit::SetAuthority)
        // Core Methods
        .FmtFunc(_SC("SetTag"), &PvUnit::ApplyTag)
        // Member Methods
        .CbFunc(_SC("OnQuery"), &PvUnit::SetOnQuery)
        .CbFunc(_SC("OnLost"), &PvUnit::SetOnLost)
        .CbFunc(_SC("OnGained"), &PvUnit::SetOnGained)
        // Member Overloads
        .Overload< bool (PvUnit::*)(const PvEntry &) const >
            (_SC("Can"), &PvUnit::Can)
        .Overload< bool (PvUnit::*)(const PvEntry &, const PvUnit &) const >
            (_SC("Can"), &PvUnit::Can)
    );

    pns.Bind(_SC("Class"),
        Class< PvClass, NoConstructor< PvClass > >(vm, ClassTn::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &ClassTn::Fn)
        .Func(_SC("_tostring"), &PvClass::ToString)
        // Core Properties
        .Prop(_SC("ID"), &PvClass::GetID)
        .Prop(_SC("Tag"), &PvClass::GetTag, &PvClass::SetTag)
        .Prop(_SC("Data"), &PvClass::GetData, &PvClass::SetData)
        .Prop(_SC("Parent"), &PvClass::GetParent, &PvClass::SetParent)
        .Prop(_SC("Manager"), &PvClass::GetManager)
        // Core Methods
        .FmtFunc(_SC("SetTag"), &PvClass::ApplyTag)
        // Member Methods
        .Func(_SC("AddUnit"), &PvClass::AddUnit)
        .Func(_SC("GetUnit"), &PvClass::GetUnitByID)
        .Func(_SC("GetUnitByID"), &PvClass::GetUnitByID)
        .Func(_SC("HaveUnitWithID"), &PvClass::HaveUnitWithID)
        .FmtFunc(_SC("GetUnitByTag"), &PvClass::GetUnitByTag)
        .FmtFunc(_SC("HaveUnitWithTag"), &PvClass::HaveUnitWithTag)
        .FmtFunc(_SC("CreateUnit"), &PvClass::CreateUnit)
        .CbFunc(_SC("OnQuery"), &PvClass::SetOnQuery)
        .CbFunc(_SC("OnLost"), &PvClass::SetOnLost)
        .CbFunc(_SC("OnGained"), &PvClass::SetOnGained)
        // Raw functions
    );

    pns.Bind(_SC("Entry"),
        Class< PvEntry, NoConstructor< PvEntry > >(vm, EntryTn::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &EntryTn::Fn)
        .Func(_SC("_tostring"), &PvEntry::ToString)
        // Core Properties
        .Prop(_SC("ID"), &PvEntry::GetID)
        .Prop(_SC("Tag"), &PvEntry::GetTag, &PvEntry::SetTag)
        .Prop(_SC("Data"), &PvEntry::GetData, &PvEntry::SetData)
        .Prop(_SC("Manager"), &PvEntry::GetManager)
        .Prop(_SC("Brief"), &PvEntry::GetBrief, &PvEntry::SetBrief)
        .Prop(_SC("Info"), &PvEntry::GetInfo, &PvEntry::SetInfo)
        .Prop(_SC("Default"), &PvEntry::GetDefault, &PvEntry::SetDefault)
        // Core Methods
        .FmtFunc(_SC("SetTag"), &PvEntry::ApplyTag)
        // Member Methods
        .FmtFunc(_SC("SetBrief"), &PvEntry::ApplyBrief)
        .FmtFunc(_SC("SetInfo"), &PvEntry::ApplyInfo)
        .CbFunc(_SC("OnQuery"), &PvEntry::SetOnQuery)
        .CbFunc(_SC("OnLost"), &PvEntry::SetOnLost)
        .CbFunc(_SC("OnGained"), &PvEntry::SetOnGained)
        // Raw functions
    );

    pns.Bind(_SC("Manager"),
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
        // Member Methods
        .Func(_SC("GetEntry"), &PvManager::GetEntryByID)
        .Func(_SC("GetEntryByID"), &PvManager::GetEntryByID)
        .Func(_SC("HaveEntryWithID"), &PvManager::HaveEntryWithID)
        .FmtFunc(_SC("GetEntryByTag"), &PvManager::GetEntryByTag)
        .FmtFunc(_SC("HaveEntryWithTag"), &PvManager::HaveEntryWithTag)
        .FmtFunc(_SC("CreateEntry"), &PvManager::CreateEntry)
        .Func(_SC("GetClass"), &PvManager::GetClassByID)
        .Func(_SC("GetClassByID"), &PvManager::GetClassByID)
        .Func(_SC("HaveClassWithID"), &PvManager::HaveClassWithID)
        .FmtFunc(_SC("GetClassByTag"), &PvManager::GetClassByTag)
        .FmtFunc(_SC("HaveClassWithTag"), &PvManager::HaveClassWithTag)
        .FmtFunc(_SC("CreateClass"), &PvManager::CreateClass)
        .Func(_SC("GetUnit"), &PvManager::GetUnitByID)
        .Func(_SC("GetUnitByID"), &PvManager::GetUnitByID)
        .Func(_SC("HaveUnitWithID"), &PvManager::HaveUnitWithID)
        .FmtFunc(_SC("GetUnitByTag"), &PvManager::GetUnitByTag)
        .FmtFunc(_SC("HaveUnitWithTag"), &PvManager::HaveUnitWithTag)
        .FmtFunc(_SC("CreateUnit"), &PvManager::CreateUnit)
        .CbFunc(_SC("OnQuery"), &PvManager::SetOnQuery)
        .CbFunc(_SC("OnLost"), &PvManager::SetOnLost)
        .CbFunc(_SC("OnGained"), &PvManager::SetOnGained)
        // Raw functions
    );

    RootTable(vm).Bind(_SC("SqPrivilege"), pns);
}

} // Namespace:: SqMod
