#ifndef _SQMYSQL_HANDLE_RESULTSET_HPP_
#define _SQMYSQL_HANDLE_RESULTSET_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Statement.hpp"
#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <unordered_map>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * The structure that holds the data associated with a certain field.
*/
struct ResBind
{
public:

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_RES       Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_FIELD     FieldType; // Database field type.
    typedef MYSQL_BIND      BindType; // Database bind type.
    typedef MYSQL_TIME      TimeType; // Database time type.
    typedef MYSQL_ROW       RowType; // Database row type.
    typedef my_bool         BoolType; // Database boolean type.

    // --------------------------------------------------------------------------------------------
    typedef std::unordered_map< String, Uint32 > IndexMap;

public:

    // --------------------------------------------------------------------------------------------
    BoolType        mIsNull; // Allows the database to specify if the field is null.
    BoolType        mError; // Allows the database if errors occured on this field.
    Buffer          mData; // Buffer to store non fundamental data for the field.
    BindType *      mBind; // The associated database bind point handle.
    TimeType        mTime; // Structure used to retrieve time data from database.

    // --------------------------------------------------------------------------------------------
    union
    {
        Uint64      mUint64; // Retrieve unsigned integer values from a field.
        Int64       mInt64; // Retrieve signed integer values from a field.
        Int32       mInt32[2]; // Retrieve 32 bit signed integer values from a field.
        Float64     mFloat64; // Retrieve 32 bit floating point values from a field.
        Float32     mFloat32[2]; // Retrieve 64 bit floating point values from the field.
    };

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ResBind()
        : mIsNull(0), mError(0), mData(), mBind(nullptr), mTime(), mUint64(0)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    ResBind(const ResBind & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    ResBind(ResBind && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    ResBind & operator = (const ResBind & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    ResBind & operator = (ResBind && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the used buffer.
    */
    CStr GetBuffer()
    {
        return mData ? mData.Data() : reinterpret_cast< CStr >(&mUint64);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the buffer length.
    */
    Ulong GetLength() const
    {
        return mBind == nullptr ? 0 : mBind->buffer_length;
    }

    /* --------------------------------------------------------------------------------------------
     * Configure the output to match the requirements of a certain field.
    */
    void SetOutput(const FieldType & field, BindType * bind);
};

/* ------------------------------------------------------------------------------------------------
 * The structure that holds the data associated with a certain result-set handle.
*/
struct ResHnd
{
public:

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_RES       Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_FIELD     FieldType; // Database field type.
    typedef MYSQL_BIND      BindType; // Database bind type.
    typedef MYSQL_TIME      TimeType; // Database time type.
    typedef MYSQL_ROW       RowType; // Database row type.
    typedef my_bool         BoolType; // Database boolean type.

    // --------------------------------------------------------------------------------------------
    typedef std::unordered_map< String, Uint32 > IndexMap; // Name to index association of fields.

public:

    // --------------------------------------------------------------------------------------------
    Pointer         mPtr; // The managed result-set handle.

    // --------------------------------------------------------------------------------------------
    Uint32          mFieldCount; // Number of fields in the result-set.
    Ulong *         mLengths; // Data length when the result-set came from a connection.
    FieldType *     mFields; // Fields in the results set.
    ResBind *       mBinds; // Bind wrappers.
    BindType *      mMyBinds; // Bind points.
    RowType         mRow; // Row data.

    // --------------------------------------------------------------------------------------------
    ConnRef         mConnection; // Associated connection.
    StmtRef         mStatement; // Associated statement.
    IndexMap        mIndexes; // Field names and their associated index.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ResHnd();

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ResHnd();

    /* --------------------------------------------------------------------------------------------
     * Grab the current error in the associated statement or connection handle.
    */
    void GrabCurrent();

    /* --------------------------------------------------------------------------------------------
     * Grab the current error in the associated statement or connection handle and throw it.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ThrowCurrent(CCStr act, CCStr file, Int32 line);
#else
    void ThrowCurrent(CCStr act);
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the statement handle and field index and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateField(Uint32 idx, CCStr file, Int32 line) const;
#else
    void ValidateField(Uint32 idx) const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Create the result-set from a Connection.
    */
    void Create(const ConnRef & conn);

    /* --------------------------------------------------------------------------------------------
     * Create the result-set from a Statement.
    */
    void Create(const StmtRef & stmt);

    /* --------------------------------------------------------------------------------------------
     * Returns the current position of the row cursor for the last Next().
    */
    Uint64 RowIndex() const;

    /* --------------------------------------------------------------------------------------------
     * Returns the number of rows in the result set.
    */
    Uint64 RowCount() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the next row from the query.
    */
    bool Next();

    /* --------------------------------------------------------------------------------------------
     * Seeks to an arbitrary row in a query result set.
    */
    bool SetRowIndex(Uint64 index);

};


} // Namespace:: SqMod

#endif // _SQMYSQL_HANDLE_RESULTSET_HPP_
