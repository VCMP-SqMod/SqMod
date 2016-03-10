#ifndef _SQMMDB_LOOKUPRESULT_HPP_
#define _SQMMDB_LOOKUPRESULT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class that can be used to traverse a list of results.
*/
class EntryDataList
{
protected:

    // --------------------------------------------------------------------------------------------
    typedef MMDB_entry_data_list_s  Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*                   Pointer; // Pointer to the managed type.
    typedef const Type*             ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&                   Reference; // Reference to the managed type.
    typedef const Type&             ConstRef; // Constant reference to the managed type.

    /* --------------------------------------------------------------------------------------------
     * Validate the database pointer and list handle and throw an error if invalid.
    */
    void Validate() const;

    /* --------------------------------------------------------------------------------------------
     * Do a regular validation and also validate the currently processed element.
    */
    void ValidateElem() const;

    /* --------------------------------------------------------------------------------------------
     * Do a regular validation and also validate the currently processed element data.
    */
    void ValidateData() const;

    /* --------------------------------------------------------------------------------------------
     * Used to retrieve the string representation of the specified type identifier.
    */
    static CSStr AsTypeStr(Uint32 id);

private:

    // ---------------------------------------------------------------------------------------------
    DbRef       m_Db; /* The database from which this list comes from. */
    Pointer     m_List; /* The managed entry data list. */
    Pointer     m_Elem; /* The currently processed element from the list. */

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    EntryDataList(const DbRef & db, Pointer list)
        : m_Db(db), m_List(list), m_Elem(list)
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    EntryDataList();

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    EntryDataList(const EntryDataList &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    EntryDataList(EntryDataList && o)
        : m_Db(o.m_Db)
        , m_List(o.m_List)
        , m_Elem(o.m_Elem)
    {
        o.m_List = nullptr;
        o.m_Elem = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~EntryDataList();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    EntryDataList & operator = (const EntryDataList &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    EntryDataList & operator = (EntryDataList && o)
    {
        if (m_List != o.m_List)
        {
            m_Db = o.m_Db;
            m_List = o.m_List;
            m_Elem = o.m_Elem;
            o.m_List = nullptr;
            o.m_Elem = nullptr;
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal entry data list structure pointer.
    */
    Pointer GetHandle()
    {
        return m_List;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal entry data list structure pointer.
    */
    Pointer GetHandle() const
    {
        return m_List;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const EntryDataList & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return m_Elem ? AsTypeStr(m_Elem->entry_data.type) : _SC("invalid");
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid database and entry data list structure.
    */
    bool IsValid() const
    {
        return m_Db && m_List;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether a valid element is currently processed.
    */
    bool HaveElement() const
    {
        return m_Elem;
    }

    /* --------------------------------------------------------------------------------------------
     * Used to retrieve the type of the current element as a string.
    */
    CSStr TypeStr() const
    {
        // Validate the database and list handle
        Validate();
        // return the requested information
        return m_Elem ? AsTypeStr(m_Elem->entry_data.type) : _SC("invalid");
    }

    /* --------------------------------------------------------------------------------------------
     * Return the total entries in the list.
    */
    Uint32 GetCount() const;

    /* --------------------------------------------------------------------------------------------
     * Go to the next element.
    */
    bool Next();

    /* --------------------------------------------------------------------------------------------
     * Advance a certain number of elements.
    */
    bool Advance(Int32 n);

    /* --------------------------------------------------------------------------------------------
     * Go back to the first element in the list.
    */
    void Reset();

    /* --------------------------------------------------------------------------------------------
     * See whether a valid element is currently processed.
    */
    bool HasData() const
    {
        // Validate the database, list and element handle
        ValidateElem();
        // Return the requested information
        return m_Elem->entry_data.has_data;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the type identifier of the current element.
    */
    SQInteger GetType() const
    {
        // Validate the database, list and element handle
        ValidateElem();
        // Return the requested information
        return static_cast< SQInteger >(m_Elem->entry_data.type);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the offset of the current element.
    */
    SQInteger GetOffset() const
    {
        // Validate the database, list and element handle
        ValidateElem();
        // Return the requested information
        return static_cast< SQInteger >(m_Elem->entry_data.offset);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the offset of the next element.
    */
    SQInteger GetOffsetToNext() const
    {
        // Validate the database, list and element handle
        ValidateElem();
        // Return the requested information
        return static_cast< SQInteger >(m_Elem->entry_data.offset_to_next);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the offset of the next element.
    */
    SQInteger DataSize() const
    {
        // Validate the database, list and element handle
        ValidateElem();
        // Return the requested information
        return static_cast< SQInteger >(m_Elem->entry_data.data_size);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a string.
    */
    CSStr GetString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a native integer.
    */
    SQInteger GetInteger() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a floating point.
    */
    SQFloat GetFloat() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a long integer.
    */
    Object GetLong() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a boolean.
    */
    bool GetBool() const;

    /* --------------------------------------------------------------------------------------------
     * Dumpt the contents of the list to the specified list.
    */
    void DumpTo(CSStr filepath)
    {
        DumpTo(filepath, 0);
    }

    /* --------------------------------------------------------------------------------------------
     * Dumpt the contents of the list to the specified list.
    */
    void DumpTo(CSStr filepath, Int32 indent) const;
};

} // Namespace:: SqMod

#endif // _SQMMDB_LOOKUPRESULT_HPP_
