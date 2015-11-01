#ifndef _SQMOD_EVENT_BASIC_HPP_
#define _SQMOD_EVENT_BASIC_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include <chrono>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
class BasicEvent
{
protected:

    // --------------------------------------------------------------------------------------------
    typedef std::chrono::time_point< std::chrono::steady_clock > TimePoint;

public:

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    BasicEvent() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    BasicEvent(SQInt32 type) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    BasicEvent(SQInt32 type, bool suspended) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    BasicEvent(const BasicEvent &) = delete;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    BasicEvent(BasicEvent &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    ~BasicEvent();

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    BasicEvent & operator = (const BasicEvent &) = delete;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    BasicEvent & operator = (BasicEvent &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    bool operator == (const BasicEvent & o) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    bool operator != (const BasicEvent & o) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    bool operator < (const BasicEvent & o) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    bool operator > (const BasicEvent & o) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    bool operator <= (const BasicEvent & o) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    bool operator >= (const BasicEvent & o) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    operator SQInt32 () const noexcept
    {
        return m_Type;
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    operator bool () const noexcept
    {
        return (m_Type != EVT_UNKNOWN && m_Type < EVT_COUNT);
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    operator ! () const noexcept
    {
        return (m_Type == EVT_UNKNOWN || m_Type >= EVT_COUNT);
    }

    /* --------------------------------------------------------------------------------------------
      * ...
     */
    void VMClose() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 Cmp(const BasicEvent & o) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    const SQChar * GetName() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    const SQChar * GetTag() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetTag(const SQChar * tag) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SqObj & GetData() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetData(SqObj & data) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 GetType() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetType(SQInt32 type) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInteger GetIdle() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetIdle(SQInteger millis) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    bool IsIdle() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 GetStride() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetStride(SQInt32 stride) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 GetIgnore() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetIgnore(SQInt32 ignore) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 GetPrimary() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetPrimary(SQInt32 subset) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 GetSecondary() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetSecondary(SQInt32 subset) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    bool GetSuspended() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetSuspended(bool toggle) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnTrigger() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnTrigger(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnTrigger_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    bool Compatible(SQInt32 type) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void BlipCreated(SQInt32 blip, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void CheckpointCreated(SQInt32 checkpoint, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void KeybindCreated(SQInt32 keybind, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ObjectCreated(SQInt32 object, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PickupCreated(SQInt32 pickup, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerCreated(SQInt32 player, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SphereCreated(SQInt32 sphere, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SpriteCreated(SQInt32 sprite, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void TextdrawCreated(SQInt32 textdraw, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void VehicleCreated(SQInt32 vehicle, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void BlipDestroyed(SQInt32 blip, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void CheckpointDestroyed(SQInt32 checkpoint, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void KeybindDestroyed(SQInt32 keybind, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ObjectDestroyed(SQInt32 object, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PickupDestroyed(SQInt32 pickup, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerDestroyed(SQInt32 player, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SphereDestroyed(SQInt32 sphere, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SpriteDestroyed(SQInt32 sprite, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void TextdrawDestroyed(SQInt32 textdraw, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void VehicleDestroyed(SQInt32 vehicle, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void BlipCustom(SQInt32 blip, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void CheckpointCustom(SQInt32 checkpoint, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void KeybindCustom(SQInt32 keybind, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ObjectCustom(SQInt32 object, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PickupCustom(SQInt32 pickup, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerCustom(SQInt32 player, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SphereCustom(SQInt32 sphere, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SpriteCustom(SQInt32 sprite, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void TextdrawCustom(SQInt32 textdraw, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void VehicleCustom(SQInt32 vehicle, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerAway(SQInt32 player, bool status) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerGameKeys(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerRename(SQInt32 player, const SQChar * previous, const SQChar * current) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerRequestClass(SQInt32 player, SQInt32 offset) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerRequestSpawn(SQInt32 player) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerSpawn(SQInt32 player) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerStartTyping(SQInt32 player) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerStopTyping(SQInt32 player) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerChat(SQInt32 player, const SQChar * message) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerCommand(SQInt32 player, const SQChar * command) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerMessage(SQInt32 player, SQInt32 receiver, const SQChar * message) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerHealth(SQInt32 player, SQFloat previous, SQFloat current) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerArmour(SQInt32 player, SQFloat previous, SQFloat current) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerWeapon(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerMove(SQInt32 player, const Vector3 & previous, const Vector3 & current) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerWasted(SQInt32 player, SQInt32 reason) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerKilled(SQInt32 player, SQInt32 killer, SQInt32 reason, SQInt32 body_part) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerTeamKill(SQInt32 player, SQInt32 killer, SQInt32 reason, SQInt32 body_part) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerSpectate(SQInt32 player, SQInt32 target) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerCrashreport(SQInt32 player, const SQChar * report) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerBurning(SQInt32 player, bool state) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerCrouching(SQInt32 player, bool state) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerState(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PlayerAction(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void StateNone(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void StateNormal(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void StateShooting(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void StateDriver(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void StatePassenger(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void StateEnterDriver(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void StateEnterPassenger(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void StateExitVehicle(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void StateUnspawned(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ActionNone(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ActionNormal(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ActionAiming(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ActionShooting(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ActionJumping(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ActionLieDown(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ActionGettingUp(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ActionJumpVehicle(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ActionDriving(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ActionDying(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ActionWasted(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ActionEmbarking(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ActionDisembarking(SQInt32 player, SQInt32 previous) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void VehicleRespawn(SQInt32 vehicle) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void VehicleExplode(SQInt32 vehicle) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void VehicleHealth(SQInt32 vehicle, SQFloat previous, SQFloat current) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void VehicleMove(SQInt32 vehicle, const Vector3 & previous, const Vector3 &current) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PickupRespawn(SQInt32 pickup) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void KeybindKeyPress(SQInt32 player, SQInt32 keybind) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void KeybindKeyRelease(SQInt32 player, SQInt32 keybind) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void VehicleEmbarking(SQInt32 player, SQInt32 vehicle, SQInt32 slot) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void VehicleEmbarked(SQInt32 player, SQInt32 vehicle, SQInt32 slot) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void VehicleDisembark(SQInt32 player, SQInt32 vehicle) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PickupClaimed(SQInt32 player, SQInt32 pickup) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PickupCollected(SQInt32 player, SQInt32 pickup) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ObjectShot(SQInt32 player, SQInt32 object, SQInt32 weapon) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ObjectBump(SQInt32 player, SQInt32 object) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void CheckpointEntered(SQInt32 player, SQInt32 checkpoint) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void CheckpointExited(SQInt32 player, SQInt32 checkpoint) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SphereEntered(SQInt32 player, SQInt32 sphere) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SphereExited(SQInt32 player, SQInt32 sphere) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ServerFrame(SQFloat delta) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ServerStartup() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ServerShutdown() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void InternalCommand(SQInt32 type, const SQChar * text) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void LoginAttempt(const SQChar * name, const SQChar * pass, const SQChar * addr) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void CustomEvent(SQInt32 group, SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void WorldOption(SQInt32 option, Object & value) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void WorldToggle(SQInt32 option, bool value) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ScriptReload(SQInt32 header, Object & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void LogMessage(SQInt32 type, const SQChar * message) noexcept;

protected:

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    bool Trigger() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void Attach() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void Detach() noexcept;

private:

    // --------------------------------------------------------------------------------------------
    SQInt32             m_Type;

    // --------------------------------------------------------------------------------------------
    SQInt32             m_Stride;
    SQInt32             m_Ignore;

    // --------------------------------------------------------------------------------------------
    SQInt32             m_Primary;
    SQInt32             m_Secondary;

    // --------------------------------------------------------------------------------------------
    TimePoint           m_Idle;

    // --------------------------------------------------------------------------------------------
    Function            m_OnTrigger;

    // --------------------------------------------------------------------------------------------
    SqTag               m_Tag;
    SqObj               m_Data;

    // --------------------------------------------------------------------------------------------
    bool                m_Suspended;
};

} // Namespace:: SqMod

#endif // _SQMOD_EVENT_BASIC_HPP_
