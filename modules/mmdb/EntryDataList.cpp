// ------------------------------------------------------------------------------------------------
#include "EntryDataList.hpp"
#include "Database.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger EntryDataList::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMEntryDataList");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void EntryDataList::Validate(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid Maxmind database reference =>[%s:%d]", file, line);
    }
}
#else
void EntryDataList::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid Maxmind database reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
EntryDataList::Pointer EntryDataList::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    // Validate the managed list
    if (!m_List)
    {
        SqThrowF("Invalid Maxmind entry data list reference =>[%s:%d]", file, line);
    }
    // return the list
    return m_List;
}
#else
EntryDataList::Pointer EntryDataList::GetValid() const
{
    Validate();
    // Validate the managed list
    if (!m_List)
    {
        SqThrowF("Invalid Maxmind entry data list reference");
    }
    // return the list
    return m_List;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
EntryDataList::Pointer EntryDataList::GetValidElem(CCStr file, Int32 line) const
{
    Validate(file, line);
    // Validate the current element
    if (!m_List)
    {
        SqThrowF("Invalid Maxmind entry data element reference =>[%s:%d]", file, line);
    }
    // return the element
    return m_Elem;
}
#else
EntryDataList::Pointer EntryDataList::GetValidElem() const
{
    Validate();
    // Validate the current element
    if (!m_List)
    {
        SqThrowF("Invalid Maxmind entry data element reference");
    }
    // return the element
    return m_Elem;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
Database EntryDataList::GetDatabase() const
{
    return Database(m_Handle);
}

// ------------------------------------------------------------------------------------------------
Uint32 EntryDataList::GetCount() const
{
    // Prepare a counter
    Uint32 count = 0;
    // Do we even have a list?
    if (m_List)
    {
        for (Pointer elem = m_List; elem; elem = elem->next)
        {
            ++count;
        }
    }
    // Return the counter
    return count;
}

// ------------------------------------------------------------------------------------------------
bool EntryDataList::Next()
{
    // Validate the database handle
    SQMOD_VALIDATE(*this);
    // Attempt to fetch the next element
    m_Elem = m_Elem ? m_Elem->next : nullptr;
    // Return whether we have a valid element
    return !!m_Elem;
}

// ------------------------------------------------------------------------------------------------
bool EntryDataList::Advance(SQInteger n)
{
    // Validate the database handle
    SQMOD_VALIDATE(*this);
    // Attempt to skip as many elements as possible
    while (n && m_Elem)
    {
        // Fetch the next element
        m_Elem = m_Elem->next;
        // Decrease the counter
        --n;
    }
    // Return whether we have a valid element
    return m_Elem;
}

// ------------------------------------------------------------------------------------------------
void EntryDataList::DumpTo(CSStr filepath, Int32 indent) const
{
    // Validate the database and list handle
    SQMOD_VALIDATE(*this);
    // Validate the specified file path
    if (!filepath || *filepath == '\0')
    {
        STHROWF("Invalid file path");
    }
    // Attempt to open the specified file
    FILE * fp = fopen(filepath, "w");
    // Validate the file handle
    if (!fp)
    {
        STHROWF("Unable to open file %s", filepath);
    }
    // Attempt to dump the entry data list
    Int32 status = MMDB_dump_entry_data_list(fp, m_List, indent);
    // Close the file handle
    fclose(fp);
    // Validate the result of the operation
    if (status != MMDB_SUCCESS)
    {
        STHROWF("Unable to dump the list [%s]", MMDB_strerror(status));
    }
}

// ================================================================================================
void Register_EntryDataList(Table & mmns)
{
    mmns.Bind(_SC("EntryDataList"),
        Class< EntryDataList, NoCopy< EntryDataList > >(mmns.GetVM(), _SC("SqMMEntryDataList"))
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &EntryDataList::Typename)
        .Func(_SC("_tostring"), &EntryDataList::ToString)
        // Properties
        .Prop(_SC("IsValid"), &EntryDataList::IsValid)
        .Prop(_SC("Database"), &EntryDataList::GetDatabase)
        .Prop(_SC("References"), &EntryDataList::GetRefCount)
        .Prop(_SC("HaveElement"), &EntryDataList::HaveElement)
        .Prop(_SC("TypeName"), &EntryDataList::TypeName)
        .Prop(_SC("Count"), &EntryDataList::GetCount)
        .Prop(_SC("HasData"), &EntryDataList::HasData)
        .Prop(_SC("Type"), &EntryDataList::GetType)
        .Prop(_SC("Offset"), &EntryDataList::GetOffset)
        .Prop(_SC("DataSize"), &EntryDataList::DataSize)
        .Prop(_SC("String"), &EntryDataList::GetString)
        .Prop(_SC("Integer"), &EntryDataList::GetInteger)
        .Prop(_SC("Float"), &EntryDataList::GetFloat)
        .Prop(_SC("Long"), &EntryDataList::GetLong)
        .Prop(_SC("Bool"), &EntryDataList::GetBool)
        .Prop(_SC("Bytes"), &EntryDataList::GetBytes)
        // Member methods
        .Func(_SC("Release"), &EntryDataList::Release)
        .Func(_SC("Next"), &EntryDataList::Next)
        .Func(_SC("Advance"), &EntryDataList::Advance)
        .Func(_SC("Reset"), &EntryDataList::Reset)
        // Member Overloads
        .Overload< void (EntryDataList::*)(CSStr) const >(_SC("DumpTo"), &EntryDataList::DumpTo)
        .Overload< void (EntryDataList::*)(CSStr, Int32) const >(_SC("DumpTo"), &EntryDataList::DumpTo)
    );
}

} // Namespace:: SqMod
