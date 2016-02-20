#ifndef _ENTITY_PLAYER_HPP_
#define _ENTITY_PLAYER_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages Player instances.
*/
class CPlayer
{
    // --------------------------------------------------------------------------------------------
    friend class Core;

private:

    // --------------------------------------------------------------------------------------------
    static Color3       s_Color3;
    static Vector3      s_Vector3;

    // --------------------------------------------------------------------------------------------
    static SQChar       s_Buffer[SQMOD_PLAYER_TMP_BUFFER];

    /* --------------------------------------------------------------------------------------------
     * Cached identifiers for fast integer to string conversion.
    */
    static SQChar s_StrID[SQMOD_PLAYER_POOL][8];

    /* --------------------------------------------------------------------------------------------
     * Identifier of the managed entity.
    */
    Int32   m_ID;

    /* --------------------------------------------------------------------------------------------
     * User tag and data associated with this instance.
    */
    String  m_Tag;
    Object  m_Data;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    CPlayer(Int32 id);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CPlayer(const CPlayer &);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    CPlayer & operator = (const CPlayer &);

public:

    // --------------------------------------------------------------------------------------------
    static const Int32 Max;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CPlayer();

    /* --------------------------------------------------------------------------------------------
     * See whether this instance manages a valid entity.
    */
    bool Validate() const
    {
        if (VALID_ENTITY(m_ID))
            return true;
        SqThrow("Invalid player reference [%s]", m_Tag.c_str());
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const CPlayer & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier of the entity managed by this instance.
    */
    Int32 GetID() const { return m_ID; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the maximum possible identifier to an entity of this type.
    */
    Int32 GetMaxID() const { return SQMOD_PLAYER_POOL; }

    /* --------------------------------------------------------------------------------------------
     * Check whether this instance manages a valid entity.
    */
    bool IsActive() const { return VALID_ENTITY(m_ID); }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user tag.
    */
    CSStr GetTag() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    void SetTag(CSStr tag);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user data.
    */
    Object & GetData();

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(Object & data);

    // --------------------------------------------------------------------------------------------
    bool BindEvent(Int32 evid, Object & env, Function & func) const;

    // --------------------------------------------------------------------------------------------
    bool IsStreamedFor(CPlayer & player) const;
    Int32 GetClass() const;
    bool GetAdmin() const;
    void SetAdmin(bool toggle) const;
    CSStr GetIP() const;
    void Kick() const;
    void Ban() const;
    bool IsConnected() const;
    bool IsSpawned() const;
    Uint32 GetKey() const;
    Int32 GetWorld() const;
    void SetWorld(Int32 world) const;
    Int32 GetSecWorld() const;
    void SetSecWorld(Int32 world) const;
    Int32 GetUniqueWorld() const;
    bool IsWorldCompatible(Int32 world) const;
    CSStr GetName() const;
    void SetName(CSStr name) const;
    Int32 GetTeam() const;
    void SetTeam(Int32 team) const;
    Int32 GetSkin() const;
    void SetSkin(Int32 skin) const;
    const Color3 & GetColor() const;
    void SetColor(const Color3 & color) const;
    void SetColorEx(Uint8 r, Uint8 g, Uint8 b) const;
    void ForceSpawn() const;
    void ForceSelect() const;
    Int32 GetMoney() const;
    void SetMoney(Int32 amount) const;
    void GiveMoney(Int32 amount) const;
    Int32 GetScore() const;
    void SetScore(Int32 score) const;
    Int32 GetPing() const;
    Float32 GetFPS() const;
    bool IsTyping() const;
    CSStr GetUID() const;
    CSStr GetUID2() const;
    Float32 GetHealth() const;
    void SetHealth(Float32 amount) const;
    Float32 GetArmor() const;
    void SetArmor(Float32 amount) const;
    Int32 GetImmunity() const;
    void SetImmunity(Int32 flags) const;
    const Vector3 & GetPosition() const;
    void SetPosition(const Vector3 & pos) const;
    void SetPositionEx(Float32 x, Float32 y, Float32 z) const;
    const Vector3 & GetSpeed() const;
    void SetSpeed(const Vector3 & vel) const;
    void SetSpeedEx(Float32 x, Float32 y, Float32 z) const;
    void AddSpeed(const Vector3 & vel) const;
    void AddSpeedEx(Float32 x, Float32 y, Float32 z) const;
    Float32 GetHeading() const;
    void SetHeading(Float32 angle) const;
    Int32 GetAlpha() const;
    void SetAlpha(Int32 alpha, Int32 fade) const;
    Int32 GetVehicleStatus() const;
    Int32 GetOccupiedSlot() const;
    Object & GetVehicle() const;
    Int32 GetVehicleID() const;
    bool GetControllable() const;
    void SetControllable(bool toggle) const;
    bool GetDriveby() const;
    void SetDriveby(bool toggle) const;
    bool GetWhiteScanlines() const;
    void SetWhiteScanlines(bool toggle) const;
    bool GetGreenScanlines() const;
    void SetGreenScanlines(bool toggle) const;
    bool GetWidescreen() const;
    void SetWidescreen(bool toggle) const;
    bool GetShowMarkers() const;
    void SetShowMarkers(bool toggle) const;
    bool GetAttackPriv() const;
    void SetAttackPriv(bool toggle) const;
    bool GetHasMarker() const;
    void SetHasMarker(bool toggle) const;
    bool GetChatTags() const;
    void SetChatTags(bool toggle) const;
    bool GetDrunkEffects() const;
    void SetDrunkEffects(bool toggle) const;
    Int32 GetWeapon() const;
    void SetWeapon(Int32 wep, Int32 ammo) const;
    void GiveWeapon(Int32 wep, Int32 ammo) const;
    void StripWeapons() const;
    void SetCameraPosition(const Vector3 & pos, const Vector3 & aim) const;
    void SetCameraPosition(Float32 xp, Float32 yp, Float32 zp, Float32 xa, Float32 ya, Float32 za) const;
    void RestoreCamera() const;
    bool IsCameraLocked() const;
    void SetAnimation(Int32 group, Int32 anim) const;
    Int32 GetWantedLevel() const;
    void SetWantedLevel(Int32 level) const;
    Object & StandingOnVehicle() const;
    Object & StandingOnObject() const;
    bool IsAway() const;
    Object & GetSpectator() const;
    void SetSpectator(CPlayer & target) const;
    bool IsBurning() const;
    bool IsCrouched() const;
    Int32 GetState() const;
    Int32 GetAction() const;
    Int32 GetGameKeys() const;
    const Vector3 & GetAimPos() const;
    const Vector3 & GetAimDir() const;
    void Embark(CVehicle & vehicle) const;
    void Embark(CVehicle & vehicle, Int32 slot, bool allocate, bool warp) const;
    void Disembark() const;
    bool Redirect(CSStr ip, Uint32 port, CSStr nick, CSStr pass, CSStr user);
    Int32 GetAuthority() const;
    void SetAuthority(Int32 level) const;
    CSStr GetMessagePrefix(Uint32 index) const;
    void SetMessagePrefix(Uint32 index, CSStr prefix) const;
    Uint32 GetMessageColor() const;
    void SetMessageColor(Uint32 color) const;
    Int32 GetAnnounceStyle() const;
    void SetAnnounceStyle(Int32 style) const;

    // --------------------------------------------------------------------------------------------
    Float32 GetPosX() const;
    Float32 GetPosY() const;
    Float32 GetPosZ() const;
    void SetPosX(Float32 x) const;
    void SetPosY(Float32 y) const;
    void SetPosZ(Float32 z) const;

    // --------------------------------------------------------------------------------------------
    static SQInteger Msg(HSQUIRRELVM vm);
    static SQInteger MsgP(HSQUIRRELVM vm);
    static SQInteger MsgEx(HSQUIRRELVM vm);
    static SQInteger Message(HSQUIRRELVM vm);
    static SQInteger Announce(HSQUIRRELVM vm);
    static SQInteger AnnounceEx(HSQUIRRELVM vm);

};

} // Namespace:: SqMod

#endif // _ENTITY_PLAYER_HPP_