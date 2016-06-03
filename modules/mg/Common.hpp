#ifndef _SQMG_COMMON_HPP_
#define _SQMG_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <mongoose.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * SOFTWARE INFORMATION
*/
#define SQMG_NAME "Squirrel Mongoose Module"
#define SQMG_AUTHOR "Sandu Liviu Catalin (S.L.C)"
#define SQMG_COPYRIGHT "Copyright (C) 2016 Sandu Liviu Catalin"
#define SQMG_HOST_NAME "SqModMongooseHost"
#define SQMG_VERSION 001
#define SQMG_VERSION_STR "0.0.1"
#define SQMG_VERSION_MAJOR 0
#define SQMG_VERSION_MINOR 0
#define SQMG_VERSION_PATCH 1

// ------------------------------------------------------------------------------------------------
enum struct HndInit
{
    InitHnd = 0
};

// ------------------------------------------------------------------------------------------------
class Manager;
class Connection;

/* ------------------------------------------------------------------------------------------------
 * Types of events supported by a connection.
*/
enum ConnectionEvents
{
    MGEV_UNKNOWN = SQMOD_UNKNOWN,
    MGCE_POLL,
    MGCE_ACCEPT,
    MGCE_CONNECT,
    MGCE_RECV,
    MGCE_SEND,
    MGCE_CLOSE,
    MGCE_TIMER,
    MGCE_HTTP_REQUEST,
    MGCE_HTTP_REPLY,
    MGCE_HTTP_CHUNK,
    MGCE_SSI_CALL,
    MGCE_WEBSOCKET_HANDSHAKE_REQUEST,
    MGCE_WEBSOCKET_HANDSHAKE_DONE,
    MGCE_WEBSOCKET_FRAME,
    MGCE_WEBSOCKET_CONTROL_FRAME,
    MGCE_HTTP_MULTIPART_REQUEST,
    MGCE_HTTP_PART_BEGIN,
    MGCE_HTTP_PART_DATA,
    MGCE_HTTP_PART_END,
    MGCE_MQTT_CONNECT,
    MGCE_MQTT_CONNACK,
    MGCE_MQTT_PUBLISH,
    MGCE_MQTT_PUBACK,
    MGCE_MQTT_PUBREC,
    MGCE_MQTT_PUBREL,
    MGCE_MQTT_PUBCOMP,
    MGCE_MQTT_SUBSCRIBE,
    MGCE_MQTT_SUBACK,
    MGCE_MQTT_UNSUBSCRIBE,
    MGCE_MQTT_UNSUBACK,
    MGCE_MQTT_PINGREQ,
    MGCE_MQTT_PINGRESP,
    MGCE_MQTT_DISCONNECT,
    MGCE_MQTT_CONNACK_ACCEPTED,
    MGCE_MQTT_CONNACK_UNACCEPTABLE_VERSION,
    MGCE_MQTT_CONNACK_IDENTIFIER_REJECTED,
    MGCE_MQTT_CONNACK_SERVER_UNAVAILABLE,
    MGCE_MQTT_CONNACK_BAD_AUTH,
    MGCE_MQTT_CONNACK_NOT_AUTHORIZED,
    MGCE_COAP_CON,
    MGCE_COAP_NOC,
    MGCE_COAP_ACK,
    MGCE_COAP_RST,
    MGCE_MAX
};

} // Namespace:: SqMod

#endif // _SQMG_COMMON_HPP_
