#ifndef _SQMYSQL_HANDLE_CONNECTION_HPP_
#define _SQMYSQL_HANDLE_CONNECTION_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

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

} // Namespace:: SqMod

#endif // _SQMYSQL_HANDLE_CONNECTION_HPP_
