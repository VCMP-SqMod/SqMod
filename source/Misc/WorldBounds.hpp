#ifndef _MISC_WORLD_BOUNDS_HPP_
#define _MISC_WORLD_BOUNDS_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Base/Vector2f.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
struct CWorldBounds
{
    // --------------------------------------------------------------------------------------------
    Vector2f min, max;
    // --------------------------------------------------------------------------------------------
    CWorldBounds() noexcept;
    CWorldBounds(const Vector2f & vec) noexcept;
    CWorldBounds(const Vector2f & min, const Vector2f & max) noexcept;
    // --------------------------------------------------------------------------------------------
    CWorldBounds(const CWorldBounds & x) noexcept;
    CWorldBounds(CWorldBounds && x) noexcept;
    // --------------------------------------------------------------------------------------------
    ~CWorldBounds();
    // --------------------------------------------------------------------------------------------
    CWorldBounds & operator= (const CWorldBounds & x) noexcept;
    CWorldBounds & operator= (CWorldBounds && x) noexcept;
    // --------------------------------------------------------------------------------------------
    CWorldBounds operator+ (const CWorldBounds & x) const noexcept;
    CWorldBounds operator- (const CWorldBounds & x) const noexcept;
    CWorldBounds operator* (const CWorldBounds & x) const noexcept;
    CWorldBounds operator/ (const CWorldBounds & x) const noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator== (const CWorldBounds & x) const noexcept;
    bool operator!= (const CWorldBounds & x) const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const CWorldBounds & x) const noexcept;
    // --------------------------------------------------------------------------------------------
    void Set() const noexcept;
    void Get() const noexcept;
};


} // Namespace:: SqMod

#endif // _MISC_WORLD_BOUNDS_HPP_