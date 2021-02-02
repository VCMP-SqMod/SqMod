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
struct ZSkt;
struct ZMsg;
struct ZCtx;
struct ZSocket;
struct ZMessage;
struct ZContext;

/* ------------------------------------------------------------------------------------------------
 * Given as a function pointer to free memory using std::free(data).
*/
inline void ZmqFreeSTD(void * data, void *)
{
    std::free(data);
}

/* ------------------------------------------------------------------------------------------------
 * Given as a function pointer to free memory using delete data.
*/
template < class T > inline void ZmqFreeDelete(void * data, void *)
{
    // If this throws an exception we may as well just be fked. But very (VERY!) low chances.
    delete static_cast< T * >(data);
}

/* ------------------------------------------------------------------------------------------------
 * Given as a function pointer to free memory using delete[] data.
*/
template < class T > inline void ZmqFreeDeleteArray(void * data, void *)
{
    // If this throws an exception we may as well just be fked. But very (VERY!) low chances.
    delete[] static_cast< T * >(data);
}

/* ------------------------------------------------------------------------------------------------
 * Allocate raw memory for a string, fill it with data from a StackStrF instance and return it.
*/
inline void * ZmqDataFromStackStrF(StackStrF & data)
{
    if (data.mLen)
    {
        // Allocate the string memory
        auto * mem = new SQChar[static_cast< size_t >(data.mLen)];
        // Why not
        assert(mem);
        // Copy the string in the memory buffer
        std::memcpy(mem, data.mPtr, static_cast< size_t >(data.mLen));
        /* normally you'd have to do static_cast< size_t >(data.mLen) * sizeof(SQChar) */
        /* but this SQChar is required to be 1 byte so we don't bother with it */
        // Yield ownership of the memory
        return mem;
    }
    // Failed!
    return nullptr;
}

/* ------------------------------------------------------------------------------------------------
 * Core implementation and management for a ZMQ context.
*/
struct ZCtx
{
    /* --------------------------------------------------------------------------------------------
     * Smart pointers to this type. Helper typedefs.
    */
    using Ptr = std::shared_ptr< ZCtx >;
    using Ref = std::weak_ptr< ZCtx >;

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
            STHROWF("Unable to initialize context: %s", zmq_strerror(errno));
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
                LogFtl("Context failed to terminate properly: [%d], %s", r, zmq_strerror(r));
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
 * Core implementation and management for a ZMQ message.
*/
struct ZMsg
{
    /* --------------------------------------------------------------------------------------------
     * Smart pointers to this type. Helper typedefs.
    */
    using Ptr = std::shared_ptr< ZMsg >;
    using Ref = std::weak_ptr< ZMsg >;

    /* --------------------------------------------------------------------------------------------
     * The underlying message.
    */
    std::unique_ptr< zmq_msg_t > mPtr;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ZMsg()
        : mPtr(std::make_unique< zmq_msg_t >())
    {
        int r = zmq_msg_init(mPtr.get());
        // Validate result
        if (r != 0)
        {
            STHROWF("Unable to initialize message: [%d] %s", r, zmq_strerror(r));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit message size constructor.
    */
    explicit ZMsg(SQInteger size)
        : mPtr(std::make_unique< zmq_msg_t >())
    {
        int r = zmq_msg_init_size(mPtr.get(), ClampL< SQInteger, size_t >(size));
        // Validate result
        if (r != 0)
        {
            STHROWF("Unable to initialize message: [%d] %s", r, zmq_strerror(r));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit message data and size constructor.
    */
    ZMsg(void * data, SQInteger size, zmq_free_fn * ffn, void * hint = nullptr)
        : mPtr(std::make_unique< zmq_msg_t >())
    {
        // Make sure there's data if required
        if (size > 0 && !data)
        {
            STHROWF("Invalid message data");
        }
        // Now the message can be initialized
        int r = zmq_msg_init_data(mPtr.get(), data, ClampL< SQInteger, size_t >(size), ffn, hint);
        // Validate result
        if (r != 0)
        {
            STHROWF("Unable to initialize message: [%d] %s", r, zmq_strerror(r));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    ZMsg(const ZMsg & o)
        : mPtr(std::make_unique< zmq_msg_t >())
    {
        int r = zmq_msg_init(mPtr.get());
        // Validate result
        if (r != 0)
        {
            LogFtl("Unable to initialize message: [%d] %s", r, zmq_strerror(r));
        }
        r = zmq_msg_copy(mPtr.get(), o.mPtr.get());
        // Validate result
        if (r != 0)
        {
            LogFtl("Unable to copy message: [%d] %s", r, zmq_strerror(r));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    ZMsg(ZMsg && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ZMsg()
    {
        if (mPtr)
        {
            zmq_msg_close(mPtr.get());
            // We don't really care if the above failed (i.e. returned EFAULT)
            // We probably did it already before but we need to be sure
            // This is something I can live with in this under the circumstances
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Assignment operator.
    */
    ZMsg & operator = (const ZMsg & o)
    {
        // Prevent self assignment
        if (this != &o)
        {
            // We need a message, even if empty
            if (!mPtr)
            {
                int r = zmq_msg_init(mPtr.get());
                // Validate result
                if (r != 0)
                {
                    LogFtl("Unable to initialize message: [%d] %s", r, zmq_strerror(r));
                }
            }
            // Do we have a message?
            if (mPtr)
            {
                int r = zmq_msg_copy(mPtr.get(), o.mPtr.get());
                // Validate result
                if (r != 0)
                {
                    LogFtl("Unable to copy message: [%d] %s", r, zmq_strerror(r));
                }
            }
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Move assignment.
    */
    ZMsg & operator = (ZMsg && o) noexcept
    {
        // Prevent self assignment
        if (this != &o)
        {
            // Close current message, if any
            if (mPtr)
            {
                zmq_msg_close(mPtr.get());
            }
            // Now the message can be moved
            mPtr = std::move(o.mPtr);
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to const message pointer (const zmq_msg_t *) operator.
    */
    operator zmq_msg_t * () const noexcept { return mPtr.get(); } // NOLINT(google-explicit-constructor)
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
    using Ref = std::weak_ptr< ZSkt >;

    /* --------------------------------------------------------------------------------------------
     * List of messages.
    */
    using List = std::vector< ZMsg >;

    /* --------------------------------------------------------------------------------------------
     * Message list item.
    */
    using ListItem = std::unique_ptr< List >;

    /* --------------------------------------------------------------------------------------------
     * Message queue type.
    */
    using Queue = moodycamel::ConcurrentQueue< ZMsg >;

    /* --------------------------------------------------------------------------------------------
     * Message list queue type.
    */
    using ListQueue = moodycamel::ConcurrentQueue< ListItem >;

    /* --------------------------------------------------------------------------------------------
     * Context pointer.
    */
    void * mPtr;

    /* --------------------------------------------------------------------------------------------
     * Socket status.
    */
    int mStatus;

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
     * Multi-part messages to be sent through the socket.
    */
    ListQueue mInputListQueue;

    /* --------------------------------------------------------------------------------------------
     * Message received callback.
    */
    Function mOnData;

    /* --------------------------------------------------------------------------------------------
     * Processing thread.
    */
    std::thread mThread;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    ZSkt(void * ctx, int type)
        : SqChainedInstances< ZSkt >()
        /* normally we'd validate ctx. but i have a feeling we'd be fine here */
        , mPtr(zmq_socket(ctx, type)), mStatus(0), mMtx()
        , mOutputQueue(4096), mInputQueue(4096), mInputListQueue(1024)
        , mOnData(), mThread()
    {
        if (!mPtr)
        {
            STHROWF("Unable to initialize socket: %s", zmq_strerror(errno));
        }
        // Remember this instance
        ChainInstance();
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
        if (mPtr)
        {
            int r = zmq_close(mPtr);
            // Just in case
            if (r != 0)
            {
                LogFtl("Socket failed to close properly: [%d], %s", r, zmq_strerror(r));
            }
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

        while (mStatus > 0)
        {
            using namespace std::chrono_literals;
            // Wait a bit before each iteration to not exhaust resources
            std::this_thread::sleep_for(50ms);
            // Acquire exclusive access to the socket
            std::lock_guard< std::mutex > guard(mMtx);
            // Perform tasks
            Recv();
            Send();
            SendMore();
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
            mStatus = 0;
            // Yield exclusive access
            mMtx.unlock();
            // Wait for the thread
            mThread.join();
        }
        // Make sure it wasn't closed already
        if (mPtr != nullptr)
        {
            // Now close the socket
            int r = zmq_close(mPtr);
            // Forget about this socket
            mPtr = nullptr;
            // Validate result
            if (r != 0)
            {
                STHROWF("Unable to close socket: [%d] %s", r, zmq_strerror(r));
            }
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Queue a message to be sent through the socket.
    */
    void Send(ZMsg & msg)
    {
        // Queue it
        mInputQueue.enqueue(std::move(msg));
        // Initialize it back to default
        msg = ZMsg();
    }

    /* --------------------------------------------------------------------------------------------
     * Queue a message to be sent through the socket.
    */
    void Send(const SQChar * str, size_t length)
    {
        // Create a new message
        ZMsg msg(static_cast< SQInteger >(length));
        // Populate it with data
        std::memcpy(zmq_msg_data(msg), str, length);
        // Queue it
        mInputQueue.enqueue(std::move(msg));
    }

    /* --------------------------------------------------------------------------------------------
     * Queue a multi-part message to be sent through the socket.
    */
    void Send(List & list)
    {
        mInputListQueue.enqueue(std::make_unique< List >(std::move(list)));
    }

protected:

    /* --------------------------------------------------------------------------------------------
     * Receive one message from the socket.
    */
    void Recv()
    {
        // Need someone to receive the message
        ZMsg msg;
        // Ask for a message, if any
        int r = zmq_msg_recv(msg, mPtr, ZMQ_DONTWAIT);
        // Did we have a message?
        if (r >= 0)
        {
            mOutputQueue.enqueue(std::move(msg)); // Put it in the queue
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Send one message to the socket.
    */
    bool Send()
    {
        // Need someone to receive the message
        ZMsg msg;
        // Try to get a message from the queue
        if (mInputQueue.try_dequeue(msg))
        {
            // Attempt to send the message
            int r = zmq_msg_send(msg, mPtr, ZMQ_DONTWAIT);
            // Could we send what the message had?
            if (r != zmq_msg_size(msg))
            {
                LogErr("Unable to send data to socket: [%d], %s", r, zmq_strerror(r));
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
     * Send a multi-part message to the socket.
    */
    bool SendMore()
    {
        ListItem mp_msg;
        // Try to get a multi-part message from the queue
        if (mInputListQueue.try_dequeue(mp_msg))
        {
            // Need someone to receive the message
            ZMsg msg;
            // Send all message parts
            for (size_t i = 0, n = mp_msg->size(); i < n; ++i)
            {
                // Attempt to send the message
                int r = zmq_msg_send((*mp_msg)[i], mPtr, (i + 1) == n ? ZMQ_DONTWAIT : ZMQ_SNDMORE);
                // Could we send what the message had?
                if (r != zmq_msg_size(msg))
                {
                    LogErr("Unable to send multi-part data to socket: [%d], %s", r, zmq_strerror(r));
                }
            }
            // One item was found in the queue
            return true;
        }
        else
        {
            return false; // No item in the queue
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
    ZCtx::Ptr mPtr;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ZContext()
        : mPtr(std::make_shared< ZCtx >())
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Pointer constructor.
    */
    explicit ZContext(ZCtx::Ptr ptr)
        : mPtr(std::move(ptr))
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
        if (!mPtr)
        {
            STHROWF("Invalid context instance");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a context instance is referenced and return the context.
    */
    SQMOD_NODISCARD ZCtx & Valid() { Validate(); return *mPtr; }

    /* --------------------------------------------------------------------------------------------
     * Make sure a context instance is referenced and return the context.
    */
    SQMOD_NODISCARD const ZCtx & Valid() const { Validate(); return *mPtr; }

    /* --------------------------------------------------------------------------------------------
     * Make sure a context instance is referenced and return the reference.
    */
    SQMOD_NODISCARD ZCtx::Ptr & ValidRef() { Validate(); return mPtr; }

    /* --------------------------------------------------------------------------------------------
     * Make sure a context instance is referenced and return the reference.
    */
    SQMOD_NODISCARD const ZCtx::Ptr & ValidRef() const { Validate(); return mPtr; }

    /* --------------------------------------------------------------------------------------------
     * Check if a context instance is referenced.
    */
    SQMOD_NODISCARD bool IsNull() const
    {
        return static_cast< bool >(mPtr);
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
            STHROWF("Unable to set context option: [%d] %s", r, zmq_strerror(r));
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
            STHROWF("Unable to shutdown context: %s", zmq_strerror(r));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Helper function to create sockets.
    */
    SQMOD_NODISCARD LightObj Socket(int type) const;
};

/* ------------------------------------------------------------------------------------------------
 * Interface for ZMQ messages.
*/
struct ZMessage
{
    /* --------------------------------------------------------------------------------------------
     * Pointer to the interfaced message.
    */
    ZMsg::Ptr mHnd;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ZMessage()
        : mHnd()
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit message data and size constructor.
    */
    ZMessage(StackStrF & data)
        : mHnd(std::make_shared< ZMsg >(data.mLen))
    {
        // Copy the string in the memory buffer
        std::memcpy(zmq_msg_data(*mHnd), data.mPtr, static_cast< size_t >(data.mLen));
        /* normally you'd have to do static_cast< size_t >(data.mLen) * sizeof(SQChar) */
        /* but this SQChar is required to be 1 byte so we don't bother with it */
    }

    /* --------------------------------------------------------------------------------------------
     * Pointer constructor.
    */
    explicit ZMessage(ZMsg::Ptr ptr)
        : mHnd(std::move(ptr))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    ZMessage(const ZMessage &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    ZMessage(ZMessage &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ZMessage() = default;

    /* --------------------------------------------------------------------------------------------
     * Assignment operator.
    */
    ZMessage & operator = (const ZMessage &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment.
    */
    ZMessage & operator = (ZMessage &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Make sure a message instance is referenced.
    */
    void Validate() const
    {
        if (!mHnd)
        {
            STHROWF("Invalid message instance");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a message instance is referenced and return the message.
    */
    SQMOD_NODISCARD ZMsg & Valid() { Validate(); return *mHnd; }

    /* --------------------------------------------------------------------------------------------
     * Make sure a message instance is referenced and return the message.
    */
    SQMOD_NODISCARD const ZMsg & Valid() const { Validate(); return *mHnd; }

    /* --------------------------------------------------------------------------------------------
     * Make sure a message instance is referenced and return the reference.
    */
    SQMOD_NODISCARD ZMsg::Ptr & ValidRef() { Validate(); return mHnd; }

    /* --------------------------------------------------------------------------------------------
     * Make sure a message instance is referenced and return the reference.
    */
    SQMOD_NODISCARD const ZMsg::Ptr & ValidRef() const { Validate(); return mHnd; }

    /* --------------------------------------------------------------------------------------------
     * Check if a message instance is referenced.
    */
    SQMOD_NODISCARD bool IsNull() const
    {
        return static_cast< bool >(mHnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value of a property.
    */
    SQMOD_NODISCARD int Get(int opt) const
    {
        return zmq_msg_get(Valid(), opt);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value of an property.
    */
    ZMessage & Set(int prop, int value)
    {
        int r = zmq_msg_set(Valid(), prop, value);
        // Validate result
        if (r != 0)
        {
            STHROWF("Unable to set message option: [%d] %s", r, zmq_strerror(r));
        }
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value of a meta-data property.
    */
    SQMOD_NODISCARD const SQChar * Meta(StackStrF & prop) const
    {
        return zmq_msg_gets (Valid(), prop.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy another message.
    */
    ZMessage & Copy(ZMessage & msg)
    {
        int r = zmq_msg_copy(Valid(), msg.Valid());
        // Validate result
        if (r != 0)
        {
            STHROWF("Unable to copy message: [%d] %s", r, zmq_strerror(r));
        }
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Indicate if there are more message parts to receive.
    */
    SQMOD_NODISCARD bool More() const
    {
        return static_cast< bool >(zmq_msg_more(Valid()));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve message content size in bytes.
    */
    SQMOD_NODISCARD SQInteger GetSize() const
    {
        return static_cast< SQInteger >(zmq_msg_size(Valid()));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message data as a string.
    */
    SQMOD_NODISCARD LightObj ToString() const
    {
        return LightObj(static_cast< const SQChar * >(zmq_msg_data(Valid())), GetSize());
    }

    /* --------------------------------------------------------------------------------------------
     * Generate a message from a string.
    */
    ZMessage & FromString(StackStrF & data)
    {
        // If there's no handle we make one
        if (!mHnd)
        {
            mHnd = (std::make_shared< ZMsg >(data.mLen));
        }
        else
        {
            (*mHnd) = ZMsg(data.mLen); // Update the current one
        }
        // Copy the string in the memory buffer
        std::memcpy(zmq_msg_data(*mHnd), data.mPtr, static_cast< size_t >(data.mLen));
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message data as a buffer.
    */
    SQMOD_NODISCARD LightObj ToBuffer() const
    {
        Validate();
        // Create the buffer instance and return it
        return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(),
                        Buffer(static_cast< Buffer::ConstPtr >(zmq_msg_data(*mHnd)), zmq_msg_size(*mHnd), 0));
    }

    /* --------------------------------------------------------------------------------------------
     * Generate a message from a buffer.
    */
    ZMessage & FromBuffer(SqBuffer & data)
    {
        data.ValidateDeeper();
        // If there's no handle we make one
        if (!mHnd)
        {
            mHnd = (std::make_shared< ZMsg >(data.GetPosition()));
        }
        else
        {
            (*mHnd) = ZMsg(data.GetPosition()); // Update the current one
        }

        // Copy the string in the memory buffer
        std::memcpy(zmq_msg_data(*mHnd), data.GetRef()->Data(), static_cast< size_t >(data.GetPosition()));
        // Allow chaining
        return *this;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Interface for ZMQ sockets.
*/
struct ZSocket
{
    /* --------------------------------------------------------------------------------------------
     * Pointer to the interfaced socket.
    */
    ZSkt::Ptr   mHnd;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ZSocket(const ZContext & ctx, int type)
        : mHnd(std::make_shared< ZSkt >(ctx.Valid(), type))
    {
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
            STHROWF("Unable to bind socket: [%d] %s", r, zmq_strerror(r));
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
            STHROWF("Unable to connect socket: [%d] %s", r, zmq_strerror(r));
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
            STHROWF("Unable to disconnect socket: [%d] %s", r, zmq_strerror(r));
        }
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Run the managed socket.
    */
    ZSocket & Run()
    {
        // Make sure thread exists already
        if (Valid().mThread.joinable())
        {
            STHROWF("Socket is already running");
        }
        // Allow the thread to run
        mHnd->mStatus = 1;
        // Now we can create the thread
        Valid().mThread = std::thread(&ZSkt::Proc, &Valid());
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
     * Send a ZMQ message to the socket.
    */
    ZSocket & SendMessage(ZMessage & msg)
    {
        Valid().Send(msg.Valid());
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Send a string message to the socket.
    */
    ZSocket & SendString(StackStrF & msg)
    {
        Valid().Send(msg.mPtr, msg.GetSize());
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Send a multi-part ZMQ message to the socket.
    */
    ZSocket & SendMessages(Array & arr)
    {
        Validate();

        ZSkt::List list;
        // Extract the messages from the array
        arr.Foreach([&list](HSQUIRRELVM vm, SQInteger) {
            // Extract the message from the stack
            ZMessage * msg = ClassType< ZMessage >::GetInstance(vm, -1);
            // In case we didn't fail at the null part (it should)
            if (msg && msg->mHnd->mPtr)
            {
                // Add the message to the list
                list.emplace_back(std::move((*msg->mHnd)));
                // Initialize it back to default
                (*msg->mHnd) = ZMsg();
            }
            // Continue
            return SQ_OK;
        });
        // Send the message list
        mHnd->Send(list);
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Send a multi-part string message to the socket.
    */
    ZSocket & SendStrings(Array & arr)
    {
        Validate();

        ZSkt::List list;
        // Extract the messages from the array
        arr.Foreach([&list](HSQUIRRELVM vm, SQInteger) {
            StackStrF str(vm, -1);
            // Extract the string from the stack
            if (SQ_FAILED(str.Proc(false)))
            {
                return str.mRes; // Abort
            }
            // Create a new message
            list.emplace_back(str.mLen);
            // Populate it with data
            std::memcpy(zmq_msg_data(list.back()), str.mPtr, static_cast< size_t >(str.mLen));
            // Continue
            return SQRESULT(SQ_OK);
        });
        // Send the message list
        mHnd->Send(list);
        // Allow chaining
        return *this;
    }
};


} // Namespace:: SqMod
