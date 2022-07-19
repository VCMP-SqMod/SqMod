#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <concurrentqueue.h>

// ------------------------------------------------------------------------------------------------
#include <queue>
#include <mutex>
#include <vector>
#include <atomic>
#include <thread>
#include <condition_variable>

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
     * Destructor.
    */
    virtual ~ThreadPoolItem() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    ThreadPoolItem & operator = (const ThreadPoolItem & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    ThreadPoolItem & operator = (ThreadPoolItem && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Provide a name to what type of task this is. Mainly for debugging purposes.
    */
    SQMOD_NODISCARD virtual const char * TypeName() noexcept { return "worker item"; }

    /* --------------------------------------------------------------------------------------------
     * Provide unique information that may help identify the task. Mainly for debugging purposes.
    */
    SQMOD_NODISCARD virtual const char * IdentifiableInfo() noexcept { return ""; }

    /* --------------------------------------------------------------------------------------------
     * Invoked in worker thread by the thread pool after obtaining the task from the queue.
     * Must return true to indicate that the task can be performed. False indicates failure.
    */
    SQMOD_NODISCARD virtual bool OnPrepare() { return true; }

    /* --------------------------------------------------------------------------------------------
     * Called in worker by the thread pool to performed by the associated tasks.
     * Will be called continuously while the returned value is true. While false means it finished.
    */
    SQMOD_NODISCARD virtual bool OnProcess() { return false; };

    /* --------------------------------------------------------------------------------------------
     * Invoked in main thread by the thread pool after the task was completed.
    */
    virtual void OnCompleted() { }

    /* --------------------------------------------------------------------------------------------
     * Called in worker by the thread pool to let the task know that it will be aborted.
     * Most likely due to a shutdown of the thread pool.
    */
    virtual void OnAborted(bool SQ_UNUSED_ARG(retry)) { }
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
    using Finished = moodycamel::ConcurrentQueue< Item >; // Finished items.

    // --------------------------------------------------------------------------------------------
    std::atomic_bool        m_Running; // Whether the threads are allowed to run.
    // --------------------------------------------------------------------------------------------
    std::mutex              m_Mutex;
    std::condition_variable m_CV;
    std::queue< Item >      m_Queue;
    // --------------------------------------------------------------------------------------------
    Finished                m_Finished; // Non-blocking concurrent queue of finished items.
    // --------------------------------------------------------------------------------------------
    Pool                    m_Threads; // Pool of worker threads.

private:

    /* --------------------------------------------------------------------------------------------
     * Internal function used to process tasks.
    */
    void WorkerProc();

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
     * Queue an item to be processed. Will take ownership of the given pointer!
    */
    void Enqueue(ThreadPoolItem * item)
    {
        Enqueue(Item{item});
    }

    /* --------------------------------------------------------------------------------------------
     * Queue an item to be processed. Will take ownership of the given pointer!
    */
    void Enqueue(Item && item)
    {
        // Only queue valid items
        if (!item || !m_Running) return;
        // Only queue if worker threads exist
        if (!m_Threads.empty())
        {
            // Acquire a lock on the mutex
            std::unique_lock< std::mutex > lock(m_Mutex);
            // Push the item in the queue
            m_Queue.push(std::forward< Item >(item));
            // Release the mutex before notifying
            lock.unlock();
            // Notify one thread that there's work
            m_CV.notify_one(); 
        }
        else
        {
            bool r;
            // Attempt preparation
            try {
                r = item->OnPrepare();
            } catch (const std::exception & e) {
                LogErr("Exception occured in %s preparation stage [%s] for [%s]", item->TypeName(), e.what(), item->IdentifiableInfo());
            }
            // Perform the task in-place
            if (r)
            {
                try {
                    r = item->OnProcess();
                } catch (const std::exception & e) {
                    LogErr("Exception occured in %s processing stage [%s] for [%s]", item->TypeName(), e.what(), item->IdentifiableInfo());
                }
                if (r)
                {
                    try {
                        item->OnAborted(true); // Not accepted in single thread
                    } catch (const std::exception & e) {
                        LogErr("Exception occured in %s cancelation stage [%s] for [%s]", item->TypeName(), e.what(), item->IdentifiableInfo());
                    }
                }
            }
            // Task is completed in processing stage
            m_Finished.enqueue(std::forward< Item >(item));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of worker threads.
    */
    SQMOD_NODISCARD size_t GetThreadCount()
    {
        return m_Threads.size();
    }
};

} // Namespace:: SqMod
