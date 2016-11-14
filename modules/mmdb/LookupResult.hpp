#ifndef _SQMMDB_LOOKUPRESULT_HPP_
#define _SQMMDB_LOOKUPRESULT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Database.hpp"

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
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

private:

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
    Type    m_Result; // The managed result structure.

    /* --------------------------------------------------------------------------------------------
     * Construct and take ownership of a certain result.
    */
    LookupResult(const DbRef & db, Reference result)
        : m_Handle(db), m_Result(result)
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
        return m_Handle && m_Result.found_entry;
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
     * See whether the result contains a valid entry in the associated database.
    */
    bool FoundEntry() const
    {
        // Validate the database handle
        SQMOD_VALIDATE(*this);
        // Return the requested information
        return m_Result.found_entry;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the net-mask from the result structure.
    */
    SQInteger GetNetMask() const
    {
        // Validate the database handle
        SQMOD_VALIDATE(*this);
        // Return the requested information
        return static_cast< SQInteger >(m_Result.netmask);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the entire entry data list.
    */
    Object GetEntryDataList();

    /* --------------------------------------------------------------------------------------------
     * Lookup data in the associated result using the specified path.
    */
    static SQInteger GetValue(HSQUIRRELVM vm);
};

} // Namespace:: SqMod

#endif // _SQMMDB_LOOKUPRESULT_HPP_
