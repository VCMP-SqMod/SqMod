#ifndef _ENTITY_PICKUP_HPP_
#define _ENTITY_PICKUP_HPP_

// ------------------------------------------------------------------------------------------------
#include "Entity.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class responsible for managing the referenced pickup instance.
*/
class CPickup : public Reference< CPickup >
{
    // --------------------------------------------------------------------------------------------
    static CModel       s_Model;

    // --------------------------------------------------------------------------------------------
    static Vector3      s_Vector3;

public:

    /* --------------------------------------------------------------------------------------------
     * Import the constructors, destructors and assignment operators from the base class.
    */
    using RefType::Reference;

    /* --------------------------------------------------------------------------------------------
     * Construct a reference from a base reference.
    */
    CPickup(const Reference< CPickup > & o);

    /* --------------------------------------------------------------------------------------------
     * See if the referenced pickup instance is streamed for the specified player.
    */
    bool IsStreamedFor(const Reference< CPlayer > & player) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the model of the referenced pickup instance.
    */
    const CModel & GetModel() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the model identifier of the referenced pickup instance.
    */
    SQInt32 GetModelID() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the referenced pickup instance exists.
    */
    SQInt32 GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Change the world in which the referenced pickup instance exists.
    */
    void SetWorld(SQInt32 world) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the alpha of the referenced pickup instance.
    */
    SQInt32 GetAlpha() const;

    /* --------------------------------------------------------------------------------------------
     * Change the alpha of the referenced pickup instance.
    */
    void SetAlpha(SQInt32 alpha) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced pickup instance is automatic.
    */
    bool GetAutomatic() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced pickup instance is automatic.
    */
    void SetAutomatic(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the automatic timer of the referenced pickup instance.
    */
    SQInt32 GetAutoTimer() const;

    /* --------------------------------------------------------------------------------------------
     * Change the automatic timer of the referenced pickup instance.
    */
    void SetAutoTimer(SQInt32 timer) const;

    /* --------------------------------------------------------------------------------------------
     * Refresh the referenced pickup instance.
    */
    void Refresh() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the referenced pickup instance.
    */
    const Vector3 & GetPosition();

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced pickup instance.
    */
    void SetPosition(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced pickup instance.
    */
    void SetPositionEx(SQFloat x, SQFloat y, SQFloat z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the quantity of the referenced pickup instance.
    */
    SQInt32 GetQuantity() const;
};

} // Namespace:: SqMod

#endif // _ENTITY_PICKUP_HPP_