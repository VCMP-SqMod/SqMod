#include "Misc/Weapon.hpp"
#include "Entity.hpp"
#include "Register.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const CWeapon CWeapon::NIL = CWeapon();

// ------------------------------------------------------------------------------------------------
CWeapon::CWeapon() noexcept
    : m_ID(SQMOD_UNKNOWN), m_Ammo(0), m_Name()
{

}

CWeapon::CWeapon(SQInt32 id) noexcept
    : CWeapon(id, 0)
{

}

CWeapon::CWeapon(SQInt32 id, SQInt32 ammo) noexcept
    : m_ID(VALID_ENTITYGETEX(id, Max)), m_Ammo(ammo), m_Name(GetWeaponName(id))
{

}

CWeapon::CWeapon(const SQChar * name, SQInt32 id, SQInt32 ammo) noexcept
    : CWeapon((IsWeaponValid(GetWeaponID(name)) ? GetWeaponID(name) : id), ammo)
{

}

// ------------------------------------------------------------------------------------------------
CWeapon::CWeapon(const CWeapon & w) noexcept
    : m_ID(w.m_ID)
    , m_Ammo(w.m_Ammo)
    , m_Name(w.m_Name)
    , m_Tag(w.m_Tag)
    , m_Data(w.m_Data)
{

}

CWeapon::CWeapon(CWeapon && w) noexcept
    : m_ID(w.m_ID)
    , m_Ammo(w.m_Ammo)
    , m_Name(w.m_Name)
    , m_Tag(w.m_Tag)
    , m_Data(w.m_Data)
{

}

// ------------------------------------------------------------------------------------------------
CWeapon::~CWeapon()
{

}

// ------------------------------------------------------------------------------------------------
CWeapon & CWeapon::operator = (const CWeapon & w) noexcept
{
    m_ID = w.m_ID;
    m_Ammo = w.m_Ammo;
    m_Name = w.m_Name;
    m_Tag = w.m_Tag;
    m_Data = w.m_Data;

    return *this;
}

CWeapon & CWeapon::operator = (CWeapon && w) noexcept
{
    m_ID = w.m_ID;
    m_Ammo = w.m_Ammo;
    m_Name = w.m_Name;
    m_Tag = w.m_Tag;
    m_Data = w.m_Data;

    return *this;
}

// ------------------------------------------------------------------------------------------------
CWeapon & CWeapon::operator = (SQInt32 id) noexcept
{
    if (m_ID != id)
    {
        m_Name = GetWeaponName(id);
        m_ID = id;
    }

    return *this;
}

// ------------------------------------------------------------------------------------------------
bool CWeapon::operator == (const CWeapon & w) const noexcept
{
    return (m_ID == w.m_ID);
}

bool CWeapon::operator != (const CWeapon & w) const noexcept
{
    return (m_ID != w.m_ID);
}

bool CWeapon::operator < (const CWeapon & w) const noexcept
{
    return (m_ID < w.m_ID);
}

bool CWeapon::operator > (const CWeapon & w) const noexcept
{
    return (m_ID < w.m_ID);
}

bool CWeapon::operator <= (const CWeapon & w) const noexcept
{
    return (m_ID <= w.m_ID);
}

bool CWeapon::operator >= (const CWeapon & w) const noexcept
{
    return (m_ID >= w.m_ID);
}

// ------------------------------------------------------------------------------------------------
SQInteger CWeapon::Cmp(const CWeapon & w) const noexcept
{
    return m_ID == w.m_ID ? 0 : (m_ID > w.m_ID ? 1 : -1);
}

// ------------------------------------------------------------------------------------------------
const SQChar * CWeapon::ToString() const noexcept
{
    return m_Name.c_str();
}

// ------------------------------------------------------------------------------------------------
SQInteger CWeapon::GetID() const noexcept
{
    return m_ID;
}

void CWeapon::SetID(SQInt32 id) noexcept
{
    if (m_ID != id)
    {
        m_Name = GetWeaponName(id);
        m_ID = id;
    }
}

// ------------------------------------------------------------------------------------------------
CWeapon & CWeapon::SetnGet(SQInt32 id) noexcept
{
    if (m_ID != id)
    {
        m_Name = GetWeaponName(id);
        m_ID = id;
    }

    return *this;
}

// ------------------------------------------------------------------------------------------------
const SQChar * CWeapon::GetGlobalTag() const noexcept
{
    return GlobalTag(m_ID);
}

void CWeapon::SetGlobalTag(const SQChar * tag) const noexcept
{
    GlobalTag(m_ID, tag);
}

// ------------------------------------------------------------------------------------------------
SqObj & CWeapon::GetGlobalData() const noexcept
{
    return GlobalData(m_ID);
}

void CWeapon::SetGlobalData(SqObj & data) const noexcept
{
    GlobalData(m_ID, data);
}

// ------------------------------------------------------------------------------------------------
const SQChar * CWeapon::GetLocalTag() const noexcept
{
    return m_Tag.c_str();
}

void CWeapon::SetLocalTag(const SQChar * tag) noexcept
{
    m_Tag = tag;
}

// ------------------------------------------------------------------------------------------------
SqObj & CWeapon::GetLocalData() noexcept
{
    return m_Data;
}

void CWeapon::SetLocalData(SqObj & data) noexcept
{
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CWeapon::IsValid() const noexcept
{
    return (m_ID > 0);
}

// ------------------------------------------------------------------------------------------------
const SQChar * CWeapon::GetName() const noexcept
{
    return m_Name.c_str();
}

// ------------------------------------------------------------------------------------------------
SQInteger CWeapon::GetAmmo() const noexcept
{
    return m_Ammo;
}

void CWeapon::SetAmmo(SQInt32 amount) noexcept
{
    m_Ammo = amount;
}

// ------------------------------------------------------------------------------------------------
bool CWeapon::IsNatural() const noexcept
{
    return IsWeaponNatural(m_ID);
}

// ------------------------------------------------------------------------------------------------
SQFloat CWeapon::GetDataValue(SQInt32 field) const noexcept
{
    return _Func->GetWeaponDataValue(m_ID, field);
}

void CWeapon::SetDataValue(SQInt32 field, SQFloat value) const noexcept
{
    _Func->SetWeaponDataValue(m_ID, field, value);
}

// ------------------------------------------------------------------------------------------------
void CWeapon::ResetData() const noexcept
{
    _Func->ResetWeaponData(m_ID);
}

void CWeapon::ResetData(SQInt32 field) const noexcept
{
    _Func->ResetWeaponDataValue(m_ID, field);
}

// ------------------------------------------------------------------------------------------------
bool CWeapon::IsDataModified(SQInt32 field) const noexcept
{
    return _Func->IsWeaponDataValueModified(m_ID, field);
}

// ------------------------------------------------------------------------------------------------
void CWeapon::SetOn(const Reference< CPlayer > & player) const noexcept
{
    if (*this && player)
    {
        _Func->SetPlayerWeapon(player, m_ID, m_Ammo);
    }
    else if (!player)
    {
        LogErr(_SC("Attempting to <set player weapon> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogErr(_SC("Attempting to <set player weapon> using an invalid instance: %d"), m_ID);
    }
}

void CWeapon::GiveTo(const Reference< CPlayer > & player) const noexcept
{
    if (*this && player)
    {
        _Func->GivePlayerWeapon(player, m_ID, m_Ammo);
    }
    else if (!player)
    {
        LogErr(_SC("Attempting to <give player weapon> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogErr(_SC("Attempting to <give player weapon> using an invalid instance: %d"), m_ID);
    }
}

void CWeapon::SetOn(const Reference< CPlayer > & player, SQInt32 ammo) const noexcept
{
    if (*this && player)
    {
        _Func->SetPlayerWeapon(player, m_ID, ammo);
    }
    else if (!player)
    {
        LogErr(_SC("Attempting to <set player weapon> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogErr(_SC("Attempting to <set player weapon> using an invalid instance: %d"), m_ID);
    }
}

void CWeapon::GiveTo(const Reference< CPlayer > & player, SQInt32 ammo) const noexcept
{
    if (*this && player)
    {
        _Func->GivePlayerWeapon(player, m_ID, ammo);
    }
    else if (!player)
    {
        LogErr(_SC("Attempting to <give player weapon> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogErr(_SC("Attempting to <give player weapon> using an invalid instance: %d"), m_ID);
    }
}

// ================================================================================================
bool Register_CWeapon(HSQUIRRELVM vm)
{
    // Output debugging information
    LogDbg("Beginning registration of <CWeapon> type");
    // Attempt to register the specified type
    Sqrat::RootTable(vm).Bind(_SC("CWeapon"), Sqrat::Class< CWeapon >(vm, _SC("CWeapon"))
        .Ctor()
        .Ctor< SQInt32 >()
        .Ctor< SQInt32, SQInt32 >()
        .Ctor< const SQChar *, SQInt32, SQInt32 >()

        .Func(_SC("_cmp"), &CWeapon::Cmp)

        .Prop(_SC("id"), &CWeapon::GetID, &CWeapon::SetID)
        .Prop(_SC("gtag"), &CWeapon::GetGlobalTag, &CWeapon::SetGlobalTag)
        .Prop(_SC("gdata"), &CWeapon::GetGlobalData, &CWeapon::SetGlobalData)
        .Prop(_SC("ltag"), &CWeapon::GetLocalTag, &CWeapon::SetLocalTag)
        .Prop(_SC("ldata"), &CWeapon::GetLocalData, &CWeapon::SetLocalData)
        .Prop(_SC("valid"), &CWeapon::IsValid)
        .Prop(_SC("name"), &CWeapon::GetName)
        .Prop(_SC("ammo"), &CWeapon::GetAmmo, &CWeapon::SetAmmo)
        .Prop(_SC("natural"), &CWeapon::IsNatural)

        .Func(_SC("setng"), &CWeapon::SetnGet)
        .Func(_SC("get_value"), &CWeapon::GetDataValue)
        .Func(_SC("set_value"), &CWeapon::SetDataValue)
        .Func(_SC("is_modified"), &CWeapon::IsDataModified)

        .Overload< void (CWeapon::*)(void) const >(_SC("reset_data"), &CWeapon::ResetData)
        .Overload< void (CWeapon::*)(SQInt32) const >(_SC("reset_data"), &CWeapon::ResetData)
        .Overload< void (CWeapon::*)(const Reference< CPlayer > &) const >(_SC("set_on"), &CWeapon::SetOn)
        .Overload< void (CWeapon::*)(const Reference< CPlayer > &, SQInt32) const >(_SC("set_on"), &CWeapon::SetOn)
        .Overload< void (CWeapon::*)(const Reference< CPlayer > &) const >(_SC("give_to"), &CWeapon::GiveTo)
        .Overload< void (CWeapon::*)(const Reference< CPlayer > &, SQInt32) const >(_SC("give_to"), &CWeapon::GiveTo)
    );
    // Output debugging information
    LogDbg("Registration of <CWeapon> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
