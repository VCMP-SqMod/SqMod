// ------------------------------------------------------------------------------------------------
#include "Misc/Signal.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Signal::SignalContainer Signal::s_Signals;
Signal::FreeSignals     Signal::s_FreeSignals;

// ------------------------------------------------------------------------------------------------
SQInteger Signal::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqSignal");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
void Signal::Terminate()
{
    // Terminate named signals
    for (const auto & s : s_Signals)
    {
        // Clear slots
        s.second.mPtr->Clear();
        // Release the name
        s.second.mPtr->m_Name.clear();
        // Release whatever is in the user data
        s.second.mPtr->m_Data.Release();
    }
    // Finally clear the container itself
    s_Signals.clear();
    // Create a copy so we don't invalidate iterators when destructor removes the instances
    FreeSignals fsig(s_FreeSignals);
    // Terminate anonymous signals
    for (const auto & s : fsig)
    {
        // Clear slots
        s->Clear();
        // Release whatever is in the user data
        s->m_Data.Release();
    }
    // Finally clear the container itself
    s_FreeSignals.clear();
}

// ------------------------------------------------------------------------------------------------
Object Signal::Create()
{
    // Remember the current stack size
    const StackGuard sg;
    // Create the signal instance
    DeleteGuard< Signal > dg(new Signal());
    // Attempt to create the signal instance
    ClassType< Signal >::PushInstance(DefaultVM::Get(), dg.Get());
    // This is now managed by the script
    dg.Release();
    // Return the created signal
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
Object Signal::Create(String name)
{
    // Validate the signal name
    if (name.empty())
    {
        STHROWF("Empty signal names are not allowed");
    }
    // Compute the hash of the specified name
    const std::size_t hash = std::hash< String >{}(name);
    // See if the signal already exists
    for (const auto & e : s_Signals)
    {
        if (e.first == hash)
        {
            return e.second.mObj; // Found a match so let's return it
        }
    }
    // Remember the current stack size
    const StackGuard sg;
    // Create the signal instance
    DeleteGuard< Signal > dg(new Signal(std::move(name)));
    // Grab the signal instance pointer
    Signal * ptr = dg.Get();
    // Attempt to create the signal instance
    ClassType< Signal >::PushInstance(DefaultVM::Get(), ptr);
    // This is now managed by the script
    dg.Release();
    // Grab a reference to the instance created on the stack
    s_Signals.emplace_back(hash, SignalReference(ptr, Var< Object >(DefaultVM::Get(), -1).value));
    // Return the created signal
    return s_Signals.back().second.mObj;
}

// ------------------------------------------------------------------------------------------------
void Signal::Remove(String name)
{
    // Validate the signal name
    if (name.empty())
    {
        STHROWF("Empty signal names are not allowed");
    }
    // Compute the hash of the specified name
    const std::size_t hash = std::hash< String >{}(name);
    // Iterator to the existing signal, if any
    SignalContainer::const_iterator itr = s_Signals.cbegin();
    // Search for a signal with this name
    for (; itr != s_Signals.cend(); ++itr)
    {
        if (itr->first == hash)
        {
            break;
        }
    }
    // Did we find anything?
    if (itr != s_Signals.cend())
    {
        // Clear the name
        itr->second.mPtr->m_Name.clear();
        // Put it on the free list
        s_FreeSignals.push_back(itr->second.mPtr);
        // Finally, remove it from the named list
        s_Signals.erase(itr);
    }
}

// ------------------------------------------------------------------------------------------------
Object Signal::Fetch(String name)
{
    // Validate the signal name
    if (name.empty())
    {
        STHROWF("Empty signal names are not allowed");
    }
    // Compute the hash of the specified name
    const std::size_t hash = std::hash< String >{}(name);
    // Search for a signal with this name
    for (const auto & e : s_Signals)
    {
        if (e.first == hash)
        {
            return e.second.mObj; // Found a match so let's return it
        }
    }
    // No such signal exists
    STHROWF("Unknown signal named (%s)", name.c_str());
    // SHOULD NOT REACH THIS POINT!
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqCreateSignal(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the signal name specified?
    if (top <= 1)
    {
        sq_pushobject(DefaultVM::Get(), Signal::Create());
    }
    else
    {
        // Attempt to generate the string value
        StackStrF val(vm, 2);
        // Have we failed to retrieve the string?
        if (SQ_FAILED(val.mRes))
        {
            return val.mRes; // Propagate the error!
        }
        // Create the signal instance and push it on the stack
        try
        {
            sq_pushobject(DefaultVM::Get(), Signal::Create(String(val.mPtr, val.mLen)));
        }
        catch (const Sqrat::Exception & e)
        {
            return sq_throwerror(vm, e.what());
        }
    }
    // We have an argument on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqRemoveSignal(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the signal name specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing signal name");
    }
    // Attempt to generate the string value
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Create the signal instance and push it on the stack
    try
    {
        Signal::Remove(String(val.mPtr, val.mLen));
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    // We have an argument on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqFetchSignal(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the signal name specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing signal name");
    }
    // Attempt to generate the string value
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Create the signal instance and push it on the stack
    try
    {
        sq_pushobject(DefaultVM::Get(), Signal::Fetch(String(val.mPtr, val.mLen)));
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    // We have an argument on the stack
    return 1;
}

// ================================================================================================
void Register_Signal(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqSignalWrapper"),
        Class< Signal, NoConstructor< Signal > >(vm, _SC("SqSignalWrapper"))
        // Meta-methods
        .Func(_SC("_cmp"), &Signal::Cmp)
        .SquirrelFunc(_SC("_typename"), &Signal::Typename)
        .Func(_SC("_tostring"), &Signal::ToString)
        // Core Properties
        .Prop(_SC("Data"), &Signal::GetData, &Signal::SetData)
        .Prop(_SC("Slots"), &Signal::Count)
        // Core Methods
        .Func(_SC("Clear"), &Signal::Clear)
        .Func(_SC("Connect"), &Signal::Connect)
        .Func(_SC("Connected"), &Signal::Connected)
        .Func(_SC("Disconnect"), &Signal::Disconnect)
        .Func(_SC("EliminateThis"), &Signal::DisconnectThis)
        .Func(_SC("EliminateFunc"), &Signal::DisconnectFunc)
        .Func(_SC("CountThis"), &Signal::CountThis)
        .Func(_SC("CountFunc"), &Signal::CountFunc)
        .Func(_SC("Head"), &Signal::Head)
        .Func(_SC("Tail"), &Signal::Tail)
        // Squirrel Functions
        .SquirrelFunc(_SC("Emit"), &Signal::SqEmit)
        .SquirrelFunc(_SC("Broadcast"), &Signal::SqEmit)
        .SquirrelFunc(_SC("Query"), &Signal::SqQuery)
        .SquirrelFunc(_SC("Consume"), &Signal::SqConsume)
        .SquirrelFunc(_SC("Approve"), &Signal::SqApprove)
        .SquirrelFunc(_SC("Request"), &Signal::SqRequest)
    );

    RootTable(vm)
        .SquirrelFunc(_SC("SqSignal"), &SqFetchSignal)
        .SquirrelFunc(_SC("SqCreateSignal"), &SqCreateSignal)
        .SquirrelFunc(_SC("SqRemoveSignal"), &SqRemoveSignal);
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to terminate the signals.
*/
void TerminateSignals()
{
    Signal::Terminate();
}

} // Namespace:: SqMod
