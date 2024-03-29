// ------------------------------------------------------------------------------------------------
#include "Core/Tasks.hpp"
#include "Core.hpp"
#include "Library/Chrono.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>

// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("SqTask"))

// ------------------------------------------------------------------------------------------------
Tasks::Time         Tasks::s_Last = 0;
Tasks::Time         Tasks::s_Prev = 0;
Tasks::Interval     Tasks::s_Intervals[SQMOD_MAX_TASKS];
Tasks::Task         Tasks::s_Tasks[SQMOD_MAX_TASKS];

// ------------------------------------------------------------------------------------------------
void Tasks::Task::Init(HSQOBJECT & func, HSQOBJECT & inst, Interval intrv, Iterator itr, int32_t id, int32_t type)
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
    mEntity = ConvTo< int16_t >::From(id);
    mType = ConvTo< uint8_t >::From(type);
    // Grab the virtual machine once
    HSQUIRRELVM vm = SqVM();
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
    mTag.clear();
    mFunc.Release();
    mInst.Release();
    mData.Release();
    mIterations = 0;
    mInterval = 0;
    mEntity = -1;
    mType = 0;
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
    HSQUIRRELVM vm = SqVM();
    // Push the function on the stack
    sq_pushobject(vm, mFunc);
    // Push the environment on the stack
    sq_pushobject(vm, mSelf);
    // Push function parameters, if any
    for (uint32_t n = 0; n < mArgc; ++n)
    {
        sq_pushobject(vm, mArgv[n].mObj);
    }
    // Make the function call and store the result
    const SQRESULT res = sq_call(vm, mArgc + 1, static_cast< SQBool >(false), static_cast< SQBool >(ErrorHandling::IsEnabled()));
    // Pop the callback object from the stack
    sq_pop(vm, 1);
    // Validate the result
    if (SQ_FAILED(res))
    {
        Terminate(); // Destroy ourself on error
    }
    // Decrease the number of iterations if necessary
    if (mIterations && (--mIterations) == 0)
    {
        Terminate(); // This routine reached the end of it's life
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
    const auto delta = int32_t((s_Last - s_Prev) / 1000L);
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
                // Execute and reset the elapsed time
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
        .Prop(_SC("Tag"), &Task::GetTag, &Task::SetTag)
        .Prop(_SC("Entity"), &Task::GetInst)
        .Prop(_SC("Func"), &Task::GetFunc, &Task::SetFunc)
        .Prop(_SC("Data"), &Task::GetData, &Task::SetData)
        .Prop(_SC("Interval"), &Task::GetInterval, &Task::SetInterval)
        .Prop(_SC("Iterations"), &Task::GetIterations, &Task::SetIterations)
        .Prop(_SC("Arguments"), &Task::GetArguments)
        .Prop(_SC("Inst"), &Task::GetInst)
        // Member Methods
        .FmtFunc(_SC("SetTag"), &Task::SetTag)
        .Func(_SC("Terminate"), &Task::Terminate)
        .Func(_SC("GetArgument"), &Task::GetArgument)
        // Static functions
        .StaticFunc(_SC("Used"), &Tasks::GetUsed)
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
LightObj & Tasks::FindEntity(int32_t id, int32_t type)
{
    switch (type)
    {
        case ENT_BLIP:          return Core::Get().GetBlip(id).mObj;
        case ENT_CHECKPOINT:    return Core::Get().GetCheckpoint(id).mObj;
        case ENT_KEYBIND:       return Core::Get().GetKeyBind(id).mObj;
        case ENT_OBJECT:        return Core::Get().GetObj(id).mObj;
        case ENT_PICKUP:        return Core::Get().GetPickup(id).mObj;
        case ENT_PLAYER:        return Core::Get().GetPlayer(id).mObj;
        case ENT_VEHICLE:       return Core::Get().GetVehicle(id).mObj;
        default:                return NullLightObj();
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
SQInteger Tasks::Create(int32_t id, int32_t type, HSQUIRRELVM vm)
{
    // Locate the identifier of a free slot
    const SQInteger slot = FindUnused();
    // See if we have where to store this task
    if (slot < 0)
    {
        return sq_throwerror(vm, "Reached the maximum number of tasks");
    }
    // Grab the top of the stack
    const SQInteger top = sq_gettop(vm);
    // See if too many arguments were specified
    if (top > 12) /* 4 base + 8 parameters = 12 */
    {
        return sq_throwerror(vm, "Too many parameters specified");
    }
    // Was there was a callback specified?
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
        inst =  FindEntity(id, type).GetObj();
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
    task.Init(func, inst, intrv, static_cast< Iterator >(itr), id, type);
    // Now initialize the timer
    s_Intervals[slot] = intrv;
    // Push the tag instance on the stack
    sq_pushobject(vm, task.mSelf);
    // Specify that this function returns a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Tasks::Find(int32_t id, int32_t type, SQInteger & pos, HSQUIRRELVM vm)
{
    // Grab the top of the stack
    const SQInteger top = sq_gettop(vm);
    // Was there a callback or tag specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing task callback or tag");
    }
    SQRESULT res = SQ_OK;
    // Fetch the task identifier type
    const SQObjectType ot = sq_gettype(vm, 2);
    // Are we looking for a task with a specific tag?
    if (ot == OT_STRING)
    {
        // Attempt to retrieve the value from the stack as a string
        StackStrF tag(vm, 2);
        // Have we failed to retrieve the string?
        if (SQ_FAILED(tag.Proc(true)))
        {
            return tag.mRes; // Propagate the error!
        }
        // Attempt to find the requested task
        for (const auto & t : s_Tasks)
        {
            if (t.mEntity == id && t.mType == type && t.mTag.compare(0, String::npos, tag.mPtr) == 0)
            {
                pos = static_cast< SQInteger >(&t - s_Tasks); // Store the index of this element
            }
        }
    }
    // Validate the callback type
    else if (ot != OT_CLOSURE && ot != OT_NATIVECLOSURE)
    {
        return sq_throwerror(vm, "Invalid callback type");
    }
    // Should we include the iterations in the criteria?
    else if (top > 3)
    {
        SQInteger intrv = 0;
        // Grab the interval from the stack
        res = sq_getinteger(vm, 3, &intrv);
        // Validate the result
        if (SQ_FAILED(res))
        {
            return res; // Propagate the error
        }
        // Grab the hash of the callback object
        const SQHash chash = sq_gethash(vm, 2);
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
        // Grab the hash of the callback object
        const SQHash chash = sq_gethash(vm, 2);
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
        // Grab the hash of the callback object
        const SQHash chash = sq_gethash(vm, 2);
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
SQInteger Tasks::Remove(int32_t id, int32_t type, HSQUIRRELVM vm)
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
        sq_pushbool(vm, SQFalse); // Unable to locate such task
    }
    else
    {
        // Release task resources
        s_Tasks[pos].Terminate();
        // Reset the timer
        s_Intervals[pos] = 0;
        // A task was successfully removed
        sq_pushbool(vm, SQTrue);
    }
    // Specify that we return a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Tasks::Exists(int32_t id, int32_t type, HSQUIRRELVM vm)
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
    sq_pushbool(vm, static_cast< SQBool >(pos >= 0));
    // Specify that we're returning a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
const Tasks::Task & Tasks::FindByTag(int32_t id, int32_t type, StackStrF & tag)
{
    // Attempt to find the requested task
    for (const auto & t : s_Tasks)
    {
        if (t.mEntity == id && t.mType == type && t.mTag == tag.mPtr)
        {
            return t; // Return this task instance
        }
    }
    // Unable to find such task
    STHROWF("Unable to find a task with tag ({})", tag.mPtr);
    // Should not reach this point but if it did, we have to return something
    SQ_UNREACHABLE
}

// ------------------------------------------------------------------------------------------------
void Tasks::Cleanup(int32_t id, int32_t type)
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
 * Forward the call to terminate tasks.
*/
void TerminateTasks()
{
    Tasks::Deinitialize();
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to cleanup certain tasks.
*/
void CleanupTasks(int32_t id, int32_t type)
{
    Tasks::Cleanup(id, type);
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to register tasks.
*/
void Register_Tasks(HSQUIRRELVM vm)
{
    Tasks::Register(vm);
}

} // Namespace:: SqMod
