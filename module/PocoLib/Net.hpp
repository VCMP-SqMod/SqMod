#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"
#include "Library/IO/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>

// ------------------------------------------------------------------------------------------------
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPMessage.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/WebSocket.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Implements a WebSocket according to the WebSocket protocol specification in RFC 6455.
*/
struct WsClient
{
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
    Poco::Net::WebSocket mWebSocket;

    /* --------------------------------------------------------------------------------------------
     * Receiving buffer instance.
    */
    Poco::Buffer< char > mBuffer;

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
     * Base constructor.
    */
    WsClient(StackStrF &host, uint16_t port, StackStrF &uri)
            : mClient(host.ToStr(), port),
              mRequest(Poco::Net::HTTPRequest::HTTP_GET, uri.ToStr(), Poco::Net::HTTPRequest::HTTP_1_1), mResponse(),
              mWebSocket(mClient, mRequest, mResponse), mBuffer(0), mFlags(0), mState(0)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Sends a Close control frame to the server end of the connection to initiate an orderly shutdown of the connection.
    */
    void Shutdown() {
        mWebSocket.shutdown();
    }

    /* --------------------------------------------------------------------------------------------
     * Sends a Close control frame to the server end of the connection to initiate an orderly shutdown of the connection.
    */
    void ShutdownWith(SQInteger code, StackStrF &msg) {
        mWebSocket.shutdown(static_cast< uint16_t >(code), msg.ToStr());
    }

    /* --------------------------------------------------------------------------------------------
     * Sends the contents of the given buffer through the socket as a single frame.
     * Returns the number of bytes sent, which may be less than the number of bytes specified.
    */
    SQInteger SendFrame(SqBuffer & buf, SQInteger flags) {
        return mWebSocket.sendFrame(buf.Valid().Data(), static_cast< int >(buf.Valid().Position()), static_cast< int >(flags));
    }

    /* --------------------------------------------------------------------------------------------
     * Sends the contents of the given string through the socket as a single frame.
     * Returns the number of bytes sent, which may be less than the number of bytes specified.
    */
    SQInteger SendStringFrame(SQInteger flags, StackStrF &str) {
        return mWebSocket.sendFrame(str.mPtr, static_cast< int >(str.mLen), static_cast< int >(flags));
    }

    /* --------------------------------------------------------------------------------------------
     * Receives a frame from the socket and return it as a buffer.
     * The frame's payload size must not exceed the maximum payload size set with SetMaxPayloadSize().
    */
    LightObj RecvFrame()
    {
        // Attempt to receive data
        mState = mWebSocket.receiveFrame(mBuffer, mFlags);
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
        mState = mWebSocket.receiveFrame(mBuffer, mFlags);
        // If something was returned
        if (mState != 0)
        {
            // Create a string with buffer contents
            LightObj obj(SqTypeIdentity< SqBuffer >{}, SqVM(),
                            const_cast< const SQChar * >(mBuffer.begin()),
                            static_cast< SQInteger >(mBuffer.sizeBytes()));
            // Discard buffer contents for next request
            mBuffer.resize(0);
            // Return the string object
            return obj;
        }
        // Default to null
        return LightObj{};
    }

    /* --------------------------------------------------------------------------------------------
     * Sets the maximum payload size for RecvFrame(). The default is std::numeric_limits<int>::max().
    */
    WsClient & SetMaxPayloadSize(SQInteger size)
    {
        mWebSocket.setMaxPayloadSize(static_cast< int >(size));
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the maximum payload size for RecvFrame(). The default is std::numeric_limits<int>::max().
    */
    SQMOD_NODISCARD SQInteger GetMaxPayloadSize() const
    {
        return mWebSocket.getMaxPayloadSize();
    }
};

} // Namespace:: SqMod
