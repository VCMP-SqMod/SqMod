#ifndef _MISC_WEAPON_HPP_
#define _MISC_WEAPON_HPP_

// ------------------------------------------------------------------------------------------------
#include "Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
class CWeapon : public IdentifierStorage< CWeapon, SQMOD_WEAPONID_CAP >
{
public:
    // --------------------------------------------------------------------------------------------
    static const CWeapon NIL;
    // --------------------------------------------------------------------------------------------
    CWeapon() noexcept;
    CWeapon(SQInt32 id) noexcept;
    CWeapon(SQInt32 id, SQInt32 ammo) noexcept;
    CWeapon(const SQChar * name, SQInt32 id, SQInt32 ammo) noexcept;
    // --------------------------------------------------------------------------------------------
    CWeapon(const CWeapon & w) noexcept;
    CWeapon(CWeapon && w) noexcept;
    // --------------------------------------------------------------------------------------------
    ~CWeapon() noexcept;
    // --------------------------------------------------------------------------------------------
    CWeapon & operator = (const CWeapon & w) noexcept;
    CWeapon & operator = (CWeapon && w) noexcept;
    // --------------------------------------------------------------------------------------------
    CWeapon & operator = (SQInt32 id) noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator == (const CWeapon & w) const noexcept;
    bool operator != (const CWeapon & w) const noexcept;
    bool operator < (const CWeapon & w) const noexcept;
    bool operator > (const CWeapon & w) const noexcept;
    bool operator <= (const CWeapon & w) const noexcept;
    bool operator >= (const CWeapon & w) const noexcept;
    // --------------------------------------------------------------------------------------------
    operator SQInt32 () const noexcept { return m_ID; }
    operator bool () const noexcept { return IsWeaponValid(m_ID); }
    // --------------------------------------------------------------------------------------------
    bool operator ! () const noexcept { return !IsWeaponValid(m_ID); }
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const CWeapon & w) const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger GetID() const noexcept;
    void SetID(SQInt32 id) noexcept;
    // --------------------------------------------------------------------------------------------
    CWeapon & SetnGet(SQInt32 id) noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * GetGlobalTag() const noexcept;
    void SetGlobalTag(const SQChar * tag) const noexcept;
    // --------------------------------------------------------------------------------------------
    SqObj & GetGlobalData() const noexcept;
    void SetGlobalData(SqObj & data) const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * GetLocalTag() const noexcept;
    void SetLocalTag(const SQChar * tag) noexcept;
    // --------------------------------------------------------------------------------------------
    SqObj & GetLocalData() noexcept;
    void SetLocalData(SqObj & data) noexcept;
    // --------------------------------------------------------------------------------------------
    bool IsValid() const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * GetName() const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger GetAmmo() const noexcept;
    void SetAmmo(SQInt32 amount) noexcept;
    // --------------------------------------------------------------------------------------------
    bool IsNatural() const noexcept;
    // --------------------------------------------------------------------------------------------
    SQFloat GetDataValue(SQInt32 field) const noexcept;
    void SetDataValue(SQInt32 field, SQFloat value) const noexcept;
    // --------------------------------------------------------------------------------------------
    void ResetData() const noexcept;
    void ResetData(SQInt32 field) const noexcept;
    // --------------------------------------------------------------------------------------------
    bool IsDataModified(SQInt32 field) const noexcept;
    // --------------------------------------------------------------------------------------------
    void SetOn(const Reference< CPlayer > & player) const noexcept;
    void GiveTo(const Reference< CPlayer > & player) const noexcept;
    void SetOn(const Reference< CPlayer > & player, SQInt32 ammo) const noexcept;
    void GiveTo(const Reference< CPlayer > & player, SQInt32 ammo) const noexcept;
private:
    // --------------------------------------------------------------------------------------------
    SQInt32     m_ID;
    SQInt32     m_Ammo;
    // --------------------------------------------------------------------------------------------
    String      m_Name;
    // --------------------------------------------------------------------------------------------
    SqTag       m_Tag;
    SqObj       m_Data;
};

} // Namespace:: SqMod

#endif // _MISC_WEAPON_HPP_
