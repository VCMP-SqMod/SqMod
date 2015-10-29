#include "Event/Local.hpp"
#include "Register.hpp"
#include "Entity.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
LocalEvent::LocalEvent() noexcept
    : LocalEvent(EVT_UNKNOWN, false)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
LocalEvent::LocalEvent(SQInt32 type) noexcept
    : LocalEvent(type, false)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
LocalEvent::LocalEvent(SQInt32 type, bool suspended) noexcept
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
    , m_Inversed(false)
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
    // Receive notification when the VM is about to be closed to release object references
    _Core->VMClose.Connect< LocalEvent, &LocalEvent::VMClose >(this);
    /* Entity filters are empty so there's nothing to hook right now! */
}

// ------------------------------------------------------------------------------------------------
LocalEvent::~LocalEvent()
{
    // Detach from all attached signals
    Detach();
    // Stop receiving notification when the VM is about to be closed
    _Core->VMClose.Disconnect< LocalEvent, &LocalEvent::VMClose >(this);
    /* The entity filters should to unhook themselves from the destroy signal! */
}

// ------------------------------------------------------------------------------------------------
bool LocalEvent::operator == (const LocalEvent & o) const noexcept
{
    return (m_Type == o.m_Type);
}

// ------------------------------------------------------------------------------------------------
bool LocalEvent::operator != (const LocalEvent & o) const noexcept
{
    return (m_Type != o.m_Type);
}

// ------------------------------------------------------------------------------------------------
bool LocalEvent::operator < (const LocalEvent & o) const noexcept
{
    return (m_Type < o.m_Type);
}

// ------------------------------------------------------------------------------------------------
bool LocalEvent::operator > (const LocalEvent & o) const noexcept
{
    return (m_Type > o.m_Type);
}

// ------------------------------------------------------------------------------------------------
bool LocalEvent::operator <= (const LocalEvent & o) const noexcept
{
    return (m_Type <= o.m_Type);
}

// ------------------------------------------------------------------------------------------------
bool LocalEvent::operator >= (const LocalEvent & o) const noexcept
{
    return (m_Type >= o.m_Type);
}

// ------------------------------------------------------------------------------------------------
SQInt32 LocalEvent::Cmp(const LocalEvent & o) const noexcept
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
const SQChar * LocalEvent::GetName() const noexcept
{
    return GetEventName(m_Type);
}

// ------------------------------------------------------------------------------------------------
const SQChar * LocalEvent::GetTag() const noexcept
{
    return m_Tag.c_str();
}

void LocalEvent::SetTag(const SQChar * tag) noexcept
{
    m_Tag = tag;
}

// ------------------------------------------------------------------------------------------------
SqObj & LocalEvent::GetData() noexcept
{
    return m_Data;
}

void LocalEvent::SetData(SqObj & data) noexcept
{
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
SQInt32 LocalEvent::GetType() const noexcept
{
    return m_Type;
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::SetType(SQInt32 type) noexcept
{
    // Make sure the newly specified event is compatible
    if (!Compatible(type))
    {
        LogErr("Cannot change the event to an incompatible type: %s", GetEventName(type));
    }
    else
    {
        // Clear anything that cannot adapt to the new event type
        Adaptable(type, m_Inversed);
        // Detach from the current event type
        Detach();
        // Set the new event type
        m_Type = type;
        // Attach to the new event type
        Attach();
    }
}

// ------------------------------------------------------------------------------------------------
SQInteger LocalEvent::GetIdle() const noexcept
{
    return _SCSQI(std::chrono::duration_cast<std::chrono::milliseconds>(m_Idle - std::chrono::steady_clock::now()).count());
}

void LocalEvent::SetIdle(SQInteger millis) noexcept
{
    m_Idle = (std::chrono::steady_clock::now() + std::chrono::milliseconds(_SCI64(millis)));
}

bool LocalEvent::IsIdle() const noexcept
{
    return (m_Idle > std::chrono::steady_clock::now());
}

// ------------------------------------------------------------------------------------------------
SQInt32 LocalEvent::GetStride() const noexcept
{
    return m_Stride;
}

void LocalEvent::SetStride(SQInt32 stride) noexcept
{
    m_Stride = stride > 0 ? stride : 0;
}

// ------------------------------------------------------------------------------------------------
SQInt32 LocalEvent::GetIgnore() const noexcept
{
    return m_Ignore;
}

void LocalEvent::SetIgnore(SQInt32 ignore) noexcept
{
    m_Ignore = ignore > 0 ? ignore : 0;
}

// ------------------------------------------------------------------------------------------------
SQInt32 LocalEvent::GetPrimary() const noexcept
{
    return m_Primary;
}

void LocalEvent::SetPrimary(SQInt32 subset) noexcept
{
    m_Primary = subset;
}

// ------------------------------------------------------------------------------------------------
SQInt32 LocalEvent::GetSecondary() const noexcept
{
    return m_Secondary;
}

void LocalEvent::SetSecondary(SQInt32 subset) noexcept
{
    m_Secondary = subset;
}

// ------------------------------------------------------------------------------------------------
bool LocalEvent::GetSuspended() const noexcept
{
    return m_Suspended;
}

void LocalEvent::SetSuspended(bool toggle) noexcept
{
    m_Suspended = toggle;
}

// ------------------------------------------------------------------------------------------------
bool LocalEvent::GetInversed() const noexcept
{
    return m_Inversed;
}

void LocalEvent::SetInversed(bool toggle) noexcept
{
    // See if it this is an invertible event and the specified option is different
    if (CanBeInversed(m_Type) && m_Inversed != toggle)
    {
        // Clear anything that cannot adapt to the new event type
        Adaptable(m_Type, toggle);
        // Toggle the inversed option
        m_Inversed = toggle;
        // Attach back to the new event type
        Attach();
    }
    // Just set the option to what was requested
    else
    {
        m_Inversed = toggle;
    }
}

// ------------------------------------------------------------------------------------------------
Function LocalEvent::GetOnTrigger() const noexcept
{
    return m_OnTrigger;
}

void LocalEvent::SetOnTrigger(const Function & func) noexcept
{
    m_OnTrigger = func;
}

// ------------------------------------------------------------------------------------------------
Function LocalEvent::GetOnInclude() const noexcept
{
    return m_OnInclude;
}

void LocalEvent::SetOnInclude(const Function & func) noexcept
{
    m_OnInclude = func;
}

// ------------------------------------------------------------------------------------------------
Function LocalEvent::GetOnExclude() const noexcept
{
    return m_OnExclude;
}

void LocalEvent::SetOnExclude(const Function & func) noexcept
{
    m_OnExclude = func;
}

// ------------------------------------------------------------------------------------------------
Function LocalEvent::GetOnCleared() const noexcept
{
    return m_OnCleared;
}

void LocalEvent::SetOnCleared(const Function & func) noexcept
{
    m_OnCleared = func;
}

// ------------------------------------------------------------------------------------------------
Function LocalEvent::GetOnRelease() const noexcept
{
    return m_OnRelease;
}

void LocalEvent::SetOnRelease(const Function & func) noexcept
{
    m_OnRelease = func;
}

// ------------------------------------------------------------------------------------------------
bool LocalEvent::Compatible(SQInt32 type) const noexcept
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
LocalEvent::BlipFilter & LocalEvent::GetBlipFilter() noexcept
{
    return m_Blips; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
LocalEvent::CheckpointFilter & LocalEvent::GetCheckpointFilter() noexcept
{
    return m_Checkpoints; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
LocalEvent::KeybindFilter & LocalEvent::GetKeybindFilter() noexcept
{
    return m_Keybinds; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
LocalEvent::ObjectFilter & LocalEvent::GetObjectFilter() noexcept
{
    return m_Objects; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
LocalEvent::PickupFilter & LocalEvent::GetPickupFilter() noexcept
{
    return m_Pickups; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
LocalEvent::PlayerFilter & LocalEvent::GetPlayerFilter() noexcept
{
    return m_Players; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
LocalEvent::SphereFilter & LocalEvent::GetSphereFilter() noexcept
{
    return m_Spheres; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
LocalEvent::SpriteFilter & LocalEvent::GetSpriteFilter() noexcept
{
    return m_Sprites; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
LocalEvent::TextdrawFilter & LocalEvent::GetTextdrawFilter() noexcept
{
    return m_Textdraws; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
LocalEvent::VehicleFilter & LocalEvent::GetVehicleFilter() noexcept
{
    return m_Vehicles; /* This reference should not be stored anywhere! */
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::BlipDestroyed(SQInt32 blip, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(blip, header, payload);
    }
    m_Blips.Release(blip);
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::CheckpointDestroyed(SQInt32 checkpoint, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(checkpoint, header, payload);
    }
    m_Checkpoints.Release(checkpoint);
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::KeybindDestroyed(SQInt32 keybind, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(keybind, header, payload);
    }
    m_Keybinds.Release(keybind);
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ObjectDestroyed(SQInt32 object, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(object, header, payload);
    }
    m_Objects.Release(object);
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PickupDestroyed(SQInt32 pickup, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(pickup, header, payload);
    }
    m_Pickups.Release(pickup);
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerDestroyed(SQInt32 player, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(player, header, payload);
    }
    m_Players.Release(player);
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::SphereDestroyed(SQInt32 sphere, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(sphere, header, payload);
    }
    m_Spheres.Release(sphere);
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::SpriteDestroyed(SQInt32 sprite, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(sprite, header, payload);
    }
    m_Sprites.Release(sprite);
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::TextdrawDestroyed(SQInt32 textdraw, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(textdraw, header, payload);
    }
    m_Textdraws.Release(textdraw);
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::VehicleDestroyed(SQInt32 vehicle, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
            (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(vehicle, header, payload);
    }
    m_Vehicles.Release(vehicle);
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::BlipCustom(SQInt32 blip, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(blip, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::CheckpointCustom(SQInt32 checkpoint, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(checkpoint, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::KeybindCustom(SQInt32 keybind, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(keybind, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ObjectCustom(SQInt32 object, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(object, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PickupCustom(SQInt32 pickup, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(pickup, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerCustom(SQInt32 player, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(player, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::SphereCustom(SQInt32 sphere, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(sphere, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::SpriteCustom(SQInt32 sprite, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(sprite, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::TextdrawCustom(SQInt32 textdraw, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(textdraw, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::VehicleCustom(SQInt32 vehicle, SQInt32 header, Object & payload) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(vehicle, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerAway(SQInt32 player, bool status) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || status == m_Primary) && (m_Secondary < 0 || status == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, bool >(player, status);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerGameKeys(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || current == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerRename(SQInt32 player, const SQChar * previous, const SQChar * current) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const SQChar *, const SQChar * >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerRequestClass(SQInt32 player, SQInt32 offset) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || offset == m_Primary) && (m_Secondary < 0 || offset == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, offset);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerRequestSpawn(SQInt32 player) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(player);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerSpawn(SQInt32 player) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(player);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerStartTyping(SQInt32 player) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(player);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerStopTyping(SQInt32 player) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(player);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerChat(SQInt32 player, const SQChar * message) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const SQChar * >(player, message);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerCommand(SQInt32 player, const SQChar * command) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const SQChar * >(player, command);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerMessage(SQInt32 player, SQInt32 receiver, const SQChar * message) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, const SQChar * >(player, receiver, message);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerHealth(SQInt32 player, SQFloat previous, SQFloat current) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQFloat, SQFloat >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerArmour(SQInt32 player, SQFloat previous, SQFloat current) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQFloat, SQFloat >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerWeapon(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || current == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerMove(SQInt32 player, const Vector3 & previous, const Vector3 & current) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const Vector3 &, const Vector3 & >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerWasted(SQInt32 player, SQInt32 reason) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || reason == m_Primary) && (m_Secondary < 0 || reason == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, reason);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerKilled(SQInt32 player, SQInt32 killer, SQInt32 reason, SQInt32 body_part) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || reason == m_Primary) && (m_Secondary < 0 || body_part == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32, SQInt32 >(player, killer, reason, body_part);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerTeamKill(SQInt32 player, SQInt32 killer, SQInt32 reason, SQInt32 body_part) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || reason == m_Primary) && (m_Secondary < 0 || body_part == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32, SQInt32 >(player, killer, reason, body_part);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerSpectate(SQInt32 player, SQInt32 target) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, target);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerCrashreport(SQInt32 player, const SQChar * report) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const SQChar * >(player, report);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerBurning(SQInt32 player, bool state) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || state == m_Primary) && (m_Secondary < 0 || state == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, bool >(player, state);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerCrouching(SQInt32 player, bool state) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || state == m_Primary) && (m_Secondary < 0 || state == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, bool >(player, state);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerState(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || current == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PlayerAction(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || current == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::StateNone(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::StateNormal(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::StateShooting(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::StateDriver(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::StatePassenger(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::StateEnterDriver(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::StateEnterPassenger(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::StateExitVehicle(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::StateUnspawned(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ActionNone(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ActionNormal(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ActionAiming(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ActionShooting(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ActionJumping(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ActionLieDown(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ActionGettingUp(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ActionJumpVehicle(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ActionDriving(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ActionDying(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ActionWasted(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ActionEmbarking(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ActionDisembarking(SQInt32 player, SQInt32 previous) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::VehicleRespawn(SQInt32 vehicle) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(vehicle);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::VehicleExplode(SQInt32 vehicle) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(vehicle);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::VehicleHealth(SQInt32 vehicle, SQFloat previous, SQFloat current) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQFloat, SQFloat >(vehicle, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::VehicleMove(SQInt32 vehicle, const Vector3 & previous, const Vector3 &current) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const Vector3 &, const Vector3 & >(vehicle, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PickupRespawn(SQInt32 pickup) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(pickup);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::KeybindKeyPress(SQInt32 player, SQInt32 keybind) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, keybind);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::KeybindKeyRelease(SQInt32 player, SQInt32 keybind) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, keybind);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::VehicleEmbarking(SQInt32 player, SQInt32 vehicle, SQInt32 slot) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || slot == m_Primary) && (m_Secondary < 0 || slot == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, vehicle, slot);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::VehicleEmbarked(SQInt32 player, SQInt32 vehicle, SQInt32 slot) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || slot == m_Primary) && (m_Secondary < 0 || slot == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, vehicle, slot);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::VehicleDisembark(SQInt32 player, SQInt32 vehicle) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, vehicle);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PickupClaimed(SQInt32 player, SQInt32 pickup) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, pickup);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::PickupCollected(SQInt32 player, SQInt32 pickup) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, pickup);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ObjectShot(SQInt32 player, SQInt32 object, SQInt32 weapon) noexcept
{
    if (Trigger() && \
        (m_Primary < 0 || weapon == m_Primary) && (m_Secondary < 0 || weapon == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, object, weapon);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::ObjectBump(SQInt32 player, SQInt32 object) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, object);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::CheckpointEntered(SQInt32 player, SQInt32 checkpoint) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, checkpoint);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::CheckpointExited(SQInt32 player, SQInt32 checkpoint) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, checkpoint);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::SphereEntered(SQInt32 player, SQInt32 sphere) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, sphere);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::SphereExited(SQInt32 player, SQInt32 sphere) noexcept
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, sphere);
    }
}

// ------------------------------------------------------------------------------------------------
bool LocalEvent::Trigger() noexcept
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
void LocalEvent::Attach() noexcept
{
    // Filters take care of both attaching and hooking for local filters
    Hook();
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::Attach(SQInt32 id) noexcept
{
    switch (m_Type)
    {
        case EVT_BLIPDESTROYED:
            Reference< CBlip >::Get(id).BlipDestroyed.Connect< LocalEvent, &LocalEvent::BlipDestroyed >(this);
        break;
        case EVT_CHECKPOINTDESTROYED:
            Reference< CCheckpoint >::Get(id).CheckpointDestroyed.Connect< LocalEvent, &LocalEvent::CheckpointDestroyed >(this);
        break;
        case EVT_KEYBINDDESTROYED:
            Reference< CKeybind >::Get(id).KeybindDestroyed.Connect< LocalEvent, &LocalEvent::KeybindDestroyed >(this);
        break;
        case EVT_OBJECTDESTROYED:
            Reference< CObject >::Get(id).ObjectDestroyed.Connect< LocalEvent, &LocalEvent::ObjectDestroyed >(this);
        break;
        case EVT_PICKUPDESTROYED:
            Reference< CPickup >::Get(id).PickupDestroyed.Connect< LocalEvent, &LocalEvent::PickupDestroyed >(this);
        break;
        case EVT_PLAYERDESTROYED:
            Reference< CPlayer >::Get(id).PlayerDestroyed.Connect< LocalEvent, &LocalEvent::PlayerDestroyed >(this);
        break;
        case EVT_SPHEREDESTROYED:
            Reference< CSphere >::Get(id).SphereDestroyed.Connect< LocalEvent, &LocalEvent::SphereDestroyed >(this);
        break;
        case EVT_SPRITEDESTROYED:
            Reference< CSprite >::Get(id).SpriteDestroyed.Connect< LocalEvent, &LocalEvent::SpriteDestroyed >(this);
        break;
        case EVT_TEXTDRAWDESTROYED:
            Reference< CTextdraw >::Get(id).TextdrawDestroyed.Connect< LocalEvent, &LocalEvent::TextdrawDestroyed >(this);
        break;
        case EVT_VEHICLEDESTROYED:
            Reference< CVehicle >::Get(id).VehicleDestroyed.Connect< LocalEvent, &LocalEvent::VehicleDestroyed >(this);
        break;
        case EVT_BLIPCUSTOM:
            Reference< CBlip >::Get(id).BlipCustom.Connect< LocalEvent, &LocalEvent::BlipCustom >(this);
        break;
        case EVT_CHECKPOINTCUSTOM:
            Reference< CCheckpoint >::Get(id).CheckpointCustom.Connect< LocalEvent, &LocalEvent::CheckpointCustom >(this);
        break;
        case EVT_KEYBINDCUSTOM:
            Reference< CKeybind >::Get(id).KeybindCustom.Connect< LocalEvent, &LocalEvent::KeybindCustom >(this);
        break;
        case EVT_OBJECTCUSTOM:
            Reference< CObject >::Get(id).ObjectCustom.Connect< LocalEvent, &LocalEvent::ObjectCustom >(this);
        break;
        case EVT_PICKUPCUSTOM:
            Reference< CPickup >::Get(id).PickupCustom.Connect< LocalEvent, &LocalEvent::PickupCustom >(this);
        break;
        case EVT_PLAYERCUSTOM:
            Reference< CPlayer >::Get(id).PlayerCustom.Connect< LocalEvent, &LocalEvent::PlayerCustom >(this);
        break;
        case EVT_SPHERECUSTOM:
            Reference< CSphere >::Get(id).SphereCustom.Connect< LocalEvent, &LocalEvent::SphereCustom >(this);
        break;
        case EVT_SPRITECUSTOM:
            Reference< CSprite >::Get(id).SpriteCustom.Connect< LocalEvent, &LocalEvent::SpriteCustom >(this);
        break;
        case EVT_TEXTDRAWCUSTOM:
            Reference< CTextdraw >::Get(id).TextdrawCustom.Connect< LocalEvent, &LocalEvent::TextdrawCustom >(this);
        break;
        case EVT_VEHICLECUSTOM:
            Reference< CVehicle >::Get(id).VehicleCustom.Connect< LocalEvent, &LocalEvent::VehicleCustom >(this);
        break;
        case EVT_PLAYERAWAY:
            Reference< CPlayer >::Get(id).PlayerAway.Connect< LocalEvent, &LocalEvent::PlayerAway >(this);
        break;
        case EVT_PLAYERGAMEKEYS:
            Reference< CPlayer >::Get(id).PlayerGameKeys.Connect< LocalEvent, &LocalEvent::PlayerGameKeys >(this);
        break;
        case EVT_PLAYERRENAME:
            Reference< CPlayer >::Get(id).PlayerRename.Connect< LocalEvent, &LocalEvent::PlayerRename >(this);
        break;
        case EVT_PLAYERREQUESTCLASS:
            Reference< CPlayer >::Get(id).PlayerRequestClass.Connect< LocalEvent, &LocalEvent::PlayerRequestClass >(this);
        break;
        case EVT_PLAYERREQUESTSPAWN:
            Reference< CPlayer >::Get(id).PlayerRequestSpawn.Connect< LocalEvent, &LocalEvent::PlayerRequestSpawn >(this);
        break;
        case EVT_PLAYERSPAWN:
            Reference< CPlayer >::Get(id).PlayerSpawn.Connect< LocalEvent, &LocalEvent::PlayerSpawn >(this);
        break;
        case EVT_PLAYERSTARTTYPING:
            Reference< CPlayer >::Get(id).PlayerStartTyping.Connect< LocalEvent, &LocalEvent::PlayerStartTyping >(this);
        break;
        case EVT_PLAYERSTOPTYPING:
            Reference< CPlayer >::Get(id).PlayerStopTyping.Connect< LocalEvent, &LocalEvent::PlayerStopTyping >(this);
        break;
        case EVT_PLAYERCHAT:
            Reference< CPlayer >::Get(id).PlayerChat.Connect< LocalEvent, &LocalEvent::PlayerChat >(this);
        break;
        case EVT_PLAYERCOMMAND:
            Reference< CPlayer >::Get(id).PlayerCommand.Connect< LocalEvent, &LocalEvent::PlayerCommand >(this);
        break;
        case EVT_PLAYERMESSAGE:
            Reference< CPlayer >::Get(id).PlayerMessage.Connect< LocalEvent, &LocalEvent::PlayerMessage >(this);
        break;
        case EVT_PLAYERHEALTH:
            Reference< CPlayer >::Get(id).PlayerHealth.Connect< LocalEvent, &LocalEvent::PlayerHealth >(this);
        break;
        case EVT_PLAYERARMOUR:
            Reference< CPlayer >::Get(id).PlayerArmour.Connect< LocalEvent, &LocalEvent::PlayerArmour >(this);
        break;
        case EVT_PLAYERWEAPON:
            Reference< CPlayer >::Get(id).PlayerWeapon.Connect< LocalEvent, &LocalEvent::PlayerWeapon >(this);
        break;
        case EVT_PLAYERMOVE:
            Reference< CPlayer >::Get(id).PlayerMove.Connect< LocalEvent, &LocalEvent::PlayerMove >(this);
        break;
        case EVT_PLAYERWASTED:
            Reference< CPlayer >::Get(id).PlayerWasted.Connect< LocalEvent, &LocalEvent::PlayerWasted >(this);
        break;
        case EVT_PLAYERKILLED:
            Reference< CPlayer >::Get(id).PlayerKilled.Connect< LocalEvent, &LocalEvent::PlayerKilled >(this);
        break;
        case EVT_PLAYERTEAMKILL:
            Reference< CPlayer >::Get(id).PlayerTeamKill.Connect< LocalEvent, &LocalEvent::PlayerTeamKill >(this);
        break;
        case EVT_PLAYERSPECTATE:
            Reference< CPlayer >::Get(id).PlayerSpectate.Connect< LocalEvent, &LocalEvent::PlayerSpectate >(this);
        break;
        case EVT_PLAYERCRASHREPORT:
            Reference< CPlayer >::Get(id).PlayerCrashreport.Connect< LocalEvent, &LocalEvent::PlayerCrashreport >(this);
        break;
        case EVT_PLAYERBURNING:
            Reference< CPlayer >::Get(id).PlayerBurning.Connect< LocalEvent, &LocalEvent::PlayerBurning >(this);
        break;
        case EVT_PLAYERCROUCHING:
            Reference< CPlayer >::Get(id).PlayerCrouching.Connect< LocalEvent, &LocalEvent::PlayerCrouching >(this);
        break;
        case EVT_PLAYERSTATE:
            Reference< CPlayer >::Get(id).PlayerState.Connect< LocalEvent, &LocalEvent::PlayerState >(this);
        break;
        case EVT_PLAYERACTION:
            Reference< CPlayer >::Get(id).PlayerAction.Connect< LocalEvent, &LocalEvent::PlayerAction >(this);
        break;
        case EVT_STATENONE:
            Reference< CPlayer >::Get(id).StateNone.Connect< LocalEvent, &LocalEvent::StateNone >(this);
        break;
        case EVT_STATENORMAL:
            Reference< CPlayer >::Get(id).StateNormal.Connect< LocalEvent, &LocalEvent::StateNormal >(this);
        break;
        case EVT_STATESHOOTING:
            Reference< CPlayer >::Get(id).StateShooting.Connect< LocalEvent, &LocalEvent::StateShooting >(this);
        break;
        case EVT_STATEDRIVER:
            Reference< CPlayer >::Get(id).StateDriver.Connect< LocalEvent, &LocalEvent::StateDriver >(this);
        break;
        case EVT_STATEPASSENGER:
            Reference< CPlayer >::Get(id).StatePassenger.Connect< LocalEvent, &LocalEvent::StatePassenger >(this);
        break;
        case EVT_STATEENTERDRIVER:
            Reference< CPlayer >::Get(id).StateEnterDriver.Connect< LocalEvent, &LocalEvent::StateEnterDriver >(this);
        break;
        case EVT_STATEENTERPASSENGER:
            Reference< CPlayer >::Get(id).StateEnterPassenger.Connect< LocalEvent, &LocalEvent::StateEnterPassenger >(this);
        break;
        case EVT_STATEEXITVEHICLE:
            Reference< CPlayer >::Get(id).StateExitVehicle.Connect< LocalEvent, &LocalEvent::StateExitVehicle >(this);
        break;
        case EVT_STATEUNSPAWNED:
            Reference< CPlayer >::Get(id).StateUnspawned.Connect< LocalEvent, &LocalEvent::StateUnspawned >(this);
        break;
        case EVT_ACTIONNONE:
            Reference< CPlayer >::Get(id).ActionNone.Connect< LocalEvent, &LocalEvent::ActionNone >(this);
        break;
        case EVT_ACTIONNORMAL:
            Reference< CPlayer >::Get(id).ActionNormal.Connect< LocalEvent, &LocalEvent::ActionNormal >(this);
        break;
        case EVT_ACTIONAIMING:
            Reference< CPlayer >::Get(id).ActionAiming.Connect< LocalEvent, &LocalEvent::ActionAiming >(this);
        break;
        case EVT_ACTIONSHOOTING:
            Reference< CPlayer >::Get(id).ActionShooting.Connect< LocalEvent, &LocalEvent::ActionShooting >(this);
        break;
        case EVT_ACTIONJUMPING:
            Reference< CPlayer >::Get(id).ActionJumping.Connect< LocalEvent, &LocalEvent::ActionJumping >(this);
        break;
        case EVT_ACTIONLIEDOWN:
            Reference< CPlayer >::Get(id).ActionLieDown.Connect< LocalEvent, &LocalEvent::ActionLieDown >(this);
        break;
        case EVT_ACTIONGETTINGUP:
            Reference< CPlayer >::Get(id).ActionGettingUp.Connect< LocalEvent, &LocalEvent::ActionGettingUp >(this);
        break;
        case EVT_ACTIONJUMPVEHICLE:
            Reference< CPlayer >::Get(id).ActionJumpVehicle.Connect< LocalEvent, &LocalEvent::ActionJumpVehicle >(this);
        break;
        case EVT_ACTIONDRIVING:
            Reference< CPlayer >::Get(id).ActionDriving.Connect< LocalEvent, &LocalEvent::ActionDriving >(this);
        break;
        case EVT_ACTIONDYING:
            Reference< CPlayer >::Get(id).ActionDying.Connect< LocalEvent, &LocalEvent::ActionDying >(this);
        break;
        case EVT_ACTIONWASTED:
            Reference< CPlayer >::Get(id).ActionWasted.Connect< LocalEvent, &LocalEvent::ActionWasted >(this);
        break;
        case EVT_ACTIONEMBARKING:
            Reference< CPlayer >::Get(id).ActionEmbarking.Connect< LocalEvent, &LocalEvent::ActionEmbarking >(this);
        break;
        case EVT_ACTIONDISEMBARKING:
            Reference< CPlayer >::Get(id).ActionDisembarking.Connect< LocalEvent, &LocalEvent::ActionDisembarking >(this);
        break;
        case EVT_VEHICLERESPAWN:
            Reference< CVehicle >::Get(id).VehicleRespawn.Connect< LocalEvent, &LocalEvent::VehicleRespawn >(this);
        break;
        case EVT_VEHICLEEXPLODE:
            Reference< CVehicle >::Get(id).VehicleExplode.Connect< LocalEvent, &LocalEvent::VehicleExplode >(this);
        break;
        case EVT_VEHICLEHEALTH:
            Reference< CVehicle >::Get(id).VehicleHealth.Connect< LocalEvent, &LocalEvent::VehicleHealth >(this);
        break;
        case EVT_VEHICLEMOVE:
            Reference< CVehicle >::Get(id).VehicleMove.Connect< LocalEvent, &LocalEvent::VehicleMove >(this);
        break;
        case EVT_PICKUPRESPAWN:
            Reference< CPickup >::Get(id).PickupRespawn.Connect< LocalEvent, &LocalEvent::PickupRespawn >(this);
        break;
        case EVT_KEYBINDKEYPRESS:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).KeybindKeyPress.Connect< LocalEvent, &LocalEvent::KeybindKeyPress >(this);
            }
            else
            {
                Reference< CKeybind >::Get(id).KeybindKeyPress.Connect< LocalEvent, &LocalEvent::KeybindKeyPress >(this);
            }
        break;
        case EVT_KEYBINDKEYRELEASE:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).KeybindKeyRelease.Connect< LocalEvent, &LocalEvent::KeybindKeyRelease >(this);
            }
            else
            {
                Reference< CKeybind >::Get(id).KeybindKeyRelease.Connect< LocalEvent, &LocalEvent::KeybindKeyRelease >(this);
            }
        break;
        case EVT_VEHICLEEMBARKING:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).VehicleEmbarking.Connect< LocalEvent, &LocalEvent::VehicleEmbarking >(this);
            }
            else
            {
                Reference< CVehicle >::Get(id).VehicleEmbarking.Connect< LocalEvent, &LocalEvent::VehicleEmbarking >(this);
            }
        break;
        case EVT_VEHICLEEMBARKED:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).VehicleEmbarked.Connect< LocalEvent, &LocalEvent::VehicleEmbarked >(this);
            }
            else
            {
                Reference< CVehicle >::Get(id).VehicleEmbarked.Connect< LocalEvent, &LocalEvent::VehicleEmbarked >(this);
            }
        break;
        case EVT_VEHICLEDISEMBARK:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).VehicleDisembark.Connect< LocalEvent, &LocalEvent::VehicleDisembark >(this);
            }
            else
            {
                Reference< CVehicle >::Get(id).VehicleDisembark.Connect< LocalEvent, &LocalEvent::VehicleDisembark >(this);
            }
        break;
        case EVT_PICKUPCLAIMED:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).PickupClaimed.Connect< LocalEvent, &LocalEvent::PickupClaimed >(this);
            }
            else
            {
                Reference< CPickup >::Get(id).PickupClaimed.Connect< LocalEvent, &LocalEvent::PickupClaimed >(this);
            }
        break;
        case EVT_PICKUPCOLLECTED:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).PickupCollected.Connect< LocalEvent, &LocalEvent::PickupCollected >(this);
            }
            else
            {
                Reference< CPickup >::Get(id).PickupCollected.Connect< LocalEvent, &LocalEvent::PickupCollected >(this);
            }
        break;
        case EVT_OBJECTSHOT:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).ObjectShot.Connect< LocalEvent, &LocalEvent::ObjectShot >(this);
            }
            else
            {
                Reference< CObject >::Get(id).ObjectShot.Connect< LocalEvent, &LocalEvent::ObjectShot >(this);
            }
        break;
        case EVT_OBJECTBUMP:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).ObjectBump.Connect< LocalEvent, &LocalEvent::ObjectBump >(this);
            }
            else
            {
                Reference< CObject >::Get(id).ObjectBump.Connect< LocalEvent, &LocalEvent::ObjectBump >(this);
            }
        break;
        case EVT_CHECKPOINTENTERED:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).CheckpointEntered.Connect< LocalEvent, &LocalEvent::CheckpointEntered >(this);
            }
            else
            {
                Reference< CCheckpoint >::Get(id).CheckpointEntered.Connect< LocalEvent, &LocalEvent::CheckpointEntered >(this);
            }
        break;
        case EVT_CHECKPOINTEXITED:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).CheckpointExited.Connect< LocalEvent, &LocalEvent::CheckpointExited >(this);
            }
            else
            {
                Reference< CCheckpoint >::Get(id).CheckpointExited.Connect< LocalEvent, &LocalEvent::CheckpointExited >(this);
            }
        break;
        case EVT_SPHEREENTERED:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).SphereEntered.Connect< LocalEvent, &LocalEvent::SphereEntered >(this);
            }
            else
            {
                Reference< CSphere >::Get(id).SphereEntered.Connect< LocalEvent, &LocalEvent::SphereEntered >(this);
            }
        break;
        case EVT_SPHEREEXITED:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).SphereExited.Connect< LocalEvent, &LocalEvent::SphereExited >(this);
            }
            else
            {
                Reference< CSphere >::Get(id).SphereExited.Connect< LocalEvent, &LocalEvent::SphereExited >(this);
            }
        break;
        default:
            LogErr("Attempting to <attach> to an unknown event type: %d", _SCI32(m_Type));
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::Detach() noexcept
{
    // Filters take care of both detaching and unhooking for local filters
    Unhook();
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::Detach(SQInt32 id) noexcept
{
    switch (m_Type)
    {
        case EVT_BLIPDESTROYED:
            Reference< CBlip >::Get(id).BlipDestroyed.Disconnect< LocalEvent, &LocalEvent::BlipDestroyed >(this);
        break;
        case EVT_CHECKPOINTDESTROYED:
            Reference< CCheckpoint >::Get(id).CheckpointDestroyed.Disconnect< LocalEvent, &LocalEvent::CheckpointDestroyed >(this);
        break;
        case EVT_KEYBINDDESTROYED:
            Reference< CKeybind >::Get(id).KeybindDestroyed.Disconnect< LocalEvent, &LocalEvent::KeybindDestroyed >(this);
        break;
        case EVT_OBJECTDESTROYED:
            Reference< CObject >::Get(id).ObjectDestroyed.Disconnect< LocalEvent, &LocalEvent::ObjectDestroyed >(this);
        break;
        case EVT_PICKUPDESTROYED:
            Reference< CPickup >::Get(id).PickupDestroyed.Disconnect< LocalEvent, &LocalEvent::PickupDestroyed >(this);
        break;
        case EVT_PLAYERDESTROYED:
            Reference< CPlayer >::Get(id).PlayerDestroyed.Disconnect< LocalEvent, &LocalEvent::PlayerDestroyed >(this);
        break;
        case EVT_SPHEREDESTROYED:
            Reference< CSphere >::Get(id).SphereDestroyed.Disconnect< LocalEvent, &LocalEvent::SphereDestroyed >(this);
        break;
        case EVT_SPRITEDESTROYED:
            Reference< CSprite >::Get(id).SpriteDestroyed.Disconnect< LocalEvent, &LocalEvent::SpriteDestroyed >(this);
        break;
        case EVT_TEXTDRAWDESTROYED:
            Reference< CTextdraw >::Get(id).TextdrawDestroyed.Disconnect< LocalEvent, &LocalEvent::TextdrawDestroyed >(this);
        break;
        case EVT_VEHICLEDESTROYED:
            Reference< CVehicle >::Get(id).VehicleDestroyed.Disconnect< LocalEvent, &LocalEvent::VehicleDestroyed >(this);
        break;
        case EVT_BLIPCUSTOM:
            Reference< CBlip >::Get(id).BlipCustom.Disconnect< LocalEvent, &LocalEvent::BlipCustom >(this);
        break;
        case EVT_CHECKPOINTCUSTOM:
            Reference< CCheckpoint >::Get(id).CheckpointCustom.Disconnect< LocalEvent, &LocalEvent::CheckpointCustom >(this);
        break;
        case EVT_KEYBINDCUSTOM:
            Reference< CKeybind >::Get(id).KeybindCustom.Disconnect< LocalEvent, &LocalEvent::KeybindCustom >(this);
        break;
        case EVT_OBJECTCUSTOM:
            Reference< CObject >::Get(id).ObjectCustom.Disconnect< LocalEvent, &LocalEvent::ObjectCustom >(this);
        break;
        case EVT_PICKUPCUSTOM:
            Reference< CPickup >::Get(id).PickupCustom.Disconnect< LocalEvent, &LocalEvent::PickupCustom >(this);
        break;
        case EVT_PLAYERCUSTOM:
            Reference< CPlayer >::Get(id).PlayerCustom.Disconnect< LocalEvent, &LocalEvent::PlayerCustom >(this);
        break;
        case EVT_SPHERECUSTOM:
            Reference< CSphere >::Get(id).SphereCustom.Disconnect< LocalEvent, &LocalEvent::SphereCustom >(this);
        break;
        case EVT_SPRITECUSTOM:
            Reference< CSprite >::Get(id).SpriteCustom.Disconnect< LocalEvent, &LocalEvent::SpriteCustom >(this);
        break;
        case EVT_TEXTDRAWCUSTOM:
            Reference< CTextdraw >::Get(id).TextdrawCustom.Disconnect< LocalEvent, &LocalEvent::TextdrawCustom >(this);
        break;
        case EVT_VEHICLECUSTOM:
            Reference< CVehicle >::Get(id).VehicleCustom.Disconnect< LocalEvent, &LocalEvent::VehicleCustom >(this);
        break;
        case EVT_PLAYERAWAY:
            Reference< CPlayer >::Get(id).PlayerAway.Disconnect< LocalEvent, &LocalEvent::PlayerAway >(this);
        break;
        case EVT_PLAYERGAMEKEYS:
            Reference< CPlayer >::Get(id).PlayerGameKeys.Disconnect< LocalEvent, &LocalEvent::PlayerGameKeys >(this);
        break;
        case EVT_PLAYERRENAME:
            Reference< CPlayer >::Get(id).PlayerRename.Disconnect< LocalEvent, &LocalEvent::PlayerRename >(this);
        break;
        case EVT_PLAYERREQUESTCLASS:
            Reference< CPlayer >::Get(id).PlayerRequestClass.Disconnect< LocalEvent, &LocalEvent::PlayerRequestClass >(this);
        break;
        case EVT_PLAYERREQUESTSPAWN:
            Reference< CPlayer >::Get(id).PlayerRequestSpawn.Disconnect< LocalEvent, &LocalEvent::PlayerRequestSpawn >(this);
        break;
        case EVT_PLAYERSPAWN:
            Reference< CPlayer >::Get(id).PlayerSpawn.Disconnect< LocalEvent, &LocalEvent::PlayerSpawn >(this);
        break;
        case EVT_PLAYERSTARTTYPING:
            Reference< CPlayer >::Get(id).PlayerStartTyping.Disconnect< LocalEvent, &LocalEvent::PlayerStartTyping >(this);
        break;
        case EVT_PLAYERSTOPTYPING:
            Reference< CPlayer >::Get(id).PlayerStopTyping.Disconnect< LocalEvent, &LocalEvent::PlayerStopTyping >(this);
        break;
        case EVT_PLAYERCHAT:
            Reference< CPlayer >::Get(id).PlayerChat.Disconnect< LocalEvent, &LocalEvent::PlayerChat >(this);
        break;
        case EVT_PLAYERCOMMAND:
            Reference< CPlayer >::Get(id).PlayerCommand.Disconnect< LocalEvent, &LocalEvent::PlayerCommand >(this);
        break;
        case EVT_PLAYERMESSAGE:
            Reference< CPlayer >::Get(id).PlayerMessage.Disconnect< LocalEvent, &LocalEvent::PlayerMessage >(this);
        break;
        case EVT_PLAYERHEALTH:
            Reference< CPlayer >::Get(id).PlayerHealth.Disconnect< LocalEvent, &LocalEvent::PlayerHealth >(this);
        break;
        case EVT_PLAYERARMOUR:
            Reference< CPlayer >::Get(id).PlayerArmour.Disconnect< LocalEvent, &LocalEvent::PlayerArmour >(this);
        break;
        case EVT_PLAYERWEAPON:
            Reference< CPlayer >::Get(id).PlayerWeapon.Disconnect< LocalEvent, &LocalEvent::PlayerWeapon >(this);
        break;
        case EVT_PLAYERMOVE:
            Reference< CPlayer >::Get(id).PlayerMove.Disconnect< LocalEvent, &LocalEvent::PlayerMove >(this);
        break;
        case EVT_PLAYERWASTED:
            Reference< CPlayer >::Get(id).PlayerWasted.Disconnect< LocalEvent, &LocalEvent::PlayerWasted >(this);
        break;
        case EVT_PLAYERKILLED:
            Reference< CPlayer >::Get(id).PlayerKilled.Disconnect< LocalEvent, &LocalEvent::PlayerKilled >(this);
        break;
        case EVT_PLAYERTEAMKILL:
            Reference< CPlayer >::Get(id).PlayerTeamKill.Disconnect< LocalEvent, &LocalEvent::PlayerTeamKill >(this);
        break;
        case EVT_PLAYERSPECTATE:
            Reference< CPlayer >::Get(id).PlayerSpectate.Disconnect< LocalEvent, &LocalEvent::PlayerSpectate >(this);
        break;
        case EVT_PLAYERCRASHREPORT:
            Reference< CPlayer >::Get(id).PlayerCrashreport.Disconnect< LocalEvent, &LocalEvent::PlayerCrashreport >(this);
        break;
        case EVT_PLAYERBURNING:
            Reference< CPlayer >::Get(id).PlayerBurning.Disconnect< LocalEvent, &LocalEvent::PlayerBurning >(this);
        break;
        case EVT_PLAYERCROUCHING:
            Reference< CPlayer >::Get(id).PlayerCrouching.Disconnect< LocalEvent, &LocalEvent::PlayerCrouching >(this);
        break;
        case EVT_PLAYERSTATE:
            Reference< CPlayer >::Get(id).PlayerState.Disconnect< LocalEvent, &LocalEvent::PlayerState >(this);
        break;
        case EVT_PLAYERACTION:
            Reference< CPlayer >::Get(id).PlayerAction.Disconnect< LocalEvent, &LocalEvent::PlayerAction >(this);
        break;
        case EVT_STATENONE:
            Reference< CPlayer >::Get(id).StateNone.Disconnect< LocalEvent, &LocalEvent::StateNone >(this);
        break;
        case EVT_STATENORMAL:
            Reference< CPlayer >::Get(id).StateNormal.Disconnect< LocalEvent, &LocalEvent::StateNormal >(this);
        break;
        case EVT_STATESHOOTING:
            Reference< CPlayer >::Get(id).StateShooting.Disconnect< LocalEvent, &LocalEvent::StateShooting >(this);
        break;
        case EVT_STATEDRIVER:
            Reference< CPlayer >::Get(id).StateDriver.Disconnect< LocalEvent, &LocalEvent::StateDriver >(this);
        break;
        case EVT_STATEPASSENGER:
            Reference< CPlayer >::Get(id).StatePassenger.Disconnect< LocalEvent, &LocalEvent::StatePassenger >(this);
        break;
        case EVT_STATEENTERDRIVER:
            Reference< CPlayer >::Get(id).StateEnterDriver.Disconnect< LocalEvent, &LocalEvent::StateEnterDriver >(this);
        break;
        case EVT_STATEENTERPASSENGER:
            Reference< CPlayer >::Get(id).StateEnterPassenger.Disconnect< LocalEvent, &LocalEvent::StateEnterPassenger >(this);
        break;
        case EVT_STATEEXITVEHICLE:
            Reference< CPlayer >::Get(id).StateExitVehicle.Disconnect< LocalEvent, &LocalEvent::StateExitVehicle >(this);
        break;
        case EVT_STATEUNSPAWNED:
            Reference< CPlayer >::Get(id).StateUnspawned.Disconnect< LocalEvent, &LocalEvent::StateUnspawned >(this);
        break;
        case EVT_ACTIONNONE:
            Reference< CPlayer >::Get(id).ActionNone.Disconnect< LocalEvent, &LocalEvent::ActionNone >(this);
        break;
        case EVT_ACTIONNORMAL:
            Reference< CPlayer >::Get(id).ActionNormal.Disconnect< LocalEvent, &LocalEvent::ActionNormal >(this);
        break;
        case EVT_ACTIONAIMING:
            Reference< CPlayer >::Get(id).ActionAiming.Disconnect< LocalEvent, &LocalEvent::ActionAiming >(this);
        break;
        case EVT_ACTIONSHOOTING:
            Reference< CPlayer >::Get(id).ActionShooting.Disconnect< LocalEvent, &LocalEvent::ActionShooting >(this);
        break;
        case EVT_ACTIONJUMPING:
            Reference< CPlayer >::Get(id).ActionJumping.Disconnect< LocalEvent, &LocalEvent::ActionJumping >(this);
        break;
        case EVT_ACTIONLIEDOWN:
            Reference< CPlayer >::Get(id).ActionLieDown.Disconnect< LocalEvent, &LocalEvent::ActionLieDown >(this);
        break;
        case EVT_ACTIONGETTINGUP:
            Reference< CPlayer >::Get(id).ActionGettingUp.Disconnect< LocalEvent, &LocalEvent::ActionGettingUp >(this);
        break;
        case EVT_ACTIONJUMPVEHICLE:
            Reference< CPlayer >::Get(id).ActionJumpVehicle.Disconnect< LocalEvent, &LocalEvent::ActionJumpVehicle >(this);
        break;
        case EVT_ACTIONDRIVING:
            Reference< CPlayer >::Get(id).ActionDriving.Disconnect< LocalEvent, &LocalEvent::ActionDriving >(this);
        break;
        case EVT_ACTIONDYING:
            Reference< CPlayer >::Get(id).ActionDying.Disconnect< LocalEvent, &LocalEvent::ActionDying >(this);
        break;
        case EVT_ACTIONWASTED:
            Reference< CPlayer >::Get(id).ActionWasted.Disconnect< LocalEvent, &LocalEvent::ActionWasted >(this);
        break;
        case EVT_ACTIONEMBARKING:
            Reference< CPlayer >::Get(id).ActionEmbarking.Disconnect< LocalEvent, &LocalEvent::ActionEmbarking >(this);
        break;
        case EVT_ACTIONDISEMBARKING:
            Reference< CPlayer >::Get(id).ActionDisembarking.Disconnect< LocalEvent, &LocalEvent::ActionDisembarking >(this);
        break;
        case EVT_VEHICLERESPAWN:
            Reference< CVehicle >::Get(id).VehicleRespawn.Disconnect< LocalEvent, &LocalEvent::VehicleRespawn >(this);
        break;
        case EVT_VEHICLEEXPLODE:
            Reference< CVehicle >::Get(id).VehicleExplode.Disconnect< LocalEvent, &LocalEvent::VehicleExplode >(this);
        break;
        case EVT_VEHICLEHEALTH:
            Reference< CVehicle >::Get(id).VehicleHealth.Disconnect< LocalEvent, &LocalEvent::VehicleHealth >(this);
        break;
        case EVT_VEHICLEMOVE:
            Reference< CVehicle >::Get(id).VehicleMove.Disconnect< LocalEvent, &LocalEvent::VehicleMove >(this);
        break;
        case EVT_PICKUPRESPAWN:
            Reference< CPickup >::Get(id).PickupRespawn.Disconnect< LocalEvent, &LocalEvent::PickupRespawn >(this);
        break;
        case EVT_KEYBINDKEYPRESS:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).KeybindKeyPress.Disconnect< LocalEvent, &LocalEvent::KeybindKeyPress >(this);
            }
            else
            {
                Reference< CKeybind >::Get(id).KeybindKeyPress.Disconnect< LocalEvent, &LocalEvent::KeybindKeyPress >(this);
            }
        break;
        case EVT_KEYBINDKEYRELEASE:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).KeybindKeyRelease.Disconnect< LocalEvent, &LocalEvent::KeybindKeyRelease >(this);
            }
            else
            {
                Reference< CKeybind >::Get(id).KeybindKeyRelease.Disconnect< LocalEvent, &LocalEvent::KeybindKeyRelease >(this);
            }
        break;
        case EVT_VEHICLEEMBARKING:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).VehicleEmbarking.Disconnect< LocalEvent, &LocalEvent::VehicleEmbarking >(this);
            }
            else
            {
                Reference< CVehicle >::Get(id).VehicleEmbarking.Disconnect< LocalEvent, &LocalEvent::VehicleEmbarking >(this);
            }
        break;
        case EVT_VEHICLEEMBARKED:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).VehicleEmbarked.Disconnect< LocalEvent, &LocalEvent::VehicleEmbarked >(this);
            }
            else
            {
                Reference< CVehicle >::Get(id).VehicleEmbarked.Disconnect< LocalEvent, &LocalEvent::VehicleEmbarked >(this);
            }
        break;
        case EVT_VEHICLEDISEMBARK:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).VehicleDisembark.Disconnect< LocalEvent, &LocalEvent::VehicleDisembark >(this);
            }
            else
            {
                Reference< CVehicle >::Get(id).VehicleDisembark.Disconnect< LocalEvent, &LocalEvent::VehicleDisembark >(this);
            }
        break;
        case EVT_PICKUPCLAIMED:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).PickupClaimed.Disconnect< LocalEvent, &LocalEvent::PickupClaimed >(this);
            }
            else
            {
                Reference< CPickup >::Get(id).PickupClaimed.Disconnect< LocalEvent, &LocalEvent::PickupClaimed >(this);
            }
        break;
        case EVT_PICKUPCOLLECTED:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).PickupCollected.Disconnect< LocalEvent, &LocalEvent::PickupCollected >(this);
            }
            else
            {
                Reference< CPickup >::Get(id).PickupCollected.Disconnect< LocalEvent, &LocalEvent::PickupCollected >(this);
            }
        break;
        case EVT_OBJECTSHOT:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).ObjectShot.Disconnect< LocalEvent, &LocalEvent::ObjectShot >(this);
            }
            else
            {
                Reference< CObject >::Get(id).ObjectShot.Disconnect< LocalEvent, &LocalEvent::ObjectShot >(this);
            }
        break;
        case EVT_OBJECTBUMP:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).ObjectBump.Disconnect< LocalEvent, &LocalEvent::ObjectBump >(this);
            }
            else
            {
                Reference< CObject >::Get(id).ObjectBump.Disconnect< LocalEvent, &LocalEvent::ObjectBump >(this);
            }
        break;
        case EVT_CHECKPOINTENTERED:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).CheckpointEntered.Disconnect< LocalEvent, &LocalEvent::CheckpointEntered >(this);
            }
            else
            {
                Reference< CCheckpoint >::Get(id).CheckpointEntered.Disconnect< LocalEvent, &LocalEvent::CheckpointEntered >(this);
            }
        break;
        case EVT_CHECKPOINTEXITED:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).CheckpointExited.Disconnect< LocalEvent, &LocalEvent::CheckpointExited >(this);
            }
            else
            {
                Reference< CCheckpoint >::Get(id).CheckpointExited.Disconnect< LocalEvent, &LocalEvent::CheckpointExited >(this);
            }
        break;
        case EVT_SPHEREENTERED:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).SphereEntered.Disconnect< LocalEvent, &LocalEvent::SphereEntered >(this);
            }
            else
            {
                Reference< CSphere >::Get(id).SphereEntered.Disconnect< LocalEvent, &LocalEvent::SphereEntered >(this);
            }
        break;
        case EVT_SPHEREEXITED:
            if (m_Inversed)
            {
                Reference< CPlayer >::Get(id).SphereExited.Disconnect< LocalEvent, &LocalEvent::SphereExited >(this);
            }
            else
            {
                Reference< CSphere >::Get(id).SphereExited.Disconnect< LocalEvent, &LocalEvent::SphereExited >(this);
            }
        break;
        default:
            LogErr("Attempting to <detach> from an unknown event type: %d", _SCI32(m_Type));
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::Hook() noexcept
{
    if (Ent< CBlip >::InEvent(m_Type, m_Inversed))
    {
        m_Blips.Hook();
    }

    if (Ent< CCheckpoint >::InEvent(m_Type, m_Inversed))
    {
        m_Checkpoints.Hook();
    }

    if (Ent< CKeybind >::InEvent(m_Type, m_Inversed))
    {
        m_Keybinds.Hook();
    }

    if (Ent< CObject >::InEvent(m_Type, m_Inversed))
    {
        m_Objects.Hook();
    }

    if (Ent< CPickup >::InEvent(m_Type, m_Inversed))
    {
        m_Pickups.Hook();
    }

    if (Ent< CPlayer >::InEvent(m_Type, m_Inversed))
    {
        m_Players.Hook();
    }

    if (Ent< CSphere >::InEvent(m_Type, m_Inversed))
    {
        m_Spheres.Hook();
    }

    if (Ent< CSprite >::InEvent(m_Type, m_Inversed))
    {
        m_Sprites.Hook();
    }

    if (Ent< CTextdraw >::InEvent(m_Type, m_Inversed))
    {
        m_Textdraws.Hook();
    }

    if (Ent< CVehicle >::InEvent(m_Type, m_Inversed))
    {
        m_Vehicles.Hook();
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::Unhook() noexcept
{
    if (Ent< CBlip >::InEvent(m_Type, m_Inversed))
    {
        m_Blips.Unhook();
    }

    if (Ent< CCheckpoint >::InEvent(m_Type, m_Inversed))
    {
        m_Checkpoints.Unhook();
    }

    if (Ent< CKeybind >::InEvent(m_Type, m_Inversed))
    {
        m_Keybinds.Unhook();
    }

    if (Ent< CObject >::InEvent(m_Type, m_Inversed))
    {
        m_Objects.Unhook();
    }

    if (Ent< CPickup >::InEvent(m_Type, m_Inversed))
    {
        m_Pickups.Unhook();
    }

    if (Ent< CPlayer >::InEvent(m_Type, m_Inversed))
    {
        m_Players.Unhook();
    }

    if (Ent< CSphere >::InEvent(m_Type, m_Inversed))
    {
        m_Spheres.Unhook();
    }

    if (Ent< CSprite >::InEvent(m_Type, m_Inversed))
    {
        m_Sprites.Unhook();
    }

    if (Ent< CTextdraw >::InEvent(m_Type, m_Inversed))
    {
        m_Textdraws.Unhook();
    }

    if (Ent< CVehicle >::InEvent(m_Type, m_Inversed))
    {
        m_Vehicles.Unhook();
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::Adaptable(SQInt32 type, bool inversed) noexcept
{
    if (Ent< CBlip >::InEvent(m_Type, m_Inversed) && !Ent< CBlip >::InEvent(type, inversed))
    {
        m_Blips.Clear(0);
    }

    if (Ent< CCheckpoint >::InEvent(m_Type, m_Inversed) && !Ent< CCheckpoint >::InEvent(type, inversed))
    {
        m_Checkpoints.Clear(0);
    }

    if (Ent< CKeybind >::InEvent(m_Type, m_Inversed) && !Ent< CKeybind >::InEvent(type, inversed))
    {
        m_Keybinds.Clear(0);
    }

    if (Ent< CObject >::InEvent(m_Type, m_Inversed) && !Ent< CObject >::InEvent(type, inversed))
    {
        m_Objects.Clear(0);
    }

    if (Ent< CPickup >::InEvent(m_Type, m_Inversed) && !Ent< CPickup >::InEvent(type, inversed))
    {
        m_Pickups.Clear(0);
    }

    if (Ent< CPlayer >::InEvent(m_Type, m_Inversed) && !Ent< CPlayer >::InEvent(type, inversed))
    {
        m_Players.Clear(0);
    }

    if (Ent< CSphere >::InEvent(m_Type, m_Inversed) && !Ent< CSphere >::InEvent(type, inversed))
    {
        m_Spheres.Clear(0);
    }

    if (Ent< CSprite >::InEvent(m_Type, m_Inversed) && !Ent< CSprite >::InEvent(type, inversed))
    {
        m_Sprites.Clear(0);
    }

    if (Ent< CTextdraw >::InEvent(m_Type, m_Inversed) && !Ent< CTextdraw >::InEvent(type, inversed))
    {
        m_Textdraws.Clear(0);
    }

    if (Ent< CVehicle >::InEvent(m_Type, m_Inversed) && !Ent< CVehicle >::InEvent(type, inversed))
    {
        m_Vehicles.Clear(0);
    }
}

// ------------------------------------------------------------------------------------------------
void LocalEvent::VMClose() noexcept
{
    // Release the reference to the specified callbacks
    m_OnTrigger.Release();
    m_OnInclude.Release();
    m_OnExclude.Release();
    m_OnCleared.Release();
    m_OnRelease.Release();
    // Release the reference to the specified user data
    m_Data.Release();

}

// ================================================================================================
template < class T > static bool Register_LocalFilter(HSQUIRRELVM vm, const SQChar * cname)
{
    // Output debugging information
    LogDbg("Beginning registration of <%s> type", cname);
    // Avoid using the long name of the type we're about to register
    typedef LocalFilter< T > Filter;
    // Filters should not be constructed to avoid further complications
    typedef NoConstructor< Filter > Allocator;
    // Attempt to register the specified filter type
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

        .Func(_SC("enabled"), &Filter::Enabled)
        .Func(_SC("clear"), &Filter::Clear)
        .Func(_SC("flip"), &Filter::Flip)
    );
    // Output debugging information
    LogDbg("Registration of <%s> type was successful", cname);
    // Registration succeeded
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Register_LocalEvent(HSQUIRRELVM vm)
{
    // Register dependencies
    if (!Register_LocalFilter< CBlip >(vm, _SC("BlipLocalFilter")) || \
        !Register_LocalFilter< CCheckpoint >(vm, _SC("CheckpointLocalFilter")) || \
        !Register_LocalFilter< CKeybind >(vm, _SC("KeybindLocalFilter")) || \
        !Register_LocalFilter< CObject >(vm, _SC("ObjectLocalFilter")) || \
        !Register_LocalFilter< CPickup >(vm, _SC("PickupLocalFilter")) || \
        !Register_LocalFilter< CPlayer >(vm, _SC("PlayerLocalFilter")) || \
        !Register_LocalFilter< CSphere >(vm, _SC("SphereLocalFilter")) || \
        !Register_LocalFilter< CSprite >(vm, _SC("SpriteLocalFilter")) || \
        !Register_LocalFilter< CTextdraw >(vm, _SC("TextdrawLocalFilter")) || \
        !Register_LocalFilter< CVehicle >(vm, _SC("VehicleLocalFilter")))
    {
        return false;
    }
    // Output debugging information
    LogDbg("Beginning registration of <LocalEvent> type");
    // Events should not be copied for the sake of simplicity
    typedef NoCopy< LocalEvent > Allocator;
    // Attempt to register the specified type
    Sqrat::RootTable(vm).Bind(_SC("LocalEvent"), Sqrat::Class< LocalEvent, Allocator >(vm, _SC("LocalEvent"))
        .Ctor()
        .Ctor<SQInt32>()
        .Ctor<SQInt32, bool>()

        .Func(_SC("_cmp"), &LocalEvent::Cmp)
        .Func(_SC("_tostring"), &LocalEvent::GetName)

        .Prop(_SC("ltag"), &LocalEvent::GetTag, &LocalEvent::SetTag)
        .Prop(_SC("ldata"), &LocalEvent::GetData, &LocalEvent::SetData)
        .Prop(_SC("type"), &LocalEvent::GetType, &LocalEvent::SetType)
        .Prop(_SC("idle"), &LocalEvent::GetIdle, &LocalEvent::SetIdle)
        .Prop(_SC("is_idle"), &LocalEvent::IsIdle)
        .Prop(_SC("stride"), &LocalEvent::GetStride, &LocalEvent::SetStride)
        .Prop(_SC("ignore"), &LocalEvent::GetIgnore, &LocalEvent::SetIgnore)
        .Prop(_SC("primary"), &LocalEvent::GetPrimary, &LocalEvent::SetPrimary)
        .Prop(_SC("secondary"), &LocalEvent::GetSecondary, &LocalEvent::SetSecondary)
        .Prop(_SC("inversed"), &LocalEvent::GetInversed, &LocalEvent::SetInversed)
        .Prop(_SC("suspended"), &LocalEvent::GetSuspended, &LocalEvent::SetSuspended)
        .Prop(_SC("compatible"), &LocalEvent::Compatible)
        .Prop(_SC("name"), &LocalEvent::GetName)

        .Prop(_SC("on_trigger"), &LocalEvent::GetOnTrigger, &LocalEvent::SetOnTrigger)
        .Prop(_SC("on_include"), &LocalEvent::GetOnInclude, &LocalEvent::SetOnInclude)
        .Prop(_SC("on_exclude"), &LocalEvent::GetOnExclude, &LocalEvent::SetOnExclude)
        .Prop(_SC("on_cleared"), &LocalEvent::GetOnCleared, &LocalEvent::SetOnCleared)
        .Prop(_SC("on_release"), &LocalEvent::GetOnRelease, &LocalEvent::SetOnRelease)

        .Prop(_SC("blips"), &LocalEvent::GetBlipFilter)
        .Prop(_SC("checkpoints"), &LocalEvent::GetCheckpointFilter)
        .Prop(_SC("keybinds"), &LocalEvent::GetKeybindFilter)
        .Prop(_SC("objects"), &LocalEvent::GetObjectFilter)
        .Prop(_SC("pickups"), &LocalEvent::GetPickupFilter)
        .Prop(_SC("players"), &LocalEvent::GetPlayerFilter)
        .Prop(_SC("spheres"), &LocalEvent::GetSphereFilter)
        .Prop(_SC("sprites"), &LocalEvent::GetSpriteFilter)
        .Prop(_SC("textdraws"), &LocalEvent::GetTextdrawFilter)
        .Prop(_SC("vehicles"), &LocalEvent::GetVehicleFilter)
    );
    // Output debugging information
    LogDbg("Registration of <LocalEvent> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
