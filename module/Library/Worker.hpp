#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"
#include "Base/VecMap.hpp"

// ------------------------------------------------------------------------------------------------
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <chrono>
#include <condition_variable>

// ------------------------------------------------------------------------------------------------
#include <concurrentqueue.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

using namespace moodycamel;

// ------------------------------------------------------------------------------------------------
struct Worker;

/* ------------------------------------------------------------------------------------------------
 * Used to transmit values between workers in a language agnostic way.
*/
struct Parameter
{
    enum
    {
        T_NULL=0, // Null/undefined type.
        T_INT, // Integer type.
        T_BOOL, // Boolean type.
        T_FLOAT, // Floating point type.
        T_STRING, // String type.
        T_ARRAY, // Array type.
        T_TABLE // Table type.
    };
    // --------------------------------------------------------------------------------------------
    using ArrayType = std::vector< Parameter >; // Parameter array.
    using TableType = VecMap< Parameter, Parameter >; // Parameter table.
    // --------------------------------------------------------------------------------------------
    uint32_t    mType; // Type of value stored in the parameter.
    uint32_t    mSize; // Container size. Mostly used for the string because there's space from padding.
    /* --------------------------------------------------------------------------------------------
     *
    */
    union {
        int64_t     mInt; // Parameter value represented as integer.
        uint64_t    mData; // Parameter value represented as raw bits.
        double      mFloat; // Parameter value represented as floating point.
        CStr        mString; // Parameter value represented as string pointer.
        ArrayType * mArray; // Parameter value represented as array pointer.
        TableType * mTable; // Parameter value represented as table pointer.
    };
    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    Parameter() noexcept
        : Parameter(nullptr)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Null constructor.
    */
    explicit Parameter(std::nullptr_t)
        : mType(T_NULL), mSize(0), mData(0ull)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Integer constructor.
    */
    explicit Parameter(int8_t v)
            : mType(T_INT), mSize(sizeof(v)), mInt(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Integer constructor.
    */
    explicit Parameter(uint8_t v)
            : mType(T_INT), mSize(sizeof(v)), mInt(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Integer constructor.
    */
    explicit Parameter(int16_t v)
            : mType(T_INT), mSize(sizeof(v)), mInt(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Integer constructor.
    */
    explicit Parameter(uint16_t v)
            : mType(T_INT), mSize(sizeof(v)), mInt(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Integer constructor.
    */
    explicit Parameter(int32_t v)
            : mType(T_INT), mSize(sizeof(v)), mInt(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Integer constructor.
    */
    explicit Parameter(uint32_t v)
            : mType(T_INT), mSize(sizeof(v)), mInt(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Integer constructor.
    */
    explicit Parameter(int64_t v)
            : mType(T_INT), mSize(sizeof(v)), mInt(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Integer constructor.
    */
    explicit Parameter(uint64_t v)
            : mType(T_INT), mSize(sizeof(v)), mInt(static_cast< int64_t >(v))
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Boolean constructor.
    */
    explicit Parameter(bool SQ_UNUSED_ARG(v)) //static analyzer shat the bed
            : mType(T_BOOL), mSize(1), mInt(v ? 1 : 0)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Floating point constructor.
    */
    explicit Parameter(float v)
            : mType(T_FLOAT), mSize(sizeof(v)), mFloat(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Floating point constructor.
    */
    explicit Parameter(double v)
            : mType(T_FLOAT), mSize(sizeof(v)), mFloat(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * String constructor.
    */
    explicit Parameter(CCStr str);
    /* --------------------------------------------------------------------------------------------
     * String constructor.
    */
    explicit Parameter(CCStr str, size_t len);
    /* --------------------------------------------------------------------------------------------
     * Array constructor.
    */
    explicit Parameter(ArrayType && v);
    /* --------------------------------------------------------------------------------------------
     * Array constructor.
    */
    explicit Parameter(const ArrayType & v);
    /* --------------------------------------------------------------------------------------------
     * Table constructor.
    */
    explicit Parameter(TableType && v);
    /* --------------------------------------------------------------------------------------------
     * Table constructor.
    */
    explicit Parameter(const TableType & v);
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Parameter(const Parameter & o);
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Parameter(Parameter && o);
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Parameter()
    {
        Reset();
    }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Parameter & operator = (const Parameter & o)
    {
        Assign(o);
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Parameter & operator = (Parameter && o)
    {
        Assign(std::forward< Parameter >(o));
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const Parameter & o) const noexcept;
    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const Parameter & o) const noexcept
    {
        return !(*this == o);
    }
    /* --------------------------------------------------------------------------------------------
     * Discard any current information and set to null.
    */
    void Reset()
    {
        Clear(nullptr);
    }
    /* --------------------------------------------------------------------------------------------
     * Swap parameter contents.
    */
    void Swap(Parameter & o) noexcept
    {
        std::swap(mType, o.mType);
        std::swap(mSize, o.mSize);
        std::swap(mData, o.mData);
    }
    /* --------------------------------------------------------------------------------------------
     * Assign a copy of another parameter.
    */
    void Assign(const Parameter & o);
    /* --------------------------------------------------------------------------------------------
     * Assign a ownership of another parameter.
    */
    void Assign(Parameter && o);
private:
    /* --------------------------------------------------------------------------------------------
     * Discard any and all information without performing any release of memory.
    */
    void Discard() noexcept
    {
        mType = T_NULL;
        mSize = 0;
        mData = 0ull;
    }
    /* --------------------------------------------------------------------------------------------
     * Clear/release any stored value and reset to default. Does not set to null.
    */
    void Clear();
    /* --------------------------------------------------------------------------------------------
     * Clear/release any stored value and reset to default. Specialization which sets to null.
    */
    void Clear(std::nullptr_t)
    {
        Clear(); // Do a regular clear first
        Discard(); // Now we can forget about it
    }
};

/* ------------------------------------------------------------------------------------------------
 * Used to represent a job that a worker must do, as well as a reply from the worker with the result.
*/
struct Job
{
    using Parameters = Parameter::ArrayType;
    /* --------------------------------------------------------------------------------------------
     * Describe the typeof job a worker must do.
    */
    enum class Type : uint8_t
    {
        Null=0, // Null/non existent job.
        Stop, // Inform the worker to stop.
        Eval, // Inform the worker to evaluate some code.
        Exec // Inform the worker to execute a file.
    };
    // --------------------------------------------------------------------------------------------
    Type        mType; // Job type.
    String      mTarget; // Where to perform the job.
    String      mPayload; // Where to perform the job.
    Function    mCallback; // Function to call once completed.
    Parameter   mResponse; // The value given by the worker.
    Parameters  mParameters; // Job parameters.
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Job()
        : mType(Type::Null), mTarget(), mPayload(), mCallback(), mResponse(), mParameters()
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Job(Function && callback)
        : mType(Type::Null), mTarget(), mPayload()
        , mCallback(std::forward< Function >(callback)), mResponse(), mParameters()
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Job(const Job & o) = default;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Job(Job && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Job() = default;
    /* --------------------------------------------------------------------------------------------
     * Copy assignment.
    */
    Job & operator = (const Job & o) = default;
    /* --------------------------------------------------------------------------------------------
     * Move assignment.
    */
    Job & operator = (Job && o) = default;
};

/* ------------------------------------------------------------------------------------------------
 * Thread.
*/
struct Thread
{
    // --------------------------------------------------------------------------------------------
    Worker *    mWorker; // Worker pointer.
    LightObj    mObject; // Worker object.
    std::thread mThread; // Worker thread.
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Thread()
        : mWorker(nullptr), mObject(), mThread()
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Thread(Worker * worker);
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    Thread(const Thread & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    Thread(Thread && o) noexcept;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Thread()
    {
        End();
    }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment (disabled).
    */
    Thread & operator = (const Thread & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment (disabled).
    */
    Thread & operator = (Thread && o) = delete;
    /* --------------------------------------------------------------------------------------------
     *
    */
    void End()
    {

    }
};
/* ------------------------------------------------------------------------------------------------
*
*/
struct Worker
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Worker(SQInteger stack, String && str, size_t h);
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    Worker(const Worker & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    Worker(Worker && o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Worker();
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    Worker & operator = (const Worker & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    Worker & operator = (Worker && o) = delete;
    /* --------------------------------------------------------------------------------------------
     *
    */
    static void Terminate();
    /* --------------------------------------------------------------------------------------------
     *
    */
    static void Process(size_t jobs);
    /* --------------------------------------------------------------------------------------------
     *
    */
    static LightObj Create(SQInteger stack, StackStrF & str);
    /* --------------------------------------------------------------------------------------------
     * Invoke the actual process loop only if the worker was not requested to stop.
    */
    void Start();
    /* --------------------------------------------------------------------------------------------
     * Stop the worker but do not remove from container.
    */
    void Kill()
    {
        std::lock_guard< std::mutex > lg(m_Mutex);
    }
    /* --------------------------------------------------------------------------------------------
     * Stop the worker and remove from container.
    */
    void Stop()
    {
        m_Running.clear(); // Stop running as soon as you finish what you're doing
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the worker name.
    */
    const String & GetName() const
    {
        return m_Name;
    }
    /* --------------------------------------------------------------------------------------------
     *
    */
    void Evaluate(StackStrF & str, Function & callback)
    {
        // Is there something to evaluate?
        if (str.mLen <= 0)
        {
            STHROWF("Nothing to evaluate");
        }
        // Create the job
        std::unique_ptr< Job > job(new Job(std::move(callback)));
        // Define type
        job->mType = Job::Type::Eval;
        // Assign data
        job->mPayload.assign(str.mPtr, static_cast< size_t >(str.mLen));
        // Submit job
        m_PendingJobs.enqueue(std::move(job));
    }
    /* --------------------------------------------------------------------------------------------
     *
    */
    void EvaluateTarget(StackStrF & str)
    {

    }
    /* --------------------------------------------------------------------------------------------
     *
    */
    void Execute(StackStrF & str)
    {

    }
    /* --------------------------------------------------------------------------------------------
     *
    */
    void ExecuteTarget(StackStrF & str)
    {

    }
private:
    /* --------------------------------------------------------------------------------------------
     * Worker thread container.
    */
    using Container = VecMap< size_t, std::unique_ptr< Thread > >;
    /* --------------------------------------------------------------------------------------------
     * List of active worker threads.
    */
    static Container sm_Workers;
    /* --------------------------------------------------------------------------------------------
     *
    */
    using Jobs = ConcurrentQueue< std::unique_ptr< Job > >;
    /* --------------------------------------------------------------------------------------------
     * Job queue.
    */
    Jobs                        m_PendingJobs;
    /* --------------------------------------------------------------------------------------------
     * Job queue.
    */
    Jobs                        m_FinishedJobs;
    /* --------------------------------------------------------------------------------------------
     * Loop state.
    */
    std::atomic_flag            m_Running;
    /* --------------------------------------------------------------------------------------------
     * Script state.
    */
    HSQUIRRELVM                 m_VM;
    /* --------------------------------------------------------------------------------------------
     * Instance mutex.
    */
    std::mutex                  m_Mutex;
    /* --------------------------------------------------------------------------------------------
     * Cached name hash.
    */
    size_t                      m_Hash;
    /* --------------------------------------------------------------------------------------------
     * Worker name string.
    */
    String                      m_Name;
    /* --------------------------------------------------------------------------------------------
     * Cached name hash.
    */
    SQInteger                   m_StackSize;
    /* --------------------------------------------------------------------------------------------
     * Take jobs from the queue and perform them.
    */
    void Work();
    /* --------------------------------------------------------------------------------------------
     * Script output handlers.
    */
    static void PrintFunc(HSQUIRRELVM vm, CSStr msg, ...);
    static void ErrorFunc(HSQUIRRELVM vm, CSStr msg, ...);
};

} // Namespace:: SqMod
