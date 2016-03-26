// ------------------------------------------------------------------------------------------------
#include "Misc/World.hpp"
#include "Base/Color3.hpp"
#include "Base/Vector2.hpp"
#include "Base/Vector3.hpp"
#include "Entity/Player.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void SetTimeRate(Uint32 rate) { _Func->SetTimeRate(rate); }
Uint32 GetTimeRate(void) { return _Func->GetTimeRate(); }

// ------------------------------------------------------------------------------------------------
void SetHour(Int32 hour) { _Func->SetHour(hour); }
Int32 GetHour(void) { return _Func->GetHour(); }

// ------------------------------------------------------------------------------------------------
void SetMinute(Int32 minute) { _Func->SetMinute(minute); }
Int32 GetMinute(void) { return _Func->GetMinute(); }

// ------------------------------------------------------------------------------------------------
void SetWeather(Int32 weather) { _Func->SetWeather(weather); }
Int32 GetWeather(void) { return _Func->GetWeather(); }

// ------------------------------------------------------------------------------------------------
void SetGravity(Float32 gravity) { _Func->SetGamespeed(gravity); }
Float32 GetGravity(void) { return _Func->GetGamespeed(); }

// ------------------------------------------------------------------------------------------------
void SetGamespeed(Float32 speed) { _Func->SetGamespeed(speed); }
Float32 GetGamespeed(void) { return _Func->GetGamespeed(); }

// ------------------------------------------------------------------------------------------------
void SetWaterLevel(Float32 level) { _Func->SetWaterLevel(level); }
Float32 GetWaterLevel(void) { return _Func->GetWaterLevel(); }

// ------------------------------------------------------------------------------------------------
void SetMaxHeight(Float32 height) { _Func->SetMaxHeight(height); }
Float32 GetMaxHeight(void) { return _Func->GetMaxHeight(); }

// ------------------------------------------------------------------------------------------------
void SetKillCmdDelay(Int32 delay) { _Func->SetKillCmdDelay(delay); }
Int32 GetKillCmdDelay(void) { return _Func->GetKillCmdDelay(); }

// ------------------------------------------------------------------------------------------------
void SetVehiclesForcedRespawnHeight(Float32 height)
{ _Func->SetVehiclesForcedRespawnHeight(height); }
Float32 GetVehiclesForcedRespawnHeight(void)
{ return _Func->GetVehiclesForcedRespawnHeight(); }

// ------------------------------------------------------------------------------------------------
void ToggleSyncFrameLimiter(bool toggle)
{ _Func->ToggleSyncFrameLimiter(toggle); }
bool EnabledSyncFrameLimiter(void)
{ return _Func->EnabledSyncFrameLimiter(); }

// ------------------------------------------------------------------------------------------------
void ToggleFrameLimiter(bool toggle)
{ _Func->ToggleFrameLimiter(toggle); }
bool EnabledFrameLimiter(void)
{ return _Func->EnabledFrameLimiter(); }

// ------------------------------------------------------------------------------------------------
void ToggleTaxiBoostJump(bool toggle)
{ _Func->ToggleTaxiBoostJump(toggle); }
bool EnabledTaxiBoostJump(void)
{ return _Func->EnabledTaxiBoostJump(); }

// ------------------------------------------------------------------------------------------------
void ToggleDriveOnWater(bool toggle)
{ _Func->ToggleDriveOnWater(toggle); }
bool EnabledDriveOnWater(void)
{ return _Func->EnabledDriveOnWater(); }

// ------------------------------------------------------------------------------------------------
void ToggleFastSwitch(bool toggle)
{ _Func->ToggleFastSwitch(toggle); }
bool EnabledFastSwitch(void)
{ return _Func->EnabledFastSwitch(); }

// ------------------------------------------------------------------------------------------------
void ToggleFriendlyFire(bool toggle)
{ _Func->ToggleFriendlyFire(toggle); }
bool EnabledFriendlyFire(void)
{ return _Func->EnabledFriendlyFire(); }

// ------------------------------------------------------------------------------------------------
void ToggleDisableDriveby(bool toggle)
{ _Func->ToggleDisableDriveby(toggle); }
bool EnabledDisableDriveby(void)
{ return _Func->EnabledDisableDriveby(); }

// ------------------------------------------------------------------------------------------------
void TogglePerfectHandling(bool toggle)
{ _Func->TogglePerfectHandling(toggle); }
bool EnabledPerfectHandling(void)
{ return _Func->EnabledPerfectHandling(); }

// ------------------------------------------------------------------------------------------------
void ToggleFlyingCars(bool toggle)
{ _Func->ToggleFlyingCars(toggle); }
bool EnabledFlyingCars(void)
{ return _Func->EnabledFlyingCars(); }

// ------------------------------------------------------------------------------------------------
void ToggleJumpSwitch(bool toggle)
{ _Func->ToggleJumpSwitch(toggle); }
bool EnabledJumpSwitch(void)
{ return _Func->EnabledJumpSwitch(); }

// ------------------------------------------------------------------------------------------------
void ToggleShowMarkers(bool toggle)
{ _Func->ToggleShowMarkers(toggle); }
bool EnabledShowMarkers(void)
{ return _Func->EnabledShowMarkers(); }

// ------------------------------------------------------------------------------------------------
void ToggleStuntBike(bool toggle)
{ _Func->ToggleStuntBike(toggle); }
bool EnabledStuntBike(void)
{ return _Func->EnabledStuntBike(); }

// ------------------------------------------------------------------------------------------------
void ToggleShootInAir(bool toggle)
{ _Func->ToggleShootInAir(toggle); }
bool EnabledShootInAir(void)
{ return _Func->EnabledShootInAir(); }

// ------------------------------------------------------------------------------------------------
void ToggleShowNametags(bool toggle)
{ _Func->ToggleShowNametags(toggle); }
bool EnabledShowNametags(void)
{ return _Func->EnabledShowNametags(); }

// ------------------------------------------------------------------------------------------------
void ToggleJoinMessages(bool toggle)
{ _Func->ToggleJoinMessages(toggle); }
bool EnabledJoinMessages(void)
{ return _Func->EnabledJoinMessages(); }

// ------------------------------------------------------------------------------------------------
void ToggleDeathMessages(bool toggle)
{ _Func->ToggleDeathMessages(toggle); }
bool EnabledDeathMessages(void)
{ return _Func->EnabledDeathMessages(); }

// ------------------------------------------------------------------------------------------------
void ToggleChatTagsByDefaultEnabled(bool toggle)
{ _Func->ToggleChatTagsByDefaultEnabled(toggle); }
bool EnabledChatTagsByDefault(void)
{ return _Func->EnabledChatTagsByDefault(); }

// ------------------------------------------------------------------------------------------------
void CreateExplosion(Int32 world, Int32 type, const Vector3 & pos, CPlayer & source, Uint32 level)
{
    // Validate the specified player
    source.Validate();
    // Perform the requested operation
    _Func->CreateExplosion(world, type, pos.x, pos.y, pos.z, source.GetID(), level);
}

void CreateExplosionEx(Int32 world, Int32 type, Float32 x, Float32 y, Float32 z, CPlayer & source, Uint32 level)
{
    // Validate the specified player
    source.Validate();
    // Perform the requested operation
    _Func->CreateExplosion(world, type, x, y, z, source.GetID(), level);
}

// ------------------------------------------------------------------------------------------------
void HideMapObject(Int32 model, const Vector3 & pos)
{
    _Func->HideMapObject(model,
        static_cast< Int32 >(std::floor(pos.x * 10.0f) + 0.5f),
        static_cast< Int32 >(std::floor(pos.y * 10.0f) + 0.5f),
        static_cast< Int32 >(std::floor(pos.z * 10.0f) + 0.5f)
    );
}

// ------------------------------------------------------------------------------------------------
void HideMapObjectEx(Int32 model, Float32 x, Float32 y, Float32 z)
{
    _Func->HideMapObject(model,
        static_cast< Int32 >(std::floor(x * 10.0f) + 0.5f),
        static_cast< Int32 >(std::floor(y * 10.0f) + 0.5f),
        static_cast< Int32 >(std::floor(z * 10.0f) + 0.5f)
    );
}

// ------------------------------------------------------------------------------------------------
void HideMapObjectRaw(Int32 model, Int32 x, Int32 y, Int32 z)
{
    _Func->HideMapObject(model, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void ShowMapObject(Int32 model, const Vector3 & pos)
{ _Func->ShowMapObject(model, pos.x, pos.y, pos.z); }
void ShowMapObjectEx(Int32 model, Float32 x, Float32 y, Float32 z)
{ _Func->ShowMapObject(model, x, y, z); }

// ------------------------------------------------------------------------------------------------
void ShowAllMapObjects(void)
{ _Func->ShowAllMapObjects(); }

// ------------------------------------------------------------------------------------------------
void SetWastedSettings(Uint32 dt, Uint32 ft, Float32 fis, Float32 fos,
                        const Color3 & fc, Uint32 cfs, Uint32 cft)
{
    _Func->SetWastedSettings(dt, ft, fis, fos, fc.GetRGB(), cfs, cft);
}

Table GetWastedSettings()
{
    Uint32 fc, dt, ft, cfs, cft;
    Float32 fis, fos;
    Color3 c;
    _Func->GetWastedSettings(&dt, &ft, &fis, &fos, &fc, &cfs, &cft);
    c.SetRGB(fc);
    Table t(DefaultVM::Get());
    t.SetValue(_SC("dt"), dt);
    t.SetValue(_SC("ft"), ft);
    t.SetValue(_SC("fis"), fis);
    t.SetValue(_SC("fos"), fos);
    t.SetValue(_SC("fc"), c);
    t.SetValue(_SC("cfs"), cfs);
    t.SetValue(_SC("cft"), cft);
    return t;
}

// ------------------------------------------------------------------------------------------------
void SetWorldBounds(const Vector2 & max, const Vector2 & min)
{
    _Func->SetWorldBounds(max.x, min.x, max.y, min.y);
}

void SetWorldBoundsEx(Float32 max_x, Float32 max_y, Float32 min_x, Float32 min_y)
{
    _Func->SetWorldBounds(max_x, min_x, max_y, min_y);
}

Table GetWorldBounds()
{
    Vector2 max, min;
    _Func->GetWorldBounds(&max.x, &min.x, &max.y, &min.y);
    Table t(DefaultVM::Get());
    t.SetValue(_SC("max"),max);
    t.SetValue(_SC("min"), min);
    return t;
}

} // Namespace:: SqMod
