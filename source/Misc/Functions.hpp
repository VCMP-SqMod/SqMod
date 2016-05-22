#ifndef _MISC_FUNCTIONS_HPP_
#define _MISC_FUNCTIONS_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Retrieve the name of a certain key-code.
*/
CSStr GetKeyCodeName(Uint8 keycode);

/* ------------------------------------------------------------------------------------------------
 * Modify the name of a certain key-code.
*/
void SetKeyCodeName(Uint8 keycode, CSStr name);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the server version.
*/
Uint32 GetServerVersion();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the server settings.
*/
Table GetServerSettings();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the number of loaded plug-ins.
*/
Uint32 GetNumberOfPlugins();

/* ------------------------------------------------------------------------------------------------
 * Retrieve information about a certain plug-in.
*/
Table GetPluginInfo(Int32 plugin_id);

/* ------------------------------------------------------------------------------------------------
 * Attempt to find a plug-in identifier by it's name.
*/
Int32 FindPlugin(CSStr name);

/* ------------------------------------------------------------------------------------------------
 * Send a custom command to the loaded plug-ins.
*/
void SendPluginCommand(Uint32 identifier, CSStr payload);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the server time.
*/
const ULongInt & GetTime();

/* ------------------------------------------------------------------------------------------------
 * Send a log message to the server.
*/
SQInteger SendLogMessage(HSQUIRRELVM vm);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the last error that occurred on the server.
*/
Int32 GetLastError();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the version of the host Squirrel plug-in as an integer.
*/
Uint32 GetPluginVersion();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the version of the host Squirrel plug-in as a string.
*/
CSStr GetPluginVersionStr();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the name of the host Squirrel plug-in.
*/
CSStr GetPluginName();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the author of the host Squirrel plug-in.
*/
CSStr GetPluginAuthor();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the id of the host Squirrel plug-in.
*/
Int32 GetPluginID();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the port onto which the server was binded.
*/
Uint32 GetServerPort();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the server flags.
*/
Uint32 GetServerFlags();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the maximum number of clients allowed on the server.
*/
Int32 GetMaxPlayers(void);

/* ------------------------------------------------------------------------------------------------
 * Modify the maximum number of clients allowed on the server.
*/
void SetMaxPlayers(Int32 max);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the server name.
*/
CSStr GetServerName();

/* ------------------------------------------------------------------------------------------------
 * Modify the server name.
*/
void SetServerName(CSStr name);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the server password.
*/
CSStr GetServerPassword();

/* ------------------------------------------------------------------------------------------------
 * Modify the server password.
*/
void SetServerPassword(CSStr passwd);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the game-mode text.
*/
CSStr GetGameModeText();

/* ------------------------------------------------------------------------------------------------
 * Modify the game-mode text.
*/
void SetGameModeText(CSStr text);

/* ------------------------------------------------------------------------------------------------
 * Create a radio stream.
*/
void CreateRadioStream(CSStr name, CSStr url, bool listed);

/* ------------------------------------------------------------------------------------------------
 * Create a radio stream.
*/
void CreateRadioStreamEx(Int32 id, CSStr name, CSStr url, bool listed);

/* ------------------------------------------------------------------------------------------------
 * Remove a radio stream.
*/
void RemoveRadioStream(Int32 id);

/* ------------------------------------------------------------------------------------------------
 * Shutdown the server.
*/
void ShutdownServer();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a server option.
*/
bool GetServerOption(Int32 option_id);

/* ------------------------------------------------------------------------------------------------
 * Modify a server option.
*/
void SetServerOption(Int32 option_id, bool toggle);

/* ------------------------------------------------------------------------------------------------
 * Modify a server option.
*/
void SetServerOptionEx(Int32 option_id, bool toggle, Int32 header, Object & payload);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the world bounds.
*/
Table GetWorldBounds();

/* ------------------------------------------------------------------------------------------------
 * Modify the world bounds.
*/
void SetWorldBounds(const Vector2 & max, const Vector2 & min);

/* ------------------------------------------------------------------------------------------------
 * Modify the world bounds.
*/
void SetWorldBoundsEx(Float32 max_x, Float32 max_y, Float32 min_x, Float32 min_y);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the wasted settings.
*/
Table GetWastedSettings();

/* ------------------------------------------------------------------------------------------------
 * Modify the wasted settings.
*/
void SetWastedSettings(Uint32 dt, Uint32 ft, Float32 fis, Float32 fos,
                        const Color3 & fc, Uint32 cfs, Uint32 cft);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the current time-rate.
*/
Uint32 GetTimeRate(void);

/* ------------------------------------------------------------------------------------------------
 * Modify the current time-rate.
*/
void SetTimeRate(Uint32 rate);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the game hour.
*/
Int32 GetHour(void);

/* ------------------------------------------------------------------------------------------------
 * Modify the game hour.
*/
void SetHour(Int32 hour);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the game minute.
*/
Int32 GetMinute(void);

/* ------------------------------------------------------------------------------------------------
 * Modify the game minute.
*/
void SetMinute(Int32 minute);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the weather effects.
*/
Int32 GetWeather(void);

/* ------------------------------------------------------------------------------------------------
 * Modify the weather effects.
*/
void SetWeather(Int32 weather);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the game gravity.
*/
Float32 GetGravity(void);

/* ------------------------------------------------------------------------------------------------
 * Modify the game gravity.
*/
void SetGravity(Float32 gravity);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the game speed.
*/
Float32 GetGameSpeed(void);

/* ------------------------------------------------------------------------------------------------
 * Modify the game speed.
*/
void SetGameSpeed(Float32 speed);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the water level.
*/
Float32 GetWaterLevel(void);

/* ------------------------------------------------------------------------------------------------
 * Modify the water level.
*/
void SetWaterLevel(Float32 level);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the maximum flight altitude.
*/
Float32 GetMaximumFlightAltitude(void);

/* ------------------------------------------------------------------------------------------------
 * Modify the maximum flight altitude.
*/
void SetMaximumFlightAltitude(Float32 height);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the kill command delay.
*/
Int32 GetKillCommandDelay(void);

/* ------------------------------------------------------------------------------------------------
 * Modify the kill command delay.
*/
void SetKillCommandDelay(Int32 delay);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the vehicles forced respawn height.
*/
Float32 GetVehiclesForcedRespawnHeight(void);

/* ------------------------------------------------------------------------------------------------
 * Modify the vehicles forced respawn height.
*/
void SetVehiclesForcedRespawnHeight(Float32 height);

/* ------------------------------------------------------------------------------------------------
 * Create a game explosion.
*/
void CreateExplosion(Int32 world, Int32 type, const Vector3 & pos, CPlayer & source, bool grounded);

/* ------------------------------------------------------------------------------------------------
 * Create a game explosion.
*/
void CreateExplosionEx(Int32 world, Int32 type, Float32 x, Float32 y, Float32 z, CPlayer & source, bool grounded);

/* ------------------------------------------------------------------------------------------------
 * Play a game sound.
*/
void PlaySound(Int32 world, Int32 sound, const Vector3 & pos);

/* ------------------------------------------------------------------------------------------------
 * Play a game sound.
*/
void PlaySoundEx(Int32 world, Int32 sound, Float32 x, Float32 y, Float32 z);

/* ------------------------------------------------------------------------------------------------
 * Make a map object invisible.
*/
void HideMapObject(Int32 model, const Vector3 & pos);

/* ------------------------------------------------------------------------------------------------
 * Make a map object invisible.
*/
void HideMapObjectEx(Int32 model, Float32 x, Float32 y, Float32 z);

/* ------------------------------------------------------------------------------------------------
 * Make a map object invisible.
*/
void HideMapObjectRaw(Int32 model, Int16 x, Int16 y, Int16 z);

/* ------------------------------------------------------------------------------------------------
 * Make a map object visible again.
*/
void ShowMapObject(Int32 model, const Vector3 & pos);

/* ------------------------------------------------------------------------------------------------
 * Make a map object visible again.
*/
void ShowMapObjectEx(Int32 model, Float32 x, Float32 y, Float32 z);

/* ------------------------------------------------------------------------------------------------
 * Make a map object visible again.
*/
void ShowMapObjectRaw(Int32 model, Int16 x, Int16 y, Int16 z);

/* ------------------------------------------------------------------------------------------------
 * Make all map objects visible again.
*/
void ShowAllMapObjects(void);

/* ------------------------------------------------------------------------------------------------
 * Retrieve field data of a certain weapon.
*/
SQFloat GetWeaponDataValue(Int32 weapon, Int32 field);

/* ------------------------------------------------------------------------------------------------
 * Modify field data of a certain weapon.
*/
bool SetWeaponDataValue(Int32 weapon, Int32 field, SQFloat value);

/* ------------------------------------------------------------------------------------------------
 * Reset field data of a certain weapon.
*/
bool ResetWeaponDataValue(Int32 weapon, Int32 field);

/* ------------------------------------------------------------------------------------------------
 * See whether field data of a certain weapon was modified.
*/
bool IsWeaponDataValueModified(Int32 weapon, Int32 field);

/* ------------------------------------------------------------------------------------------------
 * Reset all fields data of a certain weapon.
*/
bool ResetWeaponData(Int32 weapon);

/* ------------------------------------------------------------------------------------------------
 * Reset all fields data of a all weapons.
*/
void ResetAllWeaponData();

/* ------------------------------------------------------------------------------------------------
 * Create a new player class.
*/
Int32 AddPlayerClass(Int32 team, const Color3 & color, Int32 skin, const Vector3 & pos, Float32 angle,
                        Int32 wep1, Int32 ammo1, Int32 wep2, Int32 ammo2, Int32 wep3, Int32 ammo3);

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
void SetSpawnPlayerPositionEx(Float32 x, Float32 y, Float32 z);

/* ------------------------------------------------------------------------------------------------
 * Set the camera position when spawning.
*/
void SetSpawnCameraPositionEx(Float32 x, Float32 y, Float32 z);

/* ------------------------------------------------------------------------------------------------
 * Set the camera focus when spawning.
*/
void SetSpawnCameraLookAtEx(Float32 x, Float32 y, Float32 z);

/* ------------------------------------------------------------------------------------------------
 * Ban an IP address from the server.
*/
void BanIP(CSStr addr);

/* ------------------------------------------------------------------------------------------------
 * Unban an IP address from the server.
*/
bool UnbanIP(CSStr addr);

/* ------------------------------------------------------------------------------------------------
 * See if an IP address is banned from the server.
*/
bool IsIPBanned(CSStr addr);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the identifier of the player with the specified name.
*/
Int32 GetPlayerIdFromName(CSStr name);

/* ------------------------------------------------------------------------------------------------
 * See if a player with the specified identifier is connected.
*/
bool IsPlayerConnected(Int32 player_id);

/* ------------------------------------------------------------------------------------------------
 * Force all players on the server to select a class.
*/
void ForceAllSelect();

/* ------------------------------------------------------------------------------------------------
 * See if an entity exists on the server.
*/
bool CheckEntityExists(Int32 type, Int32 index);

} // Namespace:: SqMod

#endif // _MISC_FUNCTIONS_HPP_
