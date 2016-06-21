#ifndef _BASE_ALGO_HPP_
#define _BASE_ALGO_HPP_

// ------------------------------------------------------------------------------------------------
#include <SqBase.hpp>

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace Algo {

/* ------------------------------------------------------------------------------------------------
 * Collect all elements within the specified range that the inspector deems worthy.
*/
template < typename Iterator, typename Inspector, typename Collector >
void Collect(Iterator first, Iterator last, Inspector inspect, Collector collect)
{
    for (; first != last; ++first)
    {
        if (inspect(*first))
        {
            collect(*first);
        }
    }
}

} // Namespace:: Algo
} // Namespace:: SqMod

#endif // _BASE_ALGO_HPP_
