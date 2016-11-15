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
    Object  m_Data; // User data associated with this instance.
    String  m_Name; // The name that identifies this signal.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Signal();

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Signal(const CSStr name);

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Signal(const String & name);

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Signal(String && name);

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
    ~Signal();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Signal & operator = (const Signal & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Signal & operator = (Signal && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    const String & ToString() const
    {
        return m_Name;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user data.
    */
    Object & GetData()
    {
        return m_Data;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(Object & data)
    {
        m_Data = data;
    }

    /* --------------------------------------------------------------------------------------------
     * The number of slots connected to the signal.
    */
    SQInteger Count() const;

    /* --------------------------------------------------------------------------------------------
     * Clear all slots connected to the signal.
    */
    void Clear();

    /* --------------------------------------------------------------------------------------------
     * Connect a function with a specific environment to the signal.
    */
    void Connect(Object & env, Function & func);

    /* --------------------------------------------------------------------------------------------
     * See whether a function with a specific environment is connected to the signal.
    */
    bool Connected(Object & env, Function & func) const;

    /* --------------------------------------------------------------------------------------------
     * Disconnect a function with a specific environment from the signal.
    */
    void Disconnect(Object & env, Function & func);

    /* --------------------------------------------------------------------------------------------
     * Disconnect all functions with a specific environment from the signal.
    */
    void DisconnectThis(Object & env);

    /* --------------------------------------------------------------------------------------------
     * Disconnect all matching functions regardless of the environment from the signal.
    */
    void DisconnectFunc(Function & func);

    /* --------------------------------------------------------------------------------------------
     * Count all functions with a specific environment from the signal.
    */
    Uint32 CountThis(Object & env) const;

    /* --------------------------------------------------------------------------------------------
     * Count all matching functions regardless of the environment from the signal.
    */
    Uint32 CountFunc(Function & func) const;

    /* --------------------------------------------------------------------------------------------
     * Make sure that specified slot is positioned ahead of all other slots.
    */
    void Head(Object & env, Function & func);

    /* --------------------------------------------------------------------------------------------
     * Make sure that specified slot is positioned behind of all other slots.
    */
    void Tail(Object & env, Function & func);

    /* --------------------------------------------------------------------------------------------
     * Emit the event to the connected slots.
    */
    static SQInteger SqEmit(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Emit the event to the connected slots and collect returned values.
    */
    static SQInteger SqQuery(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Emit the event to the connected slots and see if they consume it.
    */
    static SQInteger SqConsume(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Emit the event to the connected slots and see if they approve it.
    */
    static SQInteger SqApprove(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Emit the event to the connected slots and see if they return something.
    */
    static SQInteger SqRequest(HSQUIRRELVM vm);

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
