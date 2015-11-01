#ifndef _ENTITY_SPHERE_HPP_
#define _ENTITY_SPHERE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Entity.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class responsible for managing the referenced sphere instance.
*/
class CSphere : public Reference< CSphere >
{
    // --------------------------------------------------------------------------------------------
    static Color3   s_Color3;
    static Vector3  s_Vector3;

    // --------------------------------------------------------------------------------------------
    static SQUint32 s_ColorR, s_ColorG, s_ColorB;

public:

    /* --------------------------------------------------------------------------------------------
     * Import the constructors, destructors and assignment operators from the base class.
    */
    using RefType::Reference;

    /* --------------------------------------------------------------------------------------------
     * Construct a reference from a base reference.
    */
    CSphere(const Reference< CSphere > & o);

    /* --------------------------------------------------------------------------------------------
     * See if the referenced sphere instance is streamed for the specified player.
    */
    bool IsStreamedFor(const Reference< CPlayer > & player) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the referenced sphere instance exists.
    */
    SQInt32 GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Change the world in which the referenced sphere instance exists.
    */
    void SetWorld(SQInt32 world) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the color of the referenced sphere instance.
    */
    const Color3 & GetColor() const;

    /* --------------------------------------------------------------------------------------------
     * Change the color of the referenced sphere instance.
    */
    void SetColor(const Color3 & col) const;

    /* --------------------------------------------------------------------------------------------
     * Change the color of the referenced sphere instance.
    */
    void SetColorEx(Uint8 r, Uint8 g, Uint8 b) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the referenced sphere instance.
    */
    const Vector3 & GetPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced sphere instance.
    */
    void SetPosition(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced sphere instance.
    */
    void SetPositionEx(SQFloat x, SQFloat y, SQFloat z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the radius of the referenced sphere instance.
    */
    SQFloat GetRadius() const;

    /* --------------------------------------------------------------------------------------------
     * Change the radius of the referenced sphere instance.
    */
    void SetRadius(SQFloat radius) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the owner of the referenced sphere instance.
    */
    Reference< CPlayer > GetOwner() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the owner identifier of the referenced sphere instance.
    */
    SQInt32 GetOwnerID() const;
};

} // Namespace:: SqMod

#endif // _ENTITY_SPHERE_HPP_