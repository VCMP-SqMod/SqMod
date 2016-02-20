#ifndef _ENTITY_KEYBIND_HPP_
#define _ENTITY_KEYBIND_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages Keybind instances.
*/
class CKeybind
{
    // --------------------------------------------------------------------------------------------
    friend class Core;

private:

    /* --------------------------------------------------------------------------------------------
     * Cached identifiers for fast integer to string conversion.
    */
    static SQChar s_StrID[SQMOD_KEYBIND_POOL][8];

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
    CKeybind(Int32 id);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CKeybind(const CKeybind &);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    CKeybind & operator = (const CKeybind &);

public:

    // --------------------------------------------------------------------------------------------
    static const Int32 Max;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CKeybind();

    /* --------------------------------------------------------------------------------------------
     * See whether this instance manages a valid entity.
    */
    bool Validate() const
    {
        if (VALID_ENTITY(m_ID))
            return true;
        SqThrow("Invalid keybind reference [%s]", m_Tag.c_str());
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const CKeybind & o) const;

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
    Int32 GetMaxID() const { return SQMOD_KEYBIND_POOL; }

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
    Int32 GetPrimary() const;
    Int32 GetSecondary() const;
    Int32 GetAlternative() const;
    bool IsRelease() const;
};

} // Namespace:: SqMod

#endif // _ENTITY_KEYBIND_HPP_