#ifndef _SQMMDB_DATABASE_HPP_
#define _SQMMDB_DATABASE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class that can read/write and alter the contents of INI files.
*/
class Database
{
protected:

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Database(const Database &);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Database & operator = (const Database &);

    /* --------------------------------------------------------------------------------------------
     * Validate the document reference and throw an error if invalid.
    */
    void Validate() const;

private:

    // ---------------------------------------------------------------------------------------------
    DbRef m_Db; /* The main INI document instance. */

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Database()
        : m_Db()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Database(CSStr filepath)
        : m_Db()
    {
        Open(filepath, 0);
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Database(CSStr filepath, Uint32 flags)
        : m_Db()
    {
        Open(filepath, flags);
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Database()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Database & o) const;

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
        return m_Db;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this document instance.
    */
    Uint32 GetRefCount() const
    {
        return m_Db.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to open the specified database.
    */
    void Open(CSStr filepath);

    /* --------------------------------------------------------------------------------------------
     * Attempt to open the specified database.
    */
    void Open(CSStr filepath, Uint32 addr);

    /* --------------------------------------------------------------------------------------------
     * Look up an IP address that is passed in as a null-terminated string.
    */
    LookupResult LookupString(CSStr addr);

    /* --------------------------------------------------------------------------------------------
     * Looks up an IP address that has already been resolved by getaddrinfo().
    */
    LookupResult LookupSockAddr(SockAddr & sockaddr);

};

} // Namespace:: SqMod

#endif // _SQMMDB_DATABASE_HPP_
