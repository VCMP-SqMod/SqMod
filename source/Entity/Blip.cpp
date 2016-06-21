// ------------------------------------------------------------------------------------------------
#include "Entity/Blip.hpp"
#include "Base/Algo.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Int32 CBlip::Max = SQMOD_BLIP_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CBlip::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqBlip");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
CBlip::CBlip(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_BLIP_POOL))
    , m_Tag(ToStrF("%d", id))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CBlip::~CBlip()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Int32 CBlip::Cmp(const CBlip & o) const
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
const String & CBlip::ToString() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
const String & CBlip::GetTag() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
void CBlip::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

// ------------------------------------------------------------------------------------------------
Object & CBlip::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CBlip::SetData(Object & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CBlip::Destroy(Int32 header, Object & payload)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return Core::Get().DelBlip(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
void CBlip::BindEvent(Int32 evid, Object & env, Function & func) const
{
    // Validate the managed identifier
    Validate();
    // Obtain the function instance called for this event
    Function & event = Core::Get().GetBlipEvent(m_ID, evid);
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
Int32 CBlip::GetWorld() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mWorld;
}

// ------------------------------------------------------------------------------------------------
Int32 CBlip::GetScale() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mScale;
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CBlip::GetPosition() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mPosition;
}

// ------------------------------------------------------------------------------------------------
const Color4 & CBlip::GetColor() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mColor;
}

// ------------------------------------------------------------------------------------------------
Int32 CBlip::GetSprID() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mSprID;
}

// ------------------------------------------------------------------------------------------------
Float32 CBlip::GetPositionX() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mPosition.x;
}

// ------------------------------------------------------------------------------------------------
Float32 CBlip::GetPositionY() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mPosition.y;
}

// ------------------------------------------------------------------------------------------------
Float32 CBlip::GetPositionZ() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mPosition.z;
}

// ------------------------------------------------------------------------------------------------
Int32 CBlip::GetColorR() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mColor.r;
}

// ------------------------------------------------------------------------------------------------
Int32 CBlip::GetColorG() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mColor.g;
}

// ------------------------------------------------------------------------------------------------
Int32 CBlip::GetColorB() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mColor.b;
}

// ------------------------------------------------------------------------------------------------
Int32 CBlip::GetColorA() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mColor.a;
}

// ------------------------------------------------------------------------------------------------
static Object & Blip_CreateEx(Int32 world, Float32 x, Float32 y, Float32 z, Int32 scale,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a, Int32 sprid)
{
    return Core::Get().NewBlip(-1, world, x, y, z, scale, SQMOD_PACK_RGBA(r, g, b, a), sprid,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & Blip_CreateEx(Int32 world, Float32 x, Float32 y, Float32 z, Int32 scale,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a, Int32 sprid,
                            Int32 header, Object & payload)
{
    return Core::Get().NewBlip(-1, world, x, y, z, scale, SQMOD_PACK_RGBA(r, g, b, a), sprid,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & Blip_CreateEx(Int32 index, Int32 world, Float32 x, Float32 y, Float32 z,
                            Int32 scale, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Int32 sprid)
{
    return Core::Get().NewBlip(index, world, x, y, z, scale, SQMOD_PACK_RGBA(r, g, b, a), sprid,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & Blip_CreateEx(Int32 index, Int32 world, Float32 x, Float32 y, Float32 z, Int32 scale,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a, Int32 sprid,
                            Int32 header, Object & payload)
{
    return Core::Get().NewBlip(index, world, x, y, z, scale, SQMOD_PACK_RGBA(r, g, b, a), sprid,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & Blip_Create(Int32 world, const Vector3 & pos, Int32 scale, const Color4 & color,
                            Int32 sprid)
{
    return Core::Get().NewBlip(-1, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprid,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & Blip_Create(Int32 world, const Vector3 & pos, Int32 scale, const Color4 & color,
                            Int32 sprid, Int32 header, Object & payload)
{
    return Core::Get().NewBlip(-1, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprid,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & Blip_Create(Int32 index, Int32 world, const Vector3 & pos, Int32 scale,
                            const Color4 & color, Int32 sprid)
{
    return Core::Get().NewBlip(index, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprid,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & Blip_Create(Int32 index, Int32 world, const Vector3 & pos, Int32 scale,
                            const Color4 & color, Int32 sprid, Int32 header, Object & payload)
{
    return Core::Get().NewBlip(index, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprid,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
static const Object & Blip_FindByID(Int32 id)
{
    // Perform a range check on the specified identifier
    if (INVALID_ENTITYEX(id, SQMOD_BLIP_POOL))
        STHROWF("The specified blip identifier is invalid: %d", id);
    // Obtain the ends of the entity pool
    Core::Blips::const_iterator itr = Core::Get().GetBlips().cbegin();
    Core::Blips::const_iterator end = Core::Get().GetBlips().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does the identifier match the specified one?
        if (itr->mID == id)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a blip matching the specified identifier
    return NullObject();
}

static const Object & Blip_FindByTag(CSStr tag)
{
    // Perform a validity check on the specified tag
    if (!tag || *tag == '\0')
    {
        STHROWF("The specified blip tag is invalid: null/empty");
    }
    // Obtain the ends of the entity pool
    Core::Blips::const_iterator itr = Core::Get().GetBlips().cbegin();
    Core::Blips::const_iterator end = Core::Get().GetBlips().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does this entity even exist and does the tag match the specified one?
        if (itr->mInst != nullptr && itr->mInst->GetTag().compare(tag) == 0)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a blip matching the specified tag
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
static const Object & Blip_FindBySprID(Int32 sprid)
{
    // Perform a range check on the specified identifier
    if (sprid < 0)
    {
        STHROWF("The specified sprite identifier is invalid: %d", sprid);
    }
    // Obtain the ends of the entity pool
    Core::Blips::const_iterator itr = Core::Get().GetBlips().cbegin();
    Core::Blips::const_iterator end = Core::Get().GetBlips().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does the identifier match the specified one?
        if (itr->mSprID == sprid)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a blip matching the specified identifier
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
static Array Blip_FindActive()
{
    const StackGuard sg;
    // Allocate an empty array on the stack
    sq_newarray(DefaultVM::Get(), 0);
    // Process each entity in the pool
    Algo::Collect(Core::Get().GetBlips().cbegin(), Core::Get().GetBlips().cend(),
        [](Core::Blips::const_reference inst) -> bool {
            return VALID_ENTITY(inst.mID);
        },
        [](Core::Blips::const_reference inst) -> void {
            // Push the script object on the stack
            sq_pushobject(DefaultVM::Get(), inst.mObj.GetObject());
            // Append the object at the back of the array
            if (SQ_FAILED(sq_arrayappend(DefaultVM::Get(), -2)))
            {
                STHROWF("Unable to append entity instance to the list");
            }
        }
    );
    // Return the array at the top of the stack
    return Var< Array >(DefaultVM::Get(), -1).value;
}

// ================================================================================================
void Register_CBlip(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqBlip"),
        Class< CBlip, NoConstructor< CBlip > >(vm, _SC("SqBlip"))
        // Meta-methods
        .Func(_SC("_cmp"), &CBlip::Cmp)
        .SquirrelFunc(_SC("_typename"), &CBlip::Typename)
        .Func(_SC("_tostring"), &CBlip::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CBlip::Max)
        // Core Properties
        .Prop(_SC("ID"), &CBlip::GetID)
        .Prop(_SC("Tag"), &CBlip::GetTag, &CBlip::SetTag)
        .Prop(_SC("Data"), &CBlip::GetData, &CBlip::SetData)
        .Prop(_SC("Active"), &CBlip::IsActive)
        // Core Methods
        .Func(_SC("Bind"), &CBlip::BindEvent)
        // Core Overloads
        .Overload< bool (CBlip::*)(void) >(_SC("Destroy"), &CBlip::Destroy)
        .Overload< bool (CBlip::*)(Int32) >(_SC("Destroy"), &CBlip::Destroy)
        .Overload< bool (CBlip::*)(Int32, Object &) >(_SC("Destroy"), &CBlip::Destroy)
        // Properties
        .Prop(_SC("World"), &CBlip::GetWorld)
        .Prop(_SC("Scale"), &CBlip::GetScale)
        .Prop(_SC("Pos"), &CBlip::GetPosition)
        .Prop(_SC("Position"), &CBlip::GetPosition)
        .Prop(_SC("Color"), &CBlip::GetColor)
        .Prop(_SC("Colour"), &CBlip::GetColor)
        .Prop(_SC("SprID"), &CBlip::GetSprID)
        .Prop(_SC("PosX"), &CBlip::GetPositionX)
        .Prop(_SC("PosY"), &CBlip::GetPositionY)
        .Prop(_SC("PosZ"), &CBlip::GetPositionZ)
        .Prop(_SC("Red"), &CBlip::GetColorR)
        .Prop(_SC("Green"), &CBlip::GetColorG)
        .Prop(_SC("Blue"), &CBlip::GetColorB)
        .Prop(_SC("Alpha"), &CBlip::GetColorA)
        // Static Functions
        .StaticFunc(_SC("FindByID"), &Blip_FindByID)
        .StaticFunc(_SC("FindByTag"), &Blip_FindByTag)
        .StaticFunc(_SC("FindActive"), &Blip_FindActive)
        .StaticFunc(_SC("FindBySprID"), &Blip_FindBySprID)
        // Static Overloads
        .StaticOverload< Object & (*)(Int32, Float32, Float32, Float32, Int32, Uint8, Uint8, Uint8, Uint8, Int32) >
            (_SC("CreateEx"), &Blip_CreateEx)
        .StaticOverload< Object & (*)(Int32, Float32, Float32, Float32, Int32, Uint8, Uint8, Uint8, Uint8, Int32, Int32, Object &) >
            (_SC("CreateEx"), &Blip_CreateEx)
        .StaticOverload< Object & (*)(Int32, Int32, Float32, Float32, Float32, Int32, Uint8, Uint8, Uint8, Uint8, Int32) >
            (_SC("CreateEx"), &Blip_CreateEx)
        .StaticOverload< Object & (*)(Int32, Int32, Float32, Float32, Float32, Int32, Uint8, Uint8, Uint8, Uint8, Int32, Int32, Object &) >
            (_SC("CreateEx"), &Blip_CreateEx)
        .StaticOverload< Object & (*)(Int32, const Vector3 &, Int32, const Color4 &, Int32) >
            (_SC("Create"), &Blip_Create)
        .StaticOverload< Object & (*)(Int32, const Vector3 &, Int32, const Color4 &, Int32, Int32, Object &) >
            (_SC("Create"), &Blip_Create)
        .StaticOverload< Object & (*)(Int32, Int32, const Vector3 &, Int32, const Color4 &, Int32) >
            (_SC("Create"), &Blip_Create)
        .StaticOverload< Object & (*)(Int32, Int32, const Vector3 &, Int32, const Color4 &, Int32, Int32, Object &) >
            (_SC("Create"), &Blip_Create)
    );
}

} // Namespace:: SqMod
