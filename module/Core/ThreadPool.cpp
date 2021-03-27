// ------------------------------------------------------------------------------------------------
#include "Core/ThreadPool.hpp"

// ------------------------------------------------------------------------------------------------
#include <chrono>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
ThreadPool ThreadPool::s_Inst;

// ------------------------------------------------------------------------------------------------
ThreadPool::ThreadPool() noexcept
    : m_Running(false)
    , m_Pending()
    , m_Finished()
    , m_Threads()
{
    m_Threads.reserve(MAX_WORKER_THREADS + 1); // Reserve thread memory in advance
}

// ------------------------------------------------------------------------------------------------
ThreadPool::~ThreadPool()
{
    // Desperate attempt to gracefully shutdown
    for (auto & t : m_Threads)
    {
        if (t.joinable())
        {
            t.join(); // Will block until work is finished!
        }
    }
}

// ------------------------------------------------------------------------------------------------
bool ThreadPool::Initialize(uint32_t count)
{
    // Are there any threads requested?
    // Are we already running or have threads?
    if (!count || m_Running || !m_Threads.empty())
    {
        return true; // Nothing to do!
    }
    else if (count > MAX_WORKER_THREADS)
    {
        count = MAX_WORKER_THREADS; // Hard coded worker limit
    }
    // See if any of the threads are active
    for (auto & t : m_Threads)
    {
        if (t.joinable())
        {
            return false; // Something is not right!
        }
    }
    // Make sure the threads don't stop after creation
    m_Running = true;
    // Create the specified amount of worker threads
    for (uint32_t i = 0; i < count; ++i)
    {
        // Pass the container index to the worker thread so it knows to find itself
        m_Threads.emplace_back(&ThreadPool::WorkerProc, this, i);
    }
    // Thread pool initialized
    return m_Running;
}

// ------------------------------------------------------------------------------------------------
void ThreadPool::Terminate(bool SQ_UNUSED_ARG(shutdown))
{
    // Are there threads running? 
    if (m_Threads.empty() || !m_Running)
    {
        return; // Don't bother!
    }
    // Tell the threads to stop
    m_Running = false;
    // Enqueue dummy items to wake the threads and allow them to stop
    for (size_t n = 0; n < m_Threads.size(); ++n)
    {
        m_Pending.enqueue(Item());
    }
    // Attempt to join the threads
    for (auto & t : m_Threads)
    {
        if (t.joinable())
        {
            t.join(); // Will block until work is finished!
        }
    }
    // Retrieve each item individually and process it
    for (Item item; m_Finished.try_dequeue(item);)
    {
        // Is the item valid?
        if (item)
        {
            item->OnCompleted(); // Allow the item to finish itself
        }
        // Item processed
        item.reset();
    }
}

// ------------------------------------------------------------------------------------------------
void ThreadPool::Process()
{
    // Process only what's currently in the queue
    const size_t count = m_Finished.size_approx();
    // Retrieve each item individually and process it
    for (size_t n = 0; n <= count; ++n)
    {
        Item item;
        // Try to get an item from the queue
        if (m_Finished.try_dequeue(item))
        {
            // Is the item valid?
            if (item)
            {
                item->OnCompleted(); // Allow the item to finish itself
            }
        }
    }
}

// ------------------------------------------------------------------------------------------------
void ThreadPool::WorkerProc(uint32_t SQ_UNUSED_ARG(idx))
{
    // Whether this item wants to try again
    bool retry = false;
    // Pointer to the dequeued item
    Item item;
    // Constantly process items from the queue
    while (true)
    {
        // Do we have to stop?
        if (!m_Running)
        {
            // Is there an item that requested to try again?
            if (item)
            {
                item->OnAborted(true); // NOLINT(bugprone-use-after-move) There's an `if` condition above idiot!
            }
            // Exit the loop
            break;
        }
        // Attempt to get an item from the queue
        if (!retry)
        {
            m_Pending.wait_dequeue(item);
        }
        // Do we have to stop?
        if (!m_Running)
        {
            // Is there an item to be processed?
            if (item)
            {
                item->OnAborted(false); // It should mark itself as aborted somehow!
            }
            // Exit the loop
            break;
        }
        // Perform the task
        if (item->OnPrepare())
        {
            retry = item->OnProcess();
        }
        // The task was performed
        if (!retry)
        {
            m_Finished.enqueue(std::move(item));
        }
    }
    // Did we take an item from the queue?
    if (item)
    {
        m_Finished.enqueue(std::move(item)); // Return it, even if not completed
    }
}

} // Namespace:: SqMod
