#ifndef _MISC_VEHICLE_IMMUNITY_HPP_
#define _MISC_VEHICLE_IMMUNITY_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
struct CVehicleImmunity
{
    // --------------------------------------------------------------------------------------------
    CVehicleImmunity() noexcept;
    CVehicleImmunity(SQInt32 flags) noexcept;
    // --------------------------------------------------------------------------------------------
    CVehicleImmunity(const CVehicleImmunity & x) noexcept;
    CVehicleImmunity(CVehicleImmunity && x) noexcept;
    // --------------------------------------------------------------------------------------------
    ~CVehicleImmunity();
    // --------------------------------------------------------------------------------------------
    CVehicleImmunity & operator= (const CVehicleImmunity & x) noexcept;
    CVehicleImmunity & operator= (CVehicleImmunity && x) noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator == (const CVehicleImmunity & x) const noexcept;
    bool operator != (const CVehicleImmunity & x) const noexcept;
    bool operator < (const CVehicleImmunity & x) const noexcept;
    bool operator > (const CVehicleImmunity & x) const noexcept;
    bool operator <= (const CVehicleImmunity & x) const noexcept;
    bool operator >= (const CVehicleImmunity & x) const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const CVehicleImmunity & x) const noexcept;
    // --------------------------------------------------------------------------------------------
    operator SQInt32 () const noexcept;
    operator bool () const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInt32 GetFlags() const noexcept;
    void SetFlags(SQInt32 flags) noexcept;
    // --------------------------------------------------------------------------------------------
    CVehicleImmunity & SetnGet(SQInt32 flags) noexcept;
    // --------------------------------------------------------------------------------------------
    void Apply(const CVehicle & vehicle) const noexcept;
    // --------------------------------------------------------------------------------------------
    void EnableAll() noexcept;
    void DisableAll() noexcept;
protected:
    // --------------------------------------------------------------------------------------------
    SQInt32 m_Flags;
};

} // Namespace:: SqMod

#endif // _MISC_VEHICLE_IMMUNITY_HPP_