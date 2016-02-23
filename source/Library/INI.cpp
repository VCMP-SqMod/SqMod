// ------------------------------------------------------------------------------------------------
#include "Library/INI.hpp"

// ------------------------------------------------------------------------------------------------
#include <errno.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace INI {

// ------------------------------------------------------------------------------------------------
Int32 Entries::Cmp(const Entries & o) const
{
    if (m_Elem == o.m_Elem)
        return 0;
    else if (this > &o)
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
CSStr Entries::ToString() const
{
    return GetItem();
}

// ------------------------------------------------------------------------------------------------
void Entries::Next()
{
    // Are there any other elements ahead?
    if (!m_List.empty() && m_Elem != m_List.end())
        ++m_Elem; /* Go ahead one element */
}

// ------------------------------------------------------------------------------------------------
void Entries::Prev()
{
    // Are there any other elements behind?
    if (!m_List.empty() && m_Elem != m_List.begin())
        --m_Elem; /* Go back one element */
}

// ------------------------------------------------------------------------------------------------
void Entries::Advance(Int32 n)
{
    // Are there any other elements ahead?
    if (m_List.empty() || m_Elem == m_List.end())
        return;
    // Jump as many elements as possible
    while ((--n >= 0) && m_Elem != m_List.end()) ++m_Elem;
}

// ------------------------------------------------------------------------------------------------
void Entries::Retreat(Int32 n)
{
    // Are there any other elements behind?
    if (m_List.empty() || m_Elem == m_List.begin())
        return;
    // Jump as many elements as possible
    while ((--n >= 0) && m_Elem != m_List.begin()) --m_Elem;
}

// ------------------------------------------------------------------------------------------------
CSStr Entries::GetItem() const
{
    if (m_List.empty() || m_Elem == m_List.end())
        SqThrow("Invalid INI entry [item]");
    else
        return m_Elem->pItem;
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
CSStr Entries::GetComment() const
{
    if (m_List.empty() || m_Elem == m_List.end())
        SqThrow("Invalid INI entry [comment]");
    else
        return m_Elem->pComment;
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
Int32 Entries::GetOrder() const
{
    if (m_List.empty() || m_Elem == m_List.end())
        SqThrow("Invalid INI entry [order]");
    else
        return m_Elem->nOrder;
    return -1;
}

// ------------------------------------------------------------------------------------------------
Document::Document()
    : m_Doc(false, false, true)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Document::Document(bool utf8, bool multikey, bool multiline)
    : m_Doc(utf8, multikey, multiline)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Document::~Document()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Int32 Document::Cmp(const Document & o) const
{
    if (m_Doc == o.m_Doc)
        return 0;
    else if (this > &o)
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
CSStr Document::ToString() const
{
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
void Document::LoadFile(CSStr filepath)
{
    if (!Validate())
        return; /* Unable to proceed */
    // Attempt to load the file from disk
    const SI_Error ret = m_Doc->LoadFile(filepath);
    // See if the file could be loaded
    if (ret < 0)
    {
        // Identify the error type
        switch (ret)
        {
            case SI_FAIL:
                SqThrow("Failed to load the INI file. Probably invalid");
            break;
            case SI_NOMEM:
                SqThrow("Ran out of memory while loading the INI file");
            break;
            case SI_FILE:
                SqThrow("Failed to load the INI file. %s", strerror(errno));
            break;
            default:
                SqThrow("Failed to load the INI file for some unforeseen reason");
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Document::LoadData(CSStr source, Int32 size)
{
    if (!Validate())
        return; /* Unable to proceed */
    // Attempt to load the source from memory
    const SI_Error ret = m_Doc->LoadData(source, size < 0 ? strlen(source) : size);
    // See if the source could be loaded
    if (ret < 0)
    {
        // Identify the error type
        switch (ret)
        {
            case SI_FAIL:
                SqThrow("Failed to load the INI data. Probably invalid");
            break;
            case SI_NOMEM:
                SqThrow("Ran out of memory while loading the INI data");
            break;
            default:
                SqThrow("Failed to load the INI data for some unforeseen reason");
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Document::SaveFile(CSStr filepath, bool signature)
{
    if (!Validate())
        return; /* Unable to proceed */
    // Attempt to save the file to disk
    const SI_Error ret = m_Doc->SaveFile(filepath, signature);
    // See if the file could be saved
    if (ret == SI_FAIL)
    {
        SqThrow("Failed to save the INI file. Probably invalid");
    }
}

// ------------------------------------------------------------------------------------------------
Object Document::SaveData(bool signature)
{
    if (!Validate())
        return NullObject(); /* Unable to proceed */
    // The string where the content will be saved
    String source;
    // Attempt to save the data to string
    const SI_Error ret = m_Doc->Save(source, signature);
    // See if the data could be saved
    if (ret == SI_FAIL)
    {
        SqThrow("Failed to save the INI data. Probably invalid");
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
Entries Document::GetAllSections() const
{
    if (!Validate())
        return Entries(); /* Unable to proceed */
    // Prepare a container to receive the entries
    static Container entries;
    // Obtain all sections from the INI document
    m_Doc->GetAllSections(entries);
    // Return the entries and take over content
    return Entries(m_Doc, entries);
}

// ------------------------------------------------------------------------------------------------
Entries Document::GetAllKeys(CSStr section) const
{
    if (!Validate())
        return Entries(); /* Unable to proceed */
    // Prepare a container to receive the entries
    static Container entries;
    // Obtain all sections from the INI document
    m_Doc->GetAllKeys(section, entries);
    // Return the entries and take over content
    return Entries(m_Doc, entries);
}

// ------------------------------------------------------------------------------------------------
Entries Document::GetAllValues(CSStr section, CSStr key) const
{
    if (!Validate())
        return Entries(); /* Unable to proceed */
    // Prepare a container to receive the entries
    static Container entries;
    // Obtain all sections from the INI document
    m_Doc->GetAllValues(section, key, entries);
    // Return the entries and take over content
    return Entries(m_Doc, entries);
}

// ------------------------------------------------------------------------------------------------
Int32 Document::GetSectionSize(CSStr section) const
{
    if (Validate())
        // Return the requested information
        return m_Doc->GetSectionSize(section);
    // Return invalid size
    return -1;
}

// ------------------------------------------------------------------------------------------------
bool Document::HasMultipleKeys(CSStr section, CSStr key) const
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
CCStr Document::GetValue(CSStr section, CSStr key, CSStr def) const
{
    if (!Validate())
        return _SC(""); /* Unable to proceed */
    // Attempt to query the information and return it
    return m_Doc->GetValue(section, key, def, NULL);
}

// ------------------------------------------------------------------------------------------------
SQInteger Document::GetInteger(CSStr section, CSStr key, SQInteger def) const
{
    if (!Validate())
        return 0; /* Unable to proceed */
    // Attempt to query the information and return it
    return (SQInteger)m_Doc->GetLongValue(section, key, def, NULL);
}

// ------------------------------------------------------------------------------------------------
SQFloat Document::GetFloat(CSStr section, CSStr key, SQFloat def) const
{
    if (!Validate())
        return 0.0; /* Unable to proceed */
    // Attempt to query the information and return it
    return (SQFloat)m_Doc->GetDoubleValue(section, key, def, NULL);
}

// ------------------------------------------------------------------------------------------------
bool Document::GetBoolean(CSStr section, CSStr key, bool def) const
{
    if (!Validate())
        return false; /* Unable to proceed */
    // Attempt to query the information and return it
    return m_Doc->GetBoolValue(section, key, def, NULL);
}

// ------------------------------------------------------------------------------------------------
void Document::SetValue(CSStr section, CSStr key, CSStr value, bool force, CSStr comment)
{
    if (!Validate())
        return; /* Unable to proceed */
    // Attempt to apply the specified information
    const SI_Error ret = m_Doc->SetValue(section, key, value, comment, force);
    // See if the information could be applied
    if (ret < 0)
    {
        // Identify the error type
        switch (ret)
        {
            case SI_FAIL:
                SqThrow("Failed to set the INI value. Probably invalid");
            break;
            case SI_NOMEM:
                SqThrow("Ran out of memory while setting INI value");
            break;
            default:
                SqThrow("Failed to set the INI value for some unforeseen reason");
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Document::SetInteger(CSStr section, CSStr key, SQInteger value, bool hex, bool force, CSStr comment)
{
    if (!Validate())
        return; /* Unable to proceed */
    // Attempt to apply the specified information
    const SI_Error ret = m_Doc->SetLongValue(section, key, value, comment, hex, force);
    // See if the information could be applied
    if (ret < 0)
    {
        // Identify the error type
        switch (ret)
        {
            case SI_FAIL:
                SqThrow("Failed to set the INI integer. Probably invalid");
            break;
            case SI_NOMEM:
                SqThrow("Ran out of memory while setting INI integer");
            break;
            default:
                SqThrow("Failed to set the INI integer for some unforeseen reason");
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Document::SetFloat(CSStr section, CSStr key, SQFloat value, bool force, CSStr comment)
{
    if (!Validate())
        return; /* Unable to proceed */
    // Attempt to apply the specified information
    const SI_Error ret = m_Doc->SetDoubleValue(section, key, value, comment, force);
    // See if the information could be applied
    if (ret < 0)
    {
        // Identify the error type
        switch (ret)
        {
            case SI_FAIL:
                SqThrow("Failed to set the INI float. Probably invalid");
            break;
            case SI_NOMEM:
                SqThrow("Ran out of memory while setting INI float");
            break;
            default:
                SqThrow("Failed to set the INI float for some unforeseen reason");
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Document::SetBoolean(CSStr section, CSStr key, bool value, bool force, CSStr comment)
{
    if (!Validate())
        return; /* Unable to proceed */
    // Attempt to apply the specified information
    const SI_Error ret = m_Doc->SetBoolValue(section, key, value, comment, force);
    // See if the information could be applied
    if (ret < 0)
    {
        // Identify the error type
        switch (ret)
        {
            case SI_FAIL:
                SqThrow("Failed to set the INI boolean. Probably invalid");
            break;
            case SI_NOMEM:
                SqThrow("Ran out of memory while setting INI boolean");
            break;
            default:
                SqThrow("Failed to set the INI boolean for some unforeseen reason");
        }
    }
}

// ------------------------------------------------------------------------------------------------
bool Document::DeleteValue(CSStr section, CSStr key, CSStr value, bool empty)
{
    if (!Validate())
        return false; /* Unable to proceed */
    else if (!section)
    {
        SqThrow("Invalid INI section");
        return false; /* Unable to proceed */
    }
    else if (!key)
    {
        SqThrow("Invalid INI key");
        return false; /* Unable to proceed */
    }
    // Attempt to remove the specified value
    return m_Doc->DeleteValue(section, key, value, empty);
}

} // Namespace:: INI

// ================================================================================================
void Register_INI(HSQUIRRELVM vm)
{
    using namespace INI;

    Table inins(vm);

    inins.Bind(_SC("Entries"), Class< Entries >(vm, _SC("SqIniEntries"))
        /* Constructors */
        .Ctor()
        .Ctor< const Entries & >()
        /* Core Metamethods */
        .Func(_SC("_tostring"), &Entries::ToString)
        .Func(_SC("_cmp"), &Entries::Cmp)
        /* Properties */
        .Prop(_SC("Valid"), &Entries::IsValid)
        .Prop(_SC("Empty"), &Entries::IsEmpty)
        .Prop(_SC("References"), &Entries::GetRefCount)
        .Prop(_SC("Size"), &Entries::GetSize)
        .Prop(_SC("Item"), &Entries::GetItem)
        .Prop(_SC("Comment"), &Entries::GetComment)
        .Prop(_SC("Order"), &Entries::GetOrder)
        /* Functions */
        .Func(_SC("Reset"), &Entries::Reset)
        .Func(_SC("Next"), &Entries::Next)
        .Func(_SC("Prev"), &Entries::Prev)
        .Func(_SC("Advance"), &Entries::Advance)
        .Func(_SC("Retreat"), &Entries::Retreat)
        .Func(_SC("Sort"), &Entries::Sort)
        .Func(_SC("SortByKeyOrder"), &Entries::SortByKeyOrder)
        .Func(_SC("SortByLoadOrder"), &Entries::SortByLoadOrder)
        .Func(_SC("SortByLoadOrderEmptyFirst"), &Entries::SortByLoadOrderEmptyFirst)
    );

    inins.Bind(_SC("Document"), Class< Document, NoCopy< Document > >(vm, _SC("SqIniDocument"))
        /* Constructors */
        .Ctor()
        .Ctor< bool, bool, bool >()
        /* Core Metamethods */
        .Func(_SC("_tostring"), &Document::ToString)
        .Func(_SC("_cmp"), &Document::Cmp)
        /* Properties */
        .Prop(_SC("Valid"), &Document::IsValid)
        .Prop(_SC("Empty"), &Document::IsEmpty)
        .Prop(_SC("References"), &Document::GetRefCount)
        .Prop(_SC("Unicode"), &Document::GetUnicode, &Document::SetUnicode)
        .Prop(_SC("MultiKey"), &Document::GetMultiKey, &Document::SetMultiKey)
        .Prop(_SC("MultiLine"), &Document::GetMultiLine, &Document::SetMultiLine)
        .Prop(_SC("Spaces"), &Document::GetSpaces, &Document::SetSpaces)
        /* Functions */
        .Func(_SC("Reset"), &Document::Reset)
        .Func(_SC("LoadFile"), &Document::LoadFile)
        .Overload< void (Document::*)(CSStr) >(_SC("LoadData"), &Document::LoadData)
        .Overload< void (Document::*)(CSStr, Int32) >(_SC("LoadData"), &Document::LoadData)
        .Overload< void (Document::*)(CSStr) >(_SC("SaveFile"), &Document::SaveFile)
        .Overload< void (Document::*)(CSStr, bool) >(_SC("SaveFile"), &Document::SaveFile)
        .Func(_SC("SaveData"), &Document::SaveData)
        .Func(_SC("GetSections"), &Document::GetAllSections)
        .Func(_SC("GetKeys"), &Document::GetAllKeys)
        .Func(_SC("GetValues"), &Document::GetAllValues)
        .Func(_SC("GetSectionSize"), &Document::GetSectionSize)
        .Func(_SC("HasMultipleKeys"), &Document::HasMultipleKeys)
        .Func(_SC("GetValue"), &Document::GetValue)
        .Func(_SC("GetInteger"), &Document::GetInteger)
        .Func(_SC("GetFloat"), &Document::GetFloat)
        .Func(_SC("GetBoolean"), &Document::GetBoolean)
        .Overload< void (Document::*)(CSStr, CSStr, CSStr) >(_SC("SetValue"), &Document::SetValue)
        .Overload< void (Document::*)(CSStr, CSStr, CSStr, bool) >(_SC("SetValue"), &Document::SetValue)
        .Overload< void (Document::*)(CSStr, CSStr, CSStr, bool, CSStr) >(_SC("SetValue"), &Document::SetValue)
        .Overload< void (Document::*)(CSStr, CSStr, SQInteger) >(_SC("SetInteger"), &Document::SetInteger)
        .Overload< void (Document::*)(CSStr, CSStr, SQInteger, bool) >(_SC("SetInteger"), &Document::SetInteger)
        .Overload< void (Document::*)(CSStr, CSStr, SQInteger, bool, bool) >(_SC("SetInteger"), &Document::SetInteger)
        .Overload< void (Document::*)(CSStr, CSStr, SQInteger, bool, bool, CSStr) >(_SC("SetInteger"), &Document::SetInteger)
        .Overload< void (Document::*)(CSStr, CSStr, SQFloat) >(_SC("SetFloat"), &Document::SetFloat)
        .Overload< void (Document::*)(CSStr, CSStr, SQFloat, bool) >(_SC("SetFloat"), &Document::SetFloat)
        .Overload< void (Document::*)(CSStr, CSStr, SQFloat, bool, CSStr) >(_SC("SetFloat"), &Document::SetFloat)
        .Overload< void (Document::*)(CSStr, CSStr, bool) >(_SC("SetBoolean"), &Document::SetBoolean)
        .Overload< void (Document::*)(CSStr, CSStr, bool, bool) >(_SC("SetBoolean"), &Document::SetBoolean)
        .Overload< void (Document::*)(CSStr, CSStr, bool, bool, CSStr) >(_SC("SetBoolean"), &Document::SetBoolean)
        .Overload< bool (Document::*)(CSStr) >(_SC("DeleteValue"), &Document::DeleteValue)
        .Overload< bool (Document::*)(CSStr, CSStr) >(_SC("DeleteValue"), &Document::DeleteValue)
        .Overload< bool (Document::*)(CSStr, CSStr, CSStr) >(_SC("DeleteValue"), &Document::DeleteValue)
        .Overload< bool (Document::*)(CSStr, CSStr, CSStr, bool) >(_SC("DeleteValue"), &Document::DeleteValue)
    );

    RootTable(vm).Bind(_SC("SqIni"), inins);
}

} // Namespace:: SqMod
