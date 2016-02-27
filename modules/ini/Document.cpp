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
bool Document::Validate() const
{
    if (m_Doc)
        return true;
    // No document available
    _SqMod->SqThrow("Invalid INI document reference");
    return false;
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
    if (!Validate())
        return IniResult("load INI file", SI_BADREF); /* Unable to proceed */
    // Attempt to load the file from disk and return the result
    return IniResult("load INI file",  m_Doc->LoadFile(filepath));
}

// ------------------------------------------------------------------------------------------------
IniResult Document::LoadData(CSStr source, Int32 size)
{
    if (!Validate())
        return IniResult("load INI file", SI_BADREF); /* Unable to proceed */
    // Attempt to load the file from memory and return the result
    return IniResult("load INI file", m_Doc->LoadData(source, size < 0 ? strlen(source) : size));
}

// ------------------------------------------------------------------------------------------------
IniResult Document::SaveFile(CSStr filepath, bool signature)
{
    if (!Validate())
        return IniResult("save INI file", SI_BADREF); /* Unable to proceed */
    // Attempt to save the file to disk and return the result
    return IniResult("save INI file", m_Doc->SaveFile(filepath, signature));
}

// ------------------------------------------------------------------------------------------------
Object Document::SaveData(bool signature)
{
    if (!Validate())
        return Object(); /* Unable to proceed */
    // The string where the content will be saved
    String source;
    // Attempt to save the data to string
    if (m_Doc->Save(source, signature) < 0)
        return Object(); /* Unable to proceed */
    // Transform it into a script object
    sq_pushstring(DefaultVM::Get(), source.c_str(), source.size());
    // Get the object from the stack
    Var< Object > var(DefaultVM::Get(), -1);
    // Pop the created object from the stack
    if (!var.value.IsNull())
        sq_pop(DefaultVM::Get(), 1);
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
        return SQFloat(0.0); /* Unable to proceed */
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
IniResult Document::SetValue(CSStr section, CSStr key, CSStr value, bool force, CSStr comment)
{
    if (!Validate())
        return IniResult("set INI value", SI_BADREF); /* Unable to proceed */
    // Attempt to apply the specified information and return the result
    return IniResult("set INI value", m_Doc->SetValue(section, key, value, comment, force));
}

// ------------------------------------------------------------------------------------------------
IniResult Document::SetInteger(CSStr section, CSStr key, SQInteger value, bool hex, bool force, CSStr comment)
{
    if (!Validate())
        return IniResult("set INI integer", SI_BADREF); /* Unable to proceed */
    // Attempt to apply the specified information and return the result
    return IniResult("set INI integer", m_Doc->SetLongValue(section, key, value, comment, hex, force));
}

// ------------------------------------------------------------------------------------------------
IniResult Document::SetFloat(CSStr section, CSStr key, SQFloat value, bool force, CSStr comment)
{
    if (!Validate())
        return IniResult("set INI float", SI_BADREF); /* Unable to proceed */
    // Attempt to apply the specified information and return the result
    return IniResult("set INI float", m_Doc->SetDoubleValue(section, key, value, comment, force));
}

// ------------------------------------------------------------------------------------------------
IniResult Document::SetBoolean(CSStr section, CSStr key, bool value, bool force, CSStr comment)
{
    if (!Validate())
        return IniResult("set INI boolean", SI_BADREF); /* Unable to proceed */
    // Attempt to apply the specified information
    return IniResult("set INI boolean", m_Doc->SetBoolValue(section, key, value, comment, force));
}

// ------------------------------------------------------------------------------------------------
bool Document::DeleteValue(CSStr section, CSStr key, CSStr value, bool empty)
{
    if (!Validate())
        return false; /* Unable to proceed */
    // Attempt to remove the specified value and return the result
    return m_Doc->DeleteValue(section, key, value, empty);
}

} // Namespace:: SqMod
