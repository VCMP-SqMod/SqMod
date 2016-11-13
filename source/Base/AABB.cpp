// ------------------------------------------------------------------------------------------------
#include "Base/AABB.hpp"
#include "Base/Sphere.hpp"
#include "Base/Shared.hpp"
#include "Base/DynArg.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const AABB AABB::NIL = AABB(0, 0);
const AABB AABB::MIN = AABB(-1, -1, -1, 1, 1, 1);
const AABB AABB::MAX = AABB(HUGE_VALF, -HUGE_VALF);

// ------------------------------------------------------------------------------------------------
SQChar AABB::Delim = ',';

// ------------------------------------------------------------------------------------------------
SQInteger AABB::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("AABB");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
AABB::AABB()
    : min(HUGE_VALF), max(-HUGE_VALF)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
AABB::AABB(Value mins, Value maxs)
    : min(mins), max(maxs)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
AABB::AABB(Value xv, Value yv, Value zv)
    : min(xv, yv, zv)
    , max(xv, yv, zv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
AABB::AABB(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax)
    : min(xmin, ymin, zmin), max(xmax, ymax, zmax)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
AABB::AABB(const Vector3 & vmin, const Vector3 & vmax)
    : min(vmin), max(vmax)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator = (const Vector3 & v)
{
    DefineVector3(v);
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator += (const AABB & b)
{
    min += b.min;
    max += b.max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator -= (const AABB & b)
{
    min -= b.min;
    max -= b.max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator *= (const AABB & b)
{
    min *= b.min;
    max *= b.max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator /= (const AABB & b)
{
    min /= b.min;
    max /= b.max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator %= (const AABB & b)
{
    min %= b.min;
    max %= b.max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator += (Value s)
{
    min += s;
    max += s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator -= (Value s)
{
    min -= s;
    max -= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator *= (Value s)
{
    min *= s;
    max *= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator /= (Value s)
{
    min /= s;
    max /= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator %= (Value s)
{
    min %= s;
    max %= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator ++ ()
{
    ++min;
    ++max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator -- ()
{
    --min;
    --max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator ++ (int)
{
    AABB state(*this);
    ++min;
    ++max;
    return state;
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator -- (int)
{
    AABB state(*this);
    --min;
    --max;
    return state;
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator + (const AABB & b) const
{
    return AABB(min + b.min, max + b.max);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator - (const AABB & b) const
{
    return AABB(min - b.min, max - b.max);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator * (const AABB & b) const
{
    return AABB(min * b.min, max * b.max);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator / (const AABB & b) const
{
    return AABB(min / b.min, max / b.max);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator % (const AABB & b) const
{
    return AABB(min % b.min, max % b.max);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator + (Value s) const
{
    return AABB(min + s, max + s);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator - (Value s) const
{
    return AABB(min - s, max - s);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator * (Value s) const
{
    return AABB(min * s, max * s);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator / (Value s) const
{
    return AABB(min / s, max / s);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator % (Value s) const
{
    return AABB(min % s, max % s);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator + () const
{
    return AABB(min.Abs(), max.Abs());
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator - () const
{
    return AABB(-min, -max);
}

// ------------------------------------------------------------------------------------------------
bool AABB::operator == (const AABB & b) const
{
    return (min == b.min) && (max == b.max);
}

// ------------------------------------------------------------------------------------------------
bool AABB::operator != (const AABB & b) const
{
    return (min != b.min) || (max != b.max);
}

// ------------------------------------------------------------------------------------------------
bool AABB::operator < (const AABB & b) const
{
    return (min < b.min) && (max < b.max);
}

// ------------------------------------------------------------------------------------------------
bool AABB::operator > (const AABB & b) const
{
    return (min > b.min) && (max > b.max);
}

// ------------------------------------------------------------------------------------------------
bool AABB::operator <= (const AABB & b) const
{
    return (min <= b.min) && (max <= b.max);
}

// ------------------------------------------------------------------------------------------------
bool AABB::operator >= (const AABB & b) const
{
    return (min >= b.min) && (max >= b.max);
}

// ------------------------------------------------------------------------------------------------
Int32 AABB::Cmp(const AABB & o) const
{
    if (*this == o)
    {
        return 0;
    }
    else if (*this > o)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
CSStr AABB::ToString() const
{
    return ToStrF("%f,%f,%f,%f,%f,%f", min.x, min.y, min.z, max.x, max.y, max.z);
}

// ------------------------------------------------------------------------------------------------
void AABB::SetStr(CSStr values, SQChar delim)
{
    DefineAABB(AABB::Get(values, delim));
}

// ------------------------------------------------------------------------------------------------
void AABB::Clear()
{
    min.SetVector3Ex(HUGE_VALF, HUGE_VALF, HUGE_VALF);
    max.SetVector3Ex(-HUGE_VALF, -HUGE_VALF, -HUGE_VALF);
}

// ------------------------------------------------------------------------------------------------
void AABB::DefineScalar(Value mins, Value maxs)
{
    min.SetVector3Ex(mins, mins, mins);
    max.SetVector3Ex(maxs, maxs, maxs);
}

// ------------------------------------------------------------------------------------------------
void AABB::DefineVector3(const Vector3 & point)
{
    min = max = point;
}

// ------------------------------------------------------------------------------------------------
void AABB::DefineVector3Ex(Value x, Value y, Value z)
{
    min.SetVector3Ex(x, y, z);
    max.SetVector3Ex(x, y, z);
}

// ------------------------------------------------------------------------------------------------
void AABB::DefineAllVector3(const Vector3 & nmin, const Vector3 & nmax)
{
    min = nmin;
    max = nmax;
}

// ------------------------------------------------------------------------------------------------
void AABB::DefineAllVector3Ex(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax)
{
    min.SetVector3Ex(xmin, ymin, zmin);
    max.SetVector3Ex(xmax, ymax, zmax);
}

// ------------------------------------------------------------------------------------------------
void AABB::DefineAABB(const AABB & box)
{
    min = box.min;
    max = box.max;
}

// ------------------------------------------------------------------------------------------------
void AABB::DefineSphere(const Sphere & sphere)
{
    min = sphere.pos + Vector3(-sphere.rad);
    max = sphere.pos + Vector3(sphere.rad);
}

// ------------------------------------------------------------------------------------------------
void AABB::DefineSphereEx(Value x, Value y, Value z, Value r)
{
    DefineSphere(Sphere(x, y, z, r));
}

// ------------------------------------------------------------------------------------------------
void AABB::MergeVector3(const Vector3 & point)
{
    MergeVector3Ex(point.x, point.y, point.z);
}

// ------------------------------------------------------------------------------------------------
void AABB::MergeVector3Ex(Value x, Value y, Value z)
{
    if (x < min.x)
    {
        min.x = x;
    }
    if (y < min.y)
    {
        min.y = y;
    }
    if (z < min.z)
    {
        min.z = z;
    }
    if (x > max.x)
    {
        max.x = x;
    }
    if (y > max.y)
    {
        max.y = y;
    }
    if (z > max.z)
    {
        max.z = z;
    }
}

// ------------------------------------------------------------------------------------------------
void AABB::MergeAABB(const AABB & box)
{
    if (box.min.x < min.x)
    {
        min.x = box.min.x;
    }
    if (box.min.y < min.y)
    {
        min.y = box.min.y;
    }
    if (box.min.z < min.z)
    {
        min.z = box.min.z;
    }
    if (box.max.x > max.x)
    {
        max.x = box.max.x;
    }
    if (box.max.y > max.y)
    {
        max.y = box.max.y;
    }
    if (box.max.z > max.z)
    {
        max.z = box.max.z;
    }
}

// ------------------------------------------------------------------------------------------------
void AABB::MergeAABBEx(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax)
{
    if (xmin < min.x)
    {
        min.x = xmin;
    }
    if (ymin < min.y)
    {
        min.y = ymin;
    }
    if (zmin < min.z)
    {
        min.z = zmin;
    }
    if (xmax > max.x)
    {
        max.x = xmax;
    }
    if (ymax > max.y)
    {
        max.y = ymax;
    }
    if (zmax > max.z)
    {
        max.z = zmax;
    }
}

// ------------------------------------------------------------------------------------------------
void AABB::MergeSphere(const Sphere & sphere)
{
    MergeVector3(sphere.pos + Vector3(sphere.rad));
    MergeVector3(sphere.pos + Vector3(-sphere.rad));
}

// ------------------------------------------------------------------------------------------------
void AABB::MergeSphereEx(Value x, Value y, Value z, Value r)
{
    MergeSphere(Sphere(x, y, z, r));
}

// ------------------------------------------------------------------------------------------------
bool AABB::Empty() const
{
    return (min == max);
}

// ------------------------------------------------------------------------------------------------
bool AABB::Defined() const
{
    return min.x != HUGE_VALF;
}

// ------------------------------------------------------------------------------------------------
Vector3 AABB::Center() const
{
    return (max + min) * 0.5f;
}

// ------------------------------------------------------------------------------------------------
Vector3 AABB::Size() const
{
    return max - min;
}

// ------------------------------------------------------------------------------------------------
Vector3 AABB::HalfSize() const
{
    return (max - min) * 0.5f;
}

// ------------------------------------------------------------------------------------------------
AABB::Value AABB::Radius() const
{
    return Size().GetLength() / Value(2);
}

// ------------------------------------------------------------------------------------------------
AABB::Value AABB::Volume() const
{
    const Vector3 v = Size();
    return static_cast< Value >(v.x * v.y * v.z);
}

// ------------------------------------------------------------------------------------------------
AABB::Value AABB::Area() const
{
    const Vector3 v = Size();
    return static_cast< Value >(Value(2) * (v.x * v.y + v.x * v.z + v.y * v.z));
}

// ------------------------------------------------------------------------------------------------
Int32 AABB::IsVector3Inside(const Vector3 & point) const
{
    return (point.x < min.x || point.x > max.x ||
            point.y < min.y || point.y > max.y ||
            point.z < min.z || point.z > max.z) ? SQMODI_OUTSIDE : SQMODI_INSIDE;
}

// ------------------------------------------------------------------------------------------------
Int32 AABB::IsVector3InsideEx(Value x, Value y, Value z) const
{
    return (x < min.x || x > max.x ||
            y < min.y || y > max.y ||
            z < min.z || z > max.z) ? SQMODI_OUTSIDE : SQMODI_INSIDE;
}

// ------------------------------------------------------------------------------------------------
Int32 AABB::IsAABBInside(const AABB & box) const
{
    if (box.max.x < min.x || box.min.x > max.x ||
        box.max.y < min.y || box.min.y > max.y ||
        box.max.z < min.z || box.min.z > max.z)
    {
        return SQMODI_OUTSIDE;
    }
    else if (box.min.x < min.x || box.max.x > max.x ||
             box.min.y < min.y || box.max.y > max.y ||
             box.min.z < min.z || box.max.z > max.z)
    {
        return SQMODI_INTERSECTS;
    }
    else
    {
        return SQMODI_INSIDE;
    }
}

// ------------------------------------------------------------------------------------------------
Int32 AABB::IsAABBInsideEx(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax) const
{
    if (xmax < min.x || xmin > max.x ||
        ymax < min.y || ymin > max.y ||
        zmax < min.z || zmin > max.z)
    {
        return SQMODI_OUTSIDE;
    }
    else if (xmin < min.x || xmax > max.x ||
             ymin < min.y || ymax > max.y ||
             zmin < min.z || zmax > max.z)
    {
        return SQMODI_INTERSECTS;
    }
    else
    {
        return SQMODI_INSIDE;
    }
}

// ------------------------------------------------------------------------------------------------
Int32 AABB::IsAABBInsideFast(const AABB & box) const
{
    if (box.max.x < min.x || box.min.x > max.x ||
        box.max.y < min.y || box.min.y > max.y ||
        box.max.z < min.z || box.min.z > max.z)
    {
        return SQMODI_OUTSIDE;
    }
    else
    {
        return SQMODI_INSIDE;
    }
}

// ------------------------------------------------------------------------------------------------
Int32 AABB::IsAABBInsideFastEx(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax) const
{
    if (xmax < min.x || xmin > max.x ||
        ymax < min.y || ymin > max.y ||
        zmax < min.z || zmin > max.z)
    {
        return SQMODI_OUTSIDE;
    }
    else
    {
        return SQMODI_INSIDE;
    }
}

// ------------------------------------------------------------------------------------------------
Int32 AABB::IsSphereInside(const Sphere & sphere) const
{
    Value dist_squared = 0, temp;
    const Vector3 & center = sphere.pos;

    if (center.x < min.x)
    {
        temp = center.x - min.x;
        dist_squared += temp * temp;
    }
    else if (center.x > max.x)
    {
        temp = center.x - max.x;
        dist_squared += temp * temp;
    }
    if (center.y < min.y)
    {
        temp = center.y - min.y;
        dist_squared += temp * temp;
    }
    else if (center.y > max.y)
    {
        temp = center.y - max.y;
        dist_squared += temp * temp;
    }
    if (center.z < min.z)
    {
        temp = center.z - min.z;
        dist_squared += temp * temp;
    }
    else if (center.z > max.z)
    {
        temp = center.z - max.z;
        dist_squared += temp * temp;
    }

    const Value radius = sphere.rad;

    if (dist_squared >= radius * radius)
    {
        return SQMODI_OUTSIDE;
    }
    else if (center.x - radius < min.x || center.x + radius > max.x || center.y - radius < min.y ||
             center.y + radius > max.y || center.z - radius < min.z || center.z + radius > max.z)
    {
        return SQMODI_INTERSECTS;
    }
    else
    {
        return SQMODI_INSIDE;
    }
}

// ------------------------------------------------------------------------------------------------
Int32 AABB::IsSphereInsideEx(Value x, Value y, Value z, Value r) const
{
    return IsSphereInside(Sphere(x, y, z, r));
}


// ------------------------------------------------------------------------------------------------
Int32 AABB::IsSphereInsideFast(const Sphere & sphere) const
{
    Value dist_squared = 0, temp;
    const Vector3& center = sphere.pos;

    if (center.x < min.x)
    {
        temp = center.x - min.x;
        dist_squared += temp * temp;
    }
    else if (center.x > max.x)
    {
        temp = center.x - max.x;
        dist_squared += temp * temp;
    }
    if (center.y < min.y)
    {
        temp = center.y - min.y;
        dist_squared += temp * temp;
    }
    else if (center.y > max.y)
    {
        temp = center.y - max.y;
        dist_squared += temp * temp;
    }
    if (center.z < min.z)
    {
        temp = center.z - min.z;
        dist_squared += temp * temp;
    }
    else if (center.z > max.z)
    {
        temp = center.z - max.z;
        dist_squared += temp * temp;
    }

    const Value radius = sphere.rad;

    if (dist_squared >= radius * radius)
    {
        return SQMODI_OUTSIDE;
    }
    else
    {
        return SQMODI_INSIDE;
    }
}

// ------------------------------------------------------------------------------------------------
Int32 AABB::IsSphereInsideFastEx(Value x, Value y, Value z, Value r) const
{
    return IsSphereInsideFast(Sphere(x, y, z, r));
}

// ------------------------------------------------------------------------------------------------
const AABB & AABB::Get(CSStr str)
{
    return AABB::Get(str, AABB::Delim);
}

// ------------------------------------------------------------------------------------------------
const AABB & AABB::Get(CSStr str, SQChar delim)
{
    // The format specifications that will be used to scan the string
    static SQChar fs[] = _SC(" %f , %f , %f , %f , %f , %f ");
    static AABB box;
    // Clear previous values, if any
    box.Clear();
    // Is the specified string empty?
    if (!str || *str == '\0')
    {
        return box; // Return the value as is!
    }
    // Assign the specified delimiter
    fs[4] = delim;
    fs[9] = delim;
    fs[14] = delim;
    fs[19] = delim;
    fs[24] = delim;
    // Attempt to extract the component values from the specified string
    std::sscanf(str, fs, &box.min.x, &box.min.y, &box.min.z, &box.max.x, &box.max.y, &box.max.z);
    // Return the resulted value
    return box;
}

// ------------------------------------------------------------------------------------------------
const AABB & GetAABB()
{
    static AABB box;
    box.Clear();
    return box;
}

// ------------------------------------------------------------------------------------------------
const AABB & GetAABB(Float32 mins, Float32 maxs)
{
    static AABB box;
    box.DefineScalar(mins, maxs);
    return box;
}

// ------------------------------------------------------------------------------------------------
const AABB & GetAABB(Float32 xv, Float32 yv, Float32 zv)
{
    static AABB box;
    box.DefineVector3Ex(xv, yv, zv);
    return box;
}

// ------------------------------------------------------------------------------------------------
const AABB & GetAABB(Float32 xmin, Float32 ymin, Float32 zmin, Float32 xmax, Float32 ymax, Float32 zmax)
{
    static AABB box;
    box.DefineAllVector3Ex(xmin, ymin, zmin, xmax, ymax, zmax);
    return box;
}

// ------------------------------------------------------------------------------------------------
const AABB & GetAABB(const Vector3 & vmin, const Vector3 & vmax)
{
    static AABB box;
    box.DefineAllVector3(vmin, vmax);
    return box;
}

// ------------------------------------------------------------------------------------------------
const AABB & GetAABB(const AABB & o)
{
    static AABB box;
    box.DefineAABB(o);
    return box;
}

// ================================================================================================
void Register_AABB(HSQUIRRELVM vm)
{
    typedef AABB::Value Val;

    RootTable(vm).Bind(_SC("AABB"), Class< AABB >(vm, _SC("AABB"))
        // Constructors
        .Ctor()
        .Ctor< const AABB & >()
        .Ctor< Val, Val, Val >()
        .Ctor< Val, Val, Val, Val, Val, Val >()
        .Ctor< const Vector3 &, const Vector3 & >()
        // Member Variables
        .Var(_SC("min"), &AABB::min)
        .Var(_SC("max"), &AABB::max)
        .Var(_SC("Min"), &AABB::min)
        .Var(_SC("Max"), &AABB::max)
        // Core Meta-methods
        .Func(_SC("_tostring"), &AABB::ToString)
        .SquirrelFunc(_SC("_typename"), &AABB::Typename)
        // We cannot set _cmp for c++ classes so we use this instead
        .SquirrelFunc(_SC("cmp"), &SqDynArgFwd< SqDynArgCmpFn< AABB >, SQFloat, SQInteger, bool, std::nullptr_t, AABB >)
        // Meta-methods
        .SquirrelFunc(_SC("_add"), &SqDynArgFwd< SqDynArgAddFn< AABB >, SQFloat, SQInteger, bool, std::nullptr_t, AABB >)
        .SquirrelFunc(_SC("_sub"), &SqDynArgFwd< SqDynArgSubFn< AABB >, SQFloat, SQInteger, bool, std::nullptr_t, AABB >)
        .SquirrelFunc(_SC("_mul"), &SqDynArgFwd< SqDynArgMulFn< AABB >, SQFloat, SQInteger, bool, std::nullptr_t, AABB >)
        .SquirrelFunc(_SC("_div"), &SqDynArgFwd< SqDynArgDivFn< AABB >, SQFloat, SQInteger, bool, std::nullptr_t, AABB >)
        .SquirrelFunc(_SC("_modulo"), &SqDynArgFwd< SqDynArgModFn< AABB >, SQFloat, SQInteger, bool, std::nullptr_t, AABB >)
        .Func< AABB (AABB::*)(void) const >(_SC("_unm"), &AABB::operator -)
        // Properties
        .Prop(_SC("Empty"), &AABB::Empty)
        .Prop(_SC("Defined"), &AABB::Defined)
        .Prop(_SC("Center"), &AABB::Center)
        .Prop(_SC("Size"), &AABB::Size)
        .Prop(_SC("Extent"), &AABB::Size)
        .Prop(_SC("HalfSize"), &AABB::HalfSize)
        .Prop(_SC("HalfExtent"), &AABB::HalfSize)
        .Prop(_SC("Radius"), &AABB::Radius)
        .Prop(_SC("Volume"), &AABB::Volume)
        .Prop(_SC("Area"), &AABB::Area)
        // Member Methods
        .Func(_SC("SetStr"), &AABB::SetStr)
        .Func(_SC("Clear"), &AABB::Clear)
        .Func(_SC("DefineScalar"), &AABB::DefineScalar)
        .Func(_SC("DefineVector3"), &AABB::DefineVector3)
        .Func(_SC("DefineVector3Ex"), &AABB::DefineVector3Ex)
        .Func(_SC("DefineAllVector3"), &AABB::DefineAllVector3)
        .Func(_SC("DefineAllVector3Ex"), &AABB::DefineAllVector3Ex)
        .Func(_SC("DefineAABB"), &AABB::DefineAABB)
        .Func(_SC("DefineSphere"), &AABB::DefineSphere)
        .Func(_SC("DefineSphereEx"), &AABB::DefineSphereEx)
        .Func(_SC("MergeVector3"), &AABB::MergeVector3)
        .Func(_SC("MergeVector3Ex"), &AABB::MergeVector3Ex)
        .Func(_SC("MergeAABB"), &AABB::MergeAABB)
        .Func(_SC("MergeAABBEx"), &AABB::MergeAABBEx)
        .Func(_SC("MergeSphere"), &AABB::MergeSphere)
        .Func(_SC("MergeSphereEx"), &AABB::MergeSphereEx)
        .Func(_SC("IsVector3Inside"), &AABB::IsVector3Inside)
        .Func(_SC("IsVector3InsideEx"), &AABB::IsVector3InsideEx)
        .Func(_SC("IsAABBInside"), &AABB::IsAABBInside)
        .Func(_SC("IsAABBInsideEx"), &AABB::IsAABBInsideEx)
        .Func(_SC("IsAABBInsideFast"), &AABB::IsAABBInsideFast)
        .Func(_SC("IsAABBInsideFastEx"), &AABB::IsAABBInsideFastEx)
        .Func(_SC("IsSphereInside"), &AABB::IsSphereInside)
        .Func(_SC("IsSphereInsideEx"), &AABB::IsSphereInsideEx)
        .Func(_SC("IsSphereInsideFast"), &AABB::IsSphereInsideFast)
        .Func(_SC("IsSphereInsideFastEx"), &AABB::IsSphereInsideFastEx)
        // Static Overloads
        .StaticOverload< const AABB & (*)(CSStr) >(_SC("FromStr"), &AABB::Get)
        .StaticOverload< const AABB & (*)(CSStr, SQChar) >(_SC("FromStr"), &AABB::Get)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< AABB >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< AABB >)
        // Operator Exposure
        .Func< AABB & (AABB::*)(const AABB &) >(_SC("opAddAssign"), &AABB::operator +=)
        .Func< AABB & (AABB::*)(const AABB &) >(_SC("opSubAssign"), &AABB::operator -=)
        .Func< AABB & (AABB::*)(const AABB &) >(_SC("opMulAssign"), &AABB::operator *=)
        .Func< AABB & (AABB::*)(const AABB &) >(_SC("opDivAssign"), &AABB::operator /=)
        .Func< AABB & (AABB::*)(const AABB &) >(_SC("opModAssign"), &AABB::operator %=)
        .Func< AABB & (AABB::*)(AABB::Value) >(_SC("opAddAssignS"), &AABB::operator +=)
        .Func< AABB & (AABB::*)(AABB::Value) >(_SC("opSubAssignS"), &AABB::operator -=)
        .Func< AABB & (AABB::*)(AABB::Value) >(_SC("opMulAssignS"), &AABB::operator *=)
        .Func< AABB & (AABB::*)(AABB::Value) >(_SC("opDivAssignS"), &AABB::operator /=)
        .Func< AABB & (AABB::*)(AABB::Value) >(_SC("opModAssignS"), &AABB::operator %=)
        .Func< AABB & (AABB::*)(void) >(_SC("opPreInc"), &AABB::operator ++)
        .Func< AABB & (AABB::*)(void) >(_SC("opPreDec"), &AABB::operator --)
        .Func< AABB (AABB::*)(int) >(_SC("opPostInc"), &AABB::operator ++)
        .Func< AABB (AABB::*)(int) >(_SC("opPostDec"), &AABB::operator --)
        .Func< AABB (AABB::*)(const AABB &) const >(_SC("opAdd"), &AABB::operator +)
        .Func< AABB (AABB::*)(const AABB &) const >(_SC("opSub"), &AABB::operator -)
        .Func< AABB (AABB::*)(const AABB &) const >(_SC("opMul"), &AABB::operator *)
        .Func< AABB (AABB::*)(const AABB &) const >(_SC("opDiv"), &AABB::operator /)
        .Func< AABB (AABB::*)(const AABB &) const >(_SC("opMod"), &AABB::operator %)
        .Func< AABB (AABB::*)(AABB::Value) const >(_SC("opAddS"), &AABB::operator +)
        .Func< AABB (AABB::*)(AABB::Value) const >(_SC("opSubS"), &AABB::operator -)
        .Func< AABB (AABB::*)(AABB::Value) const >(_SC("opMulS"), &AABB::operator *)
        .Func< AABB (AABB::*)(AABB::Value) const >(_SC("opDivS"), &AABB::operator /)
        .Func< AABB (AABB::*)(AABB::Value) const >(_SC("opModS"), &AABB::operator %)
        .Func< AABB (AABB::*)(void) const >(_SC("opUnPlus"), &AABB::operator +)
        .Func< AABB (AABB::*)(void) const >(_SC("opUnMinus"), &AABB::operator -)
        .Func< bool (AABB::*)(const AABB &) const >(_SC("opEqual"), &AABB::operator ==)
        .Func< bool (AABB::*)(const AABB &) const >(_SC("opNotEqual"), &AABB::operator !=)
        .Func< bool (AABB::*)(const AABB &) const >(_SC("opLessThan"), &AABB::operator <)
        .Func< bool (AABB::*)(const AABB &) const >(_SC("opGreaterThan"), &AABB::operator >)
        .Func< bool (AABB::*)(const AABB &) const >(_SC("opLessEqual"), &AABB::operator <=)
        .Func< bool (AABB::*)(const AABB &) const >(_SC("opGreaterEqual"), &AABB::operator >=)
    );
}

} // Namespace:: SqMod
