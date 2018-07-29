// ------------------------------------------------------------------------------------------------
#include "SearchNode.hpp"
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
SQInteger SearchNode::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMSearchNode");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SearchNode::Validate(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid Maxmind database reference =>[%s:%d]", file, line);
    }
}
#else
void SearchNode::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid Maxmind database reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
SearchNode::Reference SearchNode::GetValid(CCStr file, Int32 line)
{
    Validate(file, line);
    return m_Node;
}
#else
SearchNode::Reference SearchNode::GetValid()
{
    Validate();
    return m_Node;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
SearchNode::SearchNode()
    : m_Handle(), m_Node()
{
    std::memset(&m_Node, 0, sizeof(Type));
}

// ------------------------------------------------------------------------------------------------
void SearchNode::Release()
{
    std::memset(&m_Node, 0, sizeof(Type));
    m_Handle.Reset();
}

// ------------------------------------------------------------------------------------------------
Database SearchNode::GetDatabase() const
{
    return Database(m_Handle);
}

// ------------------------------------------------------------------------------------------------
Object SearchNode::GetLeftRecordEntryDataList()
{
    // Prepare a temporary entry data list pointer
    MMDB_entry_data_list_s * entry_data_list = nullptr;
    // Attempt to retrieve the entire entry data list at once
    const int status = MMDB_get_entry_data_list(&(SQMOD_GET_VALID(*this).left_record_entry), &entry_data_list);
    // Validate the status code
    if (status != MMDB_SUCCESS)
    {
        STHROWF("Unable to get entry data list [%s]", MMDB_strerror(status));
    }
    // Return the resulted list
    return Object(new EntryDataList(m_Handle, entry_data_list));
}

// ------------------------------------------------------------------------------------------------
Object SearchNode::GetRightRecordEntryDataList()
{
    // Prepare a temporary entry data list pointer
    MMDB_entry_data_list_s * entry_data_list = nullptr;
    // Attempt to retrieve the entire entry data list at once
    const int status = MMDB_get_entry_data_list(&(SQMOD_GET_VALID(*this).right_record_entry), &entry_data_list);
    // Validate the status code
    if (status != MMDB_SUCCESS)
    {
        STHROWF("Unable to get entry data list [%s]", MMDB_strerror(status));
    }
    // Return the resulted list
    return Object(new EntryDataList(m_Handle, entry_data_list));
}

// ------------------------------------------------------------------------------------------------
SQInteger SearchNode::GetRecordEntryData(HSQUIRRELVM vm, bool right)
{
    const Int32 top = sq_gettop(vm);
    // The search node result instance
    SearchNode * node = nullptr;
    // Attempt to extract the search node result instance
    try
    {
        node = Var< SearchNode * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    // Do we have a valid search node result instance?
    if (!node)
    {
        return sq_throwerror(vm, "Invalid search node result instance");
    }
    // See if there's a handle
    else if (!node->m_Handle)
    {
        return sq_throwerror(vm, "Invalid Maxmind database reference");
    }

    typedef std::vector< StackStrF > ArgList;
    // The list of extracted arguments
    ArgList arglist;
    // Extract each argument as a string
    for (SQInteger i = 2; i <= top; ++i)
    {
        arglist.emplace_back(vm, i);
        // Did we fail to extract the argument value?
        if (SQ_FAILED(arglist.back().Proc(false)))
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

    // Grab the requested entry
    MMDB_entry_s * entry = &(right ? node->m_Node.right_record_entry : node->m_Node.left_record_entry);

    MMDB_entry_data_s entry_data;
    // Attempt to retrieve the specified entry data
    const int status = MMDB_aget_value(entry, &entry_data, ptrlist.data());
    // Validate the status code
    if (status != MMDB_SUCCESS)
    {
        return sq_throwerror(vm, ToStrF("Unable to get entry data [%s]", MMDB_strerror(status)));
    }
    // Push the resulted list object onto the stack
    try
    {
        ClassType< EntryData >::PushInstance(vm, new EntryData(node->m_Handle, entry_data));
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }

    // Specify that we returned a value
    return 1;
}

// ================================================================================================
void Register_SearchNode(Table & mmns)
{
    mmns.Bind(_SC("SearchNode"),
        Class< SearchNode >(mmns.GetVM(), _SC("SqMMSearchNode"))
        // Constructors
        .Ctor()
        .Ctor< const SearchNode & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SearchNode::Typename)
        .Func(_SC("_tostring"), &SearchNode::ToString)
        // Properties
        .Prop(_SC("IsValid"), &SearchNode::IsValid)
        .Prop(_SC("Database"), &SearchNode::GetDatabase)
        .Prop(_SC("References"), &SearchNode::GetRefCount)
        .Prop(_SC("LeftRecord"), &SearchNode::GetLeftRecord)
        .Prop(_SC("RightRecord"), &SearchNode::GetRightRecord)
        .Prop(_SC("LeftRecordType"), &SearchNode::GetLeftRecordType)
        .Prop(_SC("RightRecordType"), &SearchNode::GetRightRecordType)
        .Prop(_SC("LeftRecordEntryDataList"), &SearchNode::GetLeftRecordEntryDataList)
        .Prop(_SC("RightRecordEntryDataList"), &SearchNode::GetRightRecordEntryDataList)
        // Member methods
        .Func(_SC("Release"), &SearchNode::Release)
        // Squirrel methods
        .SquirrelFunc(_SC("GetLeftRecordValue"), &SearchNode::GetLeftRecordEntryData)
        .SquirrelFunc(_SC("GetRightRecordValue"), &SearchNode::GetRightRecordEntryData)
    );
}

} // Namespace:: SqMod
