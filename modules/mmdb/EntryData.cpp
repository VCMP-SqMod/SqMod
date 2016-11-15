// ------------------------------------------------------------------------------------------------
#include "EntryData.hpp"
#include "Database.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger EntryData::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMEntryData");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void EntryData::Validate(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid Maxmind database reference =>[%s:%d]", file, line);
    }
}
#else
void EntryData::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid Maxmind database reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
EntryData::ConstRef EntryData::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    // See if the entry has any data
    if (!m_Entry.has_data)
    {
        SqThrowF("The referenced entry has no data =>[%s:%d]", file, line);
    }
    // Return the entry
    return m_Entry;
}
#else
EntryData::ConstRef EntryData::GetValid() const
{
    Validate();
    // See if the entry has any data
    if (!m_Entry.has_data)
    {
        SqThrowF("The referenced entry has no data");
    }
    // Return the entry
    return m_Entry;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
EntryData::EntryData()
    : m_Handle(), m_Entry()
{
    std::memset(&m_Entry, 0, sizeof(Type));
}

// ------------------------------------------------------------------------------------------------
void EntryData::Release()
{
    std::memset(&m_Entry, 0, sizeof(Type));
    m_Handle.Reset();
}

// ------------------------------------------------------------------------------------------------
Database EntryData::GetDatabase() const
{
    return Database(m_Handle);
}

// ================================================================================================
void Register_EntryData(Table & mmns)
{
    mmns.Bind(_SC("EntryData"),
        Class< EntryData >(mmns.GetVM(), _SC("SqMMEntryData"))
        // Constructors
        .Ctor()
        .Ctor< const EntryData & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &EntryData::Typename)
        .Func(_SC("_tostring"), &EntryData::ToString)
        // Properties
        .Prop(_SC("IsValid"), &EntryData::IsValid)
        .Prop(_SC("Database"), &EntryData::GetDatabase)
        .Prop(_SC("References"), &EntryData::GetRefCount)
        .Prop(_SC("TypeName"), &EntryData::TypeName)
        .Prop(_SC("HasData"), &EntryData::HasData)
        .Prop(_SC("Type"), &EntryData::GetType)
        .Prop(_SC("Offset"), &EntryData::GetOffset)
        .Prop(_SC("DataSize"), &EntryData::DataSize)
        .Prop(_SC("String"), &EntryData::GetString)
        .Prop(_SC("Integer"), &EntryData::GetInteger)
        .Prop(_SC("Float"), &EntryData::GetFloat)
        .Prop(_SC("Long"), &EntryData::GetLong)
        .Prop(_SC("Bool"), &EntryData::GetBool)
        .Prop(_SC("Bytes"), &EntryData::GetBytes)
        // Member methods
        .Func(_SC("Release"), &EntryData::Release)
    );
}

} // Namespace:: SqMod
