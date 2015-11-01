/*
   Project: Vice City Multiplayer 0.4 Server / Plugin Kit
   File: plugin.h

   Copyright 2011 Ago Allikmaa (maxorator)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#pragma once

#if !defined _MSC_VER || _MSC_VER >= 1600
#include <stdint.h>
#elif defined _MSC_VER
typedef unsigned __int64 uint64_t;
#endif

typedef struct {
    unsigned int uStructSize;
    char szServerName[128];
    unsigned int uMaxPlayers;
    unsigned int uPort;
    unsigned int uFlags;
} ServerSettings;

typedef struct {
    unsigned int uStructSize;
    int nPluginId;
    char szName[32];
    unsigned int uPluginVer;
} PluginInfo;

#define SDK_ENTPOOL_VEHICLE     1
#define SDK_ENTPOOL_OBJECT      2
#define SDK_ENTPOOL_PICKUP      3
#define SDK_ENTPOOL_RADIO       4
#define SDK_ENTPOOL_SPRITE      5
#define SDK_ENTPOOL_TEXTDRAW    6
#define SDK_ENTPOOL_BLIP        7

#define SDK_PSTATE_NONE         0
#define SDK_PSTATE_NORMAL       1
#define SDK_PSTATE_AIM          2
#define SDK_PSTATE_DRIVER       3
#define SDK_PSTATE_PASSENGER    4
#define SDK_PSTATE_ENTER_DRIVER 5
#define SDK_PSTATE_ENTER_PASS   6
#define SDK_PSTATE_EXIT         7
#define SDK_PSTATE_UNSPAWNED    8

#define SDK_PLAYERUPD_NORMAL    0
#define SDK_PLAYERUPD_AIMING    1
#define SDK_PLAYERUPD_DRIVER    2
#define SDK_PLAYERUPD_PASSENGER 3

#define SDK_VSYNCTYPE_NONE      0
#define SDK_VSYNCTYPE_DRIVER    1
#define SDK_VSYNCTYPE_DRIVEREX  2
#define SDK_VSYNCTYPE_PASSENGER 3
#define SDK_VSYNCTYPE_NEAR      4

#define SDK_VUPDATE_DRIVERSYNC  0
#define SDK_VUPDATE_OTHERSYNC   1
#define SDK_VUPDATE_POSITION    2
#define SDK_VUPDATE_HEALTH      4
#define SDK_VUPDATE_COLOUR      5
#define SDK_VUPDATE_ROTATION    6

typedef unsigned int (*SDK_GetServerVersion) (void);
typedef unsigned int (*SDK_GetServerSettings) (ServerSettings* pstSettings);
typedef unsigned int (*SDK_ExportFunctions) (int nPluginId, void** ppFunctionList, unsigned int uSize);
typedef unsigned int (*SDK_GetNumberOfPlugins) (void);
typedef unsigned int (*SDK_GetPluginInfo) (int nPluginId, PluginInfo* pstPluginInfo);
typedef int (*SDK_FindPlugin) (char* pszPluginName);
typedef void** (*SDK_GetPluginExports) (int nPluginId, unsigned int* puSize);
typedef int (*SDK_GetTime) (uint64_t* pullTime);
typedef int (*SDK_printf) (const char* pszFormat, ...);
typedef int (*SDK_SendCustomCommand) (unsigned int uCmdType, const char* pszFormat, ...);
typedef int (*SDK_SendClientMessage) (int nPlayerId, unsigned int uColour, const char* pszFormat, ...);
typedef int (*SDK_SendGameMessage) (int nPlayerId, int nType, const char* pszFormat, ... );
typedef int (*SDK_SetServerName) (const char* pszText);
typedef int (*SDK_GetServerName) (char* pszBuffer, int nBufferLen);
typedef int (*SDK_SetMaxPlayers) (int nMaxPlayers);
typedef int (*SDK_GetMaxPlayers) (void);
typedef int (*SDK_SetServerPassword) (char* pszBuffer);
typedef int (*SDK_GetServerPassword) (char* pszBuffer, int nBufferLen);
typedef int (*SDK_SetGameModeText) (const char* pszText);
typedef int (*SDK_GetGameModeText) (char* pszBuffer, int nBufferLen);
typedef int (*SDK_ShutdownServer) (void);
typedef int (*SDK_SetWorldBounds) (float fMaxX, float fMinX, float fMaxY, float fMinY);
typedef int (*SDK_GetWorldBounds) (float* pfMaxX, float* pfMinX, float* pfMaxY, float* pfMinY);
typedef int (*SDK_SetWastedSettings) (unsigned int dwDeathTimer, unsigned int dwFadeTimer, float fFadeInSpeed, float fFadeOutSpeed, unsigned int dwFadeColour, unsigned int dwCorpseFadeStart, unsigned int dwCorpseFadeTime);
typedef int (*SDK_GetWastedSettings) (unsigned int* pdwDeathTimer, unsigned int* pdwFadeTimer, float* pfFadeInSpeed, float* pfFadeOutSpeed, unsigned int* pdwFadeColour, unsigned int* pdwCorpseFadeStart, unsigned int* pdwCorpseFadeTime);
typedef int (*SDK_SetTimeRate) (unsigned int uTimeRate);
typedef unsigned int (*SDK_GetTimeRate) (void);
typedef int (*SDK_SetHour) (int nHour);
typedef int (*SDK_GetHour) (void);
typedef int (*SDK_SetMinute) (int nMinute);
typedef int (*SDK_GetMinute) (void);
typedef int (*SDK_SetWeather) (int nWeather);
typedef int (*SDK_GetWeather) (void);
typedef int (*SDK_SetGravity) (float fGravity);
typedef float (*SDK_GetGravity) (void);
typedef int (*SDK_SetGamespeed) (float fGamespeed);
typedef float (*SDK_GetGamespeed) (void);
typedef int (*SDK_SetWaterLevel) (float fWaterLevel);
typedef float (*SDK_GetWaterLevel) (void);
typedef int (*SDK_SetMaxHeight) (float fHeight);
typedef float (*SDK_GetMaxHeight) (void);
typedef int (*SDK_SetKillCmdDelay) (int nDelay);
typedef int (*SDK_GetKillCmdDelay) (void);
typedef int (*SDK_SetVehiclesForcedRespawnHeight) (float fHeight);
typedef float (*SDK_GetVehiclesForcedRespawnHeight) (void);
typedef int (*SDK_ToggleSyncFrameLimiter) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledSyncFrameLimiter) (void);
typedef int (*SDK_ToggleFrameLimiter) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledFrameLimiter) (void);
typedef int (*SDK_ToggleTaxiBoostJump) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledTaxiBoostJump) (void);
typedef int (*SDK_ToggleDriveOnWater) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledDriveOnWater) (void);
typedef int (*SDK_ToggleFastSwitch) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledFastSwitch) (void);
typedef int (*SDK_ToggleFriendlyFire) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledFriendlyFire) (void);
typedef int (*SDK_ToggleDisableDriveby) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledDisableDriveby) (void);
typedef int (*SDK_TogglePerfectHandling) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledPerfectHandling) (void);
typedef int (*SDK_ToggleFlyingCars) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledFlyingCars) (void);
typedef int (*SDK_ToggleJumpSwitch) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledJumpSwitch) (void);
typedef int (*SDK_ToggleShowMarkers) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledShowMarkers) (void);
typedef int (*SDK_ToggleOnlyShowTeamMarkers) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledOnlyShowTeamMarkers) (void);
typedef int (*SDK_ToggleStuntBike) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledStuntBike) (void);
typedef int (*SDK_ToggleShootInAir) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledShootInAir) (void);
typedef int (*SDK_ToggleShowNametags) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledShowNametags) (void);
typedef int (*SDK_ToggleJoinMessages) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledJoinMessages) (void);
typedef int (*SDK_ToggleDeathMessages) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledDeathMessages) (void);
typedef int (*SDK_ToggleChatTagsByDefaultEnabled) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledChatTagsByDefault) (void);
typedef int (*SDK_CreateExplosion) (int nWorldId, int nType, float fPosX, float fPosY, float fPosZ, int nSourcePlayerId, unsigned int bGroundLevel);
typedef int (*SDK_PlaySound) (int nWorldId, int nSoundId, float fPosX, float fPosY, float fPosZ);
typedef int (*SDK_HideMapObject) (int nModelId, int nTenthX, int nTenthY, int nTenthZ);
typedef int (*SDK_ShowMapObject) (int nModelId, int nTenthX, int nTenthY, int nTenthZ);
typedef int (*SDK_ShowAllMapObjects) (void);
typedef int (*SDK_SetWeaponDataValue) (int nWeaponId, int nFieldId, double fValue);
typedef double (*SDK_GetWeaponDataValue) (int nWeaponId, int nFieldId);
typedef int (*SDK_ResetWeaponDataValue) (int nWeaponId, int nFieldId);
typedef unsigned int (*SDK_IsWeaponDataValueModified) (int nWeaponId, int nFieldId);
typedef int (*SDK_ResetWeaponData) (int nWeaponId);
typedef int (*SDK_ResetAllWeaponData) (void);
typedef int (*SDK_GetKeyBindUnusedSlot) (void);
typedef unsigned int (*SDK_GetKeyBindData) (int nBindId, unsigned int* pbOnRelease, int* nKeyOne, int* pnKeyTwo, int* pnKeyThree);
typedef unsigned int (*SDK_RegisterKeyBind) (int nBindId, unsigned int bOnRelease, int nKeyOne, int nKeyTwo, int nKeyThree);
typedef unsigned int (*SDK_RemoveKeyBind) (int nBindId);
typedef void (*SDK_RemoveAllKeyBinds) (void);
typedef int (*SDK_CreateCoordBlip) (int nIndex, int nWorld, float fX, float fY, float fZ, int nScale, unsigned int uColour, int nSprite);
typedef void (*SDK_DestroyCoordBlip) (int nIndex);
typedef unsigned int (*SDK_GetCoordBlipInfo) (int nIndex, int* pnWorld, float* pfX, float* pfY, float* pfZ, int* pnScale, unsigned int* puColour, int* pnSprite);
typedef int (*SDK_CreateSprite) (int nIndex, const char * pszFilename, int fX, int fY, int fRotX, int fRotY, float fRotation, unsigned char byAlpha, unsigned int isRelative);
typedef void (*SDK_DestroySprite) (int nIndex);
typedef void (*SDK_ShowSprite) (int nIndex, int nPlayerId);
typedef void (*SDK_HideSprite) (int nIndex, int nPlayerId);
typedef void (*SDK_MoveSprite) (int nIndex, int nPlayerId, int fX, int fY);
typedef void (*SDK_SetSpriteCenter) (int nIndex, int nPlayerId, int fX, int fY);
typedef void (*SDK_RotateSprite) (int nIndex, int nPlayerId, float fRotation);
typedef void (*SDK_SetSpriteAlpha) (int nIndex, int nPlayerId, unsigned char byAlpha);
typedef void (*SDK_SetSpriteRelativity) (int nIndex, int nPlayerId, unsigned int isRelative);
typedef int (*SDK_CreateTextdraw) (int nIndex, const char * pszText, int lX, int lY, unsigned int dwColour, unsigned int isRelative);
typedef void (*SDK_DestroyTextdraw) (int nIndex);
typedef void (*SDK_ShowTextdraw) (int nIndex, int nPlayerId);
typedef void (*SDK_HideTextdraw) (int nIndex, int nPlayerId);
typedef void (*SDK_MoveTextdraw) (int nIndex, int nPlayerId, int lX, int lY);
typedef void (*SDK_SetTextdrawColour) (int nIndex, int nPlayerId, unsigned int dwColour);
typedef void (*SDK_SetTextdrawRelativity) (int nIndex, int nPlayerId, unsigned int isRelative);
typedef int (*SDK_AddRadioStream) (int nRadioId, const char* pszRadioName, const char* pszRadioURL, unsigned int bIsListed);
typedef int (*SDK_RemoveRadioStream) (int nRadioId);
typedef int (*SDK_SetUseClasses) (unsigned int bToggle);
typedef unsigned int (*SDK_GetUseClasses) (void);
typedef int (*SDK_GetPlayerClass) (int nPlayerId);
typedef int (*SDK_AddPlayerClass) (int nTeamId, unsigned int uColour, int nModelId, float fSpawnX, float fSpawnY, float fSpawnZ, float fAngleZ, int nWep1, int nWep1Ammo, int nWep2, int nWep2Ammo, int nWep3, int nWep3Ammo);
typedef int (*SDK_SetSpawnPlayerPos) (float fPosX, float fPosY, float fPosZ);
typedef int (*SDK_SetSpawnCameraPos) (float fPosX, float fPosY, float fPosZ);
typedef int (*SDK_SetSpawnCameraLookAt) (float fPosX, float fPosY, float fPosZ);
typedef unsigned int (*SDK_IsPlayerAdmin) (int nPlayerId);
typedef int (*SDK_SetPlayerAdmin) (int nPlayerId, unsigned int bToggle);
typedef int (*SDK_GetPlayerIP) (int nPlayerId, char* pszBuffer, int nBufferLen);
typedef int (*SDK_KickPlayer) (int nPlayerId);
typedef int (*SDK_BanPlayer) (int nPlayerId);
typedef int (*SDK_BanIP) (char* pszIPAddress);
typedef int (*SDK_UnbanIP) (char* pszIPAddress);
typedef unsigned int (*SDK_IsIPBanned) (char* pszIPAddress);
typedef int (*SDK_GetPlayerIDFromName) (char* pszName);
typedef unsigned int (*SDK_IsPlayerConnected) (int nPlayerId);
typedef unsigned int (*SDK_IsPlayerSpawned) (int nPlayerId);
typedef unsigned int (*SDK_IsPlayerStreamedForPlayer) (int nCheckPlayer, int nPlayerId);
typedef unsigned int (*SDK_GetPlayerKey) (int nPlayerId);
typedef int (*SDK_SetPlayerWorld) (int nPlayerId, int nWorld);
typedef int (*SDK_GetPlayerWorld) (int nPlayerId);
typedef int (*SDK_SetPlayerSecWorld) (int nPlayerId, int nSecWorld);
typedef int (*SDK_GetPlayerSecWorld) (int nPlayerId);
typedef int (*SDK_GetPlayerUniqueWorld) (int nPlayerId);
typedef unsigned int (*SDK_IsPlayerWorldCompatible) (int nPlayerId, int nWorld);
typedef int (*SDK_GetPlayerState) (int nPlayerId);
typedef int (*SDK_GetPlayerName) (int nPlayerId, char* szBuffer, int nBufferLen);
typedef unsigned int (*SDK_SetPlayerName) (int nPlayerId, const char* pszName);
typedef int (*SDK_SetPlayerTeam) (int nPlayerId, int nTeamId);
typedef int (*SDK_GetPlayerTeam) (int nPlayerId);
typedef int (*SDK_SetPlayerSkin) (int nPlayerId, int nSkinId);
typedef int (*SDK_GetPlayerSkin) (int nPlayerId);
typedef int (*SDK_SetPlayerColour) (int nPlayerId, unsigned int uColour);
typedef unsigned int (*SDK_GetPlayerColour) (int nPlayerId);
typedef int (*SDK_ForcePlayerSpawn) (int nPlayerId);
typedef int (*SDK_ForcePlayerSelect) (int nPlayerId);
typedef int (*SDK_ForceAllSelect) (void);
typedef int (*SDK_GivePlayerMoney) (int nPlayerId, int nAmount);
typedef int (*SDK_SetPlayerMoney) (int nPlayerId, int nAmount);
typedef int (*SDK_GetPlayerMoney) (int nPlayerId);
typedef int (*SDK_SetPlayerScore) (int nPlayerId, int nScore);
typedef int (*SDK_GetPlayerScore) (int nPlayerId);
typedef int (*SDK_GetPlayerPing) (int nPlayerId);
typedef unsigned int (*SDK_IsPlayerTyping) (int nPlayerId);
typedef double (*SDK_GetPlayerFPS) (int nPlayerId);
typedef int (*SDK_GetPlayerUID) (int nPlayerId, char* szBuffer, int nBufferLen);
typedef int (*SDK_GetPlayerWantedLevel) (int nPlayerId);
typedef int (*SDK_SetPlayerHealth) (int nPlayerId, float fHealth);
typedef float (*SDK_GetPlayerHealth) (int nPlayerId);
typedef int (*SDK_SetPlayerArmour) (int nPlayerId, float fArmour);
typedef float (*SDK_GetPlayerArmour) (int nPlayerId);
typedef int (*SDK_SetPlayerImmunityFlags) (int nPlayerId, int nFlags);
typedef int (*SDK_GetPlayerImmunityFlags) (int nPlayerId);
typedef int (*SDK_SetPlayerPos) (int nPlayerId, float fPosX, float fPosY, float fPosZ);
typedef int (*SDK_GetPlayerPos) (int nPlayerId, float* pfPosX, float* pfPosY, float* pfPosZ);
typedef int (*SDK_SetPlayerSpeed) (int nPlayerId, float fSpeedX, float fSpeedY, float fSpeedZ);
typedef int (*SDK_GetPlayerSpeed) (int nPlayerId, float* pfSpeedX, float* pfSpeedY, float* pfSpeedZ);
typedef int (*SDK_AddPlayerSpeed) (int nPlayerId, float fSpeedX, float fSpeedY, float fSpeedZ);
typedef int (*SDK_SetPlayerHeading) (int nPlayerId, float fAngleZ);
typedef float (*SDK_GetPlayerHeading) (int nPlayerId);
typedef int (*SDK_SetPlayerAlpha) (int nPlayerId, int nAlpha, int nFadeTime);
typedef int (*SDK_GetPlayerAlpha) (int nPlayerId);
typedef unsigned int (*SDK_GetPlayerOnFireStatus) (int nPlayerId);
typedef unsigned int (*SDK_GetPlayerCrouchStatus) (int nPlayerId);
typedef int (*SDK_GetPlayerAction) (int nPlayerId);
typedef int (*SDK_GetPlayerGameKeys) (int nPlayerId);
typedef unsigned int (*SDK_GetPlayerAimPos) (int nPlayerId, float* pfX, float* pfY, float* pfZ);
typedef unsigned int (*SDK_GetPlayerAimDir) (int nPlayerId, float* pfX, float* pfY, float* pfZ);
typedef int (*SDK_PutPlayerInVehicle) (int nPlayerId, int nVehicleId, int nSlot, unsigned int bMakeRoom, unsigned int bWarp);
typedef int (*SDK_RemovePlayerFromVehicle) (int nPlayerId);
typedef int (*SDK_GetPlayerInVehicleStatus) (int nPlayerId);
typedef int (*SDK_GetPlayerInVehicleSlot) (int nPlayerId);
typedef int (*SDK_GetPlayerVehicleID) (int nPlayerId);
typedef int (*SDK_TogglePlayerControllable) (int nPlayerId, unsigned int bToggle);
typedef unsigned int (*SDK_EnabledPlayerControllable) (int nPlayerId);
typedef int (*SDK_TogglePlayerDriveby) (int nPlayerId, unsigned int bToggle);
typedef unsigned int (*SDK_EnabledPlayerDriveby) (int nPlayerId);
typedef int (*SDK_TogglePlayerWhiteScanlines) (int nPlayerId, unsigned int bToggle);
typedef unsigned int (*SDK_EnabledPlayerWhiteScanlines) (int nPlayerId);
typedef int (*SDK_TogglePlayerGreenScanlines) (int nPlayerId, unsigned int bToggle);
typedef unsigned int (*SDK_EnabledPlayerGreenScanlines) (int nPlayerId);
typedef int (*SDK_TogglePlayerWidescreen) (int nPlayerId, unsigned int bToggle);
typedef unsigned int (*SDK_EnabledPlayerWidescreen) (int nPlayerId);
typedef int (*SDK_TogglePlayerShowMarkers) (int nPlayerId, unsigned int bToggle);
typedef unsigned int (*SDK_EnabledPlayerShowMarkers) (int nPlayerId);
typedef int (*SDK_TogglePlayerAttackPriv) (int nPlayerId, unsigned int bToggle);
typedef unsigned int (*SDK_EnabledPlayerAttackPriv) (int nPlayerId);
typedef int (*SDK_TogglePlayerHasMarker) (int nPlayerId, unsigned int bToggle);
typedef unsigned int (*SDK_EnabledPlayerHasMarker) (int nPlayerId);
typedef int (*SDK_TogglePlayerChatTagsEnabled) (int nPlayerId, unsigned int bToggle);
typedef unsigned int (*SDK_EnabledPlayerChatTags) (int nPlayerId);
typedef int (*SDK_TogglePlayerDrunkEffects) (int nPlayerId, unsigned int bToggle);
typedef unsigned int (*SDK_EnabledPlayerDrunkEffects) (int nPlayerId);
typedef int (*SDK_GivePlayerWeapon) (int nPlayerId, int nWeaponId, int nAmmo);
typedef int (*SDK_SetPlayerWeapon) (int nPlayerId, int nWeaponId, int nAmmo);
typedef int (*SDK_GetPlayerWeapon) (int nPlayerId);
typedef int (*SDK_GetPlayerWeaponAmmo) (int nPlayerId);
typedef int (*SDK_SetPlayerWeaponSlot) (int nPlayerId, int nSlot);
typedef int (*SDK_GetPlayerWeaponSlot) (int nPlayerId);
typedef int (*SDK_GetPlayerWeaponAtSlot) (int nPlayerId, int nSlot);
typedef int (*SDK_GetPlayerAmmoAtSlot) (int nPlayerId, int nSlot);
typedef int (*SDK_RemovePlayerWeapon) (int nPlayerId, int nWeaponId);
typedef int (*SDK_RemoveAllWeapons) (int nPlayerId);
typedef int (*SDK_SetCameraPosition) (int nPlayerId, float fPosX, float fPosY, float fPosZ, float fLookX, float fLookY, float fLookZ);
typedef int (*SDK_RestoreCamera) (int nPlayerId);
typedef unsigned int (*SDK_IsCameraLocked) (int nPlayerId);
typedef int (*SDK_SetPlayerAnimation) (int nPlayerId, int nGroupId, int nAnimationId);
typedef int (*SDK_SetPlayerWantedLevel) (int nPlayerId, int nLevel);
typedef int (*SDK_GetPlayerStandingOnVehicle) (int nPlayerId);
typedef int (*SDK_GetPlayerStandingOnObject) (int nPlayerId);
typedef unsigned int (*SDK_IsPlayerAway) (int nPlayerId);
typedef int (*SDK_GetPlayerSpectateTarget) (int nPlayerId);
typedef int (*SDK_SetPlayerSpectateTarget) (int nPlayerId, int nTargetId);
typedef unsigned int (*SDK_RedirectPlayerToServer) (int nPlayerId, const char* szIP, unsigned int usPort, const char* szNickname, const char* szServerPass, const char* szUserPass);
typedef int (*SDK_CreateVehicle) (int nModelId, int nWorld, float fPosX, float fPosY, float fPosZ, float fAngleZ, int nColour1, int nColour2);
typedef int (*SDK_DeleteVehicle) (int nVehicleId);
typedef int (*SDK_GetVehicleSyncSource) (int nVehicleId);
typedef int (*SDK_GetVehicleSyncType) (int nVehicleId);
typedef unsigned int (*SDK_IsVehicleStreamedForPlayer) (int nVehicleId, int nPlayerId);
typedef int (*SDK_SetVehicleWorld) (int nVehicleId, int nWorld);
typedef int (*SDK_GetVehicleWorld) (int nVehicleId);
typedef int (*SDK_GetVehicleModel) (int nVehicleId);
typedef int (*SDK_GetVehicleOccupant) (int nVehicleId, int nSlotIndex);
typedef int (*SDK_RespawnVehicle) (int nVehicleId);
typedef int (*SDK_SetVehicleImmunityFlags) (int nVehicleId, int nImmuFlags);
typedef int (*SDK_GetVehicleImmunityFlags) (int nVehicleId);
typedef int (*SDK_KillVehicle) (int nVehicleId);
typedef unsigned int (*SDK_IsVehicleWrecked) (int nVehicleId);
typedef int (*SDK_SetVehiclePos) (int nVehicleId, float fPosX, float fPosY, float fPosZ, unsigned int bRemoveOccupants);
typedef int (*SDK_GetVehiclePos) (int nVehicleId, float* pfPosX, float* pfPosY, float* pfPosZ);
typedef int (*SDK_SetVehicleRot) (int nVehicleId, float fX, float fY, float fZ, float fW);
typedef int (*SDK_SetVehicleRotEuler) (int nVehicleId, float fX, float fY, float fZ);
typedef int (*SDK_GetVehicleRot) (int nVehicleId, float* pfX, float* pfY, float *pfZ, float *pfW);
typedef int (*SDK_GetVehicleRotEuler) (int nVehicleId, float* pfX, float* pfY, float *pfZ);
typedef int (*SDK_SetVehicleSpeed) (int nVehicleId, float fX, float fY, float fZ);
typedef int (*SDK_GetVehicleSpeed) (int nVehicleId, float* pfX, float* pfY, float* pfZ);
typedef int (*SDK_AddVehicleSpeed) (int nVehicleId, float fX, float fY, float fZ);
typedef int (*SDK_SetVehicleRelSpeed) (int nVehicleId, float fX, float fY, float fZ);
typedef int (*SDK_GetVehicleRelSpeed) (int nVehicleId, float* pfX, float* pfY, float* pfZ);
typedef int (*SDK_AddVehicleRelSpeed) (int nVehicleId, float fX, float fY, float fZ);
typedef int (*SDK_SetVehicleTurnSpeed) (int nVehicleId, float fX, float fY, float fZ);
typedef int (*SDK_GetVehicleTurnSpeed) (int nVehicleId, float* pfX, float* pfY, float* pfZ);
typedef int (*SDK_AddVehicleTurnSpeed) (int nVehicleId, float fX, float fY, float fZ);
typedef int (*SDK_SetVehicleRelTurnSpeed) (int nVehicleId, float fX, float fY, float fZ);
typedef int (*SDK_GetVehicleRelTurnSpeed) (int nVehicleId, float* pfX, float* pfY, float* pfZ);
typedef int (*SDK_AddVehicleRelTurnSpeed) (int nVehicleId, float fX, float fY, float fZ);
typedef int (*SDK_SetVehicleSpawnPos) (int nVehicleId, float fPosX, float fPosY, float fPosZ, float fAngleZ);
typedef int (*SDK_GetVehicleSpawnPos) (int nVehicleId, float* pfPosX, float* pfPosY, float* pfPosZ, float* pfAngleZ);
typedef int (*SDK_SetVehicleSpawnRot) (int nVehicleId, float fX, float fY, float fZ, float fW);
typedef int (*SDK_SetVehicleSpawnRotEuler) (int nVehicleId, float fX, float fY, float fZ);
typedef int (*SDK_GetVehicleSpawnRot) (int nVehicleId, float* pfX, float* pfY, float* pfZ, float* pfW);
typedef int (*SDK_GetVehicleSpawnRotEuler) (int nVehicleId, float* pfX, float* pfY, float* pfZ);
typedef int (*SDK_SetVehicleIdleRespawnTimer) (int nVehicleId, unsigned int uTimer);
typedef unsigned int (*SDK_GetVehicleIdleRespawnTimer) (int nVehicleId);
typedef int (*SDK_SetVehicleHealth) (int nVehicleId, float fHealth);
typedef float (*SDK_GetVehicleHealth) (int nVehicleId);
typedef int (*SDK_SetVehicleColour) (int nVehicleId, int nColour1, int nColour2);
typedef int (*SDK_GetVehicleColour) (int nVehicleId, int* pnColour1, int* pnColour2);
typedef int (*SDK_SetVehicleDoorsLocked) (int nVehicleId, unsigned int bToggle);
typedef unsigned int (*SDK_GetVehicleDoorsLocked) (int nVehicleId);
typedef int (*SDK_SetVehiclePartStatus) (int nVehicleId, int nPartId, int nStatus);
typedef int (*SDK_GetVehiclePartStatus) (int nVehicleId, int nPartId);
typedef int (*SDK_SetVehicleTyreStatus) (int nVehicleId, int nTyreId, int nStatus);
typedef int (*SDK_GetVehicleTyreStatus) (int nVehicleId, int nTyreId);
typedef int (*SDK_SetVehicleDamageData) (int nVehicleId, unsigned int uDamageData);
typedef unsigned int (*SDK_GetVehicleDamageData) (int nVehicleId);
typedef int (*SDK_SetVehicleAlarm) (int nVehicleId, unsigned int bToggle);
typedef unsigned int (*SDK_GetVehicleAlarm) (int nVehicleId);
typedef int (*SDK_SetVehicleLights) (int nVehicleId, unsigned int bToggle);
typedef unsigned int (*SDK_GetVehicleLights) (int nVehicleId);
typedef int (*SDK_SetVehicleRadio) (int nVehicleId, int nRadioId);
typedef int (*SDK_GetVehicleRadio) (int nVehicleId);
typedef int (*SDK_SetVehicleRadioLocked) (int nVehicleId, unsigned int bToggle);
typedef unsigned int (*SDK_IsVehicleRadioLocked) (int nVehicleId);
typedef unsigned int (*SDK_GetVehicleGhostState) (int nVehicleId);
typedef int (*SDK_SetVehicleGhostState) (int nVehicleId, unsigned int bToggle);
typedef unsigned int (*SDK_GetVehicleTurretRotation) (int nVehicleId, float* pfH, float* pfV);
typedef int (*SDK_ResetAllVehicleHandlings) (void);
typedef unsigned int (*SDK_ExistsHandlingRule) (int nModelIndex, int nRuleIndex);
typedef int (*SDK_SetHandlingRule) (int nModelIndex, int nRuleIndex, double fValue);
typedef double (*SDK_GetHandlingRule) (int nModelIndex, int nRuleIndex);
typedef int (*SDK_ResetHandlingRule) (int nModelIndex, int nRuleIndex);
typedef int (*SDK_ResetHandling) (int nModelIndex);
typedef unsigned int (*SDK_ExistsInstHandlingRule) (int nVehicleId, int nRuleIndex);
typedef int (*SDK_SetInstHandlingRule) (int nVehicleId, int nRuleIndex, double fValue);
typedef double (*SDK_GetInstHandlingRule) (int nVehicleId, int nRuleIndex);
typedef int (*SDK_ResetInstHandlingRule) (int nVehicleId, int nRuleIndex);
typedef int (*SDK_ResetInstHandling) (int nVehicleId);
typedef int (*SDK_CreatePickup) (int nModel, int nWorld, int nQuantity, float fPosX, float fPosY, float fPosZ, int nAlpha, unsigned int bAutomatic);
typedef int (*SDK_DeletePickup) (int nPickupId);
typedef unsigned int (*SDK_IsPickupStreamedForPlayer) (int nPickupId, int nPlayerId);
typedef int (*SDK_SetPickupWorld) (int nPickupId, int nWorld);
typedef int (*SDK_GetPickupWorld) (int nPickupId);
typedef int (*SDK_PickupGetAlpha) (int nPickupId);
typedef int (*SDK_PickupSetAlpha) (int nPickupId, int nAlpha);
typedef unsigned int (*SDK_PickupIsAutomatic) (int nPickupId);
typedef int (*SDK_PickupSetAutomatic) (int nPickupId, unsigned int bToggle);
typedef int (*SDK_SetPickupAutoTimer) (int nPickupId, int nTimer);
typedef int (*SDK_GetPickupAutoTimer) (int nPickupId);
typedef int (*SDK_PickupRefresh) (int nPickupId);
typedef int (*SDK_PickupGetPos) (int nPickupId, float* pfPosX, float* pfPosY, float* pfPosZ);
typedef int (*SDK_PickupSetPos) (int nPickupId, float fPosX, float fPosY, float fPosZ);
typedef int (*SDK_PickupGetModel) (int nPickupId);
typedef int (*SDK_PickupGetQuantity) (int nPickupId);
typedef int (*SDK_CreateObject) (int nModelId, int nWorld, float fPosX, float fPosY, float fPosZ, int nAlpha);
typedef int (*SDK_DeleteObject) (int nObjectId);
typedef unsigned int (*SDK_IsObjectStreamedForPlayer) (int nObjectId, int nPlayerId);
typedef int (*SDK_GetObjectModel) (int nObjectId);
typedef int (*SDK_SetObjectWorld) (int nObjectId, int nWorld);
typedef int (*SDK_GetObjectWorld) (int nObjectId);
typedef int (*SDK_SetObjectAlpha) (int nObjectId, int nAlpha, int nTime);
typedef int (*SDK_GetObjectAlpha) (int nObjectId);
typedef int (*SDK_MoveObjectTo) (int nObjectId, float fX, float fY, float fZ, int nTime);
typedef int (*SDK_MoveObjectBy) (int nObjectId, float fX, float fY, float fZ, int nTime);
typedef int (*SDK_SetObjectPos) (int nObjectId, float fX, float fY, float fZ);
typedef int (*SDK_GetObjectPos) (int nObjectId, float* pfPosX, float* pfPosY, float* pfPosZ);
typedef int (*SDK_RotObjectTo) (int nObjectId, float fX, float fY, float fZ, float fW, int nTime);
typedef int (*SDK_RotObjectToEuler) (int nObjectId, float fX, float fY, float fZ, int nTime);
typedef int (*SDK_RotObjectBy) (int nObjectId, float fX, float fY, float fZ, float fW, int nTime);
typedef int (*SDK_RotObjectByEuler) (int nObjectId, float fX, float fY, float fZ, int nTime);
typedef int (*SDK_GetObjectRot) (int nObjectId, float* pfX, float* pfY, float *pfZ, float *pfW);
typedef int (*SDK_GetObjectRotEuler) (int nObjectId, float* pfX, float* pfY, float *pfZ);
typedef int (*SDK_SetObjectShotReport) (int nObjectId, unsigned int bToggle);
typedef unsigned int (*SDK_IsObjectShotReport) (int nObjectId);
typedef int (*SDK_SetObjectBumpReport) (int nObjectId, unsigned int bToggle);
typedef unsigned int (*SDK_IsObjectBumpReport) (int nObjectId);
typedef int (*SDK_ToggleWallglitch) (unsigned int bToggle);
typedef unsigned int (*SDK_EnabledWallglitch) (void);
typedef int (*SDK_SetVehicleSiren) (int nVehicleId, unsigned int bToggle);
typedef unsigned int (*SDK_GetVehicleSiren) (int nVehicleId);
typedef int (*SDK_GetPlayerUID2) (int nPlayerId, char* szBuffer, int nBufferLen);
typedef int (*SDK_CreateCheckpoint) (int nPlayerId, int nWorld, float fPosX, float fPosY, float fPosZ, unsigned int nR, unsigned int nG, unsigned int nB, unsigned int nA, float fRadius);
typedef int (*SDK_DeleteCheckpoint) (int nCheckpointId);
typedef unsigned int (*SDK_IsCheckpointStreamedForPlayer) (int nCheckpointId, int nPlayerId);
typedef int (*SDK_SetCheckpointWorld) (int nCheckpointId, int nWorld);
typedef int (*SDK_GetCheckpointWorld) (int nCheckpointId);
typedef int (*SDK_SetCheckpointColor) (int nCheckpointId, unsigned int nR, unsigned int nG, unsigned int nB, unsigned int nA);
typedef unsigned int (*SDK_GetCheckpointColor) (int nCheckpointId, unsigned int* pnR, unsigned int* pnG, unsigned int* pnB, unsigned int* pnA);
typedef int (*SDK_SetCheckpointPos) (int nCheckpointId, float fPosX, float fPosY, float fPosZ);
typedef int (*SDK_GetCheckpointPos) (int nCheckpointId, float* pfPosX, float* pfPosY, float* pfPosZ);
typedef int (*SDK_SetCheckpointRadius) (int nCheckpointId, float fRadius);
typedef float (*SDK_GetCheckpointRadius) (int nCheckpointId);
typedef int (*SDK_GetCheckpointOwner) (int nCheckpointId);
typedef int (*SDK_CreateSphere) (int nPlayerId, int nWorld, float fPosX, float fPosY, float fPosZ, unsigned int nR, unsigned int nG, unsigned int nB, float fRadius);
typedef int (*SDK_DeleteSphere) (int nSphereId);
typedef unsigned int (*SDK_IsSphereStreamedForPlayer) (int nSphereId, int nPlayerId);
typedef int (*SDK_SetSphereWorld) (int nSphereId, int nWorld);
typedef int (*SDK_GetSphereWorld) (int nSphereId);
typedef int (*SDK_SetSphereColor) (int nSphereId, unsigned int nR, unsigned int nG, unsigned int nB);
typedef unsigned int (*SDK_GetSphereColor) (int nSphereId, unsigned int* pnR, unsigned int* pnG, unsigned int* pnB);
typedef int (*SDK_SetSpherePos) (int nSphereId, float fPosX, float fPosY, float fPosZ);
typedef int (*SDK_GetSpherePos) (int nSphereId, float* pfPosX, float* pfPosY, float* pfPosZ);
typedef int (*SDK_SetSphereRadius) (int nSphereId, float fRadius);
typedef float (*SDK_GetSphereRadius) (int nSphereId);
typedef int (*SDK_GetSphereOwner) (int nSphereId);
typedef int (*SDK_OnInitServer) (void);
typedef void (*SDK_OnShutdownServer) (void);
typedef void (*SDK_OnFrame) (float fElapsedTime);
typedef void (*SDK_OnPlayerConnect) (int nPlayerId);
typedef void (*SDK_OnPlayerDisconnect) (int nPlayerId, int nReason);
typedef void (*SDK_OnPlayerBeginTyping) (int nPlayerId);
typedef void (*SDK_OnPlayerEndTyping) (int nPlayerId);
typedef int (*SDK_OnPlayerRequestClass) (int nPlayerId, int nOffset);
typedef int (*SDK_OnPlayerRequestSpawn) (int nPlayerId);
typedef void (*SDK_OnPlayerSpawn) (int nPlayerId);
typedef void (*SDK_OnPlayerDeath) (int nPlayerId, int nKillerId, int nReason, int nBodyPart);
typedef void (*SDK_OnPlayerUpdate) (int nPlayerId, int nUpdateType);
typedef int (*SDK_OnPlayerRequestEnter) (int nPlayerId, int nVehicleId, int nSlotId);
typedef void (*SDK_OnPlayerEnterVehicle) (int nPlayerId, int nVehicleId, int nSlotId);
typedef void (*SDK_OnPlayerExitVehicle) (int nPlayerId, int nVehicleId);
typedef void (*SDK_OnPlayerNameChange) (int nPlayerId, const char* pszOldName, const char* pszNewName);
typedef void (*SDK_OnPlayerStateChange) (int nPlayerId, int nOldState, int nNewState);
typedef void (*SDK_OnPlayerActionChange) (int nPlayerId, int nOldAction, int nNewAction);
typedef void (*SDK_OnPlayerOnFireChange) (int nPlayerId, unsigned int bIsOnFireNow);
typedef void (*SDK_OnPlayerCrouchChange) (int nPlayerId, unsigned int bIsCrouchingNow);
typedef void (*SDK_OnPlayerGameKeysChange) (int nPlayerId, int nOldKeys, int nNewKeys);
typedef int (*SDK_OnPickupClaimPicked) (int nPickupId, int nPlayerId);
typedef void (*SDK_OnPickupPickedUp) (int nPickupId, int nPlayerId);
typedef void (*SDK_OnPickupRespawn) (int nPickupId);
typedef void (*SDK_OnVehicleUpdate) (int nVehicleId, int nUpdateType);
typedef void (*SDK_OnVehicleExplode) (int nVehicleId);
typedef void (*SDK_OnVehicleRespawn) (int nVehicleId);
typedef void (*SDK_OnObjectShot) (int nObjectId, int nPlayerId, int nWeapon);
typedef void (*SDK_OnObjectBump) (int nObjectId, int nPlayerId);
typedef int (*SDK_OnPublicMessage) (int nPlayerId, const char* pszText);
typedef int (*SDK_OnCommandMessage) (int nPlayerId, const char* pszText);
typedef int (*SDK_OnPrivateMessage) (int nPlayerId, int nTargetId, const char* pszText);
typedef int (*SDK_OnInternalCommand) (unsigned int uCmdType, const char* pszText);
typedef int (*SDK_OnLoginAttempt) (char* pszPlayerName, const char* pszUserPassword, const char* pszIpAddress);
typedef void (*SDK_OnEntityPoolChange) (int nEntityType, int nEntityId, unsigned int bDeleted);
typedef void (*SDK_OnKeyBindDown) (int nPlayerId, int nBindId);
typedef void (*SDK_OnKeyBindUp) (int nPlayerId, int nBindId);
typedef void (*SDK_OnPlayerAwayChange) (int nPlayerId, unsigned int bNewStatus);
typedef void (*SDK_OnPlayerSpectate) (int nPlayerId, int nTargetId);
typedef void (*SDK_OnPlayerCrashReport) (int nPlayerId, const char* pszReport);
typedef void (*SDK_OnServerPerformanceReport) (int nNumStats, const char** ppszDescription, unsigned long long* pnMillisecsSpent);
typedef void (*SDK_OnCheckpointEntered) (int nCheckpointId, int nPlayerId);
typedef void (*SDK_OnCheckpointExited) (int nCheckpointId, int nPlayerId);
typedef void (*SDK_OnSphereEntered) (int nSphereId, int nPlayerId);
typedef void (*SDK_OnSphereExited) (int nSphereId, int nPlayerId);

typedef struct {
    unsigned int                        uStructSize;

    //PLUGIN SYSTEM
    SDK_GetServerVersion GetServerVersion;
    SDK_GetServerSettings GetServerSettings;
    SDK_ExportFunctions ExportFunctions;
    SDK_GetNumberOfPlugins GetNumberOfPlugins;
    SDK_GetPluginInfo GetPluginInfo;
    SDK_FindPlugin FindPlugin;
    SDK_GetPluginExports GetPluginExports;
    SDK_GetTime GetTime;

    //MESSAGES
    SDK_printf printf;
    SDK_SendCustomCommand SendCustomCommand;
    SDK_SendClientMessage SendClientMessage;
    SDK_SendGameMessage SendGameMessage;

    //SERVER SETTINGS
    SDK_SetServerName SetServerName;
    SDK_GetServerName GetServerName;
    SDK_SetMaxPlayers SetMaxPlayers;
    SDK_GetMaxPlayers GetMaxPlayers;
    SDK_SetServerPassword SetServerPassword;
    SDK_GetServerPassword GetServerPassword;
    SDK_SetGameModeText SetGameModeText;
    SDK_GetGameModeText GetGameModeText;
    SDK_ShutdownServer ShutdownServer;

    //WORLD: settings
    SDK_SetWorldBounds SetWorldBounds;
    SDK_GetWorldBounds GetWorldBounds;
    SDK_SetWastedSettings SetWastedSettings;
    SDK_GetWastedSettings GetWastedSettings;
    SDK_SetTimeRate SetTimeRate;
    SDK_GetTimeRate GetTimeRate;
    SDK_SetHour SetHour;
    SDK_GetHour GetHour;
    SDK_SetMinute SetMinute;
    SDK_GetMinute GetMinute;
    SDK_SetWeather SetWeather;
    SDK_GetWeather GetWeather;
    SDK_SetGravity SetGravity;
    SDK_GetGravity GetGravity;
    SDK_SetGamespeed SetGamespeed;
    SDK_GetGamespeed GetGamespeed;
    SDK_SetWaterLevel SetWaterLevel;
    SDK_GetWaterLevel GetWaterLevel;
    SDK_SetMaxHeight SetMaxHeight;
    SDK_GetMaxHeight GetMaxHeight;
    SDK_SetKillCmdDelay SetKillCmdDelay;
    SDK_GetKillCmdDelay GetKillCmdDelay;
    SDK_SetVehiclesForcedRespawnHeight SetVehiclesForcedRespawnHeight;
    SDK_GetVehiclesForcedRespawnHeight GetVehiclesForcedRespawnHeight;

    //WORLD: toggles
    SDK_ToggleSyncFrameLimiter ToggleSyncFrameLimiter;
    SDK_EnabledSyncFrameLimiter EnabledSyncFrameLimiter;
    SDK_ToggleFrameLimiter ToggleFrameLimiter;
    SDK_EnabledFrameLimiter EnabledFrameLimiter;
    SDK_ToggleTaxiBoostJump ToggleTaxiBoostJump;
    SDK_EnabledTaxiBoostJump EnabledTaxiBoostJump;
    SDK_ToggleDriveOnWater ToggleDriveOnWater;
    SDK_EnabledDriveOnWater EnabledDriveOnWater;
    SDK_ToggleFastSwitch ToggleFastSwitch;
    SDK_EnabledFastSwitch EnabledFastSwitch;
    SDK_ToggleFriendlyFire ToggleFriendlyFire;
    SDK_EnabledFriendlyFire EnabledFriendlyFire;
    SDK_ToggleDisableDriveby ToggleDisableDriveby;
    SDK_EnabledDisableDriveby EnabledDisableDriveby;
    SDK_TogglePerfectHandling TogglePerfectHandling;
    SDK_EnabledPerfectHandling EnabledPerfectHandling;
    SDK_ToggleFlyingCars ToggleFlyingCars;
    SDK_EnabledFlyingCars EnabledFlyingCars;
    SDK_ToggleJumpSwitch ToggleJumpSwitch;
    SDK_EnabledJumpSwitch EnabledJumpSwitch;
    SDK_ToggleShowMarkers ToggleShowMarkers;
    SDK_EnabledShowMarkers EnabledShowMarkers;
    SDK_ToggleOnlyShowTeamMarkers ToggleOnlyShowTeamMarkers;
    SDK_EnabledOnlyShowTeamMarkers EnabledOnlyShowTeamMarkers;
    SDK_ToggleStuntBike ToggleStuntBike;
    SDK_EnabledStuntBike EnabledStuntBike;
    SDK_ToggleShootInAir ToggleShootInAir;
    SDK_EnabledShootInAir EnabledShootInAir;
    SDK_ToggleShowNametags ToggleShowNametags;
    SDK_EnabledShowNametags EnabledShowNametags;
    SDK_ToggleJoinMessages ToggleJoinMessages;
    SDK_EnabledJoinMessages EnabledJoinMessages;
    SDK_ToggleDeathMessages ToggleDeathMessages;
    SDK_EnabledDeathMessages EnabledDeathMessages;
    SDK_ToggleChatTagsByDefaultEnabled ToggleChatTagsByDefaultEnabled;
    SDK_EnabledChatTagsByDefault EnabledChatTagsByDefault;

    //MISC
    SDK_CreateExplosion CreateExplosion;
    SDK_PlaySound PlaySound;
    SDK_HideMapObject HideMapObject;
    SDK_ShowMapObject ShowMapObject;
    SDK_ShowAllMapObjects ShowAllMapObjects;

    //WEAPONDATA
    SDK_SetWeaponDataValue SetWeaponDataValue;
    SDK_GetWeaponDataValue GetWeaponDataValue;
    SDK_ResetWeaponDataValue ResetWeaponDataValue;
    SDK_IsWeaponDataValueModified IsWeaponDataValueModified;
    SDK_ResetWeaponData ResetWeaponData;
    SDK_ResetAllWeaponData ResetAllWeaponData;

    //KEYBINDS
    SDK_GetKeyBindUnusedSlot GetKeyBindUnusedSlot;
    SDK_GetKeyBindData GetKeyBindData;
    SDK_RegisterKeyBind RegisterKeyBind;
    SDK_RemoveKeyBind RemoveKeyBind;
    SDK_RemoveAllKeyBinds RemoveAllKeyBinds;

    //BLIPS
    SDK_CreateCoordBlip CreateCoordBlip;
    SDK_DestroyCoordBlip DestroyCoordBlip;
    SDK_GetCoordBlipInfo GetCoordBlipInfo;

    //SPRITES
    SDK_CreateSprite CreateSprite;
    SDK_DestroySprite DestroySprite;
    SDK_ShowSprite ShowSprite;
    SDK_HideSprite HideSprite;
    SDK_MoveSprite MoveSprite;
    SDK_SetSpriteCenter SetSpriteCenter;
    SDK_RotateSprite RotateSprite;
    SDK_SetSpriteAlpha SetSpriteAlpha;
    SDK_SetSpriteRelativity SetSpriteRelativity;

    //TEXTDRAWS
    SDK_CreateTextdraw CreateTextdraw;
    SDK_DestroyTextdraw DestroyTextdraw;
    SDK_ShowTextdraw ShowTextdraw;
    SDK_HideTextdraw HideTextdraw;
    SDK_MoveTextdraw MoveTextdraw;
    SDK_SetTextdrawColour SetTextdrawColour;
    SDK_SetTextdrawRelativity SetTextdrawRelativity;

    //RADIOS
    SDK_AddRadioStream AddRadioStream;
    SDK_RemoveRadioStream RemoveRadioStream;

    //CLASSES
    SDK_SetUseClasses SetUseClasses;
    SDK_GetUseClasses GetUseClasses;
    SDK_GetPlayerClass GetPlayerClass;
    SDK_AddPlayerClass AddPlayerClass;
    SDK_SetSpawnPlayerPos SetSpawnPlayerPos;
    SDK_SetSpawnCameraPos SetSpawnCameraPos;
    SDK_SetSpawnCameraLookAt SetSpawnCameraLookAt;

    //ADMIN
    SDK_IsPlayerAdmin IsPlayerAdmin;
    SDK_SetPlayerAdmin SetPlayerAdmin;
    SDK_GetPlayerIP GetPlayerIP;
    SDK_KickPlayer KickPlayer;
    SDK_BanPlayer BanPlayer;
    SDK_BanIP BanIP;
    SDK_UnbanIP UnbanIP;
    SDK_IsIPBanned IsIPBanned;

    //PLAYERS: basic
    SDK_GetPlayerIDFromName GetPlayerIDFromName;
    SDK_IsPlayerConnected IsPlayerConnected;
    SDK_IsPlayerSpawned IsPlayerSpawned;
    SDK_IsPlayerStreamedForPlayer IsPlayerStreamedForPlayer;
    SDK_GetPlayerKey GetPlayerKey;
    SDK_SetPlayerWorld SetPlayerWorld;
    SDK_GetPlayerWorld GetPlayerWorld;
    SDK_SetPlayerSecWorld SetPlayerSecWorld;
    SDK_GetPlayerSecWorld GetPlayerSecWorld;
    SDK_GetPlayerUniqueWorld GetPlayerUniqueWorld;
    SDK_IsPlayerWorldCompatible IsPlayerWorldCompatible;
    SDK_GetPlayerState GetPlayerState;
    SDK_GetPlayerName GetPlayerName;
    SDK_SetPlayerName SetPlayerName;
    SDK_SetPlayerTeam SetPlayerTeam;
    SDK_GetPlayerTeam GetPlayerTeam;
    SDK_SetPlayerSkin SetPlayerSkin;
    SDK_GetPlayerSkin GetPlayerSkin;
    SDK_SetPlayerColour SetPlayerColour;
    SDK_GetPlayerColour GetPlayerColour;
    SDK_ForcePlayerSpawn ForcePlayerSpawn;
    SDK_ForcePlayerSelect ForcePlayerSelect;
    SDK_ForceAllSelect ForceAllSelect;

    //PLAYERS: score, ping, money, typing
    SDK_GivePlayerMoney GivePlayerMoney;
    SDK_SetPlayerMoney SetPlayerMoney;
    SDK_GetPlayerMoney GetPlayerMoney;
    SDK_SetPlayerScore SetPlayerScore;
    SDK_GetPlayerScore GetPlayerScore;
    SDK_GetPlayerPing GetPlayerPing;
    SDK_IsPlayerTyping IsPlayerTyping;
    SDK_GetPlayerFPS GetPlayerFPS;
    SDK_GetPlayerUID GetPlayerUID;
    SDK_GetPlayerWantedLevel GetPlayerWantedLevel;

    //PLAYERS: health and location
    SDK_SetPlayerHealth SetPlayerHealth;
    SDK_GetPlayerHealth GetPlayerHealth;
    SDK_SetPlayerArmour SetPlayerArmour;
    SDK_GetPlayerArmour GetPlayerArmour;
    SDK_SetPlayerImmunityFlags SetPlayerImmunityFlags;
    SDK_GetPlayerImmunityFlags GetPlayerImmunityFlags;
    SDK_SetPlayerPos SetPlayerPos;
    SDK_GetPlayerPos GetPlayerPos;
    SDK_SetPlayerSpeed SetPlayerSpeed;
    SDK_GetPlayerSpeed GetPlayerSpeed;
    SDK_AddPlayerSpeed AddPlayerSpeed;
    SDK_SetPlayerHeading SetPlayerHeading;
    SDK_GetPlayerHeading GetPlayerHeading;
    SDK_SetPlayerAlpha SetPlayerAlpha;
    SDK_GetPlayerAlpha GetPlayerAlpha;
    SDK_GetPlayerOnFireStatus GetPlayerOnFireStatus;
    SDK_GetPlayerCrouchStatus GetPlayerCrouchStatus;
    SDK_GetPlayerAction GetPlayerAction;
    SDK_GetPlayerGameKeys GetPlayerGameKeys;
    SDK_GetPlayerAimPos GetPlayerAimPos;
    SDK_GetPlayerAimDir GetPlayerAimDir;

    //PLAYERS: vehicle
    SDK_PutPlayerInVehicle PutPlayerInVehicle;
    SDK_RemovePlayerFromVehicle RemovePlayerFromVehicle;
    SDK_GetPlayerInVehicleStatus GetPlayerInVehicleStatus;
    SDK_GetPlayerInVehicleSlot GetPlayerInVehicleSlot;
    SDK_GetPlayerVehicleID GetPlayerVehicleID;

    //PLAYERS: toggles
    SDK_TogglePlayerControllable TogglePlayerControllable;
    SDK_EnabledPlayerControllable EnabledPlayerControllable;
    SDK_TogglePlayerDriveby TogglePlayerDriveby;
    SDK_EnabledPlayerDriveby EnabledPlayerDriveby;
    SDK_TogglePlayerWhiteScanlines TogglePlayerWhiteScanlines;
    SDK_EnabledPlayerWhiteScanlines EnabledPlayerWhiteScanlines;
    SDK_TogglePlayerGreenScanlines TogglePlayerGreenScanlines;
    SDK_EnabledPlayerGreenScanlines EnabledPlayerGreenScanlines;
    SDK_TogglePlayerWidescreen TogglePlayerWidescreen;
    SDK_EnabledPlayerWidescreen EnabledPlayerWidescreen;
    SDK_TogglePlayerShowMarkers TogglePlayerShowMarkers;
    SDK_EnabledPlayerShowMarkers EnabledPlayerShowMarkers;
    SDK_TogglePlayerAttackPriv TogglePlayerAttackPriv;
    SDK_EnabledPlayerAttackPriv EnabledPlayerAttackPriv;
    SDK_TogglePlayerHasMarker TogglePlayerHasMarker;
    SDK_EnabledPlayerHasMarker EnabledPlayerHasMarker;
    SDK_TogglePlayerChatTagsEnabled TogglePlayerChatTagsEnabled;
    SDK_EnabledPlayerChatTags EnabledPlayerChatTags;
    SDK_TogglePlayerDrunkEffects TogglePlayerDrunkEffects;
    SDK_EnabledPlayerDrunkEffects EnabledPlayerDrunkEffects;

    //PLAYERS: weapons
    SDK_GivePlayerWeapon GivePlayerWeapon;
    SDK_SetPlayerWeapon SetPlayerWeapon;
    SDK_GetPlayerWeapon GetPlayerWeapon;
    SDK_GetPlayerWeaponAmmo GetPlayerWeaponAmmo;
    SDK_SetPlayerWeaponSlot SetPlayerWeaponSlot;
    SDK_GetPlayerWeaponSlot GetPlayerWeaponSlot;
    SDK_GetPlayerWeaponAtSlot GetPlayerWeaponAtSlot;
    SDK_GetPlayerAmmoAtSlot GetPlayerAmmoAtSlot;
    SDK_RemovePlayerWeapon RemovePlayerWeapon;
    SDK_RemoveAllWeapons RemoveAllWeapons;

    //PLAYERS: camera
    SDK_SetCameraPosition SetCameraPosition;
    SDK_RestoreCamera RestoreCamera;
    SDK_IsCameraLocked IsCameraLocked;

    //PLAYERS: misc
    SDK_SetPlayerAnimation SetPlayerAnimation;
    SDK_SetPlayerWantedLevel SetPlayerWantedLevel;
    SDK_GetPlayerStandingOnVehicle GetPlayerStandingOnVehicle;
    SDK_GetPlayerStandingOnObject GetPlayerStandingOnObject;
    SDK_IsPlayerAway IsPlayerAway;
    SDK_GetPlayerSpectateTarget GetPlayerSpectateTarget;
    SDK_SetPlayerSpectateTarget SetPlayerSpectateTarget;
    SDK_RedirectPlayerToServer RedirectPlayerToServer;

    //VEHICLES
    SDK_CreateVehicle CreateVehicle;
    SDK_DeleteVehicle DeleteVehicle;
    SDK_GetVehicleSyncSource GetVehicleSyncSource;
    SDK_GetVehicleSyncType GetVehicleSyncType;
    SDK_IsVehicleStreamedForPlayer IsVehicleStreamedForPlayer;
    SDK_SetVehicleWorld SetVehicleWorld;
    SDK_GetVehicleWorld GetVehicleWorld;
    SDK_GetVehicleModel GetVehicleModel;
    SDK_GetVehicleOccupant GetVehicleOccupant;
    SDK_RespawnVehicle RespawnVehicle;
    SDK_SetVehicleImmunityFlags SetVehicleImmunityFlags;
    SDK_GetVehicleImmunityFlags GetVehicleImmunityFlags;
    SDK_KillVehicle KillVehicle;
    SDK_IsVehicleWrecked IsVehicleWrecked;
    SDK_SetVehiclePos SetVehiclePos;
    SDK_GetVehiclePos GetVehiclePos;
    SDK_SetVehicleRot SetVehicleRot;
    SDK_SetVehicleRotEuler SetVehicleRotEuler;
    SDK_GetVehicleRot GetVehicleRot;
    SDK_GetVehicleRotEuler GetVehicleRotEuler;
    SDK_SetVehicleSpeed SetVehicleSpeed;
    SDK_GetVehicleSpeed GetVehicleSpeed;
    SDK_AddVehicleSpeed AddVehicleSpeed;
    SDK_SetVehicleRelSpeed SetVehicleRelSpeed;
    SDK_GetVehicleRelSpeed GetVehicleRelSpeed;
    SDK_AddVehicleRelSpeed AddVehicleRelSpeed;
    SDK_SetVehicleTurnSpeed SetVehicleTurnSpeed;
    SDK_GetVehicleTurnSpeed GetVehicleTurnSpeed;
    SDK_AddVehicleTurnSpeed AddVehicleTurnSpeed;
    SDK_SetVehicleRelTurnSpeed SetVehicleRelTurnSpeed;
    SDK_GetVehicleRelTurnSpeed GetVehicleRelTurnSpeed;
    SDK_AddVehicleRelTurnSpeed AddVehicleRelTurnSpeed;
    SDK_SetVehicleSpawnPos SetVehicleSpawnPos;
    SDK_GetVehicleSpawnPos GetVehicleSpawnPos;
    SDK_SetVehicleSpawnRot SetVehicleSpawnRot;
    SDK_SetVehicleSpawnRotEuler SetVehicleSpawnRotEuler;
    SDK_GetVehicleSpawnRot GetVehicleSpawnRot;
    SDK_GetVehicleSpawnRotEuler GetVehicleSpawnRotEuler;
    SDK_SetVehicleIdleRespawnTimer SetVehicleIdleRespawnTimer;
    SDK_GetVehicleIdleRespawnTimer GetVehicleIdleRespawnTimer;
    SDK_SetVehicleHealth SetVehicleHealth;
    SDK_GetVehicleHealth GetVehicleHealth;
    SDK_SetVehicleColour SetVehicleColour;
    SDK_GetVehicleColour GetVehicleColour;
    SDK_SetVehicleDoorsLocked SetVehicleDoorsLocked;
    SDK_GetVehicleDoorsLocked GetVehicleDoorsLocked;
    SDK_SetVehiclePartStatus SetVehiclePartStatus;
    SDK_GetVehiclePartStatus GetVehiclePartStatus;
    SDK_SetVehicleTyreStatus SetVehicleTyreStatus;
    SDK_GetVehicleTyreStatus GetVehicleTyreStatus;
    SDK_SetVehicleDamageData SetVehicleDamageData;
    SDK_GetVehicleDamageData GetVehicleDamageData;
    SDK_SetVehicleAlarm SetVehicleAlarm;
    SDK_GetVehicleAlarm GetVehicleAlarm;
    SDK_SetVehicleLights SetVehicleLights;
    SDK_GetVehicleLights GetVehicleLights;
    SDK_SetVehicleRadio SetVehicleRadio;
    SDK_GetVehicleRadio GetVehicleRadio;
    SDK_SetVehicleRadioLocked SetVehicleRadioLocked;
    SDK_IsVehicleRadioLocked IsVehicleRadioLocked;
    SDK_GetVehicleGhostState GetVehicleGhostState;
    SDK_SetVehicleGhostState SetVehicleGhostState;
    SDK_GetVehicleTurretRotation GetVehicleTurretRotation;

    //VEHICLES: handling
    SDK_ResetAllVehicleHandlings ResetAllVehicleHandlings;
    SDK_ExistsHandlingRule ExistsHandlingRule;
    SDK_SetHandlingRule SetHandlingRule;
    SDK_GetHandlingRule GetHandlingRule;
    SDK_ResetHandlingRule ResetHandlingRule;
    SDK_ResetHandling ResetHandling;
    SDK_ExistsInstHandlingRule ExistsInstHandlingRule;
    SDK_SetInstHandlingRule SetInstHandlingRule;
    SDK_GetInstHandlingRule GetInstHandlingRule;
    SDK_ResetInstHandlingRule ResetInstHandlingRule;
    SDK_ResetInstHandling ResetInstHandling;

    //PICKUPS
    SDK_CreatePickup CreatePickup;
    SDK_DeletePickup DeletePickup;
    SDK_IsPickupStreamedForPlayer IsPickupStreamedForPlayer;
    SDK_SetPickupWorld SetPickupWorld;
    SDK_GetPickupWorld GetPickupWorld;
    SDK_PickupGetAlpha PickupGetAlpha;
    SDK_PickupSetAlpha PickupSetAlpha;
    SDK_PickupIsAutomatic PickupIsAutomatic;
    SDK_PickupSetAutomatic PickupSetAutomatic;
    SDK_SetPickupAutoTimer SetPickupAutoTimer;
    SDK_GetPickupAutoTimer GetPickupAutoTimer;
    SDK_PickupRefresh PickupRefresh;
    SDK_PickupGetPos PickupGetPos;
    SDK_PickupSetPos PickupSetPos;
    SDK_PickupGetModel PickupGetModel;
    SDK_PickupGetQuantity PickupGetQuantity;

    //OBJECTS
    SDK_CreateObject CreateObject;
    SDK_DeleteObject DeleteObject;
    SDK_IsObjectStreamedForPlayer IsObjectStreamedForPlayer;
    SDK_GetObjectModel GetObjectModel;
    SDK_SetObjectWorld SetObjectWorld;
    SDK_GetObjectWorld GetObjectWorld;
    SDK_SetObjectAlpha SetObjectAlpha;
    SDK_GetObjectAlpha GetObjectAlpha;
    SDK_MoveObjectTo MoveObjectTo;
    SDK_MoveObjectBy MoveObjectBy;
    SDK_SetObjectPos SetObjectPos;
    SDK_GetObjectPos GetObjectPos;
    SDK_RotObjectTo RotObjectTo;
    SDK_RotObjectToEuler RotObjectToEuler;
    SDK_RotObjectBy RotObjectBy;
    SDK_RotObjectByEuler RotObjectByEuler;
    SDK_GetObjectRot GetObjectRot;
    SDK_GetObjectRotEuler GetObjectRotEuler;
    SDK_SetObjectShotReport SetObjectShotReport;
    SDK_IsObjectShotReport IsObjectShotReport;
    SDK_SetObjectBumpReport SetObjectBumpReport;
    SDK_IsObjectBumpReport IsObjectBumpReport;

    // TODO: Move these functions to proper sections on major plugin update
    SDK_ToggleWallglitch ToggleWallglitch;
    SDK_EnabledWallglitch EnabledWallglitch;
    SDK_SetVehicleSiren SetVehicleSiren;
    SDK_GetVehicleSiren GetVehicleSiren;
    SDK_GetPlayerUID2 GetPlayerUID2;

    SDK_CreateCheckpoint CreateCheckpoint;
    SDK_DeleteCheckpoint DeleteCheckpoint;
    SDK_IsCheckpointStreamedForPlayer IsCheckpointStreamedForPlayer;
    SDK_SetCheckpointWorld SetCheckpointWorld;
    SDK_GetCheckpointWorld GetCheckpointWorld;
    SDK_SetCheckpointColor SetCheckpointColor;
    SDK_GetCheckpointColor GetCheckpointColor;
    SDK_SetCheckpointPos SetCheckpointPos;
    SDK_GetCheckpointPos GetCheckpointPos;
    SDK_SetCheckpointRadius SetCheckpointRadius;
    SDK_GetCheckpointRadius GetCheckpointRadius;
    SDK_GetCheckpointOwner GetCheckpointOwner;

    SDK_CreateSphere CreateSphere;
    SDK_DeleteSphere DeleteSphere;
    SDK_IsSphereStreamedForPlayer IsSphereStreamedForPlayer;
    SDK_SetSphereWorld SetSphereWorld;
    SDK_GetSphereWorld GetSphereWorld;
    SDK_SetSphereColor SetSphereColor;
    SDK_GetSphereColor GetSphereColor;
    SDK_SetSpherePos SetSpherePos;
    SDK_GetSpherePos GetSpherePos;
    SDK_SetSphereRadius SetSphereRadius;
    SDK_GetSphereRadius GetSphereRadius;
    SDK_GetSphereOwner GetSphereOwner;
} PluginFuncs;

typedef struct {
    unsigned int                uStructSize;

    SDK_OnInitServer OnInitServer;
    SDK_OnShutdownServer OnShutdownServer;
    SDK_OnFrame OnFrame;
    SDK_OnPlayerConnect OnPlayerConnect;
    SDK_OnPlayerDisconnect OnPlayerDisconnect;
    SDK_OnPlayerBeginTyping OnPlayerBeginTyping;
    SDK_OnPlayerEndTyping OnPlayerEndTyping;
    SDK_OnPlayerRequestClass OnPlayerRequestClass;
    SDK_OnPlayerRequestSpawn OnPlayerRequestSpawn;
    SDK_OnPlayerSpawn OnPlayerSpawn;
    SDK_OnPlayerDeath OnPlayerDeath;
    SDK_OnPlayerUpdate OnPlayerUpdate;
    SDK_OnPlayerRequestEnter OnPlayerRequestEnter;
    SDK_OnPlayerEnterVehicle OnPlayerEnterVehicle;
    SDK_OnPlayerExitVehicle OnPlayerExitVehicle;
    SDK_OnPlayerNameChange OnPlayerNameChange;
    SDK_OnPlayerStateChange OnPlayerStateChange;
    SDK_OnPlayerActionChange OnPlayerActionChange;
    SDK_OnPlayerOnFireChange OnPlayerOnFireChange;
    SDK_OnPlayerCrouchChange OnPlayerCrouchChange;
    SDK_OnPlayerGameKeysChange OnPlayerGameKeysChange;
    SDK_OnPickupClaimPicked OnPickupClaimPicked;
    SDK_OnPickupPickedUp OnPickupPickedUp;
    SDK_OnPickupRespawn OnPickupRespawn;
    SDK_OnVehicleUpdate OnVehicleUpdate;
    SDK_OnVehicleExplode OnVehicleExplode;
    SDK_OnVehicleRespawn OnVehicleRespawn;
    SDK_OnObjectShot OnObjectShot;
    SDK_OnObjectBump OnObjectBump;
    SDK_OnPublicMessage OnPublicMessage;
    SDK_OnCommandMessage OnCommandMessage;
    SDK_OnPrivateMessage OnPrivateMessage;
    SDK_OnInternalCommand OnInternalCommand;
    SDK_OnLoginAttempt OnLoginAttempt;
    SDK_OnEntityPoolChange OnEntityPoolChange;
    SDK_OnKeyBindDown OnKeyBindDown;
    SDK_OnKeyBindUp OnKeyBindUp;
    SDK_OnPlayerAwayChange OnPlayerAwayChange;
    SDK_OnPlayerSpectate OnPlayerSpectate;
    SDK_OnPlayerCrashReport OnPlayerCrashReport;
    SDK_OnServerPerformanceReport OnServerPerformanceReport;

    // TODO: Move these functions to proper sections on major plugin update
    SDK_OnCheckpointEntered OnCheckpointEntered;
    SDK_OnCheckpointExited OnCheckpointExited;
    SDK_OnSphereEntered OnSphereEntered;
    SDK_OnSphereExited OnSphereExited;

} PluginCallbacks;
