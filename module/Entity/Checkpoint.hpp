#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Circular locks employed by the checkpoint manager.
*/
enum CheckpointCircularLocks
{
    CHECKPOINTCL_EMIT_CHECKPOINT_WORLD  = (1u << 0u),
    CHECKPOINTCL_EMIT_CHECKPOINT_RADIUS = (1u << 1u)
};

/* ------------------------------------------------------------------------------------------------
 * Manages a single checkpoint entity.
*/
class CCheckpoint
{
    // --------------------------------------------------------------------------------------------
    friend class Core;
    friend class CheckpointInst;

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
    explicit CCheckpoint(int32_t id);

public:

    /* --------------------------------------------------------------------------------------------
     * Maximum possible number that could represent an identifier for this entity type.
    */
    static const int32_t Max;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CCheckpoint(const CCheckpoint &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    CCheckpoint(CCheckpoint &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    CCheckpoint & operator = (const CCheckpoint &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    CCheckpoint & operator = (CCheckpoint &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * See whether this instance manages a valid entity otherwise throw an exception.
    */
    void Validate() const
    {
        if (INVALID_ENTITY(m_ID))
        {
            STHROWF("Invalid checkpoint reference [%s]", m_Tag.c_str());
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
    CCheckpoint & ApplyTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user data.
    */
    SQMOD_NODISCARD LightObj & GetData();

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(LightObj & data);

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed checkpoint entity.
    */
    bool Destroy0() const // NOLINT(modernize-use-nodiscard)
    {
        return Destroy(0, NullLightObj());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed checkpoint entity.
    */
    bool Destroy1(int32_t header) const // NOLINT(modernize-use-nodiscard)
    {
        return Destroy(header, NullLightObj());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed checkpoint entity.
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
     * See if the managed checkpoint entity is streamed for the specified player.
    */
    SQMOD_NODISCARD bool IsStreamedFor(CPlayer & player) const;

    /* --------------------------------------------------------------------------------------------
     * See if the managed checkpoint entity of sphere type.
    */
    SQMOD_NODISCARD bool IsSphere() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the managed checkpoint entity exists.
    */
    SQMOD_NODISCARD int32_t GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the world in which the managed checkpoint entity exists.
    */
    void SetWorld(int32_t world);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the color of the managed checkpoint entity.
    */
    SQMOD_NODISCARD Color4 GetColor() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the color of the managed checkpoint entity.
    */
    void SetColor(const Color4 & col) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the color of the managed checkpoint entity.
    */
    void SetColorEx3(uint8_t r, uint8_t g, uint8_t b) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the color of the managed checkpoint entity.
    */
    void SetColorEx4(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the managed checkpoint entity.
    */
    SQMOD_NODISCARD Vector3 GetPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed checkpoint entity.
    */
    void SetPosition(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed checkpoint entity.
    */
    void SetPositionEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the radius of the managed checkpoint entity.
    */
    SQMOD_NODISCARD float GetRadius() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the radius of the managed checkpoint entity.
    */
    void SetRadius(float radius);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the owner of the managed checkpoint entity.
    */
    SQMOD_NODISCARD LightObj & GetOwner() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the owner identifier of the managed checkpoint entity.
    */
    SQMOD_NODISCARD int32_t GetOwnerID() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the x axis of the managed checkpoint entity.
    */
    SQMOD_NODISCARD float GetPositionX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the y axis of the managed checkpoint entity.
    */
    SQMOD_NODISCARD float GetPositionY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the z axis of the managed checkpoint entity.
    */
    SQMOD_NODISCARD float GetPositionZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed checkpoint entity.
    */
    void SetPositionX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed checkpoint entity.
    */
    void SetPositionY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed checkpoint entity.
    */
    void SetPositionZ(float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the red color of the managed checkpoint entity.
    */
    SQMOD_NODISCARD int32_t GetColorR() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the green color of the managed checkpoint entity.
    */
    SQMOD_NODISCARD int32_t GetColorG() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the blue color of the managed checkpoint entity.
    */
    SQMOD_NODISCARD int32_t GetColorB() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the alpha transparency of the managed checkpoint entity.
    */
    SQMOD_NODISCARD int32_t GetColorA() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the red color of the managed checkpoint entity.
    */
    void SetColorR(int32_t r) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the green color of the managed checkpoint entity.
    */
    void SetColorG(int32_t g) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the blue color of the managed checkpoint entity.
    */
    void SetColorB(int32_t b) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the alpha transparency of the managed checkpoint entity.
    */
    void SetColorA(int32_t a) const;
};

} // Namespace:: SqMod
