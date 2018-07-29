// ------------------------------------------------------------------------------------------------
#include "Entity/Keybind.hpp"
#include "Core.hpp"
#include "Misc/Tasks.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("SqKeybind"))

// ------------------------------------------------------------------------------------------------
const Int32 CKeybind::Max = SQMOD_KEYBIND_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CKeybind::SqGetNull(HSQUIRRELVM vm)
{
    sq_pushobject(vm, Core::Get().GetNullKeybind().GetObject());
    return 1;
}

// ------------------------------------------------------------------------------------------------
LightObj & CKeybind::GetNull()
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
void CKeybind::SetTag(const StackStrF & tag)
{
    if (tag.mLen > 0)
    {
        m_Tag.assign(tag.mPtr, tag.mLen);
    }
    else
    {
        m_Tag.clear();
    }
}

// ------------------------------------------------------------------------------------------------
CKeybind & CKeybind::ApplyTag(const StackStrF & tag)
{
    SetTag(tag);
    return *this;
}

// ------------------------------------------------------------------------------------------------
LightObj & CKeybind::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CKeybind::SetData(LightObj & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CKeybind::Destroy(Int32 header, LightObj & payload)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return Core::Get().DelKeybind(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
LightObj & CKeybind::GetEvents() const
{
    // Validate the managed identifier
    Validate();
    // Return the associated event table
    return Core::Get().GetKeybind(m_ID).mEvents;
}

// ------------------------------------------------------------------------------------------------
void CKeybind::CustomEvent(Int32 header, LightObj & payload) const
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
static LightObj & Keybind_CreateEx(Int32 slot, bool release, Int32 primary, Int32 secondary,
                                Int32 alternative)
{
    return Core::Get().NewKeybind(slot, release, primary, secondary, alternative,
                            SQMOD_CREATE_DEFAULT, NullLightObj());
}

static LightObj & Keybind_CreateEx(Int32 slot, bool release, Int32 primary, Int32 secondary,
                            Int32 alternative, Int32 header, LightObj & payload)
{
    return Core::Get().NewKeybind(slot, release, primary, secondary, alternative, header, payload);
}

// ------------------------------------------------------------------------------------------------
static LightObj & Keybind_Create(bool release, Int32 primary, Int32 secondary, Int32 alternative)
{
    return Core::Get().NewKeybind(-1, release, primary, secondary, alternative,
                            SQMOD_CREATE_DEFAULT, NullLightObj());
}

static LightObj & Keybind_Create(bool release, Int32 primary, Int32 secondary, Int32 alternative,
                            Int32 header, LightObj & payload)
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
    RootTable(vm).Bind(Typename::Str,
        Class< CKeybind, NoConstructor< CKeybind > >(vm, Typename::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &CKeybind::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CKeybind::Max)
        // Core Properties
        .Prop(_SC("On"), &CKeybind::GetEvents)
        .Prop(_SC("ID"), &CKeybind::GetID)
        .Prop(_SC("Tag"), &CKeybind::GetTag, &CKeybind::SetTag)
        .Prop(_SC("Data"), &CKeybind::GetData, &CKeybind::SetData)
        .Prop(_SC("Active"), &CKeybind::IsActive)
        // Core Methods
        .FmtFunc(_SC("SetTag"), &CKeybind::ApplyTag)
        .Func(_SC("CustomEvent"), &CKeybind::CustomEvent)
        // Core Overloads
        .Overload< bool (CKeybind::*)(void) >(_SC("Destroy"), &CKeybind::Destroy)
        .Overload< bool (CKeybind::*)(Int32) >(_SC("Destroy"), &CKeybind::Destroy)
        .Overload< bool (CKeybind::*)(Int32, LightObj &) >(_SC("Destroy"), &CKeybind::Destroy)
        // Properties
        .Prop(_SC("First"), &CKeybind::GetFirst)
        .Prop(_SC("Second"), &CKeybind::GetSecond)
        .Prop(_SC("Third"), &CKeybind::GetThird)
        .Prop(_SC("Release"), &CKeybind::IsRelease)
        // Static Functions
        .StaticFunc(_SC("UnusedSlot"), &Keybind_UnusedSlot)
        // Static Overloads
        .StaticOverload< LightObj & (*)(Int32, bool, Int32, Int32, Int32) >
            (_SC("CreateEx"), &Keybind_CreateEx)
        .StaticOverload< LightObj & (*)(Int32, bool, Int32, Int32, Int32, Int32, LightObj &) >
            (_SC("CreateEx"), &Keybind_CreateEx)
        .StaticOverload< LightObj & (*)(bool, Int32, Int32, Int32) >
            (_SC("Create"), &Keybind_Create)
        .StaticOverload< LightObj & (*)(bool, Int32, Int32, Int32, Int32, LightObj &) >
            (_SC("Create"), &Keybind_Create)
        // Raw Squirrel Methods
        .SquirrelFunc(_SC("NullInst"), &CKeybind::SqGetNull)
        .SquirrelFunc(_SC("MakeTask"), &Tasks::MakeTask< CKeybind, ENT_KEYBIND >)
        .SquirrelFunc(_SC("DropTask"), &Tasks::DropTask< CKeybind, ENT_KEYBIND >)
        .SquirrelFunc(_SC("DoesTask"), &Tasks::DoesTask< CKeybind, ENT_KEYBIND >)
        .SquirrelFunc(_SC("FindTask"), &Tasks::FindTask< CKeybind, ENT_KEYBIND >)
    );
}

} // Namespace:: SqMod
