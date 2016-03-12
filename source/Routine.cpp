// ------------------------------------------------------------------------------------------------
#include "Routine.hpp"
#include "Library/Time.hpp"

// ------------------------------------------------------------------------------------------------
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool                Routine::s_Lock = false;
Routine::Time       Routine::s_Last = 0;
Routine::Time       Routine::s_Prev = 0;
Routine::Queue      Routine::s_Queue;
Routine::Buckets    Routine::s_Buckets;
Routine::Objects    Routine::s_Objects;

// ------------------------------------------------------------------------------------------------
SQInteger Routine::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqRoutine");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
void Routine::Attach(Routine * routine, Interval interval)
{
    // Do we have a valid routine and interval bucket to attach?
    if (!routine || ! interval)
    {
        return;
    }
    // Attempt to locate the bucket with the specified interval
    Buckets::iterator itr = std::find_if(s_Buckets.begin(), s_Buckets.end(), IntrvFunc(interval));
    // Does this bucket exist?
    if (itr == s_Buckets.end())
    {
        // Then create it
        s_Buckets.emplace_back(interval);
        // And attach this routine
        s_Buckets.back().mRoutines.push_back(routine);
    }
    // Is this routine already attached to this bucket?
    else if (std::find(itr->mRoutines.begin(), itr->mRoutines.end(), routine) != itr->mRoutines.end())
    {
        itr->mRoutines.push_back(routine); // Then let's attach it now
    }
}

// ------------------------------------------------------------------------------------------------
void Routine::Detach(Routine * routine, Interval interval)
{
    // Do we have a valid routine and interval to detach?
    if (!routine || ! interval)
    {
        return;
    }
    // Attempt to locate the bucket with this interval
    Buckets::iterator bitr = std::find_if(s_Buckets.begin(), s_Buckets.end(), IntrvFunc(interval));
    // Was there a bucket with this interval?
    if (bitr == s_Buckets.end())
    {
        return; // Nothing to detach from!
    }
    // Attempt to find this routine in the associated bucket
    Routines::iterator ritr = std::find(bitr->mRoutines.begin(), bitr->mRoutines.end(), routine);
    // Was this routine even attached?
    if (ritr != bitr->mRoutines.end())
    {
        bitr->mRoutines.erase(ritr); // Then erase it and move on
    }
    // Any reason to keep this bucket?
    if (bitr->mRoutines.empty())
    {
        s_Buckets.erase(bitr); // Remove the bucket as well
    }
}

// ------------------------------------------------------------------------------------------------
Object Routine::Associate(Routine * routine)
{
    // Attempt to see if this instance already exists in the pool
    Objects::iterator itr = s_Objects.find(routine);
    // Is this routine remembered for the first time?
    if (itr == s_Objects.end())
    {
        // Obtain the initial stack size
        const StackGuard sg;
        // Push this instance on the stack
        ClassType< Routine >::PushInstance(DefaultVM::Get(), routine);
        // Initialize this element into the pool
        itr = s_Objects.emplace(routine, Var< Object >(DefaultVM::Get(), -1).value).first;
        // If the iterator still points to a null element then we failed
        if (itr == s_Objects.end())
        {
            SqThrowF("Unable to remember routine instance");
        }
    }
    // Does this routine still keep a strong reference to it self?
    else if (itr->second.IsNull())
    {
        // Obtain the initial stack size
        const StackGuard sg;
        // Push this instance on the stack
        ClassType< Routine >::PushInstance(DefaultVM::Get(), routine);
        // Obtain a strong reference to it
        itr->second = Var< Object >(DefaultVM::Get(), -1).value;
    }
    // Return the object that we have
    return itr->second;
}

// ------------------------------------------------------------------------------------------------
void Routine::Dissociate(Routine * routine)
{
    // Attempt to see if this instance already exists in the pool
    Objects::iterator itr = s_Objects.find(routine);
    // Was this routine even stored in the pool?
    if (itr != s_Objects.end() && !itr->second.IsNull())
    {
        itr->second.Release(); // Release the reference to self
    }
}

// ------------------------------------------------------------------------------------------------
bool Routine::Associated(Routine * routine)
{
    // Attempt to see if this instance already exists in the pool
    Objects::iterator itr = s_Objects.find(routine);
    // Return whether this routine is pooled and references itself
    return (itr != s_Objects.end() && !itr->second.IsNull());
}

// ------------------------------------------------------------------------------------------------
void Routine::Forget(Routine * routine)
{
    s_Objects.erase(routine);
}

// ------------------------------------------------------------------------------------------------
void Routine::ProcQueue()
{
    // Do we have any queued commands that must be performed when unlocked?
    if (s_Queue.empty() || s_Lock)
    {
        return; // We're done here!
    }
    // Process all commands in the queue
    for (const auto & cmd : s_Queue)
    {
        // Are we supposed to detach the associated routine?
        if (cmd.mCommand == CMD_DETACH)
        {
            // Detach the routine from it's associated bucket first
            Detach(cmd.mRoutine, cmd.mInterval);
            // Break association to allow the instance to be destroyed
            Dissociate(cmd.mRoutine);
        }
        // Are we supposed to attach the associated routine?
        else if (cmd.mCommand == CMD_ATTACH)
        {
            // Attach the routine to it's associated bucket first
            Attach(cmd.mRoutine, cmd.mInterval);
            // Prevent destruction of this routine while buckets are locked
            Associate(cmd.mRoutine);
        }
    }
    // Clear processed commands
    s_Queue.clear();
}

// ------------------------------------------------------------------------------------------------
void Routine::Process()
{
    // In case an exception prevented the unlock last time
    s_Lock = false;
    // Normally there shouldn't be any but just in case the above happened
    ProcQueue();
    // Is this the first call?
    if (s_Last == 0)
    {
        s_Last = GetCurrentSysTime();
        // We'll do it text time
        return;
    }
    // Lock the buckets
    s_Lock = true;
    // Backup the last known time-stamp
    s_Prev = s_Last;
    // Get the current time-stamp
    s_Last = GetCurrentSysTime();
    // Calculate the elapsed time
    Int32 delta = Int32((s_Last - s_Prev) / 1000L);
    // Process all available buckets
    for (auto & bucket : s_Buckets)
    {
        // Update the bucket elapsed time
        bucket.mElapsed += delta;
        // Have we completed the bucket interval?
        if (bucket.mElapsed < bucket.mInterval)
        {
            continue; // Move to the next one
        }
        // Attempt to execute bucket routines, if any
        for (auto & routine : bucket.mRoutines)
        {
            routine->Execute();
        }
        // Reset the bucket elapsed time
        bucket.mElapsed = 0;
    }
    // Unlock the buckets
    s_Lock = false;
    // Process operations that couldn't be performed while buckets were locked
    ProcQueue();
}

// ------------------------------------------------------------------------------------------------
void Routine::TerminateAll()
{
    // Let's make sure no pending commands are left
    ProcQueue();
    // Process all buckets
    for (auto & bucket : s_Buckets)
    {
        //  Process all routines in this bucket
        for (auto & routine : bucket.mRoutines)
        {
            // Release all resources
            routine->Release();
            // Mark it as terminated
            routine->m_Terminated = true;
        }
    }
    // Clear all references to routines
    s_Buckets.clear();
    // Clear all routine instance associations
    s_Objects.clear();
    // Clear the last time-stamp in case of a reload
    s_Last = 0;
}

// ------------------------------------------------------------------------------------------------
Routine::Routine(Object & env, Function & func, Interval interval)
    : m_Iterations(0)
    , m_Interval(interval)
    , m_Arguments(0)
    , m_Suspended(false)
    , m_Terminated(false)
    , m_Callback(env.GetVM(), env, func.GetFunc())
    , m_Tag()
    , m_Data()
{
    Create();
}

Routine::Routine(Object & env, Function & func, Interval interval, Iterate iterations)
    : m_Iterations(iterations)
    , m_Interval(interval)
    , m_Arguments(0)
    , m_Suspended(false)
    , m_Terminated(false)
    , m_Callback(env.GetVM(), env, func.GetFunc())
    , m_Tag()
    , m_Data()
{
   Create();
}

// ------------------------------------------------------------------------------------------------
Routine::Routine(Object & env, Function & func, Interval interval, Iterate iterations
        , Object & a1)
    : m_Iterations(iterations)
    , m_Interval(interval)
    , m_Arguments(1)
    , m_Suspended(false)
    , m_Terminated(false)
    , m_Callback(env.GetVM(), env, func.GetFunc())
    , m_Tag()
    , m_Data()
    , m_Arg1(a1)
{
   Create();
}

// ------------------------------------------------------------------------------------------------
Routine::Routine(Object & env, Function & func, Interval interval, Iterate iterations
        , Object & a1, Object & a2)
    : m_Iterations(iterations)
    , m_Interval(interval)
    , m_Arguments(2)
    , m_Suspended(false)
    , m_Terminated(false)
    , m_Callback(env.GetVM(), env, func.GetFunc())
    , m_Tag()
    , m_Data()
    , m_Arg1(a1), m_Arg2(a2)
{
   Create();
}

// ------------------------------------------------------------------------------------------------
Routine::Routine(Object & env, Function & func, Interval interval, Iterate iterations
        , Object & a1, Object & a2, Object & a3)
    : m_Iterations(iterations)
    , m_Interval(interval)
    , m_Arguments(3)
    , m_Suspended(false)
    , m_Terminated(false)
    , m_Callback(env.GetVM(), env, func.GetFunc())
    , m_Tag()
    , m_Data()
    , m_Arg1(a1), m_Arg2(a2), m_Arg3(a3)
{
   Create();
}

// ------------------------------------------------------------------------------------------------
Routine::Routine(Object & env, Function & func, Interval interval, Iterate iterations
        , Object & a1, Object & a2, Object & a3, Object & a4)
    : m_Iterations(iterations)
    , m_Interval(interval)
    , m_Arguments(4)
    , m_Suspended(false)
    , m_Terminated(false)
    , m_Callback(env.GetVM(), env, func.GetFunc())
    , m_Tag()
    , m_Data()
    , m_Arg1(a1), m_Arg2(a2), m_Arg3(a3), m_Arg4(a4)
{
   Create();
}

// ------------------------------------------------------------------------------------------------
Routine::Routine(Object & env, Function & func, Interval interval, Iterate iterations
        , Object & a1, Object & a2, Object & a3, Object & a4, Object & a5)
    : m_Iterations(iterations)
    , m_Interval(interval)
    , m_Arguments(5)
    , m_Suspended(false)
    , m_Terminated(false)
    , m_Callback(env.GetVM(), env, func.GetFunc())
    , m_Tag()
    , m_Data()
    , m_Arg1(a1), m_Arg2(a2), m_Arg3(a3), m_Arg4(a4), m_Arg5(a5)
{
   Create();
}

// ------------------------------------------------------------------------------------------------
Routine::~Routine()
{
    // Remove this instance from the pool
    Forget(this);
    // Was the routine already terminated?
    if (m_Terminated)
    {
        return; // Nothing to release!
    }
    // Detach from the associated bucket
    Detach();
    // Release script resources
    Release();
}

// ------------------------------------------------------------------------------------------------
Int32 Routine::Cmp(const Routine & o) const
{
    if (m_Interval == o.m_Interval)
        return 0;
    else if (m_Interval > o.m_Interval)
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
CSStr Routine::ToString() const
{
    return ToStrF(_PRINT_INT_FMT, m_Interval);
}

// ------------------------------------------------------------------------------------------------
const String & Routine::GetTag() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
void Routine::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

// ------------------------------------------------------------------------------------------------
Object & Routine::GetData()
{
    // Validate the routine lifetime
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void Routine::SetData(Object & data)
{
    // Validate the routine lifetime
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
Routine & Routine::ApplyTag(CSStr tag)
{
    m_Tag.assign(tag);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Routine & Routine::ApplyData(Object & data)
{
    // Validate the routine lifetime
    Validate();
    // Apply the specified value
    m_Data = data;
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
void Routine::Terminate()
{
    // Was the routine already terminated?
    if (m_Terminated)
    {
        SqThrowF("Routine [%s] => Was already terminated", m_Tag.c_str());
    }
    // Detach from the associated bucket
    Detach();
    // Release script resources and mark it as terminated
    Release();
}

// ------------------------------------------------------------------------------------------------
Routine & Routine::SetArg(Uint8 num, Object & val)
{
    // Validate the routine lifetime
    Validate();
    // Identify which argument was requested
    switch (num)
    {
        case 1: m_Arg1 = val; break;
        case 2: m_Arg2 = val; break;
        case 3: m_Arg3 = val; break;
        case 4: m_Arg4 = val; break;
        case 5: m_Arg5 = val; break;
        case 6: m_Arg6 = val; break;
        case 7: m_Arg7 = val; break;
        case 8: m_Arg8 = val; break;
        case 9: m_Arg9 = val; break;
        case 10: m_Arg10 = val; break;
        case 11: m_Arg11 = val; break;
        case 12: m_Arg12 = val; break;
        case 13: m_Arg13 = val; break;
        case 14: m_Arg14 = val; break;
        default: SqThrowF("Routine [%s] => Argument is out of range: %d", m_Tag.c_str(), num);
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Object & Routine::GetArg(Uint8 num)
{
    // Validate the routine lifetime
    Validate();
    // Identify which argument was requested
    switch (num)
    {
        case 1: return m_Arg1;
        case 2: return m_Arg2;
        case 3: return m_Arg3;
        case 4: return m_Arg4;
        case 5: return m_Arg5;
        case 6: return m_Arg6;
        case 7: return m_Arg7;
        case 8: return m_Arg8;
        case 9: return m_Arg9;
        case 10: return m_Arg10;
        case 11: return m_Arg11;
        case 12: return m_Arg12;
        case 13: return m_Arg13;
        case 14: return m_Arg14;
        default: SqThrowF("Routine [%s] => Argument is out of range: %d", m_Tag.c_str(), num);
    }
    // Shouldn't really reach this point
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
Routine::Interval Routine::GetInterval() const
{
    return m_Interval;
}

// ------------------------------------------------------------------------------------------------
void Routine::SetInterval(Interval interval)
{
    // Validate the routine lifetime
    Validate();
    // Is the specified interval valid?
    if (!interval)
    {
        SqThrowF("Routine [%s] => Invalid interval", m_Tag.c_str());
    }
    // Detach from the current bucket
    Detach();
    // Update the interval
    m_Interval = interval;
    // Attach to the new bucket
    Attach();
}

// ------------------------------------------------------------------------------------------------
Routine::Iterate Routine::GetIterations() const
{
    return m_Iterations;
}

// ------------------------------------------------------------------------------------------------
void Routine::SetIterations(Iterate iterations)
{
    // Validate the routine lifetime
    Validate();
    // Perform the requested operation
    m_Iterations = iterations;
}

// ------------------------------------------------------------------------------------------------
Uint8 Routine::GetArguments() const
{
    return m_Arguments;
}

void Routine::SetArguments(Uint8 num)
{
    // Validate the routine lifetime
    Validate();
    // Is the specified argument count valid?
    if (num > 14)
    {
        SqThrowF("Routine [%s] => Argument is out of range: %d", m_Tag.c_str(), num);
    }
    // Perform the requested operation
    m_Arguments = num;
}

// ------------------------------------------------------------------------------------------------
bool Routine::GetSuspended() const
{
    return m_Suspended;
}

// ------------------------------------------------------------------------------------------------
void Routine::SetSuspended(bool toggle)
{
    // Validate the routine lifetime
    Validate();
    // Perform the requested operation
    m_Suspended = toggle;
}

// ------------------------------------------------------------------------------------------------
bool Routine::GetTerminated() const
{
    return m_Terminated;
}

// ------------------------------------------------------------------------------------------------
Function & Routine::GetCallback()
{
    // Validate the routine lifetime
    Validate();
    // Return the requested information
    return m_Callback;
}

// ------------------------------------------------------------------------------------------------
void Routine::SetCallback(Object & env, Function & func)
{
    // Validate the routine lifetime
    Validate();
    // Perform the requested operation
    m_Callback = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
void Routine::Release()
{
    // Was the routine already terminated?
    if (m_Terminated)
    {
        return; // Nothing to release!
    }
    // Mark it as terminated
    m_Terminated = true;
    // Release the callback
    m_Callback.ReleaseGently();
    // Release the arguments
    m_Arg1.Release();
    m_Arg2.Release();
    m_Arg3.Release();
    m_Arg4.Release();
    m_Arg5.Release();
    m_Arg6.Release();
    m_Arg7.Release();
    m_Arg8.Release();
    m_Arg9.Release();
    m_Arg10.Release();
    m_Arg11.Release();
    m_Arg12.Release();
    m_Arg13.Release();
    m_Arg14.Release();
}

// ------------------------------------------------------------------------------------------------
void Routine::Create()
{
    // Do we even have a valid interval?
    if (!m_Interval)
    {
        SqThrowF("Routine [%s] => Invalid interval", m_Tag.c_str());
    }
    // Is the specified callback even valid?
    else if (m_Callback.IsNull())
    {
        SqThrowF("Routine [%s] => Invalid callback", m_Tag.c_str());
    }
    // Always use the command queue to attach the routine when created
    s_Queue.emplace_back(this, m_Interval, CMD_ATTACH);
}

// ------------------------------------------------------------------------------------------------
void Routine::Attach()
{
    // Do we have a valid interval?
    if (!m_Interval)
    {
        return; // Nothing to attach to!
    }
    // Are the buckets locked?
    else if (s_Lock)
    {
        // Queue a command to attach this routine when the bucket is unlocked
        s_Queue.emplace_back(this, m_Interval, CMD_ATTACH);
    }
    // Attempt to attach the the routine now
    else
    {
        // Attach to the associated bucket
        Attach(this, m_Interval);
        // Associate the instance with it's script object to prevent unexpected release
        Associate(this);
    }
}

// ------------------------------------------------------------------------------------------------
void Routine::Detach()
{
    // Do we have a valid interval?
    if (!m_Interval)
    {
        return; // Nothing to detach from!
    }
    // Are the buckets locked?
    else if (s_Lock)
    {
        // Queue a command to detach this routine when the bucket is unlocked
        s_Queue.emplace_back(this, m_Interval, CMD_DETACH);
    }
    // Attempt to detach the the routine now
    else
    {
        // Detach from the associated bucket
        Detach(this, m_Interval);
        // Break association to allow the instance to be released
        Dissociate(this);
    }
}

// ------------------------------------------------------------------------------------------------
void Routine::Execute()
{
    // Is this routine suspended or has nothing to call?
    if (m_Suspended || m_Callback.IsNull())
    {
        return; // We're done here!
    }
    // Make sure that we have a known number of arguments
    else if (m_Arguments > 14)
    {
        SqThrowF("Routine [%s] => Out of range argument count: %d", m_Tag.c_str(), m_Arguments);
    }
    // Attempt to forward the call
    try
    {
        switch (m_Arguments)
        {
            // Attempt to identify how many arguments should be passed
            case 0:
                m_Callback.Execute();
            break;
            case 1:
                m_Callback.Execute(m_Arg1);
            break;
            case 2:
                m_Callback.Execute(m_Arg1, m_Arg2);
            break;
            case 3:
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3);
            break;
            case 4:
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4);
            break;
            case 5:
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5);
            break;
            case 6:
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6);
            break;
            case 7:
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7);
            break;
            case 8:
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8);
            break;
            case 9:
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9);
            break;
            case 10:
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9, m_Arg10);
            break;
            case 11:
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9, m_Arg10, m_Arg11);
            break;
            case 12:
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9, m_Arg10, m_Arg11, m_Arg12);
            break;
            case 13:
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9, m_Arg10, m_Arg11, m_Arg12, m_Arg13);
            break;
            case 14:
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9, m_Arg10, m_Arg11, m_Arg12, m_Arg13, m_Arg14);
            break;
        }
    }
    catch (const Sqrat::Exception & e)
    {
        SqThrowF("Routine [%s] => Squirrel error: %s", m_Tag.c_str(), e.Message().c_str());
    }
    catch (const std::exception & e)
    {
        SqThrowF("Routine [%s] => Program error: %s", m_Tag.c_str(), e.what());
    }
    catch (...)
    {
        SqThrowF("Routine [%s] => Unknown error", m_Tag.c_str());
    }
    // Decrease the number of iterations if necessary
    if (m_Iterations && (--m_Iterations) == 0)
    {
        Terminate(); // This routine reached the end of it's life
    }
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to process routines.
*/
void ProcessRoutine()
{
    Routine::Process();
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to terminate routines.
*/
void TerminateRoutine()
{
    Routine::TerminateAll();
}

// ------------------------------------------------------------------------------------------------
Object Routine::Create(Object & env, Function & func, Interval interval)
{
    return Associate(new Routine(env, func, interval));
}

// ------------------------------------------------------------------------------------------------
Object Routine::Create(Object & env, Function & func, Interval interval, Iterate iterations)
{
    return Associate(new Routine(env, func, interval, iterations));
}

// ------------------------------------------------------------------------------------------------
Object Routine::Create(Object & env, Function & func, Interval interval, Iterate iterations
                        , Object & a1)
{
    return Associate(new Routine(env, func, interval, iterations, a1));
}

// ------------------------------------------------------------------------------------------------
Object Routine::Create(Object & env, Function & func, Interval interval, Iterate iterations
                        , Object & a1, Object & a2)
{
    return Associate(new Routine(env, func, interval, iterations, a1, a2));
}

// ------------------------------------------------------------------------------------------------
Object Routine::Create(Object & env, Function & func, Interval interval, Iterate iterations
                        , Object & a1, Object & a2, Object & a3)
{
    return Associate(new Routine(env, func, interval, iterations, a1, a2, a3));
}

// ------------------------------------------------------------------------------------------------
Object Routine::Create(Object & env, Function & func, Interval interval, Iterate iterations
                        , Object & a1, Object & a2, Object & a3, Object & a4)
{
    return Associate(new Routine(env, func, interval, iterations, a1, a2, a3, a4));
}

// ------------------------------------------------------------------------------------------------
Object Routine::Create(Object & env, Function & func, Interval interval, Iterate iterations
                        , Object & a1, Object & a2, Object & a3, Object & a4, Object & a5)
{
    return Associate(new Routine(env, func, interval, iterations, a1, a2, a3, a4, a5));
}

// ------------------------------------------------------------------------------------------------
Uint32 Routine::Count()
{
    return static_cast< Uint32 >(s_Objects.size());
}

// ------------------------------------------------------------------------------------------------
Object Routine::FindByTag(CSStr tag)
{
    // Perform a validity check on the specified tag
    if (!tag || *tag == '\0')
    {
        SqThrowF("The specified routine tag is invalid: null/empty");
    }
    // Process each routine in the pool
    for (const auto & elem : s_Objects)
    {
        // Is this routine valid (should always be) and does the tag match the specified one?
        if (elem.first != nullptr && elem.first->m_Tag.compare(tag) == 0)
        {
            // Do we need to obtain the script object again?
            if (elem.second.IsNull())
            {
                // Obtain the initial stack size
                const StackGuard sg;
                // Push this instance on the stack
                ClassType< Routine >::PushInstance(DefaultVM::Get(), elem.first);
                // Obtain a strong reference to it and return it
                return Var< Object >(DefaultVM::Get(), -1).value;
            }
            // Stop searching and return this routine
            return elem.second;
        }
    }
    // Unable to locate a routine matching the specified tag
    return NullObject();
}

// ================================================================================================
void Register_Routine(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqRoutine"),
        Class< Routine, NoConstructor< Routine > >(vm, _SC("SqRoutine"))
        /* Metamethods */
        .Func(_SC("_cmp"), &Routine::Cmp)
        .SquirrelFunc(_SC("_typename"), &Routine::Typename)
        .Func(_SC("_tostring"), &Routine::ToString)
        /* Properties */
        .Prop(_SC("Tag"), &Routine::GetTag, &Routine::SetTag)
        .Prop(_SC("Data"), &Routine::GetData, &Routine::SetData)
        .Prop(_SC("Interval"), &Routine::GetInterval, &Routine::SetInterval)
        .Prop(_SC("Iterations"), &Routine::GetIterations, &Routine::SetIterations)
        .Prop(_SC("Arguments"), &Routine::GetArguments, &Routine::SetArguments)
        .Prop(_SC("Suspended"), &Routine::GetSuspended, &Routine::SetSuspended)
        .Prop(_SC("Terminated"), &Routine::GetTerminated)
        .Prop(_SC("Callback"), &Routine::GetCallback)
        /* Functions */
        .Func(_SC("SetTag"), &Routine::ApplyTag)
        .Func(_SC("SetData"), &Routine::ApplyData)
        .Func(_SC("Terminate"), &Routine::Terminate)
        .Func(_SC("Bind"), &Routine::SetCallback)
        .Func(_SC("GetArg"), &Routine::GetArg)
        .Func(_SC("SetArg"), &Routine::SetArg)
        // Static Functions
        .StaticFunc(_SC("Count"), &Routine::Count)
        .StaticFunc(_SC("FindByTag"), &Routine::FindByTag)
        // Static Overloads
        .StaticOverload< Object (*)(Object &, Function &, Routine::Interval) >
            (_SC("Create"), &Routine::Create)
        .StaticOverload< Object (*)(Object &, Function &, Routine::Interval, Routine::Iterate) >
            (_SC("Create"), &Routine::Create)
        .StaticOverload< Object (*)(Object &, Function &, Routine::Interval, Routine::Iterate,
                                        Object &) >
            (_SC("Create"), &Routine::Create)
        .StaticOverload< Object (*)(Object &, Function &, Routine::Interval, Routine::Iterate,
                                        Object &, Object &) >
            (_SC("Create"), &Routine::Create)
        .StaticOverload< Object (*)(Object &, Function &, Routine::Interval, Routine::Iterate,
                                        Object &, Object &, Object &) >
            (_SC("Create"), &Routine::Create)
        .StaticOverload< Object (*)(Object &, Function &, Routine::Interval, Routine::Iterate,
                                        Object &, Object &, Object &, Object &) >
            (_SC("Create"), &Routine::Create)
        .StaticOverload< Object (*)(Object &, Function &, Routine::Interval, Routine::Iterate,
                                        Object &, Object &, Object &, Object &, Object &) >
            (_SC("Create"), &Routine::Create)
    );
}

} // Namespace:: SqMod
