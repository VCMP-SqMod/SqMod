#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"
#include "Library/IO/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>
#include <utility>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
#include <Poco/Thread.h>
#include <Poco/AutoPtr.h>
#include <Poco/Runnable.h>
#include <Poco/Observer.h>
#include <Poco/NObserver.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPMessage.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/SocketAcceptor.h>
#include <Poco/Net/SocketReactor.h>
#include <Poco/Net/WebSocket.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Implements a WebSocket according to the WebSocket protocol specification in RFC 6455.
*/
struct WsClient
{

    /* --------------------------------------------------------------------------------------------
     * Flags received in the last call to Recv[String]Frame() (will be overwritten on next call).
    */
    int mFlags{0};

    /* --------------------------------------------------------------------------------------------
     * Return value from the last call to Recv[String]Frame() (will be overwritten on next call).
     * A return value of 0, with flags also 0, means that the peer has shut down or closed the connection.
     * A return value of 0, with non-zero flags, indicates an reception of an empty frame (e.g., in case of a PING).
    */
    int mState{0};

    /* --------------------------------------------------------------------------------------------
     * Receiving buffer instance.
    */
    Poco::Buffer< char > mBuffer;

    /* --------------------------------------------------------------------------------------------
     * User tag associated with this instance.
    */
    String mTag;

    /* --------------------------------------------------------------------------------------------
     * User data associated with this instance.
    */
    LightObj mData;

    /* --------------------------------------------------------------------------------------------
     * HTTP client session instance.
    */
    Poco::Net::HTTPClientSession mClient;

    /* --------------------------------------------------------------------------------------------
     * HTTP request instance.
    */
    Poco::Net::HTTPRequest mRequest;

    /* --------------------------------------------------------------------------------------------
     * HTTP response instance.
    */
    Poco::Net::HTTPResponse mResponse;

    /* --------------------------------------------------------------------------------------------
     * WebSocket instance.
    */
    Poco::Net::WebSocket mSocket;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    WsClient(StackStrF & host, uint16_t port, StackStrF & uri)
        : mFlags(0), mState(0), mBuffer(0), mTag(), mData()
        , mClient(host.ToStr(), port)
        , mRequest(Poco::Net::HTTPRequest::HTTP_GET, uri.ToStr(), Poco::Net::HTTPRequest::HTTP_1_1)
        , mResponse()
        , mSocket(mClient, mRequest, mResponse)
    {
        mSocket.setBlocking(false); // Disable blocking
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user tag.
    */
    SQMOD_NODISCARD const String & GetTag() const
    {
        return mTag;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    void SetTag(StackStrF & tag)
    {
        if (tag.mLen > 0)
        {
            mTag.assign(tag.mPtr, static_cast< size_t >(tag.mLen));
        }
        else
        {
            mTag.clear();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    WsClient & ApplyTag(StackStrF & tag)
    {
        SetTag(tag);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user data.
    */
    SQMOD_NODISCARD LightObj & GetData()
    {
        return mData;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(LightObj & data)
    {
        mData = data;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    WsClient & ApplyData(LightObj & data)
    {
        mData = data;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Sends a Close control frame to the server end of the connection to initiate an orderly shutdown of the connection.
    */
    void Shutdown()
    {
        mSocket.shutdown();
    }

    /* --------------------------------------------------------------------------------------------
     * Sends a Close control frame to the server end of the connection to initiate an orderly shutdown of the connection.
    */
    void ShutdownWith(SQInteger code, StackStrF & msg)
    {
        mSocket.shutdown(static_cast< uint16_t >(code), msg.ToStr());
    }

    /* --------------------------------------------------------------------------------------------
     * Sends the contents of the given buffer through the socket as a single frame.
     * Returns the number of bytes sent, which may be less than the number of bytes specified.
    */
    SQInteger SendFrame(SqBuffer & buf, SQInteger flags)
    {
        return mSocket.sendFrame(buf.Valid().Data(), static_cast< int >(buf.Valid().Position()), static_cast< int >(flags));
    }

    /* --------------------------------------------------------------------------------------------
     * Sends the contents of the given string through the socket as a single frame.
     * Returns the number of bytes sent, which may be less than the number of bytes specified.
    */
    SQInteger SendStringFrame(SQInteger flags, StackStrF & str)
    {
        return mSocket.sendFrame(str.mPtr, static_cast< int >(str.mLen), static_cast< int >(flags));
    }

    /* --------------------------------------------------------------------------------------------
     * Receives a frame from the socket and return it as a buffer.
     * The frame's payload size must not exceed the maximum payload size set with SetMaxPayloadSize().
    */
    LightObj RecvFrame()
    {
        // Attempt to receive data
        try {
            mState = mSocket.receiveFrame(mBuffer, mFlags);
        } catch (const Poco::TimeoutException &) {
            mState = mFlags = 0; // Make sure these don't indicate otherwise
            return LightObj{}; // We handle timeout so we can be non blocking
        }
        // If something was returned
        if (mState != 0)
        {
            // Fetch buffer information
            const auto cap = static_cast< Buffer::SzType >(mBuffer.capacityBytes());
            const auto len = static_cast< Buffer::SzType >(mBuffer.sizeBytes());
            // Steal buffer memory
            Buffer::Pointer ptr = mBuffer.steal();
            // Construct our buffer
            Buffer b(ptr, cap, len, Buffer::OwnIt{});
            // Transform it into a script object and return it
            return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(), std::move(b));
        }
        // Default to null
        return LightObj{};
    }

    /* --------------------------------------------------------------------------------------------
     * Receives a frame from the socket and return it as a string.
     * The frame's payload size must not exceed the maximum payload size set with SetMaxPayloadSize().
    */
    LightObj RecvStringFrame()
    {
        // Attempt to receive data
        try {
            mState = mSocket.receiveFrame(mBuffer, mFlags);
        } catch (const Poco::TimeoutException &) {
            mState = mFlags = 0; // Make sure these don't indicate otherwise
            return LightObj{}; // We handle timeout so we can be non blocking
        }
        // If something was returned
        if (mState != 0)
        {
            // Create a string with buffer contents
            LightObj obj(const_cast< const SQChar * >(mBuffer.begin()), static_cast< SQInteger >(mBuffer.sizeBytes()), SqVM());
            // Discard buffer contents for next request
            mBuffer.resize(0);
            // Return the string object
            return obj;
        }
        // Default to null
        return LightObj{};
    }

    /* --------------------------------------------------------------------------------------------
     * Receives a frame from the socket and return it as a buffer. Only invokes callback if response is valid.
     * The frame's payload size must not exceed the maximum payload size set with SetMaxPayloadSize().
    */
    SQInteger RecvFrameIn(Function & cb)
    {
        auto obj = RecvFrame();
        // Only invoke the callback if we have a valid response
        if (mState != 0 || mFlags != 0)
        {
            cb(obj, mState, mFlags);
        }
        // Return result
        return mState;
    }

    /* --------------------------------------------------------------------------------------------
     * Receives a frame from the socket and return it as a string. Only invokes callback if response is valid.
     * The frame's payload size must not exceed the maximum payload size set with SetMaxPayloadSize().
    */
    SQInteger RecvStringFrameIn(Function & cb)
    {
        auto obj = RecvStringFrame();
        // Only invoke the callback if we have data response
        if (mState != 0 || mFlags != 0)
        {
            cb(obj, mState, mFlags);
        }
        // Return result
        return mState;
    }

    /* --------------------------------------------------------------------------------------------
     * Sets the maximum payload size for RecvFrame(). The default is std::numeric_limits<int>::max().
    */
    WsClient & SetMaxPayloadSize(SQInteger size)
    {
        mSocket.setMaxPayloadSize(static_cast< int >(size));
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the maximum payload size for RecvFrame(). The default is std::numeric_limits<int>::max().
    */
    SQMOD_NODISCARD SQInteger GetMaxPayloadSize() const
    {
        return mSocket.getMaxPayloadSize();
    }
};

} // Namespace:: SqMod
