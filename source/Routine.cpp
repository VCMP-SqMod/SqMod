// ------------------------------------------------------------------------------------------------
#include "Routine.hpp"
#include "Library/Chrono.hpp"

// ------------------------------------------------------------------------------------------------
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Routine::Time       Routine::s_Last = 0;
Routine::Time       Routine::s_Prev = 0;
Routine::Routines   Routine::s_Routines;
Routine::Objects    Routine::s_Objects;

// ------------------------------------------------------------------------------------------------
SQInteger Routine::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqRoutine");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

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
    for (auto & r : s_Routines)
    {
        // Is this routine valid?
        if (!r.second)
        {
            continue;
        }
        // Decrease the elapsed time
        r.first -= delta;
        // Have we completed the routine interval?
        if (r.first <= 0)
        {
            // Reset the elapsed time
            r.first = r.second->m_Interval;
            // Execute the routine
            r.second->Execute();
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Routine::Initialize()
{
    // Clear all slots
    s_Routines.fill(Element(0, nullptr));
}

// ------------------------------------------------------------------------------------------------
void Routine::Deinitialize()
{
    // Process all routine instances
    for (auto & r : s_Objects)
    {
        // Release all resources
        r.first->Release();
        // Mark it as terminated
        r.first->m_Terminated = true;
    }
    // Release all references
    s_Objects.clear();
    // Clear all slots
    s_Routines.fill(Element(0, nullptr));
    // Clear the last time-stamp in case of a reload
    s_Last = 0;
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
            // NOTE: Routine instance is destroyed by the script object if necessary!
            STHROWF("Unable to remember routine instance");
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
void Routine::Insert(Routine * routine, bool associate)
{
    // Do we even have what to insert?
    if (!routine)
    {
        return;
    }
    // See if this routine is already inserted
    Routines::iterator itr = std::find_if(s_Routines.begin(), s_Routines.end(),
        [routine](Element & e) -> bool { return (e.second == routine); });
    // Is this instance already inserted?
    if (itr != s_Routines.end())
    {
        return; // We have what we came here for!
    }
    // Attempt to locate an unused routine slot
    itr = std::find_if(s_Routines.begin(), s_Routines.end(),
        [](Element & e) -> bool { return (e.second == nullptr); });
    // Are there any slots available?
    if (itr == s_Routines.end())
    {
        STHROWF("Maximum number of active routines was reached: %d", SQMOD_MAX_ROUTINES);
    }
    // Insert the routine at the obtained slot
    itr->first = routine->m_Interval;
    itr->second = routine;
    // Associate this slot with the routine instance
    routine->m_Slot = std::distance(s_Routines.begin(), itr);
    // Keep a string reference to this routine instance, if requested
    if (associate)
    {
        Associate(routine);
    }
}

// ------------------------------------------------------------------------------------------------
void Routine::Remove(Routine * routine)
{
    // Do we even have what to remove?
    if (!routine)
    {
        return;
    }
    Routines::iterator itr;
    // Make sure the routine wasn't activated multiple time (paranoia)
    do {
        // Search for the slot with the specified routine instance
        itr = std::find_if(s_Routines.begin(), s_Routines.end(),
            [routine](Element & e) -> bool { return (e.second == routine); });
        // Was this routine instance activated?
        if (itr != s_Routines.end())
        {
            itr->first = 0;
            itr->second = nullptr;
        }
    } while (itr != s_Routines.end());
    // Reset the routine instance slot
    routine->m_Slot = 0xFFFF;
    // Release any strong reference to this routine instance
    Dissociate(routine);
}

// ------------------------------------------------------------------------------------------------
void Routine::Release()
{
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
        STHROWF("Routine [%s] => Out of range argument count [%d]", m_Tag.c_str(), m_Arguments);
    }
    // Attempt to forward the call
    try
    {
        switch (m_Arguments)
        {
            // Attempt to identify how many arguments should be passed
            case 0:
            {
                m_Callback.Execute();
            } break;
            case 1:
            {
                m_Callback.Execute(m_Arg1);
            } break;
            case 2:
            {
                m_Callback.Execute(m_Arg1, m_Arg2);
            } break;
            case 3:
            {
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3);
            } break;
            case 4:
            {
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4);
            } break;
            case 5:
            {
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5);
            } break;
            case 6:
            {
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6);
            } break;
            case 7:
            {
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7);
            } break;
            case 8:
            {
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8);
            } break;
            case 9:
            {
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9);
            } break;
            case 10:
            {
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9, m_Arg10);
            } break;
            case 11:
            {
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9, m_Arg10, m_Arg11);
            } break;
            case 12:
            {
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9, m_Arg10, m_Arg11, m_Arg12);
            } break;
            case 13:
            {
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9, m_Arg10, m_Arg11, m_Arg12, m_Arg13);
            } break;
            case 14:
            {
                m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9, m_Arg10, m_Arg11, m_Arg12, m_Arg13, m_Arg14);
            } break;
        }
    }
    catch (const Sqrat::Exception & e)
    {
        LogErr("Routine [%s] => Squirrel error [%s]", m_Tag.c_str(), e.what());
    }
    catch (const std::exception & e)
    {
        LogErr("Routine [%s] => Program error [%s]", m_Tag.c_str(), e.what());
    }
    catch (...)
    {
        LogErr("Routine [%s] => Unknown error", m_Tag.c_str());
    }
    // Decrease the number of iterations if necessary
    if (m_Iterations && (--m_Iterations) == 0)
    {
        Terminate(); // This routine reached the end of it's life
    }
}
// ------------------------------------------------------------------------------------------------
Routine::Routine(Object & env, Function & func, Interval interval)
    : m_Iterations(0)
    , m_Interval(interval)
    , m_Slot(0xFFFF)
    , m_Arguments(0)
    , m_Suspended(false)
    , m_Terminated(false)
    , m_Callback()
    , m_Tag(_SC(""))
    , m_Data()
{
    // Make sure the interval is good
    if (m_Interval < 0)
    {
        STHROWF("Invalid routine interval");
    }
    else
    {
        // Does the callback need a custom environment?
        if (env.IsNull())
        {
            m_Callback = func;
        }
        else
        {
            m_Callback = Function(env.GetVM(), env, func.GetFunc());
        }
        // Activate this routine
        Insert(this, false);
    }
}

Routine::Routine(Object & env, Function & func, Interval interval, Iterator iterations)
    : m_Iterations(iterations)
    , m_Interval(interval)
    , m_Slot(0xFFFF)
    , m_Arguments(0)
    , m_Suspended(false)
    , m_Terminated(false)
    , m_Callback()
    , m_Tag(_SC(""))
    , m_Data()
{
    // Make sure the interval is good
    if (m_Interval < 0)
    {
        STHROWF("Invalid routine interval");
    }
    else
    {
        // Does the callback need a custom environment?
        if (env.IsNull())
        {
            m_Callback = func;
        }
        else
        {
            m_Callback = Function(env.GetVM(), env, func.GetFunc());
        }
        // Activate this routine
        Insert(this, false);
    }
}

// ------------------------------------------------------------------------------------------------
Routine::Routine(Object & env, Function & func, Interval interval, Iterator iterations
        , Object & a1)
    : m_Iterations(iterations)
    , m_Interval(interval)
    , m_Slot(0xFFFF)
    , m_Arguments(1)
    , m_Suspended(false)
    , m_Terminated(false)
    , m_Callback()
    , m_Tag(_SC(""))
    , m_Data()
    , m_Arg1(a1)
{
    // Make sure the interval is good
    if (m_Interval < 0)
    {
        STHROWF("Invalid routine interval");
    }
    else
    {
        // Does the callback need a custom environment?
        if (env.IsNull())
        {
            m_Callback = func;
        }
        else
        {
            m_Callback = Function(env.GetVM(), env, func.GetFunc());
        }
        // Activate this routine
        Insert(this, false);
    }
}

// ------------------------------------------------------------------------------------------------
Routine::Routine(Object & env, Function & func, Interval interval, Iterator iterations
        , Object & a1, Object & a2)
    : m_Iterations(iterations)
    , m_Interval(interval)
    , m_Slot(0xFFFF)
    , m_Arguments(2)
    , m_Suspended(false)
    , m_Terminated(false)
    , m_Callback()
    , m_Tag(_SC(""))
    , m_Data()
    , m_Arg1(a1), m_Arg2(a2)
{
    // Make sure the interval is good
    if (m_Interval < 0)
    {
        STHROWF("Invalid routine interval");
    }
    else
    {
        // Does the callback need a custom environment?
        if (env.IsNull())
        {
            m_Callback = func;
        }
        else
        {
            m_Callback = Function(env.GetVM(), env, func.GetFunc());
        }
        // Activate this routine
        Insert(this, false);
    }
}

// ------------------------------------------------------------------------------------------------
Routine::Routine(Object & env, Function & func, Interval interval, Iterator iterations
        , Object & a1, Object & a2, Object & a3)
    : m_Iterations(iterations)
    , m_Interval(interval)
    , m_Slot(0xFFFF)
    , m_Arguments(3)
    , m_Suspended(false)
    , m_Terminated(false)
    , m_Callback()
    , m_Tag(_SC(""))
    , m_Data()
    , m_Arg1(a1), m_Arg2(a2), m_Arg3(a3)
{
    // Make sure the interval is good
    if (m_Interval < 0)
    {
        STHROWF("Invalid routine interval");
    }
    else
    {
        // Does the callback need a custom environment?
        if (env.IsNull())
        {
            m_Callback = func;
        }
        else
        {
            m_Callback = Function(env.GetVM(), env, func.GetFunc());
        }
        // Activate this routine
        Insert(this, false);
    }
}

// ------------------------------------------------------------------------------------------------
Routine::Routine(Object & env, Function & func, Interval interval, Iterator iterations
        , Object & a1, Object & a2, Object & a3, Object & a4)
    : m_Iterations(iterations)
    , m_Interval(interval)
    , m_Slot(0xFFFF)
    , m_Arguments(4)
    , m_Suspended(false)
    , m_Terminated(false)
    , m_Callback()
    , m_Tag(_SC(""))
    , m_Data()
    , m_Arg1(a1), m_Arg2(a2), m_Arg3(a3), m_Arg4(a4)
{
    // Make sure the interval is good
    if (m_Interval < 0)
    {
        STHROWF("Invalid routine interval");
    }
    else
    {
        // Does the callback need a custom environment?
        if (env.IsNull())
        {
            m_Callback = func;
        }
        else
        {
            m_Callback = Function(env.GetVM(), env, func.GetFunc());
        }
        // Activate this routine
        Insert(this, false);
    }
}

// ------------------------------------------------------------------------------------------------
Routine::Routine(Object & env, Function & func, Interval interval, Iterator iterations
        , Object & a1, Object & a2, Object & a3, Object & a4, Object & a5)
    : m_Iterations(iterations)
    , m_Interval(interval)
    , m_Slot(0xFFFF)
    , m_Arguments(5)
    , m_Suspended(false)
    , m_Terminated(false)
    , m_Callback()
    , m_Tag(_SC(""))
    , m_Data()
    , m_Arg1(a1), m_Arg2(a2), m_Arg3(a3), m_Arg4(a4), m_Arg5(a5)
{
    // Make sure the interval is good
    if (m_Interval < 0)
    {
        STHROWF("Invalid routine interval");
    }
    else
    {
        // Does the callback need a custom environment?
        if (env.IsNull())
        {
            m_Callback = func;
        }
        else
        {
            m_Callback = Function(env.GetVM(), env, func.GetFunc());
        }
        // Activate this routine
        Insert(this, false);
    }
}

// ------------------------------------------------------------------------------------------------
Routine::~Routine()
{
    // Remove this instance from the pool
    Forget(this);
    // Was the routine already terminated?
    if (!m_Terminated)
    {
        return; // Nothing to release!
    }
    // Remove it from the pool
    Remove(this);
    // Release script resources
    Release();
}

// ------------------------------------------------------------------------------------------------
Int32 Routine::Cmp(const Routine & o) const
{
    if (m_Interval == o.m_Interval)
    {
        return 0;
    }
    else if (m_Interval > o.m_Interval)
    {
        return 1;
    }
    else
    {
        return -1;
    }
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
    m_Tag.assign(tag ? tag : _SC(""));
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
    m_Tag.assign(tag ? tag : _SC(""));
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
Routine & Routine::Activate()
{
    // Validate the routine lifetime
    Validate();
    // Make sure the interval is good
    if (m_Interval < 0)
    {
        STHROWF("Invalid routine interval");
    }
    // Make sure no slot is associated
    else if (m_Slot >= s_Routines.size())
    {
        Insert(this);
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Routine & Routine::Deactivate()
{
    // Validate the routine lifetime
    Validate();
    // Remove it from the pool
    Remove(this);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Routine & Routine::Terminate()
{
    // Was the routine already terminated?
    if (m_Terminated)
    {
        STHROWF("Routine was already terminated");
    }
    // Remove it from the pool
    Remove(this);
    // Release script resources
    Release();
    // Mark it as terminated
    m_Terminated = true;
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Routine & Routine::Reanimate()
{
    // Allow the instance to be used
    m_Terminated = false;
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Routine & Routine::SetArg(Uint16 num, Object & val)
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
        default: STHROWF("Argument is out of range: %d", num);
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Object & Routine::GetArg(Uint16 num)
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
        default: STHROWF("Argument is out of range: %d", num);
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
    if (interval < 0)
    {
        STHROWF("Invalid routine interval");
    }
    // Do we have any slot to update?
    if (m_Slot >= s_Routines.size())
    {
        // Update the interval
        m_Interval = interval;
        // We're done here!
        return;
    }
    // Make changes visible immediately
    if (interval > m_Interval)
    {
        s_Routines[m_Slot].first += (interval - m_Interval);
    }
    else
    {
        s_Routines[m_Slot].first -= (m_Interval - interval);
    }
    // Update the interval
    m_Interval = interval;
}

// ------------------------------------------------------------------------------------------------
Routine::Iterator Routine::GetIterations() const
{
    return m_Iterations;
}

// ------------------------------------------------------------------------------------------------
void Routine::SetIterations(Iterator iterations)
{
    // Validate the routine lifetime
    Validate();
    // Perform the requested operation
    m_Iterations = iterations;
}

// ------------------------------------------------------------------------------------------------
Uint16 Routine::GetArguments() const
{
    return m_Arguments;
}

void Routine::SetArguments(Uint16 num)
{
    // Validate the routine lifetime
    Validate();
    // Is the specified argument count valid?
    if (num > 14)
    {
        STHROWF("Argument is out of range: %d", num);
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
Object Routine::Create(Object & env, Function & func, Interval interval)
{
    return Associate(new Routine(env, func, interval));
}

// ------------------------------------------------------------------------------------------------
Object Routine::Create(Object & env, Function & func, Interval interval, Iterator iterations)
{
    return Associate(new Routine(env, func, interval, iterations));
}

// ------------------------------------------------------------------------------------------------
Object Routine::Create(Object & env, Function & func, Interval interval, Iterator iterations
                        , Object & a1)
{
    return Associate(new Routine(env, func, interval, iterations, a1));
}

// ------------------------------------------------------------------------------------------------
Object Routine::Create(Object & env, Function & func, Interval interval, Iterator iterations
                        , Object & a1, Object & a2)
{
    return Associate(new Routine(env, func, interval, iterations, a1, a2));
}

// ------------------------------------------------------------------------------------------------
Object Routine::Create(Object & env, Function & func, Interval interval, Iterator iterations
                        , Object & a1, Object & a2, Object & a3)
{
    return Associate(new Routine(env, func, interval, iterations, a1, a2, a3));
}

// ------------------------------------------------------------------------------------------------
Object Routine::Create(Object & env, Function & func, Interval interval, Iterator iterations
                        , Object & a1, Object & a2, Object & a3, Object & a4)
{
    return Associate(new Routine(env, func, interval, iterations, a1, a2, a3, a4));
}

// ------------------------------------------------------------------------------------------------
Object Routine::Create(Object & env, Function & func, Interval interval, Iterator iterations
                        , Object & a1, Object & a2, Object & a3, Object & a4, Object & a5)
{
    return Associate(new Routine(env, func, interval, iterations, a1, a2, a3, a4, a5));
}

// ------------------------------------------------------------------------------------------------
Object Routine::FindByTag(CSStr tag)
{
    // Perform a validity check on the specified tag
    if (!tag || *tag == '\0')
    {
        STHROWF("The specified routine tag is invalid: null/empty");
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
    RootTable(vm).Bind(_SC("SqRoutine"),
        Class< Routine, NoConstructor< Routine > >(vm, _SC("SqRoutine"))
        // Meta-methods
        .Func(_SC("_cmp"), &Routine::Cmp)
        .SquirrelFunc(_SC("_typename"), &Routine::Typename)
        .Func(_SC("_tostring"), &Routine::ToString)
        // Properties
        .Prop(_SC("Tag"), &Routine::GetTag, &Routine::SetTag)
        .Prop(_SC("Data"), &Routine::GetData, &Routine::SetData)
        .Prop(_SC("Interval"), &Routine::GetInterval, &Routine::SetInterval)
        .Prop(_SC("Iterations"), &Routine::GetIterations, &Routine::SetIterations)
        .Prop(_SC("Arguments"), &Routine::GetArguments, &Routine::SetArguments)
        .Prop(_SC("Suspended"), &Routine::GetSuspended, &Routine::SetSuspended)
        .Prop(_SC("Terminated"), &Routine::GetTerminated)
        .Prop(_SC("Callback"), &Routine::GetCallback)
        // Functions
        .Func(_SC("SetTag"), &Routine::ApplyTag)
        .Func(_SC("SetData"), &Routine::ApplyData)
        .Func(_SC("Terminate"), &Routine::Terminate)
        .Func(_SC("Bind"), &Routine::SetCallback)
        .Func(_SC("GetArg"), &Routine::GetArg)
        .Func(_SC("SetArg"), &Routine::SetArg)
        // Static Functions
        .StaticFunc(_SC("FindByTag"), &Routine::FindByTag)
        // Static Overloads
        .StaticOverload< Object (*)(Object &, Function &, Routine::Interval) >
            (_SC("Create"), &Routine::Create)
        .StaticOverload< Object (*)(Object &, Function &, Routine::Interval, Routine::Iterator) >
            (_SC("Create"), &Routine::Create)
        .StaticOverload< Object (*)(Object &, Function &, Routine::Interval, Routine::Iterator,
                                        Object &) >
            (_SC("Create"), &Routine::Create)
        .StaticOverload< Object (*)(Object &, Function &, Routine::Interval, Routine::Iterator,
                                        Object &, Object &) >
            (_SC("Create"), &Routine::Create)
        .StaticOverload< Object (*)(Object &, Function &, Routine::Interval, Routine::Iterator,
                                        Object &, Object &, Object &) >
            (_SC("Create"), &Routine::Create)
        .StaticOverload< Object (*)(Object &, Function &, Routine::Interval, Routine::Iterator,
                                        Object &, Object &, Object &, Object &) >
            (_SC("Create"), &Routine::Create)
        .StaticOverload< Object (*)(Object &, Function &, Routine::Interval, Routine::Iterator,
                                        Object &, Object &, Object &, Object &, Object &) >
            (_SC("Create"), &Routine::Create)
    );
}

} // Namespace:: SqMod
