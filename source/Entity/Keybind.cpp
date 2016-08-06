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
SQInteger CKeybind::SqGetNull(HSQUIRRELVM vm)
{
    sq_pushobject(vm, Core::Get().GetNullKeybind().GetObject());
    return 1;
}

// ------------------------------------------------------------------------------------------------
Object & CKeybind::GetNull()
{
    return Core::Get().GetNullKeybind();
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
void CKeybind::CustomEvent(Int32 header, Object & payload) const
{
    // Validate the managed identifier
    Validate();
    // Perfrom the requested action
    Core::Get().EmitKeybindCustom(m_ID, header, payload);
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
        .Func(_SC("CustomEvent"), &CKeybind::CustomEvent)
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
        // Raw Squirrel Methods
        .SquirrelFunc(_SC("NullInst"), &CKeybind::SqGetNull)
    );
}

} // Namespace:: SqMod
