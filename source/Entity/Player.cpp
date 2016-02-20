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
SQChar CPlayer::s_StrID[SQMOD_PLAYER_POOL][8];

// ------------------------------------------------------------------------------------------------
const Int32 CPlayer::Max = SQMOD_PLAYER_POOL;

// ------------------------------------------------------------------------------------------------
CPlayer::CPlayer(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_PLAYER_POOL))
    , m_Tag(VALID_ENTITY(m_ID) ? s_StrID[m_ID] : _SC("-1"))
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

CSStr CPlayer::ToString() const
{
    return VALID_ENTITYEX(m_ID, SQMOD_PLAYER_POOL) ? s_StrID[m_ID] : _SC("-1");
}

// ------------------------------------------------------------------------------------------------
CSStr CPlayer::GetTag() const
{
    return m_Tag.c_str();
}

void CPlayer::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

Object & CPlayer::GetData()
{
    if (Validate())
        return m_Data;
    return NullObject();
}

void CPlayer::SetData(Object & data)
{
    if (Validate())
        m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::BindEvent(Int32 evid, Object & env, Function & func) const
{
    if (!Validate())
        return false;

    Function & event = _Core->GetPlayerEvent(m_ID, evid);

    if (func.IsNull())
        event.Release();
    else
        event = Function(env.GetVM(), env, func.GetFunc());

    return true;
}

// ------------------------------------------------------------------------------------------------
bool CPlayer::IsStreamedFor(CPlayer & player) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        return _Func->IsPlayerStreamedForPlayer(m_ID, player.GetID());
    return false;
}

Int32 CPlayer::GetClass() const
{
    if (Validate())
        return _Func->GetPlayerClass(m_ID);
    return -1;
}

bool CPlayer::GetAdmin() const
{
    if (Validate())
        return _Func->IsPlayerAdmin(m_ID);
    return false;
}

void CPlayer::SetAdmin(bool toggle) const
{
    if (Validate())
        _Func->SetPlayerAdmin(m_ID, toggle);
}

CSStr CPlayer::GetIP() const
{
    s_Buffer[0] = 0;

    if (Validate())
    {
        memset(s_Buffer, 0, sizeof(s_Buffer));
        _Func->GetPlayerIP(m_ID, s_Buffer, sizeof(s_Buffer));
    }

    return s_Buffer;
}

void CPlayer::Kick() const
{
    if (Validate())
        _Func->KickPlayer(m_ID);
}

void CPlayer::Ban() const
{
    if (Validate())
        _Func->BanPlayer(m_ID);
}

bool CPlayer::IsConnected() const
{
    return _Func->IsPlayerConnected(m_ID);
}

bool CPlayer::IsSpawned() const
{
    if (Validate())
        return _Func->IsPlayerSpawned(m_ID);
    return false;
}

Uint32 CPlayer::GetKey() const
{
    if (Validate())
        return _Func->GetPlayerKey(m_ID);
    return 0;
}

Int32 CPlayer::GetWorld() const
{
    if (Validate())
        return _Func->GetPlayerWorld(m_ID);
    return -1;
}

void CPlayer::SetWorld(Int32 world) const
{
    if (Validate())
        _Func->SetPlayerWorld(m_ID, world);
}

Int32 CPlayer::GetSecWorld() const
{
    if (Validate())
        return _Func->GetPlayerSecWorld(m_ID);
    return -1;
}

void CPlayer::SetSecWorld(Int32 world) const
{
    if (Validate())
        _Func->SetPlayerSecWorld(m_ID, world);
}

Int32 CPlayer::GetUniqueWorld() const
{
    if (Validate())
        return _Func->GetPlayerUniqueWorld(m_ID);
    return -1;
}

bool CPlayer::IsWorldCompatible(Int32 world) const
{
    if (Validate())
        return _Func->IsPlayerWorldCompatible(m_ID, world);
    return false;
}

CSStr CPlayer::GetName() const
{
    s_Buffer[0] = 0;

    if (Validate())
    {
        memset(s_Buffer, 0, sizeof(s_Buffer));
        _Func->GetPlayerName(m_ID, s_Buffer, sizeof(s_Buffer));
    }

    return s_Buffer;
}

void CPlayer::SetName(CSStr name) const
{
    if (Validate())
        _Func->SetPlayerName(m_ID, static_cast< CCStr >(name));
}

Int32 CPlayer::GetTeam() const
{
    if (Validate())
        return _Func->GetPlayerTeam(m_ID);
    return -1;
}

void CPlayer::SetTeam(Int32 team) const
{
    if (Validate())
        _Func->SetPlayerTeam(m_ID, team);
}

Int32 CPlayer::GetSkin() const
{
    if (Validate())
        return _Func->GetPlayerSkin(m_ID);
    return -1;
}

void CPlayer::SetSkin(Int32 skin) const
{

    if (Validate())
        _Func->SetPlayerSkin(m_ID, skin);
}

const Color3 & CPlayer::GetColor() const
{
    s_Color3.Clear();
    if (Validate())
        s_Color3.SetRGB(_Func->GetPlayerColour(m_ID));
    return s_Color3;
}

void CPlayer::SetColor(const Color3 & color) const
{
    if (Validate())
        _Func->SetPlayerColour(m_ID, color.GetRGB());
}

void CPlayer::SetColorEx(Uint8 r, Uint8 g, Uint8 b) const
{
    if (Validate())
        _Func->SetPlayerColour(m_ID, SQMOD_PACK_RGB(r, g, b));
}

void CPlayer::ForceSpawn() const
{
    if (Validate())
        _Func->ForcePlayerSpawn(m_ID);
}

void CPlayer::ForceSelect() const
{
    if (Validate())
        _Func->ForcePlayerSelect(m_ID);
}

Int32 CPlayer::GetMoney() const
{
    if (Validate())
        return _Func->GetPlayerMoney(m_ID);
    return -1;
}

void CPlayer::SetMoney(Int32 amount) const
{
    if (Validate())
        _Func->SetPlayerMoney(m_ID, amount);
}

void CPlayer::GiveMoney(Int32 amount) const
{
    if (Validate())
        _Func->GivePlayerMoney(m_ID, amount);
}

Int32 CPlayer::GetScore() const
{
    if (Validate())
        return _Func->GetPlayerScore(m_ID);
    return -1;
}

void CPlayer::SetScore(Int32 score) const
{
    if (Validate())
        _Func->SetPlayerScore(m_ID, score);
}

Int32 CPlayer::GetPing() const
{
    if (Validate())
        return _Func->GetPlayerPing(m_ID);
    return -1;
}

Float32 CPlayer::GetFPS() const
{
    if (Validate())
        return _Func->GetPlayerFPS(m_ID);
    return 0;
}

bool CPlayer::IsTyping() const
{
    if (Validate())
        return _Func->IsPlayerTyping(m_ID);
    return false;
}

CSStr CPlayer::GetUID() const
{
    s_Buffer[0] = 0;

    if (Validate())
    {
        memset(s_Buffer, 0, sizeof(s_Buffer));
        _Func->GetPlayerUID(m_ID, s_Buffer, sizeof(s_Buffer));
    }

    return s_Buffer;
}

CSStr CPlayer::GetUID2() const
{
    s_Buffer[0] = 0;

    if (Validate())
    {
        memset(s_Buffer, 0, sizeof(s_Buffer));
        _Func->GetPlayerUID2(m_ID, s_Buffer, sizeof(s_Buffer));
    }

    return s_Buffer;
}

Float32 CPlayer::GetHealth() const
{
    if (Validate())
        return _Func->GetPlayerHealth(m_ID);
    return false;
}

void CPlayer::SetHealth(Float32 amount) const
{
    if (Validate())
        _Func->SetPlayerHealth(m_ID, amount);
}

Float32 CPlayer::GetArmor() const
{
    if (Validate())
        return _Func->GetPlayerArmour(m_ID);
    return false;
}

void CPlayer::SetArmor(Float32 amount) const
{
    if (Validate())
        _Func->SetPlayerArmour(m_ID, amount);
}

Int32 CPlayer::GetImmunity() const
{
    if (Validate())
        return _Func->GetPlayerImmunityFlags(m_ID);
    return 0;
}

void CPlayer::SetImmunity(Int32 flags) const
{
    if (Validate())
        _Func->SetPlayerImmunityFlags(m_ID, flags);
}

const Vector3 & CPlayer::GetPosition() const
{
    s_Vector3.Clear();
    if (Validate())
        _Func->GetPlayerPos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    return s_Vector3;
}

void CPlayer::SetPosition(const Vector3 & pos) const
{
    if (Validate())
        _Func->SetPlayerPos(m_ID, pos.x, pos.y, pos.z);
}

void CPlayer::SetPositionEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->SetPlayerPos(m_ID, x, y, z);
}

const Vector3 & CPlayer::GetSpeed() const
{
    s_Vector3.Clear();
    if (Validate())
        _Func->GetPlayerSpeed(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    return s_Vector3;
}

void CPlayer::SetSpeed(const Vector3 & vel) const
{
    if (Validate())
        _Func->SetPlayerSpeed(m_ID, vel.x, vel.y, vel.z);
}

void CPlayer::SetSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->SetPlayerSpeed(m_ID, x, y, z);
}

void CPlayer::AddSpeed(const Vector3 & vel) const
{
    if (Validate())
        _Func->AddPlayerSpeed(m_ID, vel.x, vel.y, vel.z);
}

void CPlayer::AddSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->AddPlayerSpeed(m_ID, x, y, z);
}

Float32 CPlayer::GetHeading() const
{
    if (Validate())
        return _Func->GetPlayerHeading(m_ID);
    return 0;
}

void CPlayer::SetHeading(Float32 angle) const
{
    if (Validate())
        _Func->SetPlayerHeading(m_ID, angle);
}

Int32 CPlayer::GetAlpha() const
{
    if (Validate())
        return _Func->GetPlayerAlpha(m_ID);
    return -1;
}

void CPlayer::SetAlpha(Int32 alpha, Int32 fade) const
{
    if (Validate())
        _Func->SetPlayerAlpha(m_ID, alpha, fade);
}

Int32 CPlayer::GetVehicleStatus() const
{
    if (Validate())
        return _Func->GetPlayerInVehicleStatus(m_ID);
    return -1;
}

Int32 CPlayer::GetOccupiedSlot() const
{
    if (Validate())
        return _Func->GetPlayerInVehicleSlot(m_ID);
    return -1;
}

Object & CPlayer::GetVehicle() const
{
    if (Validate())
        return _Core->GetVehicle(_Func->GetPlayerVehicleID(m_ID)).mObj;
    return NullObject();
}

Int32 CPlayer::GetVehicleID() const
{
    if (Validate())
        return _Func->GetPlayerVehicleID(m_ID);
    return -1;
}

bool CPlayer::GetControllable() const
{
    if (Validate())
        return _Func->EnabledPlayerControllable(m_ID);
    return false;
}

void CPlayer::SetControllable(bool toggle) const
{
    if (Validate())
        _Func->TogglePlayerControllable(m_ID, toggle);
}

bool CPlayer::GetDriveby() const
{
    if (Validate())
        return _Func->EnabledPlayerDriveby(m_ID);
    return false;
}

void CPlayer::SetDriveby(bool toggle) const
{
    if (Validate())
        _Func->TogglePlayerDriveby(m_ID, toggle);
}

bool CPlayer::GetWhiteScanlines() const
{
    if (Validate())
        return _Func->EnabledPlayerWhiteScanlines(m_ID);
    return false;
}

void CPlayer::SetWhiteScanlines(bool toggle) const
{
    if (Validate())
        _Func->TogglePlayerWhiteScanlines(m_ID, toggle);
}

bool CPlayer::GetGreenScanlines() const
{
    if (Validate())
        return _Func->EnabledPlayerGreenScanlines(m_ID);
    return false;
}

void CPlayer::SetGreenScanlines(bool toggle) const
{
    if (Validate())
        _Func->TogglePlayerGreenScanlines(m_ID, toggle);
}

bool CPlayer::GetWidescreen() const
{
    if (Validate())
        return _Func->EnabledPlayerWidescreen(m_ID);
    return false;
}

void CPlayer::SetWidescreen(bool toggle) const
{
    if (Validate())
        _Func->TogglePlayerWidescreen(m_ID, toggle);
}

bool CPlayer::GetShowMarkers() const
{
    if (Validate())
        return _Func->EnabledPlayerShowMarkers(m_ID);
    return false;
}

void CPlayer::SetShowMarkers(bool toggle) const
{
    if (Validate())
        _Func->TogglePlayerShowMarkers(m_ID, toggle);
}

bool CPlayer::GetAttackPriv() const
{
    if (Validate())
        return _Func->EnabledPlayerAttackPriv(m_ID);
    return false;
}

void CPlayer::SetAttackPriv(bool toggle) const
{
    if (Validate())
        _Func->TogglePlayerAttackPriv(m_ID, toggle);
}

bool CPlayer::GetHasMarker() const
{
    if (Validate())
        return _Func->EnabledPlayerHasMarker(m_ID);
    return false;
}

void CPlayer::SetHasMarker(bool toggle) const
{
    if (Validate())
        _Func->TogglePlayerHasMarker(m_ID, toggle);
}

bool CPlayer::GetChatTags() const
{
    if (Validate())
        return _Func->EnabledPlayerChatTags(m_ID);
    return false;
}

void CPlayer::SetChatTags(bool toggle) const
{
    if (Validate())
        _Func->TogglePlayerChatTagsEnabled(m_ID, toggle);
}

bool CPlayer::GetDrunkEffects() const
{
    if (Validate())
        return _Func->EnabledPlayerDrunkEffects(m_ID);
    return false;
}

void CPlayer::SetDrunkEffects(bool toggle) const
{
    if (Validate())
        _Func->TogglePlayerDrunkEffects(m_ID, toggle);
}

Int32 CPlayer::GetWeapon() const
{
    if (Validate())
        return _Func->GetPlayerWeapon(m_ID);
    return -1;
}

void CPlayer::SetWeapon(Int32 wep, Int32 ammo) const
{
    if (Validate())
        _Func->SetPlayerWeapon(m_ID, wep, ammo);
}

void CPlayer::GiveWeapon(Int32 wep, Int32 ammo) const
{
    if (Validate())
        _Func->GivePlayerWeapon(m_ID, wep, ammo);
}

void CPlayer::StripWeapons() const
{
    if (Validate())
        _Func->RemoveAllWeapons(m_ID);
}

void CPlayer::SetCameraPosition(const Vector3 & pos, const Vector3 & aim) const
{
    if (Validate())
        _Func->SetCameraPosition(m_ID, pos.x, pos.y, pos.z, aim.x, aim.y, aim.z);
}

void CPlayer::SetCameraPosition(Float32 xp, Float32 yp, Float32 zp, Float32 xa, Float32 ya, Float32 za) const
{
    if (Validate())
        _Func->SetCameraPosition(m_ID, xp, yp, zp, xa, ya, za);
}

void CPlayer::RestoreCamera() const
{
    if (Validate())
        _Func->RestoreCamera(m_ID);
}

bool CPlayer::IsCameraLocked() const
{
    if (Validate())
        _Func->IsCameraLocked(m_ID);
    return false;
}

void CPlayer::SetAnimation(Int32 group, Int32 anim) const
{
    if (Validate())
        _Func->SetPlayerAnimation(m_ID, group, anim);
}

Int32 CPlayer::GetWantedLevel() const
{
    if (Validate())
        return _Func->GetPlayerWantedLevel(m_ID);
    return -1;
}

void CPlayer::SetWantedLevel(Int32 level) const
{
    if (Validate())
        _Func->SetPlayerWantedLevel(m_ID, level);
}

Object & CPlayer::StandingOnVehicle() const
{
    if (Validate())
        return _Core->GetVehicle(_Func->GetPlayerStandingOnVehicle(m_ID)).mObj;
    return NullObject();
}

Object & CPlayer::StandingOnObject() const
{
    if (Validate())
        return _Core->GetObject(_Func->GetPlayerStandingOnObject(m_ID)).mObj;
    return NullObject();
}

bool CPlayer::IsAway() const
{
    if (Validate())
        return _Func->IsPlayerAway(m_ID);
    return false;
}

Object & CPlayer::GetSpectator() const
{
    if (Validate())
        return _Core->GetPlayer(_Func->GetPlayerSpectateTarget(m_ID)).mObj;
    return NullObject();
}

void CPlayer::SetSpectator(CPlayer & target) const
{
    if (!target.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->SetPlayerSpectateTarget(m_ID, target.GetID());
}

bool CPlayer::IsBurning() const
{
    if (Validate())
        return _Func->GetPlayerOnFireStatus(m_ID);
    return false;
}

bool CPlayer::IsCrouched() const
{
    if (Validate())
        return _Func->GetPlayerCrouchStatus(m_ID);
    return false;
}

Int32 CPlayer::GetState() const
{
    if (Validate())
        return _Func->GetPlayerState(m_ID);
    return false;
}

Int32 CPlayer::GetAction() const
{
    if (Validate())
        return _Func->GetPlayerAction(m_ID);
    return -1;
}

Int32 CPlayer::GetGameKeys() const
{
    if (Validate())
        return _Func->GetPlayerGameKeys(m_ID);
    return -1;
}

const Vector3 & CPlayer::GetAimPos() const
{
    s_Vector3.Clear();
    if (Validate())
        _Func->GetPlayerAimPos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    return s_Vector3;
}

const Vector3 & CPlayer::GetAimDir() const
{
    s_Vector3.Clear();
    if (Validate())
        _Func->GetPlayerAimDir(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    return s_Vector3;
}

void CPlayer::Embark(CVehicle & vehicle) const
{
    if (!vehicle.IsActive())
        SqThrow("Invalid vehicle argument: null");
    else if (Validate())
        _Func->PutPlayerInVehicle(m_ID, vehicle.GetID(), 0, true, true);
}

void CPlayer::Embark(CVehicle & vehicle, Int32 slot, bool allocate, bool warp) const
{
    if (!vehicle.IsActive())
        SqThrow("Invalid vehicle argument: null");
    else if (Validate())
        _Func->PutPlayerInVehicle(m_ID, vehicle.GetID(), slot, allocate, warp);
}

void CPlayer::Disembark() const
{
    if (Validate())
        _Func->RemovePlayerFromVehicle(m_ID);
}

bool CPlayer::Redirect(CSStr ip, Uint32 port, CSStr nick, CSStr pass, CSStr user)
{
    if (Validate())
        return _Func->RedirectPlayerToServer(m_ID, ip, port, nick, pass, user);
    return false;
}

Int32 CPlayer::GetAuthority() const
{
    if (Validate())
        return _Core->GetPlayer(m_ID).mAuthority;
    return -1;
}

void CPlayer::SetAuthority(Int32 level) const
{
    if (Validate())
        _Core->GetPlayer(m_ID).mAuthority = level;
}

CSStr CPlayer::GetMessagePrefix(Uint32 index) const
{
    if (index >= SQMOD_PLAYER_MSG_PREFIXES)
        SqThrow("Prefix index is out of range: %u >= %d", index, SQMOD_PLAYER_MSG_PREFIXES);
    else if (Validate())
        return _Core->GetPlayer(m_ID).mPrefixes[index].c_str();
    return _SC("");
}

void CPlayer::SetMessagePrefix(Uint32 index, CSStr prefix) const
{
    if (index >= SQMOD_PLAYER_MSG_PREFIXES)
        SqThrow("Prefix index is out of range: %u >= %d", index, SQMOD_PLAYER_MSG_PREFIXES);
    else if (Validate())
        _Core->GetPlayer(m_ID).mPrefixes[index].assign(prefix);
}

Uint32 CPlayer::GetMessageColor() const
{
    if (Validate())
        return _Core->GetPlayer(m_ID).mMessageColor;
    return 0;
}

void CPlayer::SetMessageColor(Uint32 color) const
{
    if (Validate())
         _Core->GetPlayer(m_ID).mMessageColor = color;
}

Int32 CPlayer::GetAnnounceStyle() const
{
    if (Validate())
        return _Core->GetPlayer(m_ID).mAnnounceStyle;
    return -1;
}

void CPlayer::SetAnnounceStyle(Int32 style) const
{
    if (Validate())
        _Core->GetPlayer(m_ID).mAnnounceStyle = style;
}

// ------------------------------------------------------------------------------------------------
Float32 CPlayer::GetPosX() const
{
    s_Vector3.x = 0;
    if (Validate())
        _Func->GetPlayerPos(m_ID, &s_Vector3.x, NULL, NULL);
    return s_Vector3.x;
}

Float32 CPlayer::GetPosY() const
{
    s_Vector3.y = 0;
    if (Validate())
        _Func->GetPlayerPos(m_ID, NULL, &s_Vector3.y, NULL);
    return s_Vector3.y;
}

Float32 CPlayer::GetPosZ() const
{
    s_Vector3.z = 0;
    if (Validate())
        _Func->GetPlayerPos(m_ID, NULL, NULL, &s_Vector3.z);
    return s_Vector3.z;
}

// ------------------------------------------------------------------------------------------------
void CPlayer::SetPosX(Float32 x) const
{
    if (Validate())
    {
        _Func->GetPlayerPos(m_ID, NULL, &s_Vector3.y, &s_Vector3.z);
        _Func->SetPlayerPos(m_ID, x, s_Vector3.y, s_Vector3.z);
    }
}

void CPlayer::SetPosY(Float32 y) const
{
    if (Validate())
    {
        _Func->GetPlayerPos(m_ID, &s_Vector3.x, NULL, &s_Vector3.z);
        _Func->SetPlayerPos(m_ID, s_Vector3.x, y, s_Vector3.z);
    }
}

void CPlayer::SetPosZ(Float32 z) const
{
    if (Validate())
    {
        _Func->GetPlayerPos(m_ID, &s_Vector3.x, &s_Vector3.y, NULL);
        _Func->SetPlayerPos(m_ID, s_Vector3.z, s_Vector3.y, z);
    }
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::Msg(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);

    if (top <= 1)
    {
        SqThrow("Missing the message color");
        return 0;
    }
    else if (top == 2)
    {
        SqThrow("Missing the message value");
        return 0;
    }

    Var< CPlayer & > inst(vm, 1);
    Var< Color3 > color(vm, 2);

    if (!inst.value.IsActive())
        SqThrow("Invalid player reference: %d", inst.value.GetID());
    else if (top == 3 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        CSStr msg = NULL;

        if (SQ_FAILED(sq_getstring(vm, -1, &msg)))
            SqThrow("Unable to retrieve the message");
        else
            _Func->SendClientMessage(inst.value.GetID(), color.value.GetRGBA(), "%s", msg);

        sq_settop(vm, top);
    }
    else if (top > 3)
    {
        SStr msg = NULL;
        SQInteger len = 0;

        if (SQ_FAILED(sqstd_format(vm, 3, &len, &msg)))
            SqThrow("Unable to generate the player message [%s]", Error::Message(vm).c_str());
        else
            _Func->SendClientMessage(inst.value.GetID(), color.value.GetRGBA(), "%s", msg);
    }
    else
        SqThrow("Unable to extract the player message");

    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::MsgP(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);

    if (top <= 1)
    {
        SqThrow("Missing message prefix index");
        return 0;
    }
    else if (top == 2)
    {
        SqThrow("Missing the message value");
        return 0;
    }

    Var< CPlayer & > inst(vm, 1);
    Var< Uint32 > index(vm, 2);

    if (!inst.value.IsActive())
        SqThrow("Invalid player reference: %d", inst.value.GetID());
    else if (index.value > SQMOD_PLAYER_MSG_PREFIXES)
        SqThrow("Prefix index is out of range: %u > %u", index.value, SQMOD_PLAYER_MSG_PREFIXES);
    else if (top == 3 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        CSStr msg = NULL;
        const Int32 id = inst.value.GetID();

        if (SQ_FAILED(sq_getstring(vm, -1, &msg)))
            SqThrow("Unable to retrieve the message");
        else
            _Func->SendClientMessage(id, _Core->GetPlayer(id).mMessageColor, "%s%s",
                                            _Core->GetPlayer(id).mPrefixes[index.value].c_str(), msg);
        sq_settop(vm, top);
    }
    else if (top > 3)
    {
        SStr msg = NULL;
        SQInteger len = 0;
        const Int32 id = inst.value.GetID();

        if (SQ_FAILED(sqstd_format(vm, 3, &len, &msg)))
            SqThrow("Unable to generate the player message [%s]", Error::Message(vm).c_str());
        else
            _Func->SendClientMessage(id, _Core->GetPlayer(id).mMessageColor, "%s%s",
                                            _Core->GetPlayer(id).mPrefixes[index.value].c_str(), msg);
    }
    else
        SqThrow("Unable to extract the player message");

    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::MsgEx(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);

    if (top <= 1)
    {
        SqThrow("Missing the message color");
        return 0;
    }
    else if (top <= 4)
    {
        SqThrow("Missing the message value");
        return 0;
    }

    Var< CPlayer & > inst(vm, 1);

    Var< Int32 > r(vm, 2);
    Var< Int32 > g(vm, 3);
    Var< Int32 > b(vm, 4);

    if (!inst.value.IsActive())
        SqThrow("Invalid player reference: %d", inst.value.GetID());
    else if (top == 5 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        CSStr msg = NULL;

        if (SQ_FAILED(sq_getstring(vm, -1, &msg)))
            SqThrow("Unable to retrieve the message");
        else
            _Func->SendClientMessage(inst.value.GetID(),
                                        SQMOD_PACK_RGBA(r.value, g.value, b.value, 0), "%s", msg);

        sq_settop(vm, top);
    }
    else if (top > 5)
    {
        SStr msg = NULL;
        SQInteger len = 0;

        if (SQ_FAILED(sqstd_format(vm, 5, &len, &msg)))
            SqThrow("Unable to generate the player message [%s]", Error::Message(vm).c_str());
        else
            _Func->SendClientMessage(inst.value.GetID(),
                                        SQMOD_PACK_RGBA(r.value, g.value, b.value, 0), "%s", msg);
    }
    else
        SqThrow("Unable to extract the player message");

    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::Message(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);

    if (top <= 1)
    {
        SqThrow("Missing the message value");
        return 0;
    }

    Var< CPlayer & > inst(vm, 1);

    if (!inst.value.IsActive())
        SqThrow("Invalid player reference: %d", inst.value.GetID());
    else if (top == 2 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        CSStr msg = NULL;
        const Int32 id = inst.value.GetID();

        if (SQ_FAILED(sq_getstring(vm, -1, &msg)))
            SqThrow("Unable to retrieve the message");
        else
            _Func->SendClientMessage(id, _Core->GetPlayer(id).mMessageColor, "%s", msg);

        sq_settop(vm, top);
    }
    else if (top > 2)
    {
        SStr msg = NULL;
        SQInteger len = 0;
        const Int32 id = inst.value.GetID();

        if (SQ_FAILED(sqstd_format(vm, 2, &len, &msg)))
            SqThrow("Unable to generate the player message [%s]", Error::Message(vm).c_str());
        else
            _Func->SendClientMessage(id, _Core->GetPlayer(id).mMessageColor, "%s", msg);
    }
    else
        SqThrow("Unable to extract the player message");

    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::Announce(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);

    if (top <= 1)
    {
        SqThrow("Missing the announcement value");
        return 0;
    }

    Var< CPlayer & > inst(vm, 1);

    if (!inst.value.IsActive())
        SqThrow("Invalid player reference: %d", inst.value.GetID());
    else if (top == 2 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        CSStr msg = 0;
        const Int32 id = inst.value.GetID();

        if (SQ_FAILED(sq_getstring(vm, -1, &msg)))
            SqThrow("Unable to retrieve the announcement");
        else
            _Func->SendGameMessage(id, _Core->GetPlayer(id).mAnnounceStyle, "%s", msg);

        sq_settop(vm, top);
    }
    else if (top > 2)
    {
        SStr msg = NULL;
        SQInteger len = 0;
        const Int32 id = inst.value.GetID();

        if (SQ_FAILED(sqstd_format(vm, 2, &len, &msg)))
            SqThrow("Unable to generate the player announcement [%s]", Error::Message(vm).c_str());
        else
            _Func->SendGameMessage(id, _Core->GetPlayer(id).mAnnounceStyle, "%s", msg);
    }
    else
        SqThrow("Unable to extract the player message");

    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger CPlayer::AnnounceEx(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);

    if (top <= 1)
    {
        SqThrow("Missing the announcement style");
        return 0;
    }
    else if (top == 2)
    {
        SqThrow("Missing the announcement value");
        return 0;
    }

    Var< CPlayer & > inst(vm, 1);
    Var< SQInt32 > type(vm, 2);

    if (!inst.value.IsActive())
        SqThrow("Invalid player reference: %d", inst.value.GetID());
    else if (top == 3 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        CSStr msg = 0;

        if (SQ_FAILED(sq_getstring(vm, -1, &msg)))
            SqThrow("Unable to retrieve the announcement");
        else
            _Func->SendGameMessage(inst.value.GetID(), type.value, "%s", msg);

        sq_settop(vm, top);
    }
    else if (top > 3)
    {
        SStr msg = NULL;
        SQInteger len = 0;

        if (SQ_FAILED(sqstd_format(vm, 3, &len, &msg)))
            SqThrow("Unable to generate the player announcement [%s]", Error::Message(vm).c_str());
        else
            _Func->SendGameMessage(inst.value.GetID(), type.value, "%s", msg);
    }
    else
        SqThrow("Unable to extract the player message");

    return 0;
}

// ================================================================================================
void Register_CPlayer(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqPlayer"),
        Class< CPlayer, NoConstructor< CPlayer > >(vm, _SC("SqPlayer"))
        /* Metamethods */
        .Func(_SC("_cmp"), &CPlayer::Cmp)
        .Func(_SC("_tostring"), &CPlayer::ToString)
        /* Core Properties */
        .Prop(_SC("ID"), &CPlayer::GetID)
        .Prop(_SC("Tag"), &CPlayer::GetTag, &CPlayer::SetTag)
        .Prop(_SC("Data"), &CPlayer::GetData, &CPlayer::SetData)
        .Prop(_SC("MaxID"), &CPlayer::GetMaxID)
        .Prop(_SC("Active"), &CPlayer::IsActive)
        /* Core Functions */
        .Func(_SC("Bind"), &CPlayer::BindEvent)
        /* Properties */
        .Prop(_SC("Cls"), &CPlayer::GetClass)
        .Prop(_SC("ClassID"), &CPlayer::GetClass)
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
        .Prop(_SC("Armur"), &CPlayer::GetArmor, &CPlayer::SetArmor)
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
        /* Functions */
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
        /* Raw Functions */
        .SquirrelFunc(_SC("Msg"), &CPlayer::Msg)
        .SquirrelFunc(_SC("MsgP"), &CPlayer::MsgP)
        .SquirrelFunc(_SC("MsgEx"), &CPlayer::MsgEx)
        .SquirrelFunc(_SC("Message"), &CPlayer::Message)
        .SquirrelFunc(_SC("Announce"), &CPlayer::Announce)
        .SquirrelFunc(_SC("AnnounceEx"), &CPlayer::AnnounceEx)
        .SquirrelFunc(_SC("Text"), &CPlayer::Announce)
        .SquirrelFunc(_SC("TextEx"), &CPlayer::AnnounceEx)
        /* Overloads */
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
    );
}

} // Namespace:: SqMod
