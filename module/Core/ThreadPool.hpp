#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <concurrentqueue.h>
#include <blockingconcurrentqueue.h>

// ------------------------------------------------------------------------------------------------
#include <queue>
#include <mutex>
#include <vector>
#include <atomic>
#include <thread>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static constexpr uint32_t MAX_WORKER_THREADS = 32; // Hard coded worker threads limit.

/* ------------------------------------------------------------------------------------------------
 * Item that can be given to the thread pool to process data in a separate thread.
*/
struct ThreadPoolItem
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ThreadPoolItem() noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    ThreadPoolItem(const ThreadPoolItem & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    ThreadPoolItem(ThreadPoolItem && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    ThreadPoolItem & operator = (const ThreadPoolItem & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    ThreadPoolItem & operator = (ThreadPoolItem && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Invoked in worker thread by the thread pool after obtaining the task from the queue.
     * Must return true to indicate that the task can be performed. False indicates failure.
    */
    virtual bool OnPrepare() { return true; }

    /* --------------------------------------------------------------------------------------------
     * Called in worker by the thread pool to performed by the associated tasks.
     * Will be called continuously while the returned value is true. While false means it finished.
    */
    virtual bool OnProcess() = 0;

    /* --------------------------------------------------------------------------------------------
     * Invoked in main thread by the thread pool after the task was completed.
     * If true is returned, the task is added back to the queue to be processed again.
    */
    virtual bool OnCompleted() { return false; }

    /* --------------------------------------------------------------------------------------------
     * Called in worker by the thread pool to let the task know that it will be aborted.
     * Most likely due to a shutdown of the thread pool.
    */
    virtual void OnAborted(bool SQ_UNUSED_ARG(retry)) { }
};

/* ------------------------------------------------------------------------------------------------
 * Common implementation of a basic item.
*/
struct BasicThreadPoolItem
{
    // --------------------------------------------------------------------------------------------
    LightObj    mCallback{};

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    BasicThreadPoolItem() noexcept = default;
};

/* ------------------------------------------------------------------------------------------------
 * Internal thread pool used to reduce stuttering from the plug-in whenever necessary and/or possible.
*/
class ThreadPool
{
private:

    // --------------------------------------------------------------------------------------------
    static ThreadPool s_Inst; // ThreadPool instance.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ThreadPool() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ThreadPool();

private:

    // --------------------------------------------------------------------------------------------
    using Pool = std::vector< std::thread >; // Worker container.
    using Item = std::unique_ptr< ThreadPoolItem >; // Owning pointer of an item.
    // --------------------------------------------------------------------------------------------
    using Pending = moodycamel::BlockingConcurrentQueue< Item >; // Pending items.
    using Finished = moodycamel::ConcurrentQueue< Item >; // Finished items.

    // --------------------------------------------------------------------------------------------
    std::atomic_bool    m_Running; // Whether the threads are allowed to run.
    // --------------------------------------------------------------------------------------------
    Pending             m_Pending; // Blocking concurrent queue of pending items.
    Finished            m_Finished; // Non-blocking concurrent queue of finished items.
    // --------------------------------------------------------------------------------------------
    Pool                m_Threads; // Pool of worker threads.

private:

    /* --------------------------------------------------------------------------------------------
     * Internal function used to process tasks.
    */
    void WorkerProc(uint32_t idx);

public:

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    ThreadPool(const ThreadPool & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    ThreadPool(ThreadPool && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    ThreadPool & operator = (const ThreadPool & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    ThreadPool & operator = (ThreadPool && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the thread pool instance.
    */
    static ThreadPool & Get()
    {
        return s_Inst;
    }

    /* --------------------------------------------------------------------------------------------
     * Initialize the thread pool.
    */
    bool Initialize(uint32_t count);

    /* --------------------------------------------------------------------------------------------
     * Terminate the thread pool.
    */
    void Terminate(bool shutdown = false);

    /* --------------------------------------------------------------------------------------------
     * Process finished items.
    */
    void Process();

    /* --------------------------------------------------------------------------------------------
     * Queue an item to be processed.
    */
    void Enqueue(ThreadPoolItem * item)
    {
        // Only queue valid items
        if (!item || !m_Running) return;
        // Only queue if worker threads exist
        if (!m_Threads.empty())
        {
            m_Pending.enqueue(Item(item));
        }
        else
        {
            // Perform the task in-place
            if (item->OnPrepare())
            {
                if (item->OnProcess())
                {
                    item->OnAborted(true); // Not accepted in single thread
                }
            }
            // Item was finished in main thread
            item->OnCompleted();
        }
    }
};

} // Namespace:: SqMod
