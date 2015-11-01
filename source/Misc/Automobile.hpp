#ifndef _MISC_AUTOMOBILE_HPP_
#define _MISC_AUTOMOBILE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Misc/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class responsible for managing and interacting with vehicle models.
*/
class CAutomobile : public IdentifierStorage< CAutomobile, SQMOD_VEHICLEID_CAP >
{
public:

    /* --------------------------------------------------------------------------------------------
     * Helper member for times when a null reference to an instance of this type is needed.
    */
    static const CAutomobile NIL;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CAutomobile() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the model specified.
    */
    CAutomobile(SQInt32 id) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the model extracted from the specified name.
    */
    CAutomobile(const SQChar * name, SQInt32 id) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CAutomobile(const CAutomobile & a) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CAutomobile(CAutomobile && a) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CAutomobile();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CAutomobile & operator = (const CAutomobile & a) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CAutomobile & operator = (CAutomobile && a) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Model identifier assignment operator.
    */
    CAutomobile & operator = (SQInt32 id) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const CAutomobile & a) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const CAutomobile & a) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const CAutomobile & a) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const CAutomobile & a) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const CAutomobile & a) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const CAutomobile & a) const noexcept;

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
        return IsAutomobileValid(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Negation operator.
    */
    bool operator ! () const noexcept
    {
        return !IsAutomobileValid(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script to compare two instances of this type.
    */
    SQInteger Cmp(const CAutomobile & a) const noexcept;

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
    CAutomobile & SetnGet(SQInt32 id) noexcept;

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
     * Create a vehicle instance using the referenced model.
    */
    Reference < CVehicle > Create(SQInt32 world, const Vector3 & pos, SQFloat angle,
                                    SQInt32 header, SqObj & payload) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Create a vehicle instance using the referenced model.
    */
    Reference < CVehicle > Create(SQInt32 world, const Vector3 & pos, SQFloat angle,
                                    SQInt32 primary, SQInt32 secondary, SQInt32 header,
                                    SqObj & payload) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Create a vehicle instance using the referenced model.
    */
    Reference < CVehicle > Create(SQInt32 world, SQFloat x, SQFloat y, SQFloat z, SQFloat angle,
                                    SQInt32 header, SqObj & payload) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Create a vehicle instance using the referenced model.
    */
    Reference < CVehicle > Create(SQInt32 world, SQFloat x, SQFloat y, SQFloat z, SQFloat angle,
                                    SQInt32 primary, SQInt32 secondary, SQInt32 header,
                                    SqObj & payload) const noexcept;

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

#endif // _MISC_AUTOMOBILE_HPP_
