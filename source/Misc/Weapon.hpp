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
    CWeapon() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the model specified.
    */
    CWeapon(SQInt32 id) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the model specified.
    */
    CWeapon(SQInt32 id, SQInt32 ammo) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct an instance of this type and reference the model extracted from the specified name.
    */
    CWeapon(const SQChar * name, SQInt32 id, SQInt32 ammo) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CWeapon(const CWeapon & w) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CWeapon(CWeapon && w) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CWeapon() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CWeapon & operator = (const CWeapon & w) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CWeapon & operator = (CWeapon && w) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Model identifier assignment operator.
    */
    CWeapon & operator = (SQInt32 id) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const CWeapon & w) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const CWeapon & w) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const CWeapon & w) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const CWeapon & w) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const CWeapon & w) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const CWeapon & w) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to model identifier.
    */
    operator SQInt32 () const noexcept
    {
        return m_ID;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean.
    */
    operator bool () const noexcept
    {
        return IsWeaponValid(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Negation operator.
    */
    bool operator ! () const noexcept
    {
        return !IsWeaponValid(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script to compare two instances of this type.
    */
    SQInteger Cmp(const CWeapon & w) const noexcept;

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
    CWeapon & SetnGet(SQInt32 id) noexcept;

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
     * Retrieve the ammount of ammo associated with this instance.
    */
    SQInteger GetAmmo() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the ammount of ammo associated with this instance.
    */
    void SetAmmo(SQInt32 amount) noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced weapon identifier points to a weapon that may not be used by
     * another player to perform a kill. Such as a player fall, drown, explosion etc.
    */
    bool IsNatural() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value in the specified field from the weapon data.
    */
    SQFloat GetDataValue(SQInt32 field) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the value in the specified field from the weapon data.
    */
    void SetDataValue(SQInt32 field, SQFloat value) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Reset the data for the referenced weapon identifier.
    */
    void ResetData() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Reset the foe;d data data for the referenced weapon identifier.
    */
    void ResetData(SQInt32 field) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the data at the specified field was modified. 
    */
    bool IsDataModified(SQInt32 field) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set the weapon of the specified player instance to the referenced weapon identifier.
    */
    void SetOn(const Reference< CPlayer > & player) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Give the referenced weapon identifier to the specified player instance.
    */
    void GiveTo(const Reference< CPlayer > & player) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set the weapon of the specified player instance to the referenced weapon identifier.
    */
    void SetOn(const Reference< CPlayer > & player, SQInt32 ammo) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Give the referenced weapon identifier to the specified player instance.
    */
    void GiveTo(const Reference< CPlayer > & player, SQInt32 ammo) const noexcept;

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
