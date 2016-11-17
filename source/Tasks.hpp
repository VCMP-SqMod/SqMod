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
        LightObj    mData; // A reference to the arbitrary data associated with this instance.
        Iterator    mIterations; // Number of iterations before self destruct.
        Interval    mInterval; // Interval between task invocations.
        Int16       mEntity; // The identifier of the entity to which is belongs.
        Uint8       mType; // The type of the entity to which is belongs.
        Uint8       mArgc; // The number of arguments that the task must forward.
        Argument    mArgv[8]; // The arguments that the task must forward.

        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        Task()
            : mHash(0)
            , mSelf()
            , mFunc()
            , mInst()
            , mData()
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
         * Used by the script engine to convert an instance of this type to a string.
        */
        CSStr ToString() const
        {
            return ToStrF("%lld", static_cast< Int64 >(mInterval));
        }

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
         * Terminate the task
        */
        void Terminate()
        {
            Release();
            Clear();
        }

        /* ----------------------------------------------------------------------------------------
         * Execute the managed task.
        */
        Interval Execute();

        /* ----------------------------------------------------------------------------------------
         * Retrieve the instance to entity instance.
        */
        const LightObj & GetInst() const
        {
            return mInst;
        }

        /* ----------------------------------------------------------------------------------------
         * Retrieve the function object.
        */
        const LightObj & GetFunc() const
        {
            return mFunc;
        }

        /* ----------------------------------------------------------------------------------------
         * Modify the function object.
        */
        void SetFunc(const Function & func)
        {
            // Validate the specified
            if (!sq_isclosure(func.GetFunc()) && !sq_isnativeclosure(func.GetFunc()))
            {
                STHROWF("Invalid callback type %s", SqTypeName(mFunc.GetType()));
            }
            // Grab the virtual machine once
            HSQUIRRELVM vm = DefaultVM::Get();
            // Remember the current stack size
            const StackGuard sg(vm);
            // Push the callback on the stack
            sq_pushobject(vm, func.GetFunc());
            // Grab the hash of the callback object
            mHash = sq_gethash(vm, -1);
            // Now store the function without the environment
            mFunc = LightObj(func.GetFunc());
        }

        /* ----------------------------------------------------------------------------------------
         * Retrieve the arbitrary user data object.
        */
        const LightObj & GetData() const
        {
            return mData;
        }

        /* ----------------------------------------------------------------------------------------
         * Modify the arbitrary user data object.
        */
        void SetData(const LightObj & data)
        {
            mData = data;
        }

        /* ----------------------------------------------------------------------------------------
         * Retrieve the execution interval.
        */
        SQInteger GetInterval() const
        {
            return ConvTo< SQInteger >::From(mInterval);
        }

        /* ----------------------------------------------------------------------------------------
         * Modify the execution interval.
        */
        void SetInterval(SQInteger itr)
        {
            mInterval = ClampMin(ConvTo< Interval >::From(itr), static_cast< Interval >(0));
        }

        /* ----------------------------------------------------------------------------------------
         * Retrieve the number of iterations.
        */
        SQInteger GetIterations() const
        {
            return ConvTo< SQInteger >::From(mIterations);
        }

        /* ----------------------------------------------------------------------------------------
         * Modify the number of iterations.
        */
        void SetIterations(SQInteger itr)
        {
            mIterations = ConvTo< Iterator >::From(itr);
        }

        /* ----------------------------------------------------------------------------------------
         * Retrieve the number of arguments to be forwarded.
        */
        SQInteger GetArguments() const
        {
            return ConvTo< SQInteger >::From(mArgc);
        }

        /* ----------------------------------------------------------------------------------------
         * Retrieve a certain argument.
        */
        LightObj GetArgument(SQInteger arg) const
        {
            constexpr Uint32 argvn = (sizeof(mArgv) / sizeof(mArgv[0]));
            // Cast the index to the proper value
            Uint8 idx = ConvTo< Uint8 >::From(arg);
            // Validate the specified index
            if (idx >= argvn)
            {
                STHROWF("The specified index is out of range: %u >= %u", idx, argvn);
            }
            // Return the requested argument
            return mArgv[idx];
        }
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
