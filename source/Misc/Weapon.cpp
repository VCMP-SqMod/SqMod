#include "Misc/Weapon.hpp"
#include "Entity.hpp"
#include "Register.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const CWeapon CWeapon::NIL = CWeapon();

// ------------------------------------------------------------------------------------------------
CWeapon::CWeapon()
    : m_ID(SQMOD_UNKNOWN), m_Ammo(0)
{

}

CWeapon::CWeapon(SQInt32 id)
    : CWeapon(id, 0)
{

}

CWeapon::CWeapon(SQInt32 id, SQInt32 ammo)
    : m_ID(VALID_ENTITYGETEX(id, Max)), m_Ammo(ammo)
{

}

CWeapon::CWeapon(const SQChar * name, SQInt32 id, SQInt32 ammo)
    : m_ID(GetWeaponID(name)), m_Ammo(ammo)
{
    if (VALID_ENTITYEX(m_ID, Max))
    {
        m_ID = id;
    }
}

// ------------------------------------------------------------------------------------------------
CWeapon::CWeapon(const CWeapon & w)
    : m_ID(w.m_ID)
    , m_Ammo(w.m_Ammo)
    , m_Tag(w.m_Tag)
    , m_Data(w.m_Data)
{

}

CWeapon::CWeapon(CWeapon && w)
    : m_ID(w.m_ID)
    , m_Ammo(w.m_Ammo)
    , m_Tag(w.m_Tag)
    , m_Data(w.m_Data)
{

}

// ------------------------------------------------------------------------------------------------
CWeapon::~CWeapon()
{

}

// ------------------------------------------------------------------------------------------------
CWeapon & CWeapon::operator = (const CWeapon & w)
{
    m_ID = w.m_ID;
    m_Ammo = w.m_Ammo;
    m_Tag = w.m_Tag;
    m_Data = w.m_Data;

    return *this;
}

CWeapon & CWeapon::operator = (CWeapon && w)
{
    m_ID = w.m_ID;
    m_Ammo = w.m_Ammo;
    m_Tag = w.m_Tag;
    m_Data = w.m_Data;

    return *this;
}

// ------------------------------------------------------------------------------------------------
CWeapon & CWeapon::operator = (SQInt32 id)
{
    m_ID = VALID_ENTITYGETEX(id, Max);

    return *this;
}

// ------------------------------------------------------------------------------------------------
bool CWeapon::operator == (const CWeapon & w) const
{
    return (m_ID == w.m_ID);
}

bool CWeapon::operator != (const CWeapon & w) const
{
    return (m_ID != w.m_ID);
}

bool CWeapon::operator < (const CWeapon & w) const
{
    return (m_ID < w.m_ID);
}

bool CWeapon::operator > (const CWeapon & w) const
{
    return (m_ID < w.m_ID);
}

bool CWeapon::operator <= (const CWeapon & w) const
{
    return (m_ID <= w.m_ID);
}

bool CWeapon::operator >= (const CWeapon & w) const
{
    return (m_ID >= w.m_ID);
}

// ------------------------------------------------------------------------------------------------
SQInteger CWeapon::Cmp(const CWeapon & w) const
{
    if (m_ID == w.m_ID)
    {
        return 0;
    }
    else if (m_ID > w.m_ID)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * CWeapon::ToString() const
{
    return GetWeaponName(m_ID);
}

// ------------------------------------------------------------------------------------------------
SQInteger CWeapon::GetID() const
{
    return m_ID;
}

void CWeapon::SetID(SQInt32 id)
{
    m_ID = VALID_ENTITYGETEX(id, Max);
}

// ------------------------------------------------------------------------------------------------
CWeapon & CWeapon::SetnGet(SQInt32 id)
{
    m_ID = VALID_ENTITYGETEX(id, Max);

    return *this;
}

// ------------------------------------------------------------------------------------------------
const SQChar * CWeapon::GetGlobalTag() const
{
    return GlobalTag(m_ID);
}

void CWeapon::SetGlobalTag(const SQChar * tag) const
{
    GlobalTag(m_ID, tag);
}

// ------------------------------------------------------------------------------------------------
SqObj & CWeapon::GetGlobalData() const
{
    return GlobalData(m_ID);
}

void CWeapon::SetGlobalData(SqObj & data) const
{
    GlobalData(m_ID, data);
}

// ------------------------------------------------------------------------------------------------
const SQChar * CWeapon::GetLocalTag() const
{
    return m_Tag.c_str();
}

void CWeapon::SetLocalTag(const SQChar * tag)
{
    m_Tag = tag;
}

// ------------------------------------------------------------------------------------------------
SqObj & CWeapon::GetLocalData()
{
    return m_Data;
}

void CWeapon::SetLocalData(SqObj & data)
{
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CWeapon::IsValid() const
{
    return (VALID_ENTITYEX(m_ID, Max));
}

// ------------------------------------------------------------------------------------------------
const SQChar * CWeapon::GetName() const
{
    return GetWeaponName(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CWeapon::SetName(const SQChar * name)
{
    m_ID = GetWeaponID(name);
    m_ID = VALID_ENTITYGETEX(m_ID, Max);
}

// ------------------------------------------------------------------------------------------------
SQInteger CWeapon::GetAmmo() const
{
    return m_Ammo;
}

void CWeapon::SetAmmo(SQInt32 amount)
{
    m_Ammo = amount;
}

// ------------------------------------------------------------------------------------------------
bool CWeapon::IsNatural() const
{
    return IsWeaponNatural(m_ID);
}

// ------------------------------------------------------------------------------------------------
SQFloat CWeapon::GetDataValue(SQInt32 field) const
{
    return _Func->GetWeaponDataValue(m_ID, field);
}

void CWeapon::SetDataValue(SQInt32 field, SQFloat value) const
{
    _Func->SetWeaponDataValue(m_ID, field, value);
}

// ------------------------------------------------------------------------------------------------
void CWeapon::ResetData() const
{
    _Func->ResetWeaponData(m_ID);
}

void CWeapon::ResetData(SQInt32 field) const
{
    _Func->ResetWeaponDataValue(m_ID, field);
}

// ------------------------------------------------------------------------------------------------
bool CWeapon::IsDataModified(SQInt32 field) const
{
    return _Func->IsWeaponDataValueModified(m_ID, field);
}

// ------------------------------------------------------------------------------------------------
void CWeapon::SetOn(const Reference< CPlayer > & player) const
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

void CWeapon::GiveTo(const Reference< CPlayer > & player) const
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

void CWeapon::SetOn(const Reference< CPlayer > & player, SQInt32 ammo) const
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

void CWeapon::GiveTo(const Reference< CPlayer > & player, SQInt32 ammo) const
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
        /* Constructors */
        .Ctor()
        .Ctor< SQInt32 >()
        .Ctor< SQInt32, SQInt32 >()
        .Ctor< const SQChar *, SQInt32, SQInt32 >()
        /* Metamethods */
        .Func(_SC("_cmp"), &CWeapon::Cmp)
        /* Properties */
        .Prop(_SC("id"), &CWeapon::GetID, &CWeapon::SetID)
        .Prop(_SC("gtag"), &CWeapon::GetGlobalTag, &CWeapon::SetGlobalTag)
        .Prop(_SC("gdata"), &CWeapon::GetGlobalData, &CWeapon::SetGlobalData)
        .Prop(_SC("ltag"), &CWeapon::GetLocalTag, &CWeapon::SetLocalTag)
        .Prop(_SC("ldata"), &CWeapon::GetLocalData, &CWeapon::SetLocalData)
        .Prop(_SC("name"), &CWeapon::GetName, &CWeapon::SetName)
        .Prop(_SC("valid"), &CWeapon::IsValid)
        .Prop(_SC("ammo"), &CWeapon::GetAmmo, &CWeapon::SetAmmo)
        .Prop(_SC("natural"), &CWeapon::IsNatural)
        /* Functions */
        .Func(_SC("setng"), &CWeapon::SetnGet)
        .Func(_SC("get_value"), &CWeapon::GetDataValue)
        .Func(_SC("set_value"), &CWeapon::SetDataValue)
        .Func(_SC("is_modified"), &CWeapon::IsDataModified)
        /* Overloads */
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
