// ------------------------------------------------------------------------------------------------
#include "Database.hpp"
#include "SockAddr.hpp"
#include "LookupResult.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Database::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMDBDatabase");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
void Database::Validate() const
{
    // Is the document handle valid?
    if (!m_Db)
        STHROWF("Invalid Maxmind database reference");
}

// ------------------------------------------------------------------------------------------------
Int32 Database::Cmp(const Database & o) const
{
    if (m_Db == o.m_Db)
        return 0;
    else if (m_Db.m_Ptr > o.m_Db.m_Ptr)
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
void Database::Open(CSStr filepath)
{
    Open(filepath, 0);
}

// ------------------------------------------------------------------------------------------------
void Database::Open(CSStr filepath, Uint32 flags)
{
    // Is there a database handle available?
    if (!m_Db)
        m_Db = DbRef(true); // Create a database handle
    // Check if the database handle could be allocated one more time
    if (!m_Db)
        STHROWF("Unable to create a Maxmind database reference");
    // Are there any other references?
    else if (m_Db.Count() > 1)
        // To load new values now, would mean to cause undefined behavior in existing references
        STHROWF("Loading is disabled while database is referenced");
    // Validate the specified file path
    else if (!filepath || strlen(filepath) <= 0)
        STHROWF("Invalid database file path");
    // Let's attempt to open the specified database
    const Int32 status = MMDB_open(filepath, flags, m_Db.m_Ptr);
    // Validate the result of the operation
    if (status != MMDB_SUCCESS)
    {
        // Release the database reference
        m_Db.Drop();
        // Now it's safe to throw the error
        STHROWF("Unable to open the specified database [%s]", MMDB_strerror(status));
    }
}

// ------------------------------------------------------------------------------------------------
LookupResult Database::LookupString(CSStr addr)
{
    // Validate the database handle
    Validate();
    // Validate the specified string
    if (!addr || strlen(addr) <= 0)
        STHROWF("Invalid address string");
    // Dummy variables to obtain the status codes
    int gai_error, mmdb_error;
    // Attempt to perform the actual lookup
    MMDB_lookup_result_s result = MMDB_lookup_string(m_Db, addr, &gai_error, &mmdb_error);
    // Validate the result of the getaddrinfo() function call
    if (gai_error != 0)
        STHROWF("Unable to resolve address (%s) because [%s]", addr, gai_strerror(gai_error));
    // Validate the lookup status code
    else if (mmdb_error != MMDB_SUCCESS)
        STHROWF("Unable to lookup address (%s) because [%s]", addr, MMDB_strerror(mmdb_error));
    // Now it's safe to return the lookup result
    return LookupResult(m_Db, result);
}

// ------------------------------------------------------------------------------------------------
LookupResult Database::LookupSockAddr(SockAddr & addr)
{
    // Validate the database handle
    Validate();
    // Validate the specified socket address
    if (!addr.IsValid())
        STHROWF("Invalid address instance");
    // Dummy variable to obtain the status codes
    int mmdb_error;
    // Attempt to perform the actual lookup
    MMDB_lookup_result_s result = MMDB_lookup_sockaddr(m_Db, addr.GetHandle()->ai_addr, &mmdb_error);
    // Validate the lookup status code
    if (mmdb_error != MMDB_SUCCESS)
        STHROWF("Unable to lookup address (%s) because [%s]",
                    addr.GetAddress(), MMDB_strerror(mmdb_error));
    // Now it's safe to return the lookup result
    return LookupResult(m_Db, result);
}

} // Namespace:: SqMod
