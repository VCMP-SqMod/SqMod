// ------------------------------------------------------------------------------------------------
#include "Library/INI.hpp"

// ------------------------------------------------------------------------------------------------
#include <errno.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Int32 IniEntries::Cmp(const IniEntries & o) const
{
    if (m_Elem == o.m_Elem)
        return 0;
    else if (this > &o)
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
CSStr IniEntries::ToString() const
{
    return GetItem();
}

// ------------------------------------------------------------------------------------------------
void IniEntries::Next()
{
    // Are there any other elements ahead?
    if (!m_List.empty() && m_Elem != m_List.end())
        ++m_Elem; /* Go ahead one element */
}

// ------------------------------------------------------------------------------------------------
void IniEntries::Prev()
{
    // Are there any other elements behind?
    if (!m_List.empty() && m_Elem != m_List.begin())
        --m_Elem; /* Go back one element */
}

// ------------------------------------------------------------------------------------------------
void IniEntries::Advance(Int32 n)
{
    // Are there any other elements ahead?
    if (m_List.empty() || m_Elem == m_List.end())
        return;
    // Jump as many elements as possible
    while ((--n >= 0) && m_Elem != m_List.end()) ++m_Elem;
}

// ------------------------------------------------------------------------------------------------
void IniEntries::Retreat(Int32 n)
{
    // Are there any other elements behind?
    if (m_List.empty() || m_Elem == m_List.begin())
        return;
    // Jump as many elements as possible
    while ((--n >= 0) && m_Elem != m_List.begin()) --m_Elem;
}

// ------------------------------------------------------------------------------------------------
CSStr IniEntries::GetItem() const
{
    if (m_List.empty() || m_Elem == m_List.end())
        SqThrow("Invalid INI entry [item]");
    else
        return m_Elem->pItem;
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
CSStr IniEntries::GetComment() const
{
    if (m_List.empty() || m_Elem == m_List.end())
        SqThrow("Invalid INI entry [comment]");
    else
        return m_Elem->pComment;
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
Int32 IniEntries::GetOrder() const
{
    if (m_List.empty() || m_Elem == m_List.end())
        SqThrow("Invalid INI entry [order]");
    else
        return m_Elem->nOrder;
    return -1;
}

// ------------------------------------------------------------------------------------------------
IniDocument::IniDocument()
    : m_Doc(false, false, true)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
IniDocument::IniDocument(bool utf8, bool multikey, bool multiline)
    : m_Doc(utf8, multikey, multiline)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
IniDocument::~IniDocument()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Int32 IniDocument::Cmp(const IniDocument & o) const
{
    if (m_Doc == o.m_Doc)
        return 0;
    else if (this > &o)
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
CSStr IniDocument::ToString() const
{
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
void IniDocument::LoadFile(CSStr filepath)
{
    if (!Validate())
        return; /* Unable to proceed */
    // Attempt to load the file from disk
    const SI_Error ini_ret = m_Doc->LoadFile(filepath);
    // See if the file could be loaded
    if (ini_ret < 0)
    {
        // Identify the error type
        switch (ini_ret)
        {
            case SI_FAIL:
                SqThrow("Failed to load the ini file. Probably invalid");
            break;
            case SI_NOMEM:
                SqThrow("Ran out of memory while loading the ini file");
            break;
            case SI_FILE:
                SqThrow("Failed to load the ini file. %s", strerror(errno));
            break;
            default:
                SqThrow("Failed to load the ini file for some unforeseen reason");
        }
    }
}

// ------------------------------------------------------------------------------------------------
void IniDocument::LoadData(CSStr source, Int32 size)
{
    if (!Validate())
        return; /* Unable to proceed */
    // Attempt to load the source from memory
    const SI_Error ini_ret = m_Doc->LoadData(source, size < 0 ? strlen(source) : size);
    // See if the source could be loaded
    if (ini_ret < 0)
    {
        // Identify the error type
        switch (ini_ret)
        {
            case SI_FAIL:
                SqThrow("Failed to load the ini data. Probably invalid");
            break;
            case SI_NOMEM:
                SqThrow("Ran out of memory while loading the ini data");
            break;
            default:
                SqThrow("Failed to load the ini data for some unforeseen reason");
        }
    }
}

// ------------------------------------------------------------------------------------------------
void IniDocument::SaveFile(CSStr filepath, bool signature)
{
    if (!Validate())
        return; /* Unable to proceed */
    // Attempt to save the file to disk
    const SI_Error ini_ret = m_Doc->SaveFile(filepath, signature);
    // See if the file could be saved
    if (ini_ret == SI_FAIL)
    {
        SqThrow("Failed to save the ini file. Probably invalid");
    }
}

// ------------------------------------------------------------------------------------------------
Object IniDocument::SaveData(bool signature)
{
    if (!Validate())
        return NullObject(); /* Unable to proceed */
    // The string where the content will be saved
    String source;
    // Attempt to save the data to string
    const SI_Error ini_ret = m_Doc->Save(source, signature);
    // See if the data could be saved
    if (ini_ret == SI_FAIL)
    {
        SqThrow("Failed to save the ini data. Probably invalid");
        return NullObject(); /* Unable to proceed */
    }
    // Transform it into a script object
    sq_pushstring(DefaultVM::Get(), source.c_str(), source.size());
    // Get the object from the stack
    Var< Object & > var(DefaultVM::Get(), -1);
    // Pop the created object from the stack
    if (!var.value.IsNull())
    {
        sq_pop(DefaultVM::Get(), 1);
    }
    // Return the script object
    return var.value;
}

// ------------------------------------------------------------------------------------------------
IniEntries IniDocument::GetAllSections() const
{
    if (!Validate())
        return IniEntries(); /* Unable to proceed */
    // Prepare a container to receive the entries
    static Container entries;
    // Obtain all sections from the ini document
    m_Doc->GetAllSections(entries);
    // Return the entries and take over content
    return IniEntries(m_Doc, entries);
}

// ------------------------------------------------------------------------------------------------
IniEntries IniDocument::GetAllKeys(CSStr section) const
{
    if (!Validate())
        return IniEntries(); /* Unable to proceed */
    // Prepare a container to receive the entries
    static Container entries;
    // Obtain all sections from the ini document
    m_Doc->GetAllKeys(section, entries);
    // Return the entries and take over content
    return IniEntries(m_Doc, entries);
}

// ------------------------------------------------------------------------------------------------
IniEntries IniDocument::GetAllValues(CSStr section, CSStr key) const
{
    if (!Validate())
        return IniEntries(); /* Unable to proceed */
    // Prepare a container to receive the entries
    static Container entries;
    // Obtain all sections from the ini document
    m_Doc->GetAllValues(section, key, entries);
    // Return the entries and take over content
    return IniEntries(m_Doc, entries);
}

// ------------------------------------------------------------------------------------------------
Int32 IniDocument::GetSectionSize(CSStr section) const
{
    if (Validate())
        // Return the requested information
        return m_Doc->GetSectionSize(section);
    // Return invalid size
    return -1;
}

// ------------------------------------------------------------------------------------------------
bool IniDocument::HasMultipleKeys(CSStr section, CSStr key) const
{
    if (!Validate())
        return false; /* Unable to proceed */
    // Where to retrive whether the key has multiple instances
    bool multiple = false;
    // Attempt to query the information
    if (m_Doc->GetValue(section, key, NULL, &multiple) == NULL)
        return true; /* Doesn't exist */
    return multiple;
}

// ------------------------------------------------------------------------------------------------
CCStr IniDocument::GetValue(CSStr section, CSStr key, CSStr def) const
{
    if (!Validate())
        return _SC(""); /* Unable to proceed */
    // Attempt to query the information and return it
    return m_Doc->GetValue(section, key, def, NULL);
}

// ------------------------------------------------------------------------------------------------
SQInteger IniDocument::GetInteger(CSStr section, CSStr key, SQInteger def) const
{
    if (!Validate())
        return 0; /* Unable to proceed */
    // Attempt to query the information and return it
    return (SQInteger)m_Doc->GetLongValue(section, key, def, NULL);
}

// ------------------------------------------------------------------------------------------------
SQFloat IniDocument::GetFloat(CSStr section, CSStr key, SQFloat def) const
{
    if (!Validate())
        return 0.0; /* Unable to proceed */
    // Attempt to query the information and return it
    return (SQFloat)m_Doc->GetDoubleValue(section, key, def, NULL);
}

// ------------------------------------------------------------------------------------------------
bool IniDocument::GetBoolean(CSStr section, CSStr key, bool def) const
{
    if (!Validate())
        return false; /* Unable to proceed */
    // Attempt to query the information and return it
    return m_Doc->GetBoolValue(section, key, def, NULL);
}

// ------------------------------------------------------------------------------------------------
void IniDocument::SetValue(CSStr section, CSStr key, CSStr value, bool force, CSStr comment)
{
    if (!Validate())
        return; /* Unable to proceed */
    // Attempt to apply the specified information
    const SI_Error ini_ret = m_Doc->SetValue(section, key, value, comment, force);
    // See if the information could be applied
    if (ini_ret < 0)
    {
        // Identify the error type
        switch (ini_ret)
        {
            case SI_FAIL:
                SqThrow("Failed to set the ini value. Probably invalid");
            break;
            case SI_NOMEM:
                SqThrow("Ran out of memory while setting ini value");
            break;
            default:
                SqThrow("Failed to set the ini value for some unforeseen reason");
        }
    }
}

// ------------------------------------------------------------------------------------------------
void IniDocument::SetInteger(CSStr section, CSStr key, SQInteger value, bool hex, bool force, CSStr comment)
{
    if (!Validate())
        return; /* Unable to proceed */
    // Attempt to apply the specified information
    const SI_Error ini_ret = m_Doc->SetLongValue(section, key, value, comment, hex, force);
    // See if the information could be applied
    if (ini_ret < 0)
    {
        // Identify the error type
        switch (ini_ret)
        {
            case SI_FAIL:
                SqThrow("Failed to set the ini integer. Probably invalid");
            break;
            case SI_NOMEM:
                SqThrow("Ran out of memory while setting ini integer");
            break;
            default:
                SqThrow("Failed to set the ini integer for some unforeseen reason");
        }
    }
}

// ------------------------------------------------------------------------------------------------
void IniDocument::SetFloat(CSStr section, CSStr key, SQFloat value, bool force, CSStr comment)
{
    if (!Validate())
        return; /* Unable to proceed */
    // Attempt to apply the specified information
    const SI_Error ini_ret = m_Doc->SetDoubleValue(section, key, value, comment, force);
    // See if the information could be applied
    if (ini_ret < 0)
    {
        // Identify the error type
        switch (ini_ret)
        {
            case SI_FAIL:
                SqThrow("Failed to set the ini float. Probably invalid");
            break;
            case SI_NOMEM:
                SqThrow("Ran out of memory while setting ini float");
            break;
            default:
                SqThrow("Failed to set the ini float for some unforeseen reason");
        }
    }
}

// ------------------------------------------------------------------------------------------------
void IniDocument::SetBoolean(CSStr section, CSStr key, bool value, bool force, CSStr comment)
{
    if (!Validate())
        return; /* Unable to proceed */
    // Attempt to apply the specified information
    const SI_Error ini_ret = m_Doc->SetBoolValue(section, key, value, comment, force);
    // See if the information could be applied
    if (ini_ret < 0)
    {
        // Identify the error type
        switch (ini_ret)
        {
            case SI_FAIL:
                SqThrow("Failed to set the ini boolean. Probably invalid");
            break;
            case SI_NOMEM:
                SqThrow("Ran out of memory while setting ini boolean");
            break;
            default:
                SqThrow("Failed to set the ini boolean for some unforeseen reason");
        }
    }
}

// ------------------------------------------------------------------------------------------------
bool IniDocument::DeleteValue(CSStr section, CSStr key, CSStr value, bool empty)
{
    if (!Validate())
        return false; /* Unable to proceed */
    else if (!section)
    {
        SqThrow("Invalid ini section");
        return false; /* Unable to proceed */
    }
    else if (!key)
    {
        SqThrow("Invalid ini key");
        return false; /* Unable to proceed */
    }
    // Attempt to remove the specified value
    return m_Doc->DeleteValue(section, key, value, empty);
}

// ================================================================================================
void Register_INI(HSQUIRRELVM vm)
{
    Table inins(vm);

    inins.Bind(_SC("Entries"), Class< IniEntries >(vm, _SC("SqIniEntries"))
        /* Constructors */
        .Ctor()
        .Ctor< const IniEntries & >()
        /* Core Metamethods */
        .Func(_SC("_tostring"), &IniEntries::ToString)
        .Func(_SC("_cmp"), &IniEntries::Cmp)
        /* Properties */
        .Prop(_SC("Valid"), &IniEntries::IsValid)
        .Prop(_SC("Empty"), &IniEntries::IsEmpty)
        .Prop(_SC("References"), &IniEntries::GetRefCount)
        .Prop(_SC("Size"), &IniEntries::GetSize)
        .Prop(_SC("Item"), &IniEntries::GetItem)
        .Prop(_SC("Comment"), &IniEntries::GetComment)
        .Prop(_SC("Order"), &IniEntries::GetOrder)
        /* Functions */
        .Func(_SC("Reset"), &IniEntries::Reset)
        .Func(_SC("Next"), &IniEntries::Next)
        .Func(_SC("Prev"), &IniEntries::Prev)
        .Func(_SC("Advance"), &IniEntries::Advance)
        .Func(_SC("Retreat"), &IniEntries::Retreat)
        .Func(_SC("Sort"), &IniEntries::Sort)
        .Func(_SC("SortByKeyOrder"), &IniEntries::SortByKeyOrder)
        .Func(_SC("SortByLoadOrder"), &IniEntries::SortByLoadOrder)
        .Func(_SC("SortByLoadOrderEmptyFirst"), &IniEntries::SortByLoadOrderEmptyFirst)
    );

    inins.Bind(_SC("Document"), Class< IniDocument, NoCopy< IniDocument > >(vm, _SC("SqIniDocument"))
        /* Constructors */
        .Ctor()
        .Ctor< bool, bool, bool >()
        /* Core Metamethods */
        .Func(_SC("_tostring"), &IniDocument::ToString)
        .Func(_SC("_cmp"), &IniDocument::Cmp)
        /* Properties */
        .Prop(_SC("Valid"), &IniDocument::IsValid)
        .Prop(_SC("Empty"), &IniDocument::IsEmpty)
        .Prop(_SC("References"), &IniDocument::GetRefCount)
        .Prop(_SC("Unicode"), &IniDocument::GetUnicode, &IniDocument::SetUnicode)
        .Prop(_SC("MultiKey"), &IniDocument::GetMultiKey, &IniDocument::SetMultiKey)
        .Prop(_SC("MultiLine"), &IniDocument::GetMultiLine, &IniDocument::SetMultiLine)
        .Prop(_SC("Spaces"), &IniDocument::GetSpaces, &IniDocument::SetSpaces)
        /* Functions */
        .Func(_SC("Reset"), &IniDocument::Reset)
        .Func(_SC("LoadFile"), &IniDocument::LoadFile)
        .Overload< void (IniDocument::*)(CSStr) >(_SC("LoadData"), &IniDocument::LoadData)
        .Overload< void (IniDocument::*)(CSStr, Int32) >(_SC("LoadData"), &IniDocument::LoadData)
        .Overload< void (IniDocument::*)(CSStr) >(_SC("SaveFile"), &IniDocument::SaveFile)
        .Overload< void (IniDocument::*)(CSStr, bool) >(_SC("SaveFile"), &IniDocument::SaveFile)
        .Func(_SC("SaveData"), &IniDocument::SaveData)
        .Func(_SC("GetSections"), &IniDocument::GetAllSections)
        .Func(_SC("GetKeys"), &IniDocument::GetAllKeys)
        .Func(_SC("GetValues"), &IniDocument::GetAllValues)
        .Func(_SC("GetSectionSize"), &IniDocument::GetSectionSize)
        .Func(_SC("HasMultipleKeys"), &IniDocument::HasMultipleKeys)
        .Func(_SC("GetValue"), &IniDocument::GetValue)
        .Func(_SC("GetInteger"), &IniDocument::GetInteger)
        .Func(_SC("GetFloat"), &IniDocument::GetFloat)
        .Func(_SC("GetBoolean"), &IniDocument::GetBoolean)
        .Overload< void (IniDocument::*)(CSStr, CSStr, CSStr) >(_SC("SetValue"), &IniDocument::SetValue)
        .Overload< void (IniDocument::*)(CSStr, CSStr, CSStr, bool) >(_SC("SetValue"), &IniDocument::SetValue)
        .Overload< void (IniDocument::*)(CSStr, CSStr, CSStr, bool, CSStr) >(_SC("SetValue"), &IniDocument::SetValue)
        .Overload< void (IniDocument::*)(CSStr, CSStr, SQInteger) >(_SC("SetInteger"), &IniDocument::SetInteger)
        .Overload< void (IniDocument::*)(CSStr, CSStr, SQInteger, bool) >(_SC("SetInteger"), &IniDocument::SetInteger)
        .Overload< void (IniDocument::*)(CSStr, CSStr, SQInteger, bool, bool) >(_SC("SetInteger"), &IniDocument::SetInteger)
        .Overload< void (IniDocument::*)(CSStr, CSStr, SQInteger, bool, bool, CSStr) >(_SC("SetInteger"), &IniDocument::SetInteger)
        .Overload< void (IniDocument::*)(CSStr, CSStr, SQFloat) >(_SC("SetFloat"), &IniDocument::SetFloat)
        .Overload< void (IniDocument::*)(CSStr, CSStr, SQFloat, bool) >(_SC("SetFloat"), &IniDocument::SetFloat)
        .Overload< void (IniDocument::*)(CSStr, CSStr, SQFloat, bool, CSStr) >(_SC("SetFloat"), &IniDocument::SetFloat)
        .Overload< void (IniDocument::*)(CSStr, CSStr, bool) >(_SC("SetBoolean"), &IniDocument::SetBoolean)
        .Overload< void (IniDocument::*)(CSStr, CSStr, bool, bool) >(_SC("SetBoolean"), &IniDocument::SetBoolean)
        .Overload< void (IniDocument::*)(CSStr, CSStr, bool, bool, CSStr) >(_SC("SetBoolean"), &IniDocument::SetBoolean)
        .Overload< bool (IniDocument::*)(CSStr) >(_SC("DeleteValue"), &IniDocument::DeleteValue)
        .Overload< bool (IniDocument::*)(CSStr, CSStr) >(_SC("DeleteValue"), &IniDocument::DeleteValue)
        .Overload< bool (IniDocument::*)(CSStr, CSStr, CSStr) >(_SC("DeleteValue"), &IniDocument::DeleteValue)
        .Overload< bool (IniDocument::*)(CSStr, CSStr, CSStr, bool) >(_SC("DeleteValue"), &IniDocument::DeleteValue)
    );

    RootTable(vm).Bind(_SC("SqIni"), inins);
}

} // Namespace:: SqMod
