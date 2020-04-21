// ------------------------------------------------------------------------------------------------
#include "Library/Socket.hpp"
#include "Misc/Signal.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(SimpleSocketTypename, _SC("SimpleSocket"))
SQMODE_DECL_TYPENAME(PassiveSocketTypename, _SC("PassiveSocket"))
SQMODE_DECL_TYPENAME(ActiveSocketTypename, _SC("ActiveSocket"))

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SimpleSocket::Validate(CCStr file, Int32 line) const
{
    if (!m_Socket)
    {
        SqThrowF("Invalid socket reference =>[%s:%d]", file, line);
    }
}
#else
void SimpleSocket::Validate() const
{
    if (!m_Socket)
    {
        SqThrowF("Invalid socket reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
LightObj PassiveSocket::Accept()
{
    SQMOD_VALIDATE(*this);
    // Forward the request and store the returned instance
    CActiveSocket * socket = Get()->Accept();
    // Was there a socket returned?
    if (socket)
    {
        // Return an object with the new socket instance
        return LightObj(SqTypeIdentity< ActiveSocket >{}, SqVM(), socket);
    }
    // Return a null object
    return LightObj{};
}

// ================================================================================================
void Register_Socket(HSQUIRRELVM vm)
{
    Table skns(vm);

    skns.Bind(_SC("Simple"),
        Class< SimpleSocket, NoConstructor< SimpleSocket > >(vm, SimpleSocketTypename::Str)
        // Properties
        .Prop(_SC("Tag"), &SimpleSocket::GetTag, &SimpleSocket::SetTag)
        .Prop(_SC("Data"), &SimpleSocket::GetData, &SimpleSocket::SetData)
        .Prop(_SC("IsValid"), &SimpleSocket::IsSocketValid)
        .Prop(_SC("ErrorDescription"), &SimpleSocket::DescribeError)
        .Prop(_SC("NonBlocking"), &SimpleSocket::IsNonBlocking)
        .Prop(_SC("InternalData"), &SimpleSocket::GetInternalData)
        .Prop(_SC("BytesReceived"), &SimpleSocket::GetBytesReceived)
        .Prop(_SC("BytesSent"), &SimpleSocket::GetBytesSent)
        .Prop(_SC("ConnectTimeoutSec"), &SimpleSocket::GetConnectTimeoutSec)
        .Prop(_SC("ConnectTimeoutUSec"), &SimpleSocket::GetConnectTimeoutUSec)
        .Prop(_SC("ReceiveTimeoutSec"), &SimpleSocket::GetReceiveTimeoutSec)
        .Prop(_SC("ReceiveTimeoutUSec"), &SimpleSocket::GetReceiveTimeoutUSec)
        .Prop(_SC("Multicast"), &SimpleSocket::GetMulticast, &SimpleSocket::SetMulticast)
        .Prop(_SC("SendTimeoutSec"), &SimpleSocket::GetSendTimeoutSec)
        .Prop(_SC("SendTimeoutUSec"), &SimpleSocket::GetSendTimeoutUSec)
        .Prop(_SC("SocketError"), &SimpleSocket::GetSocketError)
        .Prop(_SC("TotalTimeMs"), &SimpleSocket::GetTotalTimeMs)
        .Prop(_SC("TotalTimeUsec"), &SimpleSocket::GetTotalTimeUsec)
        .Prop(_SC("SocketDSCP"), &SimpleSocket::GetSocketDSCP, &SimpleSocket::SetSocketDSCP)
        .Prop(_SC("SocketType"), &SimpleSocket::GetSocketType)
        .Prop(_SC("ClientAddr"), &SimpleSocket::GetClientAddr)
        .Prop(_SC("ClientPort"), &SimpleSocket::GetClientPort)
        .Prop(_SC("ServerAddr"), &SimpleSocket::GetServerAddr)
        .Prop(_SC("ServerPort"), &SimpleSocket::GetServerPort)
        .Prop(_SC("ReceiveWindowSize"), &SimpleSocket::GetReceiveWindowSize, &SimpleSocket::SetReceiveWindowSize)
        .Prop(_SC("SendWindowSize"), &SimpleSocket::GetSendWindowSize, &SimpleSocket::SetSendWindowSize)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SimpleSocketTypename::Fn)
        // Member Methods
        .FmtFunc(_SC("SetTag"), &SimpleSocket::ApplyTag)
        .Func(_SC("Initialize"), &SimpleSocket::Initialize)
        .Func(_SC("Close"), &SimpleSocket::Close)
        .Func(_SC("Shutdown"), &SimpleSocket::Shutdown)
        .Func(_SC("Select"), &SimpleSocket::Select)
        .Func(_SC("SelectEx"), &SimpleSocket::SelectEx)
        .Func(_SC("TranslateSocketError"), &SimpleSocket::TranslateSocketError)
        .Func(_SC("DescribeError"), &SimpleSocket::DescribeError)
        .Func(_SC("Receive"), &SimpleSocket::Receive)
        .Func(_SC("ReceiveInto"), &SimpleSocket::ReceiveInto)
        .Func(_SC("Send"), &SimpleSocket::Send)
        .Func(_SC("SendFile"), &SimpleSocket::SendFile)
        .Func(_SC("SetBlocking"), &SimpleSocket::SetBlocking)
        .Func(_SC("SetNonBlocking"), &SimpleSocket::SetNonBlocking)
        .Func(_SC("SetOptionLinger"), &SimpleSocket::SetOptionLinger)
        .Func(_SC("SetOptionReuseAddr"), &SimpleSocket::SetOptionReuseAddr)
        .Func(_SC("SetConnectTimeout"), &SimpleSocket::SetConnectTimeout)
        .Func(_SC("SetReceiveTimeout"), &SimpleSocket::SetReceiveTimeout)
        .Func(_SC("SetMulticast"), &SimpleSocket::SetMulticastEx)
        .FmtFunc(_SC("BindInterface"), &SimpleSocket::BindInterface)
        .Func(_SC("SetSendTimeout"), &SimpleSocket::SetSendTimeout)
        .Func(_SC("SetSocketDSCP"), &SimpleSocket::SetSocketDSCPEx)
        .Func(_SC("SetReceiveWindowSize"), &SimpleSocket::SetReceiveWindowSize)
        .Func(_SC("SetSendWindowSize"), &SimpleSocket::SetSendWindowSize)
        .Func(_SC("DisableNagleAlgoritm"), &SimpleSocket::DisableNagleAlgoritm)
        .Func(_SC("EnableNagleAlgoritm"), &SimpleSocket::EnableNagleAlgoritm)
        // Static Member Methods
        .StaticFunc(_SC("DescribeErrorCode"), &SimpleSocket::DescribeErrorCode)
    );

    skns.Bind(_SC("Passive"),
        DerivedClass< PassiveSocket, SimpleSocket, NoCopy< PassiveSocket > >(vm, PassiveSocketTypename::Str)
        // Constructors
        .Ctor()
        .Ctor< SQInteger >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &PassiveSocketTypename::Fn)
        .Func(_SC("_tostring"), &PassiveSocket::ToString)
        // Member Methods
        .Func(_SC("Accept"), &PassiveSocket::Accept)
        .Func(_SC("BindMulticast"), &PassiveSocket::BindMulticast)
        .FmtFunc(_SC("Listen"), &PassiveSocket::Listen)
        .FmtFunc(_SC("ListenEx"), &PassiveSocket::ListenEx)
        .FmtFunc(_SC("SendStr"), &PassiveSocket::SendStr)
    );

    skns.Bind(_SC("Active"),
        DerivedClass< ActiveSocket, SimpleSocket, NoCopy< ActiveSocket > >(vm, ActiveSocketTypename::Str)
        // Constructors
        .Ctor()
        .Ctor< SQInteger >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &ActiveSocketTypename::Fn)
        .Func(_SC("_tostring"), &ActiveSocket::ToString)
        // Member Methods
        .Func(_SC("Open"), &ActiveSocket::Open)
        .FmtFunc(_SC("SendStr"), &ActiveSocket::SendStr)
    );

    RootTable(vm).Bind(_SC("SqSocket"), skns);

    ConstTable(vm).Enum(_SC("SqSocketType"), Enumeration(vm)
        .Const(_SC("Invalid"),                  static_cast< SQInteger >(CSimpleSocket::SocketTypeInvalid))
        .Const(_SC("TCP"),                      static_cast< SQInteger >(CSimpleSocket::SocketTypeTcp))
        .Const(_SC("UDP"),                      static_cast< SQInteger >(CSimpleSocket::SocketTypeUdp))
        .Const(_SC("TCP6"),                     static_cast< SQInteger >(CSimpleSocket::SocketTypeTcp6))
        .Const(_SC("UDP6"),                     static_cast< SQInteger >(CSimpleSocket::SocketTypeUdp6))
        .Const(_SC("Raw"),                      static_cast< SQInteger >(CSimpleSocket::SocketTypeRaw))
    );

    ConstTable(vm).Enum(_SC("SqSocketError"), Enumeration(vm)
        .Const(_SC("Error"),                    static_cast< SQInteger >(CSimpleSocket::SocketError))
        .Const(_SC("Success"),                  static_cast< SQInteger >(CSimpleSocket::SocketSuccess))
        .Const(_SC("InvalidSocket"),            static_cast< SQInteger >(CSimpleSocket::SocketInvalidSocket))
        .Const(_SC("InvalidAddress"),           static_cast< SQInteger >(CSimpleSocket::SocketInvalidAddress))
        .Const(_SC("InvalidPort"),              static_cast< SQInteger >(CSimpleSocket::SocketInvalidPort))
        .Const(_SC("ConnectionRefused"),        static_cast< SQInteger >(CSimpleSocket::SocketConnectionRefused))
        .Const(_SC("Timedout"),                 static_cast< SQInteger >(CSimpleSocket::SocketTimedout))
        .Const(_SC("Ewouldblock"),              static_cast< SQInteger >(CSimpleSocket::SocketEwouldblock))
        .Const(_SC("Notconnected"),             static_cast< SQInteger >(CSimpleSocket::SocketNotconnected))
        .Const(_SC("Einprogress"),              static_cast< SQInteger >(CSimpleSocket::SocketEinprogress))
        .Const(_SC("Interrupted"),              static_cast< SQInteger >(CSimpleSocket::SocketInterrupted))
        .Const(_SC("ConnectionAborted"),        static_cast< SQInteger >(CSimpleSocket::SocketConnectionAborted))
        .Const(_SC("ProtocolError"),            static_cast< SQInteger >(CSimpleSocket::SocketProtocolError))
        .Const(_SC("FirewallError"),            static_cast< SQInteger >(CSimpleSocket::SocketFirewallError))
        .Const(_SC("InvalidSocketBuffer"),      static_cast< SQInteger >(CSimpleSocket::SocketInvalidSocketBuffer))
        .Const(_SC("ConnectionReset"),          static_cast< SQInteger >(CSimpleSocket::SocketConnectionReset))
        .Const(_SC("AddressInUse"),             static_cast< SQInteger >(CSimpleSocket::SocketAddressInUse))
        .Const(_SC("InvalidPointer"),           static_cast< SQInteger >(CSimpleSocket::SocketInvalidPointer))
        .Const(_SC("Eunknown"),                 static_cast< SQInteger >(CSimpleSocket::SocketEunknown))
    );

    ConstTable(vm).Enum(_SC("SqSocketShutdownMode"), Enumeration(vm)
        .Const(_SC("Receives"),                 static_cast< SQInteger >(CSimpleSocket::Receives))
        .Const(_SC("Sends"),                    static_cast< SQInteger >(CSimpleSocket::Sends))
        .Const(_SC("Both"),                     static_cast< SQInteger >(CSimpleSocket::Both))
    );
}

} // Namespace:: SqMod
