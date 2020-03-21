#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include <SimpleIni.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Allows the user to inspect the result of certain operations and act accordingly.
*/
class IniResult
{
private:

    // --------------------------------------------------------------------------------------------
    String      m_Action; // The action that was performed.
    SQInteger   m_Result; // The internal result code.

public:

    /* --------------------------------------------------------------------------------------------
     * Construct with no specific action or result.
    */
    IniResult()
        : m_Action("unknown action"), m_Result(SI_OK)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Construct with no specific result.
    */
    explicit IniResult(CSStr action)
        : m_Action(!action ? _SC("") : action), m_Result(SI_OK)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Construct with no specific action.
    */
    explicit IniResult(SQInteger result)
        : m_Action("unknown action"), m_Result(result)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Construct with specific action and result.
    */
    IniResult(CSStr action, SQInteger result)
        : m_Action(!action ? _SC("") : action), m_Result(result)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    IniResult(const IniResult & o)
        : m_Action(o.m_Action), m_Result(o.m_Result)

    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~IniResult()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    IniResult & operator = (const IniResult & o)
    {
        m_Action = o.m_Action;
        m_Result = o.m_Result;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two results.
    */
    bool operator == (const IniResult & o) const
    {
        return (m_Result == o.m_Result);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two results.
    */
    bool operator != (const IniResult & o) const
    {
        return (m_Result != o.m_Result);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const
    {
        return (m_Result >= 0);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const IniResult & o) const
    {
        if (m_Result == o.m_Result)
        {
            return 0;
        }
        else if (m_Result > o.m_Result)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return m_Action.c_str();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid INI result.
    */
    bool IsValid() const
    {
        return (m_Result >= 0);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated action.
    */
    CSStr GetAction() const
    {
        return m_Action.c_str();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the resulted code.
    */
    SQInteger GetResult() const
    {
        return m_Result;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the resulted code.
    */
    void Check() const;
};

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted INI document instance.
*/
class IniDocumentRef
{
    // --------------------------------------------------------------------------------------------
    friend class IniDocument;

public:

    // --------------------------------------------------------------------------------------------
    typedef CSimpleIniA     Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef unsigned int    Counter; // Reference counter type.

    /* --------------------------------------------------------------------------------------------
     * Validate the document reference and throw an error if invalid.
    */
    void Validate() const;

private:

    // --------------------------------------------------------------------------------------------
    Pointer     m_Ptr; // The document reader, writer and manager instance.
    Counter*    m_Ref; // Reference count to the managed instance.

    /* --------------------------------------------------------------------------------------------
     * Grab a strong reference to a document instance.
    */
    void Grab()
    {
        if (m_Ptr)
        {
            ++(*m_Ref);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Drop a strong reference to a document instance.
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
    IniDocumentRef(bool utf8, bool multikey, bool multiline)
        : m_Ptr(new Type(utf8, multikey, multiline)), m_Ref(new Counter(1))
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    IniDocumentRef()
        : m_Ptr(NULL), m_Ref(NULL)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    IniDocumentRef(const IniDocumentRef & o)
        : m_Ptr(o.m_Ptr), m_Ref(o.m_Ref)

    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    IniDocumentRef(IniDocumentRef && o)
        : m_Ptr(o.m_Ptr), m_Ref(o.m_Ref)

    {
        o.m_Ptr = NULL;
        o.m_Ref = NULL;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~IniDocumentRef()
    {
        Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    IniDocumentRef & operator = (const IniDocumentRef & o)
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
     * Move assignment operator.
    */
    IniDocumentRef & operator = (IniDocumentRef && o)
    {
        if (m_Ptr != o.m_Ptr)
        {
            m_Ptr = o.m_Ptr;
            m_Ref = o.m_Ref;
            o.m_Ptr = NULL;
            o.m_Ref = NULL;
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two document instances.
    */
    bool operator == (const IniDocumentRef & o) const
    {
        return (m_Ptr == o.m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two document instances.
    */
    bool operator != (const IniDocumentRef & o) const
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
     * Implicit conversion to the managed instance pointer.
    */
    operator Pointer ()
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance pointer.
    */
    operator ConstPtr () const
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance reference.
    */
    operator Reference ()
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance reference.
    */
    operator ConstRef () const
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Member operator for dereferencing the managed pointer.
    */
    Pointer operator -> () const
    {
        assert(m_Ptr);
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Indirection operator for obtaining a reference of the managed pointer.
    */
    Reference operator * () const
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of active references to the managed instance.
    */
    Counter Count() const
    {
        return (m_Ptr && m_Ref) ? (*m_Ref) : 0;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Class that can access and iterate a series of entries in the INI document.
*/
class IniEntries
{
    // --------------------------------------------------------------------------------------------
    friend class IniDocument;

protected:

    // --------------------------------------------------------------------------------------------
    typedef IniDocumentRef::Type::TNamesDepend     Container;

    // --------------------------------------------------------------------------------------------
    typedef Container::iterator                 Iterator;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    IniEntries(const IniDocumentRef & ini, Container & list)
        : m_Doc(ini), m_List(), m_Elem()
    {
        m_List.swap(list);
        Reset();
    }

private:

    // ---------------------------------------------------------------------------------------------
    IniDocumentRef     m_Doc; // The document that contains the elements.
    Container       m_List; // The list of elements to iterate.
    Iterator        m_Elem; // The currently processed element.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    IniEntries()
        : m_Doc(), m_List(), m_Elem(m_List.end())
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    IniEntries(const IniEntries & o)
        : m_Doc(o.m_Doc), m_List(o.m_List), m_Elem()
    {
        Reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~IniEntries()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    IniEntries & operator = (const IniEntries & o)
    {
        m_Doc = o.m_Doc;
        m_List = o.m_List;
        Reset();
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const IniEntries & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return GetItem();
    }

    /* --------------------------------------------------------------------------------------------
     * Return whether the current element is valid and can be accessed.
    */
    bool IsValid() const
    {
        return !(m_List.empty() || m_Elem == m_List.end());
    }

    /* --------------------------------------------------------------------------------------------
     * Return whether the entry list is empty.
    */
    bool IsEmpty() const
    {
        return m_List.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this document instance.
    */
    Uint32 GetRefCount() const
    {
        return m_Doc.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the total entries in the list.
    */
    Int32 GetSize() const
    {
        return static_cast< Int32 >(m_List.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Reset the internal iterator to the first element.
    */
    void Reset()
    {
        if (m_List.empty())
        {
            m_Elem = m_List.end();
        }
        else
        {
            m_Elem = m_List.begin();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Go to the next element.
    */
    void Next();

    /* --------------------------------------------------------------------------------------------
     * Go to the previous element.
    */
    void Prev();

    /* --------------------------------------------------------------------------------------------
     * Advance a certain number of elements.
    */
    void Advance(Int32 n);

    /* --------------------------------------------------------------------------------------------
     * Retreat a certain number of elements.
    */
    void Retreat(Int32 n);

    /* --------------------------------------------------------------------------------------------
     * Sort the entries using the default options.
    */
    void Sort()
    {
        if (!m_List.empty())
        {
            m_List.sort(IniDocumentRef::Type::Entry::KeyOrder());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Sort the entries by name of key only.
    */
    void SortByKeyOrder()
    {
        if (!m_List.empty())
        {
            m_List.sort(IniDocumentRef::Type::Entry::KeyOrder());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Sort the entries by their load order and then name of key.
    */
    void SortByLoadOrder()
    {
        if (!m_List.empty())
        {
            m_List.sort(IniDocumentRef::Type::Entry::LoadOrder());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the string value of the current element item.
    */
    CSStr GetItem() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the string value of the current element comment.
    */
    CSStr GetComment() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the order of the current element.
    */
    Int32 GetOrder() const;
};

/* ------------------------------------------------------------------------------------------------
 * Class that can read/write and alter the contents of INI files.
*/
class IniDocument
{
protected:

    // --------------------------------------------------------------------------------------------
    typedef IniDocumentRef::Type::TNamesDepend Container;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    IniDocument(const IniDocument & o);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    IniDocument & operator = (const IniDocument & o);

private:

    // ---------------------------------------------------------------------------------------------
    IniDocumentRef     m_Doc; // The main INI document instance.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    IniDocument()
        : m_Doc(false, false, true)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    IniDocument(bool utf8)
        : m_Doc(utf8, false, true)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    IniDocument(bool utf8, bool multikey)
        : m_Doc(utf8, multikey, true)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    IniDocument(bool utf8, bool multikey, bool multiline)
        : m_Doc(utf8, multikey, multiline)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~IniDocument()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const IniDocument & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return _SC("");
    }

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
    IniEntries GetAllSections() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve all unique key names in a section.
    */
    IniEntries GetAllKeys(CSStr section) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve all values for a specific key.
    */
    IniEntries GetAllValues(CSStr section, CSStr key) const;

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
