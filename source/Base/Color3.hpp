#ifndef _BASE_COLOR3_HPP_
#define _BASE_COLOR3_HPP_

// ------------------------------------------------------------------------------------------------
#include "Config.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
struct Color3
{
    // --------------------------------------------------------------------------------------------
    typedef Uint8 Value;
    // --------------------------------------------------------------------------------------------
    static const Color3 NIL;
    static const Color3 MIN;
    static const Color3 MAX;
    // --------------------------------------------------------------------------------------------
    static SQChar Delim;
    // --------------------------------------------------------------------------------------------
    Value r, g, b;
    // --------------------------------------------------------------------------------------------
    Color3() noexcept;
    Color3(Value s) noexcept;
    Color3(Value r, Value g, Value b) noexcept;
    // --------------------------------------------------------------------------------------------
    Color3(const Color4 & c) noexcept;
    // --------------------------------------------------------------------------------------------
    Color3(const SQChar * name) noexcept;
    Color3(const SQChar * str, SQChar delim) noexcept;
    // --------------------------------------------------------------------------------------------
    Color3(const Color3 & c) noexcept;
    Color3(Color3 && c) noexcept;
    // --------------------------------------------------------------------------------------------
    ~Color3();
    // --------------------------------------------------------------------------------------------
    Color3 & operator = (const Color3 & c) noexcept;
    Color3 & operator = (Color3 && c) noexcept;
    // --------------------------------------------------------------------------------------------
    Color3 & operator = (Value s) noexcept;
    Color3 & operator = (const SQChar * name) noexcept;
    Color3 & operator = (const Color4 & c) noexcept;
    // --------------------------------------------------------------------------------------------
    Color3 & operator += (const Color3 & c) noexcept;
    Color3 & operator -= (const Color3 & c) noexcept;
    Color3 & operator *= (const Color3 & c) noexcept;
    Color3 & operator /= (const Color3 & c) noexcept;
    Color3 & operator %= (const Color3 & c) noexcept;
    Color3 & operator &= (const Color3 & c) noexcept;
    Color3 & operator |= (const Color3 & c) noexcept;
    Color3 & operator ^= (const Color3 & c) noexcept;
    Color3 & operator <<= (const Color3 & c) noexcept;
    Color3 & operator >>= (const Color3 & c) noexcept;
    // --------------------------------------------------------------------------------------------
    Color3 & operator += (Value s) noexcept;
    Color3 & operator -= (Value s) noexcept;
    Color3 & operator *= (Value s) noexcept;
    Color3 & operator /= (Value s) noexcept;
    Color3 & operator %= (Value s) noexcept;
    Color3 & operator &= (Value s) noexcept;
    Color3 & operator |= (Value s) noexcept;
    Color3 & operator ^= (Value s) noexcept;
    Color3 & operator <<= (Value s) noexcept;
    Color3 & operator >>= (Value s) noexcept;
    // --------------------------------------------------------------------------------------------
    Color3 & operator ++ () noexcept;
    Color3 & operator -- () noexcept;
    // --------------------------------------------------------------------------------------------
    Color3 operator ++ (int) noexcept;
    Color3 operator -- (int) noexcept;
    // --------------------------------------------------------------------------------------------
    Color3 operator + (const Color3 & c) const noexcept;
    Color3 operator - (const Color3 & c) const noexcept;
    Color3 operator * (const Color3 & c) const noexcept;
    Color3 operator / (const Color3 & c) const noexcept;
    Color3 operator % (const Color3 & c) const noexcept;
    Color3 operator & (const Color3 & c) const noexcept;
    Color3 operator | (const Color3 & c) const noexcept;
    Color3 operator ^ (const Color3 & c) const noexcept;
    Color3 operator << (const Color3 & c) const noexcept;
    Color3 operator >> (const Color3 & c) const noexcept;
    // --------------------------------------------------------------------------------------------
    Color3 operator + (Value s) const noexcept;
    Color3 operator - (Value s) const noexcept;
    Color3 operator * (Value s) const noexcept;
    Color3 operator / (Value s) const noexcept;
    Color3 operator % (Value s) const noexcept;
    Color3 operator & (Value s) const noexcept;
    Color3 operator | (Value s) const noexcept;
    Color3 operator ^ (Value s) const noexcept;
    Color3 operator << (Value s) const noexcept;
    Color3 operator >> (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Color3 operator + () const noexcept;
    Color3 operator - () const noexcept;
    // --------------------------------------------------------------------------------------------
    Color3 operator ~ () const noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator == (const Color3 & c) const noexcept;
    bool operator != (const Color3 & c) const noexcept;
    bool operator < (const Color3 & c) const noexcept;
    bool operator > (const Color3 & c) const noexcept;
    bool operator <= (const Color3 & c) const noexcept;
    bool operator >= (const Color3 & c) const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const Color3 & c) const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(Value ns) noexcept;
    void Set(Value nr, Value ng, Value nb) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const Color3 & c) noexcept;
    void Set(const Color4 & c) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const SQChar * str, SQChar delim) noexcept;
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
    void Generate(Value rmin, Value rmax, Value gmin, Value gmax, Value bmin, Value bmax) noexcept;
    // --------------------------------------------------------------------------------------------
    void Clear() noexcept { r = 0, g = 0, b = 0; }
    // --------------------------------------------------------------------------------------------
    void Random() noexcept;
    // --------------------------------------------------------------------------------------------
    void Inverse() noexcept;
};

} // Namespace:: SqMod

#endif // _BASE_COLOR3_HPP_
