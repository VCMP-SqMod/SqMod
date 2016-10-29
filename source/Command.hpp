#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"
#include "Base/Buffer.hpp"
#include "Logger.hpp"

// ------------------------------------------------------------------------------------------------
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <map>
#include <vector>
#include <iterator>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace Cmd {

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
class Context;
class Guard;
class Command;
class Controller;
class Manager;
class Listener;

/* ------------------------------------------------------------------------------------------------
 * Helper typedefs.
*/

// ------------------------------------------------------------------------------------------------
typedef SharedPtr< Context >        CtxRef; // Shared reference to an execution context.
typedef WeakPtr< Context >          CtxPtr; // Shared reference to an execution context.

// ------------------------------------------------------------------------------------------------
typedef SharedPtr< Controller >     CtrRef; // Shared reference to a command controller.
typedef WeakPtr< Controller >       CtrPtr; // Shared reference to a command controller.

// ------------------------------------------------------------------------------------------------
typedef std::pair< Uint8, Object >  Argument; // Can hold the argument value and type.
typedef std::vector< Argument >     Arguments; // A list of extracted arguments.

// ------------------------------------------------------------------------------------------------
typedef std::vector< Command >      Commands; // List of attached command instances.
typedef std::vector< Controller * > Controllers; // List of active controllers.

/* ------------------------------------------------------------------------------------------------
 * Types of arguments supported by the command system.
*/
enum CmdArgType
{
    CMDARG_ANY         = 0,
    CMDARG_INTEGER     = (1 << 1),
    CMDARG_FLOAT       = (1 << 2),
    CMDARG_BOOLEAN     = (1 << 3),
    CMDARG_STRING      = (1 << 4),
    CMDARG_LOWER       = (1 << 5),
    CMDARG_UPPER       = (1 << 6),
    CMDARG_GREEDY      = (1 << 7)
};

/* ------------------------------------------------------------------------------------------------
 * Types of errors reported by the command system.
*/
enum CmdError
{
    // The command failed for unknown reasons
    CMDERR_UNKNOWN = 0,
    // The command failed to execute because there was nothing to execute
    CMDERR_EMPTY_COMMAND,
    // The command failed to execute because the command name was invalid after processing
    CMDERR_INVALID_COMMAND,
    // The command failed to execute because there was a syntax error in the arguments
    CMDERR_SYNTAX_ERROR,
    // The command failed to execute because there was no such command
    CMDERR_UNKNOWN_COMMAND,
    // The command failed to execute because the it's currently suspended
    CMDERR_COMMAND_SUSPENDED,
    // The command failed to execute because the invoker does not have the proper authority
    CMDERR_INSUFFICIENT_AUTH,
    // The command failed to execute because there was no callback to handle the execution
    CMDERR_MISSING_EXECUTER,
    // The command was unable to execute because the argument limit was not reached
    CMDERR_INCOMPLETE_ARGS,
    // The command was unable to execute because the argument limit was exceeded
    CMDERR_EXTRANEOUS_ARGS,
    // Command was unable to execute due to argument type mismatch
    CMDERR_UNSUPPORTED_ARG,
    // The command arguments contained more data than the internal buffer can handle
    CMDERR_BUFFER_OVERFLOW,
    // The command failed to complete execution due to a runtime exception
    CMDERR_EXECUTION_FAILED,
    // The command completed the execution but returned a negative result
    CMDERR_EXECUTION_ABORTED,
    // The post execution callback failed to execute due to a runtime exception
    CMDERR_POST_PROCESSING_FAILED,
    // The callback that was supposed to deal with the failure also failed due to a runtime exception
    CMDERR_UNRESOLVED_FAILURE,
    // Maximum command error identifier
    CMDERR_MAX
};

// ------------------------------------------------------------------------------------------------
inline CSStr ValidateName(CSStr name)
{
    // Is the name empty?
    if (!name || *name == '\0')
    {
        STHROWF("Invalid or empty command name");
    }
    // Create iterator to name start
    CSStr str = name;
    // Inspect name characters
    while (*str != '\0')
    {
        // Does it contain spaces?
        if (std::isspace(*str) != 0)
        {
            STHROWF("Command names cannot contain spaces");
        }
        // Move to the next character
        ++str;
    }
    // Return the name
    return name;
}

// ------------------------------------------------------------------------------------------------
inline CSStr ArgSpecToStr(Uint8 spec)
{
    switch (spec)
    {
        case CMDARG_ANY:        return _SC("any");
        case CMDARG_INTEGER:    return _SC("integer");
        case CMDARG_FLOAT:      return _SC("float");
        case CMDARG_BOOLEAN:    return _SC("boolean");
        case CMDARG_STRING:
        case CMDARG_LOWER:
        case CMDARG_UPPER:
        case CMDARG_GREEDY:     return _SC("string");
        default:                return _SC("unknown");
    }
}

/* ------------------------------------------------------------------------------------------------
 * Holds the context of a command execution.
*/
struct Context
{
public:

    // --------------------------------------------------------------------------------------------
    Buffer          mBuffer; // Shared buffer used to extract arguments and process data.

    // --------------------------------------------------------------------------------------------
    const Object    mInvoker; // Reference to the entity that invoked the command.
    String          mCommand; // Command name extracted from the command string.
    String          mArgument; // Command argument extracted from the command string.
    Listener*       mInstance; // Pointer to the currently executed command listener.
    Object          mObject; // Script object of the currently executed command.

    // --------------------------------------------------------------------------------------------
    Arguments       mArgv; // Extracted command arguments.
    Uint32          mArgc; // Extracted arguments count.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Context(Object & invoker)
        : mBuffer(512)
        , mInvoker(invoker)
        , mCommand()
        , mArgument()
        , mInstance(nullptr)
        , mObject()
        , mArgv()
        , mArgc(0)
    {
        // Reserve enough space upfront
        mCommand.reserve(64);
        mArgument.reserve(512);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Context(const Context & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    Context(Context && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Context & operator = (const Context & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Context & operator = (Context && o) = delete;
};

/* ------------------------------------------------------------------------------------------------
 * Helper class implementing RAII to release the command context.
*/
struct Guard
{
public:

    // --------------------------------------------------------------------------------------------
    CtrRef mController; // Reference to the guarded controller.
    CtxRef mPrevious; // Previous context when this guard was created.
    CtxRef mCurrent; // The context managed by this guard.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Guard(const CtrRef & ctr, Object & invoker);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Guard(const Guard & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Guard(Guard && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Guard();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Guard & operator = (const Guard & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Guard & operator = (Guard && o) = delete;
};

/* ------------------------------------------------------------------------------------------------
 * Structure that represents a unique command in the pool.
*/
struct Command
{
    // --------------------------------------------------------------------------------------------
    std::size_t     mHash; // The unique hash that identifies this command.
    String          mName; // The unique name that identifies this command.
    Listener*       mPtr; // The listener that reacts to this command.
    Object          mObj; // A strong reference to the script object.
    CtrPtr          mCtr; // The associated contoller.

    /* --------------------------------------------------------------------------------------------
     * Construct a command and the also create a script object from the specified listener.
    */
    Command(std::size_t hash, const String & name, Listener * ptr, const CtrPtr & ctr);

    /* --------------------------------------------------------------------------------------------
     * Construct a command and extract the listener from the specified script object.
    */
    Command(std::size_t hash, const String & name, const Object & obj, const CtrPtr & ctr);

    /* --------------------------------------------------------------------------------------------
     * Construct a command and extract the listener from the specified script object.
    */
    Command(std::size_t hash, const String & name, Object && obj, const CtrPtr & ctr);

    /* --------------------------------------------------------------------------------------------
     * Construct a command with the given parameters.
    */
    Command(std::size_t hash, const String & name, Listener * ptr, const Object & obj, const CtrPtr & ctr);

    /* --------------------------------------------------------------------------------------------
     * Construct a command with the given parameters.
    */
    Command(std::size_t hash, const String & name, Listener * ptr, Object && obj, const CtrPtr & ctr);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Command(const Command & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Command(Command && o)
        : mHash(o.mHash)
        , mName(std::move(o.mName))
        , mPtr(o.mPtr)
        , mObj(o.mObj)
        , mCtr(o.mCtr)
    {
        o.mPtr = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Command();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Command & operator = (const Command & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Command & operator = (Command && o)
    {
        if (this != &o)
        {
            mHash = o.mHash;
            mName = std::move(o.mName);
            mPtr = o.mPtr;
            mObj = o.mObj;
            mCtr = o.mCtr;
            o.mPtr = nullptr;
        }
        return *this;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Holds a list of commands to execute as well as authentication or failure resolvers.
*/
struct Controller
{
    // --------------------------------------------------------------------------------------------
    friend class Guard; // Allow the guard to swap the execution context.
    friend class Manager; // Allow the manager to manage the controller.
    friend class Listener; // Allow the listener to attach and detach.

private:

    // --------------------------------------------------------------------------------------------
    Commands        m_Commands; // List of available command instances.
    CtxRef          m_Context; // Context of the currently executed command.

    // --------------------------------------------------------------------------------------------
    Function        m_OnFail; // Callback when something failed while running a command.
    Function        m_OnAuth; // Callback to authenticate execution for a certain invoker.

    // --------------------------------------------------------------------------------------------
    Manager *       m_Manager;

    // --------------------------------------------------------------------------------------------
    static Controllers s_Controllers;

protected:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Controller(Manager * mgr)
        : m_Commands()
        , m_Context()
        , m_OnFail()
        , m_OnAuth()
        , m_Manager(mgr)
    {
        s_Controllers.push_back(this);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Controller(const Controller & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    Controller(Controller && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Controller & operator = (const Controller & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Controller & operator = (Controller && o) = delete;

protected:

    /* --------------------------------------------------------------------------------------------
     * Forward error message to the error callback.
    */
    template < typename T > void SqError(Int32 type, CSStr msg, T data)
    {
        // Is there a callback that deals with errors?
        if (m_OnFail.IsNull())
        {
            return;
        }
        // Attempt to forward the error to that callback
        try
        {
            m_OnFail.Execute< Int32, CSStr, T >(type, msg, data);
        }
        catch (const Sqrat::Exception & e)
        {
            // The debugger probably took care of reporting the details
            LogErr("Command error callback failed [%s]", e.what());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Execute one of the managed commands.
    */
    Int32 Run(const Guard & guard, CSStr command);

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute the specified command.
    */
    Int32 Exec(Context & ctx);

    /* --------------------------------------------------------------------------------------------
     * Attempt to parse the specified argument.
    */
    bool Parse(Context & ctx);

    /* --------------------------------------------------------------------------------------------
     * Attach a command listener to a certain name.
    */
    Object & Attach(Object & obj, Listener * ptr)
    {
        return Attach(Object(obj), ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Attach a command listener to a certain name.
    */
    Object & Attach(const Object & obj, Listener * ptr)
    {
        return Attach(Object(obj), ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Attach a command listener to a certain name.
    */
    Object & Attach(Object && obj, Listener * ptr);

    /* --------------------------------------------------------------------------------------------
     * Detach a command listener from a certain name.
    */
    void Detach(const String & name)
    {
        // Obtain the unique identifier of the specified name
        const std::size_t hash = std::hash< String >()(name);
        // Iterator to the found command, if any
        Commands::const_iterator itr = m_Commands.cbegin();
        // Attempt to find the specified command
        for (; itr != m_Commands.cend(); ++itr)
        {
            // Are the hashes identical?
            if (itr->mHash == hash)
            {
                break; // We found our command!
            }
        }
        // Make sure the command exist before attempting to remove it
        if (itr != m_Commands.end())
        {
            m_Commands.erase(itr);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Detach a command listener from a certain name.
    */
    void Detach(Listener * ptr)
    {
        // Iterator to the found command, if any
        Commands::const_iterator itr = m_Commands.cbegin();
        // Attempt to find the specified command
        for (; itr != m_Commands.cend(); ++itr)
        {
            // Are the instances identical?
            if (itr->mPtr == ptr)
            {
                break; // We found our command!
            }
        }
        // Make sure the command exists before attempting to remove it
        if (itr != m_Commands.end())
        {
            m_Commands.erase(itr);
        }
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Terminate the all controllers by releasing their command listeners and callbacks.
    */
    static void Terminate()
    {
        for (auto & ctr : s_Controllers)
        {
            // Clear the command listeners
            ctr->Clear();
            // Release the script callbacks, if any
            ctr->m_OnFail.ReleaseGently();
            ctr->m_OnAuth.ReleaseGently();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Controller()
    {
        s_Controllers.erase(std::remove(s_Controllers.begin(), s_Controllers.end(), this),
                            s_Controllers.end());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the currently active execution context.
    */
    const CtxRef & GetCtx() const
    {
        return m_Context;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether a certain name exist in the command list.
    */
    bool Attached(const String & name) const
    {
        // Obtain the unique identifier of the specified name
        const std::size_t hash = std::hash< String >()(name);
        // Attempt to find the specified command
        for (const auto & cmd : m_Commands)
        {
            // Are the hashes identical?
            if (cmd.mHash == hash)
            {
                return true; // We found our command!
            }
        }
        // No such command exists
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether a certain instance exist in the command list.
    */
    bool Attached(const Listener * ptr) const
    {
        // Attempt to find the specified command
        for (const auto & cmd : m_Commands)
        {
            // Are the instances identical?
            if (cmd.mPtr == ptr)
            {
                return true; // We found our command!
            }
        }
        // No such command exists
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Sort the command list in an ascending order.
    */
    void Sort()
    {
        std::sort(m_Commands.begin(), m_Commands.end(),
            [](Commands::const_reference a, Commands::const_reference b) -> bool {
                return (a.mName < b.mName);
            });
    }

    /* --------------------------------------------------------------------------------------------
     * Detach all the associated command listeners.
    */
    void Clear()
    {
        m_Commands.clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Locate and retrieve a command listener by name.
    */
    const Object & FindByName(const String & name)
    {
        // Obtain the unique identifier of the specified name
        const std::size_t hash = std::hash< String >()(name);
        // Attempt to find the specified command
        for (const auto & cmd : m_Commands)
        {
            // Are the hashes identical?
            if (cmd.mHash == hash)
            {
                return cmd.mObj; // We found our command!
            }
        }
        // No such command exist
        return NullObject();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the error callback.
    */
    Function & GetOnFail()
    {
        return m_OnFail;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the error callback.
    */
    void SetOnFail(Object & env, Function & func)
    {
        // Are we supposed to unbind current callback?
        if (func.IsNull())
        {
            m_OnFail.ReleaseGently();
        }
        // Was there a custom environment specified?
        else if (env.IsNull())
        {
            m_OnFail = func;
        }
        else
        {
            m_OnFail = Function(env.GetVM(), env.GetObject(), func.GetFunc());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the authentication callback.
    */
    Function & GetOnAuth()
    {
        return m_OnAuth;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the authentication callback.
    */
    void SetOnAuth(Object & env, Function & func)
    {
        // Are we supposed to unbind current callback?
        if (func.IsNull())
        {
            m_OnAuth.ReleaseGently();
        }
        // Was there a custom environment specified?
        if (env.IsNull())
        {
            m_OnAuth = func;
        }
        else
        {
            m_OnAuth = Function(env.GetVM(), env.GetObject(), func.GetFunc());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * See whether an execution context is currently active.
    */
    bool IsContext() const
    {
        return !!m_Context;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the invoker from the current execution context.
    */
    const Object & GetInvoker() const
    {
        // See if there's an execution context available
        if (!m_Context)
        {
            STHROWF("No active execution context");
        }
        // Return the requested information
        return m_Context->mInvoker;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the listener object from the current execution context.
    */
    const Object & GetListener() const
    {
        // See if there's an execution context available
        if (!m_Context)
        {
            STHROWF("No active execution context");
        }
        // Return the requested information
        return m_Context->mObject;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the command name from the current execution context.
    */
    const String & GetCommand() const
    {
        // See if there's an execution context available
        if (!m_Context)
        {
            STHROWF("No active execution context");
        }
        // Return the requested information
        return m_Context->mCommand;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the command argument from the current execution context.
    */
    const String & GetArgument() const
    {
        // See if there's an execution context available
        if (!m_Context)
        {
            STHROWF("No active execution context");
        }
        // Return the requested information
        return m_Context->mArgument;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve all command listeners in an array.
    */
    Array GetCommandsArray() const
    {
        // Allocate an array with an adequate size
        Array arr(DefaultVM::Get(), m_Commands.size());
        // Index of the currently processed command listener
        SQInteger index = 0;
        // Populate the array with the command listeners
        for (const auto & cmd : m_Commands)
        {
            arr.SetValue(index++, cmd.mObj);
        }
        // Return the resulted array
        return arr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve all command listeners in a table.
    */
    Table GetCommandsTable() const
    {
        // Allocate an empty table
        Table tbl(DefaultVM::Get());
        // Populate the table with the command listeners
        for (const auto & cmd : m_Commands)
        {
            tbl.SetValue(cmd.mName.c_str(), cmd.mObj);
        }
        // Return the resulted table
        return tbl;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all command listeners with a function.
    */
    void ForeachCommand(Function & func) const
    {
        // Make sure that the specified function works
        if (func.IsNull())
        {
            return;
        }
        // Process all the managed command listeners
        for (const auto & cmd : m_Commands)
        {
            func.Execute(cmd.mObj);
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Allows interaction with a command controller from script.
*/
class Manager
{
private:

    // --------------------------------------------------------------------------------------------
    CtrRef  m_Controller; // Reference to the managed controller.

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated controller if valid otherwise throw an exception.
    */
    const CtrRef & GetValid() const
    {
        // Validate the managed controller
        if (!m_Controller)
        {
            STHROWF("No controller associated with this manager");
        }
        // Return the controller reference
        return m_Controller;
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Manager()
        : m_Controller(new Controller(this))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Manager(const Manager & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    Manager(Manager && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Manager & operator = (const Manager & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Manager & operator = (Manager && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Manager & o) const
    {
        if (m_Controller == o.m_Controller)
        {
            return 0;
        }
        else if (m_Controller.Get() > o.m_Controller.Get())
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert this instance to a string.
    */
    CSStr ToString() const
    {
        return ToStrF("%d", m_Controller.Count());
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated controller reference.
    */
    const CtrRef & GetCtr() const
    {
        return m_Controller;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of references to the managed controller.
    */
    Uint32 GetRefCount() const
    {
        return m_Controller.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Run a command under a specific invoker.
    */
    Int32 Run(Object & invoker, CSStr command)
    {
        return GetValid()->Run(Guard(m_Controller, invoker), command);
    }

    /* --------------------------------------------------------------------------------------------
     * Sort the command list in an ascending order.
    */
    void Sort()
    {
        GetValid()->Sort();
    }

    /* --------------------------------------------------------------------------------------------
     * Detach all the associated command listeners.
    */
    void Clear()
    {
        GetValid()->Clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Attach a command listener to the managed controller.
    */
    void Attach(Object & obj)
    {
        GetValid()->Attach(obj, nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Locate and retrieve a command listener by name.
    */
    const Object & FindByName(const String & name)
    {
        return GetValid()->FindByName(name);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of managed command listeners.
    */
    SQInteger GetCount() const
    {
        return ConvTo< SQInteger >::From(GetValid()->m_Commands.size());
    }

    /* --------------------------------------------------------------------------------------------
     * See whether an execution context is currently active.
    */
    bool IsContext() const
    {
        return GetValid()->IsContext();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the error callback.
    */
    Function & GetOnFail()
    {
        return GetValid()->GetOnFail();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the error callback.
    */
    void SetOnFail(Object & env, Function & func)
    {
        GetValid()->SetOnFail(env, func);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the authentication callback.
    */
    Function & GetOnAuth()
    {
        return GetValid()->GetOnAuth();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the authentication callback.
    */
    void SetOnAuth(Object & env, Function & func)
    {
        GetValid()->SetOnAuth(env, func);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the invoker from the current execution context.
    */
    const Object & GetInvoker() const
    {
        return GetValid()->GetInvoker();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the listener object from the current execution context.
    */
    const Object & GetListener() const
    {
        return GetValid()->GetListener();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the command name from the current execution context.
    */
    const String & GetCommand() const
    {
        return GetValid()->GetCommand();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the command argument from the current execution context.
    */
    const String & GetArgument() const
    {
        return GetValid()->GetArgument();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve all command listeners in an array.
    */
    Array GetCommandsArray() const
    {
        return GetValid()->GetCommandsArray();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve all command listeners in a table.
    */
    Table GetCommandsTable() const
    {
        return GetValid()->GetCommandsTable();
    }

    /* --------------------------------------------------------------------------------------------
     * Process all command listeners with a function.
    */
    void ForeachCommand(Object & env, Function & func) const
    {
        if (env.IsNull())
        {
            GetValid()->ForeachCommand(func);
        }
        else
        {
            Function fn(env.GetVM(), env, func.GetFunc());
            GetValid()->ForeachCommand(fn);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object Create(CSStr name)
    {
        return Create(name, _SC(""), NullArray(), 0, SQMOD_MAX_CMD_ARGS-1, -1, false, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object Create(CSStr name, CSStr spec)
    {
        return Create(name, spec, NullArray(), 0, SQMOD_MAX_CMD_ARGS-1, -1, false, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object Create(CSStr name, CSStr spec, Array & tags)
    {
        return Create(name, spec, tags, 0, SQMOD_MAX_CMD_ARGS-1, -1, false, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object Create(CSStr name, CSStr spec, Uint8 min, Uint8 max)
    {
        return Create(name, spec, NullArray(), min, max, -1, false, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object Create(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max)
    {
        return Create(name, spec, tags, min, max, -1, false, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object Create(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max, SQInteger auth)
    {
        return Create(name, spec, tags, min, max, auth, auth >= 0, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object Create(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max, SQInteger auth, bool prot)
    {
        return Create(name, spec, tags, min, max, auth, prot, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object Create(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max, SQInteger auth, bool prot, bool assoc);
};

/* ------------------------------------------------------------------------------------------------
 * Attaches to a command name and listens for invocations.
*/
class Listener
{
    // --------------------------------------------------------------------------------------------
    friend class Controller; // Allow the controller to execute this listener.
    friend class Command; // Allow the command to interact with this listener.
    friend class Manager; // Allow the manager to interact with this listener.

public:

    /* --------------------------------------------------------------------------------------------
     * Release any trace of script resources from all the listener instances.
    */
    static void Terminate()
    {
        // Go forward and release resources
        for (Listener * node = s_Head; node != nullptr; node = node->m_Next)
        {
            node->m_Data.Release();
        }
        // Go backwards and release resources
        for (Listener * node = s_Head; node != nullptr; node = node->m_Prev)
        {
            node->m_Data.Release();
        }
        // Kinda useless but Squirrel doesn't play nice with loose references
        // Better safe than sorry
    }

    /* --------------------------------------------------------------------------------------------
     * Convenience constructor.
    */
    Listener(CSStr name)
        : Listener(name, _SC(""), NullArray(), 0, SQMOD_MAX_CMD_ARGS-1, -1, false, false)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Convenience constructor.
    */
    Listener(CSStr name, CSStr spec)
        : Listener(name, spec, NullArray(), 0, SQMOD_MAX_CMD_ARGS-1, -1, false, false)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Convenience constructor.
    */
    Listener(CSStr name, CSStr spec, Array & tags)
        : Listener(name, spec, tags, 0, SQMOD_MAX_CMD_ARGS-1, -1, false, false)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Convenience constructor.
    */
    Listener(CSStr name, CSStr spec, Uint8 min, Uint8 max)
        : Listener(name, spec, NullArray(), min, max, -1, false, false)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Convenience constructor.
    */
    Listener(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max)
        : Listener(name, spec, tags, min, max, -1, false, false)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Convenience constructor.
    */
    Listener(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max, SQInteger auth)
        : Listener(name, spec, tags, min, max, auth, auth >= 0, false)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Convenience constructor.
    */
    Listener(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max, SQInteger auth, bool prot)
        : Listener(name, spec, tags, min, max, auth, prot, false)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Listener(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max, SQInteger auth, bool prot, bool assoc)
        : m_Controller()
        , m_Name(ValidateName(name))
        , m_ArgSpec()
        , m_ArgTags()
        , m_MinArgc(0)
        , m_MaxArgc(SQMOD_MAX_CMD_ARGS-1)
        , m_Spec()
        , m_Help()
        , m_Info()
        , m_OnExec()
        , m_OnAuth()
        , m_OnPost()
        , m_OnFail()
        , m_Authority(ConvTo< Int32 >::From(auth))
        , m_Protected(prot)
        , m_Suspended(false)
        , m_Associate(assoc)
        , m_Prev(nullptr)
        , m_Next(s_Head)
    {
        // Initialize the specifiers to default values
        for (Uint8 n = 0; n < SQMOD_MAX_CMD_ARGS; ++n)
        {
            m_ArgSpec[n] = CMDARG_ANY;
        }
        // Apply the specified argument rules/specifications
        SetSpec(spec);
        // Extract the specified argument tags
        SetArgTags(tags);
        // Set the specified minimum and maximum allowed arguments
        SetMinArgC(min);
        SetMaxArgC(max);
        // Generate information for the command
        GenerateInfo(false);
        // We're the head element now
        s_Head = this;
        // Was there a previous head?
        if (m_Next != nullptr)
        {
            // Steal previous element from previous head
            m_Prev = m_Next->m_Prev;
            // Did that head element had a previous element?
            if (m_Prev != nullptr)
            {
                // Tell it we're the next element now
                m_Prev->m_Next = this;
            }
            // Tell the previous head that we're the previous element now
            m_Next->m_Prev = this;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Listener(const Listener & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    Listener(Listener && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Listener()
    {
        // Detach the command
        if (!m_Controller.Expired())
        {
            m_Controller.Lock()->Detach(this);
        }
        // Release callbacks
        m_OnExec.ReleaseGently();
        m_OnAuth.ReleaseGently();
        m_OnPost.ReleaseGently();
        m_OnFail.ReleaseGently();
        // Is there an element behind us?
        if (m_Prev != nullptr)
        {
            // Tell it to point to the element ahead of us
            m_Prev->m_Next = m_Next;
        }
        // Is there an element ahead of us?
        if (m_Next != nullptr)
        {
            // Tell it to point to the element behind us
            m_Next->m_Prev = m_Prev;
        }
        // Are we the head element in the chain?
        if (s_Head == this)
        {
            s_Head = m_Next == nullptr ? m_Prev : m_Next;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Listener & operator = (const Listener & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Listener & operator = (Listener && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Listener & o) const
    {
        if (m_Name == o.m_Name)
        {
            return 0;
        }
        else if (m_Name.size() > o.m_Name.size())
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert this instance to a string.
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
     * Retrieve the number of weak references to the managed controller.
    */
    Uint32 GetRefCount() const
    {
        return m_Controller.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Attach the listener instance to the associated command name.
    */
    void Attach(const Manager & mgr)
    {
        // Is the associated name even valid?
        if (m_Name.empty())
        {
            STHROWF("Invalid or empty command name");
        }
        // Detach from the current command controller, if any
        Detach();
        // Is the specified manager valid to even attempt association?
        if (mgr.GetCtr())
        {
            // Attempt to associate with it's controller
            mgr.GetCtr()->Attach(NullObject(), this);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Detach the listener instance from the associated command name.
    */
    void Detach()
    {
        // Make sure that we are even associated with a controller
        if (!m_Controller.Expired())
        {
            // Detach from the associated controller
            m_Controller.Lock()->Detach(this);
            // Release the controller reference
            m_Controller.Reset();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the listener instance is attached to the associated command name.
    */
    bool Attached() const
    {
        return (!m_Controller.Expired() && m_Controller.Lock()->Attached(this));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the manager associated with this command listener instance.
    */
    Object GetManager() const
    {
        // Are we even associated with a controller?
        if (m_Controller.Expired())
        {
            return NullObject(); // Default to null
        }
        // Obtain the initial stack size
        const StackGuard sg;
        // Push the instance on the stack
        ClassType< Manager >::PushInstance(DefaultVM::Get(), m_Controller.Lock()->m_Manager);
        // Grab the instance from the stack
        return Var< Object >(DefaultVM::Get(), -1).value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name that triggers this command listener instance.
    */
    const String & GetName() const
    {
        return m_Name;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name that triggers this command listener instance.
    */
    void SetName(CSStr name)
    {
        // Validate the specified name
        ValidateName(name);
        // Is this command already attached to a name?
        if (!m_Controller.Expired() && m_Controller.Lock()->Attached(this))
        {
            const CtrRef ctr = m_Controller.Lock();
            // Detach from the current name if necessary
            ctr->Detach(this);
            // Now it's safe to assign the new name
            m_Name.assign(name);
            // We know the new name is valid
            ctr->Attach(NullObject(), this);
        }
        else
        {
            m_Name.assign(name); // Just assign the name
        }
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
     * Retrieve the argument specification string.
    */
    const String & GetSpec() const
    {
        return m_Spec;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the argument specification string.
    */
    void SetSpec(CSStr spec)
    {
        // Attempt to process the specified string
        ProcSpec(spec);
        // Assign the specifier, if any
        if (spec)
        {
            m_Spec.assign(spec);
        }
        else
        {
            m_Spec.clear();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the argument tags array.
    */
    Array GetArgTags() const
    {
        // Allocate an array to encapsulate all tags
        Array arr(DefaultVM::Get(), SQMOD_MAX_CMD_ARGS);
        // Put the tags to the allocated array
        for (Uint32 arg = 0; arg < SQMOD_MAX_CMD_ARGS; ++arg)
        {
            arr.SetValue(arg, m_ArgTags[arg]);
        }
        // Return the array with the tags
        return arr;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the argument tags array.
    */
    void SetArgTags(Array & tags)
    {
        // Preliminary checks before even attempting anything
        if (tags.GetType() != OT_ARRAY || tags.IsNull())
        {
            for (Uint8 n = 0; n < SQMOD_MAX_CMD_ARGS; ++n)
            {
                m_ArgTags[n].clear();
            }
            // We're done here!
            return;
        }
        // Attempt to retrieve the number of specified tags
        const Uint32 max = ConvTo< Uint32 >::From(tags.Length());
        // If no tags were specified then clear current tags
        if (!max)
        {
            for (Uint8 n = 0; n < SQMOD_MAX_CMD_ARGS; ++n)
            {
                m_ArgTags[n].clear();
            }
        }
        // See if we're in range
        else if (max < SQMOD_MAX_CMD_ARGS)
        {
            // Attempt to get all arguments in one go
            tags.GetArray< String >(m_ArgTags, max);
        }
        else
        {
            STHROWF("Argument tag (%u) is out of range (%d)", max, SQMOD_MAX_CMD_ARGS);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the help message associated with this command listener instance.
    */
    const String & GetHelp() const
    {
        return m_Help;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the help message associated with this command listener instance.
    */
    void SetHelp(CSStr help)
    {
        if (help)
        {
            m_Help.assign(help);
        }
        else
        {
            m_Help.clear();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the informational message associated with this command listener instance.
    */
    const String & GetInfo() const
    {
        return m_Info;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the informational message associated with this command listener instance.
    */
    void SetInfo(CSStr info)
    {
        if (info)
        {
            m_Info.assign(info);
        }
        else
        {
            m_Info.clear();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the authority level required to execute this command listener instance.
    */
    SQInteger GetAuthority() const
    {
        return m_Authority;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the authority level required to execute this command listener instance.
    */
    void SetAuthority(SQInteger level)
    {
        m_Authority = ConvTo< Int32 >::From(level);
    }

    /* --------------------------------------------------------------------------------------------
     * See whether this command listener instance requires explicit authority inspection.
    */
    bool GetProtected() const
    {
        return m_Protected;
    }

    /* --------------------------------------------------------------------------------------------
     * Set whether this command listener instance requires explicit authority inspection.
    */
    void SetProtected(bool toggle)
    {
        m_Protected = toggle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether this command listener instance is currently ignoring calls.
    */
    bool GetSuspended() const
    {
        return m_Suspended;
    }

    /* --------------------------------------------------------------------------------------------
     * Set whether this command listener instance should start ignoring calls.
    */
    void SetSuspended(bool toggle)
    {
        m_Suspended = toggle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether this command listener instance receives arguments in an associative container.
    */
    bool GetAssociate() const
    {
        return m_Associate;
    }

    /* --------------------------------------------------------------------------------------------
     * Set whether this command listener instance receives arguments in an associative container.
    */
    void SetAssociate(bool toggle)
    {
        m_Associate = toggle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the maximum arguments supported by this command listener.
    */
    Uint8 GetMinArgC() const
    {
        return m_MinArgc;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the minimum arguments supported by this command listener.
    */
    void SetMinArgC(SQInteger val)
    {
        // Limit the specified number withing allowed range
        val = ConvTo< Uint8 >::From(val);
        // Perform a range check on the specified argument index
        if (val >= SQMOD_MAX_CMD_ARGS)
        {
            STHROWF("Argument (%d) is out of total range (%d)", val, SQMOD_MAX_CMD_ARGS);
        }
        else if (static_cast< Uint8 >(val) > m_MaxArgc)
        {
            STHROWF("Minimum argument (%d) exceeds maximum (%u)", val, m_MaxArgc);
        }
        // Apply the specified value
        m_MinArgc = static_cast< Uint8 >(val);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the maximum arguments supported by this command listener.
    */
    SQInteger GetMaxArgC() const
    {
        return m_MaxArgc;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the maximum arguments supported by this command listener.
    */
    void SetMaxArgC(SQInteger val)
    {
        // Limit the specified number withing allowed range
        val = ConvTo< Uint8 >::From(val);
        // Perform a range check on the specified argument index
        if (val >= SQMOD_MAX_CMD_ARGS)
        {
            STHROWF("Argument (%d) is out of total range (%d)", val, SQMOD_MAX_CMD_ARGS);
        }
        else if (static_cast< Uint8 >(val) < m_MinArgc)
        {
            STHROWF("Maximum argument (%d) exceeds minimum (%u)", val, m_MinArgc);
        }
        // Apply the specified value
        m_MaxArgc = static_cast< Uint8 >(val);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the function that must be called when this command listener is executed.
    */
    Function & GetOnExec()
    {
        return m_OnExec;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the function that must be called when this command listener is executed.
    */
    void SetOnExec(Object & env, Function & func)
    {
        // Are we supposed to unbind current callback?
        if (func.IsNull())
        {
            m_OnExec.ReleaseGently();
        }
        // Was there a custom environment specified?
        else if (env.IsNull())
        {
            m_OnExec = func;
        }
        else
        {
            m_OnExec = Function(env.GetVM(), env.GetObject(), func.GetFunc());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the function that must be called when this command listener needs to authenticate.
    */
    Function & GetOnAuth()
    {
        return m_OnAuth;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the function that must be called when this command listener needs to authenticate.
    */
    void SetOnAuth(Object & env, Function & func)
    {
        // Are we supposed to unbind current callback?
        if (func.IsNull())
        {
            m_OnAuth.ReleaseGently();
        }
        // Was there a custom environment specified?
        else if (env.IsNull())
        {
            m_OnAuth = func;
        }
        else
        {
            m_OnAuth = Function(env.GetVM(), env.GetObject(), func.GetFunc());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the function that must be called when this command listener finished execution.
    */
    Function & GetOnPost()
    {
        return m_OnPost;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the function that must be called when this command listener finished execution.
    */
    void SetOnPost(Object & env, Function & func)
    {
        // Are we supposed to unbind current callback?
        if (func.IsNull())
        {
            m_OnPost.ReleaseGently();
        }
        // Was there a custom environment specified?
        else if (env.IsNull())
        {
            m_OnPost = func;
        }
        else
        {
            m_OnPost = Function(env.GetVM(), env.GetObject(), func.GetFunc());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the function that must be called when this command listener failed execution.
    */
    Function & GetOnFail()
    {
        return m_OnFail;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the function that must be called when this command listener failed execution.
    */
    void SetOnFail(Object & env, Function & func)
    {
        // Are we supposed to unbind current callback?
        if (func.IsNull())
        {
            m_OnFail.ReleaseGently();
        }
        // Was there a custom environment specified?
        else if (env.IsNull())
        {
            m_OnFail = func;
        }
        else
        {
            m_OnFail = Function(env.GetVM(), env.GetObject(), func.GetFunc());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the tag of a certain argument.
    */
    const String & GetArgTag(Uint32 arg) const
    {
        // Perform a range check on the specified argument index
        if (arg >= SQMOD_MAX_CMD_ARGS)
        {
            STHROWF("Argument (%u) is out of total range (%u)", arg, SQMOD_MAX_CMD_ARGS);
        }
        // Return the requested information
        return m_ArgTags[arg];
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the tag of a certain argument.
    */
    void SetArgTag(Uint32 arg, CSStr name)
    {
        // Perform a range check on the specified argument index
        if (arg >= SQMOD_MAX_CMD_ARGS)
        {
            STHROWF("Argument (%u) is out of total range (%u)", arg, SQMOD_MAX_CMD_ARGS);
        }
        // The string type doesn't appreciate null values
        else if (name != nullptr)
        {
            m_ArgTags[arg].assign(name);
        }
        // Clear previous name in this case
        else
        {
            m_ArgTags[arg].clear();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the flags of the specified argument.
    */
    Uint8 GetArgFlags(Uint32 idx) const
    {
        // Perform a range check on the specified argument index
        if (idx >= SQMOD_MAX_CMD_ARGS)
        {
            STHROWF("Argument (%u) is out of total range (%u)", idx, SQMOD_MAX_CMD_ARGS);
        }
        // Return the requested information
        return m_ArgSpec[idx];
    }

    /* --------------------------------------------------------------------------------------------
     * See whether whether the specified argument can be used on this command listener instance.
    */
    bool ArgCheck(Uint32 arg, Uint8 flag) const
    {
        // Perform a range check on the specified argument index
        if (arg >= SQMOD_MAX_CMD_ARGS)
        {
            STHROWF("Argument (%u) is out of total range (%u)", arg, SQMOD_MAX_CMD_ARGS);
        }
        // Retrieve the argument flags
        const Uint8 f = m_ArgSpec[arg];
        // Perform the requested check
        return  (f == CMDARG_ANY) || ((f & flag) != 0x0) || ((f & CMDARG_GREEDY) && (flag & CMDARG_STRING));
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the specified invoker entity has the proper authority to run this command.
    */
    bool AuthCheck(const Object & invoker)
    {
        // Do we need explicit authority verification?
        if (!m_Protected)
        {
            return true; // Anyone can invoke this command
        }
        // Was there a custom authority inspector specified?
        else if (!m_OnAuth.IsNull())
        {
            // Ask the specified authority inspector if this execution should be allowed
            const SharedPtr< bool > ret = m_OnAuth.Evaluate< bool, const Object &, Listener * >(invoker, this);
            // See what the custom authority inspector said or default to disallow
            return !ret ? false : *ret;
        }
        // Was there a global authority inspector specified?
        else if (!m_Controller.Expired() && !m_Controller.Lock()->GetOnAuth().IsNull())
        {
            // Ask the specified authority inspector if this execution should be allowed
            const SharedPtr< bool > ret = m_Controller.Lock()->GetOnAuth().Evaluate< bool, const Object &, Listener * >(invoker, this);
            // See what the custom authority inspector said or default to disallow
            return !ret ? false : *ret;
        }
        // A negative authority level is considered to be the same as unprotected
        else if (m_Authority < 0)
        {
            return true;
        }
        // Default to blocking this execution
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Use the command listener argument properties to generate an informational message.
    */
    void GenerateInfo(bool full);

protected:

    // --------------------------------------------------------------------------------------------
    typedef Uint8   ArgSpec[SQMOD_MAX_CMD_ARGS];
    typedef String  ArgTags[SQMOD_MAX_CMD_ARGS];

    /* --------------------------------------------------------------------------------------------
     * Execute the designated callback by passing the arguments in their specified order.
    */
    SQInteger Execute(const Object & invoker, const Array & args)
    {
        // Attempt to evaluate the specified executer knowing the manager did the validations
        SharedPtr< SQInteger > ret = m_OnExec.Evaluate< SQInteger, const Object &, const Array & >(invoker, args);
        // See if the executer succeeded and return the result or default to failed
        return (!ret ? 0 : *ret);
    }

    /* --------------------------------------------------------------------------------------------
     * Execute the designated callback by passing the arguments using an associative container.
    */
    SQInteger Execute(const Object & invoker, const Table & args)
    {
        // Attempt to evaluate the specified executer knowing the manager did the validations
        SharedPtr< SQInteger > ret = m_OnExec.Evaluate< SQInteger, const Object &, const Table & >(invoker, args);
        // See if the executer succeeded and return the result or default to failed
        return (!ret ? 0 : *ret);
    }

    /* --------------------------------------------------------------------------------------------
     * Process the specified string and extract the argument properties in it.
    */
    void ProcSpec(CSStr spec);

private:

    // --------------------------------------------------------------------------------------------
    CtrPtr     m_Controller; // Manager that controls this command listener.

    // --------------------------------------------------------------------------------------------
    String      m_Name; // Name of the command that triggers this listener.
    Object      m_Data; // Arbitrary user data associated with this particular instance.

    // --------------------------------------------------------------------------------------------
    ArgSpec     m_ArgSpec; // List of argument type specifications.
    ArgTags     m_ArgTags; // List of argument tags/names.

    // --------------------------------------------------------------------------------------------
    Uint8       m_MinArgc; // Minimum number of arguments supported by this listener.
    Uint8       m_MaxArgc; // Maximum number of arguments supported by this listener.

    // --------------------------------------------------------------------------------------------
    String      m_Spec; // String used to generate the argument type specification list.
    String      m_Help; // String describing what the command is supposed to do.
    String      m_Info; // String with syntax information for the command.

    // --------------------------------------------------------------------------------------------
    Function    m_OnExec; // Function to call when the command is executed.
    Function    m_OnAuth; // Function to call when the invoker must be authenticated.
    Function    m_OnPost; // Function to call after the command was successfully executed.
    Function    m_OnFail; // Function to call after the command execution failed.

    // --------------------------------------------------------------------------------------------
    Int32       m_Authority; // Built-in authority level required to execute this command.

    // --------------------------------------------------------------------------------------------
    bool        m_Protected; // Whether explicit authentication of the invoker is required.
    bool        m_Suspended; // Whether this command should block further invocations.
    bool        m_Associate; // Whether arguments are sent as table instead of array.

    // --------------------------------------------------------------------------------------------
    Listener *  m_Prev; // Previous listener in the chain.
    Listener *  m_Next; // Next listener in the chain.

    // --------------------------------------------------------------------------------------------
    static Listener *  s_Head; // The head of the listener chain.
};

} // Namespace:: Cmd
} // Namespace:: SqMod

#endif // _COMMAND_HPP_
