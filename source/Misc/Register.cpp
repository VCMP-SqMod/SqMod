// ------------------------------------------------------------------------------------------------
#include "Core.hpp"
#include "Base/Shared.hpp"
#include "Base/Color3.hpp"
#include "Base/Color4.hpp"
#include "Base/Vector2.hpp"
#include "Base/Vector3.hpp"
#include "Entity/Player.hpp"
#include "Library/Numeric/LongInt.hpp"

// ------------------------------------------------------------------------------------------------
#include "Misc/Functions.hpp"
#include "Misc/Model.hpp"
#include "Misc/Player.hpp"
#include "Misc/Vehicle.hpp"
#include "Misc/Weapon.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static const Object & FindPlayer(Object & by)
{
    switch (by.GetType())
    {
        case OT_INTEGER:
        {
            return Core::Get().GetPlayer(by.Cast< Int32 >()).mObj;
        } break;
        case OT_FLOAT:
        {
            return Core::Get().GetPlayer(std::round(by.Cast< Float32 >())).mObj;
        } break;
        case OT_STRING:
        {
            // Obtain the argument as a string
            String str(by.Cast< String >());
            // Attempt to locate the player with this name
            Int32 id = _Func->GetPlayerIdFromName(&str[0]);
            // Was there a player with this name?
            if (VALID_ENTITYEX(id, SQMOD_PLAYER_POOL))
            {
                Core::Get().GetPlayer(id).mObj;
            }
        } break;
        default: STHROWF("Unsupported search identifier");
    }
    // Default to a null object
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqBroadcastMsg(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the message color specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing message color");
    }
    // Was the message value specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing message value");
    }
    // Failed to retrieve a Color4 value
    bool failed = false;
    // The message color
    Color4 color;
    // Attempt to extract the argument values
    try
    {
        color = Var< Color4 >(vm, 2).value;
    }
    catch (...)
    {
        failed = true;
    }
    // Did we failed to retrieve a Color4 instance?
    if (failed)
    {
        // Attempt to extract the argument values
        try
        {
            color = Var< Color3 >(vm, 2).value;
        }
        catch (const Sqrat::Exception & e)
        {
            // Propagate the error
            return sq_throwerror(vm, e.Message().c_str());
        }
    }
    // Attempt to generate the string value
    StackStrF val(vm, 3);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && itr->mInst != nullptr)
        {
            // Grab the player instance
            CPlayer * player = itr->mInst;
            // Send the resulted message string
            const vcmpError result = _Func->SendClientMessage(itr->mID, color.GetRGBA(),
                                                        "%s%s%s",
                                                        player->mMessagePrefix.c_str(),
                                                        val.mPtr,
                                                        player->mMessagePostfix.c_str());
            // Check the result
            if (result == vcmpErrorTooLargeInput)
            {
                return sq_throwerror(vm, ToStrF("Client message too big [%s]", player->GetTag().c_str()));
            }
            // Add this player to the count
            ++count;
        }
    }
    // Push the count count on the stack
    sq_pushinteger(vm, count);
    // Specify that this function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqBroadcastMsgP(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the index of the message prefix specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing prefix index");
    }
    // Was the message value specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing message value");
    }
    // The prefix index
    Uint32 index;
    // Attempt to extract the argument values
    try
    {
        index = Var< Uint32 >(vm, 2).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.Message().c_str());
    }
    // Perform a range check on the specified prefix index
    if (index > SQMOD_PLAYER_MSG_PREFIXES)
    {
        return sq_throwerror(vm, ToStrF("Prefix index is out of range: %u > %u",
                                        index, SQMOD_PLAYER_MSG_PREFIXES));
    }
    // Attempt to generate the string value
    StackStrF val(vm, 3);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    vcmpError result = vcmpErrorNone;
    // Send the resulted message string
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && itr->mInst != nullptr)
        {
            // Grab the player instance
            CPlayer * player = itr->mInst;
            // Send the resulted message string
            if (player->mLimitPrefixPostfixMessage)
            {
                result = _Func->SendClientMessage(itr->mID, player->mMessageColor, "%s%s",
                                                    player->mMessagePrefixes[index].c_str(), val.mPtr);
            }
            else
            {
                result = _Func->SendClientMessage(itr->mID, player->mMessageColor, "%s%s%s%s",
                                                    player->mMessagePrefix.c_str(),
                                                    player->mMessagePrefixes[index].c_str(), val.mPtr,
                                                    player->mMessagePostfix.c_str());
            }
            // Check the result
            if (result == vcmpErrorTooLargeInput)
            {
                return sq_throwerror(vm, ToStrF("Client message too big [%s]", player->GetTag().c_str()));
            }
            // Add this player to the count
            ++count;
        }
    }
    // Push the count count on the stack
    sq_pushinteger(vm, count);
    // Specify that this function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqBroadcastMsgEx(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the message color specified?
    if (top <= 4)
    {
        return sq_throwerror(vm, "Missing message color");
    }
    // Was the message value specified?
    else if (top <= 5)
    {
        return sq_throwerror(vm, "Missing message value");
    }
    // The message color
    Uint32 color;
    // Attempt to extract the argument values
    try
    {
        color = SQMOD_PACK_RGBA(Var< Uint8 >(vm, 2).value,
                                Var< Uint8 >(vm, 3).value,
                                Var< Uint8 >(vm, 4).value,
                                Var< Uint8 >(vm, 5).value);
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.Message().c_str());
    }
    // Attempt to generate the string value
    StackStrF val(vm, 6);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && itr->mInst != nullptr)
        {
            // Grab the player instance
            CPlayer * player = itr->mInst;
            // Send the resulted message string
            const vcmpError result = _Func->SendClientMessage(itr->mID, color,
                                                                "%s%s%s",
                                                                player->mMessagePrefix.c_str(),
                                                                val.mPtr,
                                                                player->mMessagePostfix.c_str());
            // Check the result
            if (result == vcmpErrorTooLargeInput)
            {
                return sq_throwerror(vm, ToStrF("Client message too big [%s]", player->GetTag().c_str()));
            }
            // Add this player to the count
            ++count;
        }
    }
    // Push the count count on the stack
    sq_pushinteger(vm, count);
    // Specify that this function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqBroadcastMessage(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the message value specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing message value");
    }
    // Attempt to generate the string value
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && itr->mInst != nullptr)
        {
            // Grab the player instance
            CPlayer * player = itr->mInst;
            // Send the resulted message string
            const vcmpError result = _Func->SendClientMessage(itr->mID, player->mMessageColor,
                                                                "%s%s%s",
                                                                player->mMessagePrefix.c_str(),
                                                                val.mPtr,
                                                                player->mMessagePostfix.c_str());
            // Check the result
            if (result == vcmpErrorTooLargeInput)
            {
                return sq_throwerror(vm, ToStrF("Client message too big [%s]", player->GetTag().c_str()));
            }
            // Add this player to the count
            ++count;
        }
    }
    // Push the count count on the stack
    sq_pushinteger(vm, count);
    // Specify that this function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqBroadcastAnnounce(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the announcement value specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing announcement value");
    }
    // Attempt to generate the string value
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && itr->mInst != nullptr)
        {
            // Grab the player instance
            CPlayer * player = itr->mInst;
            // Send the resulted announcement string
            const vcmpError result = _Func->SendGameMessage(itr->mID, player->mAnnounceStyle,
                                                            "%s%s%s",
                                                            player->mAnnouncePrefix.c_str(),
                                                            val.mPtr,
                                                            player->mAnnouncePostfix.c_str());
            // Validate the result
            if (result == vcmpErrorArgumentOutOfBounds)
            {
                return sq_throwerror(vm, ToStrF("Invalid announcement style [%s]", player->GetTag().c_str()));
            }
            else if (result == vcmpErrorTooLargeInput)
            {
                return sq_throwerror(vm, ToStrF("Game message too big [%s]", player->GetTag().c_str()));
            }
            // Add this player to the count
            ++count;
        }
    }
    // Push the count count on the stack
    sq_pushinteger(vm, count);
    // Specify that this function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqBroadcastAnnounceEx(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the announcement style specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing announcement style");
    }
    // Was the announcement value specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing announcement value");
    }
    // The announcement style
    Int32 style;
    // style to extract the argument values
    try
    {
        style = Var< Int32 >(vm, 2).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.Message().c_str());
    }
    // Attempt to generate the string value
    StackStrF val(vm, 3);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && itr->mInst != nullptr)
        {
            // Grab the player instance
            CPlayer * player = itr->mInst;
            // Send the resulted announcement string
            const vcmpError result = _Func->SendGameMessage(itr->mID, style,
                                                            "%s%s%s",
                                                            player->mAnnouncePrefix.c_str(),
                                                            val.mPtr,
                                                            player->mAnnouncePostfix.c_str());
            // Validate the result
            if (result == vcmpErrorArgumentOutOfBounds)
            {
                return sq_throwerror(vm, ToStrF("Invalid announcement style [%s]", player->GetTag().c_str()));
            }
            else if (result == vcmpErrorTooLargeInput)
            {
                return sq_throwerror(vm, ToStrF("Game message too big [%s]", player->GetTag().c_str()));
            }
            // Add this player to the count
            ++count;
        }
    }
    // Push the count count on the stack
    sq_pushinteger(vm, count);
    // Specify that this function returned a value
    return 1;
}

// ================================================================================================
void Register_Misc(HSQUIRRELVM vm)
{
    Table srvns(vm);

    srvns.SquirrelFunc(_SC("SendLogMessage"), &SendLogMessage)
    .Func(_SC("GetVersion"), &GetServerVersion)
    .Func(_SC("GetSettings"), &GetServerSettings)
    .Func(_SC("GetNumberOfPlugins"), &GetNumberOfPlugins)
    .Func(_SC("GetPluginInfo"), &GetPluginInfo)
    .Func(_SC("FindPlugin"), &FindPlugin)
    .Func(_SC("SendPluginCommand"), &SendPluginCommand)
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
    .Func(_SC("SetServerName"), &SetServerName)
    .Func(_SC("GetPassword"), &GetServerPassword)
    .Func(_SC("SetPassword"), &SetServerPassword)
    .Func(_SC("GetGameModeText"), &GetGameModeText)
    .Func(_SC("SetGameModeText"), &SetGameModeText)
    .Func(_SC("CreateRadioStream"), &CreateRadioStream)
    .Func(_SC("CreateRadioStreamEx"), &CreateRadioStreamEx)
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
    .Func(_SC("HideMapObject"), &HideMapObject)
    .Func(_SC("HideMapObjectEx"), &SetKeyCodeName)
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
    .Func(_SC("BanIP"), &BanIP)
    .Func(_SC("UnbanIP"), &UnbanIP)
    .Func(_SC("IsIPBanned"), &IsIPBanned)
    .Func(_SC("GetPlayerIdFromName"), &GetPlayerIdFromName)
    .Func(_SC("IsPlayerConnected"), &IsPlayerConnected)
    .Func(_SC("ForceAllSelect"), &ForceAllSelect)
    .Func(_SC("CheckEntityExists"), &SetKeyCodeName);

    RootTable(vm).Bind(_SC("SqServer"), srvns);

    RootTable(vm)
    .Func(_SC("FindPlayer"), &FindPlayer)
    .Func(_SC("GetKeyCodeName"), &GetKeyCodeName)
    .Func(_SC("SetKeyCodeName"), &SetKeyCodeName)
    .Func(_SC("GetModelName"), &GetModelName)
    .Func(_SC("SetModelName"), &SetModelName)
    .Func(_SC("IsModelWeapon"), &IsModelWeapon)
    .Func(_SC("IsModelActuallyWeapon"), &IsModelActuallyWeapon)
    .Func(_SC("GetSkinName"), &GetSkinName)
    .Func(_SC("SetSkinName"), &SetSkinName)
    .Func(_SC("GetSkinID"), &GetSkinID)
    .Func(_SC("IsSkinValid"), &IsSkinValid)
    .Func(_SC("GetAutomobileName"), &GetAutomobileName)
    .Func(_SC("SetAutomobileName"), &SetAutomobileName)
    .Func(_SC("GetAutomobileID"), &GetAutomobileID)
    .Func(_SC("IsAutomobileValid"), &IsAutomobileValid)
    .Func(_SC("GetWeaponName"), &GetWeaponName)
    .Func(_SC("SetWeaponName"), &SetWeaponName)
    .Func(_SC("GetWeaponID"), &GetWeaponID)
    .Func(_SC("IsWeaponValid"), &IsWeaponValid)
    .Func(_SC("WeaponToModel"), &WeaponToModel)
    .Func(_SC("IsWeaponNatural"), &IsWeaponNatural)
    .Func(_SC("PlaySound"), &PlaySound)
    .Func(_SC("PlaySoundEx"), &PlaySoundEx)
    .Func(_SC("CreateExplosion"), &CreateExplosion)
    .Func(_SC("CreateExplosionEx"), &CreateExplosionEx)
    .SquirrelFunc(_SC("SqBroadcastMsg"), &SqBroadcastMsg)
    .SquirrelFunc(_SC("SqBroadcastMsgP"), &SqBroadcastMsgP)
    .SquirrelFunc(_SC("SqBroadcastMsgEx"), &SqBroadcastMsgEx)
    .SquirrelFunc(_SC("SqBroadcastMessage"), &SqBroadcastMessage)
    .SquirrelFunc(_SC("SqBroadcastAnnounce"), &SqBroadcastAnnounce)
    .SquirrelFunc(_SC("SqBroadcastAnnounceEx"), &SqBroadcastAnnounceEx)
    .SquirrelFunc(_SC("SqBroadcastText"), &SqBroadcastAnnounce)
    .SquirrelFunc(_SC("SqBroadcastTextEx"), &SqBroadcastAnnounceEx);
}

} // Namespace:: SqMod
