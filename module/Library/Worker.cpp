// ------------------------------------------------------------------------------------------------
#include "Library/Worker.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(SqWorker, _SC("SqWorker"))

// ------------------------------------------------------------------------------------------------
Worker::Container Worker::sm_Workers{};

// ------------------------------------------------------------------------------------------------
Parameter::Parameter(CCStr str)
	: Parameter()
{
    // Make sure there's a string
    if (str)
    {
        mSize = static_cast< uint32_t >(std::strlen(str));
        mString = new SQChar[mSize+1];
        std::strcpy(mString, str);
    }
    // Even an empty string should still be marked as a string
    mType = T_STRING;
}
// ------------------------------------------------------------------------------------------------
Parameter::Parameter(CCStr str, size_t len)
	: Parameter()
{
    // Make sure there's a string
    if (str && len)
    {
        mSize = ConvTo< uint32_t >::From(len);
        mString = new SQChar[mSize+1];
        std::strncpy(mString, str, mSize);
        mString[mSize] = '\0'; // Null terminator
    }
    // Even an empty string should still be marked as a string
    mType = T_STRING;
}
// ------------------------------------------------------------------------------------------------
Parameter::Parameter(ArrayType && v)
    : mType(T_ARRAY), mSize(static_cast< uint32_t >(v.size()))
    , mArray(new ArrayType(std::forward< ArrayType >(v)))
{
}
// ------------------------------------------------------------------------------------------------
Parameter::Parameter(const ArrayType & v)
    : mType(T_ARRAY), mSize(static_cast< uint32_t >(v.size()))
    , mArray(new ArrayType(v))
{
}
// ------------------------------------------------------------------------------------------------
Parameter::Parameter(TableType && v)
    : mType(T_ARRAY), mSize(static_cast< uint32_t >(v.size()))
    , mTable(new TableType(std::forward< TableType >(v)))
{
}
// ------------------------------------------------------------------------------------------------
Parameter::Parameter(const TableType & v)
    : mType(T_ARRAY), mSize(static_cast< uint32_t >(v.size()))
    , mTable(new TableType(v))
{
}
// ------------------------------------------------------------------------------------------------
Parameter::Parameter(const Parameter & o)
    : mType(o.mType), mSize(o.mSize), mData(o.mData)
{
    // Identify the type to be copied
    switch (mType)
    {
        // Fundamental types can be copied bit-wise (which we did)
        case T_NULL:
        case T_INT:
        case T_BOOL:
        case T_FLOAT: break;
        case T_STRING:
            if (mSize)
            {
                mString = new SQChar[mSize];
                std::strncpy(mString, o.mString, mSize);
                mString[mSize] = '\0'; // Null terminator
            }
            else
            {
                mString = nullptr; // Empty string?
            }
            break;
        case T_ARRAY:
            mArray = o.mArray ? new ArrayType(*o.mArray) : nullptr;
            break;
        case T_TABLE:
            mTable = o.mTable ? new TableType(*o.mTable) : nullptr;
            break;
            // How did we get here?
        default: break;
    }
}
// ------------------------------------------------------------------------------------------------
Parameter::Parameter(Parameter && o)
    : mType(o.mType), mSize(o.mSize), mData(o.mData)
{
    o.Discard(); // Take ownership
}
// ------------------------------------------------------------------------------------------------
bool Parameter::operator == (const Parameter & o) const noexcept
{
    // If they're not the same type then there's no point in comparing
    if (mType != o.mType)
    {
        return false;
    }
    // Identify which type to compare
    switch (mType)
    {
        // Null is same regardless
        case T_NULL: return true;
        // Boolean is stored as integer
        case T_INT:
        case T_BOOL: return (mInt == o.mInt);
        // Take into account precision errors
        case T_FLOAT: return EpsEq(mFloat, o.mFloat);
        case T_STRING:
            // Only perform a comparison if there's actually a string to compare
            if (mSize && mSize == o.mSize)
            {
                return std::strncmp(mString, o.mString, mSize) == 0;
            }
            else
            {
                return false; // If they're not the same size then they can't be the same
            }
        // For table or arrays we only test if they're the same rather then each value individually
        case T_ARRAY: return (mArray == o.mArray);
        case T_TABLE: return (mTable == o.mTable);
        // How did we get here?
        default: return false;
    }
}
// ------------------------------------------------------------------------------------------------
void Parameter::Assign(const Parameter & o)
{
    // Avoid self assignment
    if (this == &o) return;
    /* We could probably optimize this by reusing current container memory.
     * But chances are we would complicate code for the simpler case.
     * And the simpler case is likely to be the more common scenario.
    */
    // Discard current information
    Clear();
    // The size and type are copied bit-wise
    mType = o.mType;
    mSize = o.mSize;
    // Identify the type to be copied
    switch (mType)
    {
        // Fundamental types can be copied bit-wise
        case T_NULL:
        case T_INT:
        case T_BOOL:
        case T_FLOAT:
            mData = o.mData;
        break;
        // Strings require memory to be allocated
        case T_STRING:
            if (mSize)
            {
                mString = new SQChar[mSize];
                std::strncpy(mString, o.mString, mSize);
                mString[mSize] = '\0'; // Null terminator
            }
            else
            {
                mString = nullptr; // Empty string?
            }
        break;
        case T_ARRAY:
            mArray = o.mArray ? new ArrayType(*o.mArray) : nullptr;
        break;
        case T_TABLE:
            mTable = o.mTable ? new TableType(*o.mTable) : nullptr;
        break;
            // How did we get here?
        default: break;
    }
}
// ------------------------------------------------------------------------------------------------
void Parameter::Assign(Parameter && o)
{
    // Avoid self assignment
    if (this == &o) return;
    // Discard current information
    Clear();
    // We don't care about the type since we take ownership
    mType = o.mType;
    mSize = o.mSize;
    mData = o.mData;
    // Take ownership
    o.Discard();
}
// ------------------------------------------------------------------------------------------------
void Parameter::Clear()
{
    switch (mType)
    {
        case T_STRING: delete[] mString; break;
        case T_ARRAY: delete mArray; break;
        case T_TABLE: delete mTable; break;
        default: break;
    }
}

// ------------------------------------------------------------------------------------------------
Thread::Thread( Worker * worker)
    : mWorker(worker)
    , mObject(worker)
    , mThread(&Worker::Start, worker)
{
}
// ------------------------------------------------------------------------------------------------
Thread::Thread(Thread && o) noexcept
    : mWorker(o.mWorker)
    , mObject(std::forward< LightObj >(o.mObject))
    , mThread(std::forward< std::thread >(o.mThread))
{
    o.mWorker = nullptr;
}
// ------------------------------------------------------------------------------------------------
Worker::Worker(SQInteger stack, String && str, size_t h)
    : m_PendingJobs(4096), m_FinishedJobs(4096)
    , m_Running()
    , m_VM(nullptr)
    , m_Mutex()
    , m_Hash(h)
    , m_Name(std::forward< String >(str))
    , m_StackSize(stack)
{
}
// ------------------------------------------------------------------------------------------------
Worker::~Worker()
{
    // Instruct the thread to stop whenever possible
    Stop();
    // Locate our self in the list
    std::unique_ptr< Thread > & t = sm_Workers[m_Hash];
    // Wait for the thread to finish
    if (t->mThread.joinable())
    {
        t->mThread.join();
    }
    // Remove ourselves from the list
    sm_Workers.erase(m_Hash);
}

// ------------------------------------------------------------------------------------------------
void Worker::Terminate()
{
    // Attempt to stop workers
    for (auto & t : sm_Workers)
    {
        // Tell the thread to stop as soon as it can
        t.second->mWorker->Stop();
        // Wait for it to stop
        if (t.second->mThread.joinable())
        {
            t.second->mThread.join();
        }
    }
    // Simply get rid of them
    sm_Workers.clear();
}
// ------------------------------------------------------------------------------------------------
void Worker::Process(size_t jobs)
{
    std::vector< Worker * > workers;
    workers.reserve(sm_Workers.size());
    for (auto & t : sm_Workers)
    {
        workers.push_back(t.second->mWorker);
    }
    for (auto & t : workers)
    {
        for (size_t n = 0; n < jobs; ++n)
        {
            std::unique_ptr< Job > job;
            // Try to get a job from the queue
            if (!t->m_FinishedJobs.try_dequeue(job))
            {
                break; // No jobs
            }
            // Does it have a callback?
            if (!job->mCallback.IsNull())
            {
                job->mCallback.Execute();
            }
        }
    }
}
// ------------------------------------------------------------------------------------------------
LightObj Worker::Create(SQInteger stack, StackStrF & str)
{
    HSQUIRRELVM vm = SqVM();
	// Make sure there's a name
	if (str.mLen <= 0)
	{
		STHROWF("Invalid or empty worker name");
	}
	// Extract the worker name
    String name(str.mPtr, static_cast< size_t >(str.mLen));
    // Create the name hash
    size_t name_hash = std::hash< String >{}(name);
    // Make sure this worker doesn't exist
    if (sm_Workers.exists(name_hash))
    {
        STHROWF("Worker already exists");
    }
    // Attempt to create a routine instance
    DeleteGuard< Worker > dg(new Worker(stack, std::move(name), name_hash));
    ClassType< Worker >::PushInstance(vm, dg.Get());
    Worker * worker = dg.Grab();
    // Create the worker thread
    std::unique_ptr< Thread > & th = sm_Workers.emplace_back(name_hash, new Thread{worker});
    // Return the worker object
    return th->mObject;
}
// ------------------------------------------------------------------------------------------------
void Worker::Start()
{
    // Initialize
    {
        // Acquire exclusive access to this instance
        std::lock_guard< std::mutex > lg(m_Mutex);
        // This should never be the case but why not
        if (m_VM)
        {
            STHROWF("Worker was already started.");
        }
        // Create the JS state
        m_VM = sq_open(m_StackSize);
        // Tell the VM to use these functions to output user on error messages
        sq_setprintfunc(m_VM, PrintFunc, ErrorFunc);
        // This is now running
        m_Running.test_and_set();
    }
    // Process
    while (m_Running.test_and_set())
    {
        // Acquire exclusive access to this instance
        std::lock_guard< std::mutex > lg(m_Mutex);
        // Do the actual processing
        Work();
    }
    // Cleanup
    {
        // Acquire exclusive access to this instance
        std::lock_guard< std::mutex > lg(m_Mutex);
        // We're out of the process loop
        sq_close(m_VM);
    }
}
// ------------------------------------------------------------------------------------------------
void Worker::Work()
{
    std::unique_ptr< Job > job;
    // Try to get a job from the queue
    if (!m_PendingJobs.try_dequeue(job))
    {
        using namespace std::chrono_literals;
        // Do not hammer the CPU if there are no jobs
        std::this_thread::sleep_for(50ms);
        // Try again
        return;
    }
    // Identify the job type
    switch (job->mType)
    {
        // This type of job demands no work
        case Job::Type::Null: break;
        case Job::Type::Stop:

        break;
        case Job::Type::Eval: {
            sq_compilebuffer(m_VM, job->mPayload.data(), job->mPayload.size(), _SC("eval"), SQTrue);
            SQInteger top = sq_gettop(m_VM);
            sq_pushroottable(m_VM);
            sq_call(m_VM, 1, false, true);
            sq_settop(m_VM, top);
        } break;
        case Job::Type::Exec:

        break;
        // We're not qualified for this job
        default: break;
    }
    // This job was finished
    m_FinishedJobs.enqueue(std::move(job));
}
// ------------------------------------------------------------------------------------------------
void Worker::PrintFunc(HSQUIRRELVM /*vm*/, CSStr msg, ...)
{
    // Initialize the variable argument list
    va_list args;
    va_start(args, msg);
    // Forward the message to the logger
    std::vprintf(msg, args);
    std::puts("");
    // Finalize the variable argument list
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void Worker::ErrorFunc(HSQUIRRELVM /*vm*/, CSStr msg, ...)
{
    // Initialize the variable argument list
    va_list args;
    va_start(args, msg);
    // Tell the logger to display debugging information
    std::vprintf(msg, args);
    std::puts("");
    // Finalize the variable argument list
    va_end(args);
}
// ------------------------------------------------------------------------------------------------
void TerminateWorkers()
{
    Worker::Terminate();
}
// ================================================================================================
void Register_Worker(HSQUIRRELVM vm)
{
    Table thns(vm);

    thns.Bind(_SC("Worker"),
        Class< Worker, NoConstructor< Worker > >(vm, SqWorker::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqWorker::Fn)
        // Properties
        .Prop(_SC("Name"), &Worker::GetName)
        // Core Methods
        .CbFunc(_SC("Evaluate"), &Worker::Evaluate)
        // Static Member Methods
    );

    thns.FmtFunc(_SC("Process"), &Worker::Process);
    thns.FmtFunc(_SC("Create"), &Worker::Create);

    RootTable(vm).Bind(_SC("SqThread"), thns);
}

} // Namespace:: SqMod
