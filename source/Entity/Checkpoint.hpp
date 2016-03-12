#ifndef _ENTITY_CHECKPOINT_HPP_
#define _ENTITY_CHECKPOINT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages a single checkpoint entity.
*/
class CCheckpoint
{
    // --------------------------------------------------------------------------------------------
    friend class Core;

private:

    // --------------------------------------------------------------------------------------------
    static Color4       s_Color4;
    static Vector3      s_Vector3;

    // --------------------------------------------------------------------------------------------
    static Uint32       s_ColorR, s_ColorG, s_ColorB, s_ColorA;

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
            SqThrowF("Invalid checkpoint reference [%s]", m_Tag.c_str());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const CCheckpoint & o) const;

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
     * Destroy the managed checkpoint entity.
    */
    bool Destroy()
    {
        return Destroy(0, NullObject());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed checkpoint entity.
    */
    bool Destroy(Int32 header)
    {
        return Destroy(header, NullObject());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the managed checkpoint entity.
    */
    bool Destroy(Int32 header, Object & payload);

    /* --------------------------------------------------------------------------------------------
     * Bind to an event supported by this entity type.
    */
    void BindEvent(Int32 evid, Object & env, Function & func) const;

    /* --------------------------------------------------------------------------------------------
     * See if the managed checkpoint entity is streamed for the specified player.
    */
    bool IsStreamedFor(CPlayer & player) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the managed checkpoint entity exists.
    */
    Int32 GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the world in which the managed checkpoint entity exists.
    */
    void SetWorld(Int32 world) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the color of the managed checkpoint entity.
    */
    const Color4 & GetColor() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the color of the managed checkpoint entity.
    */
    void SetColor(const Color4 & col) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the color of the managed checkpoint entity.
    */
    void SetColorEx(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the managed checkpoint entity.
    */
    const Vector3 & GetPosition() const;

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
    void SetRadius(Float32 radius) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the owner of the managed checkpoint entity.
    */
    Object & GetOwner() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the owner identifier of the managed checkpoint entity.
    */
    Int32 GetOwnerID() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the x axis of the managed checkpoint entity.
    */
    Float32 GetPosX() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the y axis of the managed checkpoint entity.
    */
    Float32 GetPosY() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position on the z axis of the managed checkpoint entity.
    */
    Float32 GetPosZ() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the x axis of the managed checkpoint entity.
    */
    void SetPosX(Float32 x) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the y axis of the managed checkpoint entity.
    */
    void SetPosY(Float32 y) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the position on the z axis of the managed checkpoint entity.
    */
    void SetPosZ(Float32 z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the red color of the managed checkpoint entity.
    */
    Uint32 GetColR() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the green color of the managed checkpoint entity.
    */
    Uint32 GetColG() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the blue color of the managed checkpoint entity.
    */
    Uint32 GetColB() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the alpha transparency of the managed checkpoint entity.
    */
    Uint32 GetColA() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the red color of the managed checkpoint entity.
    */
    void SetColR(Uint32 r) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the green color of the managed checkpoint entity.
    */
    void SetColG(Uint32 g) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the blue color of the managed checkpoint entity.
    */
    void SetColB(Uint32 b) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the alpha transparency of the managed checkpoint entity.
    */
    void SetColA(Uint32 a) const;
};

} // Namespace:: SqMod

#endif // _ENTITY_CHECKPOINT_HPP_