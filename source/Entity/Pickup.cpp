// ------------------------------------------------------------------------------------------------
#include "Entity/Pickup.hpp"
#include "Entity/Player.hpp"
#include "Base/Vector3.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Vector3     CPickup::s_Vector3;

// ------------------------------------------------------------------------------------------------
SQChar CPickup::s_StrID[SQMOD_PICKUP_POOL][8];

// ------------------------------------------------------------------------------------------------
const Int32 CPickup::Max = SQMOD_PICKUP_POOL;

// ------------------------------------------------------------------------------------------------
CPickup::CPickup(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_PICKUP_POOL))
    , m_Tag(VALID_ENTITY(m_ID) ? s_StrID[m_ID] : _SC("-1"))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CPickup::~CPickup()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Int32 CPickup::Cmp(const CPickup & o) const
{
    if (m_ID == o.m_ID)
        return 0;
    else if (m_ID > o.m_ID)
        return 1;
    else
        return -1;
}

CSStr CPickup::ToString() const
{
    return VALID_ENTITYEX(m_ID, SQMOD_PICKUP_POOL) ? s_StrID[m_ID] : _SC("-1");
}

// ------------------------------------------------------------------------------------------------
CSStr CPickup::GetTag() const
{
    return m_Tag.c_str();
}

void CPickup::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

Object & CPickup::GetData()
{
    if (Validate())
        return m_Data;
    return NullObject();
}

void CPickup::SetData(Object & data)
{
    if (Validate())
        m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CPickup::Destroy(Int32 header, Object & payload)
{
    return _Core->DelPickup(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
bool CPickup::BindEvent(Int32 evid, Object & env, Function & func) const
{
    if (!Validate())
        return false;

    Function & event = _Core->GetPickupEvent(m_ID, evid);

    if (func.IsNull())
        event.Release();
    else
        event = Function(env.GetVM(), env, func.GetFunc());

    return true;
}

// ------------------------------------------------------------------------------------------------
bool CPickup::IsStreamedFor(CPlayer & player) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        return _Func->IsPickupStreamedForPlayer(m_ID, player.GetID());
    return false;
}

Int32 CPickup::GetModel() const
{
    if (Validate())
        return _Func->PickupGetModel(m_ID);
    return -1;
}

Int32 CPickup::GetWorld() const
{
    if (Validate())
        return _Func->GetPickupWorld(m_ID);
    return -1;
}

void CPickup::SetWorld(Int32 world) const
{
    if (Validate())
        _Func->SetPickupWorld(m_ID, world);
}

Int32 CPickup::GetAlpha() const
{
    if (Validate())
        return _Func->GetVehicleModel(m_ID);
    return -1;
}

void CPickup::SetAlpha(Int32 alpha) const
{
    if (Validate())
        _Func->PickupSetAlpha(m_ID, alpha);
}

bool CPickup::GetAutomatic() const
{
    if (Validate())
        return _Func->PickupIsAutomatic(m_ID);
    return false;
}

void CPickup::SetAutomatic(bool toggle) const
{
    if (Validate())
        _Func->PickupSetAutomatic(m_ID, toggle);
}

Int32 CPickup::GetAutoTimer() const
{
    if (Validate())
        return _Func->GetPickupAutoTimer(m_ID);
    return -1;
}

void CPickup::SetAutoTimer(Int32 timer) const
{
    if (Validate())
        _Func->SetPickupAutoTimer(m_ID, timer);
}

void CPickup::Refresh() const
{
    if (Validate())
        _Func->PickupRefresh(m_ID);
}

const Vector3 & CPickup::GetPosition()
{
    s_Vector3.Clear();
    if (Validate())
        _Func->PickupGetPos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    return s_Vector3;
}

void CPickup::SetPosition(const Vector3 & pos) const
{
    if (Validate())
        _Func->PickupSetPos(m_ID, pos.x, pos.y, pos.z);
}

void CPickup::SetPositionEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->PickupSetPos(m_ID, x, y, z);
}

Int32 CPickup::GetQuantity() const
{
    if (Validate())
        return _Func->PickupGetQuantity(m_ID);
    return -1;
}

// ------------------------------------------------------------------------------------------------
Float32 CPickup::GetPosX() const
{
    s_Vector3.x = 0;
    if (Validate())
        _Func->PickupGetPos(m_ID, &s_Vector3.x, NULL, NULL);
    return s_Vector3.x;
}

Float32 CPickup::GetPosY() const
{
    s_Vector3.y = 0;
    if (Validate())
        _Func->PickupGetPos(m_ID, NULL, &s_Vector3.y, NULL);
    return s_Vector3.y;
}

Float32 CPickup::GetPosZ() const
{
    s_Vector3.z = 0;
    if (Validate())
        _Func->PickupGetPos(m_ID, NULL, NULL, &s_Vector3.z);
    return s_Vector3.z;
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetPosX(Float32 x) const
{
    if (Validate())
    {
        _Func->PickupGetPos(m_ID, NULL, &s_Vector3.y, &s_Vector3.z);
        _Func->PickupSetPos(m_ID, x, s_Vector3.y, s_Vector3.z);
    }
}

void CPickup::SetPosY(Float32 y) const
{
    if (Validate())
    {
        _Func->PickupGetPos(m_ID, &s_Vector3.x, NULL, &s_Vector3.z);
        _Func->PickupSetPos(m_ID, s_Vector3.x, y, s_Vector3.z);
    }
}

void CPickup::SetPosZ(Float32 z) const
{
    if (Validate())
    {
        _Func->PickupGetPos(m_ID, &s_Vector3.x, &s_Vector3.y, NULL);
        _Func->PickupSetPos(m_ID, s_Vector3.z, s_Vector3.y, z);
    }
}

// ------------------------------------------------------------------------------------------------
static Object & CreatePickupEx(Int32 model, Int32 world, Int32 quantity,
                        Float32 x, Float32 y, Float32 z, Int32 alpha, bool automatic)
{
    return _Core->NewPickup(model, world, quantity, x, y, z, alpha, automatic,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreatePickupEx(Int32 model, Int32 world, Int32 quantity,
                        Float32 x, Float32 y, Float32 z, Int32 alpha, bool automatic,
                        Int32 header, Object & payload)
{
    return _Core->NewPickup(model, world, quantity, x, y, z, alpha, automatic, header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & CreatePickup(Int32 model, Int32 world, Int32 quantity, const Vector3 & pos,
                            Int32 alpha, bool automatic)
{
    return _Core->NewPickup(model, world, quantity, pos.x, pos.y, pos.z, alpha, automatic,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreatePickup(Int32 model, Int32 world, Int32 quantity, const Vector3 & pos,
                            Int32 alpha, bool automatic, Int32 header, Object & payload)
{
    return _Core->NewPickup(model, world, quantity, pos.x, pos.y, pos.z, alpha, automatic,
                            header, payload);
}

// ================================================================================================
void Register_CPickup(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqPickup"),
        Class< CPickup, NoConstructor< CPickup > >(vm, _SC("SqPickup"))
        /* Metamethods */
        .Func(_SC("_cmp"), &CPickup::Cmp)
        .Func(_SC("_tostring"), &CPickup::ToString)
        /* Core Properties */
        .Prop(_SC("ID"), &CPickup::GetID)
        .Prop(_SC("Tag"), &CPickup::GetTag, &CPickup::SetTag)
        .Prop(_SC("Data"), &CPickup::GetData, &CPickup::SetData)
        .Prop(_SC("MaxID"), &CPickup::GetMaxID)
        .Prop(_SC("Active"), &CPickup::IsActive)
        /* Core Functions */
        .Func(_SC("Bind"), &CPickup::BindEvent)
        /* Core Overloads */
        .Overload< bool (CPickup::*)(void) >(_SC("Destroy"), &CPickup::Destroy)
        .Overload< bool (CPickup::*)(Int32) >(_SC("Destroy"), &CPickup::Destroy)
        .Overload< bool (CPickup::*)(Int32, Object &) >(_SC("Destroy"), &CPickup::Destroy)
        /* Properties */
        .Prop(_SC("Model"), &CPickup::GetModel)
        .Prop(_SC("World"), &CPickup::GetWorld, &CPickup::SetWorld)
        .Prop(_SC("Alpha"), &CPickup::GetAlpha, &CPickup::SetAlpha)
        .Prop(_SC("Auto"), &CPickup::GetAutomatic, &CPickup::SetAutomatic)
        .Prop(_SC("Automatic"), &CPickup::GetAutomatic, &CPickup::SetAutomatic)
        .Prop(_SC("Timer"), &CPickup::GetAutoTimer, &CPickup::SetAutoTimer)
        .Prop(_SC("Autotimer"), &CPickup::GetAutoTimer, &CPickup::SetAutoTimer)
        .Prop(_SC("Pos"), &CPickup::GetPosition, &CPickup::SetPosition)
        .Prop(_SC("Position"), &CPickup::GetPosition, &CPickup::SetPosition)
        .Prop(_SC("Quantity"), &CPickup::GetQuantity)
        .Prop(_SC("X"), &CPickup::GetPosX, &CPickup::SetPosX)
        .Prop(_SC("Y"), &CPickup::GetPosY, &CPickup::SetPosY)
        .Prop(_SC("Z"), &CPickup::GetPosZ, &CPickup::SetPosZ)
        /* Functions */
        .Func(_SC("StreamedFor"), &CPickup::IsStreamedFor)
        .Func(_SC("Refresh"), &CPickup::Refresh)
        .Func(_SC("SetPos"), &CPickup::SetPositionEx)
        .Func(_SC("SetPosition"), &CPickup::SetPositionEx)
    );

    RootTable(vm)
    .Overload< Object & (*)(Int32, Int32, Int32, Float32, Float32, Float32, Int32, bool) >
        (_SC("CreatePickupEx"), &CreatePickupEx)
    .Overload< Object & (*)(Int32, Int32, Int32, Float32, Float32, Float32, Int32, bool, Int32, Object &) >
        (_SC("CreatePickupEx"), &CreatePickupEx)
    .Overload< Object & (*)(Int32, Int32, Int32, const Vector3 &, Int32, bool) >
        (_SC("CreatePickup"), &CreatePickup)
    .Overload< Object & (*)(Int32, Int32, Int32, const Vector3 &, Int32, bool, Int32, Object &) >
        (_SC("CreatePickup"), &CreatePickup);
}

} // Namespace:: SqMod