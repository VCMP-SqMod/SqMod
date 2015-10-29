#ifndef _ENTITY_SPRITE_HPP_
#define _ENTITY_SPRITE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Entity.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
class CSprite : public Reference< CSprite >
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
    void SetPositionAll(const Vector2u & pos) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetPositionAllEx(SQUint32 x, SQUint32 y) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetPositionFor(const Reference< CPlayer > & player, const Vector2u & pos) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetPositionForEx(const Reference< CPlayer > & player, SQUint32 x, SQUint32 y) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetPositionRange(SQInt32 first, SQInt32 last, const Vector2u & pos) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetCenterAll(const Vector2u & pos) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetCenterAllEx(SQUint32 x, SQUint32 y) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetCenterFor(const Reference< CPlayer > & player, const Vector2u & pos) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetCenterForEx(const Reference< CPlayer > & player, SQUint32 x, SQUint32 y) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetCenterRange(SQInt32 first, SQInt32 last, const Vector2u & pos) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetRotationAll(SQFloat rot) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetRotationFor(const Reference< CPlayer > & player, SQFloat rot) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetRotationRange(SQInt32 first, SQInt32 last, SQFloat rot) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetAlphaAll(Uint8 alpha) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetAlphaFor(const Reference< CPlayer > & player, Uint8 alpha) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetAlphaRange(SQInt32 first, SQInt32 last, Uint8 alpha) const noexcept;

    /* --------------------------------------------------------------------------------------------
     *
    */
    const SQChar * GetFilePath() const noexcept;
};

} // Namespace:: SqMod

#endif // _ENTITY_SPRITE_HPP_