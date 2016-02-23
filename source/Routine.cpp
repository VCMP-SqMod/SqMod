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

// ------------------------------------------------------------------------------------------------
void Routine::Process()
{
    s_Lock = false; /* In case an exception prevented the unlock last time */
    if (!s_Queue.empty())
    {
        for (Queue::iterator itr = s_Queue.begin(); itr != s_Queue.end(); ++itr)
        {
            if (itr->first && itr->second)
                itr->second->Attach();
            else if (itr->second)
                itr->second->Terminate();
        }
        s_Queue.clear();
    }
    // Is this the first call?
    if (s_Last == 0)
    {
        s_Last = GetCurrentSysTime();
        return;
    }
    s_Lock = true;
    s_Prev = s_Last;
    s_Last = GetCurrentSysTime();
    Int32 delta = Int32((s_Last - s_Prev) / 1000L);
    for (Buckets::iterator bucket = s_Buckets.begin(); bucket != s_Buckets.end(); ++bucket)
    {
        bucket->mElapsed += delta;
        if (bucket->mElapsed < bucket->mInterval)
            continue;
        Routines::iterator itr = bucket->mRoutines.begin();
        Routines::iterator end = bucket->mRoutines.end();
        for (; itr != end; ++itr)
            (*itr)->Execute();
        bucket->mElapsed = 0;
    }
    s_Lock = false;
}

// ------------------------------------------------------------------------------------------------
void Routine::TerminateAll()
{
    for (Buckets::iterator bucket = s_Buckets.begin(); bucket != s_Buckets.end(); ++bucket)
    {
        Routines::iterator itr = bucket->mRoutines.begin();
        Routines::iterator end = bucket->mRoutines.end();
        for (; itr != end; ++itr)
        {
            (*itr)->Release();
            (*itr)->m_Terminated = true;
        }
    }
    // Clear all references
    s_Buckets.clear();
}

// ------------------------------------------------------------------------------------------------
Routine::Routine(Object & env, Function & func, Interval interval)
    : m_Iterations(0)
    , m_Interval(interval)
    , m_Arguments(0)
    , m_Suspended(false)
    , m_Terminated(false)
    , m_Callback(env.GetVM(), env, func.GetFunc())
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
    , m_Arg1(a1), m_Arg2(a2), m_Arg3(a3), m_Arg4(a4), m_Arg5(a5)
{
   Create();
}

// ------------------------------------------------------------------------------------------------
Routine::~Routine()
{
    if (!m_Terminated)
        Terminate();
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

CSStr Routine::ToString() const
{
    return ToStrF(_PRINT_INT_FMT, m_Interval);
}

// ------------------------------------------------------------------------------------------------
void Routine::Terminate()
{
    if (m_Terminated)
        SqThrow("Routine was already terminated");
    else if (s_Lock)
        s_Queue.push_back(Queue::value_type(false, this));
    else
    {
        Detach();
        Release();
        m_Terminated = true;
    }
}

// ------------------------------------------------------------------------------------------------
void Routine::SetArg(Uint8 num, Object & val)
{
    if (m_Terminated)
    {
        SqThrow("Routine was terminated");
        return;
    }

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
        default: SqThrow("Argument is out of range: %d", num);
    }
}

Object & Routine::GetArg(Uint8 num)
{
    if (m_Terminated)
    {
        SqThrow("Routine was terminated");
        return NullObject();
    }

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
        default: SqThrow("Argument is out of range: %d", num);
    }

    return NullObject();
}

// ------------------------------------------------------------------------------------------------
Routine::Interval Routine::GetInterval() const
{
    return m_Interval;
}

void Routine::SetInterval(Interval interval)
{
    if (m_Terminated)
        SqThrow("Routine was terminated");
    else if (!interval)
        SqThrow("Invalid routine interval");
    else
    {
        Detach();
        m_Interval = interval;
        Attach();
    }
}

// ------------------------------------------------------------------------------------------------
Routine::Iterate Routine::GetIterations() const
{
    return m_Iterations;
}

void Routine::SetIterations(Iterate iterations)
{
    if (m_Terminated)
        SqThrow("Routine was terminated");
    else
        m_Iterations = iterations;
}

// ------------------------------------------------------------------------------------------------
Uint8 Routine::GetArguments() const
{
    return m_Arguments;
}

void Routine::SetArguments(Uint8 num)
{
    if (m_Terminated)
        SqThrow("Routine was terminated");
    else if (num > 14)
        SqThrow("Argument is out of range: %d", num);
    else
        m_Arguments = num;
}

// ------------------------------------------------------------------------------------------------
bool Routine::GetSuspended() const
{
    return m_Suspended;
}

void Routine::SetSuspended(bool toggle)
{
    if (m_Terminated)
        SqThrow("Routine was terminated");
    else
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
    if (m_Terminated)
        SqThrow("Routine was terminated");
    else
        return m_Callback;
    return NullFunction();
}

void Routine::SetCallback(Object & env, Function & func)
{
    if (m_Terminated)
        SqThrow("Routine was terminated");
    else
        m_Callback = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
void Routine::Release()
{
    if (m_Terminated)
        return;
    m_Terminated = true;
    m_Callback.ReleaseGently();
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
    if (!m_Interval)
        SqThrow("Invalid routine interval");
    else if (m_Callback.IsNull())
        SqThrow("Invalid routine callback");
    else
    {
        Attach();
        return;
    }
    Release();
}

void Routine::Attach()
{
    if (!m_Interval)
        return;
    else if (s_Lock)
    {
        s_Queue.push_back(Queue::value_type(true, this));
        return;
    }
    Buckets::iterator itr = std::find_if(s_Buckets.begin(), s_Buckets.end(), IntrvFunc(m_Interval));
    if (itr == s_Buckets.end())
    {
        s_Buckets.push_back(Buckets::value_type(m_Interval));
        s_Buckets.back().mRoutines.push_back(this);
    }
    else if (std::find(itr->mRoutines.begin(), itr->mRoutines.end(), this) != itr->mRoutines.end())
        return;
    else
        itr->mRoutines.push_back(this);
}

void Routine::Detach()
{
    if (!m_Interval)
        return;
    else if (s_Lock)
    {
        s_Queue.push_back(Queue::value_type(false, this));
        return;
    }
    Buckets::iterator bitr = std::find_if(s_Buckets.begin(), s_Buckets.end(), IntrvFunc(m_Interval));
    if (bitr == s_Buckets.end())
        return;
    Routines::iterator ritr = std::find(bitr->mRoutines.begin(), bitr->mRoutines.end(), this);
    if (ritr != bitr->mRoutines.end())
        bitr->mRoutines.erase(ritr);
    // Any reason to keep this bucket?
    if (bitr->mRoutines.empty())
        s_Buckets.erase(bitr);
}

// ------------------------------------------------------------------------------------------------
void Routine::Execute()
{
    if (m_Suspended || m_Callback.IsNull())
        return;
    switch (m_Arguments)
    {
        case 0: m_Callback.Execute();
        break;
        case 1: m_Callback.Execute(m_Arg1);
        break;
        case 2: m_Callback.Execute(m_Arg1, m_Arg2);
        break;
        case 3: m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3);
        break;
        case 4: m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4);
        break;
        case 5: m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5);
        break;
        case 6: m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6);
        break;
        case 7: m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7);
        break;
        case 8: m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8);
        break;
        case 9: m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9);
        break;
        case 10: m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9, m_Arg10);
        break;
        case 11: m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9, m_Arg10, m_Arg11);
        break;
        case 12: m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9, m_Arg10, m_Arg11, m_Arg12);
        break;
        case 13: m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9, m_Arg10, m_Arg11, m_Arg12, m_Arg13);
        break;
        case 14: m_Callback.Execute(m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                                    m_Arg8, m_Arg9, m_Arg10, m_Arg11, m_Arg12, m_Arg13, m_Arg14);
        break;
        default:
            SqThrow("Unknown argument count: %d", m_Arguments);
    }

    if (m_Iterations && (--m_Iterations) == 0)
    {
        Terminate();
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

// ================================================================================================
void Register_Routine(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqRoutine"),
        Class< Routine, NoCopy< Routine > >(vm, _SC("SqRoutine"))
        /* Constructors */
        .Ctor< Object &, Function &, Routine::Interval >()
        .Ctor< Object &, Function &, Routine::Interval, Routine::Iterate >()
        .Ctor< Object &, Function &, Routine::Interval, Routine::Iterate
                , Object & >()
        .Ctor< Object &, Function &, Routine::Interval, Routine::Iterate
                , Object &, Object & >()
        .Ctor< Object &, Function &, Routine::Interval, Routine::Iterate
                , Object &, Object &, Object & >()
        .Ctor< Object &, Function &, Routine::Interval, Routine::Iterate
                , Object &, Object &, Object &, Object & >()
        .Ctor< Object &, Function &, Routine::Interval, Routine::Iterate
                , Object &, Object &, Object &, Object &, Object & >()
        /* Metamethods */
        .Func(_SC("_cmp"), &Routine::Cmp)
        .Func(_SC("_tostring"), &Routine::ToString)
        /* Properties */
        .Prop(_SC("Interval"), &Routine::GetInterval, &Routine::SetInterval)
        .Prop(_SC("Iterations"), &Routine::GetIterations, &Routine::SetIterations)
        .Prop(_SC("Arguments"), &Routine::GetArguments, &Routine::SetArguments)
        .Prop(_SC("Suspended"), &Routine::GetSuspended, &Routine::SetSuspended)
        .Prop(_SC("Terminated"), &Routine::GetTerminated)
        .Prop(_SC("Callback"), &Routine::GetCallback)
        /* Functions */
        .Func(_SC("Terminate"), &Routine::Terminate)
        .Func(_SC("Bind"), &Routine::SetCallback)
        .Func(_SC("GetArg"), &Routine::GetArg)
        .Func(_SC("SetArg"), &Routine::SetArg)
    );
}

} // Namespace:: SqMod
