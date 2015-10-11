#ifndef _MISC_AUTOMOBILE_HPP_
#define _MISC_AUTOMOBILE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Misc/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
class CAutomobile : public IdentifierStorage< CAutomobile, SQMOD_VEHICLEID_CAP >
{
public:
    // --------------------------------------------------------------------------------------------
    static const CAutomobile NIL;
    // --------------------------------------------------------------------------------------------
    CAutomobile() noexcept;
    CAutomobile(SQInt32 id) noexcept;
    CAutomobile(const SQChar * name, SQInt32 id) noexcept;
    // --------------------------------------------------------------------------------------------
    CAutomobile(const CAutomobile & a) noexcept;
    CAutomobile(CAutomobile && a) noexcept;
    // --------------------------------------------------------------------------------------------
    ~CAutomobile();
    // --------------------------------------------------------------------------------------------
    CAutomobile & operator = (const CAutomobile & a) noexcept;
    CAutomobile & operator = (CAutomobile && a) noexcept;
    // --------------------------------------------------------------------------------------------
    CAutomobile & operator = (SQInt32 id) noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator == (const CAutomobile & a) const noexcept;
    bool operator != (const CAutomobile & a) const noexcept;
    bool operator < (const CAutomobile & a) const noexcept;
    bool operator > (const CAutomobile & a) const noexcept;
    bool operator <= (const CAutomobile & a) const noexcept;
    bool operator >= (const CAutomobile & a) const noexcept;
    // --------------------------------------------------------------------------------------------
    operator SQInt32 () const noexcept { return m_ID; }
    operator bool () const noexcept { return IsAutomobileValid(m_ID); }
    // --------------------------------------------------------------------------------------------
    bool operator ! () const noexcept { return !IsAutomobileValid(m_ID); }
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const CAutomobile & a) const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger GetID() const noexcept;
    void SetID(SQInt32 id) noexcept;
    // --------------------------------------------------------------------------------------------
    CAutomobile & SetnGet(SQInt32 id) noexcept;
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
    Reference < CVehicle > Create(SQInt32 world, const Vector3 & pos, SQFloat angle, \
                                    SQInt32 header, SqObj & payload) const noexcept;
    Reference < CVehicle > Create(SQInt32 world, const Vector3 & pos, SQFloat angle, \
                                    SQInt32 primary, SQInt32 secondary, SQInt32 header, \
                                    SqObj & payload) const noexcept;
    Reference < CVehicle > Create(SQInt32 world, SQFloat x, SQFloat y, SQFloat z, SQFloat angle, \
                                    SQInt32 header, SqObj & payload) const noexcept;
    Reference < CVehicle > Create(SQInt32 world, SQFloat x, SQFloat y, SQFloat z, SQFloat angle, \
                                    SQInt32 primary, SQInt32 secondary, SQInt32 header, \
                                    SqObj & payload) const noexcept;
private:
    // --------------------------------------------------------------------------------------------
    SQInt32     m_ID;
    // --------------------------------------------------------------------------------------------
    String      m_Name;
    // --------------------------------------------------------------------------------------------
    SqTag       m_Tag;
    SqObj       m_Data;
};

} // Namespace:: SqMod

#endif // _MISC_AUTOMOBILE_HPP_
