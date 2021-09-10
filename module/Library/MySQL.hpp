#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include "Library/IO/Buffer.hpp"
#include "Library/Chrono.hpp"
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Datetime.hpp"
#include "Library/Chrono/Time.hpp"
#include "Library/Chrono/Timestamp.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdbool>
#include <unordered_map>

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_POCO_HAS_MYSQL
    #include <mysql/mysql.h>
#else
    #error Enable MySQL support in order to compile this library.
#endif

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
SQMOD_NODISCARD const SQChar * SqMySQLTypename(enum_field_types type);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the capitalized name of a MySQL data-type.
*/
SQMOD_NODISCARD const SQChar * SqMySQLTypenameC(enum_field_types type);

/* ------------------------------------------------------------------------------------------------
 * Utility used to convert from database types to known types.
*/
template < typename T > struct DbConvTo;

/* ------------------------------------------------------------------------------------------------
 * Specialization for signed 8 bit integer.
*/
template < > struct DbConvTo< int8_t >
{
    SQMOD_NODISCARD static int8_t From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn = _SC("int8_t"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for unsigned 8 bit integer.
*/
template < > struct DbConvTo< uint8_t >
{
    SQMOD_NODISCARD static uint8_t From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn = _SC("uint8_t"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for signed 16 bit integer.
*/
template < > struct DbConvTo< int16_t >
{
    SQMOD_NODISCARD static int16_t From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn = _SC("int16_t"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for unsigned 16 bit integer.
*/
template < > struct DbConvTo< uint16_t >
{
    SQMOD_NODISCARD static uint16_t From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn = _SC("uint16_t"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for signed 32 bit integer.
*/
template < > struct DbConvTo< int32_t >
{
    SQMOD_NODISCARD static int32_t From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn = _SC("int32_t"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for unsigned 32 bit integer.
*/
template < > struct DbConvTo< uint32_t >
{
    SQMOD_NODISCARD static uint32_t From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn = _SC("uint32_t"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for signed 64 bit integer.
*/
template < > struct DbConvTo< int64_t >
{
    SQMOD_NODISCARD static int64_t From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn = _SC("int64_t"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for unsigned 64 bit integer.
*/
template < > struct DbConvTo< uint64_t >
{
    SQMOD_NODISCARD static uint64_t From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn = _SC("uint64_t"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for 32 floating point.
*/
template < > struct DbConvTo< float >
{
    SQMOD_NODISCARD static float From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn = _SC("float"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for 64 floating point.
*/
template < > struct DbConvTo< double >
{
    SQMOD_NODISCARD static double From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn = _SC("double"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for boolean value.
*/
template < > struct DbConvTo< bool >
{
    SQMOD_NODISCARD static bool From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn = _SC("bool"));
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for char value.
*/
template < > struct DbConvTo< char >
{
    SQMOD_NODISCARD static char From(const SQChar * value, unsigned long length, enum_field_types type, const SQChar * tn = _SC("char"));
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
    typedef Type *          Pointer; // Pointer to the managed type.
    typedef const Type *    ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type &          Reference; // Reference to the managed type.
    typedef const Type &    ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_RES       ResType; // Database result type.

public:

    // --------------------------------------------------------------------------------------------
    Pointer     mPtr; // The connection handle resource.

    // --------------------------------------------------------------------------------------------
    uint32_t    mErrNo; // Last received error string.
    String      mErrStr; // Last received error message.

    // --------------------------------------------------------------------------------------------
    uint16_t        mPort; // Server port.
    String          mHost; // Host address.
    String          mUser; // User name user.
    String          mPass; // User password.
    String          mName; // Database name.
    String          mSocket; // Unix socket.
    unsigned long   mFlags; // Client flags.

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
    void ThrowCurrent(const char * act, const char * file, int32_t line);
#else
    void ThrowCurrent(const char * act);
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
    uint64_t Execute(const SQChar * query, unsigned long size = 0UL);
};

/* ------------------------------------------------------------------------------------------------
 * The structure that holds the data associated with a certain bind point.
*/
struct StmtBind // NOLINT(cppcoreguidelines-pro-type-member-init)
{
public:

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_STMT      Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type *          Pointer; // Pointer to the managed type.
    typedef const Type *    ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type &          Reference; // Reference to the managed type.
    typedef const Type &    ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_BIND      BindType; // Database bind type.
    typedef MYSQL_TIME      TimeType; // Database time type.
#if (defined(MYSQL_VERSION_ID) && (MYSQL_VERSION_ID < 80000)) || defined(LIBMARIADB)
    typedef my_bool         BoolType; // Database boolean type.
#else
    typedef _Bool           BoolType; // Database boolean type.
#endif

public:

    // --------------------------------------------------------------------------------------------
    BoolType        mIsNull{false}; // Allows the database to specify if the parameter is null.
    BoolType        mError{false}; // Allows the database if errors occurred on this parameter.
    Buffer          mData{}; // Buffer to store non fundamental data for the parameter.
    BindType *      mBind{nullptr}; // The associated database bind point handle.
    TimeType        mTime{0}; // Structure used to store time data from database.

    // --------------------------------------------------------------------------------------------
    union
    {
        uint64_t    mUint64; // Store unsigned integer values for the parameter.
        int64_t     mInt64; // Store signed integer values for the parameter.
        int32_t     mInt32[2]; // Store 32 bit signed integer values for the parameter.
        double      mFloat64; // Store 32 bit floating point values for the parameter.
        float       mFloat32[2]; // Store 64 bit floating point values for the parameter.
    };

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    StmtBind() = default;

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
    SQMOD_NODISCARD char * GetBuffer()
    {
        return mData ? mData.Data() : reinterpret_cast< char * >(&mUint64);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the buffer length.
    */
    SQMOD_NODISCARD unsigned long GetLength() const
    {
        return (mBind == nullptr) ? 0 : mBind->buffer_length;
    }

    /* --------------------------------------------------------------------------------------------
     * Configure the input of a certain statement parameter.
    */
    void SetInput(enum_field_types type, BindType * bind, const char * buffer = nullptr, unsigned long length = 0);
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
    typedef Type *          Pointer; // Pointer to the managed type.
    typedef const Type *    ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type &          Reference; // Reference to the managed type.
    typedef const Type &    ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_BIND      BindType; // Database bind type.
    typedef MYSQL_TIME      TimeType; // Database time type.
#if (defined(MYSQL_VERSION_ID) && (MYSQL_VERSION_ID < 80000)) || defined(LIBMARIADB)
    typedef my_bool         BoolType; // Database boolean type.
#else
    typedef _Bool           BoolType; // Database boolean type.
#endif

public:

    // --------------------------------------------------------------------------------------------
    Pointer         mPtr; // The managed statement handle.

    // --------------------------------------------------------------------------------------------
    uint32_t        mErrNo; // Last received error string.
    String          mErrStr; // Last received error message.

    // --------------------------------------------------------------------------------------------
    unsigned long   mParams; // Number of parameters in the statement.
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
    void ThrowCurrent(const char * act, const char * file, int32_t line);
#else
    void ThrowCurrent(const char * act);
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated statement handle and parameter index and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateParam(uint32_t idx, const char * file, int32_t line) const;
#else
    void ValidateParam(uint32_t idx) const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Check whether a specific param index is within range.
    */
    SQMOD_NODISCARD bool CheckParamIndex(uint32_t idx) const
    {
        return (idx < mParams);
    }

    /* --------------------------------------------------------------------------------------------
     * Create the actual statement.
    */
    void Create(const ConnRef & conn, const SQChar * query);
};

/* ------------------------------------------------------------------------------------------------
 * The structure that holds the data associated with a certain field.
*/
struct ResBind // NOLINT(cppcoreguidelines-pro-type-member-init)
{
public:

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_RES       Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type *          Pointer; // Pointer to the managed type.
    typedef const Type *    ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type &          Reference; // Reference to the managed type.
    typedef const Type &    ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_FIELD     FieldType; // Database field type.
    typedef MYSQL_BIND      BindType; // Database bind type.
    typedef MYSQL_TIME      TimeType; // Database time type.
    typedef MYSQL_ROW       RowType; // Database row type.
#if (defined(MYSQL_VERSION_ID) && (MYSQL_VERSION_ID < 80000)) || defined(LIBMARIADB)
    typedef my_bool         BoolType; // Database boolean type.
#else
    typedef _Bool           BoolType; // Database boolean type.
#endif

    // --------------------------------------------------------------------------------------------
    typedef std::unordered_map< String, uint32_t > IndexMap;

public:

    // --------------------------------------------------------------------------------------------
    BoolType        mIsNull{false}; // Allows the database to specify if the field is null.
    BoolType        mError{false}; // Allows the database if errors occurred on this field.
    Buffer          mData{}; // Buffer to store non fundamental data for the field.
    BindType *      mBind{nullptr}; // The associated database bind point handle.
    TimeType        mTime{0}; // Structure used to retrieve time data from database.

    // --------------------------------------------------------------------------------------------
    union
    {
        uint64_t    mUint64{}; // Retrieve unsigned integer values from a field.
        int64_t     mInt64; // Retrieve signed integer values from a field.
        int32_t     mInt32[2]; // Retrieve 32 bit signed integer values from a field.
        double      mFloat64; // Retrieve 32 bit floating point values from a field.
        float       mFloat32[2]; // Retrieve 64 bit floating point values from the field.
    };

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ResBind() = default;

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
    SQMOD_NODISCARD char * GetBuffer()
    {
        return mData ? mData.Data() : reinterpret_cast< char * >(&mUint64);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the buffer length.
    */
    SQMOD_NODISCARD unsigned long GetLength() const
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
    typedef Type *          Pointer; // Pointer to the managed type.
    typedef const Type *    ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type &          Reference; // Reference to the managed type.
    typedef const Type &    ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_FIELD     FieldType; // Database field type.
    typedef MYSQL_BIND      BindType; // Database bind type.
    typedef MYSQL_TIME      TimeType; // Database time type.
    typedef MYSQL_ROW       RowType; // Database row type.
#if (defined(MYSQL_VERSION_ID) && (MYSQL_VERSION_ID < 80000)) || defined(LIBMARIADB)
    typedef my_bool         BoolType; // Database boolean type.
#else
    typedef _Bool           BoolType; // Database boolean type.
#endif

    // --------------------------------------------------------------------------------------------
    typedef std::unordered_map< String, uint32_t > IndexMap; // Name to index association of fields.

public:

    // --------------------------------------------------------------------------------------------
    Pointer         mPtr; // The managed result-set handle.

    // --------------------------------------------------------------------------------------------
    uint32_t        mFieldCount; // Number of fields in the result-set.
    unsigned long * mLengths; // Data length when the result-set came from a connection.
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
    void GrabCurrent() const;

    /* --------------------------------------------------------------------------------------------
     * Grab the current error in the associated statement or connection handle and throw it.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ThrowCurrent(const char * act, const char * file, int32_t line) const;
#else
    void ThrowCurrent(const char * act) const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the statement handle and field index and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateField(uint32_t idx, const char * file, int32_t line) const;
#else
    void ValidateField(uint32_t idx) const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Check whether a specific field index is within range.
    */
    bool CheckFieldIndex(uint32_t idx) const
    {
        return (idx < mFieldCount);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the field index associated with the specified name.
    */
    uint32_t GetFieldIndex(const SQChar * name);

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
    uint64_t RowIndex() const;

    /* --------------------------------------------------------------------------------------------
     * Returns the number of rows in the result set.
    */
    uint64_t RowCount() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the next row from the query.
    */
    bool Next();

    /* --------------------------------------------------------------------------------------------
     * Seeks to an arbitrary row in a query result set.
    */
    bool SetRowIndex(uint64_t index);

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
    uint16_t        m_Port; // Server port.
    String          m_Host; // Host address.
    String          m_User; // User name.
    String          m_Pass; // User password.
    String          m_Name; // Database name.
    String          m_Socket; // Unix socket.
    unsigned long   m_Flags; // Client connection flags.

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
    Account(const SQChar * host, const SQChar * user)
        : Account(host, user, _SC(""), _SC(""), 3306, _SC(""))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base Constructor.
    */
    Account(const SQChar * host, const SQChar * user, const SQChar * pass)
        : Account(host, user, pass, _SC(""), 3306, _SC(""))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base Constructor.
    */
    Account(const SQChar * host, const SQChar * user, const SQChar * pass, const SQChar * name)
        : Account(host, user, pass, name, 3306, _SC(""))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base Constructor.
    */
    Account(const SQChar * host, const SQChar * user, const SQChar * pass, const SQChar * name, SQInteger port)
        : Account(host, user, pass, name, port, _SC(""))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base Constructor.
    */
    Account(const SQChar * host, const SQChar * user, const SQChar * pass, const SQChar * name, SQInteger port, const SQChar * socket);

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
    int32_t Cmp(const Account & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    String ToString() const;

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
    void SetHost(const SQChar * addr);

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
    void SetUser(const SQChar * user)
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
    void SetPass(const SQChar * pass)
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
    void SetName(const SQChar * name)
    {
        m_Name.assign(name != nullptr ? name : _SC(""));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the server port specified during creation.
    */
    uint16_t GetPortNum() const
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
    void SetSocket(const SQChar * socket)
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
        m_Flags = static_cast<unsigned long>(flags);
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
    void SetSSL_Key(const SQChar * key)
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
    void SetSSL_Cert(const SQChar * cert)
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
    void SetSSL_CA(const SQChar * ca)
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
    void SetSSL_CA_Path(const SQChar * capath)
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
    void SetSSL_Cipher(const SQChar * cipher)
    {
        m_SSL_Cipher.assign(cipher != nullptr ? cipher : _SC(""));
    }

    /* --------------------------------------------------------------------------------------------
     * Set the SSL information.
    */
    void SetSSL(const SQChar * key, const SQChar * cert, const SQChar * ca, const SQChar * ca_path, const SQChar * cipher);

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
    const String & GetOption(const SQChar * name) const;

    /* --------------------------------------------------------------------------------------------
     * Set a value in the options container.
    */
    void SetOption(const SQChar * name, const SQChar * value);

    /* --------------------------------------------------------------------------------------------
     * Remove a value from the options container.
    */
    void RemoveOption(const SQChar * name);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of values in the options container.
    */
    uint32_t OptionsCount() const
    {
        return static_cast< uint32_t >(m_Options.size());
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
    ConnRef     m_Handle{}; // Reference to the actual database connection.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(const char * file, int32_t line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateCreated(const char * file, int32_t line) const;
#else
    void ValidateCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ConnRef & GetValid(const char * file, int32_t line) const;
#else
    SQMOD_NODISCARD const ConnRef & GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ConnRef & GetCreated(const char * file, int32_t line) const;
#else
    SQMOD_NODISCARD const ConnRef & GetCreated() const;
#endif // _DEBUG

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Connection() = default;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit Connection(const Account & acc)
        : m_Handle(new ConnHnd())
    {
        m_Handle->Create(acc);
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit Connection(const ConnRef & conn) // NOLINT(modernize-pass-by-value)
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
    SQMOD_NODISCARD int32_t Cmp(const Connection & o) const
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
    SQMOD_NODISCARD const SQChar * ToString() const
    {
        return m_Handle ? mysql_get_host_info(m_Handle->mPtr) : _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    SQMOD_NODISCARD static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated connection handle.
    */
    SQMOD_NODISCARD const ConnRef & GetHandle() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the managed handle is valid.
    */
    SQMOD_NODISCARD bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the managed connection handle was connected.
    */
    SQMOD_NODISCARD bool IsConnected() const
    {
        return m_Handle && (m_Handle->mPtr != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this connection handle.
    */
    SQMOD_NODISCARD uint32_t GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current error number.
    */
    SQMOD_NODISCARD SQInteger GetErrNo() const
    {
        return static_cast< SQInteger >(mysql_errno(SQMOD_GET_CREATED(*this)->mPtr));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current error message.
    */
    SQMOD_NODISCARD const SQChar * GetErrStr() const
    {
        return mysql_error(SQMOD_GET_CREATED(*this)->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last received error number.
    */
    SQMOD_NODISCARD SQInteger GetLastErrNo() const
    {
        return static_cast< SQInteger >(SQMOD_GET_VALID(*this)->mErrNo);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last received error message.
    */
    SQMOD_NODISCARD const String & GetLastErrStr() const
    {
        return SQMOD_GET_VALID(*this)->mErrStr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection port number.
    */
    SQMOD_NODISCARD SQInteger GetPortNum() const
    {
        return static_cast< SQInteger >(SQMOD_GET_VALID(*this)->mPort);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection host address.
    */
    SQMOD_NODISCARD const String & GetHost() const
    {
        return SQMOD_GET_VALID(*this)->mHost;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection user name.
    */
    SQMOD_NODISCARD const String & GetUser() const
    {
        return SQMOD_GET_VALID(*this)->mUser;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection password.
    */
    SQMOD_NODISCARD const String & GetPass() const
    {
        return SQMOD_GET_VALID(*this)->mPass;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the selected database name.
    */
    SQMOD_NODISCARD const String & GetName() const
    {
        return SQMOD_GET_VALID(*this)->mName;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the selected database name.
    */
    void SetName(const SQChar * name)
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
    SQMOD_NODISCARD const String & GetSocket() const
    {
        return SQMOD_GET_VALID(*this)->mSocket;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection flags.
    */
    SQMOD_NODISCARD SQInteger GetFlags() const
    {
        return static_cast< SQInteger >(SQMOD_GET_VALID(*this)->mFlags);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL key.
    */
    SQMOD_NODISCARD const String & GetSSL_Key() const
    {
        return SQMOD_GET_VALID(*this)->mSSL_Key;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL certificate.
    */
    SQMOD_NODISCARD const String & GetSSL_Cert() const
    {
        return SQMOD_GET_VALID(*this)->mSSL_Cert;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL certificate authority.
    */
    SQMOD_NODISCARD const String & GetSSL_CA() const
    {
        return SQMOD_GET_VALID(*this)->mSSL_CA;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL certificate authority path.
    */
    SQMOD_NODISCARD const String & GetSSL_CA_Path() const
    {
        return SQMOD_GET_VALID(*this)->mSSL_CA_Path;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL cipher.
    */
    SQMOD_NODISCARD const String & GetSSL_Cipher() const
    {
        return SQMOD_GET_VALID(*this)->mSSL_Cipher;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the default character set for the managed connection.
    */
    SQMOD_NODISCARD const String & GetCharset() const
    {
        return SQMOD_GET_VALID(*this)->mCharset;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the default character set for the managed connection.
    */
    void SetCharset(const SQChar * charset)
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
    SQMOD_NODISCARD bool GetAutoCommit() const
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
            mysql_autocommit(m_Handle->mPtr, static_cast< StmtBind::BoolType >(toggle)) != 0)
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
    SQMOD_NODISCARD bool GetInTransaction() const
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
    SQInteger Execute(const SQChar * query)
    {
        return static_cast< SQInteger >(SQMOD_GET_CREATED(*this)->Execute(query));
    }

    /* --------------------------------------------------------------------------------------------
     * Execute a query on the server.
    */
    SQInteger Insert(const SQChar * query);

    /* --------------------------------------------------------------------------------------------
     * Execute a query on the server.
    */
    ResultSet Query(const SQChar * query);

    /* --------------------------------------------------------------------------------------------
     * Create a new statement on the managed connection.
    */
    SQMOD_NODISCARD Statement GetStatement(const SQChar * query);

    /* --------------------------------------------------------------------------------------------
     * Create a new transaction on the managed connection.
    */
    //SQMOD_NODISCARD Transaction GetTransaction();

    /* --------------------------------------------------------------------------------------------
     * Escape unwanted characters from a given string.
    */
    SQMOD_NODISCARD LightObj EscapeString(StackStrF & str);

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
    uint32_t    m_Index; // The actual index of the referenced field.
    ResRef      m_Handle; // Reference to the actual database result-set.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle and field index, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(const char * file, int32_t line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle and field index, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateCreated(const char * file, int32_t line) const;
#else
    void ValidateCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle, field index and row, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateStepped(const char * file, int32_t line) const;
#else
    void ValidateStepped() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle and field index, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    SQMOD_NODISCARD const ResRef & GetValid(const char * file, int32_t line) const;
#else
    SQMOD_NODISCARD const ResRef & GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle and field index, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    SQMOD_NODISCARD const ResRef & GetCreated(const char * file, int32_t line) const;
#else
    SQMOD_NODISCARD const ResRef & GetCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle field index and row, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    SQMOD_NODISCARD const ResRef & GetStepped(const char * file, int32_t line) const;
#else
    SQMOD_NODISCARD const ResRef & GetStepped() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle and field index, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateField(uint32_t idx, const char * file, int32_t line) const;
#else
    void ValidateField(uint32_t idx) const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(uint32_t idx)
    {
        SQMOD_VALIDATE_FIELD(*this, idx);
        // Assign the new index
        m_Index = idx;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(const SQChar * name)
    {
        SetIndex(SQMOD_GET_CREATED(*this)->GetFieldIndex(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(const Object & field);

public:

    // --------------------------------------------------------------------------------------------
    static constexpr uint32_t INVALID_INDEX = std::numeric_limits< uint32_t >::max(); // Value that represents an invalid index.

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
    explicit Field(const ResRef & rset) // NOLINT(modernize-pass-by-value)
        : m_Index(INVALID_INDEX), m_Handle(rset)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Index constructor.
    */
    Field(const ResRef & rset, uint32_t idx) // NOLINT(modernize-pass-by-value)
        : m_Index(idx), m_Handle(rset)
    {
        SQMOD_VALIDATE_FIELD(*this, m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Name constructor.
    */
    Field(const ResRef & rset, const SQChar * name)
        : m_Index(rset ? rset->GetFieldIndex(name) : -1), m_Handle(rset)
    {
        SQMOD_VALIDATE_FIELD(*this, m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Dynamic constructor.
    */
    Field(const ResRef & rset, const Object & field) // NOLINT(modernize-pass-by-value)
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
    operator bool () const // NOLINT(google-explicit-constructor)
    {
        return (m_Handle && m_Handle->CheckFieldIndex(m_Index));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    SQMOD_NODISCARD int32_t Cmp(const Field & o) const
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
    SQMOD_NODISCARD String ToString() const
    {
        // Can we attempt to return the parameter name?
        if (m_Handle && m_Handle->CheckFieldIndex(m_Index))
        {
            return m_Handle->mFields[m_Index].name;
        }
        return fmt::format("{}", m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether the field is valid.
    */
    SQMOD_NODISCARD bool IsValid() const
    {
        return m_Handle; // An invalid result-set means an invalid field
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to the associated result-set handle.
    */
    SQMOD_NODISCARD uint32_t GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the referenced field index.
    */
    SQMOD_NODISCARD SQInteger GetIndex() const
    {
        return ConvTo< SQInteger >::From(m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the referenced database result-set.
    */
    SQMOD_NODISCARD Object GetResultSet() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the referenced database connection.
    */
    SQMOD_NODISCARD Object GetConnection() const;

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
    SQMOD_NODISCARD bool GetBoolean() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a character.
    */
    SQMOD_NODISCARD SQChar GetChar() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a native script integer.
    */
    SQMOD_NODISCARD SQInteger GetInteger() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a native script floating point.
    */
    SQMOD_NODISCARD SQFloat GetFloat() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a signed 8 bit integer value.
    */
    SQMOD_NODISCARD SQInteger GetInt8() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as an unsigned 8 bit integer value.
    */
    SQMOD_NODISCARD SQInteger GetUint8() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a signed 16 bit integer value.
    */
    SQMOD_NODISCARD SQInteger GetInt16() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as an unsigned 16 bit integer value.
    */
    SQMOD_NODISCARD SQInteger GetUint16() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a signed 32 bit integer value.
    */
    SQMOD_NODISCARD SQInteger GetInt32() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as an unsigned 32 bit integer value.
    */
    SQMOD_NODISCARD SQInteger GetUint32() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a signed 64 bit integer value.
    */
    SQMOD_NODISCARD SQInteger GetInt64() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as an unsigned 64 bit integer value.
    */
    SQMOD_NODISCARD SQInteger GetUint64() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a 32 bit floating point value.
    */
    SQMOD_NODISCARD SQFloat GetFloat32() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a 64 bit floating point value.
    */
    SQMOD_NODISCARD SQFloat GetFloat64() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a string value.
    */
    SQMOD_NODISCARD Object GetString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a memory buffer.
    */
    SQMOD_NODISCARD Object GetBuffer() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a memory blob.
    */
    SQMOD_NODISCARD Object GetBlob() const;
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
    void Validate(const char * file, int32_t line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateCreated(const char * file, int32_t line) const;
#else
    void ValidateCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and row, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateStepped(const char * file, int32_t line) const;
#else
    void ValidateStepped() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    SQMOD_NODISCARD const ResRef & GetValid(const char * file, int32_t line) const;
#else
    SQMOD_NODISCARD const ResRef & GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    SQMOD_NODISCARD const ResRef & GetCreated(const char * file, int32_t line) const;
#else
    SQMOD_NODISCARD const ResRef & GetCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and row, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    SQMOD_NODISCARD const ResRef & GetStepped(const char * file, int32_t line) const;
#else
    SQMOD_NODISCARD const ResRef & GetStepped() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference and field index, and throw an error if they're invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateField(int32_t idx, const char * file, int32_t line) const;
#else
    void ValidateField(int32_t idx) const;
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
    SQMOD_NODISCARD int32_t Cmp(const ResultSet & o) const
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
    SQMOD_NODISCARD String ToString() const
    {
        // Do we have a valid handle?
        if (m_Handle)
        {
            fmt::format("{}", m_Handle->mFieldCount);
        }
        // Default to a negative value
        return "-1";
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    SQMOD_NODISCARD static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether the managed handle is valid.
    */
    SQMOD_NODISCARD bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Returns an array with all the field names available in the managed result set.
    */
    SQMOD_NODISCARD Array GetFieldNames() const;

    /* --------------------------------------------------------------------------------------------
     * Returns an array with wrapper instances for all the field available in the managed result set.
    */
    SQMOD_NODISCARD Array GetFieldsArray() const;

    /* --------------------------------------------------------------------------------------------
     * Returns an array with wrapper instances for the specified fields in the managed result set.
    */
    SQMOD_NODISCARD Array FetchFieldsArray(Array & fields) const;

    /* --------------------------------------------------------------------------------------------
     * Returns a table with wrapper instances for all the field available in the managed result set.
    */
    SQMOD_NODISCARD Table GetFieldsTable() const;

    /* --------------------------------------------------------------------------------------------
     * Returns a table with wrapper instances for all the specified fields in the managed result set.
    */
    SQMOD_NODISCARD Table FetchFieldsTable(Array & fields) const;

    /* --------------------------------------------------------------------------------------------
     * Returns the current position of the row cursor for the last Next().
    */
    SQMOD_NODISCARD SQInteger RowIndex() const
    {
        return static_cast< SQInteger >(SQMOD_GET_CREATED(*this)->RowIndex());
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the number of rows in the result set.
    */
    SQMOD_NODISCARD SQInteger RowCount() const
    {
        return static_cast< SQInteger >(SQMOD_GET_CREATED(*this)->RowCount());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the next row from the query.
    */
    SQMOD_NODISCARD bool Next() const
    {
        return SQMOD_GET_CREATED(*this)->Next();
    }

    /* --------------------------------------------------------------------------------------------
     * Seeks to an arbitrary row in a query result set.
    */
    SQMOD_NODISCARD bool SetRowIndex(SQInteger index) const
    {
        return SQMOD_GET_CREATED(*this)->SetRowIndex(ConvTo< uint64_t >::From(index));
    }

    /* --------------------------------------------------------------------------------------------
     * Seeks to an arbitrary row in a query result set.
    */
    SQMOD_NODISCARD bool SetLongRowIndex(SQInteger index) const
    {
        return SQMOD_GET_CREATED(*this)->SetRowIndex(static_cast< uint64_t >(index));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the field with the specified name or index.
    */
    SQMOD_NODISCARD Field GetField(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a boolean value.
    */
    SQMOD_NODISCARD bool GetBoolean(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetBoolean();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a character.
    */
    SQMOD_NODISCARD SQChar GetChar(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetChar();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a native script integer.
    */
    SQMOD_NODISCARD SQInteger GetInteger(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetInteger();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a native script floating point.
    */
    SQMOD_NODISCARD SQFloat GetFloat(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetFloat();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a signed 8 bit integer value.
    */
    SQMOD_NODISCARD SQInteger GetInt8(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetInt8();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as an unsigned 8 bit integer value.
    */
    SQMOD_NODISCARD SQInteger GetUint8(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetUint8();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a signed 16 bit integer value.
    */
    SQMOD_NODISCARD SQInteger GetInt16(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetInt16();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as an unsigned 16 bit integer value.
    */
    SQMOD_NODISCARD SQInteger GetUint16(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetUint16();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a signed 32 bit integer value.
    */
    SQMOD_NODISCARD SQInteger GetInt32(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetInt32();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as an unsigned 32 bit integer value.
    */
    SQMOD_NODISCARD SQInteger GetUint32(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetUint32();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a signed 64 bit integer value.
    */
    SQMOD_NODISCARD Object GetInt64(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetInt64();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as an unsigned 64 bit integer value.
    */
    SQMOD_NODISCARD Object GetUint64(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetUint64();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a 32 bit floating point value.
    */
    SQMOD_NODISCARD SQFloat GetFloat32(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetFloat32();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a 64 bit floating point value.
    */
    SQMOD_NODISCARD SQFloat GetFloat64(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetFloat64();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a string value.
    */
    SQMOD_NODISCARD Object GetString(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetString();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a memory buffer.
    */
    SQMOD_NODISCARD Object GetBuffer(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetBuffer();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a memory blob.
    */
    SQMOD_NODISCARD Object GetBlob(const Object & field) const
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
    void Validate(const char * file, int32_t line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateCreated(const char * file, int32_t line) const;
#else
    void ValidateCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const StmtRef & GetValid(const char * file, int32_t line) const;
#else
    SQMOD_NODISCARD const StmtRef & GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const StmtRef & GetCreated(const char * file, int32_t line) const;
#else
    SQMOD_NODISCARD const StmtRef & GetCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference and parameter index, and throw an error if they're invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateParam(int32_t idx, const char * file, int32_t line) const;
#else
    void ValidateParam(int32_t idx) const;
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
    Statement(const ConnRef & connection, const SQChar * query)
        : m_Handle(new StmtHnd())
    {
        m_Handle->Create(connection, query);
    }

    /* --------------------------------------------------------------------------------------------
     * Construct a statement under the specified connection using the specified string.
    */
    Statement(const Connection & connection, const SQChar * query);

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
    SQMOD_NODISCARD int32_t Cmp(const Statement & o) const
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
    SQMOD_NODISCARD const String & ToString() const
    {
        // Do we have a valid handle?
        if (m_Handle)
        {
            return m_Handle->mQuery;
        }
        // Default to an empty string
        return NullString();
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    SQMOD_NODISCARD static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated connection handle.
    */
    SQMOD_NODISCARD const StmtRef & GetHandle() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the managed handle is valid.
    */
    SQMOD_NODISCARD bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the currently associated statement connection.
    */
    SQMOD_NODISCARD Connection GetConnection() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the currently associated statement connection.
    */
    void SetConnection(const Connection & conn);

    /* --------------------------------------------------------------------------------------------
     * Execute the statement.
    */
    SQMOD_NODISCARD int32_t Execute();

    /* --------------------------------------------------------------------------------------------
     * Execute the statement.
    */
    SQMOD_NODISCARD uint32_t Insert();

    /* --------------------------------------------------------------------------------------------
     * Execute the statement.
    */
    SQMOD_NODISCARD ResultSet Query();

    /* --------------------------------------------------------------------------------------------
     * Assign a signed 8bit integer to a parameter.
    */
    void SetInt8(uint32_t idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an unsigned 8bit integer to a parameter.
    */
    void SetUint8(uint32_t idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a signed 16bit integer to a parameter.
    */
    void SetInt16(uint32_t idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an unsigned 16bit integer to a parameter.
    */
    void SetUint16(uint32_t idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a signed 32bit integer to a parameter.
    */
    void SetInt32(uint32_t idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an unsigned 32bit integer to a parameter.
    */
    void SetUint32(uint32_t idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a signed 64bit integer to a parameter.
    */
    void SetInt64(uint32_t idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an unsigned 64bit integer to a parameter.
    */
    void SetUint64(uint32_t idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a signed long integer to a parameter.
    */
    void SetSLongInt(uint32_t idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an unsigned long integer to a parameter.
    */
    void SetULongInt(uint32_t idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a native integer to a parameter.
    */
    void SetInteger(uint32_t idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a 32bit floating point to a parameter.
    */
    void SetFloat32(uint32_t idx, SQFloat val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a 64bit floating point to a parameter.
    */
    void SetFloat64(uint32_t idx, SQFloat val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a native float to a parameter.
    */
    void SetFloat(uint32_t idx, SQFloat val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a boolean to a parameter.
    */
    void SetBoolean(uint32_t idx, bool val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a date to a parameter.
    */
    void SetDate(uint32_t idx, const Date & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a time to a parameter.
    */
    void SetTime(uint32_t idx, const Time & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a date and time to a parameter.
    */
    void SetDatetime(uint32_t idx, const Datetime & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a string to a parameter.
    */
    void SetString(uint32_t idx, const SQChar * val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an enumeration to a parameter.
    */
    void SetEnum(uint32_t idx, const SQChar * val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an enumeration to a parameter.
    */
    void SetSet(uint32_t idx, const SQChar * val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a blob to a parameter.
    */
    void SetBlob(uint32_t idx, Object & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a buffer to a parameter.
    */
    void SetData(uint32_t idx, Object & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a null to a parameter.
    */
    void SetNull(uint32_t idx) const;
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
struct Parameter
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Parameter() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Parameter(const Parameter & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Parameter(Parameter && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Parameter() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Parameter & operator = (const Parameter & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Parameter & operator = (Parameter && o) = default;
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
struct Transaction
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Transaction() = default;

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
    ~Transaction() = default;

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
