#pragma once

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Retrieve the name of a certain key-code.
*/
const SQChar * GetKeyCodeName(uint8_t keycode);

/* ------------------------------------------------------------------------------------------------
 * Modify the name of a certain key-code.
*/
void SetKeyCodeName(uint8_t keycode, StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the server version.
*/
SQMOD_NODISCARD uint32_t GetServerVersion();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the server settings.
*/
SQMOD_NODISCARD Table GetServerSettings();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the number of loaded plug-ins.
*/
SQMOD_NODISCARD uint32_t GetNumberOfPlugins();

/* ------------------------------------------------------------------------------------------------
 * Retrieve information about a certain plug-in.
*/
SQMOD_NODISCARD Table GetPluginInfo(int32_t plugin_id);

/* ------------------------------------------------------------------------------------------------
 * Attempt to find a plug-in identifier by it's name.
*/
SQMOD_NODISCARD int32_t FindPlugin(StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * Send a custom command to the loaded plug-ins.
*/
void SendPluginCommand(uint32_t identifier, StackStrF & payload);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the server time.
*/
const ULongInt & GetTime();

/* ------------------------------------------------------------------------------------------------
 * Send a log message to the server.
*/
void SendLogMessage(StackStrF & msg);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the last error that occurred on the server.
*/
SQMOD_NODISCARD int32_t GetLastError();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the version of the host Squirrel plug-in as an integer.
*/
SQMOD_NODISCARD uint32_t GetPluginVersion();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the version of the host Squirrel plug-in as a string.
*/
SQMOD_NODISCARD const SQChar * GetPluginVersionStr();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the name of the host Squirrel plug-in.
*/
SQMOD_NODISCARD const SQChar * GetPluginName();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the author of the host Squirrel plug-in.
*/
SQMOD_NODISCARD const SQChar * GetPluginAuthor();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the id of the host Squirrel plug-in.
*/
SQMOD_NODISCARD int32_t GetPluginID();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the port onto which the server was binded.
*/
SQMOD_NODISCARD uint32_t GetServerPort();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the server flags.
*/
SQMOD_NODISCARD uint32_t GetServerFlags();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the maximum number of clients allowed on the server.
*/
SQMOD_NODISCARD int32_t GetMaxPlayers();

/* ------------------------------------------------------------------------------------------------
 * Modify the maximum number of clients allowed on the server.
*/
void SetMaxPlayers(int32_t max);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the server name.
*/
SQMOD_NODISCARD const SQChar * GetServerName();

/* ------------------------------------------------------------------------------------------------
 * Modify the server name.
*/
void SetServerName(StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the server password.
*/
SQMOD_NODISCARD const SQChar * GetServerPassword();

/* ------------------------------------------------------------------------------------------------
 * Modify the server password.
*/
void SetServerPassword(StackStrF & passwd);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the game-mode text.
*/
SQMOD_NODISCARD const SQChar * GetGameModeText();

/* ------------------------------------------------------------------------------------------------
 * Modify the game-mode text.
*/
void SetGameModeText(StackStrF & text);

/* ------------------------------------------------------------------------------------------------
 * Create a radio stream.
*/
SQMOD_NODISCARD SQInteger CreateRadioStream(bool listed, StackStrF & name, StackStrF & url);

/* ------------------------------------------------------------------------------------------------
 * Create a radio stream.
*/
SQMOD_NODISCARD SQInteger CreateRadioStreamEx(int32_t id, bool listed, StackStrF & name, StackStrF & url);

/* ------------------------------------------------------------------------------------------------
 * Remove a radio stream.
*/
void RemoveRadioStream(int32_t id);

/* ------------------------------------------------------------------------------------------------
 * Shutdown the server.
*/
void ShutdownServer();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a server option.
*/
bool GetServerOption(int32_t option_id);

/* ------------------------------------------------------------------------------------------------
 * Modify a server option.
*/
void SetServerOption(int32_t option_id, bool toggle);

/* ------------------------------------------------------------------------------------------------
 * Modify a server option.
*/
void SetServerOptionEx(int32_t option_id, bool toggle, int32_t header, LightObj & payload);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the world bounds.
*/
SQMOD_NODISCARD Table GetWorldBounds();

/* ------------------------------------------------------------------------------------------------
 * Modify the world bounds.
*/
void SetWorldBounds(const Vector2 & max, const Vector2 & min);

/* ------------------------------------------------------------------------------------------------
 * Modify the world bounds.
*/
void SetWorldBoundsEx(float max_x, float max_y, float min_x, float min_y);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the wasted settings.
*/
SQMOD_NODISCARD Table GetWastedSettings();

/* ------------------------------------------------------------------------------------------------
 * Modify the wasted settings.
*/
void SetWastedSettings(uint32_t dt, uint32_t ft, float fis, float fos,
                        const Color3 & fc, uint32_t cfs, uint32_t cft);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the current time-rate.
*/
SQMOD_NODISCARD int32_t GetTimeRate();

/* ------------------------------------------------------------------------------------------------
 * Modify the current time-rate.
*/
void SetTimeRate(uint32_t rate);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the game hour.
*/
SQMOD_NODISCARD int32_t GetHour();

/* ------------------------------------------------------------------------------------------------
 * Modify the game hour.
*/
void SetHour(int32_t hour);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the game minute.
*/
SQMOD_NODISCARD int32_t GetMinute();

/* ------------------------------------------------------------------------------------------------
 * Modify the game minute.
*/
void SetMinute(int32_t minute);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the weather effects.
*/
SQMOD_NODISCARD int32_t GetWeather();

/* ------------------------------------------------------------------------------------------------
 * Modify the weather effects.
*/
void SetWeather(int32_t weather);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the game gravity.
*/
SQMOD_NODISCARD float GetGravity();

/* ------------------------------------------------------------------------------------------------
 * Modify the game gravity.
*/
void SetGravity(float gravity);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the game speed.
*/
SQMOD_NODISCARD float GetGameSpeed();

/* ------------------------------------------------------------------------------------------------
 * Modify the game speed.
*/
void SetGameSpeed(float speed);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the water level.
*/
SQMOD_NODISCARD float GetWaterLevel();

/* ------------------------------------------------------------------------------------------------
 * Modify the water level.
*/
void SetWaterLevel(float level);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the maximum flight altitude.
*/
SQMOD_NODISCARD float GetMaximumFlightAltitude();

/* ------------------------------------------------------------------------------------------------
 * Modify the maximum flight altitude.
*/
void SetMaximumFlightAltitude(float height);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the kill command delay.
*/
SQMOD_NODISCARD int32_t GetKillCommandDelay();

/* ------------------------------------------------------------------------------------------------
 * Modify the kill command delay.
*/
void SetKillCommandDelay(int32_t delay);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the vehicles forced respawn height.
*/
SQMOD_NODISCARD float GetVehiclesForcedRespawnHeight();

/* ------------------------------------------------------------------------------------------------
 * Modify the vehicles forced respawn height.
*/
void SetVehiclesForcedRespawnHeight(float height);

/* ------------------------------------------------------------------------------------------------
 * Create a game explosion.
*/
void CreateExplosion(int32_t world, int32_t type, const Vector3 & pos, CPlayer & source, bool grounded);

/* ------------------------------------------------------------------------------------------------
 * Create a game explosion.
*/
void CreateExplosionEx(int32_t world, int32_t type, float x, float y, float z, CPlayer & source, bool grounded);

/* ------------------------------------------------------------------------------------------------
 * Play a game sound.
*/
void PlaySound(int32_t world, int32_t sound, const Vector3 & pos);

/* ------------------------------------------------------------------------------------------------
 * Play a game sound.
*/
void PlaySoundEx(int32_t world, int32_t sound, float x, float y, float z);

/* ------------------------------------------------------------------------------------------------
 * Play a game sound to a specific world.
*/
void PlaySoundForWorld(int32_t world, int32_t sound);

/* ------------------------------------------------------------------------------------------------
 * Make a map object invisible.
*/
void HideMapObject(int32_t model, const Vector3 & pos);

/* ------------------------------------------------------------------------------------------------
 * Make a map object invisible.
*/
void HideMapObjectEx(int32_t model, float x, float y, float z);

/* ------------------------------------------------------------------------------------------------
 * Make a map object invisible.
*/
void HideMapObjectRaw(int32_t model, int16_t x, int16_t y, int16_t z);

/* ------------------------------------------------------------------------------------------------
 * Make a map object visible again.
*/
void ShowMapObject(int32_t model, const Vector3 & pos);

/* ------------------------------------------------------------------------------------------------
 * Make a map object visible again.
*/
void ShowMapObjectEx(int32_t model, float x, float y, float z);

/* ------------------------------------------------------------------------------------------------
 * Make a map object visible again.
*/
void ShowMapObjectRaw(int32_t model, int16_t x, int16_t y, int16_t z);

/* ------------------------------------------------------------------------------------------------
 * Make all map objects visible again.
*/
void ShowAllMapObjects();

/* ------------------------------------------------------------------------------------------------
 * Retrieve field data of a certain weapon.
*/
SQMOD_NODISCARD SQFloat GetWeaponDataValue(int32_t weapon, int32_t field);

/* ------------------------------------------------------------------------------------------------
 * Modify field data of a certain weapon.
*/
bool SetWeaponDataValue(int32_t weapon, int32_t field, SQFloat value);

/* ------------------------------------------------------------------------------------------------
 * Reset field data of a certain weapon.
*/
bool ResetWeaponDataValue(int32_t weapon, int32_t field);

/* ------------------------------------------------------------------------------------------------
 * See whether field data of a certain weapon was modified.
*/
bool IsWeaponDataValueModified(int32_t weapon, int32_t field);

/* ------------------------------------------------------------------------------------------------
 * Reset all fields data of a certain weapon.
*/
bool ResetWeaponData(int32_t weapon);

/* ------------------------------------------------------------------------------------------------
 * Reset all fields data of a all weapons.
*/
void ResetAllWeaponData();

/* ------------------------------------------------------------------------------------------------
 * Create a new player class.
*/
SQMOD_NODISCARD int32_t AddPlayerClass(int32_t team, const Color3 & color, int32_t skin, const Vector3 & pos, float angle,
                        int32_t wep1, int32_t ammo1, int32_t wep2, int32_t ammo2, int32_t wep3, int32_t ammo3);

/* ------------------------------------------------------------------------------------------------
 * Set the player position when spawning.
*/
void SetSpawnPlayerPosition(const Vector3 & pos);

/* ------------------------------------------------------------------------------------------------
 * Set the camera position when spawning.
*/
void SetSpawnCameraPosition(const Vector3 & pos);

/* ------------------------------------------------------------------------------------------------
 * Set the camera focus when spawning.
*/
void SetSpawnCameraLookAt(const Vector3 & pos);

/* ------------------------------------------------------------------------------------------------
 * Set the player position when spawning.
*/
void SetSpawnPlayerPositionEx(float x, float y, float z);

/* ------------------------------------------------------------------------------------------------
 * Set the camera position when spawning.
*/
void SetSpawnCameraPositionEx(float x, float y, float z);

/* ------------------------------------------------------------------------------------------------
 * Set the camera focus when spawning.
*/
void SetSpawnCameraLookAtEx(float x, float y, float z);

/* ------------------------------------------------------------------------------------------------
 * Ban an IP address from the server.
*/
void BanIP(StackStrF & addr);

/* ------------------------------------------------------------------------------------------------
 * Unban an IP address from the server.
*/
bool UnbanIP(StackStrF & addr);

/* ------------------------------------------------------------------------------------------------
 * See if an IP address is banned from the server.
*/
bool IsIPBanned(StackStrF & addr);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the identifier of the player with the specified name.
*/
SQMOD_NODISCARD int32_t GetPlayerIdFromName(StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * See if a player with the specified identifier is connected.
*/
bool IsPlayerConnected(int32_t player_id);

/* ------------------------------------------------------------------------------------------------
 * Force all players on the server to select a class.
*/
void ForceAllSelect();

/* ------------------------------------------------------------------------------------------------
 * See if an entity exists on the server.
*/
bool CheckEntityExists(int32_t type, int32_t index);

/* ------------------------------------------------------------------------------------------------
 * See if a certain point is within a known district on the bas game map.
*/
SQMOD_NODISCARD const SQChar * GetDistrictName(const Vector2 & point);

/* ------------------------------------------------------------------------------------------------
 * See if a certain point is within a known district on the bas game map.
*/
SQMOD_NODISCARD const SQChar * GetDistrictNameEx(SQFloat x, SQFloat y);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the fall timer rate.
*/
SQMOD_NODISCARD uint16_t GetFallTimer();

/* ------------------------------------------------------------------------------------------------
 * Modify the fall timer rate.
*/
void SetFallTimer(uint16_t rate);
#if SQMOD_SDK_LEAST(2, 1)
/* ------------------------------------------------------------------------------------------------
 * Retrieve network statistics related to the server.
*/
SQMOD_NODISCARD SQFloat GetNetworkStatisticsF(int32_t option_id);

/* ------------------------------------------------------------------------------------------------
 * Retrieve network statistics related to the server.
*/
SQMOD_NODISCARD SQInteger GetNetworkStatisticsI(int32_t option_id);
#endif

} // Namespace:: SqMod
