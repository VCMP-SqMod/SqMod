#ifndef _MISC_SHARED_HPP_
#define _MISC_SHARED_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <array>
#include <utility>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Attempt to convert the specified key code to a name.
*/
const SQChar * GetKeyCodeName(SQInt32 keycode);

/* ------------------------------------------------------------------------------------------------
 * Attemp to convert the specified model identifier to a name.
*/
const SQChar * GetModelName(SQInt32 id);

/* ------------------------------------------------------------------------------------------------
 * See whether the specified model identifier points to model that is used to represent weapons.
*/
bool IsModelWeapon(SQInt32 id);

/* ------------------------------------------------------------------------------------------------
 * See whether the specified model identifier points to model that is used to represent
 * an actual weapon.
*/
bool IsModelActuallyWeapon(SQInt32 id);

/* ------------------------------------------------------------------------------------------------
 * See whether the specified model identifier is valid.
*/
bool IsModelValid(SQInt32 id);

/* ------------------------------------------------------------------------------------------------
 * Attemp to convert the specified player skin identifier to a name.
*/
const SQChar * GetSkinName(SQInt32 id);

/* ------------------------------------------------------------------------------------------------
 * Attempt to convert the specified player skin name to an identifier.
*/
SQInt32 GetSkinID(const SQChar * name);

/* ------------------------------------------------------------------------------------------------
 * See whether the specified player skin identifier is valid.
*/
bool IsSkinValid(SQInt32 id);

/* ------------------------------------------------------------------------------------------------
 * Attemp to convert the specified vehicle model identifier to a name.
*/
const SQChar * GetAutomobileName(SQInt32 id);

/* ------------------------------------------------------------------------------------------------
 * Attempt to convert the specified vehicle model name to an identifier.
*/
SQInt32 GetAutomobileID(const SQChar * name);

/* ------------------------------------------------------------------------------------------------
 * See whether the specified vehicle model identifier is valid.
*/
bool IsAutomobileValid(SQInt32 id);

/* ------------------------------------------------------------------------------------------------
 * Attemp to convert the specified weapon identifier to a name.
*/
const SQChar * GetWeaponName(SQInt32 id);

/* ------------------------------------------------------------------------------------------------
 * Attempt to convert the specified weapon name to an identifier.
*/
SQInt32 GetWeaponID(const SQChar * name);

/* ------------------------------------------------------------------------------------------------
 * See whether the specified weapon identifier is valid.
*/
bool IsWeaponValid(SQInt32 id);

/* ------------------------------------------------------------------------------------------------
 * See whether the specified weapon identifier points to a weapon that may not be used by
 * another player to perform a kill. Such as a player fall, drown, explosion etc.
*/
bool IsWeaponNatural(SQInt32 id);

/* ------------------------------------------------------------------------------------------------
 * Attempt to convert the weapon identifier to it's corresponding model identifier.
*/
SQInt32 WeaponToModel(SQInt32 id);

/* ------------------------------------------------------------------------------------------------
 * Helper class used to associate a tag and arbitrary data to an identifier.
*/
template < class T, unsigned N > class IdentifierStorage
{
public:
    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 Max = N;

protected:

    // --------------------------------------------------------------------------------------------
    typedef std::array< std::pair< SqTag, SqObj >, N > GlobalStorage;

    // --------------------------------------------------------------------------------------------
    static GlobalStorage s_Globals;

protected:

    /* --------------------------------------------------------------------------------------------
     * Retrieve the global tag.
    */
    static const SQChar * GlobalTag(SQUint32 id)
    {
        if (id < N)
        {
            return s_Globals[id].first.c_str();
        }

        return _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Change the global tag.
    */
    static void GlobalTag(SQUint32 id,  const SQChar * tag)
    {
        if (id < N)
        {
            s_Globals[id].first.assign(tag);
        }
        else
        {
            LogErr("Attempting to set global tag for invalid automobile id: %d", id);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the global data.
    */
    static SqObj & GlobalData(SQUint32 id)
    {
        if (id < N)
        {
            return s_Globals[id].second;
        }
        return NullData();
    }

    /* --------------------------------------------------------------------------------------------
     * Change the global data.
    */
    static void GlobalData(SQUint32 id,  SqObj & data)
    {
        if (id < N)
        {
            s_Globals[id].second = data;
        }
        else
        {
            LogErr("Attempting to set global data for invalid automobile id: %d", id);
        }
    }

public:

    /* --------------------------------------------------------------------------------------------
     * See if the specified identifier is valid and in bounds.
    */
    static bool Valid(SQInt32 id)
    {
        return VALID_ENTITYGETEX(id, Max);
    }
};

// ------------------------------------------------------------------------------------------------
template < class T, unsigned N >
    typename IdentifierStorage< T, N >::GlobalStorage IdentifierStorage< T, N >::s_Globals;

} // Namespace:: SqMod

#endif // _MISC_SHARED_HPP_
