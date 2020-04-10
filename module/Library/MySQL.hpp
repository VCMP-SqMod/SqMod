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

/* ------------------------------------------------------------------------------------------------
 * Helper class containing shared connection information to avoid repetition.
*/
class Account
{
public:

    // --------------------------------------------------------------------------------------------
    typedef std::unordered_map< String, String > Options;

private:

    // --------------------------------------------------------------------------------------------
    Uint16      m_Port; // Server port.
    String      m_Host; // Host address.
    String      m_User; // User name.
    String      m_Pass; // User password.
    String      m_Name; // Database name.
    String      m_Socket; // Unix socket.
    Ulong       m_Flags; // Client connection flags.

    // --------------------------------------------------------------------------------------------
    String      m_SSL_Key; // SSL key.
    String      m_SSL_Cert; // SSL certificate.
    String      m_SSL_CA; // SSL certificate authority.
    String      m_SSL_CA_Path; // SSL certificate authority path.
    String      m_SSL_Cipher; // SSL Cipher.

    // --------------------------------------------------------------------------------------------
    Options     m_Options; // Option container.

    // --------------------------------------------------------------------------------------------
    bool        m_AutoCommit; // Toggle autocommit.

    // --------------------------------------------------------------------------------------------
    static const String s_String; // Dummy used to return a reference to a null string.

public:

    /* --------------------------------------------------------------------------------------------
     * Base Constructor.
    */
    Account(CSStr host, CSStr user)
        : Account(host, user, _SC(""), _SC(""), 3306, _SC(""))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base Constructor.
    */
    Account(CSStr host, CSStr user, CSStr pass)
        : Account(host, user, pass, _SC(""), 3306, _SC(""))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base Constructor.
    */
    Account(CSStr host, CSStr user, CSStr pass, CSStr name)
        : Account(host, user, pass, name, 3306, _SC(""))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base Constructor.
    */
    Account(CSStr host, CSStr user, CSStr pass, CSStr name, SQInteger port)
        : Account(host, user, pass, name, port, _SC(""))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base Constructor.
    */
    Account(CSStr host, CSStr user, CSStr pass, CSStr name, SQInteger port, CSStr socket);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Account(const Account & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Account(Account && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Account() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Account & operator = (const Account & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Account & operator = (Account && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Account & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the host address specified during creation.
    */
    const String & GetHost() const
    {
        return m_Host;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the host address specified during creation.
    */
    void SetHost(CSStr addr);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the user name specified during creation.
    */
    const String & GetUser() const
    {
        return m_User;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the user name specified during creation.
    */
    void SetUser(CSStr user)
    {
        m_User.assign(user != nullptr ? user : _SC(""));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the password specified during creation.
    */
    const String & GetPass() const
    {
        return m_Pass;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the password specified during creation.
    */
    void SetPass(CSStr pass)
    {
        m_Pass.assign(pass != nullptr ? pass : _SC(""));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the database name specified during creation.
    */
    const String & GetName() const
    {
        return m_Name;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the database name specified during creation.
    */
    void SetName(CSStr name)
    {
        m_Name.assign(name != nullptr ? name : _SC(""));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the server port specified during creation.
    */
    Uint16 GetPortNum() const
    {
        return m_Port;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the server port specified during creation.
    */
    void SetPortNum(SQInteger port);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the server socket.
    */
    const String & GetSocket() const
    {
        return m_Socket;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the server socket.
    */
    void SetSocket(CSStr socket)
    {
        m_Socket.assign(socket != nullptr ? socket : _SC(""));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current client connection flags.
    */
    SQInteger GetFlags() const
    {
        return m_Flags;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the current client connection flags.
    */
    void SetFlags(SQInteger flags)
    {
        m_Flags = flags;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the current client connection flags.
    */
    void EnableFlags(SQInteger flags)
    {
        m_Flags |= flags;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the current client connection flags.
    */
    void DisableFlags(SQInteger flags)
    {
        m_Flags |= flags;
        m_Flags ^= flags;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the SSL key.
    */
    const String & GetSSL_Key() const
    {
        return m_SSL_Key;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the SSL key.
    */
    void SetSSL_Key(CSStr key)
    {
        m_SSL_Key.assign(key != nullptr ? key : _SC(""));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the SSL certificate.
    */
    const String & GetSSL_Cert() const
    {
        return m_SSL_Cert;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the SSL certificate.
    */
    void SetSSL_Cert(CSStr cert)
    {
        m_SSL_Cert.assign(cert != nullptr ? cert : _SC(""));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the SSL certificate authority.
    */
    const String & GetSSL_CA() const
    {
        return m_SSL_CA;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the SSL certificate authority.
    */
    void SetSSL_CA(CSStr ca)
    {
        m_SSL_CA.assign(ca != nullptr ? ca : _SC(""));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the SSL certificate authority path.
    */
    const String & GetSSL_CA_Path() const
    {
        return m_SSL_CA_Path;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the SSL certificate authority path.
    */
    void SetSSL_CA_Path(CSStr capath)
    {
        m_SSL_CA_Path.assign(capath != nullptr ? capath : _SC(""));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the SSL cipher.
    */
    const String & GetSSL_Cipher() const
    {
        return m_SSL_Cipher;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the SSL cipher.
    */
    void SetSSL_Cipher(CSStr cipher)
    {
        m_SSL_Cipher.assign(cipher != nullptr ? cipher : _SC(""));
    }

    /* --------------------------------------------------------------------------------------------
     * Set the SSL information.
    */
    void SetSSL(CSStr key, CSStr cert, CSStr ca, CSStr ca_path, CSStr cipher);

    /* --------------------------------------------------------------------------------------------
     * See whether autocommit is enabled or not.
    */
    bool GetAutoCommit() const
    {
        return m_AutoCommit;
    }

    /* --------------------------------------------------------------------------------------------
     * Set whether autocommit should be enabled or not.
    */
    void SetAutoCommit(bool toggle)
    {
        m_AutoCommit = toggle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the entire options container as a table.
    */
    const Options & GetOptions() const
    {
        return m_Options;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the entire options container as a table.
    */
    Table GetOptionsTable() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a value from the options container.
    */
    const String & GetOption(CSStr name) const;

    /* --------------------------------------------------------------------------------------------
     * Set a value in the options container.
    */
    void SetOption(CSStr name, CSStr value);

    /* --------------------------------------------------------------------------------------------
     * Remove a value from the options container.
    */
    void RemoveOption(CSStr name);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of values in the options container.
    */
    Uint32 OptionsCount() const
    {
        return static_cast< Uint32 >(m_Options.size());
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the options container is empty or not.
    */
    bool OptionsEmpty() const
    {
        return m_Options.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Remove all the options from the options container.
    */
    void OptionsClear()
    {
        m_Options.clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Create a connection with the current account information.
    */
    Connection Connect() const;
};

/* ------------------------------------------------------------------------------------------------
 * Allows management and interaction with a connection handle.
*/
class Connection
{
private:

    // --------------------------------------------------------------------------------------------
    ConnRef     m_Handle; // Reference to the actual database connection.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateCreated(CCStr file, Int32 line) const;
#else
    void ValidateCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ConnRef & GetValid(CCStr file, Int32 line) const;
#else
    const ConnRef & GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ConnRef & GetCreated(CCStr file, Int32 line) const;
#else
    const ConnRef & GetCreated() const;
#endif // _DEBUG

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Connection()
        : m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Connection(const Account & acc)
        : m_Handle(new ConnHnd())
    {
        m_Handle->Create(acc);
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Connection(const ConnRef & conn)
        : m_Handle(conn)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Connection(const Connection & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Connection(Connection && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Connection & operator = (const Connection & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Connection & operator = (Connection && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Connection & o) const
    {
        if (m_Handle.Get() == o.m_Handle.Get())
        {
            return 0;
        }
        else if (m_Handle.Get() > o.m_Handle.Get())
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return m_Handle ? mysql_get_host_info(m_Handle->mPtr) : _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated connection handle.
    */
    const ConnRef & GetHandle() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the managed handle is valid.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the managed connection handle was connected.
    */
    bool IsConnected() const
    {
        return m_Handle && (m_Handle->mPtr != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this connection handle.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current error number.
    */
    SQInteger GetErrNo() const
    {
        return static_cast< SQInteger >(mysql_errno(SQMOD_GET_CREATED(*this)->mPtr));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current error message.
    */
    CSStr GetErrStr() const
    {
        return mysql_error(SQMOD_GET_CREATED(*this)->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last received error number.
    */
    SQInteger GetLastErrNo() const
    {
        return static_cast< SQInteger >(SQMOD_GET_VALID(*this)->mErrNo);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last received error message.
    */
    const String & GetLastErrStr() const
    {
        return SQMOD_GET_VALID(*this)->mErrStr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection port number.
    */
    SQInteger GetPortNum() const
    {
        return static_cast< SQInteger >(SQMOD_GET_VALID(*this)->mPort);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection host address.
    */
    const String & GetHost() const
    {
        return SQMOD_GET_VALID(*this)->mHost;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection user name.
    */
    const String & GetUser() const
    {
        return SQMOD_GET_VALID(*this)->mUser;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection password.
    */
    const String & GetPass() const
    {
        return SQMOD_GET_VALID(*this)->mPass;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the selected database name.
    */
    const String & GetName() const
    {
        return SQMOD_GET_VALID(*this)->mName;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the selected database name.
    */
    void SetName(CSStr name)
    {
        // Validate the specified name
        if (!name)
        {
            STHROWF("Invalid MySQL database name");
        }
        // Attempt to select the database with the given name
        else if (mysql_select_db(SQMOD_GET_CREATED(*this)->mPtr, name) != 0)
        {
            SQMOD_THROW_CURRENT(*m_Handle, "Cannot select MySQL database");
        }
        // Remember the database name
        m_Handle->mName.assign(name);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection socket.
    */
    const String & GetSocket() const
    {
        return SQMOD_GET_VALID(*this)->mSocket;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection flags.
    */
    SQInteger GetFlags() const
    {
        return static_cast< SQInteger >(SQMOD_GET_VALID(*this)->mFlags);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL key.
    */
    const String & GetSSL_Key() const
    {
        return SQMOD_GET_VALID(*this)->mSSL_Key;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL certificate.
    */
    const String & GetSSL_Cert() const
    {
        return SQMOD_GET_VALID(*this)->mSSL_Cert;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL certificate authority.
    */
    const String & GetSSL_CA() const
    {
        return SQMOD_GET_VALID(*this)->mSSL_CA;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL certificate authority path.
    */
    const String & GetSSL_CA_Path() const
    {
        return SQMOD_GET_VALID(*this)->mSSL_CA_Path;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL cipher.
    */
    const String & GetSSL_Cipher() const
    {
        return SQMOD_GET_VALID(*this)->mSSL_Cipher;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the default character set for the managed connection.
    */
    const String & GetCharset() const
    {
        return SQMOD_GET_VALID(*this)->mCharset;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the default character set for the managed connection.
    */
    void SetCharset(CSStr charset)
    {
        // Validate the specified string
        if (!charset)
        {
            STHROWF("Invalid charset string");
        }
        // Attempt to Set the default character set for the managed connection
        else if (mysql_set_character_set(SQMOD_GET_CREATED(*this)->mPtr, charset) != 0)
        {
            SQMOD_THROW_CURRENT(*m_Handle, "Cannot apply character set");
        }
        // Remember the character set
        m_Handle->mCharset.assign(charset);
    }

    /* --------------------------------------------------------------------------------------------
     * See whether auto-commit is enabled or not.
    */
    bool GetAutoCommit() const
    {
        return SQMOD_GET_VALID((*this))->mAutoCommit;
    }

    /* --------------------------------------------------------------------------------------------
     * Set whether auto-commit should be enabled or not.
    */
    void SetAutoCommit(bool toggle)
    {
        // Attempt to toggle auto-commit if necessary
        if (SQMOD_GET_CREATED(*this)->mAutoCommit != toggle &&
            mysql_autocommit(m_Handle->mPtr, toggle) != 0)
        {
            SQMOD_THROW_CURRENT(*m_Handle, "Cannot toggle auto-commit");
        }
        else
        {
            m_Handle->mAutoCommit = toggle;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the connection is in the middle of a transaction.
    */
    bool GetInTransaction() const
    {
        return SQMOD_GET_VALID(*this)->mInTransaction;
    }

    /* --------------------------------------------------------------------------------------------
     * Disconnect from the currently connected database.
    */
    void Disconnect()
    {
        SQMOD_GET_CREATED(*this)->Disconnect();
    }

    /* --------------------------------------------------------------------------------------------
     * Execute a query on the server.
    */
    Object Execute(CSStr query)
    {
        return Object(SqTypeIdentity< ULongInt >{}, SqVM(), SQMOD_GET_CREATED(*this)->Execute(query));
    }

    /* --------------------------------------------------------------------------------------------
     * Execute a query on the server.
    */
    Object Insert(CSStr query);

    /* --------------------------------------------------------------------------------------------
     * Execute a query on the server.
    */
    ResultSet Query(CSStr query);

    /* --------------------------------------------------------------------------------------------
     * Create a new statement on the managed connection.
    */
    Statement GetStatement(CSStr query);

    /* --------------------------------------------------------------------------------------------
     * Create a new transaction on the managed connection.
    */
    Transaction GetTransaction();

    /* --------------------------------------------------------------------------------------------
     * Escape unwanted characters from a given string.
    */
    LightObj EscapeString(StackStrF & str);

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute the specified query.
    */
    static SQInteger ExecuteF(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute the specified query.
    */
    static SQInteger InsertF(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute the specified query.
    */
    static SQInteger QueryF(HSQUIRRELVM vm);
};

/* ------------------------------------------------------------------------------------------------
 * Used to manage and interact with fields from result-sets.
*/
class Field
{
    // --------------------------------------------------------------------------------------------
    friend class ResultSet;

private:

    // --------------------------------------------------------------------------------------------
    Uint32  m_Index; // The actual index of the referenced field.
    ResRef  m_Handle; // Reference to the actual database result-set.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle and field index, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle and field index, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateCreated(CCStr file, Int32 line) const;
#else
    void ValidateCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle, field index and row, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateStepped(CCStr file, Int32 line) const;
#else
    void ValidateStepped() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle and field index, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ResRef & GetValid(CCStr file, Int32 line) const;
#else
    const ResRef & GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle and field index, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ResRef & GetCreated(CCStr file, Int32 line) const;
#else
    const ResRef & GetCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle field index and row, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ResRef & GetStepped(CCStr file, Int32 line) const;
#else
    const ResRef & GetStepped() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle and field index, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateField(Uint32 idx, CCStr file, Int32 line) const;
#else
    void ValidateField(Uint32 idx) const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(Int32 idx)
    {
        SQMOD_VALIDATE_FIELD(*this, idx);
        // Assign the new index
        m_Index = idx;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(CSStr name)
    {
        SetIndex(SQMOD_GET_CREATED(*this)->GetFieldIndex(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(const Object & field);

public:

    // --------------------------------------------------------------------------------------------
    static const Uint32 INVALID_INDEX; // Value that represents an invalid index.

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    Field()
        : m_Index(INVALID_INDEX), m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * No field constructor.
    */
    Field(const ResRef & rset)
        : m_Index(INVALID_INDEX), m_Handle(rset)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Index constructor.
    */
    Field(const ResRef & rset, Uint32 idx)
        : m_Index(idx), m_Handle(rset)
    {
        SQMOD_VALIDATE_FIELD(*this, m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Name constructor.
    */
    Field(const ResRef & rset, CSStr name)
        : m_Index(rset ? rset->GetFieldIndex(name) : -1), m_Handle(rset)
    {
        SQMOD_VALIDATE_FIELD(*this, m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Dynamic constructor.
    */
    Field(const ResRef & rset, const Object & field)
        : m_Index(INVALID_INDEX), m_Handle(rset)
    {
        if (!m_Handle)
        {
            STHROWF("Invalid MySQL result-set reference");
        }
        // Extract the index
        SetIndex(field);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Field(const Field & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Field(Field && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Field & operator = (const Field & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Field & operator = (Field && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two result-set field indexes.
    */
    bool operator == (const Field & o) const
    {
        return (m_Index == o.m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two result-set field indexes.
    */
    bool operator != (const Field & o) const
    {
        return (m_Index != o.m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const
    {
        return (m_Handle && m_Handle->CheckFieldIndex(m_Index));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Field & o) const
    {
        if (m_Index == o.m_Index)
        {
            return 0;
        }
        else if (m_Index > o.m_Index)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        CSStr val = nullptr;
        // Can we attempt to return the parameter name?
        if (m_Handle && m_Handle->CheckFieldIndex(m_Index))
        {
            val = m_Handle->mFields[m_Index].name;
        }
        else
        {
            val = ToStrF(_SC("%d"), m_Index);
        }
        // Return the value if valid
        return val ? val : _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether the field is valid.
    */
    bool IsValid() const
    {
        return m_Handle; // An invalid result-set means an invalid field
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to the associated result-set handle.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the referenced field index.
    */
    SQInteger GetIndex() const
    {
        return ConvTo< SQInteger >::From(m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the referenced database result-set.
    */
    Object GetResultSet() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the referenced database connection.
    */
    Object GetConnection() const;

    /* --------------------------------------------------------------------------------------------
     * Release the reference to the referenced database result-set and field index.
    */
    void Release()
    {
        m_Handle.Reset();
        m_Index = INVALID_INDEX;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a boolean value.
    */
    bool GetBoolean() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a character.
    */
    SQChar GetChar() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a native script integer.
    */
    SQInteger GetInteger() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a native script floating point.
    */
    SQFloat GetFloat() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a signed 8 bit integer value.
    */
    SQInteger GetInt8() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as an unsigned 8 bit integer value.
    */
    SQInteger GetUint8() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a signed 16 bit integer value.
    */
    SQInteger GetInt16() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as an unsigned 16 bit integer value.
    */
    SQInteger GetUint16() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a signed 32 bit integer value.
    */
    SQInteger GetInt32() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as an unsigned 32 bit integer value.
    */
    SQInteger GetUint32() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a signed 64 bit integer value.
    */
    Object GetInt64() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as an unsigned 64 bit integer value.
    */
    Object GetUint64() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a 32 bit floating point value.
    */
    SQFloat GetFloat32() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a 64 bit floating point value.
    */
    SQFloat GetFloat64() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a string value.
    */
    Object GetString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a memory buffer.
    */
    Object GetBuffer() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a memory blob.
    */
    Object GetBlob() const;
};

/* ------------------------------------------------------------------------------------------------
 * Allows management and interaction with a result set handle.
*/
class ResultSet
{
private:

    // --------------------------------------------------------------------------------------------
    ResRef  m_Handle; // Reference to the actual database result-set.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateCreated(CCStr file, Int32 line) const;
#else
    void ValidateCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and row, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateStepped(CCStr file, Int32 line) const;
#else
    void ValidateStepped() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ResRef & GetValid(CCStr file, Int32 line) const;
#else
    const ResRef & GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ResRef & GetCreated(CCStr file, Int32 line) const;
#else
    const ResRef & GetCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and row, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ResRef & GetStepped(CCStr file, Int32 line) const;
#else
    const ResRef & GetStepped() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference and field index, and throw an error if they're invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateField(Int32 idx, CCStr file, Int32 line) const;
#else
    void ValidateField(Int32 idx) const;
#endif // _DEBUG

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ResultSet()
        : m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Connection constructor.
    */
    explicit ResultSet(const ConnRef & conn)
        : m_Handle(new ResHnd())
    {
        m_Handle->Create(conn);
    }

    /* --------------------------------------------------------------------------------------------
     * Statement constructor.
    */
    explicit ResultSet(const StmtRef & stmt)
        : m_Handle(new ResHnd())
    {
        m_Handle->Create(stmt);
    }

    /* --------------------------------------------------------------------------------------------
     * Handle constructor.
    */
    explicit ResultSet(ResRef hnd)
        : m_Handle(std::move(hnd))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    ResultSet(const ResultSet & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    ResultSet(ResultSet && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ResultSet() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    ResultSet & operator = (const ResultSet & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    ResultSet & operator = (ResultSet && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const ResultSet & o) const
    {
        if (m_Handle.Get() == o.m_Handle.Get())
        {
            return 0;
        }
        else if (m_Handle.Get() > o.m_Handle.Get())
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        // Do we have a valid handle?
        if (m_Handle)
        {
            ToStrF("%u", m_Handle->mFieldCount);
        }
        // Default to a negative value
        return _SC("-1");
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether the managed handle is valid.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Returns an array with all the field names available in the managed result set.
    */
    Array GetFieldNames() const;

    /* --------------------------------------------------------------------------------------------
     * Returns an array with wrapper instances for all the field available in the managed result set.
    */
    Array GetFieldsArray() const;

    /* --------------------------------------------------------------------------------------------
     * Returns an array with wrapper instances for the specified fields in the managed result set.
    */
    Array FetchFieldsArray(Array & fields) const;

    /* --------------------------------------------------------------------------------------------
     * Returns a table with wrapper instances for all the field available in the managed result set.
    */
    Table GetFieldsTable() const;

    /* --------------------------------------------------------------------------------------------
     * Returns a table with wrapper instances for all the specified fields in the managed result set.
    */
    Table FetchFieldsTable(Array & fields) const;

    /* --------------------------------------------------------------------------------------------
     * Returns the current position of the row cursor for the last Next().
    */
    Object RowIndex() const
    {
        return Object(SqTypeIdentity< ULongInt >{}, SqVM(), SQMOD_GET_CREATED(*this)->RowIndex());
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the number of rows in the result set.
    */
    Object RowCount() const
    {
        return Object(SqTypeIdentity< ULongInt >{}, SqVM(), SQMOD_GET_CREATED(*this)->RowCount());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the next row from the query.
    */
    bool Next() const
    {
        return SQMOD_GET_CREATED(*this)->Next();
    }

    /* --------------------------------------------------------------------------------------------
     * Seeks to an arbitrary row in a query result set.
    */
    bool SetRowIndex(SQInteger index) const
    {
        return SQMOD_GET_CREATED(*this)->SetRowIndex(ConvTo< Uint64 >::From(index));
    }

    /* --------------------------------------------------------------------------------------------
     * Seeks to an arbitrary row in a query result set.
    */
    bool SetLongRowIndex(const ULongInt & index) const
    {
        return SQMOD_GET_CREATED(*this)->SetRowIndex(index.GetNum());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the field with the specified name or index.
    */
    Field GetField(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a boolean value.
    */
    bool GetBoolean(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetBoolean();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a character.
    */
    SQChar GetChar(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetChar();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a native script integer.
    */
    SQInteger GetInteger(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetInteger();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a native script floating point.
    */
    SQFloat GetFloat(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetFloat();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a signed 8 bit integer value.
    */
    SQInteger GetInt8(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetInt8();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as an unsigned 8 bit integer value.
    */
    SQInteger GetUint8(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetUint8();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a signed 16 bit integer value.
    */
    SQInteger GetInt16(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetInt16();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as an unsigned 16 bit integer value.
    */
    SQInteger GetUint16(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetUint16();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a signed 32 bit integer value.
    */
    SQInteger GetInt32(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetInt32();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as an unsigned 32 bit integer value.
    */
    SQInteger GetUint32(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetUint32();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a signed 64 bit integer value.
    */
    Object GetInt64(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetInt64();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as an unsigned 64 bit integer value.
    */
    Object GetUint64(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetUint64();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a 32 bit floating point value.
    */
    SQFloat GetFloat32(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetFloat32();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a 64 bit floating point value.
    */
    SQFloat GetFloat64(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetFloat64();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a string value.
    */
    Object GetString(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetString();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a memory buffer.
    */
    Object GetBuffer(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetBuffer();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a memory blob.
    */
    Object GetBlob(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetBlob();
    }
};

/* ------------------------------------------------------------------------------------------------
 * Allows management and interaction with a statement handle.
*/
class Statement
{
private:

    // --------------------------------------------------------------------------------------------
    StmtRef m_Handle; // Reference to the actual database statement.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateCreated(CCStr file, Int32 line) const;
#else
    void ValidateCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const StmtRef & GetValid(CCStr file, Int32 line) const;
#else
    const StmtRef & GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const StmtRef & GetCreated(CCStr file, Int32 line) const;
#else
    const StmtRef & GetCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference and parameter index, and throw an error if they're invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateParam(Int32 idx, CCStr file, Int32 line) const;
#else
    void ValidateParam(Int32 idx) const;
#endif // _DEBUG

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Statement()
        : m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Construct a statement under the specified connection using the specified string.
    */
    Statement(const ConnRef & connection, CSStr query)
        : m_Handle(new StmtHnd())
    {
        m_Handle->Create(connection, query);
    }

    /* --------------------------------------------------------------------------------------------
     * Construct a statement under the specified connection using the specified string.
    */
    Statement(const Connection & connection, CSStr query);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Statement(const Statement & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Statement(Statement && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Statement & operator = (const Statement & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Statement & operator = (Statement && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Statement & o) const
    {
        if (m_Handle.Get() == o.m_Handle.Get())
        {
            return 0;
        }
        else if (m_Handle.Get() > o.m_Handle.Get())
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    const String & ToString() const
    {
        // Do we have a valid handle?
        if (m_Handle)
        {
            m_Handle->mQuery;
        }
        // Default to an empty string
        return NullString();
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated connection handle.
    */
    const StmtRef & GetHandle() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the managed handle is valid.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the currently associated statement connection.
    */
    Connection GetConnection() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the currently associated statement connection.
    */
    void SetConnection(const Connection & conn);

    /* --------------------------------------------------------------------------------------------
     * Execute the statement.
    */
    Int32 Execute();

    /* --------------------------------------------------------------------------------------------
     * Execute the statement.
    */
    Uint32 Insert();

    /* --------------------------------------------------------------------------------------------
     * Execute the statement.
    */
    ResultSet Query();

    /* --------------------------------------------------------------------------------------------
     * Assign a signed 8bit integer to a parameter.
    */
    void SetInt8(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an unsigned 8bit integer to a parameter.
    */
    void SetUint8(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a signed 16bit integer to a parameter.
    */
    void SetInt16(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an unsigned 16bit integer to a parameter.
    */
    void SetUint16(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a signed 32bit integer to a parameter.
    */
    void SetInt32(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an unsigned 32bit integer to a parameter.
    */
    void SetUint32(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a signed 64bit integer to a parameter.
    */
    void SetInt64(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an unsigned 64bit integer to a parameter.
    */
    void SetUint64(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a signed long integer to a parameter.
    */
    void SetSLongInt(Uint32 idx, const SLongInt & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an unsigned long integer to a parameter.
    */
    void SetULongInt(Uint32 idx, const ULongInt & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a native integer to a parameter.
    */
    void SetInteger(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a 32bit floating point to a parameter.
    */
    void SetFloat32(Uint32 idx, SQFloat val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a 64bit floating point to a parameter.
    */
    void SetFloat64(Uint32 idx, SQFloat val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a native float to a parameter.
    */
    void SetFloat(Uint32 idx, SQFloat val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a boolean to a parameter.
    */
    void SetBoolean(Uint32 idx, bool val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a date to a parameter.
    */
    void SetDate(Uint32 idx, const Date & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a time to a parameter.
    */
    void SetTime(Uint32 idx, const Time & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a date and time to a parameter.
    */
    void SetDatetime(Uint32 idx, const Datetime & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a string to a parameter.
    */
    void SetString(Uint32 idx, CSStr val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an enumeration to a parameter.
    */
    void SetEnum(Uint32 idx, CSStr val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an enumeration to a parameter.
    */
    void SetSet(Uint32 idx, CSStr val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a blob to a parameter.
    */
    void SetBlob(Uint32 idx, Object & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a buffer to a paramete.
    */
    void SetData(Uint32 idx, Object & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a null to a parameter.
    */
    void SetNull(Uint32 idx) const;
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
class Parameter
{
public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Parameter();

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Parameter(const Parameter & o);

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Parameter(Parameter && o);

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Parameter();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Parameter & operator = (const Parameter & o);

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Parameter & operator = (Parameter && o);

};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
class Transaction
{
public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Transaction()
    {

    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Transaction(const Transaction & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    Transaction(Transaction && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Transaction()
    {

    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Transaction & operator = (const Transaction & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Transaction & operator = (Transaction && o) = default;

};

} // Namespace:: SqMod
