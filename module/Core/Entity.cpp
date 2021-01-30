// ------------------------------------------------------------------------------------------------
#include "Core/Entity.hpp"
#include "Core.hpp"
#include "Logger.hpp"

// ------------------------------------------------------------------------------------------------
#include "Entity/Blip.hpp"
#include "Entity/Checkpoint.hpp"
#include "Entity/KeyBind.hpp"
#include "Entity/Object.hpp"
#include "Entity/Pickup.hpp"
#include "Entity/Player.hpp"
#include "Entity/Vehicle.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
#define SQMOD_CATCH_EVENT_EXCEPTION(action) /*
*/ catch (const Sqrat::Exception & e) /*
*/ { /*
*/  LogErr("Squirrel exception caught " action); /*
*/  Logger::Get().DebugF(SqVM(), "%s", e.what()); /*
*/ } /*
*/

// ------------------------------------------------------------------------------------------------
extern void CleanupTasks(int32_t id, int32_t type);

// ------------------------------------------------------------------------------------------------
BlipInst::~BlipInst()
{
    if (VALID_ENTITY(mID))
    {
        Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullLightObj());
    }
}

// ------------------------------------------------------------------------------------------------
CheckpointInst::~CheckpointInst()
{
    if (VALID_ENTITY(mID))
    {
        Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullLightObj());
    }
}

// ------------------------------------------------------------------------------------------------
KeyBindInst::~KeyBindInst()
{
    if (VALID_ENTITY(mID))
    {
        Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullLightObj());
    }
}

// ------------------------------------------------------------------------------------------------
ObjectInst::~ObjectInst()
{
    if (VALID_ENTITY(mID))
    {
        Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullLightObj());
    }
}

// ------------------------------------------------------------------------------------------------
PickupInst::~PickupInst()
{
    if (VALID_ENTITY(mID))
    {
        Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullLightObj());
    }
}

// ------------------------------------------------------------------------------------------------
PlayerInst::~PlayerInst()
{
    if (VALID_ENTITY(mID))
    {
        Destroy(false, SQMOD_DESTROY_CLEANUP, NullLightObj());
    }
}

// ------------------------------------------------------------------------------------------------
VehicleInst::~VehicleInst()
{
    if (VALID_ENTITY(mID))
    {
        Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullLightObj());
    }
}

// ------------------------------------------------------------------------------------------------
void BlipInst::Destroy(bool destroy, int32_t header, LightObj & payload)
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
    {
        // Don't leave exceptions to prevent us from releasing this instance
        try
        {
            Core::Get().EmitBlipDestroyed(mID, header, payload);
        }
        SQMOD_CATCH_EVENT_EXCEPTION("while destroying blip")
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Prevent further use of the manager instance
    mInst = nullptr;
    // Release the script object, if any
    mObj.Release();
    // Release tasks, if any
    CleanupTasks(mID, ENT_BLIP);
    // Are we supposed to clean up this entity? (only at reload)
    if (destroy && VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU32 bg(mFlags, static_cast< uint32_t >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->DestroyCoordBlip(mID);
    }
    // Reset the instance to it's initial state
    ResetInstance();
    // Don't release the callbacks abruptly
    DropEvents();
}

// ------------------------------------------------------------------------------------------------
void CheckpointInst::Destroy(bool destroy, int32_t header, LightObj & payload)
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
    {
        // Don't leave exceptions to prevent us from releasing this instance
        try
        {
            Core::Get().EmitCheckpointDestroyed(mID, header, payload);
        }
        SQMOD_CATCH_EVENT_EXCEPTION("while destroying checkpoint")
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Prevent further use of the manager instance
    mInst = nullptr;
    // Release the script object, if any
    mObj.Release();
    // Release tasks, if any
    CleanupTasks(mID, ENT_CHECKPOINT);
    // Are we supposed to clean up this entity? (only at reload)
    if (destroy && VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU32 bg(mFlags, static_cast< uint32_t >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->DeleteCheckPoint(mID);
    }
    // Reset the instance to it's initial state
    ResetInstance();
    // Don't release the callbacks abruptly
    DropEvents();
}

// ------------------------------------------------------------------------------------------------
void KeyBindInst::Destroy(bool destroy, int32_t header, LightObj & payload)
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
    {
        // Don't leave exceptions to prevent us from releasing this instance
        try
        {
            Core::Get().EmitKeyBindDestroyed(mID, header, payload);
        }
        SQMOD_CATCH_EVENT_EXCEPTION("while destroying keybind")
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Prevent further use of the manager instance
    mInst = nullptr;
    // Release the script object, if any
    mObj.Release();
    // Release tasks, if any
    CleanupTasks(mID, ENT_KEYBIND);
    // Are we supposed to clean up this entity? (only at reload)
    if (destroy && VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU32 bg(mFlags, static_cast< uint32_t >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->RemoveKeyBind(mID);
    }
    // Reset the instance to it's initial state
    ResetInstance();
    // Don't release the callbacks abruptly
    DropEvents();
}

// ------------------------------------------------------------------------------------------------
void ObjectInst::Destroy(bool destroy, int32_t header, LightObj & payload)
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
    {
        // Don't leave exceptions to prevent us from releasing this instance
        try
        {
            Core::Get().EmitObjectDestroyed(mID, header, payload);
        }
        SQMOD_CATCH_EVENT_EXCEPTION("while destroying object")
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Prevent further use of the manager instance
    mInst = nullptr;
    // Release the script object, if any
    mObj.Release();
    // Release tasks, if any
    CleanupTasks(mID, ENT_OBJECT);
    // Are we supposed to clean up this entity? (only at reload)
    if (destroy && VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU32 bg(mFlags, static_cast< uint32_t >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->DeleteObject(mID);
    }
    // Reset the instance to it's initial state
    ResetInstance();
    // Don't release the callbacks abruptly
    DropEvents();
}

// ------------------------------------------------------------------------------------------------
void PickupInst::Destroy(bool destroy, int32_t header, LightObj & payload)
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
    {
        // Don't leave exceptions to prevent us from releasing this instance
        try
        {
            Core::Get().EmitPickupDestroyed(mID, header, payload);
        }
        SQMOD_CATCH_EVENT_EXCEPTION("while destroying pickup")
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Prevent further use of the manager instance
    mInst = nullptr;
    // Release the script object, if any
    mObj.Release();
    // Release tasks, if any
    CleanupTasks(mID, ENT_PICKUP);
    // Are we supposed to clean up this entity? (only at reload)
    if (destroy && VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU32 bg(mFlags, static_cast< uint32_t >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->DeletePickup(mID);
    }
    // Reset the instance to it's initial state
    ResetInstance();
    // Don't release the callbacks abruptly
    DropEvents();
}

// ------------------------------------------------------------------------------------------------
void PlayerInst::Destroy(bool /*destroy*/, int32_t header, LightObj & payload)
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
    {
        // Don't leave exceptions to prevent us from releasing this instance
        try
        {
            Core::Get().EmitPlayerDestroyed(mID, header, payload);
        }
        SQMOD_CATCH_EVENT_EXCEPTION("while destroying player")
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
        // Release the used memory buffer
        mInst->m_Buffer.ResetAll();
    }
    // Prevent further use of the manager instance
    mInst = nullptr;
    // Release the script object, if any
    mObj.Release();
    // Release tasks, if any
    CleanupTasks(mID, ENT_PLAYER);
    // Reset the instance to it's initial state
    ResetInstance();
    // Don't release the callbacks abruptly
    DropEvents();
}

// ------------------------------------------------------------------------------------------------
void VehicleInst::Destroy(bool destroy, int32_t header, LightObj & payload)
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
    {
        // Don't leave exceptions to prevent us from releasing this instance
        try
        {
            Core::Get().EmitVehicleDestroyed(mID, header, payload);
        }
        SQMOD_CATCH_EVENT_EXCEPTION("while destroying vehicle")
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Prevent further use of the manager instance
    mInst = nullptr;
    // Release the script object, if any
    mObj.Release();
    // Release tasks, if any
    CleanupTasks(mID, ENT_VEHICLE);
    // Are we supposed to clean up this entity? (only at reload)
    if (destroy && VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU32 bg(mFlags, static_cast< uint32_t >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->DeleteVehicle(mID);
    }
    // Reset the instance to it's initial state
    ResetInstance();
    // Don't release the callbacks abruptly
    DropEvents();
}

// ------------------------------------------------------------------------------------------------
void BlipInst::ResetInstance()
{
    mID = -1;
    mFlags = ENF_DEFAULT;
    mWorld = -1;
    mScale = -1;
    mSprID = -1;
    mPosition.Clear();
    mColor.Clear();
}

// ------------------------------------------------------------------------------------------------
void CheckpointInst::ResetInstance()
{
    mID = -1;
    mFlags = ENF_DEFAULT;
}

// ------------------------------------------------------------------------------------------------
void KeyBindInst::ResetInstance()
{
    mID = -1;
    mFlags = ENF_DEFAULT;
    mFirst = -1;
    mSecond = -1;
    mThird = -1;
    mRelease = -1;
}

// ------------------------------------------------------------------------------------------------
void ObjectInst::ResetInstance()
{
    mID = -1;
    mFlags = ENF_DEFAULT;
}

// ------------------------------------------------------------------------------------------------
void PickupInst::ResetInstance()
{
    mID = -1;
    mFlags = ENF_DEFAULT;
}

// ------------------------------------------------------------------------------------------------
void PlayerInst::ResetInstance()
{
    mID = -1;
    mFlags = ENF_DEFAULT;
    mAreas.clear();
    mDistance = 0;
    mTrackPosition = 0;
    mTrackHeading = 0;
    mTrackPositionHeader = 0;
    mTrackPositionPayload.Release();
    mKickBanHeader = 0;
    mKickBanPayload.Release();
    mLastWeapon = -1;
    mLastHealth = 0.0;
    mLastArmour = 0.0;
    mLastHeading = 0.0;
    mLastPosition.Clear();
    mAuthority = 0;
}

// ------------------------------------------------------------------------------------------------
void VehicleInst::ResetInstance()
{
    mID = -1;
    mFlags = ENF_DEFAULT;
    mAreas.clear();
    mDistance = 0;
    mTrackPosition = 0;
    mTrackRotation = 0;
    mLastPrimaryColor = -1;
    mLastSecondaryColor = -1;
    mLastHealth = 0.0;
    mLastPosition.Clear();
    mLastRotation.Clear();
}

// ------------------------------------------------------------------------------------------------
void BlipInst::InitEvents()
{
    // Ignore the call if already initialized
    if (!mEvents.IsNull())
    {
        return;
    }
    // Create a new table on the stack
    sq_newtableex(SqVM(), 4);
    // Grab the table object from the stack
    mEvents = LightObj(-1, SqVM());
    // Pop the table object from the stack
    sq_pop(SqVM(), 1);
    // Proceed to initializing the events
    InitSignalPair(mOnDestroyed, mEvents, "Destroyed");
    InitSignalPair(mOnCustom, mEvents, "Custom");
}

// ------------------------------------------------------------------------------------------------
void BlipInst::DropEvents()
{
    ResetSignalPair(mOnDestroyed);
    ResetSignalPair(mOnCustom);
    mEvents.Release();
}

// ------------------------------------------------------------------------------------------------
void CheckpointInst::InitEvents()
{
    // Ignore the call if already initialized
    if (!mEvents.IsNull())
    {
        return;
    }
    // Create a new table on the stack
    sq_newtableex(SqVM(), 8);
    // Grab the table object from the stack
    mEvents = LightObj(-1, SqVM());
    // Pop the table object from the stack
    sq_pop(SqVM(), 1);
    // Proceed to initializing the events
    InitSignalPair(mOnDestroyed, mEvents, "Destroyed");
    InitSignalPair(mOnCustom, mEvents, "Custom");
#if SQMOD_SDK_LEAST(2, 1)
    InitSignalPair(mOnStream, mEvents, "Stream");
#endif
    InitSignalPair(mOnEntered, mEvents, "Entered");
    InitSignalPair(mOnExited, mEvents, "Exited");
    InitSignalPair(mOnWorld, mEvents, "World");
    InitSignalPair(mOnRadius, mEvents, "Radius");
}

// ------------------------------------------------------------------------------------------------
void CheckpointInst::DropEvents()
{
    ResetSignalPair(mOnDestroyed);
    ResetSignalPair(mOnCustom);
#if SQMOD_SDK_LEAST(2, 1)
    ResetSignalPair(mOnStream);
#endif
    ResetSignalPair(mOnEntered);
    ResetSignalPair(mOnExited);
    ResetSignalPair(mOnWorld);
    ResetSignalPair(mOnRadius);
    mEvents.Release();
}

// ------------------------------------------------------------------------------------------------
void KeyBindInst::InitEvents()
{
    // Ignore the call if already initialized
    if (!mEvents.IsNull())
    {
        return;
    }
    // Create a new table on the stack
    sq_newtableex(SqVM(), 8);
    // Grab the table object from the stack
    mEvents = LightObj(-1, SqVM());
    // Pop the table object from the stack
    sq_pop(SqVM(), 1);
    // Proceed to initializing the events
    InitSignalPair(mOnDestroyed, mEvents, "Destroyed");
    InitSignalPair(mOnCustom, mEvents, "Custom");
    InitSignalPair(mOnKeyPress, mEvents, "KeyPress");
    InitSignalPair(mOnKeyRelease, mEvents, "KeyRelease");
}

// ------------------------------------------------------------------------------------------------
void KeyBindInst::DropEvents()
{
    ResetSignalPair(mOnDestroyed);
    ResetSignalPair(mOnCustom);
    ResetSignalPair(mOnKeyPress);
    ResetSignalPair(mOnKeyRelease);
    mEvents.Release();
}

// ------------------------------------------------------------------------------------------------
void ObjectInst::InitEvents()
{
    // Ignore the call if already initialized
    if (!mEvents.IsNull())
    {
        return;
    }
    // Create a new table on the stack
    sq_newtableex(SqVM(), 12);
    // Grab the table object from the stack
    mEvents = LightObj(-1, SqVM());
    // Pop the table object from the stack
    sq_pop(SqVM(), 1);
    // Proceed to initializing the events
    InitSignalPair(mOnDestroyed, mEvents, "Destroyed");
    InitSignalPair(mOnCustom, mEvents, "Custom");
#if SQMOD_SDK_LEAST(2, 1)
    InitSignalPair(mOnStream, mEvents, "Stream");
#endif
    InitSignalPair(mOnShot, mEvents, "Shot");
    InitSignalPair(mOnTouched, mEvents, "Touched");
    InitSignalPair(mOnWorld, mEvents, "World");
    InitSignalPair(mOnAlpha, mEvents, "Alpha");
    InitSignalPair(mOnReport, mEvents, "Report");
}

// ------------------------------------------------------------------------------------------------
void ObjectInst::DropEvents()
{
    ResetSignalPair(mOnDestroyed);
    ResetSignalPair(mOnCustom);
#if SQMOD_SDK_LEAST(2, 1)
    ResetSignalPair(mOnStream);
#endif
    ResetSignalPair(mOnShot);
    ResetSignalPair(mOnTouched);
    ResetSignalPair(mOnWorld);
    ResetSignalPair(mOnAlpha);
    ResetSignalPair(mOnReport);
    mEvents.Release();
}

// ------------------------------------------------------------------------------------------------
void PickupInst::InitEvents()
{
    // Ignore the call if already initialized
    if (!mEvents.IsNull())
    {
        return;
    }
    // Create a new table on the stack
    sq_newtableex(SqVM(), 16);
    // Grab the table object from the stack
    mEvents = LightObj(-1, SqVM());
    // Pop the table object from the stack
    sq_pop(SqVM(), 1);
    // Proceed to initializing the events
    InitSignalPair(mOnDestroyed, mEvents, "Destroyed");
    InitSignalPair(mOnCustom, mEvents, "Custom");
#if SQMOD_SDK_LEAST(2, 1)
    InitSignalPair(mOnStream, mEvents, "Stream");
#endif
    InitSignalPair(mOnRespawn, mEvents, "Respawn");
    InitSignalPair(mOnClaimed, mEvents, "Claimed");
    InitSignalPair(mOnCollected, mEvents, "Collected");
    InitSignalPair(mOnWorld, mEvents, "World");
    InitSignalPair(mOnAlpha, mEvents, "Alpha");
    InitSignalPair(mOnAutomatic, mEvents, "Automatic");
    InitSignalPair(mOnAutoTimer, mEvents, "AutoTimer");
    InitSignalPair(mOnOption, mEvents, "Option");
}

// ------------------------------------------------------------------------------------------------
void PickupInst::DropEvents()
{
    ResetSignalPair(mOnDestroyed);
    ResetSignalPair(mOnCustom);
#if SQMOD_SDK_LEAST(2, 1)
    ResetSignalPair(mOnStream);
#endif
    ResetSignalPair(mOnRespawn);
    ResetSignalPair(mOnClaimed);
    ResetSignalPair(mOnCollected);
    ResetSignalPair(mOnWorld);
    ResetSignalPair(mOnAlpha);
    ResetSignalPair(mOnAutomatic);
    ResetSignalPair(mOnAutoTimer);
    ResetSignalPair(mOnOption);
    mEvents.Release();
}

// ------------------------------------------------------------------------------------------------
void PlayerInst::InitEvents()
{
    // Ignore the call if already initialized
    if (!mEvents.IsNull())
    {
        return;
    }
    // Create a new table on the stack
    sq_newtableex(SqVM(), 86);
    // Grab the table object from the stack
    mEvents = LightObj(-1, SqVM());
    // Pop the table object from the stack
    sq_pop(SqVM(), 1);
    // Proceed to initializing the events
    InitSignalPair(mOnDestroyed, mEvents, "Destroyed");
    InitSignalPair(mOnCustom, mEvents, "Custom");
#if SQMOD_SDK_LEAST(2, 1)
    InitSignalPair(mOnStream, mEvents, "Stream");
#endif
    InitSignalPair(mOnRequestClass, mEvents, "RequestClass");
    InitSignalPair(mOnRequestSpawn, mEvents, "RequestSpawn");
    InitSignalPair(mOnSpawn, mEvents, "Spawn");
    InitSignalPair(mOnWasted, mEvents, "Wasted");
    InitSignalPair(mOnKilled, mEvents, "Killed");
    InitSignalPair(mOnEmbarking, mEvents, "Embarking");
    InitSignalPair(mOnEmbarked, mEvents, "Embarked");
    InitSignalPair(mOnDisembark, mEvents, "Disembark");
    InitSignalPair(mOnRename, mEvents, "Rename");
    InitSignalPair(mOnState, mEvents, "State");
    InitSignalPair(mOnStateNone, mEvents, "StateNone");
    InitSignalPair(mOnStateNormal, mEvents, "StateNormal");
    InitSignalPair(mOnStateAim, mEvents, "StateAim");
    InitSignalPair(mOnStateDriver, mEvents, "StateDriver");
    InitSignalPair(mOnStatePassenger, mEvents, "StatePassenger");
    InitSignalPair(mOnStateEnterDriver, mEvents, "StateEnterDriver");
    InitSignalPair(mOnStateEnterPassenger, mEvents, "StateEnterPassenger");
    InitSignalPair(mOnStateExit, mEvents, "StateExit");
    InitSignalPair(mOnStateUnspawned, mEvents, "StateUnspawned");
    InitSignalPair(mOnAction, mEvents, "Action");
    InitSignalPair(mOnActionNone, mEvents, "ActionNone");
    InitSignalPair(mOnActionNormal, mEvents, "ActionNormal");
    InitSignalPair(mOnActionAiming, mEvents, "ActionAiming");
    InitSignalPair(mOnActionShooting, mEvents, "ActionShooting");
    InitSignalPair(mOnActionJumping, mEvents, "ActionJumping");
    InitSignalPair(mOnActionLieDown, mEvents, "ActionLieDown");
    InitSignalPair(mOnActionGettingUp, mEvents, "ActionGettingUp");
    InitSignalPair(mOnActionJumpVehicle, mEvents, "ActionJumpVehicle");
    InitSignalPair(mOnActionDriving, mEvents, "ActionDriving");
    InitSignalPair(mOnActionDying, mEvents, "ActionDying");
    InitSignalPair(mOnActionWasted, mEvents, "ActionWasted");
    InitSignalPair(mOnActionEmbarking, mEvents, "ActionEmbarking");
    InitSignalPair(mOnActionDisembarking, mEvents, "ActionDisembarking");
    InitSignalPair(mOnBurning, mEvents, "Burning");
    InitSignalPair(mOnCrouching, mEvents, "Crouching");
    InitSignalPair(mOnGameKeys, mEvents, "GameKeys");
    InitSignalPair(mOnStartTyping, mEvents, "StartTyping");
    InitSignalPair(mOnStopTyping, mEvents, "StopTyping");
    InitSignalPair(mOnAway, mEvents, "Away");
    InitSignalPair(mOnMessage, mEvents, "Message");
    InitSignalPair(mOnCommand, mEvents, "Command");
    InitSignalPair(mOnPrivateMessage, mEvents, "PrivateMessage");
    InitSignalPair(mOnKeyPress, mEvents, "KeyPress");
    InitSignalPair(mOnKeyRelease, mEvents, "KeyRelease");
    InitSignalPair(mOnSpectate, mEvents, "Spectate");
    InitSignalPair(mOnUnspectate, mEvents, "Unspectate");
    InitSignalPair(mOnCrashReport, mEvents, "CrashReport");
    InitSignalPair(mOnModuleList, mEvents, "ModuleList");
    InitSignalPair(mOnObjectShot, mEvents, "ObjectShot");
    InitSignalPair(mOnObjectTouched, mEvents, "ObjectTouched");
    InitSignalPair(mOnPickupClaimed, mEvents, "PickupClaimed");
    InitSignalPair(mOnPickupCollected, mEvents, "PickupCollected");
    InitSignalPair(mOnCheckpointEntered, mEvents, "CheckpointEntered");
    InitSignalPair(mOnCheckpointExited, mEvents, "CheckpointExited");
    InitSignalPair(mOnClientScriptData, mEvents, "ClientScriptData");
#if SQMOD_SDK_LEAST(2, 1)
    InitSignalPair(mOnEntityStream, mEvents, "EntityStream");
#endif
    InitSignalPair(mOnUpdate, mEvents, "Update");
    InitSignalPair(mOnHealth, mEvents, "Health");
    InitSignalPair(mOnArmour, mEvents, "Armour");
    InitSignalPair(mOnWeapon, mEvents, "Weapon");
    InitSignalPair(mOnHeading, mEvents, "Heading");
    InitSignalPair(mOnPosition, mEvents, "Position");
    InitSignalPair(mOnOption, mEvents, "Option");
    InitSignalPair(mOnAdmin, mEvents, "Admin");
    InitSignalPair(mOnWorld, mEvents, "World");
    InitSignalPair(mOnTeam, mEvents, "Team");
    InitSignalPair(mOnSkin, mEvents, "Skin");
    InitSignalPair(mOnMoney, mEvents, "Money");
    InitSignalPair(mOnScore, mEvents, "Score");
    InitSignalPair(mOnWantedLevel, mEvents, "WantedLevel");
    InitSignalPair(mOnImmunity, mEvents, "Immunity");
    InitSignalPair(mOnAlpha, mEvents, "Alpha");
    InitSignalPair(mOnEnterArea, mEvents, "EnterArea");
    InitSignalPair(mOnLeaveArea, mEvents, "LeaveArea");
}

// ------------------------------------------------------------------------------------------------
void PlayerInst::DropEvents()
{
    ResetSignalPair(mOnDestroyed);
    ResetSignalPair(mOnCustom);
#if SQMOD_SDK_LEAST(2, 1)
    ResetSignalPair(mOnStream);
#endif
    ResetSignalPair(mOnRequestClass);
    ResetSignalPair(mOnRequestSpawn);
    ResetSignalPair(mOnSpawn);
    ResetSignalPair(mOnWasted);
    ResetSignalPair(mOnKilled);
    ResetSignalPair(mOnEmbarking);
    ResetSignalPair(mOnEmbarked);
    ResetSignalPair(mOnDisembark);
    ResetSignalPair(mOnRename);
    ResetSignalPair(mOnState);
    ResetSignalPair(mOnStateNone);
    ResetSignalPair(mOnStateNormal);
    ResetSignalPair(mOnStateAim);
    ResetSignalPair(mOnStateDriver);
    ResetSignalPair(mOnStatePassenger);
    ResetSignalPair(mOnStateEnterDriver);
    ResetSignalPair(mOnStateEnterPassenger);
    ResetSignalPair(mOnStateExit);
    ResetSignalPair(mOnStateUnspawned);
    ResetSignalPair(mOnAction);
    ResetSignalPair(mOnActionNone);
    ResetSignalPair(mOnActionNormal);
    ResetSignalPair(mOnActionAiming);
    ResetSignalPair(mOnActionShooting);
    ResetSignalPair(mOnActionJumping);
    ResetSignalPair(mOnActionLieDown);
    ResetSignalPair(mOnActionGettingUp);
    ResetSignalPair(mOnActionJumpVehicle);
    ResetSignalPair(mOnActionDriving);
    ResetSignalPair(mOnActionDying);
    ResetSignalPair(mOnActionWasted);
    ResetSignalPair(mOnActionEmbarking);
    ResetSignalPair(mOnActionDisembarking);
    ResetSignalPair(mOnBurning);
    ResetSignalPair(mOnCrouching);
    ResetSignalPair(mOnGameKeys);
    ResetSignalPair(mOnStartTyping);
    ResetSignalPair(mOnStopTyping);
    ResetSignalPair(mOnAway);
    ResetSignalPair(mOnMessage);
    ResetSignalPair(mOnCommand);
    ResetSignalPair(mOnPrivateMessage);
    ResetSignalPair(mOnKeyPress);
    ResetSignalPair(mOnKeyRelease);
    ResetSignalPair(mOnSpectate);
    ResetSignalPair(mOnUnspectate);
    ResetSignalPair(mOnCrashReport);
    ResetSignalPair(mOnModuleList);
    ResetSignalPair(mOnObjectShot);
    ResetSignalPair(mOnObjectTouched);
    ResetSignalPair(mOnPickupClaimed);
    ResetSignalPair(mOnPickupCollected);
    ResetSignalPair(mOnCheckpointEntered);
    ResetSignalPair(mOnCheckpointExited);
    ResetSignalPair(mOnClientScriptData);
#if SQMOD_SDK_LEAST(2, 1)
    ResetSignalPair(mOnEntityStream);
#endif
    ResetSignalPair(mOnUpdate);
    ResetSignalPair(mOnHealth);
    ResetSignalPair(mOnArmour);
    ResetSignalPair(mOnWeapon);
    ResetSignalPair(mOnHeading);
    ResetSignalPair(mOnPosition);
    ResetSignalPair(mOnOption);
    ResetSignalPair(mOnAdmin);
    ResetSignalPair(mOnWorld);
    ResetSignalPair(mOnTeam);
    ResetSignalPair(mOnSkin);
    ResetSignalPair(mOnMoney);
    ResetSignalPair(mOnScore);
    ResetSignalPair(mOnWantedLevel);
    ResetSignalPair(mOnImmunity);
    ResetSignalPair(mOnAlpha);
    ResetSignalPair(mOnEnterArea);
    ResetSignalPair(mOnLeaveArea);
    mEvents.Release();
}

// ------------------------------------------------------------------------------------------------
void VehicleInst::InitEvents()
{
    // Ignore the call if already initialized
    if (!mEvents.IsNull())
    {
        return;
    }
    // Create a new table on the stack
    sq_newtableex(SqVM(), 32);
    // Grab the table object from the stack
    mEvents = LightObj(-1, SqVM());
    // Pop the table object from the stack
    sq_pop(SqVM(), 1);
    // Proceed to initializing the events
    InitSignalPair(mOnDestroyed, mEvents, "Destroyed");
    InitSignalPair(mOnCustom, mEvents, "Custom");
#if SQMOD_SDK_LEAST(2, 1)
    InitSignalPair(mOnStream, mEvents, "Stream");
#endif
    InitSignalPair(mOnEmbarking, mEvents, "Embarking");
    InitSignalPair(mOnEmbarked, mEvents, "Embarked");
    InitSignalPair(mOnDisembark, mEvents, "Disembark");
    InitSignalPair(mOnExplode, mEvents, "Explode");
    InitSignalPair(mOnRespawn, mEvents, "Respawn");
    InitSignalPair(mOnUpdate, mEvents, "Update");
    InitSignalPair(mOnColor, mEvents, "Color");
    InitSignalPair(mOnHealth, mEvents, "Health");
    InitSignalPair(mOnPosition, mEvents, "Position");
    InitSignalPair(mOnRotation, mEvents, "Rotation");
    InitSignalPair(mOnOption, mEvents, "Option");
    InitSignalPair(mOnWorld, mEvents, "World");
    InitSignalPair(mOnImmunity, mEvents, "Immunity");
    InitSignalPair(mOnPartStatus, mEvents, "PartStatus");
    InitSignalPair(mOnTyreStatus, mEvents, "TyreStatus");
    InitSignalPair(mOnDamageData, mEvents, "DamageData");
    InitSignalPair(mOnRadio, mEvents, "Radio");
    InitSignalPair(mOnHandlingRule, mEvents, "HandlingRule");
    InitSignalPair(mOnEnterArea, mEvents, "EnterArea");
    InitSignalPair(mOnLeaveArea, mEvents, "LeaveArea");
}

// ------------------------------------------------------------------------------------------------
void VehicleInst::DropEvents()
{
    ResetSignalPair(mOnDestroyed);
    ResetSignalPair(mOnCustom);
#if SQMOD_SDK_LEAST(2, 1)
    ResetSignalPair(mOnStream);
#endif
    ResetSignalPair(mOnEmbarking);
    ResetSignalPair(mOnEmbarked);
    ResetSignalPair(mOnDisembark);
    ResetSignalPair(mOnExplode);
    ResetSignalPair(mOnRespawn);
    ResetSignalPair(mOnUpdate);
    ResetSignalPair(mOnColor);
    ResetSignalPair(mOnHealth);
    ResetSignalPair(mOnPosition);
    ResetSignalPair(mOnRotation);
    ResetSignalPair(mOnOption);
    ResetSignalPair(mOnWorld);
    ResetSignalPair(mOnImmunity);
    ResetSignalPair(mOnPartStatus);
    ResetSignalPair(mOnTyreStatus);
    ResetSignalPair(mOnDamageData);
    ResetSignalPair(mOnRadio);
    ResetSignalPair(mOnHandlingRule);
    ResetSignalPair(mOnEnterArea);
    ResetSignalPair(mOnLeaveArea);
    mEvents.Release();
}

} // Namespace:: SqMod
