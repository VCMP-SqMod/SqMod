// ------------------------------------------------------------------------------------------------
#include "Sockaddr.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger SockAddr::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMDBSockAddr");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
void SockAddr::Validate() const
{
    // Is the document handle valid?
    if (!m_Handle)
        SqThrowF("Invalid sockaddr structure handle");
}

// ------------------------------------------------------------------------------------------------
SockAddr::SockAddr(CSStr addr)
    : m_Handle(NULL), m_Addres(_SC(""))
{
    struct addrinfo hints;
    // Configure the hints structure
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_NUMERICHOST;
    // We set ai_socktype so that we only get one result back
    hints.ai_socktype = SOCK_STREAM;
    // Attempt to obtain information about the specified address
    Int32 status = getaddrinfo(addr, NULL, &hints, &m_Handle);
    // Validate the success of the operation
    if (!status)
    {
        // See if we must free any handles (just in case)
        if (m_Handle)
            freeaddrinfo(m_Handle);
        // Now it's safe to throw the error
        SqThrowF("Unable to query the specified address for information [%s]", gai_strerror(status));
    }
    // Save the specified string address
    m_Addres.assign(addr ? addr : _SC(""));
}

// ------------------------------------------------------------------------------------------------
SockAddr::~SockAddr()
{
    if (m_Handle)
        freeaddrinfo(m_Handle);
}

// ------------------------------------------------------------------------------------------------
Int32 SockAddr::Cmp(const SockAddr & o) const
{
    if (m_Handle == o.m_Handle)
        return 0;
    else if (m_Handle > o.m_Handle)
        return 1;
    else
        return -1;
}


} // Namespace:: SqMod
