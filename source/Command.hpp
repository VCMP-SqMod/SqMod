#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <unordered_map>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Flags of the types of values supported by the command arguments.
*/
enum CmdArgType
{
    CMDARG_ANY         = 0,
    CMDARG_INTEGER     = (1 << 1),
    CMDARG_FLOAT       = (1 << 2),
    CMDARG_BOOLEAN     = (1 << 3),
    CMDARG_STRING      = (1 << 4),
};

/* ------------------------------------------------------------------------------------------------
 * Convert a command type specifier to a name string.
*/
const SQChar * CmdArgSpecToStr(Uint8 spec);

/* ------------------------------------------------------------------------------------------------
 * The type of error that's being reported by the command manager.
*/
enum CmdError
{
    CMDERR_UNKNOWN = 0,
    CMDERR_SYNTAX_ERROR,
    CMDERR_UNKNOWN_COMMAND,
    CMDERR_MISSING_EXECUTER,
    CMDERR_INSUFFICIENT_AUTH,
    CMDERR_INCOMPLETE_ARGS,
    CMDERR_EXTRANEOUS_ARGS,
    CMDERR_UNSUPPORTED_ARG,
    CMDERR_EXECUTION_FAILED,
};

// ------------------------------------------------------------------------------------------------
class CmdListener;

// ------------------------------------------------------------------------------------------------
#define MAX_CMD_ARGS 12

/* ------------------------------------------------------------------------------------------------
 * Helper class used to simplify the code for creating and managing commands.
*/
class CmdManager
{
    // --------------------------------------------------------------------------------------------
    friend class std::unique_ptr< CmdManager, void(*)(CmdManager *) >;

protected:

    /* --------------------------------------------------------------------------------------------
     * The type of container for storing command listeners.
    */
    typedef std::unordered_map< String, CmdListener * >             CmdPool;

    /* --------------------------------------------------------------------------------------------
     * The type of container for storing command arguments.
    */
    typedef std::array< std::pair< Uint8, SqObj >, MAX_CMD_ARGS >   CmdArgs;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CmdManager();

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    CmdManager(const CmdManager &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    CmdManager(CmdManager &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CmdManager();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    CmdManager & operator = (const CmdManager &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    CmdManager & operator = (CmdManager &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Called by the smart pointer to delete the instance of this class.
    */
    static void _Finalizer(CmdManager * ptr);

public:

    // --------------------------------------------------------------------------------------------
    typedef std::unique_ptr< CmdManager, void(*)(CmdManager *) >  Pointer;

    /* --------------------------------------------------------------------------------------------
     * Creates an instance of this type if one doesn't already exist and returns it.
    */
    static Pointer Inst();

    /* --------------------------------------------------------------------------------------------
      * Called by the core class before the VM is clodes to release all resources.
    */
    void VMClose();

    /* --------------------------------------------------------------------------------------------
     * Bind a command listener to a certain command name.
    */
    void Bind(const String & name, CmdListener * cmd);

    /* --------------------------------------------------------------------------------------------
     * Unbind a command listener from a certain command name.
    */
    void Unbind(const String & name);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the currently used error handler.
    */
    Function & GetOnError();

    /* --------------------------------------------------------------------------------------------
     * Change the error handler.
    */
    void SetOnError(Function & func);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the default authority inspector.
    */
    Function & GetOnAuth();

    /* --------------------------------------------------------------------------------------------
     * Change the default authority inspector.
    */
    void SetOnAuth(Function & func);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the currently executed command.
    */
    const SQChar * GetName();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the argument text of the currently executed command.
    */
    const SQChar * GetText();

    /* --------------------------------------------------------------------------------------------
     * Execute the specified command for the specified player.
    */
    void Execute(SQInt32 invoker, const String & str);

protected:

    /* --------------------------------------------------------------------------------------------
     * Execute the specified comand.
    */
    void Exec(CmdListener & cmd);

    /* --------------------------------------------------------------------------------------------
     * Forward the error message to the error handler.
    */
    template < typename... Args> void Error(SQInt32 type, const SQChar * msg, Args&&... args)
    {
        // Skip if there's no error handler
        if (!m_OnError.IsNull())
        {
            m_OnError.Execute< SQInt32, const SQChar * >(type, ToStringF(msg, std::forward< Args >(args)...));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Parse the specified argument text and extract the arguments from it.
    */
    bool Parse(SQUint32 max);

private:

    /* --------------------------------------------------------------------------------------------
     * List of command listeners and their associated names.
    */
    CmdPool         m_Commands;

    /* --------------------------------------------------------------------------------------------
     * Currently and last command invoker.
    */
    SQInt32         m_Invoker;

    /* --------------------------------------------------------------------------------------------
     * Currently and last used command name.
    */
    String          m_Name;

    /* --------------------------------------------------------------------------------------------
     * Currently and last used command argument text.
    */
    String          m_Text;

    /* --------------------------------------------------------------------------------------------
     * Extracted values from the argument text.
    */
    CmdArgs         m_Argv;

    /* --------------------------------------------------------------------------------------------
     * Number of values extracted from the argument text.
    */
    SQUint32        m_Argc;

    /* --------------------------------------------------------------------------------------------
     * Custom error handler.
    */
    Function        m_OnError;

    /* --------------------------------------------------------------------------------------------
     * Default authority inspector for newlly created commands.
    */
    Function        m_OnAuth;
};

// ------------------------------------------------------------------------------------------------
extern const CmdManager::Pointer _Cmd;

/* ------------------------------------------------------------------------------------------------
 * Class used to bind to certain commands.
*/
class CmdListener
{
public:

    // --------------------------------------------------------------------------------------------
    friend class CmdManager;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CmdListener();

    /* --------------------------------------------------------------------------------------------
     * Construct and instance and attach it to the specified name.
    */
    CmdListener(const SQChar * name);

    /* --------------------------------------------------------------------------------------------
     * Construct and instance and attach it to the specified name.
    */
    CmdListener(const SQChar * name, const SQChar * spec);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    CmdListener(const CmdListener & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    CmdListener(CmdListener && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CmdListener();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    CmdListener & operator = (const CmdListener & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    CmdListener & operator = (CmdListener && o) = delete;

    /* --------------------------------------------------------------------------------------------
      * Used to released stored resouces before the VM is closed.
    */
    void VMClose();

    /* --------------------------------------------------------------------------------------------
     * Compare two instances of this type.
    */
    SQInt32 Cmp(const CmdListener & o) const;

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert this instance to a string.
    */
    const SQChar * ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the local tag.
    */
    const SQChar * GetTag() const;

    /* --------------------------------------------------------------------------------------------
     * Change the local tag.
    */
    void SetTag(const SQChar * tag);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the local data.
    */
    SqObj & GetData();

    /* --------------------------------------------------------------------------------------------
     * Change the local data.
    */
    void SetData(SqObj & data);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the command.
    */
    const SQChar * GetName() const;

    /* --------------------------------------------------------------------------------------------
     * Change the name of the command.
    */
    void SetName(const SQChar * name);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the argument type specifiers for this command as a string.
    */
    const SQChar * GetSpec() const;

    /* --------------------------------------------------------------------------------------------
     * Change the argument type specifiers for this command by extracing them from a string.
    */
    void SetSpec(const SQChar * spec);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the help text associated with this command.
    */
    const SQChar * GetHelp() const;

    /* --------------------------------------------------------------------------------------------
     * Change the help text associated with this command.
    */
    void SetHelp(const SQChar * help);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the informational text associated with this command.
    */
    const SQChar * GetInfo() const;

    /* --------------------------------------------------------------------------------------------
     * Chage the informational text associated with this command.
    */
    void SetInfo(const SQChar * info);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the function responsible for testing the invoker authority.
    */
    Function & GetOnAuth();

    /* --------------------------------------------------------------------------------------------
     * Change the function responsible for testing the invoker authority.
    */
    void SetOnAuth(Function & func);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the function responsible for processing the command.
    */
    Function & GetOnExec();

    /* --------------------------------------------------------------------------------------------
     * Change the function responsible for processing the command.
    */
    void SetOnExec(Function & func);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal level required to execute this command.
    */
    SQInt32 GetLevel() const;

    /* --------------------------------------------------------------------------------------------
     * Change the internal level required to execute this command.
    */
    void SetLevel(SQInt32 level);

    /* --------------------------------------------------------------------------------------------
     * See whether this command listener is allowed to execute or not.
    */
    bool GetSuspended() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether this command listener is allowed to execute or not.
    */
    void SetSuspended(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * See whether this command needs explicit authority clearance to execute.
    */
    bool GetAuthority() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether this command needs explicit authority clearance to execute.
    */
    void SetAuthority(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the minimum arguments allowed required to execute this command.
    */
    SQUint32 GetMinArgC() const;

    /* --------------------------------------------------------------------------------------------
     * Change the minimum arguments allowed required to execute this command.
    */
    void SetMinArgC(SQUint32 val);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the maximum arguments allowed required to execute this command.
    */
    SQUint32 GetMaxArgC() const;

    /* --------------------------------------------------------------------------------------------
     * Change the maximum arguments allowed required to execute this command.
    */
    void SetMaxArgC(SQUint32 val);

    /* --------------------------------------------------------------------------------------------
     * Check whether the specified argument is compatible with the specified type.
    */
    bool ArgCheck(SQUint32 arg, Uint8 mask) const;

    /* --------------------------------------------------------------------------------------------
     * Sheck whether the specified player is allowed to execute this command.
    */
    bool AuthCheck(SQInt32 id);

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute this command.
    */
    bool Execute(SQInt32 invoker, Array & args);

protected:

    // --------------------------------------------------------------------------------------------
    typedef std::array< Uint8 , MAX_CMD_ARGS > Args;

    /* --------------------------------------------------------------------------------------------
     * Process the specifiers string.
    */
    bool ProcSpec(const SQChar * spec);

private:

    /* --------------------------------------------------------------------------------------------
     * Array of type specifiers for each of the command arguments.
    */
    Args        m_Args;

    /* --------------------------------------------------------------------------------------------
     * Minimum arguments allowed to execute this command.
    */
    SQUint32    m_MinArgc;

    /* --------------------------------------------------------------------------------------------
     * Maximum arguments allowed to execute this command.
    */
    SQUint32    m_MaxArgc;

    /* --------------------------------------------------------------------------------------------
     * The name of the command.
    */
    String      m_Name;

    /* --------------------------------------------------------------------------------------------
     * The specifiers for each of the command arguments represented as a string.
    */
    String      m_Spec;

    /* --------------------------------------------------------------------------------------------
     * Help about the purpose and requirements of the command.
    */
    String      m_Help;

    /* --------------------------------------------------------------------------------------------
     * Information for when the command execution failed.
    */
    String      m_Info;

    /* --------------------------------------------------------------------------------------------
     * Function responsible for deciding whether the invoker is allowed to execute.
    */
    Function    m_OnAuth;

    /* --------------------------------------------------------------------------------------------
     * Function responsible for processing the received command arguments.
    */
    Function    m_OnExec;

    /* --------------------------------------------------------------------------------------------
     * Arbitrary tag associated with this instance.
    */
    String      m_Tag;

    /* --------------------------------------------------------------------------------------------
     * Arbitrary data associated with this instance.
    */
    SqObj       m_Data;

    /* --------------------------------------------------------------------------------------------
     * The level required to execute this command.
    */
    SQInt32     m_Level;

    /* --------------------------------------------------------------------------------------------
     * Whether the command is allowed to execute or not.
    */
    bool        m_Suspended;

    /* --------------------------------------------------------------------------------------------
     * Whether this command needs an explicit authority verification in order to execute.
    */
    bool        m_Authority;

    /* --------------------------------------------------------------------------------------------
     * Whether the command is allowed to change name.
    */
    bool        m_Lock;
};

} // Namespace:: SqMod

#endif // _COMMAND_HPP_
