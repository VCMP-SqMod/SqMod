#ifndef _MISC_MODEL_HPP_
#define _MISC_MODEL_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Retrieve the name associated with a model identifier.
*/
CSStr GetModelName(Int32 id);

/* ------------------------------------------------------------------------------------------------
 * Modify the name associated with a model identifier.
*/
void SetModelName(Int32 id, StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * See whether the given model identifier is used a weapon model.
*/
bool IsModelWeapon(Int32 id);

/* ------------------------------------------------------------------------------------------------
 * See whether the given model identifier is an actual weapon model.
*/
bool IsModelActuallyWeapon(Int32 id);

} // Namespace:: SqMod

#endif // _MISC_MODEL_HPP_
