#include "Entity/Object.hpp"
#include "Base/Quaternion.hpp"
#include "Misc/Model.hpp"
#include "Core.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CModel       CObject::s_Model;

// ------------------------------------------------------------------------------------------------
Vector3      CObject::s_Vector3;
Quaternion   CObject::s_Quaternion;

// ------------------------------------------------------------------------------------------------
CObject::CObject(const Reference< CObject > & o)
    : Reference(o)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
bool CObject::IsStreamedFor(const Reference< CPlayer > & player) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        return _Func->IsObjectStreamedForPlayer(m_ID, player);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <see if object is streamed for player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <see if object is streamed for player> using an invalid reference: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
const CModel & CObject::GetModel() const
{
    // Clear any previous model
    s_Model.SetID(SQMOD_UNKNOWN);
    // Attempt to retrieve the model
    if (VALID_ENTITY(m_ID))
    {
        s_Model.SetID(_Func->GetObjectModel(m_ID));
    }
    else
    {
        LogWrn(_SC("Attempting to <get object model> using an invalid reference: %d"), m_ID);
    }
    // Return the model that could be retrieved
    return s_Model;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CObject::GetModelID() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetObjectModel(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get object model id> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CObject::GetWorld() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetObjectWorld(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get object world> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CObject::SetWorld(SQInt32 world) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetObjectWorld(m_ID, world);
    }
    else
    {
        LogWrn(_SC("Attempting to <set object world> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CObject::GetAlpha() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetObjectAlpha(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get object alpha> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CObject::SetAlpha(SQInt32 alpha) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetObjectAlpha(m_ID, alpha, 0);
    }
    else
    {
        LogWrn(_SC("Attempting to <set object alpha> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::SetAlphaEx(SQInt32 alpha, SQInt32 time) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetObjectAlpha(m_ID, alpha, time);
    }
    else
    {
        LogWrn(_SC("Attempting to <set object alpha> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveToPr(const Vector3 & pos) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->MoveObjectTo(m_ID, pos.x, pos.y, pos.z, 0);
    }
    else
    {
        LogWrn(_SC("Attempting to <move object to> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveTo(const Vector3 & pos, SQInt32 time) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->MoveObjectTo(m_ID, pos.x, pos.y, pos.z, time);
    }
    else
    {
        LogWrn(_SC("Attempting to <move object to> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveToEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->MoveObjectTo(m_ID, x, y, z, 0);
    }
    else
    {
        LogWrn(_SC("Attempting to <move object to> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveToEx(SQFloat x, SQFloat y, SQFloat z, SQInt32 time) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->MoveObjectTo(m_ID, x, y, z, time);
    }
    else
    {
        LogWrn(_SC("Attempting to <move object to> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveByPr(const Vector3 & pos) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->MoveObjectBy(m_ID, pos.x, pos.y, pos.z, 0);
    }
    else
    {
        LogWrn(_SC("Attempting to <move object by> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveBy(const Vector3 & pos, SQInt32 time) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->MoveObjectBy(m_ID, pos.x, pos.y, pos.z, time);
    }
    else
    {
        LogWrn(_SC("Attempting to <move object by> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveByEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->MoveObjectBy(m_ID, x, y, z, 0);
    }
    else
    {
        LogWrn(_SC("Attempting to <move object by> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveByEx(SQFloat x, SQFloat y, SQFloat z, SQInt32 time) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->MoveObjectBy(m_ID, x, y, z, time);
    }
    else
    {
        LogWrn(_SC("Attempting to <move object by> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CObject::GetPosition()
{
    // Clear any previous position
    s_Vector3.Clear();
    // Attempt to retrieve the position
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetObjectPos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <get object position> using an invalid reference: %d"), m_ID);
    }
    // Return the position that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CObject::SetPosition(const Vector3 & pos) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetObjectPos(m_ID, pos.x, pos.y, pos.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set object position> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::SetPositionEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetObjectPos(m_ID, x, y, z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set object position> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToPr(const Quaternion & rot) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotObjectTo(m_ID, rot.x, rot.y, rot.z, rot.w, 0);
    }
    else
    {
        LogWrn(_SC("Attempting to <rotate object to> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateTo(const Quaternion & rot, SQInt32 time) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotObjectTo(m_ID, rot.x, rot.y, rot.z, rot.w, time);
    }
    else
    {
        LogWrn(_SC("Attempting to <rotate object to> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotObjectTo(m_ID, x, y, z, w, 0);
    }
    else
    {
        LogWrn(_SC("Attempting to <rotate object to> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w, SQInt32 time) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotObjectTo(m_ID, x, y, z, w, time);
    }
    else
    {
        LogWrn(_SC("Attempting to <rotate object to> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEulerPr(const Vector3 & rot) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotObjectToEuler(m_ID, rot.x, rot.y, rot.z, 0);
    }
    else
    {
        LogWrn(_SC("Attempting to <rotate object to euler> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEuler(const Vector3 & rot, SQInt32 time) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotObjectToEuler(m_ID, rot.x, rot.y, rot.z, time);
    }
    else
    {
        LogWrn(_SC("Attempting to <rotate object to euler> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEulerEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotObjectToEuler(m_ID, x, y, z, 0);
    }
    else
    {
        LogWrn(_SC("Attempting to <rotate object to euler> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEulerEx(SQFloat x, SQFloat y, SQFloat z, SQInt32 time) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotObjectToEuler(m_ID, x, y, z, time);
    }
    else
    {
        LogWrn(_SC("Attempting to <rotate object to euler> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByPr(const Quaternion & rot) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotObjectBy(m_ID, rot.x, rot.y, rot.z, rot.w, 0);
    }
    else
    {
        LogWrn(_SC("Attempting to <rotate object by> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateBy(const Quaternion & rot, SQInt32 time) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotObjectBy(m_ID, rot.x, rot.y, rot.z, rot.w, time);
    }
    else
    {
        LogWrn(_SC("Attempting to <rotate object by> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotObjectBy(m_ID, x, y, z, w, 0);
    }
    else
    {
        LogWrn(_SC("Attempting to <rotate object by> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w, SQInt32 time) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotObjectBy(m_ID, x, y, z, w, time);
    }
    else
    {
        LogWrn(_SC("Attempting to <rotate object by> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEulerPr(const Vector3 & rot) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotObjectByEuler(m_ID, rot.x, rot.y, rot.z, 0);
    }
    else
    {
        LogWrn(_SC("Attempting to <rotate object by euler> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEuler(const Vector3 & rot, SQInt32 time) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotObjectByEuler(m_ID, rot.x, rot.y, rot.z, time);
    }
    else
    {
        LogWrn(_SC("Attempting to <rotate object by euler> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEulerEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotObjectByEuler(m_ID, x, y, z, 0);
    }
    else
    {
        LogWrn(_SC("Attempting to <rotate object by euler> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEulerEx(SQFloat x, SQFloat y, SQFloat z, SQInt32 time) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotObjectByEuler(m_ID, x, y, z, time);
    }
    else
    {
        LogWrn(_SC("Attempting to <rotate object by euler> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const Quaternion & CObject::GetRotation()
{
    // Clear any previous rotation
    s_Quaternion.Clear();
    // Attempt to retrieve the rotation
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetObjectRot(m_ID, &s_Quaternion.x, &s_Quaternion.y, &s_Quaternion.z, &s_Quaternion.w);
    }
    else
    {
        LogWrn(_SC("Attempting to <get object rotation> using an invalid reference: %d"), m_ID);
    }
    // Return the rotation that could be retrieved
    return s_Quaternion;
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CObject::GetRotationEuler()
{
    // Clear any previous rotation
    s_Vector3.Clear();
    // Attempt to retrieve the rotation
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetObjectRotEuler(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <get object euler rotation> using an invalid reference: %d"), m_ID);
    }
    // Return the rotation that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
bool CObject::GetShotReport() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->IsObjectShotReport(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get object shot report> using an invalid reference: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CObject::SetShotReport(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetObjectShotReport(m_ID, toggle);
    }
    else
    {
        LogWrn(_SC("Attempting to <set object shot report> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
bool CObject::GetBumpReport() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->IsObjectBumpReport(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get object bump report> using an invalid reference: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CObject::SetBumpReport(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetObjectBumpReport(m_ID, toggle);
    }
    else
    {
        LogWrn(_SC("Attempting to <set object bump report> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
Reference< CObject > CreateBaseObject_PEF(SQInt32 model, SQInt32 world,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha)
{
    return _Core->NewObject(model, world, x, y, z, alpha,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CObject > CreateBaseObject_PEF(SQInt32 model, SQInt32 world,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewObject(model, world, x, y, z, alpha,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CObject > CreateBaseObject_PCF(SQInt32 model, SQInt32 world,
                        const Vector3 & pos, SQInt32 alpha)
{
    return _Core->NewObject(model, world, pos.x, pos.y, pos.z, alpha,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CObject > CreateBaseObject_PCF(SQInt32 model, SQInt32 world,
                        const Vector3 & pos, SQInt32 alpha,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewObject(model, world, pos.x, pos.y, pos.z, alpha,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CObject > CreateBaseObject_EF(const CModel & model, SQInt32 world,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha)
{
    return _Core->NewObject(model, world, x, y, z, alpha,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CObject > CreateBaseObject_EF(const CModel & model, SQInt32 world,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewObject(model, world, x, y, z, alpha,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CObject > CreateBaseObject_CF(const CModel & model, SQInt32 world,
                        const Vector3 & pos, SQInt32 alpha)
{
    return _Core->NewObject(model, world, pos.x, pos.y, pos.z, alpha,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CObject > CreateBaseObject_CF(const CModel & model, SQInt32 world,
                        const Vector3 & pos, SQInt32 alpha,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewObject(model, world, pos.x, pos.y, pos.z, alpha,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CObject CreateObject_PEF(SQInt32 model, SQInt32 world,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha)
{
    return _Core->NewObject(model, world, x, y, z, alpha,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CObject CreateObject_PEF(SQInt32 model, SQInt32 world,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewObject(model, world, x, y, z, alpha,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CObject CreateObject_PCF(SQInt32 model, SQInt32 world,
                        const Vector3 & pos, SQInt32 alpha)
{
    return _Core->NewObject(model, world, pos.x, pos.y, pos.z, alpha,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CObject CreateObject_PCF(SQInt32 model, SQInt32 world,
                        const Vector3 & pos, SQInt32 alpha,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewObject(model, world, pos.x, pos.y, pos.z, alpha,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CObject CreateObject_EF(const CModel & model, SQInt32 world,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha)
{
    return _Core->NewObject(model, world, x, y, z, alpha,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CObject CreateObject_EF(const CModel & model, SQInt32 world,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewObject(model, world, x, y, z, alpha,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CObject CreateObject_CF(const CModel & model, SQInt32 world,
                        const Vector3 & pos, SQInt32 alpha)
{
    return _Core->NewObject(model, world, pos.x, pos.y, pos.z, alpha,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CObject CreateObject_CF(const CModel & model, SQInt32 world,
                        const Vector3 & pos, SQInt32 alpha,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewObject(model, world, pos.x, pos.y, pos.z, alpha,
                            header, payload);
}

// ================================================================================================
bool Register_CObject(HSQUIRRELVM vm)
{
    // Attempt to register the base reference type before the actual implementation
    if (!Register_Reference< CObject >(vm, _SC("BaseObject")))
    {
        LogFtl("Unable to register the base class <BaseObject> for <CObject> type");
        // Registration failed
        return false;
    }
    // Typedef the base reference type for simplicity
    typedef Reference< CObject > RefType;
    // Output debugging information
    LogDbg("Beginning registration of <CObject> type");
    // Attempt to register the actual reference that implements all of the entity functionality
    Sqrat::RootTable(vm).Bind(_SC("CObject"), Sqrat::DerivedClass< CObject, RefType >(vm, _SC("CObject"))
        /* Constructors */
        .Ctor()
        .Ctor< SQInt32 >()
        /* Properties */
        .Prop(_SC("model"), &CObject::GetModel)
        .Prop(_SC("model_id"), &CObject::GetModelID)
        .Prop(_SC("world"), &CObject::GetWorld, &CObject::SetWorld)
        .Prop(_SC("alpha"), &CObject::GetAlpha, &CObject::SetAlpha)
        .Prop(_SC("mov_to"), &CObject::MoveToPr)
        .Prop(_SC("mov_by"), &CObject::MoveByPr)
        .Prop(_SC("position"), &CObject::GetPosition, &CObject::SetPosition)
        .Prop(_SC("rot_to"), &CObject::RotateToPr)
        .Prop(_SC("rot_to_euler"), &CObject::RotateToEulerPr)
        .Prop(_SC("rot_by"), &CObject::RotateByPr)
        .Prop(_SC("rot_by_euler"), &CObject::RotateByEulerPr)
        .Prop(_SC("rotation"), &CObject::GetRotation)
        .Prop(_SC("rotation_euler"), &CObject::GetRotationEuler)
        .Prop(_SC("shot_report"), &CObject::GetShotReport, &CObject::SetShotReport)
        .Prop(_SC("bumb_report"), &CObject::GetBumpReport, &CObject::SetBumpReport)
        /* Functions */
        .Func(_SC("streamed_for"), &CObject::IsStreamedFor)
        .Func(_SC("set_alpha"), &CObject::SetAlphaEx)
        .Func(_SC("set_position"), &CObject::SetPositionEx)
        /* Overloads */
        .Overload< void (CObject::*)(const Vector3 &, SQInt32) const >
            (_SC("move_to"), &CObject::MoveTo)
        .Overload< void (CObject::*)(SQFloat, SQFloat, SQFloat) const >
            (_SC("move_to"), &CObject::MoveToEx)
        .Overload< void (CObject::*)(SQFloat, SQFloat, SQFloat, SQInt32) const >
            (_SC("move_to"), &CObject::MoveToEx)
        .Overload< void (CObject::*)(const Vector3 &, SQInt32) const >
            (_SC("move_by"), &CObject::MoveBy)
        .Overload< void (CObject::*)(SQFloat, SQFloat, SQFloat) const >
            (_SC("move_by"), &CObject::MoveByEx)
        .Overload< void (CObject::*)(SQFloat, SQFloat, SQFloat, SQInt32) const >
            (_SC("move_by"), &CObject::MoveByEx)
        .Overload< void (CObject::*)(const Quaternion &, SQInt32) const >
            (_SC("rotate_to"), &CObject::RotateTo)
        .Overload< void (CObject::*)(SQFloat, SQFloat, SQFloat, SQFloat) const >
            (_SC("rotate_to"), &CObject::RotateToEx)
        .Overload< void (CObject::*)(SQFloat, SQFloat, SQFloat, SQFloat, SQInt32) const >
            (_SC("rotate_to"), &CObject::RotateToEx)
        .Overload< void (CObject::*)(const Vector3 &, SQInt32) const >
            (_SC("rotate_to_euler"), &CObject::RotateToEuler)
        .Overload< void (CObject::*)(SQFloat, SQFloat, SQFloat) const >
            (_SC("rotate_to_euler"), &CObject::RotateToEulerEx)
        .Overload< void (CObject::*)(SQFloat, SQFloat, SQFloat, SQInt32) const >
            (_SC("rotate_to_euler"), &CObject::RotateToEulerEx)
        .Overload< void (CObject::*)(const Quaternion &, SQInt32) const >
            (_SC("rotate_by"), &CObject::RotateBy)
        .Overload< void (CObject::*)(SQFloat, SQFloat, SQFloat, SQFloat) const >
            (_SC("rotate_by"), &CObject::RotateByEx)
        .Overload< void (CObject::*)(SQFloat, SQFloat, SQFloat, SQFloat, SQInt32) const >
            (_SC("rotate_by"), &CObject::RotateByEx)
        .Overload< void (CObject::*)(const Vector3 &, SQInt32) const >
            (_SC("rotate_by_euler"), &CObject::RotateByEuler)
        .Overload< void (CObject::*)(SQFloat, SQFloat, SQFloat) const >
            (_SC("rotate_by_euler"), &CObject::RotateByEulerEx)
        .Overload< void (CObject::*)(SQFloat, SQFloat, SQFloat, SQInt32) const >
            (_SC("rotate_by_euler"), &CObject::RotateByEulerEx)
    );
    // Output debugging information
    LogDbg("Registration of <Object> type was successful");
    // Output debugging information
    LogDbg("Beginning registration of <CObject functions> type");
    // Register global functions related to this entity type
    Sqrat::RootTable(vm)
    /* Create BaseObject [P]rimitive [E]xtended [F]Full */
    .Overload< RefType (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32) >
        (_SC("CreateBaseObject_PEF"), &CreateBaseObject_PEF)
    .Overload< RefType (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBaseObject_PEF"), &CreateBaseObject_PEF)
    /* Create BaseObject [P]rimitive [C]ompact [F]ull */
    .Overload< RefType (*)(SQInt32, SQInt32, const Vector3 &, SQInt32) >
        (_SC("CreateBaseObject_PCF"), &CreateBaseObject_PCF)
    .Overload< RefType (*)(SQInt32, SQInt32, const Vector3 &, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBaseObject_PCF"), &CreateBaseObject_PCF)
    /* Create BaseObject [E]xtended [F]Full */
    .Overload< RefType (*)(const CModel &, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32) >
        (_SC("CreateBaseObject_EF"), &CreateBaseObject_EF)
    .Overload< RefType (*)(const CModel &, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBaseObject_EF"), &CreateBaseObject_EF)
    /* Create BaseObject [C]ompact [F]ull */
    .Overload< RefType (*)(const CModel &, SQInt32, const Vector3 &, SQInt32) >
        (_SC("CreateBaseObject_CF"), &CreateBaseObject_CF)
    .Overload< RefType (*)(const CModel &, SQInt32, const Vector3 &, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBaseObject_CF"), &CreateBaseObject_CF)
    /* Create CObject [P]rimitive [E]xtended [F]Full */
    .Overload< CObject (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32) >
        (_SC("CreateObject_PEF"), &CreateObject_PEF)
    .Overload< CObject (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateObject_PEF"), &CreateObject_PEF)
    /* Create CObject [P]rimitive [C]ompact [F]ull */
    .Overload< CObject (*)(SQInt32, SQInt32, const Vector3 &, SQInt32) >
        (_SC("CreateObject_PCF"), &CreateObject_PCF)
    .Overload< CObject (*)(SQInt32, SQInt32, const Vector3 &, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateObject_PCF"), &CreateObject_PCF)
    /* Create CObject [E]xtended [F]Full */
    .Overload< CObject (*)(const CModel &, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32) >
        (_SC("CreateObject_EF"), &CreateObject_EF)
    .Overload< CObject (*)(const CModel &, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateObject_EF"), &CreateObject_EF)
    /* Create CObject [C]ompact [F]ull */
    .Overload< CObject (*)(const CModel &, SQInt32, const Vector3 &, SQInt32) >
        (_SC("CreateObject_CF"), &CreateObject_CF)
    .Overload< CObject (*)(const CModel &, SQInt32, const Vector3 &, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateObject_CF"), &CreateObject_CF);
    // Output debugging information
    LogDbg("Registration of <Object functions> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
