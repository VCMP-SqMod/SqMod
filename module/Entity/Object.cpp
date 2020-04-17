// ------------------------------------------------------------------------------------------------
#include "Entity/Object.hpp"
#include "Entity/Player.hpp"
#include "Base/Quaternion.hpp"
#include "Base/Vector3.hpp"
#include "Core.hpp"
#include "Misc/Tasks.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("SqObject"))

// ------------------------------------------------------------------------------------------------
const Int32 CObject::Max = SQMOD_OBJECT_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CObject::SqGetNull(HSQUIRRELVM vm)
{
    sq_pushobject(vm, Core::Get().GetNullObject().GetObj());
    return 1;
}

// ------------------------------------------------------------------------------------------------
LightObj & CObject::GetNull()
{
    return Core::Get().GetNullObject();
}

// ------------------------------------------------------------------------------------------------
CObject::CObject(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_OBJECT_POOL))
    , m_Tag(ToStrF("%d", id)), m_Data(), m_CircularLocks(0)
    , mMoveToDuration(0)
    , mMoveByDuration(0)
    , mRotateToDuration(0)
    , mRotateByDuration(0)
    , mRotateToEulerDuration(0)
    , mRotateByEulerDuration(0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
const String & CObject::ToString() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
const String & CObject::GetTag() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
void CObject::SetTag(StackStrF & tag)
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
CObject & CObject::ApplyTag(StackStrF & tag)
{
    SetTag(tag);
    return *this;
}

// ------------------------------------------------------------------------------------------------
LightObj & CObject::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CObject::SetData(LightObj & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CObject::Destroy(Int32 header, LightObj & payload)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return Core::Get().DelObject(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
LightObj & CObject::GetEvents() const
{
    // Validate the managed identifier
    Validate();
    // Return the associated event table
    return Core::Get().GetObj(m_ID).mEvents;
}

// ------------------------------------------------------------------------------------------------
void CObject::CustomEvent(Int32 header, LightObj & payload) const
{
    // Validate the managed identifier
    Validate();
    // Perfrom the requested action
    Core::Get().EmitObjectCustom(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
bool CObject::IsStreamedFor(CPlayer & player) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsObjectStreamedForPlayer(m_ID, player.GetID());
}

// ------------------------------------------------------------------------------------------------
Int32 CObject::GetModel() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetObjectModel(m_ID);
}

// ------------------------------------------------------------------------------------------------
Int32 CObject::GetWorld() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetObjectWorld(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetWorld(Int32 world)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const Int32 current = _Func->GetObjectWorld(m_ID);
    // Don't even bother if it's the same value
    if (current == world)
    {
        return;
    }
    // Avoid property unwind from a recursive call
    _Func->SetObjectWorld(m_ID, world);
    // Avoid infinite recursive event loops
    if (!(m_CircularLocks & OBJECTCL_EMIT_OBJECT_WORLD))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, OBJECTCL_EMIT_OBJECT_WORLD);
        // Now forward the event call
        Core::Get().EmitObjectWorld(m_ID, current, world);
    }
}

// ------------------------------------------------------------------------------------------------
Int32 CObject::GetAlpha() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetObjectAlpha(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetAlpha(Int32 alpha)
{
    SetAlphaEx(alpha, 0);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetAlphaEx(Int32 alpha, Uint32 time)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const Int32 current = _Func->GetObjectAlpha(m_ID);
    // Don't even bother if it's the same value
    if (current == alpha)
    {
        return;
    }
    // Avoid property unwind from a recursive call
    _Func->SetObjectAlpha(m_ID, alpha, time);
    // Avoid infinite recursive event loops
    if (!(m_CircularLocks & OBJECTCL_EMIT_OBJECT_ALPHA))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, OBJECTCL_EMIT_OBJECT_ALPHA);
        // Now forward the event call
        Core::Get().EmitObjectAlpha(m_ID, current, alpha, time);
    }
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveTo(const Vector3 & pos, Uint32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectTo(m_ID, pos.x, pos.y, pos.z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveToEx(Float32 x, Float32 y, Float32 z, Uint32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectTo(m_ID, x, y, z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveBy(const Vector3 & pos, Uint32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectBy(m_ID, pos.x, pos.y, pos.z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveByEx(Float32 x, Float32 y, Float32 z, Uint32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectBy(m_ID, x, y, z, time);
}

// ------------------------------------------------------------------------------------------------
Vector3 CObject::GetPosition()
{
    // Validate the managed identifier
    Validate();
    // Create a default vector instance
    Vector3 vec;
    // Query the server for the values
    _Func->GetObjectPosition(m_ID, &vec.x, &vec.y, &vec.z);
    // Return the requested information
    return vec;
}

// ------------------------------------------------------------------------------------------------
void CObject::SetPosition(const Vector3 & pos) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetObjectPosition(m_ID, pos.x, pos.y, pos.z);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetPositionEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetObjectPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateTo(const Quaternion & rot, Uint32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectTo(m_ID, rot.x, rot.y, rot.z, rot.w, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEx(Float32 x, Float32 y, Float32 z, Float32 w, Uint32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectTo(m_ID, x, y, z, w, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEuler(const Vector3 & rot, Uint32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectToEuler(m_ID, rot.x, rot.y, rot.z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEulerEx(Float32 x, Float32 y, Float32 z, Uint32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectToEuler(m_ID, x, y, z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateBy(const Quaternion & rot, Uint32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectBy(m_ID, rot.x, rot.y, rot.z, rot.w, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEx(Float32 x, Float32 y, Float32 z, Float32 w, Uint32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectBy(m_ID, x, y, z, w, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEuler(const Vector3 & rot, Uint32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectByEuler(m_ID, rot.x, rot.y, rot.z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEulerEx(Float32 x, Float32 y, Float32 z, Uint32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectByEuler(m_ID, x, y, z, time);
}

// ------------------------------------------------------------------------------------------------
Quaternion CObject::GetRotation()
{
    // Validate the managed identifier
    Validate();
    // Create a default quaternion instance
    Quaternion quat;
    // Query the server for the values
    _Func->GetObjectRotation(m_ID, &quat.x, &quat.y, &quat.z, &quat.w);
    // Return the requested information
    return quat;
}

// ------------------------------------------------------------------------------------------------
Vector3 CObject::GetRotationEuler()
{
    // Validate the managed identifier
    Validate();
    // Create a default vector instance
    Vector3 vec;
    // Query the server for the values
    _Func->GetObjectRotationEuler(m_ID, &vec.x, &vec.y, &vec.z);
    // Return the requested information
    return vec;
}

// ------------------------------------------------------------------------------------------------
bool CObject::GetShotReport() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsObjectShotReportEnabled(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetShotReport(bool toggle)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const bool current = _Func->IsObjectShotReportEnabled(m_ID);
    // Don't even bother if it's the same value
    if (current == toggle)
    {
        return;
    }
    // Avoid property unwind from a recursive call
    _Func->SetObjectShotReportEnabled(m_ID, static_cast< uint8_t >(toggle));
    // Avoid infinite recursive event loops
    if (!(m_CircularLocks & OBJECTCL_EMIT_OBJECT_REPORT))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, OBJECTCL_EMIT_OBJECT_REPORT);
        // Now forward the event call
        Core::Get().EmitObjectReport(m_ID, current, toggle, false);
    }
}

// ------------------------------------------------------------------------------------------------
bool CObject::GetTouchedReport() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsObjectTouchedReportEnabled(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetTouchedReport(bool toggle)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const bool current = _Func->IsObjectTouchedReportEnabled(m_ID);
    // Don't even bother if it's the same value
    if (current == toggle)
    {
        return;
    }
    // Avoid property unwind from a recursive call
    _Func->SetObjectTouchedReportEnabled(m_ID, static_cast< uint8_t >(toggle));
    // Avoid infinite recursive event loops
    if (!(m_CircularLocks & OBJECTCL_EMIT_OBJECT_REPORT))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, OBJECTCL_EMIT_OBJECT_REPORT);
        // Now forward the event call
        Core::Get().EmitObjectReport(m_ID, current, toggle, true);
    }
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetPositionX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    Float32 x = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectPosition(m_ID, &x, &dummy, &dummy);
    // Return the requested information
    return x;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetPositionY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    Float32 y = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectPosition(m_ID, &dummy, &y, &dummy);
    // Return the requested information
    return y;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetPositionZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    Float32 z = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectPosition(m_ID, &dummy, &dummy, &z);
    // Return the requested information
    return z;
}

// ------------------------------------------------------------------------------------------------
void CObject::SetPositionX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    Float32 y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectPosition(m_ID, &dummy, &y, &z);
    // Perform the requested operation
    _Func->SetObjectPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetPositionY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    Float32 x, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectPosition(m_ID, &x, &dummy, &z);
    // Perform the requested operation
    _Func->SetObjectPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetPositionZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    Float32 x, y, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectPosition(m_ID, &x, &y, &dummy);
    // Perform the requested operation
    _Func->SetObjectPosition(m_ID, z, y, z);
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetRotationX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    Float32 x = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectRotation(m_ID, &x, &dummy, &dummy, &dummy);
    // Return the requested information
    return x;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetRotationY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    Float32 y = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectRotation(m_ID, &dummy, &y, &dummy, &dummy);
    // Return the requested information
    return y;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetRotationZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    Float32 z = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectRotation(m_ID, &dummy, &dummy, &z, &dummy);
    // Return the requested information
    return z;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetRotationW() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    Float32 w = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectRotation(m_ID, &dummy, &dummy, &dummy, &w);
    // Return the requested information
    return w;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetEulerRotationX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    Float32 x = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectRotationEuler(m_ID, &x, &dummy, &dummy);
    // Return the requested information
    return x;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetEulerRotationY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    Float32 y = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectRotationEuler(m_ID, &dummy, &y, &dummy);
    // Return the requested information
    return y;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetEulerRotationZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    Float32 z = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectRotationEuler(m_ID, &dummy, &dummy, &z);
    // Return the requested information
    return z;
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveToX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    Float32 y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectPosition(m_ID, &dummy, &y, &z);
    // Perform the requested operation
    _Func->MoveObjectTo(m_ID, x, y, z, mMoveToDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveToY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    Float32 x, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectPosition(m_ID, &x, &dummy, &z);
    // Perform the requested operation
    _Func->MoveObjectTo(m_ID, x, y, z, mMoveToDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveToZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    Float32 x, y, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectPosition(m_ID, &x, &y, &dummy);
    // Perform the requested operation
    _Func->MoveObjectTo(m_ID, z, y, z, mMoveToDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveByX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectBy(m_ID, x, 0.0f, 0.0f, mMoveByDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveByY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectBy(m_ID, 0.0f, y, 0.0f, mMoveByDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveByZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectBy(m_ID, 0.0f, 0.0f, z, mMoveByDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    Float32 y, z, w, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectRotation(m_ID, &dummy, &y, &z, &w);
    // Perform the requested operation
    _Func->RotateObjectTo(m_ID, x, y, z, w, mRotateToDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    Float32 x, z, w, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectRotation(m_ID, &x, &dummy, &z, &w);
    // Perform the requested operation
    _Func->RotateObjectTo(m_ID, x, y, z, w, mRotateToDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    Float32 x, y, w, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectRotation(m_ID, &x, &y, &dummy, &w);
    // Perform the requested operation
    _Func->RotateObjectTo(m_ID, x, y, z, w, mRotateToDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToW(Float32 w) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    Float32 x, y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectRotation(m_ID, &x, &y, &z, &dummy);
    // Perform the requested operation
    _Func->RotateObjectTo(m_ID, x, y, z, w, mRotateToDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectBy(m_ID, x, 0.0f, 0.0f, 0.0f, mRotateByDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectBy(m_ID, 0.0f, y, 0.0f, 0.0f, mRotateByDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectBy(m_ID, 0.0f, 0.0f, z, 0.0f, mRotateByDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByW(Float32 w) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectBy(m_ID, 0.0f, 0.0f, 0.0f, w, mRotateByDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEulerX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    Float32 y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectRotationEuler(m_ID, &dummy, &y, &z);
    // Perform the requested operation
    _Func->RotateObjectToEuler(m_ID, x, y, z, mRotateToEulerDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEulerY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    Float32 x, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectRotationEuler(m_ID, &x, &dummy, &z);
    // Perform the requested operation
    _Func->RotateObjectToEuler(m_ID, x, y, z, mRotateToEulerDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEulerZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    Float32 x, y, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectRotationEuler(m_ID, &x, &y, &dummy);
    // Perform the requested operation
    _Func->RotateObjectToEuler(m_ID, z, y, z, mRotateToEulerDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEulerX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectByEuler(m_ID, x, 0.0f, 0.0f, mRotateByEulerDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEulerY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectByEuler(m_ID, 0.0f, y, 0.0f, mRotateByEulerDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEulerZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectByEuler(m_ID, 0.0f, 0.0f, z, mRotateByEulerDuration);
}

// ------------------------------------------------------------------------------------------------
static LightObj & Object_CreateEx(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z,
                                Int32 alpha)
{
    return Core::Get().NewObject(model, world, x, y, z, alpha, SQMOD_CREATE_DEFAULT, NullLightObj());
}

static LightObj & Object_CreateEx(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z,
                        Int32 alpha, Int32 header, LightObj & payload)
{
    return Core::Get().NewObject(model, world, x, y, z, alpha, header, payload);
}

// ------------------------------------------------------------------------------------------------
static LightObj & Object_Create(Int32 model, Int32 world, const Vector3 & pos, Int32 alpha)
{
    return Core::Get().NewObject(model, world, pos.x, pos.y, pos.z, alpha,
                            SQMOD_CREATE_DEFAULT, NullLightObj());
}

static LightObj & Object_Create(Int32 model, Int32 world, const Vector3 & pos, Int32 alpha,
                            Int32 header, LightObj & payload)
{
    return Core::Get().NewObject(model, world, pos.x, pos.y, pos.z, alpha, header, payload);
}

// ================================================================================================
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void Register_CObject(HSQUIRRELVM vm)
#pragma clang diagnostic pop
{
    RootTable(vm).Bind(Typename::Str,
        Class< CObject, NoConstructor< CObject > >(vm, Typename::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &CObject::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CObject::Max)
        // Member Variables
        .Var(_SC("MoveToDuration"), &CObject::mMoveToDuration)
        .Var(_SC("MoveByDuration"), &CObject::mMoveByDuration)
        .Var(_SC("RotateToDuration"), &CObject::mRotateToDuration)
        .Var(_SC("RotateByDuration"), &CObject::mRotateByDuration)
        .Var(_SC("RotateToEulerDuration"), &CObject::mRotateToEulerDuration)
        .Var(_SC("RotateByEulerDuration"), &CObject::mRotateByEulerDuration)
        // Core Properties
        .Prop(_SC("On"), &CObject::GetEvents)
        .Prop(_SC("ID"), &CObject::GetID)
        .Prop(_SC("Tag"), &CObject::GetTag, &CObject::SetTag)
        .Prop(_SC("Data"), &CObject::GetData, &CObject::SetData)
        .Prop(_SC("Active"), &CObject::IsActive)
        // Core Methods
        .FmtFunc(_SC("SetTag"), &CObject::ApplyTag)
        .Func(_SC("CustomEvent"), &CObject::CustomEvent)
        // Core Overloads
        .Overload< bool (CObject::*)(void) >(_SC("Destroy"), &CObject::Destroy)
        .Overload< bool (CObject::*)(Int32) >(_SC("Destroy"), &CObject::Destroy)
        .Overload< bool (CObject::*)(Int32, LightObj &) >(_SC("Destroy"), &CObject::Destroy)
        // Properties
        .Prop(_SC("Model"), &CObject::GetModel)
        .Prop(_SC("World"), &CObject::GetWorld, &CObject::SetWorld)
        .Prop(_SC("Alpha"), &CObject::GetAlpha, &CObject::SetAlpha)
        .Prop(_SC("Pos"), &CObject::GetPosition, &CObject::SetPosition)
        .Prop(_SC("Position"), &CObject::GetPosition, &CObject::SetPosition)
        .Prop(_SC("Rot"), &CObject::GetRotation)
        .Prop(_SC("Rotation"), &CObject::GetRotation)
        .Prop(_SC("EulerRot"), &CObject::GetRotationEuler)
        .Prop(_SC("EulerRotation"), &CObject::GetRotationEuler)
        .Prop(_SC("ShotReport"), &CObject::GetShotReport, &CObject::SetShotReport)
        .Prop(_SC("BumpReport"), &CObject::GetTouchedReport, &CObject::SetTouchedReport)
        .Prop(_SC("TouchedReport"), &CObject::GetTouchedReport, &CObject::SetTouchedReport)
        .Prop(_SC("PosX"), &CObject::GetPositionX, &CObject::SetPositionX)
        .Prop(_SC("PosY"), &CObject::GetPositionY, &CObject::SetPositionY)
        .Prop(_SC("PosZ"), &CObject::GetPositionZ, &CObject::SetPositionZ)
        .Prop(_SC("RotX"), &CObject::GetRotationX)
        .Prop(_SC("RotY"), &CObject::GetRotationY)
        .Prop(_SC("RotZ"), &CObject::GetRotationZ)
        .Prop(_SC("RotW"), &CObject::GetRotationW)
        .Prop(_SC("EulerRotX"), &CObject::GetEulerRotationX)
        .Prop(_SC("EulerRotY"), &CObject::GetEulerRotationY)
        .Prop(_SC("EulerRotZ"), &CObject::GetEulerRotationZ)
        .Prop(_SC("MoveToX"), &CObject::GetPositionX, &CObject::MoveToX)
        .Prop(_SC("MoveToY"), &CObject::GetPositionY, &CObject::MoveToY)
        .Prop(_SC("MoveToZ"), &CObject::GetPositionZ, &CObject::MoveToZ)
        .Prop(_SC("MoveByX"), &CObject::GetPositionX, &CObject::MoveByX)
        .Prop(_SC("MoveByY"), &CObject::GetPositionY, &CObject::MoveByY)
        .Prop(_SC("MoveByZ"), &CObject::GetPositionZ, &CObject::MoveByZ)
        .Prop(_SC("RotateToX"), &CObject::GetRotationX, &CObject::RotateToX)
        .Prop(_SC("RotateToY"), &CObject::GetRotationY, &CObject::RotateToY)
        .Prop(_SC("RotateToZ"), &CObject::GetRotationZ, &CObject::RotateToZ)
        .Prop(_SC("RotateToW"), &CObject::GetRotationW, &CObject::RotateToW)
        .Prop(_SC("RotateByX"), &CObject::GetRotationX, &CObject::RotateByX)
        .Prop(_SC("RotateByY"), &CObject::GetRotationY, &CObject::RotateByY)
        .Prop(_SC("RotateByZ"), &CObject::GetRotationZ, &CObject::RotateByZ)
        .Prop(_SC("RotateByW"), &CObject::GetRotationW, &CObject::RotateByW)
        .Prop(_SC("RotateToEulerX"), &CObject::GetEulerRotationX, &CObject::RotateToEulerX)
        .Prop(_SC("RotateToEulerY"), &CObject::GetEulerRotationY, &CObject::RotateToEulerY)
        .Prop(_SC("RotateToEulerZ"), &CObject::GetEulerRotationZ, &CObject::RotateToEulerZ)
        .Prop(_SC("RotateByEulerX"), &CObject::GetEulerRotationX, &CObject::RotateByEulerX)
        .Prop(_SC("RotateByEulerY"), &CObject::GetEulerRotationY, &CObject::RotateByEulerY)
        .Prop(_SC("RotateByEulerZ"), &CObject::GetEulerRotationZ, &CObject::RotateByEulerZ)
        // Member Methods
        .Func(_SC("StreamedFor"), &CObject::IsStreamedFor)
        .Func(_SC("SetAlpha"), &CObject::SetAlphaEx)
        .Func(_SC("SetPosition"), &CObject::SetPositionEx)
        // Member Overloads
        .Overload< void (CObject::*)(const Vector3 &, Uint32) const >
            (_SC("MoveTo"), &CObject::MoveTo)
        .Overload< void (CObject::*)(Float32, Float32, Float32, Uint32) const >
            (_SC("MoveTo"), &CObject::MoveToEx)
        .Overload< void (CObject::*)(const Vector3 &, Uint32) const >
            (_SC("MoveBy"), &CObject::MoveBy)
        .Overload< void (CObject::*)(Float32, Float32, Float32, Uint32) const >
            (_SC("MoveBy"), &CObject::MoveByEx)
        .Overload< void (CObject::*)(const Quaternion &, Uint32) const >
            (_SC("RotateTo"), &CObject::RotateTo)
        .Overload< void (CObject::*)(Float32, Float32, Float32, Float32, Uint32) const >
            (_SC("RotateTo"), &CObject::RotateToEx)
        .Overload< void (CObject::*)(const Vector3 &, Uint32) const >
            (_SC("RotateToEuler"), &CObject::RotateToEuler)
        .Overload< void (CObject::*)(Float32, Float32, Float32, Uint32) const >
            (_SC("RotateToEuler"), &CObject::RotateToEulerEx)
        .Overload< void (CObject::*)(const Quaternion &, Uint32) const >
            (_SC("RotateBy"), &CObject::RotateBy)
        .Overload< void (CObject::*)(Float32, Float32, Float32, Float32, Uint32) const >
            (_SC("RotateBy"), &CObject::RotateByEx)
        .Overload< void (CObject::*)(const Vector3 &, Uint32) const >
            (_SC("RotateByEuler"), &CObject::RotateByEuler)
        .Overload< void (CObject::*)(Float32, Float32, Float32, Uint32) const >
            (_SC("RotateByEuler"), &CObject::RotateByEulerEx)
        // Static Overloads
        .StaticOverload< LightObj & (*)(Int32, Int32, Float32, Float32, Float32, Int32) >
            (_SC("CreateEx"), &Object_CreateEx)
        .StaticOverload< LightObj & (*)(Int32, Int32, Float32, Float32, Float32, Int32, Int32, LightObj &) >
            (_SC("CreateEx"), &Object_CreateEx)
        .StaticOverload< LightObj & (*)(Int32, Int32, const Vector3 &, Int32) >
            (_SC("Create"), &Object_Create)
        .StaticOverload< LightObj & (*)(Int32, Int32, const Vector3 &, Int32, Int32, LightObj &) >
            (_SC("Create"), &Object_Create)
        // Raw Squirrel Methods
        .SquirrelFunc(_SC("NullInst"), &CObject::SqGetNull)
        .SquirrelFunc(_SC("MakeTask"), &Tasks::MakeTask< CObject, ENT_OBJECT >)
        .SquirrelFunc(_SC("DropTask"), &Tasks::DropTask< CObject, ENT_OBJECT >)
        .SquirrelFunc(_SC("DoesTask"), &Tasks::DoesTask< CObject, ENT_OBJECT >)
        .SquirrelFunc(_SC("FindTask"), &Tasks::FindTask< CObject, ENT_OBJECT >)
    );
}

} // Namespace:: SqMod
