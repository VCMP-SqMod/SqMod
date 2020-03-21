#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"
#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Circular locks employed by the player manager.
*/
enum PlayerCircularLocks
{
    PLAYERCL_EMIT_PLAYER_OPTION          = (1 << 0),
    PLAYERCL_EMIT_PLAYER_ADMIN           = (1 << 1),
    PLAYERCL_EMIT_PLAYER_WORLD           = (1 << 2),
    PLAYERCL_EMIT_PLAYER_TEAM            = (1 << 3),
    PLAYERCL_EMIT_PLAYER_SKIN            = (1 << 4),
    PLAYERCL_EMIT_PLAYER_MONEY           = (1 << 5),
    PLAYERCL_EMIT_PLAYER_SCORE           = (1 << 6),
    PLAYERCL_EMIT_PLAYER_WANTED_LEVEL    = (1 << 7),
    PLAYERCL_EMIT_PLAYER_IMMUNITY        = (1 << 8),
    PLAYERCL_EMIT_PLAYER_ALPHA           = (1 << 9)
};

/* ------------------------------------------------------------------------------------------------
 * Manages a single player entity.
*/
class CPlayer
{
    // --------------------------------------------------------------------------------------------
    friend class Core;

private:

    // --------------------------------------------------------------------------------------------
    static SQChar       s_Buffer[SQMOD_PLAYER_TMP_BUFFER];

    /* --------------------------------------------------------------------------------------------
     * Identifier of the managed entity.
    */
    Int32       m_ID;

    /* --------------------------------------------------------------------------------------------
     * User tag associated with this instance.
    */
    String      m_Tag;

    /* --------------------------------------------------------------------------------------------
     * User data associated with this instance.
    */
    LightObj    m_Data;

    /* --------------------------------------------------------------------------------------------
     * Buffer used to generate client data.
    */
    Buffer      m_Buffer;

    /* --------------------------------------------------------------------------------------------
     * Prevent events from triggering themselves.
    */
    Uint32      m_CircularLocks;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    CPlayer(Int32 id);

public:

    // --------------------------------------------------------------------------------------------
    typedef String MsgPrefix[SQMOD_PLAYER_MSG_PREFIXES];

    /* --------------------------------------------------------------------------------------------
     * Maximum possible number that could represent an identifier for this entity type.
    */
    static const Int32 Max;

    /* --------------------------------------------------------------------------------------------
     * The initial size of the allocated memory buffer when starting a new stream.
    */
    Uint32          mBufferInitSize;

    /* --------------------------------------------------------------------------------------------
     * Default color to use in client messages.
    */
    Uint32          mMessageColor;

    /* --------------------------------------------------------------------------------------------
     * Default style to use in client announcements.
    */
    Int32           mAnnounceStyle;

    /* --------------------------------------------------------------------------------------------
     * Default ammo to give to the managed player when not specifying any.
    */
    Int32           mDefaultAmmo;

    /* --------------------------------------------------------------------------------------------
     * Set of strings to add before a client message automatically.
    */
    String          mMessagePrefix;

    /* --------------------------------------------------------------------------------------------
     * Set of strings to add before a client message automatically.
    */
    String          mMessagePostfix;

    /* --------------------------------------------------------------------------------------------
     * Set of strings to add before a client announcement automatically.
    */
    String          mAnnouncePrefix;

    /* --------------------------------------------------------------------------------------------
     * Set of strings to add before a client announcement automatically.
    */
    String          mAnnouncePostfix;

    /* --------------------------------------------------------------------------------------------
     * Set of strings to add before a client message automatically.
    */
    MsgPrefix       mMessagePrefixes;

    /* --------------------------------------------------------------------------------------------
     * Don't include the auto message prefix/postfix in already prefixed messages.
    */
    bool            mLimitPrefixPostfixMessage;

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
        {
            STHROWF("Invalid player reference [%s]", m_Tag.c_str());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    const String & ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated null entity instance.
    */
    static SQInteger SqGetNull(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated null entity instance.
    */
    static LightObj & GetNull();

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
    void SetTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    CPlayer & ApplyTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user data.
    */
    LightObj & GetData();

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(LightObj & data);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the events table of this entity.
    */
    LightObj & GetEvents() const;

    /* --------------------------------------------------------------------------------------------
     * Emit a custom event for the managed entity
    */
    void CustomEvent(Int32 header, LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is connected.
    */
    bool IsConnected() const;

    /* --------------------------------------------------------------------------------------------
     * See if the managed player entity is streamed for the specified player.
    */
    bool IsStreamedFor(CPlayer & player) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity has administrator privileges.
    */
    bool GetAdmin() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed player entity has administrator privileges.
    */
    void SetAdmin(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the ip address of the managed player entity.
    */
    CSStr GetIP() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unique user identifier of the managed player entity.
    */
    CSStr GetUID() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unique user identifier version 2 of the managed player entity.
    */
    CSStr GetUID2() const;
#if SQMOD_SDK_LEAST(2, 1)
    /* --------------------------------------------------------------------------------------------
     * Set player's health to 0 and reset the death reason.
    */
    void Kill() const;
#endif
    /* --------------------------------------------------------------------------------------------
     * Kick the managed player entity from the server.
    */
    void Kick() const;

    /* --------------------------------------------------------------------------------------------
     * Kick the managed player entity from the server.
    */
    void KickBecause(Int32 header, LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * Ban the managed player entity from the server.
    */
    void Ban() const;

    /* --------------------------------------------------------------------------------------------
     * Ban the managed player entity from the server.
    */
    void BanBecause(Int32 header, LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the key of the managed player entity.
    */
    Uint32 GetKey() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the nick name of the managed player entity.
    */
    CSStr GetName() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the nick name of the managed player entity.
    */
    void SetName(StackStrF & name) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current state of the managed player entity.
    */
    Int32 GetState() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current option value of the managed player entity.
    */
    Int32 GetOption(Int32 option_id) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the current option value of the managed player entity.
    */
    void SetOption(Int32 option_id, bool toggle);

    /* --------------------------------------------------------------------------------------------
     * Modify the current option value of the managed player entity.
    */
    void SetOptionEx(Int32 option_id, bool toggle, Int32 header, LightObj & payload);
#if SQMOD_SDK_LEAST(2, 1)
    /* --------------------------------------------------------------------------------------------
     * Retrieve network statistics related to the managed player entity.
    */
    SQFloat GetNetworkStatisticsF(Int32 option_id) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve network statistics related to the managed player entity.
    */
    SQInteger GetNetworkStatisticsI(Int32 option_id) const;
#endif
    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the managed player entity exists.
    */
    Int32 GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the world in which the managed player entity exists.
    */
    void SetWorld(Int32 world);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the secondary world of the managed player entity.
    */
    Int32 GetSecondaryWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the secondary world of the managed player entity.
    */
    void SetSecondaryWorld(Int32 world);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unique world of the managed player entity.
    */
    Int32 GetUniqueWorld() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is compatible with the specified world.
    */
    bool IsWorldCompatible(Int32 world) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the class of the managed player entity.
    */
    Int32 GetClass() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the team of the managed player entity.
    */
    Int32 GetTeam() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the team of the managed player entity.
    */
    void SetTeam(Int32 team);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the skin identifier of the managed player entity.
    */
    Int32 GetSkin() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the skin identifier of the managed player entity.
    */
    void SetSkin(Int32 skin);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the color of the managed player entity.
    */
    Color3 GetColor() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the color of the managed player entity.
    */
    void SetColor(const Color3 & color) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the color of the managed player entity.
    */
    void SetColorEx(Uint8 r, Uint8 g, Uint8 b) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is spawned.
    */
    bool IsSpawned() const;

    /* --------------------------------------------------------------------------------------------
     * Force the managed player entity to spawn in the game.
    */
    void ForceSpawn() const;

    /* --------------------------------------------------------------------------------------------
     * Force the managed player entity to select a class.
    */
    void ForceSelect() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is typing.
    */
    bool IsTyping() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the money amount of the managed player entity.
    */
    Int32 GetMoney() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the money amount of the managed player entity.
    */
    void SetMoney(Int32 amount);

    /* --------------------------------------------------------------------------------------------
     * Give a certain amount of money to the managed player entity.
    */
    void GiveMoney(Int32 amount);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the score of the managed player entity.
    */
    Int32 GetScore() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the score of the managed player entity.
    */
    void SetScore(Int32 score);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the wanted level of the managed player entity.
    */
    Int32 GetWantedLevel() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the wanted level of the managed player entity.
    */
    void SetWantedLevel(Int32 level);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection latency of the managed player entity.
    */
    Int32 GetPing() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the frames per second of the managed player entity.
    */
    Float32 GetFPS() const;

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
    void SetImmunity(Int32 flags);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the managed player entity.
    */
    Vector3 GetPosition() const;

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
    Vector3 GetSpeed() const;

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
    void SetAlpha(Int32 alpha);

    /* --------------------------------------------------------------------------------------------
     * Modify the alpha of the managed player entity.
    */
    void SetAlphaEx(Int32 alpha, Int32 fade);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the aim position of the managed player entity.
    */
    Vector3 GetAimPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the aim direction of the managed player entity.
    */
    Vector3 GetAimDirection() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is burning.
    */
    bool IsBurning() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is crouched.
    */
    bool IsCrouched() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current action of the managed player entity.
    */
    Int32 GetAction() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the game keys of the managed player entity.
    */
    Int32 GetGameKeys() const;

    /* --------------------------------------------------------------------------------------------
     * Embark the managed player entity into the specified vehicle entity.
    */
    bool Embark(CVehicle & vehicle) const;

    /* --------------------------------------------------------------------------------------------
     * Embark the managed player entity into the specified vehicle entity.
    */
    bool Embark(CVehicle & vehicle, Int32 slot, bool allocate, bool warp) const;

    /* --------------------------------------------------------------------------------------------
     * Disembark the managed player entity from the currently embarked vehicle entity.
    */
    void Disembark() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle status of the managed player entity.
    */
    Int32 GetVehicleStatus() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the occupied vehicle slot by the managed player entity.
    */
    Int32 GetVehicleSlot() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle in which the managed player entity is embarked.
    */
    LightObj & GetVehicle() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle identifier in which the managed player entity is embarked.
    */
    Int32 GetVehicleID() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the weapon identifier of the managed player entity.
    */
    Int32 GetWeapon() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the weapon of the managed player entity.
    */
    void SetWeapon(Int32 wep) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the weapon of the managed player entity.
    */
    void SetWeaponEx(Int32 wep, Int32 ammo) const;

    /* --------------------------------------------------------------------------------------------
     * Give a weapon of the managed player entity.
    */
    void GiveWeapon(Int32 wep, Int32 ammo) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the active weapon ammo of the managed player entity.
    */
    Int32 GetAmmo() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the weapon slot of the managed player entity.
    */
    Int32 GetWeaponSlot() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the weapon slot of the managed player entity.
    */
    void SetWeaponSlot(Int32 slot) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the weapon identifier at the specified slot for the managed player entity.
    */
    Int32 GetWeaponAtSlot(Int32 slot) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the ammo from the weapon at the specified slot for the managed player entity.
    */
    Int32 GetAmmoAtSlot(Int32 slot) const;

    /* --------------------------------------------------------------------------------------------
     * Remove a certain weapon from the managed player entity.
    */
    void RemoveWeapon(Int32 wep) const;

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
    void SetAnimation(Int32 anim) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the animation of the managed player entity.
    */
    void SetAnimation(Int32 anim, Int32 group) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle that the managed player entity is standing on.
    */
    LightObj & StandingOnVehicle() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the object that the managed player entity is standing on.
    */
    LightObj & StandingOnObject() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is away.
    */
    bool IsAway() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the player that the managed player entity is spectating.
    */
    LightObj & GetSpectator() const;

    /* --------------------------------------------------------------------------------------------
     * Set the managed player entity to spectate the specified player entity.
    */
    void SetSpectator(CPlayer & target) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier of the player that the managed player entity is spectating.
    */
    SQInteger GetSpectatorID() const;

    /* --------------------------------------------------------------------------------------------
     * Set the managed player entity to spectate the specified player entity.
    */
    void SetSpectatorID(SQInteger id) const;

    /* --------------------------------------------------------------------------------------------
     * Set the managed player entity to spectate no one.
    */
    void Unspectate() const;
#if SQMOD_SDK_LEAST(2, 1)
    /* --------------------------------------------------------------------------------------------
     * Set whether the target player will see an objective arrow over a player.
    */
    void SetPlayer3DArrow(CPlayer & target, bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the target player sees an objective arrow over a player.
    */
    bool GetPlayer3DArrow(CPlayer & target) const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the target player will see an objective arrow over a player.
    */
    void SetPlayer3DArrowID(SQInteger id, bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the target player sees an objective arrow over a player.
    */
    bool GetPlayer3DArrowID(SQInteger id) const;

    /* --------------------------------------------------------------------------------------------
     * Smoothly pivots the camera angle.
    */
    bool InterpolateCameraLookAt(const Vector3 & pos, Uint32 ms) const;

    /* --------------------------------------------------------------------------------------------
     * Smoothly pivots the camera angle.
    */
    bool InterpolateCameraLookAtEx(Float32 x, Float32 y, Float32 z, Uint32 ms) const;
#endif
    /* --------------------------------------------------------------------------------------------
     * Redirect the managed player entity to the specified server.
    */
    void Redirect(StackStrF & ip, Uint32 port, StackStrF & nick,
                    StackStrF & server_pass, StackStrF & user_pass);

    /* --------------------------------------------------------------------------------------------
     * Request a list of the modules loaded into the client session.
    */
    void GetModuleList() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the authority level of the managed player entity.
    */
    void PlaySound(Int32 sound_id) const;
#if SQMOD_SDK_LEAST(2, 1)
    /* --------------------------------------------------------------------------------------------
     * Set how delayed a player's turn handling is when in a vehicle.
    */
    void SetDrunkHandling(SQInteger level) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve how delayed a player's turn handling is when in a vehicle.
    */
    SQInteger GetDrunkHandling() const;

    /* --------------------------------------------------------------------------------------------
     * Set how intense the drunk blur overlay is for a player.
    */
    void SetDrunkVisuals(SQInteger level) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve how intense the drunk blur overlay is for a player.
    */
    SQInteger GetDrunkVisuals() const;
#endif
    /* --------------------------------------------------------------------------------------------
     * Create a checkpoint or sphere for this player.
    */
    LightObj & CreateCheckpointEx(Int32 world, bool sphere, Float32 x, Float32 y, Float32 z,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a, Float32 radius) const;

    /* --------------------------------------------------------------------------------------------
     * Create a checkpoint or sphere for this player.
    */
    LightObj & CreateCheckpointEx(Int32 world, bool sphere, Float32 x, Float32 y, Float32 z,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a, Float32 radius,
                            Int32 header, LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * Create a checkpoint or sphere for this player.
    */
    LightObj & CreateCheckpoint(Int32 world, bool sphere, const Vector3 & pos,
                        const Color4 & color, Float32 radius) const;

    /* --------------------------------------------------------------------------------------------
     * Create a checkpoint or sphere for this player.
    */
    LightObj & CreateCheckpoint(Int32 world, bool sphere, const Vector3 & pos, const Color4 & color,
                                    Float32 radius, Int32 header, LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity collides with user defined areas.
    */
    bool GetCollideAreas() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed player entity can collide with user defined areas.
    */
    void SetCollideAreas(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed player entity can collide with user defined areas (with last test).
    */
    void SetAreasCollide(bool toggle) const;

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
    const String & GetMessagePrefix(Uint32 index) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the message prefix at the specified index for the managed player entity.
    */
    void SetMessagePrefix(Uint32 index, StackStrF & prefix);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of tracked position changes for the managed player entity.
    */
    SQInteger GetTrackPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the amount of tracked position changes for the managed player entity.
    */
    void SetTrackPosition(SQInteger num) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the amount of tracked position changes for the managed player entity.
    */
    void SetTrackPositionEx(SQInteger num, Int32 header, const LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of tracked heading changes for the managed player entity.
    */
    SQInteger GetTrackHeading() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the amount of tracked heading changes for the managed player entity.
    */
    void SetTrackHeading(SQInteger num) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known weapon for the managed player entity.
    */
    Int32 GetLastWeapon() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known health for the managed player entity.
    */
    Float32 GetLastHealth() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known armour for the managed player entity.
    */
    Float32 GetLastArmour() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known heading for the managed player entity.
    */
    Float32 GetLastHeading() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known position for the managed player entity.
    */
    const Vector3 & GetLastPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Start a new stream with the default size.
    */
    void StartStream();

    /* --------------------------------------------------------------------------------------------
     * Start a new stream with a custom size.
    */
    void StartStream(Uint32 size);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current cursor position of the stream buffer.
    */
    Int32 GetBufferCursor() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current cursor position of the stream buffer.
    */
    void SetBufferCursor(Int32 pos);

    /* --------------------------------------------------------------------------------------------
     * Write a 8bit byte to the stream buffer.
    */
    void StreamByte(SQInteger val);

    /* --------------------------------------------------------------------------------------------
     * Write a 16bit short to the stream buffer.
    */
    void StreamShort(SQInteger val);

    /* --------------------------------------------------------------------------------------------
     * Write a 32bit integer to the stream buffer.
    */
    void StreamInt(SQInteger val);

    /* --------------------------------------------------------------------------------------------
     * Write a 32bit float to the stream buffer.
    */
    void StreamFloat(SQFloat val);

    /* --------------------------------------------------------------------------------------------
     * Write a string to the stream buffer.
    */
    void StreamString(StackStrF & val);

    /* --------------------------------------------------------------------------------------------
     * Write a raw string to the stream buffer.
    */
    void StreamRawString(StackStrF & val);

    /* --------------------------------------------------------------------------------------------
     * Send the data in the stream buffer to the client.
    */
    void FlushStream(bool reset);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the total capacity of the buffer stream in bytes.
    */
    Uint32 GetBufferCapacity() const;

    /* --------------------------------------------------------------------------------------------
     * Send the specified buffer contents to the managed player entity.
    */
    void SendBuffer(const SqBuffer & buffer) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the x axis of the managed player entity.
    */
    Float32 GetPositionX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the y axis of the managed player entity.
    */
    Float32 GetPositionY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the z axis of the managed player entity.
    */
    Float32 GetPositionZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed player entity.
    */
    void SetPositionX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed player entity.
    */
    void SetPositionY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed player entity.
    */
    void SetPositionZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the red color of the managed player entity.
    */
    Int32 GetColorR() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the green color of the managed player entity.
    */
    Int32 GetColorG() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the blue color of the managed player entity.
    */
    Int32 GetColorB() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the red color of the managed player entity.
    */
    void SetColorR(Int32 r) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the green color of the managed player entity.
    */
    void SetColorG(Int32 g) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the blue color of the managed player entity.
    */
    void SetColorB(Int32 b) const;

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
