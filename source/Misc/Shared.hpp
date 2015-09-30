#ifndef _MISC_SHARED_HPP_
#define _MISC_SHARED_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <array>
#include <utility>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const SQChar * GetKeyCodeName(SQInteger keycode);

// ------------------------------------------------------------------------------------------------
const SQChar * GetModelName(SQInt32 id);
bool IsModelWeapon(SQInt32 id);
bool IsModelActuallyWeapon(SQInt32 id);
bool IsModelValid(SQInt32 id);

// ------------------------------------------------------------------------------------------------
const SQChar * GetSkinName(SQInt32 id);
SQInt32 GetSkinID(const SQChar * name);
bool IsSkinValid(SQInt32 id);

// ------------------------------------------------------------------------------------------------
const SQChar * GetAutomobileName(SQInt32 id);
SQInt32 GetAutomobileID(const SQChar * name);
bool IsAutomobileValid(SQInt32 id);

// ------------------------------------------------------------------------------------------------
const SQChar * GetWeaponName(SQInt32 id);
SQInt32 GetWeaponID(const SQChar * name);
bool IsWeaponValid(SQInt32 id);
bool IsWeaponNatural(SQInt32 id);

// ------------------------------------------------------------------------------------------------
SQInt32 WeaponToModel(SQInt32 id);

// ------------------------------------------------------------------------------------------------
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
     * ...
    */
    static const SQChar * GlobalTag(SQUint32 id) noexcept
    {
        if (id < N)
        {
            return s_Globals[id].first.c_str();
        }
        return _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void GlobalTag(SQUint32 id,  const SQChar * tag) noexcept
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
     * ...
    */
    static SqObj & GlobalData(SQUint32 id) noexcept
    {
        if (id < N)
        {
            return s_Globals[id].second;
        }
        return NullData();
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void GlobalData(SQUint32 id,  SqObj & data) noexcept
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
};

// ------------------------------------------------------------------------------------------------
template < class T, unsigned N >
    typename IdentifierStorage< T, N >::GlobalStorage IdentifierStorage< T, N >::s_Globals;

} // Namespace:: SqMod

#endif // _MISC_SHARED_HPP_
