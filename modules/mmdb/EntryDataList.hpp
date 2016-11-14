#ifndef _SQMMDB_ENTRYDATALIST_HPP_
#define _SQMMDB_ENTRYDATALIST_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Database.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class that can hold and be used inspect database meta-data.
*/
class EntryDataList
{
public:

    // --------------------------------------------------------------------------------------------
    typedef MMDB_entry_data_list_s  Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*                   Pointer; // Pointer to the managed type.
    typedef const Type*             ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&                   Reference; // Reference to the managed type.
    typedef const Type&             ConstRef; // Constant reference to the managed type.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    Pointer GetValid(CCStr file, Int32 line) const;
#else
    Pointer GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    Pointer GetValidElem(CCStr file, Int32 line) const;
#else
    Pointer GetValidElem() const;
#endif // _DEBUG

private:

    // ---------------------------------------------------------------------------------------------
    DbRef   m_Handle; // The database associated with this meta-data.
    Pointer m_List; // The managed entry data list.
    Pointer m_Elem; // The currently processed element from the list.

public:

    /* --------------------------------------------------------------------------------------------
     * Construct and with a specific entry list.
    */
    EntryDataList(const DbRef & db, Pointer list)
        : m_Handle(db), m_List(list), m_Elem(list)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    EntryDataList()
        : m_Handle(), m_List(nullptr), m_Elem(nullptr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    EntryDataList(const EntryDataList &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    EntryDataList(EntryDataList && o)
        : m_Handle(o.m_Handle)
        , m_List(o.m_List)
        , m_Elem(o.m_Elem)
    {
        o.m_List = nullptr;
        o.m_Elem = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~EntryDataList()
    {
        // Do we have to free any list?
        if (m_List)
        {
            MMDB_free_entry_data_list(m_List);
        }
    }

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
            m_Handle = o.m_Handle;
            m_List = o.m_List;
            m_Elem = o.m_Elem;
            o.m_List = nullptr;
            o.m_Elem = nullptr;
        }
        return *this;
    }

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
     * See whether this instance references a valid database and element pointer.
    */
    bool IsValid() const
    {
        return m_Handle && m_Elem;
    }

    /* --------------------------------------------------------------------------------------------
     * Release the manages handles/pointers and become a null instance.
    */
    void Release()
    {
        m_Handle.Reset();
        // Do we have to free any list?
        if (m_List)
        {
            MMDB_free_entry_data_list(m_List);
        }
        // Finally, release those as well
        m_List = nullptr;
        m_Elem = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the database associated with the managed handle/pointer.
    */
    Database GetDatabase() const;

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to the managed database instance.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
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
    CSStr TypeName() const
    {
        return AsTypeStr(SQMOD_GET_VALID_ELEM(*this)->entry_data.type);
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
    bool Advance(SQInteger n);

    /* --------------------------------------------------------------------------------------------
     * Go back to the first element in the list.
    */
    void Reset()
    {
        m_Elem = SQMOD_GET_VALID_ELEM(*this);
    }

    /* --------------------------------------------------------------------------------------------
     * See whether a valid element is currently processed.
    */
    bool HasData() const
    {
        return ConvTo< bool >::From(SQMOD_GET_VALID_ELEM(*this)->entry_data.has_data);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the type identifier of the current element.
    */
    SQInteger GetType() const
    {
        return ConvTo< SQInteger >::From(SQMOD_GET_VALID_ELEM(*this)->entry_data.type);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the offset of the current element.
    */
    SQInteger GetOffset() const
    {
        return ConvTo< SQInteger >::From(SQMOD_GET_VALID_ELEM(*this)->entry_data.offset);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the offset of the next element.
    */
    SQInteger DataSize() const
    {
        return ConvTo< SQInteger >::From(SQMOD_GET_VALID_ELEM(*this)->entry_data.data_size);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a boolean.
    */
    bool GetBool() const
    {
        return GetEntryAsBool(SQMOD_GET_VALID_ELEM(*this)->entry_data);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a native integer.
    */
    SQInteger GetInteger() const
    {
        return GetEntryAsInteger(SQMOD_GET_VALID_ELEM(*this)->entry_data);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a floating point.
    */
    SQFloat GetFloat() const
    {
        return GetEntryAsFloat(SQMOD_GET_VALID_ELEM(*this)->entry_data);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a long integer.
    */
    Object GetLong() const
    {
        return GetEntryAsLong(SQMOD_GET_VALID_ELEM(*this)->entry_data);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a string.
    */
    Object GetString() const
    {
        return GetEntryAsString(SQMOD_GET_VALID_ELEM(*this)->entry_data);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a stream of bytes.
    */
    Object GetBytes() const
    {
        return GetEntryAsBytes(SQMOD_GET_VALID_ELEM(*this)->entry_data);
    }

    /* --------------------------------------------------------------------------------------------
     * Dumpt the contents of the list to the specified list.
    */
    void DumpTo(CSStr filepath) const
    {
        DumpTo(filepath, 0);
    }

    /* --------------------------------------------------------------------------------------------
     * Dumpt the contents of the list to the specified list.
    */
    void DumpTo(CSStr filepath, Int32 indent) const;
};

} // Namespace:: SqMod

#endif // _SQMMDB_ENTRYDATALIST_HPP_
