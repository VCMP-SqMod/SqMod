#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

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
    typedef int64_t                                       Time;
    typedef SQInteger                                   Interval;
    typedef uint32_t                                      Iterator;
    typedef LightObj                                    Argument;

private:

    /* --------------------------------------------------------------------------------------------
     * Structure that represents a task and keeps track of the task information.
    */
    struct Task
    {
        // ----------------------------------------------------------------------------------------
        SQHash      mHash; // The hash of the referenced function object.
        String      mTag; // An arbitrary string which represents the tag.
        LightObj    mSelf; // A reference to `this`as a script object.
        LightObj    mFunc; // A reference to the managed function object.
        LightObj    mInst; // A reference to the associated entity object.
        LightObj    mData; // A reference to the arbitrary data associated with this instance.
        Iterator    mIterations; // Number of iterations before self destruct.
        Interval    mInterval; // Interval between task invocations.
        int16_t       mEntity; // The identifier of the entity to which is belongs.
        uint8_t       mType; // The type of the entity to which is belongs.
        uint8_t       mArgc; // The number of arguments that the task must forward.
        Argument    mArgv[8]; // The arguments that the task must forward.

        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        Task() noexcept
            : mHash(0)
            , mTag()
            , mSelf()
            , mFunc()
            , mInst()
            , mData()
            , mIterations(0)
            , mInterval(0)
            , mEntity(-1)
            , mType(0)
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
            Terminate();
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
        SQMOD_NODISCARD const String & ToString() const
        {
            return mTag;
        }

        /* ----------------------------------------------------------------------------------------
         * Initializes the task parameters. (assumes previous values are already released)
        */
        void Init(HSQOBJECT & inst, HSQOBJECT & func, Interval intrv, Iterator itr, int32_t id, int32_t type);

        /* ----------------------------------------------------------------------------------------
         * Release managed script resources.
        */
        void Release();

        /* ----------------------------------------------------------------------------------------
         * Execute the managed task.
        */
        Interval Execute();

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
         * Terminate the task.
        */
        void Terminate()
        {
            Release();
            Clear();
        }

        /* ----------------------------------------------------------------------------------------
         * Retrieve the associated user tag.
        */
        SQMOD_NODISCARD const String & GetTag() const
        {
            return mTag;
        }

        /* ----------------------------------------------------------------------------------------
         * Modify the associated user tag.
        */
        void SetTag(StackStrF & tag)
        {
            mTag.assign(tag.mPtr, static_cast< size_t  >(ClampMin(tag.mLen, 0)));
        }

        /* ----------------------------------------------------------------------------------------
         * Retrieve the instance to entity instance.
        */
        SQMOD_NODISCARD const LightObj & GetInst() const
        {
            return mInst;
        }

        /* ----------------------------------------------------------------------------------------
         * Retrieve the function object.
        */
        SQMOD_NODISCARD const LightObj & GetFunc() const
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
            HSQUIRRELVM vm = SqVM();
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
        SQMOD_NODISCARD const LightObj & GetData() const
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
        SQMOD_NODISCARD SQInteger GetInterval() const
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
        SQMOD_NODISCARD SQInteger GetIterations() const
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
        SQMOD_NODISCARD SQInteger GetArguments() const
        {
            return ConvTo< SQInteger >::From(mArgc);
        }

        /* ----------------------------------------------------------------------------------------
         * Retrieve a certain argument.
        */
        SQMOD_NODISCARD const Argument & GetArgument(SQInteger arg) const
        {
            constexpr uint32_t argvn = (sizeof(mArgv) / sizeof(mArgv[0]));
            // Cast the index to the proper value
            uint8_t idx = ConvTo< uint8_t >::From(arg);
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
    static Time         s_Last; // Last time point.
    static Time         s_Prev; // Previous time point.
    static Interval     s_Intervals[SQMOD_MAX_TASKS]; // List of intervals to be processed.
    static Task         s_Tasks[SQMOD_MAX_TASKS]; // List of tasks to be executed.

public:

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Tasks & operator = (const Tasks & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Tasks & operator = (Tasks && o) = delete;

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
    static LightObj & FindEntity(int32_t id, int32_t type);

    /* --------------------------------------------------------------------------------------------
     * Find an unoccupied task slot.
    */
    static SQInteger FindUnused();

    /* --------------------------------------------------------------------------------------------
     * Locate the first task with the specified parameters.
    */
    static SQInteger Find(int32_t id, int32_t type, SQInteger & pos, HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Attempt to create a task with the specified parameters.
    */
    static SQInteger Create(int32_t id, int32_t type, HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Attempt to remove the task with the specified parameters.
    */
    static SQInteger Remove(int32_t id, int32_t type, HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See if a task with the specified parameters exists.
    */
    static SQInteger Exists(int32_t id, int32_t type, HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Cleanup all tasks associated with the specified entity.
    */
    static const Task & FindByTag(int32_t id, int32_t type, StackStrF & tag);

public:

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of used tasks slots.
    */
    SQMOD_NODISCARD static SQInteger GetUsed()
    {
        SQInteger n = 0;
        // Iterate task list
        for (const auto & t : s_Tasks)
        {
            if (VALID_ENTITY(t.mEntity))
            {
                ++n;
            }
        }
        // Return the final count
        return n;
    }

    /* --------------------------------------------------------------------------------------------
     * Cleanup all tasks associated with the specified entity.
    */
    static void Cleanup(int32_t id, int32_t type);

    /* --------------------------------------------------------------------------------------------
     * Forwards calls to create tasks.
    */
    template < typename Entity, int32_t Type > static SQInteger MakeTask(HSQUIRRELVM vm)
    {
        // The entity instance
        const Entity * inst;
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
            // Validate the std::exception entity instance
            inst->Validate();
        }
        catch (const std::exception & e)
        {
            return sq_throwerror(vm, e.what());
        }
        // Forward the call and return the result
        return Create(inst->GetID(), Type, vm);
    }

    /* --------------------------------------------------------------------------------------------
     * Forwards calls to remove tasks.
    */
    template < typename Entity, int32_t Type > static SQInteger DropTask(HSQUIRRELVM vm)
    {
        // The entity instance
        const Entity * inst;
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
        catch (const std::exception & e)
        {
            return sq_throwerror(vm, e.what());
        }
        // Forward the call and return the result
        return Remove(inst->GetID(), Type, vm);
    }

    /* --------------------------------------------------------------------------------------------
     * Forwards calls to check tasks.
    */
    template < typename Entity, int32_t Type > static SQInteger DoesTask(HSQUIRRELVM vm)
    {
        // The entity instance
        const Entity * inst;
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
        catch (const std::exception & e)
        {
            return sq_throwerror(vm, e.what());
        }
        // Forward the call and return the result
        return Exists(inst->GetID(), Type, vm);
    }

    /* --------------------------------------------------------------------------------------------
     * Forwards calls to find tasks.
    */
    template < typename Entity, int32_t Type > static SQInteger FindTask(HSQUIRRELVM vm)
    {
        // Was the tag string specified?
        if (sq_gettop(vm) <= 1)
        {
            return sq_throwerror(vm, "Missing tag string");
        }
        // The entity instance
        const Entity * inst;
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
        catch (const std::exception & e)
        {
            return sq_throwerror(vm, e.what());
        }
        // Attempt to generate the string value
        StackStrF tag(vm, 2);
        // Have we failed to retrieve the string?
        if (SQ_FAILED(tag.Proc(true)))
        {
            return tag.mRes; // Propagate the error!
        }
        // Attempt to find the specified task
        try
        {
            // Perform the search
            const Task & task = FindByTag(inst->GetID(), Type, tag);
            // Now push the instance on the stack
            sq_pushobject(vm, task.mSelf.mObj);
        }
        catch (const std::exception & e)
        {
            return sq_throwerror(vm, e.what());
        }
        // Specify that this function returns a value
        return 1;
    }
};

} // Namespace:: SqMod
