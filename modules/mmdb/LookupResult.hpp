#ifndef _SQMMDB_LOOKUPRESULT_HPP_
#define _SQMMDB_LOOKUPRESULT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class that can hold and be used to work with lookup results.
*/
class LookupResult
{
    // --------------------------------------------------------------------------------------------
    friend class Database; // Only a valid database instance can construct this type.

protected:

    // --------------------------------------------------------------------------------------------
    typedef MMDB_lookup_result_s    Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*                   Pointer; // Pointer to the managed type.
    typedef const Type*             ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&                   Reference; // Reference to the managed type.
    typedef const Type&             ConstRef; // Constant reference to the managed type.

    /* --------------------------------------------------------------------------------------------
     * Validate the database pointer and throw an error if invalid.
    */
    void Validate() const;

private:

    // ---------------------------------------------------------------------------------------------
    DbRef   m_Db; /* The database from which this result comes from. */
    Type    m_Result; /* The managed result structure. */

    /* --------------------------------------------------------------------------------------------
     * Construct and take ownership of a certain result.
    */
    LookupResult(const DbRef & db, Reference result)
        : m_Db(db), m_Result(result)
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    LookupResult();

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    LookupResult(const LookupResult &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    LookupResult(LookupResult &&) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~LookupResult()
    {
        /* We let the smart reference deal with deallocations if necessary. */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    LookupResult & operator = (const LookupResult &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    LookupResult & operator = (LookupResult &&) = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal result structure reference.
    */
    Reference GetHandle()
    {
        return m_Result;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal result structure reference.
    */
    ConstRef GetHandle() const
    {
        return m_Result;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const LookupResult & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return FmtStr("%u", m_Result.entry.offset);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid database and result structure.
    */
    bool IsValid() const
    {
        return m_Db && m_Result.found_entry;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the result contains a valid entry in the associated database.
    */
    bool FoundEntry() const
    {
        // Validate the database handle
        Validate();
        // Return the requested information
        return m_Result.found_entry;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the net-mask from the result structure.
    */
    SQInteger GetNetMask() const
    {
        // Validate the database handle
        Validate();
        // Return the requested information
        return static_cast< SQInteger >(m_Result.netmask);
    }

    /* --------------------------------------------------------------------------------------------
     * Lookup data in the associated result using an array as the path.
    */
    EntryDataList GetValueA(CSStr path, Array & arr) const;

    /* --------------------------------------------------------------------------------------------
     * Lookup data in the associated result using a table as the path.
    */
    EntryDataList GetValueT(CSStr path, Table & tbl) const;

};

} // Namespace:: SqMod

#endif // _SQMMDB_LOOKUPRESULT_HPP_
