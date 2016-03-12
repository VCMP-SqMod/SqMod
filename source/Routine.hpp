#ifndef _ROUTINE_HPP_
#define _ROUTINE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>
#include <map>

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
     * Commands that can be performed when the buckets are unlocked.
    */
    enum
    {
        CMD_REMOVE = 1,
        CMD_DETACH = 2,
        CMD_ATTACH = 3
    };

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
         * Default constructor.
        */
        Bucket(Interval interval)
            : mInterval(interval), mElapsed(0), mRoutines()
        {
            /* ... */
        }

        /* ----------------------------------------------------------------------------------------
         * Copy constructor.
        */
        Bucket(const Bucket & o) = default;

        /* ----------------------------------------------------------------------------------------
         * Move constructor.
        */
        Bucket(Bucket && o) = default;

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Bucket() = default;

        /* ----------------------------------------------------------------------------------------
         * Copy assignment operator.
        */
        Bucket & operator = (const Bucket & o) = default;

        /* ----------------------------------------------------------------------------------------
         * Move assignment operator.
        */
        Bucket & operator = (Bucket && o) = default;
    };

    /* --------------------------------------------------------------------------------------------
     * A command to perform certain actions when the buckets are unlocked.
    */
    struct Cmd
    {
        // ----------------------------------------------------------------------------------------
        Routine*    mRoutine; /* The routine to which this command applies */
        Interval    mInterval; /* The bucket where this routine is stored. */
        Uint16      mCommand; /* The command that must be performed. */

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Cmd(Routine * routine, Interval interval, Uint16 command)
            : mRoutine(routine), mInterval(interval), mCommand(command)
        {
            /* ... */
        }

        /* ----------------------------------------------------------------------------------------
         * Copy constructor.
        */
        Cmd(const Cmd & o) = default;

        /* ----------------------------------------------------------------------------------------
         * Move constructor.
        */
        Cmd(Cmd && o) = default;

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Cmd() = default;

        /* ----------------------------------------------------------------------------------------
         * Copy assignment operator.
        */
        Cmd & operator = (const Cmd & o) = default;

        /* ----------------------------------------------------------------------------------------
         * Move assignment operator.
        */
        Cmd & operator = (Cmd && o) = default;
    };

    // --------------------------------------------------------------------------------------------
    typedef Int64                                   Time;
    typedef std::vector< Cmd >                      Queue;
    typedef std::vector< Bucket >                   Buckets;
    typedef std::unordered_map< Routine *, Object > Objects;

    /* --------------------------------------------------------------------------------------------
     * Functor used to search for buckets with a certain interval.
    */
    struct IntrvFunc
    {
    private:

        // ----------------------------------------------------------------------------------------
        const Interval m_Interval; /* The interval to be matched. */

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

        /* ----------------------------------------------------------------------------------------
         * Function call operator.
        */
        bool operator () (Buckets::const_reference elem) const
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
    static Objects      s_Objects; /* List of existing routines and their associated object. */

    /* --------------------------------------------------------------------------------------------
     * Attach a routine to a certain bucket.
    */
    static void Attach(Routine * routine, Interval interval);

    /* --------------------------------------------------------------------------------------------
     * Detach a routine from a certain bucket.
    */
    static void Detach(Routine * routine, Interval interval);

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
     * Process queue commands.
    */
    static void ProcQueue();

    /* --------------------------------------------------------------------------------------------
     * See whether this routine is valid otherwise throw an exception.
    */
    void Validate() const
    {
        if (m_Terminated)
        {
            SqThrowF("Routine was terminated [%s]", m_Tag.c_str());
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
     * Copy constructor. (disabled)
    */
    Routine(const Routine &);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Routine & operator = (const Routine &);

private:

    /* --------------------------------------------------------------------------------------------
     * Number of iterations before self destruct.
    */    
    Iterate     m_Iterations;

    /* --------------------------------------------------------------------------------------------
     * Interval between calls.
    */  
    Interval    m_Interval;

    /* --------------------------------------------------------------------------------------------
     * Number of arguments to forward.
    */  
    Uint8       m_Arguments;

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
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user tag.
    */
    const String & GetTag() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    void SetTag(CSStr tag);

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
    Routine & ApplyTag(CSStr tag);

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data and allow chaining of operations.
    */
    Routine & ApplyData(Object & data);

    /* --------------------------------------------------------------------------------------------
     * Terminate this routine by releasing all resources and scheduling it for detachment.
    */
    void Terminate();

    /* --------------------------------------------------------------------------------------------
     * Modify an explicit value to be passed as the specified argument.
    */
    Routine & SetArg(Uint8 num, Object & val);

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

public:

    /* --------------------------------------------------------------------------------------------
     * Create a routine with just an interval.
    */
    static Object Create(Object & env, Function & func, Interval interval);

    /* --------------------------------------------------------------------------------------------
     * Create a routine with just an interval and explicit iterations.
    */
    static Object Create(Object & env, Function & func, Interval interval, Iterate iterations);

    /* --------------------------------------------------------------------------------------------
     * Create a routine with just an interval, explicit iterations and arguments.
    */
    static Object Create(Object & env, Function & func, Interval interval, Iterate iterations
                            , Object & a1);

    /* --------------------------------------------------------------------------------------------
     * Create a routine with just an interval, explicit iterations and arguments.
    */
    static Object Create(Object & env, Function & func, Interval interval, Iterate iterations
                            , Object & a1, Object & a2);

    /* --------------------------------------------------------------------------------------------
     * Create a routine with just an interval, explicit iterations and arguments.
    */
    static Object Create(Object & env, Function & func, Interval interval, Iterate iterations
                            , Object & a1, Object & a2, Object & a3);

    /* --------------------------------------------------------------------------------------------
     * Create a routine with just an interval, explicit iterations and arguments.
    */
    static Object Create(Object & env, Function & func, Interval interval, Iterate iterations
                            , Object & a1, Object & a2, Object & a3, Object & a4);

    /* --------------------------------------------------------------------------------------------
     * Create a routine with just an interval, explicit iterations and arguments.
    */
    static Object Create(Object & env, Function & func, Interval interval, Iterate iterations
                            , Object & a1, Object & a2, Object & a3, Object & a4, Object & a5);

    /* --------------------------------------------------------------------------------------------
     * Return the number of known routines.
    */
    static Uint32 Count();

    /* --------------------------------------------------------------------------------------------
     * Attempt to find a certain routine by its associated tag.
    */
    static Object FindByTag(CSStr tag);

};

} // Namespace:: SqMod

#endif // _ROUTINE_HPP_
