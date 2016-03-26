#ifndef _MISC_WORLD_HPP_
#define _MISC_WORLD_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void SetTimeRate(Uint32 rate);
Uint32 GetTimeRate(void);

// ------------------------------------------------------------------------------------------------
void SetHour(Int32 hour);
Int32 GetHour(void);

// ------------------------------------------------------------------------------------------------
void SetMinute(Int32 minute);
Int32 GetMinute(void);

// ------------------------------------------------------------------------------------------------
void SetWeather(Int32 weather);
Int32 GetWeather(void);

// ------------------------------------------------------------------------------------------------
void SetGravity(Float32 gravity);
Float32 GetGravity(void);

// ------------------------------------------------------------------------------------------------
void SetGamespeed(Float32 speed);
Float32 GetGamespeed(void);

// ------------------------------------------------------------------------------------------------
void SetWaterLevel(Float32 level);
Float32 GetWaterLevel(void);

// ------------------------------------------------------------------------------------------------
void SetMaxHeight(Float32 height);
Float32 GetMaxHeight(void);

// ------------------------------------------------------------------------------------------------
void SetKillCmdDelay(Int32 delay);
Int32 GetKillCmdDelay(void);

// ------------------------------------------------------------------------------------------------
void SetVehiclesForcedRespawnHeight(Float32 height);
Float32 GetVehiclesForcedRespawnHeight(void);

// ------------------------------------------------------------------------------------------------
void ToggleSyncFrameLimiter(bool toggle);
bool EnabledSyncFrameLimiter(void);
void ToggleFrameLimiter(bool toggle);
bool EnabledFrameLimiter(void);
void ToggleTaxiBoostJump(bool toggle);
bool EnabledTaxiBoostJump(void);
void ToggleDriveOnWater(bool toggle);
bool EnabledDriveOnWater(void);
void ToggleFastSwitch(bool toggle);
bool EnabledFastSwitch(void);
void ToggleFriendlyFire(bool toggle);
bool EnabledFriendlyFire(void);
void ToggleDisableDriveby(bool toggle);
bool EnabledDisableDriveby(void);
void TogglePerfectHandling(bool toggle);
bool EnabledPerfectHandling(void);
void ToggleFlyingCars(bool toggle);
bool EnabledFlyingCars(void);
void ToggleJumpSwitch(bool toggle);
bool EnabledJumpSwitch(void);
void ToggleShowMarkers(bool toggle);
bool EnabledShowMarkers(void);
void ToggleStuntBike(bool toggle);
bool EnabledStuntBike(void);
void ToggleShootInAir(bool toggle);
bool EnabledShootInAir(void);
void ToggleShowNametags(bool toggle);
bool EnabledShowNametags(void);
void ToggleJoinMessages(bool toggle);
bool EnabledJoinMessages(void);
void ToggleDeathMessages(bool toggle);
bool EnabledDeathMessages(void);
void ToggleChatTagsByDefaultEnabled(bool toggle);
bool EnabledChatTagsByDefault(void);

// ------------------------------------------------------------------------------------------------
void CreateExplosion(Int32 world, Int32 type, const Vector3 & pos, CPlayer & source, Uint32 level);
void CreateExplosionEx(Int32 world, Int32 type, Float32 x, Float32 y, Float32 z, CPlayer & source, Uint32 level);

// ------------------------------------------------------------------------------------------------
void HideMapObject(Int32 model, const Vector3 & pos);
void HideMapObjectEx(Int32 model, Float32 x, Float32 y, Float32 z);
void HideMapObjectRaw(Int32 model, Int32 x, Int32 y, Int32 z);

// ------------------------------------------------------------------------------------------------
void ShowMapObject(Int32 model, const Vector3 & pos);
void ShowMapObjectEx(Int32 model, Float32 x, Float32 y, Float32 z);

// ------------------------------------------------------------------------------------------------
void ShowAllMapObjects(void);

// ------------------------------------------------------------------------------------------------
void SetWastedSettings(Uint32 dt, Uint32 ft, Float32 fis, Float32 fos,
                        const Color3 & fc, Uint32 cfs, Uint32 cft);
Table GetWastedSettings();

// ------------------------------------------------------------------------------------------------
void SetWorldBounds(const Vector2 & max, const Vector2 & min);
void SetWorldBoundsEx(Float32 max_x, Float32 max_y, Float32 min_x, Float32 min_y);
Table GetWorldBounds();

} // Namespace:: SqMod

#endif // _MISC_WORLD_HPP_
