// ------------------------------------------------------------------------------------------------
#include "Core.hpp"
#include "Base/Color3.hpp"
#include "Base/Vector2.hpp"
#include "Entity/Player.hpp"

// ------------------------------------------------------------------------------------------------
#include "Misc/Functions.hpp"
#include "Misc/Model.hpp"
#include "Misc/Player.hpp"
#include "Misc/Vehicle.hpp"
#include "Misc/Weapon.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern SQInteger Player_FindAuto(HSQUIRRELVM vm);
extern SQInteger Player_ExistsAuto(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
extern void Register_Broadcast(HSQUIRRELVM vm);

// ================================================================================================
void Register_Misc(HSQUIRRELVM vm)
{
    Table srvns(vm);

    srvns
    .FmtFunc(_SC("SendLogMessage"), &SendLogMessage)
    .Func(_SC("GetVersion"), &GetServerVersion)
    .Func(_SC("GetSettings"), &GetServerSettings)
    .Func(_SC("GetNumberOfPlugins"), &GetNumberOfPlugins)
    .Func(_SC("GetPluginInfo"), &GetPluginInfo)
    .FmtFunc(_SC("FindPlugin"), &FindPlugin)
    .FmtFunc(_SC("SendPluginCommand"), &SendPluginCommand)
    .Func(_SC("GetTime"), &GetTime)
    .Func(_SC("GetLastError"), &GetLastError)
    .Func(_SC("GetPluginVersion"), &GetPluginVersion)
    .Func(_SC("GetPluginVersionStr"), &GetPluginVersionStr)
    .Func(_SC("GetPluginName"), &GetPluginName)
    .Func(_SC("GetPluginAuthor"), &GetPluginAuthor)
    .Func(_SC("GetPluginID"), &GetPluginID)
    .Func(_SC("GetServerPort"), &GetServerPort)
    .Func(_SC("GetServerFlags"), &GetServerFlags)
    .Func(_SC("GetMaxPlayers"), &GetMaxPlayers)
    .Func(_SC("SetMaxPlayers"), &SetMaxPlayers)
    .Func(_SC("GetServerName"), &GetServerName)
    .FmtFunc(_SC("SetServerName"), &SetServerName)
    .Func(_SC("GetPassword"), &GetServerPassword)
    .FmtFunc(_SC("SetPassword"), &SetServerPassword)
    .Func(_SC("GetGameModeText"), &GetGameModeText)
    .FmtFunc(_SC("SetGameModeText"), &SetGameModeText)
    .FmtFunc(_SC("CreateRadioStream"), &CreateRadioStream)
    .FmtFunc(_SC("CreateRadioStreamEx"), &CreateRadioStreamEx)
    .Func(_SC("RemoveRadioStream"), &RemoveRadioStream)
    .Func(_SC("Shutdown"), &ShutdownServer)
    .Func(_SC("GetOption"), &GetServerOption)
    .Func(_SC("SetOption"), &SetServerOption)
    .Func(_SC("SetOptionEx"), &SetServerOptionEx)
    .Func(_SC("GetWorldBounds"), &GetWorldBounds)
    .Func(_SC("SetWorldBounds"), &SetWorldBounds)
    .Func(_SC("SetWorldBoundsEx"), &SetWorldBoundsEx)
    .Func(_SC("GetWastedSettings"), &GetWastedSettings)
    .Func(_SC("SetWastedSettings"), &SetWastedSettings)
    .Func(_SC("GetTimeRate"), &GetTimeRate)
    .Func(_SC("SetTimeRate"), &SetTimeRate)
    .Func(_SC("GetHour"), &GetHour)
    .Func(_SC("SetHour"), &SetHour)
    .Func(_SC("GetMinute"), &GetMinute)
    .Func(_SC("SetMinute"), &SetMinute)
    .Func(_SC("GetWeather"), &GetWeather)
    .Func(_SC("SetWeather"), &SetWeather)
    .Func(_SC("GetGravity"), &GetGravity)
    .Func(_SC("SetGravity"), &SetGravity)
    .Func(_SC("GetGameSpeed"), &GetGameSpeed)
    .Func(_SC("SetGameSpeed"), &SetGameSpeed)
    .Func(_SC("GetWaterLevel"), &GetWaterLevel)
    .Func(_SC("SetWaterLevel"), &SetWaterLevel)
    .Func(_SC("GetMaximumFlightAltitude"), &GetMaximumFlightAltitude)
    .Func(_SC("SetMaximumFlightAltitude"), &SetMaximumFlightAltitude)
    .Func(_SC("GetKillCommandDelay"), &GetKillCommandDelay)
    .Func(_SC("SetKillCommandDelay"), &SetKillCommandDelay)
    .Func(_SC("GetVehiclesForcedRespawnHeight"), &GetVehiclesForcedRespawnHeight)
    .Func(_SC("SetVehiclesForcedRespawnHeight"), &SetVehiclesForcedRespawnHeight)
    .Func(_SC("CreateExplosion"), &CreateExplosion)
    .Func(_SC("CreateExplosionEx"), &CreateExplosionEx)
    .Func(_SC("PlaySound"), &PlaySound)
    .Func(_SC("PlaySoundEx"), &PlaySoundEx)
    .Func(_SC("PlaySoundForWorld"), &PlaySoundForWorld)
    .Func(_SC("HideMapObject"), &HideMapObject)
    .Func(_SC("HideMapObjectEx"), &HideMapObjectEx)
    .Func(_SC("HideMapObjectRaw"), &HideMapObjectRaw)
    .Func(_SC("ShowMapObject"), &ShowMapObject)
    .Func(_SC("ShowMapObjectEx"), &ShowMapObjectEx)
    .Func(_SC("ShowMapObjectRaw"), &ShowMapObjectRaw)
    .Func(_SC("ShowAllMapObjects"), &ShowAllMapObjects)
    .Func(_SC("GetWeaponDataValue"), &GetWeaponDataValue)
    .Func(_SC("SetWeaponDataValue"), &SetWeaponDataValue)
    .Func(_SC("ResetWeaponDataValue"), &ResetWeaponDataValue)
    .Func(_SC("IsWeaponDataValueModified"), &IsWeaponDataValueModified)
    .Func(_SC("ResetWeaponData"), &ResetWeaponData)
    .Func(_SC("ResetAllWeaponData"), &ResetAllWeaponData)
    .Func(_SC("AddPlayerClass"), &AddPlayerClass)
    .Func(_SC("SetSpawnPlayerPosition"), &SetSpawnPlayerPosition)
    .Func(_SC("SetSpawnCameraPosition"), &SetSpawnCameraPosition)
    .Func(_SC("SetSpawnCameraLookAt"), &SetSpawnCameraLookAt)
    .Func(_SC("SetSpawnPlayerPositionEx"), &SetSpawnPlayerPositionEx)
    .Func(_SC("SetSpawnCameraPositionEx"), &SetSpawnCameraPositionEx)
    .Func(_SC("SetSpawnCameraLookAtEx"), &SetSpawnCameraLookAtEx)
    .FmtFunc(_SC("BanIP"), &BanIP)
    .FmtFunc(_SC("UnbanIP"), &UnbanIP)
    .FmtFunc(_SC("IsIPBanned"), &IsIPBanned)
    .FmtFunc(_SC("GetPlayerIdFromName"), &GetPlayerIdFromName)
    .Func(_SC("IsPlayerConnected"), &IsPlayerConnected)
    .Func(_SC("ForceAllSelect"), &ForceAllSelect)
    .Func(_SC("CheckEntityExists"), &CheckEntityExists)
    .Func(_SC("GetDistrictName"), &GetDistrictName)
    .Func(_SC("GetDistrictNameEx"), &GetDistrictNameEx)
    .Func(_SC("GetFallTimer"), &GetFallTimer)
    .Func(_SC("SetFallTimer"), &SetFallTimer)
#if SQMOD_SDK_LEAST(2, 1)
    .Func(_SC("GetNetworkStatisticsF"), &GetNetworkStatisticsF)
    .Func(_SC("GetNetworkStatisticsI"), &GetNetworkStatisticsI)
#endif
    ; // Uggly? I know. But does the job.

    RootTable(vm).Bind(_SC("SqServer"), srvns);

    RootTable(vm)
    .SquirrelFunc(_SC("FindPlayer"), &Player_FindAuto, -2, ".i|f|s")
    .SquirrelFunc(_SC("PlayerExists"), &Player_ExistsAuto, -2, ".i|f|s")
    .Func(_SC("GetKeyCodeName"), &GetKeyCodeName)
    .FmtFunc(_SC("SetKeyCodeName"), &SetKeyCodeName)
    .Func(_SC("GetModelName"), &GetModelName)
    .FmtFunc(_SC("SetModelName"), &SetModelName)
    .Func(_SC("IsModelWeapon"), &IsModelWeapon)
    .Func(_SC("IsModelActuallyWeapon"), &IsModelActuallyWeapon)
    .Func(_SC("GetSkinName"), &GetSkinName)
    .FmtFunc(_SC("SetSkinName"), &SetSkinName)
    .FmtFunc(_SC("GetSkinID"), &GetSkinID)
    .Func(_SC("IsSkinValid"), &IsSkinValid)
    .Func(_SC("GetAutomobileName"), &GetAutomobileName)
    .FmtFunc(_SC("SetAutomobileName"), &SetAutomobileName)
    .FmtFunc(_SC("GetAutomobileID"), &GetAutomobileID)
    .Func(_SC("IsAutomobileValid"), &IsAutomobileValid)
    .Func(_SC("GetWeaponSlot"), &GetWeaponSlot)
    .Func(_SC("GetWeaponName"), &GetWeaponName)
    .FmtFunc(_SC("SetWeaponName"), &SetWeaponName)
    .Func(_SC("GetCustomWeaponNamePoolSize"), &GetCustomWeaponNamePoolSize)
    .Func(_SC("ClearCustomWeaponNamePool"), &ClearCustomWeaponNamePool)
    .FmtFunc(_SC("GetWeaponID"), &GetWeaponID)
    .Func(_SC("IsWeaponValid"), &IsWeaponValid)
    .Func(_SC("WeaponToModel"), &WeaponToModel)
    .Func(_SC("IsWeaponNatural"), &IsWeaponNatural)
    .Func(_SC("PlaySound"), &PlaySound)
    .Func(_SC("PlaySoundEx"), &PlaySoundEx)
    .Func(_SC("PlaySoundForWorld"), &PlaySoundForWorld)
    .Func(_SC("CreateExplosion"), &CreateExplosion)
    .Func(_SC("CreateExplosionEx"), &CreateExplosionEx);

    Register_Broadcast(vm);
}

} // Namespace:: SqMod
