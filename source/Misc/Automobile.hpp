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
    CAutomobile();

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the model specified.
    */
    CAutomobile(SQInt32 id);

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the model extracted from the specified name.
    */
    CAutomobile(const SQChar * name, SQInt32 id);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CAutomobile(const CAutomobile & a);

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CAutomobile(CAutomobile && a);

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CAutomobile();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CAutomobile & operator = (const CAutomobile & a);

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CAutomobile & operator = (CAutomobile && a);

    /* --------------------------------------------------------------------------------------------
     * Model identifier assignment operator.
    */
    CAutomobile & operator = (SQInt32 id);

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const CAutomobile & a) const;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const CAutomobile & a) const;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const CAutomobile & a) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const CAutomobile & a) const;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const CAutomobile & a) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const CAutomobile & a) const;

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
        return IsAutomobileValid(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Negation operator.
    */
    bool operator ! () const
    {
        return !IsAutomobileValid(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script to compare two instances of this type.
    */
    SQInteger Cmp(const CAutomobile & a) const;

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
    CAutomobile & SetnGet(SQInt32 id);

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
     * Create a vehicle instance using the referenced model.
    */
    Reference < CVehicle > Create(SQInt32 world, const Vector3 & pos, SQFloat angle,
                                    SQInt32 header, SqObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * Create a vehicle instance using the referenced model.
    */
    Reference < CVehicle > Create(SQInt32 world, const Vector3 & pos, SQFloat angle,
                                    SQInt32 primary, SQInt32 secondary, SQInt32 header,
                                    SqObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * Create a vehicle instance using the referenced model.
    */
    Reference < CVehicle > Create(SQInt32 world, SQFloat x, SQFloat y, SQFloat z, SQFloat angle,
                                    SQInt32 header, SqObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * Create a vehicle instance using the referenced model.
    */
    Reference < CVehicle > Create(SQInt32 world, SQFloat x, SQFloat y, SQFloat z, SQFloat angle,
                                    SQInt32 primary, SQInt32 secondary, SQInt32 header,
                                    SqObj & payload) const;

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
