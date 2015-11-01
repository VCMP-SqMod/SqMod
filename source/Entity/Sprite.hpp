#ifndef _ENTITY_SPRITE_HPP_
#define _ENTITY_SPRITE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Entity.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class responsible for managing the referenced sprite instance.
*/
class CSprite : public Reference< CSprite >
{
public:

    /* --------------------------------------------------------------------------------------------
     * Import the constructors, destructors and assignment operators from the base class.
    */
    using RefType::Reference;

    /* --------------------------------------------------------------------------------------------
     * Construct a reference from a base reference.
    */
    CSprite(const Reference< CSprite > & o);

    /* --------------------------------------------------------------------------------------------
     * Show the referenced sprite instance to all players on the server.
    */
    void ShowAll() const;

    /* --------------------------------------------------------------------------------------------
     * Show the referenced sprite instance to the specified player instance.
    */
    void ShowFor(const Reference< CPlayer > & player) const;

    /* --------------------------------------------------------------------------------------------
     * Show the referenced sprite instance to all players in the specified range.
    */
    void ShowRange(SQInt32 first, SQInt32 last) const;

    /* --------------------------------------------------------------------------------------------
     * Hide the referenced sprite instance from all players on the server.
    */
    void HideAll() const;

    /* --------------------------------------------------------------------------------------------
     * Hide the referenced sprite instance from the specified player instance.
    */
    void HideFor(const Reference< CPlayer > & player) const;

    /* --------------------------------------------------------------------------------------------
     * Hide the referenced sprite instance from all players in the specified range.
    */
    void HideRange(SQInt32 first, SQInt32 last) const;

    /* --------------------------------------------------------------------------------------------
     * Set the position of the referenced sprite instance for all players on the server.
    */
    void SetPositionAll(const Vector2i & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Set the position of the referenced sprite instance for all players on the server.
    */
    void SetPositionAllEx(SQInt32 x, SQInt32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Set the position of the referenced sprite instance for the specified player instance.
    */
    void SetPositionFor(const Reference< CPlayer > & player, const Vector2i & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Set the position of the referenced sprite instance for the specified player instance.
    */
    void SetPositionForEx(const Reference< CPlayer > & player, SQInt32 x, SQInt32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Set the position of the referenced sprite instance for all players in the specified range.
    */
    void SetPositionRange(SQInt32 first, SQInt32 last, const Vector2i & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Set the center of the referenced sprite instance for all players on the server.
    */
    void SetCenterAll(const Vector2i & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Set the center of the referenced sprite instance for all players on the server.
    */
    void SetCenterAllEx(SQInt32 x, SQInt32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Set the center of the referenced sprite instance for the specified player instance.
    */
    void SetCenterFor(const Reference< CPlayer > & player, const Vector2i & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Set the center of the referenced sprite instance for the specified player instance.
    */
    void SetCenterForEx(const Reference< CPlayer > & player, SQInt32 x, SQInt32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Set the center of the referenced sprite instance for all players in the specified range.
    */
    void SetCenterRange(SQInt32 first, SQInt32 last, const Vector2i & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Set the rotation of the referenced sprite instance for all players on the server.
    */
    void SetRotationAll(SQFloat rot) const;

    /* --------------------------------------------------------------------------------------------
     * Set the rotation of the referenced sprite instance for the specified player instance.
    */
    void SetRotationFor(const Reference< CPlayer > & player, SQFloat rot) const;

    /* --------------------------------------------------------------------------------------------
     * Set the rotation of the referenced sprite instance for all players in the specified range.
    */
    void SetRotationRange(SQInt32 first, SQInt32 last, SQFloat rot) const;

    /* --------------------------------------------------------------------------------------------
     * Set the rotation of the referenced sprite instance for all players on the server.
    */
    void SetAlphaAll(Uint8 alpha) const;

    /* --------------------------------------------------------------------------------------------
     * Set the rotation of the referenced sprite instance for the specified player instance.
    */
    void SetAlphaFor(const Reference< CPlayer > & player, Uint8 alpha) const;

    /* --------------------------------------------------------------------------------------------
     * Set the rotation of the referenced sprite instance for all players in the specified range.
    */
    void SetAlphaRange(SQInt32 first, SQInt32 last, Uint8 alpha) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the file path of the texture used by the referenced sprite instance.
    */
    const SQChar * GetFilePath() const;
};

} // Namespace:: SqMod

#endif // _ENTITY_SPRITE_HPP_