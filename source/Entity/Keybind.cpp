// ------------------------------------------------------------------------------------------------
#include "Entity/Keybind.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Int32 CKeybind::Max = SQMOD_KEYBIND_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CKeybind::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqKeybind");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
CKeybind::CKeybind(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_KEYBIND_POOL))
    , m_Tag(ToStrF("%d", id))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CKeybind::~CKeybind()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Int32 CKeybind::Cmp(const CKeybind & o) const
{
    if (m_ID == o.m_ID)
    {
        return 0;
    }
    else if (m_ID > o.m_ID)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
const String & CKeybind::ToString() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
const String & CKeybind::GetTag() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
void CKeybind::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

// ------------------------------------------------------------------------------------------------
Object & CKeybind::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CKeybind::SetData(Object & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CKeybind::Destroy(Int32 header, Object & payload)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return Core::Get().DelKeybind(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
void CKeybind::BindEvent(Int32 evid, Object & env, Function & func) const
{
    // Validate the managed identifier
    Validate();
    // Obtain the function instance called for this event
    Function & event = Core::Get().GetKeybindEvent(m_ID, evid);
    // Is the specified callback function null?
    if (func.IsNull())
    {
        event.ReleaseGently(); // Then release the current callback
    }
    // Does this function need a custom environment?
    else if (env.IsNull())
    {
        event = func;
    }
    // Assign the specified environment and function
    else
    {
        event = Function(env.GetVM(), env, func.GetFunc());
    }
}

// ------------------------------------------------------------------------------------------------
Int32 CKeybind::GetFirst() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetKeybind(m_ID).mFirst;
}

// ------------------------------------------------------------------------------------------------
Int32 CKeybind::GetSecond() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetKeybind(m_ID).mSecond;
}

// ------------------------------------------------------------------------------------------------
Int32 CKeybind::GetThird() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetKeybind(m_ID).mThird;
}

// ------------------------------------------------------------------------------------------------
bool CKeybind::IsRelease() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetKeybind(m_ID).mRelease;
}

// ------------------------------------------------------------------------------------------------
static Object & Keybind_CreateEx(Int32 slot, bool release, Int32 primary, Int32 secondary,
                                Int32 alternative)
{
    return Core::Get().NewKeybind(slot, release, primary, secondary, alternative,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & Keybind_CreateEx(Int32 slot, bool release, Int32 primary, Int32 secondary,
                            Int32 alternative, Int32 header, Object & payload)
{
    return Core::Get().NewKeybind(slot, release, primary, secondary, alternative, header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & Keybind_Create(bool release, Int32 primary, Int32 secondary, Int32 alternative)
{
    return Core::Get().NewKeybind(-1, release, primary, secondary, alternative,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & Keybind_Create(bool release, Int32 primary, Int32 secondary, Int32 alternative,
                            Int32 header, Object & payload)
{
    return Core::Get().NewKeybind(-1, release, primary, secondary, alternative, header, payload);
}

// ------------------------------------------------------------------------------------------------
static const Object & Keybind_FindByID(Int32 id)
{
    // Perform a range check on the specified identifier
    if (INVALID_ENTITYEX(id, SQMOD_KEYBIND_POOL))
    {
        STHROWF("The specified keybind identifier is invalid: %d", id);
    }
    // Obtain the ends of the entity pool
    Core::Keybinds::const_iterator itr = Core::Get().GetKeybinds().cbegin();
    Core::Keybinds::const_iterator end = Core::Get().GetKeybinds().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does the identifier match the specified one?
        if (itr->mID == id)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a keybind matching the specified identifier
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
static const Object & Keybind_FindByTag(CSStr tag)
{
    // Perform a validity check on the specified tag
    if (!tag || *tag == '\0')
    {
        STHROWF("The specified keybind tag is invalid: null/empty");
    }
    // Obtain the ends of the entity pool
    Core::Keybinds::const_iterator itr = Core::Get().GetKeybinds().cbegin();
    Core::Keybinds::const_iterator end = Core::Get().GetKeybinds().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does this entity even exist and does the tag match the specified one?
        if (itr->mInst != nullptr && itr->mInst->GetTag().compare(tag) == 0)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a keybind matching the specified tag
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
static Array Keybind_FindActive()
{
    // Remember the initial stack size
    StackGuard sg;
    // Obtain the ends of the entity pool
    Core::Keybinds::const_iterator itr = Core::Get().GetKeybinds().cbegin();
    Core::Keybinds::const_iterator end = Core::Get().GetKeybinds().cend();
    // Allocate an empty array on the stack
    sq_newarray(DefaultVM::Get(), 0);
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Is this entity instance active?
        if (VALID_ENTITY(itr->mID))
        {
            // Push the script object on the stack
            sq_pushobject(DefaultVM::Get(), (HSQOBJECT &)((*itr).mObj));
            // Append the object at the back of the array
            if (SQ_FAILED(sq_arrayappend(DefaultVM::Get(), -1)))
            {
                STHROWF("Unable to append entity instance to the list");
            }
        }
    }
    // Return the array at the top of the stack
    return Var< Array >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
static SQInteger Keybind_UnusedSlot()
{
    return _Func->GetKeyBindUnusedSlot();
}

// ================================================================================================
void Register_CKeybind(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqKeybind"),
        Class< CKeybind, NoConstructor< CKeybind > >(vm, _SC("SqKeybind"))
        // Meta-methods
        .Func(_SC("_cmp"), &CKeybind::Cmp)
        .SquirrelFunc(_SC("_typename"), &CKeybind::Typename)
        .Func(_SC("_tostring"), &CKeybind::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CKeybind::Max)
        // Core Properties
        .Prop(_SC("ID"), &CKeybind::GetID)
        .Prop(_SC("Tag"), &CKeybind::GetTag, &CKeybind::SetTag)
        .Prop(_SC("Data"), &CKeybind::GetData, &CKeybind::SetData)
        .Prop(_SC("Active"), &CKeybind::IsActive)
        // Core Methods
        .Func(_SC("Bind"), &CKeybind::BindEvent)
        // Core Overloads
        .Overload< bool (CKeybind::*)(void) >(_SC("Destroy"), &CKeybind::Destroy)
        .Overload< bool (CKeybind::*)(Int32) >(_SC("Destroy"), &CKeybind::Destroy)
        .Overload< bool (CKeybind::*)(Int32, Object &) >(_SC("Destroy"), &CKeybind::Destroy)
        // Properties
        .Prop(_SC("First"), &CKeybind::GetFirst)
        .Prop(_SC("Second"), &CKeybind::GetSecond)
        .Prop(_SC("Third"), &CKeybind::GetThird)
        .Prop(_SC("Release"), &CKeybind::IsRelease)
        // Static Functions
        .StaticFunc(_SC("FindByID"), &Keybind_FindByID)
        .StaticFunc(_SC("FindByTag"), &Keybind_FindByTag)
        .StaticFunc(_SC("FindActive"), &Keybind_FindActive)
        .StaticFunc(_SC("UnusedSlot"), &Keybind_UnusedSlot)
        // Static Overloads
        .StaticOverload< Object & (*)(Int32, bool, Int32, Int32, Int32) >
            (_SC("CreateEx"), &Keybind_CreateEx)
        .StaticOverload< Object & (*)(Int32, bool, Int32, Int32, Int32, Int32, Object &) >
            (_SC("CreateEx"), &Keybind_CreateEx)
        .StaticOverload< Object & (*)(bool, Int32, Int32, Int32) >
            (_SC("Create"), &Keybind_Create)
        .StaticOverload< Object & (*)(bool, Int32, Int32, Int32, Int32, Object &) >
            (_SC("Create"), &Keybind_Create)
    );
}

} // Namespace:: SqMod
