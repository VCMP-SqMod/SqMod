// ------------------------------------------------------------------------------------------------
#include "Entity/Object.hpp"
#include "Entity/Player.hpp"
#include "Base/Quaternion.hpp"
#include "Base/Vector3.hpp"
#include "Core.hpp"
#include "Core/Tasks.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("SqObject"))

// ------------------------------------------------------------------------------------------------
const int32_t CObject::Max = SQMOD_OBJECT_POOL;

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
CObject::CObject(int32_t id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_OBJECT_POOL))
    , m_Tag(fmt::format("{}", id)), m_Data(), m_CircularLocks(0)
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
bool CObject::Destroy(int32_t header, LightObj & payload) const
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
void CObject::CustomEvent(int32_t header, LightObj & payload) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested action
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
int32_t CObject::GetModel() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetObjectModel(m_ID);
}

// ------------------------------------------------------------------------------------------------
int32_t CObject::GetWorld() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetObjectWorld(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetWorld(int32_t world)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const int32_t current = _Func->GetObjectWorld(m_ID);
    // Don't even bother if it's the same value
    if (current == world)
    {
        return;
    }
    // Avoid infinite recursive event loops
    else if (!(m_CircularLocks & OBJECTCL_EMIT_OBJECT_WORLD))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, OBJECTCL_EMIT_OBJECT_WORLD);
        // Now forward the event call
        Core::Get().EmitObjectWorld(m_ID, current, world);
    }
    // Avoid property unwind from a recursive call
    _Func->SetObjectWorld(m_ID, world);
}

// ------------------------------------------------------------------------------------------------
int32_t CObject::GetAlpha() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetObjectAlpha(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetAlpha(int32_t alpha)
{
    SetAlphaEx(alpha, 0);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetAlphaEx(int32_t alpha, uint32_t time)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const int32_t current = _Func->GetObjectAlpha(m_ID);
    // Don't even bother if it's the same value
    if (current == alpha)
    {
        return;
    }
    // Avoid infinite recursive event loops
    else if (!(m_CircularLocks & OBJECTCL_EMIT_OBJECT_ALPHA))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, OBJECTCL_EMIT_OBJECT_ALPHA);
        // Now forward the event call
        Core::Get().EmitObjectAlpha(m_ID, current, alpha, time);
    }
    // Avoid property unwind from a recursive call
    _Func->SetObjectAlpha(m_ID, alpha, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveTo(const Vector3 & pos, uint32_t time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectTo(m_ID, pos.x, pos.y, pos.z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveToEx(float x, float y, float z, uint32_t time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectTo(m_ID, x, y, z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveBy(const Vector3 & pos, uint32_t time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectBy(m_ID, pos.x, pos.y, pos.z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveByEx(float x, float y, float z, uint32_t time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectBy(m_ID, x, y, z, time);
}

// ------------------------------------------------------------------------------------------------
Vector3 CObject::GetPosition() const
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
void CObject::SetPositionEx(float x, float y, float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetObjectPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateTo(const Quaternion & rot, uint32_t time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectTo(m_ID, rot.x, rot.y, rot.z, rot.w, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEx(float x, float y, float z, float w, uint32_t time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectTo(m_ID, x, y, z, w, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEuler(const Vector3 & rot, uint32_t time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectToEuler(m_ID, rot.x, rot.y, rot.z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEulerEx(float x, float y, float z, uint32_t time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectToEuler(m_ID, x, y, z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateBy(const Quaternion & rot, uint32_t time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectBy(m_ID, rot.x, rot.y, rot.z, rot.w, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEx(float x, float y, float z, float w, uint32_t time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectBy(m_ID, x, y, z, w, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEuler(const Vector3 & rot, uint32_t time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectByEuler(m_ID, rot.x, rot.y, rot.z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEulerEx(float x, float y, float z, uint32_t time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectByEuler(m_ID, x, y, z, time);
}

// ------------------------------------------------------------------------------------------------
Quaternion CObject::GetRotation() const
{
    // Validate the managed identifier
    Validate();
    // Create a default quaternion instance
    Quaternion q;
    // Query the server for the values
    _Func->GetObjectRotation(m_ID, &q.x, &q.y, &q.z, &q.w);
    // Return the requested information
    return q;
}

// ------------------------------------------------------------------------------------------------
Vector3 CObject::GetRotationEuler() const
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
    // Avoid infinite recursive event loops
    else if (!(m_CircularLocks & OBJECTCL_EMIT_OBJECT_REPORT))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, OBJECTCL_EMIT_OBJECT_REPORT);
        // Now forward the event call
        Core::Get().EmitObjectReport(m_ID, current, toggle, false);
    }
    // Avoid property unwind from a recursive call
    _Func->SetObjectShotReportEnabled(m_ID, static_cast< uint8_t >(toggle));
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
    // Avoid infinite recursive event loops
    else if (!(m_CircularLocks & OBJECTCL_EMIT_OBJECT_REPORT))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, OBJECTCL_EMIT_OBJECT_REPORT);
        // Now forward the event call
        Core::Get().EmitObjectReport(m_ID, current, toggle, true);
    }
    // Avoid property unwind from a recursive call
    _Func->SetObjectTouchedReportEnabled(m_ID, static_cast< uint8_t >(toggle));
}

// ------------------------------------------------------------------------------------------------
float CObject::GetPositionX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float x = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectPosition(m_ID, &x, &dummy, &dummy);
    // Return the requested information
    return x;
}

// ------------------------------------------------------------------------------------------------
float CObject::GetPositionY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float y = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectPosition(m_ID, &dummy, &y, &dummy);
    // Return the requested information
    return y;
}

// ------------------------------------------------------------------------------------------------
float CObject::GetPositionZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float z = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectPosition(m_ID, &dummy, &dummy, &z);
    // Return the requested information
    return z;
}

// ------------------------------------------------------------------------------------------------
void CObject::SetPositionX(float x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectPosition(m_ID, &dummy, &y, &z);
    // Perform the requested operation
    _Func->SetObjectPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetPositionY(float y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectPosition(m_ID, &x, &dummy, &z);
    // Perform the requested operation
    _Func->SetObjectPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetPositionZ(float z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, y, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectPosition(m_ID, &x, &y, &dummy);
    // Perform the requested operation
    _Func->SetObjectPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
float CObject::GetRotationX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float x = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectRotation(m_ID, &x, &dummy, &dummy, &dummy);
    // Return the requested information
    return x;
}

// ------------------------------------------------------------------------------------------------
float CObject::GetRotationY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float y = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectRotation(m_ID, &dummy, &y, &dummy, &dummy);
    // Return the requested information
    return y;
}

// ------------------------------------------------------------------------------------------------
float CObject::GetRotationZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float z = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectRotation(m_ID, &dummy, &dummy, &z, &dummy);
    // Return the requested information
    return z;
}

// ------------------------------------------------------------------------------------------------
float CObject::GetRotationW() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float w = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectRotation(m_ID, &dummy, &dummy, &dummy, &w);
    // Return the requested information
    return w;
}

// ------------------------------------------------------------------------------------------------
float CObject::GetEulerRotationX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float x = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectRotationEuler(m_ID, &x, &dummy, &dummy);
    // Return the requested information
    return x;
}

// ------------------------------------------------------------------------------------------------
float CObject::GetEulerRotationY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float y = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectRotationEuler(m_ID, &dummy, &y, &dummy);
    // Return the requested information
    return y;
}

// ------------------------------------------------------------------------------------------------
float CObject::GetEulerRotationZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float z = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetObjectRotationEuler(m_ID, &dummy, &dummy, &z);
    // Return the requested information
    return z;
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveToX(float x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectPosition(m_ID, &dummy, &y, &z);
    // Perform the requested operation
    _Func->MoveObjectTo(m_ID, x, y, z, mMoveToDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveToY(float y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectPosition(m_ID, &x, &dummy, &z);
    // Perform the requested operation
    _Func->MoveObjectTo(m_ID, x, y, z, mMoveToDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveToZ(float z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, y, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectPosition(m_ID, &x, &y, &dummy);
    // Perform the requested operation
    _Func->MoveObjectTo(m_ID, z, y, z, mMoveToDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveByX(float x) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectBy(m_ID, x, 0.0f, 0.0f, mMoveByDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveByY(float y) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectBy(m_ID, 0.0f, y, 0.0f, mMoveByDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveByZ(float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectBy(m_ID, 0.0f, 0.0f, z, mMoveByDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToX(float x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float y, z, w, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectRotation(m_ID, &dummy, &y, &z, &w);
    // Perform the requested operation
    _Func->RotateObjectTo(m_ID, x, y, z, w, mRotateToDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToY(float y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, z, w, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectRotation(m_ID, &x, &dummy, &z, &w);
    // Perform the requested operation
    _Func->RotateObjectTo(m_ID, x, y, z, w, mRotateToDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToZ(float z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, y, w, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectRotation(m_ID, &x, &y, &dummy, &w);
    // Perform the requested operation
    _Func->RotateObjectTo(m_ID, x, y, z, w, mRotateToDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToW(float w) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectRotation(m_ID, &x, &y, &z, &dummy);
    // Perform the requested operation
    _Func->RotateObjectTo(m_ID, x, y, z, w, mRotateToDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByX(float x) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectBy(m_ID, x, 0.0f, 0.0f, 0.0f, mRotateByDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByY(float y) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectBy(m_ID, 0.0f, y, 0.0f, 0.0f, mRotateByDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByZ(float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectBy(m_ID, 0.0f, 0.0f, z, 0.0f, mRotateByDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByW(float w) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectBy(m_ID, 0.0f, 0.0f, 0.0f, w, mRotateByDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEulerX(float x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectRotationEuler(m_ID, &dummy, &y, &z);
    // Perform the requested operation
    _Func->RotateObjectToEuler(m_ID, x, y, z, mRotateToEulerDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEulerY(float y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectRotationEuler(m_ID, &x, &dummy, &z);
    // Perform the requested operation
    _Func->RotateObjectToEuler(m_ID, x, y, z, mRotateToEulerDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEulerZ(float z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, y, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetObjectRotationEuler(m_ID, &x, &y, &dummy);
    // Perform the requested operation
    _Func->RotateObjectToEuler(m_ID, z, y, z, mRotateToEulerDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEulerX(float x) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectByEuler(m_ID, x, 0.0f, 0.0f, mRotateByEulerDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEulerY(float y) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectByEuler(m_ID, 0.0f, y, 0.0f, mRotateByEulerDuration);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEulerZ(float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateObjectByEuler(m_ID, 0.0f, 0.0f, z, mRotateByEulerDuration);
}
#ifdef VCMP_ENABLE_OFFICIAL
// ------------------------------------------------------------------------------------------------
LightObj & CObject::GetLegacyObject() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetObj(m_ID).mLgObj;
}
#endif

// ------------------------------------------------------------------------------------------------
static LightObj & Object_CreateEx1a(int32_t model, int32_t world, float x, float y, float z,
                                int32_t alpha)
{
    return Core::Get().NewObject(model, world, x, y, z, alpha, SQMOD_CREATE_DEFAULT, NullLightObj()).mObj;
}

static LightObj & Object_CreateEx1b(int32_t model, int32_t world, float x, float y, float z,
                        int32_t alpha, int32_t header, LightObj & payload)
{
    return Core::Get().NewObject(model, world, x, y, z, alpha, header, payload).mObj;
}

// ------------------------------------------------------------------------------------------------
static LightObj & Object_Create1a(int32_t model, int32_t world, const Vector3 & pos, int32_t alpha)
{
    return Core::Get().NewObject(model, world, pos.x, pos.y, pos.z, alpha,
                            SQMOD_CREATE_DEFAULT, NullLightObj()).mObj;
}

static LightObj & Object_Create1b(int32_t model, int32_t world, const Vector3 & pos, int32_t alpha,
                            int32_t header, LightObj & payload)
{
    return Core::Get().NewObject(model, world, pos.x, pos.y, pos.z, alpha, header, payload).mObj;
}

// ================================================================================================
void Register_CObject(HSQUIRRELVM vm)
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
#ifdef VCMP_ENABLE_OFFICIAL
        .Prop(_SC("Legacy"), &CObject::GetLegacyObject)
#endif
        // Core Methods
        .FmtFunc(_SC("SetTag"), &CObject::ApplyTag)
        .Func(_SC("CustomEvent"), &CObject::CustomEvent)
        // Core Overloads
        .Overload(_SC("Destroy"), &CObject::Destroy0)
        .Overload(_SC("Destroy"), &CObject::Destroy1)
        .Overload(_SC("Destroy"), &CObject::Destroy)
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
        .Overload(_SC("MoveTo"), &CObject::MoveTo)
        .Overload(_SC("MoveTo"), &CObject::MoveToEx)
        .Overload(_SC("MoveBy"), &CObject::MoveBy)
        .Overload(_SC("MoveBy"), &CObject::MoveByEx)
        .Overload(_SC("RotateTo"), &CObject::RotateTo)
        .Overload(_SC("RotateTo"), &CObject::RotateToEx)
        .Overload(_SC("RotateToEuler"), &CObject::RotateToEuler)
        .Overload(_SC("RotateToEuler"), &CObject::RotateToEulerEx)
        .Overload(_SC("RotateBy"), &CObject::RotateBy)
        .Overload(_SC("RotateBy"), &CObject::RotateByEx)
        .Overload(_SC("RotateByEuler"), &CObject::RotateByEuler)
        .Overload(_SC("RotateByEuler"), &CObject::RotateByEulerEx)
        // Static Overloads
        .StaticOverload(_SC("CreateEx"), &Object_CreateEx1a)
        .StaticOverload(_SC("CreateEx"), &Object_CreateEx1b)
        .StaticOverload(_SC("Create"), &Object_Create1a)
        .StaticOverload(_SC("Create"), &Object_Create1b)
        // Raw Squirrel Methods
        .SquirrelFunc(_SC("NullInst"), &CObject::SqGetNull)
        .SquirrelFunc(_SC("MakeTask"), &Tasks::MakeTask< CObject, ENT_OBJECT >)
        .SquirrelFunc(_SC("DropTask"), &Tasks::DropTask< CObject, ENT_OBJECT >)
        .SquirrelFunc(_SC("DoesTask"), &Tasks::DoesTask< CObject, ENT_OBJECT >)
        .SquirrelFunc(_SC("FindTask"), &Tasks::FindTask< CObject, ENT_OBJECT >)
    );
}

} // Namespace:: SqMod
