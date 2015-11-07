#include "Entity/Player.hpp"
#include "Base/Color3.hpp"
#include "Misc/Skin.hpp"
#include "Misc/Weapon.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CSkin    CPlayer::s_Skin;
CWeapon  CPlayer::s_Weapon;

// ------------------------------------------------------------------------------------------------
Color3   CPlayer::s_Color3;
Vector3  CPlayer::s_Vector3;

// ------------------------------------------------------------------------------------------------
SQChar   CPlayer::s_Buffer[128];

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
        LogWrn(_SC("Attempting to <get player level> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player level> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player is streamed for player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <see if player is streamed for player> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player class> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player is admin> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set whether player is admin> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player ip address> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <kick player> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <ban player> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player is connected> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player is spawned> using an invalid reference: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
SQUnsignedInteger CPlayer::GetKey() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPlayerKey(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get player key> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player world> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player world> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player secondary world> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player secondary world> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player unique world> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player is compatible with world> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player state> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player name> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player name> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player team> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player team> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player skin> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player skin> using an invalid argument: %d"), _SCI32(skin));
    }
    else
    {
        LogWrn(_SC("Attempting to <set player skin> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player skin id> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player skin id> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player color> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player color> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player color> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <force player to spawn> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <force player to select> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player money> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player money> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <give player money> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player score> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player score> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player ping> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player fps> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player is typing> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player unique identifier> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player unique identifier version 2> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player health> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player health> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player armour> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player armour> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player immunity flags> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player immunity flags> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player position> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player position> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player position> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player speed> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player speed> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player speed> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <add player speed> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <add player speed> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player heading> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <st player heading> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player alpha> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player alpha> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player vehicle status> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player occupied slot> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player embarked vehicle> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player embarked vehicle id> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player is controllable> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set whether player is controllable> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player can driveby> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set whether player can driveby> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player has white scanlines> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set whether player has white scanlines> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player has green scanlines> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set whether player has green scanlines> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player has widescreen> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set whether player has widescreen> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player displays markers> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set whether player displays markers> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player has attack privilege> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set whether player has attack privilege> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player has marker> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set whether player has marker> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player has chat tags> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set whether player has chat tags> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player is under drunk effects> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set whether player is under drunk effects> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player weapon> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player weapon> using an invalid argument: %d"), _SCI32(wep));
    }
    else
    {
        LogWrn(_SC("Attempting to <set player weapon> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player weapon> using an invalid argument: %d"), _SCI32(wep));
    }
    else
    {
        LogWrn(_SC("Attempting to <set player weapon> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player weapon id> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player weapon> using an invalid argument: %d"), wep);
    }
    else
    {
        LogWrn(_SC("Attempting to <set player weapon> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player weapon> using an invalid argument: %d"), wep);
    }
    else
    {
        LogWrn(_SC("Attempting to <set player weapon> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <give player weapon> using an invalid argument: %d"), _SCI32(wep));
    }
    else
    {
        LogWrn(_SC("Attempting to <give player weapon> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <give player weapon> using an invalid argument: %d"), _SCI32(wep));
    }
    else
    {
        LogWrn(_SC("Attempting to <give player weapon> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <give player weapon> using an invalid argument: %d"), wep);
    }
    else
    {
        LogWrn(_SC("Attempting to <give player weapon> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <strip player of weapons> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player camera position> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <restore player camera position> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player has camera locked> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player animation> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player wanted level> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set player wanted level> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get the vehicle on which the player is standing> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get the object on which the player is standing> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player is away> using an invalid reference: %d"), m_ID);
    }

    return false;
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
        LogWrn(_SC("Attempting to <get spectated player> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <set spectated player> using an invalid argument: %d"), _SCI32(target));
    }
    else
    {
        LogWrn(_SC("Attempting to <set spectated player> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player is burning> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <see if player is crouched> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player action> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player game keys> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player aim position> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <get player aim direction> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <embark player in vehicle> using an invalid argument: %d"), _SCI32(vehicle));
    }
    else
    {
        LogWrn(_SC("Attempting to <embark player in vehicle> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <embark player in vehicle> using an invalid argument: %d"), _SCI32(vehicle));
    }
    else
    {
        LogWrn(_SC("Attempting to <embark player in vehicle> using an invalid reference: %d"), m_ID);
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
        LogWrn(_SC("Attempting to <disembark player from vehicle> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::Redirect(const SQChar * ip, SQUnsignedInteger port, const SQChar * nick, \
                const SQChar * pass, const SQChar * user)
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->RedirectPlayerToServer(m_ID, ip, port, nick, pass, user);
    }
    else
    {
        LogWrn(_SC("Attempting to <redirect player to server> using an invalid reference: %d"), m_ID);
    }

    return false;
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
        .Prop(_SC("cls"), &CPlayer::GetClass)
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
        .Prop(_SC("touched_object"), &CPlayer::StandingOnObject, &CPlayer::Spectate)
        .Prop(_SC("away"), &CPlayer::IsAway)
        .Prop(_SC("spectating"), &CPlayer::Spectating)
        .Prop(_SC("crouched"), &CPlayer::IsCrouched)
        .Prop(_SC("game_keys"), &CPlayer::GetGameKeys)
        .Prop(_SC("aim_pos"), &CPlayer::GetAimPos)
        .Prop(_SC("aim_position"), &CPlayer::GetAimPos)
        .Prop(_SC("aim_dir"), &CPlayer::GetAimDir)
        .Prop(_SC("aim_direction"), &CPlayer::GetAimDir)
        /* Functions */
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
        .Func(_SC("camera_position"), &CPlayer::SetCameraPosition)
        .Func(_SC("restore_camera"), &CPlayer::RestoreCamera)
        .Func(_SC("animation"), &CPlayer::SetAnimation)
        .Func(_SC("spectate"), &CPlayer::Spectate)
        .Func(_SC("disembark"), &CPlayer::Disembark)
        .Func(_SC("redirect"), &CPlayer::Redirect)
        /* Overloads */
        .Overload< void (CPlayer::*)(const Vector3 &) const >
            (_SC("add_speed"), &CPlayer::AddSpeed)
        .Overload< void (CPlayer::*)(SQFloat, SQFloat, SQFloat) const >
            (_SC("add_speed"), &CPlayer::AddSpeedEx)
        .Overload< void (CPlayer::*)(const CWeapon &) const >
            (_SC("give_weapon"), &CPlayer::GiveWeapon)
        .Overload< void (CPlayer::*)(const CWeapon &, SQInt32) const >
            (_SC("give_weapon"), &CPlayer::GiveWeaponEx)
        .Overload< void (CPlayer::*)(const Reference < CVehicle > &) const >
            (_SC("embark"), &CPlayer::Embark)
        .Overload< void (CPlayer::*)(const Reference < CVehicle > &, SQInt32, bool, bool) const >
            (_SC("embark"), &CPlayer::Embark)
    );
    // Output debugging information
    LogDbg("Registration of <CPlayer> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
