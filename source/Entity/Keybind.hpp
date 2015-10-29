#ifndef _ENTITY_KEYBIND_HPP_
#define _ENTITY_KEYBIND_HPP_

// ------------------------------------------------------------------------------------------------
#include "Entity.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class responsible for managing the referenced keybind instance.
*/
class CKeybind : public Reference< CKeybind >
{
public:

    /* --------------------------------------------------------------------------------------------
     * Import the constructors, destructors and assignment operators from the base class.
    */
    using RefType::Reference;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the primary key code of the referenced keybind instance.
    */
    SQInt32 GetPrimary() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the secondary key code of the referenced keybind instance.
    */
    SQInt32 GetSecondary() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the alternative key code of the referenced keybind instance.
    */
    SQInt32 GetAlternative() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced keybind instance reacts to key press events.
    */
    bool IsRelease() const noexcept;
};

} // Namespace:: SqMod

#endif // _ENTITY_KEYBIND_HPP_