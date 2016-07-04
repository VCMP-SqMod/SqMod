// ------------------------------------------------------------------------------------------------
#include "Handle/Connection.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void ConnectionHnd::Validate() const
{
    // Is the handle valid?
    if ((m_Hnd == nullptr))
    {
        STHROWF("Invalid Mongoose connection reference");
    }
}

// ------------------------------------------------------------------------------------------------
void ConnectionHnd::Validate(CSStr act) const
{
    // Is the handle valid?
    if ((m_Hnd == nullptr))
    {
        STHROWF("Invalid Mongoose connection reference while: %s", act);
    }
}

// ------------------------------------------------------------------------------------------------
void ConnectionHnd::EvFwd(Pointer nc, Int32 ev_type, void * ev_data)
{
    if (!nc->user_data)
    {
        SqMod_LogErr("Event dispatched without valid instance");
    }
    else
    {
        static_cast< Handle * >(nc->user_data)->Event(ev_type, ev_data);
    }
}

// ------------------------------------------------------------------------------------------------
ConnectionHnd::Handle::Handle(Pointer con)
    : mCon(con)
    , mRef(1)
{
    // Validate the connection handle
    if (!mCon)
    {
        STHROWF("Invalid Mongoose connection handle");
    }
    // Associate with the connection instance
    mCon->user_data = this;
}

// ------------------------------------------------------------------------------------------------
ConnectionHnd::Handle::~Handle()
{
    // Tell the manager to close this connection as soon as possible
    mCon->flags |= MG_F_CLOSE_IMMEDIATELY;
}

// ------------------------------------------------------------------------------------------------
void ConnectionHnd::Handle::Event(Int32 ev_type, void * ev_data)
{
    switch (ev_type)
    {
        case MG_EV_POLL:
        {
            if (!mOnPOLL.IsNull())
            {
                mOnPOLL.Execute();
            }
        } break;
        case MG_EV_ACCEPT:
        {
            if (!mOnACCEPT.IsNull())
            {
                mOnACCEPT.Execute();
            }
        } break;
        case MG_EV_CONNECT:
        {
            if (!mOnCONNECT.IsNull())
            {
                mOnCONNECT.Execute(*static_cast< Int32 * >(ev_data));
            }
        } break;
        case MG_EV_RECV:
        {
            if (!mOnRECV.IsNull())
            {
                mOnRECV.Execute(*static_cast< Int32 * >(ev_data));
            }
        } break;
        case MG_EV_SEND:
        {
            if (!mOnSEND.IsNull())
            {
                mOnSEND.Execute(*static_cast< Int32 * >(ev_data));
            }
        } break;
        case MG_EV_CLOSE:
        {
            if (!mOnCLOSE.IsNull())
            {
                mOnCLOSE.Execute();
            }
        } break;
        case MG_EV_TIMER:
        {
            if (!mOnTIMER.IsNull())
            {
                mOnTIMER.Execute();
            }
        } break;
        default:
        {
            if (!mOnUNKNOWN.IsNull())
            {
                mOnUNKNOWN.Execute();
            }
        } break;
    }
}

// ------------------------------------------------------------------------------------------------
Function & ConnectionHnd::Handle::GetEvent(Int32 ev_type)
{
    // Identify the requested event type
    switch (ev_type)
    {
        case MGCE_POLL:                                 return mOnPOLL;
        case MGCE_ACCEPT:                               return mOnACCEPT;
        case MGCE_CONNECT:                              return mOnCONNECT;
        case MGCE_RECV:                                 return mOnRECV;
        case MGCE_SEND:                                 return mOnSEND;
        case MGCE_CLOSE:                                return mOnCLOSE;
        case MGCE_TIMER:                                return mOnTIMER;
        case MGCE_HTTP_REQUEST:                         return mOnHTTP_REQUEST;
        case MGCE_HTTP_REPLY:                           return mOnHTTP_REPLY;
        case MGCE_HTTP_CHUNK:                           return mOnHTTP_CHUNK;
        case MGCE_SSI_CALL:                             return mOnSSI_CALL;
        case MGCE_WEBSOCKET_HANDSHAKE_REQUEST:          return mOnWEBSOCKET_HANDSHAKE_REQUEST;
        case MGCE_WEBSOCKET_HANDSHAKE_DONE:             return mOnWEBSOCKET_HANDSHAKE_DONE;
        case MGCE_WEBSOCKET_FRAME:                      return mOnWEBSOCKET_FRAME;
        case MGCE_WEBSOCKET_CONTROL_FRAME:              return mOnWEBSOCKET_CONTROL_FRAME;
        case MGCE_HTTP_MULTIPART_REQUEST:               return mOnHTTP_MULTIPART_REQUEST;
        case MGCE_HTTP_PART_BEGIN:                      return mOnHTTP_PART_BEGIN;
        case MGCE_HTTP_PART_DATA:                       return mOnHTTP_PART_DATA;
        case MGCE_HTTP_PART_END:                        return mOnHTTP_PART_END;
        case MGCE_MQTT_CONNECT:                         return mOnMQTT_CONNECT;
        case MGCE_MQTT_CONNACK:                         return mOnMQTT_CONNACK;
        case MGCE_MQTT_PUBLISH:                         return mOnMQTT_PUBLISH;
        case MGCE_MQTT_PUBACK:                          return mOnMQTT_PUBACK;
        case MGCE_MQTT_PUBREC:                          return mOnMQTT_PUBREC;
        case MGCE_MQTT_PUBREL:                          return mOnMQTT_PUBREL;
        case MGCE_MQTT_PUBCOMP:                         return mOnMQTT_PUBCOMP;
        case MGCE_MQTT_SUBSCRIBE:                       return mOnMQTT_SUBSCRIBE;
        case MGCE_MQTT_SUBACK:                          return mOnMQTT_SUBACK;
        case MGCE_MQTT_UNSUBSCRIBE:                     return mOnMQTT_UNSUBSCRIBE;
        case MGCE_MQTT_UNSUBACK:                        return mOnMQTT_UNSUBACK;
        case MGCE_MQTT_PINGREQ:                         return mOnMQTT_PINGREQ;
        case MGCE_MQTT_PINGRESP:                        return mOnMQTT_PINGRESP;
        case MGCE_MQTT_DISCONNECT:                      return mOnMQTT_DISCONNECT;
        case MGCE_MQTT_CONNACK_ACCEPTED:                return mOnMQTT_CONNACK_ACCEPTED;
        case MGCE_MQTT_CONNACK_UNACCEPTABLE_VERSION:    return mOnMQTT_CONNACK_UNACCEPTABLE_VERSION;
        case MGCE_MQTT_CONNACK_IDENTIFIER_REJECTED:     return mOnMQTT_CONNACK_IDENTIFIER_REJECTED;
        case MGCE_MQTT_CONNACK_SERVER_UNAVAILABLE:      return mOnMQTT_CONNACK_SERVER_UNAVAILABLE;
        case MGCE_MQTT_CONNACK_BAD_AUTH:                return mOnMQTT_CONNACK_BAD_AUTH;
        case MGCE_MQTT_CONNACK_NOT_AUTHORIZED:          return mOnMQTT_CONNACK_NOT_AUTHORIZED;
        case MGCE_COAP_CON:                             return mOnCOAP_CON;
        case MGCE_COAP_NOC:                             return mOnCOAP_NOC;
        case MGCE_COAP_ACK:                             return mOnCOAP_ACK;
        case MGCE_COAP_RST:                             return mOnCOAP_RST;
        case MGEV_UNKNOWN:                              return mOnUNKNOWN;
        default: break;
    }
    // Default to a null function
    return NullFunction();
}

} // Namespace:: SqMod