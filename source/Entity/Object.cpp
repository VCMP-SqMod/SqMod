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
const Int32 CObject::Max = SQMOD_OBJECT_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CObject::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqObject");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
CObject::CObject(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_OBJECT_POOL))
    , m_Tag(ToStrF("%d", id))
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
void CObject::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

// ------------------------------------------------------------------------------------------------
Object & CObject::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CObject::SetData(Object & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CObject::Destroy(Int32 header, Object & payload)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return _Core->DelObject(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
void CObject::BindEvent(Int32 evid, Object & env, Function & func) const
{
    // Validate the managed identifier
    Validate();
    // Obtain the function instance called for this event
    Function & event = _Core->GetObjectEvent(m_ID, evid);
    // Is the specified callback function null?
    if (func.IsNull())
    {
        event.Release(); // Then release the current callback
    }
    // Assign the specified environment and function
    else
    {
        event = Function(env.GetVM(), env, func.GetFunc());
    }
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
void CObject::SetWorld(Int32 world) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetObjectWorld(m_ID, world);
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
void CObject::SetAlpha(Int32 alpha) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetObjectAlpha(m_ID, alpha, 0);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetAlphaEx(Int32 alpha, Int32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetObjectAlpha(m_ID, alpha, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveTo(const Vector3 & pos, Int32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectTo(m_ID, pos.x, pos.y, pos.z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveToEx(Float32 x, Float32 y, Float32 z, Int32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectTo(m_ID, x, y, z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveBy(const Vector3 & pos, Int32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectBy(m_ID, pos.x, pos.y, pos.z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::MoveByEx(Float32 x, Float32 y, Float32 z, Int32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveObjectBy(m_ID, x, y, z, time);
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CObject::GetPosition()
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information
    s_Vector3.Clear();
    // Query the server for the position values
    _Func->GetObjectPos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CObject::SetPosition(const Vector3 & pos) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetObjectPos(m_ID, pos.x, pos.y, pos.z);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetPositionEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetObjectPos(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateTo(const Quaternion & rot, Int32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotObjectTo(m_ID, rot.x, rot.y, rot.z, rot.w, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEx(Float32 x, Float32 y, Float32 z, Float32 w, Int32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotObjectTo(m_ID, x, y, z, w, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEuler(const Vector3 & rot, Int32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotObjectToEuler(m_ID, rot.x, rot.y, rot.z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateToEulerEx(Float32 x, Float32 y, Float32 z, Int32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotObjectToEuler(m_ID, x, y, z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateBy(const Quaternion & rot, Int32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotObjectBy(m_ID, rot.x, rot.y, rot.z, rot.w, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEx(Float32 x, Float32 y, Float32 z, Float32 w, Int32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotObjectBy(m_ID, x, y, z, w, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEuler(const Vector3 & rot, Int32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotObjectByEuler(m_ID, rot.x, rot.y, rot.z, time);
}

// ------------------------------------------------------------------------------------------------
void CObject::RotateByEulerEx(Float32 x, Float32 y, Float32 z, Int32 time) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotObjectByEuler(m_ID, x, y, z, time);
}

// ------------------------------------------------------------------------------------------------
const Quaternion & CObject::GetRotation()
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information
    s_Quaternion.Clear();
    // Query the server for the rotation values
    _Func->GetObjectRot(m_ID, &s_Quaternion.x, &s_Quaternion.y, &s_Quaternion.z, &s_Quaternion.w);
    // Return the requested information
    return s_Quaternion;
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CObject::GetRotationEuler()
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information
    s_Vector3.Clear();
    // Query the server for the rotation values
    _Func->GetObjectRotEuler(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
bool CObject::GetShotReport() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsObjectShotReport(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetShotReport(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetObjectShotReport(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
bool CObject::GetBumpReport() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsObjectBumpReport(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetBumpReport(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetObjectBumpReport(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetPosX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.x = 0;
    // Query the server for the requested component value
    _Func->GetObjectPos(m_ID, &s_Vector3.x, NULL, NULL);
    // Return the requested information
    return s_Vector3.x;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetPosY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.y = 0;
    // Query the server for the requested component value
    _Func->GetObjectPos(m_ID, NULL, &s_Vector3.y, NULL);
    // Return the requested information
    return s_Vector3.y;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetPosZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.z = 0;
    // Query the server for the requested component value
    _Func->GetObjectPos(m_ID, NULL, NULL, &s_Vector3.z);
    // Return the requested information
    return s_Vector3.z;
}

// ------------------------------------------------------------------------------------------------
void CObject::SetPosX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetObjectPos(m_ID, NULL, &s_Vector3.y, &s_Vector3.z);
    // Perform the requested operation
    _Func->SetObjectPos(m_ID, x, s_Vector3.y, s_Vector3.z);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetPosY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetObjectPos(m_ID, &s_Vector3.x, NULL, &s_Vector3.z);
    // Perform the requested operation
    _Func->SetObjectPos(m_ID, s_Vector3.x, y, s_Vector3.z);
}

// ------------------------------------------------------------------------------------------------
void CObject::SetPosZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetObjectPos(m_ID, &s_Vector3.x, &s_Vector3.y, NULL);
    // Perform the requested operation
    _Func->SetObjectPos(m_ID, s_Vector3.z, s_Vector3.y, z);
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetRotX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Quaternion.x = 0;
    // Query the server for the requested component value
    _Func->GetObjectRot(m_ID, &s_Quaternion.x, NULL, NULL, NULL);
    // Return the requested information
    return s_Quaternion.x;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetRotY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Quaternion.y = 0;
    // Query the server for the requested component value
    _Func->GetObjectRot(m_ID, NULL, &s_Quaternion.y, NULL, NULL);
    // Return the requested information
    return s_Quaternion.y;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetRotZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Quaternion.z = 0;
    // Query the server for the requested component value
    _Func->GetObjectRot(m_ID, NULL, NULL, &s_Quaternion.z, NULL);
    // Return the requested information
    return s_Quaternion.z;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetRotW() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Quaternion.w = 0;
    // Query the server for the requested component value
    _Func->GetObjectRot(m_ID, NULL, NULL, NULL, &s_Quaternion.w);
    // Return the requested information
    return s_Quaternion.w;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetERotX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Vector3.x = 0;
    // Query the server for the requested component value
    _Func->GetObjectRotEuler(m_ID, &s_Vector3.x, NULL, NULL);
    // Return the requested information
    return s_Vector3.x;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetERotY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Vector3.y = 0;
    // Query the server for the requested component value
    _Func->GetObjectRotEuler(m_ID, NULL, &s_Vector3.y, NULL);
    // Return the requested information
    return s_Vector3.y;
}

// ------------------------------------------------------------------------------------------------
Float32 CObject::GetERotZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Vector3.z = 0;
    // Query the server for the requested component value
    _Func->GetObjectRotEuler(m_ID, NULL, NULL, &s_Vector3.z);
    // Return the requested information
    return s_Vector3.z;
}

// ------------------------------------------------------------------------------------------------
static Object & Object_CreateEx(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z,
                                Int32 alpha)
{
    return _Core->NewObject(model, world, x, y, z, alpha, SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & Object_CreateEx(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z,
                        Int32 alpha, Int32 header, Object & payload)
{
    return _Core->NewObject(model, world, x, y, z, alpha, header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & Object_Create(Int32 model, Int32 world, const Vector3 & pos, Int32 alpha)
{
    return _Core->NewObject(model, world, pos.x, pos.y, pos.z, alpha,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & Object_Create(Int32 model, Int32 world, const Vector3 & pos, Int32 alpha,
                            Int32 header, Object & payload)
{
    return _Core->NewObject(model, world, pos.x, pos.y, pos.z, alpha, header, payload);
}

// ------------------------------------------------------------------------------------------------
static const Object & Object_FindByID(Int32 id)
{
    // Perform a range check on the specified identifier
    if (INVALID_ENTITYEX(id, SQMOD_OBJECT_POOL))
    {
        STHROWF("The specified object identifier is invalid: %d", id);
    }
    // Obtain the ends of the entity pool
    Core::Objects::const_iterator itr = _Core->GetObjects().cbegin();
    Core::Objects::const_iterator end = _Core->GetObjects().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does the identifier match the specified one?
        if (itr->mID == id)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a object matching the specified identifier
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
static const Object & Object_FindByTag(CSStr tag)
{
    // Perform a validity check on the specified tag
    if (!tag || *tag == '\0')
        STHROWF("The specified object tag is invalid: null/empty");
    // Obtain the ends of the entity pool
    Core::Objects::const_iterator itr = _Core->GetObjects().cbegin();
    Core::Objects::const_iterator end = _Core->GetObjects().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does this entity even exist and does the tag match the specified one?
        if (itr->mInst != nullptr && itr->mInst->GetTag().compare(tag) == 0)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a object matching the specified tag
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
static Array Object_FindActive()
{
    // Remember the initial stack size
    StackGuard sg;
    // Obtain the ends of the entity pool
    Core::Objects::const_iterator itr = _Core->GetObjects().cbegin();
    Core::Objects::const_iterator end = _Core->GetObjects().cend();
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

// ================================================================================================
void Register_CObject(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqObject"),
        Class< CObject, NoConstructor< CObject > >(vm, _SC("SqObject"))
        // Metamethods
        .Func(_SC("_cmp"), &CObject::Cmp)
        .SquirrelFunc(_SC("_typename"), &CObject::Typename)
        .Func(_SC("_tostring"), &CObject::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CObject::Max)
        // Core Properties
        .Prop(_SC("ID"), &CObject::GetID)
        .Prop(_SC("Tag"), &CObject::GetTag, &CObject::SetTag)
        .Prop(_SC("Data"), &CObject::GetData, &CObject::SetData)
        .Prop(_SC("Active"), &CObject::IsActive)
        // Core Methods
        .Func(_SC("Bind"), &CObject::BindEvent)
        // Core Overloads
        .Overload< bool (CObject::*)(void) >(_SC("Destroy"), &CObject::Destroy)
        .Overload< bool (CObject::*)(Int32) >(_SC("Destroy"), &CObject::Destroy)
        .Overload< bool (CObject::*)(Int32, Object &) >(_SC("Destroy"), &CObject::Destroy)
        // Properties
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
        // Member Methods
        .Func(_SC("StreamedFor"), &CObject::IsStreamedFor)
        .Func(_SC("SetAlpha"), &CObject::SetAlphaEx)
        .Func(_SC("SetPosition"), &CObject::SetPositionEx)
        // Member Overloads
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
        // Static Functions
        .StaticFunc(_SC("FindByID"), &Object_FindByID)
        .StaticFunc(_SC("FindByTag"), &Object_FindByTag)
        .StaticFunc(_SC("FindActive"), &Object_FindActive)
        // Static Overloads
        .StaticOverload< Object & (*)(Int32, Int32, Float32, Float32, Float32, Int32) >
            (_SC("CreateEx"), &Object_CreateEx)
        .StaticOverload< Object & (*)(Int32, Int32, Float32, Float32, Float32, Int32, Int32, Object &) >
            (_SC("CreateEx"), &Object_CreateEx)
        .StaticOverload< Object & (*)(Int32, Int32, const Vector3 &, Int32) >
            (_SC("Create"), &Object_Create)
        .StaticOverload< Object & (*)(Int32, Int32, const Vector3 &, Int32, Int32, Object &) >
            (_SC("Create"), &Object_Create)
    );
}

} // Namespace:: SqMod
