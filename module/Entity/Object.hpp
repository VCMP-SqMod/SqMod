#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Circular locks employed by the object manager.
*/
enum ObjectCircularLocks
{
    OBJECTCL_EMIT_OBJECT_WORLD  = (1u << 0u),
    OBJECTCL_EMIT_OBJECT_ALPHA  = (2u << 0u),
    OBJECTCL_EMIT_OBJECT_REPORT = (3u << 0u)
};

/* ------------------------------------------------------------------------------------------------
 * Manages a single object entity.
*/
class CObject
{
    // --------------------------------------------------------------------------------------------
    friend class Core;
    friend class ObjectInst;

private:

    /* --------------------------------------------------------------------------------------------
     * Identifier of the managed entity.
    */
    int32_t       m_ID;

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
    uint32_t      m_CircularLocks;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit CObject(int32_t id);

public:

    /* --------------------------------------------------------------------------------------------
     * The default duration to use when moving the object.
    */
    uint32_t mMoveToDuration;
    uint32_t mMoveByDuration;

    /* --------------------------------------------------------------------------------------------
     * The default duration to use when rotating the object to Quaternion.
    */
    uint32_t mRotateToDuration;
    uint32_t mRotateByDuration;

    /* --------------------------------------------------------------------------------------------
     * The default duration to use when rotating the object to Euler.
    */
    uint32_t mRotateToEulerDuration;
    uint32_t mRotateByEulerDuration;

    /* --------------------------------------------------------------------------------------------
     * Maximum possible number that could represent an identifier for this entity type.
    */
    static const int32_t Max;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CObject(const CObject &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    CObject(CObject &&) = delete;

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
            STHROWF("Invalid object reference [{}]", m_Tag);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    SQMOD_NODISCARD const String & ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated null entity instance.
    */
    static SQInteger SqGetNull(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated null entity instance.
    */
    SQMOD_NODISCARD static LightObj & GetNull();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier of the entity managed by this instance.
    */
    SQMOD_NODISCARD int32_t GetID() const
    {
        return m_ID;
    }

    /* --------------------------------------------------------------------------------------------
     * Check whether this instance manages a valid entity.
    */
    SQMOD_NODISCARD bool IsActive() const
    {
        return VALID_ENTITY(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user tag.
    */
    SQMOD_NODISCARD const String & GetTag() const;

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
    SQMOD_NODISCARD LightObj & GetData();

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(LightObj & data);

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed object entity.
    */
    bool Destroy0() const // NOLINT(modernize-use-nodiscard)
    {
        return Destroy(0, NullLightObj());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed object entity.
    */
    bool Destroy1(int32_t header) const // NOLINT(modernize-use-nodiscard)
    {
        return Destroy(header, NullLightObj());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed object entity.
    */
    bool Destroy(int32_t header, LightObj & payload) const; // NOLINT(modernize-use-nodiscard)

    /* --------------------------------------------------------------------------------------------
     * Retrieve the events table of this entity.
    */
    SQMOD_NODISCARD LightObj & GetEvents() const;

    /* --------------------------------------------------------------------------------------------
     * Emit a custom event for the managed entity
    */
    void CustomEvent(int32_t header, LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * See if the managed object entity is streamed for the specified player.
    */
    SQMOD_NODISCARD bool IsStreamedFor(CPlayer & player) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the model of the managed object entity.
    */
    SQMOD_NODISCARD int32_t GetModel() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the managed object entity exists.
    */
    SQMOD_NODISCARD int32_t GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the world in which the managed object entity exists.
    */
    void SetWorld(int32_t world);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the alpha of the managed object entity.
    */
    SQMOD_NODISCARD int32_t GetAlpha() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the alpha of the managed object entity.
    */
    void SetAlpha(int32_t alpha);

    /* --------------------------------------------------------------------------------------------
     * Modify the alpha of the managed object entity over the specified time.
    */
    void SetAlphaEx(int32_t alpha, uint32_t time);

    /* --------------------------------------------------------------------------------------------
     * Move the managed object entity to the specified position over the specified time.
    */
    void MoveTo(const Vector3 & pos, uint32_t time) const;

    /* --------------------------------------------------------------------------------------------
     * Move the managed object entity to the specified position over the specified time.
    */
    void MoveToEx(float x, float y, float z, uint32_t time) const;

    /* --------------------------------------------------------------------------------------------
     * Move the managed object entity by the specified position over the specified time.
    */
    void MoveBy(const Vector3 & pos, uint32_t time) const;

    /* --------------------------------------------------------------------------------------------
     * Move the managed object entity by the specified position over the specified time.
    */
    void MoveByEx(float x, float y, float z, uint32_t time) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the managed object entity.
    */
    SQMOD_NODISCARD Vector3 GetPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed object entity.
    */
    void SetPosition(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed object entity.
    */
    void SetPositionEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity to the specified rotation over the specified time.
    */
    void RotateTo(const Quaternion & rot, uint32_t time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity to the specified rotation over the specified time.
    */
    void RotateToEx(float x, float y, float z, float w, uint32_t time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity to the specified Euler rotation over the specified time.
    */
    void RotateToEuler(const Vector3 & rot, uint32_t time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity to the specified Euler rotation over the specified time.
    */
    void RotateToEulerEx(float x, float y, float z, uint32_t time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity by the specified rotation over the specified time.
    */
    void RotateBy(const Quaternion & rot, uint32_t time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity by the specified rotation over the specified time.
    */
    void RotateByEx(float x, float y, float z, float w, uint32_t time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity by the specified Euler rotation over the specified time.
    */
    void RotateByEuler(const Vector3 & rot, uint32_t time) const;

    /* --------------------------------------------------------------------------------------------
     * Rotate the managed object entity by the specified Euler rotation over the specified time.
    */
    void RotateByEulerEx(float x, float y, float z, uint32_t time) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation of the managed object entity.
    */
    SQMOD_NODISCARD Quaternion GetRotation() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the Euler rotation of the managed object entity.
    */
    SQMOD_NODISCARD Vector3 GetRotationEuler() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the managed object entity reports gunshots.
    */
    SQMOD_NODISCARD bool GetShotReport() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed object entity reports gunshots.
    */
    void SetShotReport(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * See whether the managed object entity reports player bumps.
    */
    SQMOD_NODISCARD bool GetTouchedReport() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed object entity reports player bumps.
    */
    void SetTouchedReport(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the x axis of the managed object entity.
    */
    SQMOD_NODISCARD float GetPositionX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the y axis of the managed object entity.
    */
    SQMOD_NODISCARD float GetPositionY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the z axis of the managed object entity.
    */
    SQMOD_NODISCARD float GetPositionZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed object entity.
    */
    void SetPositionX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed object entity.
    */
    void SetPositionY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed object entity.
    */
    void SetPositionZ(float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the x axis of the managed object entity.
    */
    SQMOD_NODISCARD float GetRotationX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the y axis of the managed object entity.
    */
    SQMOD_NODISCARD float GetRotationY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation on the z axis of the managed object entity.
    */
    SQMOD_NODISCARD float GetRotationZ() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the rotation amount of the managed object entity.
    */
    SQMOD_NODISCARD float GetRotationW() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the Euler rotation on the x axis of the managed object entity.
    */
    SQMOD_NODISCARD float GetEulerRotationX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the Euler rotation on the y axis of the managed object entity.
    */
   SQMOD_NODISCARD float GetEulerRotationY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the Euler rotation on the z axis of the managed object entity.
    */
    SQMOD_NODISCARD float GetEulerRotationZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed object entity.
    */
    void MoveToX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed object entity.
    */
    void MoveToY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed object entity.
    */
    void MoveToZ(float z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed object entity.
    */
    void MoveByX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed object entity.
    */
    void MoveByY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed object entity.
    */
    void MoveByZ(float z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the x axis of the managed object entity.
    */
    void RotateToX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the y axis of the managed object entity.
    */
    void RotateToY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the z axis of the managed object entity.
    */
    void RotateToZ(float z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the w axis of the managed object entity.
    */
    void RotateToW(float w) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the x axis of the managed object entity.
    */
    void RotateByX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the y axis of the managed object entity.
    */
    void RotateByY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the z axis of the managed object entity.
    */
    void RotateByZ(float z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the w axis of the managed object entity.
    */
    void RotateByW(float w) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the x axis of the managed object entity.
    */
    void RotateToEulerX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the y axis of the managed object entity.
    */
    void RotateToEulerY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the z axis of the managed object entity.
    */
    void RotateToEulerZ(float z) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the x axis of the managed object entity.
    */
    void RotateByEulerX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the y axis of the managed object entity.
    */
    void RotateByEulerY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the rotation on the z axis of the managed object entity.
    */
    void RotateByEulerZ(float z) const;
#ifdef VCMP_ENABLE_OFFICIAL
    /* --------------------------------------------------------------------------------------------
     * Retrieve legacy object instance for this entity.
    */
    LightObj & GetLegacyObject() const;
#endif
};

} // Namespace:: SqMod
