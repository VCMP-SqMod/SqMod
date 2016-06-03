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
const Int32 CPickup::Max = SQMOD_PICKUP_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CPickup::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqPickup");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
CPickup::CPickup(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_PICKUP_POOL))
    , m_Tag(ToStrF("%d", id))
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
const String & CPickup::ToString() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
const String & CPickup::GetTag() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

// ------------------------------------------------------------------------------------------------
Object & CPickup::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetData(Object & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CPickup::Destroy(Int32 header, Object & payload)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return Core::Get().DelPickup(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
void CPickup::BindEvent(Int32 evid, Object & env, Function & func) const
{
    // Validate the managed identifier
    Validate();
    // Obtain the function instance called for this event
    Function & event = Core::Get().GetPickupEvent(m_ID, evid);
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
bool CPickup::IsStreamedFor(CPlayer & player) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsPickupStreamedForPlayer(m_ID, player.GetID());
}

// ------------------------------------------------------------------------------------------------
Int32 CPickup::GetWorld() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPickupWorld(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetWorld(Int32 world) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPickupWorld(m_ID, world);
}

// ------------------------------------------------------------------------------------------------
Int32 CPickup::GetAlpha() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPickupAlpha(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetAlpha(Int32 alpha) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPickupAlpha(m_ID, alpha);
}

// ------------------------------------------------------------------------------------------------
bool CPickup::GetAutomatic() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsPickupAutomatic(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetAutomatic(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPickupIsAutomatic(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
Int32 CPickup::GetAutoTimer() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPickupAutoTimer(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetAutoTimer(Int32 timer) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPickupAutoTimer(m_ID, timer);
}

// ------------------------------------------------------------------------------------------------
void CPickup::Refresh() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RefreshPickup(m_ID);
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CPickup::GetPosition()
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.Clear();
    // Query the server for the position values
    _Func->GetPickupPosition(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetPosition(const Vector3 & pos) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPickupPosition(m_ID, pos.x, pos.y, pos.z);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetPositionEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPickupPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
Int32 CPickup::GetModel() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPickupModel(m_ID);
}

// ------------------------------------------------------------------------------------------------
Int32 CPickup::GetQuantity() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPickupQuantity(m_ID);
}

// ------------------------------------------------------------------------------------------------
Float32 CPickup::GetPositionX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.x = 0;
    // Query the server for the requested component value
    _Func->GetPickupPosition(m_ID, &s_Vector3.x, NULL, NULL);
    // Return the requested information
    return s_Vector3.x;
}

// ------------------------------------------------------------------------------------------------
Float32 CPickup::GetPositionY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.y = 0;
    // Query the server for the requested component value
    _Func->GetPickupPosition(m_ID, NULL, &s_Vector3.y, NULL);
    // Return the requested information
    return s_Vector3.y;
}

// ------------------------------------------------------------------------------------------------
Float32 CPickup::GetPositionZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.z = 0;
    // Query the server for the requested component value
    _Func->GetPickupPosition(m_ID, NULL, NULL, &s_Vector3.z);
    // Return the requested information
    return s_Vector3.z;
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetPositionX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetPickupPosition(m_ID, NULL, &s_Vector3.y, &s_Vector3.z);
    // Perform the requested operation
    _Func->SetPickupPosition(m_ID, x, s_Vector3.y, s_Vector3.z);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetPositionY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetPickupPosition(m_ID, &s_Vector3.x, NULL, &s_Vector3.z);
    // Perform the requested operation
    _Func->SetPickupPosition(m_ID, s_Vector3.x, y, s_Vector3.z);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetPositionZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetPickupPosition(m_ID, &s_Vector3.x, &s_Vector3.y, NULL);
    // Perform the requested operation
    _Func->SetPickupPosition(m_ID, s_Vector3.z, s_Vector3.y, z);
}

// ------------------------------------------------------------------------------------------------
static Object & Pickup_CreateEx(Int32 model, Int32 world, Int32 quantity,
                        Float32 x, Float32 y, Float32 z, Int32 alpha, bool automatic)
{
    return Core::Get().NewPickup(model, world, quantity, x, y, z, alpha, automatic,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & Pickup_CreateEx(Int32 model, Int32 world, Int32 quantity,
                        Float32 x, Float32 y, Float32 z, Int32 alpha, bool automatic,
                        Int32 header, Object & payload)
{
    return Core::Get().NewPickup(model, world, quantity, x, y, z, alpha, automatic, header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & Pickup_Create(Int32 model, Int32 world, Int32 quantity, const Vector3 & pos,
                            Int32 alpha, bool automatic)
{
    return Core::Get().NewPickup(model, world, quantity, pos.x, pos.y, pos.z, alpha, automatic,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & Pickup_Create(Int32 model, Int32 world, Int32 quantity, const Vector3 & pos,
                            Int32 alpha, bool automatic, Int32 header, Object & payload)
{
    return Core::Get().NewPickup(model, world, quantity, pos.x, pos.y, pos.z, alpha, automatic,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
static const Object & Pickup_FindByID(Int32 id)
{
    // Perform a range check on the specified identifier
    if (INVALID_ENTITYEX(id, SQMOD_PICKUP_POOL))
    {
        STHROWF("The specified pickup identifier is invalid: %d", id);
    }
    // Obtain the ends of the entity pool
    Core::Pickups::const_iterator itr = Core::Get().GetPickups().cbegin();
    Core::Pickups::const_iterator end = Core::Get().GetPickups().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does the identifier match the specified one?
        if (itr->mID == id)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a pickup matching the specified identifier
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
static const Object & Pickup_FindByTag(CSStr tag)
{
    // Perform a validity check on the specified tag
    if (!tag || *tag == '0')
    {
        STHROWF("The specified pickup tag is invalid: null/empty");
    }
    // Obtain the ends of the entity pool
    Core::Pickups::const_iterator itr = Core::Get().GetPickups().cbegin();
    Core::Pickups::const_iterator end = Core::Get().GetPickups().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does this entity even exist and does the tag match the specified one?
        if (itr->mInst != nullptr && itr->mInst->GetTag().compare(tag) == 0)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a pickup matching the specified tag
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
static Array Pickup_FindActive()
{
    // Remember the initial stack size
    StackGuard sg;
    // Obtain the ends of the entity pool
    Core::Pickups::const_iterator itr = Core::Get().GetPickups().cbegin();
    Core::Pickups::const_iterator end = Core::Get().GetPickups().cend();
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
void Register_CPickup(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqPickup"),
        Class< CPickup, NoConstructor< CPickup > >(vm, _SC("SqPickup"))
        // Meta-methods
        .Func(_SC("_cmp"), &CPickup::Cmp)
        .SquirrelFunc(_SC("_typename"), &CPickup::Typename)
        .Func(_SC("_tostring"), &CPickup::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CPickup::Max)
        // Core Properties
        .Prop(_SC("ID"), &CPickup::GetID)
        .Prop(_SC("Tag"), &CPickup::GetTag, &CPickup::SetTag)
        .Prop(_SC("Data"), &CPickup::GetData, &CPickup::SetData)
        .Prop(_SC("Active"), &CPickup::IsActive)
        // Core Methods
        .Func(_SC("Bind"), &CPickup::BindEvent)
        // Core Overloads
        .Overload< bool (CPickup::*)(void) >(_SC("Destroy"), &CPickup::Destroy)
        .Overload< bool (CPickup::*)(Int32) >(_SC("Destroy"), &CPickup::Destroy)
        .Overload< bool (CPickup::*)(Int32, Object &) >(_SC("Destroy"), &CPickup::Destroy)
        // Properties
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
        .Prop(_SC("PosX"), &CPickup::GetPositionX, &CPickup::SetPositionX)
        .Prop(_SC("PosY"), &CPickup::GetPositionY, &CPickup::SetPositionY)
        .Prop(_SC("PosZ"), &CPickup::GetPositionZ, &CPickup::SetPositionZ)
        // Member Methods
        .Func(_SC("StreamedFor"), &CPickup::IsStreamedFor)
        .Func(_SC("Refresh"), &CPickup::Refresh)
        .Func(_SC("SetPos"), &CPickup::SetPositionEx)
        .Func(_SC("SetPosition"), &CPickup::SetPositionEx)
        // Static Functions
        .StaticFunc(_SC("FindByID"), &Pickup_FindByID)
        .StaticFunc(_SC("FindByTag"), &Pickup_FindByTag)
        .StaticFunc(_SC("FindActive"), &Pickup_FindActive)
        // Static Overloads
        .StaticOverload< Object & (*)(Int32, Int32, Int32, Float32, Float32, Float32, Int32, bool) >
            (_SC("CreateEx"), &Pickup_CreateEx)
        .StaticOverload< Object & (*)(Int32, Int32, Int32, Float32, Float32, Float32, Int32, bool, Int32, Object &) >
            (_SC("CreateEx"), &Pickup_CreateEx)
        .StaticOverload< Object & (*)(Int32, Int32, Int32, const Vector3 &, Int32, bool) >
            (_SC("Create"), &Pickup_Create)
        .StaticOverload< Object & (*)(Int32, Int32, Int32, const Vector3 &, Int32, bool, Int32, Object &) >
            (_SC("Create"), &Pickup_Create)
    );
}

} // Namespace:: SqMod
