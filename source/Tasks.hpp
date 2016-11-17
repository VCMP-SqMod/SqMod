#ifndef _TASKS_HPP_
#define _TASKS_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Execute callbacks for specific entities after specific intervals of time.
*/
class Tasks
{
public:

    /* --------------------------------------------------------------------------------------------
     * Simplify future changes to a single point of change.
    */
    typedef Int64                                       Time;
    typedef SQInteger                                   Interval;
    typedef Uint32                                      Iterator;
    typedef LightObj                                    Argument;

private:

    /* --------------------------------------------------------------------------------------------
     * Structure that represents a task and keeps track of the task information.
    */
    struct Task
    {
        // ----------------------------------------------------------------------------------------
        SQHash      mHash; // The hash of the referenced function object.
        LightObj    mSelf; // A reference to `this`as a script object.
        LightObj    mFunc; // A reference to the managed function object.
        LightObj    mInst; // A reference to the associated entity object.
        Iterator    mIterations; // Number of iterations before self destruct.
        Interval    mInterval; // Interval between task invocations.
        Int16       mEntity; // The identifier of the entity to which is belongs.
        Uint8       mType; // The type of the entity to which is belongs.
        Uint8       mArgc; // The number of arguments that the task must forward.
        Argument    mArgv[14]; // The arguments that the task must forward.

        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        Task()
            : mHash(0)
            , mSelf()
            , mFunc()
            , mInst()
            , mIterations(0)
            , mInterval(0)
            , mEntity(-1)
            , mType(-1)
            , mArgc(0)
            , mArgv()
        {
            /* ... */
        }

        /* ----------------------------------------------------------------------------------------
         * Copy constructor. (disabled)
        */
        Task(const Task & o) = delete;

        /* ----------------------------------------------------------------------------------------
         * Move constructor. (disabled)
        */
        Task(Task && o) = delete;

        /* ----------------------------------------------------------------------------------------
         * Release managed script resources.
        */
        ~Task()
        {
            Release();
            Clear();
        }

        /* ----------------------------------------------------------------------------------------
         * Copy assignment operator. (disabled)
        */
        Task & operator = (const Task & o) = delete;

        /* ----------------------------------------------------------------------------------------
         * Move assignment operator. (disabled)
        */
        Task & operator = (Task && o) = delete;

        /* ----------------------------------------------------------------------------------------
         * Initializes the task parameters. (assumes previous values are already released)
        */
        void Init(HSQOBJECT & inst, HSQOBJECT & func, Interval intrv, Iterator itr, Int32 id, Int32 type);

        /* ----------------------------------------------------------------------------------------
         * Clear the arguments.
        */
        void Clear()
        {
            // Now release the arguments
            for (auto & a : mArgv)
            {
                a.Release();
            }
            // Reset the counter
            mArgc = 0;
        }

        /* ----------------------------------------------------------------------------------------
         * Release managed script resources.
        */
        void Release();

        /* ----------------------------------------------------------------------------------------
         * Execute the managed task.
        */
        Interval Execute();
    };

    // --------------------------------------------------------------------------------------------
    static Uint32       s_Used; // The number of occupied slots.
    static Time         s_Last; // Last time point.
    static Time         s_Prev; // Previous time point.
    static Interval     s_Intervals[SQMOD_MAX_TASKS]; // List of intervals to be processed.
    static Task         s_Tasks[SQMOD_MAX_TASKS]; // List of tasks to be executed.

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (disabled)
    */
    Tasks() = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Tasks(const Tasks & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    Tasks(Tasks && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor. (disabled)
    */
    ~Tasks() = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Tasks & operator = (const Tasks & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Tasks & operator = (Tasks && o) = delete;

public:

    /* --------------------------------------------------------------------------------------------
     * Process all active tasks and update elapsed time.
    */
    static void Process();

    /* --------------------------------------------------------------------------------------------
     * Initialize all resources and prepare for startup.
    */
    static void Initialize();

    /* --------------------------------------------------------------------------------------------
     * Register the task class.
    */
    static void Register(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Release all resources and prepare for shutdown.
    */
    static void Deinitialize();

protected:

    /* --------------------------------------------------------------------------------------------
     * Retrieve the instance of the specified entity.
    */
    static Object & FindEntity(Int32 id, Int32 type);

    /* --------------------------------------------------------------------------------------------
     * Find an unoccupied task slot.
    */
    static SQInteger FindUnused();

    /* --------------------------------------------------------------------------------------------
     * Locate the first task with the specified parameters.
    */
    static SQInteger Find(Int32 id, Int32 type, SQInteger & pos, HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Attempt to create a task with the specified parameters.
    */
    static SQInteger Create(Int32 id, Int32 type, HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Attempt to remove the task with the specified parameters.
    */
    static SQInteger Remove(Int32 id, Int32 type, HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See if a task with the specified parameters exists.
    */
    static SQInteger Exists(Int32 id, Int32 type, HSQUIRRELVM vm);

public:

    /* --------------------------------------------------------------------------------------------
     * Cleanup all tasks associated with the specified entity.
    */
    static void Cleanup(Int32 id, Int32 type);

    /* --------------------------------------------------------------------------------------------
     * Forwards calls to create tasks.
    */
    template < typename Entity, Int32 Type > static SQInteger MakeTask(HSQUIRRELVM vm)
    {
        // The entity instance
        const Entity * inst = nullptr;
        // Attempt to extract the instance
        try
        {
            // Fetch the instance from the stack
            inst = Var< const Entity * >(vm, 1).value;
            // Do we have a valid instance?
            if (!inst)
            {
                STHROWF("Invalid entity instance");
            }
            // Validate the actual entity instance
            inst->Validate();
        }
        catch (const Sqrat::Exception & e)
        {
            return sq_throwerror(vm, e.what());
        }
        // Forward the call and return the result
        return Create(inst->GetID(), Type, vm);
    }

    /* --------------------------------------------------------------------------------------------
     * Forwards calls to remove tasks.
    */
    template < typename Entity, Int32 Type > static SQInteger DropTask(HSQUIRRELVM vm)
    {
        // The entity instance
        const Entity * inst = nullptr;
        // Attempt to extract the instance
        try
        {
            // Fetch the instance from the stack
            inst = Var< const Entity * >(vm, 1).value;
            // Do we have a valid instance?
            if (!inst)
            {
                STHROWF("Invalid entity instance");
            }
            // Validate the actual entity instance
            inst->Validate();
        }
        catch (const Sqrat::Exception & e)
        {
            return sq_throwerror(vm, e.what());
        }
        // Forward the call and return the result
        return Remove(inst->GetID(), Type, vm);
    }

    /* --------------------------------------------------------------------------------------------
     * Forwards calls to check tasks.
    */
    template < typename Entity, Int32 Type > static SQInteger DoesTask(HSQUIRRELVM vm)
    {
        // The entity instance
        const Entity * inst = nullptr;
        // Attempt to extract the instance
        try
        {
            // Fetch the instance from the stack
            inst = Var< const Entity * >(vm, 1).value;
            // Do we have a valid instance?
            if (!inst)
            {
                STHROWF("Invalid entity instance");
            }
            // Validate the actual entity instance
            inst->Validate();
        }
        catch (const Sqrat::Exception & e)
        {
            return sq_throwerror(vm, e.what());
        }
        // Forward the call and return the result
        return Exists(inst->GetID(), Type, vm);
    }
};

} // Namespace:: SqMod

#endif // _TASKS_HPP_
