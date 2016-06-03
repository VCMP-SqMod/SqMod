// -----------------------------------------------------------------------------------------------
#include "Manager.hpp"
#include "Connection.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstring>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Manager::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMgManager");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Int32 Manager::Cmp(const Manager & o) const
{
    if (m_Handle == o.m_Handle)
    {
        return 0;
    }
    else if (m_Handle.HndPtr() > o.m_Handle.HndPtr())
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
Connection Manager::Bind(CSStr addr) const
{
    return Bind(addr, 0);
}

// ------------------------------------------------------------------------------------------------
Connection Manager::Bind(CSStr addr, Uint32 flags) const
{
    // Validate the managed handle
    m_Handle.Validate();
    // Validate the specified address
    if (!addr)
    {
        STHROWF("Invalid bind address: null");
    }
    // Create a binding options structure
    mg_bind_opts opts;
    // Initialize the structure to 0
    std::memset(&opts, 0, sizeof(mg_bind_opts));
    // Assign the specified flags
    opts.flags = flags;
    // Attemmt to create the connection handle
    mg_connection * connection = mg_bind_opt(m_Handle, addr, ConnectionHnd::EvFwd, opts);
    // Validate the obtained connection handle
    if (!connection)
    {
        STHROWF("Unable to create listening connection [%s]", *opts.error_string);
    }
    // Return the obtained connection
    return Connection(ConnectionHnd(connection));
}

// ------------------------------------------------------------------------------------------------
Connection Manager::Connect(CSStr addr) const
{
    return Connect(addr, 0);
}

// ------------------------------------------------------------------------------------------------
Connection Manager::Connect(CSStr addr, Uint32 flags) const
{
    // Validate the managed handle
    m_Handle.Validate();
    // Validate the specified address
    if (!addr)
    {
        STHROWF("Invalid bind address: null");
    }
    // Create a binding options structure
    mg_connect_opts opts;
    // Initialize the structure to 0
    std::memset(&opts, 0, sizeof(mg_connect_opts));
    // Assign the specified flags
    opts.flags = flags;
    // Attemmt to create the connection handle
    mg_connection * connection = mg_connect_opt(m_Handle, addr, ConnectionHnd::EvFwd, opts);
    // Validate the obtained connection handle
    if (!connection)
    {
        STHROWF("Unable to create listening connection [%s]", *opts.error_string);
    }
    // Return the obtained connection
    return Connection(ConnectionHnd(connection));
}

// ------------------------------------------------------------------------------------------------
Connection Manager::ConnectWS(CSStr url) const
{
    return ConnectWS(url, nullptr, nullptr, 0);
}

// ------------------------------------------------------------------------------------------------
Connection Manager::ConnectWS(CSStr url, CSStr protocol) const
{
    return ConnectWS(url, protocol, nullptr, 0);
}

// ------------------------------------------------------------------------------------------------
Connection Manager::ConnectWS(CSStr url, CSStr protocol, CSStr headers) const
{
    return ConnectWS(url, protocol, headers, 0);
}

// ------------------------------------------------------------------------------------------------
Connection Manager::ConnectWS(CSStr url, CSStr protocol, CSStr headers, Uint32 flags) const
{
    return Connection();
}

// ------------------------------------------------------------------------------------------------
Connection Manager::ConnectHTTP(CSStr url) const
{
    return ConnectHTTP(url, nullptr, nullptr, 0);
}

// ------------------------------------------------------------------------------------------------
Connection Manager::ConnectHTTP(CSStr url, CSStr headers) const
{
    return ConnectHTTP(url, headers, nullptr, 0);
}

// ------------------------------------------------------------------------------------------------
Connection Manager::ConnectHTTP(CSStr url, CSStr headers, CSStr post) const
{
    return ConnectHTTP(url, headers, post, 0);
}

// ------------------------------------------------------------------------------------------------
Connection Manager::ConnectHTTP(CSStr url, CSStr headers, CSStr post, Uint32 flags) const
{
    return Connection();
}

} // Namespace:: SqMod
