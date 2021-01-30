// ------------------------------------------------------------------------------------------------
#include "Core/Areas.hpp"

// ------------------------------------------------------------------------------------------------
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(AreaTypename, _SC("SqArea"))

// ------------------------------------------------------------------------------------------------
AreaManager AreaManager::s_Inst;

// ------------------------------------------------------------------------------------------------
void Area::AddArray(const Sqrat::Array & a)
{
    float values[2];

    a.Foreach([this, &values, n = int(0)](HSQUIRRELVM vm, SQInteger i) mutable -> SQRESULT {
        // Retrieve the type of the value
        const SQObjectType type = sq_gettype(vm, -1);
        // Are we dealing with a vector?
        if (type == OT_INSTANCE)
        {
            // Next time, start again for floats
            n = 0;
            // Grab the instance from the stack
            this->AddPoint(*ClassType< Vector2 >::GetInstance(vm, -1));
        }
        else if (type & SQOBJECT_NUMERIC)
        {
            // Retrieve the value from the stack
            values[n] = Var< float >(vm, -1).value;
            // Do we have enough to form a vector?
            if (++n == 2)
            {
                this->AddPointEx(values[0], values[1]);
                // Reset the counter
                n = 0;
            }
        }
        // Ignore anything else
        return SQ_OK;
    });
}
// ------------------------------------------------------------------------------------------------
void Area::AddCircleEx(SQFloat cx, SQFloat cy, SQFloat cr, SQInteger num_segments)
{
    for(SQInteger i = 0; i < num_segments; ++i)
    {
        CheckLock();
        // Get the current angle
#ifdef SQUSEDOUBLE
        SQFloat theta = 2.0d * SQMOD_PI64 * static_cast< SQFloat >(i) / static_cast< SQFloat >(num_segments);
#else
        SQFloat theta = 2.0f * SQMOD_PI * static_cast< SQFloat >(i) / static_cast< SQFloat >(num_segments);
#endif // SQUSEDOUBLE
        // Calculate the x component
        SQFloat x = (cr * std::cos(theta)) + cx;
        // Calculate the y component
        SQFloat y = (cr * std::sin(theta)) + cy;
        // Insert the point into the list
        mPoints.emplace_back(x, y);
        // Update the bounding box
        Expand(x, y);
    }
}

// ------------------------------------------------------------------------------------------------
bool Area::Manage()
{
    // Are we connected to any cells?
    if (!mCells.empty())
    {
        STHROWF("The area is already managed");
    }
    // We expect this to be called only from the script so that the first parameter in the vm
    // is the area instance
    LightObj obj(1, SqVM());
    // Attempt to manage this area
    AreaManager::Get().InsertArea(*this, obj);
    // Return whether the area is now managed by any cells
    return !mCells.empty();
}

// ------------------------------------------------------------------------------------------------
bool Area::Unmanage()
{
    // Are we connected to any cells?
    if (mCells.empty())
    {
        return true; // Already unmanaged
    }
    // Attempt to unmanage this area
    AreaManager::Get().RemoveArea(*this);
    // Return whether the area is not managed by any cells
    return mCells.empty();
}

// ------------------------------------------------------------------------------------------------
bool Area::IsInside(float x, float y) const
{
    // Is the an area to test?
    if (mPoints.size() < 3)
    {
        return false; // Can't possibly be in an area that doesn't exist
    }
    // http://sidvind.com/wiki/Point-in-polygon:_Jordan_Curve_Theorem
    // The points creating the polygon
    float x1, x2;
    // How many times the ray crosses a line segment
    int crossings = 0;
    // Iterate through each line
    for (uint32_t i = 0, n = static_cast< uint32_t >(mPoints.size()); i < n; ++i)
    {
        Points::const_reference a = mPoints[i];
        Points::const_reference b = mPoints[(i + 1) % n];
        // This is done to ensure that we get the same result when
        // the line goes from left to right and right to left.
        if (a.x < b.x)
        {
            x1 = a.x;
            x2 = b.x;
        }
        else
        {
            x1 = b.x;
            x2 = a.x;
        }
        // First check if the ray is able to cross the line
        if (x > x1 && x <= x2 && (y < a.y || y <= b.y))
        {
            // Calculate the equation of the line
            const float dx = (b.x - a.x);
            const float dy = (b.y - a.y);
            float k;

            if (fabsf(dx) < 0.000001f)
            {
                k = static_cast< float >(0xffffffffu); // NOLINT(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
            }
            else
            {
                k = (dy / dx);
            }

            const float m = (a.y - k * a.x);
            const float y2 = (k * x + m);
            // Does the ray cross the line?
            if (y <= y2)
            {
                ++crossings;
            }
        }
    }
    // Return if the crossings are not even
    return (crossings % 2 == 1);
}

// ------------------------------------------------------------------------------------------------
AreaManager::AreaManager(size_t sz) noexcept
    : m_Queue(), m_ProcList(), m_Grid{}
{
    // Negative half grid size (left)
    int l = (-GRIDH * CELLD);
    // Positive half grid size minus one cell (bottom)
    int b = (abs(l) - CELLD);
    // Negative half grid size minus one cell (right)
    int r = (l + CELLD);
    // Positive half grid size (top)
    int t = abs(l);
    // Initialize the grid cells
    for (auto & a : m_Grid)
    {
        for (auto & c : a)
        {
            // Grab a reference to the cell
            // Configure the range of the cell
            c.mL = static_cast< float >(l);
            c.mB = static_cast< float >(b);
            c.mR = static_cast< float >(r);
            c.mT = static_cast< float >(t);
            // Reserve area memory if requested
            c.mAreas.reserve(sz);
            // Reset the locks on this area
            c.mLocks = 0;
            // Advance the left side
            l = r;
            // Advance the right side
            r += CELLD;
            // Should we advance to the next row?
            if (r > (GRIDH * CELLD))
            {
                // Reset the left side
                l = (-GRIDH * CELLD);
                // Reset the right side
                r = (l + CELLD);
                // Advance the bottom
                b -= CELLD;
                // Advance the top
                t -= CELLD;
            }
        }
    }
    // Reserve some space in the queue
    m_Queue.reserve(128);
    m_ProcList.reserve(128);
}

// ------------------------------------------------------------------------------------------------
void AreaManager::Insert(AreaCell & c, Area & a, LightObj & obj)
{
    // Is this cell currently locked?
    if (c.mLocks)
    {
        m_Queue.emplace_back(c, a, obj); // Queue this request for now
    }
    else
    {
        c.mAreas.emplace_back(&a, obj);
    }
    // Associate the area with this cell so it can't be managed again (even while in the queue)
    a.mCells.push_back(&c);
}

// ------------------------------------------------------------------------------------------------
void AreaManager::Remove(AreaCell & c, Area & a)
{
    // Is this cell currently locked?
    if (c.mLocks)
    {
        m_Queue.emplace_back(c, a); // Queue this request for now
    }
    else
    {
        // Attempt to locate this area in the cell
        auto itr = std::find_if(c.mAreas.begin(), c.mAreas.end(),
            [&a](AreaCell::Areas::reference p) -> bool {
                return (p.first == &a);
        });
        // Have we found it?
        if (itr != c.mAreas.end())
        {
            c.mAreas.erase(itr); // Erase it
        }
    }
    // Dissociate the area with this cell so it can be managed again (even while in the queue)
    auto itr = std::find(a.mCells.begin(), a.mCells.end(), &c);
    // Was is associated?
    if (itr != a.mCells.end())
    {
        a.mCells.erase(itr); // Dissociate them
    }
}

// ------------------------------------------------------------------------------------------------
void AreaManager::ProcQueue()
{
    // Look for actions that can be completed
    for (auto itr = m_Queue.begin(); itr != m_Queue.end(); ++itr)
    {
        // Was this cell unlocked in the meantime?
        if (itr->mCell->mLocks <= 0)
        {
            m_ProcList.push_back(itr);
        }
    }
    // Process the actions that are ready
    for (auto & itr : m_ProcList)
    {
        // Was this a remove request?
        if (itr->mObj.IsNull())
        {
            Remove(*(itr->mCell), *(itr->mArea));
        }
        else
        {
            Insert(*(itr->mCell), *(itr->mArea), itr->mObj);
        }
    }
    // Remove processed requests
    for (auto & itr : m_ProcList)
    {
        m_Queue.erase(itr);
    }
    // Actions were processed
    m_ProcList.clear();
}

// ------------------------------------------------------------------------------------------------
void AreaManager::Clear()
{
    // Clear the cells
    for (AreaCell (&row)[GRIDN] : m_Grid)
    {
        for (AreaCell & c : row)
        {
            c.mAreas.clear();
        }
    }
    // Clear the queue as well
    m_Queue.clear();
    m_ProcList.clear();
}

// ------------------------------------------------------------------------------------------------
void AreaManager::InsertArea(Area & a, LightObj & obj)
{
    // See if this area is already managed
    if (!a.mCells.empty() || a.mPoints.empty())
    {
        return; // Already managed or nothing to manage
    }
    // Go through each cell and check if the area touches it
    for (auto & y : m_Grid)
    {
        for (auto & c : y)
        {
            // Does the bounding box of this cell intersect with the one of the area?
            if (a.mL <= c.mR && c.mL <= a.mR && a.mB <= c.mT && c.mB <= a.mT)
            {
                Insert(c, a, obj); // Attempt to insert the area into this cell
            }
        }
    }
}

// ------------------------------------------------------------------------------------------------
void AreaManager::RemoveArea(Area & a)
{
    // Just remove the associated cells
    for (auto c : a.mCells)
    {
        Remove(*c, a);
    }
}

// ------------------------------------------------------------------------------------------------
Vector2i AreaManager::LocateCell(float x, float y)
{
    // Transform the world coordinates into a cell coordinates
    // and cast to integral after rounding the value
    int xc = static_cast< int >(std::round(x / CELLD));
    int yc = static_cast< int >(std::round(y / CELLD));
    // Grab the absolute cell coordinates for range checking
    const int xca = std::abs(xc);
    const int yca = std::abs(yc);
    // Make sure the cell coordinates are within range
    if (xca > (GRIDH+1) || yca > (GRIDH+1))
    {
        return {NOCELL, NOCELL}; // Out of our scanning area
    }
    // Clamp the x coordinate if necessary
    if (xca >= (GRIDH))
    {
        xc = xc < 0 ? -(GRIDH-1) : (GRIDH-1);
    }
    // Clamp the y coordinate if necessary
    if (yca >= (GRIDH))
    {
        yc = xc < 0 ? -(GRIDH-1) : (GRIDH-1);
    }
    // Return the identified cell row and column
    return {GRIDH+xc, GRIDH-yc};
}

// ------------------------------------------------------------------------------------------------
static void Areas_TestPointEx(float x, float y, Function & func)
{
    // Is the function valid?
    if (func.IsNull())
    {
        STHROWF("Invalid callback object");
    }
    // Begin testing
    AreaManager::Get().TestPoint([&func](AreaCell::Areas::reference ap) -> void {
        func.Execute(ap.second);
    }, x, y);
}

// ------------------------------------------------------------------------------------------------
static void Areas_TestPoint(const Vector2 & v, Function & func)
{
    Areas_TestPointEx(v.x, v.y, func);
}

// ------------------------------------------------------------------------------------------------
static void Areas_TestPointOnEx(float x, float y, Object & ctx, Function & func)
{
    // Begin testing
    AreaManager::Get().TestPoint([&ctx, &func](AreaCell::Areas::reference ap) -> void {
        func.Execute(ctx, ap.second);
    }, x, y);
}

// ------------------------------------------------------------------------------------------------
static void Areas_TestPointOn(const Vector2 & v, Object & ctx, Function & func)
{
    Areas_TestPointOnEx(v.x, v.y, ctx, func);
}

// ------------------------------------------------------------------------------------------------
static Vector2i Areas_LocatePointCell(const Vector2 & v)
{
    return AreaManager::Get().LocateCell(v.x, v.y);
}

// ------------------------------------------------------------------------------------------------
static Vector2i Areas_LocatePointCellEx(float x, float y)
{
    return AreaManager::Get().LocateCell(x, y);
}

// ------------------------------------------------------------------------------------------------
void TerminateAreas()
{
    AreaManager::Get().Clear();
}

// ================================================================================================
void Register_Areas(HSQUIRRELVM vm)
{
     RootTable(vm).Bind(_SC("SqArea"),
        Class< Area >(vm, AreaTypename::Str)
        // Constructors
        .Ctor()
        .Ctor< StackStrF & >()
        .Ctor< SQInteger, StackStrF & >()
        .Ctor< const Vector2 &, const Vector2 &, const Vector2 & >()
        .Ctor< const Vector2 &, const Vector2 &, const Vector2 &, StackStrF & >()
        .Ctor< const Vector2 &, const Vector2 &, const Vector2 &, SQInteger, StackStrF & >()
        .Ctor< float, float, float, float, float, float >()
        .Ctor< float, float, float, float, float, float, StackStrF & >()
        .Ctor< float, float, float, float, float, float, SQInteger, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &AreaTypename::Fn)
        .Func(_SC("_tostring"), &Area::ToString)
        // Member Properties
        .Prop(_SC("Name"), &Area::GetName, &Area::SetName)
        .Prop(_SC("ID"), &Area::GetID, &Area::SetID)
        .Prop(_SC("Locked"), &Area::IsLocked)
        .Prop(_SC("IsLocked"), &Area::IsLocked)
        .Prop(_SC("Center"), &Area::GetCenter)
        .Prop(_SC("Box"), &Area::GetBoundingBox, &Area::SetBoundingBox)
        .Prop(_SC("Empty"), &Area::Empty)
        .Prop(_SC("Empty"), &Area::Empty)
        .Prop(_SC("Size"), &Area::Size)
        .Prop(_SC("Points"), &Area::Size)
        .Prop(_SC("Capacity"), &Area::Capacity)
        // Member Methods
        .FmtFunc(_SC("SetName"), &Area::ApplyName)
        .Func(_SC("SetID"), &Area::ApplyID)
        .Func(_SC("Clear"), &Area::Clear)
        .Func(_SC("Reserve"), &Area::Reserve)
        .Func(_SC("SetBox"), &Area::SetBoundingBoxEx)
        .Func(_SC("SetBoundingBox"), &Area::SetBoundingBoxEx)
        .Func(_SC("Add"), &Area::AddPoint)
        .Func(_SC("AddEx"), &Area::AddPointEx)
        .Func(_SC("AddVirtual"), &Area::AddVirtualPoint)
        .Func(_SC("AddVirtualEx"), &Area::AddVirtualPointEx)
        .Func(_SC("AddCircle"), &Area::AddCircle)
        .Func(_SC("AddCircleEx"), &Area::AddCircleEx)
        .Func(_SC("AddFake"), &Area::AddVirtualPoint)
        .Func(_SC("AddFakeEx"), &Area::AddVirtualPointEx)
        .Func(_SC("AddArray"), &Area::AddArray)
        .Func(_SC("Test"), &Area::Test)
        .Func(_SC("TestEx"), &Area::TestEx)
        .Func(_SC("Manage"), &Area::Manage)
        .Func(_SC("Unmanage"), &Area::Unmanage)
        // Static Functions
        .StaticFunc(_SC("GlobalTest"), &Areas_TestPoint)
        .StaticFunc(_SC("GlobalTestEx"), &Areas_TestPointEx)
        .StaticFunc(_SC("GlobalTestOn"), &Areas_TestPointOn)
        .StaticFunc(_SC("GlobalTestOnEx"), &Areas_TestPointOnEx)
        .StaticFunc(_SC("LocatePointCell"), &Areas_LocatePointCell)
        .StaticFunc(_SC("LocatePointCellEx"), &Areas_LocatePointCellEx)
        .StaticFunc(_SC("UnmanageAll"), &TerminateAreas)
    );
}

} // Namespace:: SqMod
