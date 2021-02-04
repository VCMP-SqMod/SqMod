// ------------------------------------------------------------------------------------------------
#include "Library/ZMQ.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqZContext, _SC("SqZmqContext"))
SQMOD_DECL_TYPENAME(SqZSocket, _SC("SqZmqSocket"))

// ------------------------------------------------------------------------------------------------
static void FlushSingleString(HSQUIRRELVM, Function & callback, Buffer & data)
{
    // Transform the message into a script object
    LightObj o(static_cast< const SQChar * >(data.Get()),
                static_cast< SQInteger >(data.Position()));
    // Forward it to the callback
    callback(o, false);
}

// ------------------------------------------------------------------------------------------------
static void FlushSingleBuffer(HSQUIRRELVM vm, Function & callback, Buffer & data)
{
    // Transform the message into a script object
    LightObj o(SqTypeIdentity< SqBuffer >{}, vm, std::move(data));
    // Forward it to the callback
    callback(o, false);
}

// ------------------------------------------------------------------------------------------------
static void FlushMultiString(HSQUIRRELVM vm, Function & callback, ZMsg::List & list)
{
    // Create a script array
    Array a(vm);
    // Reserve space upfront
    a.Reserve(static_cast< SQInteger >(list.size()));
    // Populate the array with elements from the list
    a.AppendFromCounted([&list](HSQUIRRELVM vm, SQInteger i) -> bool {
        // Are we still withing range of our list?
        if (static_cast< size_t >(i) < list.size())
        {
            // Transform the message into a script object
            sq_pushstring(vm, static_cast< const SQChar * >(list[i].Get()),
                                static_cast< SQInteger >(list[i].Position()));
            // We have an element on the stack
            return true;
        }
        // We don't have an element on the stack
        return false;
    });
    // Forward it to the callback
    callback(a, true);
}

// ------------------------------------------------------------------------------------------------
static void FlushMultiBuffer(HSQUIRRELVM vm, Function & callback, ZMsg::List & list)
{
    // Create a script array
    Array a(vm);
    // Reserve space upfront
    a.Reserve(static_cast< SQInteger >(list.size()));
    // Populate the array with elements from the list
    a.AppendFromCounted([&list](HSQUIRRELVM vm, SQInteger i) -> bool {
        // Are we still withing range of our list?
        if (static_cast< size_t >(i) < list.size())
        {
            // Transform the message into a script object
            LightObj o(SqTypeIdentity< SqBuffer >{}, vm, std::move(list[i]));
            // Push it on the stack
            Var< LightObj >::push(vm, o);
            // We have an element on the stack
            return true;
        }
        // We don't have an element on the stack
        return false;
    });
    // Forward it to the callback
    callback(a, true);
}

// ------------------------------------------------------------------------------------------------
void ZSkt::Flush(HSQUIRRELVM vm)
{
    // Need someone to receive the message
    Item item;
    // Try to get a message from the queue
    while (mOutputQueue.try_dequeue(item))
    {
        // Is there a callback to receive the message?
        if (!mOnData.IsNull())
        {
            // Is this a multi-part message?
            if (!(item->mMulti))
            {
                if (mStringMessages)
                {
                    FlushSingleString(vm, mOnData, item->mBuff);
                }
                else
                {
                    FlushSingleBuffer(vm, mOnData, item->mBuff);
                }
            }
            else
            {
                if (mStringMessages)
                {
                    FlushMultiString(vm, mOnData, item->mList);
                }
                else
                {
                    FlushMultiBuffer(vm, mOnData, item->mList);
                }
            }
        }
    }
}

// ------------------------------------------------------------------------------------------------
LightObj ZContext::Socket(int type) const
{
    return LightObj(SqTypeIdentity< ZSocket >{}, SqVM(), *this, type);
}
// ------------------------------------------------------------------------------------------------
LightObj ZSocket::GetOpt(int opt)
{
    int r = 0;
    // Acquire exclusive access to the socket
    std::lock_guard< std::mutex > guard(Valid().mMtx);
    // Identify option
    switch (opt)
    {
        // int
        case ZMQ_BACKLOG:
        case ZMQ_CONFLATE:
        case ZMQ_CONNECT_TIMEOUT:
        case ZMQ_CURVE_SERVER:
        case ZMQ_GSSAPI_PLAINTEXT:
        case ZMQ_GSSAPI_SERVER:
        case ZMQ_GSSAPI_PRINCIPAL_NAMETYPE:
        case ZMQ_HANDSHAKE_IVL:
        case ZMQ_HEARTBEAT_IVL:
        case ZMQ_HEARTBEAT_TIMEOUT:
        case ZMQ_HEARTBEAT_TTL:
        case ZMQ_IMMEDIATE:
        case ZMQ_INVERT_MATCHING:
        case ZMQ_IPV6:
        case ZMQ_LINGER:
        case ZMQ_MULTICAST_HOPS:
        case ZMQ_MULTICAST_MAXTPDU:
        case ZMQ_PLAIN_SERVER:
        case ZMQ_USE_FD:
        case ZMQ_PROBE_ROUTER:
        case ZMQ_RATE:
        case ZMQ_RCVBUF:
        case ZMQ_RCVHWM:
        case ZMQ_RCVTIMEO:
        case ZMQ_RECONNECT_IVL:
        case ZMQ_RECONNECT_IVL_MAX:
        case ZMQ_RECOVERY_IVL:
        case ZMQ_REQ_CORRELATE:
        case ZMQ_REQ_RELAXED:
        case ZMQ_ROUTER_HANDOVER:
        case ZMQ_ROUTER_MANDATORY:
        case ZMQ_ROUTER_RAW:
        case ZMQ_SNDBUF:
        case ZMQ_SNDHWM:
        case ZMQ_SNDTIMEO:
        case ZMQ_STREAM_NOTIFY:
        case ZMQ_TCP_KEEPALIVE:
        case ZMQ_TCP_KEEPALIVE_CNT:
        case ZMQ_TCP_KEEPALIVE_IDLE:
        case ZMQ_TCP_KEEPALIVE_INTVL:
        case ZMQ_TCP_MAXRT:
        case ZMQ_TOS:
        case ZMQ_XPUB_VERBOSE:
        case ZMQ_XPUB_VERBOSER:
        case ZMQ_XPUB_MANUAL:
        case ZMQ_XPUB_NODROP:
        case ZMQ_IPV4ONLY:
        case ZMQ_VMCI_CONNECT_TIMEOUT: {
            int out;
            size_t len = sizeof(out);
            r = zmq_getsockopt(Valid(), opt, &out, &len);
            // Validate and return it
            if (r == 0)
            {
                sq_pushinteger(SqVM(), out);
                LightObj o(-1);
                sq_poptop(SqVM());
                return o;
            }
        } break;
        // character string
        case ZMQ_BINDTODEVICE:
        case ZMQ_GSSAPI_PRINCIPAL:
        case ZMQ_GSSAPI_SERVICE_PRINCIPAL:
        case ZMQ_PLAIN_PASSWORD:
        case ZMQ_PLAIN_USERNAME:
        case ZMQ_SOCKS_PROXY:
        case ZMQ_ZAP_DOMAIN: {
            SQChar out[1024]; // Let's hope this is reasonable. I really don't care much about this feature.
            size_t len = sizeof(out);
            r = zmq_getsockopt(Valid(), opt, &out, &len);
            // Validate and return it
            if (r == 0)
            {
                return LightObj(out, static_cast< SQInteger >(len));
            }
        } break;
        // uint64_t
        case ZMQ_AFFINITY:
        case ZMQ_VMCI_BUFFER_SIZE:
        case ZMQ_VMCI_BUFFER_MIN_SIZE:
        case ZMQ_VMCI_BUFFER_MAX_SIZE: {
            uint64_t out;
            size_t len = sizeof(out);
            r = zmq_getsockopt(Valid(), opt, &out, &len);
            // Validate and return it
            if (r == 0)
            {
                sq_pushinteger(SqVM(), static_cast< SQInteger >(out));
                LightObj o(-1);
                sq_poptop(SqVM());
                return o;
            }
        } break;
        // int64_t
        case ZMQ_MAXMSGSIZE: {
            int64_t out;
            size_t len = sizeof(out);
            r = zmq_getsockopt(Valid(), opt, &out, &len);
            // Validate and return it
            if (r == 0)
            {
                sq_pushinteger(SqVM(), static_cast< SQInteger >(out));
                LightObj o(-1);
                sq_poptop(SqVM());
                return o;
            }
        } break;
        // binary data
        case ZMQ_CONNECT_ROUTING_ID:
        case ZMQ_CURVE_PUBLICKEY:
        case ZMQ_CURVE_SECRETKEY:
        case ZMQ_CURVE_SERVERKEY:
        case ZMQ_ROUTING_ID:
        case ZMQ_SUBSCRIBE:
        case ZMQ_UNSUBSCRIBE:
        case ZMQ_XPUB_WELCOME_MSG:
        case ZMQ_TCP_ACCEPT_FILTER: {
            Buffer out(4096); // Let's hope this is reasonable. I really don't care much about this feature.
            size_t len = out.Size();
            r = zmq_getsockopt(Valid(), opt, &out, &len);
            out.Move(static_cast< SQInteger >(len));
            // Validate and return it
            if (r == 0)
            {
                return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(), std::move(out));
            }
        } break;
        default: STHROWF("Unknown socket option");
    }
    // Validate result
    if (r != 0)
    {
        STHROWF("Unable to retrieve socket option: [{}] {}", r, zmq_strerror(errno));
    }
    SQ_UNREACHABLE
    // Never reaches here
    return LightObj();
}

// ------------------------------------------------------------------------------------------------
void ZSocket::SetOpt(int opt, LightObj & value)
{
    int r = 0;
    // Acquire exclusive access to the socket
    std::lock_guard< std::mutex > guard(Valid().mMtx);
    // Identify option
    switch (opt)
    {
        // int
        case ZMQ_BACKLOG:
        case ZMQ_CONFLATE:
        case ZMQ_CONNECT_TIMEOUT:
        case ZMQ_CURVE_SERVER:
        case ZMQ_GSSAPI_PLAINTEXT:
        case ZMQ_GSSAPI_SERVER:
        case ZMQ_GSSAPI_PRINCIPAL_NAMETYPE:
        case ZMQ_HANDSHAKE_IVL:
        case ZMQ_HEARTBEAT_IVL:
        case ZMQ_HEARTBEAT_TIMEOUT:
        case ZMQ_HEARTBEAT_TTL:
        case ZMQ_IMMEDIATE:
        case ZMQ_INVERT_MATCHING:
        case ZMQ_IPV6:
        case ZMQ_LINGER:
        case ZMQ_MULTICAST_HOPS:
        case ZMQ_MULTICAST_MAXTPDU:
        case ZMQ_PLAIN_SERVER:
        case ZMQ_USE_FD:
        case ZMQ_PROBE_ROUTER:
        case ZMQ_RATE:
        case ZMQ_RCVBUF:
        case ZMQ_RCVHWM:
        case ZMQ_RCVTIMEO:
        case ZMQ_RECONNECT_IVL:
        case ZMQ_RECONNECT_IVL_MAX:
        case ZMQ_RECOVERY_IVL:
        case ZMQ_REQ_CORRELATE:
        case ZMQ_REQ_RELAXED:
        case ZMQ_ROUTER_HANDOVER:
        case ZMQ_ROUTER_MANDATORY:
        case ZMQ_ROUTER_RAW:
        case ZMQ_SNDBUF:
        case ZMQ_SNDHWM:
        case ZMQ_SNDTIMEO:
        case ZMQ_STREAM_NOTIFY:
        case ZMQ_TCP_KEEPALIVE:
        case ZMQ_TCP_KEEPALIVE_CNT:
        case ZMQ_TCP_KEEPALIVE_IDLE:
        case ZMQ_TCP_KEEPALIVE_INTVL:
        case ZMQ_TCP_MAXRT:
        case ZMQ_TOS:
        case ZMQ_XPUB_VERBOSE:
        case ZMQ_XPUB_VERBOSER:
        case ZMQ_XPUB_MANUAL:
        case ZMQ_XPUB_NODROP:
        case ZMQ_IPV4ONLY:
        case ZMQ_VMCI_CONNECT_TIMEOUT: {
            auto in = value.Cast< int >();
            r = zmq_setsockopt(Valid(), opt, &in, sizeof(in));
        } break;
        // character string
        case ZMQ_BINDTODEVICE:
        case ZMQ_GSSAPI_PRINCIPAL:
        case ZMQ_GSSAPI_SERVICE_PRINCIPAL:
        case ZMQ_PLAIN_PASSWORD:
        case ZMQ_PLAIN_USERNAME:
        case ZMQ_SOCKS_PROXY:
        case ZMQ_ZAP_DOMAIN: {
            Var<LightObj>::push(SqVM(), value);
            StackStrF str(SqVM(), -1);
            if (SQ_SUCCEEDED(str.Proc(false)))
            {
                r = zmq_setsockopt(Valid(), opt, str.mPtr, str.GetSize());
            } else r = -1;
            sq_poptop(SqVM());
        } break;
        // uint64_t
        case ZMQ_AFFINITY:
        case ZMQ_VMCI_BUFFER_SIZE:
        case ZMQ_VMCI_BUFFER_MIN_SIZE:
        case ZMQ_VMCI_BUFFER_MAX_SIZE: {
            auto in = value.Cast< uint64_t >();
            r = zmq_setsockopt(Valid(), opt, &in, sizeof(in));
        } break;
        // int64_t
        case ZMQ_MAXMSGSIZE: {
            auto in = value.Cast< int64_t >();
            r = zmq_setsockopt(Valid(), opt, &in, sizeof(in));
        } break;
        // binary data
        case ZMQ_CONNECT_ROUTING_ID:
        case ZMQ_CURVE_PUBLICKEY:
        case ZMQ_CURVE_SECRETKEY:
        case ZMQ_CURVE_SERVERKEY:
        case ZMQ_ROUTING_ID:
        case ZMQ_SUBSCRIBE:
        case ZMQ_UNSUBSCRIBE:
        case ZMQ_XPUB_WELCOME_MSG:
        case ZMQ_TCP_ACCEPT_FILTER: {
            // Is this a string?
            if (value.GetType() == OT_STRING)
            {

                // Push the stringobject on the stack
                Var< LightObj >::push(SqVM(), value);
                // Prepare for string extraction
                StackStrF str(SqVM(), -1);
                // Attempt to extract the string
                if (SQ_SUCCEEDED(str.Proc(false)))
                {
                    r = zmq_setsockopt(Valid(), opt, str.mPtr, static_cast< size_t >(str.mLen));
                    // Pop the string object from the stack
                    sq_poptop(SqVM());
                }
                // Failed
                else
                {
                    STHROWF("Invalid string value");
                }
            }
            // Is this a binary buffer?
            else if (value.GetTypeTag() != StaticClassTypeTag< SqBuffer >::Get())
            {
                 STHROWF("Invalid buffer value");
            }
            else
            {
                auto * inst = value.CastI< SqBuffer >();
                // Validate the instance
                if (inst)
                {
                    r = zmq_setsockopt(Valid(), opt, inst->GetRef()->Data(), static_cast< size_t >(inst->GetPosition()));
                // Failed
                }
                else
                {
                    STHROWF("Invalid instance value");
                }
            }
        } break;
        default: STHROWF("Unknown socket option");
    }
    // Validate result
    if (r != 0)
    {
        STHROWF("Unable to modify socket option: [{}] {}", r, zmq_strerror(errno));
    }
}

// ------------------------------------------------------------------------------------------------
static String SqZmqVersion()
{
    int major=0, minor=0, patch=0;
    zmq_version(&major, &minor, &patch);
    return fmt::format("{}.{}.{}", major, minor, patch);
}

// ------------------------------------------------------------------------------------------------
static void SqZmqProcess()
{
    // Go over all sockets and try to update them
    for (ZSkt * inst = ZSkt::sHead; inst && inst->mNext != ZSkt::sHead; inst = inst->mNext)
    {
        // Flush pending messages
        inst->Flush(SqVM());
    }
}

// ------------------------------------------------------------------------------------------------
void ZmqTerminate()
{
    // Go over all sockets and try to close them
    for (ZSkt * inst = ZSkt::sHead; inst && inst->mNext != ZSkt::sHead; inst = inst->mNext)
    {
        // Close the socket
        inst->Close();
        // Flush pending messages
        inst->Flush(SqVM());
    }
}

// ================================================================================================
void Register_ZMQ(HSQUIRRELVM vm)
{
    Table ns(vm);

    ns.Func(_SC("Process"), &SqZmqProcess);
    ns.Func(_SC("Version"), &SqZmqVersion);

    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Context"),
        Class< ZContext, NoCopy< ZContext > >(vm, SqZContext::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqZContext::Fn)
        // Properties
        .Prop(_SC("IsNull"), &ZContext::IsNull)
        // Member Methods
        .Func(_SC("Get"), &ZContext::Get)
        .Func(_SC("Set"), &ZContext::Set)
        .Func(_SC("Shutdown"), &ZContext::Shutdown)
        .Func(_SC("Socket"), &ZContext::Socket)
    );

    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Socket"),
        Class< ZSocket, NoCopy< ZSocket > >(vm, SqZSocket::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqZSocket::Fn)
        // Properties
        .Prop(_SC("IsNull"), &ZSocket::IsNull)
        .Prop(_SC("StringMessages"), &ZSocket::GetStringMessages, &ZSocket::SetStringMessages)
        // Member Methods
        .CbFunc(_SC("OnData"), &ZSocket::OnData)
        .FmtFunc(_SC("Bind"), &ZSocket::Bind)
        .FmtFunc(_SC("Connect"), &ZSocket::Connect)
        .FmtFunc(_SC("Disconnect"), &ZSocket::Disconnect)
        .Func(_SC("Close"), &ZSocket::Close)
        .Func(_SC("SendBuffer"), &ZSocket::SendBuffer)
        .FmtFunc(_SC("SendString"), &ZSocket::SendString)
        .Func(_SC("SendBuffers"), &ZSocket::SendBuffers)
        .Func(_SC("SendStrings"), &ZSocket::SendStrings)
        .Func(_SC("GetOpt"), &ZSocket::GetOpt)
        .Func(_SC("SetOpt"), &ZSocket::SetOpt)
    );

    RootTable(vm).Bind(_SC("SqZMQ"), ns);

    ConstTable(vm).Enum(_SC("SqZmq"), Enumeration(vm)
        /* Context options  */
        .Const(_SC("IO_THREADS"),                                           int32_t(ZMQ_IO_THREADS))
        .Const(_SC("MAX_SOCKETS"),                                          int32_t(ZMQ_MAX_SOCKETS))
        .Const(_SC("SOCKET_LIMIT"),                                         int32_t(ZMQ_SOCKET_LIMIT))
        .Const(_SC("THREAD_PRIORITY"),                                      int32_t(ZMQ_THREAD_PRIORITY))
        .Const(_SC("THREAD_SCHED_POLICY"),                                  int32_t(ZMQ_THREAD_SCHED_POLICY))
        .Const(_SC("MAX_MSGSZ"),                                            int32_t(ZMQ_MAX_MSGSZ))
        .Const(_SC("MSG_T_SIZE"),                                           int32_t(ZMQ_MSG_T_SIZE))
        .Const(_SC("THREAD_AFFINITY_CPU_ADD"),                              int32_t(ZMQ_THREAD_AFFINITY_CPU_ADD))
        .Const(_SC("THREAD_AFFINITY_CPU_REMOVE"),                           int32_t(ZMQ_THREAD_AFFINITY_CPU_REMOVE))
        .Const(_SC("THREAD_NAME_PREFIX"),                                   int32_t(ZMQ_THREAD_NAME_PREFIX))
        /* Socket types. */
        .Const(_SC("PAIR"),                                                 int32_t(ZMQ_PAIR))
        .Const(_SC("PUB"),                                                  int32_t(ZMQ_PUB))
        .Const(_SC("SUB"),                                                  int32_t(ZMQ_SUB))
        .Const(_SC("REQ"),                                                  int32_t(ZMQ_REQ))
        .Const(_SC("REP"),                                                  int32_t(ZMQ_REP))
        .Const(_SC("DEALER"),                                               int32_t(ZMQ_DEALER))
        .Const(_SC("ROUTER"),                                               int32_t(ZMQ_ROUTER))
        .Const(_SC("PULL"),                                                 int32_t(ZMQ_PULL))
        .Const(_SC("PUSH"),                                                 int32_t(ZMQ_PUSH))
        .Const(_SC("XPUB"),                                                 int32_t(ZMQ_XPUB))
        .Const(_SC("XSUB"),                                                 int32_t(ZMQ_XSUB))
        .Const(_SC("STREAM"),                                               int32_t(ZMQ_STREAM))
        /* Socket options */
        .Const(_SC("AFFINITY"),                                             int32_t(ZMQ_AFFINITY))
        .Const(_SC("ROUTING_ID"),                                           int32_t(ZMQ_ROUTING_ID))
        .Const(_SC("SUBSCRIBE"),                                            int32_t(ZMQ_SUBSCRIBE))
        .Const(_SC("UNSUBSCRIBE"),                                          int32_t(ZMQ_UNSUBSCRIBE))
        .Const(_SC("RATE"),                                                 int32_t(ZMQ_RATE))
        .Const(_SC("RECOVERY_IVL"),                                         int32_t(ZMQ_RECOVERY_IVL))
        .Const(_SC("SNDBUF"),                                               int32_t(ZMQ_SNDBUF))
        .Const(_SC("RCVBUF"),                                               int32_t(ZMQ_RCVBUF))
        .Const(_SC("RCVMORE"),                                              int32_t(ZMQ_RCVMORE))
        .Const(_SC("FD"),                                                   int32_t(ZMQ_FD))
        .Const(_SC("EVENTS"),                                               int32_t(ZMQ_EVENTS))
        .Const(_SC("TYPE"),                                                 int32_t(ZMQ_TYPE))
        .Const(_SC("LINGER"),                                               int32_t(ZMQ_LINGER))
        .Const(_SC("RECONNECT_IVL"),                                        int32_t(ZMQ_RECONNECT_IVL))
        .Const(_SC("BACKLOG"),                                              int32_t(ZMQ_BACKLOG))
        .Const(_SC("RECONNECT_IVL_MAX"),                                    int32_t(ZMQ_RECONNECT_IVL_MAX))
        .Const(_SC("MAXMSGSIZE"),                                           int32_t(ZMQ_MAXMSGSIZE))
        .Const(_SC("SNDHWM"),                                               int32_t(ZMQ_SNDHWM))
        .Const(_SC("RCVHWM"),                                               int32_t(ZMQ_RCVHWM))
        .Const(_SC("MULTICAST_HOPS"),                                       int32_t(ZMQ_MULTICAST_HOPS))
        .Const(_SC("RCVTIMEO"),                                             int32_t(ZMQ_RCVTIMEO))
        .Const(_SC("SNDTIMEO"),                                             int32_t(ZMQ_SNDTIMEO))
        .Const(_SC("LAST_ENDPOINT"),                                        int32_t(ZMQ_LAST_ENDPOINT))
        .Const(_SC("ROUTER_MANDATORY"),                                     int32_t(ZMQ_ROUTER_MANDATORY))
        .Const(_SC("TCP_KEEPALIVE"),                                        int32_t(ZMQ_TCP_KEEPALIVE))
        .Const(_SC("TCP_KEEPALIVE_CNT"),                                    int32_t(ZMQ_TCP_KEEPALIVE_CNT))
        .Const(_SC("TCP_KEEPALIVE_IDLE"),                                   int32_t(ZMQ_TCP_KEEPALIVE_IDLE))
        .Const(_SC("TCP_KEEPALIVE_INTVL"),                                  int32_t(ZMQ_TCP_KEEPALIVE_INTVL))
        .Const(_SC("IMMEDIATE"),                                            int32_t(ZMQ_IMMEDIATE))
        .Const(_SC("XPUB_VERBOSE"),                                         int32_t(ZMQ_XPUB_VERBOSE))
        .Const(_SC("ROUTER_RAW"),                                           int32_t(ZMQ_ROUTER_RAW))
        .Const(_SC("IPV6"),                                                 int32_t(ZMQ_IPV6))
        .Const(_SC("MECHANISM"),                                            int32_t(ZMQ_MECHANISM))
        .Const(_SC("PLAIN_SERVER"),                                         int32_t(ZMQ_PLAIN_SERVER))
        .Const(_SC("PLAIN_USERNAME"),                                       int32_t(ZMQ_PLAIN_USERNAME))
        .Const(_SC("PLAIN_PASSWORD"),                                       int32_t(ZMQ_PLAIN_PASSWORD))
        .Const(_SC("CURVE_SERVER"),                                         int32_t(ZMQ_CURVE_SERVER))
        .Const(_SC("CURVE_PUBLICKEY"),                                      int32_t(ZMQ_CURVE_PUBLICKEY))
        .Const(_SC("CURVE_SECRETKEY"),                                      int32_t(ZMQ_CURVE_SECRETKEY))
        .Const(_SC("CURVE_SERVERKEY"),                                      int32_t(ZMQ_CURVE_SERVERKEY))
        .Const(_SC("PROBE_ROUTER"),                                         int32_t(ZMQ_PROBE_ROUTER))
        .Const(_SC("REQ_CORRELATE"),                                        int32_t(ZMQ_REQ_CORRELATE))
        .Const(_SC("REQ_RELAXED"),                                          int32_t(ZMQ_REQ_RELAXED))
        .Const(_SC("CONFLATE"),                                             int32_t(ZMQ_CONFLATE))
        .Const(_SC("ZAP_DOMAIN"),                                           int32_t(ZMQ_ZAP_DOMAIN))
        .Const(_SC("ROUTER_HANDOVER"),                                      int32_t(ZMQ_ROUTER_HANDOVER))
        .Const(_SC("TOS"),                                                  int32_t(ZMQ_TOS))
        .Const(_SC("CONNECT_ROUTING_ID"),                                   int32_t(ZMQ_CONNECT_ROUTING_ID))
        .Const(_SC("GSSAPI_SERVER"),                                        int32_t(ZMQ_GSSAPI_SERVER))
        .Const(_SC("GSSAPI_PRINCIPAL"),                                     int32_t(ZMQ_GSSAPI_PRINCIPAL))
        .Const(_SC("GSSAPI_SERVICE_PRINCIPAL"),                             int32_t(ZMQ_GSSAPI_SERVICE_PRINCIPAL))
        .Const(_SC("GSSAPI_PLAINTEXT"),                                     int32_t(ZMQ_GSSAPI_PLAINTEXT))
        .Const(_SC("HANDSHAKE_IVL"),                                        int32_t(ZMQ_HANDSHAKE_IVL))
        .Const(_SC("SOCKS_PROXY"),                                          int32_t(ZMQ_SOCKS_PROXY))
        .Const(_SC("XPUB_NODROP"),                                          int32_t(ZMQ_XPUB_NODROP))
        .Const(_SC("BLOCKY"),                                               int32_t(ZMQ_BLOCKY))
        .Const(_SC("XPUB_MANUAL"),                                          int32_t(ZMQ_XPUB_MANUAL))
        .Const(_SC("XPUB_WELCOME_MSG"),                                     int32_t(ZMQ_XPUB_WELCOME_MSG))
        .Const(_SC("STREAM_NOTIFY"),                                        int32_t(ZMQ_STREAM_NOTIFY))
        .Const(_SC("INVERT_MATCHING"),                                      int32_t(ZMQ_INVERT_MATCHING))
        .Const(_SC("HEARTBEAT_IVL"),                                        int32_t(ZMQ_HEARTBEAT_IVL))
        .Const(_SC("HEARTBEAT_TTL"),                                        int32_t(ZMQ_HEARTBEAT_TTL))
        .Const(_SC("HEARTBEAT_TIMEOUT"),                                    int32_t(ZMQ_HEARTBEAT_TIMEOUT))
        .Const(_SC("XPUB_VERBOSER"),                                        int32_t(ZMQ_XPUB_VERBOSER))
        .Const(_SC("CONNECT_TIMEOUT"),                                      int32_t(ZMQ_CONNECT_TIMEOUT))
        .Const(_SC("TCP_MAXRT"),                                            int32_t(ZMQ_TCP_MAXRT))
        .Const(_SC("THREAD_SAFE"),                                          int32_t(ZMQ_THREAD_SAFE))
        .Const(_SC("MULTICAST_MAXTPDU"),                                    int32_t(ZMQ_MULTICAST_MAXTPDU))
        .Const(_SC("VMCI_BUFFER_SIZE"),                                     int32_t(ZMQ_VMCI_BUFFER_SIZE))
        .Const(_SC("VMCI_BUFFER_MIN_SIZE"),                                 int32_t(ZMQ_VMCI_BUFFER_MIN_SIZE))
        .Const(_SC("VMCI_BUFFER_MAX_SIZE"),                                 int32_t(ZMQ_VMCI_BUFFER_MAX_SIZE))
        .Const(_SC("VMCI_CONNECT_TIMEOUT"),                                 int32_t(ZMQ_VMCI_CONNECT_TIMEOUT))
        .Const(_SC("USE_FD"),                                               int32_t(ZMQ_USE_FD))
        .Const(_SC("GSSAPI_PRINCIPAL_NAMETYPE"),                            int32_t(ZMQ_GSSAPI_PRINCIPAL_NAMETYPE))
        .Const(_SC("GSSAPI_SERVICE_PRINCIPAL_NAMETYPE"),                    int32_t(ZMQ_GSSAPI_SERVICE_PRINCIPAL_NAMETYPE))
        .Const(_SC("BINDTODEVICE"),                                         int32_t(ZMQ_BINDTODEVICE))
        /* Message options */
        .Const(_SC("MORE"),                                                 int32_t(ZMQ_MORE))
        .Const(_SC("SHARED"),                                               int32_t(ZMQ_SHARED))
        /* Send/recv options. */
        .Const(_SC("DONTWAIT"),                                             int32_t(ZMQ_DONTWAIT))
        .Const(_SC("SNDMORE"),                                              int32_t(ZMQ_SNDMORE))
        .Const(_SC("NULL"),                                                 int32_t(ZMQ_NULL))
        .Const(_SC("PLAIN"),                                                int32_t(ZMQ_PLAIN))
        /* Security mechanisms */
        //.Const(_SC("CURVE"),                                                int32_t(ZMQ_CURVE))
        //.Const(_SC("GSSAPI"),                                               int32_t(ZMQ_GSSAPI))
        /* RADIO-DISH protocol */
        //.Const(_SC("GROUP_MAX_LENGTH"),                                     int32_t(ZMQ_GROUP_MAX_LENGTH))
        /* GSSAPI principal name types */
        //.Const(_SC("GSSAPI_NT_HOSTBASED"),                                  int32_t(ZMQ_GSSAPI_NT_HOSTBASED))
        //.Const(_SC("GSSAPI_NT_USER_NAME"),                                  int32_t(ZMQ_GSSAPI_NT_USER_NAME))
        //.Const(_SC("GSSAPI_NT_KRB5_PRINCIPAL"),                             int32_t(ZMQ_GSSAPI_NT_KRB5_PRINCIPAL))
        /* Socket transport events (TCP, IPC and TIPC only) */
        //.Const(_SC("EVENT_CONNECTED"),                                      int32_t(ZMQ_EVENT_CONNECTED))
        //.Const(_SC("EVENT_CONNECT_DELAYED"),                                int32_t(ZMQ_EVENT_CONNECT_DELAYED))
        //.Const(_SC("EVENT_CONNECT_RETRIED"),                                int32_t(ZMQ_EVENT_CONNECT_RETRIED))
        //.Const(_SC("EVENT_LISTENING"),                                      int32_t(ZMQ_EVENT_LISTENING))
        //.Const(_SC("EVENT_BIND_FAILED"),                                    int32_t(ZMQ_EVENT_BIND_FAILED))
        //.Const(_SC("EVENT_ACCEPTED"),                                       int32_t(ZMQ_EVENT_ACCEPTED))
        //.Const(_SC("EVENT_ACCEPT_FAILED"),                                  int32_t(ZMQ_EVENT_ACCEPT_FAILED))
        //.Const(_SC("EVENT_CLOSED"),                                         int32_t(ZMQ_EVENT_CLOSED))
        //.Const(_SC("EVENT_CLOSE_FAILED"),                                   int32_t(ZMQ_EVENT_CLOSE_FAILED))
        //.Const(_SC("EVENT_DISCONNECTED"),                                   int32_t(ZMQ_EVENT_DISCONNECTED))
        //.Const(_SC("EVENT_MONITOR_STOPPED"),                                int32_t(ZMQ_EVENT_MONITOR_STOPPED))
        //.Const(_SC("EVENT_ALL"),                                            int32_t(ZMQ_EVENT_ALL))
        //.Const(_SC("EVENT_HANDSHAKE_FAILED_NO_DETAIL"),                     int32_t(ZMQ_EVENT_HANDSHAKE_FAILED_NO_DETAIL))
        //.Const(_SC("EVENT_HANDSHAKE_SUCCEEDED"),                            int32_t(ZMQ_EVENT_HANDSHAKE_SUCCEEDED))
        //.Const(_SC("EVENT_HANDSHAKE_FAILED_PROTOCOL"),                      int32_t(ZMQ_EVENT_HANDSHAKE_FAILED_PROTOCOL))
        //.Const(_SC("EVENT_HANDSHAKE_FAILED_AUTH"),                          int32_t(ZMQ_EVENT_HANDSHAKE_FAILED_AUTH))
        //.Const(_SC("PROTOCOL_ERROR_ZMTP_UNSPECIFIED"),                      int32_t(ZMQ_PROTOCOL_ERROR_ZMTP_UNSPECIFIED))
        //.Const(_SC("PROTOCOL_ERROR_ZMTP_UNEXPECTED_COMMAND"),               int32_t(ZMQ_PROTOCOL_ERROR_ZMTP_UNEXPECTED_COMMAND))
        //.Const(_SC("PROTOCOL_ERROR_ZMTP_INVALID_SEQUENCE"),                 int32_t(ZMQ_PROTOCOL_ERROR_ZMTP_INVALID_SEQUENCE))
        //.Const(_SC("PROTOCOL_ERROR_ZMTP_KEY_EXCHANGE"),                     int32_t(ZMQ_PROTOCOL_ERROR_ZMTP_KEY_EXCHANGE))
        //.Const(_SC("PROTOCOL_ERROR_ZMTP_MALFORMED_COMMAND_UNSPECIFIED"),    int32_t(ZMQ_PROTOCOL_ERROR_ZMTP_MALFORMED_COMMAND_UNSPECIFIED))
        //.Const(_SC("PROTOCOL_ERROR_ZMTP_MALFORMED_COMMAND_MESSAGE"),        int32_t(ZMQ_PROTOCOL_ERROR_ZMTP_MALFORMED_COMMAND_MESSAGE))
        //.Const(_SC("PROTOCOL_ERROR_ZMTP_MALFORMED_COMMAND_HELLO"),          int32_t(ZMQ_PROTOCOL_ERROR_ZMTP_MALFORMED_COMMAND_HELLO))
        //.Const(_SC("PROTOCOL_ERROR_ZMTP_MALFORMED_COMMAND_INITIATE"),       int32_t(ZMQ_PROTOCOL_ERROR_ZMTP_MALFORMED_COMMAND_INITIATE))
        //.Const(_SC("PROTOCOL_ERROR_ZMTP_MALFORMED_COMMAND_ERROR"),          int32_t(ZMQ_PROTOCOL_ERROR_ZMTP_MALFORMED_COMMAND_ERROR))
        //.Const(_SC("PROTOCOL_ERROR_ZMTP_MALFORMED_COMMAND_READY"),          int32_t(ZMQ_PROTOCOL_ERROR_ZMTP_MALFORMED_COMMAND_READY))
        //.Const(_SC("PROTOCOL_ERROR_ZMTP_MALFORMED_COMMAND_WELCOME"),        int32_t(ZMQ_PROTOCOL_ERROR_ZMTP_MALFORMED_COMMAND_WELCOME))
        //.Const(_SC("PROTOCOL_ERROR_ZMTP_INVALID_METADATA"),                 int32_t(ZMQ_PROTOCOL_ERROR_ZMTP_INVALID_METADATA))
        //.Const(_SC("PROTOCOL_ERROR_ZMTP_CRYPTOGRAPHIC"),                    int32_t(ZMQ_PROTOCOL_ERROR_ZMTP_CRYPTOGRAPHIC))
        //.Const(_SC("PROTOCOL_ERROR_ZMTP_MECHANISM_MISMATCH"),               int32_t(ZMQ_PROTOCOL_ERROR_ZMTP_MECHANISM_MISMATCH))
        //.Const(_SC("PROTOCOL_ERROR_ZAP_UNSPECIFIED"),                       int32_t(ZMQ_PROTOCOL_ERROR_ZAP_UNSPECIFIED))
        //.Const(_SC("PROTOCOL_ERROR_ZAP_MALFORMED_REPLY"),                   int32_t(ZMQ_PROTOCOL_ERROR_ZAP_MALFORMED_REPLY))
        //.Const(_SC("PROTOCOL_ERROR_ZAP_BAD_REQUEST_ID"),                    int32_t(ZMQ_PROTOCOL_ERROR_ZAP_BAD_REQUEST_ID))
        //.Const(_SC("PROTOCOL_ERROR_ZAP_BAD_VERSION"),                       int32_t(ZMQ_PROTOCOL_ERROR_ZAP_BAD_VERSION))
        //.Const(_SC("PROTOCOL_ERROR_ZAP_INVALID_STATUS_CODE"),               int32_t(ZMQ_PROTOCOL_ERROR_ZAP_INVALID_STATUS_CODE))
        //.Const(_SC("PROTOCOL_ERROR_ZAP_INVALID_METADATA"),                  int32_t(ZMQ_PROTOCOL_ERROR_ZAP_INVALID_METADATA))
        //.Const(_SC("PROTOCOL_ERROR_WS_UNSPECIFIED"),                        int32_t(ZMQ_PROTOCOL_ERROR_WS_UNSPECIFIED))
    );
}

} // Namespace:: SqMod
