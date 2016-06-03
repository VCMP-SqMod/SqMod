#ifndef _SQMG_HANDLE_CONNECTION_HPP_
#define _SQMG_HANDLE_CONNECTION_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted connection and its associated resources.
*/
class ConnectionHnd
{
    // --------------------------------------------------------------------------------------------
    friend class Manager;
    friend class Connection;

public:

    // --------------------------------------------------------------------------------------------
    typedef mg_connection           Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*                   Pointer; // Pointer to the managed type.
    typedef const Type*             ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&                   Reference; // Reference to the managed type.
    typedef const Type&             ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef unsigned int            Counter; // Reference counter type.

    /* --------------------------------------------------------------------------------------------
     * Validate the connection handle and throw an error if invalid.
    */
    void Validate() const;

    /* --------------------------------------------------------------------------------------------
     * Validate the connection handle and throw an error if invalid.
    */
    void Validate(CSStr act) const;

    /* --------------------------------------------------------------------------------------------
     * Forward connection events to the associated instance.
    */
    static void EvFwd(Pointer nc, Int32 ev_type, void * ev_data);

protected:

    /* --------------------------------------------------------------------------------------------
     * The structure that holds the data associated with a physically simulated world.
    */
    struct Handle
    {
        // ----------------------------------------------------------------------------------------
        Pointer                 mCon; // The managed connection structure.
        Counter                 mRef; // Reference count to the managed handles.

        // ----------------------------------------------------------------------------------------
        Function                mOnPOLL;
        Function                mOnACCEPT;
        Function                mOnCONNECT;
        Function                mOnRECV;
        Function                mOnSEND;
        Function                mOnCLOSE;
        Function                mOnTIMER;
        Function                mOnHTTP_REQUEST;
        Function                mOnHTTP_REPLY;
        Function                mOnHTTP_CHUNK;
        Function                mOnSSI_CALL;
        Function                mOnWEBSOCKET_HANDSHAKE_REQUEST;
        Function                mOnWEBSOCKET_HANDSHAKE_DONE;
        Function                mOnWEBSOCKET_FRAME;
        Function                mOnWEBSOCKET_CONTROL_FRAME;
        Function                mOnHTTP_MULTIPART_REQUEST;
        Function                mOnHTTP_PART_BEGIN;
        Function                mOnHTTP_PART_DATA;
        Function                mOnHTTP_PART_END;
        Function                mOnMQTT_CONNECT;
        Function                mOnMQTT_CONNACK;
        Function                mOnMQTT_PUBLISH;
        Function                mOnMQTT_PUBACK;
        Function                mOnMQTT_PUBREC;
        Function                mOnMQTT_PUBREL;
        Function                mOnMQTT_PUBCOMP;
        Function                mOnMQTT_SUBSCRIBE;
        Function                mOnMQTT_SUBACK;
        Function                mOnMQTT_UNSUBSCRIBE;
        Function                mOnMQTT_UNSUBACK;
        Function                mOnMQTT_PINGREQ;
        Function                mOnMQTT_PINGRESP;
        Function                mOnMQTT_DISCONNECT;
        Function                mOnMQTT_CONNACK_ACCEPTED;
        Function                mOnMQTT_CONNACK_UNACCEPTABLE_VERSION;
        Function                mOnMQTT_CONNACK_IDENTIFIER_REJECTED;
        Function                mOnMQTT_CONNACK_SERVER_UNAVAILABLE;
        Function                mOnMQTT_CONNACK_BAD_AUTH;
        Function                mOnMQTT_CONNACK_NOT_AUTHORIZED;
        Function                mOnCOAP_CON;
        Function                mOnCOAP_NOC;
        Function                mOnCOAP_ACK;
        Function                mOnCOAP_RST;
        Function                mOnUNKNOWN;

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Handle(Pointer con);

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Handle();

        /* ----------------------------------------------------------------------------------------
         * Handle a certain event for this connection.
        */
        void Event(Int32 ev_type, void * ev_data);

        /* ----------------------------------------------------------------------------------------
         * Retrieve the sript callback associated with a certain identifier.
        */
        Function & GetEvent(Int32 ev_type);
    };

private:

    // --------------------------------------------------------------------------------------------
    Handle * m_Hnd; // The managed handle instance.

    /* --------------------------------------------------------------------------------------------
     * Grab a strong reference to a connection handle.
    */
    void Grab()
    {
        if (m_Hnd)
        {
            ++(m_Hnd->mRef);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Drop a strong reference to a connection handle.
    */
    void Drop()
    {
        if (m_Hnd && --(m_Hnd->mRef) == 0)
        {
            delete m_Hnd; // Let the destructor take care of cleaning up (if necessary)
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    ConnectionHnd(Pointer con)
        : m_Hnd(new Handle(con))
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    ConnectionHnd()
        : m_Hnd(nullptr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Handle constructor.
    */
    ConnectionHnd(Handle * hnd)
        : m_Hnd(hnd)
    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    ConnectionHnd(const ConnectionHnd & o)
        : m_Hnd(o.m_Hnd)
    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    ConnectionHnd(ConnectionHnd && o)
        : m_Hnd(o.m_Hnd)
    {
        o.m_Hnd = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ConnectionHnd()
    {
        Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    ConnectionHnd & operator = (const ConnectionHnd & o)
    {
        if (m_Hnd != o.m_Hnd)
        {
            Drop();
            m_Hnd = o.m_Hnd;
            Grab();
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    ConnectionHnd & operator = (ConnectionHnd && o)
    {
        if (m_Hnd != o.m_Hnd)
        {
            m_Hnd = o.m_Hnd;
            o.m_Hnd = nullptr;
        }

        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two connection handles.
    */
    bool operator == (const ConnectionHnd & o) const
    {
        return (m_Hnd == o.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two connection handles.
    */
    bool operator != (const ConnectionHnd & o) const
    {
        return (m_Hnd != o.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const
    {
        return (m_Hnd != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Pointer ()
    {
        return (m_Hnd != nullptr) ? (m_Hnd->mCon) : nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Pointer () const
    {
        return (m_Hnd != nullptr) ? (m_Hnd->mCon) : nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Reference ()
    {
        assert((m_Hnd != nullptr));
        return *(m_Hnd->mCon);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator ConstRef () const
    {
        assert((m_Hnd != nullptr));
        return *(m_Hnd->mCon);
    }

    /* --------------------------------------------------------------------------------------------
     * Member operator for dereferencing the managed pointer.
    */
    Handle * operator -> () const
    {
        assert(m_Hnd != nullptr);
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Indirection operator for obtaining a reference of the managed pointer.
    */
    Handle & operator * () const
    {
        assert(m_Hnd != nullptr);
        return *m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw handle structure pointer.
    */
    Handle * HndPtr()
    {
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw handle structure pointer.
    */
    Handle * HndPtr() const
    {
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of active references to the managed instance.
    */
    Counter Count() const
    {
        return (m_Hnd != nullptr) ? m_Hnd->mRef : 0;
    }
};

} // Namespace:: SqMod

#endif // _SQMG_HANDLE_CONNECTION_HPP_
