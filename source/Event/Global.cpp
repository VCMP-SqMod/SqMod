#include "Event/Global.hpp"
#include "Register.hpp"
#include "Entity.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
GlobalEvent::GlobalEvent() noexcept
    : GlobalEvent(EVT_UNKNOWN, false)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
GlobalEvent::GlobalEvent(SQInt32 type) noexcept
    : GlobalEvent(type, false)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
GlobalEvent::GlobalEvent(SQInt32 type, bool suspended) noexcept
    : m_Type(type)
    , m_Stride(0)
    , m_Ignore(0)
    , m_Primary(SQMOD_UNKNOWN)
    , m_Secondary(SQMOD_UNKNOWN)
    , m_Idle()
    , m_OnTrigger()
    , m_OnInclude()
    , m_OnExclude()
    , m_OnCleared()
    , m_OnRelease()
    , m_Tag()
    , m_Data()
    , m_Confined(false)
    , m_Suspended(suspended)
    , m_Blips(this)
    , m_Checkpoints(this)
    , m_Keybinds(this)
    , m_Objects(this)
    , m_Pickups(this)
    , m_Players(this)
    , m_Spheres(this)
    , m_Sprites(this)
    , m_Textdraws(this)
    , m_Vehicles(this)
{
    // Attach to the specified event signal
    Attach();
    /* Entity filters are empty so there's nothing to hook to! */
}

// ------------------------------------------------------------------------------------------------
GlobalEvent::~GlobalEvent()
{
    // Detach from the specified event signal
    Detach();
    /* We're expecting the entity filters to unhook themselves from the destroy signal! */
}

// ------------------------------------------------------------------------------------------------
bool GlobalEvent::operator == (const GlobalEvent & o) const noexcept
{
    return (m_Type == o.m_Type);
}

// ------------------------------------------------------------------------------------------------
bool GlobalEvent::operator != (const GlobalEvent & o) const noexcept
{
    return (m_Type != o.m_Type);
}

// ------------------------------------------------------------------------------------------------
bool GlobalEvent::operator < (const GlobalEvent & o) const noexcept
{
    return (m_Type < o.m_Type);
}

// ------------------------------------------------------------------------------------------------
bool GlobalEvent::operator > (const GlobalEvent & o) const noexcept
{
    return (m_Type > o.m_Type);
}

// ------------------------------------------------------------------------------------------------
bool GlobalEvent::operator <= (const GlobalEvent & o) const noexcept
{
    return (m_Type <= o.m_Type);
}

// ------------------------------------------------------------------------------------------------
bool GlobalEvent::operator >= (const GlobalEvent & o) const noexcept
{
    return (m_Type >= o.m_Type);
}

// ------------------------------------------------------------------------------------------------
SQInt32 GlobalEvent::Cmp(const GlobalEvent & o) const noexcept
{
    if (m_Type == o.m_Type)
    {
        return 0;
    }
    else if (m_Type > o.m_Type)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * GlobalEvent::GetName() const noexcept
{
    return GetEventName(m_Type);
}

// ------------------------------------------------------------------------------------------------
const SQChar * GlobalEvent::GetTag() const noexcept
{
    return m_Tag.c_str();
}

void GlobalEvent::SetTag(const SQChar * tag) noexcept
{
    m_Tag = tag;
}

// ------------------------------------------------------------------------------------------------
SqObj & GlobalEvent::GetData() noexcept
{
    return m_Data;
}

void GlobalEvent::SetData(SqObj & data) noexcept
{
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
SQInt32 GlobalEvent::GetType() const noexcept
{
    return m_Type;
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::SetType(SQInt32 type) noexcept
{
    // Make sure the newly specified event is compatible
    if (!Compatible(type))
    {
        LogErr("Cannot change the event to an incompatible type: %s", GetEventName(type));
    }
    else
    {
        // Clear anything that cannot adapt to the new event type
        Adaptable(type);
        // Detach from the current event type
        Detach();
        // Set the new event type
        m_Type = type;
        // Attach to the new event type
        Attach();
        /* We don't need to hook back filters that could adapt because they're already hooked! */
    }
}

// ------------------------------------------------------------------------------------------------
SQInteger GlobalEvent::GetIdle() const noexcept
{
    return _SCSQI(std::chrono::duration_cast<std::chrono::milliseconds>(m_Idle - std::chrono::steady_clock::now()).count());
}

void GlobalEvent::SetIdle(SQInteger millis) noexcept
{
    m_Idle = (std::chrono::steady_clock::now() + std::chrono::milliseconds(_SCI64(millis)));
}

bool GlobalEvent::IsIdle() const noexcept
{
    return (m_Idle > std::chrono::steady_clock::now());
}

// ------------------------------------------------------------------------------------------------
SQInt32 GlobalEvent::GetStride() const noexcept
{
    return m_Stride;
}

void GlobalEvent::SetStride(SQInt32 stride) noexcept
{
    m_Stride = stride > 0 ? stride : 0;
}

// ------------------------------------------------------------------------------------------------
SQInt32 GlobalEvent::GetIgnore() const noexcept
{
    return m_Ignore;
}

void GlobalEvent::SetIgnore(SQInt32 ignore) noexcept
{
    m_Ignore = ignore > 0 ? ignore : 0;
}

// ------------------------------------------------------------------------------------------------
SQInt32 GlobalEvent::GetPrimary() const noexcept
{
    return m_Primary;
}

void GlobalEvent::SetPrimary(SQInt32 subset) noexcept
{
    m_Primary = subset;
}

// ------------------------------------------------------------------------------------------------
SQInt32 GlobalEvent::GetSecondary() const noexcept
{
    return m_Secondary;
}

void GlobalEvent::SetSecondary(SQInt32 subset) noexcept
{
    m_Secondary = subset;
}

// ------------------------------------------------------------------------------------------------
bool GlobalEvent::GetSuspended() const noexcept
{
    return m_Suspended;
}

void GlobalEvent::SetSuspended(bool toggle) noexcept
{
    m_Suspended = toggle;
}

// ------------------------------------------------------------------------------------------------
bool GlobalEvent::GetConfined() const noexcept
{
    return m_Confined;
}

void GlobalEvent::SetConfined(bool toggle) noexcept
{
    m_Confined = toggle;
}

// ------------------------------------------------------------------------------------------------
Function GlobalEvent::GetOnTrigger() const noexcept
{
    return m_OnTrigger;
}

void GlobalEvent::SetOnTrigger(const Function & func) noexcept
{
    m_OnTrigger = func;
}

// ------------------------------------------------------------------------------------------------
Function GlobalEvent::GetOnInclude() const noexcept
{
    return m_OnInclude;
}

void GlobalEvent::SetOnInclude(const Function & func) noexcept
{
    m_OnInclude = func;
}

// ------------------------------------------------------------------------------------------------
Function GlobalEvent::GetOnExclude() const noexcept
{
    return m_OnExclude;
}

void GlobalEvent::SetOnExclude(const Function & func) noexcept
{
    m_OnExclude = func;
}

// ------------------------------------------------------------------------------------------------
Function GlobalEvent::GetOnCleared() const noexcept
{
    return m_OnCleared;
}

void GlobalEvent::SetOnCleared(const Function & func) noexcept
{
    m_OnCleared = func;
}

// ------------------------------------------------------------------------------------------------
Function GlobalEvent::GetOnRelease() const noexcept
{
    return m_OnRelease;
}

void GlobalEvent::SetOnRelease(const Function & func) noexcept
{
    m_OnRelease = func;
}

// ------------------------------------------------------------------------------------------------
bool GlobalEvent::Compatible(SQInt32 type) const noexcept
{
    switch (type)
    {
        case EVT_BLIPDESTROYED:
        case EVT_CHECKPOINTDESTROYED:
        case EVT_KEYBINDDESTROYED:
        case EVT_OBJECTDESTROYED:
        case EVT_PICKUPDESTROYED:
        case EVT_PLAYERDESTROYED:
        case EVT_SPHEREDESTROYED:
        case EVT_SPRITEDESTROYED:
        case EVT_TEXTDRAWDESTROYED:
        case EVT_VEHICLEDESTROYED:
        case EVT_BLIPCUSTOM:
        case EVT_CHECKPOINTCUSTOM:
        case EVT_KEYBINDCUSTOM:
        case EVT_OBJECTCUSTOM:
        case EVT_PICKUPCUSTOM:
        case EVT_PLAYERCUSTOM:
        case EVT_SPHERECUSTOM:
        case EVT_SPRITECUSTOM:
        case EVT_TEXTDRAWCUSTOM:
        case EVT_VEHICLECUSTOM:
        case EVT_PLAYERAWAY:
        case EVT_PLAYERGAMEKEYS:
        case EVT_PLAYERRENAME:
        case EVT_PLAYERREQUESTCLASS:
        case EVT_PLAYERREQUESTSPAWN:
        case EVT_PLAYERSPAWN:
        case EVT_PLAYERSTARTTYPING:
        case EVT_PLAYERSTOPTYPING:
        case EVT_PLAYERCHAT:
        case EVT_PLAYERCOMMAND:
        case EVT_PLAYERMESSAGE:
        case EVT_PLAYERHEALTH:
        case EVT_PLAYERARMOUR:
        case EVT_PLAYERWEAPON:
        case EVT_PLAYERMOVE:
        case EVT_PLAYERWASTED:
        case EVT_PLAYERKILLED:
        case EVT_PLAYERTEAMKILL:
        case EVT_PLAYERSPECTATE:
        case EVT_PLAYERCRASHREPORT:
        case EVT_PLAYERBURNING:
        case EVT_PLAYERCROUCHING:
        case EVT_PLAYERSTATE:
        case EVT_PLAYERACTION:
        case EVT_STATENONE:
        case EVT_STATENORMAL:
        case EVT_STATESHOOTING:
        case EVT_STATEDRIVER:
        case EVT_STATEPASSENGER:
        case EVT_STATEENTERDRIVER:
        case EVT_STATEENTERPASSENGER:
        case EVT_STATEEXITVEHICLE:
        case EVT_STATEUNSPAWNED:
        case EVT_ACTIONNONE:
        case EVT_ACTIONNORMAL:
        case EVT_ACTIONAIMING:
        case EVT_ACTIONSHOOTING:
        case EVT_ACTIONJUMPING:
        case EVT_ACTIONLIEDOWN:
        case EVT_ACTIONGETTINGUP:
        case EVT_ACTIONJUMPVEHICLE:
        case EVT_ACTIONDRIVING:
        case EVT_ACTIONDYING:
        case EVT_ACTIONWASTED:
        case EVT_ACTIONEMBARKING:
        case EVT_ACTIONDISEMBARKING:
        case EVT_VEHICLERESPAWN:
        case EVT_VEHICLEEXPLODE:
        case EVT_VEHICLEHEALTH:
        case EVT_VEHICLEMOVE:
        case EVT_PICKUPRESPAWN:
        case EVT_KEYBINDKEYPRESS:
        case EVT_KEYBINDKEYRELEASE:
        case EVT_VEHICLEEMBARKING:
        case EVT_VEHICLEEMBARKED:
        case EVT_VEHICLEDISEMBARK:
        case EVT_PICKUPCLAIMED:
        case EVT_PICKUPCOLLECTED:
        case EVT_OBJECTSHOT:
        case EVT_OBJECTBUMP:
        case EVT_CHECKPOINTENTERED:
        case EVT_CHECKPOINTEXITED:
        case EVT_SPHEREENTERED:
        case EVT_SPHEREEXITED:
            return true;
        default:
            return false;
    }
}

// ------------------------------------------------------------------------------------------------
GlobalEvent::BlipFilter & GlobalEvent::GetBlipFilter() noexcept
{
    return m_Blips; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
GlobalEvent::CheckpointFilter & GlobalEvent::GetCheckpointFilter() noexcept
{
    return m_Checkpoints; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
GlobalEvent::KeybindFilter & GlobalEvent::GetKeybindFilter() noexcept
{
    return m_Keybinds; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
GlobalEvent::ObjectFilter & GlobalEvent::GetObjectFilter() noexcept
{
    return m_Objects; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
GlobalEvent::PickupFilter & GlobalEvent::GetPickupFilter() noexcept
{
    return m_Pickups; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
GlobalEvent::PlayerFilter & GlobalEvent::GetPlayerFilter() noexcept
{
    return m_Players; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
GlobalEvent::SphereFilter & GlobalEvent::GetSphereFilter() noexcept
{
    return m_Spheres; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
GlobalEvent::SpriteFilter & GlobalEvent::GetSpriteFilter() noexcept
{
    return m_Sprites; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
GlobalEvent::TextdrawFilter & GlobalEvent::GetTextdrawFilter() noexcept
{
    return m_Textdraws; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
GlobalEvent::VehicleFilter & GlobalEvent::GetVehicleFilter() noexcept
{
    return m_Vehicles; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::BlipDestroyed(SQInt32 blip, SQInt32 header, Object & payload) noexcept
{
    if (m_Blips.m_Filter[blip])
    {
        m_Blips.Release(blip);
    }
    else if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(blip, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::CheckpointDestroyed(SQInt32 checkpoint, SQInt32 header, Object & payload) noexcept
{
    if (m_Checkpoints.m_Filter[checkpoint])
    {
        m_Checkpoints.Release(checkpoint);
    }
    else if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(checkpoint, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::KeybindDestroyed(SQInt32 keybind, SQInt32 header, Object & payload) noexcept
{
    if (m_Keybinds.m_Filter[keybind])
    {
        m_Keybinds.Release(keybind);
    }
    else if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(keybind, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ObjectDestroyed(SQInt32 object, SQInt32 header, Object & payload) noexcept
{
    if (m_Objects.m_Filter[object])
    {
        m_Objects.Release(object);
    }
    else if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(object, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PickupDestroyed(SQInt32 pickup, SQInt32 header, Object & payload) noexcept
{
    if (m_Pickups.m_Filter[pickup])
    {
        m_Pickups.Release(pickup);
    }
    else if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(pickup, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerDestroyed(SQInt32 player, SQInt32 header, Object & payload) noexcept
{
    if (m_Players.m_Filter[player])
    {
        m_Players.Release(player);
    }
    else if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(player, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::SphereDestroyed(SQInt32 sphere, SQInt32 header, Object & payload) noexcept
{
    if (m_Spheres.m_Filter[sphere])
    {
        m_Spheres.Release(sphere);
    }
    else if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(sphere, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::SpriteDestroyed(SQInt32 sprite, SQInt32 header, Object & payload) noexcept
{
    if (m_Sprites.m_Filter[sprite])
    {
        m_Sprites.Release(sprite);
    }
    else if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(sprite, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::TextdrawDestroyed(SQInt32 textdraw, SQInt32 header, Object & payload) noexcept
{
    if (m_Textdraws.m_Filter[textdraw])
    {
        m_Textdraws.Release(textdraw);
    }
    else if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(textdraw, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::VehicleDestroyed(SQInt32 vehicle, SQInt32 header, Object & payload) noexcept
{
    if (m_Vehicles.m_Filter[vehicle])
    {
        m_Vehicles.Release(vehicle);
    }
    else if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(vehicle, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::BlipCustom(SQInt32 blip, SQInt32 header, Object & payload) noexcept
{
    if (!m_Blips.m_Filter[blip] && Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(blip, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::CheckpointCustom(SQInt32 checkpoint, SQInt32 header, Object & payload) noexcept
{
    if (!m_Checkpoints.m_Filter[checkpoint] && Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(checkpoint, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::KeybindCustom(SQInt32 keybind, SQInt32 header, Object & payload) noexcept
{
    if (!m_Keybinds.m_Filter[keybind] && Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(keybind, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ObjectCustom(SQInt32 object, SQInt32 header, Object & payload) noexcept
{
    if (!m_Objects.m_Filter[object] && Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(object, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PickupCustom(SQInt32 pickup, SQInt32 header, Object & payload) noexcept
{
    if (!m_Pickups.m_Filter[pickup] && Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(pickup, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerCustom(SQInt32 player, SQInt32 header, Object & payload) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(player, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::SphereCustom(SQInt32 sphere, SQInt32 header, Object & payload) noexcept
{
    if (!m_Spheres.m_Filter[sphere] && Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(sphere, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::SpriteCustom(SQInt32 sprite, SQInt32 header, Object & payload) noexcept
{
    if (!m_Sprites.m_Filter[sprite] && Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(sprite, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::TextdrawCustom(SQInt32 textdraw, SQInt32 header, Object & payload) noexcept
{
    if (!m_Textdraws.m_Filter[textdraw] && Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(textdraw, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::VehicleCustom(SQInt32 vehicle, SQInt32 header, Object & payload) noexcept
{
    if (!m_Vehicles.m_Filter[vehicle] && Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(vehicle, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerAway(SQInt32 player, bool status) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || status == m_Primary) && (m_Secondary < 0 || status == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, bool >(player, status);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerGameKeys(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || current == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerRename(SQInt32 player, const SQChar * previous, const SQChar * current) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const SQChar *, const SQChar * >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerRequestClass(SQInt32 player, SQInt32 offset) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || offset == m_Primary) && (m_Secondary < 0 || offset == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, offset);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerRequestSpawn(SQInt32 player) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(player);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerSpawn(SQInt32 player) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(player);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerStartTyping(SQInt32 player) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(player);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerStopTyping(SQInt32 player) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(player);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerChat(SQInt32 player, const SQChar * message) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const SQChar * >(player, message);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerCommand(SQInt32 player, const SQChar * command) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const SQChar * >(player, command);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerMessage(SQInt32 player, SQInt32 receiver, const SQChar * message) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Players.m_Filter[receiver];

    if (((m_Confined && res != 3) || (!m_Confined && res != 0)) && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, const SQChar * >(player, receiver, message);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerHealth(SQInt32 player, SQFloat previous, SQFloat current) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQFloat, SQFloat >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerArmour(SQInt32 player, SQFloat previous, SQFloat current) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQFloat, SQFloat >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerWeapon(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || current == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerMove(SQInt32 player, const Vector3 & previous, const Vector3 & current) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const Vector3 &, const Vector3 & >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerWasted(SQInt32 player, SQInt32 reason) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || reason == m_Primary) && (m_Secondary < 0 || reason == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, reason);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerKilled(SQInt32 player, SQInt32 killer, SQInt32 reason, SQInt32 body_part) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Players.m_Filter[killer];

    if (((m_Confined && res != 3) || (!m_Confined && res != 0)) && Trigger() && \
        (m_Primary < 0 || reason == m_Primary) && (m_Secondary < 0 || body_part == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32, SQInt32 >(player, killer, reason, body_part);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerTeamKill(SQInt32 player, SQInt32 killer, SQInt32 reason, SQInt32 body_part) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Players.m_Filter[killer];

    if (((m_Confined && res != 3) || (!m_Confined && res != 0)) && Trigger() && \
        (m_Primary < 0 || reason == m_Primary) && (m_Secondary < 0 || body_part == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32, SQInt32 >(player, killer, reason, body_part);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerSpectate(SQInt32 player, SQInt32 target) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Players.m_Filter[target];

    if (((m_Confined && res != 3) || (!m_Confined && res != 0)) && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, target);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerCrashreport(SQInt32 player, const SQChar * report) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const SQChar * >(player, report);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerBurning(SQInt32 player, bool state) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || state == m_Primary) && (m_Secondary < 0 || state == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, bool >(player, state);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerCrouching(SQInt32 player, bool state) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || state == m_Primary) && (m_Secondary < 0 || state == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, bool >(player, state);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerState(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || current == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PlayerAction(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || current == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::StateNone(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::StateNormal(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::StateShooting(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::StateDriver(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::StatePassenger(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::StateEnterDriver(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::StateEnterPassenger(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::StateExitVehicle(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::StateUnspawned(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ActionNone(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ActionNormal(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ActionAiming(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ActionShooting(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ActionJumping(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ActionLieDown(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ActionGettingUp(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ActionJumpVehicle(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ActionDriving(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ActionDying(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ActionWasted(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ActionEmbarking(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ActionDisembarking(SQInt32 player, SQInt32 previous) noexcept
{
    if (!m_Players.m_Filter[player] && Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::VehicleRespawn(SQInt32 vehicle) noexcept
{
    if (!m_Vehicles.m_Filter[vehicle] && Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(vehicle);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::VehicleExplode(SQInt32 vehicle) noexcept
{
    if (!m_Vehicles.m_Filter[vehicle] && Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(vehicle);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::VehicleHealth(SQInt32 vehicle, SQFloat previous, SQFloat current) noexcept
{
    if (!m_Vehicles.m_Filter[vehicle] && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQFloat, SQFloat >(vehicle, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::VehicleMove(SQInt32 vehicle, const Vector3 & previous, const Vector3 &current) noexcept
{
    if (!m_Vehicles.m_Filter[vehicle] && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const Vector3 &, const Vector3 & >(vehicle, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PickupRespawn(SQInt32 pickup) noexcept
{
    if (!m_Pickups.m_Filter[pickup] && Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(pickup);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::KeybindKeyPress(SQInt32 player, SQInt32 keybind) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Keybinds.m_Filter[keybind];

    if (((m_Confined && res != 3) || (!m_Confined && res != 0)) && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, keybind);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::KeybindKeyRelease(SQInt32 player, SQInt32 keybind) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Keybinds.m_Filter[keybind];

    if (((m_Confined && res == 3) || (!m_Confined && res != 0)) && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, keybind);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::VehicleEmbarking(SQInt32 player, SQInt32 vehicle, SQInt32 slot) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Vehicles.m_Filter[vehicle];

    if (((m_Confined && res == 3) || (!m_Confined && res != 0)) && Trigger() && \
        (m_Primary < 0 || slot == m_Primary) && (m_Secondary < 0 || slot == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, vehicle, slot);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::VehicleEmbarked(SQInt32 player, SQInt32 vehicle, SQInt32 slot) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Vehicles.m_Filter[vehicle];

    if (((m_Confined && res == 3) || (!m_Confined && res != 0)) && Trigger() && \
        (m_Primary < 0 || slot == m_Primary) && (m_Secondary < 0 || slot == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, vehicle, slot);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::VehicleDisembark(SQInt32 player, SQInt32 vehicle) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Vehicles.m_Filter[vehicle];

    if (((m_Confined && res == 3) || (!m_Confined && res != 0)) && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, vehicle);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PickupClaimed(SQInt32 player, SQInt32 pickup) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Pickups.m_Filter[pickup];

    if (((m_Confined && res == 3) || (!m_Confined && res != 0)) && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, pickup);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::PickupCollected(SQInt32 player, SQInt32 pickup) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Pickups.m_Filter[pickup];

    if (((m_Confined && res == 3) || (!m_Confined && res != 0)) && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, pickup);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ObjectShot(SQInt32 player, SQInt32 object, SQInt32 weapon) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Objects.m_Filter[object];

    if (((m_Confined && res == 3) || (!m_Confined && res != 0)) && Trigger() && \
        (m_Primary < 0 || weapon == m_Primary) && (m_Secondary < 0 || weapon == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, object, weapon);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::ObjectBump(SQInt32 player, SQInt32 object) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Objects.m_Filter[object];

    if (((m_Confined && res == 3) || (!m_Confined && res != 0)) && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, object);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::CheckpointEntered(SQInt32 player, SQInt32 checkpoint) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Checkpoints.m_Filter[checkpoint];

    if (((m_Confined && res == 3) || (!m_Confined && res != 0)) && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, checkpoint);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::CheckpointExited(SQInt32 player, SQInt32 checkpoint) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Checkpoints.m_Filter[checkpoint];

    if (((m_Confined && res == 3) || (!m_Confined && res != 0)) && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, checkpoint);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::SphereEntered(SQInt32 player, SQInt32 sphere) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Spheres.m_Filter[sphere];

    if (((m_Confined && res == 3) || (!m_Confined && res != 0)) && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, sphere);
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::SphereExited(SQInt32 player, SQInt32 sphere) noexcept
{
    const char res = (!m_Players.m_Filter[player] << 1) | !m_Spheres.m_Filter[sphere];

    if (((m_Confined && res == 3) || (!m_Confined && res != 0)) && Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, sphere);
    }
}

// ------------------------------------------------------------------------------------------------
bool GlobalEvent::Trigger() noexcept
{
    if (m_Suspended || (m_Idle > std::chrono::steady_clock::now()))
    {
        return false;
    }
    else if (m_Ignore > 0)
    {
        --m_Ignore;
        return false;
    }
    else if (m_OnTrigger.IsNull())
    {
        return false;
    }

    if (m_Stride > 0 && m_Ignore < 1)
    {
        m_Ignore = m_Stride;
    }

    return true;
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::Attach() noexcept
{
    switch (m_Type)
    {
        case EVT_BLIPDESTROYED:
            _Core->BlipDestroyed.Connect< GlobalEvent, &GlobalEvent::BlipDestroyed >(this);
        break;
        case EVT_CHECKPOINTDESTROYED:
            _Core->CheckpointDestroyed.Connect< GlobalEvent, &GlobalEvent::CheckpointDestroyed >(this);
        break;
        case EVT_KEYBINDDESTROYED:
            _Core->KeybindDestroyed.Connect< GlobalEvent, &GlobalEvent::KeybindDestroyed >(this);
        break;
        case EVT_OBJECTDESTROYED:
            _Core->ObjectDestroyed.Connect< GlobalEvent, &GlobalEvent::ObjectDestroyed >(this);
        break;
        case EVT_PICKUPDESTROYED:
            _Core->PickupDestroyed.Connect< GlobalEvent, &GlobalEvent::PickupDestroyed >(this);
        break;
        case EVT_PLAYERDESTROYED:
            _Core->PlayerDestroyed.Connect< GlobalEvent, &GlobalEvent::PlayerDestroyed >(this);
        break;
        case EVT_SPHEREDESTROYED:
            _Core->SphereDestroyed.Connect< GlobalEvent, &GlobalEvent::SphereDestroyed >(this);
        break;
        case EVT_SPRITEDESTROYED:
            _Core->SpriteDestroyed.Connect< GlobalEvent, &GlobalEvent::SpriteDestroyed >(this);
        break;
        case EVT_TEXTDRAWDESTROYED:
            _Core->TextdrawDestroyed.Connect< GlobalEvent, &GlobalEvent::TextdrawDestroyed >(this);
        break;
        case EVT_VEHICLEDESTROYED:
            _Core->VehicleDestroyed.Connect< GlobalEvent, &GlobalEvent::VehicleDestroyed >(this);
        break;
        case EVT_BLIPCUSTOM:
            _Core->BlipCustom.Connect< GlobalEvent, &GlobalEvent::BlipCustom >(this);
        break;
        case EVT_CHECKPOINTCUSTOM:
            _Core->CheckpointCustom.Connect< GlobalEvent, &GlobalEvent::CheckpointCustom >(this);
        break;
        case EVT_KEYBINDCUSTOM:
            _Core->KeybindCustom.Connect< GlobalEvent, &GlobalEvent::KeybindCustom >(this);
        break;
        case EVT_OBJECTCUSTOM:
            _Core->ObjectCustom.Connect< GlobalEvent, &GlobalEvent::ObjectCustom >(this);
        break;
        case EVT_PICKUPCUSTOM:
            _Core->PickupCustom.Connect< GlobalEvent, &GlobalEvent::PickupCustom >(this);
        break;
        case EVT_PLAYERCUSTOM:
            _Core->PlayerCustom.Connect< GlobalEvent, &GlobalEvent::PlayerCustom >(this);
        break;
        case EVT_SPHERECUSTOM:
            _Core->SphereCustom.Connect< GlobalEvent, &GlobalEvent::SphereCustom >(this);
        break;
        case EVT_SPRITECUSTOM:
            _Core->SpriteCustom.Connect< GlobalEvent, &GlobalEvent::SpriteCustom >(this);
        break;
        case EVT_TEXTDRAWCUSTOM:
            _Core->TextdrawCustom.Connect< GlobalEvent, &GlobalEvent::TextdrawCustom >(this);
        break;
        case EVT_VEHICLECUSTOM:
            _Core->VehicleCustom.Connect< GlobalEvent, &GlobalEvent::VehicleCustom >(this);
        break;
        case EVT_PLAYERAWAY:
            _Core->PlayerAway.Connect< GlobalEvent, &GlobalEvent::PlayerAway >(this);
        break;
        case EVT_PLAYERGAMEKEYS:
            _Core->PlayerGameKeys.Connect< GlobalEvent, &GlobalEvent::PlayerGameKeys >(this);
        break;
        case EVT_PLAYERRENAME:
            _Core->PlayerRename.Connect< GlobalEvent, &GlobalEvent::PlayerRename >(this);
        break;
        case EVT_PLAYERREQUESTCLASS:
            _Core->PlayerRequestClass.Connect< GlobalEvent, &GlobalEvent::PlayerRequestClass >(this);
        break;
        case EVT_PLAYERREQUESTSPAWN:
            _Core->PlayerRequestSpawn.Connect< GlobalEvent, &GlobalEvent::PlayerRequestSpawn >(this);
        break;
        case EVT_PLAYERSPAWN:
            _Core->PlayerSpawn.Connect< GlobalEvent, &GlobalEvent::PlayerSpawn >(this);
        break;
        case EVT_PLAYERSTARTTYPING:
            _Core->PlayerStartTyping.Connect< GlobalEvent, &GlobalEvent::PlayerStartTyping >(this);
        break;
        case EVT_PLAYERSTOPTYPING:
            _Core->PlayerStopTyping.Connect< GlobalEvent, &GlobalEvent::PlayerStopTyping >(this);
        break;
        case EVT_PLAYERCHAT:
            _Core->PlayerChat.Connect< GlobalEvent, &GlobalEvent::PlayerChat >(this);
        break;
        case EVT_PLAYERCOMMAND:
            _Core->PlayerCommand.Connect< GlobalEvent, &GlobalEvent::PlayerCommand >(this);
        break;
        case EVT_PLAYERMESSAGE:
            _Core->PlayerMessage.Connect< GlobalEvent, &GlobalEvent::PlayerMessage >(this);
        break;
        case EVT_PLAYERHEALTH:
            _Core->PlayerHealth.Connect< GlobalEvent, &GlobalEvent::PlayerHealth >(this);
        break;
        case EVT_PLAYERARMOUR:
            _Core->PlayerArmour.Connect< GlobalEvent, &GlobalEvent::PlayerArmour >(this);
        break;
        case EVT_PLAYERWEAPON:
            _Core->PlayerWeapon.Connect< GlobalEvent, &GlobalEvent::PlayerWeapon >(this);
        break;
        case EVT_PLAYERMOVE:
            _Core->PlayerMove.Connect< GlobalEvent, &GlobalEvent::PlayerMove >(this);
        break;
        case EVT_PLAYERWASTED:
            _Core->PlayerWasted.Connect< GlobalEvent, &GlobalEvent::PlayerWasted >(this);
        break;
        case EVT_PLAYERKILLED:
            _Core->PlayerKilled.Connect< GlobalEvent, &GlobalEvent::PlayerKilled >(this);
        break;
        case EVT_PLAYERTEAMKILL:
            _Core->PlayerTeamKill.Connect< GlobalEvent, &GlobalEvent::PlayerTeamKill >(this);
        break;
        case EVT_PLAYERSPECTATE:
            _Core->PlayerSpectate.Connect< GlobalEvent, &GlobalEvent::PlayerSpectate >(this);
        break;
        case EVT_PLAYERCRASHREPORT:
            _Core->PlayerCrashreport.Connect< GlobalEvent, &GlobalEvent::PlayerCrashreport >(this);
        break;
        case EVT_PLAYERBURNING:
            _Core->PlayerBurning.Connect< GlobalEvent, &GlobalEvent::PlayerBurning >(this);
        break;
        case EVT_PLAYERCROUCHING:
            _Core->PlayerCrouching.Connect< GlobalEvent, &GlobalEvent::PlayerCrouching >(this);
        break;
        case EVT_PLAYERSTATE:
            _Core->PlayerState.Connect< GlobalEvent, &GlobalEvent::PlayerState >(this);
        break;
        case EVT_PLAYERACTION:
            _Core->PlayerAction.Connect< GlobalEvent, &GlobalEvent::PlayerAction >(this);
        break;
        case EVT_STATENONE:
            _Core->StateNone.Connect< GlobalEvent, &GlobalEvent::StateNone >(this);
        break;
        case EVT_STATENORMAL:
            _Core->StateNormal.Connect< GlobalEvent, &GlobalEvent::StateNormal >(this);
        break;
        case EVT_STATESHOOTING:
            _Core->StateShooting.Connect< GlobalEvent, &GlobalEvent::StateShooting >(this);
        break;
        case EVT_STATEDRIVER:
            _Core->StateDriver.Connect< GlobalEvent, &GlobalEvent::StateDriver >(this);
        break;
        case EVT_STATEPASSENGER:
            _Core->StatePassenger.Connect< GlobalEvent, &GlobalEvent::StatePassenger >(this);
        break;
        case EVT_STATEENTERDRIVER:
            _Core->StateEnterDriver.Connect< GlobalEvent, &GlobalEvent::StateEnterDriver >(this);
        break;
        case EVT_STATEENTERPASSENGER:
            _Core->StateEnterPassenger.Connect< GlobalEvent, &GlobalEvent::StateEnterPassenger >(this);
        break;
        case EVT_STATEEXITVEHICLE:
            _Core->StateExitVehicle.Connect< GlobalEvent, &GlobalEvent::StateExitVehicle >(this);
        break;
        case EVT_STATEUNSPAWNED:
            _Core->StateUnspawned.Connect< GlobalEvent, &GlobalEvent::StateUnspawned >(this);
        break;
        case EVT_ACTIONNONE:
            _Core->ActionNone.Connect< GlobalEvent, &GlobalEvent::ActionNone >(this);
        break;
        case EVT_ACTIONNORMAL:
            _Core->ActionNormal.Connect< GlobalEvent, &GlobalEvent::ActionNormal >(this);
        break;
        case EVT_ACTIONAIMING:
            _Core->ActionAiming.Connect< GlobalEvent, &GlobalEvent::ActionAiming >(this);
        break;
        case EVT_ACTIONSHOOTING:
            _Core->ActionShooting.Connect< GlobalEvent, &GlobalEvent::ActionShooting >(this);
        break;
        case EVT_ACTIONJUMPING:
            _Core->ActionJumping.Connect< GlobalEvent, &GlobalEvent::ActionJumping >(this);
        break;
        case EVT_ACTIONLIEDOWN:
            _Core->ActionLieDown.Connect< GlobalEvent, &GlobalEvent::ActionLieDown >(this);
        break;
        case EVT_ACTIONGETTINGUP:
            _Core->ActionGettingUp.Connect< GlobalEvent, &GlobalEvent::ActionGettingUp >(this);
        break;
        case EVT_ACTIONJUMPVEHICLE:
            _Core->ActionJumpVehicle.Connect< GlobalEvent, &GlobalEvent::ActionJumpVehicle >(this);
        break;
        case EVT_ACTIONDRIVING:
            _Core->ActionDriving.Connect< GlobalEvent, &GlobalEvent::ActionDriving >(this);
        break;
        case EVT_ACTIONDYING:
            _Core->ActionDying.Connect< GlobalEvent, &GlobalEvent::ActionDying >(this);
        break;
        case EVT_ACTIONWASTED:
            _Core->ActionWasted.Connect< GlobalEvent, &GlobalEvent::ActionWasted >(this);
        break;
        case EVT_ACTIONEMBARKING:
            _Core->ActionEmbarking.Connect< GlobalEvent, &GlobalEvent::ActionEmbarking >(this);
        break;
        case EVT_ACTIONDISEMBARKING:
            _Core->ActionDisembarking.Connect< GlobalEvent, &GlobalEvent::ActionDisembarking >(this);
        break;
        case EVT_VEHICLERESPAWN:
            _Core->VehicleRespawn.Connect< GlobalEvent, &GlobalEvent::VehicleRespawn >(this);
        break;
        case EVT_VEHICLEEXPLODE:
            _Core->VehicleExplode.Connect< GlobalEvent, &GlobalEvent::VehicleExplode >(this);
        break;
        case EVT_VEHICLEHEALTH:
            _Core->VehicleHealth.Connect< GlobalEvent, &GlobalEvent::VehicleHealth >(this);
        break;
        case EVT_VEHICLEMOVE:
            _Core->VehicleMove.Connect< GlobalEvent, &GlobalEvent::VehicleMove >(this);
        break;
        case EVT_PICKUPRESPAWN:
            _Core->PickupRespawn.Connect< GlobalEvent, &GlobalEvent::PickupRespawn >(this);
        break;
        case EVT_KEYBINDKEYPRESS:
            _Core->KeybindKeyPress.Connect< GlobalEvent, &GlobalEvent::KeybindKeyPress >(this);
        break;
        case EVT_KEYBINDKEYRELEASE:
            _Core->KeybindKeyRelease.Connect< GlobalEvent, &GlobalEvent::KeybindKeyRelease >(this);
        break;
        case EVT_VEHICLEEMBARKING:
            _Core->VehicleEmbarking.Connect< GlobalEvent, &GlobalEvent::VehicleEmbarking >(this);
        break;
        case EVT_VEHICLEEMBARKED:
            _Core->VehicleEmbarked.Connect< GlobalEvent, &GlobalEvent::VehicleEmbarked >(this);
        break;
        case EVT_VEHICLEDISEMBARK:
            _Core->VehicleDisembark.Connect< GlobalEvent, &GlobalEvent::VehicleDisembark >(this);
        break;
        case EVT_PICKUPCLAIMED:
            _Core->PickupClaimed.Connect< GlobalEvent, &GlobalEvent::PickupClaimed >(this);
        break;
        case EVT_PICKUPCOLLECTED:
            _Core->PickupCollected.Connect< GlobalEvent, &GlobalEvent::PickupCollected >(this);
        break;
        case EVT_OBJECTSHOT:
            _Core->ObjectShot.Connect< GlobalEvent, &GlobalEvent::ObjectShot >(this);
        break;
        case EVT_OBJECTBUMP:
            _Core->ObjectBump.Connect< GlobalEvent, &GlobalEvent::ObjectBump >(this);
        break;
        case EVT_CHECKPOINTENTERED:
            _Core->CheckpointEntered.Connect< GlobalEvent, &GlobalEvent::CheckpointEntered >(this);
        break;
        case EVT_CHECKPOINTEXITED:
            _Core->CheckpointExited.Connect< GlobalEvent, &GlobalEvent::CheckpointExited >(this);
        break;
        case EVT_SPHEREENTERED:
            _Core->SphereEntered.Connect< GlobalEvent, &GlobalEvent::SphereEntered >(this);
        break;
        case EVT_SPHEREEXITED:
            _Core->SphereExited.Connect< GlobalEvent, &GlobalEvent::SphereExited >(this);
        break;
        default:
            LogErr("Attempting to <attach> to an unknown event type: %d", _SCI32(m_Type));
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::Detach() noexcept
{
    switch (m_Type)
    {
        case EVT_BLIPDESTROYED:
            _Core->BlipDestroyed.Disconnect< GlobalEvent, &GlobalEvent::BlipDestroyed >(this);
        break;
        case EVT_CHECKPOINTDESTROYED:
            _Core->CheckpointDestroyed.Disconnect< GlobalEvent, &GlobalEvent::CheckpointDestroyed >(this);
        break;
        case EVT_KEYBINDDESTROYED:
            _Core->KeybindDestroyed.Disconnect< GlobalEvent, &GlobalEvent::KeybindDestroyed >(this);
        break;
        case EVT_OBJECTDESTROYED:
            _Core->ObjectDestroyed.Disconnect< GlobalEvent, &GlobalEvent::ObjectDestroyed >(this);
        break;
        case EVT_PICKUPDESTROYED:
            _Core->PickupDestroyed.Disconnect< GlobalEvent, &GlobalEvent::PickupDestroyed >(this);
        break;
        case EVT_PLAYERDESTROYED:
            _Core->PlayerDestroyed.Disconnect< GlobalEvent, &GlobalEvent::PlayerDestroyed >(this);
        break;
        case EVT_SPHEREDESTROYED:
            _Core->SphereDestroyed.Disconnect< GlobalEvent, &GlobalEvent::SphereDestroyed >(this);
        break;
        case EVT_SPRITEDESTROYED:
            _Core->SpriteDestroyed.Disconnect< GlobalEvent, &GlobalEvent::SpriteDestroyed >(this);
        break;
        case EVT_TEXTDRAWDESTROYED:
            _Core->TextdrawDestroyed.Disconnect< GlobalEvent, &GlobalEvent::TextdrawDestroyed >(this);
        break;
        case EVT_VEHICLEDESTROYED:
            _Core->VehicleDestroyed.Disconnect< GlobalEvent, &GlobalEvent::VehicleDestroyed >(this);
        break;
        case EVT_BLIPCUSTOM:
            _Core->BlipCustom.Disconnect< GlobalEvent, &GlobalEvent::BlipCustom >(this);
        break;
        case EVT_CHECKPOINTCUSTOM:
            _Core->CheckpointCustom.Disconnect< GlobalEvent, &GlobalEvent::CheckpointCustom >(this);
        break;
        case EVT_KEYBINDCUSTOM:
            _Core->KeybindCustom.Disconnect< GlobalEvent, &GlobalEvent::KeybindCustom >(this);
        break;
        case EVT_OBJECTCUSTOM:
            _Core->ObjectCustom.Disconnect< GlobalEvent, &GlobalEvent::ObjectCustom >(this);
        break;
        case EVT_PICKUPCUSTOM:
            _Core->PickupCustom.Disconnect< GlobalEvent, &GlobalEvent::PickupCustom >(this);
        break;
        case EVT_PLAYERCUSTOM:
            _Core->PlayerCustom.Disconnect< GlobalEvent, &GlobalEvent::PlayerCustom >(this);
        break;
        case EVT_SPHERECUSTOM:
            _Core->SphereCustom.Disconnect< GlobalEvent, &GlobalEvent::SphereCustom >(this);
        break;
        case EVT_SPRITECUSTOM:
            _Core->SpriteCustom.Disconnect< GlobalEvent, &GlobalEvent::SpriteCustom >(this);
        break;
        case EVT_TEXTDRAWCUSTOM:
            _Core->TextdrawCustom.Disconnect< GlobalEvent, &GlobalEvent::TextdrawCustom >(this);
        break;
        case EVT_VEHICLECUSTOM:
            _Core->VehicleCustom.Disconnect< GlobalEvent, &GlobalEvent::VehicleCustom >(this);
        break;
        case EVT_PLAYERAWAY:
            _Core->PlayerAway.Disconnect< GlobalEvent, &GlobalEvent::PlayerAway >(this);
        break;
        case EVT_PLAYERGAMEKEYS:
            _Core->PlayerGameKeys.Disconnect< GlobalEvent, &GlobalEvent::PlayerGameKeys >(this);
        break;
        case EVT_PLAYERRENAME:
            _Core->PlayerRename.Disconnect< GlobalEvent, &GlobalEvent::PlayerRename >(this);
        break;
        case EVT_PLAYERREQUESTCLASS:
            _Core->PlayerRequestClass.Disconnect< GlobalEvent, &GlobalEvent::PlayerRequestClass >(this);
        break;
        case EVT_PLAYERREQUESTSPAWN:
            _Core->PlayerRequestSpawn.Disconnect< GlobalEvent, &GlobalEvent::PlayerRequestSpawn >(this);
        break;
        case EVT_PLAYERSPAWN:
            _Core->PlayerSpawn.Disconnect< GlobalEvent, &GlobalEvent::PlayerSpawn >(this);
        break;
        case EVT_PLAYERSTARTTYPING:
            _Core->PlayerStartTyping.Disconnect< GlobalEvent, &GlobalEvent::PlayerStartTyping >(this);
        break;
        case EVT_PLAYERSTOPTYPING:
            _Core->PlayerStopTyping.Disconnect< GlobalEvent, &GlobalEvent::PlayerStopTyping >(this);
        break;
        case EVT_PLAYERCHAT:
            _Core->PlayerChat.Disconnect< GlobalEvent, &GlobalEvent::PlayerChat >(this);
        break;
        case EVT_PLAYERCOMMAND:
            _Core->PlayerCommand.Disconnect< GlobalEvent, &GlobalEvent::PlayerCommand >(this);
        break;
        case EVT_PLAYERMESSAGE:
            _Core->PlayerMessage.Disconnect< GlobalEvent, &GlobalEvent::PlayerMessage >(this);
        break;
        case EVT_PLAYERHEALTH:
            _Core->PlayerHealth.Disconnect< GlobalEvent, &GlobalEvent::PlayerHealth >(this);
        break;
        case EVT_PLAYERARMOUR:
            _Core->PlayerArmour.Disconnect< GlobalEvent, &GlobalEvent::PlayerArmour >(this);
        break;
        case EVT_PLAYERWEAPON:
            _Core->PlayerWeapon.Disconnect< GlobalEvent, &GlobalEvent::PlayerWeapon >(this);
        break;
        case EVT_PLAYERMOVE:
            _Core->PlayerMove.Disconnect< GlobalEvent, &GlobalEvent::PlayerMove >(this);
        break;
        case EVT_PLAYERWASTED:
            _Core->PlayerWasted.Disconnect< GlobalEvent, &GlobalEvent::PlayerWasted >(this);
        break;
        case EVT_PLAYERKILLED:
            _Core->PlayerKilled.Disconnect< GlobalEvent, &GlobalEvent::PlayerKilled >(this);
        break;
        case EVT_PLAYERTEAMKILL:
            _Core->PlayerTeamKill.Disconnect< GlobalEvent, &GlobalEvent::PlayerTeamKill >(this);
        break;
        case EVT_PLAYERSPECTATE:
            _Core->PlayerSpectate.Disconnect< GlobalEvent, &GlobalEvent::PlayerSpectate >(this);
        break;
        case EVT_PLAYERCRASHREPORT:
            _Core->PlayerCrashreport.Disconnect< GlobalEvent, &GlobalEvent::PlayerCrashreport >(this);
        break;
        case EVT_PLAYERBURNING:
            _Core->PlayerBurning.Disconnect< GlobalEvent, &GlobalEvent::PlayerBurning >(this);
        break;
        case EVT_PLAYERCROUCHING:
            _Core->PlayerCrouching.Disconnect< GlobalEvent, &GlobalEvent::PlayerCrouching >(this);
        break;
        case EVT_PLAYERSTATE:
            _Core->PlayerState.Disconnect< GlobalEvent, &GlobalEvent::PlayerState >(this);
        break;
        case EVT_PLAYERACTION:
            _Core->PlayerAction.Disconnect< GlobalEvent, &GlobalEvent::PlayerAction >(this);
        break;
        case EVT_STATENONE:
            _Core->StateNone.Disconnect< GlobalEvent, &GlobalEvent::StateNone >(this);
        break;
        case EVT_STATENORMAL:
            _Core->StateNormal.Disconnect< GlobalEvent, &GlobalEvent::StateNormal >(this);
        break;
        case EVT_STATESHOOTING:
            _Core->StateShooting.Disconnect< GlobalEvent, &GlobalEvent::StateShooting >(this);
        break;
        case EVT_STATEDRIVER:
            _Core->StateDriver.Disconnect< GlobalEvent, &GlobalEvent::StateDriver >(this);
        break;
        case EVT_STATEPASSENGER:
            _Core->StatePassenger.Disconnect< GlobalEvent, &GlobalEvent::StatePassenger >(this);
        break;
        case EVT_STATEENTERDRIVER:
            _Core->StateEnterDriver.Disconnect< GlobalEvent, &GlobalEvent::StateEnterDriver >(this);
        break;
        case EVT_STATEENTERPASSENGER:
            _Core->StateEnterPassenger.Disconnect< GlobalEvent, &GlobalEvent::StateEnterPassenger >(this);
        break;
        case EVT_STATEEXITVEHICLE:
            _Core->StateExitVehicle.Disconnect< GlobalEvent, &GlobalEvent::StateExitVehicle >(this);
        break;
        case EVT_STATEUNSPAWNED:
            _Core->StateUnspawned.Disconnect< GlobalEvent, &GlobalEvent::StateUnspawned >(this);
        break;
        case EVT_ACTIONNONE:
            _Core->ActionNone.Disconnect< GlobalEvent, &GlobalEvent::ActionNone >(this);
        break;
        case EVT_ACTIONNORMAL:
            _Core->ActionNormal.Disconnect< GlobalEvent, &GlobalEvent::ActionNormal >(this);
        break;
        case EVT_ACTIONAIMING:
            _Core->ActionAiming.Disconnect< GlobalEvent, &GlobalEvent::ActionAiming >(this);
        break;
        case EVT_ACTIONSHOOTING:
            _Core->ActionShooting.Disconnect< GlobalEvent, &GlobalEvent::ActionShooting >(this);
        break;
        case EVT_ACTIONJUMPING:
            _Core->ActionJumping.Disconnect< GlobalEvent, &GlobalEvent::ActionJumping >(this);
        break;
        case EVT_ACTIONLIEDOWN:
            _Core->ActionLieDown.Disconnect< GlobalEvent, &GlobalEvent::ActionLieDown >(this);
        break;
        case EVT_ACTIONGETTINGUP:
            _Core->ActionGettingUp.Disconnect< GlobalEvent, &GlobalEvent::ActionGettingUp >(this);
        break;
        case EVT_ACTIONJUMPVEHICLE:
            _Core->ActionJumpVehicle.Disconnect< GlobalEvent, &GlobalEvent::ActionJumpVehicle >(this);
        break;
        case EVT_ACTIONDRIVING:
            _Core->ActionDriving.Disconnect< GlobalEvent, &GlobalEvent::ActionDriving >(this);
        break;
        case EVT_ACTIONDYING:
            _Core->ActionDying.Disconnect< GlobalEvent, &GlobalEvent::ActionDying >(this);
        break;
        case EVT_ACTIONWASTED:
            _Core->ActionWasted.Disconnect< GlobalEvent, &GlobalEvent::ActionWasted >(this);
        break;
        case EVT_ACTIONEMBARKING:
            _Core->ActionEmbarking.Disconnect< GlobalEvent, &GlobalEvent::ActionEmbarking >(this);
        break;
        case EVT_ACTIONDISEMBARKING:
            _Core->ActionDisembarking.Disconnect< GlobalEvent, &GlobalEvent::ActionDisembarking >(this);
        break;
        case EVT_VEHICLERESPAWN:
            _Core->VehicleRespawn.Disconnect< GlobalEvent, &GlobalEvent::VehicleRespawn >(this);
        break;
        case EVT_VEHICLEEXPLODE:
            _Core->VehicleExplode.Disconnect< GlobalEvent, &GlobalEvent::VehicleExplode >(this);
        break;
        case EVT_VEHICLEHEALTH:
            _Core->VehicleHealth.Disconnect< GlobalEvent, &GlobalEvent::VehicleHealth >(this);
        break;
        case EVT_VEHICLEMOVE:
            _Core->VehicleMove.Disconnect< GlobalEvent, &GlobalEvent::VehicleMove >(this);
        break;
        case EVT_PICKUPRESPAWN:
            _Core->PickupRespawn.Disconnect< GlobalEvent, &GlobalEvent::PickupRespawn >(this);
        break;
        case EVT_KEYBINDKEYPRESS:
            _Core->KeybindKeyPress.Disconnect< GlobalEvent, &GlobalEvent::KeybindKeyPress >(this);
        break;
        case EVT_KEYBINDKEYRELEASE:
            _Core->KeybindKeyRelease.Disconnect< GlobalEvent, &GlobalEvent::KeybindKeyRelease >(this);
        break;
        case EVT_VEHICLEEMBARKING:
            _Core->VehicleEmbarking.Disconnect< GlobalEvent, &GlobalEvent::VehicleEmbarking >(this);
        break;
        case EVT_VEHICLEEMBARKED:
            _Core->VehicleEmbarked.Disconnect< GlobalEvent, &GlobalEvent::VehicleEmbarked >(this);
        break;
        case EVT_VEHICLEDISEMBARK:
            _Core->VehicleDisembark.Disconnect< GlobalEvent, &GlobalEvent::VehicleDisembark >(this);
        break;
        case EVT_PICKUPCLAIMED:
            _Core->PickupClaimed.Disconnect< GlobalEvent, &GlobalEvent::PickupClaimed >(this);
        break;
        case EVT_PICKUPCOLLECTED:
            _Core->PickupCollected.Disconnect< GlobalEvent, &GlobalEvent::PickupCollected >(this);
        break;
        case EVT_OBJECTSHOT:
            _Core->ObjectShot.Disconnect< GlobalEvent, &GlobalEvent::ObjectShot >(this);
        break;
        case EVT_OBJECTBUMP:
            _Core->ObjectBump.Disconnect< GlobalEvent, &GlobalEvent::ObjectBump >(this);
        break;
        case EVT_CHECKPOINTENTERED:
            _Core->CheckpointEntered.Disconnect< GlobalEvent, &GlobalEvent::CheckpointEntered >(this);
        break;
        case EVT_CHECKPOINTEXITED:
            _Core->CheckpointExited.Disconnect< GlobalEvent, &GlobalEvent::CheckpointExited >(this);
        break;
        case EVT_SPHEREENTERED:
            _Core->SphereEntered.Disconnect< GlobalEvent, &GlobalEvent::SphereEntered >(this);
        break;
        case EVT_SPHEREEXITED:
            _Core->SphereExited.Disconnect< GlobalEvent, &GlobalEvent::SphereExited >(this);
        break;
        default:
            LogErr("Attempting to <detach> from an unknown event type: %d", _SCI32(m_Type));
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::Hook() noexcept
{
    if (Ent< CBlip >::InEvent(m_Type))
    {
        m_Blips.Hook();
    }

    if (Ent< CCheckpoint >::InEvent(m_Type))
    {
        m_Checkpoints.Hook();
    }

    if (Ent< CKeybind >::InEvent(m_Type))
    {
        m_Keybinds.Hook();
    }

    if (Ent< CObject >::InEvent(m_Type))
    {
        m_Objects.Hook();
    }

    if (Ent< CPickup >::InEvent(m_Type))
    {
        m_Pickups.Hook();
    }

    if (Ent< CPlayer >::InEvent(m_Type))
    {
        m_Players.Hook();
    }

    if (Ent< CSphere >::InEvent(m_Type))
    {
        m_Spheres.Hook();
    }

    if (Ent< CSprite >::InEvent(m_Type))
    {
        m_Sprites.Hook();
    }

    if (Ent< CTextdraw >::InEvent(m_Type))
    {
        m_Textdraws.Hook();
    }

    if (Ent< CVehicle >::InEvent(m_Type))
    {
        m_Vehicles.Hook();
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::Unhook() noexcept
{
    if (Ent< CBlip >::InEvent(m_Type))
    {
        m_Blips.Unhook();
    }

    if (Ent< CCheckpoint >::InEvent(m_Type))
    {
        m_Checkpoints.Unhook();
    }

    if (Ent< CKeybind >::InEvent(m_Type))
    {
        m_Keybinds.Unhook();
    }

    if (Ent< CObject >::InEvent(m_Type))
    {
        m_Objects.Unhook();
    }

    if (Ent< CPickup >::InEvent(m_Type))
    {
        m_Pickups.Unhook();
    }

    if (Ent< CPlayer >::InEvent(m_Type))
    {
        m_Players.Unhook();
    }

    if (Ent< CSphere >::InEvent(m_Type))
    {
        m_Spheres.Unhook();
    }

    if (Ent< CSprite >::InEvent(m_Type))
    {
        m_Sprites.Unhook();
    }

    if (Ent< CTextdraw >::InEvent(m_Type))
    {
        m_Textdraws.Unhook();
    }

    if (Ent< CVehicle >::InEvent(m_Type))
    {
        m_Vehicles.Unhook();
    }
}

// ------------------------------------------------------------------------------------------------
void GlobalEvent::Adaptable(SQInt32 type) noexcept
{
    if (Ent< CBlip >::InEvent(m_Type) && !Ent< CBlip >::InEvent(type))
    {
        m_Blips.Clear(0);
    }

    if (Ent< CCheckpoint >::InEvent(m_Type) && !Ent< CCheckpoint >::InEvent(type))
    {
        m_Checkpoints.Clear(0);
    }

    if (Ent< CKeybind >::InEvent(m_Type) && !Ent< CKeybind >::InEvent(type))
    {
        m_Keybinds.Clear(0);
    }

    if (Ent< CObject >::InEvent(m_Type) && !Ent< CObject >::InEvent(type))
    {
        m_Objects.Clear(0);
    }

    if (Ent< CPickup >::InEvent(m_Type) && !Ent< CPickup >::InEvent(type))
    {
        m_Pickups.Clear(0);
    }

    if (Ent< CPlayer >::InEvent(m_Type) && !Ent< CPlayer >::InEvent(type))
    {
        m_Players.Clear(0);
    }

    if (Ent< CSphere >::InEvent(m_Type) && !Ent< CSphere >::InEvent(type))
    {
        m_Spheres.Clear(0);
    }

    if (Ent< CSprite >::InEvent(m_Type) && !Ent< CSprite >::InEvent(type))
    {
        m_Sprites.Clear(0);
    }

    if (Ent< CTextdraw >::InEvent(m_Type) && !Ent< CTextdraw >::InEvent(type))
    {
        m_Textdraws.Clear(0);
    }

    if (Ent< CVehicle >::InEvent(m_Type) && !Ent< CVehicle >::InEvent(type))
    {
        m_Vehicles.Clear(0);
    }
}

// ================================================================================================
template < class T > static bool Register_GlobalFilter(HSQUIRRELVM vm, const SQChar * cname)
{
    // Output debugging information
    LogDbg("Beginning registration of <%s> type", cname);
    // Avoid using the long name of the type we're about to register
    typedef GlobalFilter< T > Filter;
    // Filters should not be constructed to avoid further complications
    typedef NoConstructor< Filter > Allocator;
    // Attempt to register the specified filtertype
    Sqrat::RootTable(vm).Bind(cname, Sqrat::Class< Filter, Allocator >(vm, cname)
        .Func(_SC("_cmp"), &Filter::Cmp)
        .Func(_SC("_tostring"), &Filter::ToString)

        .Prop(_SC("count"), &Filter::Count)
        .Prop(_SC("any"), &Filter::Any)
        .Prop(_SC("none"), &Filter::None)
        .Prop(_SC("all"), &Filter::All)

        .template Overload< bool (Filter::*)(const typename Filter::RefType &) >(_SC("include"), &Filter::Include)
        .template Overload< bool (Filter::*)(const typename Filter::RefType &, SQInt32) >(_SC("include"), &Filter::Include)
        .template Overload< bool (Filter::*)(const typename Filter::RefType &) >(_SC("exclude"), &Filter::Exclude)
        .template Overload< bool (Filter::*)(const typename Filter::RefType &, SQInt32) >(_SC("exclude"), &Filter::Exclude)
        //.Func(_SC("exclude"), &Filter::Exclude)
        .Func(_SC("enabled"), &Filter::Enabled)
        .Func(_SC("clear"), &Filter::Clear)
        .Func(_SC("flip"), &Filter::Flip)
    );
    // Output debugging information
    LogDbg("Registration of <%s> type was successful", cname);
    // Registration succeeded
    return true;
}

bool Register_GlobalEvent(HSQUIRRELVM vm)
{
    // Register dependencies
    if (!Register_GlobalFilter< CBlip >(vm, _SC("BlipGlobalFilter")) || \
        !Register_GlobalFilter< CCheckpoint >(vm, _SC("CheckpointGlobalFilter")) || \
        !Register_GlobalFilter< CKeybind >(vm, _SC("KeybindGlobalFilter")) || \
        !Register_GlobalFilter< CObject >(vm, _SC("ObjectGlobalFilter")) || \
        !Register_GlobalFilter< CPickup >(vm, _SC("PickupGlobalFilter")) || \
        !Register_GlobalFilter< CPlayer >(vm, _SC("PlayerGlobalFilter")) || \
        !Register_GlobalFilter< CSphere >(vm, _SC("SphereGlobalFilter")) || \
        !Register_GlobalFilter< CSprite >(vm, _SC("SpriteGlobalFilter")) || \
        !Register_GlobalFilter< CTextdraw >(vm, _SC("TextdrawGlobalFilter")) || \
        !Register_GlobalFilter< CVehicle >(vm, _SC("VehicleGlobalFilter")))
    {
        return false;
    }
    // Output debugging information
    LogDbg("Beginning registration of <GlobalEvent> type");
    // Events should not be copied for the sake of simplicity
    typedef NoCopy< GlobalEvent > Allocator;
    // Attempt to register the specified type
    Sqrat::RootTable(vm).Bind(_SC("GlobalEvent"), Sqrat::Class< GlobalEvent, Allocator >(vm, _SC("GlobalEvent"))
        .Ctor()
        .Ctor<SQInt32>()
        .Ctor<SQInt32, bool>()

        .Func(_SC("_cmp"), &GlobalEvent::Cmp)
        .Func(_SC("_tostring"), &GlobalEvent::GetName)

        .Prop(_SC("ltag"), &GlobalEvent::GetTag, &GlobalEvent::SetTag)
        .Prop(_SC("ldata"), &GlobalEvent::GetData, &GlobalEvent::SetData)
        .Prop(_SC("type"), &GlobalEvent::GetType, &GlobalEvent::SetType)
        .Prop(_SC("idle"), &GlobalEvent::GetIdle, &GlobalEvent::SetIdle)
        .Prop(_SC("is_idle"), &GlobalEvent::IsIdle)
        .Prop(_SC("stride"), &GlobalEvent::GetStride, &GlobalEvent::SetStride)
        .Prop(_SC("ignore"), &GlobalEvent::GetIgnore, &GlobalEvent::SetIgnore)
        .Prop(_SC("primary"), &GlobalEvent::GetPrimary, &GlobalEvent::SetPrimary)
        .Prop(_SC("secondary"), &GlobalEvent::GetSecondary, &GlobalEvent::SetSecondary)
        .Prop(_SC("confined"), &GlobalEvent::GetConfined, &GlobalEvent::SetConfined)
        .Prop(_SC("suspended"), &GlobalEvent::GetSuspended, &GlobalEvent::SetSuspended)
        .Prop(_SC("compatible"), &GlobalEvent::Compatible)
        .Prop(_SC("name"), &GlobalEvent::GetName)

        .Prop(_SC("on_trigger"), &GlobalEvent::GetOnTrigger, &GlobalEvent::SetOnTrigger)
        .Prop(_SC("on_include"), &GlobalEvent::GetOnInclude, &GlobalEvent::SetOnInclude)
        .Prop(_SC("on_exclude"), &GlobalEvent::GetOnExclude, &GlobalEvent::SetOnExclude)
        .Prop(_SC("on_cleared"), &GlobalEvent::GetOnCleared, &GlobalEvent::SetOnCleared)
        .Prop(_SC("on_release"), &GlobalEvent::GetOnRelease, &GlobalEvent::SetOnRelease)

        .Prop(_SC("blips"), &GlobalEvent::GetBlipFilter)
        .Prop(_SC("checkpoints"), &GlobalEvent::GetCheckpointFilter)
        .Prop(_SC("keybinds"), &GlobalEvent::GetKeybindFilter)
        .Prop(_SC("objects"), &GlobalEvent::GetObjectFilter)
        .Prop(_SC("pickups"), &GlobalEvent::GetPickupFilter)
        .Prop(_SC("players"), &GlobalEvent::GetPlayerFilter)
        .Prop(_SC("spheres"), &GlobalEvent::GetSphereFilter)
        .Prop(_SC("sprites"), &GlobalEvent::GetSpriteFilter)
        .Prop(_SC("textdraws"), &GlobalEvent::GetTextdrawFilter)
        .Prop(_SC("vehicles"), &GlobalEvent::GetVehicleFilter)
    );
    // Output debugging information
    LogDbg("Registration of <GlobalEvent> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
