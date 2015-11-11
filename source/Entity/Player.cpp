#include "Entity/Player.hpp"
#include "Base/Color3.hpp"
#include "Misc/Skin.hpp"
#include "Misc/Weapon.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqstdstring.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CSkin    CPlayer::s_Skin;
CWeapon  CPlayer::s_Weapon;

// ------------------------------------------------------------------------------------------------
Color3   CPlayer::s_Color3;
Vector3  CPlayer::s_Vector3;

// ------------------------------------------------------------------------------------------------
SQChar   CPlayer::s_Buffer[MAX_PLAYER_TEMPORARY_BUFFER];

// ------------------------------------------------------------------------------------------------
CPlayer::CPlayer(const Reference< CPlayer > & o)
    : Reference(o)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetLevel() const
{
    if (VALID_ENTITY(m_ID))
    {
        return Get(m_ID).Level;
    }
    else
    {
        BadRef("@level", "get level");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetLevel(SQInt32 val) const
{
    if (VALID_ENTITY(m_ID))
    {
        Get(m_ID).Level = val;
    }
    else
    {
        BadRef("@level", "set level");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetLocalPrefixes() const
{
    if (VALID_ENTITY(m_ID))
    {
        return Get(m_ID).LocalPrefixes;
    }
    else
    {
        BadRef("@lprefix", "see whether it uses local message prefixes");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetLocalPrefixes(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        Get(m_ID).LocalPrefixes = toggle;
    }
    else
    {
        BadRef("@lprefix", "et whether it uses local message prefixes");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetLocalMessageColor() const
{
    if (VALID_ENTITY(m_ID))
    {
        return Get(m_ID).LocalMessageColor;
    }
    else
    {
        BadRef("@lmesage_color", "see whether it uses local message color");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetLocalMessageColor(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        Get(m_ID).LocalMessageColor = toggle;
    }
    else
    {
        BadRef("@lmesage_color", "set whether it uses local message color");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetLocalAnnounceStyle() const
{
    if (VALID_ENTITY(m_ID))
    {
        return Get(m_ID).LocalAnnounceStyle;
    }
    else
    {
        BadRef("@lannounce_style", "see whether it uses local announce style");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetLocalAnnounceStyle(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        Get(m_ID).LocalAnnounceStyle = toggle;
    }
    else
    {
        BadRef("@lannounce_style", "set whether it uses local announce style");
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * CPlayer::GetMessagePrefix(SQUint32 index) const
{
    if (VALID_ENTITY(m_ID) && index < MAX_PLAYER_MESSAGE_PREFIXES)
    {
        return Get(m_ID).Prefixes[index].c_str();
    }
    else if (index >= MAX_PLAYER_MESSAGE_PREFIXES)
    {
        BadArg("get_msg_prefix", "get local message prefix", "using an out of bounds index", index);
    }
    else
    {
        BadRef("get_msg_prefix", "get local message prefix");
    }

    return _SC("");
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetMessagePrefix(SQUint32 index, const SQChar * prefix) const
{
    if (VALID_ENTITY(m_ID) && index < MAX_PLAYER_MESSAGE_PREFIXES)
    {
        Get(m_ID).Prefixes[index].assign(prefix);
    }
    else if (index >= MAX_PLAYER_MESSAGE_PREFIXES)
    {
        BadArg("set_msg_prefix", "set local message prefix", "using an out of bounds index", index);
    }
    else
    {
        BadRef("set_msg_prefix", "set local message prefix");
    }
}

// ------------------------------------------------------------------------------------------------
SQUint32 CPlayer::GetMessageColor() const
{
    if (VALID_ENTITY(m_ID))
    {
        return Get(m_ID).MessageColor;
    }
    else
    {
        BadRef("@message_color", "get local message color");
    }

    return 0x0;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetMessageColor(SQUint32 color) const
{
    if (VALID_ENTITY(m_ID))
    {
        Get(m_ID).MessageColor = color;
    }
    else
    {
        BadRef("@message_color", "set local message color");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetAnnounceStyle() const
{
    if (VALID_ENTITY(m_ID))
    {
        return Get(m_ID).AnnounceStyle;
    }
    else
    {
        BadRef("@announce_style", "get local announce style");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetAnnounceStyle(SQInt32 style) const
{
    if (VALID_ENTITY(m_ID))
    {
        Get(m_ID).AnnounceStyle = style;
    }
    else
    {
        BadRef("@announce_style", "set local announce style");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsStreamedFor(const Reference < CPlayer > & player) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        return _Func->IsPlayerStreamedForPlayer(m_ID, player);
    }
    else if (!player)
    {
        BadArg("streamed_for", "see whether is streamed for player", _SCI32(player));
    }
    else
    {
        BadRef("streamed_for", "see whether is streamed for player");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetClass() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerClass(m_ID);
    }
    else
    {
        BadRef("@class_id", "get class");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetAdmin() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->IsPlayerAdmin(m_ID);
    }
    else
    {
        BadRef("@admin", "see whether is admin");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetAdmin(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerAdmin(m_ID, toggle);
    }
    else
    {
        BadRef("@admin", "set whether is admin");
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * CPlayer::GetIP() const
{
    // Clear any previous ip address
    s_Buffer[0] = '\0';
    // Attempt to retrieve the ip address
    if (VALID_ENTITY(m_ID))
    {
        std::memset(s_Buffer, '\0', sizeof(s_Buffer));
        _Func->GetPlayerIP(m_ID, s_Buffer, sizeof(s_Buffer));
    }
    else
    {
        BadRef("@ip", "get ip address");
    }
    // Return the ip address that could be retrieved
    return s_Buffer;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::Kick() const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->KickPlayer(m_ID);
    }
    else
    {
        BadRef("kick", "kick client");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::Ban() const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->BanPlayer(m_ID);
    }
    else
    {
        BadRef("ban", "ban client");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsConnected() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->IsPlayerConnected(m_ID);
    }
    else
    {
        BadRef("@connected", "see whether is connected");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsSpawned() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->IsPlayerSpawned(m_ID);
    }
    else
    {
        BadRef("@spawned", "see whether is spawned");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
SQUint32 CPlayer::GetKey() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerKey(m_ID);
    }
    else
    {
        BadRef("@key", "get key");
    }

    return SQMOD_BLANK;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetWorld() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerWorld(m_ID);
    }
    else
    {
        BadRef("@world", "get world");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetWorld(SQInt32 world) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerWorld(m_ID, world);
    }
    else
    {
        BadRef("@world", "set world");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetSecWorld() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerSecWorld(m_ID);
    }
    else
    {
        BadRef("@sec_world", "<get secondary world");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetSecWorld(SQInt32 world) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerSecWorld(m_ID, world);
    }
    else
    {
        BadRef("@sec_world", "set secondary world");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetUniqueWorld() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerUniqueWorld(m_ID);
    }
    else
    {
        BadRef("@unique_world", "get unique world");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsWorldCompatible(SQInt32 world) const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->IsPlayerWorldCompatible(m_ID, world);
    }
    else
    {
        BadRef("world_compatible", "see whether is compatible with world");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetState() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerState(m_ID);
    }
    else
    {
        BadRef("@state", "get current state");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
const SQChar * CPlayer::GetName() const
{
    // Clear any previous ip address
    s_Buffer[0] = '\0';
    // Attempt to retrieve the ip address
    if (VALID_ENTITY(m_ID))
    {
        std::memset(s_Buffer, '\0', sizeof(s_Buffer));
        _Func->GetPlayerName(m_ID, s_Buffer, sizeof(s_Buffer));
    }
    else
    {
        BadRef("@name", "get name");
    }
    // Return the ip address that could be retrieved
    return s_Buffer;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetName(const SQChar * name) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerName(m_ID, static_cast< const char * >(name));
    }
    else
    {
        BadRef("@name", "set name");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetTeam() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerTeam(m_ID);
    }
    else
    {
        BadRef("@team", "get team");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetTeam(SQInt32 team) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerTeam(m_ID, team);
    }
    else
    {
        BadRef("@team", "set team");
    }
}

// ------------------------------------------------------------------------------------------------
const CSkin & CPlayer::GetSkin() const
{
    // Clear any previous skin
    s_Skin.SetID(SQMOD_UNKNOWN);
    // Attempt to retrieve the skin
    if (VALID_ENTITY(m_ID))
    {
        s_Skin.SetID(_Func->GetPlayerSkin(m_ID));
    }
    else
    {
        BadRef("@skin", "get skin");
    }
    // Return the skin that could be retrieved
    return s_Skin;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetSkin(const CSkin & skin) const
{
    if (VALID_ENTITY(m_ID) && skin)
    {
        _Func->SetPlayerSkin(m_ID, skin);
    }
    else if (!skin)
    {
        BadArg("@skin", "set skin", _SCI32(skin));
    }
    else
    {
        BadRef("@skin", "set skin");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetSkinID() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerSkin(m_ID);
    }
    else
    {
        BadRef("@skin_id", "get skin id");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetSkinID(SQInt32 skin) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerSkin(m_ID, skin);
    }
    else
    {
        BadRef("@skin_id", "set skin id");
    }
}

// ------------------------------------------------------------------------------------------------
const Color3 & CPlayer::GetColor() const
{
    // Clear any previous color
    s_Color3.Clear();
    // Attempt to retrieve the color
    if (VALID_ENTITY(m_ID))
    {
        s_Color3.SetRGB(_Func->GetPlayerColour(m_ID));
    }
    else
    {
        BadRef("@color", "get color");
    }
    // Return the color that could be retrieved
    return s_Color3;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetColor(const Color3 & color) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerColour(m_ID, color.GetRGB());
    }
    else
    {
        BadRef("@color", "set color");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetColorEx(Uint8 r, Uint8 g, Uint8 b) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerColour(m_ID, PACK_RGB(r, g, b));
    }
    else
    {
        BadRef("set_color", "set color");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::ForceSpawn() const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->ForcePlayerSpawn(m_ID);
    }
    else
    {
        BadRef("spawn", "force to spawn");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::ForceSelect() const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->ForcePlayerSelect(m_ID);
    }
    else
    {
        BadRef("@select", "force to select");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetMoney() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerMoney(m_ID);
    }
    else
    {
        BadRef("@money", "get money");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetMoney(SQInt32 amount) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerMoney(m_ID, amount);
    }
    else
    {
        BadRef("@money", "set money");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::GiveMoney(SQInt32 amount) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->GivePlayerMoney(m_ID, amount);
    }
    else
    {
        BadRef("give_money", "give money");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetScore() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerScore(m_ID);
    }
    else
    {
        BadRef("@score", "get score");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetScore(SQInt32 score) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerScore(m_ID, score);
    }
    else
    {
        BadRef("@score", "set score");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetPing() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerPing(m_ID);
    }
    else
    {
        BadRef("@ping", "get ping");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQFloat CPlayer::GetFPS() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerFPS(m_ID);
    }
    else
    {
        BadRef("@fps", "get fps");
    }

    return 0.0;
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsTyping() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->IsPlayerTyping(m_ID);
    }
    else
    {
        BadRef("@typing", "see whether is typing");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
const SQChar * CPlayer::GetUID() const
{
    // Clear any previous uid
    s_Buffer[0] = '\0';
    // Attempt to retrieve the uid
    if (VALID_ENTITY(m_ID))
    {
        std::memset(s_Buffer, '\0', sizeof(s_Buffer));
        _Func->GetPlayerUID(m_ID, s_Buffer, sizeof(s_Buffer));
    }
    else
    {
        BadRef("@uid", "get unique identifier");
    }
    // Return the uid that could be retrieved
    return s_Buffer;
}

// ------------------------------------------------------------------------------------------------
const SQChar * CPlayer::GetUID2() const
{
    // Clear any previous uid2
    s_Buffer[0] = '\0';
    // Attempt to retrieve the uid2
    if (VALID_ENTITY(m_ID))
    {
        std::memset(s_Buffer, '\0', sizeof(s_Buffer));
        _Func->GetPlayerUID2(m_ID, s_Buffer, sizeof(s_Buffer));
    }
    else
    {
        BadRef("@uid2", "get unique identifier version 2");
    }
    // Return the uid2 that could be retrieved
    return s_Buffer;
}

// ------------------------------------------------------------------------------------------------
SQFloat CPlayer::GetHealth() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerHealth(m_ID);
    }
    else
    {
        BadRef("@health", "get health");
    }

    return 0.0;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetHealth(SQFloat amount) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerHealth(m_ID, amount);
    }
    else
    {
        BadRef("@health", "set health");
    }
}

// ------------------------------------------------------------------------------------------------
SQFloat CPlayer::GetArmour() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerArmour(m_ID);
    }
    else
    {
        BadRef("@armour", "get armour");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetArmour(SQFloat amount) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerArmour(m_ID, amount);
    }
    else
    {
        BadRef("@armour", "set armour");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetImmunity() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerImmunityFlags(m_ID);
    }
    else
    {
        BadRef("@immunity", "get immunity flags");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetImmunity(SQInt32 flags) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerImmunityFlags(m_ID, flags);
    }
    else
    {
        BadRef("@immunity", "set player flags");
    }
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CPlayer::GetPosition() const
{
    // Clear any previous position
    s_Vector3.Clear();
    // Attempt to retrieve the position
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetPlayerPos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    }
    else
    {
        BadRef("@position", "get position");
    }
    // Return the position that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetPosition(const Vector3 & pos) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerPos(m_ID, pos.x, pos.y, pos.z);
    }
    else
    {
        BadRef("@position", "set position");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetPositionEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerPos(m_ID, x, y, z);
    }
    else
    {
        BadRef("set_position", "set position");
    }
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CPlayer::GetSpeed() const
{
    // Clear any previous speed
    s_Vector3.Clear();
    // Attempt to retrieve the speed
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetPlayerSpeed(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    }
    else
    {
        BadRef("@speed", "get speed");
    }
    // Return the speed that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetSpeed(const Vector3 & vel) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerSpeed(m_ID, vel.x, vel.y, vel.z);
    }
    else
    {
        BadRef("@speed", "set speed");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetSpeedEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerSpeed(m_ID, x, y, z);
    }
    else
    {
        BadRef("set_speed", "set speed");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::AddSpeed(const Vector3 & vel) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->AddPlayerSpeed(m_ID, vel.x, vel.y, vel.z);
    }
    else
    {
        BadRef("add_speed", "add speed");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::AddSpeedEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->AddPlayerSpeed(m_ID, x, y, z);
    }
    else
    {
        BadRef("@add_speed", "add speed");
    }
}

// ------------------------------------------------------------------------------------------------
SQFloat CPlayer::GetHeading() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerHeading(m_ID);
    }
    else
    {
        BadRef("@heading", "get heading");
    }
    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetHeading(SQFloat angle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerHeading(m_ID, angle);
    }
    else
    {
        BadRef("@heading", "set heading");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetAlpha() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerAlpha(m_ID);
    }
    else
    {
        BadRef("@alpha", "get alpha");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetAlpha(SQInt32 alpha, SQInt32 fade) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerAlpha(m_ID, alpha, fade);
    }
    else
    {
        BadRef("@alpha", "set alpha");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetVehicleStatus() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerInVehicleStatus(m_ID);
    }
    else
    {
        BadRef("@vehicle_status", "get vehicle status");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetOccupiedSlot() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerInVehicleSlot(m_ID);
    }
    else
    {
        BadRef("@slot", "get occupied slot");
        DbgWrn("Attempting to <> using an invalid reference: %d", m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
Reference < CVehicle > CPlayer::GetVehicle() const
{
    if (VALID_ENTITY(m_ID))
    {
        return Reference < CVehicle >(_Func->GetPlayerVehicleID(m_ID));
    }
    else
    {
        BadRef("@vehicle", "get embarked vehicle");
    }

    return Reference < CVehicle >();
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetVehicleID() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerVehicleID(m_ID);
    }
    else
    {
        BadRef("@vehicle_id", "get embarked vehicle id");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetControllable() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->EnabledPlayerControllable(m_ID);
    }
    else
    {
        BadRef("@controllable", "see whether is controllable");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetControllable(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->TogglePlayerControllable(m_ID, toggle);
    }
    else
    {
        BadRef("@controllable", "set whether is controllable");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetDriveby() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->EnabledPlayerDriveby(m_ID);
    }
    else
    {
        BadRef("@driveby", "see whether can driveby");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetDriveby(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->TogglePlayerDriveby(m_ID, toggle);
    }
    else
    {
        BadRef("@driveby", "set whether can driveby");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetWhiteScanlines() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->EnabledPlayerWhiteScanlines(m_ID);
    }
    else
    {
        BadRef("@white_scanlines", "see whether has white scanlines");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetWhiteScanlines(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->TogglePlayerWhiteScanlines(m_ID, toggle);
    }
    else
    {
        BadRef("@white_scanlines", "set whether has white scanlines");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetGreenScanlines() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->EnabledPlayerGreenScanlines(m_ID);
    }
    else
    {
        BadRef("@green_scanlines", "see whether has green scanlines");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetGreenScanlines(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->TogglePlayerGreenScanlines(m_ID, toggle);
    }
    else
    {
        BadRef("@green_scanlines", "set whether has green scanlines");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetWidescreen() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->EnabledPlayerWidescreen(m_ID);
    }
    else
    {
        BadRef("@widescreen", "see whether has widescreen");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetWidescreen(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->TogglePlayerWidescreen(m_ID, toggle);
    }
    else
    {
        BadRef("@widescreen", "set whether has widescreen");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetShowMarkers() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->EnabledPlayerShowMarkers(m_ID);
    }
    else
    {
        BadRef("@show_markers", "see whether displays markers");
    }
    return false;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetShowMarkers(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->TogglePlayerShowMarkers(m_ID, toggle);
    }
    else
    {
        BadRef("@show_markers", "set whether displays markers");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetAttackPriv() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->EnabledPlayerAttackPriv(m_ID);
    }
    else
    {
        BadRef("@attack_priv", "see whether has attack privilege");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetAttackPriv(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->TogglePlayerAttackPriv(m_ID, toggle);
    }
    else
    {
        BadRef("@attack_priv", "set whether has attack privilege");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetHasMarker() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->EnabledPlayerHasMarker(m_ID);
    }
    else
    {
        BadRef("@has_marker", "see whether has marker");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetHasMarker(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->TogglePlayerHasMarker(m_ID, toggle);
    }
    else
    {
        BadRef("@has_marker", "set whether has marker");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetChatTags() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->EnabledPlayerChatTags(m_ID);
    }
    else
    {
        BadRef("@chat_tags", "see whether has chat tags");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetChatTags(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->TogglePlayerChatTagsEnabled(m_ID, toggle);
    }
    else
    {
        BadRef("@chat_tags", "set whether has chat tags");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetDrunkEffects() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->EnabledPlayerDrunkEffects(m_ID);
    }
    else
    {
        BadRef("@drunk_effects", "see whether is under drunk effects");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetDrunkEffects(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->TogglePlayerDrunkEffects(m_ID, toggle);
    }
    else
    {
        BadRef("@drunk_effects", "set whether is under drunk effects");
    }
}

// ------------------------------------------------------------------------------------------------
const CWeapon & CPlayer::GetWeapon() const
{
    // Clear any previous weapon
    s_Weapon.SetID(SQMOD_UNKNOWN);
    // Attempt to retrieve the weapon
    if (VALID_ENTITY(m_ID))
    {
        s_Weapon.SetID(_Func->GetPlayerWeapon(m_ID));
    }
    else
    {
        BadRef("@weapon", "get weapon");
    }
    // Return the model that could be weapon
    return s_Weapon;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetWeapon(const CWeapon & wep) const
{
    if (VALID_ENTITY(m_ID) && wep)
    {
        _Func->SetPlayerWeapon(m_ID, wep, wep.GetAmmo());
    }
    else if (!wep)
    {
        BadArg("@weapon", "set weapon", _SCI32(wep));
    }
    else
    {
        BadRef("@weapon", "set weapon");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetWeaponEx(const CWeapon & wep, SQInt32 ammo) const
{
    if (VALID_ENTITY(m_ID) && wep)
    {
        _Func->SetPlayerWeapon(m_ID, wep, ammo);
    }
    else if (!wep)
    {
        BadArg("set_weapon", "set weapon", _SCI32(wep));
    }
    else
    {
        BadRef("set_weapon", "set weapon");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetWeaponID() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerWeapon(m_ID);
    }
    else
    {
        BadRef("@weapon_id", "get weapon id");
    }
    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetWeaponID(SQInt32 wep) const
{
    if (VALID_ENTITY(m_ID) && VALID_ENTITY(wep))
    {
        _Func->SetPlayerWeapon(m_ID, wep, 0);
    }
    else if (!VALID_ENTITY(wep))
    {
        BadArg("@weapon_id", "set weapon", wep);
    }
    else
    {
        BadRef("@weapon_id", "set weapon");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetWeaponIDEx(SQInt32 wep, SQInt32 ammo) const
{
    if (VALID_ENTITY(m_ID) && VALID_ENTITY(wep))
    {
        _Func->SetPlayerWeapon(m_ID, wep, ammo);
    }
    else if (!VALID_ENTITY(wep))
    {
        BadArg("set_weapon_id", "set weapon", wep);
    }
    else
    {
        BadRef("set_weapon_id", "set weapon");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::GiveWeapon(const CWeapon & wep) const
{
    if (VALID_ENTITY(m_ID) && wep)
    {
        _Func->GivePlayerWeapon(m_ID, wep, wep.GetAmmo());
    }
    else if (!wep)
    {
        BadArg("give_weapon", "give weapon", _SCI32(wep));
    }
    else
    {
        BadRef("give_weapon", "give weapon");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::GiveWeaponEx(const CWeapon & wep, SQInt32 ammo) const
{
    if (VALID_ENTITY(m_ID) && wep)
    {
        _Func->GivePlayerWeapon(m_ID, wep, ammo);
    }
    else if (!wep)
    {
        BadArg("give_weapon", "give weapon", _SCI32(wep));
    }
    else
    {
        BadRef("give_weapon", "give weapon");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::GiveWeaponIDEx(SQInt32 wep, SQInt32 ammo) const
{
    if (VALID_ENTITY(m_ID) && VALID_ENTITY(wep))
    {
        _Func->GivePlayerWeapon(m_ID, wep, ammo);
    }
    else if (!VALID_ENTITY(wep))
    {
        BadArg("give_weapon_id", "give weapon", wep);
    }
    else
    {
        BadRef("give_weapon_id", "give weapon");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::StripWeapons() const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RemoveAllWeapons(m_ID);
    }
    else
    {
        BadRef("strip_weapons", "strip of weapons");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetCameraPosition(const Vector3 & pos, const Vector3 & aim) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetCameraPosition(m_ID, pos.x, pos.y, pos.z, aim.x, aim.y, aim.z);
    }
    else
    {
        BadRef("camera_position", "set camera position");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetCameraPosition(SQFloat xp, SQFloat yp, SQFloat zp, SQFloat xa, SQFloat ya, SQFloat za) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetCameraPosition(m_ID, xp, yp, zp, xa, ya, za);
    }
    else
    {
        BadRef("camera_position", "set camera position");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::RestoreCamera() const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RestoreCamera(m_ID);
    }
    else
    {
        BadRef("restore_camera", "restore camera position");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsCameraLocked() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->IsCameraLocked(m_ID);
    }
    else
    {
        BadRef("@camera_locked", "see whether has camera locked");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetAnimation(SQInt32 group, SQInt32 anim) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerAnimation(m_ID, group, anim);
    }
    else
    {
        BadRef("animation", "set animation");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetWantedLevel() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerWantedLevel(m_ID);
    }
    else
    {
        BadRef("@wanted_level", "get wanted level");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetWantedLevel(SQInt32 level) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPlayerWantedLevel(m_ID, level);
    }
    else
    {
        BadRef("@wanted_level", "set wanted level");
    }
}

// ------------------------------------------------------------------------------------------------
Reference < CVehicle > CPlayer::StandingOnVehicle() const
{
    if (VALID_ENTITY(m_ID))
    {
        return Reference < CVehicle >(_Func->GetPlayerStandingOnVehicle(m_ID));
    }
    else
    {
        BadRef("@touched_vehicle", "get the vehicle on which the is standing");
    }

    return Reference < CVehicle >();
}

// ------------------------------------------------------------------------------------------------
Reference < CObject > CPlayer::StandingOnObject() const
{
    if (VALID_ENTITY(m_ID))
    {
        return Reference < CObject >(_Func->GetPlayerStandingOnObject(m_ID));
    }
    else
    {
        BadRef("@touched_object", "get the object on which the is standing");
    }

    return Reference < CObject >();
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsAway() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->IsPlayerAway(m_ID);
    }
    else
    {
        BadRef("@away", "see whether is away");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
Reference < CPlayer > CPlayer::GetSpectator() const
{
    if (VALID_ENTITY(m_ID))
    {
        return Reference < CPlayer >(_Func->GetPlayerSpectateTarget(m_ID));
    }
    else
    {
        BadRef("@spec", "get spectated player");
    }

    return Reference < CPlayer >();
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetSpectator(const Reference < CPlayer > & target) const
{
    if (VALID_ENTITY(m_ID) && target)
    {
        _Func->SetPlayerSpectateTarget(m_ID, target);
    }
    else if (!target)
    {
        BadArg("@spec", "set spectated player", _SCI32(target));
    }
    else
    {
        BadRef("@spec", "set spectated player");
    }
}

// ------------------------------------------------------------------------------------------------
Reference < CPlayer > CPlayer::Spectating() const
{
    if (VALID_ENTITY(m_ID))
    {
        return Reference < CPlayer >(_Func->GetPlayerSpectateTarget(m_ID));
    }
    else
    {
        BadRef("@spectating", "get spectated player");
    }

    return Reference < CPlayer >();
}

// ------------------------------------------------------------------------------------------------
void CPlayer::Spectate(const Reference < CPlayer > & target) const
{
    if (VALID_ENTITY(m_ID) && target)
    {
        _Func->SetPlayerSpectateTarget(m_ID, target);
    }
    else if (!target)
    {
        BadArg("spectate", "set spectated player", _SCI32(target));
    }
    else
    {
        BadRef("spectate", "set spectated player");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsBurning() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerOnFireStatus(m_ID);
    }
    else
    {
        BadRef("@burning", "see whether is burning");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsCrouched() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerCrouchStatus(m_ID);
    }
    else
    {
        BadRef("@crouched", "see whether is crouched");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetAction() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerAction(m_ID);
    }
    else
    {
        BadRef("@action", "get current action");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPlayer::GetGameKeys() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerGameKeys(m_ID);
    }
    else
    {
        BadRef("@game_keys", "get game keys");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CPlayer::GetAimPos() const
{
    // Clear any previous aim position
    s_Vector3.Clear();
    // Attempt to retrieve the aim position
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetPlayerAimPos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    }
    else
    {
        BadRef("@aim_position", "get aim position");
    }
    // Return the aim position that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CPlayer::GetAimDir() const
{
    // Clear any previous aim direction
    s_Vector3.Clear();
    // Attempt to retrieve the aim direction
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetPlayerAimDir(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    }
    else
    {
        BadRef("@aim_direction", "get aim direction");
    }
    // Return the aim direction that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::Embark(const Reference < CVehicle > & vehicle) const
{
    if (VALID_ENTITY(m_ID) && vehicle)
    {
        _Func->PutPlayerInVehicle(m_ID, vehicle, 0, true, true);
    }
    else if (!vehicle)
    {
        BadArg("embark", "embark in vehicle", _SCI32(vehicle));
    }
    else
    {
        BadRef("embark", "embark in vehicle");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::Embark(const Reference < CVehicle > & vehicle, SQInt32 slot, bool allocate, bool warp) const
{
    if (VALID_ENTITY(m_ID) && vehicle)
    {
        _Func->PutPlayerInVehicle(m_ID, vehicle, slot, allocate, warp);
    }
    else if (!vehicle)
    {
        BadArg("embark", "embark in vehicle", _SCI32(vehicle));
    }
    else
    {
        BadRef("embark", "embark in vehicle");
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::Disembark() const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RemovePlayerFromVehicle(m_ID);
    }
    else
    {
        BadRef("disembark", "disembark from vehicle");
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::Redirect(const SQChar * ip, SQUint32 port, const SQChar * nick, \
                const SQChar * pass, const SQChar * user)
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->RedirectPlayerToServer(m_ID, ip, port, nick, pass, user);
    }
    else
    {
        BadRef("redirect", "redirect to server");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::Msg(HSQUIRRELVM vm)
{
    const SQInteger top = sq_gettop(vm);
    // Are there any arguments on the stack?
    if (top <= 1)
    {
        DbgErr("Attempting to <send player message> without specifying a color");
        // Failed to send the message
        return 0;
    }
    // Is there a valid color on the stack?
    else if (top == 2)
    {
        DbgErr("Attempting to <send player message> without specifying a value");
        // Failed to send the message
        return 0;
    }
    // Attempt to retrieve the player instance
    Var< CPlayer & > inst(vm, 1);
    // Attempt to retrieve the color
    Var< Color3 > color(vm, 2);
    // Validate the player instance
    if (!inst.value)
    {
        DbgErr("Attempting to <send player message> using an invalid reference: %d", _SCI32(inst.value));
        // Failed to send the message
        return 0;
    }
    // Is there a single string or at least something that can convert to a string on the stack?
    else if (top == 3 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        // Variable where the resulted string will be retrieved
        const SQChar * msg = 0;
        // Attempt to retrieve the specified message from the stack
        if (SQ_FAILED(sq_getstring(vm, -1, &msg)))
        {
            DbgErr("Unable to <retrieve the player message> from the stack");
            // Pop any pushed values pushed to the stack
            sq_settop(vm, top);
            // Failed to send the value
            return 0;
        }
        // Pop any pushed values pushed to the stack
        sq_settop(vm, top);
        // Send the specified string
        _Func->SendClientMessage(_SCI32(inst.value), color.value.GetRGBA(), "%s", msg);
    }
    else if (top > 3)
    {
        // Variables containing the resulted string
        SQChar * msg = NULL;
        SQInteger len = 0;
        // Attempt to call the format function with the passed arguments
        if (SQ_FAILED(sqstd_format(vm, 3, &len, &msg)))
        {
            DbgErr("Unable to <generate the player message> because : %s", Error::Message(vm).c_str());
            // Failed to send the value
            return 0;
        }
        // Send the resulted string
        _Func->SendClientMessage(_SCI32(inst.value), color.value.GetRGBA(), "%s", msg);
    }
    else
    {
        DbgErr("Unable to <extract the player message> from the specified value");
    }
    // At this point everything went correctly
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::MsgP(HSQUIRRELVM vm)
{
    const SQInteger top = sq_gettop(vm);
    // Are there any arguments on the stack?
    if (top <= 1)
    {
        DbgErr("Attempting to <send player message> without specifying a prefix index");
        // Failed to send the message
        return 0;
    }
    // Is there a valid prefix index on the stack?
    else if (top == 2)
    {
        DbgErr("Attempting to <send player message> without specifying a value");
        // Failed to send the message
        return 0;
    }
    // Attempt to retrieve the player instance
    Var< CPlayer & > inst(vm, 1);
    // Attempt to retrieve the prefix index
    Var< SQUint32 > index(vm, 2);
    // Validate the player instance
    if (!inst.value)
    {
        DbgErr("Attempting to <send player message> using an invalid reference: %d", _SCI32(inst.value));
        // Failed to send the message
        return 0;
    }
    else if (index.value > MAX_PLAYER_MESSAGE_PREFIXES)
    {
        DbgErr("Attempting to <send player message> using an out of range prefix: %u", index.value);
        // Failed to send the message
        return 0;
    }
    // Is there a single string or at least something that can convert to a string on the stack?
    else if (top == 3 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        // Variable where the resulted string will be retrieved
        const SQChar * msg = 0;
        // Attempt to retrieve the specified message from the stack
        if (SQ_FAILED(sq_getstring(vm, -1, &msg)))
        {
            DbgErr("Unable to <retrieve the player message> from the stack");
            // Pop any pushed values pushed to the stack
            sq_settop(vm, top);
            // Failed to send the value
            return 0;
        }
        // Pop any pushed values pushed to the stack
        sq_settop(vm, top);
        // Send the specified string
        _Func->SendClientMessage(_SCI32(inst.value), FetchMessageColor(_SCI32(inst.value)), "%s%s",
                                 FetchMessagePrefix(_SCI32(inst.value), index.value), msg);
    }
    else if (top > 3)
    {
        // Variables containing the resulted string
        SQChar * msg = NULL;
        SQInteger len = 0;
        // Attempt to call the format function with the passed arguments
        if (SQ_FAILED(sqstd_format(vm, 3, &len, &msg)))
        {
            DbgErr("Unable to <generate the player message> because : %s", Error::Message(vm).c_str());
            // Failed to send the value
            return 0;
        }
        // Send the resulted string
        _Func->SendClientMessage(_SCI32(inst.value), FetchMessageColor(_SCI32(inst.value)), "%s%s",
                                 FetchMessagePrefix(_SCI32(inst.value), index.value), msg);
    }
    else
    {
        DbgErr("Unable to <extract the player message> from the specified value");
    }
    // At this point everything went correctly
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::MsgEx(HSQUIRRELVM vm)
{
    const SQInteger top = sq_gettop(vm);
    // Are there any arguments on the stack?
    if (top <= 1)
    {
        DbgErr("Attempting to <send player message> without specifying a color");
        // Failed to send the message
        return 0;
    }
    // Is there a valid color on the stack?
    else if (top <= 4)
    {
        DbgErr("Attempting to <send player message> without specifying a value");
        // Failed to send the message
        return 0;
    }
    // Attempt to retrieve the player instance
    Var< CPlayer & > inst(vm, 1);
    // Attempt to retrieve the color
    Var< SQInt32 > r(vm, 2);
    Var< SQInt32 > g(vm, 3);
    Var< SQInt32 > b(vm, 4);
    // Validate the player instance
    if (!inst.value)
    {
        DbgErr("Attempting to <send player message> using an invalid reference: %d", _SCI32(inst.value));
        // Failed to send the message
        return 0;
    }
    // Is there a single string or at least something that can convert to a string on the stack?
    else if (top == 5 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        // Variable where the resulted string will be retrieved
        const SQChar * msg = 0;
        // Attempt to retrieve the specified message from the stack
        if (SQ_FAILED(sq_getstring(vm, -1, &msg)))
        {
            DbgErr("Unable to <retrieve the player message> from the stack");
            // Pop any pushed values pushed to the stack
            sq_settop(vm, top);
            // Failed to send the value
            return 0;
        }
        // Pop any pushed values pushed to the stack
        sq_settop(vm, top);
        // Send the specified string
        _Func->SendClientMessage(_SCI32(inst.value), PACK_RGBA(r.value, g.value, b.value, 0), "%s", msg);
    }
    else if (top > 5)
    {
        // Variables containing the resulted string
        SQChar * msg = NULL;
        SQInteger len = 0;
        // Attempt to call the format function with the passed arguments
        if (SQ_FAILED(sqstd_format(vm, 5, &len, &msg)))
        {
            DbgErr("Unable to <generate the player message> because : %s", Error::Message(vm).c_str());
            // Failed to send the value
            return 0;
        }
        // Send the resulted string
        _Func->SendClientMessage(_SCI32(inst.value), PACK_RGBA(r.value, g.value, b.value, 0), "%s", msg);
    }
    else
    {
        DbgErr("Unable to <extract the player message> from the specified value");
    }
    // At this point everything went correctly
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::Message(HSQUIRRELVM vm)
{
    const SQInteger top = sq_gettop(vm);
    // Are there any arguments on the stack?
    if (top <= 1)
    {
        DbgErr("Attempting to <send player message> without specifying a value");
        // Failed to send the message
        return 0;
    }
    // Attempt to retrieve the player instance
    Var< CPlayer & > inst(vm, 1);
    // Validate the player instance
    if (!inst.value)
    {
        DbgErr("Attempting to <send player message> using an invalid reference: %d", _SCI32(inst.value));
        // Failed to send the message
        return 0;
    }
    // Is there a single string or at least something that can convert to a string on the stack?
    else if (top == 2 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        // Variable where the resulted string will be retrieved
        const SQChar * msg = 0;
        // Attempt to retrieve the specified message from the stack
        if (SQ_FAILED(sq_getstring(vm, -1, &msg)))
        {
            DbgErr("Unable to <retrieve the player message> from the stack");
            // Pop any pushed values pushed to the stack
            sq_settop(vm, top);
            // Failed to log the value
            return 0;
        }
        // Pop any pushed values pushed to the stack
        sq_settop(vm, top);
        // Send the specified string
        _Func->SendClientMessage(_SCI32(inst.value), FetchMessageColor(_SCI32(inst.value)), "%s", msg);
    }
    else if (top > 2)
    {
        // Variables containing the resulted string
        SQChar * msg = NULL;
        SQInteger len = 0;
        // Attempt to call the format function with the passed arguments
        if (SQ_FAILED(sqstd_format(vm, 2, &len, &msg)))
        {
            DbgErr("Unable to <generate the player message> because : %s", Error::Message(vm).c_str());
            // Failed to log the value
            return 0;
        }
        // Send the resulted string
        _Func->SendClientMessage(_SCI32(inst.value), FetchMessageColor(_SCI32(inst.value)), "%s", msg);
    }
    else
    {
        DbgErr("Unable to <extract the player message> from the specified value");
    }
    // At this point everything went correctly
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::Announce(HSQUIRRELVM vm)
{
    const SQInteger top = sq_gettop(vm);
    // Are there any arguments on the stack?
    if (top <= 1)
    {
        DbgErr("Attempting to <send player announcement> without specifying a value");
        // Failed to send the message
        return 0;
    }
    // Attempt to retrieve the player instance
    Var< CPlayer & > inst(vm, 1);
    // Validate the player instance
    if (!inst.value)
    {
        DbgErr("Attempting to <send player announcement> using an invalid reference: %d", _SCI32(inst.value));
        // Failed to send the message
        return 0;
    }
    // Is there a single string or at least something that can convert to a string on the stack?
    else if (top == 2 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        // Variable where the resulted string will be retrieved
        const SQChar * msg = 0;
        // Attempt to retrieve the specified message from the stack
        if (SQ_FAILED(sq_getstring(vm, -1, &msg)))
        {
            DbgErr("Unable to <retrieve the player announcement> from the stack");
            // Pop any pushed values pushed to the stack
            sq_settop(vm, top);
            // Failed to log the value
            return 0;
        }
        // Pop any pushed values pushed to the stack
        sq_settop(vm, top);
        // Send the specified string
        _Func->SendGameMessage(_SCI32(inst.value), FetchAnnounceStyle(_SCI32(inst.value)), "%s", msg);
    }
    else if (top > 2)
    {
        // Variables containing the resulted string
        SQChar * msg = NULL;
        SQInteger len = 0;
        // Attempt to call the format function with the passed arguments
        if (SQ_FAILED(sqstd_format(vm, 2, &len, &msg)))
        {
            DbgErr("Unable to <generate the player announcement> because : %s", Error::Message(vm).c_str());
            // Failed to log the value
            return 0;
        }
        // Send the resulted string
        _Func->SendGameMessage(_SCI32(inst.value), FetchAnnounceStyle(_SCI32(inst.value)), "%s", msg);
    }
    else
    {
        DbgErr("Unable to <extract the player announcement> from the specified value");
    }
    // At this point everything went correctly
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::AnnounceEx(HSQUIRRELVM vm)
{
    const SQInteger top = sq_gettop(vm);
    // Are there any arguments on the stack?
    if (top <= 1)
    {
        DbgErr("Attempting to <send player announcement> without specifying a type");
        // Failed to send the message
        return 0;
    }
    // Is there a valid type on the stack?
    else if (top == 2)
    {
        DbgErr("Attempting to <send player announcement> without specifying a value");
        // Failed to send the message
        return 0;
    }
    // Attempt to retrieve the player instance
    Var< CPlayer & > inst(vm, 1);
    // Attempt to retrieve the type
    Var< SQInt32 > type(vm, 2);
    // Validate the player instance
    if (!inst.value)
    {
        DbgErr("Attempting to <send player announcement> using an invalid reference: %d", _SCI32(inst.value));
        // Failed to send the message
        return 0;
    }
    // Is there a single string or at least something that can convert to a string on the stack?
    else if (top == 3 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        // Variable where the resulted string will be retrieved
        const SQChar * msg = 0;
        // Attempt to retrieve the specified message from the stack
        if (SQ_FAILED(sq_getstring(vm, -1, &msg)))
        {
            DbgErr("Unable to <retrieve the player announcement> from the stack");
            // Pop any pushed values pushed to the stack
            sq_settop(vm, top);
            // Failed to log the value
            return 0;
        }
        // Pop any pushed values pushed to the stack
        sq_settop(vm, top);
        // Send the specified string
        _Func->SendGameMessage(_SCI32(inst.value), type.value, "%s", msg);
    }
    else if (top > 3)
    {
        // Variables containing the resulted string
        SQChar * msg = NULL;
        SQInteger len = 0;
        // Attempt to call the format function with the passed arguments
        if (SQ_FAILED(sqstd_format(vm, 3, &len, &msg)))
        {
            DbgErr("Unable to <generate the player announcement> because : %s", Error::Message(vm).c_str());
            // Failed to log the value
            return 0;
        }
        // Send the resulted string
        _Func->SendGameMessage(_SCI32(inst.value), type.value, "%s", msg);
    }
    else
    {
        DbgErr("Unable to <extract the player announcement> from the specified value");
    }
    // At this point everything went correctly
    return 0;
}

// ------------------------------------------------------------------------------------------------
const SQChar * CPlayer::FetchMessagePrefix(SQInt32 player, SQUint32 index)
{
    /* Assuming that the caller verified if the player identifier and prefix index is in range! */
    if (Get(player).LocalPrefixes)
    {
        return Get(player).Prefixes[index].c_str();
    }
    // Fallback to the global value
    return Ent< CPlayer >::Prefixes[index].c_str();
}

// ------------------------------------------------------------------------------------------------
SQUint32 CPlayer::FetchMessageColor(SQInt32 player)
{
    /* Assuming that the caller verified if the player identifier is in range! */
    if (Get(player).LocalMessageColor)
    {
        return Get(player).MessageColor;
    }
    // Fallback to the global value
    return Ent< CPlayer >::MessageColor;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::FetchAnnounceStyle(SQInt32 player)
{
    /* Assuming that the caller verified if the player identifier is in range! */
    if (Get(player).LocalAnnounceStyle)
    {
        return Get(player).AnnounceStyle;
    }
    // Fallback to the global value
    return Ent< CPlayer >::AnnounceStyle;
}

// ------------------------------------------------------------------------------------------------
static const SQChar * GetPlayerMessagePrefix(SQUint32 index)
{
    if (index < MAX_PLAYER_MESSAGE_PREFIXES)
    {
        return Ent< CPlayer >::Prefixes[index].c_str();
    }
    else
    {
        DbgWrn("Attempting to <get player global message prefix> using an out of bounds index: %u", index);
    }

    return _SC("");
}

// ------------------------------------------------------------------------------------------------
static void SetPlayerMessagePrefix(SQUint32 index, const SQChar * prefix)
{
    if (index < MAX_PLAYER_MESSAGE_PREFIXES)
    {
        Ent< CPlayer >::Prefixes[index].assign(prefix);
    }
    else
    {
        DbgWrn("Attempting to <set player global message prefix> using an out of bounds index: %u", index);
    }
}

// ------------------------------------------------------------------------------------------------
static SQUint32 GetPlayerMessageColor()
{
    return Ent< CPlayer >::MessageColor;
}

// ------------------------------------------------------------------------------------------------
static void SetPlayerMessageColor(SQUint32 color)
{
    Ent< CPlayer >::MessageColor = color;
}

// ------------------------------------------------------------------------------------------------
static SQUint32 GetPlayerAnnounceStyle()
{
    return Ent< CPlayer >::AnnounceStyle;
}

// ------------------------------------------------------------------------------------------------
static void SetPlayerAnnounceStyle(SQInt32 style)
{
    Ent< CPlayer >::AnnounceStyle = style;
}

// ================================================================================================
bool Register_CPlayer(HSQUIRRELVM vm)
{
    // Attempt to register the base reference type before the actual implementation
    if (!Register_Reference< CPlayer >(vm, _SC("BasePlayer")))
    {
        LogFtl("Unable to register the base class <BasePlayer> for <CPlayer> type");
        // Registration failed
        return false;
    }
    // Typedef the base reference type for simplicity
    typedef Reference< CPlayer > RefType;
    // Output debugging information
    LogDbg("Beginning registration of <CPlayer> type");
    // Attempt to register the actual reference that implements all of the entity functionality
    Sqrat::RootTable(vm).Bind(_SC("CPlayer"), Sqrat::DerivedClass< CPlayer, RefType >(vm, _SC("CPlayer"))
        /* Constructors */
        .Ctor()
        .Ctor< SQInt32 >()
        /* Properties */
        .Prop(_SC("level"), &CPlayer::GetLevel, &CPlayer::SetLevel)
        .Prop(_SC("lprefix"), &CPlayer::GetLocalPrefixes, &CPlayer::SetLocalPrefixes)
        .Prop(_SC("lmsg_color"), &CPlayer::GetLocalMessageColor, &CPlayer::SetLocalMessageColor)
        .Prop(_SC("lmessage_color"), &CPlayer::GetLocalMessageColor, &CPlayer::SetLocalMessageColor)
        .Prop(_SC("ltext_style"), &CPlayer::GetLocalAnnounceStyle, &CPlayer::SetLocalAnnounceStyle)
        .Prop(_SC("lannounce_style"), &CPlayer::GetLocalAnnounceStyle, &CPlayer::SetLocalAnnounceStyle)
        .Prop(_SC("msg_color"), &CPlayer::GetMessageColor, &CPlayer::SetMessageColor)
        .Prop(_SC("message_color"), &CPlayer::GetMessageColor, &CPlayer::SetMessageColor)
        .Prop(_SC("announce_style"), &CPlayer::GetAnnounceStyle, &CPlayer::SetAnnounceStyle)
        .Prop(_SC("level"), &CPlayer::GetLevel, &CPlayer::SetLevel)
        .Prop(_SC("cls"), &CPlayer::GetClass)
        .Prop(_SC("class_id"), &CPlayer::GetClass)
        .Prop(_SC("admin"), &CPlayer::GetAdmin, &CPlayer::SetAdmin)
        .Prop(_SC("ip"), &CPlayer::GetIP)
        .Prop(_SC("connected"), &CPlayer::IsConnected)
        .Prop(_SC("spawned"), &CPlayer::IsSpawned)
        .Prop(_SC("key"), &CPlayer::GetKey)
        .Prop(_SC("world"), &CPlayer::GetWorld, &CPlayer::SetWorld)
        .Prop(_SC("sec_world"), &CPlayer::GetSecWorld, &CPlayer::SetSecWorld)
        .Prop(_SC("unique_world"), &CPlayer::GetUniqueWorld)
        .Prop(_SC("state"), &CPlayer::GetState)
        .Prop(_SC("name"), &CPlayer::GetName, &CPlayer::SetName)
        .Prop(_SC("team"), &CPlayer::GetTeam, &CPlayer::SetTeam)
        .Prop(_SC("skin"), &CPlayer::GetSkin, &CPlayer::SetSkin)
        .Prop(_SC("skin_id"), &CPlayer::GetSkinID, &CPlayer::SetSkinID)
        .Prop(_SC("color"), &CPlayer::GetColor, &CPlayer::SetColor)
        .Prop(_SC("money"), &CPlayer::GetMoney, &CPlayer::SetMoney)
        .Prop(_SC("score"), &CPlayer::GetScore, &CPlayer::SetScore)
        .Prop(_SC("ping"), &CPlayer::GetPing)
        .Prop(_SC("fps"), &CPlayer::GetFPS)
        .Prop(_SC("typing"), &CPlayer::IsTyping)
        .Prop(_SC("uid"), &CPlayer::GetUID)
        .Prop(_SC("uid2"), &CPlayer::GetUID2)
        .Prop(_SC("health"), &CPlayer::GetHealth, &CPlayer::SetHealth)
        .Prop(_SC("armour"), &CPlayer::GetArmour, &CPlayer::SetArmour)
        .Prop(_SC("immunity"), &CPlayer::GetImmunity, &CPlayer::SetImmunity)
        .Prop(_SC("pos"), &CPlayer::GetPosition, &CPlayer::SetPosition)
        .Prop(_SC("position"), &CPlayer::GetPosition, &CPlayer::SetPosition)
        .Prop(_SC("speed"), &CPlayer::GetSpeed, &CPlayer::SetSpeed)
        .Prop(_SC("angle"), &CPlayer::GetHeading, &CPlayer::SetHeading)
        .Prop(_SC("heading"), &CPlayer::GetHeading, &CPlayer::SetHeading)
        .Prop(_SC("alpha"), &CPlayer::GetAlpha, &CPlayer::SetAlpha)
        .Prop(_SC("vehicle_status"), &CPlayer::GetVehicleStatus)
        .Prop(_SC("slot"), &CPlayer::GetOccupiedSlot)
        .Prop(_SC("vehicle"), &CPlayer::GetVehicle)
        .Prop(_SC("vehicle_id"), &CPlayer::GetVehicleID)
        .Prop(_SC("controllable"), &CPlayer::GetControllable, &CPlayer::SetControllable)
        .Prop(_SC("driveby"), &CPlayer::GetDriveby, &CPlayer::SetDriveby)
        .Prop(_SC("white_scanlines"), &CPlayer::GetWhiteScanlines, &CPlayer::SetWhiteScanlines)
        .Prop(_SC("green_scanlines"), &CPlayer::GetGreenScanlines, &CPlayer::SetGreenScanlines)
        .Prop(_SC("widescreen"), &CPlayer::GetWidescreen, &CPlayer::SetWidescreen)
        .Prop(_SC("show_markers"), &CPlayer::GetShowMarkers, &CPlayer::SetShowMarkers)
        .Prop(_SC("attack_priv"), &CPlayer::GetAttackPriv, &CPlayer::SetAttackPriv)
        .Prop(_SC("has_marker"), &CPlayer::GetHasMarker, &CPlayer::SetHasMarker)
        .Prop(_SC("chat_tags"), &CPlayer::GetChatTags, &CPlayer::SetChatTags)
        .Prop(_SC("drunk_effects"), &CPlayer::GetDrunkEffects, &CPlayer::SetDrunkEffects)
        .Prop(_SC("weapon"), &CPlayer::GetWeapon, &CPlayer::SetWeapon)
        .Prop(_SC("weapon_id"), &CPlayer::GetWeaponID, &CPlayer::SetWeaponID)
        .Prop(_SC("camera_locked"), &CPlayer::IsCameraLocked)
        .Prop(_SC("wanted_level"), &CPlayer::GetWantedLevel, &CPlayer::SetWantedLevel)
        .Prop(_SC("touched_vehicle"), &CPlayer::StandingOnVehicle)
        .Prop(_SC("touched_object"), &CPlayer::StandingOnObject)
        .Prop(_SC("away"), &CPlayer::IsAway)
        .Prop(_SC("spectating"), &CPlayer::Spectating)
        .Prop(_SC("spec"), &CPlayer::GetSpectator, &CPlayer::SetSpectator)
        .Prop(_SC("crouched"), &CPlayer::IsCrouched)
        .Prop(_SC("game_keys"), &CPlayer::GetGameKeys)
        .Prop(_SC("aim_pos"), &CPlayer::GetAimPos)
        .Prop(_SC("aim_position"), &CPlayer::GetAimPos)
        .Prop(_SC("aim_dir"), &CPlayer::GetAimDir)
        .Prop(_SC("aim_direction"), &CPlayer::GetAimDir)
        /* Functions */
        .Func(_SC("get_msg_prefix"), &CPlayer::GetMessagePrefix)
        .Func(_SC("set_msg_prefix"), &CPlayer::SetMessagePrefix)
        .Func(_SC("streamed_for"), &CPlayer::IsStreamedFor)
        .Func(_SC("kick"), &CPlayer::Kick)
        .Func(_SC("ban"), &CPlayer::Ban)
        .Func(_SC("world_compatible"), &CPlayer::IsWorldCompatible)
        .Func(_SC("set_color"), &CPlayer::SetColorEx)
        .Func(_SC("spawn"), &CPlayer::ForceSpawn)
        .Func(_SC("select"), &CPlayer::ForceSelect)
        .Func(_SC("give_money"), &CPlayer::GiveMoney)
        .Func(_SC("set_position"), &CPlayer::SetPositionEx)
        .Func(_SC("set_pos"), &CPlayer::SetPositionEx)
        .Func(_SC("set_speed"), &CPlayer::SetSpeedEx)
        .Func(_SC("set_weapon"), &CPlayer::SetWeaponEx)
        .Func(_SC("set_weapon_id"), &CPlayer::SetWeaponIDEx)
        .Func(_SC("give_weapon_id"), &CPlayer::GiveWeaponIDEx)
        .Func(_SC("strip_weapons"), &CPlayer::StripWeapons)
        .Func(_SC("restore_camera"), &CPlayer::RestoreCamera)
        .Func(_SC("set_anim"), &CPlayer::SetAnimation)
        .Func(_SC("animation"), &CPlayer::SetAnimation)
        .Func(_SC("spectate"), &CPlayer::Spectate)
        .Func(_SC("disembark"), &CPlayer::Disembark)
        .Func(_SC("redirect"), &CPlayer::Redirect)
        /* Raw Functions */
        .SquirrelFunc(_SC("msg"), &CPlayer::Msg)
        .SquirrelFunc(_SC("msgp"), &CPlayer::MsgP)
        .SquirrelFunc(_SC("emsg"), &CPlayer::MsgEx)
        .SquirrelFunc(_SC("message"), &CPlayer::Message)
        .SquirrelFunc(_SC("announce"), &CPlayer::Announce)
        .SquirrelFunc(_SC("eannounce"), &CPlayer::AnnounceEx)
        .SquirrelFunc(_SC("text"), &CPlayer::Announce)
        .SquirrelFunc(_SC("etext"), &CPlayer::AnnounceEx)
        /* Overloads */
        .Overload< void (CPlayer::*)(const Vector3 &) const >
            (_SC("add_speed"), &CPlayer::AddSpeed)
        .Overload< void (CPlayer::*)(SQFloat, SQFloat, SQFloat) const >
            (_SC("add_speed"), &CPlayer::AddSpeedEx)
        .Overload< void (CPlayer::*)(const CWeapon &) const >
            (_SC("give_weapon"), &CPlayer::GiveWeapon)
        .Overload< void (CPlayer::*)(const CWeapon &, SQInt32) const >
            (_SC("give_weapon"), &CPlayer::GiveWeaponEx)
        .Overload< void (CPlayer::*)(const Vector3 &, const Vector3 &) const >
            (_SC("camera_position"), &CPlayer::SetCameraPosition)
        .Overload< void (CPlayer::*)(SQFloat, SQFloat, SQFloat, SQFloat, SQFloat, SQFloat) const >
            (_SC("camera_position"), &CPlayer::SetCameraPosition)
        .Overload< void (CPlayer::*)(const Reference < CVehicle > &) const >
            (_SC("embark"), &CPlayer::Embark)
        .Overload< void (CPlayer::*)(const Reference < CVehicle > &, SQInt32, bool, bool) const >
            (_SC("embark"), &CPlayer::Embark)
    );
    // Output debugging information
    LogDbg("Registration of <CPlayer> type was successful");

    // Output debugging information
    LogDbg("Beginning registration of <Player> functions type");
    // Several global functions
    Sqrat::RootTable root(vm);
    root.Func(_SC("GetPlayerMessagePrefix"), &GetPlayerMessagePrefix);
    root.Func(_SC("SetPlayerMessagePrefix"), &SetPlayerMessagePrefix);
    root.Func(_SC("GetPlayerMessageColor"), &GetPlayerMessageColor);
    root.Func(_SC("SetPlayerMessageColor"), &SetPlayerMessageColor);
    root.Func(_SC("GetPlayerAnnounceStyle"), &GetPlayerAnnounceStyle);
    root.Func(_SC("SetPlayerAnnounceStyle"), &SetPlayerAnnounceStyle);
    // Output debugging information
    LogDbg("Registration of <Player> functions type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
