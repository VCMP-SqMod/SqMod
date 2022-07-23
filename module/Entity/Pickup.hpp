#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Circular locks employed by the pickup manager.
*/
enum PickupCircularLocks
{
    PICKUPCL_EMIT_PICKUP_OPTION     = (1u << 0u),
    PICKUPCL_EMIT_PICKUP_WORLD      = (1u << 1u),
    PICKUPCL_EMIT_PICKUP_ALPHA      = (1u << 2u),
    PICKUPCL_EMIT_PICKUP_AUTOMATIC  = (1u << 3u),
    PICKUPCL_EMIT_PICKUP_AUTOTIMER  = (1u << 4u)
};

/* ------------------------------------------------------------------------------------------------
 * Manages a single pickup entity.
*/
class CPickup
{
    // --------------------------------------------------------------------------------------------
    friend class Core;
    friend class PickupInst;

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
    explicit CPickup(int32_t id);

public:

    /* --------------------------------------------------------------------------------------------
     * Maximum possible number that could represent an identifier for this entity type.
    */
    static const int32_t Max;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CPickup(const CPickup &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    CPickup(CPickup &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    CPickup & operator = (const CPickup &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    CPickup & operator = (CPickup &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * See whether this instance manages a valid entity instance otherwise throw an exception.
    */
    void Validate() const
    {
        if (INVALID_ENTITY(m_ID))
        {
            STHROWF("Invalid pickup reference [{}]", m_Tag);
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
    CPickup & ApplyTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user data.
    */
    SQMOD_NODISCARD LightObj & GetData();

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(LightObj & data);

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed pickup entity.
    */
    bool Destroy0() const // NOLINT(modernize-use-nodiscard)
    {
        return Destroy(0, NullLightObj());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed pickup entity.
    */
    bool Destroy1(int32_t header) const // NOLINT(modernize-use-nodiscard)
    {
        return Destroy(header, NullLightObj());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed pickup entity.
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
     * See if the managed pickup entity is streamed for the specified player.
    */
    SQMOD_NODISCARD bool IsStreamedFor(CPlayer & player) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current option value of the managed pickup entity.
    */
    SQMOD_NODISCARD bool GetOption(int32_t option_id) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the current option value of the managed pickup entity.
    */
    void SetOption(int32_t option_id, bool toggle);

    /* --------------------------------------------------------------------------------------------
     * Modify the current option value of the managed pickup entity.
    */
    void SetOptionEx(int32_t option_id, bool toggle, int32_t header, LightObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the managed pickup entity exists.
    */
    SQMOD_NODISCARD int32_t GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the world in which the managed pickup entity exists.
    */
    void SetWorld(int32_t world);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the alpha of the managed pickup entity.
    */
    SQMOD_NODISCARD int32_t GetAlpha() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the alpha of the managed pickup entity.
    */
    void SetAlpha(int32_t alpha);

    /* --------------------------------------------------------------------------------------------
     * See whether the managed pickup entity is automatic.
    */
    SQMOD_NODISCARD bool GetAutomatic() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the managed pickup entity is automatic.
    */
    void SetAutomatic(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the automatic timer of the managed pickup entity.
    */
    SQMOD_NODISCARD int32_t GetAutoTimer() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the automatic timer of the managed pickup entity.
    */
    void SetAutoTimer(int32_t timer);

    /* --------------------------------------------------------------------------------------------
     * Refresh the managed pickup entity.
    */
    void Refresh() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the managed pickup entity.
    */
    SQMOD_NODISCARD Vector3 GetPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed pickup entity.
    */
    void SetPosition(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position of the managed pickup entity.
    */
    void SetPositionEx(float x, float y, float z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the model of the managed pickup entity.
    */
    SQMOD_NODISCARD int32_t GetModel() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the quantity of the managed pickup entity.
    */
    SQMOD_NODISCARD int32_t GetQuantity() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the x axis of the managed pickup entity.
    */
    SQMOD_NODISCARD float GetPositionX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the y axis of the managed pickup entity.
    */
    SQMOD_NODISCARD float GetPositionY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the z axis of the managed pickup entity.
    */
    SQMOD_NODISCARD float GetPositionZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed pickup entity.
    */
    void SetPositionX(float x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed pickup entity.
    */
    void SetPositionY(float y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed pickup entity.
    */
    void SetPositionZ(float z) const;
#ifdef VCMP_ENABLE_OFFICIAL
    /* --------------------------------------------------------------------------------------------
     * Retrieve legacy object instance for this entity.
    */
    LightObj & GetLegacyObject() const;
#endif
};

} // Namespace:: SqMod
