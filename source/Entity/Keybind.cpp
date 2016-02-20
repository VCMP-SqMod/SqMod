// ------------------------------------------------------------------------------------------------
#include "Entity/Keybind.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQChar CKeybind::s_StrID[SQMOD_KEYBIND_POOL][8];

// ------------------------------------------------------------------------------------------------
const Int32 CKeybind::Max = SQMOD_KEYBIND_POOL;

// ------------------------------------------------------------------------------------------------
CKeybind::CKeybind(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_KEYBIND_POOL))
    , m_Tag(VALID_ENTITY(m_ID) ? s_StrID[m_ID] : _SC("-1"))
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
        return 0;
    else if (m_ID > o.m_ID)
        return 1;
    else
        return -1;
}

CSStr CKeybind::ToString() const
{
    return VALID_ENTITYEX(m_ID, SQMOD_KEYBIND_POOL) ? s_StrID[m_ID] : _SC("-1");
}

// ------------------------------------------------------------------------------------------------
CSStr CKeybind::GetTag() const
{
    return m_Tag.c_str();
}

void CKeybind::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

Object & CKeybind::GetData()
{
    if (Validate())
        return m_Data;
    return NullObject();
}

void CKeybind::SetData(Object & data)
{
    if (Validate())
        m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CKeybind::Destroy(Int32 header, Object & payload)
{
    return _Core->DelKeybind(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
bool CKeybind::BindEvent(Int32 evid, Object & env, Function & func) const
{
    if (!Validate())
        return false;

    Function & event = _Core->GetKeybindEvent(m_ID, evid);

    if (func.IsNull())
        event.Release();
    else
        event = Function(env.GetVM(), env, func.GetFunc());

    return true;
}

// ------------------------------------------------------------------------------------------------
Int32 CKeybind::GetPrimary() const
{
    if (Validate())
        return _Core->GetKeybind(m_ID).mPrimary;
    return -1;
}

Int32 CKeybind::GetSecondary() const
{
    if (Validate())
        return _Core->GetKeybind(m_ID).mSecondary;
    return -1;
}

Int32 CKeybind::GetAlternative() const
{
    if (Validate())
        return _Core->GetKeybind(m_ID).mAlternative;
    return -1;
}

bool CKeybind::IsRelease() const
{
    if (Validate())
        return _Core->GetKeybind(m_ID).mRelease;
    return false;
}

// ------------------------------------------------------------------------------------------------
static Object & CreateKeybindEx(Int32 slot, bool release, Int32 primary, Int32 secondary,
                                Int32 alternative)
{
    return _Core->NewKeybind(slot, release, primary, secondary, alternative,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateKeybindEx(Int32 slot, bool release, Int32 primary, Int32 secondary,
                            Int32 alternative, Int32 header, Object & payload)
{
    return _Core->NewKeybind(slot, release, primary, secondary, alternative, header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & CreateKeybind(bool release, Int32 primary, Int32 secondary, Int32 alternative)
{
    return _Core->NewKeybind(-1, release, primary, secondary, alternative,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateKeybind(bool release, Int32 primary, Int32 secondary, Int32 alternative,
                            Int32 header, Object & payload)
{
    return _Core->NewKeybind(-1, release, primary, secondary, alternative, header, payload);
}

// ================================================================================================
void Register_CKeybind(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqKeybind"),
        Class< CKeybind, NoConstructor< CKeybind > >(vm, _SC("SqKeybind"))
        /* Metamethods */
        .Func(_SC("_cmp"), &CKeybind::Cmp)
        .Func(_SC("_tostring"), &CKeybind::ToString)
        /* Core Properties */
        .Prop(_SC("ID"), &CKeybind::GetID)
        .Prop(_SC("Tag"), &CKeybind::GetTag, &CKeybind::SetTag)
        .Prop(_SC("Data"), &CKeybind::GetData, &CKeybind::SetData)
        .Prop(_SC("MaxID"), &CKeybind::GetMaxID)
        .Prop(_SC("Active"), &CKeybind::IsActive)
        /* Core Functions */
        .Func(_SC("Bind"), &CKeybind::BindEvent)
        /* Core Overloads */
        .Overload< bool (CKeybind::*)(void) >(_SC("Destroy"), &CKeybind::Destroy)
        .Overload< bool (CKeybind::*)(Int32) >(_SC("Destroy"), &CKeybind::Destroy)
        .Overload< bool (CKeybind::*)(Int32, Object &) >(_SC("Destroy"), &CKeybind::Destroy)
        /* Properties */
        .Prop(_SC("Primary"), &CKeybind::GetPrimary)
        .Prop(_SC("Secondary"), &CKeybind::GetSecondary)
        .Prop(_SC("Alternative"), &CKeybind::GetAlternative)
        .Prop(_SC("Release"), &CKeybind::IsRelease)
    );

    RootTable(vm)
    .Overload< Object & (*)(Int32, bool, Int32, Int32, Int32) >
        (_SC("CreateKeybindEx"), &CreateKeybindEx)
    .Overload< Object & (*)(Int32, bool, Int32, Int32, Int32, Int32, Object &) >
        (_SC("CreateKeybindEx"), &CreateKeybindEx)
    .Overload< Object & (*)(bool, Int32, Int32, Int32) >
        (_SC("CreateKeybind"), &CreateKeybind)
    .Overload< Object & (*)(bool, Int32, Int32, Int32, Int32, Object &) >
        (_SC("CreateKeybind"), &CreateKeybind);
}

} // Namespace:: SqMod