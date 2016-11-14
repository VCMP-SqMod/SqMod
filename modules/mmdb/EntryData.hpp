#ifndef _SQMMDB_ENTRYDATA_HPP_
#define _SQMMDB_ENTRYDATA_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Database.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class that can hold and be used to inspect entry data values.
*/
class EntryData
{
public:

    // --------------------------------------------------------------------------------------------
    typedef MMDB_entry_data_s       Type; // The managed type.

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
    const DbRef & GetValid(CCStr file, Int32 line) const;
#else
    const DbRef & GetValid() const;
#endif // _DEBUG

private:

    // ---------------------------------------------------------------------------------------------
    DbRef   m_Handle; // The database from which this result comes from.
    Type    m_Entry; // The managed entry-data structure.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    EntryData();

    /* --------------------------------------------------------------------------------------------
     * Construct and take ownership of a certain entry data.
    */
    EntryData(const DbRef & db, Reference entry)
        : m_Handle(db), m_Entry(entry)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    EntryData(const EntryData &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    EntryData(EntryData &&) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    EntryData & operator = (const EntryData &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    EntryData & operator = (EntryData &&) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return AsTypeStr(m_Entry.type);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid database and entry structure.
    */
    bool IsValid() const
    {
        return m_Handle && m_Entry.has_data;
    }

    /* --------------------------------------------------------------------------------------------
     * Release the manages handles/pointers and become a null instance.
    */
    void Release();

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
     * Used to retrieve the type of the current element as a string.
    */
    CSStr TypeName() const
    {
        // Validate the handle
        SQMOD_VALIDATE(*this);
        // Return the requested information
        return AsTypeStr(m_Entry.type);
    }

    /* --------------------------------------------------------------------------------------------
     * See whether a valid element is currently processed.
    */
    bool HasData() const
    {
        // Validate the handle
        SQMOD_VALIDATE(*this);
        // Return the requested information
        return ConvTo< bool >::From(m_Entry.has_data);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the type identifier of the current element.
    */
    SQInteger GetType() const
    {
        // Validate the handle
        SQMOD_VALIDATE(*this);
        // Return the requested information
        return ConvTo< SQInteger >::From(m_Entry.type);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the offset of the current element.
    */
    SQInteger GetOffset() const
    {
        // Validate the handle
        SQMOD_VALIDATE(*this);
        // Return the requested information
        return ConvTo< SQInteger >::From(m_Entry.offset);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the offset of the next element.
    */
    SQInteger DataSize() const
    {
        // Validate the handle
        SQMOD_VALIDATE(*this);
        // Return the requested information
        return ConvTo< SQInteger >::From(m_Entry.data_size);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a boolean.
    */
    bool GetBool() const
    {
        // Validate the handle
        SQMOD_VALIDATE(*this);
        // Return the requested information
        return GetEntryAsBool(m_Entry);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a native integer.
    */
    SQInteger GetInteger() const
    {
        // Validate the handle
        SQMOD_VALIDATE(*this);
        // Return the requested information
        return GetEntryAsInteger(m_Entry);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a floating point.
    */
    SQFloat GetFloat() const
    {
        // Validate the handle
        SQMOD_VALIDATE(*this);
        // Return the requested information
        return GetEntryAsFloat(m_Entry);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a long integer.
    */
    Object GetLong() const
    {
        // Validate the handle
        SQMOD_VALIDATE(*this);
        // Return the requested information
        return GetEntryAsLong(m_Entry);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a string.
    */
    Object GetString() const
    {
        // Validate the handle
        SQMOD_VALIDATE(*this);
        // Return the requested information
        return GetEntryAsString(m_Entry);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a stream of bytes.
    */
    Object GetBytes() const
    {
        // Validate the handle
        SQMOD_VALIDATE(*this);
        // Return the requested information
        return GetEntryAsBytes(m_Entry);
    }

};

} // Namespace:: SqMod

#endif // _SQMMDB_ENTRYDATA_HPP_
