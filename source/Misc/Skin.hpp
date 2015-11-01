#ifndef _MISC_SKIN_HPP_
#define _MISC_SKIN_HPP_

// ------------------------------------------------------------------------------------------------
#include "Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class responsible for managing and interacting with skin models.
*/
class CSkin : public IdentifierStorage< CSkin, SQMOD_SKINID_CAP >
{
public:

    /* --------------------------------------------------------------------------------------------
     * Helper member for times when a null reference to an instance of this type is needed.
    */
    static const CSkin NIL;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CSkin();

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the specified skin.
    */
    CSkin(SQInt32 id);

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the skin extracted from the specified name.
    */
    CSkin(const SQChar * name, SQInt32 id);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CSkin(const CSkin & s);

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CSkin(CSkin && s);

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CSkin();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CSkin & operator = (const CSkin & s);

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CSkin & operator = (CSkin && s);

    /* --------------------------------------------------------------------------------------------
     * Skin identifier assignment operator.
    */
    CSkin & operator = (SQInt32 id);

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const CSkin & s) const;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const CSkin & s) const;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const CSkin & s) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const CSkin & s) const;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const CSkin & s) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const CSkin & s) const;

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to skin identifier.
    */
    operator SQInt32 () const
    {
        return m_ID;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to skin identifier.
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
        return IsSkinValid(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Negation operator.
    */
    bool operator ! () const
    {
        return !IsSkinValid(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script to compare two instances of this type.
    */
    SQInteger Cmp(const CSkin & s) const;

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
    CSkin & SetnGet(SQInt32 id);

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
     *  Retrieve the local tag.
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
     * See whether the referenced skin identifier is valid.
    */
    bool IsValid() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the referenced skin.
    */
    const SQChar * GetName() const;

    /* --------------------------------------------------------------------------------------------
     * Change the identifier of the referenced skin.
    */
    void SetName(const SQChar * name);

    /* --------------------------------------------------------------------------------------------
     * Apply the referenced skin identifier to the specified player instance.
    */
    void Apply(const Reference< CPlayer > & player) const;

private:

    /* --------------------------------------------------------------------------------------------
     * The identifier of the referenced skin.
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

#endif // _MISC_SKIN_HPP_
