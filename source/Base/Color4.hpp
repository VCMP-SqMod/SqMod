#ifndef _BASE_COLOR4_HPP_
#define _BASE_COLOR4_HPP_

// ------------------------------------------------------------------------------------------------
#include "Config.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
struct Color4
{
    // --------------------------------------------------------------------------------------------
    typedef Uint8 Value;
    // --------------------------------------------------------------------------------------------
    static const Color4 NIL;
    static const Color4 MIN;
    static const Color4 MAX;
    // --------------------------------------------------------------------------------------------
    static SQChar Delim;
    // --------------------------------------------------------------------------------------------
    Value r, g, b, a;
    // --------------------------------------------------------------------------------------------
    Color4() noexcept;
    Color4(Value s) noexcept;
    Color4(Value r, Value g, Value b) noexcept;
    Color4(Value r, Value g, Value b, Value a) noexcept;
    // --------------------------------------------------------------------------------------------
    Color4(const Color3 & c) noexcept;
    // --------------------------------------------------------------------------------------------
    Color4(const SQChar * name) noexcept;
    Color4(const SQChar * str, SQChar delim) noexcept;
    // --------------------------------------------------------------------------------------------
    Color4(const Color4 & c) noexcept;
    Color4(Color4 && c) noexcept;
    // --------------------------------------------------------------------------------------------
    ~Color4();
    // --------------------------------------------------------------------------------------------
    Color4 & operator = (const Color4 & c) noexcept;
    Color4 & operator = (Color4 && c) noexcept;
    // --------------------------------------------------------------------------------------------
    Color4 & operator = (Value s) noexcept;
    Color4 & operator = (const SQChar * name) noexcept;
    Color4 & operator = (const Color3 & c) noexcept;
    // --------------------------------------------------------------------------------------------
    Color4 & operator += (const Color4 & c) noexcept;
    Color4 & operator -= (const Color4 & c) noexcept;
    Color4 & operator *= (const Color4 & c) noexcept;
    Color4 & operator /= (const Color4 & c) noexcept;
    Color4 & operator %= (const Color4 & c) noexcept;
    Color4 & operator &= (const Color4 & c) noexcept;
    Color4 & operator |= (const Color4 & c) noexcept;
    Color4 & operator ^= (const Color4 & c) noexcept;
    Color4 & operator <<= (const Color4 & c) noexcept;
    Color4 & operator >>= (const Color4 & c) noexcept;
    // --------------------------------------------------------------------------------------------
    Color4 & operator += (Value s) noexcept;
    Color4 & operator -= (Value s) noexcept;
    Color4 & operator *= (Value s) noexcept;
    Color4 & operator /= (Value s) noexcept;
    Color4 & operator %= (Value s) noexcept;
    Color4 & operator &= (Value s) noexcept;
    Color4 & operator |= (Value s) noexcept;
    Color4 & operator ^= (Value s) noexcept;
    Color4 & operator <<= (Value s) noexcept;
    Color4 & operator >>= (Value s) noexcept;
    // --------------------------------------------------------------------------------------------
    Color4 & operator ++ () noexcept;
    Color4 & operator -- () noexcept;
    // --------------------------------------------------------------------------------------------
    Color4 operator ++ (int) noexcept;
    Color4 operator -- (int) noexcept;
    // --------------------------------------------------------------------------------------------
    Color4 operator + (const Color4 & c) const noexcept;
    Color4 operator - (const Color4 & c) const noexcept;
    Color4 operator * (const Color4 & c) const noexcept;
    Color4 operator / (const Color4 & c) const noexcept;
    Color4 operator % (const Color4 & c) const noexcept;
    Color4 operator & (const Color4 & c) const noexcept;
    Color4 operator | (const Color4 & c) const noexcept;
    Color4 operator ^ (const Color4 & c) const noexcept;
    Color4 operator << (const Color4 & c) const noexcept;
    Color4 operator >> (const Color4 & c) const noexcept;
    // --------------------------------------------------------------------------------------------
    Color4 operator + (Value s) const noexcept;
    Color4 operator - (Value s) const noexcept;
    Color4 operator * (Value s) const noexcept;
    Color4 operator / (Value s) const noexcept;
    Color4 operator % (Value s) const noexcept;
    Color4 operator & (Value s) const noexcept;
    Color4 operator | (Value s) const noexcept;
    Color4 operator ^ (Value s) const noexcept;
    Color4 operator << (Value s) const noexcept;
    Color4 operator >> (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Color4 operator + () const noexcept;
    Color4 operator - () const noexcept;
    // --------------------------------------------------------------------------------------------
    Color4 operator ~ () const noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator == (const Color4 & c) const noexcept;
    bool operator != (const Color4 & c) const noexcept;
    bool operator < (const Color4 & c) const noexcept;
    bool operator > (const Color4 & c) const noexcept;
    bool operator <= (const Color4 & c) const noexcept;
    bool operator >= (const Color4 & c) const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const Color4 & c) const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(Value ns) noexcept;
    void Set(Value nr, Value ng, Value nb) noexcept;
    void Set(Value nr, Value ng, Value nb, Value na) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const Color4 & c) noexcept;
    void Set(const Color3 & c) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const SQChar * name, SQChar delim) noexcept;
    // --------------------------------------------------------------------------------------------
    void SetCol(const SQChar * name) noexcept;
    // --------------------------------------------------------------------------------------------
    SQUint32 GetRGB() const noexcept;
    void SetRGB(SQUint32 p) noexcept;
    // --------------------------------------------------------------------------------------------
    SQUint32 GetRGBA() const noexcept;
    void SetRGBA(SQUint32 p) noexcept;
    // --------------------------------------------------------------------------------------------
    SQUint32 GetARGB() const noexcept;
    void SetARGB(SQUint32 p) noexcept;
    // --------------------------------------------------------------------------------------------
    void Generate() noexcept;
    void Generate(Value min, Value max) noexcept;
    void Generate(Value rmin, Value rmax, Value gmin, Value gmax, Value bmin, Value bmax, Value amin, Value amax) noexcept;
    // --------------------------------------------------------------------------------------------
    void Clear() noexcept { r = 0, g = 0, b = 0, a = 0; }
    // --------------------------------------------------------------------------------------------
    void Random() noexcept;
    // --------------------------------------------------------------------------------------------
    void Inverse() noexcept;
};

} // Namespace:: SqMod

#endif // _BASE_COLOR4_HPP_
