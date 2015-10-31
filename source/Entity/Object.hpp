#ifndef _ENTITY_OBJECT_HPP_
#define _ENTITY_OBJECT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Entity.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class responsible for managing the referenced object instance.
*/
class CObject : public Reference< CObject >
{
    // --------------------------------------------------------------------------------------------
    static CModel       s_Model;

    // --------------------------------------------------------------------------------------------
    static Vector3      s_Vector3;
    static Quaternion   s_Quaternion;

public:

    /* --------------------------------------------------------------------------------------------
     * Import the constructors, destructors and assignment operators from the base class.
    */
    using RefType::Reference;

    /* --------------------------------------------------------------------------------------------
     * Construct a reference from a base reference.
    */
    CObject(const Reference< CObject > & o) noexcept;

    /* --------------------------------------------------------------------------------------------
     * See if the referenced object instance is streamed for the specified player.
    */
    bool IsStreamedFor(const Reference< CPlayer > & player) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the model of the referenced object instance.
    */
    const CModel & GetModel() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the model identifier of the referenced object instance.
    */
    SQInt32 GetModelID() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the referenced object instance exists.
    */
    SQInt32 GetWorld() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the world in which the referenced object instance exists.
    */
    void SetWorld(SQInt32 world) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the alpha of the referenced object instance.
    */
    SQInt32 GetAlpha() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the alpha of the referenced object instance.
    */
    void SetAlpha(SQInt32 alpha) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the alpha of the referenced object instance over the specified time.
    */
    void SetAlphaEx(SQInt32 alpha, SQInt32 time) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Move the referenced object instance to the specified position instantly.
    */
    void MoveToPr(const Vector3 & pos) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Move the referenced object instance to the specified position over the specified time.
    */
    void MoveTo(const Vector3 & pos, SQInt32 time) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Move the referenced object instance to the specified position instantly.
    */
    void MoveToEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Move the referenced object instance to the specified position over the specified time.
    */
    void MoveToEx(SQFloat x, SQFloat y, SQFloat z, SQInt32 time) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Move the referenced object instance by the specified position instantly.
    */
    void MoveByPr(const Vector3 & pos) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Move the referenced object instance by the specified position over the specified time.
    */
    void MoveBy(const Vector3 & pos, SQInt32 time) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Move the referenced object instance by the specified position instantly.
    */
    void MoveByEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Move the referenced object instance by the specified position over the specified time.
    */
    void MoveByEx(SQFloat x, SQFloat y, SQFloat z, SQInt32 time) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the referenced object instance.
    */
    const Vector3 & GetPosition() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced object instance.
    */
    void SetPosition(const Vector3 & pos) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced object instance.
    */
    void SetPositionEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance to the specified rotation instantly.
    */
    void RotateToPr(const Quaternion & rot) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance to the specified rotation over the specified time.
    */
    void RotateTo(const Quaternion & rot, SQInt32 time) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance to the specified rotation instantly.
    */
    void RotateToEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance to the specified rotation over the specified time.
    */
    void RotateToEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w, SQInt32 time) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance to the specified euler rotation instantly.
    */
    void RotateToEulerPr(const Vector3 & rot) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance to the specified euler rotation over the specified time.
    */
    void RotateToEuler(const Vector3 & rot, SQInt32 time) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance to the specified euler rotation instantly.
    */
    void RotateToEulerEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance to the specified euler rotation over the specified time.
    */
    void RotateToEulerEx(SQFloat x, SQFloat y, SQFloat z, SQInt32 time) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance by the specified rotation instantly.
    */
    void RotateByPr(const Quaternion & rot) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance by the specified rotation over the specified time.
    */
    void RotateBy(const Quaternion & rot, SQInt32 time) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance by the specified rotation instantly.
    */
    void RotateByEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance by the specified rotation over the specified time.
    */
    void RotateByEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w, SQInt32 time) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance by the specified euler rotation instantly.
    */
    void RotateByEulerPr(const Vector3 & rot) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance by the specified euler rotation over the specified time.
    */
    void RotateByEuler(const Vector3 & rot, SQInt32 time) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance by the specified euler rotation instantly.
    */
    void RotateByEulerEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance by the specified euler rotation over the specified time.
    */
    void RotateByEulerEx(SQFloat x, SQFloat y, SQFloat z, SQInt32 time) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation of the referenced object instance.
    */
    const Quaternion & GetRotation() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation of the referenced object instance.
    */
    const Vector3 & GetRotationEuler() noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced object instance reports gunshots.
    */
    bool GetShotReport() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced object instance reports gunshots.
    */
    void SetShotReport(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced object instance reports player bumps.
    */
    bool GetBumpReport() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced object instance reports player bumps.
    */
    void SetBumpReport(bool toggle) const noexcept;
};

} // Namespace:: SqMod

#endif // _ENTITY_OBJECT_HPP_