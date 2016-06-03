#ifndef _SQMG_MANAGER_HPP_
#define _SQMG_MANAGER_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Manager.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Allows management of the connection manager.
*/
class Manager
{
private:

    // --------------------------------------------------------------------------------------------
    ManagerHnd  m_Handle; // Reference to the manager instance.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Manager()
        : m_Handle(HndInit::InitHnd)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Handle constructor.
    */
    Manager(const ManagerHnd & m)
        : m_Handle(m)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Manager(const Manager & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Manager(Manager && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Manager()
    {
        // Let the reference handle the deallocation!
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Manager & operator = (const Manager & o);

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Manager & operator = (Manager && o);

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Manager & o) const;

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
     * Create listening connection.
    */
    Connection Bind(CSStr addr) const;

    /* --------------------------------------------------------------------------------------------
     * Create listening connection.
    */
    Connection Bind(CSStr addr, Uint32 flags) const;

    /* --------------------------------------------------------------------------------------------
     * Connect to a remote host.
    */
    Connection Connect(CSStr addr) const;

    /* --------------------------------------------------------------------------------------------
     * Connect to a remote host.
    */
    Connection Connect(CSStr addr, Uint32 flags) const;

    /* --------------------------------------------------------------------------------------------
     * Creates an outbound WebSocket connection.
    */
    Connection ConnectWS(CSStr url) const;

    /* --------------------------------------------------------------------------------------------
     * Creates an outbound WebSocket connection.
    */
    Connection ConnectWS(CSStr url, CSStr protocol) const;

    /* --------------------------------------------------------------------------------------------
     * Creates an outbound WebSocket connection.
    */
    Connection ConnectWS(CSStr url, CSStr protocol, CSStr headers) const;

    /* --------------------------------------------------------------------------------------------
     * Creates an outbound WebSocket connection.
    */
    Connection ConnectWS(CSStr url, CSStr protocol, CSStr headers, Uint32 flags) const;

    /* --------------------------------------------------------------------------------------------
     * Creates outbound HTTP connection.
    */
    Connection ConnectHTTP(CSStr url) const;

    /* --------------------------------------------------------------------------------------------
     * Creates outbound HTTP connection.
    */
    Connection ConnectHTTP(CSStr url, CSStr headers) const;

    /* --------------------------------------------------------------------------------------------
     * Creates outbound HTTP connection.
    */
    Connection ConnectHTTP(CSStr url, CSStr headers, CSStr post) const;

    /* --------------------------------------------------------------------------------------------
     * Creates outbound HTTP connection.
    */
    Connection ConnectHTTP(CSStr url, CSStr headers, CSStr post, Uint32 flags) const;
};

} // Namespace:: SqMod

#endif // _SQMG_MANAGER_HPP_
