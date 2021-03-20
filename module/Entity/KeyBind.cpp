// ------------------------------------------------------------------------------------------------
#include "Entity/KeyBind.hpp"
#include "Core.hpp"
#include "Core/Tasks.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("SqKeyBind"))

// ------------------------------------------------------------------------------------------------
const int32_t CKeyBind::Max = SQMOD_KEYBIND_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CKeyBind::SqGetNull(HSQUIRRELVM vm)
{
    sq_pushobject(vm, Core::Get().GetNullKeyBind().GetObj());
    return 1;
}

// ------------------------------------------------------------------------------------------------
LightObj & CKeyBind::GetNull()
{
    return Core::Get().GetNullKeyBind();
}

// ------------------------------------------------------------------------------------------------
CKeyBind::CKeyBind(int32_t id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_KEYBIND_POOL))
    , m_Tag(fmt::format("{}", id))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
const String & CKeyBind::ToString() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
const String & CKeyBind::GetTag() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
void CKeyBind::SetTag(StackStrF & tag)
{
    if (tag.mLen > 0)
    {
        m_Tag.assign(tag.mPtr, static_cast< size_t >(tag.mLen));
    }
    else
    {
        m_Tag.clear();
    }
}

// ------------------------------------------------------------------------------------------------
CKeyBind & CKeyBind::ApplyTag(StackStrF & tag)
{
    SetTag(tag);
    return *this;
}

// ------------------------------------------------------------------------------------------------
LightObj & CKeyBind::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CKeyBind::SetData(LightObj & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CKeyBind::Destroy(int32_t header, LightObj & payload) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return Core::Get().DelKeyBind(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
LightObj & CKeyBind::GetEvents() const
{
    // Validate the managed identifier
    Validate();
    // Return the associated event table
    return Core::Get().GetKeyBind(m_ID).mEvents;
}

// ------------------------------------------------------------------------------------------------
void CKeyBind::CustomEvent(int32_t header, LightObj & payload) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested action
    Core::Get().EmitKeyBindCustom(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
int32_t CKeyBind::GetFirst() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetKeyBind(m_ID).mFirst;
}

// ------------------------------------------------------------------------------------------------
int32_t CKeyBind::GetSecond() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetKeyBind(m_ID).mSecond;
}

// ------------------------------------------------------------------------------------------------
int32_t CKeyBind::GetThird() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetKeyBind(m_ID).mThird;
}

// ------------------------------------------------------------------------------------------------
bool CKeyBind::IsRelease() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return static_cast< bool >(Core::Get().GetKeyBind(m_ID).mRelease);
}

// ------------------------------------------------------------------------------------------------
static LightObj & KeyBind_CreateEx1a(int32_t slot, bool release, int32_t primary, int32_t secondary,
                                   int32_t alternative)
{
    return Core::Get().NewKeyBind(slot, release, primary, secondary, alternative,
                                  SQMOD_CREATE_DEFAULT, NullLightObj()).mObj;
}

static LightObj & KeyBind_CreateEx1b(int32_t slot, bool release, int32_t primary, int32_t secondary,
                                   int32_t alternative, int32_t header, LightObj & payload)
{
    return Core::Get().NewKeyBind(slot, release, primary, secondary, alternative, header, payload).mObj;
}

// ------------------------------------------------------------------------------------------------
static LightObj & KeyBind_Create1a(bool release, int32_t primary, int32_t secondary, int32_t alternative)
{
    return Core::Get().NewKeyBind(-1, release, primary, secondary, alternative,
                                  SQMOD_CREATE_DEFAULT, NullLightObj()).mObj;
}

static LightObj & KeyBind_Create1b(bool release, int32_t primary, int32_t secondary, int32_t alternative,
                                 int32_t header, LightObj & payload)
{
    return Core::Get().NewKeyBind(-1, release, primary, secondary, alternative, header, payload).mObj;
}

// ------------------------------------------------------------------------------------------------
static SQInteger KeyBind_UnusedSlot()
{
    return _Func->GetKeyBindUnusedSlot();
}

// ================================================================================================
void Register_CKeyBind(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(Typename::Str,
        Class< CKeyBind, NoConstructor< CKeyBind > >(vm, Typename::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &CKeyBind::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CKeyBind::Max)
        // Core Properties
        .Prop(_SC("On"), &CKeyBind::GetEvents)
        .Prop(_SC("ID"), &CKeyBind::GetID)
        .Prop(_SC("Tag"), &CKeyBind::GetTag, &CKeyBind::SetTag)
        .Prop(_SC("Data"), &CKeyBind::GetData, &CKeyBind::SetData)
        .Prop(_SC("Active"), &CKeyBind::IsActive)
        // Core Methods
        .FmtFunc(_SC("SetTag"), &CKeyBind::ApplyTag)
        .Func(_SC("CustomEvent"), &CKeyBind::CustomEvent)
        // Core Overloads
        .Overload(_SC("Destroy"), &CKeyBind::Destroy0)
        .Overload(_SC("Destroy"), &CKeyBind::Destroy1)
        .Overload(_SC("Destroy"), &CKeyBind::Destroy)
        // Properties
        .Prop(_SC("First"), &CKeyBind::GetFirst)
        .Prop(_SC("Second"), &CKeyBind::GetSecond)
        .Prop(_SC("Third"), &CKeyBind::GetThird)
        .Prop(_SC("Release"), &CKeyBind::IsRelease)
        // Static Functions
        .StaticFunc(_SC("UnusedSlot"), &KeyBind_UnusedSlot)
        // Static Overloads
        .StaticOverload(_SC("CreateEx"), &KeyBind_CreateEx1a)
        .StaticOverload(_SC("CreateEx"), &KeyBind_CreateEx1b)
        .StaticOverload(_SC("Create"), &KeyBind_Create1a)
        .StaticOverload(_SC("Create"), &KeyBind_Create1b)
        // Raw Squirrel Methods
        .SquirrelFunc(_SC("NullInst"), &CKeyBind::SqGetNull)
        .SquirrelFunc(_SC("MakeTask"), &Tasks::MakeTask< CKeyBind, ENT_KEYBIND >)
        .SquirrelFunc(_SC("DropTask"), &Tasks::DropTask< CKeyBind, ENT_KEYBIND >)
        .SquirrelFunc(_SC("DoesTask"), &Tasks::DoesTask< CKeyBind, ENT_KEYBIND >)
        .SquirrelFunc(_SC("FindTask"), &Tasks::FindTask< CKeyBind, ENT_KEYBIND >)
    );
}

} // Namespace:: SqMod
