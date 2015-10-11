#include "Misc/Model.hpp"
#include "Base/Vector3.hpp"
#include "Entity.hpp"
#include "Register.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const CModel CModel::NIL;

// ------------------------------------------------------------------------------------------------
CModel::CModel() noexcept
    : m_ID(SQMOD_UNKNOWN), m_Name()
{

}

CModel::CModel(SQInt32 id) noexcept
    : m_ID(VALID_ENTITYGETEX(id, Max)), m_Name(GetModelName(id))
{

}

CModel::CModel(const SQChar * name, SQInt32 id) noexcept
    : CModel(WeaponToModel(IsWeaponValid(GetWeaponID(name)) ? GetWeaponID(name) : id))
{

}

// ------------------------------------------------------------------------------------------------
CModel::CModel(const CModel & m) noexcept
    : m_ID(m.m_ID)
    , m_Name(m.m_Name)
    , m_Tag(m.m_Tag)
    , m_Data(m.m_Data)
{

}

CModel::CModel(CModel && m) noexcept
    : m_ID(m.m_ID)
    , m_Name(m.m_Name)
    , m_Tag(m.m_Tag)
    , m_Data(m.m_Data)
{

}

// ------------------------------------------------------------------------------------------------
CModel::~CModel()
{

}

// ------------------------------------------------------------------------------------------------
CModel & CModel::operator = (const CModel & m) noexcept
{
    m_ID = m.m_ID;
    m_Name = m.m_Name;
    m_Tag = m.m_Tag;
    m_Data = m.m_Data;
    return *this;
}

CModel & CModel::operator = (CModel && m) noexcept
{
    m_ID = m.m_ID;
    m_Name = m.m_Name;
    m_Tag = m.m_Tag;
    m_Data = m.m_Data;
    return *this;
}

// ------------------------------------------------------------------------------------------------
CModel & CModel::operator = (SQInt32 id) noexcept
{
    if (m_ID != id)
    {
        m_Name = GetModelName(id);
        m_ID = id;
    }

    return *this;
}

// ------------------------------------------------------------------------------------------------
bool CModel::operator == (const CModel & m) const noexcept
{
    return (m_ID == m.m_ID);
}

bool CModel::operator != (const CModel & m) const noexcept
{
    return (m_ID != m.m_ID);
}

bool CModel::operator < (const CModel & m) const noexcept
{
    return (m_ID < m.m_ID);
}

bool CModel::operator > (const CModel & m) const noexcept
{
    return (m_ID < m.m_ID);
}

bool CModel::operator <= (const CModel & m) const noexcept
{
    return (m_ID <= m.m_ID);
}

bool CModel::operator >= (const CModel & m) const noexcept
{
    return (m_ID >= m.m_ID);
}

// ------------------------------------------------------------------------------------------------
SQInteger CModel::Cmp(const CModel & m) const noexcept
{
    return m_ID == m.m_ID ? 0 : (m_ID > m.m_ID ? 1 : -1);
}

// ------------------------------------------------------------------------------------------------
const SQChar * CModel::ToString() const noexcept
{
    return m_Name.c_str();
}

// ------------------------------------------------------------------------------------------------
SQInteger CModel::GetID() const noexcept
{
    return m_ID;
}

void CModel::SetID(SQInt32 id) noexcept
{
    if (m_ID != id)
    {
        m_Name = GetModelName(id);
        m_ID = id;
    }
}

// ------------------------------------------------------------------------------------------------
CModel & CModel::SetnGet(SQInt32 id) noexcept
{
    if (m_ID != id)
    {
        m_Name = GetModelName(id);
        m_ID = id;
    }

    return *this;
}

// ------------------------------------------------------------------------------------------------
const SQChar * CModel::GetGlobalTag() const noexcept
{
    return GlobalTag(m_ID);
}

void CModel::SetGlobalTag(const SQChar * tag) const noexcept
{
    GlobalTag(m_ID, tag);
}

// ------------------------------------------------------------------------------------------------
SqObj & CModel::GetGlobalData() const noexcept
{
    return GlobalData(m_ID);
}

void CModel::SetGlobalData(SqObj & data) const noexcept
{
    GlobalData(m_ID, data);
}

// ------------------------------------------------------------------------------------------------
const SQChar * CModel::GetLocalTag() const noexcept
{
    return m_Tag.c_str();
}

void CModel::SetLocalTag(const SQChar * tag) noexcept
{
    m_Tag = tag;
}

// ------------------------------------------------------------------------------------------------
SqObj & CModel::GetLocalData() noexcept
{
    return m_Data;
}

void CModel::SetLocalData(SqObj & data) noexcept
{
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CModel::IsValid() const noexcept
{
    return (m_ID > 0);
}

// ------------------------------------------------------------------------------------------------
const SQChar * CModel::GetName() const noexcept
{
    return m_Name.c_str();
}

// ------------------------------------------------------------------------------------------------
bool CModel::IsWeapon() const noexcept
{
    return IsModelWeapon(m_ID);
}

bool CModel::IsActuallyWeapon() const noexcept
{
    return IsModelActuallyWeapon(m_ID);
}

// ------------------------------------------------------------------------------------------------
Reference< CObject > CModel::Object(SQInt32 world, const Vector3 & pos, SQInt32 alpha, SQInt32 header, \
                                    SqObj & payload) const noexcept
{
    return _Core->CreateObject(*this, world, pos, alpha, header, payload);
}

Reference< CObject > CModel::Object(SQInt32 world, SQFloat x, SQFloat y, SQFloat z, SQInt32 alpha, \
                                    SQInt32 header, SqObj & payload) const noexcept
{
    return _Core->CreateObject(*this, world, Vector3(x, y, z), alpha, header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CPickup > CModel::Pickup(SQInt32 world, SQInt32 quantity, const Vector3 & pos, SQInt32 alpha, \
                                    bool automatic, SQInt32 header, SqObj & payload) const noexcept
{
    return _Core->CreatePickup(*this, world, quantity, pos, alpha, automatic, header, payload);
}

Reference< CPickup > CModel::Pickup(SQInt32 world, SQInt32 quantity, SQFloat x, SQFloat y, SQFloat z, \
                                    SQInt32 alpha, bool automatic, SQInt32 header, SqObj & payload) const noexcept
{
    return _Core->CreatePickup(*this, world, quantity, Vector3(x, y, z), alpha, automatic, header, payload);
}

// ================================================================================================
bool Register_CModel(HSQUIRRELVM vm)
{
    // Output debugging information
    LogDbg("Beginning registration of <CModel> type");
    // Attempt to register the specified type
    Sqrat::RootTable(vm).Bind(_SC("CModel"), Sqrat::Class< CModel >(vm, _SC("CModel"))
        .Ctor()
        .Ctor< SQInt32 >()
        .Ctor< const SQChar *, SQInt32 >()

        .Func(_SC("_cmp"), &CModel::Cmp)
        .Func(_SC("_tostring"), &CModel::ToString)

        .Prop(_SC("id"), &CModel::GetID, &CModel::SetID)
        .Prop(_SC("gtag"), &CModel::GetGlobalTag, &CModel::SetGlobalTag)
        .Prop(_SC("gdata"), &CModel::GetGlobalData, &CModel::SetGlobalData)
        .Prop(_SC("ltag"), &CModel::GetLocalTag, &CModel::SetLocalTag)
        .Prop(_SC("ldata"), &CModel::GetLocalData, &CModel::SetLocalData)
        .Prop(_SC("valid"), &CModel::IsValid)
        .Prop(_SC("name"), &CModel::GetName)
        .Prop(_SC("weapon"), &CModel::IsWeapon)
        .Prop(_SC("truly_weapon"), &CModel::IsActuallyWeapon)

        .Func(_SC("setng"), &CModel::SetnGet)

        .Overload< Reference< CObject > (CModel::*)(SQInt32, const Vector3 &, SQInt32, SQInt32, SqObj &) const >(_SC("object"), &CModel::Object)
        .Overload< Reference< CObject > (CModel::*)(SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, SQInt32, SqObj &) const >(_SC("object"), &CModel::Object)
        .Overload< Reference< CPickup > (CModel::*)(SQInt32, SQInt32, const Vector3 &, SQInt32, bool, SQInt32, SqObj &) const >(_SC("pickup"), &CModel::Pickup)
        .Overload< Reference< CPickup > (CModel::*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, bool, SQInt32, SqObj &) const >(_SC("pickup"), &CModel::Pickup)
    );
    // Output debugging information
    LogDbg("Registration of <CModel> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
