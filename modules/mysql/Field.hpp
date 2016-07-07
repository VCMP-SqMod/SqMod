#ifndef _SQMYSQL_FIELD_HPP_
#define _SQMYSQL_FIELD_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/ResultSet.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
class Field
{
    // --------------------------------------------------------------------------------------------
    friend class ResultSet;

private:

    // --------------------------------------------------------------------------------------------
    ResRef  m_Handle; // Reference to the actual database result-set.
    Uint32  m_Index; // Index to the managed field.

protected:

    /* --------------------------------------------------------------------------------------------
     * Handle constructor.
    */
    Field(const ResRef & hnd, Uint32 idx)
        : m_Handle(hnd), m_Index(idx)
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Field(const Field & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Field(Field && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Field & operator = (const Field & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Field & operator = (Field && o) = default;

};

} // Namespace:: SqMod

#endif // _SQMYSQL_FIELD_HPP_
