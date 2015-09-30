#ifndef _MISC_MODEL_HPP_
#define _MISC_MODEL_HPP_

// ------------------------------------------------------------------------------------------------
#include "Misc/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
class CModel : public IdentifierStorage< CModel, SQMOD_MODELID_CAP >
{
public:
    // --------------------------------------------------------------------------------------------
    static const CModel NIL;
    // --------------------------------------------------------------------------------------------
    CModel() noexcept;
    CModel(SQInt32 id) noexcept;
    CModel(const SQChar * name, SQInt32 id) noexcept;
    // --------------------------------------------------------------------------------------------
    CModel(const CModel & m) noexcept;
    CModel(CModel && m) noexcept;
    // --------------------------------------------------------------------------------------------
    ~CModel();
    // --------------------------------------------------------------------------------------------
    CModel & operator = (const CModel & m) noexcept;
    CModel & operator = (CModel && m) noexcept;
    // --------------------------------------------------------------------------------------------
    CModel & operator = (SQInt32 id) noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator == (const CModel & m) const noexcept;
    bool operator != (const CModel & m) const noexcept;
    bool operator < (const CModel & m) const noexcept;
    bool operator > (const CModel & m) const noexcept;
    bool operator <= (const CModel & m) const noexcept;
    bool operator >= (const CModel & m) const noexcept;
    // --------------------------------------------------------------------------------------------
    operator SQInt32 () const noexcept { return m_ID; }
    operator bool () const noexcept { return IsModelValid(m_ID); }
    // --------------------------------------------------------------------------------------------
    bool operator ! () const noexcept { return !IsModelValid(m_ID); }
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const CModel & m) const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger GetID() const noexcept;
    void SetID(SQInt32 id) noexcept;
    // --------------------------------------------------------------------------------------------
    CModel & SetnGet(SQInt32 id) noexcept;
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
    bool IsWeapon() const noexcept;
    bool IsActuallyWeapon() const noexcept;
    // --------------------------------------------------------------------------------------------
    Reference< CObject > Object(SQInt32 world, const Vector3 & pos, SQInt32 alpha, SQInt32 header, \
                                const SqObj & payload) const noexcept;
    Reference< CObject > Object(SQInt32 world, SQFloat x, SQFloat y, SQFloat z, SQInt32 alpha, \
                                SQInt32 header, const SqObj & payload) const noexcept;
    // --------------------------------------------------------------------------------------------
    Reference< CPickup > Pickup(SQInt32 world, SQInt32 quantity, const Vector3 & pos, SQInt32 alpha, \
                                bool automatic, SQInt32 header, const SqObj & payload) const noexcept;
    Reference< CPickup > Pickup(SQInt32 world, SQInt32 quantity, SQFloat x, SQFloat y, SQFloat z, \
                                SQInt32 alpha, bool automatic, SQInt32 header, const SqObj & payload) const noexcept;
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

#endif // _MISC_MODEL_HPP_
