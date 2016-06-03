#ifndef _SQMG_CONNECTION_HPP_
#define _SQMG_CONNECTION_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Connection.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Allows management and interaction with a connection handle.
*/
class Connection
{
private:

    // --------------------------------------------------------------------------------------------
    ConnectionHnd   m_Handle; // Reference to the connection instance.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Connection()
        : m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Handle constructor.
    */
    Connection(const ConnectionHnd & c)
        : m_Handle(c)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Connection(const Connection & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Connection(Connection && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Connection()
    {
        // Let the reference handle the deallocation!
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Connection & operator = (const Connection & o);

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Connection & operator = (Connection && o);

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Connection & o) const;

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
     * Bind to an event emitted by this connection.
    */
    void Bind(Int32 ev, Object & env, Function & func) const;
};

} // Namespace:: SqMod

#endif // _SQMG_CONNECTION_HPP_
