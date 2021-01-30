// ------------------------------------------------------------------------------------------------
#include "Entity/Blip.hpp"
#include "Core.hpp"
#include "Core/Tasks.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("SqBlip"))

// ------------------------------------------------------------------------------------------------
const int32_t CBlip::Max = SQMOD_BLIP_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CBlip::SqGetNull(HSQUIRRELVM vm)
{
    sq_pushobject(vm, Core::Get().GetNullBlip().GetObj());
    return 1;
}

// ------------------------------------------------------------------------------------------------
LightObj & CBlip::GetNull()
{
    return Core::Get().GetNullBlip();
}

// ------------------------------------------------------------------------------------------------
CBlip::CBlip(int32_t id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_BLIP_POOL))
    , m_Tag(fmt::format("{}", id))
{
    /* ... */
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
void CBlip::SetTag(StackStrF & tag)
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
CBlip & CBlip::ApplyTag(StackStrF & tag)
{
    SetTag(tag);
    return *this;
}

// ------------------------------------------------------------------------------------------------
LightObj & CBlip::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CBlip::SetData(LightObj & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CBlip::Destroy(int32_t header, LightObj & payload) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return Core::Get().DelBlip(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
LightObj & CBlip::GetEvents() const
{
    // Validate the managed identifier
    Validate();
    // Return the associated event table
    return Core::Get().GetBlip(m_ID).mEvents;
}

// ------------------------------------------------------------------------------------------------
void CBlip::CustomEvent(int32_t header, LightObj & payload) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested action
    Core::Get().EmitBlipCustom(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
int32_t CBlip::GetWorld() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mWorld;
}

// ------------------------------------------------------------------------------------------------
int32_t CBlip::GetScale() const
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
int32_t CBlip::GetSprID() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mSprID;
}

// ------------------------------------------------------------------------------------------------
float CBlip::GetPositionX() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mPosition.x;
}

// ------------------------------------------------------------------------------------------------
float CBlip::GetPositionY() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mPosition.y;
}

// ------------------------------------------------------------------------------------------------
float CBlip::GetPositionZ() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mPosition.z;
}

// ------------------------------------------------------------------------------------------------
int32_t CBlip::GetColorR() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mColor.r;
}

// ------------------------------------------------------------------------------------------------
int32_t CBlip::GetColorG() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mColor.g;
}

// ------------------------------------------------------------------------------------------------
int32_t CBlip::GetColorB() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mColor.b;
}

// ------------------------------------------------------------------------------------------------
int32_t CBlip::GetColorA() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetBlip(m_ID).mColor.a;
}

// ------------------------------------------------------------------------------------------------
static LightObj & Blip_CreateEx1a(int32_t world, float x, float y, float z, int32_t scale,
                            uint8_t r, uint8_t g, uint8_t b, uint8_t a, int32_t spr_id)
{
    return Core::Get().NewBlip(-1, world, x, y, z, scale, SQMOD_PACK_RGBA(r, g, b, a), spr_id, // NOLINT(hicpp-signed-bitwise)
                            SQMOD_CREATE_DEFAULT, NullLightObj());
}

static LightObj & Blip_CreateEx1b(int32_t world, float x, float y, float z, int32_t scale,
                            uint8_t r, uint8_t g, uint8_t b, uint8_t a, int32_t spr_id,
                            int32_t header, LightObj & payload)
{
    return Core::Get().NewBlip(-1, world, x, y, z, scale, SQMOD_PACK_RGBA(r, g, b, a), spr_id, // NOLINT(hicpp-signed-bitwise)
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
static LightObj & Blip_CreateEx2a(int32_t index, int32_t world, float x, float y, float z,
                            int32_t scale, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int32_t spr_id)
{
    return Core::Get().NewBlip(index, world, x, y, z, scale, SQMOD_PACK_RGBA(r, g, b, a), spr_id, // NOLINT(hicpp-signed-bitwise)
                            SQMOD_CREATE_DEFAULT, NullLightObj());
}

static LightObj & Blip_CreateEx2b(int32_t index, int32_t world, float x, float y, float z, int32_t scale,
                            uint8_t r, uint8_t g, uint8_t b, uint8_t a, int32_t spr_id,
                            int32_t header, LightObj & payload)
{
    return Core::Get().NewBlip(index, world, x, y, z, scale, SQMOD_PACK_RGBA(r, g, b, a), spr_id, // NOLINT(hicpp-signed-bitwise)
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
static LightObj & Blip_Create1a(int32_t world, const Vector3 & pos, int32_t scale, const Color4 & color,
                            int32_t spr_id)
{
    return Core::Get().NewBlip(-1, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), spr_id,
                            SQMOD_CREATE_DEFAULT, NullLightObj());
}

static LightObj & Blip_Create1b(int32_t world, const Vector3 & pos, int32_t scale, const Color4 & color,
                            int32_t spr_id, int32_t header, LightObj & payload)
{
    return Core::Get().NewBlip(-1, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), spr_id,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
static LightObj & Blip_Create2a(int32_t index, int32_t world, const Vector3 & pos, int32_t scale,
                            const Color4 & color, int32_t spr_id)
{
    return Core::Get().NewBlip(index, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), spr_id,
                            SQMOD_CREATE_DEFAULT, NullLightObj());
}

static LightObj & Blip_Create2b(int32_t index, int32_t world, const Vector3 & pos, int32_t scale,
                            const Color4 & color, int32_t spr_id, int32_t header, LightObj & payload)
{
    return Core::Get().NewBlip(index, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), spr_id,
                            header, payload);
}

// ================================================================================================
void Register_CBlip(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(Typename::Str,
        Class< CBlip, NoConstructor< CBlip > >(vm, Typename::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &CBlip::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CBlip::Max)
        // Core Properties
        .Prop(_SC("On"), &CBlip::GetEvents)
        .Prop(_SC("ID"), &CBlip::GetID)
        .Prop(_SC("Tag"), &CBlip::GetTag, &CBlip::SetTag)
        .Prop(_SC("Data"), &CBlip::GetData, &CBlip::SetData)
        .Prop(_SC("Active"), &CBlip::IsActive)
        // Core Methods
        .FmtFunc(_SC("SetTag"), &CBlip::ApplyTag)
        .Func(_SC("CustomEvent"), &CBlip::CustomEvent)
        // Core Overloads
        .Overload(_SC("Destroy"), &CBlip::Destroy0)
        .Overload(_SC("Destroy"), &CBlip::Destroy1)
        .Overload(_SC("Destroy"), &CBlip::Destroy)
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
        // Static Overloads
        .StaticOverload(_SC("CreateEx"), &Blip_CreateEx1a)
        .StaticOverload(_SC("CreateEx"), &Blip_CreateEx1b)
        .StaticOverload(_SC("CreateEx"), &Blip_CreateEx2a)
        .StaticOverload(_SC("CreateEx"), &Blip_CreateEx2b)
        .StaticOverload(_SC("Create"), &Blip_Create1a)
        .StaticOverload(_SC("Create"), &Blip_Create1b)
        .StaticOverload(_SC("Create"), &Blip_Create2a)
        .StaticOverload(_SC("Create"), &Blip_Create2b)
        // Raw Squirrel Methods
        .SquirrelFunc(_SC("NullInst"), &CBlip::SqGetNull)
        .SquirrelFunc(_SC("MakeTask"), &Tasks::MakeTask< CBlip, ENT_BLIP >)
        .SquirrelFunc(_SC("DropTask"), &Tasks::DropTask< CBlip, ENT_BLIP >)
        .SquirrelFunc(_SC("DoesTask"), &Tasks::DoesTask< CBlip, ENT_BLIP >)
        .SquirrelFunc(_SC("FindTask"), &Tasks::FindTask< CBlip, ENT_BLIP >)
    );
}

} // Namespace:: SqMod
