#include "Misc/Automobile.hpp"
#include "Base/Color3.hpp"
#include "Base/Vector3.hpp"
#include "Entity.hpp"
#include "Register.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const CAutomobile CAutomobile::NIL;

// ------------------------------------------------------------------------------------------------
CAutomobile::CAutomobile() noexcept
    : m_ID(SQMOD_UNKNOWN)
{

}

CAutomobile::CAutomobile(SQInt32 id) noexcept
    : m_ID(VALID_ENTITYGETEX(id, Max))
{

}

CAutomobile::CAutomobile(const SQChar * name, SQInt32 id) noexcept
    : m_ID(GetAutomobileID(name))
{
    if (VALID_ENTITYEX(m_ID, Max))
    {
        m_ID = id;
    }
}

// ------------------------------------------------------------------------------------------------
CAutomobile::CAutomobile(const CAutomobile & a) noexcept
    : m_ID(a.m_ID)
    , m_Tag(a.m_Tag)
    , m_Data(a.m_Data)
{

}

CAutomobile::CAutomobile(CAutomobile && a) noexcept
    : m_ID(a.m_ID)
    , m_Tag(a.m_Tag)
    , m_Data(a.m_Data)
{

}

// ------------------------------------------------------------------------------------------------
CAutomobile::~CAutomobile()
{

}

// ------------------------------------------------------------------------------------------------
CAutomobile & CAutomobile::operator = (const CAutomobile & a) noexcept
{
    m_ID = a.m_ID;
    m_Tag = a.m_Tag;
    m_Data = a.m_Data;

    return *this;
}

CAutomobile & CAutomobile::operator = (CAutomobile && a) noexcept
{
    m_ID = a.m_ID;
    m_Tag = a.m_Tag;
    m_Data = a.m_Data;

    return *this;
}

// ------------------------------------------------------------------------------------------------
CAutomobile & CAutomobile::operator = (SQInt32 id) noexcept
{
    m_ID = VALID_ENTITYGETEX(id, Max);

    return *this;
}

// ------------------------------------------------------------------------------------------------
bool CAutomobile::operator == (const CAutomobile & a) const noexcept
{
    return (m_ID == a.m_ID);
}

bool CAutomobile::operator != (const CAutomobile & a) const noexcept
{
    return (m_ID != a.m_ID);
}

bool CAutomobile::operator < (const CAutomobile & a) const noexcept
{
    return (m_ID < a.m_ID);
}

bool CAutomobile::operator > (const CAutomobile & a) const noexcept
{
    return (m_ID < a.m_ID);
}

bool CAutomobile::operator <= (const CAutomobile & a) const noexcept
{
    return (m_ID <= a.m_ID);
}

bool CAutomobile::operator >= (const CAutomobile & a) const noexcept
{
    return (m_ID >= a.m_ID);
}

// ------------------------------------------------------------------------------------------------
SQInteger CAutomobile::Cmp(const CAutomobile & a) const noexcept
{
    if (m_ID == a.m_ID)
    {
        return 0;
    }
    else if (m_ID > a.m_ID)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * CAutomobile::ToString() const noexcept
{
    return GetAutomobileName(m_ID);
}

// ------------------------------------------------------------------------------------------------
SQInteger CAutomobile::GetID() const noexcept
{
    return m_ID;
}

void CAutomobile::SetID(SQInt32 id) noexcept
{
    m_ID = VALID_ENTITYGETEX(id, Max);
}

// ------------------------------------------------------------------------------------------------
CAutomobile & CAutomobile::SetnGet(SQInt32 id) noexcept
{
    m_ID = VALID_ENTITYGETEX(id, Max);

    return *this;
}

// ------------------------------------------------------------------------------------------------
const SQChar * CAutomobile::GetGlobalTag() const noexcept
{
    return GlobalTag(m_ID);
}

void CAutomobile::SetGlobalTag(const SQChar * tag) const noexcept
{
    GlobalTag(m_ID, tag);
}

// ------------------------------------------------------------------------------------------------
SqObj & CAutomobile::GetGlobalData() const noexcept
{
    return GlobalData(m_ID);
}

void CAutomobile::SetGlobalData(SqObj & data) const noexcept
{
    GlobalData(m_ID, data);
}

// ------------------------------------------------------------------------------------------------
const SQChar * CAutomobile::GetLocalTag() const noexcept
{
    return m_Tag.c_str();
}

void CAutomobile::SetLocalTag(const SQChar * tag) noexcept
{
    m_Tag = tag;
}

// ------------------------------------------------------------------------------------------------
SqObj & CAutomobile::GetLocalData() noexcept
{
    return m_Data;
}

void CAutomobile::SetLocalData(SqObj & data) noexcept
{
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CAutomobile::IsValid() const noexcept
{
    return (VALID_ENTITYEX(m_ID, Max));
}

// ------------------------------------------------------------------------------------------------
const SQChar * CAutomobile::GetName() const noexcept
{
    return GetAutomobileName(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CAutomobile::SetName(const SQChar * name) noexcept
{
    m_ID = GetAutomobileID(name);
    m_ID = VALID_ENTITYGETEX(m_ID, Max);
}

// ------------------------------------------------------------------------------------------------
Reference< CVehicle > CAutomobile::Create(SQInt32 world, const Vector3 & pos, SQFloat angle,
                                            SQInt32 header, SqObj & payload) const noexcept
{
    return _Core->CreateVehicle(*this, world, pos, angle, SQMOD_UNKNOWN, SQMOD_UNKNOWN, header, payload);
}

Reference< CVehicle > CAutomobile::Create(SQInt32 world, const Vector3 & pos, SQFloat angle,
                                            SQInt32 primary, SQInt32 secondary, SQInt32 header,
                                            SqObj & payload) const noexcept
{
    return _Core->CreateVehicle(*this, world, pos, angle, primary, secondary, header, payload);
}

Reference< CVehicle > CAutomobile::Create(SQInt32 world, SQFloat x, SQFloat y, SQFloat z, SQFloat angle,
                                            SQInt32 header, SqObj & payload) const noexcept
{
    return _Core->CreateVehicle(*this, world, Vector3(x, y, z), angle, SQMOD_UNKNOWN, SQMOD_UNKNOWN, header, payload);
}

Reference< CVehicle > CAutomobile::Create(SQInt32 world, SQFloat x, SQFloat y, SQFloat z, SQFloat angle,
                                            SQInt32 primary, SQInt32 secondary, SQInt32 header,
                                            SqObj & payload) const noexcept
{
    return _Core->CreateVehicle(*this, world, Vector3(x, y, z), angle, primary, secondary, header, payload);
}

// ================================================================================================
bool Register_CAutomobile(HSQUIRRELVM vm)
{
    // Output debugging information
    LogDbg("Beginning registration of <CAutomobile> type");
    // Attempt to register the specified type
    Sqrat::RootTable(vm).Bind(_SC("CAutomobile"), Sqrat::Class< CAutomobile >(vm, _SC("CAutomobile"))
        /* Constructors */
        .Ctor()
        .Ctor< SQInt32 >()
        .Ctor< const SQChar *, SQInt32 >()
        /* Metamethods */
        .Func(_SC("_cmp"), &CAutomobile::Cmp)
        .Func(_SC("_tostring"), &CAutomobile::ToString)
        /* Properties */
        .Prop(_SC("id"), &CAutomobile::GetID, &CAutomobile::SetID)
        .Prop(_SC("gtag"), &CAutomobile::GetGlobalTag, &CAutomobile::SetGlobalTag)
        .Prop(_SC("gdata"), &CAutomobile::GetGlobalData, &CAutomobile::SetGlobalData)
        .Prop(_SC("ltag"), &CAutomobile::GetLocalTag, &CAutomobile::SetLocalTag)
        .Prop(_SC("ldata"), &CAutomobile::GetLocalData, &CAutomobile::SetLocalData)
        .Prop(_SC("valid"), &CAutomobile::IsValid)
        .Prop(_SC("name"), &CAutomobile::GetName, &CAutomobile::SetName)
        /* Functions */
        .Func(_SC("setng"), &CAutomobile::SetnGet)
        /* Overloads */
        .Overload< Reference< CVehicle > (CAutomobile::*)(SQInt32, const Vector3 &, SQFloat, SQInt32, SqObj &) const >
            (_SC("vehicle"), &CAutomobile::Create)
        .Overload< Reference< CVehicle > (CAutomobile::*)(SQInt32, const Vector3 &, SQFloat, SQInt32, SQInt32, SQInt32, SqObj &) const >
            (_SC("vehicle"), &CAutomobile::Create)
        .Overload< Reference< CVehicle > (CAutomobile::*)(SQInt32, SQFloat, SQFloat, SQFloat, SQFloat, SQInt32, SqObj &) const >
            (_SC("vehicle"), &CAutomobile::Create)
        .Overload< Reference< CVehicle > (CAutomobile::*)(SQInt32, SQFloat, SQFloat, SQFloat, SQFloat, SQInt32, SQInt32, SQInt32, SqObj &) const >
            (_SC("vehicle"), &CAutomobile::Create)
    );
    // Output debugging information
    LogDbg("Registration of <CAutomobile> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
