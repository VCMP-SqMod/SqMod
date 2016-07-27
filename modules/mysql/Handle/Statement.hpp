#ifndef _SQMYSQL_HANDLE_STATEMENT_HPP_
#define _SQMYSQL_HANDLE_STATEMENT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Connection.hpp"
#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * The structure that holds the data associated with a certain bind point.
*/
struct StmtBind
{
public:

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_STMT      Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_BIND      BindType; // Database bind type.
    typedef MYSQL_TIME      TimeType; // Database time type.
    typedef my_bool         BoolType; // Database boolean type.

public:

    // --------------------------------------------------------------------------------------------
    BoolType        mIsNull; // Allows the database to specify if the parameter is null.
    BoolType        mError; // Allows the database if errors occured on this parameter.
    Buffer          mData; // Buffer to store non fundamental data for the parameter.
    BindType *      mBind; // The associated database bind point handle.
    TimeType        mTime; // Structure used to store time data from database.

    // --------------------------------------------------------------------------------------------
    union
    {
        Uint64      mUint64; // Store unsigned integer values for the parameter.
        Int64       mInt64; // Store signed integer values for the parameter.
        Int32       mInt32[2]; // Store 32 bit signed integer values for the parameter.
        Float64     mFloat64; // Store 32 bit floating point values for the parameter.
        Float32     mFloat32[2]; // Store 64 bit floating point values for the parameter.
    };

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    StmtBind()
        : mIsNull(0), mError(0), mData(), mBind(nullptr), mTime(), mUint64(0)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    StmtBind(const StmtBind & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    StmtBind(StmtBind && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    StmtBind & operator = (const StmtBind & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    StmtBind & operator = (StmtBind && o) = delete;

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
        return (mBind == nullptr) ? 0 : mBind->buffer_length;
    }

    /* --------------------------------------------------------------------------------------------
     * Configure the input of a certain statement parameter.
    */
    void SetInput(enum_field_types type, BindType * bind, CCStr buffer = nullptr, Ulong length = 0);
};

/* ------------------------------------------------------------------------------------------------
 * The structure that holds the data associated with a certain statement handle.
*/
struct StmtHnd
{
public:

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_STMT      Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_BIND      BindType; // Database bind type.
    typedef MYSQL_TIME      TimeType; // Database time type.
    typedef my_bool         BoolType; // Database boolean type.

public:

    // --------------------------------------------------------------------------------------------
    Pointer         mPtr; // The managed statement handle.

    // --------------------------------------------------------------------------------------------
    Uint32          mErrNo; // Last received error string.
    String          mErrStr; // Last received error message.

    // --------------------------------------------------------------------------------------------
    Ulong           mParams; // Number of parameters in the statement.
    StmtBind *      mBinds; // List of parameter binds.
    BindType *      mMyBinds; // List of parameter binds.

    // --------------------------------------------------------------------------------------------
    ConnRef         mConnection; // Reference to the associated connection.
    String          mQuery; // The query string.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    StmtHnd();

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~StmtHnd();

    /* --------------------------------------------------------------------------------------------
     * Grab the current error in the associated statement handle.
    */
    void GrabCurrent();

    /* --------------------------------------------------------------------------------------------
     * Grab the current error in the associated statement handle and throw it.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ThrowCurrent(CCStr act, CCStr file, Int32 line);
#else
    void ThrowCurrent(CCStr act);
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated statement handle and parameter index and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateParam(Uint32 idx, CCStr file, Int32 line) const;
#else
    void ValidateParam(Uint32 idx) const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Check whether a specific param index is within range.
    */
    bool CheckParamIndex(Uint32 idx) const
    {
        return (idx < mParams);
    }

    /* --------------------------------------------------------------------------------------------
     * Create the actual statement.
    */
    void Create(const ConnRef & conn, CSStr query);
};


} // Namespace:: SqMod

#endif // _SQMYSQL_HANDLE_STATEMENT_HPP_
