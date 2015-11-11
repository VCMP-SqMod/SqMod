// ------------------------------------------------------------------------------------------------
#include "Entity/Blip.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CBlip::CBlip(const Reference< CBlip > & o)
    : Reference(o)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
SQInteger CBlip::GetWorld() const
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).World;
    }
    else
    {
        BadRef("@world", "get world");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInteger CBlip::GetScale() const
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Scale;
    }
    else
    {
        BadRef("@scale", "get scale");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CBlip::GetPosition() const
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Position;
    }
    else
    {
        BadRef("@position", "get position");
    }

    return Vector3::NIL;
}

// ------------------------------------------------------------------------------------------------
SQFloat CBlip::GetPositionX() const
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Position.x;
    }
    else
    {
        BadRef("@pos_x", "get x axis");
    }

    return 0.0;
}

// ------------------------------------------------------------------------------------------------
SQFloat CBlip::GetPositionY() const
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Position.y;
    }
    else
    {
        BadRef("@pos_y", "get y axis");
    }

    return 0.0;
}

// ------------------------------------------------------------------------------------------------
SQFloat CBlip::GetPositionZ() const
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Position.z;
    }
    else
    {
        BadRef("@pos_z", "get z axis");
    }

    return 0.0;
}

// ------------------------------------------------------------------------------------------------
const Color4 & CBlip::GetColor() const
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Color;
    }
    else
    {
        BadRef("@color", "get color");
    }

    return Color4::NIL;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CBlip::GetSprID() const
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).SprID;
    }
    else
    {
        BadRef("@spr_id", "get sprite id");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
Reference< CBlip > CreateBaseBlip_ES(SQInt32 world,
                    SQFloat x, SQFloat y, SQFloat z,
                    SQInt32 scale,
                    Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                    SQInt32 sprid)
{
    return _Core->NewBlip(SQMOD_UNKNOWN, world, x, y, z, scale, PACK_RGBA(r, g, b, a), sprid,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CBlip > CreateBaseBlip_ES(SQInt32 world,
                    SQFloat x, SQFloat y, SQFloat z,
                    SQInt32 scale,
                    Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                    SQInt32 sprid,
                    SQInt32 header, SqObj & payload)
{
    return _Core->NewBlip(SQMOD_UNKNOWN, world, x, y, z, scale, PACK_RGBA(r, g, b, a), sprid,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CBlip > CreateBaseBlip_EF(SQInt32 index, SQInt32 world,
                    SQFloat x, SQFloat y, SQFloat z,
                    SQInt32 scale,
                    Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                    SQInt32 sprid)
{
    return _Core->NewBlip(index, world, x, y, z, scale, PACK_RGBA(r, g, b, a), sprid,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CBlip > CreateBaseBlip_EF(SQInt32 index, SQInt32 world,
                    SQFloat x, SQFloat y, SQFloat z,
                    SQInt32 scale,
                    Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                    SQInt32 sprid,
                    SQInt32 header, SqObj & payload)
{
    return _Core->NewBlip(index, world, x, y, z, scale, PACK_RGBA(r, g, b, a), sprid,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CBlip > CreateBaseBlip_CS(SQInt32 world, const Vector3 & pos,
                    SQInt32 scale, const Color4 & color, SQInt32 sprid)
{
    return _Core->NewBlip(SQMOD_UNKNOWN, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprid,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CBlip > CreateBaseBlip_CS(SQInt32 world, const Vector3 & pos,
                    SQInt32 scale, const Color4 & color, SQInt32 sprid,
                    SQInt32 header, SqObj & payload)
{
    return _Core->NewBlip(SQMOD_UNKNOWN, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprid,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CBlip > CreateBaseBlip_CF(SQInt32 index, SQInt32 world, const Vector3 & pos,
                    SQInt32 scale, const Color4 & color, SQInt32 sprid)
{
    return _Core->NewBlip(index, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprid,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CBlip > CreateBaseBlip_CF(SQInt32 index, SQInt32 world, const Vector3 & pos,
                    SQInt32 scale, const Color4 & color, SQInt32 sprid,
                    SQInt32 header, SqObj & payload)
{
    return _Core->NewBlip(index, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprid,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CBlip CreateBlip_ES(SQInt32 world,
                    SQFloat x, SQFloat y, SQFloat z,
                    SQInt32 scale,
                    Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                    SQInt32 sprid)
{
    return _Core->NewBlip(SQMOD_UNKNOWN, world, x, y, z, scale, PACK_RGBA(r, g, b, a), sprid,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CBlip CreateBlip_ES(SQInt32 world,
                    SQFloat x, SQFloat y, SQFloat z,
                    SQInt32 scale,
                    Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                    SQInt32 sprid,
                    SQInt32 header, SqObj & payload)
{
    return _Core->NewBlip(SQMOD_UNKNOWN, world, x, y, z, scale, PACK_RGBA(r, g, b, a), sprid,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CBlip CreateBlip_EF(SQInt32 index, SQInt32 world,
                    SQFloat x, SQFloat y, SQFloat z,
                    SQInt32 scale,
                    Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                    SQInt32 sprid)
{
    return _Core->NewBlip(index, world, x, y, z, scale, PACK_RGBA(r, g, b, a), sprid,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CBlip CreateBlip_EF(SQInt32 index, SQInt32 world,
                    SQFloat x, SQFloat y, SQFloat z,
                    SQInt32 scale,
                    Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                    SQInt32 sprid,
                    SQInt32 header, SqObj & payload)
{
    return _Core->NewBlip(index, world, x, y, z, scale, PACK_RGBA(r, g, b, a), sprid,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CBlip CreateBlip_CS(SQInt32 world, const Vector3 & pos,
                    SQInt32 scale, const Color4 & color, SQInt32 sprid)
{
    return _Core->NewBlip(SQMOD_UNKNOWN, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprid,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CBlip CreateBlip_CS(SQInt32 world, const Vector3 & pos,
                    SQInt32 scale, const Color4 & color, SQInt32 sprid,
                    SQInt32 header, SqObj & payload)
{
    return _Core->NewBlip(SQMOD_UNKNOWN, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprid,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CBlip CreateBlip_CF(SQInt32 index, SQInt32 world, const Vector3 & pos,
                    SQInt32 scale, const Color4 & color, SQInt32 sprid)
{
    return _Core->NewBlip(index, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprid,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CBlip CreateBlip_CF(SQInt32 index, SQInt32 world, const Vector3 & pos,
                    SQInt32 scale, const Color4 & color, SQInt32 sprid,
                    SQInt32 header, SqObj & payload)
{
    return _Core->NewBlip(index, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprid,
                            header, payload);
}

// ================================================================================================
bool Register_CBlip(HSQUIRRELVM vm)
{
    // Attempt to register the base reference type before the actual implementation
    if (!Register_Reference< CBlip >(vm, _SC("BaseBlip")))
    {
        LogFtl("Unable to register the base class <BaseBlip> for <CBlip> type");
        // Registration failed
        return false;
    }
    // Typedef the base reference type for simplicity
    typedef Reference< CBlip > RefType;
    // Output debugging information
    LogDbg("Beginning registration of <CBlip> type");
    // Attempt to register the actual reference that implements all of the entity functionality
    Sqrat::RootTable(vm).Bind(_SC("CBlip"), Sqrat::DerivedClass< CBlip, RefType >(vm, _SC("CBlip"))
        /* Constructors */
        .Ctor()
        .Ctor< SQInt32 >()
        /* Properties */
        .Prop(_SC("world"), &CBlip::GetWorld)
        .Prop(_SC("scale"), &CBlip::GetScale)
        .Prop(_SC("pos"), &CBlip::GetPosition)
        .Prop(_SC("position"), &CBlip::GetPosition)
        .Prop(_SC("color"), &CBlip::GetColor)
        .Prop(_SC("spr_id"), &CBlip::GetSprID)
        .Prop(_SC("pos_x"), &CBlip::GetPositionX)
        .Prop(_SC("pos_y"), &CBlip::GetPositionY)
        .Prop(_SC("pos_z"), &CBlip::GetPositionZ)
    );
    // Output debugging information
    LogDbg("Registration of <CBlip> type was successful");
    // Output debugging information
    LogDbg("Beginning registration of <Blip> functions");
    // Register global functions related to this entity type
    Sqrat::RootTable(vm)
    /* Create BaseBlip [E]xtended [S]ubstitute */
    .Overload< RefType (*)(SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, Uint8, Uint8, Uint8, Uint8, SQInt32) >
        (_SC("CreateBaseBlip_ES"), &CreateBaseBlip_ES)
    .Overload< RefType (*)(SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, Uint8, Uint8, Uint8, Uint8, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBaseBlip_ES"), &CreateBaseBlip_ES)
    /* Create BaseBlip [E]xtended [F]Full */
    .Overload< RefType (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, Uint8, Uint8, Uint8, Uint8, SQInt32) >
        (_SC("CreateBaseBlip_EF"), &CreateBaseBlip_EF)
    .Overload< RefType (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, Uint8, Uint8, Uint8, Uint8, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBaseBlip_EF"), &CreateBaseBlip_EF)
    /* Create BaseBlip [C]ompact [S]ubstitute */
    .Overload< RefType (*)(SQInt32, const Vector3 &, SQInt32, const Color4 &, SQInt32) >
        (_SC("CreateBaseBlip_CS"), &CreateBaseBlip_CS)
    .Overload< RefType (*)(SQInt32, const Vector3 &, SQInt32, const Color4 &, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBaseBlip_CS"), &CreateBaseBlip_CS)
    /* Create BaseBlip [C]ompact [F]ull */
    .Overload< RefType (*)(SQInt32, SQInt32, const Vector3 &, SQInt32, const Color4 &, SQInt32) >
        (_SC("CreateBaseBlip_CF"), &CreateBaseBlip_CF)
    .Overload< RefType (*)(SQInt32, SQInt32, const Vector3 &, SQInt32, const Color4 &, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBaseBlip_CF"), &CreateBaseBlip_CF)
    /* Create CBlip [E]xtended [S]ubstitute */
    .Overload< CBlip (*)(SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, Uint8, Uint8, Uint8, Uint8, SQInt32) >
        (_SC("CreateBlip_ES"), &CreateBlip_ES)
    .Overload< CBlip (*)(SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, Uint8, Uint8, Uint8, Uint8, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBlip_ES"), &CreateBlip_ES)
    /* Create CBlip [E]xtended [F]Full */
    .Overload< CBlip (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, Uint8, Uint8, Uint8, Uint8, SQInt32) >
        (_SC("CreateBlip_EF"), &CreateBlip_EF)
    .Overload< CBlip (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, Uint8, Uint8, Uint8, Uint8, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBlip_EF"), &CreateBlip_EF)
    /* Create CBlip [C]ompact [S]ubstitute */
    .Overload< CBlip (*)(SQInt32, const Vector3 &, SQInt32, const Color4 &, SQInt32) >
        (_SC("CreateBlip_CS"), &CreateBlip_CS)
    .Overload< CBlip (*)(SQInt32, const Vector3 &, SQInt32, const Color4 &, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBlip_CS"), &CreateBlip_CS)
    /* Create CBlip [C]ompact [F]ull */
    .Overload< CBlip (*)(SQInt32, SQInt32, const Vector3 &, SQInt32, const Color4 &, SQInt32) >
        (_SC("CreateBlip_CF"), &CreateBlip_CF)
    .Overload< CBlip (*)(SQInt32, SQInt32, const Vector3 &, SQInt32, const Color4 &, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBlip_CF"), &CreateBlip_CF);
    // Output debugging information
    LogDbg("Registration of <Blip> functions was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
