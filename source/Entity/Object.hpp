#ifndef _ENTITY_OBJECT_HPP_
#define _ENTITY_OBJECT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages a single object entity.
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
     * Identifier of the managed entity.
    */
    Int32   m_ID;

    /* --------------------------------------------------------------------------------------------
     * User tag associated with this instance.
    */
    String  m_Tag;

    /* --------------------------------------------------------------------------------------------
     * User data associated with this instance.
    */
    Object  m_Data;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    CObject(Int32 id);

public:

    /* --------------------------------------------------------------------------------------------
     * Maximum possible number that could represent an identifier for this entity type.
    */
    static const Int32 Max;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CObject(const CObject &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    CObject(CObject &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CObject();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    CObject & operator = (const CObject &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    CObject & operator = (CObject &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * See whether this instance manages a valid entity instance otherwise throw an exception.
    */
    void Validate() const
    {
        if (INVALID_ENTITY(m_ID))
        {
            SqThrowF("Invalid object reference [%s]", m_Tag.c_str());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const CObject & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    const String & ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier of the entity managed by this instance.
    */
    Int32 GetID() const
    {
        return m_ID;
    }

    /* --------------------------------------------------------------------------------------------
     * Check whether this instance manages a valid entity.
    */
    bool IsActive() const
    {
        return VALID_ENTITY(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user tag.
    */
    const String & GetTag() const;

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

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed object entity.
    */
    bool Destroy()
    {
        return Destroy(0, NullObject());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed object entity.
    */
    bool Destroy(Int32 header)
    {
        return Destroy(header, NullObject());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed object entity.
    */
    bool Destroy(Int32 header, Object & payload);

    /* --------------------------------------------------------------------------------------------
     * Bind to an event supported by this entity type.
    */
    void BindEvent(Int32 evid, Object & env, Function & func) const;

    /* --------------------------------------------------------------------------------------------
     * See if the managed object entity is streamed for the specified player.
    */
    bool IsStreamedFor(CPlayer & player) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the model of the managed object entity.
    */
    Int32 GetModel() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the managed object entity exists.
    */
    Int32 GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the world in which the managed object entity exists.
    */
    void SetWorld(Int32 world) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the alpha of the managed object entity.
    */
    Int32 GetAlpha() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the alpha of the managed object entity.
    */
    void SetAlpha(Int32 alpha) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the alpha of the managed object entity over the specified time.
    */
    void SetAlphaEx(Int32 alpha, Int32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Move the managed object entity to the specified position over the specified time.
    */
    void MoveTo(const Vector3 & pos, Int32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Move the managed object entity to the specified position over the specified time.
    */
    void MoveToEx(Float32 x, Float32 y, Float32 z, Int32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Move the managed object entity by the specified position over the specified time.
    */
    void MoveBy(const Vector3 & pos, Int32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Move the managed object entity by the specified position over the specified time.
    */
    void MoveByEx(Float32 x, Float32 y, Float32 z, Int32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the managed object entity.
    */
    const Vector3 & GetPosition();

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed object entity.
    */
    void SetPosition(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed object entity.
    */
    void SetPositionEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity to the specified rotation over the specified time.
    */
    void RotateTo(const Quaternion & rot, Int32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity to the specified rotation over the specified time.
    */
    void RotateToEx(Float32 x, Float32 y, Float32 z, Float32 w, Int32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity to the specified euler rotation over the specified time.
    */
    void RotateToEuler(const Vector3 & rot, Int32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity to the specified euler rotation over the specified time.
    */
    void RotateToEulerEx(Float32 x, Float32 y, Float32 z, Int32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity by the specified rotation over the specified time.
    */
    void RotateBy(const Quaternion & rot, Int32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity by the specified rotation over the specified time.
    */
    void RotateByEx(Float32 x, Float32 y, Float32 z, Float32 w, Int32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity by the specified euler rotation over the specified time.
    */
    void RotateByEuler(const Vector3 & rot, Int32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity by the specified euler rotation over the specified time.
    */
    void RotateByEulerEx(Float32 x, Float32 y, Float32 z, Int32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation of the managed object entity.
    */
    const Quaternion & GetRotation();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation of the managed object entity.
    */
    const Vector3 & GetRotationEuler();

    /* --------------------------------------------------------------------------------------------
     * See whether the managed object entity reports gunshots.
    */
    bool GetShotReport() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed object entity reports gunshots.
    */
    void SetShotReport(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed object entity reports player bumps.
    */
    bool GetBumpReport() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed object entity reports player bumps.
    */
    void SetBumpReport(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the x axis of the managed object entity.
    */
    Float32 GetPosX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the y axis of the managed object entity.
    */
    Float32 GetPosY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the z axis of the managed object entity.
    */
    Float32 GetPosZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed object entity.
    */
    void SetPosX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed object entity.
    */
    void SetPosY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed object entity.
    */
    void SetPosZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the x axis of the managed object entity.
    */
    Float32 GetRotX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the y axis of the managed object entity.
    */
    Float32 GetRotY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the z axis of the managed object entity.
    */
    Float32 GetRotZ() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation amount of the managed object entity.
    */
    Float32 GetRotW() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation on the x axis of the managed object entity.
    */
    Float32 GetERotX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation on the y axis of the managed object entity.
    */
    Float32 GetERotY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the euler rotation on the z axis of the managed object entity.
    */
    Float32 GetERotZ() const;
};

} // Namespace:: SqMod

#endif // _ENTITY_OBJECT_HPP_