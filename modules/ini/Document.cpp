// ------------------------------------------------------------------------------------------------
#include "Document.hpp"
#include "Entries.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
#include <errno.h>

// --------------------------------------------------------------------------------------------
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Document::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqIniDocument");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
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
    const StackGuard sg(_SqVM);
    // Transform it into a script object
    sq_pushstring(_SqVM, source.c_str(), source.size());
    // Get the object from the stack and return it
    return Var< Object >(_SqVM, -1).value;
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

} // Namespace:: SqMod
