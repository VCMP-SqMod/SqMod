#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"
#include "Library/Worker/Parameter.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
struct Worker;

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

} // Namespace:: SqMod
