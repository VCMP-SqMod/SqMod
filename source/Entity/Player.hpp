#ifndef _ENTITY_PLAYER_HPP_
#define _ENTITY_PLAYER_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages a single player entity.
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
     * Identifier of the managed entity.
    */
    Int32   m_ID;

    /* --------------------------------------------------------------------------------------------
     * User tag associated with this instance.
    */
    String  m_Tag;

    /* --------------------------------------------------------------------------------------------
     * User data associated with this instance.
    */
    Object  m_Data;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    CPlayer(Int32 id);

public:

    /* --------------------------------------------------------------------------------------------
     * Maximum possible number that could represent an identifier for this entity type.
    */
    static const Int32 Max;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CPlayer(const CPlayer &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    CPlayer(CPlayer &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CPlayer();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    CPlayer & operator = (const CPlayer &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    CPlayer & operator = (CPlayer &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * See whether this instance manages a valid entity.
    */
    void Validate() const
    {
        if (INVALID_ENTITY(m_ID))
            SqThrowF("Invalid player reference [%s]", m_Tag.c_str());
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const CPlayer & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    const String & ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier of the entity managed by this instance.
    */
    Int32 GetID() const
    {
        return m_ID;
    }

    /* --------------------------------------------------------------------------------------------
     * Check whether this instance manages a valid entity.
    */
    bool IsActive() const
    {
        return VALID_ENTITY(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user tag.
    */
    const String & GetTag() const;

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

    /* --------------------------------------------------------------------------------------------
     * Bind to an event supported by this entity type.
    */
    void BindEvent(Int32 evid, Object & env, Function & func) const;

    /* --------------------------------------------------------------------------------------------
     * See if the managed player entity is streamed for the specified player.
    */
    bool IsStreamedFor(CPlayer & player) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the class of the managed player entity.
    */
    Int32 GetClass() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity has administrator privileges.
    */
    bool GetAdmin() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed player entity has administrator privileges.
    */
    void SetAdmin(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the ip address of the managed player entity.
    */
    CSStr GetIP() const;

    /* --------------------------------------------------------------------------------------------
     * Kick the managed player entity from the server.
    */
    void Kick() const;

    /* --------------------------------------------------------------------------------------------
     * Ban the managed player entity from the server.
    */
    void Ban() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is connected.
    */
    bool IsConnected() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is spawned.
    */
    bool IsSpawned() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the key of the managed player entity.
    */
    Uint32 GetKey() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the managed player entity exists.
    */
    Int32 GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the world in which the managed player entity exists.
    */
    void SetWorld(Int32 world) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the secondary world of the managed player entity.
    */
    Int32 GetSecWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the secondary world of the managed player entity.
    */
    void SetSecWorld(Int32 world) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unique world of the managed player entity.
    */
    Int32 GetUniqueWorld() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is compatible with the specified world.
    */
    bool IsWorldCompatible(Int32 world) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the nick name of the managed player entity.
    */
    CSStr GetName() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the nick name of the managed player entity.
    */
    void SetName(CSStr name) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the team of the managed player entity.
    */
    Int32 GetTeam() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the team of the managed player entity.
    */
    void SetTeam(Int32 team) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the skin identifier of the managed player entity.
    */
    Int32 GetSkin() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the skin identifier of the managed player entity.
    */
    void SetSkin(Int32 skin) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the color of the managed player entity.
    */
    const Color3 & GetColor() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the color of the managed player entity.
    */
    void SetColor(const Color3 & color) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the color of the managed player entity.
    */
    void SetColorEx(Uint8 r, Uint8 g, Uint8 b) const;

    /* --------------------------------------------------------------------------------------------
     * Force the managed player entity to spawn in the game.
    */
    void ForceSpawn() const;

    /* --------------------------------------------------------------------------------------------
     * Force the managed player entity to select a class.
    */
    void ForceSelect() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the money amount of the managed player entity.
    */
    Int32 GetMoney() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the money amount of the managed player entity.
    */
    void SetMoney(Int32 amount) const;

    /* --------------------------------------------------------------------------------------------
     * Give a certain amount of money to the managed player entity.
    */
    void GiveMoney(Int32 amount) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the score of the managed player entity.
    */
    Int32 GetScore() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the score of the managed player entity.
    */
    void SetScore(Int32 score) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection latency of the managed player entity.
    */
    Int32 GetPing() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the frames per second of the managed player entity.
    */
    Float32 GetFPS() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is typing.
    */
    bool IsTyping() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unique user identifier of the managed player entity.
    */
    CSStr GetUID() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unique user identifier version 2 of the managed player entity.
    */
    CSStr GetUID2() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current health of the managed player entity.
    */
    Float32 GetHealth() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the health of the managed player entity.
    */
    void SetHealth(Float32 amount) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current health of the managed player entity.
    */
    Float32 GetArmor() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the health of the managed player entity.
    */
    void SetArmor(Float32 amount) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the immunity flags of the managed player entity.
    */
    Int32 GetImmunity() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the immunity flags of the managed player entity.
    */
    void SetImmunity(Int32 flags) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the managed player entity.
    */
    const Vector3 & GetPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed player entity.
    */
    void SetPosition(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed player entity.
    */
    void SetPositionEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the speed of the managed player entity.
    */
    const Vector3 & GetSpeed() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the speed of the managed player entity.
    */
    void SetSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the speed of the managed player entity.
    */
    void SetSpeedEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the speed of the managed player entity.
    */
    void AddSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the speed of the managed player entity.
    */
    void AddSpeedEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the heading angle of the managed player entity.
    */
    Float32 GetHeading() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the heading angle of the managed player entity.
    */
    void SetHeading(Float32 angle) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the alpha of the managed player entity.
    */
    Int32 GetAlpha() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the alpha of the managed player entity.
    */
    void SetAlpha(Int32 alpha, Int32 fade) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle status of the managed player entity.
    */
    Int32 GetVehicleStatus() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the occupied vehicle slot by the managed player entity.
    */
    Int32 GetOccupiedSlot() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle in which the managed player entity is embarked.
    */
    Object & GetVehicle() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle identifier in which the managed player entity is embarked.
    */
    Int32 GetVehicleID() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity can be controlled.
    */
    bool GetControllable() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed player entity can be controlled.
    */
    void SetControllable(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity can driveby.
    */
    bool GetDriveby() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed player entity can driveby.
    */
    void SetDriveby(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity has white scanlines.
    */
    bool GetWhiteScanlines() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed player entity has white scanlines.
    */
    void SetWhiteScanlines(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity has green scanlines.
    */
    bool GetGreenScanlines() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed player entity has green scanlines.
    */
    void SetGreenScanlines(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity has widescreen.
    */
    bool GetWidescreen() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed player entity has widescreen.
    */
    void SetWidescreen(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity displays markers.
    */
    bool GetShowMarkers() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed player entity displays markers.
    */
    void SetShowMarkers(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity has attacking privileges.
    */
    bool GetAttackPriv() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed player entity has attacking privileges.
    */
    void SetAttackPriv(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity has markers.
    */
    bool GetHasMarker() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed player entity has markers.
    */
    void SetHasMarker(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity has chat tags.
    */
    bool GetChatTags() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed player entity has chat tags.
    */
    void SetChatTags(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is under drunk effects.
    */
    bool GetDrunkEffects() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed player entity is under drunk effects.
    */
    void SetDrunkEffects(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the weapon identifier of the managed player entity.
    */
    Int32 GetWeapon() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the weapon of the managed player entity.
    */
    void SetWeapon(Int32 wep, Int32 ammo) const;

    /* --------------------------------------------------------------------------------------------
     * Give a weapon of the managed player entity.
    */
    void GiveWeapon(Int32 wep, Int32 ammo) const;

    /* --------------------------------------------------------------------------------------------
     * Strip the managed player entity of all weapons.
    */
    void StripWeapons() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the camera position of the managed player entity.
    */
    void SetCameraPosition(const Vector3 & pos, const Vector3 & aim) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the camera position of the managed player entity.
    */
    void SetCameraPosition(Float32 xp, Float32 yp, Float32 zp, Float32 xa, Float32 ya, Float32 za) const;

    /* --------------------------------------------------------------------------------------------
     * Restore the camera position of the managed player entity.
    */
    void RestoreCamera() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity has camera locked.
    */
    bool IsCameraLocked() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the animation of the managed player entity.
    */
    void SetAnimation(Int32 group, Int32 anim) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the wanted level of the managed player entity.
    */
    Int32 GetWantedLevel() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the wanted level of the managed player entity.
    */
    void SetWantedLevel(Int32 level) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle that the managed player entity is standing on.
    */
    Object & StandingOnVehicle() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the object that the managed player entity is standing on.
    */
    Object & StandingOnObject() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is away.
    */
    bool IsAway() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the player that the managed player entity is spectating.
    */
    Object & GetSpectator() const;

    /* --------------------------------------------------------------------------------------------
     * Set the managed player entity to spectate the specified player entity.
    */
    void SetSpectator(CPlayer & target) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is burning.
    */
    bool IsBurning() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is crouched.
    */
    bool IsCrouched() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current state of the managed player entity.
    */
    Int32 GetState() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current action of the managed player entity.
    */
    Int32 GetAction() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the game keys of the managed player entity.
    */
    Int32 GetGameKeys() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the aim position of the managed player entity.
    */
    const Vector3 & GetAimPos() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the aim direction of the managed player entity.
    */
    const Vector3 & GetAimDir() const;

    /* --------------------------------------------------------------------------------------------
     * Embark the managed player entity into the specified vehicle entity.
    */
    void Embark(CVehicle & vehicle) const;

    /* --------------------------------------------------------------------------------------------
     * Embark the managed player entity into the specified vehicle entity.
    */
    void Embark(CVehicle & vehicle, Int32 slot, bool allocate, bool warp) const;

    /* --------------------------------------------------------------------------------------------
     * Disembark the managed player entity from the currently embarked vehicle entity.
    */
    void Disembark() const;

    /* --------------------------------------------------------------------------------------------
     * Redirect the managed player entity to the specified server.
    */
    bool Redirect(CSStr ip, Uint32 port, CSStr nick, CSStr pass, CSStr user);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the authority level of the managed player entity.
    */
    Int32 GetAuthority() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the authority level of the managed player entity.
    */
    void SetAuthority(Int32 level) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message prefix at the specified index for the managed player entity.
    */
    CSStr GetMessagePrefix(Uint32 index) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the message prefix at the specified index for the managed player entity.
    */
    void SetMessagePrefix(Uint32 index, CSStr prefix) const;
    /* --------------------------------------------------------------------------------------------
     * Retrieve the message color for the managed player entity.
    */
    Uint32 GetMessageColor() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the message color for the managed player entity.
    */
    void SetMessageColor(Uint32 color) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the announcement style for the managed player entity.
    */
    Int32 GetAnnounceStyle() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the announcement style for the managed player entity.
    */
    void SetAnnounceStyle(Int32 style) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the x axis of the managed player entity.
    */
    Float32 GetPosX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the y axis of the managed player entity.
    */
    Float32 GetPosY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the z axis of the managed player entity.
    */
    Float32 GetPosZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed player entity.
    */
    void SetPosX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed player entity.
    */
    void SetPosY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed player entity.
    */
    void SetPosZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Send a formatted colored message to the managed player entity.
    */
    static SQInteger Msg(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Send a formatted message with a prefix to the managed player entity.
    */
    static SQInteger MsgP(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Send a formatted colored message to the managed player entity.
    */
    static SQInteger MsgEx(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Send a formatted message to the managed player entity.
    */
    static SQInteger Message(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Send a formatted announcement message to the managed player entity.
    */
    static SQInteger Announce(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Send a formatted announcement message to the managed player entity.
    */
    static SQInteger AnnounceEx(HSQUIRRELVM vm);
};

} // Namespace:: SqMod

#endif // _ENTITY_PLAYER_HPP_