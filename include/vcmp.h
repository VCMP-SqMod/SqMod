/*
   Project: Vice City Multiplayer 0.4 Server / Plugin Kit
   File: plugin.h

   Copyright 2011-2016 Ago Allikmaa (maxorator)

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

#include <stdint.h>
#include <stdlib.h>

typedef struct {
	uint32_t structSize;
	char serverName[128];
	uint32_t maxPlayers;
	uint32_t port;
	uint32_t flags;
} ServerSettings;

#define PLUGIN_API_MAJOR 2
#define PLUGIN_API_MINOR 0

typedef struct {
	uint32_t structSize;
	uint32_t pluginId;
	char name[32];
	uint32_t pluginVersion;
	uint16_t apiMajorVersion;
	uint16_t apiMinorVersion;
} PluginInfo;

typedef enum {
	vcmpErrorNone = 0,
	vcmpErrorNoSuchEntity = 1,
	vcmpErrorBufferTooSmall = 2,
	vcmpErrorTooLargeInput = 3,
	vcmpErrorArgumentOutOfBounds = 4,
	vcmpErrorNullArgument = 5,
	vcmpErrorPoolExhausted = 6,
	vcmpErrorInvalidName = 7,
	vcmpErrorRequestDenied = 8,
	forceSizeVcmpError = INT32_MAX
} vcmpError;

typedef enum {
	vcmpEntityPoolVehicle = 1,
	vcmpEntityPoolObject = 2,
	vcmpEntityPoolPickup = 3,
	vcmpEntityPoolRadio = 4,
	vcmpEntityPoolBlip = 7,
	vcmpEntityPoolCheckPoint = 8,
	forceSizeVcmpEntityPool = INT32_MAX
} vcmpEntityPool;

typedef enum {
	vcmpDisconnectReasonTimeout = 0,
	vcmpDisconnectReasonQuit = 1,
	vcmpDisconnectReasonKick = 2,
	vcmpDisconnectReasonCrash = 3,
	vcmpDisconnectReasonAntiCheat = 4,
	forceSizeVcmpDisconnectReason = INT32_MAX
} vcmpDisconnectReason;

typedef enum {
	vcmpBodyPartBody = 0,
	vcmpBodyPartTorso = 1,
	vcmpBodyPartLeftArm = 2,
	vcmpBodyPartRightArm = 3,
	vcmpBodyPartLeftLeg = 4,
	vcmpBodyPartRightLeg = 5,
	vcmpBodyPartHead = 6,
	vcmpBodyPartInVehicle = 7,
	forceSizeVcmpBodyPart = INT32_MAX
} vcmpBodyPart;

typedef enum {
	vcmpPlayerStateNone = 0,
	vcmpPlayerStateNormal = 1,
	vcmpPlayerStateAim = 2,
	vcmpPlayerStateDriver = 3,
	vcmpPlayerStatePassenger = 4,
	vcmpPlayerStateEnterDriver = 5,
	vcmpPlayerStateEnterPassenger = 6,
	vcmpPlayerStateExit = 7,
	vcmpPlayerStateUnspawned = 8,
	forceSizeVcmpPlayerState = INT32_MAX
} vcmpPlayerState;

typedef enum {
	vcmpPlayerUpdateNormal = 0,
	vcmpPlayerUpdateAiming = 1,
	vcmpPlayerUpdateDriver = 2,
	vcmpPlayerUpdatePassenger = 3,
	forceSizeVcmpPlayerUpdate = INT32_MAX
} vcmpPlayerUpdate;

typedef enum {
	vcmpPlayerVehicleOut = 0,
	vcmpPlayerVehicleEntering = 1,
	vcmpPlayerVehicleExiting = 2,
	vcmpPlayerVehicleIn = 3,
	forceSizeVcmpPlayerVehicle = INT32_MAX
} vcmpPlayerVehicle;

typedef enum {
	vcmpVehicleSyncNone = 0,
	vcmpVehicleSyncDriver = 1,
	vcmpVehicleSyncPassenger = 3,
	vcmpVehicleSyncNear = 4,
	forceSizeVcmpVehicleSync = INT32_MAX
} vcmpVehicleSync;

typedef enum {
	vcmpVehicleUpdateDriverSync = 0,
	vcmpVehicleUpdateOtherSync = 1,
	vcmpVehicleUpdatePosition = 2,
	vcmpVehicleUpdateHealth = 4,
	vcmpVehicleUpdateColour = 5,
	vcmpVehicleUpdateRotation = 6,
	forceSizeVcmpVehicleUpdate = INT32_MAX
} vcmpVehicleUpdate;

typedef enum {
	vcmpServerOptionSyncFrameLimiter = 0,
	vcmpServerOptionFrameLimiter = 1,
	vcmpServerOptionTaxiBoostJump = 2,
	vcmpServerOptionDriveOnWater = 3,
	vcmpServerOptionFastSwitch = 4,
	vcmpServerOptionFriendlyFire = 5,
	vcmpServerOptionDisableDriveBy = 6,
	vcmpServerOptionPerfectHandling = 7,
	vcmpServerOptionFlyingCars = 8,
	vcmpServerOptionJumpSwitch = 9,
	vcmpServerOptionShowMarkers = 10,
	vcmpServerOptionOnlyShowTeamMarkers = 11,
	vcmpServerOptionStuntBike = 12,
	vcmpServerOptionShootInAir = 13,
	vcmpServerOptionShowNameTags = 14,
	vcmpServerOptionJoinMessages = 15,
	vcmpServerOptionDeathMessages = 16,
	vcmpServerOptionChatTagsEnabled = 17,
	vcmpServerOptionUseClasses = 18,
	vcmpServerOptionWallGlitch = 19,
	vcmpServerOptionDisableBackfaceCulling = 20,
	vcmpServerOptionDisableHeliBladeDamage = 21,
	forceSizeVcmpServerOption = INT32_MAX
} vcmpServerOption;

typedef enum {
	vcmpPlayerOptionControllable = 0,
	vcmpPlayerOptionDriveBy = 1,
	vcmpPlayerOptionWhiteScanlines = 2,
	vcmpPlayerOptionGreenScanlines = 3,
	vcmpPlayerOptionWidescreen = 4,
	vcmpPlayerOptionShowMarkers = 5,
	vcmpPlayerOptionCanAttack = 6,
	vcmpPlayerOptionHasMarker = 7,
	vcmpPlayerOptionChatTagsEnabled = 8,
	vcmpPlayerOptionDrunkEffects = 9,
	forceSizeVcmpPlayerOption = INT32_MAX
} vcmpPlayerOption;

typedef enum {
	vcmpVehicleOptionDoorsLocked = 0,
	vcmpVehicleOptionAlarm = 1,
	vcmpVehicleOptionLights = 2,
	vcmpVehicleOptionRadioLocked = 3,
	vcmpVehicleOptionGhost = 4,
	vcmpVehicleOptionSiren = 5,
	forceSizeVcmpVehicleOption = INT32_MAX
} vcmpVehicleOption;

typedef struct {
	uint32_t structSize;

	/**
	 * Plugin system
	 */

	/* success */
	uint32_t (*GetServerVersion) (void);
	/* vcmpErrorNullArgument */
	vcmpError (*GetServerSettings) (ServerSettings* settings);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*ExportFunctions) (int32_t pluginId, const void** functionList, size_t size);
	/* success */
	uint32_t (*GetNumberOfPlugins) (void);
	/* vcmpErrorNoSuchEntity, vcmpErrorNullArgument */
	vcmpError (*GetPluginInfo) (int32_t pluginId, PluginInfo* pluginInfo);
	/* -1 == vcmpEntityNone */
	int32_t (*FindPlugin) (const char* pluginName);
	/* GetLastError: vcmpErrorNoSuchEntity */
	const void** (*GetPluginExports) (int32_t pluginId, size_t* exportCount);
	/* vcmpErrorNullArgument, vcmpErrorTooLargeInput */
	vcmpError (*SendPluginCommand) (uint32_t commandIdentifier, const char* format, ...);
	/* success */
	uint64_t (*GetTime) (void);
	/* vcmpErrorNullArgument, vcmpErrorTooLargeInput */
	vcmpError (*LogMessage) (const char* format, ...);
	/* success */
	vcmpError (*GetLastError) (void);

	/**
	 * Client messages
	 */

	/* vcmpErrorNoSuchEntity, vcmpErrorNullArgument, vcmpErrorTooLargeInput */
	vcmpError (*SendClientScriptData) (int32_t playerId, const void* data, size_t size);
	/* vcmpErrorNoSuchEntity, vcmpErrorNullArgument, vcmpErrorTooLargeInput */
	vcmpError (*SendClientMessage) (int32_t playerId, uint32_t colour, const char* format, ...);
	/* vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds, vcmpErrorNullArgument, vcmpErrorTooLargeInput */
	vcmpError (*SendGameMessage) (int32_t playerId, int32_t type, const char* format, ...);

	/*
	 * Server settings
	 */

	/* vcmpErrorNullArgument, vcmpErrorTooLargeInput */
	vcmpError (*SetServerName) (const char* text);
	/* vcmpErrorNullArgument, vcmpErrorBufferTooSmall */
	vcmpError (*GetServerName) (char* buffer, size_t size);
	/* vcmpErrorArgumentOutOfBounds */
	vcmpError (*SetMaxPlayers) (uint32_t maxPlayers);
	/* success */
	uint32_t (*GetMaxPlayers) (void);
	/* vcmpErrorNullArgument, vcmpErrorTooLargeInput */
	vcmpError (*SetServerPassword) (const char* password);
	/* vcmpErrorNullArgument, vcmpErrorBufferTooSmall */
	vcmpError (*GetServerPassword) (char* buffer, size_t size);
	/* vcmpErrorNullArgument, vcmpErrorTooLargeInput */
	vcmpError (*SetGameModeText) (const char* gameMode);
	/* vcmpErrorNullArgument, vcmpErrorBufferTooSmall */
	vcmpError (*GetGameModeText) (char* buffer, size_t size);
	/* success */
	void (*ShutdownServer) (void);

	/*
	 * Game environment settings
	 */

	/* vcmpErrorArgumentOutOfBounds */
	vcmpError (*SetServerOption) (vcmpServerOption option, uint8_t toggle);
	/* GetLastError: vcmpErrorArgumentOutOfBounds */
	uint8_t (*GetServerOption) (vcmpServerOption option);
	/* success */
	void (*SetWorldBounds) (float maxX, float minX, float maxY, float minY);
	/* success */
	void (*GetWorldBounds) (float* maxXOut, float* minXOut, float* maxYOut, float* minYOut);
	/* success */
	void (*SetWastedSettings) (uint32_t deathTimer, uint32_t fadeTimer, float fadeInSpeed, float fadeOutSpeed, uint32_t fadeColour, uint32_t corpseFadeStart, uint32_t corpseFadeTime);
	/* success */
	void (*GetWastedSettings) (uint32_t* deathTimerOut, uint32_t* fadeTimerOut, float* fadeInSpeedOut, float* fadeOutSpeedOut, uint32_t* fadeColourOut, uint32_t* corpseFadeStartOut, uint32_t* corpseFadeTimeOut);
	/* success */
	void (*SetTimeRate) (int32_t timeRate);
	/* success */
	int32_t (*GetTimeRate) (void);
	/* success */
	void (*SetHour) (int32_t hour);
	/* success */
	int32_t (*GetHour) (void);
	/* success */
	void (*SetMinute) (int32_t minute);
	/* success */
	int32_t (*GetMinute) (void);
	/* success */
	void (*SetWeather) (int32_t weather);
	/* success */
	int32_t (*GetWeather) (void);
	/* success */
	void (*SetGravity) (float gravity);
	/* success */
	float (*GetGravity) (void);
	/* success */
	void (*SetGameSpeed) (float gameSpeed);
	/* success */
	float (*GetGameSpeed) (void);
	/* success */
	void (*SetWaterLevel) (float waterLevel);
	/* success */
	float (*GetWaterLevel) (void);
	/* success */
	void (*SetMaximumFlightAltitude) (float height);
	/* success */
	float (*GetMaximumFlightAltitude) (void);
	/* success */
	void (*SetKillCommandDelay) (int32_t delay);
	/* success */
	int32_t (*GetKillCommandDelay) (void);
	/* success */
	void (*SetVehiclesForcedRespawnHeight) (float height);
	/* success */
	float (*GetVehiclesForcedRespawnHeight) (void);

	/*
	 * Miscellaneous things
	 */

	/* vcmpErrorArgumentOutOfBounds, vcmpErrorNoSuchEntity */
	vcmpError (*CreateExplosion) (int32_t worldId, int32_t type, float x, float y, float z, int32_t responsiblePlayerId, uint8_t atGroundLevel);
	/* vcmpErrorArgumentOutOfBounds */
	vcmpError (*PlaySound) (int32_t worldId, int32_t soundId, float x, float y, float z);
	/* success */
	void (*HideMapObject) (int32_t modelId, int16_t tenthX, int16_t tenthY, int16_t tenthZ);
	/* success */
	void (*ShowMapObject) (int32_t modelId, int16_t tenthX, int16_t tenthY, int16_t tenthZ);
	/* success */
	void (*ShowAllMapObjects) (void);

	/*
	 * Weapon settings
	 */

	/* vcmpErrorArgumentOutOfBounds */
	vcmpError (*SetWeaponDataValue) (int32_t weaponId, int32_t fieldId, double value);
	/* GetLastError: vcmpErrorArgumentOutOfBounds */
	double (*GetWeaponDataValue) (int32_t weaponId, int32_t fieldId);
	/* vcmpErrorArgumentOutOfBounds */
	vcmpError (*ResetWeaponDataValue) (int32_t weaponId, int32_t fieldId);
	/* GetLastError: vcmpErrorArgumentOutOfBounds */
	uint8_t (*IsWeaponDataValueModified) (int32_t weaponId, int32_t fieldId);
	/* vcmpErrorArgumentOutOfBounds */
	vcmpError (*ResetWeaponData) (int32_t weaponId);
	/* success */
	void (*ResetAllWeaponData) (void);

	/*
	 * Key binds
	 */

	/* -1 == vcmpEntityNone */
	int32_t (*GetKeyBindUnusedSlot) (void);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetKeyBindData) (int32_t bindId, uint8_t* isCalledOnReleaseOut, int32_t* keyOneOut, int32_t* keyTwoOut, int32_t* keyThreeOut);
	/* vcmpErrorArgumentOutOfBounds */
	vcmpError (*RegisterKeyBind) (int32_t bindId, uint8_t isCalledOnRelease, int32_t keyOne, int32_t keyTwo, int32_t keyThree);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*RemoveKeyBind) (int32_t bindId);
	/* success */
	void (*RemoveAllKeyBinds) (void);

	/*
	 * Coordinate blips
	 */

	/* GetLastError: vcmpErrorPoolExhausted */
	int32_t (*CreateCoordBlip) (int32_t index, int32_t world, float x, float y, float z, int32_t scale, uint32_t colour, int32_t sprite);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*DestroyCoordBlip) (int32_t index);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetCoordBlipInfo) (int32_t index, int32_t* worldOut, float* xOut, float* yOUt, float* zOut, int32_t* scaleOut, uint32_t* colourOut, int32_t* spriteOut);

	/*
	 * Radios
	 */

	/* vcmpErrorArgumentOutOfBounds, vcmpErrorNullArgument */
	vcmpError (*AddRadioStream) (int32_t radioId, const char* radioName, const char* radioUrl, uint8_t isListed);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*RemoveRadioStream) (int32_t radioId);

	/*
	 * Spawning and classes
	 */

	/* GetLastError: vcmpErrorArgumentOutOfBounds, vcmpErrorPoolExhausted */
	int32_t (*AddPlayerClass) (int32_t teamId, uint32_t colour, int32_t modelIndex, float x, float y, float z, float angle, int32_t weaponOne, int32_t weaponOneAmmo, int32_t weaponTwo, int32_t weaponTwoAmmo, int32_t weaponThree, int32_t weaponThreeAmmo);
	/* success */
	void (*SetSpawnPlayerPosition) (float x, float y, float z);
	/* success */
	void (*SetSpawnCameraPosition) (float x, float y, float z);
	/* success */
	void (*SetSpawnCameraLookAt) (float x, float y, float z);

	/*
	 * Administration
	 */
	
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint8_t (*IsPlayerAdmin) (int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPlayerAdmin) (int32_t playerId, uint8_t toggle);
	/* vcmpErrorNoSuchEntity, vcmpErrorNullArgument, vcmpErrorBufferTooSmall */
	vcmpError (*GetPlayerIP) (int32_t playerId, char* buffer, size_t size);
	/* vcmpErrorNoSuchEntity, vcmpErrorNullArgument, vcmpErrorBufferTooSmall */
	vcmpError (*GetPlayerUID) (int32_t playerId, char* buffer, size_t size);
	/* vcmpErrorNoSuchEntity, vcmpErrorNullArgument, vcmpErrorBufferTooSmall */
	vcmpError (*GetPlayerUID2) (int32_t playerId, char* buffer, size_t size);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*KickPlayer) (int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*BanPlayer) (int32_t playerId);
	/* success */
	void (*BanIP) (char* ipAddress);
	/* success */
	uint8_t (*UnbanIP) (char* ipAddress);
	/* success */
	uint8_t (*IsIPBanned) (char* ipAddress);

	/*
	 * Player access and basic info
	 */

	/* -1 == vcmpEntityNone */
	int32_t (*GetPlayerIdFromName) (const char* name);
	/* success */
	uint8_t (*IsPlayerConnected) (int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	uint8_t (*IsPlayerStreamedForPlayer) (int32_t checkedPlayerId, int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	uint32_t (*GetPlayerKey) (int32_t playerId);
	/* vcmpErrorNoSuchEntity, vcmpErrorNullArgument, vcmpErrorBufferTooSmall */
	vcmpError (*GetPlayerName) (int32_t playerId, char* buffer, size_t size);
	/* vcmpErrorNoSuchEntity, vcmpErrorNullArgument, vcmpErrorInvalidName, vcmpErrorTooLargeInput */
	vcmpError (*SetPlayerName) (int32_t playerId, const char* name);
	/* GetLastError: vcmpErrorNoSuchEntity */
	vcmpPlayerState (*GetPlayerState) (int32_t playerId);
	/* vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds */
	vcmpError (*SetPlayerOption) (int32_t playerId, vcmpPlayerOption option, uint8_t toggle);
	/* GetLastError: vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds */
	uint8_t (*GetPlayerOption) (int32_t playerId, vcmpPlayerOption option);

	/*
	 * Player world
	 */

	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPlayerWorld) (int32_t playerId, int32_t world);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerWorld) (int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPlayerSecondaryWorld) (int32_t playerId, int32_t secondaryWorld);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerSecondaryWorld) (int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerUniqueWorld) (int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint8_t (*IsPlayerWorldCompatible) (int32_t playerId, int32_t world);

	/*
	 * Player class, team, skin, colour
	 */

	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerClass) (int32_t playerId);
	/* vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds */
	vcmpError (*SetPlayerTeam) (int32_t playerId, int32_t teamId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerTeam) (int32_t playerId);
	/* vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds */
	vcmpError (*SetPlayerSkin) (int32_t playerId, int32_t skinId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerSkin) (int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPlayerColour) (int32_t playerId, uint32_t colour);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint32_t (*GetPlayerColour) (int32_t playerId);

	/*
	 * Player spawn cycle
	 */

	/* vcmpErrorNoSuchEntity */
	uint8_t (*IsPlayerSpawned) (int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*ForcePlayerSpawn) (int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*ForcePlayerSelect) (int32_t playerId);
	/* success */
	void (*ForceAllSelect) (void);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint8_t (*IsPlayerTyping) (int32_t playerId);

	/*
	 * Player money, score, wanted level
	 */

	/* vcmpErrorNoSuchEntity */
	vcmpError (*GivePlayerMoney) (int32_t playerId, int32_t amount);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPlayerMoney) (int32_t playerId, int32_t amount);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerMoney) (int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPlayerScore) (int32_t playerId, int32_t score);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerScore) (int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPlayerWantedLevel) (int32_t playerId, int32_t level);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerWantedLevel) (int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerPing) (int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	double (*GetPlayerFPS) (int32_t playerId);

	/*
	 * Player health and immunity
	 */

	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPlayerHealth) (int32_t playerId, float health);
	/* GetLastError: vcmpErrorNoSuchEntity */
	float (*GetPlayerHealth) (int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPlayerArmour) (int32_t playerId, float armour);
	/* GetLastError: vcmpErrorNoSuchEntity */
	float (*GetPlayerArmour) (int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPlayerImmunityFlags) (int32_t playerId, uint32_t flags);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint32_t (*GetPlayerImmunityFlags) (int32_t playerId);

	/*
	 * Player position and rotation
	 */

	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPlayerPosition) (int32_t playerId, float x, float y, float z);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetPlayerPosition) (int32_t playerId, float* xOut, float* yOut, float* zOut);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPlayerSpeed) (int32_t playerId, float x, float y, float z);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetPlayerSpeed) (int32_t playerId, float* xOut, float* yOut, float* zOut);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*AddPlayerSpeed) (int32_t playerId, float x, float y, float z);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPlayerHeading) (int32_t playerId, float angle);
	/* GetLastError: vcmpErrorNoSuchEntity */
	float (*GetPlayerHeading) (int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPlayerAlpha) (int32_t playerId, int32_t alpha, uint32_t fadeTime);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerAlpha) (int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetPlayerAimPosition) (int32_t playerId, float* xOut, float* yOut, float* zOut);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetPlayerAimDirection) (int32_t playerId, float* xOut, float* yOut, float* zOut);

	/*
	 * Player actions and keys
	 */

	/* GetLastError: vcmpErrorNoSuchEntity */
	uint8_t (*IsPlayerOnFire) (int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint8_t (*IsPlayerCrouching) (int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerAction) (int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint32_t (*GetPlayerGameKeys) (int32_t playerId);

	/*
	 * Player vehicle
	 */

	/* vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds, vcmpErrorRequestDenied */
	vcmpError (*PutPlayerInVehicle) (int32_t playerId, int32_t vehicleId, int32_t slotIndex, uint8_t makeRoom, uint8_t warp);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*RemovePlayerFromVehicle) (int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	vcmpPlayerVehicle (*GetPlayerInVehicleStatus) (int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerInVehicleSlot) (int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerVehicleId) (int32_t playerId);

	/*
	 * Player weapons
	 */

	/* vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds */
	vcmpError (*GivePlayerWeapon) (int32_t playerId, int32_t weaponId, int32_t ammo);
	/* vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds */
	vcmpError (*SetPlayerWeapon) (int32_t playerId, int32_t weaponId, int32_t ammo);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerWeapon) (int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerWeaponAmmo) (int32_t playerId);
	/* vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds */
	vcmpError (*SetPlayerWeaponSlot) (int32_t playerId, int32_t slot);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerWeaponSlot) (int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds */
	int32_t (*GetPlayerWeaponAtSlot) (int32_t playerId, int32_t slot);
	/* GetLastError: vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds */
	int32_t (*GetPlayerAmmoAtSlot) (int32_t playerId, int32_t slot);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*RemovePlayerWeapon) (int32_t playerId, int32_t weaponId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*RemoveAllWeapons) (int32_t playerId);

	/*
	 * Player camera
	 */

	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetCameraPosition) (int32_t playerId, float posX, float posY, float posZ, float lookX, float lookY, float lookZ);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*RestoreCamera) (int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint8_t (*IsCameraLocked) (int32_t playerId);

	/*
	 * Player miscellaneous stuff
	 */

	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPlayerAnimation) (int32_t playerId, int32_t groupId, int32_t animationId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerStandingOnVehicle) (int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerStandingOnObject) (int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint8_t (*IsPlayerAway) (int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	int32_t (*GetPlayerSpectateTarget) (int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	vcmpError (*SetPlayerSpectateTarget) (int32_t playerId, int32_t targetId);
	/* vcmpErrorNoSuchEntity, vcmpErrorNullArgument */
	vcmpError (*RedirectPlayerToServer) (int32_t playerId, const char* ip, uint32_t port, const char* nick, const char* serverPassword, const char* userPassword);

	/*
	 * All entities
	 */

	/* GetLastError: vcmpArgumentOutOfBounds */
	uint8_t (*CheckEntityExists) (vcmpEntityPool entityPool, int32_t index);

	/*
	 * Vehicles
	 */

	/* GetLastError: vcmpErrorArgumentOutOfBounds, vcmpErrorPoolExhausted */
	int32_t (*CreateVehicle) (int32_t modelIndex, int32_t world, float x, float y, float z, float angle, int32_t primaryColour, int32_t secondaryColour);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*DeleteVehicle) (int32_t vehicleId);
	/* vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds */
	vcmpError (*SetVehicleOption) (int32_t vehicleId, vcmpVehicleOption option, uint8_t toggle);
	/* GetLastError: vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds */
	uint8_t (*GetVehicleOption) (int32_t vehicleId, vcmpVehicleOption option);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetVehicleSyncSource) (int32_t vehicleId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	vcmpVehicleSync (*GetVehicleSyncType) (int32_t vehicleId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint8_t (*IsVehicleStreamedForPlayer) (int32_t vehicleId, int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehicleWorld) (int32_t vehicleId, int32_t world);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetVehicleWorld) (int32_t vehicleId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetVehicleModel) (int32_t vehicleId);
	/* GetLastError: vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds */
	int32_t (*GetVehicleOccupant) (int32_t vehicleId, int32_t slotIndex);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*RespawnVehicle) (int32_t vehicleId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehicleImmunityFlags) (int32_t vehicleId, uint32_t immunityFlags);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint32_t (*GetVehicleImmunityFlags) (int32_t vehicleId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*ExplodeVehicle) (int32_t vehicleId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint8_t (*IsVehicleWrecked) (int32_t vehicleId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehiclePosition) (int32_t vehicleId, float x, float y, float z, uint8_t removeOccupants);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetVehiclePosition) (int32_t vehicleId, float* xOut, float* yOut, float* zOut);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehicleRotation) (int32_t vehicleId, float x, float y, float z, float w);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehicleRotationEuler) (int32_t vehicleId, float x, float y, float z);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetVehicleRotation) (int32_t vehicleId, float* xOut, float* yOut, float* zOut, float* wOut);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetVehicleRotationEuler) (int32_t vehicleId, float* xOut, float* yOut, float* zOut);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehicleSpeed) (int32_t vehicleId, float x, float y, float z, uint8_t add, uint8_t relative);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetVehicleSpeed) (int32_t vehicleId, float* xOut, float* yOut, float* zOut, uint8_t relative);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehicleTurnSpeed) (int32_t vehicleId, float x, float y, float z, uint8_t add, uint8_t relative);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetVehicleTurnSpeed) (int32_t vehicleId, float* xOut, float* yOut, float* zOut, uint8_t relative);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehicleSpawnPosition) (int32_t vehicleId, float x, float y, float z);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetVehicleSpawnPosition) (int32_t vehicleId, float* xOut, float* yOut, float* zOut);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehicleSpawnRotation) (int32_t vehicleId, float x, float y, float z, float w);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehicleSpawnRotationEuler) (int32_t vehicleId, float x, float y, float z);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetVehicleSpawnRotation) (int32_t vehicleId, float* xOut, float* yOut, float* zOut, float* wOut);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetVehicleSpawnRotationEuler) (int32_t vehicleId, float* xOut, float* yOut, float* zOut);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehicleIdleRespawnTimer) (int32_t vehicleId, uint32_t millis);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint32_t (*GetVehicleIdleRespawnTimer) (int32_t vehicleId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehicleHealth) (int32_t vehicleId, float health);
	/* GetLastError: vcmpErrorNoSuchEntity */
	float (*GetVehicleHealth) (int32_t vehicleId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehicleColour) (int32_t vehicleId, int32_t primaryColour, int32_t secondaryColour);
	/* vcmpErrorNoSuchEntity, vcmpErrorNullArgument */
	vcmpError (*GetVehicleColour) (int32_t vehicleId, int32_t* primaryColourOut, int32_t* secondaryColourOut);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehiclePartStatus) (int32_t vehicleId, int32_t partId, int32_t status);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetVehiclePartStatus) (int32_t vehicleId, int32_t partId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehicleTyreStatus) (int32_t vehicleId, int32_t tyreId, int32_t status);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetVehicleTyreStatus) (int32_t vehicleId, int32_t tyreId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehicleDamageData) (int32_t vehicleId, uint32_t damageData);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint32_t (*GetVehicleDamageData) (int32_t vehicleId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetVehicleRadio) (int32_t vehicleId, int32_t radioId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetVehicleRadio) (int32_t vehicleId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetVehicleTurretRotation) (int32_t vehicleId, float* horizontalOut, float* verticalOut);

	/*
	 * Vehicle handling
	 */

	/* success */
	void (*ResetAllVehicleHandlings) (void);
	/* vcmpErrorArgumentOutOfBounds */
	uint8_t (*ExistsHandlingRule) (int32_t modelIndex, int32_t ruleIndex);
	/* vcmpErrorArgumentOutOfBounds */
	vcmpError (*SetHandlingRule) (int32_t modelIndex, int32_t ruleIndex, double value);
	/* GetLastError: vcmpErrorArgumentOutOfBounds */
	double (*GetHandlingRule) (int32_t modelIndex, int32_t ruleIndex);
	/* vcmpErrorArgumentOutOfBounds */
	vcmpError (*ResetHandlingRule) (int32_t modelIndex, int32_t ruleIndex);
	/* vcmpErrorArgumentOutOfBounds */
	vcmpError (*ResetHandling) (int32_t modelIndex);
	/* GetLastError: vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds */
	uint8_t (*ExistsInstHandlingRule) (int32_t vehicleId, int32_t ruleIndex);
	/* vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds */
	vcmpError (*SetInstHandlingRule) (int32_t vehicleId, int32_t ruleIndex, double value);
	/* GetLastError: vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds */
	double (*GetInstHandlingRule) (int32_t vehicleId, int32_t ruleIndex);
	/* vcmpErrorNoSuchEntity, vcmpErrorArgumentOutOfBounds */
	vcmpError (*ResetInstHandlingRule) (int32_t vehicleId, int32_t ruleIndex);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*ResetInstHandling) (int32_t vehicleId);

	/*
	 * Pickups
	 */

	/* vcmpErrorPoolExhausted */
	int32_t (*CreatePickup) (int32_t modelIndex, int32_t world, int32_t quantity, float x, float y, float z, int32_t alpha, uint8_t isAutomatic);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*DeletePickup) (int32_t pickupId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint8_t (*IsPickupStreamedForPlayer) (int32_t pickupId, int32_t playerId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPickupWorld) (int32_t pickupId, int32_t world);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPickupWorld) (int32_t pickupId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPickupAlpha) (int32_t pickupId, int32_t alpha);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPickupAlpha) (int32_t pickupId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPickupIsAutomatic) (int32_t pickupId, uint8_t toggle);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint8_t (*IsPickupAutomatic) (int32_t pickupId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPickupAutoTimer) (int32_t pickupId, uint32_t durationMillis);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint32_t (*GetPickupAutoTimer) (int32_t pickupId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*RefreshPickup) (int32_t pickupId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetPickupPosition) (int32_t pickupId, float x, float y, float z);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetPickupPosition) (int32_t pickupId, float* xOut, float* yOut, float* zOut);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPickupModel) (int32_t pickupId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetPickupQuantity) (int32_t pickupId);

	/*
	 * Checkpoints
	 */

	/* vcmpErrorPoolExhausted, vcmpErrorNoSuchEntity */
	int32_t (*CreateCheckPoint) (int32_t playerId, int32_t world, uint8_t isSphere, float x, float y, float z, int32_t red, int32_t green, int32_t blue, int32_t alpha, float radius);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*DeleteCheckPoint) (int32_t checkPointId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint8_t (*IsCheckPointStreamedForPlayer) (int32_t checkPointId, int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint8_t (*IsCheckPointSphere) (int32_t checkPointId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetCheckPointWorld) (int32_t checkPointId, int32_t world);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetCheckPointWorld) (int32_t checkPointId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetCheckPointColour) (int32_t checkPointId, int32_t red, int32_t green, int32_t blue, int32_t alpha);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetCheckPointColour) (int32_t checkPointId, int32_t* redOut, int32_t* greenOut, int32_t* blueOut, int32_t* alphaOut);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetCheckPointPosition) (int32_t checkPointId, float x, float y, float z);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetCheckPointPosition) (int32_t checkPointId, float* xOut, float* yOut, float* zOut);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetCheckPointRadius) (int32_t checkPointId, float radius);
	/* GetLastError: vcmpErrorNoSuchEntity */
	float (*GetCheckPointRadius) (int32_t checkPointId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetCheckPointOwner) (int32_t checkPointId);

	/*
	 * Objects
	 */

	/* GetLastError: vcmpErrorPoolExhausted */
	int32_t (*CreateObject) (int32_t modelIndex, int32_t world, float x, float y, float z, int32_t alpha);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*DeleteObject) (int32_t objectId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint8_t (*IsObjectStreamedForPlayer) (int32_t objectId, int32_t playerId);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetObjectModel) (int32_t objectId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetObjectWorld) (int32_t objectId, int32_t world);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetObjectWorld) (int32_t objectId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetObjectAlpha) (int32_t objectId, int32_t alpha, uint32_t duration);
	/* GetLastError: vcmpErrorNoSuchEntity */
	int32_t (*GetObjectAlpha) (int32_t objectId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*MoveObjectTo) (int32_t objectId, float x, float y, float z, uint32_t duration);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*MoveObjectBy) (int32_t objectId, float x, float y, float z, uint32_t duration);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetObjectPosition) (int32_t objectId, float x, float y, float z);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetObjectPosition) (int32_t objectId, float* xOut, float* yOut, float* zOut);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*RotateObjectTo) (int32_t objectId, float x, float y, float z, float w, uint32_t duration);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*RotateObjectToEuler) (int32_t objectId, float x, float y, float z, uint32_t duration);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*RotateObjectBy) (int32_t objectId, float x, float y, float z, float w, uint32_t duration);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*RotateObjectByEuler) (int32_t objectId, float x, float y, float z, uint32_t duration);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetObjectRotation) (int32_t objectId, float* xOut, float* yOut, float *zOut, float *wOut);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*GetObjectRotationEuler) (int32_t objectId, float* xOut, float* yOut, float *zOut);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetObjectShotReportEnabled) (int32_t objectId, uint8_t toggle);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint8_t (*IsObjectShotReportEnabled) (int32_t objectId);
	/* vcmpErrorNoSuchEntity */
	vcmpError (*SetObjectTouchedReportEnabled) (int32_t objectId, uint8_t toggle);
	/* GetLastError: vcmpErrorNoSuchEntity */
	uint8_t (*IsObjectTouchedReportEnabled) (int32_t objectId);

} PluginFuncs;

typedef struct {
	uint32_t structSize;

	uint8_t (*OnServerInitialise) (void);
	void (*OnServerShutdown) (void);
	void (*OnServerFrame) (float elapsedTime);

	uint8_t (*OnPluginCommand) (uint32_t commandIdentifier, const char* message);
	uint8_t (*OnIncomingConnection) (char* playerName, size_t nameBufferSize, const char* userPassword, const char* ipAddress);
	void (*OnClientScriptData) (int32_t playerId, const uint8_t* data, size_t size);

	void (*OnPlayerConnect) (int32_t playerId);
	void (*OnPlayerDisconnect) (int32_t playerId, vcmpDisconnectReason reason);

	uint8_t (*OnPlayerRequestClass) (int32_t playerId, int32_t offset);
	uint8_t (*OnPlayerRequestSpawn) (int32_t playerId);
	void (*OnPlayerSpawn) (int32_t playerId);
	void (*OnPlayerDeath) (int32_t playerId, int32_t killerId, int32_t reason, vcmpBodyPart bodyPart);
	void (*OnPlayerUpdate) (int32_t playerId, vcmpPlayerUpdate updateType);

	uint8_t (*OnPlayerRequestEnterVehicle) (int32_t playerId, int32_t vehicleId, int32_t slotIndex);
	void (*OnPlayerEnterVehicle) (int32_t playerId, int32_t vehicleId, int32_t slotIndex);
	void (*OnPlayerExitVehicle) (int32_t playerId, int32_t vehicleId);

	void (*OnPlayerNameChange) (int32_t playerId, const char* oldName, const char* newName);
	void (*OnPlayerStateChange) (int32_t playerId, vcmpPlayerState oldState, vcmpPlayerState newState);
	void (*OnPlayerActionChange) (int32_t playerId, int32_t oldAction, int32_t newAction);
	void (*OnPlayerOnFireChange) (int32_t playerId, uint8_t isOnFire);
	void (*OnPlayerCrouchChange) (int32_t playerId, uint8_t isCrouching);
	void (*OnPlayerGameKeysChange) (int32_t playerId, uint32_t oldKeys, uint32_t newKeys);
	void (*OnPlayerBeginTyping) (int32_t playerId);
	void (*OnPlayerEndTyping) (int32_t playerId);
	void (*OnPlayerAwayChange) (int32_t playerId, uint8_t isAway);

	uint8_t (*OnPlayerMessage) (int32_t playerId, const char* message);
	uint8_t (*OnPlayerCommand) (int32_t playerId, const char* message);
	uint8_t (*OnPlayerPrivateMessage) (int32_t playerId, int32_t targetPlayerId, const char* message);

	void (*OnPlayerKeyBindDown) (int32_t playerId, int32_t bindId);
	void (*OnPlayerKeyBindUp) (int32_t playerId, int32_t bindId);
	void (*OnPlayerSpectate) (int32_t playerId, int32_t targetPlayerId);
	void (*OnPlayerCrashReport) (int32_t playerId, const char* report);

	void (*OnVehicleUpdate) (int32_t vehicleId, vcmpVehicleUpdate updateType);
	void (*OnVehicleExplode) (int32_t vehicleId);
	void (*OnVehicleRespawn) (int32_t vehicleId);

	void (*OnObjectShot) (int32_t objectId, int32_t playerId, int32_t weaponId);
	void (*OnObjectTouched) (int32_t objectId, int32_t playerId);

	uint8_t (*OnPickupPickAttempt) (int32_t pickupId, int32_t playerId);
	void (*OnPickupPicked) (int32_t pickupId, int32_t playerId);
	void (*OnPickupRespawn) (int32_t pickupId);

	void (*OnCheckpointEntered) (int32_t checkPointId, int32_t playerId);
	void (*OnCheckpointExited) (int32_t checkPointId, int32_t playerId);

	void (*OnEntityPoolChange) (vcmpEntityPool entityType, int32_t entityId, uint8_t isDeleted);
	void (*OnServerPerformanceReport) (size_t entryCount, const char** descriptions, uint64_t* times);

} PluginCallbacks;
