#ifndef _MISC_WASTED_SETTINGS_HPP_
#define _MISC_WASTED_SETTINGS_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Base/Color3.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
struct CWastedSettings
{

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    typedef SQUnsignedInteger32 U32;
    typedef SQFloat F32;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    U32 DeathTimer, FadeTimer;
    F32 FadeInSpeed, FadeOutSpeed;
    Color3 FadeColor;
    U32 CorpseFadeStart, CorpseFadeTime;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    CWastedSettings() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    CWastedSettings(U32 dt, U32 ft, F32 fis, F32 fos, const Color3 & fc, U32 cfs, U32 cft) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    CWastedSettings(const CWastedSettings & x) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    CWastedSettings(CWastedSettings && x) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    ~CWastedSettings();

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    CWastedSettings & operator= (const CWastedSettings & x) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    CWastedSettings & operator= (CWastedSettings && x) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    CWastedSettings operator+ (const CWastedSettings & x) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    CWastedSettings operator- (const CWastedSettings & x) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    CWastedSettings operator* (const CWastedSettings & x) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    CWastedSettings operator/ (const CWastedSettings & x) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    bool operator== (const CWastedSettings & x) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    bool operator!= (const CWastedSettings & x) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInteger Cmp(const CWastedSettings & x) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void Set() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void Get() const noexcept;
};


} // Namespace:: SqMod

#endif // _MISC_WASTED_SETTINGS_HPP_