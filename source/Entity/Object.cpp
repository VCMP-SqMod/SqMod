// ------------------------------------------------------------------------------------------------
#include "Entity/Object.hpp"
#include "Entity/Player.hpp"
#include "Base/Quaternion.hpp"
#include "Base/Vector3.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Vector3      CObject::s_Vector3;
Quaternion   CObject::s_Quaternion;

// ------------------------------------------------------------------------------------------------
SQChar CObject::s_StrID[SQMOD_OBJECT_POOL][8];

// ------------------------------------------------------------------------------------------------
const Int32 CObject::Max = SQMOD_OBJECT_POOL;

// ------------------------------------------------------------------------------------------------
CObject::CObject(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_OBJECT_POOL))
    , m_Tag(VALID_ENTITY(m_ID) ? s_StrID[m_ID] : _SC("-1"))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CObject::~CObject()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Int32 CObject::Cmp(const CObject & o) const
{
    if (m_ID == o.m_ID)
        return 0;
    else if (m_ID > o.m_ID)
        return 1;
    else
        return -1;
}

CSStr CObject::ToString() const
{
    return VALID_ENTITYEX(m_ID, SQMOD_OBJECT_POOL) ? s_StrID[m_ID] : _SC("-1");
}

// ------------------------------------------------------------------------------------------------
CSStr CObject::GetTag() const
{
    return m_Tag.c_str();
}

void CObject::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

Object & CObject::GetData()
{
    if (Validate())
        return m_Data;
    return NullObject();
}

void CObject::SetData(Object & data)
{
    if (Validate())
        m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CObject::Destroy(Int32 header, Object & payload)
{
    return _Core->DelObject(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
bool CObject::BindEvent(Int32 evid, Object & env, Function & func) const
{
    if (!Validate())
        return false;

    Function & event = _Core->GetObjectEvent(m_ID, evid);

    if (func.IsNull())
        event.Release();
    else
        event = Function(env.GetVM(), env, func.GetFunc());

    return true;
}

// ------------------------------------------------------------------------------------------------
bool CObject::IsStreamedFor(CPlayer & player) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        return _Func->IsObjectStreamedForPlayer(m_ID, player.GetID());
    return false;
}

Int32 CObject::GetModel() const
{
    if (Validate())
        return _Func->GetObjectModel(m_ID);
    return -1;
}

Int32 CObject::GetWorld() const
{
    if (Validate())
        return _Func->GetObjectWorld(m_ID);
    return -1;
}

void CObject::SetWorld(Int32 world) const
{
    if (Validate())
        _Func->SetObjectWorld(m_ID, world);
}

Int32 CObject::GetAlpha() const
{
    if (Validate())
        return _Func->GetObjectAlpha(m_ID);
    return -1;
}

void CObject::SetAlpha(Int32 alpha) const
{
    if (Validate())
        _Func->SetObjectAlpha(m_ID, alpha, 0);
}

void CObject::SetAlphaEx(Int32 alpha, Int32 time) const
{
    if (Validate())
        _Func->SetObjectAlpha(m_ID, alpha, time);
}

void CObject::MoveTo(const Vector3 & pos, Int32 time) const
{
    if (Validate())
        _Func->MoveObjectTo(m_ID, pos.x, pos.y, pos.z, time);
}

void CObject::MoveToEx(Float32 x, Float32 y, Float32 z, Int32 time) const
{
    if (Validate())
        _Func->MoveObjectTo(m_ID, x, y, z, time);
}

void CObject::MoveBy(const Vector3 & pos, Int32 time) const
{
    if (Validate())
        _Func->MoveObjectBy(m_ID, pos.x, pos.y, pos.z, time);
}

void CObject::MoveByEx(Float32 x, Float32 y, Float32 z, Int32 time) const
{
    if (Validate())
        _Func->MoveObjectBy(m_ID, x, y, z, time);
}

const Vector3 & CObject::GetPosition()
{
    s_Vector3.Clear();
    if (Validate())
        _Func->GetObjectPos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    return s_Vector3;
}

void CObject::SetPosition(const Vector3 & pos) const
{
    if (Validate())
        _Func->SetObjectPos(m_ID, pos.x, pos.y, pos.z);
}

void CObject::SetPositionEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->SetObjectPos(m_ID, x, y, z);
}

void CObject::RotateTo(const Quaternion & rot, Int32 time) const
{
    if (Validate())
        _Func->RotObjectTo(m_ID, rot.x, rot.y, rot.z, rot.w, time);
}

void CObject::RotateToEx(Float32 x, Float32 y, Float32 z, Float32 w, Int32 time) const
{
    if (Validate())
        _Func->RotObjectTo(m_ID, x, y, z, w, time);
}

void CObject::RotateToEuler(const Vector3 & rot, Int32 time) const
{
    if (Validate())
        _Func->RotObjectToEuler(m_ID, rot.x, rot.y, rot.z, time);
}

void CObject::RotateToEulerEx(Float32 x, Float32 y, Float32 z, Int32 time) const
{
    if (Validate())
        _Func->RotObjectToEuler(m_ID, x, y, z, time);
}

void CObject::RotateBy(const Quaternion & rot, Int32 time) const
{
    if (Validate())
        _Func->RotObjectBy(m_ID, rot.x, rot.y, rot.z, rot.w, time);
}

void CObject::RotateByEx(Float32 x, Float32 y, Float32 z, Float32 w, Int32 time) const
{
    if (Validate())
        _Func->RotObjectBy(m_ID, x, y, z, w, time);
}

void CObject::RotateByEuler(const Vector3 & rot, Int32 time) const
{
    if (Validate())
        _Func->RotObjectByEuler(m_ID, rot.x, rot.y, rot.z, time);
}

void CObject::RotateByEulerEx(Float32 x, Float32 y, Float32 z, Int32 time) const
{
    if (Validate())
        _Func->RotObjectByEuler(m_ID, x, y, z, time);
}

const Quaternion & CObject::GetRotation()
{
    s_Quaternion.Clear();
    if (Validate())
        _Func->GetObjectRot(m_ID, &s_Quaternion.x, &s_Quaternion.y, &s_Quaternion.z, &s_Quaternion.w);
    return s_Quaternion;
}

const Vector3 & CObject::GetRotationEuler()
{
    s_Vector3.Clear();
    if (Validate())
        _Func->GetObjectRotEuler(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    return s_Vector3;
}

bool CObject::GetShotReport() const
{
    if (Validate())
        return _Func->IsObjectShotReport(m_ID);
    return false;
}

void CObject::SetShotReport(bool toggle) const
{
    if (Validate())
        _Func->SetObjectShotReport(m_ID, toggle);
}

bool CObject::GetBumpReport() const
{
    if (Validate())
        return _Func->IsObjectBumpReport(m_ID);
    return false;
}

void CObject::SetBumpReport(bool toggle) const
{
    if (Validate())
        _Func->SetObjectBumpReport(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetPosX() const
{
    s_Vector3.x = 0;
    if (Validate())
        _Func->GetObjectPos(m_ID, &s_Vector3.x, NULL, NULL);
    return s_Vector3.x;
}

Float32 CObject::GetPosY() const
{
    s_Vector3.y = 0;
    if (Validate())
        _Func->GetObjectPos(m_ID, NULL, &s_Vector3.y, NULL);
    return s_Vector3.y;
}

Float32 CObject::GetPosZ() const
{
    s_Vector3.z = 0;
    if (Validate())
        _Func->GetObjectPos(m_ID, NULL, NULL, &s_Vector3.z);
    return s_Vector3.z;
}

// ------------------------------------------------------------------------------------------------
void CObject::SetPosX(Float32 x) const
{
    if (Validate())
    {
        _Func->GetObjectPos(m_ID, NULL, &s_Vector3.y, &s_Vector3.z);
        _Func->SetObjectPos(m_ID, x, s_Vector3.y, s_Vector3.z);
    }
}

void CObject::SetPosY(Float32 y) const
{
    if (Validate())
    {
        _Func->GetObjectPos(m_ID, &s_Vector3.x, NULL, &s_Vector3.z);
        _Func->SetObjectPos(m_ID, s_Vector3.x, y, s_Vector3.z);
    }
}

void CObject::SetPosZ(Float32 z) const
{
    if (Validate())
    {
        _Func->GetObjectPos(m_ID, &s_Vector3.x, &s_Vector3.y, NULL);
        _Func->SetObjectPos(m_ID, s_Vector3.z, s_Vector3.y, z);
    }
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetRotX() const
{
    s_Quaternion.x = 0;
    if (Validate())
        _Func->GetObjectRot(m_ID, &s_Quaternion.x, NULL, NULL, NULL);
    return s_Quaternion.x;
}

Float32 CObject::GetRotY() const
{
    s_Quaternion.y = 0;
    if (Validate())
        _Func->GetObjectRot(m_ID, NULL, &s_Quaternion.y, NULL, NULL);
    return s_Quaternion.y;
}

Float32 CObject::GetRotZ() const
{
    s_Quaternion.z = 0;
    if (Validate())
        _Func->GetObjectRot(m_ID, NULL, NULL, &s_Quaternion.z, NULL);
    return s_Quaternion.z;
}

Float32 CObject::GetRotW() const
{
    s_Quaternion.w = 0;
    if (Validate())
        _Func->GetObjectRot(m_ID, NULL, NULL, NULL, &s_Quaternion.w);
    return s_Quaternion.w;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetERotX() const
{
    s_Vector3.x = 0;
    if (Validate())
        _Func->GetObjectRotEuler(m_ID, &s_Vector3.x, NULL, NULL);
    return s_Vector3.x;
}

Float32 CObject::GetERotY() const
{
    s_Vector3.y = 0;
    if (Validate())
        _Func->GetObjectRotEuler(m_ID, NULL, &s_Vector3.y, NULL);
    return s_Vector3.y;
}

Float32 CObject::GetERotZ() const
{
    s_Vector3.z = 0;
    if (Validate())
        _Func->GetObjectRotEuler(m_ID, NULL, NULL, &s_Vector3.z);
    return s_Vector3.z;
}

// ------------------------------------------------------------------------------------------------
static Object & CreateObjectEx(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z,
                                Int32 alpha)
{
    return _Core->NewObject(model, world, x, y, z, alpha, SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateObjectEx(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z,
                        Int32 alpha, Int32 header, Object & payload)
{
    return _Core->NewObject(model, world, x, y, z, alpha, header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & CreateObject(Int32 model, Int32 world, const Vector3 & pos, Int32 alpha)
{
    return _Core->NewObject(model, world, pos.x, pos.y, pos.z, alpha,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateObject(Int32 model, Int32 world, const Vector3 & pos, Int32 alpha,
                            Int32 header, Object & payload)
{
    return _Core->NewObject(model, world, pos.x, pos.y, pos.z, alpha, header, payload);
}

// ================================================================================================
void Register_CObject(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqObject"),
        Class< CObject, NoConstructor< CObject > >(vm, _SC("SqObject"))
        /* Metamethods */
        .Func(_SC("_cmp"), &CObject::Cmp)
        .Func(_SC("_tostring"), &CObject::ToString)
        /* Core Properties */
        .Prop(_SC("ID"), &CObject::GetID)
        .Prop(_SC("Tag"), &CObject::GetTag, &CObject::SetTag)
        .Prop(_SC("Data"), &CObject::GetData, &CObject::SetData)
        .Prop(_SC("MaxID"), &CObject::GetMaxID)
        .Prop(_SC("Active"), &CObject::IsActive)
        /* Core Functions */
        .Func(_SC("Bind"), &CObject::BindEvent)
        /* Core Overloads */
        .Overload< bool (CObject::*)(void) >(_SC("Destroy"), &CObject::Destroy)
        .Overload< bool (CObject::*)(Int32) >(_SC("Destroy"), &CObject::Destroy)
        .Overload< bool (CObject::*)(Int32, Object &) >(_SC("Destroy"), &CObject::Destroy)
        /* Properties */
        .Prop(_SC("Model"), &CObject::GetModel)
        .Prop(_SC("World"), &CObject::GetWorld, &CObject::SetWorld)
        .Prop(_SC("Alpha"), &CObject::GetAlpha, &CObject::SetAlpha)
        .Prop(_SC("Pos"), &CObject::GetPosition, &CObject::SetPosition)
        .Prop(_SC("Position"), &CObject::GetPosition, &CObject::SetPosition)
        .Prop(_SC("ERot"), &CObject::GetRotation)
        .Prop(_SC("Rotation"), &CObject::GetRotation)
        .Prop(_SC("RotationEuler"), &CObject::GetRotationEuler)
        .Prop(_SC("ShotReport"), &CObject::GetShotReport, &CObject::SetShotReport)
        .Prop(_SC("BumpReport"), &CObject::GetBumpReport, &CObject::SetBumpReport)
        .Prop(_SC("X"), &CObject::GetPosX, &CObject::SetPosX)
        .Prop(_SC("Y"), &CObject::GetPosY, &CObject::SetPosY)
        .Prop(_SC("Z"), &CObject::GetPosZ, &CObject::SetPosZ)
        .Prop(_SC("RX"), &CObject::GetRotX)
        .Prop(_SC("RY"), &CObject::GetRotY)
        .Prop(_SC("RZ"), &CObject::GetRotZ)
        .Prop(_SC("RW"), &CObject::GetRotW)
        .Prop(_SC("EX"), &CObject::GetERotX)
        .Prop(_SC("EY"), &CObject::GetERotY)
        .Prop(_SC("EZ"), &CObject::GetERotZ)
        /* Functions */
        .Func(_SC("StreamedFor"), &CObject::IsStreamedFor)
        .Func(_SC("SetAlpha"), &CObject::SetAlphaEx)
        .Func(_SC("SetPosition"), &CObject::SetPositionEx)
        /* Overloads */
        .Overload< void (CObject::*)(const Vector3 &, Int32) const >
            (_SC("MoveTo"), &CObject::MoveTo)
        .Overload< void (CObject::*)(Float32, Float32, Float32, Int32) const >
            (_SC("MoveTo"), &CObject::MoveToEx)
        .Overload< void (CObject::*)(const Vector3 &, Int32) const >
            (_SC("MoveBy"), &CObject::MoveBy)
        .Overload< void (CObject::*)(Float32, Float32, Float32, Int32) const >
            (_SC("MoveBy"), &CObject::MoveByEx)
        .Overload< void (CObject::*)(const Quaternion &, Int32) const >
            (_SC("RotateTo"), &CObject::RotateTo)
        .Overload< void (CObject::*)(Float32, Float32, Float32, Float32, Int32) const >
            (_SC("RotateTo"), &CObject::RotateToEx)
        .Overload< void (CObject::*)(const Vector3 &, Int32) const >
            (_SC("RotateToEuler"), &CObject::RotateToEuler)
        .Overload< void (CObject::*)(Float32, Float32, Float32, Int32) const >
            (_SC("RotateToEuler"), &CObject::RotateToEulerEx)
        .Overload< void (CObject::*)(const Quaternion &, Int32) const >
            (_SC("RotateBy"), &CObject::RotateBy)
        .Overload< void (CObject::*)(Float32, Float32, Float32, Float32, Int32) const >
            (_SC("RotateBy"), &CObject::RotateByEx)
        .Overload< void (CObject::*)(const Vector3 &, Int32) const >
            (_SC("RotateByEuler"), &CObject::RotateByEuler)
        .Overload< void (CObject::*)(Float32, Float32, Float32, Int32) const >
            (_SC("RotateByEuler"), &CObject::RotateByEulerEx)
    );

    RootTable(vm)
    .Overload< Object & (*)(Int32, Int32, Float32, Float32, Float32, Int32) >
        (_SC("CreateObjectEx"), &CreateObjectEx)
    .Overload< Object & (*)(Int32, Int32, Float32, Float32, Float32, Int32, Int32, Object &) >
        (_SC("CreateObjectEx"), &CreateObjectEx)
    .Overload< Object & (*)(Int32, Int32, const Vector3 &, Int32) >
        (_SC("CreateObject"), &CreateObject)
    .Overload< Object & (*)(Int32, Int32, const Vector3 &, Int32, Int32, Object &) >
        (_SC("CreateObject"), &CreateObject);
}

} // Namespace:: SqMod