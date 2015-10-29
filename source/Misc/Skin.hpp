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
    CSkin() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the specified skin.
    */
    CSkin(SQInt32 id) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the skin extracted from the specified name.
    */
    CSkin(const SQChar * name, SQInt32 id) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CSkin(const CSkin & s) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CSkin(CSkin && s) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CSkin() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CSkin & operator = (const CSkin & s) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CSkin & operator = (CSkin && s) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Skin identifier assignment operator.
    */
    CSkin & operator = (SQInt32 id) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const CSkin & s) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const CSkin & s) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const CSkin & s) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const CSkin & s) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const CSkin & s) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const CSkin & s) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to skin identifier.
    */
    operator SQInt32 () const noexcept { return m_ID; }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean.
    */
    operator bool () const noexcept { return IsSkinValid(m_ID); }

    /* --------------------------------------------------------------------------------------------
     * Negation operator.
    */
    bool operator ! () const noexcept { return !IsSkinValid(m_ID); }

    /* --------------------------------------------------------------------------------------------
     * Used by the script to compare two instances of this type.
    */
    SQInteger Cmp(const CSkin & s) const noexcept;

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
    CSkin & SetnGet(SQInt32 id) noexcept;

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
     *  Retrieve the local tag.
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
     * See whether the referenced skin identifier is valid.
    */
    bool IsValid() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the referenced skin.
    */
    const SQChar * GetName() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the identifier of the referenced skin.
    */
    void SetName(const SQChar * name) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Apply the referenced skin identifier to the specified player instance.
    */
    void Apply(const Reference< CPlayer > & player) const noexcept;

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
