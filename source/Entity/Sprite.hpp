#ifndef _ENTITY_SPRITE_HPP_
#define _ENTITY_SPRITE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages Sprite instances.
*/
class CSprite
{
    // --------------------------------------------------------------------------------------------
    friend class Core;

private:

    /* --------------------------------------------------------------------------------------------
     * Cached identifiers for fast integer to string conversion.
    */
    static SQChar s_StrID[SQMOD_SPRITE_POOL][8];

    /* --------------------------------------------------------------------------------------------
     * Identifier of the managed entity.
    */
    Int32   m_ID;

    /* --------------------------------------------------------------------------------------------
     * User tag and data associated with this instance.
    */
    String  m_Tag;
    Object  m_Data;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    CSprite(Int32 id);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CSprite(const CSprite &);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    CSprite & operator = (const CSprite &);

public:

    // --------------------------------------------------------------------------------------------
    static const Int32 Max;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CSprite();

    /* --------------------------------------------------------------------------------------------
     * See whether this instance manages a valid entity.
    */
    bool Validate() const
    {
        if (VALID_ENTITY(m_ID))
            return true;
        SqThrow("Invalid sprite reference [%s]", m_Tag.c_str());
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const CSprite & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier of the entity managed by this instance.
    */
    Int32 GetID() const { return m_ID; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the maximum possible identifier to an entity of this type.
    */
    Int32 GetMaxID() const { return SQMOD_SPRITE_POOL; }

    /* --------------------------------------------------------------------------------------------
     * Check whether this instance manages a valid entity.
    */
    bool IsActive() const { return VALID_ENTITY(m_ID); }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user tag.
    */
    CSStr GetTag() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    void SetTag(CSStr tag);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user data.
    */
    Object & GetData();

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(Object & data);

    // --------------------------------------------------------------------------------------------
    bool Destroy(Int32 header, Object & payload);
    bool Destroy() { return Destroy(0, NullObject()); }
    bool Destroy(Int32 header) { return Destroy(header, NullObject()); }

    // --------------------------------------------------------------------------------------------
    bool BindEvent(Int32 evid, Object & env, Function & func) const;

    // --------------------------------------------------------------------------------------------
    void ShowAll() const;
    void ShowFor(CPlayer & player) const;
    void ShowRange(Int32 first, Int32 last) const;
    void HideAll() const;
    void HideFor(CPlayer & player) const;
    void HideRange(Int32 first, Int32 last) const;
    void SetPositionAll(const Vector2i & pos) const;
    void SetPositionAllEx(Int32 x, Int32 y) const;
    void SetPositionFor(CPlayer & player, const Vector2i & pos) const;
    void SetPositionForEx(CPlayer & player, Int32 x, Int32 y) const;
    void SetPositionRange(Int32 first, Int32 last, const Vector2i & pos) const;
    void SetCenterAll(const Vector2i & pos) const;
    void SetCenterAllEx(Int32 x, Int32 y) const;
    void SetCenterFor(CPlayer & player, const Vector2i & pos) const;
    void SetCenterForEx(CPlayer & player, Int32 x, Int32 y) const;
    void SetCenterRange(Int32 first, Int32 last, const Vector2i & pos) const;
    void SetRotationAll(SQFloat rot) const;
    void SetRotationFor(CPlayer & player, SQFloat rot) const;
    void SetRotationRange(Int32 first, Int32 last, SQFloat rot) const;
    void SetAlphaAll(Uint8 alpha) const;
    void SetAlphaFor(CPlayer & player, Uint8 alpha) const;
    void SetAlphaRange(Int32 first, Int32 last, Uint8 alpha) const;
    CSStr GetFilePath() const;
};

} // Namespace:: SqMod

#endif // _ENTITY_SPRITE_HPP_