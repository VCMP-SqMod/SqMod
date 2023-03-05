#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Helper utility used to provide reusable unique IDs of signed integer type.
 * It is not thread-safe since the script runs in single-threaded mode.
*/
struct SqIdPool
{
    using Type = SQInteger; // Type that is used to represent an ID.
    using Pool = std::vector< Type >; // Container for both used and unused IDs.

    // --------------------------------------------------------------------------------------------
    // Pool of available IDs.
    Pool mPool{};
    // Pool of currently used IDs.
    Pool mUsed{};
    // The ID that will be generated next time the pool is empty.
    Type mNext{0};
    // How much to increment with each ID.
    Type mStep{1};
    // Where to start generating IDs.
    Type mStart{0};

    /* --------------------------------------------------------------------------------------------
     * Base constructors.
    */
    SqIdPool() noexcept = default;
    SqIdPool(Type start) noexcept
        : SqIdPool(start, 1)
    {
    }
    SqIdPool(Type start, Type step) noexcept
        : mPool(), mUsed(), mNext(start), mStep(step), mStart(start)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy/Move constructors (disabled).
    */
    SqIdPool(const SqIdPool &) noexcept = delete;
    SqIdPool(SqIdPool &&) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy/Move assignment operators (disabled).
    */
    SqIdPool & operator = (const SqIdPool &) noexcept = delete;
    SqIdPool & operator = (SqIdPool &&) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Discard all current IDs (free and used) and reset the start to the specified start.
     * This invalidates all IDs that are currently left in use.
    */
    void Reset()
    {
        mNext = mStart;
        mPool.clear();
        mUsed.clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Acquire a unique ID from the pool.
    */
    SQMOD_NODISCARD Type Acquire()
    {
        Type id = mNext;
        // Do we have some reusable IDs?
        if (mPool.empty())
        {
            mNext += mStep; // Create a new one and update the next one
        }
        else
        {
            id = mPool.back(); // Get one from the back of the pool
            mPool.pop_back(); // Remove it from the free pool
        }
        // Store it in the list of active IDs
        mUsed.push_back(id);
        // Return this ID
        return id;
    }

    /* --------------------------------------------------------------------------------------------
     * Release a unique ID back to the pool.
    */
    bool Release(Type id)
    {
        // Find the specified ID into 
        for (Pool::size_type i = 0; i < mUsed.size(); ++i)
        {
            // Is this the ID we're looking for?
            if (mUsed[i] == id)
            {
                // Swap the element with the last one
                std::swap(mUsed[i], mUsed.back());
                // Remove the last element
                mUsed.pop_back();
                // Make this ID available, again
                mPool.push_back(id);
                // We actually found this ID
                return true;
            }
        }
        // This ID does not belong to this pool
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Check if the pool has the specified ID currently in use.
    */
    SQMOD_NODISCARD bool Using(Type id)
    {
        return std::find(mUsed.begin(), mUsed.end(), id) != mUsed.end();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of IDs that are currently available in the free pool.
    */
    SQMOD_NODISCARD SQInteger FreeCount() const
    {
        return mPool.size();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of IDs that are currently in use.
    */
    SQMOD_NODISCARD SQInteger UsedCount() const
    {
        return mUsed.size();
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all used IDs through a functor.
    */
    void EachUsed(Function & fn) const
    {
        for (const auto & id : mUsed)
        {
            fn.Execute(id);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all used IDs through a functor until stopped (i.e false is returned).
    */
    void WhileUsed(Function & fn) const
    {
        for (const auto & id : mUsed)
        {
            auto ret = fn.Eval(id);
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.template Cast< bool >())
            {
                break;
            }
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all free IDs through a functor.
    */
    void EachFree(Function & fn) const
    {
        for (const auto & id : mPool)
        {
            fn.Execute(id);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all free IDs through a functor until stopped (i.e false is returned).
    */
    void WhileFree(Function & fn) const
    {
        for (const auto & id : mPool)
        {
            auto ret = fn.Eval(id);
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.template Cast< bool >())
            {
                break;
            }
        }
    }
};

} // Namespace:: SqMod
