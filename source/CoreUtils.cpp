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
void Core::InitEvents()
{
    // Ignore the call if already initialized
    if (!m_Events.IsNull())
    {
        return;
    }
    // Create a new table on the stack
    sq_newtableex(DefaultVM::Get(), 128);
    // Grab the table object from the stack
    m_Events = LightObj(-1, DefaultVM::Get());
    // Pop the table object from the stack
    sq_pop(DefaultVM::Get(), 1);
    // Proceed to initializing the events
    InitSignalPair(mOnCustomEvent, m_Events, "CustomEvent");
    InitSignalPair(mOnBlipCreated, m_Events, "BlipCreated");
    InitSignalPair(mOnCheckpointCreated, m_Events, "CheckpointCreated");
    InitSignalPair(mOnKeybindCreated, m_Events, "KeybindCreated");
    InitSignalPair(mOnObjectCreated, m_Events, "ObjectCreated");
    InitSignalPair(mOnPickupCreated, m_Events, "PickupCreated");
    InitSignalPair(mOnPlayerCreated, m_Events, "PlayerCreated");
    InitSignalPair(mOnVehicleCreated, m_Events, "VehicleCreated");
    InitSignalPair(mOnBlipDestroyed, m_Events, "BlipDestroyed");
    InitSignalPair(mOnCheckpointDestroyed, m_Events, "CheckpointDestroyed");
    InitSignalPair(mOnKeybindDestroyed, m_Events, "KeybindDestroyed");
    InitSignalPair(mOnObjectDestroyed, m_Events, "ObjectDestroyed");
    InitSignalPair(mOnPickupDestroyed, m_Events, "PickupDestroyed");
    InitSignalPair(mOnPlayerDestroyed, m_Events, "PlayerDestroyed");
    InitSignalPair(mOnVehicleDestroyed, m_Events, "VehicleDestroyed");
    InitSignalPair(mOnBlipCustom, m_Events, "BlipCustom");
    InitSignalPair(mOnCheckpointCustom, m_Events, "CheckpointCustom");
    InitSignalPair(mOnKeybindCustom, m_Events, "KeybindCustom");
    InitSignalPair(mOnObjectCustom, m_Events, "ObjectCustom");
    InitSignalPair(mOnPickupCustom, m_Events, "PickupCustom");
    InitSignalPair(mOnPlayerCustom, m_Events, "PlayerCustom");
    InitSignalPair(mOnVehicleCustom, m_Events, "VehicleCustom");
    InitSignalPair(mOnServerStartup, m_Events, "ServerStartup");
    InitSignalPair(mOnServerShutdown, m_Events, "ServerShutdown");
    InitSignalPair(mOnServerFrame, m_Events, "ServerFrame");
    InitSignalPair(mOnIncomingConnection, m_Events, "IncomingConnection");
    InitSignalPair(mOnPlayerRequestClass, m_Events, "PlayerRequestClass");
    InitSignalPair(mOnPlayerRequestSpawn, m_Events, "PlayerRequestSpawn");
    InitSignalPair(mOnPlayerSpawn, m_Events, "PlayerSpawn");
    InitSignalPair(mOnPlayerWasted, m_Events, "PlayerWasted");
    InitSignalPair(mOnPlayerKilled, m_Events, "PlayerKilled");
    InitSignalPair(mOnPlayerEmbarking, m_Events, "PlayerEmbarking");
    InitSignalPair(mOnPlayerEmbarked, m_Events, "PlayerEmbarked");
    InitSignalPair(mOnPlayerDisembark, m_Events, "PlayerDisembark");
    InitSignalPair(mOnPlayerRename, m_Events, "PlayerRename");
    InitSignalPair(mOnPlayerState, m_Events, "PlayerState");
    InitSignalPair(mOnStateNone, m_Events, "StateNone");
    InitSignalPair(mOnStateNormal, m_Events, "StateNormal");
    InitSignalPair(mOnStateAim, m_Events, "StateAim");
    InitSignalPair(mOnStateDriver, m_Events, "StateDriver");
    InitSignalPair(mOnStatePassenger, m_Events, "StatePassenger");
    InitSignalPair(mOnStateEnterDriver, m_Events, "StateEnterDriver");
    InitSignalPair(mOnStateEnterPassenger, m_Events, "StateEnterPassenger");
    InitSignalPair(mOnStateExit, m_Events, "StateExit");
    InitSignalPair(mOnStateUnspawned, m_Events, "StateUnspawned");
    InitSignalPair(mOnPlayerAction, m_Events, "PlayerAction");
    InitSignalPair(mOnActionNone, m_Events, "ActionNone");
    InitSignalPair(mOnActionNormal, m_Events, "ActionNormal");
    InitSignalPair(mOnActionAiming, m_Events, "ActionAiming");
    InitSignalPair(mOnActionShooting, m_Events, "ActionShooting");
    InitSignalPair(mOnActionJumping, m_Events, "ActionJumping");
    InitSignalPair(mOnActionLieDown, m_Events, "ActionLieDown");
    InitSignalPair(mOnActionGettingUp, m_Events, "ActionGettingUp");
    InitSignalPair(mOnActionJumpVehicle, m_Events, "ActionJumpVehicle");
    InitSignalPair(mOnActionDriving, m_Events, "ActionDriving");
    InitSignalPair(mOnActionDying, m_Events, "ActionDying");
    InitSignalPair(mOnActionWasted, m_Events, "ActionWasted");
    InitSignalPair(mOnActionEmbarking, m_Events, "ActionEmbarking");
    InitSignalPair(mOnActionDisembarking, m_Events, "ActionDisembarking");
    InitSignalPair(mOnPlayerBurning, m_Events, "PlayerBurning");
    InitSignalPair(mOnPlayerCrouching, m_Events, "PlayerCrouching");
    InitSignalPair(mOnPlayerGameKeys, m_Events, "PlayerGameKeys");
    InitSignalPair(mOnPlayerStartTyping, m_Events, "PlayerStartTyping");
    InitSignalPair(mOnPlayerStopTyping, m_Events, "PlayerStopTyping");
    InitSignalPair(mOnPlayerAway, m_Events, "PlayerAway");
    InitSignalPair(mOnPlayerMessage, m_Events, "PlayerMessage");
    InitSignalPair(mOnPlayerCommand, m_Events, "PlayerCommand");
    InitSignalPair(mOnPlayerPrivateMessage, m_Events, "PlayerPrivateMessage");
    InitSignalPair(mOnPlayerKeyPress, m_Events, "PlayerKeyPress");
    InitSignalPair(mOnPlayerKeyRelease, m_Events, "PlayerKeyRelease");
    InitSignalPair(mOnPlayerSpectate, m_Events, "PlayerSpectate");
    InitSignalPair(mOnPlayerCrashreport, m_Events, "PlayerCrashreport");
    InitSignalPair(mOnVehicleExplode, m_Events, "VehicleExplode");
    InitSignalPair(mOnVehicleRespawn, m_Events, "VehicleRespawn");
    InitSignalPair(mOnObjectShot, m_Events, "ObjectShot");
    InitSignalPair(mOnObjectTouched, m_Events, "ObjectTouched");
    InitSignalPair(mOnObjectWorld, m_Events, "ObjectWorld");
    InitSignalPair(mOnObjectAlpha, m_Events, "ObjectAlpha");
    InitSignalPair(mOnObjectReport, m_Events, "ObjectReport");
    InitSignalPair(mOnPickupClaimed, m_Events, "PickupClaimed");
    InitSignalPair(mOnPickupCollected, m_Events, "PickupCollected");
    InitSignalPair(mOnPickupRespawn, m_Events, "PickupRespawn");
    InitSignalPair(mOnPickupWorld, m_Events, "PickupWorld");
    InitSignalPair(mOnPickupAlpha, m_Events, "PickupAlpha");
    InitSignalPair(mOnPickupAutomatic, m_Events, "PickupAutomatic");
    InitSignalPair(mOnPickupAutoTimer, m_Events, "PickupAutoTimer");
    InitSignalPair(mOnCheckpointEntered, m_Events, "CheckpointEntered");
    InitSignalPair(mOnCheckpointExited, m_Events, "CheckpointExited");
    InitSignalPair(mOnCheckpointWorld, m_Events, "CheckpointWorld");
    InitSignalPair(mOnCheckpointRadius, m_Events, "CheckpointRadius");
    InitSignalPair(mOnEntityPool, m_Events, "EntityPool");
    InitSignalPair(mOnClientScriptData, m_Events, "ClientScriptData");
    InitSignalPair(mOnPlayerUpdate, m_Events, "PlayerUpdate");
    InitSignalPair(mOnVehicleUpdate, m_Events, "VehicleUpdate");
    InitSignalPair(mOnPlayerHealth, m_Events, "PlayerHealth");
    InitSignalPair(mOnPlayerArmour, m_Events, "PlayerArmour");
    InitSignalPair(mOnPlayerWeapon, m_Events, "PlayerWeapon");
    InitSignalPair(mOnPlayerHeading, m_Events, "PlayerHeading");
    InitSignalPair(mOnPlayerPosition, m_Events, "PlayerPosition");
    InitSignalPair(mOnPlayerOption, m_Events, "PlayerOption");
    InitSignalPair(mOnPlayerAdmin, m_Events, "PlayerAdmin");
    InitSignalPair(mOnPlayerWorld, m_Events, "PlayerWorld");
    InitSignalPair(mOnPlayerTeam, m_Events, "PlayerTeam");
    InitSignalPair(mOnPlayerSkin, m_Events, "PlayerSkin");
    InitSignalPair(mOnPlayerMoney, m_Events, "PlayerMoney");
    InitSignalPair(mOnPlayerScore, m_Events, "PlayerScore");
    InitSignalPair(mOnPlayerWantedLevel, m_Events, "PlayerWantedLevel");
    InitSignalPair(mOnPlayerImmunity, m_Events, "PlayerImmunity");
    InitSignalPair(mOnPlayerAlpha, m_Events, "PlayerAlpha");
    InitSignalPair(mOnVehicleColor, m_Events, "VehicleColor");
    InitSignalPair(mOnVehicleHealth, m_Events, "VehicleHealth");
    InitSignalPair(mOnVehiclePosition, m_Events, "VehiclePosition");
    InitSignalPair(mOnVehicleRotation, m_Events, "VehicleRotation");
    InitSignalPair(mOnVehicleOption, m_Events, "VehicleOption");
    InitSignalPair(mOnVehicleWorld, m_Events, "VehicleWorld");
    InitSignalPair(mOnVehicleImmunity, m_Events, "VehicleImmunity");
    InitSignalPair(mOnVehiclePartStatus, m_Events, "VehiclePartStatus");
    InitSignalPair(mOnVehicleTyreStatus, m_Events, "VehicleTyreStatus");
    InitSignalPair(mOnVehicleDamageData, m_Events, "VehicleDamageData");
    InitSignalPair(mOnVehicleRadio, m_Events, "VehicleRadio");
    InitSignalPair(mOnVehicleHandlingRule, m_Events, "VehicleHandlingRule");
    InitSignalPair(mOnServerOption, m_Events, "ServerOption");
    InitSignalPair(mOnScriptReload, m_Events, "ScriptReload");
    InitSignalPair(mOnScriptLoaded, m_Events, "ScriptLoaded");
}
// ------------------------------------------------------------------------------------------------
void Core::DropEvents()
{
    ResetSignalPair(mOnCustomEvent);
    ResetSignalPair(mOnBlipCreated);
    ResetSignalPair(mOnCheckpointCreated);
    ResetSignalPair(mOnKeybindCreated);
    ResetSignalPair(mOnObjectCreated);
    ResetSignalPair(mOnPickupCreated);
    ResetSignalPair(mOnPlayerCreated);
    ResetSignalPair(mOnVehicleCreated);
    ResetSignalPair(mOnBlipDestroyed);
    ResetSignalPair(mOnCheckpointDestroyed);
    ResetSignalPair(mOnKeybindDestroyed);
    ResetSignalPair(mOnObjectDestroyed);
    ResetSignalPair(mOnPickupDestroyed);
    ResetSignalPair(mOnPlayerDestroyed);
    ResetSignalPair(mOnVehicleDestroyed);
    ResetSignalPair(mOnBlipCustom);
    ResetSignalPair(mOnCheckpointCustom);
    ResetSignalPair(mOnKeybindCustom);
    ResetSignalPair(mOnObjectCustom);
    ResetSignalPair(mOnPickupCustom);
    ResetSignalPair(mOnPlayerCustom);
    ResetSignalPair(mOnVehicleCustom);
    ResetSignalPair(mOnServerStartup);
    ResetSignalPair(mOnServerShutdown);
    ResetSignalPair(mOnServerFrame);
    ResetSignalPair(mOnIncomingConnection);
    ResetSignalPair(mOnPlayerRequestClass);
    ResetSignalPair(mOnPlayerRequestSpawn);
    ResetSignalPair(mOnPlayerSpawn);
    ResetSignalPair(mOnPlayerWasted);
    ResetSignalPair(mOnPlayerKilled);
    ResetSignalPair(mOnPlayerEmbarking);
    ResetSignalPair(mOnPlayerEmbarked);
    ResetSignalPair(mOnPlayerDisembark);
    ResetSignalPair(mOnPlayerRename);
    ResetSignalPair(mOnPlayerState);
    ResetSignalPair(mOnStateNone);
    ResetSignalPair(mOnStateNormal);
    ResetSignalPair(mOnStateAim);
    ResetSignalPair(mOnStateDriver);
    ResetSignalPair(mOnStatePassenger);
    ResetSignalPair(mOnStateEnterDriver);
    ResetSignalPair(mOnStateEnterPassenger);
    ResetSignalPair(mOnStateExit);
    ResetSignalPair(mOnStateUnspawned);
    ResetSignalPair(mOnPlayerAction);
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
    ResetSignalPair(mOnPlayerBurning);
    ResetSignalPair(mOnPlayerCrouching);
    ResetSignalPair(mOnPlayerGameKeys);
    ResetSignalPair(mOnPlayerStartTyping);
    ResetSignalPair(mOnPlayerStopTyping);
    ResetSignalPair(mOnPlayerAway);
    ResetSignalPair(mOnPlayerMessage);
    ResetSignalPair(mOnPlayerCommand);
    ResetSignalPair(mOnPlayerPrivateMessage);
    ResetSignalPair(mOnPlayerKeyPress);
    ResetSignalPair(mOnPlayerKeyRelease);
    ResetSignalPair(mOnPlayerSpectate);
    ResetSignalPair(mOnPlayerCrashreport);
    ResetSignalPair(mOnVehicleExplode);
    ResetSignalPair(mOnVehicleRespawn);
    ResetSignalPair(mOnObjectShot);
    ResetSignalPair(mOnObjectTouched);
    ResetSignalPair(mOnObjectWorld);
    ResetSignalPair(mOnObjectAlpha);
    ResetSignalPair(mOnObjectReport);
    ResetSignalPair(mOnPickupClaimed);
    ResetSignalPair(mOnPickupCollected);
    ResetSignalPair(mOnPickupRespawn);
    ResetSignalPair(mOnPickupWorld);
    ResetSignalPair(mOnPickupAlpha);
    ResetSignalPair(mOnPickupAutomatic);
    ResetSignalPair(mOnPickupAutoTimer);
    ResetSignalPair(mOnCheckpointEntered);
    ResetSignalPair(mOnCheckpointExited);
    ResetSignalPair(mOnCheckpointWorld);
    ResetSignalPair(mOnCheckpointRadius);
    ResetSignalPair(mOnEntityPool);
    ResetSignalPair(mOnClientScriptData);
    ResetSignalPair(mOnPlayerUpdate);
    ResetSignalPair(mOnVehicleUpdate);
    ResetSignalPair(mOnPlayerHealth);
    ResetSignalPair(mOnPlayerArmour);
    ResetSignalPair(mOnPlayerWeapon);
    ResetSignalPair(mOnPlayerHeading);
    ResetSignalPair(mOnPlayerPosition);
    ResetSignalPair(mOnPlayerOption);
    ResetSignalPair(mOnPlayerAdmin);
    ResetSignalPair(mOnPlayerWorld);
    ResetSignalPair(mOnPlayerTeam);
    ResetSignalPair(mOnPlayerSkin);
    ResetSignalPair(mOnPlayerMoney);
    ResetSignalPair(mOnPlayerScore);
    ResetSignalPair(mOnPlayerWantedLevel);
    ResetSignalPair(mOnPlayerImmunity);
    ResetSignalPair(mOnPlayerAlpha);
    ResetSignalPair(mOnVehicleColor);
    ResetSignalPair(mOnVehicleHealth);
    ResetSignalPair(mOnVehiclePosition);
    ResetSignalPair(mOnVehicleRotation);
    ResetSignalPair(mOnVehicleOption);
    ResetSignalPair(mOnVehicleWorld);
    ResetSignalPair(mOnVehicleImmunity);
    ResetSignalPair(mOnVehiclePartStatus);
    ResetSignalPair(mOnVehicleTyreStatus);
    ResetSignalPair(mOnVehicleDamageData);
    ResetSignalPair(mOnVehicleRadio);
    ResetSignalPair(mOnVehicleHandlingRule);
    ResetSignalPair(mOnServerOption);
    ResetSignalPair(mOnScriptReload);
    ResetSignalPair(mOnScriptLoaded);
}

} // Namespace:: SqMod
