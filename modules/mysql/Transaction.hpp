#ifndef _SQMYSQL_TRANSACTION_HPP_
#define _SQMYSQL_TRANSACTION_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
class Transaction
{
public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Transaction()
    {

    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Transaction(const Transaction & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    Transaction(Transaction && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Transaction()
    {

    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Transaction & operator = (const Transaction & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Transaction & operator = (Transaction && o) = default;

};

} // Namespace:: SqMod

#endif // _SQMYSQL_TRANSACTION_HPP_
