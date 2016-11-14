// ------------------------------------------------------------------------------------------------
#include "Sockaddr.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger SockAddr::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMSockAddr");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SockAddr::Validate(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid sockaddr structure handle =>[%s:%d]", file, line);
    }
}
#else
void SockAddr::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid sockaddr structure handle");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
SockAddr::Pointer SockAddr::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
SockAddr::Pointer SockAddr::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
SockAddr::SockAddr(CSStr addr)
    : m_Handle(nullptr), m_Addres(_SC(""))
{
    struct addrinfo hints;
    // Configure the hints structure
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_NUMERICHOST;
    // We set ai_socktype so that we only get one result back
    hints.ai_socktype = SOCK_STREAM;
    // Attempt to obtain information about the specified address
    Int32 status = getaddrinfo(addr, nullptr, &hints, &m_Handle);
    // Validate the success of the operation
    if (!status)
    {
        // See if we must free any handles (just in case)
        if (m_Handle)
        {
            freeaddrinfo(m_Handle);
        }
        // Now it's safe to throw the error
        STHROWF("Unable to query the specified address for information [%s]", gai_strerror(status));
    }
    // Save the specified string address
    m_Addres.assign(addr ? addr : _SC(""));
}

// ------------------------------------------------------------------------------------------------
SockAddr::~SockAddr()
{
    if (m_Handle)
    {
        freeaddrinfo(m_Handle);
    }
}

// ================================================================================================
void Register_SockAddr(Table & mmns)
{
    mmns.Bind(_SC("SockAddr"),
        Class< SockAddr, NoCopy< SockAddr > >(mmns.GetVM(), _SC("SqMMSockAddr"))
        // Constructors
        .Ctor()
        .Ctor< CSStr >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SockAddr::Typename)
        .Func(_SC("_tostring"), &SockAddr::ToString)
        // Properties
        .Prop(_SC("IsValid"), &SockAddr::IsValid)
        .Prop(_SC("Address"), &SockAddr::GetAddress)
    );
}

} // Namespace:: SqMod
