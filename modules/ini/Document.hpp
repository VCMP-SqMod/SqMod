#ifndef _SQINI_DOCUMENT_HPP_
#define _SQINI_DOCUMENT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class that can read/write and alter the contents of INI files.
*/
class Document
{
protected:

    // --------------------------------------------------------------------------------------------
    typedef DocumentRef::Type::TNamesDepend Container;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Document(const Document & o);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Document & operator = (const Document & o);

private:

    // ---------------------------------------------------------------------------------------------
    DocumentRef     m_Doc; /* The main INI document instance. */

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Document()
        : m_Doc(false, false, true)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    Document(bool utf8)
        : m_Doc(utf8, false, true)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    Document(bool utf8, bool multikey)
        : m_Doc(utf8, multikey, true)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    Document(bool utf8, bool multikey, bool multiline)
        : m_Doc(utf8, multikey, multiline)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Document()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Document & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid INI document.
    */
    bool IsValid() const
    {
        return m_Doc;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this document instance.
    */
    Uint32 GetRefCount() const
    {
        return m_Doc.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether any data has been loaded into this document.
    */
    bool IsEmpty() const
    {
        return m_Doc->IsEmpty();
    }

    /* --------------------------------------------------------------------------------------------
     * Deallocate all memory stored by this document.
    */
    void Reset() const
    {
        m_Doc->Reset();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the INI data is treated as unicode.
    */
    bool GetUnicode() const
    {
        return m_Doc->IsUnicode();
    }

    /* --------------------------------------------------------------------------------------------
     * Set whether the INI data should be treated as unicode.
    */
    void SetUnicode(bool toggle)
    {
        m_Doc->SetUnicode(toggle);
    }

    /* --------------------------------------------------------------------------------------------
     * See whether multiple identical keys be permitted in the file.
    */
    bool GetMultiKey() const
    {
        return m_Doc->IsMultiKey();
    }

    /* --------------------------------------------------------------------------------------------
     * Set whether multiple identical keys be permitted in the file.
    */
    void SetMultiKey(bool toggle)
    {
        m_Doc->SetMultiKey(toggle);
    }

    /* --------------------------------------------------------------------------------------------
     * See whether data values are permitted to span multiple lines in the file.
    */
    bool GetMultiLine() const
    {
        return m_Doc->IsMultiLine();
    }

    /* --------------------------------------------------------------------------------------------
     * Set whether data values are permitted to span multiple lines in the file.
    */
    void SetMultiLine(bool toggle)
    {
        m_Doc->SetMultiLine(toggle);
    }

    /* --------------------------------------------------------------------------------------------
     * See whether spaces are added around the equals sign when writing key/value pairs out.
    */
    bool GetSpaces() const
    {
        return m_Doc->UsingSpaces();
    }

    /* --------------------------------------------------------------------------------------------
     * Set whether spaces are added around the equals sign when writing key/value pairs out.
    */
    void SetSpaces(bool toggle)
    {
        m_Doc->SetSpaces(toggle);
    }

    /* --------------------------------------------------------------------------------------------
     * Load an INI file from disk into memory.
    */
    IniResult LoadFile(CSStr filepath);

    /* --------------------------------------------------------------------------------------------
     * Load INI file data direct from a string. (LoadString collides with the windows api)
    */
    IniResult LoadData(CSStr source)
    {
        return LoadData(source, -1);
    }

    /* --------------------------------------------------------------------------------------------
     * Load INI file data direct from a string. (LoadString collides with the windows api)
    */
    IniResult LoadData(CSStr source, Int32 size);

    /* --------------------------------------------------------------------------------------------
     * Save an INI file from memory to disk.
    */
    IniResult SaveFile(CSStr filepath)
    {
        return SaveFile(filepath, true);
    }

    /* --------------------------------------------------------------------------------------------
     * Save an INI file from memory to disk.
    */
    IniResult SaveFile(CSStr filepath, bool signature);

    /* --------------------------------------------------------------------------------------------
     * Save the INI data to a string.
    */
    Object SaveData(bool signature);

    /* --------------------------------------------------------------------------------------------
     * Retrieve all section names.
    */
    Entries GetAllSections() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve all unique key names in a section.
    */
    Entries GetAllKeys(CSStr section) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve all values for a specific key.
    */
    Entries GetAllValues(CSStr section, CSStr key) const;

    /* --------------------------------------------------------------------------------------------
     * Query the number of keys in a specific section.
    */
    Int32 GetSectionSize(CSStr section) const;

    /* --------------------------------------------------------------------------------------------
     * See whether a certain key has multiple instances.
    */
    bool HasMultipleKeys(CSStr section, CSStr key) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value for a specific key.
    */
    CCStr GetValue(CSStr section, CSStr key, CSStr def) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a numeric value for a specific key.
    */
    SQInteger GetInteger(CSStr section, CSStr key, SQInteger def) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a numeric value for a specific key.
    */
    SQFloat GetFloat(CSStr section, CSStr key, SQFloat def) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a boolean value for a specific key.
    */
    bool GetBoolean(CSStr section, CSStr key, bool def) const;

    /* --------------------------------------------------------------------------------------------
     * Add or update a section or value.
    */
    IniResult SetValue(CSStr section, CSStr key, CSStr value)
    {
        return SetValue(section, key, value, false, nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Add or update a section or value.
    */
    IniResult SetValue(CSStr section, CSStr key, CSStr value, bool force)
    {
        return SetValue(section, key, value, force, nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Add or update a section or value.
    */
    IniResult SetValue(CSStr section, CSStr key, CSStr value, bool force, CSStr comment);

    /* --------------------------------------------------------------------------------------------
     * Add or update a numeric value.
    */
    IniResult SetInteger(CSStr section, CSStr key, SQInteger value)
    {
        return SetInteger(section, key, value, false, false, nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Add or update a numeric value.
    */
    IniResult SetInteger(CSStr section, CSStr key, SQInteger value, bool hex)
    {
        return SetInteger(section, key, value, hex, false, nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Add or update a numeric value.
    */
    IniResult SetInteger(CSStr section, CSStr key, SQInteger value, bool hex, bool force)
    {
        return SetInteger(section, key, value, hex, force, nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Add or update a numeric value.
    */
    IniResult SetInteger(CSStr section, CSStr key, SQInteger value, bool hex, bool force, CSStr comment);

    /* --------------------------------------------------------------------------------------------
     * Add or update a double value.
    */
    IniResult SetFloat(CSStr section, CSStr key, SQFloat value)
    {
        return SetFloat(section, key, value, false, nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Add or update a double value.
    */
    IniResult SetFloat(CSStr section, CSStr key, SQFloat value, bool force)
    {
        return SetFloat(section, key, value, force, nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Add or update a double value.
    */
    IniResult SetFloat(CSStr section, CSStr key, SQFloat value, bool force, CSStr comment);

    /* --------------------------------------------------------------------------------------------
     * Add or update a double value.
    */
    IniResult SetBoolean(CSStr section, CSStr key, bool value)
    {
        return SetBoolean(section, key, value, false, nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Add or update a double value.
    */
    IniResult SetBoolean(CSStr section, CSStr key, bool value, bool force)
    {
        return SetBoolean(section, key, value, force, nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Add or update a boolean value.
    */
    IniResult SetBoolean(CSStr section, CSStr key, bool value, bool force, CSStr comment);

    /* --------------------------------------------------------------------------------------------
     * Delete an entire section, or a key from a section.
    */
    bool DeleteValue(CSStr section)
    {
        return DeleteValue(section, nullptr, nullptr, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Delete an entire section, or a key from a section.
    */
    bool DeleteValue(CSStr section, CSStr key)
    {
        return DeleteValue(section, key, nullptr, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Delete an entire section, or a key from a section.
    */
    bool DeleteValue(CSStr section, CSStr key, CSStr value)
    {
        return DeleteValue(section, key, value, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Delete an entire section, or a key from a section.
    */
    bool DeleteValue(CSStr section, CSStr key, CSStr value, bool empty);
};

} // Namespace:: SqMod

#endif // _SQINI_DOCUMENT_HPP_
