// ------------------------------------------------------------------------------------------------
#include "Database.hpp"
#include "SockAddr.hpp"
#include "Metadata.hpp"
#include "LookupResult.hpp"
#include "EntryDataList.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Database::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMDatabase");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Database::Validate(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid Maxmind database reference =>[%s:%d]", file, line);
    }
}
#else
void Database::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid Maxmind database reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const DbRef & Database::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const DbRef & Database::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
Metadata Database::GetMetadata() const
{
    return Metadata(m_Handle, &(SQMOD_GET_VALID(*this)->mDb).metadata);
}

// ------------------------------------------------------------------------------------------------
Object Database::GetMetadataAsEntryDataList() const
{
    MMDB_entry_data_list_s * entry_data_list = nullptr;
    // Attempt to retrieve the database meta-data as an entry data list
    const int status = MMDB_get_metadata_as_entry_data_list(&(SQMOD_GET_VALID(*this)->mDb), &entry_data_list);
    // Validate the status code
    if (status != MMDB_SUCCESS)
    {
        STHROWF("Unable to get meta-data entry data list [%s]", MMDB_strerror(status));
    }
    // Return the resulted list
    return Object(new EntryDataList(m_Handle, entry_data_list));
}

// ------------------------------------------------------------------------------------------------
LookupResult Database::LookupString(CSStr addr)
{
    // Validate the database handle
    SQMOD_VALIDATE(*this);
    // Validate the specified string
    if (!addr || *addr == '\0')
    {
        STHROWF("Invalid address string");
    }
    // Dummy variables to obtain the status codes
    int gai_error, mmdb_error;
    // Attempt to perform the actual lookup
    MMDB_lookup_result_s result = MMDB_lookup_string(&m_Handle->mDb, addr, &gai_error, &mmdb_error);
    // Validate the result of the getaddrinfo() function call
    if (gai_error != 0)
    {
        STHROWF("Unable to resolve address (%s) because [%s]", addr, gai_strerror(gai_error));
    }
    // Validate the lookup status code
    else if (mmdb_error != MMDB_SUCCESS)
    {
        STHROWF("Unable to lookup address (%s) because [%s]", addr, MMDB_strerror(mmdb_error));
    }
    // Now it's safe to return the lookup result
    return LookupResult(m_Handle, result);
}

// ------------------------------------------------------------------------------------------------
LookupResult Database::LookupSockAddr(SockAddr & addr)
{
    // Validate the database handle
    SQMOD_VALIDATE(*this);
    // Validate the specified socket address
    if (!addr.IsValid())
    {
        STHROWF("Invalid address instance");
    }
    // Dummy variable to obtain the status codes
    int mmdb_error;
    // Attempt to perform the actual lookup
    MMDB_lookup_result_s result = MMDB_lookup_sockaddr(&m_Handle->mDb, addr.GetHandle()->ai_addr, &mmdb_error);
    // Validate the lookup status code
    if (mmdb_error != MMDB_SUCCESS)
    {
        STHROWF("Unable to lookup address (%s) because [%s]", addr.GetAddress(), MMDB_strerror(mmdb_error));
    }
    // Now it's safe to return the lookup result
    return LookupResult(m_Handle, result);
}

// ================================================================================================
void Register_Database(Table & mmns)
{
    mmns.Bind(_SC("Database"),
        Class< Database >(mmns.GetVM(), _SC("SqMMDatabase"))
        // Constructors
        .Ctor()
        .Ctor< CSStr >()
        .Ctor< CSStr, Uint32 >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Database::Typename)
        .Func(_SC("_tostring"), &Database::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Database::IsValid)
        .Prop(_SC("References"), &Database::GetRefCount)
        .Prop(_SC("Metadata"), &Database::GetMetadata)
        .Prop(_SC("MetadataAsEntryDataList"), &Database::GetMetadataAsEntryDataList)
        // Member methods
        .Func(_SC("Release"), &Database::Release)
        .Func(_SC("LookupString"), &Database::LookupString)
        .Func(_SC("LookupSockAddr"), &Database::LookupSockAddr)
        // Member overloads
        .Overload< void (Database::*)(CSStr) >(_SC("Open"), &Database::Open)
        .Overload< void (Database::*)(CSStr, Uint32) >(_SC("Open"), &Database::Open)
    );
}

} // Namespace:: SqMod
