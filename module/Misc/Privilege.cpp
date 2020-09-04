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

/* ------------------------------------------------------------------------------------------------
 * Privilege unit wrapper.
*/
class PvUnitWrap
{
private:

    /* --------------------------------------------------------------------------------------------
     * 
    */
    PvUnit::Ref mInst;

public:

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvUnitWrap(SQInteger id, PvClass * cls)
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvUnitWrap(SQInteger id, StackStrF && tag, PvClass * cls)
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    PvUnitWrap(const PvUnitWrap & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~PvUnitWrap();

    /* -------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    PvUnitWrap & operator = (const PvUnitWrap & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Release all script resources. Recursively forward request.
    */
    void Release()
    {

    }

};

/* ------------------------------------------------------------------------------------------------
 * Privilege class wrapper.
*/
class PvClassWrap
{
    friend class PvManager;

private:

    /* --------------------------------------------------------------------------------------------
     * 
    */
    PvClass::Ref mInst;

public:

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvClassWrap(SQInteger id, PvManager * mgr)
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvClassWrap(SQInteger id, StackStrF && tag, PvManager * mgr)
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    PvClassWrap(const PvClassWrap & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    PvClassWrap(PvClassWrap && o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~PvClassWrap();

    /* -------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    PvClassWrap & operator = (const PvClassWrap & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    PvClassWrap & operator = (PvClassWrap && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Release all script resources. Recursively forward request.
    */
    void Release()
    {

    }

};

/* ------------------------------------------------------------------------------------------------
 * Privilege entry wrapper.
*/
class PvEntryWrap
{
    friend class PvManager;

private:

    /* --------------------------------------------------------------------------------------------
     * 
    */
    PvEntry::Ref mInst;

public:

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvEntryWrap(SQInteger id, PvManager * mgr)
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvEntryWrap(SQInteger id, StackStrF && tag, PvManager * mgr)

    {
    }

    /* -------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    PvEntryWrap(const PvEntryWrap & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    PvEntryWrap(PvEntryWrap && o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~PvEntryWrap();

    /* -------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    PvEntryWrap & operator = (const PvEntryWrap & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    PvEntryWrap & operator = (PvEntryWrap && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Release all script resources. Recursively forward request.
    */
    void Release()
    {

    }

};

// ------------------------------------------------------------------------------------------------
void TerminatePrivileges()
{
    PvManager::Terminate();
}

// ================================================================================================
void Register_Privilege(HSQUIRRELVM vm)
{
    Table pns(vm);
/*
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
*/
/*    pns.Bind(_SC("Manager"),
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
        .CbFunc(_SC("OnQuery"), &PvManager::SetOnQuery)
        .CbFunc(_SC("OnLost"), &PvManager::SetOnLost)
        .CbFunc(_SC("OnGained"), &PvManager::SetOnGained)
        // Raw functions
    );
*/
    RootTable(vm).Bind(_SC("SqPrivilege"), pns);
}

} // Namespace:: SqMod
