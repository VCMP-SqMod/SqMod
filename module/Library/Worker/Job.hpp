#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"
#include "Library/Worker/Parameter.hpp"

// ------------------------------------------------------------------------------------------------
#include <memory>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Handle validation.
*/
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    #define SQMOD_VALIDATE(x)               (x).Validate(__FILE__, __LINE__)
#else
    #define SQMOD_VALIDATE(x)               (x).Validate()
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
struct Worker;

/* ------------------------------------------------------------------------------------------------
 * Used to represent a job that a worker must do, as well as a reply from the worker with the result.
*/
struct BaseJob
{
    // --------------------------------------------------------------------------------------------
    Function mCallback; // Function to call once completed.
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    BaseJob()
        : mCallback()
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    BaseJob(const BaseJob & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    BaseJob(BaseJob && o) noexcept = delete;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    virtual ~BaseJob() = default;
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    BaseJob & operator = (const BaseJob & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    BaseJob & operator = (BaseJob && o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Create a new job that will replace the specified job.
    */
    virtual BaseJob * New(BaseJob & job) = 0;
    /* --------------------------------------------------------------------------------------------
     * Invoked inside a worker to perform the job.
    */
    virtual bool Start(HSQUIRRELVM vm, Worker & worker) = 0;
    /* --------------------------------------------------------------------------------------------
     * Invoked inside a worker to perform the job.
    */
    virtual void Finish(HSQUIRRELVM vm, Worker & worker)
    {
        // Do we have a callback?
        if (!mCallback.IsNull())
        {
            mCallback.Execute();
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Tell the worker to evaluate a piece of code.
*/
struct EvaluateJob : public BaseJob
{
    // --------------------------------------------------------------------------------------------
    String mCode; // The code to evaluate.
    String mName; // How to refer to this code in errors.
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    EvaluateJob()
        : BaseJob(), mCode(), mName()
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    virtual ~EvaluateJob() override = default;
    /* --------------------------------------------------------------------------------------------
     * Create a new job that will replace the specified job.
    */
    BaseJob * New(BaseJob & job) override
    {
        return new EvaluateJob();
    }
    /* --------------------------------------------------------------------------------------------
     * 
    */
    bool Start(HSQUIRRELVM vm, Worker & worker) override
    {
        SQRESULT r = sq_compilebuffer(vm, mCode.data(), mCode.size(), mName.data(), SQTrue);
        // See if the code could be compiled
        if (SQ_FAILED(r))
        {
            return false; // Job failed
        }
        // Backup the stack top
        SQInteger top = sq_gettop(vm);
        // Push the root table as environment
        sq_pushroottable(vm);
        // Attempt to invoke the compiled code
        r = sq_call(vm, 1, SQFalse, SQTrue);
        // Restore the stack top
        sq_settop(vm, top);
        // See if the code could be evaluated
        if (SQ_FAILED(r))
        {
            return false; // Job failed
        }
        // Job completed
        return true;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Tell the worker to execute the code from a file.
*/
struct ExecuteJob : public BaseJob
{
    // --------------------------------------------------------------------------------------------
    String mFile; // The file to evaluate.
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ExecuteJob()
        : BaseJob()
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    virtual ~ExecuteJob() override = default;
    /* --------------------------------------------------------------------------------------------
     * Create a new job that will replace the specified job.
    */
    BaseJob * New(BaseJob & job) override
    {
        return new ExecuteJob();
    }
    /* --------------------------------------------------------------------------------------------
     * 
    */
    bool Start(HSQUIRRELVM vm, Worker & worker) override
    {
        return true;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Used internally to wrap a job and expose it to the script.
*/
struct JobWrapperBase
{
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    JobWrapperBase(const JobWrapperBase & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    JobWrapperBase(JobWrapperBase && o) noexcept = delete;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~JobWrapperBase()
    {
        // There should always be an instance
        delete m_Inst;
        m_Inst = nullptr;
    }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    JobWrapperBase & operator = (const JobWrapperBase & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    JobWrapperBase & operator = (JobWrapperBase && o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Retrieve a raw pointer to the managed instance.
    */
    BaseJob * Get() const
    {
        return m_Inst;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a managed pointer managed instance and yield ownership.
    */
    std::unique_ptr< BaseJob > Grab()
    {
        std::unique_ptr< BaseJob > ptr(m_Inst);
        m_Inst = ptr->New(*m_Inst);
        return ptr;
    }
    /* --------------------------------------------------------------------------------------------
     * 
    */
    void SetCallback(Function & cb)
    {
        m_Inst->mCallback = std::move(cb);
    }
    /* --------------------------------------------------------------------------------------------
     * 
    */
    const Function & GetCallback() const
    {
        return m_Inst->mCallback;
    }
protected:
    /* --------------------------------------------------------------------------------------------
     * Instance of the wrapped job.
    */
    BaseJob * m_Inst;
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    JobWrapperBase(BaseJob * job) noexcept
        : m_Inst(job)
    {
    }
};

/* ------------------------------------------------------------------------------------------------
 * Used internally to wrap a job and expose it to the script.
*/
template < class T > struct JobWrapper : public JobWrapperBase
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    JobWrapper()
        : JobWrapperBase(new T)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * 
    */
    void SetCode(StackStrF & str)
    {
        static_cast< T * >(m_Inst)->mCode.assign(str.mPtr, static_cast< size_t >(str.mLen));
    }
    /* --------------------------------------------------------------------------------------------
     * 
    */
    const String & GetCode() const
    {
        return static_cast< T * >(m_Inst)->mCode;
    }
    /* --------------------------------------------------------------------------------------------
     * 
    */
    void SetName(StackStrF & str)
    {
        static_cast< T * >(m_Inst)->mName.assign(str.mPtr, static_cast< size_t >(str.mLen));
    }
    /* --------------------------------------------------------------------------------------------
     * 
    */
    JobWrapper< T > & ApplyName(StackStrF & str)
    {
        SetName(str);
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * 
    */
    const String & GetName() const
    {
        return static_cast< T * >(m_Inst)->mName;
    }
};

} // Namespace:: SqMod
