#ifndef _ROUTINE_HPP_
#define _ROUTINE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include <set>
#include <vector>

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
    typedef SQInteger                   Interval;
    typedef Uint32                      Iterate;
    typedef std::vector< Routine * >    Routines;

    /* --------------------------------------------------------------------------------------------
     * Process all active routines and update elapsed time.
    */
    static void Process();

    /* --------------------------------------------------------------------------------------------
     * Release all resources and prepare for shutdown.
    */
    static void TerminateAll();

protected:

    /* --------------------------------------------------------------------------------------------
     * Group of routines that have the same interval.
    */
    struct Bucket
    {
        // ----------------------------------------------------------------------------------------
        Interval    mInterval; /* The interval of time between calls. */
        Interval    mElapsed; /* Time elapsed since the last pulse. */
        Routines    mRoutines; /* Routines to trigger on completion. */

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Bucket(Interval interval)
            : mInterval(interval), mElapsed(0), mRoutines()
        {
            /* ... */
        }

        /* ----------------------------------------------------------------------------------------
         * Copy constructor.
        */
        Bucket(const Bucket & o)
            : mInterval(o.mInterval), mElapsed(o.mElapsed), mRoutines(o.mRoutines)
        {
            /* ... */
        }

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Bucket()
        {
            /* ... */
        }

        /* ----------------------------------------------------------------------------------------
         * Copy assignment operator.
        */
        Bucket & operator = (const Bucket & o)
        {
            mInterval = o.mInterval;
            mElapsed = o.mElapsed;
            mRoutines = o.mRoutines;
            return *this;
        }
    };

    // --------------------------------------------------------------------------------------------
    typedef Int64                                       Time;
    typedef std::vector< Bucket >                       Buckets;
    typedef std::vector< std::pair< bool, Routine * > > Queue;

    /* --------------------------------------------------------------------------------------------
     * Functor used to search for buckets with a certain interval.
    */
    struct IntrvFunc
    {
    private:

        // ----------------------------------------------------------------------------------------
        Interval m_Interval; /* The interval to be matched. */

    public:

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        IntrvFunc(Interval interval)
            : m_Interval(interval)
        {
            /* ... */
        }

        /* ----------------------------------------------------------------------------------------
         * Function call operator.
        */
        bool operator () (Buckets::reference elem) const
        {
            return (elem.mInterval == m_Interval);
        }
    };

    // --------------------------------------------------------------------------------------------
    static bool         s_Lock; /* Avoid further changes to the bucket pool. */
    static Time         s_Last; /* Last time point. */
    static Time         s_Prev; /* Previous time point. */
    static Queue        s_Queue; /* Actions to be performed when the buckets aren't locked */
    static Buckets      s_Buckets; /* Buckets of routines grouped by similar intervals. */

private:

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Routine(const Routine &);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Routine & operator = (const Routine &);

private:

    // --------------------------------------------------------------------------------------------
    Iterate     m_Iterations; /* Number of iterations before self destruct. */
    Interval    m_Interval; /* Interval between calls. */
    Uint8       m_Arguments; /* Number of arguments to forward. */
    bool        m_Suspended; /* Whether calls should be ignored. */
    bool        m_Terminated; /* Whether the routine was terminated. */

    // --------------------------------------------------------------------------------------------
    Function    m_Callback; /* The callback to be executed when triggered. */

    /* --------------------------------------------------------------------------------------------
     * Arguments to be forwarded to the callback.
    */
    Object      m_Arg1, m_Arg2, m_Arg3, m_Arg4, m_Arg5, m_Arg6, m_Arg7,
                m_Arg8, m_Arg9, m_Arg10, m_Arg11, m_Arg12, m_Arg13, m_Arg14;

public:

    /* --------------------------------------------------------------------------------------------
     * Constructor with just an interval.
    */
    Routine(Object & env, Function & func, Interval interval);

    /* --------------------------------------------------------------------------------------------
     * Constructor with just an interval and explicit iterations.
    */
    Routine(Object & env, Function & func, Interval interval, Iterate iterations);

    /* --------------------------------------------------------------------------------------------
     * Constructor with just an interval, explicit iterations and arguments.
    */
    Routine(Object & env, Function & func, Interval interval, Iterate iterations
            , Object & a1);

    /* --------------------------------------------------------------------------------------------
     * Constructor with just an interval, explicit iterations and arguments.
    */
    Routine(Object & env, Function & func, Interval interval, Iterate iterations
            , Object & a1, Object & a2);

    /* --------------------------------------------------------------------------------------------
     * Constructor with just an interval, explicit iterations and arguments.
    */
    Routine(Object & env, Function & func, Interval interval, Iterate iterations
            , Object & a1, Object & a2, Object & a3);

    /* --------------------------------------------------------------------------------------------
     * Constructor with just an interval, explicit iterations and arguments.
    */
    Routine(Object & env, Function & func, Interval interval, Iterate iterations
            , Object & a1, Object & a2, Object & a3, Object & a4);

    /* --------------------------------------------------------------------------------------------
     * Constructor with just an interval, explicit iterations and arguments.
    */
    Routine(Object & env, Function & func, Interval interval, Iterate iterations
            , Object & a1, Object & a2, Object & a3, Object & a4, Object & a5);

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Routine();

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Routine & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Terminate this routine by releasing all resources and sscheduling it for detachment.
    */
    void Terminate();

    /* --------------------------------------------------------------------------------------------
     * Modify an explicit value to be passed as the specified argument.
    */
    void SetArg(Uint8 num, Object & val);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value that is passed as the specified argument.
    */
    Object & GetArg(Uint8 num);

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
    Iterate GetIterations() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the number of times that the routine can be called before terminating itself.
    */
    void SetIterations(Iterate iterations);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of arguments that are forwarded when executing the callback.
    */
    Uint8 GetArguments() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the number of arguments that are forwarded when executing the callback.
    */
    void SetArguments(Uint8 num);

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

protected:

    /* --------------------------------------------------------------------------------------------
     * Release routine resources.
    */
    void Release();

    /* --------------------------------------------------------------------------------------------
     * Create the routine for the first time.
    */
    void Create();

    /* --------------------------------------------------------------------------------------------
     * Attach the routine to the associated bucket.
    */
    void Attach();

    /* --------------------------------------------------------------------------------------------
     * Attach the routine from the associated bucket.
    */
    void Detach();

    /* --------------------------------------------------------------------------------------------
     * Execute the binded callback.
    */
    void Execute();
};

} // Namespace:: SqMod

#endif // _ROUTINE_HPP_
