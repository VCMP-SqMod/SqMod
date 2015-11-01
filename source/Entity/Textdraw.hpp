#ifndef _ENTITY_TEXTDRAW_HPP_
#define _ENTITY_TEXTDRAW_HPP_

// ------------------------------------------------------------------------------------------------
#include "Entity.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class responsible for managing the referenced textdraw instance.
*/
class CTextdraw : public Reference< CTextdraw >
{
public:

    /* --------------------------------------------------------------------------------------------
     * Import the constructors, destructors and assignment operators from the base class.
    */
    using RefType::Reference;

    /* --------------------------------------------------------------------------------------------
     * Construct a reference from a base reference.
    */
    CTextdraw(const Reference< CTextdraw > & o);

    /* --------------------------------------------------------------------------------------------
     * Show the referenced textdraw instance to all players on the server.
    */
    void ShowAll() const;

    /* --------------------------------------------------------------------------------------------
     * Show the referenced textdraw instance to the specified player instance.
    */
    void ShowFor(const Reference< CPlayer > & player) const;

    /* --------------------------------------------------------------------------------------------
     * Show the referenced textdraw instance to all players in the specified range.
    */
    void ShowRange(SQInt32 first, SQInt32 last) const;

    /* --------------------------------------------------------------------------------------------
     * Hide the referenced textdraw instance from all players on the server.
    */
    void HideAll() const;

    /* --------------------------------------------------------------------------------------------
     * Hide the referenced textdraw instance from the specified player instance.
    */
    void HideFor(const Reference< CPlayer > & player) const;

    /* --------------------------------------------------------------------------------------------
     * Hide the referenced textdraw instance from all players in the specified range.
    */
    void HideRange(SQInt32 first, SQInt32 last) const;

    /* --------------------------------------------------------------------------------------------
     * Set the position of the referenced textdraw instance for all players on the server.
    */
    void SetPositionAll(const Vector2i & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Set the position of the referenced textdraw instance for all players on the server.
    */
    void SetPositionAllEx(SQInt32 x, SQInt32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Set the position of the referenced textdraw instance for the specified player instance.
    */
    void SetPositionFor(const Reference< CPlayer > & player, const Vector2i & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Set the position of the referenced textdraw instance for the specified player instance.
    */
    void SetPositionForEx(const Reference< CPlayer > & player, SQInt32 x, SQInt32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Set the position of the referenced textdraw instance for all players in the specified range.
    */
    void SetPositionRange(SQInt32 first, SQInt32 last, const Vector2i & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Set the center of the referenced textdraw instance for all players on the server.
    */
    void SetColorAll(const Color4 & col) const;

    /* --------------------------------------------------------------------------------------------
     * Set the color of the referenced textdraw instance for all players on the server.
    */
    void SetColorAllEx(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const;

    /* --------------------------------------------------------------------------------------------
     * Set the color of the referenced textdraw instance for the specified player instance.
    */
    void SetColorFor(const Reference< CPlayer > & player, const Color4 & col) const;

    /* --------------------------------------------------------------------------------------------
     * Set the color of the referenced textdraw instance for the specified player instance.
    */
    void SetColorForEx(const Reference< CPlayer > & player, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const;

    /* --------------------------------------------------------------------------------------------
     * Set the color of the referenced textdraw instance for all players in the specified range.
    */
    void SetColorRange(SQInt32 first, SQInt32 last, const Color4 & col) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the text string used by the referenced textdraw instance.
    */
    const SQChar * GetText() const;
};

} // Namespace:: SqMod

#endif // _ENTITY_TEXTDRAW_HPP_