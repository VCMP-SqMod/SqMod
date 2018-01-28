#ifndef _ENTITY_VEHICLE_HPP_
#define _ENTITY_VEHICLE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Circular locks employed by the vehicle manager.
*/
enum VehicleCircularLocks
{
    VEHICLECL_EMIT_VEHICLE_OPTION       = (1 << 0),
    VEHICLECL_EMIT_VEHICLE_WORLD        = (2 << 0),
    VEHICLECL_EMIT_VEHICLE_IMMUNITY     = (3 << 0),
    VEHICLECL_EMIT_VEHICLE_PARTSTATUS   = (4 << 0),
    VEHICLECL_EMIT_VEHICLE_TYRESTATUS   = (5 << 0),
    VEHICLECL_EMIT_VEHICLE_DAMAGEDATA   = (6 << 0),
    VEHICLECL_EMIT_VEHICLE_RADIO        = (7 << 0),
    VEHICLECL_EMIT_VEHICLE_HANDLINGRULE = (8 << 0)
};

/* ------------------------------------------------------------------------------------------------
 * Manages a single vehicle entity.
*/
class CVehicle
{
    // --------------------------------------------------------------------------------------------
    friend class Core;

private:

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
     * Prevent events from triggering themselves.
    */
    Uint32      m_CircularLocks;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    CVehicle(Int32 id);

public:

    /* --------------------------------------------------------------------------------------------
     * Maximum possible number that could represent an identifier for this entity type.
    */
    static const Int32 Max;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CVehicle(const CVehicle &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    CVehicle(CVehicle &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CVehicle();

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
            STHROWF("Invalid vehicle reference [%s]", m_Tag.c_str());
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
    void SetTag(const StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    CVehicle & ApplyTag(const StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user data.
    */
    LightObj & GetData();

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(LightObj & data);

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed vehicle entity.
    */
    bool Destroy()
    {
        return Destroy(0, NullLightObj());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed vehicle entity.
    */
    bool Destroy(Int32 header)
    {
        return Destroy(header, NullLightObj());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed vehicle entity.
    */
    bool Destroy(Int32 header, LightObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the events table of this entity.
    */
    LightObj & GetEvents() const;

    /* --------------------------------------------------------------------------------------------
     * Emit a custom event for the managed entity
    */
    void CustomEvent(Int32 header, LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * See if the managed vehicle entity is streamed for the specified player.
    */
    bool IsStreamedFor(CPlayer & player) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current option value of the managed vehicle entity.
    */
    bool GetOption(Int32 option_id) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the current option value of the managed vehicle entity.
    */
    void SetOption(Int32 option_id, bool toggle);

    /* --------------------------------------------------------------------------------------------
     * Modify the current option value of the managed vehicle entity.
    */
    void SetOptionEx(Int32 option_id, bool toggle, Int32 header, LightObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the synchronization source of the managed vehicle entity.
    */
    Int32 GetSyncSource() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the synchronization type of the managed vehicle entity.
    */
    Int32 GetSyncType() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the managed vehicle entity exists.
    */
    Int32 GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the world in which the managed vehicle entity exists.
    */
    void SetWorld(Int32 world);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle model of the managed vehicle entity.
    */
    Int32 GetModel() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the slot occupant from the managed vehicle entity.
    */
    LightObj & GetOccupant(Int32 slot) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the slot occupant identifier from the managed vehicle entity.
    */
    Int32 GetOccupantID(Int32 slot) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed vehicle entity has an occupant in a certain slot.
    */
    bool HasOccupant(Int32 slot) const;

    /* --------------------------------------------------------------------------------------------
     * Respawn the managed vehicle entity.
    */
    void Respawn() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the immunity flags of the managed vehicle entity.
    */
    Int32 GetImmunity() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the immunity flags of the managed vehicle entity.
    */
    void SetImmunity(Int32 flags);

    /* --------------------------------------------------------------------------------------------
     * Explode the managed vehicle entity.
    */
    void Explode() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed vehicle entity is wrecked.
    */
    bool IsWrecked() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the managed vehicle entity.
    */
    Vector3 GetPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed vehicle entity.
    */
    void SetPosition(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed vehicle entity.
    */
    void SetPositionEx(const Vector3 & pos, bool empty) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed vehicle entity.
    */
    void SetPositionEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed vehicle entity.
    */
    void SetPositionEx(Float32 x, Float32 y, Float32 z, bool empty) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation of the managed vehicle entity.
    */
    Quaternion GetRotation() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation of the managed vehicle entity.
    */
    void SetRotation(const Quaternion & rot) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation of the managed vehicle entity.
    */
    void SetRotationEx(Float32 x, Float32 y, Float32 z, Float32 w) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation of the managed vehicle entity.
    */
    Vector3 GetRotationEuler() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler rotation of the managed vehicle entity.
    */
    void SetRotationEuler(const Vector3 & rot) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler rotation of the managed vehicle entity.
    */
    void SetRotationEulerEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the speed of the managed vehicle entity.
    */
    Vector3 GetSpeed() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the speed of the managed vehicle entity.
    */
    void SetSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the speed of the managed vehicle entity.
    */
    void SetSpeedEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the speed of the managed vehicle entity.
    */
    void AddSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the speed of the managed vehicle entity.
    */
    void AddSpeedEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative speed of the managed vehicle entity.
    */
    Vector3 GetRelativeSpeed() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative speed of the managed vehicle entity.
    */
    void SetRelativeSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative speed of the managed vehicle entity.
    */
    void SetRelativeSpeedEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative speed of the managed vehicle entity.
    */
    void AddRelativeSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative speed of the managed vehicle entity.
    */
    void AddRelativeSpeedEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the turn speed of the managed vehicle entity.
    */
    Vector3 GetTurnSpeed() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the turn speed of the managed vehicle entity.
    */
    void SetTurnSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the turn speed of the managed vehicle entity.
    */
    void SetTurnSpeedEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the turn speed of the managed vehicle entity.
    */
    void AddTurnSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the turn speed of the managed vehicle entity.
    */
    void AddTurnSpeedEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative turn speed of the managed vehicle entity.
    */
    Vector3 GetRelativeTurnSpeed() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn speed of the managed vehicle entity.
    */
    void SetRelativeTurnSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn speed of the managed vehicle entity.
    */
    void SetRelativeTurnSpeedEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn speed of the managed vehicle entity.
    */
    void AddRelativeTurnSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn speed of the managed vehicle entity.
    */
    void AddRelativeTurnSpeedEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the spawn position of the managed vehicle entity.
    */
    Vector3 GetSpawnPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the spawn position of the managed vehicle entity.
    */
    void SetSpawnPosition(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the spawn position of the managed vehicle entity.
    */
    void SetSpawnPositionEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the spawn rotation of the managed vehicle entity.
    */
    Quaternion GetSpawnRotation() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the spawn rotation of the managed vehicle entity.
    */
    void SetSpawnRotation(const Quaternion & rot) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the spawn rotation of the managed vehicle entity.
    */
    void SetSpawnRotationEx(Float32 x, Float32 y, Float32 z, Float32 w) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler spawn rotation of the managed vehicle entity.
    */
    Vector3 GetSpawnRotationEuler() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler spawn rotation of the managed vehicle entity.
    */
    void SetSpawnRotationEuler(const Vector3 & rot) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler spawn rotation of the managed vehicle entity.
    */
    void SetSpawnRotationEulerEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the respawn timer of the managed vehicle entity.
    */
    Uint32 GetIdleRespawnTimer() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the respawn timer of the managed vehicle entity.
    */
    void SetIdleRespawnTimer(Uint32 millis) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the health of the managed vehicle entity.
    */
    Float32 GetHealth() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the health of the managed vehicle entity.
    */
    void SetHealth(Float32 amount) const;

    /* --------------------------------------------------------------------------------------------
     * Fix the damage and restore health for the managed vehicle entity.
    */
    void Fix() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the primary color of the managed vehicle entity.
    */
    Int32 GetPrimaryColor() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the primary color of the managed vehicle entity.
    */
    void SetPrimaryColor(Int32 col) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the secondary color of the managed vehicle entity.
    */
    Int32 GetSecondaryColor() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the secondary color of the managed vehicle entity.
    */
    void SetSecondaryColor(Int32 col) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the primary and secondary colors of the managed vehicle entity.
    */
    void SetColors(Int32 primary, Int32 secondary) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the part status of the managed vehicle entity.
    */
    Int32 GetPartStatus(Int32 part) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the part status of the managed vehicle entity.
    */
    void SetPartStatus(Int32 part, Int32 status);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the tyre status of the managed vehicle entity.
    */
    Int32 GetTyreStatus(Int32 tyre) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the tyre status of the managed vehicle entity.
    */
    void SetTyreStatus(Int32 tyre, Int32 status);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the damage data of the managed vehicle entity.
    */
    Uint32 GetDamageData() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the damage data of the managed vehicle entity.
    */
    void SetDamageData(Uint32 data);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the radio of the managed vehicle entity.
    */
    Int32 GetRadio() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the radio of the managed vehicle entity.
    */
    void SetRadio(Int32 radio);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the turret rotation of the managed vehicle entity.
    */
    Vector2 GetTurretRotation() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the horizontal turret rotation of the managed vehicle entity.
    */
    Float32 GetHorizontalTurretRotation() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vertical turret rotation of the managed vehicle entity.
    */
    Float32 GetVerticalTurretRotation() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the specified handling ruleexists in the managed vehicle entity.
    */
    bool ExistsHandlingRule(Int32 rule) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the handling data of the managed vehicle entity.
    */
    Float32 GetHandlingRule(Int32 rule) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the handling data of the managed vehicle entity.
    */
    void SetHandlingRule(Int32 rule, Float32 data);

    /* --------------------------------------------------------------------------------------------
     * Reset the specified handling rule for the managed vehicle entity.
    */
    void ResetHandlingRule(Int32 rule);

    /* --------------------------------------------------------------------------------------------
     * Reset all the handling rules for the managed vehicle entity.
    */
    void ResetHandlings() const;

    /* --------------------------------------------------------------------------------------------
     * Embark the specified player entity into the managed vehicle entity.
    */
    bool Embark(CPlayer & player) const;

    /* --------------------------------------------------------------------------------------------
     * Embark the specified player entity into the managed vehicle entity.
    */
    bool Embark(CPlayer & player, Int32 slot, bool allocate, bool warp) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed vehicle entity collides with user defined areas.
    */
    bool GetCollideAreas() const;

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
    SQInteger GetTrackPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of tracked position changes for the managed vehicle entity.
    */
    void SetTrackPosition(SQInteger num) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of tracked rotation changes for the managed vehicle entity.
    */
    SQInteger GetTrackRotation() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of tracked rotation changes for the managed vehicle entity.
    */
    void SetTrackRotation(SQInteger num) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known primary color for the managed vehicle entity.
    */
    Int32 GetLastPrimaryColor() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known secondary color for the managed vehicle entity.
    */
    Int32 GetLastSecondaryColor() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known health for the managed vehicle entity.
    */
    Float32 GetLastHealth() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known position for the managed player entity.
    */
    const Vector3 & GetLastPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known rotation for the managed player entity.
    */
    const Quaternion & GetLastRotation() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the x axis of the managed vehicle entity.
    */
    Float32 GetPositionX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the y axis of the managed vehicle entity.
    */
    Float32 GetPositionY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the z axis of the managed vehicle entity.
    */
    Float32 GetPositionZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed vehicle entity.
    */
    void SetPositionX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed vehicle entity.
    */
    void SetPositionY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed vehicle entity.
    */
    void SetPositionZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the x axis of the managed vehicle entity.
    */
    Float32 GetRotationX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the y axis of the managed vehicle entity.
    */
    Float32 GetRotationY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the z axis of the managed vehicle entity.
    */
    Float32 GetRotationZ() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation amount of the managed vehicle entity.
    */
    Float32 GetRotationW() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the x axis of the managed vehicle entity.
    */
    void SetRotationX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the y axis of the managed vehicle entity.
    */
    void SetRotationY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the z axis of the managed vehicle entity.
    */
    void SetRotationZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation amount of the managed vehicle entity.
    */
    void SetRotationW(Float32 w) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation on the x axis of the managed vehicle entity.
    */
    Float32 GetEulerRotationX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation on the y axis of the managed vehicle entity.
    */
    Float32 GetEulerRotationY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation on the z axis of the managed vehicle entity.
    */
    Float32 GetEulerRotationZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler rotation on the x axis of the managed vehicle entity.
    */
    void SetEulerRotationX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler rotation on the y axis of the managed vehicle entity.
    */
    void SetEulerRotationY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler rotation on the z axis of the managed vehicle entity.
    */
    void SetEulerRotationZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the velocity on the x axis of the managed vehicle entity.
    */
    Float32 GetSpeedX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the velocity on the y axis of the managed vehicle entity.
    */
    Float32 GetSpeedY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the velocity on the z axis of the managed vehicle entity.
    */
    Float32 GetSpeedZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the velocity on the x axis of the managed vehicle entity.
    */
    void SetSpeedX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the velocity on the y axis of the managed vehicle entity.
    */
    void SetSpeedY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the velocity on the z axis of the managed vehicle entity.
    */
    void SetSpeedZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative velocity on the x axis of the managed vehicle entity.
    */
    Float32 GetRelativeSpeedX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative velocity on the y axis of the managed vehicle entity.
    */
    Float32 GetRelativeSpeedY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative velocity on the z axis of the managed vehicle entity.
    */
    Float32 GetRelativeSpeedZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative velocity on the x axis of the managed vehicle entity.
    */
    void SetRelativeSpeedX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative velocity on the y axis of the managed vehicle entity.
    */
    void SetRelativeSpeedY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative velocity on the z axis of the managed vehicle entity.
    */
    void SetRelativeSpeedZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the turn velocity on the x axis of the managed vehicle entity.
    */
    Float32 GetTurnSpeedX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the turn velocity on the y axis of the managed vehicle entity.
    */
    Float32 GetTurnSpeedY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the turn velocity on the z axis of the managed vehicle entity.
    */
    Float32 GetTurnSpeedZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the turn velocity on the x axis of the managed vehicle entity.
    */
    void SetTurnSpeedX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the turn velocity on the y axis of the managed vehicle entity.
    */
    void SetTurnSpeedY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the turn velocity on the z axis of the managed vehicle entity.
    */
    void SetTurnSpeedZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative turn velocity on the x axis of the managed vehicle entity.
    */
    Float32 GetRelativeTurnSpeedX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative turn velocity on the y axis of the managed vehicle entity.
    */
    Float32 GetRelativeTurnSpeedY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative turn velocity on the z axis of the managed vehicle entity.
    */
    Float32 GetRelativeTurnSpeedZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn velocity on the x axis of the managed vehicle entity.
    */
    void SetRelativeTurnSpeedX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn velocity on the y axis of the managed vehicle entity.
    */
    void SetRelativeTurnSpeedY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn velocity on the z axis of the managed vehicle entity.
    */
    void SetRelativeTurnSpeedZ(Float32 z) const;
};

} // Namespace:: SqMod

#endif // _ENTITY_VEHICLE_HPP_