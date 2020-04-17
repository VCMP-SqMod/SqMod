#pragma once

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
struct Signal;
struct SignalWrapper;

/* ------------------------------------------------------------------------------------------------
 * Class used to deliver events to one or more listeners.
*/
struct Signal
{
    friend class SignalWrapper;
    // --------------------------------------------------------------------------------------------
    typedef unsigned int SizeType; // Type of value used to represent sizes and/or indexes.
    // --------------------------------------------------------------------------------------------
    enum { SMB_SIZE = 8 };

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Signal();

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    explicit Signal(const char * name)
        : Signal(String(name))
    {
        //...
    }

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    explicit Signal(const String & name)
        : Signal(String(name))
    {
        //...
    }

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    explicit Signal(String && name);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    Signal(const Signal & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    Signal(Signal && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Signal();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    Signal & operator = (const Signal & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    Signal & operator = (Signal && o) = delete;

protected:

    /* --------------------------------------------------------------------------------------------
     * Adjust the internal buffer size if necessary.
    */
    bool AdjustSlots(SizeType capacity);

    /* --------------------------------------------------------------------------------------------
     * Structure responsible for storing information about a slot.
    */
    struct Slot {
        SQHash      mThisHash; // The hash of the specified environment.
        SQHash      mFuncHash; // The hash of the specified callback.
        HSQOBJECT   mThisRef; // The specified script environment.
        HSQOBJECT   mFuncRef; // The specified script callback.

        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        Slot()
            : mThisHash(0)
            , mFuncHash(0)
            , mThisRef()
            , mFuncRef()
        {
            sq_resetobject(&mThisRef);
            sq_resetobject(&mFuncRef);
        }

        /* ----------------------------------------------------------------------------------------
         * Forwarding constructor.
        */
        Slot(Object & env, Function & func)
            : Slot(env.GetObj(), func.GetFunc())
        {
            /* ... */
        }

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Slot(HSQOBJECT & env, HSQOBJECT & func)
            : mThisHash(0)
            , mFuncHash(0)
            , mThisRef(env)
            , mFuncRef(func)
        {
            HSQUIRRELVM vm = DefaultVM::Get();
            // Remember the current stack size
            const StackGuard sg(vm);
            // Is there an explicit environment?
            if (!sq_isnull(mThisRef))
            {
                // Keep a reference to this environment
                sq_addref(vm, &mThisRef);
                // Push the environment on the stack
                sq_pushobject(vm, mThisRef);
                // Grab the hash of the environment object
                mThisHash = sq_gethash(vm, -1);
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
        }
        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Slot(HSQOBJECT & env, HSQOBJECT & func, SQHash envh, SQHash funch)
            : mThisHash(envh)
            , mFuncHash(funch)
            , mThisRef(env)
            , mFuncRef(func)
        {

        }

        /* ----------------------------------------------------------------------------------------
         * Copy constructor.
        */
        Slot(const Slot & o)
            : mThisHash(o.mThisHash)
            , mFuncHash(o.mFuncHash)
            , mThisRef(o.mThisRef)
            , mFuncRef(o.mFuncRef)
        {
            // Track reference
            if (mFuncHash != 0)
            {
                sq_addref(DefaultVM::Get(), &mThisRef);
                sq_addref(DefaultVM::Get(), &mFuncRef);
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Move constructor.
        */
        Slot(Slot && o) noexcept
            : mThisHash(o.mThisHash)
            , mFuncHash(o.mFuncHash)
            , mThisRef(o.mThisRef)
            , mFuncRef(o.mFuncRef)
        {
            // Take ownership
            sq_resetobject(&o.mThisRef);
            sq_resetobject(&o.mFuncRef);
        }

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Slot()
        {
            Release();
        }

        /* ----------------------------------------------------------------------------------------
         * Copy assignment operator. (disabled)
        */
        Slot & operator = (const Slot & o)
        {
            if (this != &o)
            {
                // Release current resources, if any
                Release();
                // Replicate data
                mThisHash = o.mThisHash;
                mFuncHash = o.mFuncHash;
                mThisRef = o.mThisRef;
                mFuncRef = o.mFuncRef;
                // Track reference
                sq_addref(DefaultVM::Get(), &const_cast< HSQOBJECT & >(o.mThisRef));
                sq_addref(DefaultVM::Get(), &const_cast< HSQOBJECT & >(o.mFuncRef));
            }

            return *this;
        }

        /* ----------------------------------------------------------------------------------------
         * Move assignment operator.
        */
        Slot & operator = (Slot && o) noexcept
        {
            if (this != &o)
            {
                // Release current resources, if any
                Release();
                // Replicate data
                mThisHash = o.mThisHash;
                mFuncHash = o.mFuncHash;
                mThisRef = o.mThisRef;
                mFuncRef = o.mFuncRef;
                // Take ownership
                sq_resetobject(&o.mThisRef);
                sq_resetobject(&o.mFuncRef);
            }

            return *this;
        }

        /* ----------------------------------------------------------------------------------------
         * Equality comparison operator.
        */
        bool operator == (const Slot & o) const
        {
            return (mThisHash == o.mThisHash) && (mFuncHash == o.mFuncHash);
        }

        /* ----------------------------------------------------------------------------------------
         * Inequality comparison operator.
        */
        bool operator != (const Slot & o) const
        {
            return (mThisHash != o.mThisHash) || (mFuncHash != o.mFuncHash);
        }

        /* ----------------------------------------------------------------------------------------
         * Release managed script resources.
        */
        bool Available() const
        {
            return (mFuncHash == 0);
        }

        /* ----------------------------------------------------------------------------------------
         * Release managed script resources.
        */
        void Release()
        {
            // Should we release any environment object?
            if (mThisHash != 0)
            {
                sq_release(DefaultVM::Get(), &mThisRef);
                sq_resetobject(&mThisRef);
                // Also reset the hash
                mThisHash = 0;
            }
            // Should we release any callback object?
            if (mFuncHash != 0)
            {
                sq_release(DefaultVM::Get(), &mFuncRef);
                sq_resetobject(&mFuncRef);
                // Also reset the hash
                mFuncHash = 0;
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Swap the values of two slots.
        */
        void Swap(Slot & s)
        {
            // Swap the environment hash
            SQHash h = mThisHash;
            mThisHash = s.mThisHash;
            s.mThisHash = h;
            // Swap the callback hash
            h = mFuncHash;
            mFuncHash = s.mFuncHash;
            s.mFuncHash = h;
            // Swap the environment object
            HSQOBJECT o = mThisRef;
            mThisRef = s.mThisRef;
            s.mThisRef = o;
            // Swap the callback object
            o = mFuncRef;
            mFuncRef = s.mFuncRef;
            s.mFuncRef = o;
        }
    };

    // --------------------------------------------------------------------------------------------
    typedef Slot                ValueType; // Value type used to represent a slot.
    typedef ValueType &         Reference; // Reference to the stored value type
    typedef const ValueType &   ConstReference; // Constant reference to the stored value type.
    typedef ValueType *         Pointer; // Pointer to the stored value type
    typedef const ValueType *   ConstPointer; // Constant pointer to the stored value type.

    // --------------------------------------------------------------------------------------------
    /// Execution scope used to adjust iterators when removing slots or adjusting the buffer.
    struct Scope {
        // ----------------------------------------------------------------------------------------
        Pointer  mItr; ///< Currently executed slot.
        Pointer  mEnd; ///< Where the execution ends.
        Scope *  mParent; ///< Previous execution scope.
        Scope *  mChild; ///< Next execution scope.
        // ----------------------------------------------------------------------------------------
        /// Default constructor.
        Scope(Scope * parent, Pointer begin, Pointer end)
            : mItr(begin), mEnd(end), mParent(parent), mChild(nullptr)
        {
            if (mParent != nullptr) mParent->mChild = this;
        }
        // ----------------------------------------------------------------------------------------
        /// Destructor.
        ~Scope() {
            if (mParent != nullptr) mParent->mChild = nullptr;
        }
        // ----------------------------------------------------------------------------------------
        /// Adjust the iterators to account for the fact that the specified slot was removed.
        void Descend(Pointer ptr);
        /// Adjust the iterators to account for the fact that the specified slot is now leading.
        void Lead(Pointer ptr);
        /// Adjust the iterators to account for the fact that the specified slot is now tailing.
        void Tail(Pointer ptr);
        /// Adjust the iterators to finish the execution abruptly.
        void Finish();
    };

private:

    // --------------------------------------------------------------------------------------------
    SizeType        m_Used; // The number of stored slots that are valid.
    SizeType        m_Size; // The size of the memory allocated for slots.
    Pointer         m_Slots; // Pointer to the memory containing the slots.
    // --------------------------------------------------------------------------------------------
    Scope *         m_Scope; // Current execution state.
    // --------------------------------------------------------------------------------------------
    String          m_Name; // The name that identifies this signal.
    LightObj        m_Data; // User data associated with this instance.
    // --------------------------------------------------------------------------------------------
    ValueType       m_SMB[SMB_SIZE]{}; // Small buffer optimization.

public:

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
    LightObj & GetData()
    {
        return m_Data;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(LightObj & data)
    {
        m_Data = data;
    }

    /* --------------------------------------------------------------------------------------------
     * The number of slots connected to the signal.
    */
    SQInteger GetUsed() const
    {
        return static_cast< SQInteger >(m_Used);
    }

    /* --------------------------------------------------------------------------------------------
     * Clear all slots connected to the signal.
    */
    void ClearSlots();

    /* --------------------------------------------------------------------------------------------
     * See if there are any slots connected.
    */
    bool IsEmpty() const
    {
        return (m_Used == 0);
    }

protected:

    /* --------------------------------------------------------------------------------------------
     * Connect the specified slot to the signal.
    */
    SQInteger Connect(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * Connect the specified slot but not before disconnecting all other occurrences.
    */
    SQInteger ConnectOnce(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * Disconnect all occurrences of the specified slot from the signal.
    */
    SQInteger  Disconnect(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * See if the specified slot is connected to the signal.
    */
    SQInteger Exists(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * See if the specified slot environment is connected to the signal.
    */
    SQInteger ExistsThis(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * See if the specified slot callback is connected to the signal.
    */
    SQInteger ExistsFunc(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * Count all occurrences of the specified slot.
    */
    SQInteger Count(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * Count all occurrences of the specified slot environment.
    */
    SQInteger CountThis(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * Count all occurrences of the specified slot callback.
    */
    SQInteger CountFunc(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * Move all occurrences of the specified slot to the front.
    */
    SQInteger Lead(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * Move all occurrences of the specified slot environment to the front.
    */
    SQInteger LeadThis(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * Move all occurrences of the specified slot callback to the front.
    */
    SQInteger LeadFunc(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * Move all occurrences of the specified slot to the back.
    */
    SQInteger Tail(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * Move all occurrences of the specified slot environment to the back.
    */
    SQInteger TailThis(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * Move all occurrences of the specified slot callback to the back.
    */
    SQInteger TailFunc(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * Remove all occurrences of the specified slot.
    */
    SQInteger Eliminate(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * Remove all occurrences of the specified slot environment.
    */
    SQInteger EliminateThis(SignalWrapper & w);

    /* --------------------------------------------------------------------------------------------
     * Remove all occurrences of the specified slot callback.
    */
    SQInteger EliminateFunc(SignalWrapper & w);

public:

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `Connect` method of this class.
    */
    static SQInteger SqConnect(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `ConnectOnce` method of this class.
    */
    static SQInteger SqConnectOnce(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `Exists` method of this class.
    */
    static SQInteger SqExists(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `Disconnect` method of this class.
    */
    static SQInteger SqDisconnect(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `ExistsThis` method of this class.
    */
    static SQInteger SqExistsThis(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `ExistsFunc` method of this class.
    */
    static SQInteger SqExistsFunc(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `Count` method of this class.
    */
    static SQInteger SqCount(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `CountThis` method of this class.
    */
    static SQInteger SqCountThis(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `CountFunc` method of this class.
    */
    static SQInteger SqCountFunc(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `Lead` method of this class.
    */
    static SQInteger SqLead(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `LeadThis` method of this class.
    */
    static SQInteger SqLeadThis(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `LeadFunc` method of this class.
    */
    static SQInteger SqLeadFunc(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `Tail` method of this class.
    */
    static SQInteger SqTail(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `TailThis` method of this class.
    */
    static SQInteger SqTailThis(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `TailFunc` method of this class.
    */
    static SQInteger SqTailFunc(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `Eliminate` method of this class.
    */
    static SQInteger SqEliminate(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `EliminateThis` method of this class.
    */
    static SQInteger SqEliminateThis(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `EliminateFunc` method of this class.
    */
    static SQInteger SqEliminateFunc(HSQUIRRELVM vm);

protected:

    /* --------------------------------------------------------------------------------------------
     * Emit the event to the connected slots.
    */
    SQInteger Emit(HSQUIRRELVM vm, SQInteger top);

    /* --------------------------------------------------------------------------------------------
     * Emit the event to the connected slots and collect returned values.
    */
    SQInteger Query(HSQUIRRELVM vm, SQInteger top);

    /* --------------------------------------------------------------------------------------------
     * Emit the event to the connected slots and see if they consume it.
    */
    SQInteger Consume(HSQUIRRELVM vm, SQInteger top);

    /* --------------------------------------------------------------------------------------------
     * Emit the event to the connected slots and see if they approve it.
    */
    SQInteger Approve(HSQUIRRELVM vm, SQInteger top);

    /* --------------------------------------------------------------------------------------------
     * Emit the event to the connected slots and see if they return something.
    */
    SQInteger Request(HSQUIRRELVM vm, SQInteger top);

public:

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `Emit` method of this class.
    */
    static SQInteger SqEmit(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `Query` method of this class.
    */
    static SQInteger SqQuery(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `Consume` method of this class.
    */
    static SQInteger SqConsume(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `Approve` method of this class.
    */
    static SQInteger SqApprove(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Squirrel wrapper for the `Request` method of this class.
    */
    static SQInteger SqRequest(HSQUIRRELVM vm);

protected:

    // --------------------------------------------------------------------------------------------
    typedef std::pair< std::size_t, SignalPair >    SignalElement;
    typedef std::vector< SignalElement >            SignalPool;
    typedef std::vector< Signal * >                 FreeSignals;

    // --------------------------------------------------------------------------------------------
    static SignalPool   s_Signals; // List of all created signals.
    static FreeSignals  s_FreeSignals; // List of signals without a name.

#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStatic"
    /* --------------------------------------------------------------------------------------------
     * Specialization for when there are no arguments given.
    */
    void PushParameters()
    {
        //...
    }
#pragma clang diagnostic pop

    /* --------------------------------------------------------------------------------------------
     * Specialization for when there's only one argument given/remaining.
    */
    template < typename T > void PushParameters(T v)
    {
        Var< T >::push(DefaultVM::Get(), v);
    }

    /* --------------------------------------------------------------------------------------------
     * Specialization for when there's more than one argument given.
    */
    template < typename T, typename... Args > void PushParameters(T v, Args... args)
    {
        Var< T >::push(DefaultVM::Get(), v);
        PushParameters(args...);
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Terminate all signal instances and release any script resources.
    */
    static void Terminate();

    /* --------------------------------------------------------------------------------------------
     * Create a free signal without a specific name.
    */
    static LightObj CreateFree();

    /* --------------------------------------------------------------------------------------------
     * Create a new signal with the specified name.
    */
    static LightObj Create(StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Remove the signal with the specified name.
    */
    static void Remove(StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the signal with the specified name.
    */
    static const LightObj & Fetch(StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Emit a signal from the module.
    */
    template < typename... Args > void operator () (Args&&... args)
    {
        // Are there any slots connected?
        if (!m_Used) return;
        // Enter a new execution scope
        Scope scope(m_Scope, m_Slots, m_Slots + m_Used);
        // Activate the current scope and create a guard to restore it
        const AutoAssign< Scope * > aa(m_Scope, scope.mParent, &scope);
        // Grab the default virtual machine
        HSQUIRRELVM vm = DefaultVM::Get();
        // Process the slots from this scope
        while (scope.mItr != scope.mEnd)
        {
            // Grab a reference to the current slot
            const Slot & slot = *(scope.mItr++);
            // Push the callback object
            sq_pushobject(vm, slot.mFuncRef);
            // Is there an explicit environment?
            if (slot.mThisHash == 0)
            {
                sq_pushroottable(vm);
            }
            else
            {
                sq_pushobject(vm, slot.mThisRef);
            }
            // Push the given parameters on the stack
            PushParameters(args...);
            // Make the function call and store the result
            const SQRESULT res = sq_call(vm, 1 + sizeof...(Args), static_cast< SQBool >(false), static_cast< SQBool >(ErrorHandling::IsEnabled()));
            // Pop the callback object from the stack
            sq_pop(vm, 1);
            // Validate the result
            if (SQ_FAILED(res))
            {
                SQTHROW(vm, LastErrorString(vm)); // Stop emitting signals NOLINT(hicpp-exception-baseclass,cert-err60-cpp)
            }
        }
    }
};

} // Namespace:: SqMod
