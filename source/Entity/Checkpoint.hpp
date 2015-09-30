#ifndef _ENTITY_CHECKPOINT_HPP_
#define _ENTITY_CHECKPOINT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Entity.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
class CCheckpoint : public Reference< CCheckpoint >
{
public:
    // --------------------------------------------------------------------------------------------
    using RefType::Reference;
};

} // Namespace:: SqMod

#endif // _ENTITY_CHECKPOINT_HPP_