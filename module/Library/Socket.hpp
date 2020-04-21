#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"
#include "Library/Utils/Buffer.hpp"
#include "Library/Numeric/LongInt.hpp"

// ------------------------------------------------------------------------------------------------
#include <PassiveSocket.h>
#include <ActiveSocket.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

using namespace moodycamel;

/* ------------------------------------------------------------------------------------------------
 * Handle validation.
*/
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    #define SQMOD_VALIDATE(x)               (x).Validate(__FILE__, __LINE__)
#else
    #define SQMOD_VALIDATE(x)               (x).Validate()
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
using CSimpleSocketPtr = SharedPtr< CSimpleSocket >;
using CSimpleSocketRef = WeakPtr< CSimpleSocket >;

/* ------------------------------------------------------------------------------------------------
 * Provides a platform independent class to for socket development. This class is designed
 * to abstract socket communication development in a platform independent manner.
*/
class SimpleSocket
{
protected:
    /* --------------------------------------------------------------------------------------------
     * The managed socket instance.
    */
    CSimpleSocket * m_Socket;
    /* --------------------------------------------------------------------------------------------
     * User tag associated with this instance.
    */
    String      m_Tag;
    /* --------------------------------------------------------------------------------------------
     * User data associated with this instance.
    */
    LightObj    m_Data;
    /* --------------------------------------------------------------------------------------------
     * Default constructor. Initializes everything to null.
    */
    SimpleSocket()
        : m_Socket(nullptr)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor. Initializes with the given socket instance.
    */
    explicit SimpleSocket(CSimpleSocket * socket)
        : m_Socket(socket)
    {
        if (!socket)
        {
            STHROWF("Invalid socket instance.");
        }
    }
    /* --------------------------------------------------------------------------------------------
     * Destructor constructor.
    */
    virtual ~SimpleSocket()
    {
        // Close the socket
        if (m_Socket && m_Socket->IsSocketValid())
        {
            m_Socket->Close();
        }
        // Delete the managed socket (`delete` checks for NULL)
        delete m_Socket;
    }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG
public:
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    SimpleSocket(const SimpleSocket &) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    SimpleSocket(SimpleSocket &&) = delete;
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    SimpleSocket & operator = (const SimpleSocket &) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    SimpleSocket & operator = (SimpleSocket &&) = delete;
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user tag.
    */
    const String & GetTag() const
    {
        return m_Tag;
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    void SetTag(StackStrF & tag)
    {
        if (tag.mLen > 0)
        {
            m_Tag.assign(tag.mPtr, static_cast< size_t >(tag.mLen));
        }
        else
        {
            m_Tag.clear();
        }
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    SimpleSocket & ApplyTag(StackStrF & tag)
    {
        SetTag(tag);
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user data.
    */
    LightObj & GetData()
    {
        return m_Data;
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(LightObj & data)
    {
        m_Data = data;
    }
    /* --------------------------------------------------------------------------------------------
     * Initialize instance of CSocket.  This method MUST be called before an object can be used.
    */
    bool Initialize()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->Initialize();
    }
    /* --------------------------------------------------------------------------------------------
     * Close the managed socket.
    */
    bool Close()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->Close();
    }
    /* --------------------------------------------------------------------------------------------
     * Shutdown the managed socket with various send and receive operations.
    */
    bool Shutdown(SQInteger mode)
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->Shutdown(static_cast< CSimpleSocket::CShutdownMode >(mode));
    }
    /* --------------------------------------------------------------------------------------------
     * Examine the socket descriptor sets currently owned by the instance of the socket class
     * (the readfds, writefds, and errorfds parameters) to see whether some of their descriptors
     * are ready for reading, are ready for writing, or have an exceptional condition pending, respectively.
     * Block until an event happens on the specified file descriptors.
    */
    bool Select()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->Select();
    }
    /* --------------------------------------------------------------------------------------------
     * Examine the socket descriptor sets currently owned by the instance of the socket class
     * (the readfds, writefds, and errorfds parameters) to see whether some of their descriptors
     * are ready for reading, are ready for writing, or have an exceptional condition pending, respectively.
    */
    bool SelectEx(SQInteger timeout_sec, SQInteger timeout_usec)
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->Select(static_cast< Int32 >(timeout_sec), static_cast< Int32 >(timeout_usec));
    }
    /* --------------------------------------------------------------------------------------------
     * Does the current instance of the socket object contain a valid socket descriptor.
    */
    bool IsSocketValid()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->IsSocketValid();
    }
    /* --------------------------------------------------------------------------------------------
     * Provides a standard error code for cross platform development by  mapping the
     * operating system error to an error defined by the CSocket class.
    */
    void TranslateSocketError()
    {
        SQMOD_VALIDATE(*this);
        m_Socket->TranslateSocketError();
    }
    /* --------------------------------------------------------------------------------------------
     * Returns a human-readable description of the given error code or the last error code of a socket.
    */
    LightObj DescribeError()
    {
        SQMOD_VALIDATE(*this);
        return LightObj(m_Socket->DescribeError(), -1);
    }
    /* --------------------------------------------------------------------------------------------
     * Returns a human-readable description of the given error code or the last error code of a socket.
    */
    static LightObj DescribeErrorCode(SQInteger err)
    {
        return LightObj(CSimpleSocket::DescribeError(static_cast< CSimpleSocket::CSocketError >(err)), -1);
    }
    /* --------------------------------------------------------------------------------------------
     * Attempts to receive a block of data on an established connection. Buffer cursor is not affected!
     * This is potentially broken because by the time a job is processed, another one could have override internal buffer.
    */
    SQInteger Receive(Buffer::SzType max_bytes)
    {
        SQMOD_VALIDATE(*this);
        // Now the call can be forwarded
        return m_Socket->Receive(max_bytes, nullptr);
    }
    /* --------------------------------------------------------------------------------------------
     * Attempts to receive a block of data on an established connection. Buffer cursor is not affected!
    */
    SQInteger ReceiveInto(Buffer::SzType max_bytes, SqBuffer & out)
    {
        SQMOD_VALIDATE(*this);
        // Make sure there is enough space in the buffer
        if (max_bytes > out.GetRef()->Capacity())
        {
            out.GetRef()->Adjust(max_bytes); // We don't keep old data!
        }
        // Now the call can be forwarded
        return m_Socket->Receive(max_bytes, out.GetRef()->Get< Uint8 >());
    }
    /* --------------------------------------------------------------------------------------------
     * Attempts to send a block of data on an established connection.. Buffer cursor is not affected!
    */
    SQInteger Send(SqBuffer & in, Buffer::SzType bytes_to_send)
    {
        SQMOD_VALIDATE(*this);
        // Make sure there is enough data in the buffer
        if (bytes_to_send > in.GetRef()->Capacity())
        {
            STHROWF("Not enough data in the buffer");
        }
        // Now the call can be forwarded
        return m_Socket->Send(in.GetRef()->Get< Uint8 >(), bytes_to_send);
    }
    /* --------------------------------------------------------------------------------------------
     * Copies data between one file descriptor and another.
    */
    SQInteger SendFile(Int32 out_fd, Int32 in_fd, SLongInt & offset, Int32 count)
    {
        SQMOD_VALIDATE(*this);
        // Create a native offset
        off_t native_offset = ConvTo< off_t >::From(offset.GetNum());
        // Forward the call and save the result
        auto r = m_Socket->SendFile(out_fd, in_fd, &native_offset, count);
        // Translate the native offset back to the wrapper
        offset.SetNum(native_offset);
        // Return back the result
        return r;
    }
    /* --------------------------------------------------------------------------------------------
     * Returns blocking/non-blocking state of socket.
    */
    bool IsNonBlocking()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->IsNonblocking();
    }
    /* --------------------------------------------------------------------------------------------
     * Set the socket to blocking.
    */
    bool SetBlocking()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->SetBlocking();
    }
    /* --------------------------------------------------------------------------------------------
     * Set the socket as non-blocking.
    */
    bool SetNonBlocking()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->SetNonblocking();
    }
    /* --------------------------------------------------------------------------------------------
     * Returns a copy of the internal buffer used to receive data. Very ineffective. Use carefully.
    */
    SqBuffer GetInternalData()
    {
        SQMOD_VALIDATE(*this);
        return SqBuffer(reinterpret_cast< Buffer::ConstPtr >(m_Socket->GetData()),
                        static_cast< Buffer::SzType >(m_Socket->GetBytesReceived()), 0);
    }
    /* --------------------------------------------------------------------------------------------
     * Returns the number of bytes received on the last call to SimpleSocket::Receive().
    */
    SQInteger GetBytesReceived()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->GetBytesReceived();
    }
    /* --------------------------------------------------------------------------------------------
     * Returns the number of bytes sent on the last call to SimpleSocket::Send().
    */
    SQInteger GetBytesSent()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->GetBytesSent();
    }
    /* --------------------------------------------------------------------------------------------
     * Controls the actions taken when CSimpleSocket::Close is executed on a socket object that has unsent data.
    */
    bool SetOptionLinger(bool toggle, SQInteger time)
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->SetOptionLinger(toggle, ConvTo< Uint16 >::From(time));
    }
    /* --------------------------------------------------------------------------------------------
     * Tells the kernel that even if this port is busy (in the TIME_WAIT state), go ahead and reuse it anyway.
    */
    bool SetOptionReuseAddr()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->SetOptionReuseAddr();
    }
    /* --------------------------------------------------------------------------------------------
     * Gets the timeout value that specifies the maximum number of seconds a call to
     * SimpleSocket::Open waits until it completes.
    */
    SQInteger GetConnectTimeoutSec()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->GetConnectTimeoutSec();
    }
    /* --------------------------------------------------------------------------------------------
     * Gets the timeout value that specifies the maximum number of microseconds a call to
     * SimpleSocket::Open waits until it completes.
    */
    SQInteger GetConnectTimeoutUSec()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->GetConnectTimeoutUSec(); 
    }
    /* --------------------------------------------------------------------------------------------
     * Sets the timeout value that specifies the maximum amount of time a call to
     * SimpleSocket::Receive waits until it completes.
    */
    void SetConnectTimeout(Int32 timeout_sec, Int32 timeout_usec)
    {
        SQMOD_VALIDATE(*this);
        m_Socket->SetConnectTimeout(timeout_sec, timeout_usec); 
    }
    /* --------------------------------------------------------------------------------------------
     * Gets the timeout value that specifies the maximum number of seconds a a call to
     * SimpleSocket::Receive waits until it completes.
    */
    SQInteger GetReceiveTimeoutSec()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->GetReceiveTimeoutSec(); 
    }
    /* --------------------------------------------------------------------------------------------
     * Gets the timeout value that specifies the maximum number of microseconds a call to
     * SimpleSocket::Receive waits until it completes.
    */
    SQInteger GetReceiveTimeoutUSec()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->GetReceiveTimeoutUSec(); 
    }
    /* --------------------------------------------------------------------------------------------
     * Sets the timeout value that specifies the maximum amount of time a call to
     * SimpleSocket::Receive waits until it completes. 
    */
    bool SetReceiveTimeout(Int32 timeout_sec, Int32 timeout_usec)
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->SetReceiveTimeout(timeout_sec, timeout_usec); 
    }
    /* --------------------------------------------------------------------------------------------
     * Enable/disable multi-cast for a socket.  This options is only valid for socket descriptors
     * of type SimpleSocket::SocketTypeUdp.
    */
    void SetMulticast(bool toggle)
    {
        SQMOD_VALIDATE(*this);
        if (!m_Socket->SetMulticast(toggle, 1)) STHROWF("Unable to set multi-cast on socket."); 
    }
    /* --------------------------------------------------------------------------------------------
     * Enable/disable multi-cast for a socket.  This options is only valid for socket descriptors
     * of type SimpleSocket::SocketTypeUdp.
    */
    bool SetMulticastEx(bool toggle, Uint8 multicast_ttl)
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->SetMulticast(toggle, multicast_ttl); 
    }
    /* --------------------------------------------------------------------------------------------
     * Return true if socket is multi-cast or false is socket is uni-cast.
    */
    bool GetMulticast()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->GetMulticast(); 
    }
    /* --------------------------------------------------------------------------------------------
     * Bind socket to a specific interface when using multi-cast.
    */
    bool BindInterface(StackStrF & intrf)
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->BindInterface(intrf.mPtr); 
    }
    /* --------------------------------------------------------------------------------------------
     * Gets the timeout value that specifies the maximum number of seconds a a call to
     * SimpleSocket::Send waits until it completes.
    */
    SQInteger GetSendTimeoutSec()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->GetSendTimeoutSec(); 
    }
    /* --------------------------------------------------------------------------------------------
     * Gets the timeout value that specifies the maximum number of microseconds a call to
     * SimpleSocket::Send waits until it completes.
    */
    SQInteger GetSendTimeoutUSec()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->GetSendTimeoutUSec(); 
    }
    /* --------------------------------------------------------------------------------------------
     * Gets the timeout value that specifies the maximum amount of time a call to
     * SimpleSocket::Send waits until it completes.
    */
    bool SetSendTimeout(Int32 timeout_sec, Int32 timeout_usec)
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->SetSendTimeout(timeout_sec, timeout_usec);
    }
    /* --------------------------------------------------------------------------------------------
     * Returns the last error that occured for the instace of the SimpleSocket instance.
     * This method should be called immediately to retrieve the error code for the failing mehtod call.
    */
    SQInteger GetSocketError()
    {
        SQMOD_VALIDATE(*this);
        return static_cast< SQInteger >(m_Socket->GetSocketError()); 
    }
    /* --------------------------------------------------------------------------------------------
     * Get the total time the of the last operation in milliseconds.
    */
    SQInteger GetTotalTimeMs()
    {
        SQMOD_VALIDATE(*this);
        return static_cast< SQInteger >(m_Socket->GetTotalTimeMs()); 
    }
    /* --------------------------------------------------------------------------------------------
     * Get the total time the of the last operation in microseconds.
    */
    SQInteger GetTotalTimeUsec()
    {
        SQMOD_VALIDATE(*this);
        return static_cast< SQInteger >(m_Socket->GetTotalTimeUsec()); 
    }
    /* --------------------------------------------------------------------------------------------
     * Return Differentiated Services Code Point (DSCP) value currently set on the socket object.
    */
    Int32 GetSocketDSCP()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->GetSocketDscp(); 
    }
    /* --------------------------------------------------------------------------------------------
     * Set Differentiated Services Code Point (DSCP) for socket object.
    */
    void SetSocketDSCP(Int32 dscp)
    {
        SQMOD_VALIDATE(*this);
        if (!m_Socket->SetSocketDscp(dscp)) STHROWF("Unable to set DSCP on socket."); 
    }
    /* --------------------------------------------------------------------------------------------
     * Set Differentiated Services Code Point (DSCP) for socket object.
    */
    bool SetSocketDSCPEx(Int32 dscp)
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->SetSocketDscp(dscp); 
    }
    /* --------------------------------------------------------------------------------------------
     * Return socket type.
    */
    SQInteger GetSocketType()
    {
        SQMOD_VALIDATE(*this);
        return static_cast< SQInteger >(m_Socket->GetSocketType()); 
    }
    /* --------------------------------------------------------------------------------------------
     * Returns clients Internet host address as a string in standard numbers-and-dots notation.
    */
    LightObj GetClientAddr()
    {
        SQMOD_VALIDATE(*this);
        return LightObj(m_Socket->GetClientAddr(), -1); 
    }
    /* --------------------------------------------------------------------------------------------
     * Returns the port number on which the client is connected.
    */
    SQInteger GetClientPort()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->GetClientPort(); 
    }
    /* --------------------------------------------------------------------------------------------
     * Returns server Internet host address as a string in standard numbers-and-dots notation.
    */
    LightObj GetServerAddr()
    {
        SQMOD_VALIDATE(*this);
        return LightObj(m_Socket->GetServerAddr(), -1); 
    }
    /* --------------------------------------------------------------------------------------------
     * Returns the port number on which the server is connected.
    */
    SQInteger GetServerPort()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->GetServerPort(); 
    }
    /* --------------------------------------------------------------------------------------------
     * Get the TCP receive buffer window size for the current socket object.
    */
    SQInteger GetReceiveWindowSize()
    {
        SQMOD_VALIDATE(*this);
        return static_cast< SQInteger >(m_Socket->GetReceiveWindowSize()); 
    }
    /* --------------------------------------------------------------------------------------------
     * Get the TCP send buffer window size for the current socket object.
    */
    SQInteger GetSendWindowSize()
    {
        SQMOD_VALIDATE(*this);
        return static_cast< SQInteger >(m_Socket->GetSendWindowSize()); 
    }
    /* --------------------------------------------------------------------------------------------
     * Set the TCP receive buffer window size for the current socket object.
    */
    SQInteger SetReceiveWindowSize(SQInteger window_size)
    {
        SQMOD_VALIDATE(*this);
        return static_cast< SQInteger >(m_Socket->SetReceiveWindowSize(ConvTo< Uint32 >::From(window_size))); 
    }
    /* --------------------------------------------------------------------------------------------
     * Set the TCP send buffer window size for the current socket object.
    */
    SQInteger SetSendWindowSize(SQInteger window_size)
    {
        SQMOD_VALIDATE(*this);
        return static_cast< SQInteger >(m_Socket->SetSendWindowSize(ConvTo< Uint32 >::From(window_size))); 
    }
    /* --------------------------------------------------------------------------------------------
     * Disable the Nagle algorithm (Set TCP_NODELAY to true)
    */
    bool DisableNagleAlgoritm()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->DisableNagleAlgoritm(); 
    }
    /* --------------------------------------------------------------------------------------------
     * Enable the Nagle algorithm (Set TCP_NODELAY to false)
    */
    bool EnableNagleAlgoritm()
    {
        SQMOD_VALIDATE(*this);
        return m_Socket->EnableNagleAlgoritm(); 
    }
};

/* ------------------------------------------------------------------------------------------------
 * Provides a platform independent class to create a passive socket. A passive socket is used to
 * create a "listening" socket. This type of object would be used when an application needs to
 * wait for inbound connections.
*/
class PassiveSocket : public SimpleSocket
{
public:
    /* --------------------------------------------------------------------------------------------
     * Default constructor. Defaults to creating a TCP socket.
    */
    PassiveSocket()
        : PassiveSocket(CSimpleSocket::SocketTypeTcp)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor. Creates a socket of the specified type.
    */
    explicit PassiveSocket(SQInteger type)
        : SimpleSocket(new CPassiveSocket(static_cast< CSimpleSocket::CSocketType >(type)))
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor. Creates a socket with the specified instance.
    */
    explicit PassiveSocket(CPassiveSocket * socket)
        : SimpleSocket(socket)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the managed socket to the type that we expect.
    */
    CPassiveSocket * Get()
    {
        return dynamic_cast< CPassiveSocket * >(m_Socket);
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the managed socket to the type that we expect.
    */
    const CPassiveSocket * Get() const
    {
        return dynamic_cast< CPassiveSocket * >(m_Socket);
    }
    /* --------------------------------------------------------------------------------------------
     * Return a string representation of this instance.
    */
    LightObj ToString()
    {
        return m_Socket ? LightObj{} : GetServerAddr();
    }
    /* --------------------------------------------------------------------------------------------
     * Extracts the first connection request on the queue of pending connections and creates a newly connected socket.
    */
    LightObj Accept();
    /* --------------------------------------------------------------------------------------------
     * Bind to a multi-cast group on  a specified interface, multi-cast group, and port.
    */
    bool BindMulticast(StackStrF & intrf, StackStrF & group, Uint16 port)
    {
        SQMOD_VALIDATE(*this);
        return Get()->BindMulticast(intrf.mPtr, group.mPtr, port);
    }
    /* --------------------------------------------------------------------------------------------
     * Create a listening socket at local IP address 'x.x.x.x' or 'localhost' if addr is NULL on port port.
    */
    bool Listen(Uint16 port, StackStrF & addr)
    {
        SQMOD_VALIDATE(*this);
        return Get()->Listen(addr.mPtr, port);
    }
    /* --------------------------------------------------------------------------------------------
     * Create a listening socket at local IP address 'x.x.x.x' or 'localhost' if addr is NULL on port port.
    */
    bool ListenEx(Uint16 port, Int32 connection_backlog, StackStrF & addr)
    {
        SQMOD_VALIDATE(*this);
        return Get()->Listen(addr.mPtr, port, connection_backlog);
    }
    /* --------------------------------------------------------------------------------------------
     * Attempts to send a string on an established connection.
    */
    Int32 SendStr(StackStrF & str)
    {
        SQMOD_VALIDATE(*this);
        return Get()->Send(reinterpret_cast< const uint8_t * >(str.mPtr),
                            static_cast< size_t >(str.mLen) * (sizeof(SQChar) / sizeof(uint8_t)));
    }
};

/* ------------------------------------------------------------------------------------------------
 * Provides a platform independent class to create an active socket. An active socket is used to
 * create a socket which connects to a server. This type of object would be used when
 * an application needs to send/receive data from a server.
*/
class ActiveSocket : public SimpleSocket
{
    friend class PassiveSocket;
public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. Defaults to creating a TCP socket.
    */
    ActiveSocket()
        : ActiveSocket(CSimpleSocket::SocketTypeTcp)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor. Creates a socket of the specified type.
    */
    explicit ActiveSocket(SQInteger type)
        : SimpleSocket(new CActiveSocket(static_cast< CSimpleSocket::CSocketType >(type)))
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor. Creates a socket with the specified instance.
    */
    explicit ActiveSocket(CActiveSocket * socket)
        : SimpleSocket(socket)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the managed socket to the type that we expect.
    */
    CActiveSocket * Get()
    {
        return dynamic_cast< CActiveSocket * >(m_Socket);
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the managed socket to the type that we expect.
    */
    const CActiveSocket * Get() const
    {
        return dynamic_cast< CActiveSocket * >(m_Socket);
    }
    /* --------------------------------------------------------------------------------------------
     * Return a string representation of this instance.
    */
    LightObj ToString()
    {
        return m_Socket ? LightObj{} : GetClientAddr();
    }
    /* --------------------------------------------------------------------------------------------
     * Establish a connection to the address specified by addr.
    */
    bool Open(Uint16 port, StackStrF & addr)
    {
        SQMOD_VALIDATE(*this);
        return Get()->Open(addr.mPtr, port);
    }
    /* --------------------------------------------------------------------------------------------
     * Attempts to send a string on an established connection.
    */
    Int32 SendStr(StackStrF & str)
    {
        SQMOD_VALIDATE(*this);
        return Get()->Send(reinterpret_cast< const uint8_t * >(str.mPtr),
                            static_cast< size_t >(str.mLen) * (sizeof(SQChar) / sizeof(uint8_t)));
    }
};

} // Namespace:: SqMod
