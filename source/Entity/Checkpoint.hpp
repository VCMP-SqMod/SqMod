#ifndef _ENTITY_CHECKPOINT_HPP_
#define _ENTITY_CHECKPOINT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Circular locks employed by the checkpoint manager.
*/
enum CheckpointCircularLocks
{
    CHECKPOINTCL_EMIT_CHECKPOINT_WORLD  = (1 << 0),
    CHECKPOINTCL_EMIT_CHECKPOINT_RADIUS = (2 << 0)
};

/* ------------------------------------------------------------------------------------------------
 * Manages a single checkpoint entity.
*/
class CCheckpoint
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
    CCheckpoint(Int32 id);

public:

    /* --------------------------------------------------------------------------------------------
     * Maximum possible number that could represent an identifier for this entity type.
    */
    static const Int32 Max;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CCheckpoint(const CCheckpoint &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    CCheckpoint(CCheckpoint &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CCheckpoint();

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
    void SetTag(const StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    CCheckpoint & ApplyTag(const StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user data.
    */
    LightObj & GetData();

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(LightObj & data);

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed checkpoint entity.
    */
    bool Destroy()
    {
        return Destroy(0, NullLightObj());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed checkpoint entity.
    */
    bool Destroy(Int32 header)
    {
        return Destroy(header, NullLightObj());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed checkpoint entity.
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
     * See if the managed checkpoint entity is streamed for the specified player.
    */
    bool IsStreamedFor(CPlayer & player) const;

    /* --------------------------------------------------------------------------------------------
     * See if the managed checkpoint entity of sphere type.
    */
    bool IsSphere() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the managed checkpoint entity exists.
    */
    Int32 GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the world in which the managed checkpoint entity exists.
    */
    void SetWorld(Int32 world);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the color of the managed checkpoint entity.
    */
    Color4 GetColor() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the color of the managed checkpoint entity.
    */
    void SetColor(const Color4 & col) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the color of the managed checkpoint entity.
    */
    void SetColorEx(Uint8 r, Uint8 g, Uint8 b) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the color of the managed checkpoint entity.
    */
    void SetColorEx(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the managed checkpoint entity.
    */
    Vector3 GetPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed checkpoint entity.
    */
    void SetPosition(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed checkpoint entity.
    */
    void SetPositionEx(Float32 x, Float32 y, Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the radius of the managed checkpoint entity.
    */
    Float32 GetRadius() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the radius of the managed checkpoint entity.
    */
    void SetRadius(Float32 radius);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the owner of the managed checkpoint entity.
    */
    LightObj & GetOwner() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the owner identifier of the managed checkpoint entity.
    */
    Int32 GetOwnerID() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the x axis of the managed checkpoint entity.
    */
    Float32 GetPositionX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the y axis of the managed checkpoint entity.
    */
    Float32 GetPositionY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the z axis of the managed checkpoint entity.
    */
    Float32 GetPositionZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed checkpoint entity.
    */
    void SetPositionX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed checkpoint entity.
    */
    void SetPositionY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed checkpoint entity.
    */
    void SetPositionZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the red color of the managed checkpoint entity.
    */
    Int32 GetColorR() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the green color of the managed checkpoint entity.
    */
    Int32 GetColorG() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the blue color of the managed checkpoint entity.
    */
    Int32 GetColorB() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the alpha transparency of the managed checkpoint entity.
    */
    Int32 GetColorA() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the red color of the managed checkpoint entity.
    */
    void SetColorR(Int32 r) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the green color of the managed checkpoint entity.
    */
    void SetColorG(Int32 g) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the blue color of the managed checkpoint entity.
    */
    void SetColorB(Int32 b) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the alpha transparency of the managed checkpoint entity.
    */
    void SetColorA(Int32 a) const;
};

} // Namespace:: SqMod

#endif // _ENTITY_CHECKPOINT_HPP_