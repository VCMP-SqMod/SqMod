#ifndef _ENTITY_VEHICLE_HPP_
#define _ENTITY_VEHICLE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Entity.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class responsible for managing the referenced vehicle instance.
*/
class CVehicle : public Reference< CVehicle >
{
    // --------------------------------------------------------------------------------------------
    static CAutomobile  s_Automobile;

    // --------------------------------------------------------------------------------------------
    static Vector3      s_Vector3;
    static Vector4      s_Vector4;
    static Quaternion   s_Quaternion;

public:

    /* --------------------------------------------------------------------------------------------
     * Import the constructors, destructors and assignment operators from the base class.
    */
    using RefType::Reference;

    /* --------------------------------------------------------------------------------------------
     * See if the referenced vehicle instance is streamed for the specified player.
    */
    bool IsStreamedFor(const Reference< CPlayer > & player) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the synchronization source of the referenced vehicle instance.
    */
    SQInt32 GetSyncSource() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the synchronization type of the referenced vehicle instance.
    */
    SQInt32 GetSyncType() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the referenced vehicle instance exists.
    */
    SQInt32 GetWorld() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the world in which the referenced vehicle instance exists.
    */
    void SetWorld(SQInt32 world) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle model of the referenced vehicle instance.
    */
    const CAutomobile & GetModel() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle model id of the referenced vehicle instance.
    */
    SQInt32 GetModelID() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the slot occupant from the referenced vehicle instance.
    */
    Reference< CPlayer > GetOccupant(SQInt32 slot) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the slot occupant identifier from the referenced vehicle instance.
    */
    SQInt32 GetOccupantID(SQInt32 slot) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Respawn the referenced vehicle instance.
    */
    void Respawn() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the immunity flags of the referenced vehicle instance.
    */
    SQInt32 GetImmunity() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the immunity flags of the referenced vehicle instance.
    */
    void SetImmunity(SQInt32 flags) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced vehicle instance is wrecked.
    */
    bool IsWrecked() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the referenced vehicle instance.
    */
    const Vector3 & GetPosition() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced vehicle instance.
    */
    void SetPosition(const Vector3 & pos) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced vehicle instance.
    */
    void SetPositionEx(const Vector3 & pos, bool empty) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced vehicle instance.
    */
    void SetPositionEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced vehicle instance.
    */
    void SetPositionEx(SQFloat x, SQFloat y, SQFloat z, bool empty) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation of the referenced vehicle instance.
    */
    const Quaternion & GetRotation() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the rotation of the referenced vehicle instance.
    */
    void SetRotation(const Quaternion & rot) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the rotation of the referenced vehicle instance.
    */
    void SetRotationEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation of the referenced vehicle instance.
    */
    const Vector3 & GetRotationEuler() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the euler rotation of the referenced vehicle instance.
    */
    void SetRotationEuler(const Vector3 & rot) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the euler rotation of the referenced vehicle instance.
    */
    void SetRotationEulerEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the speed of the referenced vehicle instance.
    */
    const Vector3 & GetSpeed() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the speed of the referenced vehicle instance.
    */
    void SetSpeed(const Vector3 & vel) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the speed of the referenced vehicle instance.
    */
    void SetSpeedEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the speed of the referenced vehicle instance.
    */
    void AddSpeed(const Vector3 & vel) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the speed of the referenced vehicle instance.
    */
    void AddSpeedEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative speed of the referenced vehicle instance.
    */
    const Vector3 & GetRelSpeed() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the relative speed of the referenced vehicle instance.
    */
    void SetRelSpeed(const Vector3 & vel) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the relative speed of the referenced vehicle instance.
    */
    void SetRelSpeedEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the relative speed of the referenced vehicle instance.
    */
    void AddRelSpeed(const Vector3 & vel) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the relative speed of the referenced vehicle instance.
    */
    void AddRelSpeedEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the turn speed of the referenced vehicle instance.
    */
    const Vector3 & GetTurnSpeed() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the turn speed of the referenced vehicle instance.
    */
    void SetTurnSpeed(const Vector3 & vel) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the turn speed of the referenced vehicle instance.
    */
    void SetTurnSpeedEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the turn speed of the referenced vehicle instance.
    */
    void AddTurnSpeed(const Vector3 & vel) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the turn speed of the referenced vehicle instance.
    */
    void AddTurnSpeedEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the relative turn speed of the referenced vehicle instance.
    */
    const Vector3 & GetRelTurnSpeed() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the relative turn speed of the referenced vehicle instance.
    */
    void SetRelTurnSpeed(const Vector3 & vel) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the relative turn speed of the referenced vehicle instance.
    */
    void SetRelTurnSpeedEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the relative turn speed of the referenced vehicle instance.
    */
    void AddRelTurnSpeed(const Vector3 & vel) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the relative turn speed of the referenced vehicle instance.
    */
    void AddRelTurnSpeedEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the spawn position of the referenced vehicle instance.
    */
    const Vector4 & GetSpawnPosition() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the spawn position of the referenced vehicle instance.
    */
    void SetSpawnPosition(const Vector4 & pos) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the spawn position of the referenced vehicle instance.
    */
    void SetSpawnPositionEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the spawn rotation of the referenced vehicle instance.
    */
    const Quaternion & GetSpawnRotation() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the spawn rotation of the referenced vehicle instance.
    */
    void SetSpawnRotation(const Quaternion & rot) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the spawn rotation of the referenced vehicle instance.
    */
    void SetSpawnRotationEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler spawn rotation of the referenced vehicle instance.
    */
    const Vector3 & GetSpawnRotationEuler() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the euler spawn rotation of the referenced vehicle instance.
    */
    void SetSpawnRotationEuler(const Vector3 & rot) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the euler spawn rotation of the referenced vehicle instance.
    */
    void SetSpawnRotationEulerEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the respawn timer of the referenced vehicle instance.
    */
    SQUint32 GetRespawnTimer() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the respawn timer of the referenced vehicle instance.
    */
    void SetRespawnTimer(SQUint32 timer) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the health of the referenced vehicle instance.
    */
    SQFloat GetHealth() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the health of the referenced vehicle instance.
    */
    void SetHealth(SQFloat amount) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the primary color of the referenced vehicle instance.
    */
    SQInt32 GetPrimaryColor() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the primary color of the referenced vehicle instance.
    */
    void SetPrimaryColor(SQInt32 col) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the secondary color of the referenced vehicle instance.
    */
    SQInt32 GetSecondaryColor() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the secondary color of the referenced vehicle instance.
    */
    void SetSecondaryColor(SQInt32 col) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the primary and secondary colors of the referenced vehicle instance.
    */
    void SetColors(SQInt32 primary, SQInt32 secondary) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced vehicle instance is locked.
    */
    bool IsLocked() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced vehicle instance is locked.
    */
    void SetLocked(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the part status of the referenced vehicle instance.
    */
    SQInt32 GetPartStatus(SQInt32 part) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the part status of the referenced vehicle instance.
    */
    void SetPartStatus(SQInt32 part, SQInt32 status) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the tyre status of the referenced vehicle instance.
    */
    SQInt32 GetTyreStatus(SQInt32 tyre) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the tyre status of the referenced vehicle instance.
    */
    void SetTyreStatus(SQInt32 tyre, SQInt32 status) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the damage data of the referenced vehicle instance.
    */
    SQUint32 GetDamageData() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the damage data of the referenced vehicle instance.
    */
    void SetDamageData(SQUint32 data) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced vehicle instance has alarm.
    */
    bool HasAlarm() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced vehicle instance has alarm.
    */
    void SetAlarm(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced vehicle instance has lights.
    */
    bool HasLights() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced vehicle instance has lights.
    */
    void SetLights(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the radio of the referenced vehicle instance.
    */
    SQInt32 GetRadio() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the radio of the referenced vehicle instance.
    */
    void SetRadio(SQInt32 radio) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced vehicle instance has radio locked.
    */
    bool IsRadioLocked() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced vehicle instance has radio locked.
    */
    void SetRadioLocked(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced vehicle instance is in ghost state.
    */
    bool IsGhostState() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced vehicle instance is in ghost state.
    */
    void SetGhostState(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Reset all the handling rules for the referenced vehicle instance.
    */
    void ResetHandling() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Reset the specified handling rule for the referenced vehicle instance.
    */
    void ResetHandling(SQInt32 rule) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the specified handling ruleexists in the referenced vehicle instance.
    */
    bool ExistsHandling(SQInt32 rule) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the handling data of the referenced vehicle instance.
    */
    SQFloat GetHandlingData(SQInt32 rule) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the handling data of the referenced vehicle instance.
    */
    void SetHandlingData(SQInt32 rule, SQFloat data) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Embark the specified player instance into the referenced vehicle instance.
    */
    void Embark(const Reference< CPlayer > & player) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Embark the specified player instance into the referenced vehicle instance.
    */
    void Embark(const Reference< CPlayer > & player, SQInt32 slot, bool allocate, bool warp) const noexcept;
};

} // Namespace:: SqMod

#endif // _ENTITY_VEHICLE_HPP_
