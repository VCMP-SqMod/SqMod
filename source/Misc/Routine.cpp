// ------------------------------------------------------------------------------------------------
#include "Misc/Routine.hpp"
#include "Library/Chrono.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
#include <utility>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("SqRoutineBase"))

// ------------------------------------------------------------------------------------------------
Routine::Time       Routine::s_Last = 0;
Routine::Time       Routine::s_Prev = 0;
Routine::Interval   Routine::s_Intervals[SQMOD_MAX_ROUTINES];
Routine::Instance   Routine::s_Instances[SQMOD_MAX_ROUTINES];

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
    const Int32 delta = Int32((s_Last - s_Prev) / 1000L);
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
SQInteger Routine::Create(HSQUIRRELVM vm)
{
    // Locate the identifier of a free slot
    const SQInteger slot = FindUnused();
    // See if we have where to store this routine
    if (slot < 0)
    {
        return sq_throwerror(vm, "Reached the maximum number of active routines");
    }
    // Grab the top of the stack
    const SQInteger top = sq_gettop(vm);
    // See if too many arguments were specified
    if (top >= 20) /* 5 base + 14 parameters = 19 */
    {
        return sq_throwerror(vm, "Too many parameters specified");
    }
    // Was there was an environment specified?
    else if (top <= 1)
    {
        return sq_throwerror(vm, "Missing routine environment");
    }
    // Was there was a callback specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing routine callback");
    }
    // Validate the callback type
    else if (sq_gettype(vm, 3) != OT_CLOSURE && sq_gettype(vm, 3) != OT_NATIVECLOSURE)
    {
        return sq_throwerror(vm, "Invalid callback type");
    }

    SQRESULT res = SQ_OK;
    // Prepare an object for the environment
    HSQOBJECT env;
    // Is the specified environment a null value?
    if (sq_gettype(vm, 2) == OT_NULL)
    {
        // Preserve the stack state
        const StackGuard sg(vm);
        // Push the root table on the stack
        sq_pushroottable(vm);
        // Attempt to retrieve the table object
        res = sq_getstackobj(vm, -1, &env);
    }
    else
    {
        sq_getstackobj(vm, 2, &env); // Just retrieve the specified environment
    }
    // Validate the result
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error
    }

    // Prepare an object for the function
    HSQOBJECT func;
    // Fetch the specified callback object
    res = sq_getstackobj(vm, 3, &func);
    // Validate the result
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error
    }

    // The number of iterations and interval to execute the routine
    SQInteger intrv = 0, itr = 0;
    // Was there an interval specified?
    if (top > 3)
    {
        // Grab the interval from the stack
        res = sq_getinteger(vm, 4, &intrv);
        // Validate the result
        if (SQ_FAILED(res))
        {
            return res; // Propagate the error
        }
    }
    // Was there a number of iterations specified?
    if (top > 4)
    {
        // Grab the iterations from the stack
        res = sq_getinteger(vm, 5, &itr);
        // Validate the result
        if (SQ_FAILED(res))
        {
            return res; // Propagate the error
        }
    }

    // Attempt to create a routine instance
    try
    {
        ClassType< Routine >::PushInstance(vm, new Routine());
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, "Unable to create the routine instance");
    }
    // Prepare an object for the routine
    HSQOBJECT obj;
    // Fetch the created routine object
    res = sq_getstackobj(vm, -1, &obj);
    // Validate the result
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error
    }

    // At this point we can grab a reference to our slot
    Instance & inst = s_Instances[slot];
    // Were there any arguments specified?
    if (top > 5)
    {
        // Grab a pointer to the arguments array
        Argument * args = inst.mArgv;
        // Reset the argument counter
        inst.mArgc = 0;
        // Grab the specified arguments from the stack
        for (SQInteger i = 6; i <= top; ++i)
        {
            res = sq_getstackobj(vm, i, &(args[inst.mArgc].mObj));
            // Validate the result
            if (SQ_FAILED(res))
            {
                // Clear previous arguments
                inst.Clear();
                // Propagate the error
                return res;
            }
            // Keep a strong reference to the argument
            sq_addref(vm, &(args[inst.mArgc].mObj));
            // Increase the argument counter
            ++inst.mArgc;
        }
    }

    // Attempt to retrieve the routine from the stack and associate it with the slot
    try
    {
        Var< Routine * >(vm, -1).value->m_Slot = ConvTo< Uint32 >::From(slot);
    }
    catch (const Sqrat::Exception & e)
    {
        // Clear extracted arguments
        inst.Clear();
        // Now it's safe to throw the error
        return sq_throwerror(vm, "Unable to create the routine instance");
    }

    // Alright, at this point we can initialize the slot
    inst.Init(env, func, obj, intrv, itr);
    // Now initialize the timer
    s_Intervals[slot] = intrv;
    // We have the created routine on the stack, so let's return it
    return 1;
}

// ------------------------------------------------------------------------------------------------
bool Routine::IsWithTag(const StackStrF & tag)
{
    // Is the specified tag valid?
    if (tag.mPtr != nullptr)
    {
        // Iterate routine list
        for (const auto & r : s_Instances)
        {
            if (!r.mInst.IsNull() && r.mTag.compare(tag.mPtr) == 0)
            {
                return true; // Yup, we're doing this
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
        .Prop(_SC("Quiet"), &Routine::GetQuiet, &Routine::SetQuiet)
        .Prop(_SC("Endure"), &Routine::GetEndure, &Routine::SetEndure)
        .Prop(_SC("Arguments"), &Routine::GetArguments)
        // Member Methods
        .FmtFunc(_SC("SetTag"), &Routine::SetTag)
        .Func(_SC("Terminate"), &Routine::Terminate)
        .Func(_SC("GetArgument"), &Routine::GetArgument)
    );
    // Global functions
    RootTable(vm).SquirrelFunc(_SC("SqRoutine"), &Routine::Create);
    RootTable(vm).FmtFunc(_SC("SqFindRoutineByTag"), &Routine::FindByTag);
    RootTable(vm).FmtFunc(_SC("SqIsRoutineWithTag"), &Routine::IsWithTag);
}

} // Namespace:: SqMod
