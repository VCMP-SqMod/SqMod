#ifndef _MISC_MODEL_HPP_
#define _MISC_MODEL_HPP_

// ------------------------------------------------------------------------------------------------
#include "Misc/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class responsible for managing and interacting with object and pickup models.
*/
class CModel : public IdentifierStorage< CModel, SQMOD_MODELID_CAP >
{
public:

    /* --------------------------------------------------------------------------------------------
     * Helper member for times when a null reference to an instance of this type is needed.
    */
    static const CModel NIL;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CModel() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the model specified.
    */
    CModel(SQInt32 id) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the model extracted from the specified name.
    */
    CModel(const SQChar * name, SQInt32 id) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CModel(const CModel & m) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CModel(CModel && m) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CModel();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CModel & operator = (const CModel & m) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CModel & operator = (CModel && m) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Model identifier assignment operator.
    */
    CModel & operator = (SQInt32 id) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const CModel & m) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const CModel & m) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const CModel & m) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const CModel & m) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const CModel & m) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const CModel & m) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to model identifier.
    */
    operator SQInt32 () const noexcept
    {
        return m_ID;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to model identifier.
    */
    operator Int64 () const noexcept
    {
        return _SCI64(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean.
    */
    operator bool () const noexcept
    {
        return IsModelValid(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Negation operator.
    */
    bool operator ! () const noexcept
    {
        return !IsModelValid(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script to compare two instances of this type.
    */
    SQInteger Cmp(const CModel & m) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Convert this type to a string.
    */
    const SQChar * ToString() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier referenced by this instance.
    */
    SQInteger GetID() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the identifier referenced by this instance.
    */
    void SetID(SQInt32 id) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set the identifier that this insance should reference and
     * get a reference to the instance to chain operations.
    */
    CModel & SetnGet(SQInt32 id) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the global tag.
    */
    const SQChar * GetGlobalTag() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the global tag.
    */
    void SetGlobalTag(const SQChar * tag) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the global data.
    */
    SqObj & GetGlobalData() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the global data.
    */
    void SetGlobalData(SqObj & data) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the local tag.
    */
    const SQChar * GetLocalTag() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the local tag.
    */
    void SetLocalTag(const SQChar * tag) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the local data.
    */
    SqObj & GetLocalData() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the local data.
    */
    void SetLocalData(SqObj & data) noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced model identifier is valid.
    */
    bool IsValid() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the referenced model.
    */
    const SQChar * GetName() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the identifier of the referenced model.
    */
    void SetName(const SQChar * name) noexcept;

    /* --------------------------------------------------------------------------------------------
     * See if the referenced model identifier is a weapon model.
    */
    bool IsWeapon() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See if the referenced model identifier is truly a weapon model
     * and not something like a camera.
    */
    bool IsActuallyWeapon() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Create an object instance using the referenced model.
    */
    Reference< CObject > Object(SQInt32 world, const Vector3 & pos, SQInt32 alpha, SQInt32 header,
                                SqObj & payload) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Create an object instance using the referenced model.
    */
    Reference< CObject > Object(SQInt32 world, SQFloat x, SQFloat y, SQFloat z, SQInt32 alpha,
                                SQInt32 header, SqObj & payload) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Create a pickup instance using the referenced model.
    */
    Reference< CPickup > Pickup(SQInt32 world, SQInt32 quantity, const Vector3 & pos, SQInt32 alpha,
                                bool automatic, SQInt32 header, SqObj & payload) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Create a pickup instance using the referenced model.
    */
    Reference< CPickup > Pickup(SQInt32 world, SQInt32 quantity, SQFloat x, SQFloat y, SQFloat z,
                                SQInt32 alpha, bool automatic, SQInt32 header, SqObj & payload) const noexcept;

private:

    /* --------------------------------------------------------------------------------------------
     * The identifier of the referenced model.
    */
    SQInt32     m_ID;

    /* --------------------------------------------------------------------------------------------
     * The local tag associated with this instance.
    */
    SqTag       m_Tag;

    /* --------------------------------------------------------------------------------------------
     * The local data associated with this instance.
    */
    SqObj       m_Data;
};

} // Namespace:: SqMod

#endif // _MISC_MODEL_HPP_
