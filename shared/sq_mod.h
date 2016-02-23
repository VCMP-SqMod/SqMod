//
// SqModule: API used to communicate with and register squirrel modules
//

//
// Copyright (c) 2009 Sandu Liviu Catalin (aka. S.L.C)
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source
//  distribution.
//

#if !defined(_SQ_MOD_H_)
#define _SQ_MOD_H_

#include "sq_api.h"
#include "vcmp.h"

#ifdef __cplusplus
extern "C" {
#endif

    #if defined(_MSC_VER)
        #define SQMOD_API_EXPORT    extern "C" __declspec(dllexport)
    #elif defined(__GNUC__)
        #define SQMOD_API_EXPORT    extern "C"
    #else
        #define SQMOD_API_EXPORT    extern "C"
    #endif

    #define SQMOD_HOST_NAME         "SqModHost"
    #define SQMOD_INITIALIZE_CMD    0xDABBAD00
    #define SQMOD_TERMINATE_CMD     0xDEADC0DE

    /* --------------------------------------------------------------------------------------------
     * Allows modules to interface with the plugin API without linking of any sorts
    */
    typedef struct
    {
        unsigned int    uStructSize;
        HSQAPI          (*GetSquirrelAPI) ( void );
        HSQUIRRELVM     (*GetSquirrelVM) (void);
    } sq_exports, SQEXPORTS, *HSQEXPORTS;

    /* ------------------------------------------------------------------------------------------------
     * PLUGIN SYSTEM
    */
    extern SDK_GetServerVersion                         SqVC_GetServerVersion;
    extern SDK_GetServerSettings                        SqVC_GetServerSettings;
    extern SDK_ExportFunctions                          SqVC_ExportFunctions;
    extern SDK_GetNumberOfPlugins                       SqVC_GetNumberOfPlugins;
    extern SDK_GetPluginInfo                            SqVC_GetPluginInfo;
    extern SDK_FindPlugin                               SqVC_FindPlugin;
    extern SDK_GetPluginExports                         SqVC_GetPluginExports;
    extern SDK_GetTime                                  SqVC_GetTime;

    /* ------------------------------------------------------------------------------------------------
     * MESSAGES
    */
    extern SDK_printf                                  SqVC_printf;
    extern SDK_SendCustomCommand                       SqVC_SendCustomCommand;
    extern SDK_SendClientMessage                       SqVC_SendClientMessage;
    extern SDK_SendGameMessage                         SqVC_SendGameMessage;

    /* ------------------------------------------------------------------------------------------------
     * SERVER SETTINGS
    */
    extern SDK_SetServerName                            SqVC_SetServerName;
    extern SDK_GetServerName                            SqVC_GetServerName;
    extern SDK_SetMaxPlayers                            SqVC_SetMaxPlayers;
    extern SDK_GetMaxPlayers                            SqVC_GetMaxPlayers;
    extern SDK_SetServerPassword                        SqVC_SetServerPassword;
    extern SDK_GetServerPassword                        SqVC_GetServerPassword;
    extern SDK_SetGameModeText                          SqVC_SetGameModeText;
    extern SDK_GetGameModeText                          SqVC_GetGameModeText;
    extern SDK_ShutdownServer                           SqVC_ShutdownServer;

    /* ------------------------------------------------------------------------------------------------
     * WORLD: settings
    */
    extern SDK_SetWorldBounds                           SqVC_SetWorldBounds;
    extern SDK_GetWorldBounds                           SqVC_GetWorldBounds;
    extern SDK_SetWastedSettings                        SqVC_SetWastedSettings;
    extern SDK_GetWastedSettings                        SqVC_GetWastedSettings;
    extern SDK_SetTimeRate                              SqVC_SetTimeRate;
    extern SDK_GetTimeRate                              SqVC_GetTimeRate;
    extern SDK_SetHour                                  SqVC_SetHour;
    extern SDK_GetHour                                  SqVC_GetHour;
    extern SDK_SetMinute                                SqVC_SetMinute;
    extern SDK_GetMinute                                SqVC_GetMinute;
    extern SDK_SetWeather                               SqVC_SetWeather;
    extern SDK_GetWeather                               SqVC_GetWeather;
    extern SDK_SetGravity                               SqVC_SetGravity;
    extern SDK_GetGravity                               SqVC_GetGravity;
    extern SDK_SetGamespeed                             SqVC_SetGamespeed;
    extern SDK_GetGamespeed                             SqVC_GetGamespeed;
    extern SDK_SetWaterLevel                            SqVC_SetWaterLevel;
    extern SDK_GetWaterLevel                            SqVC_GetWaterLevel;
    extern SDK_SetMaxHeight                             SqVC_SetMaxHeight;
    extern SDK_GetMaxHeight                             SqVC_GetMaxHeight;
    extern SDK_SetKillCmdDelay                          SqVC_SetKillCmdDelay;
    extern SDK_GetKillCmdDelay                          SqVC_GetKillCmdDelay;
    extern SDK_SetVehiclesForcedRespawnHeight           SqVC_SetVehiclesForcedRespawnHeight;
    extern SDK_GetVehiclesForcedRespawnHeight           SqVC_GetVehiclesForcedRespawnHeight;

    /* ------------------------------------------------------------------------------------------------
     * WORLD: toggles
    */
    extern SDK_ToggleSyncFrameLimiter                   SqVC_ToggleSyncFrameLimiter;
    extern SDK_EnabledSyncFrameLimiter                  SqVC_EnabledSyncFrameLimiter;
    extern SDK_ToggleFrameLimiter                       SqVC_ToggleFrameLimiter;
    extern SDK_EnabledFrameLimiter                      SqVC_EnabledFrameLimiter;
    extern SDK_ToggleTaxiBoostJump                      SqVC_ToggleTaxiBoostJump;
    extern SDK_EnabledTaxiBoostJump                     SqVC_EnabledTaxiBoostJump;
    extern SDK_ToggleDriveOnWater                       SqVC_ToggleDriveOnWater;
    extern SDK_EnabledDriveOnWater                      SqVC_EnabledDriveOnWater;
    extern SDK_ToggleFastSwitch                         SqVC_ToggleFastSwitch;
    extern SDK_EnabledFastSwitch                        SqVC_EnabledFastSwitch;
    extern SDK_ToggleFriendlyFire                       SqVC_ToggleFriendlyFire;
    extern SDK_EnabledFriendlyFire                      SqVC_EnabledFriendlyFire;
    extern SDK_ToggleDisableDriveby                     SqVC_ToggleDisableDriveby;
    extern SDK_EnabledDisableDriveby                    SqVC_EnabledDisableDriveby;
    extern SDK_TogglePerfectHandling                    SqVC_TogglePerfectHandling;
    extern SDK_EnabledPerfectHandling                   SqVC_EnabledPerfectHandling;
    extern SDK_ToggleFlyingCars                         SqVC_ToggleFlyingCars;
    extern SDK_EnabledFlyingCars                        SqVC_EnabledFlyingCars;
    extern SDK_ToggleJumpSwitch                         SqVC_ToggleJumpSwitch;
    extern SDK_EnabledJumpSwitch                        SqVC_EnabledJumpSwitch;
    extern SDK_ToggleShowMarkers                        SqVC_ToggleShowMarkers;
    extern SDK_EnabledShowMarkers                       SqVC_EnabledShowMarkers;
    extern SDK_ToggleOnlyShowTeamMarkers                SqVC_ToggleOnlyShowTeamMarkers;
    extern SDK_EnabledOnlyShowTeamMarkers               SqVC_EnabledOnlyShowTeamMarkers;
    extern SDK_ToggleStuntBike                          SqVC_ToggleStuntBike;
    extern SDK_EnabledStuntBike                         SqVC_EnabledStuntBike;
    extern SDK_ToggleShootInAir                         SqVC_ToggleShootInAir;
    extern SDK_EnabledShootInAir                        SqVC_EnabledShootInAir;
    extern SDK_ToggleShowNametags                       SqVC_ToggleShowNametags;
    extern SDK_EnabledShowNametags                      SqVC_EnabledShowNametags;
    extern SDK_ToggleJoinMessages                       SqVC_ToggleJoinMessages;
    extern SDK_EnabledJoinMessages                      SqVC_EnabledJoinMessages;
    extern SDK_ToggleDeathMessages                      SqVC_ToggleDeathMessages;
    extern SDK_EnabledDeathMessages                     SqVC_EnabledDeathMessages;
    extern SDK_ToggleChatTagsByDefaultEnabled           SqVC_ToggleChatTagsByDefaultEnabled;
    extern SDK_EnabledChatTagsByDefault                 SqVC_EnabledChatTagsByDefault;

    /* ------------------------------------------------------------------------------------------------
     * MISC
    */
    extern SDK_CreateExplosion                          SqVC_CreateExplosion;
    extern SDK_PlaySound                                SqVC_PlaySound;
    extern SDK_HideMapObject                            SqVC_HideMapObject;
    extern SDK_ShowMapObject                            SqVC_ShowMapObject;
    extern SDK_ShowAllMapObjects                        SqVC_ShowAllMapObjects;

    /* ------------------------------------------------------------------------------------------------
     * WEAPONDATA
    */
    extern SDK_SetWeaponDataValue                       SqVC_SetWeaponDataValue;
    extern SDK_GetWeaponDataValue                       SqVC_GetWeaponDataValue;
    extern SDK_ResetWeaponDataValue                     SqVC_ResetWeaponDataValue;
    extern SDK_IsWeaponDataValueModified                SqVC_IsWeaponDataValueModified;
    extern SDK_ResetWeaponData                          SqVC_ResetWeaponData;
    extern SDK_ResetAllWeaponData                       SqVC_ResetAllWeaponData;

    /* ------------------------------------------------------------------------------------------------
     * KEYBINDS
    */
    extern SDK_GetKeyBindUnusedSlot                     SqVC_GetKeyBindUnusedSlot;
    extern SDK_GetKeyBindData                           SqVC_GetKeyBindData;
    extern SDK_RegisterKeyBind                          SqVC_RegisterKeyBind;
    extern SDK_RemoveKeyBind                            SqVC_RemoveKeyBind;
    extern SDK_RemoveAllKeyBinds                        SqVC_RemoveAllKeyBinds;

    /* ------------------------------------------------------------------------------------------------
     * BLIPS
    */
    extern SDK_CreateCoordBlip                          SqVC_CreateCoordBlip;
    extern SDK_DestroyCoordBlip                         SqVC_DestroyCoordBlip;
    extern SDK_GetCoordBlipInfo                         SqVC_GetCoordBlipInfo;

    /* ------------------------------------------------------------------------------------------------
     * SPRITES
    */
    extern SDK_CreateSprite                             SqVC_CreateSprite;
    extern SDK_DestroySprite                            SqVC_DestroySprite;
    extern SDK_ShowSprite                               SqVC_ShowSprite;
    extern SDK_HideSprite                               SqVC_HideSprite;
    extern SDK_MoveSprite                               SqVC_MoveSprite;
    extern SDK_SetSpriteCenter                          SqVC_SetSpriteCenter;
    extern SDK_RotateSprite                             SqVC_RotateSprite;
    extern SDK_SetSpriteAlpha                           SqVC_SetSpriteAlpha;
    extern SDK_SetSpriteRelativity                      SqVC_SetSpriteRelativity;

    /* ------------------------------------------------------------------------------------------------
     * TEXTDRAWS
    */
    extern SDK_CreateTextdraw                           SqVC_CreateTextdraw;
    extern SDK_DestroyTextdraw                          SqVC_DestroyTextdraw;
    extern SDK_ShowTextdraw                             SqVC_ShowTextdraw;
    extern SDK_HideTextdraw                             SqVC_HideTextdraw;
    extern SDK_MoveTextdraw                             SqVC_MoveTextdraw;
    extern SDK_SetTextdrawColour                        SqVC_SetTextdrawColour;
    extern SDK_SetTextdrawRelativity                    SqVC_SetTextdrawRelativity;

    /* ------------------------------------------------------------------------------------------------
     * RADIOS
    */
    extern SDK_AddRadioStream                           SqVC_AddRadioStream;
    extern SDK_RemoveRadioStream                        SqVC_RemoveRadioStream;

    /* ------------------------------------------------------------------------------------------------
     * CLASSES
    */
    extern SDK_SetUseClasses                            SqVC_SetUseClasses;
    extern SDK_GetUseClasses                            SqVC_GetUseClasses;
    extern SDK_GetPlayerClass                           SqVC_GetPlayerClass;
    extern SDK_AddPlayerClass                           SqVC_AddPlayerClass;
    extern SDK_SetSpawnPlayerPos                        SqVC_SetSpawnPlayerPos;
    extern SDK_SetSpawnCameraPos                        SqVC_SetSpawnCameraPos;
    extern SDK_SetSpawnCameraLookAt                     SqVC_SetSpawnCameraLookAt;

    /* ------------------------------------------------------------------------------------------------
     * ADMIN
    */
    extern SDK_IsPlayerAdmin                            SqVC_IsPlayerAdmin;
    extern SDK_SetPlayerAdmin                           SqVC_SetPlayerAdmin;
    extern SDK_GetPlayerIP                              SqVC_GetPlayerIP;
    extern SDK_KickPlayer                               SqVC_KickPlayer;
    extern SDK_BanPlayer                                SqVC_BanPlayer;
    extern SDK_BanIP                                    SqVC_BanIP;
    extern SDK_UnbanIP                                  SqVC_UnbanIP;
    extern SDK_IsIPBanned                               SqVC_IsIPBanned;

    /* ------------------------------------------------------------------------------------------------
     * PLAYERS: basic
    */
    extern SDK_GetPlayerIDFromName                      SqVC_GetPlayerIDFromName;
    extern SDK_IsPlayerConnected                        SqVC_IsPlayerConnected;
    extern SDK_IsPlayerSpawned                          SqVC_IsPlayerSpawned;
    extern SDK_IsPlayerStreamedForPlayer                SqVC_IsPlayerStreamedForPlayer;
    extern SDK_GetPlayerKey                             SqVC_GetPlayerKey;
    extern SDK_SetPlayerWorld                           SqVC_SetPlayerWorld;
    extern SDK_GetPlayerWorld                           SqVC_GetPlayerWorld;
    extern SDK_SetPlayerSecWorld                        SqVC_SetPlayerSecWorld;
    extern SDK_GetPlayerSecWorld                        SqVC_GetPlayerSecWorld;
    extern SDK_GetPlayerUniqueWorld                     SqVC_GetPlayerUniqueWorld;
    extern SDK_IsPlayerWorldCompatible                  SqVC_IsPlayerWorldCompatible;
    extern SDK_GetPlayerState                           SqVC_GetPlayerState;
    extern SDK_GetPlayerName                            SqVC_GetPlayerName;
    extern SDK_SetPlayerName                            SqVC_SetPlayerName;
    extern SDK_SetPlayerTeam                            SqVC_SetPlayerTeam;
    extern SDK_GetPlayerTeam                            SqVC_GetPlayerTeam;
    extern SDK_SetPlayerSkin                            SqVC_SetPlayerSkin;
    extern SDK_GetPlayerSkin                            SqVC_GetPlayerSkin;
    extern SDK_SetPlayerColour                          SqVC_SetPlayerColour;
    extern SDK_GetPlayerColour                          SqVC_GetPlayerColour;
    extern SDK_ForcePlayerSpawn                         SqVC_ForcePlayerSpawn;
    extern SDK_ForcePlayerSelect                        SqVC_ForcePlayerSelect;
    extern SDK_ForceAllSelect                           SqVC_ForceAllSelect;

    /* ------------------------------------------------------------------------------------------------
     * PLAYERS: score, ping, money, typing
    */
    extern SDK_GivePlayerMoney                          SqVC_GivePlayerMoney;
    extern SDK_SetPlayerMoney                           SqVC_SetPlayerMoney;
    extern SDK_GetPlayerMoney                           SqVC_GetPlayerMoney;
    extern SDK_SetPlayerScore                           SqVC_SetPlayerScore;
    extern SDK_GetPlayerScore                           SqVC_GetPlayerScore;
    extern SDK_GetPlayerPing                            SqVC_GetPlayerPing;
    extern SDK_IsPlayerTyping                           SqVC_IsPlayerTyping;
    extern SDK_GetPlayerFPS                             SqVC_GetPlayerFPS;
    extern SDK_GetPlayerUID                             SqVC_GetPlayerUID;
    extern SDK_GetPlayerWantedLevel                     SqVC_GetPlayerWantedLevel;

    /* ------------------------------------------------------------------------------------------------
     * PLAYERS: health and location
    */
    extern SDK_SetPlayerHealth                          SqVC_SetPlayerHealth;
    extern SDK_GetPlayerHealth                          SqVC_GetPlayerHealth;
    extern SDK_SetPlayerArmour                          SqVC_SetPlayerArmour;
    extern SDK_GetPlayerArmour                          SqVC_GetPlayerArmour;
    extern SDK_SetPlayerImmunityFlags                   SqVC_SetPlayerImmunityFlags;
    extern SDK_GetPlayerImmunityFlags                   SqVC_GetPlayerImmunityFlags;
    extern SDK_SetPlayerPos                             SqVC_SetPlayerPos;
    extern SDK_GetPlayerPos                             SqVC_GetPlayerPos;
    extern SDK_SetPlayerSpeed                           SqVC_SetPlayerSpeed;
    extern SDK_GetPlayerSpeed                           SqVC_GetPlayerSpeed;
    extern SDK_AddPlayerSpeed                           SqVC_AddPlayerSpeed;
    extern SDK_SetPlayerHeading                         SqVC_SetPlayerHeading;
    extern SDK_GetPlayerHeading                         SqVC_GetPlayerHeading;
    extern SDK_SetPlayerAlpha                           SqVC_SetPlayerAlpha;
    extern SDK_GetPlayerAlpha                           SqVC_GetPlayerAlpha;
    extern SDK_GetPlayerOnFireStatus                    SqVC_GetPlayerOnFireStatus;
    extern SDK_GetPlayerCrouchStatus                    SqVC_GetPlayerCrouchStatus;
    extern SDK_GetPlayerAction                          SqVC_GetPlayerAction;
    extern SDK_GetPlayerGameKeys                        SqVC_GetPlayerGameKeys;
    extern SDK_GetPlayerAimPos                          SqVC_GetPlayerAimPos;
    extern SDK_GetPlayerAimDir                          SqVC_GetPlayerAimDir;

    /* ------------------------------------------------------------------------------------------------
     * PLAYERS: vehicle
    */
    extern SDK_PutPlayerInVehicle                       SqVC_PutPlayerInVehicle;
    extern SDK_RemovePlayerFromVehicle                  SqVC_RemovePlayerFromVehicle;
    extern SDK_GetPlayerInVehicleStatus                 SqVC_GetPlayerInVehicleStatus;
    extern SDK_GetPlayerInVehicleSlot                   SqVC_GetPlayerInVehicleSlot;
    extern SDK_GetPlayerVehicleID                       SqVC_GetPlayerVehicleID;

    /* ------------------------------------------------------------------------------------------------
     * PLAYERS: toggles
    */
    extern SDK_TogglePlayerControllable                 SqVC_TogglePlayerControllable;
    extern SDK_EnabledPlayerControllable                SqVC_EnabledPlayerControllable;
    extern SDK_TogglePlayerDriveby                      SqVC_TogglePlayerDriveby;
    extern SDK_EnabledPlayerDriveby                     SqVC_EnabledPlayerDriveby;
    extern SDK_TogglePlayerWhiteScanlines               SqVC_TogglePlayerWhiteScanlines;
    extern SDK_EnabledPlayerWhiteScanlines              SqVC_EnabledPlayerWhiteScanlines;
    extern SDK_TogglePlayerGreenScanlines               SqVC_TogglePlayerGreenScanlines;
    extern SDK_EnabledPlayerGreenScanlines              SqVC_EnabledPlayerGreenScanlines;
    extern SDK_TogglePlayerWidescreen                   SqVC_TogglePlayerWidescreen;
    extern SDK_EnabledPlayerWidescreen                  SqVC_EnabledPlayerWidescreen;
    extern SDK_TogglePlayerShowMarkers                  SqVC_TogglePlayerShowMarkers;
    extern SDK_EnabledPlayerShowMarkers                 SqVC_EnabledPlayerShowMarkers;
    extern SDK_TogglePlayerAttackPriv                   SqVC_TogglePlayerAttackPriv;
    extern SDK_EnabledPlayerAttackPriv                  SqVC_EnabledPlayerAttackPriv;
    extern SDK_TogglePlayerHasMarker                    SqVC_TogglePlayerHasMarker;
    extern SDK_EnabledPlayerHasMarker                   SqVC_EnabledPlayerHasMarker;
    extern SDK_TogglePlayerChatTagsEnabled              SqVC_TogglePlayerChatTagsEnabled;
    extern SDK_EnabledPlayerChatTags                    SqVC_EnabledPlayerChatTags;
    extern SDK_TogglePlayerDrunkEffects                 SqVC_TogglePlayerDrunkEffects;
    extern SDK_EnabledPlayerDrunkEffects                SqVC_EnabledPlayerDrunkEffects;

    /* ------------------------------------------------------------------------------------------------
     * PLAYERS: weapons
    */
    extern SDK_GivePlayerWeapon                         SqVC_GivePlayerWeapon;
    extern SDK_SetPlayerWeapon                          SqVC_SetPlayerWeapon;
    extern SDK_GetPlayerWeapon                          SqVC_GetPlayerWeapon;
    extern SDK_GetPlayerWeaponAmmo                      SqVC_GetPlayerWeaponAmmo;
    extern SDK_SetPlayerWeaponSlot                      SqVC_SetPlayerWeaponSlot;
    extern SDK_GetPlayerWeaponSlot                      SqVC_GetPlayerWeaponSlot;
    extern SDK_GetPlayerWeaponAtSlot                    SqVC_GetPlayerWeaponAtSlot;
    extern SDK_GetPlayerAmmoAtSlot                      SqVC_GetPlayerAmmoAtSlot;
    extern SDK_RemovePlayerWeapon                       SqVC_RemovePlayerWeapon;
    extern SDK_RemoveAllWeapons                         SqVC_RemoveAllWeapons;

    /* ------------------------------------------------------------------------------------------------
     * PLAYERS: camera
    */
    extern SDK_SetCameraPosition                        SqVC_SetCameraPosition;
    extern SDK_RestoreCamera                            SqVC_RestoreCamera;
    extern SDK_IsCameraLocked                           SqVC_IsCameraLocked;

    /* ------------------------------------------------------------------------------------------------
     * PLAYERS: misc
    */
    extern SDK_SetPlayerAnimation                       SqVC_SetPlayerAnimation;
    extern SDK_SetPlayerWantedLevel                     SqVC_SetPlayerWantedLevel;
    extern SDK_GetPlayerStandingOnVehicle               SqVC_GetPlayerStandingOnVehicle;
    extern SDK_GetPlayerStandingOnObject                SqVC_GetPlayerStandingOnObject;
    extern SDK_IsPlayerAway                             SqVC_IsPlayerAway;
    extern SDK_GetPlayerSpectateTarget                  SqVC_GetPlayerSpectateTarget;
    extern SDK_SetPlayerSpectateTarget                  SqVC_SetPlayerSpectateTarget;
    extern SDK_RedirectPlayerToServer                   SqVC_RedirectPlayerToServer;

    /* ------------------------------------------------------------------------------------------------
     * VEHICLES
    */
    extern SDK_CreateVehicle                            SqVC_CreateVehicle;
    extern SDK_DeleteVehicle                            SqVC_DeleteVehicle;
    extern SDK_GetVehicleSyncSource                     SqVC_GetVehicleSyncSource;
    extern SDK_GetVehicleSyncType                       SqVC_GetVehicleSyncType;
    extern SDK_IsVehicleStreamedForPlayer               SqVC_IsVehicleStreamedForPlayer;
    extern SDK_SetVehicleWorld                          SqVC_SetVehicleWorld;
    extern SDK_GetVehicleWorld                          SqVC_GetVehicleWorld;
    extern SDK_GetVehicleModel                          SqVC_GetVehicleModel;
    extern SDK_GetVehicleOccupant                       SqVC_GetVehicleOccupant;
    extern SDK_RespawnVehicle                           SqVC_RespawnVehicle;
    extern SDK_SetVehicleImmunityFlags                  SqVC_SetVehicleImmunityFlags;
    extern SDK_GetVehicleImmunityFlags                  SqVC_GetVehicleImmunityFlags;
    extern SDK_KillVehicle                              SqVC_KillVehicle;
    extern SDK_IsVehicleWrecked                         SqVC_IsVehicleWrecked;
    extern SDK_SetVehiclePos                            SqVC_SetVehiclePos;
    extern SDK_GetVehiclePos                            SqVC_GetVehiclePos;
    extern SDK_SetVehicleRot                            SqVC_SetVehicleRot;
    extern SDK_SetVehicleRotEuler                       SqVC_SetVehicleRotEuler;
    extern SDK_GetVehicleRot                            SqVC_GetVehicleRot;
    extern SDK_GetVehicleRotEuler                       SqVC_GetVehicleRotEuler;
    extern SDK_SetVehicleSpeed                          SqVC_SetVehicleSpeed;
    extern SDK_GetVehicleSpeed                          SqVC_GetVehicleSpeed;
    extern SDK_AddVehicleSpeed                          SqVC_AddVehicleSpeed;
    extern SDK_SetVehicleRelSpeed                       SqVC_SetVehicleRelSpeed;
    extern SDK_GetVehicleRelSpeed                       SqVC_GetVehicleRelSpeed;
    extern SDK_AddVehicleRelSpeed                       SqVC_AddVehicleRelSpeed;
    extern SDK_SetVehicleTurnSpeed                      SqVC_SetVehicleTurnSpeed;
    extern SDK_GetVehicleTurnSpeed                      SqVC_GetVehicleTurnSpeed;
    extern SDK_AddVehicleTurnSpeed                      SqVC_AddVehicleTurnSpeed;
    extern SDK_SetVehicleRelTurnSpeed                   SqVC_SetVehicleRelTurnSpeed;
    extern SDK_GetVehicleRelTurnSpeed                   SqVC_GetVehicleRelTurnSpeed;
    extern SDK_AddVehicleRelTurnSpeed                   SqVC_AddVehicleRelTurnSpeed;
    extern SDK_SetVehicleSpawnPos                       SqVC_SetVehicleSpawnPos;
    extern SDK_GetVehicleSpawnPos                       SqVC_GetVehicleSpawnPos;
    extern SDK_SetVehicleSpawnRot                       SqVC_SetVehicleSpawnRot;
    extern SDK_SetVehicleSpawnRotEuler                  SqVC_SetVehicleSpawnRotEuler;
    extern SDK_GetVehicleSpawnRot                       SqVC_GetVehicleSpawnRot;
    extern SDK_GetVehicleSpawnRotEuler                  SqVC_GetVehicleSpawnRotEuler;
    extern SDK_SetVehicleIdleRespawnTimer               SqVC_SetVehicleIdleRespawnTimer;
    extern SDK_GetVehicleIdleRespawnTimer               SqVC_GetVehicleIdleRespawnTimer;
    extern SDK_SetVehicleHealth                         SqVC_SetVehicleHealth;
    extern SDK_GetVehicleHealth                         SqVC_GetVehicleHealth;
    extern SDK_SetVehicleColour                         SqVC_SetVehicleColour;
    extern SDK_GetVehicleColour                         SqVC_GetVehicleColour;
    extern SDK_SetVehicleDoorsLocked                    SqVC_SetVehicleDoorsLocked;
    extern SDK_GetVehicleDoorsLocked                    SqVC_GetVehicleDoorsLocked;
    extern SDK_SetVehiclePartStatus                     SqVC_SetVehiclePartStatus;
    extern SDK_GetVehiclePartStatus                     SqVC_GetVehiclePartStatus;
    extern SDK_SetVehicleTyreStatus                     SqVC_SetVehicleTyreStatus;
    extern SDK_GetVehicleTyreStatus                     SqVC_GetVehicleTyreStatus;
    extern SDK_SetVehicleDamageData                     SqVC_SetVehicleDamageData;
    extern SDK_GetVehicleDamageData                     SqVC_GetVehicleDamageData;
    extern SDK_SetVehicleAlarm                          SqVC_SetVehicleAlarm;
    extern SDK_GetVehicleAlarm                          SqVC_GetVehicleAlarm;
    extern SDK_SetVehicleLights                         SqVC_SetVehicleLights;
    extern SDK_GetVehicleLights                         SqVC_GetVehicleLights;
    extern SDK_SetVehicleRadio                          SqVC_SetVehicleRadio;
    extern SDK_GetVehicleRadio                          SqVC_GetVehicleRadio;
    extern SDK_SetVehicleRadioLocked                    SqVC_SetVehicleRadioLocked;
    extern SDK_IsVehicleRadioLocked                     SqVC_IsVehicleRadioLocked;
    extern SDK_GetVehicleGhostState                     SqVC_GetVehicleGhostState;
    extern SDK_SetVehicleGhostState                     SqVC_SetVehicleGhostState;
    extern SDK_GetVehicleTurretRotation                 SqVC_GetVehicleTurretRotation;

    /* ------------------------------------------------------------------------------------------------
     * VEHICLES: handling
    */
    extern SDK_ResetAllVehicleHandlings                 SqVC_ResetAllVehicleHandlings;
    extern SDK_ExistsHandlingRule                       SqVC_ExistsHandlingRule;
    extern SDK_SetHandlingRule                          SqVC_SetHandlingRule;
    extern SDK_GetHandlingRule                          SqVC_GetHandlingRule;
    extern SDK_ResetHandlingRule                        SqVC_ResetHandlingRule;
    extern SDK_ResetHandling                            SqVC_ResetHandling;
    extern SDK_ExistsInstHandlingRule                   SqVC_ExistsInstHandlingRule;
    extern SDK_SetInstHandlingRule                      SqVC_SetInstHandlingRule;
    extern SDK_GetInstHandlingRule                      SqVC_GetInstHandlingRule;
    extern SDK_ResetInstHandlingRule                    SqVC_ResetInstHandlingRule;
    extern SDK_ResetInstHandling                        SqVC_ResetInstHandling;

    /* ------------------------------------------------------------------------------------------------
     * PICKUPS
    */
    extern SDK_CreatePickup                             SqVC_CreatePickup;
    extern SDK_DeletePickup                             SqVC_DeletePickup;
    extern SDK_IsPickupStreamedForPlayer                SqVC_IsPickupStreamedForPlayer;
    extern SDK_SetPickupWorld                           SqVC_SetPickupWorld;
    extern SDK_GetPickupWorld                           SqVC_GetPickupWorld;
    extern SDK_PickupGetAlpha                           SqVC_PickupGetAlpha;
    extern SDK_PickupSetAlpha                           SqVC_PickupSetAlpha;
    extern SDK_PickupIsAutomatic                        SqVC_PickupIsAutomatic;
    extern SDK_PickupSetAutomatic                       SqVC_PickupSetAutomatic;
    extern SDK_SetPickupAutoTimer                       SqVC_SetPickupAutoTimer;
    extern SDK_GetPickupAutoTimer                       SqVC_GetPickupAutoTimer;
    extern SDK_PickupRefresh                            SqVC_PickupRefresh;
    extern SDK_PickupGetPos                             SqVC_PickupGetPos;
    extern SDK_PickupSetPos                             SqVC_PickupSetPos;
    extern SDK_PickupGetModel                           SqVC_PickupGetModel;
    extern SDK_PickupGetQuantity                        SqVC_PickupGetQuantity;

    /* ------------------------------------------------------------------------------------------------
     * OBJECTS
    */
    extern SDK_CreateObject                             SqVC_CreateObject;
    extern SDK_DeleteObject                             SqVC_DeleteObject;
    extern SDK_IsObjectStreamedForPlayer                SqVC_IsObjectStreamedForPlayer;
    extern SDK_GetObjectModel                           SqVC_GetObjectModel;
    extern SDK_SetObjectWorld                           SqVC_SetObjectWorld;
    extern SDK_GetObjectWorld                           SqVC_GetObjectWorld;
    extern SDK_SetObjectAlpha                           SqVC_SetObjectAlpha;
    extern SDK_GetObjectAlpha                           SqVC_GetObjectAlpha;
    extern SDK_MoveObjectTo                             SqVC_MoveObjectTo;
    extern SDK_MoveObjectBy                             SqVC_MoveObjectBy;
    extern SDK_SetObjectPos                             SqVC_SetObjectPos;
    extern SDK_GetObjectPos                             SqVC_GetObjectPos;
    extern SDK_RotObjectTo                              SqVC_RotObjectTo;
    extern SDK_RotObjectToEuler                         SqVC_RotObjectToEuler;
    extern SDK_RotObjectBy                              SqVC_RotObjectBy;
    extern SDK_RotObjectByEuler                         SqVC_RotObjectByEuler;
    extern SDK_GetObjectRot                             SqVC_GetObjectRot;
    extern SDK_GetObjectRotEuler                        SqVC_GetObjectRotEuler;
    extern SDK_SetObjectShotReport                      SqVC_SetObjectShotReport;
    extern SDK_IsObjectShotReport                       SqVC_IsObjectShotReport;
    extern SDK_SetObjectBumpReport                      SqVC_SetObjectBumpReport;
    extern SDK_IsObjectBumpReport                       SqVC_IsObjectBumpReport;

    /* ------------------------------------------------------------------------------------------------
     *
    */
    extern SDK_ToggleWallglitch                         SqVC_ToggleWallglitch;
    extern SDK_EnabledWallglitch                        SqVC_EnabledWallglitch;
    extern SDK_SetVehicleSiren                          SqVC_SetVehicleSiren;
    extern SDK_GetVehicleSiren                          SqVC_GetVehicleSiren;
    extern SDK_GetPlayerUID2                            SqVC_GetPlayerUID2;

    /* ------------------------------------------------------------------------------------------------
     *
    */
    extern SDK_CreateCheckpoint                         SqVC_CreateCheckpoint;
    extern SDK_DeleteCheckpoint                         SqVC_DeleteCheckpoint;
    extern SDK_IsCheckpointStreamedForPlayer            SqVC_IsCheckpointStreamedForPlayer;
    extern SDK_SetCheckpointWorld                       SqVC_SetCheckpointWorld;
    extern SDK_GetCheckpointWorld                       SqVC_GetCheckpointWorld;
    extern SDK_SetCheckpointColor                       SqVC_SetCheckpointColor;
    extern SDK_GetCheckpointColor                       SqVC_GetCheckpointColor;
    extern SDK_SetCheckpointPos                         SqVC_SetCheckpointPos;
    extern SDK_GetCheckpointPos                         SqVC_GetCheckpointPos;
    extern SDK_SetCheckpointRadius                      SqVC_SetCheckpointRadius;
    extern SDK_GetCheckpointRadius                      SqVC_GetCheckpointRadius;
    extern SDK_GetCheckpointOwner                       SqVC_GetCheckpointOwner;

    /* ------------------------------------------------------------------------------------------------
     *
    */
    extern SDK_CreateSphere                             SqVC_CreateSphere;
    extern SDK_DeleteSphere                             SqVC_DeleteSphere;
    extern SDK_IsSphereStreamedForPlayer                SqVC_IsSphereStreamedForPlayer;
    extern SDK_SetSphereWorld                           SqVC_SetSphereWorld;
    extern SDK_GetSphereWorld                           SqVC_GetSphereWorld;
    extern SDK_SetSphereColor                           SqVC_SetSphereColor;
    extern SDK_GetSphereColor                           SqVC_GetSphereColor;
    extern SDK_SetSpherePos                             SqVC_SetSpherePos;
    extern SDK_GetSpherePos                             SqVC_GetSpherePos;
    extern SDK_SetSphereRadius                          SqVC_SetSphereRadius;
    extern SDK_GetSphereRadius                          SqVC_GetSphereRadius;
    extern SDK_GetSphereOwner                           SqVC_GetSphereOwner;

    /* --------------------------------------------------------------------------------------------
     * Import the functions from the main squirrel plugin.
    */
    SQUIRREL_API HSQEXPORTS sq_api_import(PluginFuncs * vcapi);

    /* --------------------------------------------------------------------------------------------
     * Assign the functions from the specified API structure into the global functions.
    */
    SQUIRREL_API SQRESULT sq_api_expand(HSQAPI sqapi);

    /* --------------------------------------------------------------------------------------------
     * Undo changes done by sq_api_expand.
    */
    SQUIRREL_API void sq_api_collapse();

    /* --------------------------------------------------------------------------------------------
     * Assign the functions from the specified API structure into the global functions.
    */
    SQUIRREL_API SQRESULT vc_api_expand(PluginFuncs * vcapi);

    /* --------------------------------------------------------------------------------------------
     * Undo changes done by vc_api_expand.
    */
    SQUIRREL_API void vc_api_collapse();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_SQ_API_H_*/
