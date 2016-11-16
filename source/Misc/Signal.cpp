// ------------------------------------------------------------------------------------------------
#include "Misc/Signal.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("SqSignalWrapper"))

// ------------------------------------------------------------------------------------------------
Signal::SignalContainer Signal::s_Signals;
Signal::FreeSignals     Signal::s_FreeSignals;

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
Object Signal::CreateFree()
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
Object Signal::Create(StackStrF & name)
{
    // Validate the signal name
    if (name.mLen <= 0)
    {
        return CreateFree();
    }
    // Create a copy of the name
    String sname(name.mPtr, name.mLen);
    // Compute the hash of the specified name
    const std::size_t hash = std::hash< String >{}(sname);
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
    DeleteGuard< Signal > dg(new Signal(std::move(sname)));
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
void Signal::Remove(StackStrF & name)
{
    // Validate the signal name
    if (name.mLen <= 0)
    {
        STHROWF("Signals without names cannot be removed manually");
    }
    // Create a copy of the name
    String sname(name.mPtr, name.mLen);
    // Compute the hash of the specified name
    const std::size_t hash = std::hash< String >{}(sname);
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
Object Signal::Fetch(StackStrF & name)
{
    // Validate the signal name
    if (name.mLen <= 0)
    {
        STHROWF("Signals without names cannot be retrieved manually");
    }
    // Create a copy of the name
    String sname(name.mPtr, name.mLen);
    // Compute the hash of the specified name
    const std::size_t hash = std::hash< String >{}(sname);
    // Search for a signal with this name
    for (const auto & e : s_Signals)
    {
        if (e.first == hash)
        {
            return e.second.mObj; // Found a match so let's return it
        }
    }
    // No such signal exists
    STHROWF("Unknown signal named (%s)", sname.c_str());
    // SHOULD NOT REACH THIS POINT!
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
Signal::Signal()
    : m_Head(nullptr), m_Data(), m_Name()
{
    s_FreeSignals.push_back(this);
}

// ------------------------------------------------------------------------------------------------
Signal::Signal(const CSStr name)
    : Signal(String(name ? name : _SC("")))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Signal::Signal(const String & name)
    : Signal(String(name))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Signal::Signal(String && name)
    : m_Head(nullptr), m_Data(), m_Name(std::move(name))
{
    if (m_Name.empty())
    {
        s_FreeSignals.push_back(this);
    }
}

// ------------------------------------------------------------------------------------------------
Signal::~Signal()
{
    Clear();
    if (m_Name.empty())
    {
        s_FreeSignals.erase(std::remove(s_FreeSignals.begin(), s_FreeSignals.end(), this),
                            s_FreeSignals.end());
    }
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::Count() const
{
    // Don't attempt to count anything if there's no head
    if (m_Head == nullptr)
    {
        return 0;
    }

    // Final count
    Uint32 count = 0;
    // Walk down the chain and count all nodes
    for (Slot * node = m_Head; node != nullptr; node = node->mNext)
    {
        ++count;
    }
    // Return the count
    return count;
}

// ------------------------------------------------------------------------------------------------
void Signal::Clear()
{
    // Don't attempt to clear anything if there's no head
    if (m_Head == nullptr)
    {
        return;
    }

    // Walk down the chain and delete all nodes
    for (Slot * node = m_Head, * next = nullptr; node != nullptr; node = next)
    {
        // Grab the next node upfront
        next = node->mNext;
        // Delete the node instance
        delete node;
    }
    // Reset the head
    m_Head = nullptr;
}

// ------------------------------------------------------------------------------------------------
void Signal::Connect(Object & env, Function & func)
{
    // Don't attempt to search anything if there's no head
    if (m_Head == nullptr)
    {
        m_Head = new Slot(env, func, nullptr);
        // We're done here
        return;
    }

    const Slot slot{env, func};
    // Don't attempt to search anything if there's only one element
    if (m_Head->mNext == nullptr)
    {
        // Is it already inserted?
        if (*m_Head != slot)
        {
            m_Head = new Slot(env, func, m_Head);
        }
        // We're done here
        return;
    }

    // Walk down the chain and find an already matching node
    for (Slot * node = m_Head; node != nullptr; node = node->mNext)
    {
        if (*node == slot)
        {
            return; // Already connected
        }
    }
    // Connect now
    m_Head = new Slot(env, func, m_Head);
}

// ------------------------------------------------------------------------------------------------
bool Signal::Connected(Object & env, Function & func) const
{
    // Don't attempt to search anything if there's no head
    if (m_Head == nullptr)
    {
        return false;
    }

    const Slot slot{env, func};
    // Walk down the chain and find a matching node
    for (Slot * node = m_Head; node != nullptr; node = node->mNext)
    {
        if (*node == slot)
        {
            return true; // Found it
        }
    }
    // No such slot exists
    return false;
}

// ------------------------------------------------------------------------------------------------
void Signal::Disconnect(Object & env, Function & func)
{
    // Don't attempt to search anything if there's no head
    if (m_Head == nullptr)
    {
        return;
    }

    const Slot slot{env, func};
    // Walk down the chain and remove the matching nodes
    for (Slot * node = m_Head, * next = nullptr; node != nullptr; node = next)
    {
        // Grab the next node upfront
        next = node->mNext;
        // Is this our node?
        if (*node == slot)
        {
            // Is this the head?
            if (node == m_Head)
            {
                m_Head = next; // Move the head to the next one
            }
            // Detach it from the chain
            node->Detach();
            // Delete the node instance
            delete node;
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Signal::DisconnectThis(Object & env)
{
    // Don't attempt to search anything if there's no head
    if (m_Head == nullptr)
    {
        return;
    }

    const SQHash hash = Slot{env, NullFunction()}.mEnvHash;
    // Walk down the chain and remove the matching nodes
    for (Slot * node = m_Head, * next = nullptr; node != nullptr; node = next)
    {
        // Grab the next node upfront
        next = node->mNext;
        // Is this our node?
        if (node->mEnvHash == hash)
        {
            // Is this the head?
            if (node == m_Head)
            {
                m_Head = next; // Move the head to the next one
            }
            // Detach it from the chain
            node->Detach();
            // Delete the node instance
            delete node;
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Signal::DisconnectFunc(Function & func)
{
    // Don't attempt to search anything if there's no head
    if (m_Head == nullptr)
    {
        return;
    }

    const SQHash hash = Slot{NullObject(), func}.mFuncHash;
    // Walk down the chain and remove the matching nodes
    for (Slot * node = m_Head, * next = nullptr; node != nullptr; node = next)
    {
        // Grab the next node upfront
        next = node->mNext;
        // Is this our node?
        if (node->mFuncHash == hash)
        {
            // Is this the head?
            if (node == m_Head)
            {
                m_Head = next; // Move the head to the next one
            }
            // Detach it from the chain
            node->Detach();
            // Delete the node instance
            delete node;
        }
    }
}

// ------------------------------------------------------------------------------------------------
Uint32 Signal::CountThis(Object & env) const
{
    // Don't attempt to count anything if there's no head
    if (m_Head == nullptr)
    {
        return 0;
    }

    const SQHash hash = Slot{env, NullFunction()}.mEnvHash;
    // Final count
    Uint32 count = 0;
    // Walk down the chain and count the matching nodes
    for (Slot * node = m_Head; node != nullptr; node = node->mNext)
    {
        // Is this our node?
        if (node->mEnvHash == hash)
        {
            ++count;
        }
    }
    // Return the count
    return count;
}

// ------------------------------------------------------------------------------------------------
Uint32 Signal::CountFunc(Function & func) const
{
    // Don't attempt to count anything if there's no head
    if (m_Head == nullptr)
    {
        return 0;
    }

    const SQHash hash = Slot{NullObject(), func}.mFuncHash;
    // Final count
    Uint32 count = 0;
    // Walk down the chain and count the matching nodes
    for (Slot * node = m_Head; node != nullptr; node = node->mNext)
    {
        // Is this our node?
        if (node->mFuncHash == hash)
        {
            ++count;
        }
    }
    // Return the count
    return count;
}

// ------------------------------------------------------------------------------------------------
void Signal::Head(Object & env, Function & func)
{
    // Don't attempt to search anything if there's no head
    if (m_Head == nullptr)
    {
        m_Head = new Slot(env, func, nullptr);
        // We're done here
        return;
    }

    const Slot slot{env, func};
    // Don't attempt to search anything if there's only one element
    if (m_Head->mNext == nullptr)
    {
        // Is it already inserted?
        if (*m_Head != slot)
        {
            m_Head = new Slot(env, func, m_Head);
        }
        // We're done here
        return;
    }

    // Grab the head node
    Slot * node = m_Head;
    // Walk down the chain and find a matching node
    for (; node != nullptr; node = node->mNext)
    {
        if (*node == slot)
        {
            break; // Found it
        }
    }
    // Have we found anything?
    if (node == nullptr)
    {
        m_Head = new Slot(env, func, m_Head); // Lead everyone
    }
    // Is it the head already?
    else if (m_Head != node)
    {
        node->AttachNext(m_Head);
        // We're the head now
        m_Head = node;
    }
}

// ------------------------------------------------------------------------------------------------
void Signal::Tail(Object & env, Function & func)
{
    // Don't attempt to search anything if there's no head
    if (m_Head == nullptr)
    {
        m_Head = new Slot(env, func, nullptr);
        // We're done here
        return;
    }

    const Slot slot{env, func};
    // Don't attempt to search anything if there's only one element
    if (m_Head->mNext == nullptr)
    {
        // Is it already inserted?
        if (*m_Head != slot)
        {
            m_Head->mNext = new Slot(env, func, nullptr);
            // Link with the head
            m_Head->mNext->mPrev = m_Head;
        }
        // We're done here
        return;
    }

    // Grab the head node
    Slot * node = m_Head, * prev = nullptr;
    // Walk down the chain and find a matching node
    for (; node != nullptr; prev = node, node = node->mNext)
    {
        if (*node == slot)
        {
            break; // Found it
        }
    }
    // Have we found anything?
    if (node == nullptr)
    {
        // Create the slot now
        node = new Slot(env, func, nullptr);
    }
    else
    {
        // Walk down the chain until the end
        while (prev->mNext != nullptr)
        {
            prev = prev->mNext;
        }
        // Finally, detach the node from it's current position
        node->Detach();
    }
    // Knowing 'prev' points to last element
    node->AttachPrev(prev);
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::SqEmit(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // The signal instance
    Signal * signal = nullptr;
    // Attempt to extract the signal instance
    try
    {
        signal = Var< Signal * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    // Do we have a valid signal instance?
    if (!signal)
    {
        return sq_throwerror(vm, "Invalid signal instance");
    }
    // Walk down the chain and trigger slots
    for (Slot * node = signal->m_Head, * next = nullptr; node != nullptr; node = next)
    {
        // Grab the next node upfront
        next = node->mNext;
        // Remember the current stack size
        const StackGuard sg(vm);
        // Push the callback object
        sq_pushobject(vm, node->mFuncRef);
        // Is there an explicit environment?
        if (sq_isnull(node->mEnvRef))
        {
            sq_pushroottable(vm);
        }
        else
        {
            sq_pushobject(vm, node->mEnvRef);
        }
        // Are there any parameters to forward?
        if (top > 1)
        {
            for (SQInteger i = 2; i <= top; ++i)
            {
                sq_push(vm, i);
            }
        }
        // Make the function call and store the result
        const SQRESULT res = sq_call(vm, top, false, ErrorHandling::IsEnabled());
        // Validate the result
        if (SQ_FAILED(res))
        {
            return res; // Propagate the error
        }
    }
    // Specify that we don't return anything
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::SqQuery(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Do we have the collector environment?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing collector environment");
    }
    // Do we have the collector function?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing collector callback");
    }
    // The signal instance
    Signal * signal = nullptr;
    // Attempt to extract the signal instance and collector
    try
    {
        signal = Var< Signal * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    // Do we have a valid signal instance?
    if (!signal)
    {
        return sq_throwerror(vm, "Invalid signal instance");
    }
    // The collector
    HSQOBJECT cenv, cfunc;
    // Grab the collector environment
    SQRESULT res = sq_getstackobj(vm, 2, &cenv);
    // Validate the result
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error
    }
    // Was there a valid environment?
    else if (sq_isnull(cenv))
    {
        // Remember the current stack size
        const StackGuard sg(vm);
        // Default to the root table
        sq_pushroottable(vm);
        // Try to grab the collector environment again
        SQRESULT res = sq_getstackobj(vm, -1, &cenv);
        // Validate the result
        if (SQ_FAILED(res))
        {
            return res; // Propagate the error
        }
    }
    // Grab the collector function
    res = sq_getstackobj(vm, 3, &cfunc);
    // Validate the result
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error
    }
    // Some dummy checks to make sure the collector is a callable object
    else if (!sq_isfunction(cfunc) && !sq_isclosure(cfunc) && !sq_isnativeclosure(cfunc))
    {
        return sq_throwerror(vm, "Invalid collector");
    }
    // Walk down the chain and trigger slots
    for (Slot * node = signal->m_Head, * next = nullptr; node != nullptr; node = next)
    {
        // Grab the next node upfront
        next = node->mNext;
        // Remember the current stack size
        const StackGuard sg(vm);
        // Push the callback object
        sq_pushobject(vm, node->mFuncRef);
        // Is there an explicit environment?
        if (sq_isnull(node->mEnvRef))
        {
            sq_pushroottable(vm);
        }
        else
        {
            sq_pushobject(vm, node->mEnvRef);
        }
        // Are there any parameters to forward?
        if (top > 3)
        {
            for (SQInteger i = 4; i <= top; ++i)
            {
                sq_push(vm, i);
            }
        }
        // Make the function call and store the result
        res = sq_call(vm, top - 2, true, ErrorHandling::IsEnabled());
        // Validate the result
        if (SQ_FAILED(res))
        {
            return res; // Propagate the error
        }
        // Push the collector onto the stack
        sq_pushobject(vm, cfunc);
        sq_pushobject(vm, cenv);
        // Push the returned value
        sq_push(vm, -3);
        // Make the function call and store the result
        res = sq_call(vm, 2, false, ErrorHandling::IsEnabled());
        // Validate the result
        if (SQ_FAILED(res))
        {
            return res; // Propagate the error
        }
    }
    // Specify that we don't return anything
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::SqConsume(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // The signal instance
    Signal * signal = nullptr;
    // Attempt to extract the signal instance
    try
    {
        signal = Var< Signal * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    // Do we have a valid signal instance?
    if (!signal)
    {
        return sq_throwerror(vm, "Invalid signal instance");
    }
    // Default to not consumed
    SQBool ret = SQFalse;
    // Walk down the chain and trigger slots
    for (Slot * node = signal->m_Head, * next = nullptr; node != nullptr; node = next)
    {
        // Grab the next node upfront
        next = node->mNext;
        // Remember the current stack size
        const StackGuard sg(vm);
        // Push the callback object
        sq_pushobject(vm, node->mFuncRef);
        // Is there an explicit environment?
        if (sq_isnull(node->mEnvRef))
        {
            sq_pushroottable(vm);
        }
        else
        {
            sq_pushobject(vm, node->mEnvRef);
        }
        // Are there any parameters to forward?
        if (top > 1)
        {
            for (SQInteger i = 2; i <= top; ++i)
            {
                sq_push(vm, i);
            }
        }
        // Make the function call and store the result
        const SQRESULT res = sq_call(vm, top, true, ErrorHandling::IsEnabled());
        // Validate the result
        if (SQ_FAILED(res))
        {
            return res; // Propagate the error
        }
        // Is the returned value not null?
        else if (sq_gettype(vm, -1) != OT_NULL)
        {
            // Obtain the returned value
            sq_tobool(vm, -1, &ret);
            // Should we proceed to the next slot or stop here?
            if (ret == SQTrue)
            {
                break; // The slot consumed the signal
            }
        }
    }
    // Forward the returned value to the invoker
    sq_pushbool(vm, ret);
    // Specify that we returned something
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::SqApprove(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // The signal instance
    Signal * signal = nullptr;
    // Attempt to extract the signal instance
    try
    {
        signal = Var< Signal * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    // Do we have a valid signal instance?
    if (!signal)
    {
        return sq_throwerror(vm, "Invalid signal instance");
    }
    // Default to approved
    SQBool ret = SQTrue;
    // Walk down the chain and trigger slots
    for (Slot * node = signal->m_Head, * next = nullptr; node != nullptr; node = next)
    {
        // Grab the next node upfront
        next = node->mNext;
        // Remember the current stack size
        const StackGuard sg(vm);
        // Push the callback object
        sq_pushobject(vm, node->mFuncRef);
        // Is there an explicit environment?
        if (sq_isnull(node->mEnvRef))
        {
            sq_pushroottable(vm);
        }
        else
        {
            sq_pushobject(vm, node->mEnvRef);
        }
        // Are there any parameters to forward?
        if (top > 1)
        {
            for (SQInteger i = 2; i <= top; ++i)
            {
                sq_push(vm, i);
            }
        }
        // Make the function call and store the result
        const SQRESULT res = sq_call(vm, top, true, ErrorHandling::IsEnabled());
        // Validate the result
        if (SQ_FAILED(res))
        {
            return res; // Propagate the error
        }
        // Is the returned value not null?
        else if (sq_gettype(vm, -1) != OT_NULL)
        {
            // Obtain the returned value
            sq_tobool(vm, -1, &ret);
            // Should we proceed to the next slot or stop here?
            if (ret == SQFalse)
            {
                break; // The slot did not approve the signal
            }
        }
    }
    // Forward the returned value to the invoker
    sq_pushbool(vm, ret);
    // Specify that we returned something
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::SqRequest(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // The signal instance
    Signal * signal = nullptr;
    // Attempt to extract the signal instance
    try
    {
        signal = Var< Signal * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    // Do we have a valid signal instance?
    if (!signal)
    {
        return sq_throwerror(vm, "Invalid signal instance");
    }
    // Walk down the chain and trigger slots
    for (Slot * node = signal->m_Head, * next = nullptr; node != nullptr; node = next)
    {
        // Grab the next node upfront
        next = node->mNext;
        // Remember the current stack size
        const StackGuard sg(vm);
        // Push the callback object
        sq_pushobject(vm, node->mFuncRef);
        // Is there an explicit environment?
        if (sq_isnull(node->mEnvRef))
        {
            sq_pushroottable(vm);
        }
        else
        {
            sq_pushobject(vm, node->mEnvRef);
        }
        // Are there any parameters to forward?
        if (top > 1)
        {
            for (SQInteger i = 2; i <= top; ++i)
            {
                sq_push(vm, i);
            }
        }
        // Make the function call and store the result
        const SQRESULT res = sq_call(vm, top, true, ErrorHandling::IsEnabled());
        // Validate the result
        if (SQ_FAILED(res))
        {
            return res; // Propagate the error
        }
        // Is the returned value not null?
        else if (sq_gettype(vm, -1) != OT_NULL)
        {
            // Push back the returned value
            sq_push(vm, -1);
            // Specify that we returned something
            return 1;
        }
    }
    // Specify that we returned nothing
    return 0;
}

// ================================================================================================
void Register_Signal(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(Typename::Str,
        Class< Signal, NoConstructor< Signal > >(vm, Typename::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
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
        .FmtFunc(_SC("SqSignal"), &Signal::Fetch)
        .FmtFunc(_SC("SqCreateSignal"), &Signal::Create)
        .FmtFunc(_SC("SqRemoveSignal"), &Signal::Remove);
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to terminate the signals.
*/
void TerminateSignals()
{
    Signal::Terminate();
}

} // Namespace:: SqMod
