// ------------------------------------------------------------------------------------------------
#include "Tasks.hpp"
#include "Core.hpp"
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
SQMODE_DECL_TYPENAME(Typename, _SC("SqTask"))

// ------------------------------------------------------------------------------------------------
Uint32              Tasks::s_Used = 0;
Tasks::Time         Tasks::s_Last = 0;
Tasks::Time         Tasks::s_Prev = 0;
Tasks::Interval     Tasks::s_Intervals[SQMOD_MAX_TASKS];
Tasks::Task         Tasks::s_Tasks[SQMOD_MAX_TASKS];

// ------------------------------------------------------------------------------------------------
void Tasks::Task::Init(HSQOBJECT & func, HSQOBJECT & inst, Interval intrv, Iterator itr, Int32 id, Int32 type)
{
    // Initialize the callback hash
    mHash = 0;
    // Initialize the callback objects
    mFunc = LightObj(func);
    mInst = LightObj(inst);
    // Initialize the task options
    mIterations = itr;
    mInterval = intrv;
    // Initialize the entity information
    mEntity = ConvTo< Int16 >::From(id);
    mType = ConvTo< Uint8 >::From(type);
    // Grab the virtual machine once
    HSQUIRRELVM vm = DefaultVM::Get();
    // Remember the current stack size
    const StackGuard sg(vm);
    // Is there a valid function?
    if (!mFunc.IsNull())
    {
        // Push the callback on the stack
        sq_pushobject(vm, mFunc);
        // Grab the hash of the callback object
        mHash = sq_gethash(vm, -1);
    }
}

// ------------------------------------------------------------------------------------------------
void Tasks::Task::Release()
{
    mHash = 0;
    mFunc.Release();
    mInst.Release();
    mData.Release();
    mIterations = 0;
    mInterval = 0;
    mEntity = -1;
    mType = -1;
}

// ------------------------------------------------------------------------------------------------
Tasks::Interval Tasks::Task::Execute()
{
    // Are we even a valid task?
    if (INVALID_ENTITY(mEntity))
    {
        return 0; // Dunno how we got here but it ends now
    }
    // Grab the virtual machine once
    HSQUIRRELVM vm = DefaultVM::Get();
    // Push the function on the stack
    sq_pushobject(vm, mFunc);
    // Push the environment on the stack
    sq_pushobject(vm, mSelf);
    // Push function parameters, if any
    for (Uint32 n = 0; n < mArgc; ++n)
    {
        sq_pushobject(vm, mArgv[n].mObj);
    }
    // Make the function call and store the result
    const SQRESULT res = sq_call(vm, mArgc + 1, false, ErrorHandling::IsEnabled());
    // Validate the result
    if (SQ_FAILED(res))
    {
        // Destroy ourself on error
        Terminate();
    }
    // Decrease the number of iterations if necessary
    if (mIterations && (--mIterations) == 0)
    {
        // This routine reached the end of it's life
        Terminate();
    }
    // Return the current interval
    return mInterval;
}

// ------------------------------------------------------------------------------------------------
void Tasks::Process()
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
    // Process all active tasks
    for (Interval * itr = s_Intervals; itr != (s_Intervals + SQMOD_MAX_TASKS); ++itr)
    {
        // Is this task valid?
        if (*itr)
        {
            // Decrease the elapsed time
            (*itr) -= delta;
            // Have we completed the routine interval?
            if ((*itr) <= 0)
            {
                // Reset the elapsed time
                (*itr) = s_Tasks[itr - s_Intervals].Execute();
            }
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Tasks::Initialize()
{
    std::memset(s_Intervals, 0, sizeof(s_Intervals));
    // Transform all task instances to script objects
    for (auto & t : s_Tasks)
    {
        // This is fine because they'll always outlive the virtual machine
        t.mSelf = LightObj(&t);
    }
}

// ------------------------------------------------------------------------------------------------
void Tasks::Register(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(Typename::Str,
        Class< Task, NoDestructor< Task > >(vm, Typename::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Task::ToString)
        // Properties
        .Prop(_SC("Self"), &Task::GetSelf)
        .Prop(_SC("Inst"), &Task::GetInst)
        .Prop(_SC("Func"), &Task::GetFunc, &Task::SetFunc)
        .Prop(_SC("Data"), &Task::GetData, &Task::SetData)
        .Prop(_SC("Interval"), &Task::GetInterval, &Task::SetInterval)
        .Prop(_SC("Iterations"), &Task::GetIterations, &Task::SetIterations)
        .Prop(_SC("Arguments"), &Task::GetArguments)
        .Prop(_SC("Entity"), &Task::GetInst)
        // Member Methods
        .Func(_SC("Terminate"), &Task::Terminate)
        .Func(_SC("GetArgument"), &Task::GetArgument)
    );
}

// ------------------------------------------------------------------------------------------------
void Tasks::Deinitialize()
{
    // Release any script resources that the tasks might store
    for (auto & t : s_Tasks)
    {
        t.Terminate();
        t.mSelf.Release();
    }
}

// ------------------------------------------------------------------------------------------------
Object & Tasks::FindEntity(Int32 id, Int32 type)
{
    switch (type)
    {
        case ENT_BLIP:          return Core::Get().GetBlip(id).mObj;
        case ENT_CHECKPOINT:    return Core::Get().GetCheckpoint(id).mObj;
        case ENT_KEYBIND:       return Core::Get().GetKeybind(id).mObj;
        case ENT_OBJECT:        return Core::Get().GetObject(id).mObj;
        case ENT_PICKUP:        return Core::Get().GetPickup(id).mObj;
        case ENT_PLAYER:        return Core::Get().GetPlayer(id).mObj;
        case ENT_VEHICLE:       return Core::Get().GetVehicle(id).mObj;
        default:                return NullObject();
    }
}

// ------------------------------------------------------------------------------------------------
SQInteger Tasks::FindUnused()
{
    for (const auto & t : s_Tasks)
    {
        if (INVALID_ENTITY(t.mEntity))
        {
            return (&t - s_Tasks); // Return the index of this element
        }
    }
    // No available slot
    return -1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Tasks::Create(Int32 id, Int32 type, HSQUIRRELVM vm)
{
    // See if we have where to store this task
    if (s_Used >= SQMOD_MAX_TASKS)
    {
        return sq_throwerror(vm, "Reached the maximum number of tasks");
    }
    // Grab the top of the stack
    const SQInteger top = sq_gettop(vm);
    // See if too many arguments were specified
    if (top > 18) /* 4 base + 14 parameters = 18 */
    {
        return sq_throwerror(vm, "Too many parameter specified");
    }
    // Was there a callback specified?
    else if (top <= 1)
    {
        return sq_throwerror(vm, "Missing task callback");
    }
    // Validate the callback type
    else if (sq_gettype(vm, 2) != OT_CLOSURE && sq_gettype(vm, 2) != OT_NATIVECLOSURE)
    {
        return sq_throwerror(vm, "Invalid callback type");
    }
    // Prepare an entity instance object
    HSQOBJECT inst;
    // Attempt to retrieve the entity instance
    try
    {
        inst =  FindEntity(id, type).GetObject();
    }
    catch (const std::exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    // Prepare the function object
    HSQOBJECT func;
    // Fetch the specified callback
    SQRESULT res = sq_getstackobj(vm, 2, &func);
    // Validate the result
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error
    }

    // The number of iterations and interval to execute the task
    SQInteger intrv = 0, itr = 0;
    // Was there an interval specified?
    if (top > 2)
    {
        // Grab the interval from the stack
        res = sq_getinteger(vm, 3, &intrv);
        // Validate the result
        if (SQ_FAILED(res))
        {
            return res; // Propagate the error
        }
    }
    // Was there a number of iterations specified?
    if (top > 3)
    {
        // Grab the iterations from the stack
        res = sq_getinteger(vm, 4, &itr);
        // Validate the result
        if (SQ_FAILED(res))
        {
            return res; // Propagate the error
        }
    }

    // Obtain the identifier of a free slot
    const SQInteger slot = FindUnused();
    // Validate the slot, although it shouldn't be necessary
    if (slot < 0)
    {
        return sq_throwerror(vm, "Unable to acquire a task slot");
    }

    // At this point we can grab a reference to our slot
    Task & task = s_Tasks[slot];
    // Were there any arguments specified?
    if (top > 4)
    {
        // Grab a pointer to the arguments array
        Argument * args = task.mArgv;
        // Reset the argument counter
        task.mArgc = 0;
        // Grab the specified arguments from the stack
        for (SQInteger i = 5; i <= top; ++i)
        {
            res = sq_getstackobj(vm, i, &(args[task.mArgc].mObj));
            // Validate the result
            if (SQ_FAILED(res))
            {
                // Clear previous arguments
                task.Clear();
                // Propagate the error
                return res;
            }
            // Keep a strong reference to the argument
            sq_addref(vm, &(args[task.mArgc].mObj));
            // Increase the argument counter
            ++task.mArgc;
        }
    }
    // Alright, at this point we can initialize the slot
    task.Init(func, inst, intrv, itr, id, type);
    // Now initialize the interval
    s_Intervals[slot] = intrv;
    // Increase the number of used slots
    ++s_Used;
    // Specify that this function doesn't return anything
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger Tasks::Find(Int32 id, Int32 type, SQInteger & pos, HSQUIRRELVM vm)
{
    // Grab the top of the stack
    const SQInteger top = sq_gettop(vm);
    // Was there a callback specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing task callback");
    }

    SQRESULT res = SQ_OK;
    // Grab the hash of the callback object
    const SQHash chash = sq_gethash(vm, 2);
    // Should we include the iterations in the criteria?
    if (top > 3)
    {
        SQInteger intrv = 0;
        // Grab the interval from the stack
        res = sq_getinteger(vm, 3, &intrv);
        // Validate the result
        if (SQ_FAILED(res))
        {
            return res; // Propagate the error
        }
        // Attempt to find the requested task
        for (const auto & t : s_Tasks)
        {
            if (t.mHash == chash && t.mEntity == id && t.mType == type && t.mInterval == intrv)
            {
                pos = static_cast< SQInteger >(&t - s_Tasks); // Store the index of this element
            }
        }
    }
    // Should we include the interval in the criteria?
    else if (top > 2)
    {
        SQInteger intrv = 0, sqitr = 0;
        // Grab the interval from the stack
        res = sq_getinteger(vm, 3, &intrv);
        // Validate the result
        if (SQ_FAILED(res))
        {
            return res; // Propagate the error
        }
        // Grab the iterations from the stack
        res = sq_getinteger(vm, 4, &sqitr);
        // Validate the result
        if (SQ_FAILED(res))
        {
            return res; // Propagate the error
        }
        // Cast iterations to the right type
        const Iterator itr = ConvTo< Iterator >::From(sqitr);
        // Attempt to find the requested task
        for (const auto & t : s_Tasks)
        {
            if (t.mHash == chash && t.mEntity == id && t.mType == type && t.mInterval == intrv && t.mIterations == itr)
            {
                pos = static_cast< SQInteger >(&t - s_Tasks); // Store the index of this element
            }
        }
    }
    else
    {
        // Attempt to find the requested task
        for (const auto & t : s_Tasks)
        {
            if (t.mHash == chash && t.mEntity == id && t.mType == type)
            {
                pos = static_cast< SQInteger >(&t - s_Tasks); // Store the index of this element
            }
        }
    }
    // We could not find such task
    return res;
}

// ------------------------------------------------------------------------------------------------
SQInteger Tasks::Remove(Int32 id, Int32 type, HSQUIRRELVM vm)
{
    // Default to not found
    SQInteger pos = -1;
    // Perform a search
    SQRESULT res = Find(id, type, pos, vm);
    // Did the search failed?
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error
    }
    // Did we find anything?
    else if (pos < 0)
    {
        return sq_throwerror(vm, "Unable to locate such task");
    }
    else
    {
        // Release task resources
        s_Tasks[pos].Terminate();
        // Reset the timer
        s_Intervals[pos] = 0;
    }
    // Specify that we don't return anything
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger Tasks::Exists(Int32 id, Int32 type, HSQUIRRELVM vm)
{
    // Default to not found
    SQInteger pos = -1;
    // Perform a search
    SQRESULT res = Find(id, type, pos, vm);
    // Did the search failed?
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error
    }
    // Push a boolean on whether this task was found
    sq_pushbool(vm, pos >= 0);
    // Specify that we're returning a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
void Tasks::Cleanup(Int32 id, Int32 type)
{
    for (auto & t : s_Tasks)
    {
        if (t.mEntity == id && t.mType == type)
        {
            t.Terminate();
            // Also disable the timer
            s_Intervals[&t - s_Tasks] = 0;
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to process tasks.
*/
void ProcessTasks()
{
    Tasks::Process();
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to initialize tasks.
*/
void InitializeTasks()
{
    Tasks::Initialize();
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to register tasks.
*/
void RegisterTask(HSQUIRRELVM vm)
{
    Tasks::Register(vm);
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to terminate tasks.
*/
void TerminateTasks()
{
    Tasks::Deinitialize();
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to cleanup certain tasks.
*/
void CleanupTasks(Int32 id, Int32 type)
{
    Tasks::Cleanup(id, type);
}

} // Namespace:: SqMod
