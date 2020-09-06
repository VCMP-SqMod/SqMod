// ------------------------------------------------------------------------------------------------
#include "Misc/Signal.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("SqSignalBase"))

// ------------------------------------------------------------------------------------------------
Signal::SignalPool  Signal::s_Signals;
Signal::FreeSignals Signal::s_FreeSignals;

/* ------------------------------------------------------------------------------------------------
 * Class used to control the signal emitter.
*/
struct SignalWrapper
{
    // --------------------------------------------------------------------------------------------
    using Slot = Signal::Slot;
    // --------------------------------------------------------------------------------------------
    Signal *    mSignal; // A raw pointer to the signal instance.
    // --------------------------------------------------------------------------------------------
    Slot        mSlot; // The specified slot.
    HSQUIRRELVM mVM; // The specified virtual machine.
    SQInteger   mRes; // The result of the operation.
    bool        mOne; // Limit to one slot in the operation.
    bool        mAppend; // Append instead of push when leading or tailing.

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit SignalWrapper(HSQUIRRELVM vm, bool extra = false)
        : mSignal(nullptr)
        , mSlot()
        , mVM(vm)
        , mRes(Initialize(vm, extra))
        , mOne(false), mAppend(false)
    {
        //...
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts the information from the stack of the specified virtual machine.
    */
    SQInteger Initialize(HSQUIRRELVM vm, bool extra)
    {
        const SQInteger top = sq_gettop(vm);
        // See if the minimum amount of arguments were specified
        if (top <= 1)
        {
            return sq_throwerror(vm, "Wrong number of parameters");
        }
        // Attempt to extract the signal instance
        try
        {
            mSignal = Var< Signal * >(vm, 1).value;
        }
        catch (const Sqrat::Exception & e)
        {
            return sq_throwerror(vm, e.what());
        }
        // Do we have a valid signal instance?
        if (!mSignal)
        {
            return sq_throwerror(vm, "Invalid signal instance");
        }
        // By default, extra parameters start after callback
        SQInteger extpos = 3;
        // Is the first parameter a function?
        if (sq_gettype(vm, 2) & (_RT_CLOSURE | _RT_NATIVECLOSURE))
        {
            // Attempt to grab the callback object
            SQRESULT res = sq_getstackobj(vm, 2, &mSlot.mFuncRef);
            // Did we fail to retrieve the callback object?
            if (SQ_FAILED(res))
            {
                return res; // Propagate the error
            }
            else
            {
                sq_addref(vm, &mSlot.mFuncRef);
            }
            // Grab the hash of the callback object
            mSlot.mFuncHash = sq_gethash(vm, 2);
            // Push the root table on the stack
            sq_pushroottable(vm);
            // Attempt to grab root table as the environment
            res = sq_getstackobj(vm, -1, &mSlot.mThisRef);
            // Did we fail to retrieve the root table?
            if (SQ_FAILED(res))
            {
                // Pop the root table from the stack
                sq_pop(vm, 1);
                // Propagate the error
                return res;
            }
            else
            {
                sq_addref(vm, &mSlot.mThisRef);
            }
            // Grab the hash of the root table
            mSlot.mThisHash = sq_gethash(vm, -1);
            // Pop the root table from the stack
            sq_pop(vm, 1);
        }
        // Should we look for a specific environment?
        else if (top >= 3 && sq_gettype(vm, 3) & (_RT_CLOSURE | _RT_NATIVECLOSURE))
        {
            // Is the first type at suitable to be an environment at least?
            if (!(sq_gettype(vm, 2) & (_RT_TABLE | _RT_CLASS | _RT_INSTANCE)))
            {
                return sq_throwerror(vm, "Invalid environment object");
            }
            // Attempt to grab the environment object
            SQRESULT res = sq_getstackobj(vm, 2, &mSlot.mThisRef);
            // Did we fail to retrieve the environment object?
            if (SQ_FAILED(res))
            {
                return res; // Propagate the error
            }
            else
            {
                sq_addref(vm, &mSlot.mThisRef);
            }
            // Grab the hash of the environment object
            mSlot.mThisHash = sq_gethash(vm, 2);
            // Attempt to grab the callback object
            res = sq_getstackobj(vm, 3, &mSlot.mFuncRef);
            // Did we fail to retrieve the callback object?
            if (SQ_FAILED(res))
            {
                return res; // Propagate the error
            }
            else
            {
                sq_addref(vm, &mSlot.mFuncRef);
            }
            // Grab the hash of the callback object
            mSlot.mFuncHash = sq_gethash(vm, 3);
            // The extra parameters start one slot higher
            ++extpos;
        }
        else
        {
            return sq_throwerror(vm, "Missing callback function");
        }
        // Should we look for the extra parameters?
        if (extra && top >= extpos)
        {
            SQBool value;
            // Attempt to retrieve the parameter value
            sq_tobool(vm, extpos, &value);
            // Convert the retrieved value
            mOne = static_cast< bool >(value);
            // The next extra parameter starts one slot higher
            ++extpos;
        }
        else
        {
            mOne = false;
        }
        // Should we look for the extra parameters?
        if (extra && top >= extpos)
        {
            SQBool value;
            // Attempt to retrieve the parameter value
            sq_tobool(vm, extpos, &value);
            // Convert the retrieved value
            mAppend = static_cast< bool >(value);
        }
        else
        {
            mAppend = true;
        }
        // Initialization was successful
        return SQ_OK;
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    SignalWrapper(const SignalWrapper & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    SignalWrapper(SignalWrapper && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    SignalWrapper & operator = (const SignalWrapper & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    SignalWrapper & operator = (SignalWrapper && o) = delete;

};

/* ------------------------------------------------------------------------------------------------
 * Helper functor to locate specific slots.
*/
template < class Slot > struct MatchSlot
{
    // --------------------------------------------------------------------------------------------
    const SQHash mThisHash; // The environment to search for.
    const SQHash mFuncHash; // The callback to search for.

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    MatchSlot(SQHash t, SQHash f)
        : mThisHash(t), mFuncHash(f)
    {
        //...
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator.
    */
    inline bool operator () (const Slot & s) const
    {
        return (mThisHash == s.mThisHash) && (mFuncHash == s.mFuncHash);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Helper functor to locate slots with specific environments.
*/
template < class Slot > struct MatchThis
{
    // --------------------------------------------------------------------------------------------
    const SQHash mThisHash; // The environment to search for.

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit MatchThis(SQHash t)
        : mThisHash(t)
    {
        //...
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator.
    */
    inline bool operator () (const Slot & s) const
    {
        return (mThisHash == s.mThisHash);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Helper functor to locate slots with specific callbacks.
*/
template < class Slot > struct MatchFunc
{
    // --------------------------------------------------------------------------------------------
    const SQHash mFuncHash; // The callback to search for.

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit MatchFunc(SQHash f)
        : mFuncHash(f)
    {
        //...
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator.
    */
    inline bool operator () (const Slot & s) const
    {
        return (mFuncHash == s.mFuncHash);
    }
};

/* ------------------------------------------------------------------------------------------------
 * See if a certain slot exists using the provided functor.
*/
template < typename F, class Slot > static bool ExistsIf(F func, Slot * itr, Slot * end)
{
    // Process slots within the given range
    while (itr != end)
    {
        // Does this slot satisfy the functor?
        if (func(*(itr++)))
        {
            return true;
        }
    }
    return false;
}

/* ------------------------------------------------------------------------------------------------
 * Count the number of slots that the provided functor deems worthy.
*/
template < typename F, class Slot > static Signal::SizeType CountIf(F func, Slot * itr, Slot * end)
{
    Signal::SizeType count = 0;
    // Process slots within the given range
    while (itr != end)
    {
        // Should we count this slot?
        if (func(*(itr++)))
        {
            ++count;
        }
    }
    // Return the final count
    return count;
}

/* ------------------------------------------------------------------------------------------------
 * Remove all slot from the specified range if the provided functor demands it.
*/
template < typename F, class Slot, class Scope >
static Signal::SizeType RemoveIf(F func, Slot * itr, Slot * end, Scope * scope)
{
    Slot * dest = itr;
    // Number of removed slots
    Signal::SizeType count = 0;
    // Process slots within the given range
    while (itr != end)
    {
        // Should we remove this slot?
        if (func(*itr))
        {
            // Release the stored references
            itr->Release();
            // Are we currently signaling?
            if (scope != nullptr)
            {
                scope->Descend(itr); // Update iterators
            }
            // Increase the counter
            ++count;
        }
        // Were there any slots removed?
        else if (itr != dest)
        {
            // Offset the current scope
            *dest = std::move(*itr);
            // Advance the destination
            ++dest;
        }
        else
        {
            ++dest; // We avoided a move assignment
        }
        // Advance the iterator
        ++itr;
    }
    // Return the final count
    return count;
}

/* ------------------------------------------------------------------------------------------------
 * Move to the front all slots that the provided functor demands.
*/
template < typename F, class Slot, class Scope >
static void LeadIf(F func, Slot * front, Slot * end, bool one, bool append, Scope * scope)
{
    Slot * itr = front;
    // Process slots within the given range
    while (itr != end)
    {
        // Should this slot become a lead?
        if (!func(*itr))
        {
            ++itr;
            // Skip it
            continue;
        }
        // Is this slot right behind the current lead?
        else if ((itr - front) == 1)
        {
            // Swap them
            itr->Swap(*front);
            // Are we currently signaling?
            if (scope != nullptr)
            {
                scope->Lead(itr); // Update iterators
            }
        }
        // Is this not the current lead?
        else if (itr != front)
        {
            // Backup the values of this slot
            Slot tmp(std::move(*itr));
            // Shift back all the slots before it
            for (Slot * dest = itr, * src = (itr - 1); dest != front; dest = src, --src)
            {
                dest->Swap(*src); // Swap with the one bellow it
            }
            // Finally, place the slot into lead
            *front = std::move(tmp);
            // Are we currently signaling?
            if (scope != nullptr)
            {
                scope->Lead(itr); // Update iterators
            }
        }
        // Advance the iterator
        ++itr;
        // Are we using the append method?
        if (append)
        {
            ++front; // Don't overrule the current lead
        }
        // Should we make only one lead?
        if (one)
        {
            break; // Then stop here!
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Move to the back all slots that the provided functor demands.
*/
template < typename F, class Slot, class Scope >
static void TailIf(F func, Slot * front, Slot * back, bool one, bool append, Scope * scope)
{
    Slot * itr = back;
    // Process slots within the given range
    while (itr >= front)
    {
        // Should this slot become a tail?
        if (!func(*itr))
        {
            --itr;
            // Skip it
            continue;
        }
        // Is this slot right behind the current tail?
        else if ((back - itr) == 1)
        {
            // Swap them
            itr->Swap(*front);
            // Are we currently signaling?
            if (scope != nullptr)
            {
                scope->Tail(itr); // Update iterators
            }
        }
        // Is this not the current tail?
        else if (itr != back)
        {
            // Backup the values of this slot
            Slot tmp(std::move(*itr));
            // Shift up all the slots before it
            for (Slot * dest = itr, * src = (itr + 1); dest != back; dest = src, ++src)
            {
                dest->Swap(*src); // Swap with the one above it
            }
            // Finally, place the slot into tail
            *back = std::move(tmp);
            // Are we currently signaling?
            if (scope != nullptr)
            {
                scope->Tail(itr); // Update iterators
            }
        }
        // Advance the iterator
        --itr;
        // Are we using the append method?
        if (append)
        {
            --back; // Don't overrule the current tail
        }
        // Should we make only one tail?
        if (one)
        {
            break; // Then stop here!
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Signal::Scope::Descend(Pointer ptr)
{
    // Is the descended pointer withing this scope?
    if (ptr <= mEnd)
    {
        // Is the iterator affected by this?
        if (ptr < mItr || mItr > mEnd)
        {
            --mItr;
        }
        // Update the end iterator
        --mEnd;
    }
    // Is this that last scope?
    if (mChild != nullptr)
    {
        mChild->Descend(ptr); // Let the others know as well
    }
}

// ------------------------------------------------------------------------------------------------
void Signal::Scope::Lead(Pointer ptr)
{
    // Is the descended pointer in our scope?
    if (ptr <= mEnd)
    {
        // Does it affect our iterator?
        if (ptr >= mItr && mItr != mEnd)
        {
            ++mItr;
        }
    }
    // Is the descended pointer out of our scope?
    else if (ptr > mEnd)
    {
        ++mItr, ++mEnd;
    }
    // Is this that last scope?
    if (mChild != nullptr)
    {
        mChild->Lead(ptr); // Let the others know as well
    }
}

// ------------------------------------------------------------------------------------------------
void Signal::Scope::Tail(Pointer ptr)
{
    // Is the descended pointer in our scope?
    if (ptr <= mEnd)
    {
        // Did this scope already finish processing?
        if (mItr != mEnd)
        {
            // Does it affect our iterator?
            if (ptr < mItr)
            {
                --mItr;
            }
            // Update the end iterator
            --mEnd;
        }
    }
    // Is this that last scope?
    if (mChild != nullptr)
    {
        mChild->Tail(ptr); // Let the others know as well
    }
}

// ------------------------------------------------------------------------------------------------
void Signal::Scope::Finish()
{
    // Forcefully skip all remaining slots
    mItr = mEnd;
    // Is this that last scope?
    if (mChild != nullptr)
    {
        mChild->Finish(); // Let the others know as well
    }
}

// ------------------------------------------------------------------------------------------------
Signal::Signal()
    : m_Used(0)
    , m_Size(SMB_SIZE)
    , m_Slots(m_SMB)
    , m_Scope(nullptr)
    , m_Name()
    , m_Data()
{
    s_FreeSignals.push_back(this);
}

// ------------------------------------------------------------------------------------------------
Signal::Signal(String && name)
    : m_Used(0)
    , m_Size(SMB_SIZE)
    , m_Slots(m_SMB)
    , m_Scope(nullptr)
    , m_Name(std::forward< String >(name))
    , m_Data()
{
    if (m_Name.empty())
    {
        s_FreeSignals.push_back(this);
    }
}
// ------------------------------------------------------------------------------------------------
Signal::~Signal()
{
    ClearSlots();
    // Should we erase this from the fee signals list?
    if (m_Name.empty())
    {
        s_FreeSignals.erase(std::remove(s_FreeSignals.begin(), s_FreeSignals.end(), this),
                            s_FreeSignals.end());
    }
}

// ------------------------------------------------------------------------------------------------
bool Signal::AdjustSlots(SizeType capacity)
{
    // Is it necessary to resize?
    if (capacity <= m_Size)
    {
        return true; // Already have that memory available!
    }
    // Do not alter the current capacity
    SizeType size = m_Size;
    // Calculate the next optimal size of the buffer
    while (size < capacity)
    {
        size += (size + 1u) >> 1u;
    }
    // Attempt to allocate a memory buffer of the resulted size
    auto slots = reinterpret_cast< Pointer >(new uint8_t[size * sizeof(Slot)]);
    // See if the memory could be allocated
    if (slots == nullptr)
    {
        return false; // Unable to acquire the memory!
    }
    // Do not alter the pointer to the new buffer
    Pointer dest = slots;
    // Are there any existing slots?
    if (m_Used)
    {
        // Grab the range of slots to be transferred
        Pointer src = m_Slots, end = (m_Slots + m_Used);
        // Transfer the existing slots
        while (src != end)
        {
            // Transfer to the new buffer
            new (dest++) Slot(std::move(*(src)));
            // Destroy the old instance
            (src++)->~Slot();
        }
        // Grab the end of the remaining slots
        end = (m_Slots + m_Size);
        // Destroy the remaining slots
        while (src != end)
        {
            (src++)->~Slot();
        }
    }
    // Grab the end of the new buffer
    Pointer end = (slots + size);
    // Initialize the remaining slots
    while (dest != end)
    {
        new (dest++) Slot();
    }
    // Update the iterators from current scopes
    for (Scope * scope = m_Scope; scope != nullptr; scope = scope->mChild)
    {
        scope->mItr = slots + (scope->mItr - m_Slots);
        scope->mEnd = slots + (scope->mEnd - m_Slots);
    }
    // Should we delete the current buffer?
    if (m_Slots != m_SMB)
    {
        delete[] reinterpret_cast< uint8_t * >(m_Slots);
    }
    // Assign the new buffer
    m_Slots = slots;
    // Assign the new capacity
    m_Size = size;
    // The buffer was successfully adjusted
    return true;
}

// ------------------------------------------------------------------------------------------------
void Signal::ClearSlots()
{
    // Release every connected slot
    for (Pointer itr = m_Slots, end = m_Slots + m_Used; itr != end; ++itr)
    {
        itr->Release();
    }
    // Are we currently signaling?
    if (m_Scope != nullptr)
    {
        m_Scope->Finish(); // Update iterators
    }
    // Inform the signal that these slots are now NULL
    m_Used = 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::Connect(SignalWrapper & w)
{
    // Make sure we have enough space to store the slot
    if ((m_Used < m_Size) || AdjustSlots(m_Used + 1))
    {
        m_Slots[m_Used++].Swap(w.mSlot); // Connect the slot to the signal
    }
    else
    {
        sq_throwerror(w.mVM, "Unable to acquire enough memory");
    }
    // Specify that we don't return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::ConnectOnce(SignalWrapper & w)
{
    // Disconnect every occurrence of this slot first
    w.mRes = Eliminate(w);
    // Did we fail to disconnect it?
    if (SQ_FAILED(w.mRes))
    {
        return w.mRes; // Propagate the error
    }
    // Finally, attempt to connect it again
    return Connect(w);
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal:: Disconnect(SignalWrapper & w)
{
    return Eliminate(w);
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::Exists(SignalWrapper & w)
{
    // Forward the call to the actual function
    const bool r = ExistsIf(MatchSlot< Slot >(w.mSlot.mThisHash, w.mSlot.mFuncHash),
                                    m_Slots, m_Slots + m_Used);
    // Push the resulted value on the stack
    sq_pushbool(w.mVM, static_cast< SQBool >(r));
    // Specify that we returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::ExistsThis(SignalWrapper & w)
{
    // Forward the call to the actual function
    const bool r = ExistsIf(MatchThis< Slot >(w.mSlot.mThisHash), m_Slots, m_Slots + m_Used);
    // Push the resulted value on the stack
    sq_pushbool(w.mVM, static_cast< SQBool >(r));
    // Specify that we returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::ExistsFunc(SignalWrapper & w)
{
    // Forward the call to the actual function
    const bool r = ExistsIf(MatchFunc< Slot >(w.mSlot.mFuncHash), m_Slots, m_Slots + m_Used);
    // Push the resulted value on the stack
    sq_pushbool(w.mVM, static_cast< SQBool >(r));
    // Specify that we returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::Count(SignalWrapper & w)
{
    // Forward the call to the actual function
    const SizeType r = CountIf(MatchSlot< Slot >(w.mSlot.mThisHash, w.mSlot.mFuncHash),
                                    m_Slots, m_Slots + m_Used);
    // Push the resulted value on the stack
    sq_pushinteger(w.mVM, static_cast< SQInteger >(r));
    // Specify that we returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::CountThis(SignalWrapper & w)
{
    // Forward the call to the actual function
    const SizeType r = CountIf(MatchThis< Slot >(w.mSlot.mThisHash), m_Slots, m_Slots + m_Used);
    // Push the resulted value on the stack
    sq_pushinteger(w.mVM, static_cast< SQInteger >(r));
    // Specify that we returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::CountFunc(SignalWrapper & w)
{
    // Forward the call to the actual function
    const SizeType r = CountIf(MatchFunc< Slot >(w.mSlot.mFuncHash), m_Slots, m_Slots + m_Used);
    // Push the resulted value on the stack
    sq_pushinteger(w.mVM, static_cast< SQInteger >(r));
    // Specify that we returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::Lead(SignalWrapper & w)
{
    // Make sure that there's more than one slot connected
    if (m_Used > 1)
    {
        LeadIf(MatchSlot< Slot >(w.mSlot.mThisHash, w.mSlot.mFuncHash),
                m_Slots, m_Slots + m_Used, w.mOne, w.mAppend, m_Scope);
    }
    // Specify that we don't return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::LeadThis(SignalWrapper & w)
{
    // Make sure that there's more than one slot connected
    if (m_Used > 1)
    {
        LeadIf(MatchThis< Slot >(w.mSlot.mThisHash),
                m_Slots, m_Slots + m_Used, w.mOne, w.mAppend, m_Scope);
    }
    // Specify that we don't return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::LeadFunc(SignalWrapper & w)
{
    // Make sure that there's more than one slot connected
    if (m_Used > 1)
    {
        LeadIf(MatchFunc< Slot >(w.mSlot.mFuncHash),
                m_Slots, m_Slots + m_Used, w.mOne, w.mAppend, m_Scope);
    }
    // Specify that we don't return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::Tail(SignalWrapper & w)
{
    // Make sure that there's more than one slot connected
    if (m_Used > 1)
    {
        TailIf(MatchSlot< Slot >(w.mSlot.mThisHash, w.mSlot.mFuncHash),
                m_Slots, m_Slots + m_Used, w.mOne, w.mAppend, m_Scope);
    }
    // Specify that we don't return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::TailThis(SignalWrapper & w)
{
    // Make sure that there's more than one slot connected
    if (m_Used > 1)
    {
        TailIf(MatchThis< Slot >(w.mSlot.mThisHash),
                m_Slots, m_Slots + m_Used, w.mOne, w.mAppend, m_Scope);
    }
    // Specify that we don't return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::TailFunc(SignalWrapper & w)
{
    // Make sure that there's more than one slot connected
    if (m_Used > 1)
    {
        TailIf(MatchFunc< Slot >(w.mSlot.mFuncHash),
                m_Slots, m_Slots + m_Used, w.mOne, w.mAppend, m_Scope);
    }
    // Specify that we don't return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::Eliminate(SignalWrapper & w)
{
    // Make sure that there's at least one slot connected
    if (m_Used != 0)
    {
        // Backup the current number of used slots
        const SizeType count = m_Used;
        // Forward the call to the actual function
        m_Used -= RemoveIf(MatchSlot< Slot >(w.mSlot.mThisHash, w.mSlot.mFuncHash),
                    m_Slots, m_Slots + m_Used, m_Scope);
        // Push the number of removed slots
        sq_pushinteger(w.mVM, static_cast< SQInteger >(count - m_Used));
    }
    else
    {
        sq_pushinteger(w.mVM, 0);
    }

    // Specify that we returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::EliminateThis(SignalWrapper & w)
{
    // Make sure that there's at least one slot connected
    if (m_Used != 0)
    {
        // Backup the current number of used slots
        const SizeType count = m_Used;
        // Forward the call to the actual function
        m_Used -= RemoveIf(MatchThis< Slot >(w.mSlot.mThisHash), m_Slots, m_Slots + m_Used, m_Scope);
        // Push the number of removed slots
        sq_pushinteger(w.mVM, static_cast< SQInteger >(count - m_Used));
    }
    else
    {
        sq_pushinteger(w.mVM, 0);
    }

    // Specify that we returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::EliminateFunc(SignalWrapper & w)
{
    // Make sure that there's at least one slot connected
    if (m_Used != 0)
    {
        // Backup the current number of used slots
        const SizeType count = m_Used;
        // Forward the call to the actual function
        m_Used -= RemoveIf(MatchFunc< Slot >(w.mSlot.mFuncHash), m_Slots, m_Slots + m_Used, m_Scope);
        // Push the number of removed slots
        sq_pushinteger(w.mVM, static_cast< SQInteger >(count - m_Used));
    }
    else
    {
        sq_pushinteger(w.mVM, 0);
    }

    // Specify that we returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
#define SQMOD_SIGNAL_CONTROL_WRAPPER(_f, _e) /*
*/ SQInteger Signal::Sq##_f(HSQUIRRELVM vm) { /*
*/  SignalWrapper w(vm, _e); /*
*/  if (SQ_FAILED(w.mRes)) return w.mRes; /*
*/  else return w.mSignal->_f(w); /*
*/ } /*
*/ /*
*/

// ------------------------------------------------------------------------------------------------
SQMOD_SIGNAL_CONTROL_WRAPPER(Connect, false)
SQMOD_SIGNAL_CONTROL_WRAPPER(ConnectOnce, false)
SQMOD_SIGNAL_CONTROL_WRAPPER(Exists, false)
SQMOD_SIGNAL_CONTROL_WRAPPER(Disconnect, false)
SQMOD_SIGNAL_CONTROL_WRAPPER(ExistsThis, false)
SQMOD_SIGNAL_CONTROL_WRAPPER(ExistsFunc, false)
SQMOD_SIGNAL_CONTROL_WRAPPER(Count, false)
SQMOD_SIGNAL_CONTROL_WRAPPER(CountThis, false)
SQMOD_SIGNAL_CONTROL_WRAPPER(CountFunc, false)
SQMOD_SIGNAL_CONTROL_WRAPPER(Lead, true)
SQMOD_SIGNAL_CONTROL_WRAPPER(LeadThis, true)
SQMOD_SIGNAL_CONTROL_WRAPPER(LeadFunc, true)
SQMOD_SIGNAL_CONTROL_WRAPPER(Tail, true)
SQMOD_SIGNAL_CONTROL_WRAPPER(TailThis, true)
SQMOD_SIGNAL_CONTROL_WRAPPER(TailFunc, true)
SQMOD_SIGNAL_CONTROL_WRAPPER(Eliminate, false)
SQMOD_SIGNAL_CONTROL_WRAPPER(EliminateThis, false)
SQMOD_SIGNAL_CONTROL_WRAPPER(EliminateFunc, false)

// ------------------------------------------------------------------------------------------------
SQInteger Signal::Emit(HSQUIRRELVM vm, SQInteger top)
{
    // Are there any slots connected?
    if (!m_Used) return 0;
    // Enter a new execution scope
    Scope scope(m_Scope, m_Slots, m_Slots + m_Used);
    // Activate the current scope and create a guard to restore it
    const AutoAssign< Scope * > aa(m_Scope, scope.mParent, &scope);
    // Contains the last received result
    SQRESULT res = SQ_OK;
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
        // Are there any parameters to forward?
        if (top > 1)
        {
            for (SQInteger i = 2; i <= top; ++i)
            {
                sq_push(vm, i);
            }
        }
        // Make the function call and store the result
        res = sq_call(vm, top, static_cast< SQBool >(false), static_cast< SQBool >(ErrorHandling::IsEnabled()));
        // Pop the callback object from the stack
        sq_pop(vm, 1);
        // Validate the result
        if (SQ_FAILED(res))
        {
            break; // Stop emitting signals
        }
    }
    // Return the last result
    return res;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::Query(HSQUIRRELVM vm, SQInteger top)
{
    // Are there any slots connected?
    if (!m_Used) return 0;
    // The collector and the specified environment
    HSQOBJECT cthis, cfunc;
    // Attempt to grab the collector environment
    SQRESULT res = sq_getstackobj(vm, 2, &cthis);
    // Validate the result
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error
    }
    // Was there a valid environment?
    else if (sq_isnull(cthis))
    {
        // Default to the root table
        sq_pushroottable(vm);
        // Try to grab the collector environment again
        res = sq_getstackobj(vm, -1, &cthis);
        // Pop the root table from the stack
        sq_pop(vm, 1);
        // Validate the result
        if (SQ_FAILED(res))
        {
            return res; // Propagate the error
        }
    }
    // Grab the collector callback
    res = sq_getstackobj(vm, 3, &cfunc);
    // Validate the result
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error
    }
    // Some dummy checks to make sure the collector is a callable object
    else if (!(sq_type(cfunc) & (_RT_CLOSURE | _RT_NATIVECLOSURE)))
    {
        return sq_throwerror(vm, "Invalid collector callback");
    }
    // Enter a new execution scope
    Scope scope(m_Scope, m_Slots, m_Slots + m_Used);
    // Activate the current scope and create a guard to restore it
    const AutoAssign< Scope * > aa(m_Scope, scope.mParent, &scope);
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
        // Are there any parameters to forward?
        if (top > 3)
        {
            for (SQInteger i = 4; i <= top; ++i)
            {
                sq_push(vm, i);
            }
        }
        // Make the function call and store the result
        res = sq_call(vm, top-2, static_cast< SQBool >(true), static_cast< SQBool >(ErrorHandling::IsEnabled()));
        // Validate the result
        if (SQ_FAILED(res))
        {
            // Pop the callback object and return value from the stack
            sq_pop(vm, 2);
            // Stop emitting signals
            break;
        }
        // Push the collector onto the stack
        sq_pushobject(vm, cfunc);
        sq_pushobject(vm, cthis);
        // Push the returned value
        sq_push(vm, -3);
        // Make the function call and store the result
        res = sq_call(vm, 2, static_cast< SQBool >(false), static_cast< SQBool >(ErrorHandling::IsEnabled()));
        // Pop the callback object, return value and collector from the stack
        sq_pop(vm, 3);
        // Validate the result
        if (SQ_FAILED(res))
        {
            break; // Stop emitting signals
        }
    }
    // Return the last result
    return res;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::Consume(HSQUIRRELVM vm, SQInteger top)
{
    // Are there any slots connected?
    if (!m_Used) return 0;
    // Enter a new execution scope
    Scope scope(m_Scope, m_Slots, m_Slots + m_Used);
    // Activate the current scope and create a guard to restore it
    const AutoAssign< Scope * > aa(m_Scope, scope.mParent, &scope);
    // Contains the last received result
    SQRESULT res = SQ_OK;
    // Default to not consumed
    SQBool ret = SQFalse;
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
        // Are there any parameters to forward?
        if (top > 1)
        {
            for (SQInteger i = 2; i <= top; ++i)
            {
                sq_push(vm, i);
            }
        }
        // Make the function call and store the result
        res = sq_call(vm, top, static_cast< SQBool >(true), static_cast< SQBool >(ErrorHandling::IsEnabled()));
        // Validate the result
        if (SQ_FAILED(res))
        {
            // Pop the callback object and return value from the stack
            sq_pop(vm, 2);
            // Stop emitting signals
            break;
        }
        // Is the returned value not null?
        else if (sq_gettype(vm, -1) != OT_NULL)
        {
            // Obtain the returned value
            sq_tobool(vm, -1, &ret);
            // Should we proceed to the next slot or stop here?
            if (ret == SQTrue)
            {
                // Pop the callback object and return value from the stack
                sq_pop(vm, 2);
                // The slot consumed the signal
                break;
            }
        }
        // Pop the callback object and return value from the stack
        sq_pop(vm, 2);
    }
    // Did we fail to process slots?
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error
    }
    // Forward the returned value to the invoker
    sq_pushbool(vm, ret);
    // Specify that we returned something
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::Approve(HSQUIRRELVM vm, SQInteger top)
{
    // Are there any slots connected?
    if (!m_Used) return 0;
    // Enter a new execution scope
    Scope scope(m_Scope, m_Slots, m_Slots + m_Used);
    // Activate the current scope and create a guard to restore it
    const AutoAssign< Scope * > aa(m_Scope, scope.mParent, &scope);
    // Contains the last received result
    SQRESULT res = SQ_OK;
    // Default to approved
    SQBool ret = SQTrue;
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
        // Are there any parameters to forward?
        if (top > 1)
        {
            for (SQInteger i = 2; i <= top; ++i)
            {
                sq_push(vm, i);
            }
        }
        // Make the function call and store the result
        res = sq_call(vm, top, static_cast< SQBool >(true), static_cast< SQBool >(ErrorHandling::IsEnabled()));
        // Validate the result
        if (SQ_FAILED(res))
        {
            // Pop the callback object and return value from the stack
            sq_pop(vm, 2);
            // Stop emitting signals
            break;
        }
        // Is the returned value not null?
        else if (sq_gettype(vm, -1) != OT_NULL)
        {
            // Obtain the returned value
            sq_tobool(vm, -1, &ret);
            // Should we proceed to the next slot or stop here?
            if (ret == SQFalse)
            {
                // Pop the callback object and return value from the stack
                sq_pop(vm, 2);
                // The slot did not approve the signal
                break;
            }
        }
        // Pop the callback object and return value from the stack
        sq_pop(vm, 2);
    }
    // Did we fail to process slots?
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error
    }
    // Forward the returned value to the invoker
    sq_pushbool(vm, ret);
    // Specify that we returned something
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::Request(HSQUIRRELVM vm, SQInteger top)
{
    // Are there any slots connected?
    if (!m_Used) return 0;
    // Enter a new execution scope
    Scope scope(m_Scope, m_Slots, m_Slots + m_Used);
    // Activate the current scope and create a guard to restore it
    const AutoAssign< Scope * > aa(m_Scope, scope.mParent, &scope);
    // Contains the last received result
    SQRESULT res = SQ_OK;
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
        // Are there any parameters to forward?
        if (top > 1)
        {
            for (SQInteger i = 2; i <= top; ++i)
            {
                sq_push(vm, i);
            }
        }
        // Make the function call and store the result
        res = sq_call(vm, top, static_cast< SQBool >(true), static_cast< SQBool >(ErrorHandling::IsEnabled()));
        // Validate the result
        if (SQ_FAILED(res))
        {
            // Pop the callback object and return value from the stack
            sq_pop(vm, 2);
            // Stop emitting signals
            break;
        }
        // Is the returned value not null?
        else if (sq_gettype(vm, -1) != OT_NULL)
        {
            // Remove the callback object from the stack
            sq_remove(vm, -2);
            // Specify that we returned something
            res = 1;
            // The slot did not approve the signal
            break;
        }
        // Pop the callback object and return value from the stack
        sq_pop(vm, 2);
    }
    // Return the last result
    return res;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::SqEmit(HSQUIRRELVM vm)
{
    const SQInteger top = sq_gettop(vm);
    // Contains the last received result
    SQRESULT res;
    // Attempt to forward the call to the signal instance
    try
    {
        // Attempt to grab the signal instance from the stack
        Signal * signal = Var< Signal * >(vm, 1).value;
        // Do we have a valid signal instance?
        if (!signal)
        {
            res = sq_throwerror(vm, "Invalid signal instance");
        }
        // Forward the call to the signal instance
        else
        {
            res = signal->Emit(vm, top);
        }
    }
    catch (const Sqrat::Exception & e)
    {
        res = sq_throwerror(vm, e.what());
    }
    // The execution was successful
    return res;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::SqQuery(HSQUIRRELVM vm)
{
    const SQInteger top = sq_gettop(vm);
    // Do we have the collector environment?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing collector environment");
    }
    // Do we have the collector callback?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing collector callback");
    }
    // Contains the last received result
    SQRESULT res;
    // Attempt to forward the call to the signal instance
    try
    {
        // Attempt to grab the signal instance from the stack
        Signal * signal = Var< Signal * >(vm, 1).value;
        // Do we have a valid signal instance?
        if (!signal)
        {
            res = sq_throwerror(vm, "Invalid signal instance");
        }
        // Forward the call to the signal instance
        else
        {
            res = signal->Query(vm, top);
        }
    }
    catch (const Sqrat::Exception & e)
    {
        res = sq_throwerror(vm, e.what());
    }
    // The execution was successful
    return res;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::SqConsume(HSQUIRRELVM vm)
{
    const SQInteger top = sq_gettop(vm);
    // Contains the last received result
    SQRESULT res;
    // Attempt to forward the call to the signal instance
    try
    {
        // Attempt to grab the signal instance from the stack
        Signal * signal = Var< Signal * >(vm, 1).value;
        // Do we have a valid signal instance?
        if (!signal)
        {
            res = sq_throwerror(vm, "Invalid signal instance");
        }
        // Forward the call to the signal instance
        else
        {
            res = signal->Consume(vm, top);
        }
    }
    catch (const Sqrat::Exception & e)
    {
        res = sq_throwerror(vm, e.what());
    }
    // The execution was successful
    return res;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::SqApprove(HSQUIRRELVM vm)
{
    const SQInteger top = sq_gettop(vm);
    // Contains the last received result
    SQRESULT res;
    // Attempt to forward the call to the signal instance
    try
    {
        // Attempt to grab the signal instance from the stack
        Signal * signal = Var< Signal * >(vm, 1).value;
        // Do we have a valid signal instance?
        if (!signal)
        {
            res = sq_throwerror(vm, "Invalid signal instance");
        }
        // Forward the call to the signal instance
        else
        {
            res = signal->Approve(vm, top);
        }
    }
    catch (const Sqrat::Exception & e)
    {
        res = sq_throwerror(vm, e.what());
    }
    // The execution was successful
    return res;
}

// ------------------------------------------------------------------------------------------------
SQInteger Signal::SqRequest(HSQUIRRELVM vm)
{
    const SQInteger top = sq_gettop(vm);
    // Contains the last received result
    SQRESULT res;
    // Attempt to forward the call to the signal instance
    try
    {
        // Attempt to grab the signal instance from the stack
        Signal * signal = Var< Signal * >(vm, 1).value;
        // Do we have a valid signal instance?
        if (!signal)
        {
            res = sq_throwerror(vm, "Invalid signal instance");
        }
        // Forward the call to the signal instance
        else
        {
            res = signal->Request(vm, top);
        }
    }
    catch (const Sqrat::Exception & e)
    {
        res = sq_throwerror(vm, e.what());
    }
    // The execution was successful
    return res;
}

// ------------------------------------------------------------------------------------------------
void Signal::Terminate()
{
    // Terminate named signals
    for (const auto & s : s_Signals)
    {
        // Clear slots
        s.second.first->ClearSlots();
        // Release the name
        s.second.first->m_Name.clear();
        // Release whatever is in the user data
        s.second.first->m_Data.Release();
    }
    // Finally clear the container itself
    s_Signals.clear();
    // Create a copy so we don't invalidate iterators when destructor removes the instances
    FreeSignals fsig(s_FreeSignals);
    // Terminate anonymous signals
    for (const auto & s : fsig)
    {
        // Clear slots
        s->ClearSlots();
        // Release whatever is in the user data
        s->m_Data.Release();
    }
    // Finally clear the container itself
    s_FreeSignals.clear();
}

// ------------------------------------------------------------------------------------------------
LightObj Signal::CreateFree()
{
    // Remember the current stack size
    const StackGuard sg;
    // Create the signal instance
    DeleteGuard< Signal > dg(new Signal());
    // Attempt to create the signal instance
    ClassType< Signal >::PushInstance(SqVM(), dg.Get());
    // This is now managed by the script
    dg.Release();
    // Return the created signal
    return Var< LightObj >(SqVM(), -1).value;
}

// ------------------------------------------------------------------------------------------------
LightObj Signal::Create(StackStrF & name)
{
    // Validate the signal name
    if (name.mLen <= 0)
    {
        return CreateFree();
    }
    // Create a copy of the name
    String sname(name.mPtr, static_cast< size_t >(name.mLen));
    // Compute the hash of the specified name
    const std::size_t hash = std::hash< String >{}(sname);
    // See if the signal already exists
    for (const auto & e : s_Signals)
    {
        if (e.first == hash)
        {
            return LightObj{e.second.second.mObj}; // Found a match so let's return it
        }
    }
    // Remember the current stack size
    const StackGuard sg;
    // Create the signal instance
    DeleteGuard< Signal > dg(new Signal(std::move(sname)));
    // Grab the signal instance pointer
    Signal * ptr = dg.Get();
    // Attempt to create the signal instance
    ClassType< Signal >::PushInstance(SqVM(), ptr);
    // This is now managed by the script
    dg.Release();
    // Grab a reference to the instance created on the stack
    s_Signals.emplace_back(hash, SignalPair(ptr, Var< LightObj >(SqVM(), -1).value));
    // Return the created signal
    return LightObj{s_Signals.back().second.second.mObj};
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
    const String sname(name.mPtr, static_cast< size_t >(name.mLen));
    // Compute the hash of the specified name
    const std::size_t hash = std::hash< String >{}(sname);
    // Iterator to the existing signal, if any
    auto itr = s_Signals.cbegin();
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
        itr->second.first->m_Name.clear();
        // Put it on the free list
        s_FreeSignals.push_back(itr->second.first);
        // Finally, remove it from the named list
        s_Signals.erase(itr);
    }
}

// ------------------------------------------------------------------------------------------------
const LightObj & Signal::Fetch(StackStrF & name)
{
    // Validate the signal name
    if (name.mLen <= 0)
    {
        STHROWF("Signals without names cannot be retrieved manually");
    }
    // Create a copy of the name
    const String sname(name.mPtr, static_cast< size_t >(name.mLen));
    // Compute the hash of the specified name
    const std::size_t hash = std::hash< String >{}(sname);
    // Search for a signal with this name
    for (const auto & e : s_Signals)
    {
        if (e.first == hash)
        {
            return e.second.second; // Found a match so let's return it
        }
    }
    // No such signal exists
    STHROWF("Unknown signal named (%s)", sname.c_str());
    // SHOULD NOT REACH THIS POINT!
    static LightObj slo;
    return slo;
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to terminate the signals.
*/
void TerminateSignals()
{
    Signal::Terminate();
}

// ------------------------------------------------------------------------------------------------
void InitSignalPair(SignalPair & sp, LightObj & et, const char * name)
{
    // Remember the current stack size
    const StackGuard sg;
    // Create the signal instance
    DeleteGuard< Signal > dg(new Signal());
    // Attempt to create the signal instance object
    sp.second = LightObj(dg.Get());
    // Assign the signal instance itself
    sp.first = dg.Get();
    // This is now managed by the script
    dg.Release();
    // Should we bind this to a certain object?
    if (name != nullptr)
    {
        et.Bind(name, sp.second); // Bind the signal to the specified object
    }
}

// ------------------------------------------------------------------------------------------------
void ResetSignalPair(SignalPair & sp, bool clear)
{
    // See if the slots must be cleared as well
    if (clear && sp.first != nullptr)
    {
        sp.first->ClearSlots();
    }
    // Reset the signal pair
    sp.first = nullptr;
    sp.second.Release();
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
        .Func(_SC("Name"), &Signal::ToString)
        .Prop(_SC("Slots"), &Signal::GetUsed)
        .Prop(_SC("Empty"), &Signal::IsEmpty)
        // Core Methods
        .Func(_SC("Clear"), &Signal::ClearSlots)
        // Squirrel Functions
        .SquirrelFunc(_SC("Connect"), &Signal::SqConnect)
        .SquirrelFunc(_SC("ConnectOnce"), &Signal::SqConnectOnce)
        .SquirrelFunc(_SC("Exists"), &Signal::SqExists)
        .SquirrelFunc(_SC("Disconnect"), &Signal::SqDisconnect)
        .SquirrelFunc(_SC("ExistsThis"), &Signal::SqExistsThis)
        .SquirrelFunc(_SC("ExistsFunc"), &Signal::SqExistsFunc)
        .SquirrelFunc(_SC("Count"), &Signal::SqCount)
        .SquirrelFunc(_SC("CountThis"), &Signal::SqCountThis)
        .SquirrelFunc(_SC("CountFunc"), &Signal::SqCountFunc)
        .SquirrelFunc(_SC("Lead"), &Signal::SqLead)
        .SquirrelFunc(_SC("LeadThis"), &Signal::SqLeadThis)
        .SquirrelFunc(_SC("LeadFunc"), &Signal::SqLeadFunc)
        .SquirrelFunc(_SC("Tail"), &Signal::SqTail)
        .SquirrelFunc(_SC("TailThis"), &Signal::SqTailThis)
        .SquirrelFunc(_SC("TailFunc"), &Signal::SqTailFunc)
        .SquirrelFunc(_SC("Eliminate"), &Signal::SqEliminate)
        .SquirrelFunc(_SC("EliminateThis"), &Signal::SqEliminateThis)
        .SquirrelFunc(_SC("EliminateFunc"), &Signal::SqEliminateFunc)
        .SquirrelFunc(_SC("Emit"), &Signal::SqEmit)
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

} // Namespace:: SqMod
