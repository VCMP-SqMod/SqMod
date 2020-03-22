#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include "Library/Numeric/LongInt.hpp"
#include "Library/Utils/Buffer.hpp"
#include "Library/Chrono.hpp"
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Datetime.hpp"
#include "Library/Chrono/Time.hpp"
#include "Library/Chrono/Timestamp.hpp"

// ------------------------------------------------------------------------------------------------
#include <unordered_map>

// ------------------------------------------------------------------------------------------------
#include <mysql.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Handle validation.
*/
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    #define SQMOD_THROW_CURRENT(x, a)       (x).ThrowCurrent(a, __FILE__, __LINE__)
    #define SQMOD_VALIDATE(x)               (x).Validate(__FILE__, __LINE__)
    #define SQMOD_VALIDATE_CREATED(x)       (x).ValidateCreated(__FILE__, __LINE__)
    #define SQMOD_VALIDATE_PARAM(x, i)      (x).ValidateParam((i), __FILE__, __LINE__)
    #define SQMOD_VALIDATE_FIELD(x, i)      (x).ValidateField((i), __FILE__, __LINE__)
    #define SQMOD_VALIDATE_STEPPED(x)       (x).ValidateStepped(__FILE__, __LINE__)
    #define SQMOD_GET_VALID(x)              (x).GetValid(__FILE__, __LINE__)
    #define SQMOD_GET_CREATED(x)            (x).GetCreated(__FILE__, __LINE__)
    #define SQMOD_GET_STEPPED(x)            (x).GetStepped(__FILE__, __LINE__)
#else
    #define SQMOD_THROW_CURRENT(x, a)       (x).ThrowCurrent(a)
    #define SQMOD_VALIDATE(x)               (x).Validate()
    #define SQMOD_VALIDATE_CREATED(x)       (x).ValidateCreated()
    #define SQMOD_VALIDATE_PARAM(x, i)      (x).ValidateParam((i))
    #define SQMOD_VALIDATE_FIELD(x, i)      (x).ValidateField((i))
    #define SQMOD_VALIDATE_STEPPED(x)       (x).ValidateStepped()
    #define SQMOD_GET_VALID(x)              (x).GetValid()
    #define SQMOD_GET_CREATED(x)            (x).GetCreated()
    #define SQMOD_GET_STEPPED(x)            (x).GetStepped()
#endif // _DEBUG

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
struct ConnHnd;
struct StmtHnd;
struct ResHnd;

// ------------------------------------------------------------------------------------------------
class Account;
class Column;
class Connection;
class ResultSet;
class Statement;
class Transaction;

/* ------------------------------------------------------------------------------------------------
 * Common typedefs.
*/
typedef SharedPtr< ConnHnd > ConnRef;
typedef SharedPtr< StmtHnd > StmtRef;
typedef SharedPtr< ResHnd > ResRef;

/* ------------------------------------------------------------------------------------------------
 * Replicate the values of a script Date type to a database time type.
*/
void SqDateToMySQLTime(const Date & value, MYSQL_TIME & t);

/* ------------------------------------------------------------------------------------------------
 * Replicate the values of a script Date type to a database time type.
*/
void SqTimeToMySQLTime(const Time & value, MYSQL_TIME & t);

/* ------------------------------------------------------------------------------------------------
 * Replicate the values of a script Date type to a database time type.
*/
void SqDatetimeToMySQLTime(const Datetime & value, MYSQL_TIME & t);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the lowercase name of a MySQL data-type.
*/
CSStr SqMySQLTypename(enum_field_types type);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the capitalized name of a MySQL data-type.
*/
CSStr SqMySQLTypenameC(enum_field_types type);

/* ------------------------------------------------------------------------------------------------
 * Utility used to convert from database types to known types.
*/
template < typename T > struct DbConvTo;

/* ------------------------------------------------------------------------------------------------
 * Specialization for signed 8 bit integer.
*/
template<> struct DbConvTo< Int8 >
{
    static Int8 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Int8"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for unsigned 8 bit integer.
*/
template<> struct DbConvTo< Uint8 >
{
    static Uint8 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Uint8"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for signed 16 bit integer.
*/
template<> struct DbConvTo< Int16 >
{
    static Int16 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Int16"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for unsigned 16 bit integer.
*/
template<> struct DbConvTo< Uint16 >
{
    static Uint16 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Uint16"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for signed 32 bit integer.
*/
template<> struct DbConvTo< Int32 >
{
    static Int32 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Int32"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for unsigned 32 bit integer.
*/
template<> struct DbConvTo< Uint32 >
{
    static Uint32 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Uint32"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for signed 64 bit integer.
*/
template<> struct DbConvTo< Int64 >
{
    static Int64 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Int64"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for unsigned 64 bit integer.
*/
template<> struct DbConvTo< Uint64 >
{
    static Uint64 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Uint64"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for 32 floating point.
*/
template<> struct DbConvTo< Float32 >
{
    static Float32 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Float32"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for 64 floating point.
*/
template<> struct DbConvTo< Float64 >
{
    static Float64 From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Float64"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for boolean value.
*/
template<> struct DbConvTo< bool >
{
    static bool From(CSStr value, Ulong length, enum_field_types type, CSStr tn = _SC("Boolean"));
};

/* ------------------------------------------------------------------------------------------------
 * The structure that holds the data associated with a certain connection.
*/
struct ConnHnd
{
public:

    // --------------------------------------------------------------------------------------------
    typedef MYSQL           Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_RES       ResType; // Database result type.

public:

    // --------------------------------------------------------------------------------------------
    Pointer     mPtr; // The connection handle resource.

    // --------------------------------------------------------------------------------------------
    Uint32      mErrNo; // Last received error string.
    String      mErrStr; // Last received error message.

    // --------------------------------------------------------------------------------------------
    Uint16      mPort; // Server port.
    String      mHost; // Host address.
    String      mUser; // User name user.
    String      mPass; // User password.
    String      mName; // Database name.
    String      mSocket; // Unix socket.
    Ulong       mFlags; // Client flags.

    // --------------------------------------------------------------------------------------------
    String      mSSL_Key; // SSL key.
    String      mSSL_Cert; // SSL certificate.
    String      mSSL_CA; // SSL certificate authority.
    String      mSSL_CA_Path; // SSL certificate authority path.
    String      mSSL_Cipher; // SSL Cipher.

    // --------------------------------------------------------------------------------------------
    String      mCharset; // Default connection character set.

    // --------------------------------------------------------------------------------------------
    bool        mAutoCommit; // Whether autocommit is enabled on this connection.
    bool        mInTransaction; // Whether the connection is in the middle of a transaction.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ConnHnd();

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ConnHnd();

    /* --------------------------------------------------------------------------------------------
     * Grab the current error in the connection handle.
    */
    void GrabCurrent();

    /* --------------------------------------------------------------------------------------------
     * Grab the current error in the connection handle and throw it.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ThrowCurrent(CCStr act, CCStr file, Int32 line);
#else
    void ThrowCurrent(CCStr act);
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Create the connection handle.
    */
    void Create(const Account & acc);

    /* --------------------------------------------------------------------------------------------
     * Disconnect the managed connection handle.
    */
    void Disconnect();

    /* --------------------------------------------------------------------------------------------
     * Execute a query on the server.
    */
    Uint64 Execute(CSStr query, Ulong size = 0UL);
};

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
     * Check whether a specific field index is within range.
    */
    bool CheckFieldIndex(Uint32 idx) const
    {
        return (idx < mFieldCount);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the field index associated with the specified name.
    */
    Uint32 GetFieldIndex(CSStr name);

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
