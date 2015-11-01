#include "Misc/Skin.hpp"
#include "Register.hpp"
#include "Entity.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const CSkin CSkin::NIL = CSkin();

// ------------------------------------------------------------------------------------------------
CSkin::CSkin()
    : m_ID(SQMOD_UNKNOWN)
{

}

CSkin::CSkin(SQInt32 id)
    : m_ID(VALID_ENTITYGETEX(id, Max))
{

}

CSkin::CSkin(const SQChar * name, SQInt32 id)
    : m_ID(GetSkinID(name))
{
    if (VALID_ENTITYEX(m_ID, Max))
    {
        m_ID = id;
    }
}

// ------------------------------------------------------------------------------------------------
CSkin::CSkin(const CSkin & s)
    : m_ID(s.m_ID)
    , m_Tag(s.m_Tag)
    , m_Data(s.m_Data)
{

}

CSkin::CSkin(CSkin && s)
    : m_ID(s.m_ID)
    , m_Tag(s.m_Tag)
    , m_Data(s.m_Data)
{

}

// ------------------------------------------------------------------------------------------------
CSkin::~CSkin()
{

}

// ------------------------------------------------------------------------------------------------
CSkin & CSkin::operator = (const CSkin & s)
{
    m_ID = s.m_ID;
    m_Tag = s.m_Tag;
    m_Data = s.m_Data;

    return *this;
}

CSkin & CSkin::operator = (CSkin && s)
{
    m_ID = s.m_ID;
    m_Tag = s.m_Tag;
    m_Data = s.m_Data;

    return *this;
}

// ------------------------------------------------------------------------------------------------
CSkin & CSkin::operator = (SQInt32 id)
{
    m_ID = VALID_ENTITYGETEX(id, Max);

    return *this;
}

// ------------------------------------------------------------------------------------------------
bool CSkin::operator == (const CSkin & s) const
{
    return (m_ID == s.m_ID);
}

bool CSkin::operator != (const CSkin & s) const
{
    return (m_ID != s.m_ID);
}

bool CSkin::operator < (const CSkin & s) const
{
    return (m_ID < s.m_ID);
}

bool CSkin::operator > (const CSkin & s) const
{
    return (m_ID < s.m_ID);
}

bool CSkin::operator <= (const CSkin & s) const
{
    return (m_ID <= s.m_ID);
}

bool CSkin::operator >= (const CSkin & s) const
{
    return (m_ID >= s.m_ID);
}

// ------------------------------------------------------------------------------------------------
SQInteger CSkin::Cmp(const CSkin & s) const
{
    if (m_ID == s.m_ID)
    {
        return 0;
    }
    else if (m_ID > s.m_ID)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * CSkin::ToString() const
{
    return GetSkinName(m_ID);
}

// ------------------------------------------------------------------------------------------------
SQInteger CSkin::GetID() const
{
    return m_ID;
}

void CSkin::SetID(SQInt32 id)
{
    m_ID = VALID_ENTITYGETEX(id, Max);
}

// ------------------------------------------------------------------------------------------------
CSkin & CSkin::SetnGet(SQInt32 id)
{
    m_ID = VALID_ENTITYGETEX(id, Max);

    return *this;
}

// ------------------------------------------------------------------------------------------------
const SQChar * CSkin::GetGlobalTag() const
{
    return GlobalTag(m_ID);
}

void CSkin::SetGlobalTag(const SQChar * tag) const
{
    GlobalTag(m_ID, tag);
}

// ------------------------------------------------------------------------------------------------
SqObj & CSkin::GetGlobalData() const
{
    return GlobalData(m_ID);
}

void CSkin::SetGlobalData(SqObj & data) const
{
    GlobalData(m_ID, data);
}

// ------------------------------------------------------------------------------------------------
const SQChar * CSkin::GetLocalTag() const
{
    return m_Tag.c_str();
}

void CSkin::SetLocalTag(const SQChar * tag)
{
    m_Tag = tag;
}

// ------------------------------------------------------------------------------------------------
SqObj & CSkin::GetLocalData()
{
    return m_Data;
}

void CSkin::SetLocalData(SqObj & data)
{
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CSkin::IsValid() const
{
    return (m_ID > 0);
}

// ------------------------------------------------------------------------------------------------
const SQChar * CSkin::GetName() const
{
    return GetSkinName(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CSkin::SetName(const SQChar * name)
{
    m_ID = GetSkinID(name);
    m_ID = VALID_ENTITYGETEX(m_ID, Max);
}

// ------------------------------------------------------------------------------------------------
void CSkin::Apply(const Reference< CPlayer > & player) const
{
    if (player)
    {
        _Func->SetPlayerSkin(player, m_ID);
    }
    else if (!player)
    {
        LogErr(_SC("Attempting to <set player skin> using an invalid argument: {0:d}"), _SCI32(player));
    }
    else
    {
        LogErr(_SC("Attempting to <set player skin> using an invalid instance: {0:d}"), m_ID);
    }
}

// ================================================================================================
bool Register_CSkin(HSQUIRRELVM vm)
{
    // Output debugging information
    LogDbg("Beginning registration of <CSkin> type");
    // Attempt to register the specified type
    Sqrat::RootTable(vm).Bind(_SC("CSkin"), Sqrat::Class< CSkin >(vm, _SC("CSkin"))
        /* Constructors */
        .Ctor()
        .Ctor< SQInt32 >()
        .Ctor< const SQChar *, SQInt32 >()
        /* Metamethods */
        .Func(_SC("_cmp"), &CSkin::Cmp)
        .Func(_SC("_tostring"), &CSkin::ToString)
        /* Properties */
        .Prop(_SC("id"), &CSkin::GetID, &CSkin::SetID)
        .Prop(_SC("gtag"), &CSkin::GetGlobalTag, &CSkin::SetGlobalTag)
        .Prop(_SC("gdata"), &CSkin::GetGlobalData, &CSkin::SetGlobalData)
        .Prop(_SC("ltag"), &CSkin::GetLocalTag, &CSkin::SetLocalTag)
        .Prop(_SC("ldata"), &CSkin::GetLocalData, &CSkin::SetLocalData)
        .Prop(_SC("valid"), &CSkin::IsValid)
        .Prop(_SC("name"), &CSkin::GetName, &CSkin::SetName)
        /* Functions */
        .Func(_SC("setng"), &CSkin::SetnGet)
        .Func(_SC("apply"), &CSkin::Apply)
    );
    // Output debugging information
    LogDbg("Registration of <CSkin> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
