#ifndef _ENTITY_OBJECT_HPP_
#define _ENTITY_OBJECT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages Object instances.
*/
class CObject
{
    // --------------------------------------------------------------------------------------------
    friend class Core;

private:

    // --------------------------------------------------------------------------------------------
    static Vector3      s_Vector3;
    static Quaternion   s_Quaternion;

    /* --------------------------------------------------------------------------------------------
     * Cached identifiers for fast integer to string conversion.
    */
    static SQChar s_StrID[SQMOD_OBJECT_POOL][8];

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
    CObject(Int32 id);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CObject(const CObject &);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    CObject & operator = (const CObject &);

public:

    // --------------------------------------------------------------------------------------------
    static const Int32 Max;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CObject();

    /* --------------------------------------------------------------------------------------------
     * See whether this instance manages a valid entity.
    */
    bool Validate() const
    {
        if (VALID_ENTITY(m_ID))
            return true;
        SqThrow("Invalid object reference [%s]", m_Tag.c_str());
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const CObject & o) const;

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
    Int32 GetMaxID() const { return SQMOD_OBJECT_POOL; }

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
    void SetAlphaEx(Int32 alpha, Int32 time) const;
    void MoveTo(const Vector3 & pos, Int32 time) const;
    void MoveToEx(Float32 x, Float32 y, Float32 z, Int32 time) const;
    void MoveBy(const Vector3 & pos, Int32 time) const;
    void MoveByEx(Float32 x, Float32 y, Float32 z, Int32 time) const;
    const Vector3 & GetPosition();
    void SetPosition(const Vector3 & pos) const;
    void SetPositionEx(Float32 x, Float32 y, Float32 z) const;
    void RotateTo(const Quaternion & rot, Int32 time) const;
    void RotateToEx(Float32 x, Float32 y, Float32 z, Float32 w, Int32 time) const;
    void RotateToEuler(const Vector3 & rot, Int32 time) const;
    void RotateToEulerEx(Float32 x, Float32 y, Float32 z, Int32 time) const;
    void RotateBy(const Quaternion & rot, Int32 time) const;
    void RotateByEx(Float32 x, Float32 y, Float32 z, Float32 w, Int32 time) const;
    void RotateByEuler(const Vector3 & rot, Int32 time) const;
    void RotateByEulerEx(Float32 x, Float32 y, Float32 z, Int32 time) const;
    const Quaternion & GetRotation();
    const Vector3 & GetRotationEuler();
    bool GetShotReport() const;
    void SetShotReport(bool toggle) const;
    bool GetBumpReport() const;
    void SetBumpReport(bool toggle) const;

    // --------------------------------------------------------------------------------------------
    Float32 GetPosX() const;
    Float32 GetPosY() const;
    Float32 GetPosZ() const;
    void SetPosX(Float32 x) const;
    void SetPosY(Float32 y) const;
    void SetPosZ(Float32 z) const;

    // --------------------------------------------------------------------------------------------
    Float32 GetRotX() const;
    Float32 GetRotY() const;
    Float32 GetRotZ() const;
    Float32 GetRotW() const;
    Float32 GetERotX() const;
    Float32 GetERotY() const;
    Float32 GetERotZ() const;
};

} // Namespace:: SqMod

#endif // _ENTITY_OBJECT_HPP_