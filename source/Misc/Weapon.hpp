#ifndef _MISC_WEAPON_HPP_
#define _MISC_WEAPON_HPP_

// ------------------------------------------------------------------------------------------------
#include "Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class responsible for managing and interacting with weapon models.
*/
class CWeapon : public IdentifierStorage< CWeapon, SQMOD_WEAPONID_CAP >
{
public:

    /* --------------------------------------------------------------------------------------------
     * Helper member for times when a null reference to an instance of this type is needed.
    */
    static const CWeapon NIL;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CWeapon();

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the model specified.
    */
    CWeapon(SQInt32 id);

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the model specified.
    */
    CWeapon(SQInt32 id, SQInt32 ammo);

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the model extracted from the specified name.
    */
    CWeapon(const SQChar * name, SQInt32 id, SQInt32 ammo);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CWeapon(const CWeapon & w);

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CWeapon(CWeapon && w);

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CWeapon();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CWeapon & operator = (const CWeapon & w);

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CWeapon & operator = (CWeapon && w);

    /* --------------------------------------------------------------------------------------------
     * Model identifier assignment operator.
    */
    CWeapon & operator = (SQInt32 id);

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const CWeapon & w) const;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const CWeapon & w) const;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const CWeapon & w) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const CWeapon & w) const;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const CWeapon & w) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const CWeapon & w) const;

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to weapon identifier.
    */
    operator SQInt32 () const
    {
        return m_ID;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to weapon identifier.
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
        return IsWeaponValid(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Negation operator.
    */
    bool operator ! () const
    {
        return !IsWeaponValid(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script to compare two instances of this type.
    */
    SQInteger Cmp(const CWeapon & w) const;

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
    CWeapon & SetnGet(SQInt32 id);

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
     * Retrieve the ammount of ammo associated with this instance.
    */
    SQInteger GetAmmo() const;

    /* --------------------------------------------------------------------------------------------
     * Change the ammount of ammo associated with this instance.
    */
    void SetAmmo(SQInt32 amount);

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced weapon identifier points to a weapon that may not be used by
     * another player to perform a kill. Such as a player fall, drown, explosion etc.
    */
    bool IsNatural() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value in the specified field from the weapon data.
    */
    SQFloat GetDataValue(SQInt32 field) const;

    /* --------------------------------------------------------------------------------------------
     * Change the value in the specified field from the weapon data.
    */
    void SetDataValue(SQInt32 field, SQFloat value) const;

    /* --------------------------------------------------------------------------------------------
     * Reset the data for the referenced weapon identifier.
    */
    void ResetData() const;

    /* --------------------------------------------------------------------------------------------
     * Reset the foe;d data data for the referenced weapon identifier.
    */
    void ResetData(SQInt32 field) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the data at the specified field was modified. 
    */
    bool IsDataModified(SQInt32 field) const;

    /* --------------------------------------------------------------------------------------------
     * Set the weapon of the specified player instance to the referenced weapon identifier.
    */
    void SetOn(const Reference< CPlayer > & player) const;

    /* --------------------------------------------------------------------------------------------
     * Give the referenced weapon identifier to the specified player instance.
    */
    void GiveTo(const Reference< CPlayer > & player) const;

    /* --------------------------------------------------------------------------------------------
     * Set the weapon of the specified player instance to the referenced weapon identifier.
    */
    void SetOn(const Reference< CPlayer > & player, SQInt32 ammo) const;

    /* --------------------------------------------------------------------------------------------
     * Give the referenced weapon identifier to the specified player instance.
    */
    void GiveTo(const Reference< CPlayer > & player, SQInt32 ammo) const;

private:

    /* --------------------------------------------------------------------------------------------
     * The identifier of the referenced model.
    */
    SQInt32     m_ID;

    /* --------------------------------------------------------------------------------------------
     * The ammo associated with this instance.
    */
    SQInt32     m_Ammo;

    /* --------------------------------------------------------------------------------------------
     * The local tag associated with this instance
    */
    SqTag       m_Tag;

    /* --------------------------------------------------------------------------------------------
     * The local data associated with this instance.
    */
    SqObj       m_Data;
};

} // Namespace:: SqMod

#endif // _MISC_WEAPON_HPP_
