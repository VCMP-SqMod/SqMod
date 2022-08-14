// ------------------------------------------------------------------------------------------------
#include "Library/Net.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqWebSocketClient, _SC("SqWebSocketClient"))

// ------------------------------------------------------------------------------------------------
static std::thread::id sMainThreadID{}; // Main thread ID

// ------------------------------------------------------------------------------------------------
void InitializeNet()
{
    int f = MG_FEATURES_DEFAULT;
#ifndef NO_FILES
    f |= MG_FEATURES_FILES;
#endif
#ifndef NO_SSL
    f |= MG_FEATURES_SSL;
#else
    OutputMessage("Network compiled without SSL support.");
#endif
#ifndef NO_CGI
    f |= MG_FEATURES_CGI;
#endif
#ifndef NO_CACHING
    f |= MG_FEATURES_CACHE;
#endif
#ifdef USE_IPV6
    f |= MG_FEATURES_CGI;
#endif
#ifdef USE_WEBSOCKET
    f |= MG_FEATURES_WEBSOCKET;
#endif
#ifdef USE_SERVER_STATS
    f |= MG_FEATURES_STATS;
#endif
#ifdef USE_ZLIB
    f |= MG_FEATURES_COMPRESSION;
#endif
#ifdef USE_HTTP2
    f |= MG_FEATURES_HTTP2;
#endif
#ifdef USE_X_DOM_SOCKET
    f |= MG_FEATURES_X_DOMAIN_SOCKET;
#endif
    mg_init_library(f);
}

// ------------------------------------------------------------------------------------------------
void TerminateNet()
{
    // Go over all connections and try to terminate them
    for (WebSocketClient * inst = WebSocketClient::sHead; inst && inst->mNext != WebSocketClient::sHead; inst = inst->mNext)
    {
        inst->Terminate(); // Terminate() the connection
    }
}

// ------------------------------------------------------------------------------------------------
void ProcessNet()
{
    // Go over all connections and allow them to process data
    for (WebSocketClient * inst = WebSocketClient::sHead; inst && inst->mNext != WebSocketClient::sHead; inst = inst->mNext)
    {
        inst->Process();
    }
}

// ------------------------------------------------------------------------------------------------
WebSocketClient & WebSocketClient::Connect()
{
    // Make sure another connection does not exist
    Invalid();
    // Error buffer
    char err_buf[128] = {0};
    // Connect to the given WS or WSS (WS secure) server
    mHandle = mg_connect_websocket_client(mHost.c_str(), mPort, mSecure?1:0,
                                        err_buf, sizeof(err_buf), mPath.c_str(),
                                        mOrigin.empty() ? nullptr : mOrigin.c_str(),
                                        &WebSocketClient::DataHandler_,
                                        &WebSocketClient::CloseHandler_,
                                        this);
    // Check if connection was possible
    if (!mHandle)
    {
        STHROWF("Connection failed: {}", err_buf);
    }
    // Reset memebrs
    mClosing.store(false);
    mClosed.store(false);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
WebSocketClient & WebSocketClient::ConnectExt()
{
    // Make sure another connection does not exist
    Invalid();
    // Error buffer
    char err_buf[128] = {0};
    // Connect to the given WS or WSS (WS secure) server
    mHandle = mg_connect_websocket_client_extensions(mHost.c_str(), mPort, mSecure?1:0,
                                                    err_buf, sizeof(err_buf), mPath.c_str(),
                                                    mOrigin.empty() ? nullptr : mOrigin.c_str(),
                                                    mExtensions.empty() ? nullptr : mExtensions.c_str(),
                                                    &WebSocketClient::DataHandler_,
                                                    &WebSocketClient::CloseHandler_,
                                                    this);
    // Check if connection was possible
    if (!mHandle)
    {
        STHROWF("Connection failed: {}", err_buf);
    }
    // Reset memebrs
    mClosing.store(false);
    mClosed.store(false);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
int WebSocketClient::DataHandler(int flags, char * data, size_t data_len) noexcept
{
    // Create a frame instance to store information and queue it
    try
    {
        mQueue.enqueue(std::make_unique< Frame >(data, data_len, flags));
    }
    catch(...)
    {
        LogFtl("Failed to queue web-socket data");
    }
    // Should we auto-close the connection
    if (((flags & 0xF) == MG_WEBSOCKET_OPCODE_CONNECTION_CLOSE) && mAutoClose.load() == true)
    {
        return 0;
    }
    // Return 1 to keep the connection open
    return 1;
}

// ------------------------------------------------------------------------------------------------
void WebSocketClient::CloseHandler() noexcept
{
    mClosing.store(true);
}

// ================================================================================================
void Register_Net(HSQUIRRELVM vm)
{
    Table ns(vm);
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("WebSocketClient"),
        Class< WebSocketClient, NoCopy< WebSocketClient > >(ns.GetVM(), SqWebSocketClient::Str)
        // Constructors
        .Ctor()
        .Ctor< StackStrF &, uint16_t, StackStrF & >()
        .Ctor< StackStrF &, uint16_t, StackStrF &, bool >()
        .Ctor< StackStrF &, uint16_t, StackStrF &, bool, StackStrF & >()
        .Ctor< StackStrF &, uint16_t, StackStrF &, bool, StackStrF &, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqWebSocketClient::Fn)
        // Properties
        .Prop(_SC("Tag"), &WebSocketClient::GetTag, &WebSocketClient::SetTag)
        .Prop(_SC("Data"), &WebSocketClient::GetData, &WebSocketClient::SetData)
        .Prop(_SC("Host"), &WebSocketClient::GetHost, &WebSocketClient::SetHost)
        .Prop(_SC("Port"), &WebSocketClient::GetPort, &WebSocketClient::SetPort)
        .Prop(_SC("Path"), &WebSocketClient::GetPath, &WebSocketClient::SetPath)
        .Prop(_SC("Secure"), &WebSocketClient::GetSecure, &WebSocketClient::SetSecure)
        .Prop(_SC("Origin"), &WebSocketClient::GetOrigin, &WebSocketClient::SetOrigin)
        .Prop(_SC("Extensions"), &WebSocketClient::GetExtensions, &WebSocketClient::SetExtensions)
        .Prop(_SC("OnData"), &WebSocketClient::GetOnData, &WebSocketClient::SetOnData)
        .Prop(_SC("OnClose"), &WebSocketClient::GetOnClose, &WebSocketClient::SetOnClose)
        .Prop(_SC("Valid"), &WebSocketClient::IsValid)
        .Prop(_SC("Closing"), &WebSocketClient::IsClosing)
        .Prop(_SC("AutoClose"), &WebSocketClient::GetAutoClose, &WebSocketClient::SetAutoClose)
        // Member Methods
        .FmtFunc(_SC("SetTag"), &WebSocketClient::ApplyTag)
        .FmtFunc(_SC("SetData"), &WebSocketClient::ApplyData)
        .FmtFunc(_SC("SetHost"), &WebSocketClient::ApplyHost)
        .Func(_SC("SetPort"), &WebSocketClient::ApplyPort)
        .FmtFunc(_SC("SetPath"), &WebSocketClient::ApplyPath)
        .Func(_SC("SetSecure"), &WebSocketClient::ApplySecure)
        .FmtFunc(_SC("SetOrigin"), &WebSocketClient::ApplyOrigin)
        .FmtFunc(_SC("SetExtensions"), &WebSocketClient::ApplyExtensions)
        .CbFunc(_SC("BindOnData"), &WebSocketClient::BindOnData)
        .CbFunc(_SC("BindOnClose"), &WebSocketClient::BindOnClose)
        .Func(_SC("Connect"), &WebSocketClient::Connect)
        .Func(_SC("ConnectExt"), &WebSocketClient::ConnectExt)
        .Func(_SC("SendOpCode"), &WebSocketClient::SendOpCode)
        .Func(_SC("SendBuffer"), &WebSocketClient::SendBuffer)
        .FmtFunc(_SC("SendString"), &WebSocketClient::SendString)
        .Func(_SC("Close"), &WebSocketClient::Close)
    );
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(_SC("SqNet"), ns);
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqWsOpCode"), Enumeration(vm)
        .Const(_SC("Continuation"),     static_cast< SQInteger >(MG_WEBSOCKET_OPCODE_CONTINUATION))
        .Const(_SC("Text"),             static_cast< SQInteger >(MG_WEBSOCKET_OPCODE_TEXT))
        .Const(_SC("Binary"),           static_cast< SQInteger >(MG_WEBSOCKET_OPCODE_BINARY))
        .Const(_SC("ConnectionClose"),  static_cast< SQInteger >(MG_WEBSOCKET_OPCODE_CONNECTION_CLOSE))
        .Const(_SC("Ping"),             static_cast< SQInteger >(MG_WEBSOCKET_OPCODE_PING))
        .Const(_SC("Pong"),             static_cast< SQInteger >(MG_WEBSOCKET_OPCODE_PONG))
    );
    // Main thread ID
    sMainThreadID = std::this_thread::get_id();
}

} // Namespace:: SqMod
