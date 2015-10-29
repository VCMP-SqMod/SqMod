#ifndef _ENTITY_CHECKPOINT_HPP_
#define _ENTITY_CHECKPOINT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Entity.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class responsible for managing the referenced checkpoint instance.
*/
class CCheckpoint : public Reference< CCheckpoint >
{
    // --------------------------------------------------------------------------------------------
    static Color4   s_Color4;
    static Vector3  s_Vector3;

    // --------------------------------------------------------------------------------------------
    static SQUint32 s_ColorR, s_ColorG, s_ColorB, s_ColorA;

public:

    /* --------------------------------------------------------------------------------------------
     * Import the constructors, destructors and assignment operators from the base class.
    */
    using RefType::Reference;

    /* --------------------------------------------------------------------------------------------
     * See if the referenced checkpoint instance is streamed for the specified player.
    */
    bool IsStreamedFor(const Reference< CPlayer > & player) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the referenced checkpoint instance exists.
    */
    SQInt32 GetWorld() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the world in which the referenced checkpoint instance exists.
    */
    void SetWorld(SQInt32 world) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the color of the referenced checkpoint instance.
    */
    const Color4 & GetColor() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the color of the referenced checkpoint instance.
    */
    void SetColor(const Color4 & col) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the color of the referenced checkpoint instance.
    */
    void SetColorEx(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the referenced checkpoint instance.
    */
    const Vector3 & GetPosition() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced checkpoint instance.
    */
    void SetPosition(const Vector3 & pos) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced checkpoint instance.
    */
    void SetPositionEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the radius of the referenced checkpoint instance.
    */
    SQFloat GetRadius() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the radius of the referenced checkpoint instance.
    */
    void SetRadius(SQFloat radius) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the owner of the referenced checkpoint instance.
    */
    Reference< CPlayer > GetOwner() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the owner identifier of the referenced checkpoint instance.
    */
    SQInt32 GetOwnerID() const noexcept;
};

} // Namespace:: SqMod

#endif // _ENTITY_CHECKPOINT_HPP_