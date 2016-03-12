#ifndef _ENTITY_VEHICLE_HPP_
#define _ENTITY_VEHICLE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages a single vehicle entity.
*/
class CVehicle
{
    // --------------------------------------------------------------------------------------------
    friend class Core;

private:

    // --------------------------------------------------------------------------------------------
    static Vector3      s_Vector3;
    static Vector4      s_Vector4;
    static Quaternion   s_Quaternion;

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
            SqThrowF("Invalid vehicle reference [%s]", m_Tag.c_str());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const CVehicle & o) const;

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
     * Destroy the managed vehicle entity.
    */
    bool Destroy()
    {
        return Destroy(0, NullObject());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed vehicle entity.
    */
    bool Destroy(Int32 header)
    {
        return Destroy(header, NullObject());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed vehicle entity.
    */
    bool Destroy(Int32 header, Object & payload);

    /* --------------------------------------------------------------------------------------------
     * Bind to an event supported by this entity type.
    */
    void BindEvent(Int32 evid, Object & env, Function & func) const;

    /* --------------------------------------------------------------------------------------------
     * See if the managed vehicle entity is streamed for the specified player.
    */
    bool IsStreamedFor(CPlayer & player) const;

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
    void SetWorld(Int32 world) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle model of the managed vehicle entity.
    */
    Int32 GetModel() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the slot occupant from the managed vehicle entity.
    */
    Object & GetOccupant(Int32 slot) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the slot occupant identifier from the managed vehicle entity.
    */
    Int32 GetOccupantID(Int32 slot) const;

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
    void SetImmunity(Int32 flags) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed vehicle entity is wrecked.
    */
    bool IsWrecked() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the managed vehicle entity.
    */
    const Vector3 & GetPosition() const;

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
    const Quaternion & GetRotation() const;

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
    const Vector3 & GetRotationEuler() const;

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
    const Vector3 & GetSpeed() const;

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
    const Vector3 & GetRelSpeed() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative speed of the managed vehicle entity.
    */
    void SetRelSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative speed of the managed vehicle entity.
    */
    void SetRelSpeedEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative speed of the managed vehicle entity.
    */
    void AddRelSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative speed of the managed vehicle entity.
    */
    void AddRelSpeedEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the turn speed of the managed vehicle entity.
    */
    const Vector3 & GetTurnSpeed() const;

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
    const Vector3 & GetRelTurnSpeed() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn speed of the managed vehicle entity.
    */
    void SetRelTurnSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn speed of the managed vehicle entity.
    */
    void SetRelTurnSpeedEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn speed of the managed vehicle entity.
    */
    void AddRelTurnSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the relative turn speed of the managed vehicle entity.
    */
    void AddRelTurnSpeedEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the spawn position of the managed vehicle entity.
    */
    const Vector4 & GetSpawnPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the spawn position of the managed vehicle entity.
    */
    void SetSpawnPosition(const Vector4 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the spawn position of the managed vehicle entity.
    */
    void SetSpawnPositionEx(Float32 x, Float32 y, Float32 z, Float32 w) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the spawn rotation of the managed vehicle entity.
    */
    const Quaternion & GetSpawnRotation() const;

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
    const Vector3 & GetSpawnRotationEuler() const;

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
    Uint32 GetRespawnTimer() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the respawn timer of the managed vehicle entity.
    */
    void SetRespawnTimer(Uint32 timer) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the health of the managed vehicle entity.
    */
    Float32 GetHealth() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the health of the managed vehicle entity.
    */
    void SetHealth(Float32 amount) const;

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
     * See whether the managed vehicle entity is locked.
    */
    bool GetLocked() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed vehicle entity is locked.
    */
    void SetLocked(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the part status of the managed vehicle entity.
    */
    Int32 GetPartStatus(Int32 part) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the part status of the managed vehicle entity.
    */
    void SetPartStatus(Int32 part, Int32 status) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the tyre status of the managed vehicle entity.
    */
    Int32 GetTyreStatus(Int32 tyre) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the tyre status of the managed vehicle entity.
    */
    void SetTyreStatus(Int32 tyre, Int32 status) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the damage data of the managed vehicle entity.
    */
    Uint32 GetDamageData() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the damage data of the managed vehicle entity.
    */
    void SetDamageData(Uint32 data) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed vehicle entity has alarm.
    */
    bool GetAlarm() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed vehicle entity has alarm.
    */
    void SetAlarm(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed vehicle entity has lights.
    */
    bool GetLights() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed vehicle entity has lights.
    */
    void SetLights(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the radio of the managed vehicle entity.
    */
    Int32 GetRadio() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the radio of the managed vehicle entity.
    */
    void SetRadio(Int32 radio) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed vehicle entity has radio locked.
    */
    bool GetRadioLocked() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed vehicle entity has radio locked.
    */
    void SetRadioLocked(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed vehicle entity is in ghost state.
    */
    bool GetGhostState() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed vehicle entity is in ghost state.
    */
    void SetGhostState(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * Reset all the handling rules for the managed vehicle entity.
    */
    void ResetHandling() const;

    /* --------------------------------------------------------------------------------------------
     * Reset the specified handling rule for the managed vehicle entity.
    */
    void ResetHandling(Int32 rule) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the specified handling ruleexists in the managed vehicle entity.
    */
    bool ExistsHandling(Int32 rule) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the handling data of the managed vehicle entity.
    */
    Float32 GetHandlingData(Int32 rule) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the handling data of the managed vehicle entity.
    */
    void SetHandlingData(Int32 rule, Float32 data) const;

    /* --------------------------------------------------------------------------------------------
     * Embark the specified player entity into the managed vehicle entity.
    */
    void Embark(CPlayer & player) const;

    /* --------------------------------------------------------------------------------------------
     * Embark the specified player entity into the managed vehicle entity.
    */
    void Embark(CPlayer & player, Int32 slot, bool allocate, bool warp) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the x axis of the managed vehicle entity.
    */
    Float32 GetPosX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the y axis of the managed vehicle entity.
    */
    Float32 GetPosY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the z axis of the managed vehicle entity.
    */
    Float32 GetPosZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed vehicle entity.
    */
    void SetPosX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed vehicle entity.
    */
    void SetPosY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed vehicle entity.
    */
    void SetPosZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the x axis of the managed vehicle entity.
    */
    Float32 GetRotX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the y axis of the managed vehicle entity.
    */
    Float32 GetRotY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the z axis of the managed vehicle entity.
    */
    Float32 GetRotZ() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation amount of the managed vehicle entity.
    */
    Float32 GetRotW() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the x axis of the managed vehicle entity.
    */
    void SetRotX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the y axis of the managed vehicle entity.
    */
    void SetRotY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the z axis of the managed vehicle entity.
    */
    void SetRotZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation amount of the managed vehicle entity.
    */
    void SetRotW(Float32 w) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation on the x axis of the managed vehicle entity.
    */
    Float32 GetERotX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation on the y axis of the managed vehicle entity.
    */
    Float32 GetERotY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation on the z axis of the managed vehicle entity.
    */
    Float32 GetERotZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler rotation on the x axis of the managed vehicle entity.
    */
    void SetERotX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler rotation on the y axis of the managed vehicle entity.
    */
    void SetERotY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the euler rotation on the z axis of the managed vehicle entity.
    */
    void SetERotZ(Float32 z) const;
};

} // Namespace:: SqMod

#endif // _ENTITY_VEHICLE_HPP_