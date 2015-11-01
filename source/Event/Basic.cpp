#include "Event/Basic.hpp"
#include "Register.hpp"
#include "Entity.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
BasicEvent::BasicEvent()
    : BasicEvent(EVT_UNKNOWN, false)
{

}

// ------------------------------------------------------------------------------------------------
BasicEvent::BasicEvent(SQInt32 type)
    : BasicEvent(type, false)
{

}

// ------------------------------------------------------------------------------------------------
BasicEvent::BasicEvent(SQInt32 type, bool suspended)
    : m_Type(static_cast<EventType>(type))
    , m_Stride(0)
    , m_Ignore(0)
    , m_Primary(SQMOD_UNKNOWN)
    , m_Secondary(SQMOD_UNKNOWN)
    , m_Idle()
    , m_OnTrigger()
    , m_Tag()
    , m_Data()
    , m_Suspended(suspended)
{
    // Attach to the specified event signal
    Attach();
    // Receive notification when the VM is about to be closed to release object references
    _Core->VMClose.Connect< BasicEvent, &BasicEvent::VMClose >(this);
}

// ------------------------------------------------------------------------------------------------
BasicEvent::~BasicEvent()
{
    // Detach from the specified event signal
    Detach();
    // Stop receiving notification when the VM is about to be closed
    _Core->VMClose.Disconnect< BasicEvent, &BasicEvent::VMClose >(this);
}

// ------------------------------------------------------------------------------------------------
bool BasicEvent::operator == (const BasicEvent & o) const
{
    return (m_Type == o.m_Type);
}

// ------------------------------------------------------------------------------------------------
bool BasicEvent::operator != (const BasicEvent & o) const
{
    return (m_Type != o.m_Type);
}

// ------------------------------------------------------------------------------------------------
bool BasicEvent::operator < (const BasicEvent & o) const
{
    return (m_Type < o.m_Type);
}

// ------------------------------------------------------------------------------------------------
bool BasicEvent::operator > (const BasicEvent & o) const
{
    return (m_Type > o.m_Type);
}

// ------------------------------------------------------------------------------------------------
bool BasicEvent::operator <= (const BasicEvent & o) const
{
    return (m_Type <= o.m_Type);
}

// ------------------------------------------------------------------------------------------------
bool BasicEvent::operator >= (const BasicEvent & o) const
{
    return (m_Type >= o.m_Type);
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::VMClose()
{
    // Release the reference to the specified callbacks
    m_OnTrigger.Release2();
    // Release the reference to the specified user data
    m_Data.Release();
}

// ------------------------------------------------------------------------------------------------
SQInt32 BasicEvent::Cmp(const BasicEvent & o) const
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
const SQChar * BasicEvent::GetName() const
{
    return GetEventName(m_Type);
}

// ------------------------------------------------------------------------------------------------
const SQChar * BasicEvent::GetTag() const
{
    return m_Tag.c_str();
}

void BasicEvent::SetTag(const SQChar * tag)
{
    m_Tag = tag;
}

// ------------------------------------------------------------------------------------------------
SqObj & BasicEvent::GetData()
{
    return m_Data;
}

void BasicEvent::SetData(SqObj & data)
{
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
SQInt32 BasicEvent::GetType() const
{
    return m_Type;
}

void BasicEvent::SetType(SQInt32 type)
{
    // Make sure the newly specified event is valid
    if (type == EVT_UNKNOWN || type >= EVT_COUNT)
    {
        LogErr("Cannot change the event to an incompatible type: %s", GetEventName(type));
    }
    else
    {
        // Detach from the current event type
        Detach();
        // Set the new event type
        m_Type = type;
        // Attach to the new event type
        Attach();
    }
}

// ------------------------------------------------------------------------------------------------
SQInteger BasicEvent::GetIdle() const
{
    return _SCSQI(std::chrono::duration_cast<std::chrono::milliseconds>(m_Idle - std::chrono::steady_clock::now()).count());
}

void BasicEvent::SetIdle(SQInteger millis)
{
    m_Idle = (std::chrono::steady_clock::now() + std::chrono::milliseconds(_SCI64(millis)));
}

bool BasicEvent::IsIdle() const
{
    return (m_Idle > std::chrono::steady_clock::now());
}

// ------------------------------------------------------------------------------------------------
SQInt32 BasicEvent::GetStride() const
{
    return m_Stride;
}

void BasicEvent::SetStride(SQInt32 stride)
{
    m_Stride = stride > 0 ? stride : 0;
}

// ------------------------------------------------------------------------------------------------
SQInt32 BasicEvent::GetIgnore() const
{
    return m_Ignore;
}

void BasicEvent::SetIgnore(SQInt32 ignore)
{
    m_Ignore = ignore > 0 ? ignore : 0;
}

// ------------------------------------------------------------------------------------------------
SQInt32 BasicEvent::GetPrimary() const
{
    return m_Primary;
}

void BasicEvent::SetPrimary(SQInt32 subset)
{
    m_Primary = subset;
}

// ------------------------------------------------------------------------------------------------
SQInt32 BasicEvent::GetSecondary() const
{
    return m_Secondary;
}

void BasicEvent::SetSecondary(SQInt32 subset)
{
    m_Secondary = subset;
}

// ------------------------------------------------------------------------------------------------
bool BasicEvent::GetSuspended() const
{
    return m_Suspended;
}

void BasicEvent::SetSuspended(bool toggle)
{
    m_Suspended = toggle;
}

// ------------------------------------------------------------------------------------------------
Function BasicEvent::GetOnTrigger() const
{
    return m_OnTrigger;
}

void BasicEvent::SetOnTrigger(Function & func)
{
    m_OnTrigger = func;
}

void BasicEvent::SetOnTrigger_Env(SqObj & env, Function & func)
{
    m_OnTrigger = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
bool BasicEvent::Compatible(SQInt32 type) const
{
    return (type != EVT_UNKNOWN && type < EVT_COUNT);
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::BlipCreated(SQInt32 blip, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(blip, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::CheckpointCreated(SQInt32 checkpoint, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(checkpoint, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::KeybindCreated(SQInt32 keybind, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(keybind, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ObjectCreated(SQInt32 object, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(object, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PickupCreated(SQInt32 pickup, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(pickup, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerCreated(SQInt32 player, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(player, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::SphereCreated(SQInt32 sphere, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(sphere, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::SpriteCreated(SQInt32 sprite, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(sprite, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::TextdrawCreated(SQInt32 textdraw, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(textdraw, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::VehicleCreated(SQInt32 vehicle, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(vehicle, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::BlipDestroyed(SQInt32 blip, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(blip, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::CheckpointDestroyed(SQInt32 checkpoint, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(checkpoint, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::KeybindDestroyed(SQInt32 keybind, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(keybind, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ObjectDestroyed(SQInt32 object, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(object, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PickupDestroyed(SQInt32 pickup, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(pickup, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerDestroyed(SQInt32 player, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(player, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::SphereDestroyed(SQInt32 sphere, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(sphere, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::SpriteDestroyed(SQInt32 sprite, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(sprite, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::TextdrawDestroyed(SQInt32 textdraw, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(textdraw, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::VehicleDestroyed(SQInt32 vehicle, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(vehicle, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::BlipCustom(SQInt32 blip, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(blip, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::CheckpointCustom(SQInt32 checkpoint, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(checkpoint, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::KeybindCustom(SQInt32 keybind, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(keybind, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ObjectCustom(SQInt32 object, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(object, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PickupCustom(SQInt32 pickup, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(pickup, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerCustom(SQInt32 player, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(player, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::SphereCustom(SQInt32 sphere, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(sphere, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::SpriteCustom(SQInt32 sprite, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(sprite, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::TextdrawCustom(SQInt32 textdraw, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(textdraw, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::VehicleCustom(SQInt32 vehicle, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(vehicle, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerAway(SQInt32 player, bool status)
{
    if (Trigger() && \
        (m_Primary < 0 || status == m_Primary) && (m_Secondary < 0 || status == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, bool >(player, status);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerGameKeys(SQInt32 player, SQInt32 previous, SQInt32 current)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || current == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerRename(SQInt32 player, const SQChar * previous, const SQChar * current)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const SQChar *, const SQChar * >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerRequestClass(SQInt32 player, SQInt32 offset)
{
    if (Trigger() && \
        (m_Primary < 0 || offset == m_Primary) && (m_Secondary < 0 || offset == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, offset);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerRequestSpawn(SQInt32 player)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(player);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerSpawn(SQInt32 player)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(player);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerStartTyping(SQInt32 player)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(player);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerStopTyping(SQInt32 player)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(player);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerChat(SQInt32 player, const SQChar * message)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const SQChar * >(player, message);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerCommand(SQInt32 player, const SQChar * command)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const SQChar * >(player, command);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerMessage(SQInt32 player, SQInt32 receiver, const SQChar * message)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, const SQChar * >(player, receiver, message);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerHealth(SQInt32 player, SQFloat previous, SQFloat current)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQFloat, SQFloat >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerArmour(SQInt32 player, SQFloat previous, SQFloat current)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQFloat, SQFloat >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerWeapon(SQInt32 player, SQInt32 previous, SQInt32 current)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || current == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerMove(SQInt32 player, const Vector3 & previous, const Vector3 & current)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const Vector3 &, const Vector3 & >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerWasted(SQInt32 player, SQInt32 reason)
{
    if (Trigger() && \
        (m_Primary < 0 || reason == m_Primary) && (m_Secondary < 0 || reason == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, reason);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerKilled(SQInt32 player, SQInt32 killer, SQInt32 reason, SQInt32 body_part)
{
    if (Trigger() && \
        (m_Primary < 0 || reason == m_Primary) && (m_Secondary < 0 || body_part == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32, SQInt32 >(player, killer, reason, body_part);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerTeamKill(SQInt32 player, SQInt32 killer, SQInt32 reason, SQInt32 body_part)
{
    if (Trigger() && \
        (m_Primary < 0 || reason == m_Primary) && (m_Secondary < 0 || body_part == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32, SQInt32 >(player, killer, reason, body_part);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerSpectate(SQInt32 player, SQInt32 target)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, target);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerCrashreport(SQInt32 player, const SQChar * report)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const SQChar * >(player, report);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerBurning(SQInt32 player, bool state)
{
    if (Trigger() && \
        (m_Primary < 0 || state == m_Primary) && (m_Secondary < 0 || state == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, bool >(player, state);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerCrouching(SQInt32 player, bool state)
{
    if (Trigger() && \
        (m_Primary < 0 || state == m_Primary) && (m_Secondary < 0 || state == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, bool >(player, state);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerState(SQInt32 player, SQInt32 previous, SQInt32 current)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || current == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PlayerAction(SQInt32 player, SQInt32 previous, SQInt32 current)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || current == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::StateNone(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::StateNormal(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::StateShooting(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::StateDriver(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::StatePassenger(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::StateEnterDriver(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::StateEnterPassenger(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::StateExitVehicle(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::StateUnspawned(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ActionNone(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ActionNormal(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ActionAiming(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ActionShooting(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ActionJumping(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ActionLieDown(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ActionGettingUp(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ActionJumpVehicle(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ActionDriving(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ActionDying(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ActionWasted(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ActionEmbarking(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ActionDisembarking(SQInt32 player, SQInt32 previous)
{
    if (Trigger() && \
        (m_Primary < 0 || previous == m_Primary) && (m_Secondary < 0 || previous == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, previous);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::VehicleRespawn(SQInt32 vehicle)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(vehicle);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::VehicleExplode(SQInt32 vehicle)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(vehicle);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::VehicleHealth(SQInt32 vehicle, SQFloat previous, SQFloat current)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQFloat, SQFloat >(vehicle, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::VehicleMove(SQInt32 vehicle, const Vector3 & previous, const Vector3 &current)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, const Vector3 &, const Vector3 & >(vehicle, previous, current);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PickupRespawn(SQInt32 pickup)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32 >(pickup);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::KeybindKeyPress(SQInt32 player, SQInt32 keybind)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, keybind);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::KeybindKeyRelease(SQInt32 player, SQInt32 keybind)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, keybind);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::VehicleEmbarking(SQInt32 player, SQInt32 vehicle, SQInt32 slot)
{
    if (Trigger() && \
        (m_Primary < 0 || slot == m_Primary) && (m_Secondary < 0 || slot == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, vehicle, slot);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::VehicleEmbarked(SQInt32 player, SQInt32 vehicle, SQInt32 slot)
{
    if (Trigger() && \
        (m_Primary < 0 || slot == m_Primary) && (m_Secondary < 0 || slot == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, vehicle, slot);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::VehicleDisembark(SQInt32 player, SQInt32 vehicle)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, vehicle);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PickupClaimed(SQInt32 player, SQInt32 pickup)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, pickup);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::PickupCollected(SQInt32 player, SQInt32 pickup)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, pickup);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ObjectShot(SQInt32 player, SQInt32 object, SQInt32 weapon)
{
    if (Trigger() && \
        (m_Primary < 0 || weapon == m_Primary) && (m_Secondary < 0 || weapon == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, SQInt32 >(player, object, weapon);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ObjectBump(SQInt32 player, SQInt32 object)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, object);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::CheckpointEntered(SQInt32 player, SQInt32 checkpoint)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, checkpoint);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::CheckpointExited(SQInt32 player, SQInt32 checkpoint)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, checkpoint);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::SphereEntered(SQInt32 player, SQInt32 sphere)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, sphere);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::SphereExited(SQInt32 player, SQInt32 sphere)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQInt32, SQInt32 >(player, sphere);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ServerFrame(SQFloat delta)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< SQFloat >(delta);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ServerStartup()
{
    if (Trigger())
    {
        m_OnTrigger.Execute();
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ServerShutdown()
{
    if (Trigger())
    {
        m_OnTrigger.Execute();
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::InternalCommand(SQInt32 type, const SQChar * text)
{
    if (Trigger() && \
        (m_Primary < 0 || type == m_Primary) && (m_Secondary < 0 || type == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, const SQChar * >(type, text);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::LoginAttempt(const SQChar * name, const SQChar * pass, const SQChar * addr)
{
    if (Trigger())
    {
        m_OnTrigger.Execute< const SQChar *, const SQChar *, const SQChar * >(name, pass, addr);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::CustomEvent(SQInt32 group, SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || group == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, SQInt32, Object & >(group, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::WorldOption(SQInt32 option, Object & value)
{
    if (Trigger() && \
        (m_Primary < 0 || option == m_Primary) && (m_Secondary < 0 || option == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, Object & >(option, value);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::WorldToggle(SQInt32 option, bool value)
{
    if (Trigger() && \
        (m_Primary < 0 || option == m_Primary) && (m_Secondary < 0 || value == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, bool >(option, value);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::ScriptReload(SQInt32 header, Object & payload)
{
    if (Trigger() && \
        (m_Primary < 0 || header == m_Primary) && (m_Secondary < 0 || header == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, Object & >(header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::LogMessage(SQInt32 type, const SQChar * message)
{
    if (Trigger() && \
        (m_Primary < 0 || type == m_Primary) && (m_Secondary < 0 || type == m_Secondary))
    {
        m_OnTrigger.Execute< SQInt32, const SQChar * >(type, message);
    }
}

// ------------------------------------------------------------------------------------------------
bool BasicEvent::Trigger()
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
void BasicEvent::Attach()
{
    switch (m_Type)
    {
        case EVT_BLIPCREATED:
            _Core->BlipCreated.Connect< BasicEvent, &BasicEvent::BlipCreated >(this);
        break;
        case EVT_CHECKPOINTCREATED:
            _Core->CheckpointCreated.Connect< BasicEvent, &BasicEvent::CheckpointCreated >(this);
        break;
        case EVT_KEYBINDCREATED:
            _Core->KeybindCreated.Connect< BasicEvent, &BasicEvent::KeybindCreated >(this);
        break;
        case EVT_OBJECTCREATED:
            _Core->ObjectCreated.Connect< BasicEvent, &BasicEvent::ObjectCreated >(this);
        break;
        case EVT_PICKUPCREATED:
            _Core->PickupCreated.Connect< BasicEvent, &BasicEvent::PickupCreated >(this);
        break;
        case EVT_PLAYERCREATED:
            _Core->PlayerCreated.Connect< BasicEvent, &BasicEvent::PlayerCreated >(this);
        break;
        case EVT_SPHERECREATED:
            _Core->SphereCreated.Connect< BasicEvent, &BasicEvent::SphereCreated >(this);
        break;
        case EVT_SPRITECREATED:
            _Core->SpriteCreated.Connect< BasicEvent, &BasicEvent::SpriteCreated >(this);
        break;
        case EVT_TEXTDRAWCREATED:
            _Core->TextdrawCreated.Connect< BasicEvent, &BasicEvent::TextdrawCreated >(this);
        break;
        case EVT_VEHICLECREATED:
            _Core->VehicleCreated.Connect< BasicEvent, &BasicEvent::VehicleCreated >(this);
        break;
        case EVT_BLIPDESTROYED:
            _Core->BlipDestroyed.Connect< BasicEvent, &BasicEvent::BlipDestroyed >(this);
        break;
        case EVT_CHECKPOINTDESTROYED:
            _Core->CheckpointDestroyed.Connect< BasicEvent, &BasicEvent::CheckpointDestroyed >(this);
        break;
        case EVT_KEYBINDDESTROYED:
            _Core->KeybindDestroyed.Connect< BasicEvent, &BasicEvent::KeybindDestroyed >(this);
        break;
        case EVT_OBJECTDESTROYED:
            _Core->ObjectDestroyed.Connect< BasicEvent, &BasicEvent::ObjectDestroyed >(this);
        break;
        case EVT_PICKUPDESTROYED:
            _Core->PickupDestroyed.Connect< BasicEvent, &BasicEvent::PickupDestroyed >(this);
        break;
        case EVT_PLAYERDESTROYED:
            _Core->PlayerDestroyed.Connect< BasicEvent, &BasicEvent::PlayerDestroyed >(this);
        break;
        case EVT_SPHEREDESTROYED:
            _Core->SphereDestroyed.Connect< BasicEvent, &BasicEvent::SphereDestroyed >(this);
        break;
        case EVT_SPRITEDESTROYED:
            _Core->SpriteDestroyed.Connect< BasicEvent, &BasicEvent::SpriteDestroyed >(this);
        break;
        case EVT_TEXTDRAWDESTROYED:
            _Core->TextdrawDestroyed.Connect< BasicEvent, &BasicEvent::TextdrawDestroyed >(this);
        break;
        case EVT_VEHICLEDESTROYED:
            _Core->VehicleDestroyed.Connect< BasicEvent, &BasicEvent::VehicleDestroyed >(this);
        break;
        case EVT_BLIPCUSTOM:
            _Core->BlipCustom.Connect< BasicEvent, &BasicEvent::BlipCustom >(this);
        break;
        case EVT_CHECKPOINTCUSTOM:
            _Core->CheckpointCustom.Connect< BasicEvent, &BasicEvent::CheckpointCustom >(this);
        break;
        case EVT_KEYBINDCUSTOM:
            _Core->KeybindCustom.Connect< BasicEvent, &BasicEvent::KeybindCustom >(this);
        break;
        case EVT_OBJECTCUSTOM:
            _Core->ObjectCustom.Connect< BasicEvent, &BasicEvent::ObjectCustom >(this);
        break;
        case EVT_PICKUPCUSTOM:
            _Core->PickupCustom.Connect< BasicEvent, &BasicEvent::PickupCustom >(this);
        break;
        case EVT_PLAYERCUSTOM:
            _Core->PlayerCustom.Connect< BasicEvent, &BasicEvent::PlayerCustom >(this);
        break;
        case EVT_SPHERECUSTOM:
            _Core->SphereCustom.Connect< BasicEvent, &BasicEvent::SphereCustom >(this);
        break;
        case EVT_SPRITECUSTOM:
            _Core->SpriteCustom.Connect< BasicEvent, &BasicEvent::SpriteCustom >(this);
        break;
        case EVT_TEXTDRAWCUSTOM:
            _Core->TextdrawCustom.Connect< BasicEvent, &BasicEvent::TextdrawCustom >(this);
        break;
        case EVT_VEHICLECUSTOM:
            _Core->VehicleCustom.Connect< BasicEvent, &BasicEvent::VehicleCustom >(this);
        break;
        case EVT_PLAYERAWAY:
            _Core->PlayerAway.Connect< BasicEvent, &BasicEvent::PlayerAway >(this);
        break;
        case EVT_PLAYERGAMEKEYS:
            _Core->PlayerGameKeys.Connect< BasicEvent, &BasicEvent::PlayerGameKeys >(this);
        break;
        case EVT_PLAYERRENAME:
            _Core->PlayerRename.Connect< BasicEvent, &BasicEvent::PlayerRename >(this);
        break;
        case EVT_PLAYERREQUESTCLASS:
            _Core->PlayerRequestClass.Connect< BasicEvent, &BasicEvent::PlayerRequestClass >(this);
        break;
        case EVT_PLAYERREQUESTSPAWN:
            _Core->PlayerRequestSpawn.Connect< BasicEvent, &BasicEvent::PlayerRequestSpawn >(this);
        break;
        case EVT_PLAYERSPAWN:
            _Core->PlayerSpawn.Connect< BasicEvent, &BasicEvent::PlayerSpawn >(this);
        break;
        case EVT_PLAYERSTARTTYPING:
            _Core->PlayerStartTyping.Connect< BasicEvent, &BasicEvent::PlayerStartTyping >(this);
        break;
        case EVT_PLAYERSTOPTYPING:
            _Core->PlayerStopTyping.Connect< BasicEvent, &BasicEvent::PlayerStopTyping >(this);
        break;
        case EVT_PLAYERCHAT:
            _Core->PlayerChat.Connect< BasicEvent, &BasicEvent::PlayerChat >(this);
        break;
        case EVT_PLAYERCOMMAND:
            _Core->PlayerCommand.Connect< BasicEvent, &BasicEvent::PlayerCommand >(this);
        break;
        case EVT_PLAYERMESSAGE:
            _Core->PlayerMessage.Connect< BasicEvent, &BasicEvent::PlayerMessage >(this);
        break;
        case EVT_PLAYERHEALTH:
            _Core->PlayerHealth.Connect< BasicEvent, &BasicEvent::PlayerHealth >(this);
        break;
        case EVT_PLAYERARMOUR:
            _Core->PlayerArmour.Connect< BasicEvent, &BasicEvent::PlayerArmour >(this);
        break;
        case EVT_PLAYERWEAPON:
            _Core->PlayerWeapon.Connect< BasicEvent, &BasicEvent::PlayerWeapon >(this);
        break;
        case EVT_PLAYERMOVE:
            _Core->PlayerMove.Connect< BasicEvent, &BasicEvent::PlayerMove >(this);
        break;
        case EVT_PLAYERWASTED:
            _Core->PlayerWasted.Connect< BasicEvent, &BasicEvent::PlayerWasted >(this);
        break;
        case EVT_PLAYERKILLED:
            _Core->PlayerKilled.Connect< BasicEvent, &BasicEvent::PlayerKilled >(this);
        break;
        case EVT_PLAYERTEAMKILL:
            _Core->PlayerTeamKill.Connect< BasicEvent, &BasicEvent::PlayerTeamKill >(this);
        break;
        case EVT_PLAYERSPECTATE:
            _Core->PlayerSpectate.Connect< BasicEvent, &BasicEvent::PlayerSpectate >(this);
        break;
        case EVT_PLAYERCRASHREPORT:
            _Core->PlayerCrashreport.Connect< BasicEvent, &BasicEvent::PlayerCrashreport >(this);
        break;
        case EVT_PLAYERBURNING:
            _Core->PlayerBurning.Connect< BasicEvent, &BasicEvent::PlayerBurning >(this);
        break;
        case EVT_PLAYERCROUCHING:
            _Core->PlayerCrouching.Connect< BasicEvent, &BasicEvent::PlayerCrouching >(this);
        break;
        case EVT_PLAYERSTATE:
            _Core->PlayerState.Connect< BasicEvent, &BasicEvent::PlayerState >(this);
        break;
        case EVT_PLAYERACTION:
            _Core->PlayerAction.Connect< BasicEvent, &BasicEvent::PlayerAction >(this);
        break;
        case EVT_STATENONE:
            _Core->StateNone.Connect< BasicEvent, &BasicEvent::StateNone >(this);
        break;
        case EVT_STATENORMAL:
            _Core->StateNormal.Connect< BasicEvent, &BasicEvent::StateNormal >(this);
        break;
        case EVT_STATESHOOTING:
            _Core->StateShooting.Connect< BasicEvent, &BasicEvent::StateShooting >(this);
        break;
        case EVT_STATEDRIVER:
            _Core->StateDriver.Connect< BasicEvent, &BasicEvent::StateDriver >(this);
        break;
        case EVT_STATEPASSENGER:
            _Core->StatePassenger.Connect< BasicEvent, &BasicEvent::StatePassenger >(this);
        break;
        case EVT_STATEENTERDRIVER:
            _Core->StateEnterDriver.Connect< BasicEvent, &BasicEvent::StateEnterDriver >(this);
        break;
        case EVT_STATEENTERPASSENGER:
            _Core->StateEnterPassenger.Connect< BasicEvent, &BasicEvent::StateEnterPassenger >(this);
        break;
        case EVT_STATEEXITVEHICLE:
            _Core->StateExitVehicle.Connect< BasicEvent, &BasicEvent::StateExitVehicle >(this);
        break;
        case EVT_STATEUNSPAWNED:
            _Core->StateUnspawned.Connect< BasicEvent, &BasicEvent::StateUnspawned >(this);
        break;
        case EVT_ACTIONNONE:
            _Core->ActionNone.Connect< BasicEvent, &BasicEvent::ActionNone >(this);
        break;
        case EVT_ACTIONNORMAL:
            _Core->ActionNormal.Connect< BasicEvent, &BasicEvent::ActionNormal >(this);
        break;
        case EVT_ACTIONAIMING:
            _Core->ActionAiming.Connect< BasicEvent, &BasicEvent::ActionAiming >(this);
        break;
        case EVT_ACTIONSHOOTING:
            _Core->ActionShooting.Connect< BasicEvent, &BasicEvent::ActionShooting >(this);
        break;
        case EVT_ACTIONJUMPING:
            _Core->ActionJumping.Connect< BasicEvent, &BasicEvent::ActionJumping >(this);
        break;
        case EVT_ACTIONLIEDOWN:
            _Core->ActionLieDown.Connect< BasicEvent, &BasicEvent::ActionLieDown >(this);
        break;
        case EVT_ACTIONGETTINGUP:
            _Core->ActionGettingUp.Connect< BasicEvent, &BasicEvent::ActionGettingUp >(this);
        break;
        case EVT_ACTIONJUMPVEHICLE:
            _Core->ActionJumpVehicle.Connect< BasicEvent, &BasicEvent::ActionJumpVehicle >(this);
        break;
        case EVT_ACTIONDRIVING:
            _Core->ActionDriving.Connect< BasicEvent, &BasicEvent::ActionDriving >(this);
        break;
        case EVT_ACTIONDYING:
            _Core->ActionDying.Connect< BasicEvent, &BasicEvent::ActionDying >(this);
        break;
        case EVT_ACTIONWASTED:
            _Core->ActionWasted.Connect< BasicEvent, &BasicEvent::ActionWasted >(this);
        break;
        case EVT_ACTIONEMBARKING:
            _Core->ActionEmbarking.Connect< BasicEvent, &BasicEvent::ActionEmbarking >(this);
        break;
        case EVT_ACTIONDISEMBARKING:
            _Core->ActionDisembarking.Connect< BasicEvent, &BasicEvent::ActionDisembarking >(this);
        break;
        case EVT_VEHICLERESPAWN:
            _Core->VehicleRespawn.Connect< BasicEvent, &BasicEvent::VehicleRespawn >(this);
        break;
        case EVT_VEHICLEEXPLODE:
            _Core->VehicleExplode.Connect< BasicEvent, &BasicEvent::VehicleExplode >(this);
        break;
        case EVT_VEHICLEHEALTH:
            _Core->VehicleHealth.Connect< BasicEvent, &BasicEvent::VehicleHealth >(this);
        break;
        case EVT_VEHICLEMOVE:
            _Core->VehicleMove.Connect< BasicEvent, &BasicEvent::VehicleMove >(this);
        break;
        case EVT_PICKUPRESPAWN:
            _Core->PickupRespawn.Connect< BasicEvent, &BasicEvent::PickupRespawn >(this);
        break;
        case EVT_KEYBINDKEYPRESS:
            _Core->KeybindKeyPress.Connect< BasicEvent, &BasicEvent::KeybindKeyPress >(this);
        break;
        case EVT_KEYBINDKEYRELEASE:
            _Core->KeybindKeyRelease.Connect< BasicEvent, &BasicEvent::KeybindKeyRelease >(this);
        break;
        case EVT_VEHICLEEMBARKING:
            _Core->VehicleEmbarking.Connect< BasicEvent, &BasicEvent::VehicleEmbarking >(this);
        break;
        case EVT_VEHICLEEMBARKED:
            _Core->VehicleEmbarked.Connect< BasicEvent, &BasicEvent::VehicleEmbarked >(this);
        break;
        case EVT_VEHICLEDISEMBARK:
            _Core->VehicleDisembark.Connect< BasicEvent, &BasicEvent::VehicleDisembark >(this);
        break;
        case EVT_PICKUPCLAIMED:
            _Core->PickupClaimed.Connect< BasicEvent, &BasicEvent::PickupClaimed >(this);
        break;
        case EVT_PICKUPCOLLECTED:
            _Core->PickupCollected.Connect< BasicEvent, &BasicEvent::PickupCollected >(this);
        break;
        case EVT_OBJECTSHOT:
            _Core->ObjectShot.Connect< BasicEvent, &BasicEvent::ObjectShot >(this);
        break;
        case EVT_OBJECTBUMP:
            _Core->ObjectBump.Connect< BasicEvent, &BasicEvent::ObjectBump >(this);
        break;
        case EVT_CHECKPOINTENTERED:
            _Core->CheckpointEntered.Connect< BasicEvent, &BasicEvent::CheckpointEntered >(this);
        break;
        case EVT_CHECKPOINTEXITED:
            _Core->CheckpointExited.Connect< BasicEvent, &BasicEvent::CheckpointExited >(this);
        break;
        case EVT_SPHEREENTERED:
            _Core->SphereEntered.Connect< BasicEvent, &BasicEvent::SphereEntered >(this);
        break;
        case EVT_SPHEREEXITED:
            _Core->SphereExited.Connect< BasicEvent, &BasicEvent::SphereExited >(this);
        break;
        case EVT_SERVERFRAME:
            _Core->ServerFrame.Connect< BasicEvent, &BasicEvent::ServerFrame >(this);
        break;
        case EVT_SERVERSTARTUP:
            _Core->ServerStartup.Connect< BasicEvent, &BasicEvent::ServerStartup >(this);
        break;
        case EVT_SERVERSHUTDOWN:
            _Core->ServerShutdown.Connect< BasicEvent, &BasicEvent::ServerShutdown >(this);
        break;
        case EVT_INTERNALCOMMAND:
            _Core->InternalCommand.Connect< BasicEvent, &BasicEvent::InternalCommand >(this);
        break;
        case EVT_LOGINATTEMPT:
            _Core->LoginAttempt.Connect< BasicEvent, &BasicEvent::LoginAttempt >(this);
        break;
        case EVT_CUSTOMEVENT:
            _Core->CustomEvent.Connect< BasicEvent, &BasicEvent::CustomEvent >(this);
        break;
        case EVT_WORLDOPTION:
            _Core->WorldOption.Connect< BasicEvent, &BasicEvent::WorldOption >(this);
        break;
        case EVT_WORLDTOGGLE:
            _Core->WorldToggle.Connect< BasicEvent, &BasicEvent::WorldToggle >(this);
        break;
        case EVT_SCRIPTRELOAD:
            _Core->ScriptReload.Connect< BasicEvent, &BasicEvent::ScriptReload >(this);
        break;
        case EVT_LOGMESSAGE:
            _Core->LogMessage.Connect< BasicEvent, &BasicEvent::LogMessage >(this);
        break;
        default:
            LogErr("Attempting to <attach> to an unknown event type: %d", _SCI32(m_Type));
    }
}

// ------------------------------------------------------------------------------------------------
void BasicEvent::Detach()
{
    switch (m_Type)
    {
        case EVT_BLIPCREATED:
            _Core->BlipCreated.Disconnect< BasicEvent, &BasicEvent::BlipCreated >(this);
        break;
        case EVT_CHECKPOINTCREATED:
            _Core->CheckpointCreated.Disconnect< BasicEvent, &BasicEvent::CheckpointCreated >(this);
        break;
        case EVT_KEYBINDCREATED:
            _Core->KeybindCreated.Disconnect< BasicEvent, &BasicEvent::KeybindCreated >(this);
        break;
        case EVT_OBJECTCREATED:
            _Core->ObjectCreated.Disconnect< BasicEvent, &BasicEvent::ObjectCreated >(this);
        break;
        case EVT_PICKUPCREATED:
            _Core->PickupCreated.Disconnect< BasicEvent, &BasicEvent::PickupCreated >(this);
        break;
        case EVT_PLAYERCREATED:
            _Core->PlayerCreated.Disconnect< BasicEvent, &BasicEvent::PlayerCreated >(this);
        break;
        case EVT_SPHERECREATED:
            _Core->SphereCreated.Disconnect< BasicEvent, &BasicEvent::SphereCreated >(this);
        break;
        case EVT_SPRITECREATED:
            _Core->SpriteCreated.Disconnect< BasicEvent, &BasicEvent::SpriteCreated >(this);
        break;
        case EVT_TEXTDRAWCREATED:
            _Core->TextdrawCreated.Disconnect< BasicEvent, &BasicEvent::TextdrawCreated >(this);
        break;
        case EVT_VEHICLECREATED:
            _Core->VehicleCreated.Disconnect< BasicEvent, &BasicEvent::VehicleCreated >(this);
        break;
        case EVT_BLIPDESTROYED:
            _Core->BlipDestroyed.Disconnect< BasicEvent, &BasicEvent::BlipDestroyed >(this);
        break;
        case EVT_CHECKPOINTDESTROYED:
            _Core->CheckpointDestroyed.Disconnect< BasicEvent, &BasicEvent::CheckpointDestroyed >(this);
        break;
        case EVT_KEYBINDDESTROYED:
            _Core->KeybindDestroyed.Disconnect< BasicEvent, &BasicEvent::KeybindDestroyed >(this);
        break;
        case EVT_OBJECTDESTROYED:
            _Core->ObjectDestroyed.Disconnect< BasicEvent, &BasicEvent::ObjectDestroyed >(this);
        break;
        case EVT_PICKUPDESTROYED:
            _Core->PickupDestroyed.Disconnect< BasicEvent, &BasicEvent::PickupDestroyed >(this);
        break;
        case EVT_PLAYERDESTROYED:
            _Core->PlayerDestroyed.Disconnect< BasicEvent, &BasicEvent::PlayerDestroyed >(this);
        break;
        case EVT_SPHEREDESTROYED:
            _Core->SphereDestroyed.Disconnect< BasicEvent, &BasicEvent::SphereDestroyed >(this);
        break;
        case EVT_SPRITEDESTROYED:
            _Core->SpriteDestroyed.Disconnect< BasicEvent, &BasicEvent::SpriteDestroyed >(this);
        break;
        case EVT_TEXTDRAWDESTROYED:
            _Core->TextdrawDestroyed.Disconnect< BasicEvent, &BasicEvent::TextdrawDestroyed >(this);
        break;
        case EVT_VEHICLEDESTROYED:
            _Core->VehicleDestroyed.Disconnect< BasicEvent, &BasicEvent::VehicleDestroyed >(this);
        break;
        case EVT_BLIPCUSTOM:
            _Core->BlipCustom.Disconnect< BasicEvent, &BasicEvent::BlipCustom >(this);
        break;
        case EVT_CHECKPOINTCUSTOM:
            _Core->CheckpointCustom.Disconnect< BasicEvent, &BasicEvent::CheckpointCustom >(this);
        break;
        case EVT_KEYBINDCUSTOM:
            _Core->KeybindCustom.Disconnect< BasicEvent, &BasicEvent::KeybindCustom >(this);
        break;
        case EVT_OBJECTCUSTOM:
            _Core->ObjectCustom.Disconnect< BasicEvent, &BasicEvent::ObjectCustom >(this);
        break;
        case EVT_PICKUPCUSTOM:
            _Core->PickupCustom.Disconnect< BasicEvent, &BasicEvent::PickupCustom >(this);
        break;
        case EVT_PLAYERCUSTOM:
            _Core->PlayerCustom.Disconnect< BasicEvent, &BasicEvent::PlayerCustom >(this);
        break;
        case EVT_SPHERECUSTOM:
            _Core->SphereCustom.Disconnect< BasicEvent, &BasicEvent::SphereCustom >(this);
        break;
        case EVT_SPRITECUSTOM:
            _Core->SpriteCustom.Disconnect< BasicEvent, &BasicEvent::SpriteCustom >(this);
        break;
        case EVT_TEXTDRAWCUSTOM:
            _Core->TextdrawCustom.Disconnect< BasicEvent, &BasicEvent::TextdrawCustom >(this);
        break;
        case EVT_VEHICLECUSTOM:
            _Core->VehicleCustom.Disconnect< BasicEvent, &BasicEvent::VehicleCustom >(this);
        break;
        case EVT_PLAYERAWAY:
            _Core->PlayerAway.Disconnect< BasicEvent, &BasicEvent::PlayerAway >(this);
        break;
        case EVT_PLAYERGAMEKEYS:
            _Core->PlayerGameKeys.Disconnect< BasicEvent, &BasicEvent::PlayerGameKeys >(this);
        break;
        case EVT_PLAYERRENAME:
            _Core->PlayerRename.Disconnect< BasicEvent, &BasicEvent::PlayerRename >(this);
        break;
        case EVT_PLAYERREQUESTCLASS:
            _Core->PlayerRequestClass.Disconnect< BasicEvent, &BasicEvent::PlayerRequestClass >(this);
        break;
        case EVT_PLAYERREQUESTSPAWN:
            _Core->PlayerRequestSpawn.Disconnect< BasicEvent, &BasicEvent::PlayerRequestSpawn >(this);
        break;
        case EVT_PLAYERSPAWN:
            _Core->PlayerSpawn.Disconnect< BasicEvent, &BasicEvent::PlayerSpawn >(this);
        break;
        case EVT_PLAYERSTARTTYPING:
            _Core->PlayerStartTyping.Disconnect< BasicEvent, &BasicEvent::PlayerStartTyping >(this);
        break;
        case EVT_PLAYERSTOPTYPING:
            _Core->PlayerStopTyping.Disconnect< BasicEvent, &BasicEvent::PlayerStopTyping >(this);
        break;
        case EVT_PLAYERCHAT:
            _Core->PlayerChat.Disconnect< BasicEvent, &BasicEvent::PlayerChat >(this);
        break;
        case EVT_PLAYERCOMMAND:
            _Core->PlayerCommand.Disconnect< BasicEvent, &BasicEvent::PlayerCommand >(this);
        break;
        case EVT_PLAYERMESSAGE:
            _Core->PlayerMessage.Disconnect< BasicEvent, &BasicEvent::PlayerMessage >(this);
        break;
        case EVT_PLAYERHEALTH:
            _Core->PlayerHealth.Disconnect< BasicEvent, &BasicEvent::PlayerHealth >(this);
        break;
        case EVT_PLAYERARMOUR:
            _Core->PlayerArmour.Disconnect< BasicEvent, &BasicEvent::PlayerArmour >(this);
        break;
        case EVT_PLAYERWEAPON:
            _Core->PlayerWeapon.Disconnect< BasicEvent, &BasicEvent::PlayerWeapon >(this);
        break;
        case EVT_PLAYERMOVE:
            _Core->PlayerMove.Disconnect< BasicEvent, &BasicEvent::PlayerMove >(this);
        break;
        case EVT_PLAYERWASTED:
            _Core->PlayerWasted.Disconnect< BasicEvent, &BasicEvent::PlayerWasted >(this);
        break;
        case EVT_PLAYERKILLED:
            _Core->PlayerKilled.Disconnect< BasicEvent, &BasicEvent::PlayerKilled >(this);
        break;
        case EVT_PLAYERTEAMKILL:
            _Core->PlayerTeamKill.Disconnect< BasicEvent, &BasicEvent::PlayerTeamKill >(this);
        break;
        case EVT_PLAYERSPECTATE:
            _Core->PlayerSpectate.Disconnect< BasicEvent, &BasicEvent::PlayerSpectate >(this);
        break;
        case EVT_PLAYERCRASHREPORT:
            _Core->PlayerCrashreport.Disconnect< BasicEvent, &BasicEvent::PlayerCrashreport >(this);
        break;
        case EVT_PLAYERBURNING:
            _Core->PlayerBurning.Disconnect< BasicEvent, &BasicEvent::PlayerBurning >(this);
        break;
        case EVT_PLAYERCROUCHING:
            _Core->PlayerCrouching.Disconnect< BasicEvent, &BasicEvent::PlayerCrouching >(this);
        break;
        case EVT_PLAYERSTATE:
            _Core->PlayerState.Disconnect< BasicEvent, &BasicEvent::PlayerState >(this);
        break;
        case EVT_PLAYERACTION:
            _Core->PlayerAction.Disconnect< BasicEvent, &BasicEvent::PlayerAction >(this);
        break;
        case EVT_STATENONE:
            _Core->StateNone.Disconnect< BasicEvent, &BasicEvent::StateNone >(this);
        break;
        case EVT_STATENORMAL:
            _Core->StateNormal.Disconnect< BasicEvent, &BasicEvent::StateNormal >(this);
        break;
        case EVT_STATESHOOTING:
            _Core->StateShooting.Disconnect< BasicEvent, &BasicEvent::StateShooting >(this);
        break;
        case EVT_STATEDRIVER:
            _Core->StateDriver.Disconnect< BasicEvent, &BasicEvent::StateDriver >(this);
        break;
        case EVT_STATEPASSENGER:
            _Core->StatePassenger.Disconnect< BasicEvent, &BasicEvent::StatePassenger >(this);
        break;
        case EVT_STATEENTERDRIVER:
            _Core->StateEnterDriver.Disconnect< BasicEvent, &BasicEvent::StateEnterDriver >(this);
        break;
        case EVT_STATEENTERPASSENGER:
            _Core->StateEnterPassenger.Disconnect< BasicEvent, &BasicEvent::StateEnterPassenger >(this);
        break;
        case EVT_STATEEXITVEHICLE:
            _Core->StateExitVehicle.Disconnect< BasicEvent, &BasicEvent::StateExitVehicle >(this);
        break;
        case EVT_STATEUNSPAWNED:
            _Core->StateUnspawned.Disconnect< BasicEvent, &BasicEvent::StateUnspawned >(this);
        break;
        case EVT_ACTIONNONE:
            _Core->ActionNone.Disconnect< BasicEvent, &BasicEvent::ActionNone >(this);
        break;
        case EVT_ACTIONNORMAL:
            _Core->ActionNormal.Disconnect< BasicEvent, &BasicEvent::ActionNormal >(this);
        break;
        case EVT_ACTIONAIMING:
            _Core->ActionAiming.Disconnect< BasicEvent, &BasicEvent::ActionAiming >(this);
        break;
        case EVT_ACTIONSHOOTING:
            _Core->ActionShooting.Disconnect< BasicEvent, &BasicEvent::ActionShooting >(this);
        break;
        case EVT_ACTIONJUMPING:
            _Core->ActionJumping.Disconnect< BasicEvent, &BasicEvent::ActionJumping >(this);
        break;
        case EVT_ACTIONLIEDOWN:
            _Core->ActionLieDown.Disconnect< BasicEvent, &BasicEvent::ActionLieDown >(this);
        break;
        case EVT_ACTIONGETTINGUP:
            _Core->ActionGettingUp.Disconnect< BasicEvent, &BasicEvent::ActionGettingUp >(this);
        break;
        case EVT_ACTIONJUMPVEHICLE:
            _Core->ActionJumpVehicle.Disconnect< BasicEvent, &BasicEvent::ActionJumpVehicle >(this);
        break;
        case EVT_ACTIONDRIVING:
            _Core->ActionDriving.Disconnect< BasicEvent, &BasicEvent::ActionDriving >(this);
        break;
        case EVT_ACTIONDYING:
            _Core->ActionDying.Disconnect< BasicEvent, &BasicEvent::ActionDying >(this);
        break;
        case EVT_ACTIONWASTED:
            _Core->ActionWasted.Disconnect< BasicEvent, &BasicEvent::ActionWasted >(this);
        break;
        case EVT_ACTIONEMBARKING:
            _Core->ActionEmbarking.Disconnect< BasicEvent, &BasicEvent::ActionEmbarking >(this);
        break;
        case EVT_ACTIONDISEMBARKING:
            _Core->ActionDisembarking.Disconnect< BasicEvent, &BasicEvent::ActionDisembarking >(this);
        break;
        case EVT_VEHICLERESPAWN:
            _Core->VehicleRespawn.Disconnect< BasicEvent, &BasicEvent::VehicleRespawn >(this);
        break;
        case EVT_VEHICLEEXPLODE:
            _Core->VehicleExplode.Disconnect< BasicEvent, &BasicEvent::VehicleExplode >(this);
        break;
        case EVT_VEHICLEHEALTH:
            _Core->VehicleHealth.Disconnect< BasicEvent, &BasicEvent::VehicleHealth >(this);
        break;
        case EVT_VEHICLEMOVE:
            _Core->VehicleMove.Disconnect< BasicEvent, &BasicEvent::VehicleMove >(this);
        break;
        case EVT_PICKUPRESPAWN:
            _Core->PickupRespawn.Disconnect< BasicEvent, &BasicEvent::PickupRespawn >(this);
        break;
        case EVT_KEYBINDKEYPRESS:
            _Core->KeybindKeyPress.Disconnect< BasicEvent, &BasicEvent::KeybindKeyPress >(this);
        break;
        case EVT_KEYBINDKEYRELEASE:
            _Core->KeybindKeyRelease.Disconnect< BasicEvent, &BasicEvent::KeybindKeyRelease >(this);
        break;
        case EVT_VEHICLEEMBARKING:
            _Core->VehicleEmbarking.Disconnect< BasicEvent, &BasicEvent::VehicleEmbarking >(this);
        break;
        case EVT_VEHICLEEMBARKED:
            _Core->VehicleEmbarked.Disconnect< BasicEvent, &BasicEvent::VehicleEmbarked >(this);
        break;
        case EVT_VEHICLEDISEMBARK:
            _Core->VehicleDisembark.Disconnect< BasicEvent, &BasicEvent::VehicleDisembark >(this);
        break;
        case EVT_PICKUPCLAIMED:
            _Core->PickupClaimed.Disconnect< BasicEvent, &BasicEvent::PickupClaimed >(this);
        break;
        case EVT_PICKUPCOLLECTED:
            _Core->PickupCollected.Disconnect< BasicEvent, &BasicEvent::PickupCollected >(this);
        break;
        case EVT_OBJECTSHOT:
            _Core->ObjectShot.Disconnect< BasicEvent, &BasicEvent::ObjectShot >(this);
        break;
        case EVT_OBJECTBUMP:
            _Core->ObjectBump.Disconnect< BasicEvent, &BasicEvent::ObjectBump >(this);
        break;
        case EVT_CHECKPOINTENTERED:
            _Core->CheckpointEntered.Disconnect< BasicEvent, &BasicEvent::CheckpointEntered >(this);
        break;
        case EVT_CHECKPOINTEXITED:
            _Core->CheckpointExited.Disconnect< BasicEvent, &BasicEvent::CheckpointExited >(this);
        break;
        case EVT_SPHEREENTERED:
            _Core->SphereEntered.Disconnect< BasicEvent, &BasicEvent::SphereEntered >(this);
        break;
        case EVT_SPHEREEXITED:
            _Core->SphereExited.Disconnect< BasicEvent, &BasicEvent::SphereExited >(this);
        break;
        case EVT_SERVERFRAME:
            _Core->ServerFrame.Disconnect< BasicEvent, &BasicEvent::ServerFrame >(this);
        break;
        case EVT_SERVERSTARTUP:
            _Core->ServerStartup.Disconnect< BasicEvent, &BasicEvent::ServerStartup >(this);
        break;
        case EVT_SERVERSHUTDOWN:
            _Core->ServerShutdown.Disconnect< BasicEvent, &BasicEvent::ServerShutdown >(this);
        break;
        case EVT_INTERNALCOMMAND:
            _Core->InternalCommand.Disconnect< BasicEvent, &BasicEvent::InternalCommand >(this);
        break;
        case EVT_LOGINATTEMPT:
            _Core->LoginAttempt.Disconnect< BasicEvent, &BasicEvent::LoginAttempt >(this);
        break;
        case EVT_CUSTOMEVENT:
            _Core->CustomEvent.Disconnect< BasicEvent, &BasicEvent::CustomEvent >(this);
        break;
        case EVT_WORLDOPTION:
            _Core->WorldOption.Disconnect< BasicEvent, &BasicEvent::WorldOption >(this);
        break;
        case EVT_WORLDTOGGLE:
            _Core->WorldToggle.Disconnect< BasicEvent, &BasicEvent::WorldToggle >(this);
        break;
        case EVT_SCRIPTRELOAD:
            _Core->ScriptReload.Disconnect< BasicEvent, &BasicEvent::ScriptReload >(this);
        break;
        case EVT_LOGMESSAGE:
            _Core->LogMessage.Disconnect< BasicEvent, &BasicEvent::LogMessage >(this);
        break;
        default:
            LogErr("Attempting to <dettach> to an unknown event type: %d", _SCI32(m_Type));
    }
}

// ================================================================================================
bool Register_BasicEvent(HSQUIRRELVM vm)
{
    // Output debugging information
    LogDbg("Beginning registration of <BasicEvent> type");
    // Events should not be copied for the sake of simplicity
    typedef NoCopy< BasicEvent > Allocator;
    // Attempt to register the specified type
    Sqrat::RootTable(vm).Bind(_SC("BasicEvent"), Sqrat::Class< BasicEvent, Allocator >(vm, _SC("BasicEvent"))
        /* Constructors */
        .Ctor()
        .Ctor< SQInt32 >()
        .Ctor< SQInt32, bool >()
        /* Metamethods */
        .Func(_SC("_cmp"), &BasicEvent::Cmp)
        .Func(_SC("_tostring"), &BasicEvent::GetName)
        /* Properties */
        .Prop(_SC("ltag"), &BasicEvent::GetTag, &BasicEvent::SetTag)
        .Prop(_SC("ldata"), &BasicEvent::GetData, &BasicEvent::SetData)
        .Prop(_SC("type"), &BasicEvent::GetType , &BasicEvent::SetType)
        .Prop(_SC("idle"), &BasicEvent::GetIdle, &BasicEvent::SetIdle)
        .Prop(_SC("is_idle"), &BasicEvent::IsIdle)
        .Prop(_SC("stride"), &BasicEvent::GetStride, &BasicEvent::SetStride)
        .Prop(_SC("ignore"), &BasicEvent::GetIgnore, &BasicEvent::SetIgnore)
        .Prop(_SC("primary"), &BasicEvent::GetPrimary, &BasicEvent::SetPrimary)
        .Prop(_SC("secondary"), &BasicEvent::GetSecondary, &BasicEvent::SetSecondary)
        .Prop(_SC("suspended"), &BasicEvent::GetSuspended, &BasicEvent::SetSuspended)
        .Func(_SC("compatible"), &BasicEvent::Compatible)
        .Func(_SC("name"), &BasicEvent::GetName)
        .Prop(_SC("on_trigger"), &BasicEvent::GetOnTrigger, &BasicEvent::SetOnTrigger)
        /* Functions */
        .Func(_SC("set_on_trigger"), &BasicEvent::SetOnTrigger_Env)
    );
    // Output debugging information
    LogDbg("Registration of <BasicEvent> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
