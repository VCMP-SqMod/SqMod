// ------------------------------------------------------------------------------------------------
#include "Library/IO/INI.hpp"

// ------------------------------------------------------------------------------------------------
#include <cerrno>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(ResultTypename, _SC("SqIniResult"))
SQMODE_DECL_TYPENAME(EntriesTypename, _SC("SqIniEntries"))
SQMODE_DECL_TYPENAME(DocumentTypename, _SC("SqIniDocument"))

// ------------------------------------------------------------------------------------------------
void IniResult::Check() const
{
    // Identify the error type
    switch (m_Result)
    {
        case SI_FAIL:
            STHROWF("Unable to %s. Probably invalid", m_Action.c_str());
        break;
        case SI_NOMEM:
            STHROWF("Unable to %s. Ran out of memory", m_Action.c_str());
        break;
        case SI_FILE:
            STHROWF("Unable to %s. %s", strerror(errno));
        break;
        case SI_OK:
        case SI_UPDATED:
        case SI_INSERTED:
            break; /* These are not error messahes. */
        default:
            STHROWF("Unable to %s for some unforeseen reason", m_Action.c_str());
    }
}

// ------------------------------------------------------------------------------------------------
void DocumentRef::Validate() const
{
    // Is the document handle valid?
    if (!m_Ptr)
    {
        STHROWF("Invalid INI document reference");
    }
}

// ------------------------------------------------------------------------------------------------
Int32 Entries::Cmp(const Entries & o) const
{
    if (m_Elem == o.m_Elem)
    {
        return 0;
    }
    else if (m_List.size() > o.m_List.size())
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
void Entries::Next()
{
    // Are there any other elements ahead?
    if (!m_List.empty() && m_Elem != m_List.end())
    {
        ++m_Elem; // Go ahead one element
    }
}

// ------------------------------------------------------------------------------------------------
void Entries::Prev()
{
    // Are there any other elements behind?
    if (!m_List.empty() && m_Elem != m_List.begin())
    {
        --m_Elem; // Go back one element
    }
}

// ------------------------------------------------------------------------------------------------
void Entries::Advance(Int32 n)
{
    // Are there any other elements ahead?
    if (m_List.empty() || m_Elem == m_List.end())
    {
        return;
    }
    // Jump as many elements as possible within the specified distance
    while ((--n >= 0) && m_Elem != m_List.end())
    {
        ++m_Elem;
    }
}

// ------------------------------------------------------------------------------------------------
void Entries::Retreat(Int32 n)
{
    // Are there any other elements behind?
    if (m_List.empty() || m_Elem == m_List.begin())
    {
        return;
    }
    // Jump as many elements as possible within the specified distance
    while ((--n >= 0) && m_Elem != m_List.begin())
    {
        --m_Elem;
    }
}

// ------------------------------------------------------------------------------------------------
CSStr Entries::GetItem() const
{
    // is the current element valid?
    if (m_List.empty() || m_Elem == m_List.end())
    {
        STHROWF("Invalid INI entry [item]");
    }
    // Return the requested information
    return m_Elem->pItem;
}

// ------------------------------------------------------------------------------------------------
CSStr Entries::GetComment() const
{
    // is the current element valid?
    if (m_List.empty() || m_Elem == m_List.end())
    {
        STHROWF("Invalid INI entry [comment]");
    }
    // Return the requested information
    return m_Elem->pComment;
}

// ------------------------------------------------------------------------------------------------
Int32 Entries::GetOrder() const
{
    // is the current element valid?
    if (m_List.empty() || m_Elem == m_List.end())
    {
        STHROWF("Invalid INI entry [order]");
    }
    // Return the requested information
    return m_Elem->nOrder;
}

// ------------------------------------------------------------------------------------------------
Int32 Document::Cmp(const Document & o) const
{
    if (m_Doc == o.m_Doc)
    {
        return 0;
    }
    else if (m_Doc.m_Ptr > o.m_Doc.m_Ptr)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
IniResult Document::LoadFile(CSStr filepath)
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to load the file from disk and return the result
    return IniResult("load INI file",  m_Doc->LoadFile(filepath));
}

// ------------------------------------------------------------------------------------------------
IniResult Document::LoadData(CSStr source, Int32 size)
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to load the file from memory and return the result
    return IniResult("load INI file", m_Doc->LoadData(source, size < 0 ? strlen(source) : size));
}

// ------------------------------------------------------------------------------------------------
IniResult Document::SaveFile(CSStr filepath, bool signature)
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to save the file to disk and return the result
    return IniResult("save INI file", m_Doc->SaveFile(filepath, signature));
}

// ------------------------------------------------------------------------------------------------
Object Document::SaveData(bool signature)
{
    // Validate the handle
    m_Doc.Validate();
    // The string where the content will be saved
    String source;
    // Attempt to save the data to string
    if (m_Doc->Save(source, signature) < 0)
    {
        STHROWF("Unable to save INI document");
    }
    // Obtain the initial stack size
    const StackGuard sg(DefaultVM::Get());
    // Transform it into a script object
    sq_pushstring(DefaultVM::Get(), source.c_str(), source.size());
    // Get the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
Entries Document::GetAllSections() const
{
    // Validate the handle
    m_Doc.Validate();
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
    // Validate the handle
    m_Doc.Validate();
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
    // Validate the handle
    m_Doc.Validate();
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
    // Validate the handle
    m_Doc.Validate();
    // Return the requested information
    return m_Doc->GetSectionSize(section);
}

// ------------------------------------------------------------------------------------------------
bool Document::HasMultipleKeys(CSStr section, CSStr key) const
{
    // Validate the handle
    m_Doc.Validate();
    // Where to retrive whether the key has multiple instances
    bool multiple = false;
    // Attempt to query the information
    if (m_Doc->GetValue(section, key, nullptr, &multiple) == nullptr)
    {
        return true; // Doesn't exist
    }
    // Return the result
    return multiple;
}

// ------------------------------------------------------------------------------------------------
CCStr Document::GetValue(CSStr section, CSStr key, CSStr def) const
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to query the information and return it
    return m_Doc->GetValue(section, key, def, nullptr);
}

// ------------------------------------------------------------------------------------------------
SQInteger Document::GetInteger(CSStr section, CSStr key, SQInteger def) const
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to query the information and return it
    return static_cast< SQInteger >(m_Doc->GetLongValue(section, key, def, nullptr));
}

// ------------------------------------------------------------------------------------------------
SQFloat Document::GetFloat(CSStr section, CSStr key, SQFloat def) const
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to query the information and return it
    return static_cast< SQFloat >(m_Doc->GetDoubleValue(section, key, def, nullptr));
}

// ------------------------------------------------------------------------------------------------
bool Document::GetBoolean(CSStr section, CSStr key, bool def) const
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to query the information and return it
    return m_Doc->GetBoolValue(section, key, def, nullptr);
}

// ------------------------------------------------------------------------------------------------
IniResult Document::SetValue(CSStr section, CSStr key, CSStr value, bool force, CSStr comment)
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to apply the specified information and return the result
    return IniResult("set INI value", m_Doc->SetValue(section, key, value, comment, force));
}

// ------------------------------------------------------------------------------------------------
IniResult Document::SetInteger(CSStr section, CSStr key, SQInteger value, bool hex, bool force, CSStr comment)
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to apply the specified information and return the result
    return IniResult("set INI integer", m_Doc->SetLongValue(section, key, value, comment, hex, force));
}

// ------------------------------------------------------------------------------------------------
IniResult Document::SetFloat(CSStr section, CSStr key, SQFloat value, bool force, CSStr comment)
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to apply the specified information and return the result
    return IniResult("set INI float", m_Doc->SetDoubleValue(section, key, value, comment, force));
}

// ------------------------------------------------------------------------------------------------
IniResult Document::SetBoolean(CSStr section, CSStr key, bool value, bool force, CSStr comment)
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to apply the specified information
    return IniResult("set INI boolean", m_Doc->SetBoolValue(section, key, value, comment, force));
}

// ------------------------------------------------------------------------------------------------
bool Document::DeleteValue(CSStr section, CSStr key, CSStr value, bool empty)
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to remove the specified value and return the result
    return m_Doc->DeleteValue(section, key, value, empty);
}

// ================================================================================================
void Register_INI(HSQUIRRELVM vm)
{
    Table inins(vm);

    inins.Bind(_SC("Result"),
        Class< IniResult >(vm, ResultTypename::Str)
        // Constructors
        .Ctor()
        .Ctor< CSStr, SQInteger >()
        .Ctor< const IniResult & >()
        // Core Meta-methods
        .SquirrelFunc(_SC("_typename"), &ResultTypename::Fn)
        .Func(_SC("cmp"), &IniResult::Cmp)
        .Func(_SC("_tostring"), &IniResult::ToString)
        // Properties
        .Prop(_SC("Valid"), &IniResult::IsValid)
        .Prop(_SC("Action"), &IniResult::GetAction)
        .Prop(_SC("Result"), &IniResult::GetResult)
        // Member Methods
        .Func(_SC("Check"), &IniResult::Check)
    );

    inins.Bind(_SC("Entries"),
        Class< Entries >(vm, EntriesTypename::Str)
        // Constructors
        .Ctor()
        .Ctor< const Entries & >()
        // Core Meta-methods
        .SquirrelFunc(_SC("_typename"), &EntriesTypename::Fn)
        .Func(_SC("cmp"), &Entries::Cmp)
        .Func(_SC("_tostring"), &Entries::ToString)
        // Properties
        .Prop(_SC("Valid"), &Entries::IsValid)
        .Prop(_SC("Empty"), &Entries::IsEmpty)
        .Prop(_SC("References"), &Entries::GetRefCount)
        .Prop(_SC("Size"), &Entries::GetSize)
        .Prop(_SC("Item"), &Entries::GetItem)
        .Prop(_SC("Comment"), &Entries::GetComment)
        .Prop(_SC("Order"), &Entries::GetOrder)
        // Member Methods
        .Func(_SC("Reset"), &Entries::Reset)
        .Func(_SC("Next"), &Entries::Next)
        .Func(_SC("Prev"), &Entries::Prev)
        .Func(_SC("Advance"), &Entries::Advance)
        .Func(_SC("Retreat"), &Entries::Retreat)
        .Func(_SC("Sort"), &Entries::Sort)
        .Func(_SC("SortByKeyOrder"), &Entries::SortByKeyOrder)
        .Func(_SC("SortByLoadOrder"), &Entries::SortByLoadOrder)
    );

    inins.Bind(_SC("Document"),
        Class< Document, NoCopy< Document > >(vm, DocumentTypename::Str)
        // Constructors
        .Ctor()
        .Ctor< bool >()
        .Ctor< bool, bool >()
        .Ctor< bool, bool, bool >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &Document::Cmp)
        .SquirrelFunc(_SC("_typename"), &DocumentTypename::Fn)
        .Func(_SC("_tostring"), &Document::ToString)
        // Properties
        .Prop(_SC("Valid"), &Document::IsValid)
        .Prop(_SC("Empty"), &Document::IsEmpty)
        .Prop(_SC("References"), &Document::GetRefCount)
        .Prop(_SC("Unicode"), &Document::GetUnicode, &Document::SetUnicode)
        .Prop(_SC("MultiKey"), &Document::GetMultiKey, &Document::SetMultiKey)
        .Prop(_SC("MultiLine"), &Document::GetMultiLine, &Document::SetMultiLine)
        .Prop(_SC("Spaces"), &Document::GetSpaces, &Document::SetSpaces)
        // Member Methods
        .Func(_SC("Reset"), &Document::Reset)
        .Func(_SC("LoadFile"), &Document::LoadFile)
        .Overload< IniResult (Document::*)(CSStr) >(_SC("LoadString"), &Document::LoadData)
        .Overload< IniResult (Document::*)(CSStr, Int32) >(_SC("LoadString"), &Document::LoadData)
        .Overload< IniResult (Document::*)(CSStr) >(_SC("SaveFile"), &Document::SaveFile)
        .Overload< IniResult (Document::*)(CSStr, bool) >(_SC("SaveFile"), &Document::SaveFile)
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
        .Overload< IniResult (Document::*)(CSStr, CSStr, CSStr) >(_SC("SetValue"), &Document::SetValue)
        .Overload< IniResult (Document::*)(CSStr, CSStr, CSStr, bool) >(_SC("SetValue"), &Document::SetValue)
        .Overload< IniResult (Document::*)(CSStr, CSStr, CSStr, bool, CSStr) >(_SC("SetValue"), &Document::SetValue)
        .Overload< IniResult (Document::*)(CSStr, CSStr, SQInteger) >(_SC("SetInteger"), &Document::SetInteger)
        .Overload< IniResult (Document::*)(CSStr, CSStr, SQInteger, bool) >(_SC("SetInteger"), &Document::SetInteger)
        .Overload< IniResult (Document::*)(CSStr, CSStr, SQInteger, bool, bool) >(_SC("SetInteger"), &Document::SetInteger)
        .Overload< IniResult (Document::*)(CSStr, CSStr, SQInteger, bool, bool, CSStr) >(_SC("SetInteger"), &Document::SetInteger)
        .Overload< IniResult (Document::*)(CSStr, CSStr, SQFloat) >(_SC("SetFloat"), &Document::SetFloat)
        .Overload< IniResult (Document::*)(CSStr, CSStr, SQFloat, bool) >(_SC("SetFloat"), &Document::SetFloat)
        .Overload< IniResult (Document::*)(CSStr, CSStr, SQFloat, bool, CSStr) >(_SC("SetFloat"), &Document::SetFloat)
        .Overload< IniResult (Document::*)(CSStr, CSStr, bool) >(_SC("SetBoolean"), &Document::SetBoolean)
        .Overload< IniResult (Document::*)(CSStr, CSStr, bool, bool) >(_SC("SetBoolean"), &Document::SetBoolean)
        .Overload< IniResult (Document::*)(CSStr, CSStr, bool, bool, CSStr) >(_SC("SetBoolean"), &Document::SetBoolean)
        .Overload< bool (Document::*)(CSStr) >(_SC("DeleteValue"), &Document::DeleteValue)
        .Overload< bool (Document::*)(CSStr, CSStr) >(_SC("DeleteValue"), &Document::DeleteValue)
        .Overload< bool (Document::*)(CSStr, CSStr, CSStr) >(_SC("DeleteValue"), &Document::DeleteValue)
        .Overload< bool (Document::*)(CSStr, CSStr, CSStr, bool) >(_SC("DeleteValue"), &Document::DeleteValue)
    );

    RootTable(vm).Bind(_SC("SqIni"), inins);

    ConstTable(vm).Enum(_SC("SqIniError"), Enumeration(vm)
        .Const(_SC("Ok"),                       Int32(SI_OK))
        .Const(_SC("Updated"),                  Int32(SI_UPDATED))
        .Const(_SC("Inserted"),                 Int32(SI_INSERTED))
        .Const(_SC("Fail"),                     Int32(SI_FAIL))
        .Const(_SC("NoMem"),                    Int32(SI_NOMEM))
        .Const(_SC("File"),                     Int32(SI_FILE))
    );
}

} // Namespace:: SqMod
