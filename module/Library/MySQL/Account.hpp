#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/MySQL/Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

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

} // Namespace:: SqMod
