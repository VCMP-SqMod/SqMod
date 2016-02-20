#ifndef _ENTITY_PICKUP_HPP_
#define _ENTITY_PICKUP_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages Pickup instances.
*/
class CPickup
{
    // --------------------------------------------------------------------------------------------
    friend class Core;

private:

    // --------------------------------------------------------------------------------------------
    static Vector3      s_Vector3;

    /* --------------------------------------------------------------------------------------------
     * Cached identifiers for fast integer to string conversion.
    */
    static SQChar s_StrID[SQMOD_PICKUP_POOL][8];

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
    CPickup(Int32 id);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CPickup(const CPickup &);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    CPickup & operator = (const CPickup &);

public:

    // --------------------------------------------------------------------------------------------
    static const Int32 Max;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CPickup();

    /* --------------------------------------------------------------------------------------------
     * See whether this instance manages a valid entity.
    */
    bool Validate() const
    {
        if (VALID_ENTITY(m_ID))
            return true;
        SqThrow("Invalid pickup reference [%s]", m_Tag.c_str());
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const CPickup & o) const;

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
    Int32 GetMaxID() const { return SQMOD_PICKUP_POOL; }

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
    bool IsStreamedFor(CPlayer & player) const;
    Int32 GetModel() const;
    Int32 GetWorld() const;
    void SetWorld(Int32 world) const;
    Int32 GetAlpha() const;
    void SetAlpha(Int32 alpha) const;
    bool GetAutomatic() const;
    void SetAutomatic(bool toggle) const;
    Int32 GetAutoTimer() const;
    void SetAutoTimer(Int32 timer) const;
    void Refresh() const;
    const Vector3 & GetPosition();
    void SetPosition(const Vector3 & pos) const;
    void SetPositionEx(Float32 x, Float32 y, Float32 z) const;
    Int32 GetQuantity() const;

    // --------------------------------------------------------------------------------------------
    Float32 GetPosX() const;
    Float32 GetPosY() const;
    Float32 GetPosZ() const;
    void SetPosX(Float32 x) const;
    void SetPosY(Float32 y) const;
    void SetPosZ(Float32 z) const;
};

} // Namespace:: SqMod

#endif // _ENTITY_PICKUP_HPP_