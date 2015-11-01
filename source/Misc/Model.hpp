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
    CModel();

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the model specified.
    */
    CModel(SQInt32 id);

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the model extracted from the specified name.
    */
    CModel(const SQChar * name, SQInt32 id);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CModel(const CModel & m);

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CModel(CModel && m);

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CModel();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CModel & operator = (const CModel & m);

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CModel & operator = (CModel && m);

    /* --------------------------------------------------------------------------------------------
     * Model identifier assignment operator.
    */
    CModel & operator = (SQInt32 id);

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const CModel & m) const;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const CModel & m) const;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const CModel & m) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const CModel & m) const;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const CModel & m) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const CModel & m) const;

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to model identifier.
    */
    operator SQInt32 () const
    {
        return m_ID;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to model identifier.
    */
    operator Int64 () const
    {
        return _SCI64(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean.
    */
    operator bool () const
    {
        return IsModelValid(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Negation operator.
    */
    bool operator ! () const
    {
        return !IsModelValid(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script to compare two instances of this type.
    */
    SQInteger Cmp(const CModel & m) const;

    /* --------------------------------------------------------------------------------------------
     * Convert this type to a string.
    */
    const SQChar * ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier referenced by this instance.
    */
    SQInteger GetID() const;

    /* --------------------------------------------------------------------------------------------
     * Change the identifier referenced by this instance.
    */
    void SetID(SQInt32 id);

    /* --------------------------------------------------------------------------------------------
     * Set the identifier that this insance should reference and
     * get a reference to the instance to chain operations.
    */
    CModel & SetnGet(SQInt32 id);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the global tag.
    */
    const SQChar * GetGlobalTag() const;

    /* --------------------------------------------------------------------------------------------
     * Change the global tag.
    */
    void SetGlobalTag(const SQChar * tag) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the global data.
    */
    SqObj & GetGlobalData() const;

    /* --------------------------------------------------------------------------------------------
     * Change the global data.
    */
    void SetGlobalData(SqObj & data) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the local tag.
    */
    const SQChar * GetLocalTag() const;

    /* --------------------------------------------------------------------------------------------
     * Change the local tag.
    */
    void SetLocalTag(const SQChar * tag);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the local data.
    */
    SqObj & GetLocalData();

    /* --------------------------------------------------------------------------------------------
     * Change the local data.
    */
    void SetLocalData(SqObj & data);

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced model identifier is valid.
    */
    bool IsValid() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the referenced model.
    */
    const SQChar * GetName() const;

    /* --------------------------------------------------------------------------------------------
     * Change the identifier of the referenced model.
    */
    void SetName(const SQChar * name);

    /* --------------------------------------------------------------------------------------------
     * See if the referenced model identifier is a weapon model.
    */
    bool IsWeapon() const;

    /* --------------------------------------------------------------------------------------------
     * See if the referenced model identifier is truly a weapon model
     * and not something like a camera.
    */
    bool IsActuallyWeapon() const;

    /* --------------------------------------------------------------------------------------------
     * Create an object instance using the referenced model.
    */
    Reference< CObject > Object(SQInt32 world, const Vector3 & pos, SQInt32 alpha, SQInt32 header,
                                SqObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * Create an object instance using the referenced model.
    */
    Reference< CObject > Object(SQInt32 world, SQFloat x, SQFloat y, SQFloat z, SQInt32 alpha,
                                SQInt32 header, SqObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * Create a pickup instance using the referenced model.
    */
    Reference< CPickup > Pickup(SQInt32 world, SQInt32 quantity, const Vector3 & pos, SQInt32 alpha,
                                bool automatic, SQInt32 header, SqObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * Create a pickup instance using the referenced model.
    */
    Reference< CPickup > Pickup(SQInt32 world, SQInt32 quantity, SQFloat x, SQFloat y, SQFloat z,
                                SQInt32 alpha, bool automatic, SQInt32 header, SqObj & payload) const;

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
