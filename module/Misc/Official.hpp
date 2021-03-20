#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Color3.hpp"
#include "Base/Color4.hpp"
#include "Base/Vector3.hpp"
#include "Base/Quaternion.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Entity vector.
*/
struct LgVector : public Vector3
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    LgVector() = default;
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit LgVector(const Vector3 & v)
        : Vector3(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Import from base class.
    */
    using Vector3::Vector3;
    using Vector3::operator =;
};

/* ------------------------------------------------------------------------------------------------
 * Entity vector.
*/
struct LgEntityVector : public LgVector
{
    /* --------------------------------------------------------------------------------------------
     * Entity identifier.
    */
    int16_t mID{-1};

    /* --------------------------------------------------------------------------------------------
     * Entity type.
    */
    uint8_t mType{0};

    /* --------------------------------------------------------------------------------------------
     * Vector flag.
    */
    uint8_t mFlag{0};

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    LgEntityVector(int id, int type, int flag, Value x, Value y, Value z)
        : LgVector(x, y, z)
        , mID(static_cast< int16_t >(id))
        , mType(static_cast< uint8_t >(type))
        , mFlag(static_cast< uint8_t >(flag))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    LgEntityVector(int id, int type, int flag, const Vector3 & v)
        : LgVector{v}
        , mID(static_cast< int16_t >(id))
        , mType(static_cast< uint8_t >(type))
        , mFlag(static_cast< uint8_t >(flag))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the X component of the vector.
    */
    SQMOD_NODISCARD Value GetX() const { return LgVector::x; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the Y component of the vector.
    */
    SQMOD_NODISCARD Value GetY() const { return LgVector::y; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the Z component of the vector.
    */
    SQMOD_NODISCARD Value GetZ() const { return LgVector::z; }

    /* --------------------------------------------------------------------------------------------
     * Modify the X component of the vector.
    */
    void SetX(Value v) { LgVector::x = v; Set(); }

    /* --------------------------------------------------------------------------------------------
     * Modify the Y component of the vector.
    */
    void SetY(Value v) { LgVector::y = v; Set(); }

    /* --------------------------------------------------------------------------------------------
     * Modify the Z component of the vector.
    */
    void SetZ(Value v) { LgVector::z = v; Set(); }

    /* --------------------------------------------------------------------------------------------
     * Modify the X, Y and Z component of the vector.
    */
    void Set();

    /* --------------------------------------------------------------------------------------------
     * Import from base class.
    */
    using LgVector::LgVector;
    using LgVector::operator =;
};

/* ------------------------------------------------------------------------------------------------
 * Entity quaternion.
*/
struct LgQuaternion : public Quaternion
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    LgQuaternion() = default;
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit LgQuaternion(const Quaternion & q)
        : Quaternion(q)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Import from base class.
    */
    using Quaternion::Quaternion;
    using Quaternion::operator =;
};

/* ------------------------------------------------------------------------------------------------
 * Entity quaternion.
*/
struct LgEntityQuaternion : public LgQuaternion
{
    /* --------------------------------------------------------------------------------------------
     * Entity identifier.
    */
    int16_t mID{-1};

    /* --------------------------------------------------------------------------------------------
     * Entity type.
    */
    uint8_t mType{0};

    /* --------------------------------------------------------------------------------------------
     * Quaternion flag.
    */
    uint8_t mFlag{0};

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    LgEntityQuaternion(int id, int type, int flag, Value x, Value y, Value z, Value w)
        : LgQuaternion(x, y, z, w)
        , mID(static_cast< int16_t >(id))
        , mType(static_cast< uint8_t >(type))
        , mFlag(static_cast< uint8_t >(flag))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    LgEntityQuaternion(int id, int type, int flag, const Quaternion & q)
        : LgQuaternion{q}
        , mID(static_cast< int16_t >(id))
        , mType(static_cast< uint8_t >(type))
        , mFlag(static_cast< uint8_t >(flag))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the X component of the quaternion.
    */
    SQMOD_NODISCARD Value GetX() const { return LgQuaternion::x; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the Y component of the quaternion.
    */
    SQMOD_NODISCARD Value GetY() const { return LgQuaternion::y; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the Z component of the quaternion.
    */
    SQMOD_NODISCARD Value GetZ() const { return LgQuaternion::z; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the W component of the quaternion.
    */
    SQMOD_NODISCARD Value GetW() const { return LgQuaternion::w; }

    /* --------------------------------------------------------------------------------------------
     * Modify the X component of the quaternion.
    */
    void SetX(Value v) { LgQuaternion::x = v; Set(); }

    /* --------------------------------------------------------------------------------------------
     * Modify the Y component of the quaternion.
    */
    void SetY(Value v) { LgQuaternion::y = v; Set(); }

    /* --------------------------------------------------------------------------------------------
     * Modify the Z component of the quaternion.
    */
    void SetZ(Value v) { LgQuaternion::z = v; Set(); }

    /* --------------------------------------------------------------------------------------------
     * Modify the W component of the quaternion.
    */
    void SetW(Value v) { LgQuaternion::w = v; Set(); }

    /* --------------------------------------------------------------------------------------------
     * Modify the X, Y, Z and W component of the quaternion.
    */
    void Set();

    /* --------------------------------------------------------------------------------------------
     * Import from base class.
    */
    using LgQuaternion::LgQuaternion;
    using LgQuaternion::operator =;
};

/* ------------------------------------------------------------------------------------------------
 * Class used to represent a transparent RGB color.
*/
struct LgRGB : public Color3
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    LgRGB() = default;
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit LgRGB(const Color3 & c)
        : Color3(c)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Import from base class.
    */
    using Color3::Color3;
    using Color3::operator =;
};

/* ------------------------------------------------------------------------------------------------
 * Class used to represent a transparent RGBA color.
*/
struct LgRGBA : public Color4
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    LgRGBA() = default;
    /* --------------------------------------------------------------------------------------------
     * Import from base class.
    */
    using Color4::Color4;
    using Color4::operator =;
};

/* ------------------------------------------------------------------------------------------------
 * Class used to represent a transparent ARGB color.
*/
struct LgARGB : public Color4
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    LgARGB() = default;
    /* --------------------------------------------------------------------------------------------
     * Construct with individually specified alpha, red, green and blue colors.
    */
    LgARGB(Value av, Value rv, Value gv, Value bv) noexcept
        : Color4(rv, gv, bv, av)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Import from base class.
    */
    using Color4::Color4;
    using Color4::operator =;
};

/* ------------------------------------------------------------------------------------------------
 * Entity color.
*/
struct LgEntityRGB : public LgRGB
{
    /* --------------------------------------------------------------------------------------------
     * Entity identifier.
    */
    int16_t mID{-1};

    /* --------------------------------------------------------------------------------------------
     * Entity type.
    */
    uint8_t mType{0};

    /* --------------------------------------------------------------------------------------------
     * Color flag.
    */
    uint8_t mFlag{0};

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    LgEntityRGB(int id, int type, int flag, Value r, Value g, Value b)
        : LgRGB(r, g, b)
        , mID(static_cast< int16_t >(id))
        , mType(static_cast< uint8_t >(type))
        , mFlag(static_cast< uint8_t >(flag))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    LgEntityRGB(int id, int type, int flag, const Color3 & c)
        : LgRGB{c}
        , mID(static_cast< int16_t >(id))
        , mType(static_cast< uint8_t >(type))
        , mFlag(static_cast< uint8_t >(flag))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the Red component of the color.
    */
    SQMOD_NODISCARD Value GetR() const { return LgRGB::r; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the Green component of the color.
    */
    SQMOD_NODISCARD Value GetG() const { return LgRGB::g; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the Blue component of the color.
    */
    SQMOD_NODISCARD Value GetB() const { return LgRGB::b; }

    /* --------------------------------------------------------------------------------------------
     * Modify the Red component of the color.
    */
    void SetR(Value v) { LgRGB::r = v; Set(); }

    /* --------------------------------------------------------------------------------------------
     * Modify the Green component of the color.
    */
    void SetG(Value v) { LgRGB::g = v; Set(); }

    /* --------------------------------------------------------------------------------------------
     * Modify the Blue component of the color.
    */
    void SetB(Value v) { LgRGB::b = v; Set(); }

    /* --------------------------------------------------------------------------------------------
     * Modify the red, Green and Blue component of the color.
    */
    void Set();

    /* --------------------------------------------------------------------------------------------
     * Import from base class.
    */
    using LgRGB::LgRGB;
    using LgRGB::operator =;
};

/* ------------------------------------------------------------------------------------------------
 * Class used to represent 2D area boundaries.
*/
struct LgBounds
{
    /* --------------------------------------------------------------------------------------------
     * The type of value used by components of type.
    */
    typedef float Value;

    /* --------------------------------------------------------------------------------------------
     * Upper and lower bounds of the area.
    */
    Value max_x{0};
    Value min_x{0};
    Value max_y{0};
    Value min_y{0};

    /* --------------------------------------------------------------------------------------------
     * Construct with default (empty/zero) bounds.
    */
    LgBounds() = default;

    /* --------------------------------------------------------------------------------------------
     * Construct with individually specified bounds.
    */
    LgBounds(Value xmax, Value xmin, Value ymax, Value ymin)
        : max_x(xmax), min_x(xmin), max_y(ymax), min_y(ymin)
    {
    }
};

/* ------------------------------------------------------------------------------------------------
 * Class used to represent wasted settings.
*/
struct LgWastedSettings
{
    /* --------------------------------------------------------------------------------------------
     * Settings values.
    */
    uint32_t    mDeathTime{0};
    uint32_t    mFadeTime{0};
    float       mFadeInSpeed{0.0f};
    float       mFadeOutSpeed{0.0f};
    LgRGB       mFadeColour{0};
    uint32_t    mCorpseFadeDelay{0};
    uint32_t    mCorpseFadeTime{0};

    /* --------------------------------------------------------------------------------------------
     * Construct with default settings.
    */
    LgWastedSettings() = default;

    /* --------------------------------------------------------------------------------------------
     * Construct with individually specified settings.
    */
    LgWastedSettings(uint32_t death_time, uint32_t fade_time, float fade_in_speed, float fade_out_speed,
                        LgRGB fade_colour, uint32_t corpse_fade_delay, uint32_t corpse_fade_time)
        : mDeathTime(death_time)
        , mFadeTime(fade_time)
        , mFadeInSpeed(fade_in_speed)
        , mFadeOutSpeed(fade_out_speed)
        , mFadeColour(fade_colour)
        , mCorpseFadeDelay(corpse_fade_delay)
        , mCorpseFadeTime(corpse_fade_time)
    {
    }
};

} // Namespace:: SqMod
