#ifndef _ENTITY_VEHICLE_HPP_
#define _ENTITY_VEHICLE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages Vehicle instances.
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
     * Cached identifiers for fast integer to string conversion.
    */
    static SQChar s_StrID[SQMOD_VEHICLE_POOL][8];

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
    CVehicle(Int32 id);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CVehicle(const CVehicle &);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    CVehicle & operator = (const CVehicle &);

public:

    // --------------------------------------------------------------------------------------------
    static const Int32 Max;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CVehicle();

    /* --------------------------------------------------------------------------------------------
     * See whether this instance manages a valid entity.
    */
    bool Validate() const
    {
        if (VALID_ENTITY(m_ID))
            return true;
        SqThrow("Invalid vehicle reference [%s]", m_Tag.c_str());
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const CVehicle & o) const;

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
    Int32 GetMaxID() const { return SQMOD_VEHICLE_POOL; }

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
    bool Destroy(Int32 header, Object & payload);
    bool Destroy() { return Destroy(0, NullObject()); }
    bool Destroy(Int32 header) { return Destroy(header, NullObject()); }

    // --------------------------------------------------------------------------------------------
    bool BindEvent(Int32 evid, Object & env, Function & func) const;

    // --------------------------------------------------------------------------------------------
    bool IsStreamedFor(CPlayer & player) const;
    Int32 GetSyncSource() const;
    Int32 GetSyncType() const;
    Int32 GetWorld() const;
    void SetWorld(Int32 world) const;
    Int32 GetModel() const;
    Object & GetOccupant(Int32 slot) const;
    Int32 GetOccupantID(Int32 slot) const;
    void Respawn() const;
    Int32 GetImmunity() const;
    void SetImmunity(Int32 flags) const;
    bool IsWrecked() const;
    const Vector3 & GetPosition() const;
    void SetPosition(const Vector3 & pos) const;
    void SetPositionEx(const Vector3 & pos, bool empty) const;
    void SetPositionEx(Float32 x, Float32 y, Float32 z) const;
    void SetPositionEx(Float32 x, Float32 y, Float32 z, bool empty) const;
    const Quaternion & GetRotation() const;
    void SetRotation(const Quaternion & rot) const;
    void SetRotationEx(Float32 x, Float32 y, Float32 z, Float32 w) const;
    const Vector3 & GetRotationEuler() const;
    void SetRotationEuler(const Vector3 & rot) const;
    void SetRotationEulerEx(Float32 x, Float32 y, Float32 z) const;
    const Vector3 & GetSpeed() const;
    void SetSpeed(const Vector3 & vel) const;
    void SetSpeedEx(Float32 x, Float32 y, Float32 z) const;
    void AddSpeed(const Vector3 & vel) const;
    void AddSpeedEx(Float32 x, Float32 y, Float32 z) const;
    const Vector3 & GetRelSpeed() const;
    void SetRelSpeed(const Vector3 & vel) const;
    void SetRelSpeedEx(Float32 x, Float32 y, Float32 z) const;
    void AddRelSpeed(const Vector3 & vel) const;
    void AddRelSpeedEx(Float32 x, Float32 y, Float32 z) const;
    const Vector3 & GetTurnSpeed() const;
    void SetTurnSpeed(const Vector3 & vel) const;
    void SetTurnSpeedEx(Float32 x, Float32 y, Float32 z) const;
    void AddTurnSpeed(const Vector3 & vel) const;
    void AddTurnSpeedEx(Float32 x, Float32 y, Float32 z) const;
    const Vector3 & GetRelTurnSpeed() const;
    void SetRelTurnSpeed(const Vector3 & vel) const;
    void SetRelTurnSpeedEx(Float32 x, Float32 y, Float32 z) const;
    void AddRelTurnSpeed(const Vector3 & vel) const;
    void AddRelTurnSpeedEx(Float32 x, Float32 y, Float32 z) const;
    const Vector4 & GetSpawnPosition() const;
    void SetSpawnPosition(const Vector4 & pos) const;
    void SetSpawnPositionEx(Float32 x, Float32 y, Float32 z, Float32 w) const;
    const Quaternion & GetSpawnRotation() const;
    void SetSpawnRotation(const Quaternion & rot) const;
    void SetSpawnRotationEx(Float32 x, Float32 y, Float32 z, Float32 w) const;
    const Vector3 & GetSpawnRotationEuler() const;
    void SetSpawnRotationEuler(const Vector3 & rot) const;
    void SetSpawnRotationEulerEx(Float32 x, Float32 y, Float32 z) const;
    Uint32 GetRespawnTimer() const;
    void SetRespawnTimer(Uint32 timer) const;
    Float32 GetHealth() const;
    void SetHealth(Float32 amount) const;
    Int32 GetPrimaryColor() const;
    void SetPrimaryColor(Int32 col) const;
    Int32 GetSecondaryColor() const;
    void SetSecondaryColor(Int32 col) const;
    void SetColors(Int32 primary, Int32 secondary) const;
    bool GetLocked() const;
    void SetLocked(bool toggle) const;
    Int32 GetPartStatus(Int32 part) const;
    void SetPartStatus(Int32 part, Int32 status) const;
    Int32 GetTyreStatus(Int32 tyre) const;
    void SetTyreStatus(Int32 tyre, Int32 status) const;
    Uint32 GetDamageData() const;
    void SetDamageData(Uint32 data) const;
    bool GetAlarm() const;
    void SetAlarm(bool toggle) const;
    bool GetLights() const;
    void SetLights(bool toggle) const;
    Int32 GetRadio() const;
    void SetRadio(Int32 radio) const;
    bool GetRadioLocked() const;
    void SetRadioLocked(bool toggle) const;
    bool GetGhostState() const;
    void SetGhostState(bool toggle) const;
    void ResetHandling() const;
    void ResetHandling(Int32 rule) const;
    bool ExistsHandling(Int32 rule) const;
    Float32 GetHandlingData(Int32 rule) const;
    void SetHandlingData(Int32 rule, Float32 data) const;
    void Embark(CPlayer & player) const;
    void Embark(CPlayer & player, Int32 slot, bool allocate, bool warp) const;

    // --------------------------------------------------------------------------------------------
    Float32 GetPosX() const;
    Float32 GetPosY() const;
    Float32 GetPosZ() const;
    void SetPosX(Float32 x) const;
    void SetPosY(Float32 y) const;
    void SetPosZ(Float32 z) const;

    // --------------------------------------------------------------------------------------------
    Float32 GetRotX() const;
    Float32 GetRotY() const;
    Float32 GetRotZ() const;
    Float32 GetRotW() const;
    void SetRotX(Float32 x) const;
    void SetRotY(Float32 y) const;
    void SetRotZ(Float32 z) const;
    void SetRotW(Float32 w) const;

    // --------------------------------------------------------------------------------------------
    Float32 GetERotX() const;
    Float32 GetERotY() const;
    Float32 GetERotZ() const;
    void SetERotX(Float32 x) const;
    void SetERotY(Float32 y) const;
    void SetERotZ(Float32 z) const;
};

} // Namespace:: SqMod

#endif // _ENTITY_VEHICLE_HPP_