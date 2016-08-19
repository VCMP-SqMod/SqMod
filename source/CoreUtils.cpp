// ------------------------------------------------------------------------------------------------
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void Core::ClearContainer(EntityType type)
{
    switch (type)
    {
        case ENT_BLIP:
        {
            m_Blips.clear();
        } break;
        case ENT_CHECKPOINT:
        {
            m_Checkpoints.clear();
        } break;
        case ENT_KEYBIND:
        {
            m_Keybinds.clear();
        } break;
        case ENT_OBJECT:
        {
            m_Objects.clear();
        } break;
        case ENT_PICKUP:
        {
            m_Pickups.clear();
        } break;
        case ENT_PLAYER:
        {
            m_Players.clear();
        } break;
        case ENT_VEHICLE:
        {
            m_Vehicles.clear();
        } break;
        default: STHROWF("Cannot clear unknown entity type container");
    }
}

// ------------------------------------------------------------------------------------------------
void Core::ResetInst(BlipInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
    inst.mWorld = -1;
    inst.mScale = -1;
    inst.mSprID = -1;
    inst.mPosition.Clear();
    inst.mColor.Clear();
}

// ------------------------------------------------------------------------------------------------
void Core::ResetInst(CheckpointInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
}

// ------------------------------------------------------------------------------------------------
void Core::ResetInst(KeybindInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
    inst.mFirst = -1;
    inst.mSecond = -1;
    inst.mThird = -1;
    inst.mRelease = -1;
}

// ------------------------------------------------------------------------------------------------
void Core::ResetInst(ObjectInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
}

// ------------------------------------------------------------------------------------------------
void Core::ResetInst(PickupInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
}

// ------------------------------------------------------------------------------------------------
void Core::ResetInst(PlayerInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
    inst.mTrackPosition = 0;
    inst.mTrackHeading = 0;
    inst.mTrackPositionHeader = 0;
    inst.mTrackPositionPayload.Release();
    inst.mKickBanHeader = 0;
    inst.mKickBanPayload.Release();
    inst.mLastWeapon = -1;
    inst.mLastHealth = 0.0;
    inst.mLastArmour = 0.0;
    inst.mLastHeading = 0.0;
    inst.mLastPosition.Clear();
    inst.mAuthority = 0;
}

// ------------------------------------------------------------------------------------------------
void Core::ResetInst(VehicleInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
    inst.mTrackPosition = 0;
    inst.mTrackRotation = 0;
    inst.mLastPrimaryColour = -1;
    inst.mLastSecondaryColour = -1;
    inst.mLastHealth = 0.0;
    inst.mLastPosition.Clear();
    inst.mLastRotation.Clear();
}

// ------------------------------------------------------------------------------------------------
void Core::ResetFunc(BlipInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
}

// ------------------------------------------------------------------------------------------------
void Core::ResetFunc(CheckpointInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
    inst.mOnEntered.ReleaseGently();
    inst.mOnExited.ReleaseGently();
    inst.mOnWorld.ReleaseGently();
    inst.mOnRadius.ReleaseGently();
}

// ------------------------------------------------------------------------------------------------
void Core::ResetFunc(KeybindInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
    inst.mOnKeyPress.ReleaseGently();
    inst.mOnKeyRelease.ReleaseGently();
}

// ------------------------------------------------------------------------------------------------
void Core::ResetFunc(ObjectInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
    inst.mOnShot.ReleaseGently();
    inst.mOnTouched.ReleaseGently();
    inst.mOnWorld.ReleaseGently();
    inst.mOnAlpha.ReleaseGently();
    inst.mOnReport.ReleaseGently();
}

// ------------------------------------------------------------------------------------------------
void Core::ResetFunc(PickupInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
    inst.mOnRespawn.ReleaseGently();
    inst.mOnClaimed.ReleaseGently();
    inst.mOnCollected.ReleaseGently();
    inst.mOnWorld.ReleaseGently();
}

// ------------------------------------------------------------------------------------------------
void Core::ResetFunc(PlayerInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
    inst.mOnRequestClass.ReleaseGently();
    inst.mOnRequestSpawn.ReleaseGently();
    inst.mOnSpawn.ReleaseGently();
    inst.mOnWasted.ReleaseGently();
    inst.mOnKilled.ReleaseGently();
    inst.mOnEmbarking.ReleaseGently();
    inst.mOnEmbarked.ReleaseGently();
    inst.mOnDisembark.ReleaseGently();
    inst.mOnRename.ReleaseGently();
    inst.mOnState.ReleaseGently();
    inst.mOnStateNone.ReleaseGently();
    inst.mOnStateNormal.ReleaseGently();
    inst.mOnStateAim.ReleaseGently();
    inst.mOnStateDriver.ReleaseGently();
    inst.mOnStatePassenger.ReleaseGently();
    inst.mOnStateEnterDriver.ReleaseGently();
    inst.mOnStateEnterPassenger.ReleaseGently();
    inst.mOnStateExit.ReleaseGently();
    inst.mOnStateUnspawned.ReleaseGently();
    inst.mOnAction.ReleaseGently();
    inst.mOnActionNone.ReleaseGently();
    inst.mOnActionNormal.ReleaseGently();
    inst.mOnActionAiming.ReleaseGently();
    inst.mOnActionShooting.ReleaseGently();
    inst.mOnActionJumping.ReleaseGently();
    inst.mOnActionLieDown.ReleaseGently();
    inst.mOnActionGettingUp.ReleaseGently();
    inst.mOnActionJumpVehicle.ReleaseGently();
    inst.mOnActionDriving.ReleaseGently();
    inst.mOnActionDying.ReleaseGently();
    inst.mOnActionWasted.ReleaseGently();
    inst.mOnActionEmbarking.ReleaseGently();
    inst.mOnActionDisembarking.ReleaseGently();
    inst.mOnBurning.ReleaseGently();
    inst.mOnCrouching.ReleaseGently();
    inst.mOnGameKeys.ReleaseGently();
    inst.mOnStartTyping.ReleaseGently();
    inst.mOnStopTyping.ReleaseGently();
    inst.mOnAway.ReleaseGently();
    inst.mOnMessage.ReleaseGently();
    inst.mOnCommand.ReleaseGently();
    inst.mOnPrivateMessage.ReleaseGently();
    inst.mOnKeyPress.ReleaseGently();
    inst.mOnKeyRelease.ReleaseGently();
    inst.mOnSpectate.ReleaseGently();
    inst.mOnCrashreport.ReleaseGently();
    inst.mOnObjectShot.ReleaseGently();
    inst.mOnObjectTouched.ReleaseGently();
    inst.mOnPickupClaimed.ReleaseGently();
    inst.mOnPickupCollected.ReleaseGently();
    inst.mOnPickupCollected.ReleaseGently();
    inst.mOnCheckpointEntered.ReleaseGently();
    inst.mOnCheckpointExited.ReleaseGently();
    inst.mOnClientScriptData.ReleaseGently();
    inst.mOnUpdate.ReleaseGently();
    inst.mOnHealth.ReleaseGently();
    inst.mOnArmour.ReleaseGently();
    inst.mOnWeapon.ReleaseGently();
    inst.mOnHeading.ReleaseGently();
    inst.mOnPosition.ReleaseGently();
    inst.mOnOption.ReleaseGently();
    inst.mOnAdmin.ReleaseGently();
    inst.mOnWorld.ReleaseGently();
    inst.mOnTeam.ReleaseGently();
    inst.mOnSkin.ReleaseGently();
    inst.mOnMoney.ReleaseGently();
    inst.mOnScore.ReleaseGently();
    inst.mOnWantedLevel.ReleaseGently();
    inst.mOnImmunity.ReleaseGently();
    inst.mOnAlpha.ReleaseGently();
}

// ------------------------------------------------------------------------------------------------
void Core::ResetFunc(VehicleInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
    inst.mOnEmbarking.ReleaseGently();
    inst.mOnEmbarked.ReleaseGently();
    inst.mOnDisembark.ReleaseGently();
    inst.mOnExplode.ReleaseGently();
    inst.mOnRespawn.ReleaseGently();
    inst.mOnUpdate.ReleaseGently();
    inst.mOnColour.ReleaseGently();
    inst.mOnHealth.ReleaseGently();
    inst.mOnPosition.ReleaseGently();
    inst.mOnRotation.ReleaseGently();
    inst.mOnOption.ReleaseGently();
    inst.mOnWorld.ReleaseGently();
    inst.mOnImmunity.ReleaseGently();
    inst.mOnPartStatus.ReleaseGently();
    inst.mOnTyreStatus.ReleaseGently();
    inst.mOnDamageData.ReleaseGently();
    inst.mOnRadio.ReleaseGently();
    inst.mOnHandlingRule.ReleaseGently();
}

// ------------------------------------------------------------------------------------------------
void Core::ResetFunc()
{
    Core::Get().mOnCustomEvent.ReleaseGently();
    Core::Get().mOnBlipCreated.ReleaseGently();
    Core::Get().mOnCheckpointCreated.ReleaseGently();
    Core::Get().mOnKeybindCreated.ReleaseGently();
    Core::Get().mOnObjectCreated.ReleaseGently();
    Core::Get().mOnPickupCreated.ReleaseGently();
    Core::Get().mOnPlayerCreated.ReleaseGently();
    Core::Get().mOnVehicleCreated.ReleaseGently();
    Core::Get().mOnBlipDestroyed.ReleaseGently();
    Core::Get().mOnCheckpointDestroyed.ReleaseGently();
    Core::Get().mOnKeybindDestroyed.ReleaseGently();
    Core::Get().mOnObjectDestroyed.ReleaseGently();
    Core::Get().mOnPickupDestroyed.ReleaseGently();
    Core::Get().mOnPlayerDestroyed.ReleaseGently();
    Core::Get().mOnVehicleDestroyed.ReleaseGently();
    Core::Get().mOnBlipCustom.ReleaseGently();
    Core::Get().mOnCheckpointCustom.ReleaseGently();
    Core::Get().mOnKeybindCustom.ReleaseGently();
    Core::Get().mOnObjectCustom.ReleaseGently();
    Core::Get().mOnPickupCustom.ReleaseGently();
    Core::Get().mOnPlayerCustom.ReleaseGently();
    Core::Get().mOnVehicleCustom.ReleaseGently();
    Core::Get().mOnServerStartup.ReleaseGently();
    Core::Get().mOnServerShutdown.ReleaseGently();
    Core::Get().mOnServerFrame.ReleaseGently();
    Core::Get().mOnIncomingConnection.ReleaseGently();
    Core::Get().mOnPlayerRequestClass.ReleaseGently();
    Core::Get().mOnPlayerRequestSpawn.ReleaseGently();
    Core::Get().mOnPlayerSpawn.ReleaseGently();
    Core::Get().mOnPlayerWasted.ReleaseGently();
    Core::Get().mOnPlayerKilled.ReleaseGently();
    Core::Get().mOnPlayerEmbarking.ReleaseGently();
    Core::Get().mOnPlayerEmbarked.ReleaseGently();
    Core::Get().mOnPlayerDisembark.ReleaseGently();
    Core::Get().mOnPlayerRename.ReleaseGently();
    Core::Get().mOnPlayerState.ReleaseGently();
    Core::Get().mOnStateNone.ReleaseGently();
    Core::Get().mOnStateNormal.ReleaseGently();
    Core::Get().mOnStateAim.ReleaseGently();
    Core::Get().mOnStateDriver.ReleaseGently();
    Core::Get().mOnStatePassenger.ReleaseGently();
    Core::Get().mOnStateEnterDriver.ReleaseGently();
    Core::Get().mOnStateEnterPassenger.ReleaseGently();
    Core::Get().mOnStateExit.ReleaseGently();
    Core::Get().mOnStateUnspawned.ReleaseGently();
    Core::Get().mOnPlayerAction.ReleaseGently();
    Core::Get().mOnActionNone.ReleaseGently();
    Core::Get().mOnActionNormal.ReleaseGently();
    Core::Get().mOnActionAiming.ReleaseGently();
    Core::Get().mOnActionShooting.ReleaseGently();
    Core::Get().mOnActionJumping.ReleaseGently();
    Core::Get().mOnActionLieDown.ReleaseGently();
    Core::Get().mOnActionGettingUp.ReleaseGently();
    Core::Get().mOnActionJumpVehicle.ReleaseGently();
    Core::Get().mOnActionDriving.ReleaseGently();
    Core::Get().mOnActionDying.ReleaseGently();
    Core::Get().mOnActionWasted.ReleaseGently();
    Core::Get().mOnActionEmbarking.ReleaseGently();
    Core::Get().mOnActionDisembarking.ReleaseGently();
    Core::Get().mOnPlayerBurning.ReleaseGently();
    Core::Get().mOnPlayerCrouching.ReleaseGently();
    Core::Get().mOnPlayerGameKeys.ReleaseGently();
    Core::Get().mOnPlayerStartTyping.ReleaseGently();
    Core::Get().mOnPlayerStopTyping.ReleaseGently();
    Core::Get().mOnPlayerAway.ReleaseGently();
    Core::Get().mOnPlayerMessage.ReleaseGently();
    Core::Get().mOnPlayerCommand.ReleaseGently();
    Core::Get().mOnPlayerPrivateMessage.ReleaseGently();
    Core::Get().mOnPlayerKeyPress.ReleaseGently();
    Core::Get().mOnPlayerKeyRelease.ReleaseGently();
    Core::Get().mOnPlayerSpectate.ReleaseGently();
    Core::Get().mOnPlayerCrashreport.ReleaseGently();
    Core::Get().mOnVehicleExplode.ReleaseGently();
    Core::Get().mOnVehicleRespawn.ReleaseGently();
    Core::Get().mOnObjectShot.ReleaseGently();
    Core::Get().mOnObjectTouched.ReleaseGently();
    Core::Get().mOnObjectWorld.ReleaseGently();
    Core::Get().mOnObjectAlpha.ReleaseGently();
    Core::Get().mOnObjectReport.ReleaseGently();
    Core::Get().mOnPickupClaimed.ReleaseGently();
    Core::Get().mOnPickupCollected.ReleaseGently();
    Core::Get().mOnPickupWorld.ReleaseGently();
    Core::Get().mOnPickupRespawn.ReleaseGently();
    Core::Get().mOnCheckpointEntered.ReleaseGently();
    Core::Get().mOnCheckpointExited.ReleaseGently();
    Core::Get().mOnCheckpointWorld.ReleaseGently();
    Core::Get().mOnCheckpointRadius.ReleaseGently();
    Core::Get().mOnEntityPool.ReleaseGently();
    Core::Get().mOnClientScriptData.ReleaseGently();
    Core::Get().mOnPlayerUpdate.ReleaseGently();
    Core::Get().mOnVehicleUpdate.ReleaseGently();
    Core::Get().mOnPlayerHealth.ReleaseGently();
    Core::Get().mOnPlayerArmour.ReleaseGently();
    Core::Get().mOnPlayerWeapon.ReleaseGently();
    Core::Get().mOnPlayerHeading.ReleaseGently();
    Core::Get().mOnPlayerPosition.ReleaseGently();
    Core::Get().mOnPlayerOption.ReleaseGently();
    Core::Get().mOnPlayerAdmin.ReleaseGently();
    Core::Get().mOnPlayerWorld.ReleaseGently();
    Core::Get().mOnPlayerTeam.ReleaseGently();
    Core::Get().mOnPlayerSkin.ReleaseGently();
    Core::Get().mOnPlayerMoney.ReleaseGently();
    Core::Get().mOnPlayerScore.ReleaseGently();
    Core::Get().mOnPlayerWantedLevel.ReleaseGently();
    Core::Get().mOnPlayerImmunity.ReleaseGently();
    Core::Get().mOnPlayerAlpha.ReleaseGently();
    Core::Get().mOnVehicleColour.ReleaseGently();
    Core::Get().mOnVehicleHealth.ReleaseGently();
    Core::Get().mOnVehiclePosition.ReleaseGently();
    Core::Get().mOnVehicleRotation.ReleaseGently();
    Core::Get().mOnVehicleOption.ReleaseGently();
    Core::Get().mOnVehicleWorld.ReleaseGently();
    Core::Get().mOnVehicleImmunity.ReleaseGently();
    Core::Get().mOnVehiclePartStatus.ReleaseGently();
    Core::Get().mOnVehicleTyreStatus.ReleaseGently();
    Core::Get().mOnVehicleDamageData.ReleaseGently();
    Core::Get().mOnVehicleRadio.ReleaseGently();
    Core::Get().mOnVehicleHandlingRule.ReleaseGently();
    Core::Get().mOnServerOption.ReleaseGently();
    Core::Get().mOnScriptReload.ReleaseGently();
    Core::Get().mOnScriptLoaded.ReleaseGently();
}

// ------------------------------------------------------------------------------------------------
Function & Core::GetEvent(Int32 evid)
{
    switch (evid)
    {
        case EVT_CUSTOMEVENT:           return mOnCustomEvent;
        case EVT_BLIPCREATED:           return mOnBlipCreated;
        case EVT_CHECKPOINTCREATED:     return mOnCheckpointCreated;
        case EVT_KEYBINDCREATED:        return mOnKeybindCreated;
        case EVT_OBJECTCREATED:         return mOnObjectCreated;
        case EVT_PICKUPCREATED:         return mOnPickupCreated;
        case EVT_PLAYERCREATED:         return mOnPlayerCreated;
        case EVT_VEHICLECREATED:        return mOnVehicleCreated;
        case EVT_BLIPDESTROYED:         return mOnBlipDestroyed;
        case EVT_CHECKPOINTDESTROYED:   return mOnCheckpointDestroyed;
        case EVT_KEYBINDDESTROYED:      return mOnKeybindDestroyed;
        case EVT_OBJECTDESTROYED:       return mOnObjectDestroyed;
        case EVT_PICKUPDESTROYED:       return mOnPickupDestroyed;
        case EVT_PLAYERDESTROYED:       return mOnPlayerDestroyed;
        case EVT_VEHICLEDESTROYED:      return mOnVehicleDestroyed;
        case EVT_BLIPCUSTOM:            return mOnBlipCustom;
        case EVT_CHECKPOINTCUSTOM:      return mOnCheckpointCustom;
        case EVT_KEYBINDCUSTOM:         return mOnKeybindCustom;
        case EVT_OBJECTCUSTOM:          return mOnObjectCustom;
        case EVT_PICKUPCUSTOM:          return mOnPickupCustom;
        case EVT_PLAYERCUSTOM:          return mOnPlayerCustom;
        case EVT_VEHICLECUSTOM:         return mOnVehicleCustom;
        case EVT_SERVERSTARTUP:         return mOnServerStartup;
        case EVT_SERVERSHUTDOWN:        return mOnServerShutdown;
        case EVT_SERVERFRAME:           return mOnServerFrame;
        case EVT_INCOMINGCONNECTION:    return mOnIncomingConnection;
        case EVT_PLAYERREQUESTCLASS:    return mOnPlayerRequestClass;
        case EVT_PLAYERREQUESTSPAWN:    return mOnPlayerRequestSpawn;
        case EVT_PLAYERSPAWN:           return mOnPlayerSpawn;
        case EVT_PLAYERWASTED:          return mOnPlayerWasted;
        case EVT_PLAYERKILLED:          return mOnPlayerKilled;
        case EVT_PLAYEREMBARKING:       return mOnPlayerEmbarking;
        case EVT_PLAYEREMBARKED:        return mOnPlayerEmbarked;
        case EVT_PLAYERDISEMBARK:       return mOnPlayerDisembark;
        case EVT_PLAYERRENAME:          return mOnPlayerRename;
        case EVT_PLAYERSTATE:           return mOnPlayerState;
        case EVT_STATENONE:             return mOnStateNone;
        case EVT_STATENORMAL:           return mOnStateNormal;
        case EVT_STATEAIM:              return mOnStateAim;
        case EVT_STATEDRIVER:           return mOnStateDriver;
        case EVT_STATEPASSENGER:        return mOnStatePassenger;
        case EVT_STATEENTERDRIVER:      return mOnStateEnterDriver;
        case EVT_STATEENTERPASSENGER:   return mOnStateEnterPassenger;
        case EVT_STATEEXIT:             return mOnStateExit;
        case EVT_STATEUNSPAWNED:        return mOnStateUnspawned;
        case EVT_PLAYERACTION:          return mOnPlayerAction;
        case EVT_ACTIONNONE:            return mOnActionNone;
        case EVT_ACTIONNORMAL:          return mOnActionNormal;
        case EVT_ACTIONAIMING:          return mOnActionAiming;
        case EVT_ACTIONSHOOTING:        return mOnActionShooting;
        case EVT_ACTIONJUMPING:         return mOnActionJumping;
        case EVT_ACTIONLIEDOWN:         return mOnActionLieDown;
        case EVT_ACTIONGETTINGUP:       return mOnActionGettingUp;
        case EVT_ACTIONJUMPVEHICLE:     return mOnActionJumpVehicle;
        case EVT_ACTIONDRIVING:         return mOnActionDriving;
        case EVT_ACTIONDYING:           return mOnActionDying;
        case EVT_ACTIONWASTED:          return mOnActionWasted;
        case EVT_ACTIONEMBARKING:       return mOnActionEmbarking;
        case EVT_ACTIONDISEMBARKING:    return mOnActionDisembarking;
        case EVT_PLAYERBURNING:         return mOnPlayerBurning;
        case EVT_PLAYERCROUCHING:       return mOnPlayerCrouching;
        case EVT_PLAYERGAMEKEYS:        return mOnPlayerGameKeys;
        case EVT_PLAYERSTARTTYPING:     return mOnPlayerStartTyping;
        case EVT_PLAYERSTOPTYPING:      return mOnPlayerStopTyping;
        case EVT_PLAYERAWAY:            return mOnPlayerAway;
        case EVT_PLAYERMESSAGE:         return mOnPlayerMessage;
        case EVT_PLAYERCOMMAND:         return mOnPlayerCommand;
        case EVT_PLAYERPRIVATEMESSAGE:  return mOnPlayerPrivateMessage;
        case EVT_PLAYERKEYPRESS:        return mOnPlayerKeyPress;
        case EVT_PLAYERKEYRELEASE:      return mOnPlayerKeyRelease;
        case EVT_PLAYERSPECTATE:        return mOnPlayerSpectate;
        case EVT_PLAYERCRASHREPORT:     return mOnPlayerCrashreport;
        case EVT_VEHICLEEXPLODE:        return mOnVehicleExplode;
        case EVT_VEHICLERESPAWN:        return mOnVehicleRespawn;
        case EVT_OBJECTSHOT:            return mOnObjectShot;
        case EVT_OBJECTTOUCHED:         return mOnObjectTouched;
        case EVT_OBJECTWORLD:           return mOnObjectWorld;
        case EVT_OBJECTALPHA:           return mOnObjectAlpha;
        case EVT_OBJECTREPORT:          return mOnObjectReport;
        case EVT_PICKUPCLAIMED:         return mOnPickupClaimed;
        case EVT_PICKUPCOLLECTED:       return mOnPickupCollected;
        case EVT_PICKUPWORLD:           return mOnPickupWorld;
        case EVT_PICKUPRESPAWN:         return mOnPickupRespawn;
        case EVT_CHECKPOINTENTERED:     return mOnCheckpointEntered;
        case EVT_CHECKPOINTEXITED:      return mOnCheckpointExited;
        case EVT_CHECKPOINTWORLD:       return mOnCheckpointWorld;
        case EVT_CHECKPOINTRADIUS:      return mOnCheckpointRadius;
        case EVT_ENTITYPOOL:            return mOnEntityPool;
        case EVT_CLIENTSCRIPTDATA:      return mOnClientScriptData;
        case EVT_PLAYERUPDATE:          return mOnPlayerUpdate;
        case EVT_VEHICLEUPDATE:         return mOnVehicleUpdate;
        case EVT_PLAYERHEALTH:          return mOnPlayerHealth;
        case EVT_PLAYERARMOUR:          return mOnPlayerArmour;
        case EVT_PLAYERWEAPON:          return mOnPlayerWeapon;
        case EVT_PLAYERHEADING:         return mOnPlayerHeading;
        case EVT_PLAYERPOSITION:        return mOnPlayerPosition;
        case EVT_PLAYEROPTION:          return mOnPlayerOption;
        case EVT_PLAYERADMIN:           return mOnPlayerAdmin;
        case EVT_PLAYERWORLD:           return mOnPlayerWorld;
        case EVT_PLAYERTEAM:            return mOnPlayerTeam;
        case EVT_PLAYERSKIN:            return mOnPlayerSkin;
        case EVT_PLAYERMONEY:           return mOnPlayerMoney;
        case EVT_PLAYERSCORE:           return mOnPlayerScore;
        case EVT_PLAYERWANTEDLEVEL:     return mOnPlayerWantedLevel;
        case EVT_PLAYERIMMUNITY:        return mOnPlayerImmunity;
        case EVT_PLAYERALPHA:           return mOnPlayerAlpha;
        case EVT_VEHICLECOLOUR:         return mOnVehicleColour;
        case EVT_VEHICLEHEALTH:         return mOnVehicleHealth;
        case EVT_VEHICLEPOSITION:       return mOnVehiclePosition;
        case EVT_VEHICLEROTATION:       return mOnVehicleRotation;
        case EVT_VEHICLEOPTION:         return mOnVehicleOption;
        case EVT_VEHICLEWORLD:          return mOnVehicleWorld;
        case EVT_VEHICLEIMMUNITY:       return mOnVehicleImmunity;
        case EVT_VEHICLEPARTSTATUS:     return mOnVehiclePartStatus;
        case EVT_VEHICLETYRESTATUS:     return mOnVehicleTyreStatus;
        case EVT_VEHICLEDAMAGEDATA:     return mOnVehicleDamageData;
        case EVT_VEHICLERADIO:          return mOnVehicleRadio;
        case EVT_VEHICLEHANDLINGRULE:   return mOnVehicleHandlingRule;
        case EVT_SERVEROPTION:          return mOnServerOption;
        case EVT_SCRIPTRELOAD:          return mOnScriptReload;
        case EVT_SCRIPTLOADED:          return mOnScriptLoaded;
        default:                        return NullFunction();
    }
}

// ------------------------------------------------------------------------------------------------
Function & Core::GetBlipEvent(Int32 id, Int32 evid)
{
    BlipInst & inst = m_Blips.at(id);

    switch (evid)
    {
        case EVT_BLIPDESTROYED:         return inst.mOnDestroyed;
        case EVT_BLIPCUSTOM:            return inst.mOnCustom;
        default:                        return NullFunction();
    }
}

// ------------------------------------------------------------------------------------------------
Function & Core::GetCheckpointEvent(Int32 id, Int32 evid)
{
    CheckpointInst & inst = m_Checkpoints.at(id);

    switch (evid)
    {
        case EVT_CHECKPOINTDESTROYED:   return inst.mOnDestroyed;
        case EVT_CHECKPOINTCUSTOM:      return inst.mOnCustom;
        case EVT_CHECKPOINTENTERED:     return inst.mOnEntered;
        case EVT_CHECKPOINTEXITED:      return inst.mOnExited;
        case EVT_CHECKPOINTWORLD:       return inst.mOnWorld;
        case EVT_CHECKPOINTRADIUS:      return inst.mOnRadius;
        default:                        return NullFunction();
    }
}


// ------------------------------------------------------------------------------------------------
Function & Core::GetKeybindEvent(Int32 id, Int32 evid)
{
    KeybindInst & inst = m_Keybinds.at(id);

    switch (evid)
    {
        case EVT_KEYBINDDESTROYED:      return inst.mOnDestroyed;
        case EVT_KEYBINDCUSTOM:         return inst.mOnCustom;
        case EVT_PLAYERKEYPRESS:        return inst.mOnKeyPress;
        case EVT_PLAYERKEYRELEASE:      return inst.mOnKeyRelease;
        default:                        return NullFunction();
    }
}

// ------------------------------------------------------------------------------------------------
Function & Core::GetObjectEvent(Int32 id, Int32 evid)
{
    ObjectInst & inst = m_Objects.at(id);

    switch (evid)
    {
        case EVT_OBJECTDESTROYED:       return inst.mOnDestroyed;
        case EVT_OBJECTCUSTOM:          return inst.mOnCustom;
        case EVT_OBJECTSHOT:            return inst.mOnShot;
        case EVT_OBJECTTOUCHED:         return inst.mOnTouched;
        case EVT_OBJECTWORLD:           return inst.mOnWorld;
        case EVT_OBJECTALPHA:           return inst.mOnAlpha;
        case EVT_OBJECTREPORT:          return inst.mOnReport;
        default:                        return NullFunction();
    }
}

// ------------------------------------------------------------------------------------------------
Function & Core::GetPickupEvent(Int32 id, Int32 evid)
{
    PickupInst & inst = m_Pickups.at(id);

    switch (evid)
    {
        case EVT_PICKUPDESTROYED:       return inst.mOnDestroyed;
        case EVT_PICKUPCUSTOM:          return inst.mOnCustom;
        case EVT_PICKUPRESPAWN:         return inst.mOnRespawn;
        case EVT_PICKUPCLAIMED:         return inst.mOnClaimed;
        case EVT_PICKUPCOLLECTED:       return inst.mOnCollected;
        case EVT_PICKUPWORLD:           return inst.mOnWorld;
        default:                        return NullFunction();
    }
}

// ------------------------------------------------------------------------------------------------
Function & Core::GetPlayerEvent(Int32 id, Int32 evid)
{
    PlayerInst & inst = m_Players.at(id);

    switch (evid)
    {
        case EVT_PLAYERDESTROYED:       return inst.mOnDestroyed;
        case EVT_PLAYERCUSTOM:          return inst.mOnCustom;
        case EVT_PLAYERREQUESTCLASS:    return inst.mOnRequestClass;
        case EVT_PLAYERREQUESTSPAWN:    return inst.mOnRequestSpawn;
        case EVT_PLAYERSPAWN:           return inst.mOnSpawn;
        case EVT_PLAYERWASTED:          return inst.mOnWasted;
        case EVT_PLAYERKILLED:          return inst.mOnKilled;
        case EVT_PLAYEREMBARKING:       return inst.mOnEmbarking;
        case EVT_PLAYEREMBARKED:        return inst.mOnEmbarked;
        case EVT_PLAYERDISEMBARK:       return inst.mOnDisembark;
        case EVT_PLAYERRENAME:          return inst.mOnRename;
        case EVT_PLAYERSTATE:           return inst.mOnState;
        case EVT_STATENONE:             return inst.mOnStateNone;
        case EVT_STATENORMAL:           return inst.mOnStateNormal;
        case EVT_STATEAIM:              return inst.mOnStateAim;
        case EVT_STATEDRIVER:           return inst.mOnStateDriver;
        case EVT_STATEPASSENGER:        return inst.mOnStatePassenger;
        case EVT_STATEENTERDRIVER:      return inst.mOnStateEnterDriver;
        case EVT_STATEENTERPASSENGER:   return inst.mOnStateEnterPassenger;
        case EVT_STATEEXIT:             return inst.mOnStateExit;
        case EVT_STATEUNSPAWNED:        return inst.mOnStateUnspawned;
        case EVT_PLAYERACTION:          return inst.mOnAction;
        case EVT_ACTIONNONE:            return inst.mOnActionNone;
        case EVT_ACTIONNORMAL:          return inst.mOnActionNormal;
        case EVT_ACTIONAIMING:          return inst.mOnActionAiming;
        case EVT_ACTIONSHOOTING:        return inst.mOnActionShooting;
        case EVT_ACTIONJUMPING:         return inst.mOnActionJumping;
        case EVT_ACTIONLIEDOWN:         return inst.mOnActionLieDown;
        case EVT_ACTIONGETTINGUP:       return inst.mOnActionGettingUp;
        case EVT_ACTIONJUMPVEHICLE:     return inst.mOnActionJumpVehicle;
        case EVT_ACTIONDRIVING:         return inst.mOnActionDriving;
        case EVT_ACTIONDYING:           return inst.mOnActionDying;
        case EVT_ACTIONWASTED:          return inst.mOnActionWasted;
        case EVT_ACTIONEMBARKING:       return inst.mOnActionEmbarking;
        case EVT_ACTIONDISEMBARKING:    return inst.mOnActionDisembarking;
        case EVT_PLAYERBURNING:         return inst.mOnBurning;
        case EVT_PLAYERCROUCHING:       return inst.mOnCrouching;
        case EVT_PLAYERGAMEKEYS:        return inst.mOnGameKeys;
        case EVT_PLAYERSTARTTYPING:     return inst.mOnStartTyping;
        case EVT_PLAYERSTOPTYPING:      return inst.mOnStopTyping;
        case EVT_PLAYERAWAY:            return inst.mOnAway;
        case EVT_PLAYERMESSAGE:         return inst.mOnMessage;
        case EVT_PLAYERCOMMAND:         return inst.mOnCommand;
        case EVT_PLAYERPRIVATEMESSAGE:  return inst.mOnPrivateMessage;
        case EVT_PLAYERKEYPRESS:        return inst.mOnKeyPress;
        case EVT_PLAYERKEYRELEASE:      return inst.mOnKeyRelease;
        case EVT_PLAYERSPECTATE:        return inst.mOnSpectate;
        case EVT_PLAYERCRASHREPORT:     return inst.mOnCrashreport;
        case EVT_OBJECTSHOT:            return inst.mOnObjectShot;
        case EVT_OBJECTTOUCHED:         return inst.mOnObjectTouched;
        case EVT_PICKUPCLAIMED:         return inst.mOnPickupClaimed;
        case EVT_PICKUPCOLLECTED:       return inst.mOnPickupCollected;
        case EVT_CHECKPOINTENTERED:     return inst.mOnCheckpointEntered;
        case EVT_CHECKPOINTEXITED:      return inst.mOnCheckpointExited;
        case EVT_CLIENTSCRIPTDATA:      return inst.mOnClientScriptData;
        case EVT_PLAYERUPDATE:          return inst.mOnUpdate;
        case EVT_PLAYERHEALTH:          return inst.mOnHealth;
        case EVT_PLAYERARMOUR:          return inst.mOnArmour;
        case EVT_PLAYERWEAPON:          return inst.mOnWeapon;
        case EVT_PLAYERHEADING:         return inst.mOnHeading;
        case EVT_PLAYERPOSITION:        return inst.mOnPosition;
        case EVT_PLAYEROPTION:          return inst.mOnOption;
        case EVT_PLAYERADMIN:           return inst.mOnAdmin;
        case EVT_PLAYERWORLD:           return inst.mOnWorld;
        case EVT_PLAYERTEAM:            return inst.mOnTeam;
        case EVT_PLAYERSKIN:            return inst.mOnSkin;
        case EVT_PLAYERMONEY:           return inst.mOnMoney;
        case EVT_PLAYERSCORE:           return inst.mOnScore;
        case EVT_PLAYERWANTEDLEVEL:     return inst.mOnWantedLevel;
        case EVT_PLAYERIMMUNITY:        return inst.mOnImmunity;
        case EVT_PLAYERALPHA:           return inst.mOnAlpha;
        default:                        return NullFunction();
    }
}

// ------------------------------------------------------------------------------------------------
Function & Core::GetVehicleEvent(Int32 id, Int32 evid)
{
    VehicleInst & inst = m_Vehicles.at(id);

    switch (evid)
    {
        case EVT_PLAYEREMBARKING:       return inst.mOnEmbarking;
        case EVT_PLAYEREMBARKED:        return inst.mOnEmbarked;
        case EVT_PLAYERDISEMBARK:       return inst.mOnDisembark;
        case EVT_VEHICLEEXPLODE:        return inst.mOnExplode;
        case EVT_VEHICLERESPAWN:        return inst.mOnRespawn;
        case EVT_VEHICLEUPDATE:         return inst.mOnUpdate;
        case EVT_VEHICLECOLOUR:         return inst.mOnColour;
        case EVT_VEHICLEHEALTH:         return inst.mOnHealth;
        case EVT_VEHICLEPOSITION:       return inst.mOnPosition;
        case EVT_VEHICLEROTATION:       return inst.mOnRotation;
        case EVT_VEHICLEOPTION:         return inst.mOnOption;
        case EVT_VEHICLEWORLD:          return inst.mOnWorld;
        case EVT_VEHICLEIMMUNITY:       return inst.mOnImmunity;
        case EVT_VEHICLEPARTSTATUS:     return inst.mOnPartStatus;
        case EVT_VEHICLETYRESTATUS:     return inst.mOnTyreStatus;
        case EVT_VEHICLEDAMAGEDATA:     return inst.mOnDamageData;
        case EVT_VEHICLERADIO:          return inst.mOnRadio;
        case EVT_VEHICLEHANDLINGRULE:   return inst.mOnHandlingRule;
        default:                        return NullFunction();
    }
}

} // Namespace:: SqMod
