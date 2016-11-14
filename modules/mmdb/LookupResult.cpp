// ------------------------------------------------------------------------------------------------
#include "LookupResult.hpp"
#include "EntryData.hpp"
#include "EntryDataList.hpp"
#include "Database.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger LookupResult::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMLookupResult");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void LookupResult::Validate(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid Maxmind database reference =>[%s:%d]", file, line);
    }
}
#else
void LookupResult::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid Maxmind database reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const DbRef & LookupResult::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const DbRef & LookupResult::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
LookupResult::LookupResult()
    : m_Handle(), m_Result()
{
    std::memset(&m_Result, 0, sizeof(Type));
}

// ------------------------------------------------------------------------------------------------
void LookupResult::Release()
{
    std::memset(&m_Result, 0, sizeof(Type));
    m_Handle.Reset();
}

// ------------------------------------------------------------------------------------------------
Database LookupResult::GetDatabase() const
{
    return Database(m_Handle);
}

// ------------------------------------------------------------------------------------------------
Object LookupResult::GetEntryDataList()
{
    // See if there's an entry
    if (!m_Result.found_entry)
    {
        STHROWF("Result does not have an entry");
    }

    MMDB_entry_data_list_s * entry_data_list = nullptr;
    // Attempt to retrieve the entire entry data list at once
    const int status = MMDB_get_entry_data_list(&m_Result.entry, &entry_data_list);
    // Validate the status code
    if (status != MMDB_SUCCESS)
    {
        STHROWF("Unable to get entry data list [%s]", MMDB_strerror(status));
    }
    // Return the resulted list
    return Object(new EntryDataList(m_Handle, entry_data_list));
}

// ------------------------------------------------------------------------------------------------
SQInteger LookupResult::GetValue(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // The lookup result instance
    LookupResult * lookup = nullptr;
    // Attempt to extract the lookup result instance
    try
    {
        lookup = Var< LookupResult * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    // Do we have a valid lookup result instance?
    if (!lookup)
    {
        return sq_throwerror(vm, "Invalid lookup result instance");
    }
    // See if there's a handle
    else if (!lookup->m_Handle)
    {
        return sq_throwerror(vm, "Invalid Maxmind database reference");
    }
    // See if there's an entry
    else if (!(lookup->m_Result.found_entry))
    {
        return sq_throwerror(vm, "Result does not have an entry");
    }

    typedef std::vector< StackStrF > ArgList;
    // The list of extracted arguments
    ArgList arglist;
    // Extract each argument as a string
    for (SQInteger i = 2; i <= top; ++i)
    {
        arglist.emplace_back(vm, i, false);
        // Did we fail to extract the argument value?
        if (SQ_FAILED(arglist.back().mRes))
        {
            return arglist.back().mRes; // Propagate the error
        }
    }

    typedef std::vector< CSStr > PtrList;
    // The list of pointers to path segments
    PtrList ptrlist;
    // Grab the pointers to argument values
    for (const auto & a : arglist)
    {
        ptrlist.push_back(a.mPtr);
    }
    // Push null to specify the end of the list
    ptrlist.push_back(nullptr);

    MMDB_entry_data_s entry_data;
    // Attempt to retrieve the specified entry data
    const int status = MMDB_aget_value(&(lookup->m_Result.entry), &entry_data, ptrlist.data());
    // Validate the status code
    if (status != MMDB_SUCCESS)
    {
        return sq_throwerror(vm, ToStrF("Unable to get entry data [%s]", MMDB_strerror(status)));
    }
    // Push the resulted list object onto the stack
    try
    {
        ClassType< EntryData >::PushInstance(vm, new EntryData(lookup->m_Handle, entry_data));
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    // Specify that we returned a value
    return 1;
}

// ================================================================================================
void Register_LookupResult(Table & mmns)
{
    mmns.Bind(_SC("LookupResult"),
        Class< LookupResult >(mmns.GetVM(), _SC("SqMMLookupResult"))
        // Constructors
        .Ctor()
        .Ctor< const LookupResult & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &LookupResult::Typename)
        .Func(_SC("_tostring"), &LookupResult::ToString)
        // Properties
        .Prop(_SC("IsValid"), &LookupResult::IsValid)
        .Prop(_SC("Database"), &LookupResult::GetDatabase)
        .Prop(_SC("References"), &LookupResult::GetRefCount)
        .Prop(_SC("FoundEntry"), &LookupResult::FoundEntry)
        .Prop(_SC("NetMask"), &LookupResult::GetNetMask)
        .Prop(_SC("EntryDataList"), &LookupResult::GetEntryDataList)
        // Member methods
        .Func(_SC("Release"), &LookupResult::Release)
        // Squirrel functions
        .SquirrelFunc(_SC("GetValue"), &LookupResult::GetValue)
    );
}

} // Namespace:: SqMod
