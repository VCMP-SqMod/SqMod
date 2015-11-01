#ifndef _ENTITY_BLIP_HPP_
#define _ENTITY_BLIP_HPP_

// ------------------------------------------------------------------------------------------------
#include "Entity.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class responsible for managing the referenced blip instance.
*/
class CBlip : public Reference< CBlip >
{
public:

    /* --------------------------------------------------------------------------------------------
     * Import the constructors, destructors and assignment operators from the base class.
    */
    using RefType::Reference;

    /* --------------------------------------------------------------------------------------------
     * Construct a reference from a base reference.
    */
    CBlip(const Reference< CBlip > & o);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the referenced blip instance exists.
    */
    SQInteger GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the scale of the referenced blip instance.
    */
    SQInteger GetScale() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the referenced blip instance.
    */
    const Vector3 & GetPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the x axis position of the referenced blip instance.
    */
    SQFloat GetPositionX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the y axis position of the referenced blip instance.
    */
    SQFloat GetPositionY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the z axis position of the referenced blip instance.
    */
    SQFloat GetPositionZ() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the color of the referenced blip instance.
    */
    const Color4 & GetColor() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the sprite identifier of the referenced blip instance.
    */
    SQInt32 GetSprID() const;
};

} // Namespace:: SqMod

#endif // _ENTITY_BLIP_HPP_
