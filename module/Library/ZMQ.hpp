#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"
#include "Library/IO/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <mutex>
#include <thread>
#include <vector>
#include <memory>
#include <chrono>
#include <utility>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
#include <zmq.h>
#include <concurrentqueue.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
struct ZCtx;
struct ZSkt;
struct ZContext;
struct ZSocket;

/* ------------------------------------------------------------------------------------------------
 * Core implementation and management for a ZMQ context.
*/
struct ZCtx
{
    /* --------------------------------------------------------------------------------------------
     * Smart pointers to this type. Helper typedefs.
    */
    using Ptr = std::shared_ptr< ZCtx >;

    /* --------------------------------------------------------------------------------------------
     * Context pointer.
    */
    void * mPtr;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ZCtx()
        : mPtr(zmq_ctx_new())
    {
        if (!mPtr)
        {
            STHROWF("Unable to initialize context: {}", zmq_strerror(errno));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit ZCtx(void * ptr)
        : mPtr(ptr)
    {
        if (!mPtr)
        {
            STHROWF("Invalid context");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    ZCtx(const ZCtx &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    ZCtx(ZCtx &&) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ZCtx()
    {
        if (mPtr)
        {
            int r = zmq_ctx_term(mPtr);
            // Just in case
            if (r != 0)
            {
                LogFtl("Context failed to terminate properly: [%d], %s", r, zmq_strerror(errno));
            }
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Assignment operator (disabled).
    */
    ZCtx & operator = (const ZCtx &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment (disabled).
    */
    ZCtx & operator = (ZCtx &&) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean operator.
    */
    operator bool () const noexcept { return static_cast< bool >(mPtr); } // NOLINT(google-explicit-constructor)

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to context pointer (void *) operator.
    */
    operator void * () const noexcept { return mPtr; } // NOLINT(google-explicit-constructor)
};

/* ------------------------------------------------------------------------------------------------
 * Core implementation and management for a message.
*/
struct ZMsg
{
    /* --------------------------------------------------------------------------------------------
     * List of messages.
    */
    using List = std::vector< Buffer >;

    /* --------------------------------------------------------------------------------------------
     * Raw union data size.
    */
    static constexpr size_t SIZE = sizeof(Buffer) > sizeof(List) ? sizeof(Buffer) : sizeof(List);

    /* --------------------------------------------------------------------------------------------
     * Tag used to indicate a multi-part message.
    */
    struct Multipart { };

    /* --------------------------------------------------------------------------------------------
     * Message contents.
    */
    union
    {
        Buffer      mBuff;
        List        mList;
        uint8_t     mData[SIZE];
    };

    /* --------------------------------------------------------------------------------------------
     * Whether this is a multi-part-message.
    */
    const bool  mMulti;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ZMsg() // NOLINT(cppcoreguidelines-pro-type-member-init)
        : mBuff(), mMulti(false)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Default multi-part constructor.
    */
    explicit ZMsg(Multipart) // NOLINT(cppcoreguidelines-pro-type-member-init)
        : mList(), mMulti(true)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit ZMsg(const Buffer & b) // NOLINT(cppcoreguidelines-pro-type-member-init,modernize-pass-by-value)
        : mBuff(b), mMulti(false)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy multi-part constructor.
    */
    explicit ZMsg(const List & l) // NOLINT(cppcoreguidelines-pro-type-member-init,modernize-pass-by-value)
        : mList(l), mMulti(true)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit ZMsg(Buffer && b) // NOLINT(cppcoreguidelines-pro-type-member-init)
        : mBuff(std::move(b)), mMulti(false)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Move multi-part constructor.
    */
    explicit ZMsg(List && l) // NOLINT(cppcoreguidelines-pro-type-member-init)
        : mList(std::move(l)), mMulti(true)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    ZMsg(const ZMsg &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    ZMsg(ZMsg &&) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ZMsg()
    {
        // Is this multi-part?
        if (mMulti)
        {
            mList.~List(); // Invoke list destructor
        }
        else
        {
            mBuff.~Buffer(); // Invoke buffer destructor
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Assignment operator (disabled).
    */
    ZMsg & operator = (const ZMsg &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment (disabled).
    */
    ZMsg & operator = (ZMsg &&) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Push the specified message to the multi-part list.
    */
    void Push(zmq_msg_t & msg)
    {
        mList.emplace_back(static_cast< Buffer::ConstPtr >(zmq_msg_data(&msg)),
                            static_cast< Buffer::SzType >(zmq_msg_size(&msg)));
    }
};

/* ------------------------------------------------------------------------------------------------
 * Core implementation and management for a ZMQ socket.
*/
struct ZSkt : SqChainedInstances< ZSkt >
{
    /* --------------------------------------------------------------------------------------------
     * Smart pointers to this type. Helper typedefs.
    */
    using Ptr = std::shared_ptr< ZSkt >;

    /* --------------------------------------------------------------------------------------------
     * Message queue type.
    */
    using Item = std::unique_ptr< ZMsg >;

    /* --------------------------------------------------------------------------------------------
     * Message queue type.
    */
    using Queue = moodycamel::ConcurrentQueue< Item >;

    /* --------------------------------------------------------------------------------------------
     * Context pointer.
    */
    void * mPtr;

    /* --------------------------------------------------------------------------------------------
     * Socket status.
    */
    bool mRun;

    /* --------------------------------------------------------------------------------------------
     * Messages should be delivered as string instead of binary data.
    */
    bool mStringMessages;

    /* --------------------------------------------------------------------------------------------
     * Socket type.
    */
    int mType;

    /* --------------------------------------------------------------------------------------------
     * Synchronization mutex.
    */
    std::mutex mMtx;

    /* --------------------------------------------------------------------------------------------
     * Messages received from the socket.
    */
    Queue mOutputQueue;

    /* --------------------------------------------------------------------------------------------
     * Messages to be sent through the socket.
    */
    Queue mInputQueue;

    /* --------------------------------------------------------------------------------------------
     * Message received callback.
    */
    Function mOnData;

    /* --------------------------------------------------------------------------------------------
     * Processing thread.
    */
    std::thread mThread;

    /* --------------------------------------------------------------------------------------------
     * Socket context.
    */
    ZCtx::Ptr mContext;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    ZSkt(const ZCtx::Ptr & ctx, int type)
        : SqChainedInstances< ZSkt >()
        , mPtr(nullptr), mRun(true), mStringMessages(true), mType(type), mMtx()
        , mOutputQueue(4096), mInputQueue(4096)
        , mOnData(), mThread(), mContext(ctx)
    {
        // Validate the context
        if (!ctx)
        {
            STHROWF("Invalid context");
        }
        // Create the processing thread
        mThread = std::thread(&ZSkt::Proc, this);
        // Remember this instance
        ChainInstance();
        // Wait for the socket to be created before we attempt to use it
        for (int n = 0; n < 100; ++n)
        {
            using namespace std::chrono_literals;
            // Acquire exclusive access to the socket
            mMtx.lock();
            // Was the socket created?
            if (mPtr)
            {
                // Release exclusive access to the socket
                mMtx.unlock();
                // Socket created
                break;
            }
            else
            {
                // Release exclusive access to the socket
                mMtx.unlock();
                // Wait for the socket to be created
                std::this_thread::sleep_for(10ms);
            }
        }
        // If it wasn't created by this point then something isn't right
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    ZSkt(const ZSkt &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    ZSkt(ZSkt &&) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ZSkt()
    {
        // Anything to close?
        if (mPtr)
        {
            Close();
        }
        // Forget about this instance
        UnchainInstance();
    }

    /* --------------------------------------------------------------------------------------------
     * Assignment operator (disabled).
    */
    ZSkt & operator = (const ZSkt &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment (disabled).
    */
    ZSkt & operator = (ZSkt &&) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean operator.
    */
    operator bool () const noexcept { return static_cast< bool >(mPtr); } // NOLINT(google-explicit-constructor)

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to socket pointer (void *) operator.
    */
    operator void * () const noexcept { return mPtr; } // NOLINT(google-explicit-constructor)

    /* --------------------------------------------------------------------------------------------
     * Internal processing thread.
     * NOTE: Messages are being sent in whatever order we can.
     *       Don't expect them be in the order you sent or receive them.
     *       That's the cost of simplicity. And something I can live with under the circumstances.
    */
    void Proc()
    {
        // Acquire exclusive access to the socket
        mMtx.lock();
        // Create the socket in this thread
        mPtr = zmq_socket(*mContext, mType);
        // Validate the socket
        if (!mPtr)
        {
            LogErr("Unable to initialize socket: %s", zmq_strerror(errno));
            // Stop the thread
            return;
        }
        // Release exclusive access to the socket
        mMtx.unlock();
        // Enter processing loop
        while (mRun)
        {
            using namespace std::chrono_literals;
            // Acquire exclusive access to the socket
            mMtx.lock();
            // Perform tasks until there's none left
            if (!Recv() && !Send())
            {
                // Release exclusive access to the socket
                mMtx.unlock();
                // Don't exhaust resources pointlessly
                std::this_thread::sleep_for(50ms);
            }
            else
            {
                // Release exclusive access to the socket
                mMtx.unlock();
            }
        }
        // Acquire exclusive access to the socket
        mMtx.lock();
        // Close the socket
        int r = zmq_close(mPtr);
        // Forget about it
        mPtr = nullptr;
        // Release exclusive access to the socket
        mMtx.unlock();
        // Validate result
        if (r != 0)
        {
            LogErr("Unable to close socket: [%d] {%s}", r, zmq_strerror(errno));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Flush messages from the queue to the script.
    */
    void Flush(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Stop sockets and prepare for a shutdown.
    */
    void Close()
    {
        // Is the processing thread running?
        if (mThread.joinable())
        {
            // Acquire exclusive access
            mMtx.lock();
            // Stop the loop
            mRun = false;
            // Yield exclusive access
            mMtx.unlock();
            // Wait for the thread
            mThread.join();
        }
        else
        {
            mRun = false; // Just in case
        }
        // Forget about the context
        mContext.reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Queue a message to be sent through the socket.
    */
    void Send(const Buffer & data)
    {
        mInputQueue.enqueue(std::make_unique< ZMsg >(data));
    }

    /* --------------------------------------------------------------------------------------------
     * Queue a message to be sent through the socket.
    */
    void Send(Buffer && data)
    {
        mInputQueue.enqueue(std::make_unique< ZMsg >(std::move(data)));
    }

    /* --------------------------------------------------------------------------------------------
     * Queue a multi-part message to be sent through the socket.
    */
    void Send(const ZMsg::List & list)
    {
        mInputQueue.enqueue(std::make_unique< ZMsg >(list));
    }

    /* --------------------------------------------------------------------------------------------
     * Queue a multi-part message to be sent through the socket.
    */
    void Send(ZMsg::List && list)
    {
        mInputQueue.enqueue(std::make_unique< ZMsg >(std::move(list)));
    }

protected:

    /* --------------------------------------------------------------------------------------------
     * Receive one message from the socket.
    */
    bool Recv()
    {
        // Need someone to receive the message
        zmq_msg_t msg;
        // Initialize to an empty message
        int r = zmq_msg_init(&msg);
        // Make sure we have a message
        if (r != 0)
        {
            LogErr("Unable to initialize ZMQ message");
            // We couldn't receive anything
            return false;
        }
        // Ask for a message, if any
        r = zmq_msg_recv(&msg, mPtr, ZMQ_DONTWAIT);
        // Is this a multi-part message?
        if (zmq_msg_more(&msg) == 1)
        {
            return RecvMore(msg, r);
        }
        // Extract the message data
        Item item = std::make_unique< ZMsg >(Buffer(static_cast< Buffer::ConstPtr >(zmq_msg_data(&msg)),
                                                        static_cast< Buffer::SzType >(zmq_msg_size(&msg))));
        // Release this message
        zmq_msg_close(&msg);
        // Did we have a message?
        if (r >= 0)
        {
            // Put it in the queue
            mOutputQueue.enqueue(std::move(item));
            // We received a message
            return true;
        }
        // No message was retrieved
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Receive multiple messages from the socket.
    */
    bool RecvMore(zmq_msg_t & msg, int r)
    {
        size_t more_sz = sizeof(int);
        // Used to see if more parts follow
        int more = 1;
        // Create an empty multi-part message container
        Item item = std::make_unique< ZMsg >(ZMsg::Multipart{});
        // Add the initial message to the list
        if (r >= 0)
        {
            // Save it to the list
            item->Push(msg);
            // Close the message
            zmq_msg_close(&msg);
        }
        // Keep receiving messages while there's more
        do
        {
            // Initialize an empty message
            r = zmq_msg_init(&msg);
            // Make sure we have a message
            if (r != 0)
            {
                LogErr("Unable to initialize ZMQ message part");
                // Abort everything
                return false;
            }
            // Ask for another message, if any (blocking operation!)
            r = zmq_msg_recv(&msg, mPtr, 0);
            // Do we actually have a message?
            if (r >= 0)
            {
                // Save it to the list
                item->Push(msg);
                // Close the message
                zmq_msg_close(&msg);
            }
            // See if the message part last received from the socket was a data part with more parts to follow.
            zmq_getsockopt(mPtr, ZMQ_RCVMORE, &more, &more_sz);
        } while (more);
        // Did we actually have any valid messages?
        if (!(item->mList.empty()))
        {
            mOutputQueue.enqueue(std::move(item));
            // Messages were present in the list
            return true;
        }
        // No messages were added
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Send one queued message to the socket.
    */
    bool Send()
    {
        // Need someone to receive the message
        Item data;
        // Try to get a message from the queue
        if (mInputQueue.try_dequeue(data))
        {
            if (data->mMulti)
            {
                SendMore(data->mList);
            }
            else
            {
                SendOne(data->mBuff);
            }
            // One item was found in the queue
            return true;
        }
        else
        {
            return false; // No item in the queue
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Send a single message to the socket.
    */
    void SendOne(Buffer & buff) const
    {
        // Attempt to send the message
        int r = zmq_send(mPtr, buff.Data(), buff.Position(), ZMQ_DONTWAIT);
        // Could we send what the message had?
        if (r >= 0 && static_cast< Buffer::SzType >(r) != buff.Position())
        {
            LogErr("Unable to send data to socket: [%d], {%s}", r, zmq_strerror(errno));
            // NOTE: Should we put the buffer back into the queue?
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Send a multi-part message to the socket.
    */
    void SendMore(ZMsg::List & list) const
    {
        // Send all message parts
        for (size_t i = 0, n = list.size(); i < n; ++i)
        {
            // Attempt to send the message
            int r = zmq_send(mPtr, list[i].Data(), list[i].Position(), (i + 1) == n ? ZMQ_DONTWAIT : ZMQ_SNDMORE);
            // Could we send what the message had?
            if (r >= 0 && static_cast< Buffer::SzType >(r) != list[i].Position())
            {
                LogErr("Unable to send multi-part data to socket: [%d], %s", r, zmq_strerror(errno));
                // NOTE: Should we abort the whole thing? But we probably already sent some.
            }
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Interface for ZMQ contexts.
*/
struct ZContext
{
    /* --------------------------------------------------------------------------------------------
     * Pointer to the interfaced context.
    */
    ZCtx::Ptr mHnd;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ZContext()
        : mHnd(std::make_shared< ZCtx >())
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Pointer constructor.
    */
    explicit ZContext(ZCtx::Ptr ptr)
        : mHnd(std::move(ptr))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    ZContext(const ZContext &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    ZContext(ZContext &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ZContext() = default;

    /* --------------------------------------------------------------------------------------------
     * Assignment operator.
    */
    ZContext & operator = (const ZContext &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment.
    */
    ZContext & operator = (ZContext &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Make sure a context instance is referenced.
    */
    void Validate() const
    {
        if (!mHnd)
        {
            STHROWF("Invalid context instance");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a context instance is referenced and return the context.
    */
    SQMOD_NODISCARD ZCtx & Valid() { Validate(); return *mHnd; }

    /* --------------------------------------------------------------------------------------------
     * Make sure a context instance is referenced and return the context.
    */
    SQMOD_NODISCARD const ZCtx & Valid() const { Validate(); return *mHnd; }

    /* --------------------------------------------------------------------------------------------
     * Make sure a context instance is referenced and return the reference.
    */
    SQMOD_NODISCARD ZCtx::Ptr & ValidRef() { Validate(); return mHnd; }

    /* --------------------------------------------------------------------------------------------
     * Make sure a context instance is referenced and return the reference.
    */
    SQMOD_NODISCARD const ZCtx::Ptr & ValidRef() const { Validate(); return mHnd; }

    /* --------------------------------------------------------------------------------------------
     * Check if a context instance is referenced.
    */
    SQMOD_NODISCARD bool IsNull() const
    {
        return static_cast< bool >(mHnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value of an option.
    */
    SQMOD_NODISCARD int Get(int opt) const
    {
        return zmq_ctx_get(Valid(), opt);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value of an option.
    */
    void Set(int opt, int value)
    {
        int r = zmq_ctx_set(Valid(), opt, value);
        // Validate result
        if (r != 0)
        {
            STHROWF("Unable to set context option: [{}] {}", r, zmq_strerror(errno));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value of an option.
    */
    void Shutdown() const
    {
        int r = zmq_ctx_shutdown(Valid());
        // Validate result
        if (r != 0)
        {
            STHROWF("Unable to shutdown context: {}", zmq_strerror(errno));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Helper function to create sockets.
    */
    SQMOD_NODISCARD LightObj Socket(int type) const;
};

/* ------------------------------------------------------------------------------------------------
 * Interface for ZMQ sockets.
*/
struct ZSocket
{
    /* --------------------------------------------------------------------------------------------
     * Pointer to the interfaced socket.
    */
    ZSkt::Ptr mHnd;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ZSocket(const ZContext & ctx, int type)
        : mHnd(std::make_shared< ZSkt >(ctx.mHnd, type))
    {
        if (!(mHnd->mPtr))
        {
            STHROWF("Failed to create socket");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Pointer constructor.
    */
    explicit ZSocket(ZSkt::Ptr ptr)
        : mHnd(std::move(ptr))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    ZSocket(const ZSocket &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    ZSocket(ZSocket &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ZSocket()
    {
        Close();
    }

    /* --------------------------------------------------------------------------------------------
     * Assignment operator.
    */
    ZSocket & operator = (const ZSocket &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment.
    */
    ZSocket & operator = (ZSocket &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Make sure a socket instance is referenced.
    */
    void Validate() const
    {
        if (!mHnd)
        {
            STHROWF("Invalid socket instance");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a socket instance is referenced and return the socket.
    */
    SQMOD_NODISCARD ZSkt & Valid() { Validate(); return *mHnd; }

    /* --------------------------------------------------------------------------------------------
     * Make sure a socket instance is referenced and return the socket.
    */
    SQMOD_NODISCARD const ZSkt & Valid() const { Validate(); return *mHnd; }

    /* --------------------------------------------------------------------------------------------
     * Make sure a socket instance is referenced and return the reference.
    */
    SQMOD_NODISCARD ZSkt::Ptr & ValidRef() { Validate(); return mHnd; }

    /* --------------------------------------------------------------------------------------------
     * Make sure a socket instance is referenced and return the reference.
    */
    SQMOD_NODISCARD const ZSkt::Ptr & ValidRef() const { Validate(); return mHnd; }

    /* --------------------------------------------------------------------------------------------
     * Check if a socket instance is referenced.
    */
    SQMOD_NODISCARD bool IsNull() const
    {
        return static_cast< bool >(mHnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value of a socket option.
    */
    SQMOD_NODISCARD LightObj GetOpt(int opt);

    /* --------------------------------------------------------------------------------------------
     * Modify the value of a socket option.
    */
    void SetOpt(int opt, LightObj & value);

    /* --------------------------------------------------------------------------------------------
     * Instruct the socket to always deliver messages as strings instead of binary data.
    */
    SQMOD_NODISCARD bool GetStringMessages() const
    {
        return Valid().mStringMessages;
    }

    /* --------------------------------------------------------------------------------------------
     * Instruct the socket to always deliver messages as strings instead of binary data.
    */
    void SetStringMessages(bool value)
    {
        Valid().mStringMessages = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Callback to receive incoming messages.
    */
    ZSocket & OnData(Function & cb)
    {
        Valid().mOnData = std::move(cb);
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Accept incoming connections on the socket.
    */
    ZSocket & Bind(StackStrF & ep)
    {
        // Acquire exclusive access to the socket
        std::lock_guard< std::mutex > guard(Valid().mMtx);
        // Attempt to bind the socket
        int r = zmq_bind(Valid(), ep.mPtr);
        // Validate result
        if (r != 0)
        {
            STHROWF("Unable to bind socket: [{}] {}", r, zmq_strerror(errno));
        }
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Create outgoing connection from the socket
    */
    ZSocket & Connect(StackStrF & ep)
    {
        // Acquire exclusive access to the socket
        std::lock_guard< std::mutex > guard(Valid().mMtx);
        // Attempt to connect the socket
        int r = zmq_connect(Valid(), ep.mPtr);
        // Validate result
        if (r != 0)
        {
            STHROWF("Unable to connect socket: [{}] {}", r, zmq_strerror(errno));
        }
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Create outgoing connection from the socket
    */
    ZSocket & Disconnect(StackStrF & ep)
    {
        // Acquire exclusive access to the socket
        std::lock_guard< std::mutex > guard(Valid().mMtx);
        // Attempt to connect the socket
        int r = zmq_disconnect(Valid(), ep.mPtr);
        // Validate result
        if (r != 0)
        {
            STHROWF("Unable to disconnect socket: [{}] {}", r, zmq_strerror(errno));
        }
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Close the managed socket.
    */
    ZSocket & Close()
    {
        Valid().Close();
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Send a binary message to the socket.
    */
    ZSocket & SendBuffer(SqBuffer & data)
    {
        // Validate buffer
        data.ValidateDeeper();
        // Create a copy and send it
        Valid().Send(Buffer(data.GetInst().Data(), data.GetInst().Position()));
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Send a string message to the socket.
    */
    ZSocket & SendString(StackStrF & str)
    {
        Valid().Send(Buffer(static_cast< Buffer::ConstPtr >(str.mPtr), ClampL< SQInteger, Buffer::SzType >(str.mLen)));
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Send a multi-part ZMQ message to the socket.
    */
    ZSocket & SendBuffers(Array & arr)
    {
        Validate();

        ZMsg::List list;
        // Extract the messages from the array
        arr.Foreach([&list](HSQUIRRELVM vm, SQInteger) {
            // Extract the buffer from the stack
            SqBuffer * data = ClassType< SqBuffer >::GetInstance(vm, -1);
            // In case we didn't fail at the null part (it should)
            if (data && data->GetRef())
            {
                // Add the message to the list
                list.emplace_back(data->GetInst().Data(), data->GetInst().Position());
            }
            // Continue
            return SQ_OK;
        });
        // Send the message list
        mHnd->Send(std::move(list));
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Send a multi-part string message to the socket.
    */
    ZSocket & SendStrings(Array & arr)
    {
        Validate();

        ZMsg::List list;
        // Extract the messages from the array
        arr.Foreach([&list](HSQUIRRELVM vm, SQInteger) {
            StackStrF str(vm, -1);
            // Extract the string from the stack
            if (SQ_FAILED(str.Proc(false)))
            {
                return str.mRes; // Abort
            }
            // Create a new message
            list.emplace_back(static_cast< Buffer::ConstPtr >(str.mPtr), ClampL< SQInteger, Buffer::SzType >(str.mLen));
            // Continue
            return SQRESULT(SQ_OK);
        });
        // Send the message list
        mHnd->Send(std::move(list));
        // Allow chaining
        return *this;
    }
};

} // Namespace:: SqMod
