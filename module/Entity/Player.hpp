#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"
#include "Core/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Circular locks employed by the player manager.
*/
enum PlayerCircularLocks
{
    PLAYERCL_EMIT_PLAYER_OPTION          = (1u << 0u),
    PLAYERCL_EMIT_PLAYER_ADMIN           = (1u << 1u),
    PLAYERCL_EMIT_PLAYER_WORLD           = (1u << 2u),
    PLAYERCL_EMIT_PLAYER_TEAM            = (1u << 3u),
    PLAYERCL_EMIT_PLAYER_SKIN            = (1u << 4u),
    PLAYERCL_EMIT_PLAYER_MONEY           = (1u << 5u),
    PLAYERCL_EMIT_PLAYER_SCORE           = (1u << 6u),
    PLAYERCL_EMIT_PLAYER_WANTED_LEVEL    = (1u << 7u),
    PLAYERCL_EMIT_PLAYER_IMMUNITY        = (1u << 8u),
    PLAYERCL_EMIT_PLAYER_ALPHA           = (1u << 9u),
    PLAYERCL_EMIT_PLAYER_EMBARK          = (1u << 10u)
};

/* ------------------------------------------------------------------------------------------------
 * Manages a single player entity.
*/
class CPlayer
{
    // --------------------------------------------------------------------------------------------
    friend class Core;
    friend class PlayerInst;

private:

    // --------------------------------------------------------------------------------------------
    static SQChar       s_Buffer[SQMOD_PLAYER_TMP_BUFFER];

    /* --------------------------------------------------------------------------------------------
     * Identifier of the managed entity.
    */
    int32_t       m_ID;

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
    uint32_t      m_CircularLocks;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit CPlayer(int32_t id);

public:

    // --------------------------------------------------------------------------------------------
    typedef String MsgPrefix[SQMOD_PLAYER_MSG_PREFIXES];

    /* --------------------------------------------------------------------------------------------
     * Maximum possible number that could represent an identifier for this entity type.
    */
    static const int32_t Max;

    /* --------------------------------------------------------------------------------------------
     * The initial size of the allocated memory buffer when starting a new stream.
    */
    uint32_t          mBufferInitSize;

    /* --------------------------------------------------------------------------------------------
     * Default color to use in client messages.
    */
    uint32_t          mMessageColor;

    /* --------------------------------------------------------------------------------------------
     * Default style to use in client announcements.
    */
    int32_t           mAnnounceStyle;

    /* --------------------------------------------------------------------------------------------
     * Default ammo to give to the managed player when not specifying any.
    */
    int32_t           mDefaultAmmo;

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
            STHROWF("Invalid player reference [{}]", m_Tag);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    SQMOD_NODISCARD const String & ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated null entity instance.
    */
    static SQInteger SqGetNull(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated null entity instance.
    */
    SQMOD_NODISCARD static LightObj & GetNull();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier of the entity managed by this instance.
    */
    SQMOD_NODISCARD int32_t GetID() const
    {
        return m_ID;
    }

    /* --------------------------------------------------------------------------------------------
     * Check whether this instance manages a valid entity.
    */
    SQMOD_NODISCARD bool IsActive() const
    {
        return VALID_ENTITY(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user tag.
    */
    SQMOD_NODISCARD const String & GetTag() const;

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
    SQMOD_NODISCARD LightObj & GetData();

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(LightObj & data);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the events table of this entity.
    */
    SQMOD_NODISCARD LightObj & GetEvents() const;

    /* --------------------------------------------------------------------------------------------
     * Emit a custom event for the managed entity
    */
    void CustomEvent(int32_t header, LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is connected.
    */
    SQMOD_NODISCARD bool IsConnected() const;

    /* --------------------------------------------------------------------------------------------
     * See if the managed player entity is streamed for the specified player.
    */
    bool IsStreamedFor(CPlayer & player) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity has administrator privileges.
    */
    SQMOD_NODISCARD bool GetAdmin() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed player entity has administrator privileges.
    */
    void SetAdmin(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the ip address of the managed player entity.
    */
    SQMOD_NODISCARD const SQChar * GetIP() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unique user identifier of the managed player entity.
    */
    SQMOD_NODISCARD const SQChar * GetUID() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unique user identifier version 2 of the managed player entity.
    */
    SQMOD_NODISCARD const SQChar * GetUID2() const;
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
    void KickBecause(int32_t header, LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * Ban the managed player entity from the server.
    */
    void Ban() const;

    /* --------------------------------------------------------------------------------------------
     * Ban the managed player entity from the server.
    */
    void BanBecause(int32_t header, LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the key of the managed player entity.
    */
    SQMOD_NODISCARD uint32_t GetKey() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the nick name of the managed player entity.
    */
    SQMOD_NODISCARD const SQChar * GetName() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the nick name of the managed player entity.
    */
    void SetName(StackStrF & name) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current state of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetState() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current option value of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetOption(int32_t option_id) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the current option value of the managed player entity.
    */
    void SetOption(int32_t option_id, bool toggle);

    /* --------------------------------------------------------------------------------------------
     * Modify the current option value of the managed player entity.
    */
    void SetOptionEx(int32_t option_id, bool toggle, int32_t header, LightObj & payload);
#if SQMOD_SDK_LEAST(2, 1)
    /* --------------------------------------------------------------------------------------------
     * Retrieve network statistics related to the managed player entity.
    */
    SQMOD_NODISCARD SQFloat GetNetworkStatisticsF(int32_t option_id) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve network statistics related to the managed player entity.
    */
    SQMOD_NODISCARD SQInteger GetNetworkStatisticsI(int32_t option_id) const;
#endif
    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the managed player entity exists.
    */
    SQMOD_NODISCARD int32_t GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the world in which the managed player entity exists.
    */
    void SetWorld(int32_t world);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the secondary world of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetSecondaryWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the secondary world of the managed player entity.
    */
    void SetSecondaryWorld(int32_t world);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unique world of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetUniqueWorld() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is compatible with the specified world.
    */
    SQMOD_NODISCARD bool IsWorldCompatible(int32_t world) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the class of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetClass() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the team of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetTeam() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the team of the managed player entity.
    */
    void SetTeam(int32_t team);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the skin identifier of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetSkin() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the skin identifier of the managed player entity.
    */
    void SetSkin(int32_t skin);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the color of the managed player entity.
    */
    SQMOD_NODISCARD Color3 GetColor() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the color of the managed player entity.
    */
    void SetColor(const Color3 & color) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the color of the managed player entity.
    */
    void SetColorEx(uint8_t r, uint8_t g, uint8_t b) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is spawned.
    */
    SQMOD_NODISCARD bool IsSpawned() const;

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
    SQMOD_NODISCARD bool IsTyping() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the money amount of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetMoney() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the money amount of the managed player entity.
    */
    void SetMoney(int32_t amount);

    /* --------------------------------------------------------------------------------------------
     * Give a certain amount of money to the managed player entity.
    */
    void GiveMoney(int32_t amount);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the score of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetScore() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the score of the managed player entity.
    */
    void SetScore(int32_t score);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the wanted level of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetWantedLevel() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the wanted level of the managed player entity.
    */
    void SetWantedLevel(int32_t level);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection latency of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetPing() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the frames per second of the managed player entity.
    */
    SQMOD_NODISCARD SQFloat GetFPS() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current health of the managed player entity.
    */
    SQMOD_NODISCARD float GetHealth() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the health of the managed player entity.
    */
    void SetHealth(float amount) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current health of the managed player entity.
    */
    SQMOD_NODISCARD float GetArmor() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the health of the managed player entity.
    */
    void SetArmor(float amount) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the immunity flags of the managed player entity.
    */
    SQMOD_NODISCARD uint32_t GetImmunity() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the immunity flags of the managed player entity.
    */
    void SetImmunity(uint32_t flags);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the managed player entity.
    */
    SQMOD_NODISCARD Vector3 GetPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed player entity.
    */
    void SetPosition(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed player entity.
    */
    void SetPositionEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the speed of the managed player entity.
    */
    SQMOD_NODISCARD Vector3 GetSpeed() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the speed of the managed player entity.
    */
    void SetSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the speed of the managed player entity.
    */
    void SetSpeedEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the speed of the managed player entity.
    */
    void AddSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the speed of the managed player entity.
    */
    void AddSpeedEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the heading angle of the managed player entity.
    */
    SQMOD_NODISCARD float GetHeading() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the heading angle of the managed player entity.
    */
    void SetHeading(float angle) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the alpha of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetAlpha() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the alpha of the managed player entity.
    */
    void SetAlpha(int32_t alpha);

    /* --------------------------------------------------------------------------------------------
     * Modify the alpha of the managed player entity.
    */
    void SetAlphaEx(int32_t alpha, int32_t fade);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the aim position of the managed player entity.
    */
    SQMOD_NODISCARD Vector3 GetAimPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the aim direction of the managed player entity.
    */
    SQMOD_NODISCARD Vector3 GetAimDirection() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is burning.
    */
    SQMOD_NODISCARD bool IsBurning() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is crouched.
    */
    SQMOD_NODISCARD bool IsCrouched() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current action of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetAction() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the game keys of the managed player entity.
    */
    SQMOD_NODISCARD uint32_t GetGameKeys() const;

    /* --------------------------------------------------------------------------------------------
     * Embark the managed player entity into the specified vehicle entity.
    */
    bool Embark(CVehicle & vehicle);

    /* --------------------------------------------------------------------------------------------
     * Embark the managed player entity into the specified vehicle entity.
    */
    bool EmbarkEx(CVehicle & vehicle, int32_t slot, bool allocate, bool warp);

    /* --------------------------------------------------------------------------------------------
     * Disembark the managed player entity from the currently embarked vehicle entity.
    */
    void Disembark() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle status of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetVehicleStatus() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the occupied vehicle slot by the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetVehicleSlot() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle in which the managed player entity is embarked.
    */
    SQMOD_NODISCARD LightObj & GetVehicle() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle identifier in which the managed player entity is embarked.
    */
    SQMOD_NODISCARD int32_t GetVehicleID() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the weapon identifier of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetWeapon() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the weapon of the managed player entity.
    */
    void SetWeapon(int32_t wep) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the weapon of the managed player entity.
    */
    void SetWeaponEx(int32_t wep, int32_t ammo) const;

    /* --------------------------------------------------------------------------------------------
     * Give a weapon of the managed player entity.
    */
    void GiveWeapon(int32_t wep, int32_t ammo) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the active weapon ammo of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetAmmo() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the weapon slot of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetWeaponSlot() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the weapon slot of the managed player entity.
    */
    void SetWeaponSlot(int32_t slot) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the weapon identifier at the specified slot for the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetWeaponAtSlot(int32_t slot) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the ammo from the weapon at the specified slot for the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetAmmoAtSlot(int32_t slot) const;

    /* --------------------------------------------------------------------------------------------
     * Remove a certain weapon from the managed player entity.
    */
    void RemoveWeapon(int32_t wep) const;

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
    void SetCameraPositionEx(float xp, float yp, float zp, float xa, float ya, float za) const;

    /* --------------------------------------------------------------------------------------------
     * Restore the camera position of the managed player entity.
    */
    void RestoreCamera() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity has camera locked.
    */
    SQMOD_NODISCARD bool IsCameraLocked() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the animation of the managed player entity.
    */
    void SetAnimation(int32_t anim) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the animation of the managed player entity.
    */
    void SetAnimationEx(int32_t anim, int32_t group) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle that the managed player entity is standing on.
    */
    SQMOD_NODISCARD LightObj & StandingOnVehicle() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the object that the managed player entity is standing on.
    */
    SQMOD_NODISCARD LightObj & StandingOnObject() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity is away.
    */
    SQMOD_NODISCARD bool IsAway() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the player that the managed player entity is spectating.
    */
    SQMOD_NODISCARD LightObj & GetSpectator() const;

    /* --------------------------------------------------------------------------------------------
     * Set the managed player entity to spectate the specified player entity.
    */
    void SetSpectator(CPlayer & target) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier of the player that the managed player entity is spectating.
    */
    SQMOD_NODISCARD SQInteger GetSpectatorID() const;

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
    SQMOD_NODISCARD bool GetPlayer3DArrow(CPlayer & target) const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the target player will see an objective arrow over a player.
    */
    void SetPlayer3DArrowID(SQInteger id, bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the target player sees an objective arrow over a player.
    */
    SQMOD_NODISCARD bool GetPlayer3DArrowID(SQInteger id) const;

    /* --------------------------------------------------------------------------------------------
     * Smoothly pivots the camera angle.
    */
    SQMOD_NODISCARD bool InterpolateCameraLookAt(const Vector3 & pos, uint32_t ms) const;

    /* --------------------------------------------------------------------------------------------
     * Smoothly pivots the camera angle.
    */
    SQMOD_NODISCARD bool InterpolateCameraLookAtEx(float x, float y, float z, uint32_t ms) const;
#endif
    /* --------------------------------------------------------------------------------------------
     * Redirect the managed player entity to the specified server.
    */
    void Redirect(StackStrF & ip, uint32_t port, StackStrF & nick,
                    StackStrF & server_pass, StackStrF & user_pass) const;

    /* --------------------------------------------------------------------------------------------
     * Request a list of the modules loaded into the client session.
    */
    void GetModuleList() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the authority level of the managed player entity.
    */
    void PlaySound(int32_t sound_id) const;
#if SQMOD_SDK_LEAST(2, 1)
    /* --------------------------------------------------------------------------------------------
     * Set how delayed a player's turn handling is when in a vehicle.
    */
    void SetDrunkHandling(SQInteger level) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve how delayed a player's turn handling is when in a vehicle.
    */
    SQMOD_NODISCARD SQInteger GetDrunkHandling() const;

    /* --------------------------------------------------------------------------------------------
     * Set how intense the drunk blur overlay is for a player.
    */
    void SetDrunkVisuals(SQInteger level) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve how intense the drunk blur overlay is for a player.
    */
    SQMOD_NODISCARD SQInteger GetDrunkVisuals() const;
#endif
    /* --------------------------------------------------------------------------------------------
     * Create a checkpoint or sphere for this player.
    */
    LightObj & CreateCheckpointEx1a(int32_t world, bool sphere, float x, float y, float z, // NOLINT(modernize-use-nodiscard)
                            uint8_t r, uint8_t g, uint8_t b, uint8_t a, float radius) const;

    /* --------------------------------------------------------------------------------------------
     * Create a checkpoint or sphere for this player.
    */
    LightObj & CreateCheckpointEx1b(int32_t world, bool sphere, float x, float y, float z, // NOLINT(modernize-use-nodiscard)
                            uint8_t r, uint8_t g, uint8_t b, uint8_t a, float radius,
                            int32_t header, LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * Create a checkpoint or sphere for this player.
    */
    LightObj & CreateCheckpoint1a(int32_t world, bool sphere, const Vector3 & pos, // NOLINT(modernize-use-nodiscard)
                        const Color4 & color, float radius) const;

    /* --------------------------------------------------------------------------------------------
     * Create a checkpoint or sphere for this player.
    */
    LightObj & CreateCheckpoint1b(int32_t world, bool sphere, const Vector3 & pos, const Color4 & color, // NOLINT(modernize-use-nodiscard)
                                    float radius, int32_t header, LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed player entity collides with user defined areas.
    */
    SQMOD_NODISCARD bool GetCollideAreas() const;

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
    SQMOD_NODISCARD int32_t GetAuthority() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the authority level of the managed player entity.
    */
    void SetAuthority(int32_t level) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message prefix at the specified index for the managed player entity.
    */
    SQMOD_NODISCARD const String & GetMessagePrefix(uint32_t index) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the message prefix at the specified index for the managed player entity.
    */
    void SetMessagePrefix(uint32_t index, StackStrF & prefix);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of tracked position changes for the managed player entity.
    */
    SQMOD_NODISCARD SQInteger GetTrackPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the amount of tracked position changes for the managed player entity.
    */
    void SetTrackPosition(SQInteger num) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the amount of tracked position changes for the managed player entity.
    */
    void SetTrackPositionEx(SQInteger num, int32_t header, const LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of tracked heading changes for the managed player entity.
    */
    SQMOD_NODISCARD SQInteger GetTrackHeading() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the amount of tracked heading changes for the managed player entity.
    */
    void SetTrackHeading(SQInteger num) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known weapon for the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetLastWeapon() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known health for the managed player entity.
    */
    SQMOD_NODISCARD float GetLastHealth() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known armour for the managed player entity.
    */
    SQMOD_NODISCARD float GetLastArmour() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known heading for the managed player entity.
    */
    SQMOD_NODISCARD float GetLastHeading() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known position for the managed player entity.
    */
    SQMOD_NODISCARD const Vector3 & GetLastPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the distance traveled by the managed player entity while tracking was enabled.
    */
    SQMOD_NODISCARD SQFloat GetDistance() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the distance traveled by the managed player entity.
    */
    void SetDistance(SQFloat distance) const;

    /* --------------------------------------------------------------------------------------------
     * Check whether distance tracking is enabled for the managed player entity.
    */
    SQMOD_NODISCARD bool GetTrackDistance() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether distance tracking is enabled for the managed player entity.
    */
    void SetTrackDistance(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * Start a new stream with the default size.
    */
    void StartStream();

    /* --------------------------------------------------------------------------------------------
     * Start a new stream with a custom size.
    */
    void StartStreamEx(uint32_t size);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current cursor position of the stream buffer.
    */
    SQMOD_NODISCARD uint32_t GetBufferCursor() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current cursor position of the stream buffer.
    */
    void SetBufferCursor(int32_t pos);

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
    SQMOD_NODISCARD uint32_t GetBufferCapacity() const;

    /* --------------------------------------------------------------------------------------------
     * Send the specified buffer contents to the managed player entity.
    */
    void SendBuffer(const SqBuffer & buffer) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the x axis of the managed player entity.
    */
    SQMOD_NODISCARD float GetPositionX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the y axis of the managed player entity.
    */
    SQMOD_NODISCARD float GetPositionY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the z axis of the managed player entity.
    */
    SQMOD_NODISCARD float GetPositionZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed player entity.
    */
    void SetPositionX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed player entity.
    */
    void SetPositionY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed player entity.
    */
    void SetPositionZ(float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the red color of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetColorR() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the green color of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetColorG() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the blue color of the managed player entity.
    */
    SQMOD_NODISCARD int32_t GetColorB() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the red color of the managed player entity.
    */
    void SetColorR(int32_t r) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the green color of the managed player entity.
    */
    void SetColorG(int32_t g) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the blue color of the managed player entity.
    */
    void SetColorB(int32_t b) const;

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
#ifdef VCMP_ENABLE_OFFICIAL
    /* --------------------------------------------------------------------------------------------
     * Retrieve legacy object instance for this entity.
    */
    LightObj & GetLegacyObject() const;
#endif
};

} // Namespace:: SqMod
