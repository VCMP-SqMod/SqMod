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

    // --------------------------------------------------------------------------------------------
    typedef std::map< String, CmdListener * > CmdList;

    // --------------------------------------------------------------------------------------------
    typedef std::vector< std::pair< Uint8, Object > > CmdArgs;

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
    void Attach(const String & name, CmdListener * cmd)
    {
        m_Commands[name] = cmd;
    }

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
     * Terminate current session.
    */
    void Terminate();

    /* --------------------------------------------------------------------------------------------
     * Dettach a command listener from a certain name.
    */
    void Detach(const String & name)
    {
        m_Commands.erase(name);
    }

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
     * Retrieve the last executed command.
    */
    CSStr GetCommand() const
    {
        return m_Command.c_str();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the argument of the last executed coommand.
    */
    CSStr GetArgument() const
    {
        return m_Argument.c_str();
    }

    /* --------------------------------------------------------------------------------------------
     * Run a command under a speciffic player.
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
    Buffer          m_Buffer; /* Internal buffer used for parsing commands. */
    CmdList         m_Commands; /* List of attached commands. */

    // --------------------------------------------------------------------------------------------
    Int32           m_Invoker; /* Current command invoker. */
    String          m_Command; /* Current command name. */
    String          m_Argument; /* Current command argument. */
    CmdListener*    m_Instance; /* Current command instance. */

    // --------------------------------------------------------------------------------------------
    CmdArgs         m_Argv; /* Extracted command arguments. */
    Uint32          m_Argc; /* Extracted arguments count. */

    // --------------------------------------------------------------------------------------------
    Function        m_OnError; /* Error handler. */
    Function        m_OnAuth; /* Authentication handler. */
};

/* ------------------------------------------------------------------------------------------------
 * Attaches to a command name and listens for invocations.
*/
class CmdListener
{
    // --------------------------------------------------------------------------------------------
    friend class CmdManager;

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
     * Base constructors.
    */
    CmdListener(CSStr name);
    CmdListener(CSStr name, CSStr spec);
    CmdListener(CSStr name, CSStr spec, Array & tags);
    CmdListener(CSStr name, CSStr spec, Uint8 min, Uint8 max);
    CmdListener(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max);

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

    // --------------------------------------------------------------------------------------------
    Uint8 GetArgFlags(Uint32 idx) const;

    // --------------------------------------------------------------------------------------------
    CSStr GetName() const;
    void SetName(CSStr name);

    // --------------------------------------------------------------------------------------------
    CSStr GetSpec() const;
    void SetSpec(CSStr spec);

    // --------------------------------------------------------------------------------------------
    Array GetArgTags() const;
    void SetArgTags(Array & tags);

    // --------------------------------------------------------------------------------------------
    CSStr GetHelp() const;
    void SetHelp(CSStr help);

    // --------------------------------------------------------------------------------------------
    CSStr GetInfo() const;
    void SetInfo(CSStr info);

    // --------------------------------------------------------------------------------------------
    Int32 GetAuthority() const;
    void SetAuthority(Int32 level);

    // --------------------------------------------------------------------------------------------
    bool GetProtected() const;
    void SetProtected(bool toggle);

    // --------------------------------------------------------------------------------------------
    bool GetSuspended() const;
    void SetSuspended(bool toggle);

    // --------------------------------------------------------------------------------------------
    bool GetAssociate() const;
    void SetAssociate(bool toggle);

    // --------------------------------------------------------------------------------------------
    Uint8 GetMinArgC() const;
    void SetMinArgC(Uint8 val);

    // --------------------------------------------------------------------------------------------
    Uint8 GetMaxArgC() const;
    void SetMaxArgC(Uint8 val);

    // --------------------------------------------------------------------------------------------
    bool GetLocked() const;

    // --------------------------------------------------------------------------------------------
    Function & GetOnExec();
    void SetOnExec(Object & env, Function & func);

    // --------------------------------------------------------------------------------------------
    Function & GetOnAuth();
    void SetOnAuth(Object & env, Function & func);

    // --------------------------------------------------------------------------------------------
    Function & GetOnPost();
    void SetOnPost(Object & env, Function & func);

    // --------------------------------------------------------------------------------------------
    Function & GetOnFail();
    void SetOnFail(Object & env, Function & func);

    // --------------------------------------------------------------------------------------------
    CSStr GetArgTag(Uint32 arg) const;
    void SetArgTag(Uint32 arg, CSStr name);

    // --------------------------------------------------------------------------------------------
    void GenerateInfo(bool full);

    // --------------------------------------------------------------------------------------------
    bool ArgCheck(Uint32 arg, Uint8 flag) const;

    // --------------------------------------------------------------------------------------------
    bool AuthCheck(CPlayer & player);
    bool AuthCheckID(Int32 id);

protected:

    // --------------------------------------------------------------------------------------------
    typedef Uint8   ArgSpec[SQMOD_MAX_CMD_ARGS];
    typedef String  ArgTags[SQMOD_MAX_CMD_ARGS];

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQInteger Execute(Object & invoker, Array & args);

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQInteger Execute(Object & invoker, Table & args);

    /* --------------------------------------------------------------------------------------------
     *
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
    bool        m_Locked;
};

} // Namespace:: SqMod

#endif // _COMMAND_HPP_
