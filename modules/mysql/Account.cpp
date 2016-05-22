// ------------------------------------------------------------------------------------------------
#include "Account.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Account::Account(CSStr host, CSStr user, CSStr pass, CSStr schema, Uint16 port, CSStr socket)
    : m_Host(host ? host : _SC(""))
    , m_User(user ? user : _SC(""))
    , m_Pass(pass ? pass : _SC(""))
    , m_Port(port)
    , m_Schema(schema ? schema : _SC(""))
    , m_Socket(socket ? socket : _SC(""))
    , m_SSL_Key()
    , m_SSL_Cert()
    , m_SSL_CA()
    , m_SSL_CA_Path()
    , m_SSL_Cipher()
    , m_Autocommit()
    , m_Options()
{
    // Attempt to extract the port number from the host
    String::size_type pos = m_Host.find(':');
    // Was it specified in the host
    if (pos != String::npos)
    {
        try
        {
            // Attempt to extract the numeric value
            ULong num = std::stol(m_Host.substr(pos + 1));
            // Is the port number withing range?
            if (num > 0xFFFF)
            {
                SHROWF("Port number out of range: %lu");
            }
            // Assign the extracted port number
            m_Port = static_cast< Uint16 >(num);
            // Remove everything after the port number
            m_Host = m_Host.substr(0, pos);
        }
        catch (const std::exception & e)
        {
            SHROWF("Cannot extract the port number [%s]", e.what());
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Account::SetSSL(CSStr key, CSStr cert, CSStr ca, CSStr ca_path, CSStr cipher)
{
    if (!key || *key == '\0')
    {
        m_SSL_Key.clear();
        m_SSL_Cert.clear();
        m_SSL_CA.clear();
        m_SSL_CA_Path.clear();
        m_SSL_Cipher.clear();
    }
    else
    {
        m_SSL_Key.assign(key ? key : _SC(""));
        m_SSL_Cert.assign(cert ? cert : _SC(""));
        m_SSL_CA.assign(ca ? ca : _SC(""));
        m_SSL_CA_Path.assign(ca_path ? ca_path : _SC(""));
        m_SSL_Cipher.assign(cipher ? cipher : _SC(""));
    }
}

// ------------------------------------------------------------------------------------------------
Table Account::GetOptions() const
{
    // Allocate an empty table
    Table tbl(DefaultVM::Get());
    // Insert every option into the table
    for (const auto & opt : m_Options)
    {
        tbl.SetValue(opt.first.c_str(), opt.second);
    }
    // Return the resulted table
    return tbl;
}

// ------------------------------------------------------------------------------------------------
const String & Account::GetOption(CSStr name) const
{
    // Make sure the specified name is valid
    if (!name || *name == '\0')
    {
        SHROWF("Invalid or empty option name");
    }
    // Attempt to find the requested option
    const Options::const_iterator itr = m_Options.find(name);
    // Return the result
    return (itr == m_Options.cend()) ? s_String : itr->second;
}

// ------------------------------------------------------------------------------------------------
void Account::SetOption(CSStr name, CSStr value)
{
    // Make sure the specified name is valid
    if (!name || *name == '\0')
    {
        SHROWF("Invalid or empty option name");
    }
    // Assign the specified value
    m_Options[name] = value ? value : _SC("");
}

// ------------------------------------------------------------------------------------------------
void Account::RemoveOption(CSStr name) const
{
    // Make sure the specified name is valid
    if (!name || *name == '\0')
    {
        SHROWF("Invalid or empty option name");
    }
    // Erase the specified value
    m_Options.erase(name);
}

} // Namespace:: SqMod
