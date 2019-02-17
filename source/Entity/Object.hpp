#ifndef _ENTITY_OBJECT_HPP_
#define _ENTITY_OBJECT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Circular locks employed by the object manager.
*/
enum ObjectCircularLocks
{
    OBJECTCL_EMIT_OBJECT_WORLD  = (1 << 0),
    OBJECTCL_EMIT_OBJECT_ALPHA  = (2 << 0),
    OBJECTCL_EMIT_OBJECT_REPORT = (3 << 0)
};

/* ------------------------------------------------------------------------------------------------
 * Manages a single object entity.
*/
class CObject
{
    // --------------------------------------------------------------------------------------------
    friend class Core;

private:

    /* --------------------------------------------------------------------------------------------
     * Identifier of the managed entity.
    */
    Int32       m_ID;

    /* --------------------------------------------------------------------------------------------
     * User tag associated with this instance.
    */
    String      m_Tag;

    /* --------------------------------------------------------------------------------------------
     * User data associated with this instance.
    */
    LightObj    m_Data;

    /* --------------------------------------------------------------------------------------------
     * Prevent events from triggering themselves.
    */
    Uint32      m_CircularLocks;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    CObject(Int32 id);

public:

    /* --------------------------------------------------------------------------------------------
     * The default duration to use when moving the object.
    */
    Uint32 mMoveToDuration;
    Uint32 mMoveByDuration;

    /* --------------------------------------------------------------------------------------------
     * The default duration to use when rotating the object to Quaternion.
    */
    Uint32 mRotateToDuration;
    Uint32 mRotateByDuration;

    /* --------------------------------------------------------------------------------------------
     * The default duration to use when rotating the object to Euler.
    */
    Uint32 mRotateToEulerDuration;
    Uint32 mRotateByEulerDuration;

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
            STHROWF("Invalid object reference [%s]", m_Tag.c_str());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    const String & ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated null entity instance.
    */
    static SQInteger SqGetNull(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated null entity instance.
    */
    static LightObj & GetNull();

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
    void SetTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    CObject & ApplyTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user data.
    */
    LightObj & GetData();

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(LightObj & data);

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed object entity.
    */
    bool Destroy()
    {
        return Destroy(0, NullLightObj());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed object entity.
    */
    bool Destroy(Int32 header)
    {
        return Destroy(header, NullLightObj());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed object entity.
    */
    bool Destroy(Int32 header, LightObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the events table of this entity.
    */
    LightObj & GetEvents() const;

    /* --------------------------------------------------------------------------------------------
     * Emit a custom event for the managed entity
    */
    void CustomEvent(Int32 header, LightObj & payload) const;

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
    void SetWorld(Int32 world);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the alpha of the managed object entity.
    */
    Int32 GetAlpha() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the alpha of the managed object entity.
    */
    void SetAlpha(Int32 alpha);

    /* --------------------------------------------------------------------------------------------
     * Modify the alpha of the managed object entity over the specified time.
    */
    void SetAlphaEx(Int32 alpha, Uint32 time);

    /* --------------------------------------------------------------------------------------------
     * Move the managed object entity to the specified position over the specified time.
    */
    void MoveTo(const Vector3 & pos, Uint32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Move the managed object entity to the specified position over the specified time.
    */
    void MoveToEx(Float32 x, Float32 y, Float32 z, Uint32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Move the managed object entity by the specified position over the specified time.
    */
    void MoveBy(const Vector3 & pos, Uint32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Move the managed object entity by the specified position over the specified time.
    */
    void MoveByEx(Float32 x, Float32 y, Float32 z, Uint32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the managed object entity.
    */
    Vector3 GetPosition();

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
    void RotateTo(const Quaternion & rot, Uint32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity to the specified rotation over the specified time.
    */
    void RotateToEx(Float32 x, Float32 y, Float32 z, Float32 w, Uint32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity to the specified Euler rotation over the specified time.
    */
    void RotateToEuler(const Vector3 & rot, Uint32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity to the specified Euler rotation over the specified time.
    */
    void RotateToEulerEx(Float32 x, Float32 y, Float32 z, Uint32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity by the specified rotation over the specified time.
    */
    void RotateBy(const Quaternion & rot, Uint32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity by the specified rotation over the specified time.
    */
    void RotateByEx(Float32 x, Float32 y, Float32 z, Float32 w, Uint32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity by the specified Euler rotation over the specified time.
    */
    void RotateByEuler(const Vector3 & rot, Uint32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity by the specified Euler rotation over the specified time.
    */
    void RotateByEulerEx(Float32 x, Float32 y, Float32 z, Uint32 time) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation of the managed object entity.
    */
    Quaternion GetRotation();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the Euler rotation of the managed object entity.
    */
    Vector3 GetRotationEuler();

    /* --------------------------------------------------------------------------------------------
     * See whether the managed object entity reports gunshots.
    */
    bool GetShotReport() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed object entity reports gunshots.
    */
    void SetShotReport(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * See whether the managed object entity reports player bumps.
    */
    bool GetTouchedReport() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed object entity reports player bumps.
    */
    void SetTouchedReport(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the x axis of the managed object entity.
    */
    Float32 GetPositionX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the y axis of the managed object entity.
    */
    Float32 GetPositionY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the z axis of the managed object entity.
    */
    Float32 GetPositionZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed object entity.
    */
    void SetPositionX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed object entity.
    */
    void SetPositionY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed object entity.
    */
    void SetPositionZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the x axis of the managed object entity.
    */
    Float32 GetRotationX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the y axis of the managed object entity.
    */
    Float32 GetRotationY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the z axis of the managed object entity.
    */
    Float32 GetRotationZ() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation amount of the managed object entity.
    */
    Float32 GetRotationW() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the Euler rotation on the x axis of the managed object entity.
    */
    Float32 GetEulerRotationX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the Euler rotation on the y axis of the managed object entity.
    */
    Float32 GetEulerRotationY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the Euler rotation on the z axis of the managed object entity.
    */
    Float32 GetEulerRotationZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed object entity.
    */
    void MoveToX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed object entity.
    */
    void MoveToY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed object entity.
    */
    void MoveToZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed object entity.
    */
    void MoveByX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed object entity.
    */
    void MoveByY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed object entity.
    */
    void MoveByZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the x axis of the managed object entity.
    */
    void RotateToX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the y axis of the managed object entity.
    */
    void RotateToY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the z axis of the managed object entity.
    */
    void RotateToZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the w axis of the managed object entity.
    */
    void RotateToW(Float32 w) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the x axis of the managed object entity.
    */
    void RotateByX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the y axis of the managed object entity.
    */
    void RotateByY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the z axis of the managed object entity.
    */
    void RotateByZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the w axis of the managed object entity.
    */
    void RotateByW(Float32 w) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the x axis of the managed object entity.
    */
    void RotateToEulerX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the y axis of the managed object entity.
    */
    void RotateToEulerY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the z axis of the managed object entity.
    */
    void RotateToEulerZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the x axis of the managed object entity.
    */
    void RotateByEulerX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the y axis of the managed object entity.
    */
    void RotateByEulerY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the z axis of the managed object entity.
    */
    void RotateByEulerZ(Float32 z) const;
};

} // Namespace:: SqMod

#endif // _ENTITY_OBJECT_HPP_
