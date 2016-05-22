#ifndef _SQMYSQL_ACCOUNT_HPP_
#define _SQMYSQL_ACCOUNT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

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
    Account(CSStr host, CSStr user, CSStr pass, CSStr schema)
        : Account(host, user, pass, schema, 3306, _SC(""))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base Constructor.
    */
    Account(CSStr host, CSStr user, CSStr pass, CSStr schema, Uint16 port)
        : Account(host, user, pass, schema, 3306, _SC(""))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base Constructor.
    */
    Account(CSStr host, CSStr user, CSStr pass, CSStr schema, Uint16 port, CSStr socket);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    Account(const Account & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    Account(Account && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Account() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    Account & operator = (const Account & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    Account & operator = (Account && o) = default;

private:

    // --------------------------------------------------------------------------------------------
    String      m_Host; // Host address.
    String      m_User; // User name.
    String      m_Pass; // User password.
    Uint16      m_Port; // Server port.

    // --------------------------------------------------------------------------------------------
    String      m_Schema; // Server schema.
    String      m_Socket; // Unix socket.

    // --------------------------------------------------------------------------------------------
    String      m_SSL_Key; // SSL key.
    String      m_SSL_Cert; // SSL certificate.
    String      m_SSL_CA; // SSL certificate authority.
    String      m_SSL_CA_Path; // SSL certificate authority path.
    String      m_SSL_Cipher; // SSL Cipher.

    // --------------------------------------------------------------------------------------------
    bool        m_Autocommit; // Toggle autocommit.

    // --------------------------------------------------------------------------------------------
    Options     m_Options; // Option container.

    // --------------------------------------------------------------------------------------------
    static const String s_String; // Dummy used to return a reference to a null string.

public:

    /* --------------------------------------------------------------------------------------------
     * Get the host address specified during creation.
    */
    const String & GetHost() const
    {
        return m_Host;
    }

    /* --------------------------------------------------------------------------------------------
     * Get the user name specified during creation.
    */
    const String & GetUser() const
    {
        return m_User;
    }

    /* --------------------------------------------------------------------------------------------
     * Get the password specified during creation.
    */
    const String & GetPass() const
    {
        return m_Pass;
    }

    /* --------------------------------------------------------------------------------------------
     * Get the server port specified during creation.
    */
    const Uint16 GetPort() const
    {
        return m_Port;
    }

    /* --------------------------------------------------------------------------------------------
     * Get the server schema.
    */
    const String & GetSchema() const
    {
        return m_Schema;
    }

    /* --------------------------------------------------------------------------------------------
     * Set the server schema.
    */
    const String & GetSocket() const
    {
        return m_Socket;
    }

    /* --------------------------------------------------------------------------------------------
     * Get the SSL key.
    */
    const String & GetSSL_Key() const
    {
        return m_SSL_Key;
    }

    /* --------------------------------------------------------------------------------------------
     * Get the SSL certificate.
    */
    const String & GetSSL_Cert() const
    {
        return m_SSL_Cipher;
    }

    /* --------------------------------------------------------------------------------------------
     * Get the SSL certificate authority.
    */
    const String & GetSSL_CA() const
    {
        return m_SSL_CA;
    }

    /* --------------------------------------------------------------------------------------------
     * Get the SSL certificate authority path.
    */
    const String & GetSSL_CA_Path() const
    {
        return m_SSL_CA_Path;
    }

    /* --------------------------------------------------------------------------------------------
     * Get the SSL cipher.
    */
    const String & GetSSL_Cipher() const
    {
        return m_SSL_Cipher;
    }

    /* --------------------------------------------------------------------------------------------
     * Set the SSL information.
    */
    void SetSSL(CSStr key, CSStr cert, CSStr ca, CSStr ca_path, CSStr cipher);

    /* --------------------------------------------------------------------------------------------
     * See whether autocommit is enabled or not.
    */
    bool GetAutocommit() const
    {
        return m_Autocommit;
    }

    /* --------------------------------------------------------------------------------------------
     * Set whether autocommit should be enabled or not.
    */
    void SetAutocommit(bool toggle)
    {
        m_Autocommit = toggle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the entire options container.
    */
    Table GetOptions() const;

    /* --------------------------------------------------------------------------------------------
     * Get a value from the options container.
    */
    const String & GetOption(CSStr name) const;

    /* --------------------------------------------------------------------------------------------
     * Set a value in the options container.
    */
    void SetOption(CSStr name, CSStr value);

    /* --------------------------------------------------------------------------------------------
     * Remove a value from the options container.
    */
    void RemoveOption(CSStr name) const;

    /* --------------------------------------------------------------------------------------------
     * Get the number of values in the options container.
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
};

} // Namespace:: SqMod

#endif // _SQMYSQL_ACCOUNT_HPP_
