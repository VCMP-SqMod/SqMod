#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"
#include "Base/Circle.hpp"
#include "Base/Vector2.hpp"
#include "Base/Vector4.hpp"
#include "Base/Vector2i.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>
#include <utility>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Various information associated with an area cell.
*/
struct AreaCell
{
    // --------------------------------------------------------------------------------------------
    typedef std::pair< Area *, LightObj > AreaPair; // A reference to an area object.
    typedef std::vector< AreaPair > Areas; // A list of area objects.

    // --------------------------------------------------------------------------------------------
    float   mL, mB, mR, mT; // Left-Bottom, Right-Top components of the cell bounding box.
    // --------------------------------------------------------------------------------------------
    Areas   mAreas; // Areas that intersect with the cell.
    // --------------------------------------------------------------------------------------------
    int     mLocks; // The amount of locks on the cell.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    AreaCell()
        : mL(0), mB(0), mR(0), mT(0), mAreas(0), mLocks(0)
    {
        //...
    }
};

/* ------------------------------------------------------------------------------------------------
 * Area implementation used to store area points.
*/
struct Area
{
    typedef std::vector< Vector2 > Points;
    typedef std::vector< AreaCell * > Cells;
    // --------------------------------------------------------------------------------------------
    static constexpr float DEF_L = std::numeric_limits< float >::infinity();
    static constexpr float DEF_B = std::numeric_limits< float >::infinity();
    static constexpr float DEF_R = -std::numeric_limits< float >::infinity();
    static constexpr float DEF_T = -std::numeric_limits< float >::infinity();
    // --------------------------------------------------------------------------------------------
    float       mL, mB, mR, mT; // Left-Bottom, Right-Top components of the bounding box.
    // --------------------------------------------------------------------------------------------
    Points      mPoints; // Collection of points that make up the area.
    // --------------------------------------------------------------------------------------------
    SQInteger   mID; // The user identifier given to this area.
    // --------------------------------------------------------------------------------------------
    Cells       mCells; // The cells covered by this area.
    // --------------------------------------------------------------------------------------------
    String      mName; // The user name given to this area.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Area()
        : mL(DEF_L), mB(DEF_B), mR(DEF_R), mT(DEF_T), mPoints(), mID(0), mCells(), mName()
    {
        //...
    }
    /* --------------------------------------------------------------------------------------------
     * Named constructor.
    */
    explicit Area(StackStrF & name)
        : Area(16, name)
    {
        //...
    }
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Area(SQInteger sz, StackStrF & name)
        : mL(DEF_L), mB(DEF_B), mR(DEF_R), mT(DEF_T), mPoints(), mID(0), mCells()
        , mName(name.mPtr, static_cast< size_t >(name.mLen <= 0 ? 0 : name.mLen))

    {
        // Should we reserve some space for points in advance?
        if (sz > 0)
        {
            mPoints.reserve(static_cast< size_t >(sz));
        }
    }
    /* --------------------------------------------------------------------------------------------
     * Vector2 constructor.
    */
    Area(const Vector2 & a, const Vector2 & b, const Vector2 & c)
        : Area(a.x, a.y, b.x, b.y, c.x, c.y, 16, StackStrF::Dummy())
    {
        //...
    }

    /* --------------------------------------------------------------------------------------------
     * Vector2 constructor with name.
    */
    Area(const Vector2 & a, const Vector2 & b, const Vector2 & c, StackStrF & name)
        : Area(a.x, a.y, b.x, b.y, c.x, c.y, 16, name)
    {
        //...
    }

    /* --------------------------------------------------------------------------------------------
     * Vector2 constructor with name and memory to reserve.
    */
    Area(const Vector2 & a, const Vector2 & b, const Vector2 & c, SQInteger sz, StackStrF & name)
        : Area(a.x, a.y, b.x, b.y, c.x, c.y, sz, name)
    {
        //...
    }

    /* --------------------------------------------------------------------------------------------
     * Extended constructor.
    */
    Area(float ax, float ay, float bx, float by, float cx, float cy)
        : Area(ax, ay, bx, by, cx, cy, 16, StackStrF::Dummy())
    {
        //...
    }

    /* --------------------------------------------------------------------------------------------
     * Extended constructor with name.
    */
    Area(float ax, float ay, float bx, float by, float cx, float cy, StackStrF & name)
        : Area(ax, ay, bx, by, cx, cy, 16, name)
    {
        //...
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Area(float ax, float ay, float bx, float by, float cx, float cy, SQInteger sz, StackStrF & name)
        : mL(DEF_L), mB(DEF_B), mR(DEF_R), mT(DEF_T), mPoints(), mID(0), mCells()
        , mName(name.mPtr, static_cast<size_t>(name.mLen <= 0 ? 0 : name.mLen))
    {
        // Should we reserve some space for points in advance?
        if (sz > 0)
        {
            mPoints.reserve(static_cast< size_t >(sz));
        }
        // Insert the given points
        AddPointEx(ax, ay);
        AddPointEx(bx, by);
        AddPointEx(cx, cy);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Area(const Area & o)
        : mL(o.mL), mB(o.mB), mR(o.mR), mT(o.mT), mPoints(o.mPoints), mID(o.mID), mCells(0), mName(o.mName)
    {
        //...
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    Area(Area && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Area & operator = (const Area & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Area & operator = (Area && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert this instance to a string.
    */
    const String & ToString() const
    {
        return mName;
    }

    /* --------------------------------------------------------------------------------------------
     * Checks if the area is locked from changes and throws an exception if it is.
    */
    void CheckLock()
    {
        // Are we connected to any cells?
        if (!mCells.empty())
        {
            STHROWF("The area cannot be modified while being managed");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of this area.
    */
    const String & GetName() const
    {
        return mName;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the name of this area.
    */
    void SetName(StackStrF & name)
    {
        if (name.mLen <= 0)
        {
            mName.clear();
        }
        else
        {
            mName.assign(name.mPtr, static_cast< size_t >(name.mLen));
        }
    }


    /* --------------------------------------------------------------------------------------------
     * Modify the name of this area. (allows chaining function calls)
    */
    Area & ApplyName(StackStrF & name)
    {
        SetName(name);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier of this area.
    */
    SQInteger GetID() const
    {
        return mID;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the identifier of this area.
    */
    void SetID(SQInteger id)
    {
        mID = id;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the identifier of this area. (allows chaining function calls)
    */
    Area & ApplyID(SQInteger id)
    {
        mID = id;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Check if the area is locked from changes
    */
    bool IsLocked() const
    {
        return mCells.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the center of this area.
    */
    Vector2 GetCenter() const
    {
        return {(mL * 0.5f) + (mR * 0.5f), (mB * 0.5f) + (mT * 0.5f)};
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the bounding box of this area.
    */
    Vector4 GetBoundingBox() const
    {
        return {mL, mB, mR, mT};
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the bounding box of this area.
    */
    void SetBoundingBox(const Vector4 & b)
    {
        CheckLock();
        // Apply the given bounding box
        mL = b.x;
        mB = b.y;
        mR = b.z;
        mT = b.w;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the bounding box of this area.
    */
    void SetBoundingBoxEx(float l, float b, float r, float t)
    {
        CheckLock();
        // Apply the given bounding box
        mL = l;
        mB = b;
        mR = r;
        mT = t;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the area has no points.
    */
    bool Empty() const
    {
        return mPoints.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of points in this area.
    */
    SQInteger Size() const
    {
        return ConvTo< SQInteger >::From(mPoints.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of points this area has allocated for.
    */
    SQInteger Capacity() const
    {
        return ConvTo< SQInteger >::From(mPoints.capacity());
    }

    /* --------------------------------------------------------------------------------------------
     * Clear all points in this area.
    */
    void Clear()
    {
        CheckLock();
        // Perform the requested action
        mPoints.clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Clear all points in this area.
    */
    void Reserve(SQInteger sz)
    {
        // Perform the requested action
        if (sz > 0)
        {
            mPoints.reserve(static_cast< size_t >(sz));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Add a 2D vector to the point list.
    */
    void AddPoint(const Vector2 & v)
    {
        CheckLock();
        // Perform the requested action
        mPoints.emplace_back(v);
        // Update the bounding box
        Expand(v.x, v.y);
    }

    /* --------------------------------------------------------------------------------------------
     * Add a point to the point list.
    */
    void AddPointEx(float x, float y)
    {
        CheckLock();
        // Perform the requested action
        mPoints.emplace_back(x, y);
        // Update the bounding box
        Expand(x, y);
    }

    /* --------------------------------------------------------------------------------------------
     * Add a 2D vector to the bounding box only. Not stored in the list.
    */
    void AddVirtualPoint(const Vector2 & v)
    {
        CheckLock();
        // Update the bounding box
        Expand(v.x, v.y);
    }

    /* --------------------------------------------------------------------------------------------
     * Add a point to the bounding box only. Not stored in the list.
    */
    void AddVirtualPointEx(float x, float y)
    {
        CheckLock();
        // Update the bounding box
        Expand(x, y);
    }

    /* --------------------------------------------------------------------------------------------
     * Add an array of points to the point list.
    */
    void AddArray(const Sqrat::Array & a);

    /* --------------------------------------------------------------------------------------------
     * Add a 2D circle to the point list.
    */
    void AddCircle(const Circle & c, SQInteger num_segments)
    {
        AddCircleEx(c.pos.x, c.pos.y, c.rad, num_segments);
    }

    /* --------------------------------------------------------------------------------------------
     * Add a 2D circle to the point list.
    */
    void AddCircleEx(SQFloat cx, SQFloat cy, SQFloat cr, SQInteger num_segments);

    /* --------------------------------------------------------------------------------------------
     * Test if a point is inside the bounding box and then the area.
    */
    bool Test(const Vector2 & v)
    {
        // Is the given point in this bounding box at least?
        if (mL <= v.x && mR >= v.x && mB <= v.y && mT >= v.y)
        {
            return mPoints.empty() ? true : IsInside(v.x, v.y);
        }
        // Not in this area
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Test if a point is inside the bounding box and then the area.
    */
    bool TestEx(float x, float y)
    {
        // Is the given point in this bounding box at least?
        if (mL <= x && mR >= x && mB <= y && mT >= y)
        {
            return mPoints.empty() ? true : IsInside(x, y);
        }
        // Not in this area
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Add this area to the manager to be scanned. MUST BE CALLED ONLY FROM SCRIPT!
    */
    bool Manage();

    /* --------------------------------------------------------------------------------------------
     * Remove this area from the manager to no longer be scanned.
    */
    bool Unmanage();

protected:

    /* --------------------------------------------------------------------------------------------
     * Test if a point is inside the area.
    */
    bool IsInside(float x, float y) const;

    /* --------------------------------------------------------------------------------------------
     * Expand the bounding box area to include the given point.
    */
    void Expand(float x, float y)
    {
        mL = std::fmin(mL, x);
        mB = std::fmin(mB, y);
        mR = std::fmax(mR, x);
        mT = std::fmax(mT, y);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Manager responsible for storing and partitioning areas.
*/
class AreaManager
{
    // --------------------------------------------------------------------------------------------
    static AreaManager s_Inst; // Manager instance.

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit AreaManager(size_t sz = 16) noexcept;

protected:

    /* --------------------------------------------------------------------------------------------
     * Helper used to make sure a cell is properly processed after leaving the scope.
    */
    struct CellGuard
    {
        AreaCell & mCell;
        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        explicit CellGuard(AreaCell & cell)
            : mCell(cell)
        {
            ++(cell.mLocks); // Place a lock on the cell to prevent iterator invalidation
        }
        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~CellGuard()
        {
            // Remove the lock from the cell so it can be processed
            --(mCell.mLocks);
            // Process requested actions during the lock
            AreaManager::Get().ProcQueue();
        }
    };

    // --------------------------------------------------------------------------------------------
    static constexpr int GRIDN = 16; // Number of horizontal and vertical number of cells.
    static constexpr int GRIDH = GRIDN/2; // Half of the grid horizontal and vertical size.
    static constexpr int CELLS = GRIDN*GRIDN; // Total number of cells in the grid.
    static constexpr int CELLH = CELLS/2; // Half total number of cells in the grid.
    static constexpr int CELLD = 256; // Area covered by a cell in the world.
    static constexpr int NOCELL = std::numeric_limits< int >::max(); // Inexistent cell index.

    /* --------------------------------------------------------------------------------------------
     * Helper used to queue a certain action if the cell is locked.
    */
    struct QueueElement
    {
        // ----------------------------------------------------------------------------------------
        AreaCell *  mCell; // The cell to be affected.
        Area *      mArea; // The area that made the request.
        LightObj    mObj; // Strong reference to the object.
        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        QueueElement(AreaCell & cell, Area & area)
            : mCell(&cell), mArea(&area), mObj()
        {
            //...
        }
        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        QueueElement(AreaCell & cell, Area & area, LightObj & obj)
            : mCell(&cell), mArea(&area), mObj(obj)
        {
            //...
        }

        /* --------------------------------------------------------------------------------------------
         * Copy constructor. (disabled)
        */
        QueueElement(const QueueElement & o) = delete;

        /* --------------------------------------------------------------------------------------------
         * Move constructor.
        */
        QueueElement(QueueElement && o) noexcept
            : mCell(o.mCell), mArea(o.mArea), mObj(std::move(o.mObj))
        {
            // Take ownership
            o.mCell = nullptr;
            o.mArea = nullptr;
        }

        /* --------------------------------------------------------------------------------------------
         * Copy assignment operator. (disabled)
        */
        QueueElement & operator = (const QueueElement & o) = delete;

        /* --------------------------------------------------------------------------------------------
         * Move assignment operator.
        */
        QueueElement & operator = (QueueElement && o) noexcept
        {
            // Avoid self assignment
            if (this != &o)
            {
                // Transfer values
                mCell = o.mCell;
                mArea = o.mArea;
                mObj = std::move(o.mObj);
                // Take ownership
                o.mCell = nullptr;
                o.mArea = nullptr;
            }

            return *this;
        }
    };
    // --------------------------------------------------------------------------------------------
    typedef std::vector< QueueElement > Queue; // Queued actions.
    typedef std::vector< Queue::iterator > ProcList; // Elements in the queue redy to process.

    /* --------------------------------------------------------------------------------------------
     * Attempt to insert an area into a cell or queue the action if not possible.
    */
    void Insert(AreaCell & c, Area & a, LightObj & obj);

    /* --------------------------------------------------------------------------------------------
     * Attempt to remove an area from a cell or queue the action if not possible.
    */
    void Remove(AreaCell & c, Area & a);

private:

    // --------------------------------------------------------------------------------------------
    Queue       m_Queue; // Actions currently queued.
    ProcList    m_ProcList; // Actions ready to be completed.
    // --------------------------------------------------------------------------------------------
    AreaCell    m_Grid[GRIDN][GRIDN]; // A grid of area lists.

public:

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    AreaManager(const AreaManager & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    AreaManager(AreaManager && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    AreaManager & operator = (const AreaManager & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    AreaManager & operator = (AreaManager && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the core instance.
    */
    static AreaManager & Get()
    {
        return s_Inst;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to process elements in the queue that can be completed.
    */
    void ProcQueue();

    /* --------------------------------------------------------------------------------------------
     * Clear all cell lists and release any script references.
    */
    void Clear();

    /* --------------------------------------------------------------------------------------------
     * Add an area to be managed.
    */
    void InsertArea(Area & a, LightObj & obj);

    /* --------------------------------------------------------------------------------------------
     * Add an area to be managed.
    */
    void RemoveArea(Area & a);

    /* --------------------------------------------------------------------------------------------
     * Clear all cell lists and release any script references.
    */
    static Vector2i LocateCell(float x, float y);

    /* --------------------------------------------------------------------------------------------
     * Test a point to see whether it intersects with any areas
    */
    template < typename F > void TestPoint(F && f, float x, float y)
    {
        // Transform the world coordinates into a cell coordinates
        const Vector2i cc(LocateCell(x, y));
        // Were these coordinates valid?
        if (cc.x == NOCELL)
        {
            return; // Not our problem
        }
        // Retrieve a reference to the identified cell
        AreaCell & c = m_Grid[cc.y][cc.x];
        // Is this cell empty?
        if (c.mAreas.empty())
        {
            return; // Nothing to test
        }
        // Guard the cell while processing
        const CellGuard cg(c);
        // Finally, begin processing the areas in this cell
        for (auto & a : c.mAreas)
        {
            if (a.first->TestEx(x, y))
            {
                f(a);
            }
        }
    }
};

} // Namespace:: SqMod
