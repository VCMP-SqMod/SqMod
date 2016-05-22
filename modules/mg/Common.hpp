#ifndef _SQMG_COMMON_HPP_
#define _SQMG_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "ModBase.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>

// ------------------------------------------------------------------------------------------------
#include <squirrel.h>
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

/* ------------------------------------------------------------------------------------------------
 * Retrieve the temporary buffer.
*/
SStr GetTempBuff();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the size of the temporary buffer.
*/
Uint32 GetTempBuffSize();

/* ------------------------------------------------------------------------------------------------
 * Throw a formatted exception.
*/
void SqThrowF(CSStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Generate a formatted string.
*/
CSStr FmtStr(CSStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Implements RAII to restore the VM stack to it's initial size on function exit.
*/
struct StackGuard
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    StackGuard();

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    StackGuard(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~StackGuard();

private:

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    StackGuard(const StackGuard &);

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    StackGuard(StackGuard &&);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    StackGuard & operator = (const StackGuard &);

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    StackGuard & operator = (StackGuard &&);

private:

    // --------------------------------------------------------------------------------------------
    HSQUIRRELVM m_VM; // The VM where the stack should be restored.
    Int32       m_Top; // The top of the stack when this instance was created.
};

/* ------------------------------------------------------------------------------------------------
 * Helper structure for retrieving a value from the stack as a string or a formatted string.
*/
struct StackStrF
{
    // --------------------------------------------------------------------------------------------
    CSStr       mPtr; // Pointer to the C string that was retrieved.
    SQInteger   mLen; // The string length if it could be retrieved.
    SQRESULT    mRes; // The result of the retrieval attempts.
    HSQOBJECT   mObj; // Strong reference to the string object.
    HSQUIRRELVM mVM; // The associated virtual machine.

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    StackStrF(HSQUIRRELVM vm, SQInteger idx, bool fmt = true);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    StackStrF(const StackStrF & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    StackStrF(StackStrF && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~StackStrF();

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    StackStrF & operator = (const StackStrF & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    StackStrF & operator = (StackStrF && o) = delete;
};

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null script object.
*/
Object & NullObject();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null/empty script table.
*/
Table & NullTable();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null/empty script array.
*/
Array & NullArray();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null script function.
*/
Function & NullFunction();

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
