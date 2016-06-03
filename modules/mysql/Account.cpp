// ------------------------------------------------------------------------------------------------
#include "Account.hpp"
#include "Connection.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const String Account::s_String = String(_SC(""));

// ------------------------------------------------------------------------------------------------
SQInteger Account::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMySQLAccount");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Account::Account(CSStr host, CSStr user, CSStr pass, CSStr name, SQInteger port, CSStr socket)
    : m_Port(0)
    , m_Host()
    , m_User()
    , m_Pass()
    , m_Name()
    , m_Socket()
    , m_Flags(CLIENT_MULTI_STATEMENTS)
    , m_SSL_Key()
    , m_SSL_Cert()
    , m_SSL_CA()
    , m_SSL_CA_Path()
    , m_SSL_Cipher()
    , m_Options()
    , m_AutoCommit(false)
{
    // Validate the specified port number
    if (port >= 0xFFFF)
    {
        STHROWF("Port number out of range: " _PRINT_INT_FMT, port);
    }
    // Assign the specified port
    else
    {
        m_Port = ConvTo< Uint16 >::From(port);
    }
    // Assign the remaining values
    SetHost(host);
    SetUser(user);
    SetPass(pass);
    SetName(name);
    SetSocket(socket);
}

// ------------------------------------------------------------------------------------------------
Int32 Account::Cmp(const Account & o) const
{
    if (m_User == o.m_User && m_Pass == o.m_Pass)
    {
        return 0;
    }
    else if (m_User > o.m_User && m_Pass > o.m_Pass)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
CSStr Account::ToString() const
{
    return ToStringF("%s:%s@%s:%u", m_User.c_str(), m_Pass.c_str(), m_Host.c_str(), m_Port);
}

// ------------------------------------------------------------------------------------------------
void Account::SetHost(CSStr addr)
{
    // Clear the current host address
    m_Host.assign(_SC(""));
    // Validate the given address
    if (!addr || *addr == '\0')
    {
        return; // Nothing to set.
    }
    // See if there'sa colon in the address
    CSStr pos = std::strchr(addr, ':');
    // Should we look for a port number?
    if (pos != nullptr)
    {
        // Assign the address portion
        m_Host.assign(addr, pos - addr);
        // Attempt to extract the numeric value
        const Ulong num = std::strtoul(++pos, nullptr, 10);
        // Is the port number withing range?
        if (num > 0xFFFF)
        {
            STHROWF("Port number out of range: %lu", num);
        }
        // Assign the extracted port number
        m_Port = ConvTo< Uint16 >::From(num);
    }
    // Assign the address as is
    else
    {
        m_Host.assign(addr);
    }
}

// ------------------------------------------------------------------------------------------------
void Account::SetPortNum(SQInteger port)
{
    // Validate the specified port number
    if (port >= 0xFFFF)
    {
        STHROWF("Port number out of range: " _PRINT_INT_FMT, port);
    }
    // Assign the specified port number
    m_Port = ConvTo< Uint16 >::From(port);
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
Table Account::GetOptionsTable() const
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
        STHROWF("Invalid or empty option name");
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
        STHROWF("Invalid or empty option name");
    }
    // Assign the specified value
    m_Options[name] = value ? value : _SC("");
}

// ------------------------------------------------------------------------------------------------
void Account::RemoveOption(CSStr name)
{
    // Make sure the specified name is valid
    if (!name || *name == '\0')
    {
        STHROWF("Invalid or empty option name");
    }
    // Erase the specified value
    m_Options.erase(name);
}

// ------------------------------------------------------------------------------------------------
Connection Account::Connect() const
{
    return Connection(*this);
}

} // Namespace:: SqMod
