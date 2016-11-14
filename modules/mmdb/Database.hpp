#ifndef _SQMMDB_DATABASE_HPP_
#define _SQMMDB_DATABASE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Database.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class that can be used to open and query information from MaxMind database files.
*/
class Database
{
public:

    // --------------------------------------------------------------------------------------------
    typedef DbHnd::Type             Type; // The managed type.

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
#endif //

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
    DbRef m_Handle; // The managed database handle.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    Database()
        : m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor. (default flags)
    */
    Database(CSStr filepath)
        : m_Handle(new DbHnd(filepath, 0))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Database(CSStr filepath, Uint32 flags)
        : m_Handle(new DbHnd(filepath, flags))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit handle constructor.
    */
    Database(const DbRef & db)
        : m_Handle(db)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Database(const Database & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Database(Database && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Database & operator = (const Database & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Database & operator = (Database && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return m_Handle ? m_Handle->mDb.filename : _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid database instance.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Release the manages handles/pointers and become a null instance.
    */
    void Release()
    {
        m_Handle.Reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to the managed database instance.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to open the specified database.
    */
    void Open(CSStr filepath)
    {
        Open(filepath, 0);
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to open the specified database.
    */
    void Open(CSStr filepath, Uint32 flags)
    {
        // Make sure there isn't another database handle
        if (!m_Handle)
        {
            m_Handle = DbRef(new DbHnd(filepath, flags));
        }
        else
        {
            STHROWF("Loading is disabled while database is referenced");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the metadata associated with the managed database handle.
    */
    Metadata GetMetadata() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the metadata associated with the managed database handle as an entry data list.
    */
    Object GetMetadataAsEntryDataList() const;

    /* --------------------------------------------------------------------------------------------
     * Look up an IP address that is passed in as a null-terminated string.
    */
    LookupResult LookupString(CSStr addr);

    /* --------------------------------------------------------------------------------------------
     * Looks up an IP address that has already been resolved by getaddrinfo().
    */
    LookupResult LookupSockAddr(SockAddr & sockaddr);

    /* --------------------------------------------------------------------------------------------
     * Retrieve a speciffic node from the managed database.
    */
    SearchNode ReadNode(Uint32 node) const;
};

} // Namespace:: SqMod

#endif // _SQMMDB_DATABASE_HPP_
