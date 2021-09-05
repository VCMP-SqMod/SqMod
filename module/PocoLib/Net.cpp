// ------------------------------------------------------------------------------------------------
#include "PocoLib/Net.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqWsClient, _SC("SqWsClient"))

// ================================================================================================
void Register_POCO_Net(HSQUIRRELVM vm, Table &)
{
    Table ns(vm);
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("WsClient"),
        Class< WsClient, NoCopy< WsClient > >(ns.GetVM(), SqWsClient::Str)
        // Constructors
        .Ctor< StackStrF &, uint16_t, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqWsClient::Fn)
        // Member Variables
        .Var(_SC("Flags"), &WsClient::mFlags)
        .Var(_SC("State"), &WsClient::mState)
        // Properties
        .Prop(_SC("MaxPayloadSize"), &WsClient::GetMaxPayloadSize, &WsClient::SetMaxPayloadSize)
        // Member Methods
        .Func(_SC("Shutdown"), &WsClient::Shutdown)
        .FmtFunc(_SC("ShutdownWith"), &WsClient::ShutdownWith)
        .Func(_SC("SendFrame"), &WsClient::SendFrame)
        .FmtFunc(_SC("SendStringFrame"), &WsClient::SendStringFrame)
        .Func(_SC("RecvFrame"), &WsClient::RecvFrame)
        .Func(_SC("RecvStringFrame"), &WsClient::RecvStringFrame)
    );
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(_SC("SqNet"), ns);
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqWsFrameFlags"), Enumeration(vm)
        .Const(_SC("FIN"),      static_cast< SQInteger >(Poco::Net::WebSocket::FRAME_FLAG_FIN))
        .Const(_SC("RSV1"),     static_cast< SQInteger >(Poco::Net::WebSocket::FRAME_FLAG_RSV1))
        .Const(_SC("RSV2"),     static_cast< SQInteger >(Poco::Net::WebSocket::FRAME_FLAG_RSV2))
        .Const(_SC("RSV3"),     static_cast< SQInteger >(Poco::Net::WebSocket::FRAME_FLAG_RSV3))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqWsFrameOpcodes"), Enumeration(vm)
        .Const(_SC("CONT"),     static_cast< SQInteger >(Poco::Net::WebSocket::FRAME_OP_CONT))
        .Const(_SC("TEXT"),     static_cast< SQInteger >(Poco::Net::WebSocket::FRAME_OP_TEXT))
        .Const(_SC("BINARY"),   static_cast< SQInteger >(Poco::Net::WebSocket::FRAME_OP_BINARY))
        .Const(_SC("CLOSE"),    static_cast< SQInteger >(Poco::Net::WebSocket::FRAME_OP_CLOSE))
        .Const(_SC("PING"),     static_cast< SQInteger >(Poco::Net::WebSocket::FRAME_OP_PING))
        .Const(_SC("PONG"),     static_cast< SQInteger >(Poco::Net::WebSocket::FRAME_OP_PONG))
        .Const(_SC("BITMASK"),  static_cast< SQInteger >(Poco::Net::WebSocket::FRAME_OP_BITMASK))
        .Const(_SC("SETRAW"),   static_cast< SQInteger >(Poco::Net::WebSocket::FRAME_OP_SETRAW))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqWsSendFlags"), Enumeration(vm)
        .Const(_SC("TEXT"),     static_cast< SQInteger >(Poco::Net::WebSocket::FRAME_TEXT))
        .Const(_SC("BINARY"),   static_cast< SQInteger >(Poco::Net::WebSocket::FRAME_BINARY))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqWsStatusCodes"), Enumeration(vm)
        .Const(_SC("NormalClose"),              static_cast< SQInteger >(Poco::Net::WebSocket::WS_NORMAL_CLOSE))
        .Const(_SC("EndpointGoingAway"),        static_cast< SQInteger >(Poco::Net::WebSocket::WS_ENDPOINT_GOING_AWAY))
        .Const(_SC("ProtocolError"),            static_cast< SQInteger >(Poco::Net::WebSocket::WS_PROTOCOL_ERROR))
        .Const(_SC("PayloadNotAcceptable"),     static_cast< SQInteger >(Poco::Net::WebSocket::WS_PAYLOAD_NOT_ACCEPTABLE))
        .Const(_SC("Reserved"),                 static_cast< SQInteger >(Poco::Net::WebSocket::WS_RESERVED))
        .Const(_SC("ReservedNoStatusCode"),     static_cast< SQInteger >(Poco::Net::WebSocket::WS_RESERVED_NO_STATUS_CODE))
        .Const(_SC("ReservedAbnormalClose"),    static_cast< SQInteger >(Poco::Net::WebSocket::WS_RESERVED_ABNORMAL_CLOSE))
        .Const(_SC("MalformedPayload"),         static_cast< SQInteger >(Poco::Net::WebSocket::WS_MALFORMED_PAYLOAD))
        .Const(_SC("PolicyViolation"),          static_cast< SQInteger >(Poco::Net::WebSocket::WS_POLICY_VIOLATION))
        .Const(_SC("PayloadTooBig"),            static_cast< SQInteger >(Poco::Net::WebSocket::WS_PAYLOAD_TOO_BIG))
        .Const(_SC("ExtensionRequired"),        static_cast< SQInteger >(Poco::Net::WebSocket::WS_EXTENSION_REQUIRED))
        .Const(_SC("UnexpectedCondition"),      static_cast< SQInteger >(Poco::Net::WebSocket::WS_UNEXPECTED_CONDITION))
        .Const(_SC("ReservedTlsFailure"),       static_cast< SQInteger >(Poco::Net::WebSocket::WS_RESERVED_TLS_FAILURE))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqWsSendFlags"), Enumeration(vm)
        .Const(_SC("NoHandshake"),                  static_cast< SQInteger >(Poco::Net::WebSocket::WS_ERR_NO_HANDSHAKE))
        .Const(_SC("HandshakeNoVersion"),           static_cast< SQInteger >(Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION))
        .Const(_SC("HandshakeUnsupportedVersion"),  static_cast< SQInteger >(Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION))
        .Const(_SC("HandshakeNoKey"),               static_cast< SQInteger >(Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY))
        .Const(_SC("HandshakeAccept"),              static_cast< SQInteger >(Poco::Net::WebSocket::WS_ERR_HANDSHAKE_ACCEPT))
        .Const(_SC("Unauthorized"),                 static_cast< SQInteger >(Poco::Net::WebSocket::WS_ERR_UNAUTHORIZED))
        .Const(_SC("PayloadTooBig"),                static_cast< SQInteger >(Poco::Net::WebSocket::WS_ERR_PAYLOAD_TOO_BIG))
        .Const(_SC("IncompleteFrame"),              static_cast< SQInteger >(Poco::Net::WebSocket::WS_ERR_INCOMPLETE_FRAME))
    );
}

} // Namespace:: SqMod
