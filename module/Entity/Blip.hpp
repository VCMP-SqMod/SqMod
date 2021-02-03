#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages a single blip entity.
*/
class CBlip
{
    // --------------------------------------------------------------------------------------------
    friend class Core;
    friend class BlipInst;

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
     * Base constructor.
    */
    explicit CBlip(int32_t id);

public:

    /* --------------------------------------------------------------------------------------------
     * Maximum possible number that could represent an identifier for this entity type.
    */
    static const int32_t Max;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CBlip(const CBlip &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    CBlip(CBlip &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    CBlip & operator = (const CBlip &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    CBlip & operator = (CBlip &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * See whether this instance manages a valid entity otherwise throw an exception.
    */
    void Validate() const
    {
        if (INVALID_ENTITY(m_ID))
        {
            STHROWF("Invalid blip reference [{}]", m_Tag);
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
    CBlip & ApplyTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user data.
    */
    SQMOD_NODISCARD LightObj & GetData();

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(LightObj & data);

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed blip entity.
    */
    bool Destroy0() const // NOLINT(modernize-use-nodiscard)
    {
        return Destroy(0, NullLightObj());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed blip entity.
    */
    bool Destroy1(int32_t header) const // NOLINT(modernize-use-nodiscard)
    {
        return Destroy(header, NullLightObj());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed blip entity.
    */
    bool Destroy(int32_t header, LightObj & payload) const;  // NOLINT(modernize-use-nodiscard)

    /* --------------------------------------------------------------------------------------------
     * Retrieve the events table of this entity.
    */
    SQMOD_NODISCARD LightObj & GetEvents() const;

    /* --------------------------------------------------------------------------------------------
     * Emit a custom event for the managed entity
    */
    void CustomEvent(int32_t header, LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the referenced blip entity exists.
    */
    SQMOD_NODISCARD int32_t GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the scale of the managed blip entity.
    */
    SQMOD_NODISCARD int32_t GetScale() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the managed blip entity.
    */
    SQMOD_NODISCARD const Vector3 & GetPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the color of the managed blip entity.
    */
    SQMOD_NODISCARD const Color4 & GetColor() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier of the sprite used by the managed blip entity.
    */
    SQMOD_NODISCARD int32_t GetSprID() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the x axis of the managed blip entity.
    */
    SQMOD_NODISCARD float GetPositionX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the y axis of the managed blip entity.
    */
    SQMOD_NODISCARD float GetPositionY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the z axis of the managed blip entity.
    */
    SQMOD_NODISCARD float GetPositionZ() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the red color of the managed blip entity.
    */
    SQMOD_NODISCARD int32_t GetColorR() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the green color of the managed blip entity.
    */
    SQMOD_NODISCARD int32_t GetColorG() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the blue color of the managed blip entity.
    */
    SQMOD_NODISCARD int32_t GetColorB() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the alpha transparency of the managed blip entity.
    */
    SQMOD_NODISCARD int32_t GetColorA() const;
};

} // Namespace:: SqMod
