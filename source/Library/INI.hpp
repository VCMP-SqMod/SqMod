#ifndef _LIBRARY_INI_HPP_
#define _LIBRARY_INI_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include <SimpleIni.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
class IniEntries;
class IniDocument;

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted ini source instance.
*/
class IniRef
{
    // --------------------------------------------------------------------------------------------
    friend class IniDocument;

private:

    // --------------------------------------------------------------------------------------------
    typedef CSimpleIniA     Source;

    // --------------------------------------------------------------------------------------------
    typedef unsigned int    Counter;

    // --------------------------------------------------------------------------------------------
    Source*     m_Ptr; /* The ini reader, writer and manager instance. */
    Counter*    m_Ref; /* Reference count to the managed instance. */

    /* --------------------------------------------------------------------------------------------
     * Grab a strong reference to a ini instance.
    */
    void Grab()
    {
        if (m_Ptr)
        {
            ++(*m_Ref);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Drop a strong reference to a ini instance.
    */
    void Drop()
    {
        if (m_Ptr && --(*m_Ref) == 0)
        {
            delete m_Ptr;
            delete m_Ref;
            m_Ptr = NULL;
            m_Ref = NULL;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    IniRef(bool utf8, bool multikey, bool multiline)
        : m_Ptr(new Source(utf8, multikey, multiline)), m_Ref(new Counter(1))
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    IniRef()
        : m_Ptr(NULL), m_Ref(NULL)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    IniRef(const IniRef & o)
        : m_Ptr(o.m_Ptr), m_Ref(o.m_Ref)

    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~IniRef()
    {
        Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    IniRef & operator = (const IniRef & o)
    {
        if (m_Ptr != o.m_Ptr)
        {
            Drop();
            m_Ptr = o.m_Ptr;
            m_Ref = o.m_Ref;
            Grab();
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two ini instances.
    */
    bool operator == (const IniRef & o) const
    {
        return (m_Ptr == o.m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two ini instances.
    */
    bool operator != (const IniRef & o) const
    {
        return (m_Ptr != o.m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Member operator for dereferencing the managed pointer.
    */
    Source * operator -> () const
    {
        assert(m_Ptr != NULL);
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Indirection operator for obtaining a reference of the managed pointer.
    */
    Source & operator * () const
    {
        assert(m_Ptr != NULL);
        return *m_Ptr;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Class that can access and iterate a series of entries in the ini document.
*/
class IniEntries
{
    // --------------------------------------------------------------------------------------------
    friend class IniDocument;

protected:

    // --------------------------------------------------------------------------------------------
    typedef CSimpleIniA                 Source;

    // --------------------------------------------------------------------------------------------
    typedef Source::TNamesDepend        Container;

    // --------------------------------------------------------------------------------------------
    typedef Container::iterator         Iterator;

    /* ---------------------------------------------------------------------------------------------
     * Default constructor.
    */
    IniEntries(const IniRef & ini, Container & list)
        : m_Doc(ini), m_List(), m_Elem()
    {
        m_List.swap(list);
        Reset();
    }

private:

    // ---------------------------------------------------------------------------------------------
    IniRef      m_Doc; /* The document that contains the elements. */
    Container   m_List; /* The list of elements to iterate. */
    Iterator    m_Elem; /* The currently processed element. */

public:

    /* ---------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    IniEntries()
        : m_Doc(), m_List(), m_Elem(m_List.end())
    {
        /* ... */
    }

    /* ---------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    IniEntries(const IniEntries & o)
        : m_Doc(o.m_Doc), m_List(o.m_List), m_Elem()
    {
        Reset();
    }

    /* ---------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~IniEntries()
    {
        /* ... */
    }

    /* ---------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    IniEntries & operator = (const IniEntries & o)
    {
        m_Doc = o.m_Doc;
        m_List = o.m_List;
        Reset();
        return *this;
    }

    /* ---------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const IniEntries & o);

    /* ---------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

    /* ---------------------------------------------------------------------------------------------
     * Return whether the current element is valid and can be accessed.
    */
    bool IsValid() const
    {
        return !(m_List.empty() || m_Elem == m_List.end());
    }

    /* ---------------------------------------------------------------------------------------------
     * Return whether the entry list is empty.
    */
    bool IsEmpty() const
    {
        return m_List.empty();
    }

    /* ---------------------------------------------------------------------------------------------
     * Return the total entries in the list.
    */
    Int32 GetSize() const
    {
        return (Int32)m_List.size();
    }

    /* ---------------------------------------------------------------------------------------------
     * Reset the internal iterator to the first element.
    */
    void Reset()
    {
        if (m_List.empty())
            m_Elem = m_List.end();
        else
            m_Elem = m_List.begin();
    }

    /* ---------------------------------------------------------------------------------------------
     * Go to the next element.
    */
    void Next();

    /* ---------------------------------------------------------------------------------------------
     * Go to the previous element.
    */
    void Prev();

    /* ---------------------------------------------------------------------------------------------
     * Advance a certain number of elements.
    */
    void Advance(Int32 n);

    /* ---------------------------------------------------------------------------------------------
     * Retreat a certain number of elements.
    */
    void Retreat(Int32 n);

    /* ---------------------------------------------------------------------------------------------
     * Sort the entries using the default options.
    */
    void Sort()
    {
        if (!m_List.empty())
            m_List.sort(Source::Entry::KeyOrder());
    }

    /* ---------------------------------------------------------------------------------------------
     * Sort the entries by name of key only.
    */
    void SortByKeyOrder()
    {
        if (!m_List.empty())
            m_List.sort(Source::Entry::KeyOrder());
    }

    /* ---------------------------------------------------------------------------------------------
     * Sort the entries by their load order and then name of key.
    */
    void SortByLoadOrder()
    {
        if (!m_List.empty())
            m_List.sort(Source::Entry::LoadOrder());
    }

    /* ---------------------------------------------------------------------------------------------
     * Sort the entries by their load order but empty name always goes first.
    */
    void SortByLoadOrderEmptyFirst()
    {
        if (!m_List.empty())
            m_List.sort(Source::Entry::LoadOrderEmptyFirst());
    }

    /* ---------------------------------------------------------------------------------------------
     * Retrieve the string value of the current element item.
    */
    CSStr GetItem() const;

    /* ---------------------------------------------------------------------------------------------
     * Retrieve the string value of the current element comment.
    */
    CSStr GetComment() const;

    /* ---------------------------------------------------------------------------------------------
     * Retrieve the order of the current element.
    */
    Int32 GetOrder() const;
};

/* ------------------------------------------------------------------------------------------------
 * Class that can read/write and alter the contents of ini files.
*/
class IniDocument
{
protected:

    // --------------------------------------------------------------------------------------------
    typedef CSimpleIniA                 Source;

    // --------------------------------------------------------------------------------------------
    typedef Source::TNamesDepend        Container;

    /* ---------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    IniDocument(const IniDocument & o);

    /* ---------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    IniDocument & operator = (const IniDocument & o);

    /* ---------------------------------------------------------------------------------------------
     * Validate the document reference.
    */
    bool Validate() const
    {
        if (m_Doc)
            return true;
        SqThrow("Invalid ini document reference");
        return false;
    }

private:

    // ---------------------------------------------------------------------------------------------
    IniRef  m_Doc; /* The main ini document instance. */

public:

    /* ---------------------------------------------------------------------------------------------
     * Default constructor.
    */
    IniDocument();

    /* ---------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    IniDocument(bool utf8, bool multikey, bool multiline);

    /* ---------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~IniDocument();

    /* ---------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const IniDocument & o);

    /* ---------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

    /* ---------------------------------------------------------------------------------------------
     * See whether this instance references a valid ini document.
    */
    bool IsValid() const
    {
        return m_Doc;
    }

    /* ---------------------------------------------------------------------------------------------
     * See whether any data has been loaded into this document.
    */
    bool IsEmpty() const
    {
        return m_Doc->IsEmpty();
    }

    /* ---------------------------------------------------------------------------------------------
     * Deallocate all memory stored by this document.
    */
    void Reset() const
    {
        m_Doc->Reset();
    }

    /* ---------------------------------------------------------------------------------------------
     * See whether the ini data is treated as unicode.
    */
    bool GetUnicode() const
    {
        return m_Doc->IsUnicode();
    }

    /* ---------------------------------------------------------------------------------------------
     * Set whether the ini data should be treated as unicode.
    */
    void SetUnicode(bool toggle)
    {
        m_Doc->SetUnicode(toggle);
    }

    /* ---------------------------------------------------------------------------------------------
     * See whether multiple identical keys be permitted in the file.
    */
    bool GetMultiKey() const
    {
        return m_Doc->IsMultiKey();
    }

    /* ---------------------------------------------------------------------------------------------
     * Set whether multiple identical keys be permitted in the file.
    */
    void SetMultiKey(bool toggle)
    {
        m_Doc->SetMultiKey(toggle);
    }

    /* ---------------------------------------------------------------------------------------------
     * See whether data values are permitted to span multiple lines in the file.
    */
    bool GetMultiLine() const
    {
        return m_Doc->IsMultiLine();
    }

    /* ---------------------------------------------------------------------------------------------
     * Set whether data values are permitted to span multiple lines in the file.
    */
    void SetMultiLine(bool toggle)
    {
        m_Doc->SetMultiLine(toggle);
    }

    /* ---------------------------------------------------------------------------------------------
     * See whether spaces are added around the equals sign when writing key/value pairs out.
    */
    bool GetSpaces() const
    {
        return m_Doc->UsingSpaces();
    }

    /* ---------------------------------------------------------------------------------------------
     * Set whether spaces are added around the equals sign when writing key/value pairs out.
    */
    void SetSpaces(bool toggle)
    {
        m_Doc->SetSpaces(toggle);
    }

    /* ---------------------------------------------------------------------------------------------
     * Load an INI file from disk into memory.
    */
    void LoadFile(CSStr filepath);

    /* ---------------------------------------------------------------------------------------------
     * Load INI file data direct from a string.
    */
    void LoadData(CSStr source)
    {
        LoadData(source, -1);
    }

    /* ---------------------------------------------------------------------------------------------
     * Load INI file data direct from a string.
    */
    void LoadData(CSStr source, Int32 size);

    /* ---------------------------------------------------------------------------------------------
     * Save an INI file from memory to disk.
    */
    void SaveFile(CSStr filepath)
    {
        SaveFile(filepath, true);
    }

    /* ---------------------------------------------------------------------------------------------
     * Save an INI file from memory to disk.
    */
    void SaveFile(CSStr filepath, bool signature);

    /* ---------------------------------------------------------------------------------------------
     * Save the INI data to a string.
    */
    Object SaveData(bool signature);

    /* ---------------------------------------------------------------------------------------------
     * Retrieve all section names.
    */
    IniEntries GetAllSections() const;

    /* ---------------------------------------------------------------------------------------------
     * Retrieve all unique key names in a section.
    */
    IniEntries GetAllKeys(CSStr section) const;

    /* ---------------------------------------------------------------------------------------------
     * Retrieve all values for a specific key.
    */
    IniEntries GetAllValues(CSStr section, CSStr key) const;

    /* ---------------------------------------------------------------------------------------------
     * Query the number of keys in a specific section.
    */
    Int32 GetSectionSize(CSStr section) const;

    /* ---------------------------------------------------------------------------------------------
     * See whether a certain key has multiple instances.
    */
    bool HasMultipleKeys(CSStr section, CSStr key) const;

    /* ---------------------------------------------------------------------------------------------
     * Retrieve the value for a specific key.
    */
    CCStr GetValue(CSStr section, CSStr key, CSStr def) const;

    /* ---------------------------------------------------------------------------------------------
     * Retrieve a numeric value for a specific key.
    */
    SQInteger GetInteger(CSStr section, CSStr key, SQInteger def) const;

    /* ---------------------------------------------------------------------------------------------
     * Retrieve a numeric value for a specific key.
    */
    SQFloat GetFloat(CSStr section, CSStr key, SQFloat def) const;

    /* ---------------------------------------------------------------------------------------------
     * Retrieve a boolean value for a specific key.
    */
    bool GetBoolean(CSStr section, CSStr key, bool def) const;

    /* ---------------------------------------------------------------------------------------------
     * Add or update a section or value.
    */
    void SetValue(CSStr section, CSStr key, CSStr value)
    {
        SetValue(section, key, value, false, NULL);
    }

    /* ---------------------------------------------------------------------------------------------
     * Add or update a section or value.
    */
    void SetValue(CSStr section, CSStr key, CSStr value, bool force)
    {
        SetValue(section, key, value, force, NULL);
    }

    /* ---------------------------------------------------------------------------------------------
     * Add or update a section or value.
    */
    void SetValue(CSStr section, CSStr key, CSStr value, bool force, CSStr comment);

    /* ---------------------------------------------------------------------------------------------
     * Add or update a numeric value.
    */
    void SetInteger(CSStr section, CSStr key, SQInteger value)
    {
        SetInteger(section, key, value, false, false, NULL);
    }

    /* ---------------------------------------------------------------------------------------------
     * Add or update a numeric value.
    */
    void SetInteger(CSStr section, CSStr key, SQInteger value, bool hex)
    {
        SetInteger(section, key, value, hex, false, NULL);
    }

    /* ---------------------------------------------------------------------------------------------
     * Add or update a numeric value.
    */
    void SetInteger(CSStr section, CSStr key, SQInteger value, bool hex, bool force)
    {
        SetInteger(section, key, value, hex, force, NULL);
    }

    /* ---------------------------------------------------------------------------------------------
     * Add or update a numeric value.
    */
    void SetInteger(CSStr section, CSStr key, SQInteger value, bool hex, bool force, CSStr comment);

    /* ---------------------------------------------------------------------------------------------
     * Add or update a double value.
    */
    void SetFloat(CSStr section, CSStr key, SQFloat value)
    {
        SetFloat(section, key, value, false, NULL);
    }

    /* ---------------------------------------------------------------------------------------------
     * Add or update a double value.
    */
    void SetFloat(CSStr section, CSStr key, SQFloat value, bool force)
    {
        SetFloat(section, key, value, force, NULL);
    }

    /* ---------------------------------------------------------------------------------------------
     * Add or update a double value.
    */
    void SetFloat(CSStr section, CSStr key, SQFloat value, bool force, CSStr comment);

    /* ---------------------------------------------------------------------------------------------
     * Add or update a double value.
    */
    void SetBoolean(CSStr section, CSStr key, bool value)
    {
        SetBoolean(section, key, value, false, NULL);
    }

    /* ---------------------------------------------------------------------------------------------
     * Add or update a double value.
    */
    void SetBoolean(CSStr section, CSStr key, bool value, bool force)
    {
        SetBoolean(section, key, value, force, NULL);
    }

    /* ---------------------------------------------------------------------------------------------
     * Add or update a boolean value.
    */
    void SetBoolean(CSStr section, CSStr key, bool value, bool force, CSStr comment);

    /* ---------------------------------------------------------------------------------------------
     * Delete an entire section, or a key from a section.
    */
    bool DeleteValue(CSStr section)
    {
        return DeleteValue(section, NULL, NULL, false);
    }

    /* ---------------------------------------------------------------------------------------------
     * Delete an entire section, or a key from a section.
    */
    bool DeleteValue(CSStr section, CSStr key)
    {
        return DeleteValue(section, key, NULL, false);
    }

    /* ---------------------------------------------------------------------------------------------
     * Delete an entire section, or a key from a section.
    */
    bool DeleteValue(CSStr section, CSStr key, CSStr value)
    {
        return DeleteValue(section, key, value, false);
    }

    /* ---------------------------------------------------------------------------------------------
     * Delete an entire section, or a key from a section.
    */
    bool DeleteValue(CSStr section, CSStr key, CSStr value, bool empty);
};

} // Namespace:: SqMod

#endif // _LIBRARY_INI_HPP_
