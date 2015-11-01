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
    CObject(const Reference< CObject > & o);

    /* --------------------------------------------------------------------------------------------
     * See if the referenced object instance is streamed for the specified player.
    */
    bool IsStreamedFor(const Reference< CPlayer > & player) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the model of the referenced object instance.
    */
    const CModel & GetModel() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the model identifier of the referenced object instance.
    */
    SQInt32 GetModelID() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the referenced object instance exists.
    */
    SQInt32 GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Change the world in which the referenced object instance exists.
    */
    void SetWorld(SQInt32 world) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the alpha of the referenced object instance.
    */
    SQInt32 GetAlpha() const;

    /* --------------------------------------------------------------------------------------------
     * Change the alpha of the referenced object instance.
    */
    void SetAlpha(SQInt32 alpha) const;

    /* --------------------------------------------------------------------------------------------
     * Change the alpha of the referenced object instance over the specified time.
    */
    void SetAlphaEx(SQInt32 alpha, SQInt32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Move the referenced object instance to the specified position instantly.
    */
    void MoveToPr(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Move the referenced object instance to the specified position over the specified time.
    */
    void MoveTo(const Vector3 & pos, SQInt32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Move the referenced object instance to the specified position instantly.
    */
    void MoveToEx(SQFloat x, SQFloat y, SQFloat z) const;

    /* --------------------------------------------------------------------------------------------
     * Move the referenced object instance to the specified position over the specified time.
    */
    void MoveToEx(SQFloat x, SQFloat y, SQFloat z, SQInt32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Move the referenced object instance by the specified position instantly.
    */
    void MoveByPr(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Move the referenced object instance by the specified position over the specified time.
    */
    void MoveBy(const Vector3 & pos, SQInt32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Move the referenced object instance by the specified position instantly.
    */
    void MoveByEx(SQFloat x, SQFloat y, SQFloat z) const;

    /* --------------------------------------------------------------------------------------------
     * Move the referenced object instance by the specified position over the specified time.
    */
    void MoveByEx(SQFloat x, SQFloat y, SQFloat z, SQInt32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the referenced object instance.
    */
    const Vector3 & GetPosition();

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced object instance.
    */
    void SetPosition(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced object instance.
    */
    void SetPositionEx(SQFloat x, SQFloat y, SQFloat z) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance to the specified rotation instantly.
    */
    void RotateToPr(const Quaternion & rot) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance to the specified rotation over the specified time.
    */
    void RotateTo(const Quaternion & rot, SQInt32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance to the specified rotation instantly.
    */
    void RotateToEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance to the specified rotation over the specified time.
    */
    void RotateToEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w, SQInt32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance to the specified euler rotation instantly.
    */
    void RotateToEulerPr(const Vector3 & rot) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance to the specified euler rotation over the specified time.
    */
    void RotateToEuler(const Vector3 & rot, SQInt32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance to the specified euler rotation instantly.
    */
    void RotateToEulerEx(SQFloat x, SQFloat y, SQFloat z) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance to the specified euler rotation over the specified time.
    */
    void RotateToEulerEx(SQFloat x, SQFloat y, SQFloat z, SQInt32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance by the specified rotation instantly.
    */
    void RotateByPr(const Quaternion & rot) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance by the specified rotation over the specified time.
    */
    void RotateBy(const Quaternion & rot, SQInt32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance by the specified rotation instantly.
    */
    void RotateByEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance by the specified rotation over the specified time.
    */
    void RotateByEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w, SQInt32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance by the specified euler rotation instantly.
    */
    void RotateByEulerPr(const Vector3 & rot) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance by the specified euler rotation over the specified time.
    */
    void RotateByEuler(const Vector3 & rot, SQInt32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance by the specified euler rotation instantly.
    */
    void RotateByEulerEx(SQFloat x, SQFloat y, SQFloat z) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the referenced object instance by the specified euler rotation over the specified time.
    */
    void RotateByEulerEx(SQFloat x, SQFloat y, SQFloat z, SQInt32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation of the referenced object instance.
    */
    const Quaternion & GetRotation();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation of the referenced object instance.
    */
    const Vector3 & GetRotationEuler();

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced object instance reports gunshots.
    */
    bool GetShotReport() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced object instance reports gunshots.
    */
    void SetShotReport(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced object instance reports player bumps.
    */
    bool GetBumpReport() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced object instance reports player bumps.
    */
    void SetBumpReport(bool toggle) const;
};

} // Namespace:: SqMod

#endif // _ENTITY_OBJECT_HPP_