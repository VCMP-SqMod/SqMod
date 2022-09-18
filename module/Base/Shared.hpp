#pragma once

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
struct CtxJSON;

/* ------------------------------------------------------------------------------------------------
 * Helper constants used by the bas types.
*/
static constexpr float SQMOD_PI = 3.14159265358979323846264338327950288f;
static constexpr double SQMOD_PI64 = 3.1415926535897932384626433832795028841971693993751;

static constexpr float SQMOD_RECIPROCAL_PI = (1.0f / SQMOD_PI);
static constexpr double SQMOD_RECIPROCAL_PI64 = (1.0 / SQMOD_PI64);

static constexpr float SQMOD_HALF_PI = (SQMOD_PI * 0.5f);
static constexpr double SQMOD_HALF_PI64 = (SQMOD_PI64 * 0.5);

static constexpr float SQMOD_DEGTORAD = SQMOD_PI / 180.0f;
static constexpr double SQMOD_DEGTORAD64 = SQMOD_PI64 / 180.0;

static constexpr float SQMOD_DEGTORAD_2 = SQMOD_PI / 360.0f; // M_DEGTORAD / 2.f
static constexpr double SQMOD_DEGTORAD64_2 = SQMOD_PI64 / 360.0; // M_DEGTORAD / 2.f

static constexpr float SQMOD_RADTODEG   = 1.0f / SQMOD_DEGTORAD;
static constexpr double SQMOD_RADTODEG64 = 1.0 / SQMOD_DEGTORAD64;

/* ------------------------------------------------------------------------------------------------
 * Intersection test result.
*/
enum Intersection
{
    SQMODI_OUTSIDE = 0,
    SQMODI_INTERSECTS,
    SQMODI_INSIDE,
};

/* ------------------------------------------------------------------------------------------------
 * Obtain a randomly chosen color from a list of known colors.
*/
SQMOD_NODISCARD const Color3 & GetRandomColor();

/* ------------------------------------------------------------------------------------------------
 * Attempt to identify the color in the specified name and return it.
*/
SQMOD_NODISCARD Color3 GetColorStr(const SQChar * name);

/* ------------------------------------------------------------------------------------------------
 * Attempt to identify the color in the specified name and return it.
*/
SQMOD_NODISCARD Color3 GetColor(StackStrF & name);

} // Namespace:: SqMod
