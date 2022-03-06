// ------------------------------------------------------------------------------------------------
#include "Library/IO/INI.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(ResultTypename, _SC("SqIniResult"))
SQMOD_DECL_TYPENAME(EntriesTypename, _SC("SqIniEntries"))
SQMOD_DECL_TYPENAME(DocumentTypename, _SC("SqIniDocument"))

// ------------------------------------------------------------------------------------------------
void IniResult::Check() const
{
    // Identify the error type
    switch (m_Result)
    {
        case SI_FAIL:
            STHROWF("Unable to {}. Probably invalid", m_Action);
        break;
        case SI_NOMEM:
            STHROWF("Unable to {}. Ran out of memory", m_Action);
        break;
        case SI_FILE:
            STHROWF("Unable to {}. {}", m_Action, strerror(errno));
        break;
        case SI_OK:
        case SI_UPDATED:
        case SI_INSERTED:
            break; /* These are not error messages. */
        default:
            STHROWF("Unable to {} for some unforeseen reason", m_Action);
    }
}

// ------------------------------------------------------------------------------------------------
void IniDocumentRef::Validate() const
{
    // Is the document handle valid?
    if (!m_Ptr)
    {
        STHROWF("Invalid INI document reference");
    }
}

// ------------------------------------------------------------------------------------------------
int32_t IniEntries::Cmp(const IniEntries & o) const
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
void IniEntries::Next()
{
    // Are there any other elements ahead?
    if (!m_List.empty() && m_Elem != m_List.end())
    {
        ++m_Elem; // Go ahead one element
    }
}

// ------------------------------------------------------------------------------------------------
void IniEntries::Prev()
{
    // Are there any other elements behind?
    if (!m_List.empty() && m_Elem != m_List.begin())
    {
        --m_Elem; // Go back one element
    }
}

// ------------------------------------------------------------------------------------------------
void IniEntries::Advance(int32_t n)
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
void IniEntries::Retreat(int32_t n)
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
const SQChar * IniEntries::GetItem() const
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
const SQChar * IniEntries::GetComment() const
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
int32_t IniEntries::GetOrder() const
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
int32_t IniDocument::Cmp(const IniDocument & o) const
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
IniResult IniDocument::LoadFile(const SQChar * filepath)
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to load the file from disk and return the result
    return IniResult("load INI file",  m_Doc->LoadFile(filepath));
}

// ------------------------------------------------------------------------------------------------
IniResult IniDocument::LoadData(const SQChar * source, int32_t size)
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to load the file from memory and return the result
    return IniResult("load INI file", m_Doc->LoadData(source, size < 0 ? strlen(source) : size));
}

// ------------------------------------------------------------------------------------------------
IniResult IniDocument::SaveFile(const SQChar * filepath, bool signature)
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to save the file to disk and return the result
    return IniResult("save INI file", m_Doc->SaveFile(filepath, signature));
}

// ------------------------------------------------------------------------------------------------
Object IniDocument::SaveData(bool signature)
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
    const StackGuard sg(SqVM());
    // Transform it into a script object
    sq_pushstring(SqVM(), source.c_str(), source.size());
    // Get the object from the stack and return it
    return Var< Object >(SqVM(), -1).value;
}

// ------------------------------------------------------------------------------------------------
IniEntries IniDocument::GetAllSections() const
{
    // Validate the handle
    m_Doc.Validate();
    // Prepare a container to receive the entries
    static Container entries;
    // Obtain all sections from the INI document
    m_Doc->GetAllSections(entries);
    // Return the entries and take over content
    return IniEntries(m_Doc, entries);
}

// ------------------------------------------------------------------------------------------------
IniEntries IniDocument::GetAllKeys(const SQChar * section) const
{
    // Validate the handle
    m_Doc.Validate();
    // Prepare a container to receive the entries
    static Container entries;
    // Obtain all sections from the INI document
    m_Doc->GetAllKeys(section, entries);
    // Return the entries and take over content
    return IniEntries(m_Doc, entries);
}

// ------------------------------------------------------------------------------------------------
IniEntries IniDocument::GetAllValues(const SQChar * section, const SQChar * key) const
{
    // Validate the handle
    m_Doc.Validate();
    // Prepare a container to receive the entries
    static Container entries;
    // Obtain all sections from the INI document
    m_Doc->GetAllValues(section, key, entries);
    // Return the entries and take over content
    return IniEntries(m_Doc, entries);
}

// ------------------------------------------------------------------------------------------------
int32_t IniDocument::GetSectionSize(const SQChar * section) const
{
    // Validate the handle
    m_Doc.Validate();
    // Return the requested information
    return m_Doc->GetSectionSize(section);
}

// ------------------------------------------------------------------------------------------------
bool IniDocument::HasMultipleKeys(const SQChar * section, const SQChar * key) const
{
    // Validate the handle
    m_Doc.Validate();
    // Where to retrieve whether the key has multiple instances
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
const char * IniDocument::GetValue(const SQChar * section, const SQChar * key, const SQChar * def) const
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to query the information and return it
    return m_Doc->GetValue(section, key, def, nullptr);
}

// ------------------------------------------------------------------------------------------------
SQInteger IniDocument::GetInteger(const SQChar * section, const SQChar * key, SQInteger def) const
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to query the information and return it
    return static_cast< SQInteger >(m_Doc->GetLongValue(section, key, static_cast< long >(def), nullptr));
}

// ------------------------------------------------------------------------------------------------
SQFloat IniDocument::GetFloat(const SQChar * section, const SQChar * key, SQFloat def) const
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to query the information and return it
    return static_cast< SQFloat >(m_Doc->GetDoubleValue(section, key, def, nullptr));
}

// ------------------------------------------------------------------------------------------------
bool IniDocument::GetBoolean(const SQChar * section, const SQChar * key, bool def) const
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to query the information and return it
    return m_Doc->GetBoolValue(section, key, def, nullptr);
}

// ------------------------------------------------------------------------------------------------
IniResult IniDocument::SetValue(const SQChar * section, const SQChar * key, const SQChar * value, bool force, const SQChar * comment)
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to apply the specified information and return the result
    return IniResult("set INI value", m_Doc->SetValue(section, key, value, comment, force));
}

// ------------------------------------------------------------------------------------------------
IniResult IniDocument::SetInteger(const SQChar * section, const SQChar * key, SQInteger value, bool hex, bool force, const SQChar * comment)
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to apply the specified information and return the result
    return IniResult("set INI integer", m_Doc->SetLongValue(section, key, static_cast< long >(value), comment, hex, force));
}

// ------------------------------------------------------------------------------------------------
IniResult IniDocument::SetFloat(const SQChar * section, const SQChar * key, SQFloat value, bool force, const SQChar * comment)
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to apply the specified information and return the result
    return IniResult("set INI float", m_Doc->SetDoubleValue(section, key, value, comment, force));
}

// ------------------------------------------------------------------------------------------------
IniResult IniDocument::SetBoolean(const SQChar * section, const SQChar * key, bool value, bool force, const SQChar * comment)
{
    // Validate the handle
    m_Doc.Validate();
    // Attempt to apply the specified information
    return IniResult("set INI boolean", m_Doc->SetBoolValue(section, key, value, comment, force));
}

// ------------------------------------------------------------------------------------------------
bool IniDocument::DeleteValue(const SQChar * section, const SQChar * key, const SQChar * value, bool empty)
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
        .Ctor< const SQChar *, SQInteger >()
        .Ctor< const IniResult & >()
        // Core Meta-methods
        .SquirrelFunc(_SC("_typename"), &ResultTypename::Fn)
        .Func(_SC("_tostring"), &IniResult::ToString)
        .Func(_SC("cmp"), &IniResult::Cmp)
        // Properties
        .Prop(_SC("Valid"), &IniResult::IsValid)
        .Prop(_SC("Action"), &IniResult::GetAction)
        .Prop(_SC("Result"), &IniResult::GetResult)
        // Member Methods
        .Func(_SC("Check"), &IniResult::Check)
    );

    inins.Bind(_SC("Entries"),
        Class< IniEntries >(vm, EntriesTypename::Str)
        // Constructors
        .Ctor()
        .Ctor< const IniEntries & >()
        // Core Meta-methods
        .SquirrelFunc(_SC("_typename"), &EntriesTypename::Fn)
        .Func(_SC("_tostring"), &IniEntries::ToString)
        .Func(_SC("cmp"), &IniEntries::Cmp)
        // Properties
        .Prop(_SC("Valid"), &IniEntries::IsValid)
        .Prop(_SC("Empty"), &IniEntries::IsEmpty)
        .Prop(_SC("References"), &IniEntries::GetRefCount)
        .Prop(_SC("Size"), &IniEntries::GetSize)
        .Prop(_SC("Item"), &IniEntries::GetItem)
        .Prop(_SC("Comment"), &IniEntries::GetComment)
        .Prop(_SC("Order"), &IniEntries::GetOrder)
        // Member Methods
        .Func(_SC("Reset"), &IniEntries::Reset)
        .Func(_SC("Next"), &IniEntries::Next)
        .Func(_SC("Prev"), &IniEntries::Prev)
        .Func(_SC("Advance"), &IniEntries::Advance)
        .Func(_SC("Retreat"), &IniEntries::Retreat)
        .Func(_SC("Sort"), &IniEntries::Sort)
        .Func(_SC("SortByKeyOrder"), &IniEntries::SortByKeyOrder)
        .Func(_SC("SortByLoadOrder"), &IniEntries::SortByLoadOrder)
    );

    inins.Bind(_SC("Document"),
        Class< IniDocument, NoCopy< IniDocument > >(vm, DocumentTypename::Str)
        // Constructors
        .Ctor()
        .Ctor< bool >()
        .Ctor< bool, bool >()
        .Ctor< bool, bool, bool >()
        // Core Meta-methods
        .SquirrelFunc(_SC("_typename"), &DocumentTypename::Fn)
        .Func(_SC("_tostring"), &IniDocument::ToString)
        .Func(_SC("cmp"), &IniDocument::Cmp)
        // Properties
        .Prop(_SC("Valid"), &IniDocument::IsValid)
        .Prop(_SC("Empty"), &IniDocument::IsEmpty)
        .Prop(_SC("References"), &IniDocument::GetRefCount)
        .Prop(_SC("Unicode"), &IniDocument::GetUnicode, &IniDocument::SetUnicode)
        .Prop(_SC("MultiKey"), &IniDocument::GetMultiKey, &IniDocument::SetMultiKey)
        .Prop(_SC("MultiLine"), &IniDocument::GetMultiLine, &IniDocument::SetMultiLine)
        .Prop(_SC("Spaces"), &IniDocument::GetSpaces, &IniDocument::SetSpaces)
        // Member Methods
        .Func(_SC("Reset"), &IniDocument::Reset)
        .Func(_SC("LoadFile"), &IniDocument::LoadFile)
        .Overload< IniResult (IniDocument::*)(const SQChar *) >(_SC("LoadString"), &IniDocument::LoadData)
        .Overload< IniResult (IniDocument::*)(const SQChar *, int32_t) >(_SC("LoadString"), &IniDocument::LoadData)
        .Overload< IniResult (IniDocument::*)(const SQChar *) >(_SC("SaveFile"), &IniDocument::SaveFile)
        .Overload< IniResult (IniDocument::*)(const SQChar *, bool) >(_SC("SaveFile"), &IniDocument::SaveFile)
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
        .Overload< IniResult (IniDocument::*)(const SQChar *, const SQChar *, const SQChar *) >(_SC("SetValue"), &IniDocument::SetValue)
        .Overload< IniResult (IniDocument::*)(const SQChar *, const SQChar *, const SQChar *, bool) >(_SC("SetValue"), &IniDocument::SetValue)
        .Overload< IniResult (IniDocument::*)(const SQChar *, const SQChar *, const SQChar *, bool, const SQChar *) >(_SC("SetValue"), &IniDocument::SetValue)
        .Overload< IniResult (IniDocument::*)(const SQChar *, const SQChar *, SQInteger) >(_SC("SetInteger"), &IniDocument::SetInteger)
        .Overload< IniResult (IniDocument::*)(const SQChar *, const SQChar *, SQInteger, bool) >(_SC("SetInteger"), &IniDocument::SetInteger)
        .Overload< IniResult (IniDocument::*)(const SQChar *, const SQChar *, SQInteger, bool, bool) >(_SC("SetInteger"), &IniDocument::SetInteger)
        .Overload< IniResult (IniDocument::*)(const SQChar *, const SQChar *, SQInteger, bool, bool, const SQChar *) >(_SC("SetInteger"), &IniDocument::SetInteger)
        .Overload< IniResult (IniDocument::*)(const SQChar *, const SQChar *, SQFloat) >(_SC("SetFloat"), &IniDocument::SetFloat)
        .Overload< IniResult (IniDocument::*)(const SQChar *, const SQChar *, SQFloat, bool) >(_SC("SetFloat"), &IniDocument::SetFloat)
        .Overload< IniResult (IniDocument::*)(const SQChar *, const SQChar *, SQFloat, bool, const SQChar *) >(_SC("SetFloat"), &IniDocument::SetFloat)
        .Overload< IniResult (IniDocument::*)(const SQChar *, const SQChar *, bool) >(_SC("SetBoolean"), &IniDocument::SetBoolean)
        .Overload< IniResult (IniDocument::*)(const SQChar *, const SQChar *, bool, bool) >(_SC("SetBoolean"), &IniDocument::SetBoolean)
        .Overload< IniResult (IniDocument::*)(const SQChar *, const SQChar *, bool, bool, const SQChar *) >(_SC("SetBoolean"), &IniDocument::SetBoolean)
        .Overload< bool (IniDocument::*)(const SQChar *) >(_SC("DeleteValue"), &IniDocument::DeleteValue)
        .Overload< bool (IniDocument::*)(const SQChar *, const SQChar *) >(_SC("DeleteValue"), &IniDocument::DeleteValue)
        .Overload< bool (IniDocument::*)(const SQChar *, const SQChar *, const SQChar *) >(_SC("DeleteValue"), &IniDocument::DeleteValue)
        .Overload< bool (IniDocument::*)(const SQChar *, const SQChar *, const SQChar *, bool) >(_SC("DeleteValue"), &IniDocument::DeleteValue)
    );

    RootTable(vm).Bind(_SC("SqIni"), inins);

    ConstTable(vm).Enum(_SC("SqIniError"), Enumeration(vm)
        .Const(_SC("Ok"),                       int32_t(SI_OK))
        .Const(_SC("Updated"),                  int32_t(SI_UPDATED))
        .Const(_SC("Inserted"),                 int32_t(SI_INSERTED))
        .Const(_SC("Fail"),                     int32_t(SI_FAIL))
        .Const(_SC("NoMem"),                    int32_t(SI_NOMEM))
        .Const(_SC("File"),                     int32_t(SI_FILE))
    );
}

} // Namespace:: SqMod
