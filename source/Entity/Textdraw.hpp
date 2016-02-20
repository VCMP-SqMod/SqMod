#ifndef _ENTITY_TEXTDRAW_HPP_
#define _ENTITY_TEXTDRAW_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages Textdraw instances.
*/
class CTextdraw
{
    // --------------------------------------------------------------------------------------------
    friend class Core;

private:

    /* --------------------------------------------------------------------------------------------
     * Cached identifiers for fast integer to string conversion.
    */
    static SQChar s_StrID[SQMOD_TEXTDRAW_POOL][8];

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
    CTextdraw(Int32 id);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CTextdraw(const CTextdraw &);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    CTextdraw & operator = (const CTextdraw &);

public:

    // --------------------------------------------------------------------------------------------
    static const Int32 Max;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CTextdraw();

    /* --------------------------------------------------------------------------------------------
     * See whether this instance manages a valid entity.
    */
    bool Validate() const
    {
        if (VALID_ENTITY(m_ID))
            return true;
        SqThrow("Invalid textdraw reference [%s]", m_Tag.c_str());
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const CTextdraw & o) const;

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
    Int32 GetMaxID() const { return SQMOD_TEXTDRAW_POOL; }

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
    void SetColorAll(const Color4 & col) const;
    void SetColorAllEx(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const;
    void SetColorFor(CPlayer & player, const Color4 & col) const;
    void SetColorForEx(CPlayer & player, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const;
    void SetColorRange(Int32 first, Int32 last, const Color4 & col) const;
    CSStr GetText() const;
};

} // Namespace:: SqMod

#endif // _ENTITY_TEXTDRAW_HPP_