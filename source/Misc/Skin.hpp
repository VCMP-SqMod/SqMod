#ifndef _MISC_SKIN_HPP_
#define _MISC_SKIN_HPP_

// ------------------------------------------------------------------------------------------------
#include "Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
class CSkin : public IdentifierStorage< CSkin, SQMOD_SKINID_CAP >
{
public:
    // --------------------------------------------------------------------------------------------
    static const CSkin NIL;
    // --------------------------------------------------------------------------------------------
    CSkin() noexcept;
    CSkin(SQInt32 id) noexcept;
    CSkin(const SQChar * name, SQInt32 id) noexcept;
    // --------------------------------------------------------------------------------------------
    CSkin(const CSkin & s) noexcept;
    CSkin(CSkin && s) noexcept;
    // --------------------------------------------------------------------------------------------
    ~CSkin() noexcept;
    // --------------------------------------------------------------------------------------------
    CSkin & operator = (const CSkin & s) noexcept;
    CSkin & operator = (CSkin && s) noexcept;
    // --------------------------------------------------------------------------------------------
    CSkin & operator = (SQInt32 id) noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator == (const CSkin & s) const noexcept;
    bool operator != (const CSkin & s) const noexcept;
    bool operator < (const CSkin & s) const noexcept;
    bool operator > (const CSkin & s) const noexcept;
    bool operator <= (const CSkin & s) const noexcept;
    bool operator >= (const CSkin & s) const noexcept;
    // --------------------------------------------------------------------------------------------
    operator SQInt32 () const noexcept { return m_ID; }
    operator bool () const noexcept { return IsSkinValid(m_ID); }
    // --------------------------------------------------------------------------------------------
    bool operator ! () const noexcept { return !IsSkinValid(m_ID); }
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const CSkin & s) const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger GetID() const noexcept;
    void SetID(SQInt32 id) noexcept;
    // --------------------------------------------------------------------------------------------
    CSkin & SetnGet(SQInt32 id) noexcept;
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
    void SetName(const SQChar * name) noexcept;
    // --------------------------------------------------------------------------------------------
    void Apply(const Reference< CPlayer > & player) const noexcept;
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

#endif // _MISC_SKIN_HPP_
