#ifndef _SQMG_WEBSOCKET_HPP_
#define _SQMG_WEBSOCKET_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Connection.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Allows management of the connection handle.
*/
class WebSocket
{
private:

    // --------------------------------------------------------------------------------------------
    ConnectionHnd   m_Handle; // Reference to the connection instance.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    WebSocket()
        : m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Handle constructor.
    */
    WebSocket(const ConnectionHnd & c)
        : m_Handle(c)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    WebSocket(const WebSocket & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    WebSocket(WebSocket && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~WebSocket()
    {
        // Let the reference handle the deallocation!
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    WebSocket & operator = (const WebSocket & o);

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    WebSocket & operator = (WebSocket && o);

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const WebSocket & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     *
    */

};

} // Namespace:: SqMod

#endif // _SQMG_WEBSOCKET_HPP_
