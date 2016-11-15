#ifndef _SQMMDB_SEARCHNODE_HPP_
#define _SQMMDB_SEARCHNODE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Database.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class that can hold and be used to work with search nodes.
*/
class SearchNode
{
public:

    // --------------------------------------------------------------------------------------------
    typedef MMDB_search_node_s      Type; // The managed type.

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
    Reference GetValid(CCStr file, Int32 line);
#else
    Reference GetValid();
#endif // _DEBUG

private:

    // ---------------------------------------------------------------------------------------------
    DbRef   m_Handle; // The database from which this search node comes from.
    Type    m_Node; // The managed search node structure.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    SearchNode();

    /* --------------------------------------------------------------------------------------------
     * Construct and take ownership of a certain search node.
    */
    SearchNode(const DbRef & db, Reference node)
        : m_Handle(db), m_Node(node)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SearchNode(const SearchNode &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SearchNode(SearchNode &&) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    SearchNode & operator = (const SearchNode &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    SearchNode & operator = (SearchNode &&) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return FmtStr("<%llu:%s,%llu:s>", m_Node.left_record, AsTypeStr(m_Node.left_record_type)
                                        , m_Node.right_record, AsTypeStr(m_Node.right_record_type));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid database and node structure.
    */
    bool IsValid() const
    {
        return m_Handle;
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
     * Retrieve the left record value.
    */
    Object GetLeftRecord()
    {
        // Obtain the initial stack size
        const StackGuard sg;
        // Push a long integer instance with the requested value on the stack
        SqMod_PushULongObject(DefaultVM::Get(), ConvTo< Uint64 >::From(SQMOD_GET_VALID(*this).left_record));
        // Obtain the object from the stack and return it
        return Var< Object >(DefaultVM::Get(), -1).value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the right record value.
    */
    Object GetRightRecord()
    {
        // Obtain the initial stack size
        const StackGuard sg;
        // Push a long integer instance with the requested value on the stack
        SqMod_PushULongObject(DefaultVM::Get(), ConvTo< Uint64 >::From(SQMOD_GET_VALID(*this).right_record));
        // Obtain the object from the stack and return it
        return Var< Object >(DefaultVM::Get(), -1).value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the left record value type.
    */
    SQInteger GetLeftRecordType()
    {
        return static_cast< SQInteger >(SQMOD_GET_VALID(*this).left_record_type);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the right record value type.
    */
    SQInteger GetRightRecordType()
    {
        return static_cast< SQInteger >(SQMOD_GET_VALID(*this).right_record_type);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the left record entry data list.
    */
    Object GetLeftRecordEntryDataList();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the right record entry data list.
    */
    Object GetRightRecordEntryDataList();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the left record entry data.
    */
    static SQInteger GetLeftRecordEntryData(HSQUIRRELVM vm)
    {
        return GetRecordEntryData(vm, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the left record entry data.
    */
    static SQInteger GetRightRecordEntryData(HSQUIRRELVM vm)
    {
        return GetRecordEntryData(vm, true);
    }

protected:

    /* --------------------------------------------------------------------------------------------
     * Shared function to retrieve the left/right record entry data.
    */
    static SQInteger GetRecordEntryData(HSQUIRRELVM vm, bool right);
};

} // Namespace:: SqMod

#endif // _SQMMDB_SEARCHNODE_HPP_
