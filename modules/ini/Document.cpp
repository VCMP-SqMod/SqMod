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
    static SQChar name[] = _SC("SqIniDocument");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
void Document::Validate() const
{
    // Is the document handle valid?
    if (!m_Doc)
        SqThrowF("Invalid INI document reference");
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
IniResult Document::LoadFile(CSStr filepath)
{
    // Validate the handle
    Validate();
    // Attempt to load the file from disk and return the result
    return IniResult("load INI file",  m_Doc->LoadFile(filepath));
}

// ------------------------------------------------------------------------------------------------
IniResult Document::LoadData(CSStr source, Int32 size)
{
    // Validate the handle
    Validate();
    // Attempt to load the file from memory and return the result
    return IniResult("load INI file", m_Doc->LoadData(source, size < 0 ? strlen(source) : size));
}

// ------------------------------------------------------------------------------------------------
IniResult Document::SaveFile(CSStr filepath, bool signature)
{
    // Validate the handle
    Validate();
    // Attempt to save the file to disk and return the result
    return IniResult("save INI file", m_Doc->SaveFile(filepath, signature));
}

// ------------------------------------------------------------------------------------------------
Object Document::SaveData(bool signature)
{
    // Validate the handle
    Validate();
    // The string where the content will be saved
    String source;
    // Attempt to save the data to string
    if (m_Doc->Save(source, signature) < 0)
        SqThrowF("Unable to save INI document");
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
    Validate();
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
    Validate();
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
    Validate();
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
    Validate();
    // Return the requested information
    return m_Doc->GetSectionSize(section);
}

// ------------------------------------------------------------------------------------------------
bool Document::HasMultipleKeys(CSStr section, CSStr key) const
{
    // Validate the handle
    Validate();
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
    // Validate the handle
    Validate();
    // Attempt to query the information and return it
    return m_Doc->GetValue(section, key, def, NULL);
}

// ------------------------------------------------------------------------------------------------
SQInteger Document::GetInteger(CSStr section, CSStr key, SQInteger def) const
{
    // Validate the handle
    Validate();
    // Attempt to query the information and return it
    return (SQInteger)m_Doc->GetLongValue(section, key, def, NULL);
}

// ------------------------------------------------------------------------------------------------
SQFloat Document::GetFloat(CSStr section, CSStr key, SQFloat def) const
{
    // Validate the handle
    Validate();
    // Attempt to query the information and return it
    return (SQFloat)m_Doc->GetDoubleValue(section, key, def, NULL);
}

// ------------------------------------------------------------------------------------------------
bool Document::GetBoolean(CSStr section, CSStr key, bool def) const
{
    // Validate the handle
    Validate();
    // Attempt to query the information and return it
    return m_Doc->GetBoolValue(section, key, def, NULL);
}

// ------------------------------------------------------------------------------------------------
IniResult Document::SetValue(CSStr section, CSStr key, CSStr value, bool force, CSStr comment)
{
    // Validate the handle
    Validate();
    // Attempt to apply the specified information and return the result
    return IniResult("set INI value", m_Doc->SetValue(section, key, value, comment, force));
}

// ------------------------------------------------------------------------------------------------
IniResult Document::SetInteger(CSStr section, CSStr key, SQInteger value, bool hex, bool force, CSStr comment)
{
    // Validate the handle
    Validate();
    // Attempt to apply the specified information and return the result
    return IniResult("set INI integer", m_Doc->SetLongValue(section, key, value, comment, hex, force));
}

// ------------------------------------------------------------------------------------------------
IniResult Document::SetFloat(CSStr section, CSStr key, SQFloat value, bool force, CSStr comment)
{
    // Validate the handle
    Validate();
    // Attempt to apply the specified information and return the result
    return IniResult("set INI float", m_Doc->SetDoubleValue(section, key, value, comment, force));
}

// ------------------------------------------------------------------------------------------------
IniResult Document::SetBoolean(CSStr section, CSStr key, bool value, bool force, CSStr comment)
{
    // Validate the handle
    Validate();
    // Attempt to apply the specified information
    return IniResult("set INI boolean", m_Doc->SetBoolValue(section, key, value, comment, force));
}

// ------------------------------------------------------------------------------------------------
bool Document::DeleteValue(CSStr section, CSStr key, CSStr value, bool empty)
{
    // Validate the handle
    Validate();
    // Attempt to remove the specified value and return the result
    return m_Doc->DeleteValue(section, key, value, empty);
}

} // Namespace:: SqMod
