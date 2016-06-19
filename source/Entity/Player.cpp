// ------------------------------------------------------------------------------------------------
#include "Entity/Player.hpp"
#include "Entity/Vehicle.hpp"
#include "Base/Color3.hpp"
#include "Base/Color4.hpp"
#include "Base/Vector3.hpp"
#include "Library/Utils/BufferWrapper.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>

// ------------------------------------------------------------------------------------------------
#include <sqstdstring.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern SQRESULT SqGrabPlayerMessageColor(HSQUIRRELVM vm, Int32 idx, Uint32 & color, Int32 & msgidx);

// ------------------------------------------------------------------------------------------------
Color3  CPlayer::s_Color3;
Vector3 CPlayer::s_Vector3;

// ------------------------------------------------------------------------------------------------
SQChar  CPlayer::s_Buffer[SQMOD_PLAYER_TMP_BUFFER];

// ------------------------------------------------------------------------------------------------
const Int32 CPlayer::Max = SQMOD_PLAYER_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqPlayer");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
CPlayer::CPlayer(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_PLAYER_POOL))
    , m_Tag(ToStrF("%d", id)), m_Data(), m_Buffer(256), m_CircularLocks(0)
    , mBufferInitSize(256)
    , mMessageColor(0x6599FFFF)
    , mAnnounceStyle(1)
    , mDefaultAmmo(0)
    , mMessagePrefix(_SC(""))
    , mMessagePostfix(_SC(""))
    , mAnnouncePrefix(_SC(""))
    , mAnnouncePostfix(_SC(""))
    , mMessagePrefixes()
    , mLimitPrefixPostfixMessage(true)
{
    // Reset message prefixes
    for (unsigned n = 0; n < SQMOD_PLAYER_MSG_PREFIXES; ++n)
    {
        mMessagePrefixes[n].assign(_SC(""));
    }
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
    {
        return 0;
    }
    else if (m_ID > o.m_ID)
    {
        return 1;
    }
    else
    {
        return -1;
    }
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
    Function & event = Core::Get().GetPlayerEvent(m_ID, evid);
    // Is the specified callback function null?
    if (func.IsNull())
    {
        event.ReleaseGently(); // Then release the current callback
    }
    // Does this function need a custom environment?
    else if (env.IsNull())
    {
        event = func;
    }
    // Assign the specified environment and function
    else
    {
        event = Function(env.GetVM(), env, func.GetFunc());
    }
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsConnected() const
{
    return _Func->IsPlayerConnected(m_ID);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsStreamedFor(CPlayer & player) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsPlayerStreamedForPlayer(m_ID, player.GetID());
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
    // Clear any previous string (just in case)
    s_Buffer[0] = '\0';
    // Query the server for the IP of the managed player
    if (_Func->GetPlayerIP(m_ID, s_Buffer, sizeof(s_Buffer)) == vcmpErrorBufferTooSmall)
    {
        STHROWF("The available buffer was too small to contain the IP address");
    }
    // Return the requested information
    return s_Buffer;
}

// ------------------------------------------------------------------------------------------------
CSStr CPlayer::GetUID() const
{
    // Validate the managed identifier
    Validate();
    // Clear any previous string (just in case)
    s_Buffer[0] = '\0';
    // Query the server for the UID of the managed player
    if (_Func->GetPlayerUID(m_ID, s_Buffer, sizeof(s_Buffer)) == vcmpErrorBufferTooSmall)
    {
        STHROWF("The available buffer was too small to contain the unique id");
    }
    // Return the requested information
    return s_Buffer;
}

// ------------------------------------------------------------------------------------------------
CSStr CPlayer::GetUID2() const
{
    // Validate the managed identifier
    Validate();
    // Clear any previous string (just in case)
    s_Buffer[0] = '\0';
    // Query the server for the UID2 of the managed player
    if (_Func->GetPlayerUID2(m_ID, s_Buffer, sizeof(s_Buffer)) == vcmpErrorBufferTooSmall)
    {
        STHROWF("The available buffer was too small to contain the unique id v2");
    }
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
Uint32 CPlayer::GetKey() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerKey(m_ID);
}

// ------------------------------------------------------------------------------------------------
CSStr CPlayer::GetName() const
{
    // Validate the managed identifier
    Validate();
    // Clear any previous string (just in case)
    s_Buffer[0] = '\0';
    // Query the server for the name of the managed player
    if (_Func->GetPlayerName(m_ID, s_Buffer, sizeof(s_Buffer)) == vcmpErrorBufferTooSmall)
    {
        STHROWF("The available buffer was too small to contain the nickname");
    }
    // Return the requested information
    return s_Buffer;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetName(CSStr name) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    if (_Func->SetPlayerName(m_ID, name) == vcmpErrorInvalidName)
    {
        STHROWF("The specified name is invalid");
    }
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
Int32 CPlayer::GetOption(Int32 option_id) const
{
    // Attempt to obtain the current value of the specified option
    const bool value = _Func->GetPlayerOption(m_ID, static_cast< vcmpPlayerOption >(option_id));
    // Check for errors
    if (_Func->GetLastError() == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid option identifier: %d", option_id);
    }
    // Return the requested value
    return value;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetOption(Int32 option_id, bool toggle)
{
    // Attempt to obtain the current value of the specified option
    const bool value = _Func->GetPlayerOption(m_ID, static_cast< vcmpPlayerOption >(option_id));
    // Do we even have to modify the specified option?
    if (value == toggle)
    {
        return; // Nothing to change!
    }
    // Attempt to modify the current value of the specified option
    else if (_Func->SetPlayerOption(m_ID, static_cast< vcmpPlayerOption >(option_id),
                                toggle) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid option identifier: %d", option_id);
    }
    else if (!(m_CircularLocks & PCL_EMIT_PLAYER_OPTION))
    {
        Core::Get().EmitPlayerOption(m_ID, option_id, value, 0, NullObject());
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetOptionEx(Int32 option_id, bool toggle, Int32 header, Object & payload)
{
    // Attempt to obtain the current value of the specified option
    const bool value = _Func->GetPlayerOption(m_ID, static_cast< vcmpPlayerOption >(option_id));
    // Do we even have to modify the specified option?
    if (value == toggle)
    {
        return; // Nothing to change!
    }
    // Attempt to modify the current value of the specified option
    else if (_Func->SetPlayerOption(m_ID, static_cast< vcmpPlayerOption >(option_id),
                                toggle) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid option identifier: %d", option_id);
    }
    else if (!(m_CircularLocks & PCL_EMIT_PLAYER_OPTION))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, PCL_EMIT_PLAYER_OPTION);
        // Now forward the event call
        Core::Get().EmitPlayerOption(m_ID, option_id, value, header, payload);
    }
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
Int32 CPlayer::GetSecondaryWorld() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerSecondaryWorld(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetSecondaryWorld(Int32 world) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerSecondaryWorld(m_ID, world);
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
Int32 CPlayer::GetClass() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerClass(m_ID);
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
    if (_Func->SetPlayerTeam(m_ID, team) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid team identifier: %d", team);
    }
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
    if (_Func->SetPlayerSkin(m_ID, skin) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid skin identifier: %d", skin);
    }
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
bool CPlayer::IsSpawned() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsPlayerSpawned(m_ID);
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
bool CPlayer::IsTyping() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsPlayerTyping(m_ID);
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
    // Clear previous information, if any
    s_Vector3.Clear();
    // Query the server for the values
    _Func->GetPlayerPosition(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetPosition(const Vector3 & pos) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerPosition(m_ID, pos.x, pos.y, pos.z);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetPositionEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerPosition(m_ID, x, y, z);
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
const Vector3 & CPlayer::GetAimPosition() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    s_Vector3.Clear();
    // Query the server for the values
    _Func->GetPlayerAimPosition(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CPlayer::GetAimDirection() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous direction information, if any
    s_Vector3.Clear();
    // Query the server for the direction values
    _Func->GetPlayerAimDirection(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsBurning() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsPlayerOnFire(m_ID);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsCrouched() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsPlayerCrouching(m_ID);
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
bool CPlayer::Embark(CVehicle & vehicle) const
{
    // Is the specified vehicle even valid?
    if (!vehicle.IsActive())
    {
        STHROWF("Invalid vehicle argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return (_Func->PutPlayerInVehicle(m_ID, vehicle.GetID(), 0, true, true)
            != vcmpErrorRequestDenied);
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::Embark(CVehicle & vehicle, Int32 slot, bool allocate, bool warp) const
{
    // Is the specified vehicle even valid?
    if (!vehicle.IsActive())
    {
        STHROWF("Invalid vehicle argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return (_Func->PutPlayerInVehicle(m_ID, vehicle.GetID(), slot, allocate, warp)
            != vcmpErrorRequestDenied);
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
Int32 CPlayer::GetVehicleStatus() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerInVehicleStatus(m_ID);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetVehicleSlot() const
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
    // Retrieve the identifier of the vehicle
    const Int32 id = _Func->GetPlayerVehicleId(m_ID);
    // Validate the obtained identifier
    if (VALID_ENTITYEX(id, SQMOD_VEHICLE_POOL))
    {
        // Return the requested information
        return Core::Get().GetVehicle(id).mObj;
    }
    // Default to a null object
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetVehicleID() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerVehicleId(m_ID);
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
void CPlayer::SetWeapon(Int32 wep) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    if (_Func->SetPlayerWeapon(m_ID, wep, mDefaultAmmo) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid weapon identifier: %d", wep);
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetWeaponEx(Int32 wep, Int32 ammo) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    if (_Func->SetPlayerWeapon(m_ID, wep, ammo) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid weapon ammo: %d", ammo);
    }
}

// ------------------------------------------------------------------------------------------------
void CPlayer::GiveWeapon(Int32 wep, Int32 ammo) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    if (_Func->GivePlayerWeapon(m_ID, wep, ammo) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid weapon ammo: %d", ammo);
    }
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetAmmo() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerWeaponAmmo(m_ID);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetWeaponSlot() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPlayerWeaponSlot(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetWeaponSlot(Int32 slot) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    if (_Func->SetPlayerWeaponSlot(m_ID, slot) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid weapon slot: %d", slot);
    }
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetWeaponAtSlot(Int32 slot) const
{
    // Attempt to obtain the weapon identifier of the specified slot
    const Int32 id = _Func->GetPlayerWeaponAtSlot(m_ID, slot);
    // Check for errors
    if (_Func->GetLastError() == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid weapon slot: %d", slot);
    }
    // Return the requested information
    return id;
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetAmmoAtSlot(Int32 slot) const
{
    // Attempt to obtain the weapon ammo of the specified slot
    const Int32 ammo = _Func->GetPlayerAmmoAtSlot(m_ID, slot);
    // Check for errors
    if (_Func->GetLastError() == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid weapon slot: %d", slot);
    }
    // Return the requested information
    return ammo;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::RemoveWeapon(Int32 wep) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RemovePlayerWeapon(m_ID, wep);
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
void CPlayer::SetAnimation(Int32 anim) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerAnimation(m_ID, 0, anim);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetAnimation(Int32 anim, Int32 group) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerAnimation(m_ID, group, anim);
}

// ------------------------------------------------------------------------------------------------
Object & CPlayer::StandingOnVehicle() const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the identifier of the vehicle
    const Int32 id = _Func->GetPlayerStandingOnVehicle(m_ID);
    // Validate the obtained identifier
    if (VALID_ENTITYEX(id, SQMOD_VEHICLE_POOL))
    {
        // Return the requested information
        return Core::Get().GetVehicle(id).mObj;
    }
    // Default to a null object
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
Object & CPlayer::StandingOnObject() const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the identifier of the object
    const Int32 id = _Func->GetPlayerStandingOnObject(m_ID);
    // Validate the obtained identifier
    if (VALID_ENTITYEX(id, SQMOD_OBJECT_POOL))
    {
        // Return the requested information
        return Core::Get().GetObject(id).mObj;
    }
    // Default to a null object
    return NullObject();
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
    // Retrieve the identifier of the player
    const Int32 id = _Func->GetPlayerSpectateTarget(m_ID);
    // Validated the obtained identifier
    if (_Func->GetLastError() != vcmpErrorNoSuchEntity && VALID_ENTITYEX(id, SQMOD_PLAYER_POOL))
    {
        // Return the requested information
        return Core::Get().GetPlayer(id).mObj;
    }
    // Default to a null object
    return NullObject();
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
void CPlayer::Redirect(CSStr ip, Uint32 port, CSStr nick, CSStr server_pass, CSStr user_pass)
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    if (_Func->RedirectPlayerToServer(m_ID, ip, port,
                                        nick, server_pass, user_pass) == vcmpErrorNullArgument)
    {
        STHROWF("Invalid arguments encountered");
    }
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetAuthority() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetPlayer(m_ID).mAuthority;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetAuthority(Int32 level) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    Core::Get().GetPlayer(m_ID).mAuthority = level;
}

// ------------------------------------------------------------------------------------------------
const String & CPlayer::GetMessagePrefix(Uint32 index) const
{
    // Perform a range check on the specified prefix index
    if (index >= SQMOD_PLAYER_MSG_PREFIXES)
    {
        STHROWF("Prefix index is out of range: %u >= %d", index, SQMOD_PLAYER_MSG_PREFIXES);
    }
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return mMessagePrefixes[index];
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetMessagePrefix(Uint32 index, CSStr prefix)
{
    // Perform a range check on the specified prefix index
    if (index >= SQMOD_PLAYER_MSG_PREFIXES)
    {
        STHROWF("Prefix index is out of range: %u >= %d", index, SQMOD_PLAYER_MSG_PREFIXES);
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    mMessagePrefixes[index].assign(prefix);
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::GetTrackPosition() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetPlayer(m_ID).mTrackPosition;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetTrackPosition(SQInteger num) const
{
    // Validate the managed identifier
    Validate();
    // Assign the requested information
    Core::Get().GetPlayer(m_ID).mTrackPosition = num;
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetLastWeapon() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetPlayer(m_ID).mLastWeapon;
}

// ------------------------------------------------------------------------------------------------
Float32 CPlayer::GetLastHealth() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetPlayer(m_ID).mLastHealth;
}

// ------------------------------------------------------------------------------------------------
Float32 CPlayer::GetLastArmour() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetPlayer(m_ID).mLastArmour;
}

// ------------------------------------------------------------------------------------------------
Float32 CPlayer::GetLastHeading() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetPlayer(m_ID).mLastHeading;
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CPlayer::GetLastPosition() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetPlayer(m_ID).mLastPosition;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::StartStream()
{
    // Validate the managed identifier
    Validate();
    // Initialize the stream buffer
    m_Buffer.Adjust(mBufferInitSize);
    m_Buffer.Move(0);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::StartStream(Uint32 size)
{
    // Validate the managed identifier
    Validate();
    // Initialize the stream buffer
    m_Buffer.Adjust(size);
    m_Buffer.Move(0);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetBufferCursor() const
{
    return m_Buffer.Position();
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetBufferCursor(Int32 pos)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    m_Buffer.Move(pos);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::StreamByte(SQInteger val)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    m_Buffer.Push< Uint8 >(ConvTo< Uint8 >::From(val));
}

// ------------------------------------------------------------------------------------------------
void CPlayer::StreamShort(SQInteger val)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    m_Buffer.Push< Int16 >(ConvTo< Int16 >::From(val));
}

// ------------------------------------------------------------------------------------------------
void CPlayer::StreamInt(SQInteger val)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    m_Buffer.Push< Int32 >(ConvTo< Int32 >::From(val));
}

// ------------------------------------------------------------------------------------------------
void CPlayer::StreamFloat(SQFloat val)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    m_Buffer.Push< Float32 >(ConvTo< Float32 >::From(val));
}

// ------------------------------------------------------------------------------------------------
void CPlayer::StreamString(CSStr val)
{
    // Validate the managed identifier
    Validate();
    // Is the given string value even valid?
    if (!val)
    {
        STHROWF("Invalid string argument: null");
    }
    // Calculate the string length
    Uint16 length = ConvTo< Uint16 >::From(std::strlen(val));
    // Change the size endianness to big endian
    Uint16 size = ((length >> 8) & 0xFF) | ((length & 0xFF) << 8);
    // Write the size and then the string contents
    m_Buffer.Push< Uint16 >(size);
    m_Buffer.AppendS(val, length);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::StreamRawString(CSStr val)
{
    // Validate the managed identifier
    Validate();
    // Is the given string value even valid?
    if (!val)
    {
        STHROWF("Invalid string argument: null");
    }
    // Calculate the string length
    Uint16 length = ConvTo< Uint16 >::From(std::strlen(val));
    // Write the the string contents
    m_Buffer.AppendS(val, length);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::FlushStream(bool reset)
{
    // Validate the managed identifier
    Validate();
    // Do we even have what to send?
    if (!m_Buffer)
    {
        STHROWF("Cannot send uninitialized stream buffer");
    }
    else if (!m_Buffer.Position())
    {
        STHROWF("Cannot send empty stream buffer");
    }
    // Attempt to send the stream buffer contents
    const vcmpError result = _Func->SendClientScriptData(m_ID, m_Buffer.Data(), m_Buffer.Position());
    // Should we reset the buffer cursor?
    if (reset)
    {
        m_Buffer.Move(0);
    }
    // Check for errors
    if (result == vcmpErrorTooLargeInput)
    {
        STHROWF("Stream buffer is too big");
    }
}

// ------------------------------------------------------------------------------------------------
Uint32 CPlayer::GetBufferCapacity() const
{
    return m_Buffer.Capacity();
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SendBuffer(const BufferWrapper & buffer) const
{
    // Validate the managed identifier
    Validate();
    // Validate the specified buffer
    buffer.ValidateDeeper();
    // Validate the buffer cursor
    if (!buffer.GetRef()->Position())
    {
        STHROWF("Cannot send empty stream buffer");
    }
    // Attempt to send the stream buffer contents
    const vcmpError result = _Func->SendClientScriptData(m_ID, buffer.GetRef()->Data(),
                                                                buffer.GetRef()->Position());
    // Check for errors
    if (result == vcmpErrorTooLargeInput)
    {
        STHROWF("Stream buffer is too big");
    }
}

// ------------------------------------------------------------------------------------------------
Float32 CPlayer::GetPositionX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    s_Vector3.x = 0;
    // Query the server for the requested component value
    _Func->GetPlayerPosition(m_ID, &s_Vector3.x, nullptr, nullptr);
    // Return the requested information
    return s_Vector3.x;
}

// ------------------------------------------------------------------------------------------------
Float32 CPlayer::GetPositionY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    s_Vector3.y = 0;
    // Query the server for the requested component value
    _Func->GetPlayerPosition(m_ID, nullptr, &s_Vector3.y, nullptr);
    // Return the requested information
    return s_Vector3.y;
}

// ------------------------------------------------------------------------------------------------
Float32 CPlayer::GetPositionZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    s_Vector3.z = 0;
    // Query the server for the requested component value
    _Func->GetPlayerPosition(m_ID, nullptr, nullptr, &s_Vector3.z);
    // Return the requested information
    return s_Vector3.z;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetPositionX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetPlayerPosition(m_ID, nullptr, &s_Vector3.y, &s_Vector3.z);
    // Perform the requested operation
    _Func->SetPlayerPosition(m_ID, x, s_Vector3.y, s_Vector3.z);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetPositionY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetPlayerPosition(m_ID, &s_Vector3.x, nullptr, &s_Vector3.z);
    // Perform the requested operation
    _Func->SetPlayerPosition(m_ID, s_Vector3.x, y, s_Vector3.z);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetPositionZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetPlayerPosition(m_ID, &s_Vector3.x, &s_Vector3.y, nullptr);
    // Perform the requested operation
    _Func->SetPlayerPosition(m_ID, s_Vector3.z, s_Vector3.y, z);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetColorR() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return static_cast< Int32 >((_Func->GetPlayerColour(m_ID) >> 16) & 0xFF);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetColorG() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return static_cast< Int32 >((_Func->GetPlayerColour(m_ID) >> 8) & 0xFF);
}

// ------------------------------------------------------------------------------------------------
Int32 CPlayer::GetColorB() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return static_cast< Int32 >(_Func->GetPlayerColour(m_ID) & 0xFF);
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetColorR(Int32 r) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerColour(m_ID, (ConvTo< Uint8 >::From(r) << 16) |
                                    (~(0xFF << 16) & _Func->GetPlayerColour(m_ID)));
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetColorG(Int32 g) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerColour(m_ID, (ConvTo< Uint8 >::From(g) << 8) |
                                    (~(0xFF << 8) & _Func->GetPlayerColour(m_ID)));
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetColorB(Int32 g) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPlayerColour(m_ID, (ConvTo< Uint8 >::From(g)) |
                                    (~(0xFF) & _Func->GetPlayerColour(m_ID)));
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::Msg(HSQUIRRELVM vm)
{
    // The function needs at least 2 arguments
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
    CPlayer * player;
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
        return sq_throwerror(vm, "Invalid player instance ");
    }
    // Do we have a valid player identifier?
    else if (!player->IsActive())
    {
        return sq_throwerror(vm, ToStrF("Invalid player reference [%s]", player->GetTag().c_str()));
    }

    // The index where the message should start
    Int32 msgidx = 2;
    // The message color
    Uint32 color = 0;
    // Attempt to identify and extract the color
    const SQRESULT res = SqGrabPlayerMessageColor(vm, 2, color, msgidx);
    // Did we fail to identify a color?
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error!
    }

    // Attempt to generate the string value
    StackStrF val(vm, msgidx);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }

    // Send the resulted message string
    const vcmpError result = _Func->SendClientMessage(player->GetID(), color,
                                                        "%s%s%s",
                                                        player->mMessagePrefix.c_str(),
                                                        val.mPtr,
                                                        player->mMessagePostfix.c_str());

    // Check the result
    if (result == vcmpErrorTooLargeInput)
    {
        return sq_throwerror(vm, ToStrF("Client message too big [%s]", player->GetTag().c_str()));
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
    // Attempt to extract the argument values
    try
    {
        player = Var< CPlayer * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
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
        return sq_throwerror(vm, ToStrF("Invalid player reference [%s]", player->GetTag().c_str()));
    }

    // The prefix index
    Uint32 index = 0;
    // Attempt to extract the argument values
    try
    {
        index = ConvTo< Uint32 >::From(Var< SQInteger >(vm, 2).value);
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.Message().c_str());
    }

    // Perform a range check on the specified prefix index
    if (index >= SQMOD_PLAYER_MSG_PREFIXES)
    {
        return sq_throwerror(vm, ToStrF("Prefix index is out of range: %u >= %u",
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
    if (player->mLimitPrefixPostfixMessage)
    {
        result = _Func->SendClientMessage(player->GetID(), player->mMessageColor, "%s%s",
                                            player->mMessagePrefixes[index].c_str(), val.mPtr);
    }
    else
    {
        result = _Func->SendClientMessage(player->GetID(), player->mMessageColor, "%s%s%s%s",
                                            player->mMessagePrefix.c_str(),
                                            player->mMessagePrefixes[index].c_str(), val.mPtr,
                                            player->mMessagePostfix.c_str());
    }

    // Check the result
    if (result == vcmpErrorTooLargeInput)
    {
        return sq_throwerror(vm, ToStrF("Client message too big [%s]", player->GetTag().c_str()));
    }
    // This function does not return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::MsgEx(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the index of the message prefix specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing prefix index");
    }
    // Was the message color specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing message color");
    }
    // Was the message value specified?
    else if (top <= 3)
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
        return sq_throwerror(vm, ToStrF("Invalid player reference [%s]", player->GetTag().c_str()));
    }

    // The prefix index
    Uint32 index = 0;
    // Attempt to extract the argument values
    try
    {
        index = ConvTo< Uint32 >::From(Var< SQInteger >(vm, 2).value);
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.Message().c_str());
    }

    // Perform a range check on the specified prefix index
    if (index >= SQMOD_PLAYER_MSG_PREFIXES)
    {
        return sq_throwerror(vm, ToStrF("Prefix index is out of range: %u >= %u",
                                        index, SQMOD_PLAYER_MSG_PREFIXES));
    }

    // The index where the message should start
    Int32 msgidx = 3;
    // The message color
    Uint32 color = 0;
    // Attempt to identify and extract the color
    const SQRESULT res = SqGrabPlayerMessageColor(vm, 3, color, msgidx);
    // Did we fail to identify a color?
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error!
    }

    // Attempt to generate the string value
    StackStrF val(vm, msgidx);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }

    vcmpError result = vcmpErrorNone;
    // Send the resulted message string
    if (player->mLimitPrefixPostfixMessage)
    {
        result = _Func->SendClientMessage(player->GetID(), color, "%s%s",
                                            player->mMessagePrefixes[index].c_str(), val.mPtr);
    }
    else
    {
        result = _Func->SendClientMessage(player->GetID(), color, "%s%s%s%s",
                                            player->mMessagePrefix.c_str(),
                                            player->mMessagePrefixes[index].c_str(), val.mPtr,
                                            player->mMessagePostfix.c_str());
    }
    // Check the result
    if (result == vcmpErrorTooLargeInput)
    {
        return sq_throwerror(vm, ToStrF("Client message too big [%s]", player->GetTag().c_str()));
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
        return sq_throwerror(vm, ToStrF("Invalid player reference [%s]", player->GetTag().c_str()));
    }

    // Attempt to generate the string value
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }

    // Send the resulted message string
    const vcmpError result = _Func->SendClientMessage(player->GetID(), player->mMessageColor,
                                                        "%s%s%s",
                                                        player->mMessagePrefix.c_str(),
                                                        val.mPtr,
                                                        player->mMessagePostfix.c_str());
    // Check the result
    if (result == vcmpErrorTooLargeInput)
    {
        return sq_throwerror(vm, ToStrF("Client message too big [%s]", player->GetTag().c_str()));
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
        return sq_throwerror(vm, ToStrF("Invalid player reference [%s]", player->GetTag().c_str()));
    }

    // Attempt to generate the string value
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }

    // Send the resulted announcement string
    const vcmpError result = _Func->SendGameMessage(player->GetID(), player->mAnnounceStyle,
                                                    "%s%s%s",
                                                    player->mAnnouncePrefix.c_str(),
                                                    val.mPtr,
                                                    player->mAnnouncePostfix.c_str());
    // Validate the result
    if (result == vcmpErrorArgumentOutOfBounds)
    {
        return sq_throwerror(vm, ToStrF("Invalid announcement style %d [%s]",
                                            player->mAnnounceStyle, player->GetTag().c_str()));
    }
    else if (result == vcmpErrorTooLargeInput)
    {
        return sq_throwerror(vm, ToStrF("Game message too big [%s]", player->GetTag().c_str()));
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
        return sq_throwerror(vm, ToStrF("Invalid player reference [%s]", player->GetTag().c_str()));
    }

    // Attempt to generate the string value
    StackStrF val(vm, 3);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }

    // Send the resulted announcement string
    const vcmpError result = _Func->SendGameMessage(player->GetID(), style, "%s%s%s",
                                                    player->mAnnouncePrefix.c_str(),
                                                    val.mPtr,
                                                    player->mAnnouncePostfix.c_str());
    // Validate the result
    if (result == vcmpErrorArgumentOutOfBounds)
    {
        return sq_throwerror(vm, "");
        return sq_throwerror(vm, ToStrF("Invalid announcement style %d [%s]",
                                            style, player->GetTag().c_str()));
    }
    else if (result == vcmpErrorTooLargeInput)
    {
        return sq_throwerror(vm, ToStrF("Game message too big [%s]", player->GetTag().c_str()));
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
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
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
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
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
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
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

// ------------------------------------------------------------------------------------------------
static const Object & Player_FindAuto(Object & by)
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

// ================================================================================================
void Register_CPlayer(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqPlayer"),
        Class< CPlayer, NoConstructor< CPlayer > >(vm, _SC("SqPlayer"))
        // Meta-methods
        .Func(_SC("_cmp"), &CPlayer::Cmp)
        .SquirrelFunc(_SC("_typename"), &CPlayer::Typename)
        .Func(_SC("_tostring"), &CPlayer::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CPlayer::Max)
        // Member Variables
        .Var(_SC("BufferInitSize"), &CPlayer::mBufferInitSize)
        .Var(_SC("MessageColor"), &CPlayer::mMessageColor)
        .Var(_SC("AnnounceStyle"), &CPlayer::mAnnounceStyle)
        .Var(_SC("DefaultAmmo"), &CPlayer::mDefaultAmmo)
        .Var(_SC("MessagePrefix"), &CPlayer::mMessagePrefix)
        .Var(_SC("MessagePostfix"), &CPlayer::mMessagePostfix)
        .Var(_SC("AnnouncePrefix"), &CPlayer::mAnnouncePrefix)
        .Var(_SC("AnnouncePostfix"), &CPlayer::mAnnouncePostfix)
        .Var(_SC("LimitPrefixPostfixMessage"), &CPlayer::mLimitPrefixPostfixMessage)
        // Core Properties
        .Prop(_SC("ID"), &CPlayer::GetID)
        .Prop(_SC("Tag"), &CPlayer::GetTag, &CPlayer::SetTag)
        .Prop(_SC("Data"), &CPlayer::GetData, &CPlayer::SetData)
        .Prop(_SC("Active"), &CPlayer::IsActive)
        // Core Methods
        .Func(_SC("Bind"), &CPlayer::BindEvent)
        // Properties
        .Prop(_SC("Connected"), &CPlayer::IsConnected)
        .Prop(_SC("Admin"), &CPlayer::GetAdmin, &CPlayer::SetAdmin)
        .Prop(_SC("IP"), &CPlayer::GetIP)
        .Prop(_SC("UID"), &CPlayer::GetUID)
        .Prop(_SC("UID2"), &CPlayer::GetUID2)
        .Prop(_SC("Key"), &CPlayer::GetKey)
        .Prop(_SC("Name"), &CPlayer::GetName, &CPlayer::SetName)
        .Prop(_SC("State"), &CPlayer::GetState)
        .Prop(_SC("World"), &CPlayer::GetWorld, &CPlayer::SetWorld)
        .Prop(_SC("SecWorld"), &CPlayer::GetSecondaryWorld, &CPlayer::SetSecondaryWorld)
        .Prop(_SC("SecondaryWorld"), &CPlayer::GetSecondaryWorld, &CPlayer::SetSecondaryWorld)
        .Prop(_SC("UniqueWorld"), &CPlayer::GetUniqueWorld)
        .Prop(_SC("Class"), &CPlayer::GetClass)
        .Prop(_SC("Team"), &CPlayer::GetTeam, &CPlayer::SetTeam)
        .Prop(_SC("Skin"), &CPlayer::GetSkin, &CPlayer::SetSkin)
        .Prop(_SC("Color"), &CPlayer::GetColor, &CPlayer::SetColor)
        .Prop(_SC("Colour"), &CPlayer::GetColor, &CPlayer::SetColor)
        .Prop(_SC("Spawned"), &CPlayer::IsSpawned)
        .Prop(_SC("Typing"), &CPlayer::IsTyping)
        .Prop(_SC("Money"), &CPlayer::GetMoney, &CPlayer::SetMoney)
        .Prop(_SC("Score"), &CPlayer::GetScore, &CPlayer::SetScore)
        .Prop(_SC("WantedLevel"), &CPlayer::GetWantedLevel, &CPlayer::SetWantedLevel)
        .Prop(_SC("Ping"), &CPlayer::GetPing)
        .Prop(_SC("FPS"), &CPlayer::GetFPS)
        .Prop(_SC("Health"), &CPlayer::GetHealth, &CPlayer::SetHealth)
        .Prop(_SC("Armor"), &CPlayer::GetArmor, &CPlayer::SetArmor)
        .Prop(_SC("Armour"), &CPlayer::GetArmor, &CPlayer::SetArmor)
        .Prop(_SC("Immunity"), &CPlayer::GetImmunity, &CPlayer::SetImmunity)
        .Prop(_SC("Pos"), &CPlayer::GetPosition, &CPlayer::SetPosition)
        .Prop(_SC("Position"), &CPlayer::GetPosition, &CPlayer::SetPosition)
        .Prop(_SC("Speed"), &CPlayer::GetSpeed, &CPlayer::SetSpeed)
        .Prop(_SC("Angle"), &CPlayer::GetHeading, &CPlayer::SetHeading)
        .Prop(_SC("Heading"), &CPlayer::GetHeading, &CPlayer::SetHeading)
        .Prop(_SC("Alpha"), &CPlayer::GetAlpha, &CPlayer::SetAlpha)
        .Prop(_SC("AimPos"), &CPlayer::GetAimPosition)
        .Prop(_SC("AimPosition"), &CPlayer::GetAimPosition)
        .Prop(_SC("AimDir"), &CPlayer::GetAimDirection)
        .Prop(_SC("AimDirection"), &CPlayer::GetAimDirection)
        .Prop(_SC("Burning"), &CPlayer::IsBurning)
        .Prop(_SC("Crouched"), &CPlayer::IsCrouched)
        .Prop(_SC("Action"), &CPlayer::GetAction)
        .Prop(_SC("GameKeys"), &CPlayer::GetGameKeys)
        .Prop(_SC("VehicleStatus"), &CPlayer::GetVehicleStatus)
        .Prop(_SC("VehicleSlot"), &CPlayer::GetVehicleSlot)
        .Prop(_SC("Vehicle"), &CPlayer::GetVehicle)
        .Prop(_SC("VehicleID"), &CPlayer::GetVehicleID)
        .Prop(_SC("Weapon"), &CPlayer::GetWeapon, &CPlayer::SetWeapon)
        .Prop(_SC("Ammo"), &CPlayer::GetAmmo)
        .Prop(_SC("WeaponSlot"), &CPlayer::GetWeaponSlot, &CPlayer::SetWeaponSlot)
        .Prop(_SC("CameraLocked"), &CPlayer::IsCameraLocked)
        .Prop(_SC("TouchedVehicle"), &CPlayer::StandingOnVehicle)
        .Prop(_SC("TouchedObject"), &CPlayer::StandingOnObject)
        .Prop(_SC("Away"), &CPlayer::IsAway)
        .Prop(_SC("Spec"), &CPlayer::GetSpectator, &CPlayer::SetSpectator)
        .Prop(_SC("Authority"), &CPlayer::GetAuthority, &CPlayer::SetAuthority)
        .Prop(_SC("TrackPosition"), &CPlayer::GetTrackPosition, &CPlayer::SetTrackPosition)
        .Prop(_SC("LastWeapon"), &CPlayer::GetLastWeapon)
        .Prop(_SC("LastHealth"), &CPlayer::GetLastHealth)
        .Prop(_SC("LastArmor"), &CPlayer::GetLastArmour)
        .Prop(_SC("LastArmour"), &CPlayer::GetLastArmour)
        .Prop(_SC("LastHeading"), &CPlayer::GetLastHeading)
        .Prop(_SC("LastPosition"), &CPlayer::GetLastPosition)
        .Prop(_SC("BufferCursor"), &CPlayer::GetBufferCursor, &CPlayer::SetBufferCursor)
        .Prop(_SC("BufferCapacity"), &CPlayer::GetBufferCapacity)
        .Prop(_SC("PosX"), &CPlayer::GetPositionX, &CPlayer::SetPositionX)
        .Prop(_SC("PosY"), &CPlayer::GetPositionY, &CPlayer::SetPositionY)
        .Prop(_SC("PosZ"), &CPlayer::GetPositionZ, &CPlayer::SetPositionZ)
        .Prop(_SC("Red"), &CPlayer::GetColorR, &CPlayer::SetColorR)
        .Prop(_SC("Green"), &CPlayer::GetColorG, &CPlayer::SetColorG)
        .Prop(_SC("Blue"), &CPlayer::GetColorB, &CPlayer::SetColorB)
        // Member Methods
        .Func(_SC("StreamedFor"), &CPlayer::IsStreamedFor)
        .Func(_SC("Kick"), &CPlayer::Kick)
        .Func(_SC("Ban"), &CPlayer::Ban)
        .Func(_SC("GetOption"), &CPlayer::GetOption)
        .Func(_SC("SetOption"), &CPlayer::SetOption)
        .Func(_SC("SetOptionEx"), &CPlayer::SetOptionEx)
        .Func(_SC("WorldCompatible"), &CPlayer::IsWorldCompatible)
        .Func(_SC("SetColor"), &CPlayer::SetColorEx)
        .Func(_SC("SetColour"), &CPlayer::SetColorEx)
        .Func(_SC("Spawn"), &CPlayer::ForceSpawn)
        .Func(_SC("Select"), &CPlayer::ForceSelect)
        .Func(_SC("GiveMoney"), &CPlayer::GiveMoney)
        .Func(_SC("SetPos"), &CPlayer::SetPositionEx)
        .Func(_SC("SetPosition"), &CPlayer::SetPositionEx)
        .Func(_SC("SetSpeed"), &CPlayer::SetSpeedEx)
        .Func(_SC("Disembark"), &CPlayer::Disembark)
        .Func(_SC("SetWeapon"), &CPlayer::SetWeaponEx)
        .Func(_SC("GiveWeapon"), &CPlayer::GiveWeapon)
        .Func(_SC("WeaponAtSlot"), &CPlayer::GetWeaponAtSlot)
        .Func(_SC("AmmoAtSlot"), &CPlayer::GetAmmoAtSlot)
        .Func(_SC("RemoveWeapon"), &CPlayer::RemoveWeapon)
        .Func(_SC("StripWeapons"), &CPlayer::StripWeapons)
        .Func(_SC("RestoreCamera"), &CPlayer::RestoreCamera)
        .Func(_SC("Spectating"), &CPlayer::GetSpectator)
        .Func(_SC("Spectate"), &CPlayer::SetSpectator)
        .Func(_SC("Redirect"), &CPlayer::Redirect)
        .Func(_SC("GetMsgPrefix"), &CPlayer::GetMessagePrefix)
        .Func(_SC("SetMsgPrefix"), &CPlayer::SetMessagePrefix)
        .Func(_SC("StreamByte"), &CPlayer::StreamByte)
        .Func(_SC("StreamShort"), &CPlayer::StreamShort)
        .Func(_SC("StreamInt"), &CPlayer::StreamInt)
        .Func(_SC("StreamFloat"), &CPlayer::StreamFloat)
        .Func(_SC("StreamString"), &CPlayer::StreamString)
        .Func(_SC("StreamRawString"), &CPlayer::StreamRawString)
        .Func(_SC("FlushStream"), &CPlayer::FlushStream)
        .Func(_SC("SendBuffer"), &CPlayer::SendBuffer)
        // Member Overloads
        .Overload< void (CPlayer::*)(const Vector3 &) const >
            (_SC("AddSpeed"), &CPlayer::AddSpeed)
        .Overload< void (CPlayer::*)(Float32, Float32, Float32) const >
            (_SC("AddSpeed"), &CPlayer::AddSpeedEx)
        .Overload< bool (CPlayer::*)(CVehicle &) const >
            (_SC("Embark"), &CPlayer::Embark)
        .Overload< bool (CPlayer::*)(CVehicle &, SQInt32, bool, bool) const >
            (_SC("Embark"), &CPlayer::Embark)
        .Overload< void (CPlayer::*)(const Vector3 &, const Vector3 &) const >
            (_SC("CameraPosition"), &CPlayer::SetCameraPosition)
        .Overload< void (CPlayer::*)(Float32, Float32, Float32, Float32, Float32, Float32) const >
            (_SC("CameraPosition"), &CPlayer::SetCameraPosition)
        .Overload< void (CPlayer::*)(void) >
            (_SC("StartStream"), &CPlayer::StartStream)
        .Overload< void (CPlayer::*)(Uint32) >
            (_SC("StartStream"), &CPlayer::StartStream)
        .Overload< void (CPlayer::*)(Int32) const >
            (_SC("SetAnimation"), &CPlayer::SetAnimation)
        .Overload< void (CPlayer::*)(Int32, Int32) const >
            (_SC("SetAnimation"), &CPlayer::SetAnimation)
        // Raw Squirrel Methods
        .SquirrelFunc(_SC("Msg"), &CPlayer::Msg)
        .SquirrelFunc(_SC("MsgP"), &CPlayer::MsgP)
        .SquirrelFunc(_SC("MsgEx"), &CPlayer::MsgEx)
        .SquirrelFunc(_SC("Message"), &CPlayer::Message)
        .SquirrelFunc(_SC("Announce"), &CPlayer::Announce)
        .SquirrelFunc(_SC("AnnounceEx"), &CPlayer::AnnounceEx)
        .SquirrelFunc(_SC("Text"), &CPlayer::Announce)
        .SquirrelFunc(_SC("TextEx"), &CPlayer::AnnounceEx)
        // Static Functions
        .StaticFunc(_SC("Find"), &Player_FindAuto)
        .StaticFunc(_SC("FindByID"), &Player_FindByID)
        .StaticFunc(_SC("FindByTag"), &Player_FindByTag)
        .StaticFunc(_SC("FindActive"), &Player_FindActive)
    );
}

} // Namespace:: SqMod
