#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Buffer.hpp"
#include "Core/Utility.hpp"
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
namespace SqMod { // NOLINT(modernize-concat-nested-namespaces)
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
typedef std::pair< uint8_t, Object >  Argument; // Can hold the argument value and type.
typedef std::vector< Argument >     Arguments; // A list of extracted arguments.

// ------------------------------------------------------------------------------------------------
typedef std::vector< Command >      Commands; // List of attached command instances.
typedef std::vector< Controller * > Controllers; // List of active controllers.

/* ------------------------------------------------------------------------------------------------
 * Types of arguments supported by the command system.
*/
enum CmdArgType
{
    CMDARG_ANY         = 0u,
    CMDARG_INTEGER     = (1u << 1u),
    CMDARG_FLOAT       = (1u << 2u),
    CMDARG_BOOLEAN     = (1u << 3u),
    CMDARG_STRING      = (1u << 4u),
    CMDARG_LOWER       = (1u << 5u),
    CMDARG_UPPER       = (1u << 6u),
    CMDARG_GREEDY      = (1u << 7u)
};

/* ------------------------------------------------------------------------------------------------
 * Types of errors reported by the command system.
*/
enum CmdError
{
    // The command failed for unknown reasons
    CMDERR_UNKNOWN = 0u,
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
inline const SQChar * ValidateName(const SQChar * name)
{
    // Is the name empty?
    if (!name || *name == '\0')
    {
        STHROWF("Invalid or empty command name");
    }
    // Create iterator to name start
    const SQChar * str = name;
    // Inspect name characters
    while ('\0' != *str)
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
inline const SQChar * ArgSpecToStr(uint8_t spec)
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
    uint32_t          mArgc; // Extracted arguments count.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    explicit Context(Object & invoker)
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
    Guard(CtrRef  ctr, Object & invoker);

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
    std::size_t     mHash{0}; // The unique hash that identifies this command.
    String          mName{}; // The unique name that identifies this command.
    Listener*       mPtr{nullptr}; // The listener that reacts to this command.
    Object          mObj{}; // A strong reference to the script object.
    CtrPtr          mCtr{}; // The associated controller.

    /* --------------------------------------------------------------------------------------------
     * Construct a command and the also create a script object from the specified listener.
    */
    Command(std::size_t hash, String name, Listener * ptr, CtrPtr  ctr);

    /* --------------------------------------------------------------------------------------------
     * Construct a command and extract the listener from the specified script object.
    */
    Command(std::size_t hash, String name, const Object & obj, CtrPtr  ctr);

    /* --------------------------------------------------------------------------------------------
     * Construct a command and extract the listener from the specified script object.
    */
    Command(std::size_t hash, String name, Object && obj, CtrPtr  ctr);

    /* --------------------------------------------------------------------------------------------
     * Construct a command with the given parameters.
    */
    Command(std::size_t hash, String name, Listener * ptr, const Object & obj, CtrPtr  ctr);

    /* --------------------------------------------------------------------------------------------
     * Construct a command with the given parameters.
    */
    Command(std::size_t hash, String name, Listener * ptr, Object && obj, CtrPtr  ctr);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Command(const Command & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Command(Command && o) noexcept
        : mHash(o.mHash)
        , mName(std::forward< String >(o.mName))
        , mPtr(o.mPtr)
        , mObj(std::forward< Object >(o.mObj))
        , mCtr(std::forward< CtrPtr >(o.mCtr))
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
    Command & operator = (Command && o) noexcept
    {
        if (this != &o)
        {
            mHash = o.mHash;
            mName = std::forward< String >(o.mName);
            mPtr = o.mPtr;
            mObj = std::forward< Object >(o.mObj);
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
    explicit Controller(Manager * mgr)
        : m_Commands()
        , m_Context()
        , m_OnFail()
        , m_OnAuth()
        , m_Manager(mgr)
    {
        s_Controllers.push_back(this);
    }

protected:

    /* --------------------------------------------------------------------------------------------
     * Forward error message to the error callback.
    */
    template < typename T > void SqError(int32_t type, const SQChar * msg, T data)
    {
        // Is there a callback that deals with errors?
        if (m_OnFail.IsNull())
        {
            return;
        }
        // Attempt to forward the error to that callback
        try
        {
            m_OnFail.Execute(type, msg, data);
        }
		catch (const Poco::Exception& e)
		{
            // The debugger probably took care of reporting the details
            LogErr("Command error callback failed [%s]", e.displayText().c_str());
		}
        catch (const std::exception & e)
        {
            // The debugger probably took care of reporting the details
            LogErr("Command error callback failed [%s]", e.what());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Execute one of the managed commands.
    */
    int32_t Run(const Guard & guard, const SQChar * command);

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute the specified command.
    */
    int32_t Exec(Context & ctx);

    /* --------------------------------------------------------------------------------------------
     * Attempt to parse the specified argument.
    */
    bool Parse(Context & ctx);

    /* --------------------------------------------------------------------------------------------
     * Attach a command listener to the associated name.
    */
    Object & Attach(Object & obj, Listener * ptr)
    {
        return Attach(Object(obj), ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Attach a command listener to the associated name.
    */
    Object & Attach(const Object & obj, Listener * ptr)
    {
        return Attach(Object(obj), ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Attach a command listener to the associated name.
    */
    Object & Attach(Object && obj, Listener * ptr);

    /* --------------------------------------------------------------------------------------------
     * Attach a command listener to a certain name.
    */
    Object & Attach(Object & obj, Listener * ptr, String name)
    {
        return Attach(Object(obj), ptr, std::move(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Attach a command listener to a certain name.
    */
    Object & Attach(const Object & obj, Listener * ptr, String name)
    {
        return Attach(Object(obj), ptr, std::move(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Attach a command listener to a certain name.
    */
    Object & Attach(Object && obj, Listener * ptr, String name);

    /* --------------------------------------------------------------------------------------------
     * Detach a command listener from a certain name.
    */
    void Detach(const String & name)
    {
        // Obtain the unique identifier of the specified name
        const std::size_t hash = std::hash< String >()(name);
        // Attempt to find the specified command
        auto itr = std::find_if(m_Commands.cbegin(), m_Commands.cend(), [=](Commands::const_reference c) {
            return (c.mHash == hash);
        });
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
        // Attempt to find the specified command
        auto itr = std::find_if(m_Commands.cbegin(), m_Commands.cend(), [=](Commands::const_reference c) {
            return (c.mPtr == ptr);
        });
        // Make sure the command exists before attempting to remove it
        if (itr != m_Commands.end())
        {
            m_Commands.erase(itr);
        }
    }

public:

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
            ctr->m_OnFail.Release();
            ctr->m_OnAuth.Release();
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
    SQMOD_NODISCARD const CtxRef & GetCtx() const
    {
        return m_Context;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether a certain name exist in the command list.
    */
    SQMOD_NODISCARD bool Attached(const String & name) const
    {
        // Obtain the unique identifier of the specified name
        const std::size_t hash = std::hash< String >()(name);
        // Attempt to find the specified command
        for (const auto & cmd : m_Commands) // NOLINT(readability-use-anyofallof)
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
        for (const auto & cmd : m_Commands) // NOLINT(readability-use-anyofallof)
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
                return (a.mName < b.mName); // NOLINT(modernize-use-nullptr)
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
    void SetOnFail(Function & func)
    {
        m_OnFail = std::move(func);
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
    void SetOnAuth(Function & func)
    {
        m_OnAuth = std::move(func);
    }

    /* --------------------------------------------------------------------------------------------
     * See whether an execution context is currently active.
    */
    SQMOD_NODISCARD bool IsContext() const
    {
        return !!m_Context;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the invoker from the current execution context.
    */
    SQMOD_NODISCARD const Object & GetInvoker() const
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
    SQMOD_NODISCARD const Object & GetListener() const
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
    SQMOD_NODISCARD const String & GetCommand() const
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
    SQMOD_NODISCARD const String & GetArgument() const
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
    SQMOD_NODISCARD Array GetCommandsArray() const
    {
        // Allocate an array with an adequate size
        Array arr(SqVM(), m_Commands.size());
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
    SQMOD_NODISCARD Table GetCommandsTable() const
    {
        // Allocate an empty table
        Table tbl(SqVM());
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
    SQMOD_NODISCARD const CtrRef & GetValid() const
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
    SQMOD_NODISCARD int32_t Cmp(const Manager & o) const
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
    SQMOD_NODISCARD String ToString() const
    {
        return fmt::format("{}", m_Controller.Count());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated controller reference.
    */
    SQMOD_NODISCARD const CtrRef & GetCtr() const
    {
        return m_Controller;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of references to the managed controller.
    */
    SQMOD_NODISCARD uint32_t GetRefCount() const
    {
        return m_Controller.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Run a command under a specific invoker.
    */
    int32_t Run(Object & invoker, StackStrF & command)
    {
        if ((SQ_FAILED(command.Proc())))
        {
            return static_cast< int32_t >(command.mRes);
        }
        else
        {
            return GetValid()->Run(Guard(m_Controller, invoker), command.mPtr);
        }
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
    const Object & FindByName(StackStrF & name)
    {
        // Validate the specified name
        if ((SQ_FAILED(name.Proc())))
        {
            STHROWF("Unable to extract a valid command name");
        }
        else if (name.mLen <= 0)
        {
            STHROWF("Invalid or empty command name");
        }
        // Attempt to return the requested command
        return GetValid()->FindByName(String(name.mPtr, static_cast< size_t >(name.mLen)));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of managed command listeners.
    */
    SQMOD_NODISCARD SQInteger GetCount() const
    {
        return ConvTo< SQInteger >::From(GetValid()->m_Commands.size());
    }

    /* --------------------------------------------------------------------------------------------
     * See whether an execution context is currently active.
    */
    SQMOD_NODISCARD bool IsContext() const
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
    void SetOnFail(Function & func)
    {
        GetValid()->SetOnFail(func);
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
    void SetOnAuth(Function & func)
    {
        GetValid()->SetOnAuth(func);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the invoker from the current execution context.
    */
    SQMOD_NODISCARD const Object & GetInvoker() const
    {
        return GetValid()->GetInvoker();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the listener object from the current execution context.
    */
    SQMOD_NODISCARD const Object & GetListener() const
    {
        return GetValid()->GetListener();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the command name from the current execution context.
    */
    SQMOD_NODISCARD const String & GetCommand() const
    {
        return GetValid()->GetCommand();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the command argument from the current execution context.
    */
    SQMOD_NODISCARD const String & GetArgument() const
    {
        return GetValid()->GetArgument();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve all command listeners in an array.
    */
    SQMOD_NODISCARD Array GetCommandsArray() const
    {
        return GetValid()->GetCommandsArray();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve all command listeners in a table.
    */
    SQMOD_NODISCARD Table GetCommandsTable() const
    {
        return GetValid()->GetCommandsTable();
    }

    /* --------------------------------------------------------------------------------------------
     * Process all command listeners with a function.
    */
    void ForeachCommand(Function & func) const
    {
        GetValid()->ForeachCommand(func);
    }

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object Create1(StackStrF & name)
    {
        return Create(name, StackStrF::Dummy(), NullArray(), 0, SQMOD_MAX_CMD_ARGS-1, -1, false, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object Create2(StackStrF & name, StackStrF & spec)
    {
        return Create(name, spec, NullArray(), 0, SQMOD_MAX_CMD_ARGS-1, -1, false, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object Create3(StackStrF & name, StackStrF & spec, Array & tags)
    {
        return Create(name, spec, tags, 0, SQMOD_MAX_CMD_ARGS-1, -1, false, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object Create4(StackStrF & name, StackStrF & spec, uint8_t min, uint8_t max)
    {
        return Create(name, spec, NullArray(), min, max, -1, false, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object Create5(StackStrF & name, StackStrF & spec, Array & tags, uint8_t min, uint8_t max)
    {
        return Create(name, spec, tags, min, max, -1, false, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object Create6(StackStrF & name, StackStrF & spec, Array & tags, uint8_t min, uint8_t max, SQInteger auth)
    {
        return Create(name, spec, tags, min, max, auth, auth >= 0, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object Create7(StackStrF & name, StackStrF & spec, Array & tags, uint8_t min, uint8_t max, SQInteger auth, bool prot)
    {
        return Create(name, spec, tags, min, max, auth, prot, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object Create(StackStrF & name, StackStrF & spec, Array & tags, uint8_t min, uint8_t max, SQInteger auth, bool prot, bool assoc);
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
            node->m_OnExec.Release();
            node->m_OnAuth.Release();
            node->m_OnAudit.Release();
            node->m_OnPost.Release();
            node->m_OnFail.Release();
        }
        // Go backwards and release resources
        for (Listener * node = s_Head; node != nullptr; node = node->m_Prev)
        {
            node->m_Data.Release();
            node->m_OnExec.Release();
            node->m_OnAuth.Release();
            node->m_OnAudit.Release();
            node->m_OnPost.Release();
            node->m_OnFail.Release();
        }
        // Kinda useless but Squirrel doesn't play nice with loose references
        // Better safe than sorry
    }

    /* --------------------------------------------------------------------------------------------
     * Convenience constructor.
    */
    explicit Listener(StackStrF & name)
        : Listener(name, StackStrF::Dummy(), NullArray(), 0, SQMOD_MAX_CMD_ARGS-1, -1, false, false)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Convenience constructor.
    */
    Listener(StackStrF & name, StackStrF & spec)
        : Listener(name, spec, NullArray(), 0, SQMOD_MAX_CMD_ARGS-1, -1, false, false)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Convenience constructor.
    */
    Listener(StackStrF & name, StackStrF & spec, Array & tags)
        : Listener(name, spec, tags, 0, SQMOD_MAX_CMD_ARGS-1, -1, false, false)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Convenience constructor.
    */
    Listener(StackStrF & name, StackStrF & spec, uint8_t min, uint8_t max)
        : Listener(name, spec, NullArray(), min, max, -1, false, false)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Convenience constructor.
    */
    Listener(StackStrF & name, StackStrF & spec, Array & tags, uint8_t min, uint8_t max)
        : Listener(name, spec, tags, min, max, -1, false, false)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Convenience constructor.
    */
    Listener(StackStrF & name, StackStrF & spec, Array & tags, uint8_t min, uint8_t max, SQInteger auth)
        : Listener(name, spec, tags, min, max, auth, auth >= 0, false)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Convenience constructor.
    */
    Listener(StackStrF & name, StackStrF & spec, Array & tags, uint8_t min, uint8_t max, SQInteger auth, bool prot)
        : Listener(name, spec, tags, min, max, auth, prot, false)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Listener(StackStrF & name, StackStrF & spec, Array & tags, uint8_t min, uint8_t max, SQInteger auth, bool prot, bool assoc)
        : m_Controller()
        , m_Name()
        , m_ArgSpec()
        , m_ArgTags()
        , m_MinArgc(0)
        , m_MaxArgc(SQMOD_MAX_CMD_ARGS-1)
        , m_Aliases(0)
        , m_Spec()
        , m_Help()
        , m_Info()
        , m_OnExec()
        , m_OnAuth()
        , m_OnAudit()
        , m_OnPost()
        , m_OnFail()
        , m_Authority(ConvTo< int32_t >::From(auth))
        , m_Protected(prot)
        , m_Suspended(false)
        , m_Associate(assoc)
        , m_Prev(nullptr)
        , m_Next(s_Head)
    {
        // Extract the given name
        if ((SQ_FAILED(name.Proc())))
        {
            STHROWF("Unable to extract a valid listener name");
        }
        // Validate the specified name and assign it
        m_Name.assign(ValidateName(name.mPtr), static_cast< size_t >(name.mLen));
        // Initialize the specifiers to default values
        for (unsigned char & n : m_ArgSpec)
        {
            n = CMDARG_ANY;
        }
        // Apply the specified argument rules/specifications
        SetSpec(spec); // guaranteed the value will not be modified!
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
        m_OnExec.Release();
        m_OnAuth.Release();
        m_OnAudit.Release();
        m_OnPost.Release();
        m_OnFail.Release();
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
    SQMOD_NODISCARD int32_t Cmp(const Listener & o) const
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
    SQMOD_NODISCARD const String & ToString() const
    {
        return m_Name;
    }

    /* --------------------------------------------------------------------------------------------
     * Increment the number of names that point to this command.
    */
    uint16_t AddAliases(uint16_t n)
    {
        m_Aliases += n;
        // Return new alias count
        return m_Aliases;
    }

    /* --------------------------------------------------------------------------------------------
     * Decrement the number of names that point to this command.
    */
    uint16_t SubAliases(uint16_t n)
    {
        m_Aliases -= n;
        // Return new alias count
        return m_Aliases;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of names that point to this command.
    */
    SQMOD_NODISCARD uint16_t GetAliases() const
    {
        return m_Aliases;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the number of names that point to this command.
    */
    void SetAliases(uint16_t n)
    {
        m_Aliases = n;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of weak references to the managed controller.
    */
    SQMOD_NODISCARD uint32_t GetRefCount() const
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
    SQMOD_NODISCARD bool Attached() const
    {
        return (!m_Controller.Expired() && m_Controller.Lock()->Attached(this));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the manager associated with this command listener instance.
    */
    SQMOD_NODISCARD Object GetManager() const
    {
        // Are we even associated with a controller?
        if (m_Controller.Expired())
        {
            return NullObject(); // Default to null
        }
        // Obtain the initial stack size
        const StackGuard sg;
        // Push the instance on the stack
        ClassType< Manager >::PushInstance(SqVM(), m_Controller.Lock()->m_Manager);
        // Grab the instance from the stack
        return Var< Object >(SqVM(), -1).value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name that triggers this command listener instance.
    */
    SQMOD_NODISCARD const String & GetName() const
    {
        return m_Name;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name that triggers this command listener instance.
    */
    void SetName(StackStrF & name)
    {
        // Extract the given name
        if ((SQ_FAILED(name.Proc())))
        {
            STHROWF("Unable to extract a valid listener name");
        }
        // Validate the specified name
        ValidateName(name.mPtr);
        // Is this command already attached to a name?
        if (!m_Controller.Expired() && m_Controller.Lock()->Attached(this))
        {
            const CtrRef ctr = m_Controller.Lock();
            // Detach from the current name if necessary
            ctr->Detach(this);
            // Now it's safe to assign the new name
            m_Name.assign(name.mPtr, static_cast< size_t >(name.mLen));
            // We know the new name is valid
            ctr->Attach(NullObject(), this);
        }
        else
        {
            m_Name.assign(name.mPtr, static_cast< size_t >(name.mLen)); // Just assign the name
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
    SQMOD_NODISCARD const String & GetSpec() const
    {
        return m_Spec;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the argument specification string.
    */
    void SetSpec(StackStrF & spec)
    {
        // Get the string
        if ((SQ_FAILED(spec.Proc())))
        {
            STHROWF("Unable to extract a valid specifier string");
        }
        // Attempt to process the specified string
        ProcSpec(spec.mPtr);
        // Assign the specifier, if any
        if (spec.mLen > 0)
        {
            m_Spec.assign(spec.mPtr, static_cast< size_t >(spec.mLen));
        }
        else
        {
            m_Spec.clear();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the argument tags array.
    */
    SQMOD_NODISCARD Array GetArgTags() const
    {
        // Allocate an array to encapsulate all tags
        Array arr(SqVM(), SQMOD_MAX_CMD_ARGS);
        // Put the tags to the allocated array
        for (uint32_t arg = 0; arg < SQMOD_MAX_CMD_ARGS; ++arg)
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
            for (auto & m_ArgTag : m_ArgTags)
            {
                m_ArgTag.clear();
            }
            // We're done here!
            return;
        }
        // Attempt to retrieve the number of specified tags
        const uint32_t max = ConvTo< uint32_t >::From(tags.Length());
        // If no tags were specified then clear current tags
        if (!max)
        {
            for (auto & m_ArgTag : m_ArgTags)
            {
                m_ArgTag.clear();
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
            STHROWF("Argument tag ({}) is out of range ({})", max, SQMOD_MAX_CMD_ARGS);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the help message associated with this command listener instance.
    */
    SQMOD_NODISCARD const String & GetHelp() const
    {
        return m_Help;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the help message associated with this command listener instance.
    */
    void SetHelp(StackStrF & help)
    {
        if ((SQ_FAILED(help.Proc())))
        {
            STHROWF("Unable to extract a valid help string");
        }
        else if (help.mLen > 0)
        {
            m_Help.assign(help.mPtr, static_cast< size_t >(help.mLen));
        }
        else
        {
            m_Help.clear();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the informational message associated with this command listener instance.
    */
    SQMOD_NODISCARD const String & GetInfo() const
    {
        return m_Info;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the informational message associated with this command listener instance.
    */
    void SetInfo(StackStrF & info)
    {
        if ((SQ_FAILED(info.Proc())))
        {
            STHROWF("Unable to extract a valid information string");
        }
        else if (info.mLen > 0)
        {
            m_Info.assign(info.mPtr, static_cast< size_t >(info.mLen));
        }
        else
        {
            m_Info.clear();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the authority level required to execute this command listener instance.
    */
    SQMOD_NODISCARD SQInteger GetAuthority() const
    {
        return m_Authority;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the authority level required to execute this command listener instance.
    */
    void SetAuthority(SQInteger level)
    {
        m_Authority = ConvTo< int32_t >::From(level);
    }

    /* --------------------------------------------------------------------------------------------
     * See whether this command listener instance requires explicit authority inspection.
    */
    SQMOD_NODISCARD bool GetProtected() const
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
    SQMOD_NODISCARD bool GetSuspended() const
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
    SQMOD_NODISCARD bool GetAssociate() const
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
    SQMOD_NODISCARD uint8_t GetMinArgC() const
    {
        return m_MinArgc;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the minimum arguments supported by this command listener.
    */
    void SetMinArgC(SQInteger val)
    {
        // Limit the specified number withing allowed range
        val = ConvTo< uint8_t >::From(val);
        // Perform a range check on the specified argument index
        if (val >= SQMOD_MAX_CMD_ARGS)
        {
            STHROWF("Argument ({}) is out of total range ({})", val, SQMOD_MAX_CMD_ARGS);
        }
        else if (static_cast< uint8_t >(val) > m_MaxArgc)
        {
            STHROWF("Minimum argument ({}) exceeds maximum ({})", val, m_MaxArgc);
        }
        // Apply the specified value
        m_MinArgc = static_cast< uint8_t >(val);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the maximum arguments supported by this command listener.
    */
    SQMOD_NODISCARD SQInteger GetMaxArgC() const
    {
        return m_MaxArgc;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the maximum arguments supported by this command listener.
    */
    void SetMaxArgC(SQInteger val)
    {
        // Limit the specified number withing allowed range
        val = ConvTo< uint8_t >::From(val);
        // Perform a range check on the specified argument index
        if (val >= SQMOD_MAX_CMD_ARGS)
        {
            STHROWF("Argument ({}) is out of total range ({})", val, SQMOD_MAX_CMD_ARGS);
        }
        else if (static_cast< uint8_t >(val) < m_MinArgc)
        {
            STHROWF("Maximum argument ({}) exceeds minimum ({})", val, m_MinArgc);
        }
        // Apply the specified value
        m_MaxArgc = static_cast< uint8_t >(val);
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
    void SetOnExec(Function & func)
    {
        m_OnExec = std::move(func);
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
    void SetOnAuth(Function & func)
    {
        m_OnAuth = std::move(func);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the function that must be called when this command listener needs to validate invocation.
    */
    Function & GetOnAudit()
    {
        return m_OnAudit;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the function that must be called when this command listener needs to validate invocation.
    */
    void SetOnAudit(Function & func)
    {
        m_OnAudit = std::move(func);
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
    void SetOnPost(Function & func)
    {
        m_OnPost = std::move(func);
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
    void SetOnFail(Function & func)
    {
        m_OnFail = std::move(func);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the tag of a certain argument.
    */
    SQMOD_NODISCARD const String & GetArgTag(uint32_t arg) const
    {
        // Perform a range check on the specified argument index
        if (arg >= SQMOD_MAX_CMD_ARGS)
        {
            STHROWF("Argument ({}) is out of total range ({})", arg, SQMOD_MAX_CMD_ARGS);
        }
        // Return the requested information
        return m_ArgTags[arg];
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the tag of a certain argument.
    */
    void SetArgTag(uint32_t arg, StackStrF & name)
    {
        // Perform a range check on the specified argument index
        if (arg >= SQMOD_MAX_CMD_ARGS)
        {
            STHROWF("Argument ({}) is out of total range ({})", arg, SQMOD_MAX_CMD_ARGS);
        }
        else if ((SQ_FAILED(name.Proc())))
        {
            STHROWF("Unable to extract a valid argument name");
        }
        // The string type doesn't appreciate null values
        else if (name.mLen > 0)
        {
            m_ArgTags[arg].assign(name.mPtr, static_cast< size_t >(name.mLen));
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
    SQMOD_NODISCARD uint8_t GetArgFlags(uint32_t idx) const
    {
        // Perform a range check on the specified argument index
        if (idx >= SQMOD_MAX_CMD_ARGS)
        {
            STHROWF("Argument ({}) is out of total range ({})", idx, SQMOD_MAX_CMD_ARGS);
        }
        // Return the requested information
        return m_ArgSpec[idx];
    }

    /* --------------------------------------------------------------------------------------------
     * See whether whether the specified argument can be used on this command listener instance.
    */
    SQMOD_NODISCARD bool ArgCheck(uint32_t arg, uint8_t flag) const
    {
        // Perform a range check on the specified argument index
        if (arg >= SQMOD_MAX_CMD_ARGS)
        {
            STHROWF("Argument ({}) is out of total range ({})", arg, SQMOD_MAX_CMD_ARGS);
        }
        // Retrieve the argument flags
        const uint8_t f = m_ArgSpec[arg];
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
            return !!ret && *ret;
        }
        // Was there a global authority inspector specified?
        else if (!m_Controller.Expired() && !m_Controller.Lock()->GetOnAuth().IsNull())
        {
            // Ask the specified authority inspector if this execution should be allowed
            const SharedPtr< bool > ret = m_Controller.Lock()->GetOnAuth().Evaluate< bool, const Object &, Listener * >(invoker, this);
            // See what the custom authority inspector said or default to disallow
            return !!ret && *ret;
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

    /* --------------------------------------------------------------------------------------------
     * Create an alias for this command listener.
    */
    Listener & Rebind(StackStrF & name);

protected:

    // --------------------------------------------------------------------------------------------
    typedef uint8_t   ArgSpec[SQMOD_MAX_CMD_ARGS];
    typedef String  ArgTags[SQMOD_MAX_CMD_ARGS];

    /* --------------------------------------------------------------------------------------------
     * Execute the designated audit callback by passing the arguments in their specified order.
    */
    SQInteger Audit(const Object & invoker, const LightObj & args)
    {
        // Attempt to evaluate the specified executer knowing the manager did the validations
        SharedPtr< SQInteger > ret = m_OnAudit.Evaluate< SQInteger, const Object &, const LightObj & >(invoker, args);
        // See if the executer succeeded and return the result or default to failed
        return (!ret ? 0 : *ret);
    }

    /* --------------------------------------------------------------------------------------------
     * Execute the designated callback by passing the arguments in their specified order.
    */
    SQInteger Execute(const Object & invoker, const LightObj & args)
    {
        // Attempt to evaluate the specified executer knowing the manager did the validations
        SharedPtr< SQInteger > ret = m_OnExec.Evaluate< SQInteger, const Object &, const LightObj & >(invoker, args);
        // See if the executer succeeded and return the result or default to failed
        return (!ret ? 0 : *ret);
    }

    /* --------------------------------------------------------------------------------------------
     * Process the specified string and extract the argument properties in it.
    */
    void ProcSpec(const SQChar * spec);

private:

    // --------------------------------------------------------------------------------------------
    CtrPtr      m_Controller; // Manager that controls this command listener.

    // --------------------------------------------------------------------------------------------
    String      m_Name; // Name of the command that triggers this listener.
    Object      m_Data; // Arbitrary user data associated with this particular instance.

    // --------------------------------------------------------------------------------------------
    ArgSpec     m_ArgSpec; // List of argument type specifications.
    ArgTags     m_ArgTags; // List of argument tags/names.

    // --------------------------------------------------------------------------------------------
    uint8_t     m_MinArgc; // Minimum number of arguments supported by this listener.
    uint8_t     m_MaxArgc; // Maximum number of arguments supported by this listener.
    uint16_t    m_Aliases; // Number of aliases that point to this command.

    // --------------------------------------------------------------------------------------------
    String      m_Spec; // String used to generate the argument type specification list.
    String      m_Help; // String describing what the command is supposed to do.
    String      m_Info; // String with syntax information for the command.

    // --------------------------------------------------------------------------------------------
    Function    m_OnExec; // Function to call when the command is executed.
    Function    m_OnAuth; // Function to call when the invoker must be authenticated.
    Function    m_OnAudit; // Function to call when the parameters must be validated.
    Function    m_OnPost; // Function to call after the command was successfully executed.
    Function    m_OnFail; // Function to call after the command execution failed.

    // --------------------------------------------------------------------------------------------
    int32_t     m_Authority; // Built-in authority level required to execute this command.

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
