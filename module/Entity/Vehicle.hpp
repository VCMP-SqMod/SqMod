#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Circular locks employed by the vehicle manager.
*/
enum VehicleCircularLocks
{
    VEHICLECL_EMIT_VEHICLE_OPTION       = (1u << 0u),
    VEHICLECL_EMIT_VEHICLE_WORLD        = (1u << 1u),
    VEHICLECL_EMIT_VEHICLE_IMMUNITY     = (1u << 2u),
    VEHICLECL_EMIT_VEHICLE_PARTSTATUS   = (1u << 3u),
    VEHICLECL_EMIT_VEHICLE_TYRESTATUS   = (1u << 4u),
    VEHICLECL_EMIT_VEHICLE_DAMAGEDATA   = (1u << 5u),
    VEHICLECL_EMIT_VEHICLE_RADIO        = (1u << 6u),
    VEHICLECL_EMIT_VEHICLE_HANDLINGRULE = (1u << 7u),
    VEHICLECL_EMIT_VEHICLE_EMBARK       = (1u << 8u) // This should probably be shared with CPlayer
};

/* ------------------------------------------------------------------------------------------------
 * Manages a single vehicle entity.
*/
class CVehicle
{
    // --------------------------------------------------------------------------------------------
    friend class Core;
    friend class VehicleInst;

private:

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
     * Prevent events from triggering themselves.
    */
    uint32_t      m_CircularLocks;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit CVehicle(int32_t id);

public:

    /* --------------------------------------------------------------------------------------------
     * Maximum possible number that could represent an identifier for this entity type.
    */
    static const int32_t Max;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CVehicle(const CVehicle &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    CVehicle(CVehicle &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    CVehicle & operator = (const CVehicle &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    CVehicle & operator = (CVehicle &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * See whether this instance manages a valid entity.
    */
    void Validate() const
    {
        if (INVALID_ENTITY(m_ID))
        {
            STHROWF("Invalid vehicle reference [{}]", m_Tag);
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
    CVehicle & ApplyTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user data.
    */
    SQMOD_NODISCARD LightObj & GetData();

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(LightObj & data);

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed vehicle entity.
    */
    bool Destroy0() const // NOLINT(modernize-use-nodiscard)
    {
        return Destroy(0, NullLightObj());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed vehicle entity.
    */
    bool Destroy1(int32_t header) const // NOLINT(modernize-use-nodiscard)
    {
        return Destroy(header, NullLightObj());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed vehicle entity.
    */
    bool Destroy(int32_t header, LightObj & payload) const; // NOLINT(modernize-use-nodiscard)

    /* --------------------------------------------------------------------------------------------
     * Retrieve the events table of this entity.
    */
    SQMOD_NODISCARD LightObj & GetEvents() const;

    /* --------------------------------------------------------------------------------------------
     * Emit a custom event for the managed entity
    */
    void CustomEvent(int32_t header, LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * See if the managed vehicle entity is streamed for the specified player.
    */
    bool IsStreamedFor(CPlayer & player) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current option value of the managed vehicle entity.
    */
    SQMOD_NODISCARD bool GetOption(int32_t option_id) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the current option value of the managed vehicle entity.
    */
    void SetOption(int32_t option_id, bool toggle);

    /* --------------------------------------------------------------------------------------------
     * Modify the current option value of the managed vehicle entity.
    */
    void SetOptionEx(int32_t option_id, bool toggle, int32_t header, LightObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the synchronization source of the managed vehicle entity.
    */
    SQMOD_NODISCARD int32_t GetSyncSource() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the synchronization type of the managed vehicle entity.
    */
    SQMOD_NODISCARD int32_t GetSyncType() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the managed vehicle entity exists.
    */
    SQMOD_NODISCARD int32_t GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the world in which the managed vehicle entity exists.
    */
    void SetWorld(int32_t world);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle model of the managed vehicle entity.
    */
    SQMOD_NODISCARD int32_t GetModel() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the slot occupant from the managed vehicle entity.
    */
    SQMOD_NODISCARD LightObj & GetOccupant(int32_t slot) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the slot occupant identifier from the managed vehicle entity.
    */
    SQMOD_NODISCARD int32_t GetOccupantID(int32_t slot) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed vehicle entity has an occupant in a certain slot.
    */
    SQMOD_NODISCARD bool HasOccupant(int32_t slot) const;

    /* --------------------------------------------------------------------------------------------
     * Respawn the managed vehicle entity.
    */
    void Respawn() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the immunity flags of the managed vehicle entity.
    */
    SQMOD_NODISCARD uint32_t GetImmunity() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the immunity flags of the managed vehicle entity.
    */
    void SetImmunity(uint32_t flags);

    /* --------------------------------------------------------------------------------------------
     * Explode the managed vehicle entity.
    */
    void Explode() const;

    /* --------------------------------------------------------------------------------------------
     * Flatten the tyres of the managed vehicle entity.
    */
    void FlattenTyres(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed vehicle entity is wrecked.
    */
    SQMOD_NODISCARD bool IsWrecked() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the managed vehicle entity.
    */
    SQMOD_NODISCARD Vector3 GetPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed vehicle entity.
    */
    void SetPosition(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed vehicle entity.
    */
    void SetPositionB(const Vector3 & pos, bool empty) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed vehicle entity.
    */
    void SetPositionEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed vehicle entity.
    */
    void SetPositionExB(float x, float y, float z, bool empty) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation of the managed vehicle entity.
    */
    SQMOD_NODISCARD Quaternion GetRotation() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation of the managed vehicle entity.
    */
    void SetRotation(const Quaternion & rot) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation of the managed vehicle entity.
    */
    void SetRotationEx(float x, float y, float z, float w) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation of the managed vehicle entity.
    */
    SQMOD_NODISCARD Vector3 GetRotationEuler() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler rotation of the managed vehicle entity.
    */
    void SetRotationEuler(const Vector3 & rot) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler rotation of the managed vehicle entity.
    */
    void SetRotationEulerEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the speed of the managed vehicle entity.
    */
    SQMOD_NODISCARD Vector3 GetSpeed() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the speed of the managed vehicle entity.
    */
    void SetSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the speed of the managed vehicle entity.
    */
    void SetSpeedEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the speed of the managed vehicle entity.
    */
    void AddSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the speed of the managed vehicle entity.
    */
    void AddSpeedEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative speed of the managed vehicle entity.
    */
    SQMOD_NODISCARD Vector3 GetRelativeSpeed() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative speed of the managed vehicle entity.
    */
    void SetRelativeSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative speed of the managed vehicle entity.
    */
    void SetRelativeSpeedEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative speed of the managed vehicle entity.
    */
    void AddRelativeSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative speed of the managed vehicle entity.
    */
    void AddRelativeSpeedEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the turn speed of the managed vehicle entity.
    */
    SQMOD_NODISCARD Vector3 GetTurnSpeed() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the turn speed of the managed vehicle entity.
    */
    void SetTurnSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the turn speed of the managed vehicle entity.
    */
    void SetTurnSpeedEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the turn speed of the managed vehicle entity.
    */
    void AddTurnSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the turn speed of the managed vehicle entity.
    */
    void AddTurnSpeedEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative turn speed of the managed vehicle entity.
    */
    SQMOD_NODISCARD Vector3 GetRelativeTurnSpeed() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn speed of the managed vehicle entity.
    */
    void SetRelativeTurnSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn speed of the managed vehicle entity.
    */
    void SetRelativeTurnSpeedEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn speed of the managed vehicle entity.
    */
    void AddRelativeTurnSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn speed of the managed vehicle entity.
    */
    void AddRelativeTurnSpeedEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the spawn position of the managed vehicle entity.
    */
    SQMOD_NODISCARD Vector3 GetSpawnPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the spawn position of the managed vehicle entity.
    */
    void SetSpawnPosition(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the spawn position of the managed vehicle entity.
    */
    void SetSpawnPositionEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the spawn rotation of the managed vehicle entity.
    */
    SQMOD_NODISCARD Quaternion GetSpawnRotation() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the spawn rotation of the managed vehicle entity.
    */
    void SetSpawnRotation(const Quaternion & rot) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the spawn rotation of the managed vehicle entity.
    */
    void SetSpawnRotationEx(float x, float y, float z, float w) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler spawn rotation of the managed vehicle entity.
    */
    SQMOD_NODISCARD Vector3 GetSpawnRotationEuler() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler spawn rotation of the managed vehicle entity.
    */
    void SetSpawnRotationEuler(const Vector3 & rot) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler spawn rotation of the managed vehicle entity.
    */
    void SetSpawnRotationEulerEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the respawn timer of the managed vehicle entity.
    */
    SQMOD_NODISCARD uint32_t GetIdleRespawnTimer() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the respawn timer of the managed vehicle entity.
    */
    void SetIdleRespawnTimer(uint32_t millis) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the health of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetHealth() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the health of the managed vehicle entity.
    */
    void SetHealth(float amount) const;

    /* --------------------------------------------------------------------------------------------
     * Fix the damage and restore health for the managed vehicle entity.
    */
    void Fix() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the primary color of the managed vehicle entity.
    */
    SQMOD_NODISCARD int32_t GetPrimaryColor() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the primary color of the managed vehicle entity.
    */
    void SetPrimaryColor(int32_t col) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the secondary color of the managed vehicle entity.
    */
    SQMOD_NODISCARD int32_t GetSecondaryColor() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the secondary color of the managed vehicle entity.
    */
    void SetSecondaryColor(int32_t col) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the primary and secondary colors of the managed vehicle entity.
    */
    void SetColors(int32_t primary, int32_t secondary) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the part status of the managed vehicle entity.
    */
    SQMOD_NODISCARD int32_t GetPartStatus(int32_t part) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the part status of the managed vehicle entity.
    */
    void SetPartStatus(int32_t part, int32_t status);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the tyre status of the managed vehicle entity.
    */
    SQMOD_NODISCARD int32_t GetTyreStatus(int32_t tyre) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the tyre status of the managed vehicle entity.
    */
    void SetTyreStatus(int32_t tyre, int32_t status);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the damage data of the managed vehicle entity.
    */
    SQMOD_NODISCARD uint32_t GetDamageData() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the damage data of the managed vehicle entity.
    */
    void SetDamageData(uint32_t data);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the radio of the managed vehicle entity.
    */
    SQMOD_NODISCARD int32_t GetRadio() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the radio of the managed vehicle entity.
    */
    void SetRadio(int32_t radio);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the turret rotation of the managed vehicle entity.
    */
    SQMOD_NODISCARD Vector2 GetTurretRotation() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the horizontal turret rotation of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetHorizontalTurretRotation() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vertical turret rotation of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetVerticalTurretRotation() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the specified handling rule exists in the managed vehicle entity.
    */
    SQMOD_NODISCARD bool ExistsHandlingRule(int32_t rule) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the handling data of the managed vehicle entity.
    */
    SQMOD_NODISCARD SQFloat GetHandlingRule(int32_t rule) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the handling data of the managed vehicle entity.
    */
    void SetHandlingRule(int32_t rule, float data);

    /* --------------------------------------------------------------------------------------------
     * Reset the specified handling rule for the managed vehicle entity.
    */
    void ResetHandlingRule(int32_t rule);

    /* --------------------------------------------------------------------------------------------
     * Reset all the handling rules for the managed vehicle entity.
    */
    void ResetHandlings() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the lights data for the managed vehicle entity.
    */
    SQMOD_NODISCARD uint32_t GetLightsData() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the lights data for the managed vehicle entity.
    */
    void SetLightsData(int32_t data) const;

    /* --------------------------------------------------------------------------------------------
     * Embark the specified player entity into the managed vehicle entity.
    */
    bool Embark(CPlayer & player);

    /* --------------------------------------------------------------------------------------------
     * Embark the specified player entity into the managed vehicle entity.
    */
    bool EmbarkEx(CPlayer & player, int32_t slot, bool allocate, bool warp);
#if SQMOD_SDK_LEAST(2, 1)
    /* --------------------------------------------------------------------------------------------
     * Set whether the target player will see an objective arrow over a vehicle.
    */
    void SetPlayer3DArrow(CPlayer & target, bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the target player sees an objective arrow over a vehicle.
    */
    SQMOD_NODISCARD bool GetPlayer3DArrow(CPlayer & target) const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the target player will see an objective arrow over a vehicle.
    */
    void SetPlayer3DArrowID(SQInteger id, bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the target player sees an objective arrow over a vehicle.
    */
    SQMOD_NODISCARD bool GetPlayer3DArrowID(SQInteger id) const;
#endif
    /* --------------------------------------------------------------------------------------------
     * See whether the managed vehicle entity collides with user defined areas.
    */
    SQMOD_NODISCARD bool GetCollideAreas() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed vehicle entity can collide with user defined areas.
    */
    void SetCollideAreas(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed vehicle entity can collide with user defined areas (with last test).
    */
    void SetAreasCollide(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of tracked position changes for the managed vehicle entity.
    */
    SQMOD_NODISCARD SQInteger GetTrackPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of tracked position changes for the managed vehicle entity.
    */
    void SetTrackPosition(SQInteger num) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of tracked rotation changes for the managed vehicle entity.
    */
    SQMOD_NODISCARD SQInteger GetTrackRotation() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of tracked rotation changes for the managed vehicle entity.
    */
    void SetTrackRotation(SQInteger num) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known primary color for the managed vehicle entity.
    */
    SQMOD_NODISCARD int32_t GetLastPrimaryColor() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known secondary color for the managed vehicle entity.
    */
    SQMOD_NODISCARD int32_t GetLastSecondaryColor() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known health for the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetLastHealth() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known position for the managed vehicle entity.
    */
    SQMOD_NODISCARD const Vector3 & GetLastPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known rotation for the managed vehicle entity.
    */
    SQMOD_NODISCARD const Quaternion & GetLastRotation() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the distance traveled by the managed vehicle entity while tracking was enabled.
    */
    SQMOD_NODISCARD SQFloat GetDistance() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the distance traveled by the managed vehicle entity.
    */
    void SetDistance(SQFloat distance) const;

    /* --------------------------------------------------------------------------------------------
     * Check whether distance tracking is enabled for the managed vehicle entity.
    */
    SQMOD_NODISCARD bool GetTrackDistance() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether distance tracking is enabled for the managed vehicle entity.
    */
    void SetTrackDistance(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the x axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetPositionX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the y axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetPositionY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the z axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetPositionZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed vehicle entity.
    */
    void SetPositionX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed vehicle entity.
    */
    void SetPositionY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed vehicle entity.
    */
    void SetPositionZ(float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the x axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetRotationX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the y axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetRotationY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the z axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetRotationZ() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation amount of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetRotationW() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the x axis of the managed vehicle entity.
    */
    void SetRotationX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the y axis of the managed vehicle entity.
    */
    void SetRotationY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the z axis of the managed vehicle entity.
    */
    void SetRotationZ(float z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation amount of the managed vehicle entity.
    */
    void SetRotationW(float w) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation on the x axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetEulerRotationX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation on the y axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetEulerRotationY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation on the z axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetEulerRotationZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler rotation on the x axis of the managed vehicle entity.
    */
    void SetEulerRotationX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler rotation on the y axis of the managed vehicle entity.
    */
    void SetEulerRotationY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler rotation on the z axis of the managed vehicle entity.
    */
    void SetEulerRotationZ(float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the velocity on the x axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetSpeedX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the velocity on the y axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetSpeedY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the velocity on the z axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetSpeedZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the velocity on the x axis of the managed vehicle entity.
    */
    void SetSpeedX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the velocity on the y axis of the managed vehicle entity.
    */
    void SetSpeedY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the velocity on the z axis of the managed vehicle entity.
    */
    void SetSpeedZ(float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative velocity on the x axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetRelativeSpeedX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative velocity on the y axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetRelativeSpeedY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative velocity on the z axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetRelativeSpeedZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative velocity on the x axis of the managed vehicle entity.
    */
    void SetRelativeSpeedX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative velocity on the y axis of the managed vehicle entity.
    */
    void SetRelativeSpeedY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative velocity on the z axis of the managed vehicle entity.
    */
    void SetRelativeSpeedZ(float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the turn velocity on the x axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetTurnSpeedX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the turn velocity on the y axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetTurnSpeedY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the turn velocity on the z axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetTurnSpeedZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the turn velocity on the x axis of the managed vehicle entity.
    */
    void SetTurnSpeedX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the turn velocity on the y axis of the managed vehicle entity.
    */
    void SetTurnSpeedY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the turn velocity on the z axis of the managed vehicle entity.
    */
    void SetTurnSpeedZ(float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative turn velocity on the x axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetRelativeTurnSpeedX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative turn velocity on the y axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetRelativeTurnSpeedY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative turn velocity on the z axis of the managed vehicle entity.
    */
    SQMOD_NODISCARD float GetRelativeTurnSpeedZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn velocity on the x axis of the managed vehicle entity.
    */
    void SetRelativeTurnSpeedX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn velocity on the y axis of the managed vehicle entity.
    */
    void SetRelativeTurnSpeedY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn velocity on the z axis of the managed vehicle entity.
    */
    void SetRelativeTurnSpeedZ(float z) const;
#ifdef VCMP_ENABLE_OFFICIAL
    /* --------------------------------------------------------------------------------------------
     * Retrieve legacy object instance for this entity.
    */
    LightObj & GetLegacyObject() const;
#endif
};

} // Namespace:: SqMod
