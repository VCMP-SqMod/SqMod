#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/IO/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <atomic>

// ------------------------------------------------------------------------------------------------
#include <sqratFunction.h>
#include <concurrentqueue.h>
#include <civetweb.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * WebSocket client implementation.
*/
struct WebSocketClient : public SqChainedInstances< WebSocketClient >
{
    using Base = SqChainedInstances< WebSocketClient >;
    /* --------------------------------------------------------------------------------------------
     * WebSocket frame.
    */
    struct Frame
    {
        /* ----------------------------------------------------------------------------------------
         * Frame data.
        */
        char * mData{nullptr};

        /* ----------------------------------------------------------------------------------------
         * Frame data capacity.
        */
        uint32_t mSize{0};

        /* ----------------------------------------------------------------------------------------
         * Frame flags.
        */
        int mFlags{0};

        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        Frame() = default;

        /* ----------------------------------------------------------------------------------------
         * Explicit constructor.
        */
        Frame(char * data, size_t size, int flags)
            : mData(nullptr), mSize(static_cast< uint32_t >(size)), mFlags(flags)
        {
            // Do we need to allocate a buffer?
            if (mSize != 0)
            {
                // Allocate the memory buffer.
                mData = new char[mSize];
                // Copy the data into the buffer we own
                std::memcpy(mData, data, mSize);
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Copy constructor (disabled).
        */
        Frame(const Frame & o) = delete;

        /* ----------------------------------------------------------------------------------------
         * Move constructor (disabled).
        */
        Frame(Frame && o) noexcept = delete;

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Frame()
        {
            delete[] mData;
        }

        /* ----------------------------------------------------------------------------------------
         * Copy assignment operator (disabled).
        */
        Frame & operator = (const Frame & o) = delete;

        /* ----------------------------------------------------------------------------------------
         * Move assignment operator (disabled).
        */
        Frame & operator = (Frame && o) noexcept = delete;

        /* ----------------------------------------------------------------------------------------
         * Forget about the associated memory buffer.
        */
        void ForgetBuffer() noexcept
        {
            mData = nullptr;
            mSize = 0;
        }
    };

    /* --------------------------------------------------------------------------------------------
     * Smart frame pointer.
    */
    using FramePtr = std::unique_ptr< Frame >;

    /* --------------------------------------------------------------------------------------------
     * Queue of frames written from other threads.
    */
    using FrameQueue = moodycamel::ConcurrentQueue< FramePtr >;

    /* --------------------------------------------------------------------------------------------
     * Connection handle.
    */
    struct mg_connection * mHandle{nullptr};

    /* --------------------------------------------------------------------------------------------
     * Queue of frames that must be processed.
    */
    FrameQueue mQueue{1024};

    /* --------------------------------------------------------------------------------------------
     * Callback to invoke when receiving data.
    */
    Function mOnData{};

    /* --------------------------------------------------------------------------------------------
     * Callback to invoke when the socket is shutting down.
    */
    Function mOnClose{};

    /* --------------------------------------------------------------------------------------------
     * User tag associated with this instance.
    */
    String mTag{};

    /* --------------------------------------------------------------------------------------------
     * User data associated with this instance.
    */
    LightObj mData{};

    /* --------------------------------------------------------------------------------------------
     * Server port.
    */
    int mPort{0};

    /* --------------------------------------------------------------------------------------------
     * Make a secure connection to server.
    */
    bool mSecure{false};

    /* --------------------------------------------------------------------------------------------
     * Whether the connection is currently closing.
    */
    std::atomic< bool > mClosing{false};

    /* --------------------------------------------------------------------------------------------
     * Whether the closing callback was inoked (avoid recursive calls).
    */
    std::atomic< bool > mClosed{false};

    /* --------------------------------------------------------------------------------------------
     * Whether to not keep the connection open after receiving the close event.
     * Internally this event is ignored but if set to true the connection is immediatelly closed
     * in the internal event handler, before the event may reach the script callback.
    */
    std::atomic< bool > mAutoClose{false};

    /* --------------------------------------------------------------------------------------------
     * Server host to connect to, i.e. "echo.websocket.org" or "192.168.1.1" or "localhost".
    */
    String mHost{};

    /* --------------------------------------------------------------------------------------------
     * Server path you are trying to connect to, i.e. if connection to localhost/app, path should be "/app".
    */
    String mPath{};

    /* --------------------------------------------------------------------------------------------
     * Value of the Origin HTTP header.
    */
    String mOrigin{};

    /* --------------------------------------------------------------------------------------------
     * Extensions to include in the connection.
    */
    String mExtensions{};

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    WebSocketClient()
        : Base(), mHandle(nullptr), mQueue(1024), mOnData(), mOnClose(), mTag(), mData()
        , mPort(0), mSecure(false), mClosing(false), mClosed(false), mAutoClose(false)
        , mHost(), mPath(), mOrigin(), mExtensions()
    {
        ChainInstance(); // Remember this instance
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    WebSocketClient(StackStrF & host, uint16_t port, StackStrF & path)
        : Base(), mHandle(nullptr), mQueue(1024), mOnData(), mOnClose(), mTag(), mData()
        , mPort(port), mSecure(false), mClosing(false), mClosed(false), mAutoClose(false)
        , mHost(host.mPtr, host.GetSize())
        , mPath(path.mPtr, path.GetSize())
        , mOrigin(), mExtensions()
    {
        ChainInstance(); // Remember this instance
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    WebSocketClient(StackStrF & host, uint16_t port, StackStrF & path, bool secure)
        : Base(), mHandle(nullptr), mQueue(1024), mOnData(), mOnClose(), mTag(), mData()
        , mPort(port), mSecure(secure), mClosing(false), mClosed(false), mAutoClose(false)
        , mHost(host.mPtr, host.GetSize())
        , mPath(path.mPtr, path.GetSize())
        , mOrigin(), mExtensions()
    {
        ChainInstance(); // Remember this instance
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    WebSocketClient(StackStrF & host, uint16_t port, StackStrF & path, bool secure, StackStrF & origin)
        : Base(), mHandle(nullptr), mQueue(1024), mOnData(), mOnClose(), mTag(), mData()
        , mPort(port), mSecure(secure), mClosing(false), mClosed(false), mAutoClose(false)
        , mHost(host.mPtr, host.GetSize())
        , mPath(path.mPtr, path.GetSize())
        , mOrigin(origin.mPtr, origin.GetSize())
        , mExtensions()
    {
        ChainInstance(); // Remember this instance
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    WebSocketClient(StackStrF & host, uint16_t port, StackStrF & path, bool secure, StackStrF & origin, StackStrF & ext)
        : Base(), mHandle(nullptr), mQueue(1024), mOnData(), mOnClose(), mTag(), mData()
        , mPort(port), mSecure(secure), mClosing(false), mClosed(false), mAutoClose(false)
        , mHost(host.mPtr, host.GetSize())
        , mPath(path.mPtr, path.GetSize())
        , mOrigin(origin.mPtr, origin.GetSize())
        , mExtensions(ext.mPtr, ext.GetSize())
    {
        ChainInstance(); // Remember this instance
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    WebSocketClient(const WebSocketClient &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    WebSocketClient(WebSocketClient &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor. Closes the connection.
    */
    ~WebSocketClient()
    {
        // Is there a connection left to close?
        if (mHandle != nullptr)
        {
            Close();
        }
        // Forget about this instance
        UnchainInstance();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    WebSocketClient & operator = (const WebSocketClient &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    WebSocketClient & operator = (WebSocketClient &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Return whether the associated connection handle is valid.
    */
    SQMOD_NODISCARD bool IsValid() const
    {
        return mHandle != nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Return whether the associated connection is closing.
     * This is only valid inside the OnClose callback.
    */
    SQMOD_NODISCARD bool IsClosing() const
    {
        return mClosing.load();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve whether auto-closing is enabled or not.
    */
    SQMOD_NODISCARD bool GetAutoClose() const
    {
        return mAutoClose.load();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify whether auto-closing is enabled or not.
    */
    void SetAutoClose(bool toggle)
    {
        mAutoClose.store(toggle);
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
    WebSocketClient & ApplyTag(StackStrF & tag)
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
    WebSocketClient & ApplyData(LightObj & data)
    {
        mData = data;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a connection exists.
    */
    void Validate() const
    {
        if (mHandle == nullptr)
        {
            STHROWF("No connection was made to server ({}:{}{})", mHost, mPort, mPath);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Return a valid connection.
    */
    SQMOD_NODISCARD struct mg_connection * Valid() const
    {
        Validate();
        return mHandle;
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a connection does not exist.
    */
    void Invalid() const
    {
        if (mHandle != nullptr)
        {
            STHROWF("Connection already made to server ({}:{}{})", mHost, mPort, mPath);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated server host.
    */
    SQMOD_NODISCARD const String & GetHost() const
    {
        return mHost;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated server host.
    */
    void SetHost(StackStrF & host)
    {
        Invalid();
        // Is there a valid host?
        if (host.mLen > 0)
        {
            mHost.assign(host.mPtr, static_cast< size_t >(host.mLen));
        }
        else
        {
            mHost.clear();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated server host.
    */
    WebSocketClient & ApplyHost(StackStrF & host)
    {
        SetHost(host);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated server path.
    */
    SQMOD_NODISCARD const String & GetPath() const
    {
        return mPath;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated server path.
    */
    void SetPath(StackStrF & path)
    {
        Invalid();
        // Is there a valid path?
        if (path.mLen > 0)
        {
            mPath.assign(path.mPtr, static_cast< size_t >(path.mLen));
        }
        else
        {
            mPath.clear();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated server path.
    */
    WebSocketClient & ApplyPath(StackStrF & path)
    {
        SetPath(path);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated origin value.
    */
    SQMOD_NODISCARD const String & GetOrigin() const
    {
        return mOrigin;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated origin value.
    */
    void SetOrigin(StackStrF & origin)
    {
        Invalid();
        // Is there a valid origin?
        if (origin.mLen > 0)
        {
            mOrigin.assign(origin.mPtr, static_cast< size_t >(origin.mLen));
        }
        else
        {
            mOrigin.clear();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated origin value.
    */
    WebSocketClient & ApplyOrigin(StackStrF & origin)
    {
        SetOrigin(origin);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated connection extensions.
    */
    SQMOD_NODISCARD const String & GetExtensions() const
    {
        return mExtensions;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated connection extensions.
    */
    void SetExtensions(StackStrF & ext)
    {
        Invalid();
        // Is there a valid extension?
        if (ext.mLen > 0)
        {
            mExtensions.assign(ext.mPtr, static_cast< size_t >(ext.mLen));
        }
        else
        {
            mExtensions.clear();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated connection extensions.
    */
    WebSocketClient & ApplyExtensions(StackStrF & ext)
    {
        SetExtensions(ext);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated server port number.
    */
    SQMOD_NODISCARD int GetPort() const
    {
        return mPort;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated server port number.
    */
    void SetPort(int port)
    {
        Invalid();
        // Is there a valid port?
        if (port < 0 || port > 65535)
        {
            STHROWF("Invalid port number: {0} < 0 || {0} > 65535", port);
        }
        else
        {
            mPort = port;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated server port number.
    */
    WebSocketClient & ApplyPort(int port)
    {
        SetPort(port);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated SSL status.
    */
    SQMOD_NODISCARD bool GetSecure() const
    {
        return mSecure;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated SSL status.
    */
    void SetSecure(bool secure)
    {
        Invalid();
        mSecure = secure;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated SSL status.
    */
    WebSocketClient & ApplySecure(bool secure)
    {
        SetSecure(secure);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated data callback.
    */
    SQMOD_NODISCARD Function & GetOnData()
    {
        return mOnData;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated data callback.
    */
    void SetOnData(Function & cb)
    {
        mOnData = cb;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated data callback.
    */
    WebSocketClient & BindOnData(Function & cb)
    {
        mOnData = cb;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated close callback.
    */
    SQMOD_NODISCARD Function & GetOnClose()
    {
        return mOnClose;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated close callback.
    */
    void SetOnClose(Function & cb)
    {
        mOnClose = cb;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated close callback.
    */
    WebSocketClient & BindOnClose(Function & cb)
    {
        mOnClose = cb;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Connect to a web-socket as a client.
    */
    WebSocketClient & Connect();

    /* --------------------------------------------------------------------------------------------
     * Connect to a web-socket as a client with specific extensions.
    */
    WebSocketClient & ConnectExt();

    /* --------------------------------------------------------------------------------------------
     * Close client connection.
    */
    void Close()
    {
        mg_close_connection(Valid());
        // Prevent further use
        mHandle = nullptr;
        // Process pending events
        Process(true);
    }

    /* --------------------------------------------------------------------------------------------
     * Sends the contents of the given buffer through the socket as a single frame.
    */
    SQMOD_NODISCARD SQInteger SendOpCode(SQInteger opcode)
    {
        return mg_websocket_client_write(Valid(), static_cast< int >(opcode), nullptr, 0);
    }

    /* --------------------------------------------------------------------------------------------
     * Sends the contents of the given buffer through the socket as a single frame.
    */
    SQMOD_NODISCARD SQInteger SendBuffer(SqBuffer & buf, SQInteger opcode)
    {
        return mg_websocket_client_write(Valid(), static_cast< int >(opcode), buf.Valid().Data(), buf.Valid().Position());
    }

    /* --------------------------------------------------------------------------------------------
     * Sends the contents of the given string through the socket as a single frame.
    */
    SQMOD_NODISCARD SQInteger SendString(SQInteger opcode, StackStrF & str)
    {
        return mg_websocket_client_write(Valid(), static_cast< int >(opcode), str.mPtr, static_cast< size_t >(str.mLen));
    }

    /* --------------------------------------------------------------------------------------------
     * Process received data.
    */
    void Process(bool force = false)
    {
        // Is there a valid connection?
        if (mHandle == nullptr && !force)
        {
            return; // No point in going forward
        }
        FramePtr frame;
        // See if connection is closing
        const bool closing = mClosing.load();
        // Is the connection closing?
        if (closing)
        {
            mHandle = nullptr; // Prevent further use
        }
        // Retrieve each frame individually and process it
        for (size_t count = mQueue.size_approx(), n = 0; n <= count; ++n)
        {
            // Try to get a frame from the queue
            if (mQueue.try_dequeue(frame) && !mOnData.IsNull())
            {
                // Obtain a buffer from the frame
                Buffer b(frame->mData, frame->mSize, frame->mSize, Buffer::OwnIt{});
                // Backup the frame size before forgetting about it
                const SQInteger size = static_cast< SQInteger >(frame->mSize);
                // Take ownership of the memory
                frame->ForgetBuffer();
                // Transform the buffer into a script object
                LightObj obj(SqTypeIdentity< SqBuffer >{}, SqVM(), std::move(b));
                // Forward the event to the callback
                mOnData.Execute(obj, size, frame->mFlags);
            }
        }
        // Is the server closing the connection?
        if (closing && !mClosed.load() && !mOnClose.IsNull())
        {
            // Let the user know
            mOnClose.Execute();
            // Prevent calling this callback again
            mClosed.store(true);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used internally to release script resources, if any. The VM is about to be closed.
     * If you don't close the connection yourself don't care about what is received after this.
    */
    void Terminate()
    {
        // Process pending data
        Process(true);
        // Release callbacks
        mOnData.Release();
        mOnClose.Release();
        // Release user data
        mData.Release();
    }

protected:

    /* --------------------------------------------------------------------------------------------
     * Callback for handling data received from the server
    */
    int DataHandler(int flags, char * data, size_t data_len) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Callback for handling a close message received from the server.
    */
    void CloseHandler() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Proxy for DataHandler()
    */
    static int DataHandler_(struct mg_connection * SQ_UNUSED_ARG(c), int f, char * d, size_t n, void * u) noexcept
    {
        return reinterpret_cast< WebSocketClient * >(u)->DataHandler(f, d, n);
    }

    /* --------------------------------------------------------------------------------------------
     * Proxy for CloseHandler();
    */
    static void CloseHandler_(const struct mg_connection * SQ_UNUSED_ARG(c), void * u) noexcept
    {
        reinterpret_cast< WebSocketClient * >(u)->CloseHandler();
    }
};

} // Namespace:: SqMod
