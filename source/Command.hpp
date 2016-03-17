#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"
#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <map>
#include <vector>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
class CmdListener;

/* ------------------------------------------------------------------------------------------------
 * Converts a command specifier to a string.
*/
CSStr CmdArgSpecToStr(Uint8 spec);

// ------------------------------------------------------------------------------------------------
extern CmdManager * _Cmd;

/* ------------------------------------------------------------------------------------------------
 * Manages command instances and processes executed commands.
*/
class CmdManager
{
    // --------------------------------------------------------------------------------------------
    friend class CmdListener;

private:

    /* --------------------------------------------------------------------------------------------
     * Structure that represents a unique command in the pool.
    */
    struct Command
    {
        // ----------------------------------------------------------------------------------------
        std::size_t     mHash; // The unique hash that identifies this command.
        String          mName; // The unique name that identifies this command.
        CmdListener*    mPtr; // The listener that reacts to this command.
        Object          mObj; // A strong reference to the script object.

        /* ----------------------------------------------------------------------------------------
         * Construct a command and the also create a script object from the specified listener.
        */
        Command(std::size_t hash, const String & name, CmdListener * ptr)
            : mHash(hash), mName(name), mPtr(ptr), mObj(ptr)
        {
            /* ... */
        }

        /* ----------------------------------------------------------------------------------------
         * Construct a command and extract the listener from the specified script object.
        */
        Command(std::size_t hash, const String & name, const Object & obj)
            : mHash(hash), mName(name), mPtr(obj.Cast< CmdListener * >()), mObj(obj)
        {
            /* ... */
        }

        /* ----------------------------------------------------------------------------------------
         * Copy constructor.
        */
        Command(const Command & o) = default;

        /* ----------------------------------------------------------------------------------------
         * Move constructor.
        */
        Command(Command && o) = default;

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Command() = default;

        /* ----------------------------------------------------------------------------------------
         * Copy assignment operator.
        */
        Command & operator = (const Command & o) = default;

        /* ----------------------------------------------------------------------------------------
         * Move assignment operator.
        */
        Command & operator = (Command && o) = default;
    };

    // --------------------------------------------------------------------------------------------
    typedef std::vector< Command >      CmdList;

    // --------------------------------------------------------------------------------------------
    typedef std::pair< Uint8, Object >  CmdArg;
    typedef std::vector< CmdArg >       CmdArgs;

    /* --------------------------------------------------------------------------------------------
     * Helper class implementing RAII to release the command object and instance.
    */
    struct Guard
    {
        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        Guard() = default;

        /* ----------------------------------------------------------------------------------------
         * Copy constructor.
        */
        Guard(const Guard & o) = delete;

        /* ----------------------------------------------------------------------------------------
         * Move constructor.
        */
        Guard(Guard && o) = delete;

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Guard();

        /* ----------------------------------------------------------------------------------------
         * Copy assignment operator.
        */
        Guard & operator = (const Guard & o) = delete;

        /* ----------------------------------------------------------------------------------------
         * Move assignment operator.
        */
        Guard & operator = (Guard && o) = delete;
    };

    // --------------------------------------------------------------------------------------------
    friend class Guard;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CmdManager();

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CmdManager(const CmdManager &);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    CmdManager & operator = (const CmdManager &);

    /* --------------------------------------------------------------------------------------------
     * Attach a command listener to a certain name.
    */
    Object & Attach(const String & name, CmdListener * ptr, bool autorel);

    /* --------------------------------------------------------------------------------------------
     * Detach a command listener from a certain name.
    */
    void Detach(const String & name);

    /* --------------------------------------------------------------------------------------------
     * Detach a command listener from a certain name.
    */
    void Detach(CmdListener * ptr);

    /* --------------------------------------------------------------------------------------------
     * See whether a certain name exist in the command list.
    */
    bool Attached(const String & name) const;

    /* --------------------------------------------------------------------------------------------
     * See whether a certain instance exist in the command list.
    */
    bool Attached(const CmdListener * ptr) const;

public:

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CmdManager();

    /* --------------------------------------------------------------------------------------------
     * Singleton retriever.
    */
    static CmdManager * Get()
    {
        if (!_Cmd)
        {
            _Cmd = new CmdManager();
        }

        return _Cmd;
    }

    /* --------------------------------------------------------------------------------------------
     * Sort the command list in an ascending order.
    */
    void Sort();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of commands.
    */
    Uint32 Count() const
    {
        return static_cast< Uint32 >(m_Commands.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Sort the command list in an ascending order.
    */
    const Object & FindByName(const String & name);

    /* --------------------------------------------------------------------------------------------
     * Terminate current session.
    */
    void Terminate();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the error callback.
    */
    Function & GetOnError()
    {
        return m_OnError;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the error callback.
    */
    void SetOnError(Object & env, Function & func)
    {
        m_OnError = Function(env.GetVM(), env, func.GetFunc());
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
        m_OnAuth = Function(env.GetVM(), env, func.GetFunc());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier of the last invoker.
    */
    Int32 GetInvoker() const
    {
        return m_Invoker;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the currently active command instance.
    */
    const CmdListener * GetInstance() const
    {
        return m_Instance;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the currently active command object.
    */
    const Object & GetObject() const
    {
        return m_Object;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the currently active command name.
    */
    const String & GetCommand() const
    {
        return m_Command;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the currently active command argument.
    */
    const String & GetArgument() const
    {
        return m_Argument;
    }

    /* --------------------------------------------------------------------------------------------
     * Run a command under a specific player.
    */
    Int32 Run(Int32 invoker, CSStr command);

protected:

    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Forward error message to the error callback.
    */
    template < typename T > void SqError(Int32 type, CSStr msg, T data)
    {
        // Is there a callback that deals with errors?
        if (m_OnError.IsNull())
            return;
        // Attempt to forward the error to that callback
        try
        {
            m_OnError.Execute< Int32, CSStr, T >(type, msg, data);
        }
        catch (const Sqrat::Exception & e)
        {
            // We can only log this incident and in the future maybe also include the location
            LogErr("Command error callback failed [%s]", e.Message().c_str());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute the specified command.
    */
    Int32 Exec();

    /* --------------------------------------------------------------------------------------------
     * Attempt to parse the specified argument.
    */
    bool Parse();

private:

    // --------------------------------------------------------------------------------------------
    Buffer          m_Buffer; // Internal buffer used for parsing commands.
    CmdList         m_Commands; // List of created commands.

    // --------------------------------------------------------------------------------------------
    Int32           m_Invoker; // Current command invoker.
    String          m_Command; // Current command name.
    String          m_Argument; // Current command argument.
    CmdListener*    m_Instance; // Current command instance.
    Object          m_Object; // Current command script object.

    // --------------------------------------------------------------------------------------------
    CmdArgs         m_Argv; // Extracted command arguments.
    Uint32          m_Argc; // Extracted arguments count.

    // --------------------------------------------------------------------------------------------
    Function        m_OnError; // Error handler.
    Function        m_OnAuth; // Authentication handler.

public:

    /* --------------------------------------------------------------------------------------------
     * Create command instances and obtain the associated object.
    */
    Object & Create(CSStr name);
    Object & Create(CSStr name, CSStr spec);
    Object & Create(CSStr name, CSStr spec, Array & tags);
    Object & Create(CSStr name, CSStr spec, Uint8 min, Uint8 max);
    Object & Create(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max);
};

/* ------------------------------------------------------------------------------------------------
 * Attaches to a command name and listens for invocations.
*/
class CmdListener
{
    // --------------------------------------------------------------------------------------------
    friend class CmdManager;

    /* --------------------------------------------------------------------------------------------
     * Base constructors.
    */
    CmdListener(CSStr name);
    CmdListener(CSStr name, CSStr spec);
    CmdListener(CSStr name, CSStr spec, Array & tags);
    CmdListener(CSStr name, CSStr spec, Uint8 min, Uint8 max);
    CmdListener(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    CmdListener(const CmdListener &);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    CmdListener & operator = (const CmdListener &);

    /* --------------------------------------------------------------------------------------------
     * Initialize the instance for the first time.
    */
    void Init(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max);

public:

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CmdListener();

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const CmdListener & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert this instance to a string.
    */
    CSStr ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Attach the listener instance to the associated command name.
    */
    void Attach();

    /* --------------------------------------------------------------------------------------------
     * Detach the listener instance from the associated command name.
    */
    void Detach();

    /* --------------------------------------------------------------------------------------------
     * See whether the listener instance is attached to the associated command name.
    */
    bool Attached() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the flags of the specified argument.
    */
    Uint8 GetArgFlags(Uint32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name that triggers this command listener instance.
    */
    CSStr GetName() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name that triggers this command listener instance.
    */
    void SetName(CSStr name);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the argument specification string.
    */
    CSStr GetSpec() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the argument specification string.
    */
    void SetSpec(CSStr spec);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the argument tags array.
    */
    Array GetArgTags() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the argument tags array.
    */
    void SetArgTags(Array & tags);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the help message associated with this command listener instance.
    */
    CSStr GetHelp() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the help message associated with this command listener instance.
    */
    void SetHelp(CSStr help);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the informational message associated with this command listener instance.
    */
    CSStr GetInfo() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the informational message associated with this command listener instance.
    */
    void SetInfo(CSStr info);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the authority level required to execute this command listener instance.
    */
    Int32 GetAuthority() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the authority level required to execute this command listener instance.
    */
    void SetAuthority(Int32 level);

    /* --------------------------------------------------------------------------------------------
     * See whether this command listener instance requires explicit authority inspection.
    */
    bool GetProtected() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether this command listener instance requires explicit authority inspection.
    */
    void SetProtected(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * See whether this command listener instance is currently ignoring calls.
    */
    bool GetSuspended() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether this command listener instance should start ignoring calls.
    */
    void SetSuspended(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * See whether this command listener instance receives arguments in an associative container.
    */
    bool GetAssociate() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether this command listener instance receives arguments in an associative container.
    */
    void SetAssociate(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the maximum arguments supported by this command listener.
    */
    Uint8 GetMinArgC() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the minimum arguments supported by this command listener.
    */
    void SetMinArgC(Uint8 val);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the maximum arguments supported by this command listener.
    */
    Uint8 GetMaxArgC() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the maximum arguments supported by this command listener.
    */
    void SetMaxArgC(Uint8 val);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the function that must be called when this command listener is executed.
    */
    Function & GetOnExec();

    /* --------------------------------------------------------------------------------------------
     * Modify the function that must be called when this command listener is executed.
    */
    void SetOnExec(Object & env, Function & func);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the function that must be called when this command listener needs to authenticate.
    */
    Function & GetOnAuth();

    /* --------------------------------------------------------------------------------------------
     * Modify the function that must be called when this command listener needs to authenticate.
    */
    void SetOnAuth(Object & env, Function & func);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the function that must be called when this command listener finished execution.
    */
    Function & GetOnPost();

    /* --------------------------------------------------------------------------------------------
     * Modify the function that must be called when this command listener finished execution.
    */
    void SetOnPost(Object & env, Function & func);

    // --------------------------------------------------------------------------------------------
    Function & GetOnFail();
    void SetOnFail(Object & env, Function & func);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the function that must be called when this command listener failed to execute.
    */
    CSStr GetArgTag(Uint32 arg) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the function that must be called when this command listener failed to execute.
    */
    void SetArgTag(Uint32 arg, CSStr name);

    /* --------------------------------------------------------------------------------------------
     * Use the command listener argument properties to generate an informational message.
    */
    void GenerateInfo(bool full);

    /* --------------------------------------------------------------------------------------------
     * See whether whether the specified argument can be used on this command listener instance.
    */
    bool ArgCheck(Uint32 arg, Uint8 flag) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the specified player entity has the proper authority to run this command.
    */
    bool AuthCheck(CPlayer & player);

    /* --------------------------------------------------------------------------------------------
     * See whether the specified player entity has the proper authority to run this command.
    */
    bool AuthCheckID(Int32 id);

protected:

    // --------------------------------------------------------------------------------------------
    typedef Uint8   ArgSpec[SQMOD_MAX_CMD_ARGS];
    typedef String  ArgTags[SQMOD_MAX_CMD_ARGS];

    /* --------------------------------------------------------------------------------------------
     * Execute the designated callback by passing the arguments in their specified order.
    */
    SQInteger Execute(Object & invoker, Array & args);

    /* --------------------------------------------------------------------------------------------
     * Execute the designated callback by passing the arguments using an associative container.
    */
    SQInteger Execute(Object & invoker, Table & args);

    /* --------------------------------------------------------------------------------------------
     * Process the specified string and extract the argument properties in it.
    */
    void ProcSpec(CSStr spec);

private:

    // --------------------------------------------------------------------------------------------
    String      m_Name;

    // --------------------------------------------------------------------------------------------
    ArgSpec     m_ArgSpec;
    ArgTags     m_ArgTags;

    // --------------------------------------------------------------------------------------------
    Uint8       m_MinArgc;
    Uint8       m_MaxArgc;

    // --------------------------------------------------------------------------------------------
    String      m_Spec;
    String      m_Help;
    String      m_Info;

    // --------------------------------------------------------------------------------------------
    Function    m_OnExec;
    Function    m_OnAuth;
    Function    m_OnPost;
    Function    m_OnFail;

    // --------------------------------------------------------------------------------------------
    Int32       m_Authority;

    // --------------------------------------------------------------------------------------------
    bool        m_Protected;
    bool        m_Suspended;
    bool        m_Associate;
};

} // Namespace:: SqMod

#endif // _COMMAND_HPP_
