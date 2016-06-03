#ifndef _SQMG_HTTP_HPP_
#define _SQMG_HTTP_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Connection.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Allows management of the connection handle.
*/
class HTTP
{
private:

    // --------------------------------------------------------------------------------------------
    ConnectionHnd   m_Handle; // Reference to the connection instance.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    HTTP()
        : m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Handle constructor.
    */
    HTTP(const ConnectionHnd & c)
        : m_Handle(c)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    HTTP(const HTTP & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    HTTP(HTTP && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~HTTP()
    {
        // Let the reference handle the deallocation!
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    HTTP & operator = (const HTTP & o);

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    HTTP & operator = (HTTP && o);

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const HTTP & o) const;

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

#endif // _SQMG_HTTP_HPP_
