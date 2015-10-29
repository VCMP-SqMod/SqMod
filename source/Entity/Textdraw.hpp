#ifndef _ENTITY_TEXTDRAW_HPP_
#define _ENTITY_TEXTDRAW_HPP_

// ------------------------------------------------------------------------------------------------
#include "Entity.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
class CTextdraw : public Reference< CTextdraw >
{
public:
    // --------------------------------------------------------------------------------------------
    using RefType::Reference;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void ShowAll() const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void ShowFor(const Reference< CPlayer > & player) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void ShowRange(SQInt32 first, SQInt32 last) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void HideAll() const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void HideFor(const Reference< CPlayer > & player) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void HideRange(SQInt32 first, SQInt32 last) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetPositionAll(const Vector2i & pos) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetPositionAllEx(SQInt32 x, SQInt32 y) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetPositionFor(const Reference< CPlayer > & player, const Vector2i & pos) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetPositionForEx(const Reference< CPlayer > & player, SQInt32 x, SQInt32 y) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetPositionRange(SQInt32 first, SQInt32 last, const Vector2i & pos) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetColorAll(const Color4 & col) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetColorAllEx(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetColorFor(const Reference< CPlayer > & player, const Color4 & col) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetColorForEx(const Reference< CPlayer > & player, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetColorRange(SQInt32 first, SQInt32 last, const Color4 & col) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    const SQChar * GetText() const noexcept;
};

} // Namespace:: SqMod

#endif // _ENTITY_TEXTDRAW_HPP_