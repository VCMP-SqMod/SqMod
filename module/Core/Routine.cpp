// ------------------------------------------------------------------------------------------------
#include "Core/Routine.hpp"
#include "Library/Chrono.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>

// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("SqRoutineInstance"))

// ------------------------------------------------------------------------------------------------
Routine::Time       Routine::s_Last = 0;
Routine::Time       Routine::s_Prev = 0;
Routine::Interval   Routine::s_Intervals[SQMOD_MAX_ROUTINES];
Routine::Instance   Routine::s_Instances[SQMOD_MAX_ROUTINES];
bool                Routine::s_Silenced = false;
bool                Routine::s_Persistent = false;

// ------------------------------------------------------------------------------------------------
void Routine::Process()
{
    // Is this the first call?
    if (s_Last == 0)
    {
        s_Last = Chrono::GetCurrentSysTime();
        // We'll do it text time
        return;
    }
    // Backup the last known time-stamp
    s_Prev = s_Last;
    // Get the current time-stamp
    s_Last = Chrono::GetCurrentSysTime();
    // Calculate the elapsed time
    const auto delta = int32_t((s_Last - s_Prev) / 1000L);
    // Process all active routines
    for (Interval * itr = s_Intervals; itr != (s_Intervals + SQMOD_MAX_ROUTINES); ++itr)
    {
        // Is this routine valid?
        if (*itr)
        {
            // Decrease the elapsed time
            (*itr) -= delta;
            // Have we completed the routine interval?
            if ((*itr) <= 0)
            {
                // Execute and reset the elapsed time
                (*itr) = s_Instances[itr - s_Intervals].Execute();
            }
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Routine::Initialize()
{
    std::memset(s_Intervals, 0, sizeof(s_Intervals));
    SetSilenced(!ErrorHandling::IsEnabled());
}

// ------------------------------------------------------------------------------------------------
void Routine::Deinitialize()
{
    // Release any script resources that the routines might store
    for (auto & r : s_Instances)
    {
        r.Terminate();
    }
}

// ------------------------------------------------------------------------------------------------
struct RoutineBuilder
{
    HSQUIRRELVM mVM{nullptr};
    SQRESULT    mRes{SQ_OK};
    SQInteger   mTop{0};
    SQInteger   mSlot{-1};
    HSQOBJECT   mEnv{};
    HSQOBJECT   mFunc{};
    HSQOBJECT   mInst{};
    SQInteger   mInterval{0};
    SQInteger   mIterations{0};

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    RoutineBuilder(HSQUIRRELVM vm, SQInteger slot)
        : mVM(vm), mRes(), mTop(sq_gettop(vm)), mSlot(slot), mEnv(), mFunc(), mInst(), mInterval(0), mIterations(0)
    {
        sq_resetobject(&mEnv);
        sq_resetobject(&mFunc);
        sq_resetobject(&mInst);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy/Move constructor (disabled).
    */
    RoutineBuilder(const RoutineBuilder &) = delete;
    RoutineBuilder(RoutineBuilder &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~RoutineBuilder() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy/Move assignment operator (disabled).
    */
    RoutineBuilder & operator = (const RoutineBuilder &) = delete;
    RoutineBuilder & operator = (RoutineBuilder &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Initialize routine creation.
    */
    SQRESULT Begin()
    {
        mRes = SQ_OK;
        // See if we have where to store this routine
        if (mSlot < 0)
        {
            mRes = sq_throwerror(mVM, "Reached the maximum number of active routines");
        }
        // See if too many arguments were specified
        else if (mTop >= 20) /* 5 base + 14 parameters = 19 */
        {
            mRes = sq_throwerror(mVM, "Too many parameters specified");
        }
        // Was there was an environment specified?
        else if (mTop <= 1)
        {
            mRes = sq_throwerror(mVM, "Missing routine environment");
        }
        // Was there was a callback specified?
        else if (mTop <= 2)
        {
            mRes = sq_throwerror(mVM, "Missing routine callback");
        }
        // Validate the callback type
        else if (sq_gettype(mVM, 3) != OT_CLOSURE && sq_gettype(mVM, 3) != OT_NATIVECLOSURE)
        {
            mRes = sq_throwerror(mVM, "Invalid callback type");
        }
        // Return the result
        return mRes;
    }

    /* --------------------------------------------------------------------------------------------
     * Extract target.
    */
    SQRESULT Target(SQInteger env, SQInteger func)
    {
        // Get the type of the environment object
        const SQObjectType type = sq_gettype(mVM, env);
        // Whether to default to the root table
        bool use_root = (type == OT_NULL);

        // Is the specified environment a boolean (true) value?
        if (type == OT_STRING)
        {
            StackStrF val(mVM, env);
            // Attempt to generate the string value
            mRes = val.Proc(false);
            // Have we failed to retrieve the string?
            if (SQ_FAILED(mRes))
            {
                return mRes; // Propagate the error!
            }
            // If the string is empty or "root" then we use the root table
            else if (!val.mLen || sqmod_stricmp(val.mPtr, "root") == 0)
            {
                use_root = true;
            }
            // If the string is "self" then we leave it null and default to self
            else if (sqmod_stricmp(val.mPtr, "self") == 0)
            {
                use_root = false; // Just in case
            }
        }

        // Is the specified environment a null value?
        if (use_root)
        {
            // Push the root table on the stack
            sq_pushroottable(mVM);
            // Attempt to retrieve the table object
            mRes = sq_getstackobj(mVM, -1, &mEnv);
            // Preserve the stack state
            sq_poptop(mVM);
        }
        // Should we treat it as a valid environment object?
        else if (type != OT_STRING)
        {
            mRes = sq_getstackobj(mVM, env, &mEnv); // Just retrieve the specified environment
        }
        // Can we attempt to retrieve the callback?
        if (SQ_SUCCEEDED(mRes))
        {
            // Fetch the specified callback object
            mRes = sq_getstackobj(mVM, func, &mFunc);
        }
        // Return result
        return mRes;
    }

    /* --------------------------------------------------------------------------------------------
     * Extract options.
    */
    SQRESULT Opts(SQInteger iv, SQInteger it)
    {
        // Was there an interval specified?
        if (mTop >= iv)
        {
            // Grab the interval from the stack
            mRes = sq_getinteger(mVM, iv, &mInterval);
            // Validate the result
            if (SQ_FAILED(mRes))
            {
                return mRes; // Propagate the error
            }
        }

        // Was there a number of iterations specified?
        if (mTop >= it)
        {
            // Grab the iterations from the stack
            mRes = sq_getinteger(mVM, it, &mIterations);
            // Validate the result
            if (SQ_FAILED(mRes))
            {
                return mRes; // Propagate the error
            }
        }

        // Successfully processed
        return SQ_OK;
    }

    /* --------------------------------------------------------------------------------------------
     * Create instance.
    */
    SQRESULT Inst()
    {
        // Attempt to create a routine instance
        try
        {
            DeleteGuard< Routine > dg(new Routine());
            ClassType< Routine >::PushInstance(mVM, dg.Get());
            dg.Release();
        }
        catch (const std::exception & e)
        {
            return (mRes = sq_throwerrorf(mVM, "Unable to create the routine instance: %s", e.what()));
        }

        // Fetch the created routine object
        mRes = sq_getstackobj(mVM, -1, &mInst);
        // Validate the result
        if (SQ_FAILED(mRes))
        {
            return mRes; // Propagate the error
        }

        // Successfully created
        return SQ_OK;
    }

    /* --------------------------------------------------------------------------------------------
     * Extract arguments.
    */
    SQRESULT Args(SQInteger idx)
    {
        // At this point we can grab a reference to our slot
        Routine::Instance & inst = Routine::s_Instances[mSlot];
        // Were there any arguments specified?
        if (mTop >= idx)
        {
            // Grab a pointer to the arguments array
            Routine::Argument * args = inst.mArgv;
            // Reset the argument counter
            inst.mArgc = 0;
            // Grab the specified arguments from the stack
            for (SQInteger i = idx; i <= mTop; ++i)
            {
                mRes = sq_getstackobj(mVM, i, &(args[inst.mArgc].mObj));
                // Validate the result
                if (SQ_FAILED(mRes))
                {
                    // Clear previous arguments
                    inst.Clear();
                    // Propagate the error
                    return mRes;
                }
                // Keep a strong reference to the argument
                sq_addref(mVM, &(args[inst.mArgc].mObj));
                // Increase the argument counter
                ++inst.mArgc;
            }
        }

        // Successfully processed
        return SQ_OK;
    }

    /* --------------------------------------------------------------------------------------------
     * Finish routine creation.
    */
#ifdef VCMP_ENABLE_OFFICIAL
    SQRESULT Finish(bool refs = false)
#else
    SQRESULT Finish()
#endif
    {
        // Grab a reference to our slot
        Routine::Instance & inst = Routine::s_Instances[mSlot];
        // Attempt to retrieve the routine from the stack and associate it with the slot
        try
        {
            Var< Routine * >(mVM, -1).value->m_Slot = ConvTo< uint32_t >::From(mSlot);
        }
        catch (const std::exception & e)
        {
            // Clear extracted arguments
            inst.Clear();
            // Now it's safe to throw the error
            return (mRes = sq_throwerrorf(mVM, "Unable to create the routine instance: %s", e.what()));
        }

        // Alright, at this point we can initialize the slot
        inst.Init(mEnv, mFunc, mInst, mInterval, static_cast< Routine::Iterator >(mIterations));
        // Now initialize the timer
        Routine::s_Intervals[mSlot] = mInterval;
#ifdef VCMP_ENABLE_OFFICIAL
        // Drop the temporary callback reference
        if (refs)
        {
            sq_release(mVM, &mFunc);
        }
#endif
        // Successfully finished
        return SQ_OK;
    }
#ifdef VCMP_ENABLE_OFFICIAL
    /* --------------------------------------------------------------------------------------------
     * Initialize timer creation.
    */
    SQRESULT BeginComp()
    {
        mRes = SQ_OK;
        // See if we have where to store this timers
        if (mSlot < 0)
        {
            mRes = sq_throwerror(mVM, "Reached the maximum number of active routines");
        }
        // See if too many arguments were specified
        else if (mTop >= 19) /* 4 base + 14 parameters = 18 */
        {
            mRes = sq_throwerror(mVM, "Too many parameters specified");
        }
        // Was there was a callback specified?
        else if (mTop <= 1)
        {
            mRes = sq_throwerror(mVM, "Missing timer callback");
        }
        // Validate the callback type
        else if (sq_gettype(mVM, 2) != OT_STRING)
        {
            mRes = sq_throwerror(mVM, "Invalid callback type");
        }
        // Return the result
        return mRes;
    }

    /* --------------------------------------------------------------------------------------------
     * Extract target.
    */
    SQRESULT TargetComp(SQInteger func)
    {
        StackStrF val(mVM, func);
        // Attempt to generate the string value
        mRes = val.Proc(false);
        // Have we failed to retrieve the string?
        if (SQ_FAILED(mRes))
        {
            return mRes; // Propagate the error!
        }
        // Push the root table on the stack
        sq_pushroottable(mVM);
        // Attempt to retrieve the table object
        mRes = sq_getstackobj(mVM, -1, &mEnv);
        // Have we failed to retrieve the string?
        if (SQ_FAILED(mRes))
        {
            return mRes; // Propagate the error!
        }
        // Push the callback name on the stack
        sq_pushobject(mVM, val.mObj);
        // Attempt to retrieve the value from the root table
        mRes = sq_get(mVM, -2);
        // Do we have the callback on the stack?
        if (SQ_SUCCEEDED(mRes))
        {
            // Attempt to retrieve the function object
            mRes = sq_getstackobj(mVM, -1, &mFunc);
            // Keep a reference to it for now
            if (SQ_SUCCEEDED(mRes))
            {
                sq_addref(mVM, &mFunc);
            }
            // Pop the callback and root table
            sq_pop(mVM, 2);
        }
        else
        {
            sq_poptop(mVM); // Pop the root table
        }
        // Return the result
        return mRes;
    }
#endif
};

// ------------------------------------------------------------------------------------------------
SQInteger Routine::Create(HSQUIRRELVM vm)
{
    RoutineBuilder b(vm, FindUnused());
    // Initialize routine
    if (SQ_FAILED(b.Begin()) ||
        SQ_FAILED(b.Target(2, 3)) ||
        SQ_FAILED(b.Opts(4, 5)) ||
        SQ_FAILED(b.Args(6)) ||
        SQ_FAILED(b.Inst()) ||
        SQ_FAILED(b.Finish()))
    {
        return b.mRes; // Propagate result
    }
    // We have the created routine on the stack, so let's return it
    return 1;
}
#ifdef VCMP_ENABLE_OFFICIAL
// ------------------------------------------------------------------------------------------------
SQInteger Routine::CreateOfficial(HSQUIRRELVM vm)
{
    RoutineBuilder b(vm, FindUnused());
    // Initialize routine
    if (SQ_FAILED(b.BeginComp()) ||
        SQ_FAILED(b.TargetComp(2)) ||
        SQ_FAILED(b.Opts(3, 4)) ||
        SQ_FAILED(b.Args(5)) ||
        SQ_FAILED(b.Inst()) ||
        SQ_FAILED(b.Finish(true)))
    {
        return b.mRes; // Propagate result
    }
    // We have the created routine on the stack, so let's return it
    return 1;
}
#endif
// ------------------------------------------------------------------------------------------------
bool Routine::IsWithTag(StackStrF & tag)
{
    // Is the specified tag valid?
    if (tag.mPtr != nullptr)
    {
        // Iterate routine list
        for (const auto & r : s_Instances)
        {
            if (!r.mInst.IsNull() && r.mTag == tag.mPtr)
            {
                return true; // Yup, we're doing this
            }
        }
    }
    // Unable to find such routine
    return false;
}

// ------------------------------------------------------------------------------------------------
bool Routine::TerminateWithTag(StackStrF & tag)
{
    // Is the specified tag valid?
    if (tag.mPtr != nullptr)
    {
        // Iterate routine list
        for (auto & r : s_Instances)
        {
            if (!r.mInst.IsNull() && r.mTag == tag.mPtr)
            {
                r.Terminate(); // Yup, we're doing this
                return true; // A routine was terminated
            }
        }
    }
    // Unable to find such routine
    return false;
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to process routines.
*/
void ProcessRoutines()
{
    Routine::Process();
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to initialize routines.
*/
void InitializeRoutines()
{
    Routine::Initialize();
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to terminate routines.
*/
void TerminateRoutines()
{
    Routine::Deinitialize();
}

// ================================================================================================
void Register_Routine(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(Typename::Str,
        Class< Routine, NoConstructor< Routine > >(vm, Typename::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Routine::ToString)
        // Properties
        .Prop(_SC("Tag"), &Routine::GetTag, &Routine::SetTag)
        .Prop(_SC("Env"), &Routine::GetEnv, &Routine::SetEnv)
        .Prop(_SC("Func"), &Routine::GetFunc, &Routine::SetFunc)
        .Prop(_SC("Data"), &Routine::GetData, &Routine::SetData)
        .Prop(_SC("Interval"), &Routine::GetInterval, &Routine::SetInterval)
        .Prop(_SC("Iterations"), &Routine::GetIterations, &Routine::SetIterations)
        .Prop(_SC("Suspended"), &Routine::GetSuspended, &Routine::SetSuspended)
        .Prop(_SC("Executing"), &Routine::GetExecuting)
        .Prop(_SC("Quiet"), &Routine::GetQuiet, &Routine::SetQuiet)
        .Prop(_SC("Endure"), &Routine::GetEndure, &Routine::SetEndure)
        .Prop(_SC("Inactive"), &Routine::GetInactive)
        .Prop(_SC("Persistent"), &Routine::GetPersistent, &Routine::SetPersistent)
        .Prop(_SC("Terminated"), &Routine::GetTerminated)
        .Prop(_SC("Arguments"), &Routine::GetArguments)
        // Member Methods
        .FmtFunc(_SC("SetTag"), &Routine::ApplyTag)
        .Func(_SC("SetData"), &Routine::ApplyData)
        .Func(_SC("SetInterval"), &Routine::ApplyInterval)
        .Func(_SC("SetIterations"), &Routine::ApplyIterations)
        .Func(_SC("SetSuspended"), &Routine::ApplySuspended)
        .Func(_SC("SetQuiet"), &Routine::ApplyQuiet)
        .Func(_SC("SetEndure"), &Routine::ApplyEndure)
        .Func(_SC("SetPersistent"), &Routine::ApplyPersistent)
        .Func(_SC("Terminate"), &Routine::Terminate)
        .Func(_SC("GetArgument"), &Routine::GetArgument)
        .Func(_SC("DropEnv"), &Routine::DropEnv)
        .Func(_SC("Restart"), &Routine::Restart)
        .StaticFunc(_SC("UsedCount"), &Routine::GetUsed)
        .StaticFunc(_SC("AreSilenced"), &Routine::GetSilenced)
        .StaticFunc(_SC("SetSilenced"), &Routine::SetSilenced)
        .StaticFunc(_SC("ArePersistent"), &Routine::GetPersistency)
        .StaticFunc(_SC("SetPersistency"), &Routine::SetPersistency)
    );
    // Global functions
    RootTable(vm).SquirrelFunc(_SC("SqRoutine"), &Routine::Create);
    RootTable(vm).FmtFunc(_SC("SqFindRoutineByTag"), &Routine::FindByTag);
    RootTable(vm).FmtFunc(_SC("SqIsRoutineWithTag"), &Routine::IsWithTag);
    RootTable(vm).FmtFunc(_SC("SqFetchRoutineWithTag"), &Routine::FetchWithTag);
    RootTable(vm).FmtFunc(_SC("SqTerminateRoutineWithTag"), &Routine::TerminateWithTag);
#ifdef VCMP_ENABLE_OFFICIAL
    RootTable(vm).SquirrelFunc(_SC("NewTimer"), &Routine::CreateOfficial);
#endif
}

} // Namespace:: SqMod
