#ifndef _ROUTINE_HPP_
#define _ROUTINE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include <array>
#include <utility>
#include <unordered_map>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Execute callbacks after specific intervals of time.
*/
class Routine
{
public:

    /* --------------------------------------------------------------------------------------------
     * Simplify future changes to a single point of change.
    */
    typedef Int64                                       Time;
    typedef SQInteger                                   Interval;
    typedef Uint32                                      Iterator;
    typedef std::pair< Interval, Routine * >            Element;
    typedef std::array< Element, SQMOD_MAX_ROUTINES >   Routines;
    typedef std::unordered_map< Routine *, Object >     Objects;

    /* --------------------------------------------------------------------------------------------
     * Process all active routines and update elapsed time.
    */
    static void Process();

    /* --------------------------------------------------------------------------------------------
     * Initialize all resources and prepare for startup.
    */
    static void Initialize();

    /* --------------------------------------------------------------------------------------------
     * Release all resources and prepare for shutdown.
    */
    static void Deinitialize();

protected:

    // --------------------------------------------------------------------------------------------
    static Time         s_Last; // Last time point.
    static Time         s_Prev; // Previous time point.
    static Routines     s_Routines; // Buckets of routines grouped by similar intervals.
    static Objects      s_Objects; // List of existing routines and their associated object.

protected:

    /* --------------------------------------------------------------------------------------------
     * Create or locate the object for the specified routine and keep a strong reference to it.
    */
    static Object Associate(Routine * routine);

    /* --------------------------------------------------------------------------------------------
     * Release the strong reference associated with the specified routine so it can be destroyed.
    */
    static void Dissociate(Routine * routine);

    /* --------------------------------------------------------------------------------------------
     * See whether the specified routine exists in the pool and references itself.
    */
    static bool Associated(Routine * routine);

    /* --------------------------------------------------------------------------------------------
     * Remove the specified routine from the pool and any associated reference, if any.
    */
    static void Forget(Routine * routine);

    /* --------------------------------------------------------------------------------------------
     * Insert a routine instance into the pool to be processed.
    */
    static void Insert(Routine * routine, bool associate = true);

    /* --------------------------------------------------------------------------------------------
     * Remove a routine instance from the pool to not be processed.
    */
    static void Remove(Routine * routine, bool forget = false);

    /* --------------------------------------------------------------------------------------------
     * Release routine resources.
    */
    void Release();

    /* --------------------------------------------------------------------------------------------
     * Execute the binded callback.
    */
    void Execute();

    /* --------------------------------------------------------------------------------------------
     * See whether this routine is valid otherwise throw an exception.
    */
    void Validate() const
    {
        if (m_Terminated)
        {
            STHROWF("Routine was terminated [%s]", m_Tag.c_str());
        }
    }

private:

    /* --------------------------------------------------------------------------------------------
     * Constructor with just an interval.
    */
    Routine(Object & env, Function & func, Interval interval);

    /* --------------------------------------------------------------------------------------------
     * Constructor with just an interval and explicit iterations.
    */
    Routine(Object & env, Function & func, Interval interval, Iterator iterations);

    /* --------------------------------------------------------------------------------------------
     * Constructor with just an interval, explicit iterations and arguments.
    */
    Routine(Object & env, Function & func, Interval interval, Iterator iterations
            , Object & a1);

    /* --------------------------------------------------------------------------------------------
     * Constructor with just an interval, explicit iterations and arguments.
    */
    Routine(Object & env, Function & func, Interval interval, Iterator iterations
            , Object & a1, Object & a2);

    /* --------------------------------------------------------------------------------------------
     * Constructor with just an interval, explicit iterations and arguments.
    */
    Routine(Object & env, Function & func, Interval interval, Iterator iterations
            , Object & a1, Object & a2, Object & a3);

    /* --------------------------------------------------------------------------------------------
     * Constructor with just an interval, explicit iterations and arguments.
    */
    Routine(Object & env, Function & func, Interval interval, Iterator iterations
            , Object & a1, Object & a2, Object & a3, Object & a4);

    /* --------------------------------------------------------------------------------------------
     * Constructor with just an interval, explicit iterations and arguments.
    */
    Routine(Object & env, Function & func, Interval interval, Iterator iterations
            , Object & a1, Object & a2, Object & a3, Object & a4, Object & a5);

private:

    /* --------------------------------------------------------------------------------------------
     * Number of iterations before self destruct.
    */
    Iterator     m_Iterations;

    /* --------------------------------------------------------------------------------------------
     * Interval between calls.
    */
    Interval    m_Interval;

    /* --------------------------------------------------------------------------------------------
     * The index of the slot in the pool.
    */
    Uint16       m_Slot;

    /* --------------------------------------------------------------------------------------------
     * Number of arguments to forward.
    */
    Uint16       m_Arguments;

    /* --------------------------------------------------------------------------------------------
     * Whether calls should be ignored.
    */
    bool        m_Suspended;

    /* --------------------------------------------------------------------------------------------
     * Whether the routine was terminated.
    */
    bool        m_Terminated;

    /* --------------------------------------------------------------------------------------------
     * The callback to be executed when triggered.
    */
    Function    m_Callback;

    /* --------------------------------------------------------------------------------------------
     * User tag associated with this instance.
    */
    String      m_Tag;

    /* --------------------------------------------------------------------------------------------
     * User data associated with this instance.
    */
    Object      m_Data;

    /* --------------------------------------------------------------------------------------------
     * Arguments to be forwarded to the callback.
    */
    Object      m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                m_Arg8, m_Arg9, m_Arg10, m_Arg11, m_Arg12, m_Arg13, m_Arg14;

public:

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Routine(const Routine & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    Routine(Routine && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Routine();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Routine & operator = (const Routine & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Routine & operator = (Routine && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Routine & o) const
    {
        return Cmp(static_cast< SQInteger >(o.m_Interval));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with an integer.
    */
    Int32 Cmp(SQInteger interval) const
    {
        if (m_Interval == interval)
        {
            return 0;
        }
        else if (m_Interval > interval)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with an float.
    */
    Int32 Cmp(SQFloat interval) const
    {
        return Cmp(static_cast< SQInteger >(interval));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with an string.
    */
    Int32 Cmp(CSStr tag) const
    {
        return m_Tag.compare(tag);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a boolean.
    */
    Int32 Cmp(bool suspended) const
    {
        if (m_Suspended == suspended)
        {
            return 0;
        }
        else if (m_Suspended > suspended)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a null pointer.
    */
    Int32 Cmp(std::nullptr_t) const
    {
        if (m_Terminated == true)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user tag.
    */
    const String & GetTag() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    void SetTag(const StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user data.
    */
    Object & GetData();

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(Object & data);

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag and allow chaining of operations.
    */
    Routine & ApplyTag(const StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data and allow chaining of operations.
    */
    Routine & ApplyData(Object & data);

    /* --------------------------------------------------------------------------------------------
     * Activate the routine instance.
    */
    Routine & Activate();

    /* --------------------------------------------------------------------------------------------
     * Deactivate the routine instance.
    */
    Routine & Deactivate();

    /* --------------------------------------------------------------------------------------------
     * Terminate this routine by deactivating and releasing all resources.
    */
    Routine & Terminate();

    /* --------------------------------------------------------------------------------------------
     * Unmark this routine as terminated and allow to be activated.
    */
    Routine & Reanimate();

    /* --------------------------------------------------------------------------------------------
     * Modify an explicit value to be passed as the specified argument.
    */
    Routine & SetArg(Uint16 num, Object & val);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value that is passed as the specified argument.
    */
    Object & GetArg(Uint16 num);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of time required to wait between calls to the routine.
    */
    Interval GetInterval() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the amount of time required to wait between calls to the routine.
    */
    void SetInterval(Interval interval);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of times that the routine can be called before terminating itself.
    */
    Iterator GetIterations() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the number of times that the routine can be called before terminating itself.
    */
    void SetIterations(Iterator iterations);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of arguments that are forwarded when executing the callback.
    */
    Uint16 GetArguments() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the number of arguments that are forwarded when executing the callback.
    */
    void SetArguments(Uint16 num);

    /* --------------------------------------------------------------------------------------------
     * See whether the routine is suspended from further calls.
    */
    bool GetSuspended() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the routine is suspended from further calls.
    */
    void SetSuspended(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * See whether the routine was terminated or not.
    */
    bool GetTerminated() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the currently binded callback.
    */
    Function & GetCallback();

    /* --------------------------------------------------------------------------------------------
     * Bind a certain function to be executed when this routine is triggered.
    */
    void SetCallback(Object & env, Function & func);

public:

    /* --------------------------------------------------------------------------------------------
     * Create a routine with just an interval.
    */
    static Object Create(Object & env, Function & func, Interval interval);

    /* --------------------------------------------------------------------------------------------
     * Create a routine with just an interval and explicit iterations.
    */
    static Object Create(Object & env, Function & func, Interval interval, Iterator iterations);

    /* --------------------------------------------------------------------------------------------
     * Create a routine with just an interval, explicit iterations and arguments.
    */
    static Object Create(Object & env, Function & func, Interval interval, Iterator iterations
                            , Object & a1);

    /* --------------------------------------------------------------------------------------------
     * Create a routine with just an interval, explicit iterations and arguments.
    */
    static Object Create(Object & env, Function & func, Interval interval, Iterator iterations
                            , Object & a1, Object & a2);

    /* --------------------------------------------------------------------------------------------
     * Create a routine with just an interval, explicit iterations and arguments.
    */
    static Object Create(Object & env, Function & func, Interval interval, Iterator iterations
                            , Object & a1, Object & a2, Object & a3);

    /* --------------------------------------------------------------------------------------------
     * Create a routine with just an interval, explicit iterations and arguments.
    */
    static Object Create(Object & env, Function & func, Interval interval, Iterator iterations
                            , Object & a1, Object & a2, Object & a3, Object & a4);

    /* --------------------------------------------------------------------------------------------
     * Create a routine with just an interval, explicit iterations and arguments.
    */
    static Object Create(Object & env, Function & func, Interval interval, Iterator iterations
                            , Object & a1, Object & a2, Object & a3, Object & a4, Object & a5);

    /* --------------------------------------------------------------------------------------------
     * Attempt to find a certain routine by its associated tag.
    */
    static Object FindByTag(CSStr tag);
};

} // Namespace:: SqMod

#endif // _ROUTINE_HPP_
