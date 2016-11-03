#ifndef _MISC_SIGNAL_HPP_
#define _MISC_SIGNAL_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
class Signal;

/* ------------------------------------------------------------------------------------------------
 * Helper class used to reference and keep track of signal instances.
*/
struct SignalReference
{
    // --------------------------------------------------------------------------------------------
    Signal *    mPtr; // A raw pointer to the signal instance.
    Object      mObj; // A managed reference to the signal instance.

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    SignalReference(Signal * ptr, const Object & ref)
        : mPtr(ptr), mObj(ref)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SignalReference(const SignalReference & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SignalReference(SignalReference && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~SignalReference() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    SignalReference & operator = (const SignalReference & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    SignalReference & operator = (SignalReference && o) = default;
};

/* ------------------------------------------------------------------------------------------------
 * Utility class for for delivering events to one or more listeners.
*/
class Signal
{
private:

    /* --------------------------------------------------------------------------------------------
     * Used to store a script callback and the unique hash associated with it for quick searching.
    */
    struct Slot
    {
        // ----------------------------------------------------------------------------------------
        SQHash      mEnvHash; // The hash of the specified environment.
        SQHash      mFuncHash; // The hash of the specified callback.
        HSQOBJECT   mEnvRef; // The specified script environment.
        HSQOBJECT   mFuncRef; // The specified script callback.

        // ----------------------------------------------------------------------------------------
        Slot *      mPrev; // Previous slot in the chain.
        Slot *      mNext; // Next slot in the chain.

        /* ----------------------------------------------------------------------------------------
         * Forwarding constructor.
        */
        Slot(Object & env, Function & func)
            : Slot(env.GetObject(), func.GetFunc(), nullptr)
        {
            /* ... */
        }

        /* ----------------------------------------------------------------------------------------
         * Forwarding constructor.
        */
        Slot(Object & env, Function & func, Slot * head)
            : Slot(env.GetObject(), func.GetFunc(), head)
        {
            /* ... */
        }

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Slot(HSQOBJECT & env, HSQOBJECT & func, Slot * head)
            : mEnvHash(0)
            , mFuncHash(0)
            , mEnvRef(env)
            , mFuncRef(func)
            , mPrev(nullptr)
            , mNext(head)
        {
            HSQUIRRELVM vm = DefaultVM::Get();
            // Remember the current stack size
            const StackGuard sg(vm);
            // Is there an explicit environment?
            if (!sq_isnull(mEnvRef))
            {
                // Keep a reference to this environment
                sq_addref(vm, &mEnvRef);
                // Push the environment on the stack
                sq_pushobject(vm, mEnvRef);
                // Grab the hash of the environment object
                mEnvHash = sq_gethash(vm, -1);
            }
            // Is there an explicit function?
            if (!sq_isnull(mFuncRef))
            {
                // Keep a reference to this function
                sq_addref(vm, &mFuncRef);
                // Push the callback on the stack
                sq_pushobject(vm, mFuncRef);
                // Grab the hash of the callback object
                mFuncHash = sq_gethash(vm, -1);
            }
            // Was there a previous head?
            if (mNext != nullptr)
            {
                // Steal the slot before the head
                mPrev = mNext->mPrev;
                // Did that head slot had a slot behind it?
                if (mPrev != nullptr)
                {
                    // Tell it we're the next slot now
                    mPrev->mNext = this;
                }
                // Tell the previous head that we're behind it now
                mNext->mPrev = this;
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Copy constructor. (disabled)
        */
        Slot(const Slot & o) = delete;

        /* ----------------------------------------------------------------------------------------
         * Move constructor.
        */
        Slot(Slot && o)
            : mEnvHash(o.mEnvHash)
            , mFuncHash(o.mFuncHash)
            , mEnvRef(o.mEnvRef)
            , mFuncRef(o.mFuncRef)
            , mPrev(o.mPrev)
            , mNext(o.mNext)
        {
            // Take ownership
            sq_resetobject(&o.mEnvRef);
            sq_resetobject(&o.mFuncRef);
            o.mPrev = nullptr;
            o.mNext = nullptr;
            // Attach to the chain
            Attach();
        }

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Slot()
        {
            Release();
            // Detach from the chain
            Detach();
        }

        /* ----------------------------------------------------------------------------------------
         * Copy assignment operator. (disabled)
        */
        Slot & operator = (const Slot & o) = delete;

        /* ----------------------------------------------------------------------------------------
         * Move assignment operator.
        */
        Slot & operator = (Slot && o)
        {
            if (this != &o)
            {
                // Release current resources, if any
                Release();
                // Replicate data
                mEnvHash = o.mEnvHash;
                mFuncHash = o.mFuncHash;
                mEnvRef = o.mEnvRef;
                mFuncRef = o.mFuncRef;
                mPrev = o.mPrev;
                mNext = o.mNext;
                // Take ownership
                sq_resetobject(&o.mEnvRef);
                sq_resetobject(&o.mFuncRef);
                o.mPrev = nullptr;
                o.mNext = nullptr;
                // Attach to the chain
                Attach();
            }

            return *this;
        }

        /* ----------------------------------------------------------------------------------------
         * Equality comparison operator.
        */
        bool operator == (const Slot & o) const
        {
            return (mEnvHash == o.mEnvHash) && (mFuncHash == o.mFuncHash);
        }

        /* ----------------------------------------------------------------------------------------
         * Inequality comparison operator.
        */
        bool operator != (const Slot & o) const
        {
            return (mEnvHash != o.mEnvHash) || (mFuncHash != o.mFuncHash);
        }

        /* ----------------------------------------------------------------------------------------
         * Release managed script resources.
        */
        void Release()
        {
            // Should we release any environment object?
            if (!sq_isnull(mEnvRef))
            {
                sq_release(DefaultVM::Get(), &mEnvRef);
                sq_resetobject(&mEnvRef);
            }
            // Should we release any callback object?
            if (!sq_isnull(mFuncRef))
            {
                sq_release(DefaultVM::Get(), &mFuncRef);
                sq_resetobject(&mFuncRef);
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Attach the slot to the chain.
        */
        void Attach()
        {
            // Is there a slot after us?
            if (mNext != nullptr)
            {
                // Tell it that we're behind it
                mNext->mPrev = this;
            }
            // Is there a node behind us?
            if (mPrev != nullptr)
            {
                // Tell it that we're ahead of it
                mPrev->mNext = this;
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Detach the slot from the chain.
        */
        void Detach()
        {
            // Is there a slot after us?
            if (mNext != nullptr)
            {
                // Tell it we're no longer behind it
                mNext->mPrev = mPrev;
            }
            // Is there a node behind us?
            if (mPrev != nullptr)
            {
                // Tell it we're no longer ahead of it
                mPrev->mNext = mNext;
            }
            mPrev = mNext = nullptr;
        }

        /* ----------------------------------------------------------------------------------------
         * Attach the slot after another slot.
        */
        void AttachNext(Slot * node)
        {
            Detach();
            // Assign the slot ahead of us
            mNext = node;
            // Is there a node to attach to?
            if (mNext != nullptr)
            {
                // Steal the previous slot
                mPrev = mNext->mPrev;
                // Was there a previous slot?
                if (mPrev != nullptr)
                {
                    mPrev->mNext = this; // Tell it we're the next slot now
                }
                // Place ourself behind
                mNext->mPrev = this;
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Attach the slot before another slot.
        */
        void AttachPrev(Slot * node)
        {
            Detach();
            // Assign the slot behind us
            mPrev = node;
            // Is there a node to attach to?
            if (mPrev != nullptr)
            {
                // Steal the next slot
                mNext = mPrev->mNext;
                // Was there a next slot?
                if (mNext != nullptr)
                {
                    mNext->mPrev = this; // Tell it we're the previous slot now
                }
                // Place ourself ahead
                mPrev->mNext = this;
            }
        }
    };

    // --------------------------------------------------------------------------------------------
    Slot *  m_Head; // The chain of slots bound to this signal.
    String  m_Name; // The name that identifies this signal.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Signal()
        : m_Head(nullptr), m_Name()
    {
        s_FreeSignals.push_back(this);
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Signal(const CSStr name)
        : Signal(String(name ? name : _SC("")))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Signal(const String & name)
        : Signal(String(name))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Signal(String && name)
        : m_Head(nullptr), m_Name(std::move(name))
    {
        if (m_Name.empty())
        {
            s_FreeSignals.push_back(this);
        }
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Signal(const Signal & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    Signal(Signal && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Signal()
    {
        Clear();
        if (m_Name.empty())
        {
            s_FreeSignals.erase(std::remove(s_FreeSignals.begin(), s_FreeSignals.end(), this),
                                s_FreeSignals.end());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Signal & operator = (const Signal & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Signal & operator = (Signal && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Signal & o) const
    {
        if (m_Name == o.m_Name)
        {
            return 0;
        }
        else if (m_Name > o.m_Name)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    const String & ToString() const
    {
        return m_Name;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * The number of slots connected to the signal.
    */
    SQInteger Count() const
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

    /* --------------------------------------------------------------------------------------------
     * Clear all slots connected to the signal.
    */
    void Clear()
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

    /* --------------------------------------------------------------------------------------------
     * Connect a function with a specific environment to the signal.
    */
    void Connect(Object & env, Function & func)
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

    /* --------------------------------------------------------------------------------------------
     * See whether a function with a specific environment is connected to the signal.
    */
    bool Connected(Object & env, Function & func) const
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

    /* --------------------------------------------------------------------------------------------
     * Disconnect a function with a specific environment from the signal.
    */
    void Disconnect(Object & env, Function & func)
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

    /* --------------------------------------------------------------------------------------------
     * Disconnect all functions with a specific environment from the signal.
    */
    void DisconnectThis(Object & env)
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

    /* --------------------------------------------------------------------------------------------
     * Disconnect all matching functions regardless of the environment from the signal.
    */
    void DisconnectFunc(Function & func)
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

    /* --------------------------------------------------------------------------------------------
     * Count all functions with a specific environment from the signal.
    */
    Uint32 CountThis(Object & env) const
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

    /* --------------------------------------------------------------------------------------------
     * Count all matching functions regardless of the environment from the signal.
    */
    Uint32 CountFunc(Function & func) const
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

    /* --------------------------------------------------------------------------------------------
     * Make sure that specified slot is positioned ahead of all other slots.
    */
    void Head(Object & env, Function & func)
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

    /* --------------------------------------------------------------------------------------------
     * Make sure that specified slot is positioned behind of all other slots.
    */
    void Tail(Object & env, Function & func)
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

    /* --------------------------------------------------------------------------------------------
     * Emit the event to the connected slots.
    */
    static SQInteger SqEmit(HSQUIRRELVM vm)
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

    /* --------------------------------------------------------------------------------------------
     * Emit the event to the connected slots and collect returned values.
    */
    static SQInteger SqQuery(HSQUIRRELVM vm)
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

protected:

    /* --------------------------------------------------------------------------------------------
     * Actual implementation of the consume method.
    */
    static SQInteger SqConsumeApproveImpl(HSQUIRRELVM vm, const SQBool rval)
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
        // Return value of each slot
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
            // Obtain the returned value
            sq_tobool(vm, -1, &ret);
            // Should we proceed to the next slot or stop here?
            if (ret == rval)
            {
                // Forward the returned value to the invoker
                sq_pushbool(vm, ret);
                // The slot satisfied our criteria
                break;
            }
        }
        // Specify that we returned something
        return 1;
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Emit the event to the connected slots and see if they consume it.
    */
    static SQInteger SqConsume(HSQUIRRELVM vm)
    {
        return SqConsumeApproveImpl(vm, SQTrue);
    }

    /* --------------------------------------------------------------------------------------------
     * Emit the event to the connected slots and see if they approve it.
    */
    static SQInteger SqApprove(HSQUIRRELVM vm)
    {
        return SqConsumeApproveImpl(vm, SQFalse);
    }

    /* --------------------------------------------------------------------------------------------
     * Emit the event to the connected slots and see if they return something.
    */
    static SQInteger SqRequest(HSQUIRRELVM vm)
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

private:

    // --------------------------------------------------------------------------------------------
    typedef std::pair< std::size_t, SignalReference >   SignalElement;
    typedef std::vector< SignalElement >                SignalContainer;
    typedef std::vector< Signal * >                     FreeSignals;

    // --------------------------------------------------------------------------------------------
    static SignalContainer  s_Signals; // List of all created signals.
    static FreeSignals      s_FreeSignals; // List of signals without a name.

public:

    /* --------------------------------------------------------------------------------------------
     * Terminate all signal instances and release any script resources.
    */
    static void Terminate();

    /* --------------------------------------------------------------------------------------------
     * Create a free signal without a specific name.
    */
    static Object Create();

    /* --------------------------------------------------------------------------------------------
     * Create a new signal with the specified name.
    */
    static Object Create(String name);

    /* --------------------------------------------------------------------------------------------
     * Remove the signal with the specified name.
    */
    static void Remove(String name);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the signal with the specified name.
    */
    static Object Fetch(String name);
};

} // Namespace:: SqMod

#endif // _MISC_SIGNAL_HPP_
