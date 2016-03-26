// ------------------------------------------------------------------------------------------------
#include "Entity/Player.hpp"
#include "Entity/Vehicle.hpp"
#include "Base/Color3.hpp"
#include "Base/Vector3.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqstdstring.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Color3   CPlayer::s_Color3;
Vector3  CPlayer::s_Vector3;

// ------------------------------------------------------------------------------------------------
SQChar   CPlayer::s_Buffer[SQMOD_PLAYER_TMP_BUFFER];

// ------------------------------------------------------------------------------------------------
const Int32 CPlayer::Max = SQMOD_PLAYER_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqPlayer");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
CPlayer::CPlayer(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_PLAYER_POOL))
    , m_Tag(ToStrF("%d", id))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CPlayer::~CPlayer()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::Cmp(const CPlayer & o) const
{
    if (m_ID == o.m_ID)
        return 0;
    else if (m_ID > o.m_ID)
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
const String & CPlayer::ToString() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
const String & CPlayer::GetTag() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

// ------------------------------------------------------------------------------------------------
Object & CPlayer::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetData(Object & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::BindEvent(Int32 evid, Object & env, Function & func) const
{
    // Validate the managed identifier
    Validate();
    // Obtain the function instance called for this event
    Function & event = _Core->GetPlayerEvent(m_ID, evid);
    // Is the specified callback function null?
    if (func.IsNull())
    {
        event.Release(); // Then release the current callback
    }
    // Assign the specified environment and function
    else
    {
        event = Function(env.GetVM(), env, func.GetFunc());
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsStreamedFor(CPlayer & player) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
        STHROWF("Invalid player argument: null");
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsPlayerStreamedForPlayer(m_ID, player.GetID());
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetClass() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerClass(m_ID);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetAdmin() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsPlayerAdmin(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetAdmin(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerAdmin(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
CSStr CPlayer::GetIP() const
{
    // Validate the managed identifier
    Validate();
    // Clear any previous string
    s_Buffer[0] = 0;
    // The server doesn't include the null terminator in the string (yet)
    memset(s_Buffer, 0, sizeof(s_Buffer));
    // Query the server for the ip of the managed player
    _Func->GetPlayerIP(m_ID, s_Buffer, sizeof(s_Buffer));
    // Return the requested information
    return s_Buffer;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::Kick() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->KickPlayer(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::Ban() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->BanPlayer(m_ID);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsConnected() const
{
    return _Func->IsPlayerConnected(m_ID);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsSpawned() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsPlayerSpawned(m_ID);
}

// ------------------------------------------------------------------------------------------------
Uint32 CPlayer::GetKey() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerKey(m_ID);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetWorld() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerWorld(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetWorld(Int32 world) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerWorld(m_ID, world);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetSecWorld() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerSecWorld(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetSecWorld(Int32 world) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerSecWorld(m_ID, world);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetUniqueWorld() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerUniqueWorld(m_ID);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsWorldCompatible(Int32 world) const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsPlayerWorldCompatible(m_ID, world);
}

// ------------------------------------------------------------------------------------------------
CSStr CPlayer::GetName() const
{
    // Validate the managed identifier
    Validate();
    // Clear any previous string
    s_Buffer[0] = 0;
    // The server doesn't include the null terminator in the string (yet)
    memset(s_Buffer, 0, sizeof(s_Buffer));
    // Query the server for the name of the managed player
    _Func->GetPlayerName(m_ID, s_Buffer, sizeof(s_Buffer));
    // Return the requested information
    return s_Buffer;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetName(CSStr name) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerName(m_ID, static_cast< CCStr >(name));
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetTeam() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerTeam(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetTeam(Int32 team) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerTeam(m_ID, team);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetSkin() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerSkin(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetSkin(Int32 skin) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerSkin(m_ID, skin);
}

// ------------------------------------------------------------------------------------------------
const Color3 & CPlayer::GetColor() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous color information, if any
    s_Color3.Clear();
    // Query the server for the color values
    s_Color3.SetRGB(_Func->GetPlayerColour(m_ID));
    // Return the requested information
    return s_Color3;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetColor(const Color3 & color) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerColour(m_ID, color.GetRGB());
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetColorEx(Uint8 r, Uint8 g, Uint8 b) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerColour(m_ID, SQMOD_PACK_RGB(r, g, b));
}

// ------------------------------------------------------------------------------------------------
void CPlayer::ForceSpawn() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->ForcePlayerSpawn(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::ForceSelect() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->ForcePlayerSelect(m_ID);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetMoney() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerMoney(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetMoney(Int32 amount) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerMoney(m_ID, amount);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::GiveMoney(Int32 amount) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->GivePlayerMoney(m_ID, amount);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetScore() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerScore(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetScore(Int32 score) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerScore(m_ID, score);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetPing() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerPing(m_ID);
}

// ------------------------------------------------------------------------------------------------
Float32 CPlayer::GetFPS() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerFPS(m_ID);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsTyping() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsPlayerTyping(m_ID);
}

// ------------------------------------------------------------------------------------------------
CSStr CPlayer::GetUID() const
{
    // Validate the managed identifier
    Validate();
    // Clear any previous string
    s_Buffer[0] = 0;
    // The server doesn't include the null terminator in the string (yet)
    memset(s_Buffer, 0, sizeof(s_Buffer));
    // Query the server for the uid of the managed player
    _Func->GetPlayerUID(m_ID, s_Buffer, sizeof(s_Buffer));
    // Return the requested information
    return s_Buffer;
}

// ------------------------------------------------------------------------------------------------
CSStr CPlayer::GetUID2() const
{
    // Validate the managed identifier
    Validate();
    // Clear any previous string
    s_Buffer[0] = 0;
    // The server doesn't include the null terminator in the string (yet)
    memset(s_Buffer, 0, sizeof(s_Buffer));
    // Query the server for the uid2 of the managed player
    _Func->GetPlayerUID2(m_ID, s_Buffer, sizeof(s_Buffer));
    // Return the requested information
    return s_Buffer;
}

// ------------------------------------------------------------------------------------------------
Float32 CPlayer::GetHealth() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerHealth(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetHealth(Float32 amount) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerHealth(m_ID, amount);
}

// ------------------------------------------------------------------------------------------------
Float32 CPlayer::GetArmor() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerArmour(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetArmor(Float32 amount) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerArmour(m_ID, amount);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetImmunity() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerImmunityFlags(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetImmunity(Int32 flags) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerImmunityFlags(m_ID, flags);
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CPlayer::GetPosition() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.Clear();
    // Query the server for the position values
    _Func->GetPlayerPos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetPosition(const Vector3 & pos) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerPos(m_ID, pos.x, pos.y, pos.z);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetPositionEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerPos(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CPlayer::GetSpeed() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous speed information, if any
    s_Vector3.Clear();
    // Query the server for the speed values
    _Func->GetPlayerSpeed(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerSpeed(m_ID, vel.x, vel.y, vel.z);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerSpeed(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::AddSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->AddPlayerSpeed(m_ID, vel.x, vel.y, vel.z);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::AddSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->AddPlayerSpeed(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
Float32 CPlayer::GetHeading() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerHeading(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetHeading(Float32 angle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerHeading(m_ID, angle);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetAlpha() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerAlpha(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetAlpha(Int32 alpha, Int32 fade) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerAlpha(m_ID, alpha, fade);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetVehicleStatus() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerInVehicleStatus(m_ID);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetOccupiedSlot() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerInVehicleSlot(m_ID);
}

// ------------------------------------------------------------------------------------------------
Object & CPlayer::GetVehicle() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Core->GetVehicle(_Func->GetPlayerVehicleID(m_ID)).mObj;
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetVehicleID() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerVehicleID(m_ID);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetControllable() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->EnabledPlayerControllable(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetControllable(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->TogglePlayerControllable(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetDriveby() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->EnabledPlayerDriveby(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetDriveby(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->TogglePlayerDriveby(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetWhiteScanlines() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->EnabledPlayerWhiteScanlines(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetWhiteScanlines(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->TogglePlayerWhiteScanlines(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetGreenScanlines() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->EnabledPlayerGreenScanlines(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetGreenScanlines(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->TogglePlayerGreenScanlines(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetWidescreen() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->EnabledPlayerWidescreen(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetWidescreen(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->TogglePlayerWidescreen(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetShowMarkers() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->EnabledPlayerShowMarkers(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetShowMarkers(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->TogglePlayerShowMarkers(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetAttackPriv() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->EnabledPlayerAttackPriv(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetAttackPriv(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->TogglePlayerAttackPriv(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetHasMarker() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->EnabledPlayerHasMarker(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetHasMarker(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->TogglePlayerHasMarker(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetChatTags() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->EnabledPlayerChatTags(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetChatTags(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->TogglePlayerChatTagsEnabled(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::GetDrunkEffects() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->EnabledPlayerDrunkEffects(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetDrunkEffects(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->TogglePlayerDrunkEffects(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetWeapon() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerWeapon(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetWeapon(Int32 wep, Int32 ammo) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerWeapon(m_ID, wep, ammo);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::GiveWeapon(Int32 wep, Int32 ammo) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->GivePlayerWeapon(m_ID, wep, ammo);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::StripWeapons() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RemoveAllWeapons(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetCameraPosition(const Vector3 & pos, const Vector3 & aim) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetCameraPosition(m_ID, pos.x, pos.y, pos.z, aim.x, aim.y, aim.z);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetCameraPosition(Float32 xp, Float32 yp, Float32 zp, Float32 xa, Float32 ya, Float32 za) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetCameraPosition(m_ID, xp, yp, zp, xa, ya, za);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::RestoreCamera() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RestoreCamera(m_ID);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsCameraLocked() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsCameraLocked(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetAnimation(Int32 group, Int32 anim) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerAnimation(m_ID, group, anim);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetWantedLevel() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerWantedLevel(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetWantedLevel(Int32 level) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerWantedLevel(m_ID, level);
}

// ------------------------------------------------------------------------------------------------
Object & CPlayer::StandingOnVehicle() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Core->GetVehicle(_Func->GetPlayerStandingOnVehicle(m_ID)).mObj;
}

// ------------------------------------------------------------------------------------------------
Object & CPlayer::StandingOnObject() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Core->GetObject(_Func->GetPlayerStandingOnObject(m_ID)).mObj;
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsAway() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsPlayerAway(m_ID);
}

// ------------------------------------------------------------------------------------------------
Object & CPlayer::GetSpectator() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Core->GetPlayer(_Func->GetPlayerSpectateTarget(m_ID)).mObj;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetSpectator(CPlayer & target) const
{
    // Is the specified player even valid?
    if (!target.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerSpectateTarget(m_ID, target.GetID());
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsBurning() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerOnFireStatus(m_ID);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsCrouched() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerCrouchStatus(m_ID);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetState() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerState(m_ID);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetAction() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerAction(m_ID);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetGameKeys() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerGameKeys(m_ID);
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CPlayer::GetAimPos() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.Clear();
    // Query the server for the position values
    _Func->GetPlayerAimPos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CPlayer::GetAimDir() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous direction information, if any
    s_Vector3.Clear();
    // Query the server for the direction values
    _Func->GetPlayerAimDir(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::Embark(CVehicle & vehicle) const
{
    // Is the specified vehicle even valid?
    if (!vehicle.IsActive())
    {
        STHROWF("Invalid vehicle argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->PutPlayerInVehicle(m_ID, vehicle.GetID(), 0, true, true);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::Embark(CVehicle & vehicle, Int32 slot, bool allocate, bool warp) const
{
    // Is the specified vehicle even valid?
    if (!vehicle.IsActive())
    {
        STHROWF("Invalid vehicle argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->PutPlayerInVehicle(m_ID, vehicle.GetID(), slot, allocate, warp);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::Disembark() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RemovePlayerFromVehicle(m_ID);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::Redirect(CSStr ip, Uint32 port, CSStr nick, CSStr pass, CSStr user)
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->RedirectPlayerToServer(m_ID, ip, port, nick, pass, user);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetAuthority() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Core->GetPlayer(m_ID).mAuthority;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetAuthority(Int32 level) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Core->GetPlayer(m_ID).mAuthority = level;
}

// ------------------------------------------------------------------------------------------------
CSStr CPlayer::GetMessagePrefix(Uint32 index) const
{
    // Perform a range check on the specified prefix index
    if (index >= SQMOD_PLAYER_MSG_PREFIXES)
    {
        STHROWF("Prefix index is out of range: %u >= %d", index, SQMOD_PLAYER_MSG_PREFIXES);
    }
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Core->GetPlayer(m_ID).mPrefixes[index].c_str();
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetMessagePrefix(Uint32 index, CSStr prefix) const
{
    // Perform a range check on the specified prefix index
    if (index >= SQMOD_PLAYER_MSG_PREFIXES)
    {
        STHROWF("Prefix index is out of range: %u >= %d", index, SQMOD_PLAYER_MSG_PREFIXES);
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Core->GetPlayer(m_ID).mPrefixes[index].assign(prefix);
}

// ------------------------------------------------------------------------------------------------
Uint32 CPlayer::GetMessageColor() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Core->GetPlayer(m_ID).mMessageColor;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetMessageColor(Uint32 color) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
     _Core->GetPlayer(m_ID).mMessageColor = color;
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetAnnounceStyle() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Core->GetPlayer(m_ID).mAnnounceStyle;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetAnnounceStyle(Int32 style) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Core->GetPlayer(m_ID).mAnnounceStyle = style;
}

// ------------------------------------------------------------------------------------------------
Float32 CPlayer::GetPosX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.x = 0;
    // Query the server for the requested component value
    _Func->GetPlayerPos(m_ID, &s_Vector3.x, NULL, NULL);
    // Return the requested information
    return s_Vector3.x;
}

// ------------------------------------------------------------------------------------------------
Float32 CPlayer::GetPosY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.y = 0;
    // Query the server for the requested component value
    _Func->GetPlayerPos(m_ID, NULL, &s_Vector3.y, NULL);
    // Return the requested information
    return s_Vector3.y;
}

// ------------------------------------------------------------------------------------------------
Float32 CPlayer::GetPosZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.z = 0;
    // Query the server for the requested component value
    _Func->GetPlayerPos(m_ID, NULL, NULL, &s_Vector3.z);
    // Return the requested information
    return s_Vector3.z;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetPosX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetPlayerPos(m_ID, NULL, &s_Vector3.y, &s_Vector3.z);
    // Perform the requested operation
    _Func->SetPlayerPos(m_ID, x, s_Vector3.y, s_Vector3.z);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetPosY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetPlayerPos(m_ID, &s_Vector3.x, NULL, &s_Vector3.z);
    // Perform the requested operation
    _Func->SetPlayerPos(m_ID, s_Vector3.x, y, s_Vector3.z);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetPosZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetPlayerPos(m_ID, &s_Vector3.x, &s_Vector3.y, NULL);
    // Perform the requested operation
    _Func->SetPlayerPos(m_ID, s_Vector3.z, s_Vector3.y, z);
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::Msg(HSQUIRRELVM vm)
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
    // The player instance
    CPlayer * player = nullptr;
    // The message color
    Color3 color;
    // Attempt to extract the argument values
    try
    {
        player = Var< CPlayer * >(vm, 1).value;
        color = Var< Color3 >(vm, 2).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.Message().c_str());
    }
    // Do we have a valid player instance?
    if (!player)
    {
        return sq_throwerror(vm, "Invalid player instance");
    }
    // Do we have a valid player identifier?
    else if (!player->IsActive())
    {
        return sq_throwerror(vm, "Invalid player reference");
    }
    // Do we have enough values to call the format function?
    else if (top > 3)
    {
        SStr msg = NULL;
        SQInteger len = 0;
        // Attempt to generate the specified string format
        SQRESULT ret = sqstd_format(vm, 3, &len, &msg);
        // Did the format failed?
        if (SQ_FAILED(ret))
        {
            return ret; // Propagate the exception
        }
        // Send the resulted message string
        _Func->SendClientMessage(player->GetID(), color.GetRGBA(), "%s", msg);
    }
    else
    {
        // Attempt to retrieve the value from the stack as a string
        Var< CSStr > msg(vm, 3);
        // See if the obtained value is a valid message
        if (!msg.value)
        {
            return sq_throwerror(vm, "Unable to retrieve the message");
        }
        // Send the resulted message string
        _Func->SendClientMessage(player->GetID(), color.GetRGBA(), "%s", msg.value);
    }
    // This function does not return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::MsgP(HSQUIRRELVM vm)
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
    // The player instance
    CPlayer * player = nullptr;
    // The prefix index
    Uint32 index;
    // Attempt to extract the argument values
    try
    {
        player = Var< CPlayer * >(vm, 1).value;
        index = Var< Uint32 >(vm, 2).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.Message().c_str());
    }
    // Do we have a valid player instance?
    if (!player)
    {
        return sq_throwerror(vm, "Invalid player instance");
    }
    // Do we have a valid player identifier?
    else if (!player->IsActive())
    {
        return sq_throwerror(vm, "Invalid player reference");
    }
    // Perform a range check on the specified prefix index
    else if (index > SQMOD_PLAYER_MSG_PREFIXES)
    {
        return sq_throwerror(vm, ToStrF("Prefix index is out of range: %u > %u",
                                        index, SQMOD_PLAYER_MSG_PREFIXES));
    }
    // Do we have enough values to call the format function?
    else if (top > 3)
    {
        SStr msg = NULL;
        SQInteger len = 0;
        // Attempt to generate the specified string format
        SQRESULT ret = sqstd_format(vm, 3, &len, &msg);
        // Did the format failed?
        if (SQ_FAILED(ret))
        {
            return ret; // Propagate the exception
        }
        // Retrieve the associated player structure
        const auto & splayer = _Core->GetPlayer(player->GetID());
        // Send the resulted message string
        _Func->SendClientMessage(player->GetID(), splayer.mMessageColor, "%s%s",
                                        splayer.mPrefixes[index].c_str(), msg);
    }
    else
    {
        // Attempt to retrieve the value from the stack as a string
        Var< CSStr > msg(vm, 3);
        // See if the obtained value is a valid message
        if (!msg.value)
        {
            return sq_throwerror(vm, "Unable to retrieve the message");
        }
        // Retrieve the associated player structure
        const auto & splayer = _Core->GetPlayer(player->GetID());
        // Send the resulted message string
        _Func->SendClientMessage(player->GetID(), splayer.mMessageColor, "%s%s",
                                        splayer.mPrefixes[index].c_str(), msg.value);
    }
    // This function does not return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::MsgEx(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the message color specified?
    if (top <= 3)
    {
        return sq_throwerror(vm, "Missing message color");
    }
    // Was the message value specified?
    else if (top <= 4)
    {
        return sq_throwerror(vm, "Missing message value");
    }
    // The player instance
    CPlayer * player = nullptr;
    // The message color
    Uint8 r, g, b;
    // Attempt to extract the argument values
    try
    {
        player = Var< CPlayer * >(vm, 1).value;
        r = Var< Uint8 >(vm, 2).value;
        g = Var< Uint8 >(vm, 3).value;
        b = Var< Uint8 >(vm, 4).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.Message().c_str());
    }
    // Do we have a valid player instance?
    if (!player)
    {
        return sq_throwerror(vm, "Invalid player instance");
    }
    // Do we have a valid player identifier?
    else if (!player->IsActive())
    {
        return sq_throwerror(vm, "Invalid player reference");
    }
    // Do we have enough values to call the format function?
    else if (top > 5)
    {
        SStr msg = NULL;
        SQInteger len = 0;
        // Attempt to generate the specified string format
        SQRESULT ret = sqstd_format(vm, 5, &len, &msg);
        // Did the format failed?
        if (SQ_FAILED(ret))
        {
            return ret; // Propagate the exception
        }
        // Send the resulted message string
        _Func->SendClientMessage(player->GetID(), SQMOD_PACK_RGBA(r, g, b, 0), "%s", msg);
    }
    else
    {
        // Attempt to retrieve the value from the stack as a string
        Var< CSStr > msg(vm, 5);
        // See if the obtained value is a valid message
        if (!msg.value)
        {
            return sq_throwerror(vm, "Unable to retrieve the message");
        }
        // Send the resulted message string
        _Func->SendClientMessage(player->GetID(), SQMOD_PACK_RGBA(r, g, b, 0), "%s", msg.value);
    }
    // This function does not return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::Message(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the message value specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing message value");
    }
    // The player instance
    CPlayer * player = nullptr;
    // Attempt to extract the argument values
    try
    {
        player = Var< CPlayer * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.Message().c_str());
    }
    // Do we have a valid player instance?
    if (!player)
    {
        return sq_throwerror(vm, "Invalid player instance");
    }
    // Do we have a valid player identifier?
    else if (!player->IsActive())
    {
        return sq_throwerror(vm, "Invalid player reference");
    }
    // Do we have enough values to call the format function?
    else if (top > 2)
    {
        SStr msg = NULL;
        SQInteger len = 0;
        // Attempt to generate the specified string format
        SQRESULT ret = sqstd_format(vm, 2, &len, &msg);
        // Did the format failed?
        if (SQ_FAILED(ret))
        {
            return ret; // Propagate the exception
        }
        // Send the resulted message string
        _Func->SendClientMessage(player->GetID(),
                                    _Core->GetPlayer(player->GetID()).mMessageColor, "%s", msg);
    }
    else
    {
        // Attempt to retrieve the value from the stack as a string
        Var< CSStr > msg(vm, 2);
        // See if the obtained value is a valid message
        if (!msg.value)
        {
            return sq_throwerror(vm, "Unable to retrieve the message");
        }
        // Send the resulted message string
        _Func->SendClientMessage(player->GetID(),
                                    _Core->GetPlayer(player->GetID()).mMessageColor, "%s", msg.value);
    }
    // This function does not return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::Announce(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the announcement value specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing announcement value");
    }
    // The player instance
    CPlayer * player = nullptr;
    // Attempt to extract the argument values
    try
    {
        player = Var< CPlayer * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.Message().c_str());
    }
    // Do we have a valid player instance?
    if (!player)
    {
        return sq_throwerror(vm, "Invalid player instance");
    }
    // Do we have a valid player identifier?
    else if (!player->IsActive())
    {
        return sq_throwerror(vm, "Invalid player reference");
    }
    // Do we have enough values to call the format function?
    else if (top > 2)
    {
        SStr msg = NULL;
        SQInteger len = 0;
        // Attempt to generate the specified string format
        SQRESULT ret = sqstd_format(vm, 2, &len, &msg);
        // Did the format failed?
        if (SQ_FAILED(ret))
        {
            return ret; // Propagate the exception
        }
        // Send the resulted announcement string
        _Func->SendGameMessage(player->GetID(),
                                _Core->GetPlayer(player->GetID()).mAnnounceStyle, "%s", msg);
    }
    else
    {
        // Attempt to retrieve the value from the stack as a string
        Var< CSStr > msg(vm, 2);
        // See if the obtained value is a valid announcement
        if (!msg.value)
        {
            return sq_throwerror(vm, "Unable to retrieve the announcement");
        }
        // Send the resulted announcement string
        _Func->SendGameMessage(player->GetID(),
                                _Core->GetPlayer(player->GetID()).mAnnounceStyle, "%s", msg.value);
    }
    // This function does not return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::AnnounceEx(HSQUIRRELVM vm)
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
    // The player instance
    CPlayer * player = nullptr;
    // The announcement style
    Int32 style;
    // style to extract the argument values
    try
    {
        player = Var< CPlayer * >(vm, 1).value;
        style = Var< Int32 >(vm, 2).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.Message().c_str());
    }
    // Do we have a valid player instance?
    if (!player)
    {
        return sq_throwerror(vm, "Invalid player instance");
    }
    // Do we have a valid player identifier?
    else if (!player->IsActive())
    {
        return sq_throwerror(vm, "Invalid player reference");
    }
    // Do we have enough values to call the format function?
    else if (top > 3)
    {
        SStr msg = NULL;
        SQInteger len = 0;
        // Attempt to generate the specified string format
        SQRESULT ret = sqstd_format(vm, 3, &len, &msg);
        // Did the format failed?
        if (SQ_FAILED(ret))
        {
            return ret; // Propagate the exception
        }
        // Send the resulted announcement string
        _Func->SendGameMessage(player->GetID(), style, "%s", msg);
    }
    else
    {
        // Attempt to retrieve the value from the stack as a string
        Var< CSStr > msg(vm, 3);
        // See if the obtained value is a valid announcement
        if (!msg.value)
        {
            return sq_throwerror(vm, "Unable to retrieve the announcement");
        }
        // Send the resulted announcement string
        _Func->SendGameMessage(player->GetID(), style, "%s", msg.value);
    }
    // This function does not return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
static const Object & Player_FindByID(Int32 id)
{
    // Perform a range check on the specified identifier
    if (INVALID_ENTITYEX(id, SQMOD_PLAYER_POOL))
    {
        STHROWF("The specified player identifier is invalid: %d", id);
    }
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = _Core->GetPlayers().cbegin();
    Core::Players::const_iterator end = _Core->GetPlayers().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does the identifier match the specified one?
        if (itr->mID == id)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a player matching the specified identifier
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
static const Object & Player_FindByTag(CSStr tag)
{
    // Perform a validity check on the specified tag
    if (!tag || *tag == '\0')
    {
        STHROWF("The specified player tag is invalid: null/empty");
    }
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = _Core->GetPlayers().cbegin();
    Core::Players::const_iterator end = _Core->GetPlayers().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does this entity even exist and does the tag match the specified one?
        if (itr->mInst != nullptr && itr->mInst->GetTag().compare(tag) == 0)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a player matching the specified tag
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
static Array Player_FindActive()
{
    // Remember the initial stack size
    StackGuard sg;
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = _Core->GetPlayers().cbegin();
    Core::Players::const_iterator end = _Core->GetPlayers().cend();
    // Allocate an empty array on the stack
    sq_newarray(DefaultVM::Get(), 0);
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Is this entity instance active?
        if (VALID_ENTITY(itr->mID))
        {
            // Push the script object on the stack
            sq_pushobject(DefaultVM::Get(), (HSQOBJECT &)((*itr).mObj));
            // Append the object at the back of the array
            if (SQ_FAILED(sq_arrayappend(DefaultVM::Get(), -1)))
            {
                STHROWF("Unable to append entity instance to the list");
            }
        }
    }
    // Return the array at the top of the stack
    return Var< Array >(DefaultVM::Get(), -1).value;
}

// ================================================================================================
void Register_CPlayer(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqPlayer"),
        Class< CPlayer, NoConstructor< CPlayer > >(vm, _SC("SqPlayer"))
        // Metamethods
        .Func(_SC("_cmp"), &CPlayer::Cmp)
        .SquirrelFunc(_SC("_typename"), &CPlayer::Typename)
        .Func(_SC("_tostring"), &CPlayer::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CPlayer::Max)
        // Core Properties
        .Prop(_SC("ID"), &CPlayer::GetID)
        .Prop(_SC("Tag"), &CPlayer::GetTag, &CPlayer::SetTag)
        .Prop(_SC("Data"), &CPlayer::GetData, &CPlayer::SetData)
        .Prop(_SC("Active"), &CPlayer::IsActive)
        // Core Methods
        .Func(_SC("Bind"), &CPlayer::BindEvent)
        // Properties
        .Prop(_SC("Class"), &CPlayer::GetClass)
        .Prop(_SC("Admin"), &CPlayer::GetAdmin, &CPlayer::SetAdmin)
        .Prop(_SC("IP"), &CPlayer::GetIP)
        .Prop(_SC("Connected"), &CPlayer::IsConnected)
        .Prop(_SC("Spawned"), &CPlayer::IsSpawned)
        .Prop(_SC("Key"), &CPlayer::GetKey)
        .Prop(_SC("World"), &CPlayer::GetWorld, &CPlayer::SetWorld)
        .Prop(_SC("SecWorld"), &CPlayer::GetSecWorld, &CPlayer::SetSecWorld)
        .Prop(_SC("UniqueWorld"), &CPlayer::GetUniqueWorld)
        .Prop(_SC("State"), &CPlayer::GetState)
        .Prop(_SC("Action"), &CPlayer::GetAction)
        .Prop(_SC("Name"), &CPlayer::GetName, &CPlayer::SetName)
        .Prop(_SC("Team"), &CPlayer::GetTeam, &CPlayer::SetTeam)
        .Prop(_SC("Skin"), &CPlayer::GetSkin, &CPlayer::SetSkin)
        .Prop(_SC("Color"), &CPlayer::GetColor, &CPlayer::SetColor)
        .Prop(_SC("Money"), &CPlayer::GetMoney, &CPlayer::SetMoney)
        .Prop(_SC("Score"), &CPlayer::GetScore, &CPlayer::SetScore)
        .Prop(_SC("Ping"), &CPlayer::GetPing)
        .Prop(_SC("FPS"), &CPlayer::GetFPS)
        .Prop(_SC("Typing"), &CPlayer::IsTyping)
        .Prop(_SC("UID"), &CPlayer::GetUID)
        .Prop(_SC("UID2"), &CPlayer::GetUID2)
        .Prop(_SC("Health"), &CPlayer::GetHealth, &CPlayer::SetHealth)
        .Prop(_SC("Armor"), &CPlayer::GetArmor, &CPlayer::SetArmor)
        .Prop(_SC("Immunity"), &CPlayer::GetImmunity, &CPlayer::SetImmunity)
        .Prop(_SC("Pos"), &CPlayer::GetPosition, &CPlayer::SetPosition)
        .Prop(_SC("Position"), &CPlayer::GetPosition, &CPlayer::SetPosition)
        .Prop(_SC("Speed"), &CPlayer::GetSpeed, &CPlayer::SetSpeed)
        .Prop(_SC("Angle"), &CPlayer::GetHeading, &CPlayer::SetHeading)
        .Prop(_SC("Heading"), &CPlayer::GetHeading, &CPlayer::SetHeading)
        .Prop(_SC("Alpha"), &CPlayer::GetAlpha, &CPlayer::SetAlpha)
        .Prop(_SC("VehicleStatus"), &CPlayer::GetVehicleStatus)
        .Prop(_SC("Slot"), &CPlayer::GetOccupiedSlot)
        .Prop(_SC("Vehicle"), &CPlayer::GetVehicle)
        .Prop(_SC("VehicleID"), &CPlayer::GetVehicleID)
        .Prop(_SC("Controllable"), &CPlayer::GetControllable, &CPlayer::SetControllable)
        .Prop(_SC("Driveby"), &CPlayer::GetDriveby, &CPlayer::SetDriveby)
        .Prop(_SC("WhiteScanlines"), &CPlayer::GetWhiteScanlines, &CPlayer::SetWhiteScanlines)
        .Prop(_SC("GreenScanlines"), &CPlayer::GetGreenScanlines, &CPlayer::SetGreenScanlines)
        .Prop(_SC("Widescreen"), &CPlayer::GetWidescreen, &CPlayer::SetWidescreen)
        .Prop(_SC("ShowMarkers"), &CPlayer::GetShowMarkers, &CPlayer::SetShowMarkers)
        .Prop(_SC("AttackPriv"), &CPlayer::GetAttackPriv, &CPlayer::SetAttackPriv)
        .Prop(_SC("HasMarker"), &CPlayer::GetHasMarker, &CPlayer::SetHasMarker)
        .Prop(_SC("ChatTags"), &CPlayer::GetChatTags, &CPlayer::SetChatTags)
        .Prop(_SC("DrunkEffects"), &CPlayer::GetDrunkEffects, &CPlayer::SetDrunkEffects)
        .Prop(_SC("Weapon"), &CPlayer::GetWeapon, &CPlayer::SetWeapon)
        .Prop(_SC("CameraLocked"), &CPlayer::IsCameraLocked)
        .Prop(_SC("WantedLevel"), &CPlayer::GetWantedLevel, &CPlayer::SetWantedLevel)
        .Prop(_SC("TouchedVehicle"), &CPlayer::StandingOnVehicle)
        .Prop(_SC("TouchedObject"), &CPlayer::StandingOnObject)
        .Prop(_SC("Away"), &CPlayer::IsAway)
        .Prop(_SC("Spectating"), &CPlayer::GetSpectator)
        .Prop(_SC("Spec"), &CPlayer::GetSpectator, &CPlayer::SetSpectator)
        .Prop(_SC("Crouched"), &CPlayer::IsCrouched)
        .Prop(_SC("GameKeys"), &CPlayer::GetGameKeys)
        .Prop(_SC("AimPos"), &CPlayer::GetAimPos)
        .Prop(_SC("AimPosition"), &CPlayer::GetAimPos)
        .Prop(_SC("AimDir"), &CPlayer::GetAimDir)
        .Prop(_SC("AimDirection"), &CPlayer::GetAimDir)
        .Prop(_SC("Authority"), &CPlayer::GetAuthority, &CPlayer::SetAuthority)
        .Prop(_SC("MessageColor"), &CPlayer::GetMessageColor, &CPlayer::SetMessageColor)
        .Prop(_SC("AnnounceStyle"), &CPlayer::GetAnnounceStyle, &CPlayer::SetAnnounceStyle)
        .Prop(_SC("X"), &CPlayer::GetPosX, &CPlayer::SetPosX)
        .Prop(_SC("Y"), &CPlayer::GetPosY, &CPlayer::SetPosY)
        .Prop(_SC("Z"), &CPlayer::GetPosZ, &CPlayer::SetPosZ)
        // Member Methods
        .Func(_SC("StreamedFor"), &CPlayer::IsStreamedFor)
        .Func(_SC("Kick"), &CPlayer::Kick)
        .Func(_SC("Ban"), &CPlayer::Ban)
        .Func(_SC("WorldCompatible"), &CPlayer::IsWorldCompatible)
        .Func(_SC("SetColor"), &CPlayer::SetColorEx)
        .Func(_SC("Spawn"), &CPlayer::ForceSpawn)
        .Func(_SC("Select"), &CPlayer::ForceSelect)
        .Func(_SC("GiveMoney"), &CPlayer::GiveMoney)
        .Func(_SC("SetPosition"), &CPlayer::SetPositionEx)
        .Func(_SC("SetPos"), &CPlayer::SetPositionEx)
        .Func(_SC("SetSpeed"), &CPlayer::SetSpeedEx)
        .Func(_SC("SetWeapon"), &CPlayer::SetWeapon)
        .Func(_SC("GiveWeapon"), &CPlayer::GiveWeapon)
        .Func(_SC("StripWeapons"), &CPlayer::StripWeapons)
        .Func(_SC("RestoreCamera"), &CPlayer::RestoreCamera)
        .Func(_SC("SetAnim"), &CPlayer::SetAnimation)
        .Func(_SC("Animation"), &CPlayer::SetAnimation)
        .Func(_SC("Spectate"), &CPlayer::SetSpectator)
        .Func(_SC("Disembark"), &CPlayer::Disembark)
        .Func(_SC("Redirect"), &CPlayer::Redirect)
        .Func(_SC("GetMsgPrefix"), &CPlayer::GetMessagePrefix)
        .Func(_SC("SetMsgPrefix"), &CPlayer::SetMessagePrefix)
        // Raw Methods
        .SquirrelFunc(_SC("Msg"), &CPlayer::Msg)
        .SquirrelFunc(_SC("MsgP"), &CPlayer::MsgP)
        .SquirrelFunc(_SC("MsgEx"), &CPlayer::MsgEx)
        .SquirrelFunc(_SC("Message"), &CPlayer::Message)
        .SquirrelFunc(_SC("Announce"), &CPlayer::Announce)
        .SquirrelFunc(_SC("AnnounceEx"), &CPlayer::AnnounceEx)
        .SquirrelFunc(_SC("Text"), &CPlayer::Announce)
        .SquirrelFunc(_SC("TextEx"), &CPlayer::AnnounceEx)
        // Member Overloads
        .Overload< void (CPlayer::*)(const Vector3 &) const >
            (_SC("AddSpeed"), &CPlayer::AddSpeed)
        .Overload< void (CPlayer::*)(Float32, Float32, Float32) const >
            (_SC("AddSpeed"), &CPlayer::AddSpeedEx)
        .Overload< void (CPlayer::*)(const Vector3 &, const Vector3 &) const >
            (_SC("CameraPosition"), &CPlayer::SetCameraPosition)
        .Overload< void (CPlayer::*)(Float32, Float32, Float32, Float32, Float32, Float32) const >
            (_SC("CameraPosition"), &CPlayer::SetCameraPosition)
        .Overload< void (CPlayer::*)(CVehicle &) const >
            (_SC("Embark"), &CPlayer::Embark)
        .Overload< void (CPlayer::*)(CVehicle &, SQInt32, bool, bool) const >
            (_SC("Embark"), &CPlayer::Embark)
        // Static Functions
        .StaticFunc(_SC("FindByID"), &Player_FindByID)
        .StaticFunc(_SC("FindByTag"), &Player_FindByTag)
        .StaticFunc(_SC("FindActive"), &Player_FindActive)
    );
}

} // Namespace:: SqMod
