// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Session.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_OS_WINDOWS
    #include <Winsock2.h>
#endif // SQMOD_OS_WINDOWS

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Bind specific functions to certain server events.
*/
void BindCallbacks();

/* ------------------------------------------------------------------------------------------------
 * Undo changes made with BindCallbacks().
*/
void UnbindCallbacks();

/* ------------------------------------------------------------------------------------------------
 * Register the module API under the specified virtual machine.
*/
void RegisterAPI(HSQUIRRELVM vm);

/* ------------------------------------------------------------------------------------------------
 * Initialize the plug-in by obtaining the API provided by the host plug-in.
*/
void OnSquirrelInitialize()
{
    // Attempt to import the plug-in API exported by the host plug-in
    _SqMod = sq_api_import(_Func);
    // Did we failed to obtain the plug-in exports?
    if (!_SqMod)
    {
        OutputError("Failed to attach [%s] on host plug-in.", SQIRC_NAME);
    }
    else
    {
        // Obtain the Squirrel API
        _SqAPI = _SqMod->GetSquirrelAPI();
        // Expand the Squirrel API into global functions
        sq_api_expand(_SqAPI);
    }
}

/* ------------------------------------------------------------------------------------------------
 * Load the module on the virtual machine provided by the host module.
*/
void OnSquirrelLoad()
{
    // Make sure that we have a valid plug-in API
    if (!_SqMod)
    {
        return; // Unable to proceed!
    }
    // Obtain the Squirrel API and VM
    _SqVM = _SqMod->GetSquirrelVM();
    // Make sure that a valid virtual machine exists
    if (!_SqVM)
    {
        return; // Unable to proceed!
    }
    // Set this as the default database
    DefaultVM::Set(_SqVM);
    // Prevent common null objects from using dead virtual machines
    NullArray() = Array();
    NullTable() = Table();
    NullObject() = Object();
    NullFunction() = Function();
    // Register the module API
    RegisterAPI(_SqVM);
    // Notify about the current status
    OutputMessage("Registered: %s", SQIRC_NAME);
}

/* ------------------------------------------------------------------------------------------------
 * The virtual machine is about to be terminated and script resources should be released.
*/
void OnSquirrelTerminate()
{
    OutputMessage("Terminating: %s", SQIRC_NAME);
    // Release null objects just in case
    NullObject().Release();
    NullTable().Release();
    NullArray().Release();
    NullFunction().ReleaseGently();
}

/* ------------------------------------------------------------------------------------------------
 * The virtual machined was closed and all memory associated with it was released.
*/
void OnSquirrelReleased()
{
    // Release the current virtual machine, if any
    DefaultVM::Set(nullptr);
    // Terminate all sessions, if any
    Session::Terminate();
}

/* ------------------------------------------------------------------------------------------------
 * Validate the module API to make sure we don't run into issues.
*/
bool CheckAPIVer(CCStr ver)
{
    // Obtain the numeric representation of the API version
    const LongI vernum = std::strtol(ver, nullptr, 10);
    // Check against version mismatch
    if (vernum == SQMOD_API_VER)
    {
        return true;
    }
    // Log the incident
    OutputError("API version mismatch on %s", SQIRC_NAME);
    OutputMessage("=> Requested: %ld Have: %ld", vernum, SQMOD_API_VER);
    // Invoker should not attempt to communicate through the module API
    return false;
}

/* ------------------------------------------------------------------------------------------------
 * React to command sent by other plug-ins.
*/
static uint8_t OnPluginCommand(uint32_t command_identifier, CCStr message)
{
    switch(command_identifier)
    {
        case SQMOD_INITIALIZE_CMD:
            if (CheckAPIVer(message))
            {
                OnSquirrelInitialize();
            }
        break;
        case SQMOD_LOAD_CMD:
            OnSquirrelLoad();
        break;
        case SQMOD_TERMINATE_CMD:
            OnSquirrelTerminate();
        break;
        case SQMOD_RELEASED_CMD:
            OnSquirrelReleased();
        break;
        default: break;
    }
    return 1;
}

/* ------------------------------------------------------------------------------------------------
 * The server was initialized and this plug-in was loaded successfully.
*/
static uint8_t OnServerInitialise()
{
    return 1;
}

static void OnServerShutdown(void)
{
    // The server may still send callbacks
    UnbindCallbacks();
}

/* ------------------------------------------------------------------------------------------------
 * Update the plug-in on each frame.
*/
static void OnServerFrame(float /*delta*/)
{
    // Update the sessions and pool for events
    Session::Process();
}

// ------------------------------------------------------------------------------------------------
void BindCallbacks()
{
    _Clbk->OnServerInitialise       = OnServerInitialise;
    _Clbk->OnServerShutdown         = OnServerShutdown;
    _Clbk->OnServerFrame            = OnServerFrame;
    _Clbk->OnPluginCommand          = OnPluginCommand;
}

// ------------------------------------------------------------------------------------------------
void UnbindCallbacks()
{
    _Clbk->OnServerInitialise       = nullptr;
    _Clbk->OnServerShutdown         = nullptr;
    _Clbk->OnServerFrame            = nullptr;
    _Clbk->OnPluginCommand          = nullptr;
}

// ------------------------------------------------------------------------------------------------
void RegisterAPI(HSQUIRRELVM vm)
{
    Table ircns(vm);

    ircns.Bind(_SC("Session"), Class< Session, NoCopy< Session > >(vm, _SC("SqIrcSession"))
        // Constructors
        .Ctor()
        // Core Meta-methods
        .Func(_SC("_cmp"), &Session::Cmp)
        .SquirrelFunc(_SC("_typename"), &Session::Typename)
        .Func(_SC("_tostring"), &Session::ToString)
        // Properties
        .Prop(_SC("Valid"), &Session::IsValid)
        .Prop(_SC("Connected"), &Session::IsConnected)
        .Prop(_SC("Tag"), &Session::GetTag, &Session::SetTag)
        .Prop(_SC("Data"), &Session::GetData, &Session::SetData)
        .Prop(_SC("Server"), &Session::GetServer, &Session::SetServer)
        .Prop(_SC("Password"), &Session::GetPassword, &Session::SetPassword)
        .Prop(_SC("Nick"), &Session::GetNick, &Session::SetNick)
        .Prop(_SC("User"), &Session::GetUser, &Session::SetUser)
        .Prop(_SC("Name"), &Session::GetName, &Session::SetName)
        .Prop(_SC("Port"), &Session::GetPort, &Session::SetPort)
        .Prop(_SC("PoolTime"), &Session::GetPoolTime, &Session::SetPoolTime)
        .Prop(_SC("LastCode"), &Session::GetLastCode)
        .Prop(_SC("Tries"), &Session::GetTries, &Session::SetTries)
        .Prop(_SC("Wait"), &Session::GetWait, &Session::SetWait)
        .Prop(_SC("LeftTries"), &Session::GetLeftTries, &Session::SetLeftTries)
        .Prop(_SC("NextTry"), &Session::GetNextTry, &Session::SetNextTry)
        .Prop(_SC("SessionTime"), &Session::GetSessionTime)
        .Prop(_SC("Reconnecting"), &Session::GetReconnect)
        .Prop(_SC("IPv6"), &Session::GetIPv6)
        .Prop(_SC("CtcpVersion"), (void (Session::*)(void))(nullptr), &Session::SetCtcpVersion)
        .Prop(_SC("ErrNo"), &Session::GetErrNo)
        .Prop(_SC("ErrStr"), &Session::GetErrStr)
        // Member Methods
        .Func(_SC("Bind"), &Session::BindEvent)
        .Func(_SC("GetEvent"), &Session::GetEvent)
        .Func(_SC("Disconnect"), &Session::Disconnect)
        .Func(_SC("CmdPart"), &Session::CmdPart)
        .Func(_SC("CmdInvite"), &Session::CmdInvite)
        .Func(_SC("CmdNames"), &Session::CmdNames)
        .Func(_SC("CmdMsg"), &Session::CmdMsg)
        .Func(_SC("CmdMe"), &Session::CmdMe)
        .Func(_SC("CmdNotice"), &Session::CmdNotice)
        .Func(_SC("CmdCtcpRequest"), &Session::CmdCtcpRequest)
        .Func(_SC("CmdCtcpReply"), &Session::CmdCtcpReply)
        .Func(_SC("CmdNick"), &Session::CmdNick)
        .Func(_SC("CmdWhois"), &Session::CmdWhois)
        .Func(_SC("SendRaw"), &Session::SendRaw)
        .Func(_SC("DestroyDcc"), &Session::DestroyDcc)
        .Func(_SC("SetCtcpVersion"), &Session::SetCtcpVersion)
        .Func(_SC("SetOption"), &Session::SetOption)
        .Func(_SC("ResetOption"), &Session::ResetOption)
        // Member Overloads
        .Overload< Int32 (Session::*)(void) >(_SC("Connect"), &Session::Connect)
        .Overload< Int32 (Session::*)(CSStr, Uint32, CSStr) >(_SC("Connect"), &Session::Connect)
        .Overload< Int32 (Session::*)(CSStr, Uint32, CSStr, CSStr) >(_SC("Connect"), &Session::Connect)
        .Overload< Int32 (Session::*)(CSStr, Uint32, CSStr, CSStr, CSStr) >(_SC("Connect"), &Session::Connect)
        .Overload< Int32 (Session::*)(CSStr, Uint32, CSStr, CSStr, CSStr, CSStr) >(_SC("Connect"), &Session::Connect)
        .Overload< Int32 (Session::*)(void) >(_SC("Connect6"), &Session::Connect6)
        .Overload< Int32 (Session::*)(CSStr, Uint32, CSStr) >(_SC("Connect6"), &Session::Connect6)
        .Overload< Int32 (Session::*)(CSStr, Uint32, CSStr, CSStr) >(_SC("Connect6"), &Session::Connect6)
        .Overload< Int32 (Session::*)(CSStr, Uint32, CSStr, CSStr, CSStr) >(_SC("Connect6"), &Session::Connect6)
        .Overload< Int32 (Session::*)(CSStr, Uint32, CSStr, CSStr, CSStr, CSStr) >(_SC("Connect6"), &Session::Connect6)
        .Overload< Int32 (Session::*)(CSStr) >(_SC("CmdJoin"), &Session::CmdJoin)
        .Overload< Int32 (Session::*)(CSStr, CSStr) >(_SC("CmdJoin"), &Session::CmdJoin)
        .Overload< Int32 (Session::*)(void) >(_SC("CmdList"), &Session::CmdList)
        .Overload< Int32 (Session::*)(CSStr) >(_SC("CmdList"), &Session::CmdList)
        .Overload< Int32 (Session::*)(CSStr) >(_SC("CmdTopic"), &Session::CmdTopic)
        .Overload< Int32 (Session::*)(CSStr, CSStr) >(_SC("CmdTopic"), &Session::CmdTopic)
        .Overload< Int32 (Session::*)(CSStr) >(_SC("CmdChannelMode"), &Session::CmdChannelMode)
        .Overload< Int32 (Session::*)(CSStr, CSStr) >(_SC("CmdChannelMode"), &Session::CmdChannelMode)
        .Overload< Int32 (Session::*)(void) >(_SC("CmdUserMode"), &Session::CmdUserMode)
        .Overload< Int32 (Session::*)(CSStr) >(_SC("CmdUserMode"), &Session::CmdUserMode)
        .Overload< Int32 (Session::*)(CSStr, CSStr) >(_SC("CmdKick"), &Session::CmdKick)
        .Overload< Int32 (Session::*)(CSStr, CSStr, CSStr) >(_SC("CmdKick"), &Session::CmdKick)
        .Overload< Int32 (Session::*)(void) >(_SC("CmdQuit"), &Session::CmdQuit)
        .Overload< Int32 (Session::*)(CSStr) >(_SC("CmdQuit"), &Session::CmdQuit)
        // Squirrel Methods
        .SquirrelFunc(_SC("CmdMsgF"), &Session::CmdMsgF)
        .SquirrelFunc(_SC("CmdMeF"), &Session::CmdMeF)
        .SquirrelFunc(_SC("CmdNoticeF"), &Session::CmdNoticeF)
    );

    ircns.Func(_SC("GetErrStr"), &irc_strerror);
    ircns.SquirrelFunc(_SC("GetNick"), &GetNick);
    ircns.SquirrelFunc(_SC("GetHost"), &GetHost);
    ircns.SquirrelFunc(_SC("StripColorFromMIRC"), &StripColorFromMIRC);
    ircns.SquirrelFunc(_SC("ConvertColorFromMIRC"), &ConvertColorFromMIRC);
    ircns.SquirrelFunc(_SC("ConvertColorToMIRC"), &ConvertColorToMIRC);

    RootTable(vm).Bind(_SC("SqIRC"), ircns);

    Sqrat::ConstTable(vm).Enum(_SC("SqIrcEvent"), Sqrat::Enumeration(vm)
        .Const(_SC("Connect"),                  SET_CONNECT)
        .Const(_SC("Nick"),                     SET_NICK)
        .Const(_SC("Quit"),                     SET_QUIT)
        .Const(_SC("Join"),                     SET_JOIN)
        .Const(_SC("Part"),                     SET_PART)
        .Const(_SC("Mode"),                     SET_MODE)
        .Const(_SC("Umode"),                    SET_UMODE)
        .Const(_SC("Topic"),                    SET_TOPIC)
        .Const(_SC("Kick"),                     SET_KICK)
        .Const(_SC("Channel"),                  SET_CHANNEL)
        .Const(_SC("PrivMsg"),                  SET_PRIVMSG)
        .Const(_SC("Notice"),                   SET_NOTICE)
        .Const(_SC("ChannelNotice"),            SET_CHANNELNOTICE)
        .Const(_SC("Invite"),                   SET_INVITE)
        .Const(_SC("CtcpReq"),                  SET_CTCPREQ)
        .Const(_SC("CtcpRep"),                  SET_CTCPREP)
        .Const(_SC("CtcpAction"),               SET_CTCPACTION)
        .Const(_SC("Unknown"),                  SET_UNKNOWN)
        .Const(_SC("Numeric"),                  SET_NUMERIC)
        .Const(_SC("DccChatReq"),               SET_DCCCHATREQ)
        .Const(_SC("DccSendReq"),               SET_DCCSENDREQ)
    );

    Sqrat::ConstTable(vm).Enum(_SC("SqIrcErr"), Sqrat::Enumeration(vm)
        .Const(_SC("Ok"),                       LIBIRC_ERR_OK)
        .Const(_SC("InVal"),                    LIBIRC_ERR_INVAL)
        .Const(_SC("Resolv"),                   LIBIRC_ERR_RESOLV)
        .Const(_SC("Socket"),                   LIBIRC_ERR_SOCKET)
        .Const(_SC("Connect"),                  LIBIRC_ERR_CONNECT)
        .Const(_SC("Closed"),                   LIBIRC_ERR_CLOSED)
        .Const(_SC("NoMem"),                    LIBIRC_ERR_NOMEM)
        .Const(_SC("Accept"),                   LIBIRC_ERR_ACCEPT)
        .Const(_SC("NoDccSend"),                LIBIRC_ERR_NODCCSEND)
        .Const(_SC("Read"),                     LIBIRC_ERR_READ)
        .Const(_SC("Write"),                    LIBIRC_ERR_WRITE)
        .Const(_SC("State"),                    LIBIRC_ERR_STATE)
        .Const(_SC("Timeout"),                  LIBIRC_ERR_TIMEOUT)
        .Const(_SC("OpenFile"),                 LIBIRC_ERR_OPENFILE)
        .Const(_SC("Terminated"),               LIBIRC_ERR_TERMINATED)
        .Const(_SC("NoIPv6"),                   LIBIRC_ERR_NOIPV6)
        .Const(_SC("SSLNotSupported"),          LIBIRC_ERR_SSL_NOT_SUPPORTED)
        .Const(_SC("SSLInitFailed"),            LIBIRC_ERR_SSL_INIT_FAILED)
        .Const(_SC("ConnectSSLFailed"),         LIBIRC_ERR_CONNECT_SSL_FAILED)
        .Const(_SC("SSLCertVerifyFailed"),      LIBIRC_ERR_SSL_CERT_VERIFY_FAILED)
        .Const(_SC("Max"),                      LIBIRC_ERR_MAX)
    );

    Sqrat::ConstTable(vm).Enum(_SC("SqIrcOpt"), Sqrat::Enumeration(vm)
        .Const(_SC("Debug"),                    LIBIRC_OPTION_DEBUG)
        .Const(_SC("StripNicks"),               LIBIRC_OPTION_STRIPNICKS)
        .Const(_SC("SSLNoVerify"),              LIBIRC_OPTION_SSL_NO_VERIFY)
    );

    Sqrat::ConstTable(vm).Enum(_SC("SqIrcRFC"), Sqrat::Enumeration(vm)
        .Const(_SC("RPL_WELCOME"),              LIBIRC_RFC_RPL_WELCOME)
        .Const(_SC("RPL_YOURHOST"),             LIBIRC_RFC_RPL_YOURHOST)
        .Const(_SC("RPL_CREATED"),              LIBIRC_RFC_RPL_CREATED)
        .Const(_SC("RPL_MYINFO"),               LIBIRC_RFC_RPL_MYINFO)
        .Const(_SC("RPL_BOUNCE"),               LIBIRC_RFC_RPL_BOUNCE)
        .Const(_SC("RPL_NONE"),                 LIBIRC_RFC_RPL_NONE)
        .Const(_SC("RPL_USERHOST"),             LIBIRC_RFC_RPL_USERHOST)
        .Const(_SC("RPL_ISON"),                 LIBIRC_RFC_RPL_ISON)
        .Const(_SC("RPL_AWAY"),                 LIBIRC_RFC_RPL_AWAY)
        .Const(_SC("RPL_UNAWAY"),               LIBIRC_RFC_RPL_UNAWAY)
        .Const(_SC("RPL_NOWAWAY"),              LIBIRC_RFC_RPL_NOWAWAY)
        .Const(_SC("RPL_WHOISUSER"),            LIBIRC_RFC_RPL_WHOISUSER)
        .Const(_SC("RPL_WHOISSERVER"),          LIBIRC_RFC_RPL_WHOISSERVER)
        .Const(_SC("RPL_WHOISOPERATOR"),        LIBIRC_RFC_RPL_WHOISOPERATOR)
        .Const(_SC("RPL_WHOISIDLE"),            LIBIRC_RFC_RPL_WHOISIDLE)
        .Const(_SC("RPL_ENDOFWHOIS"),           LIBIRC_RFC_RPL_ENDOFWHOIS)
        .Const(_SC("RPL_WHOISCHANNELS"),        LIBIRC_RFC_RPL_WHOISCHANNELS)
        .Const(_SC("RPL_WHOWASUSER"),           LIBIRC_RFC_RPL_WHOWASUSER)
        .Const(_SC("RPL_ENDOFWHOWAS"),          LIBIRC_RFC_RPL_ENDOFWHOWAS)
        .Const(_SC("RPL_LISTSTART"),            LIBIRC_RFC_RPL_LISTSTART)
        .Const(_SC("RPL_LIST"),                 LIBIRC_RFC_RPL_LIST)
        .Const(_SC("RPL_LISTEND"),              LIBIRC_RFC_RPL_LISTEND)
        .Const(_SC("RPL_UNIQOPIS"),             LIBIRC_RFC_RPL_UNIQOPIS)
        .Const(_SC("RPL_CHANNELMODEIS"),        LIBIRC_RFC_RPL_CHANNELMODEIS)
        .Const(_SC("RPL_NOTOPIC"),              LIBIRC_RFC_RPL_NOTOPIC)
        .Const(_SC("RPL_TOPIC"),                LIBIRC_RFC_RPL_TOPIC)
        .Const(_SC("RPL_INVITING"),             LIBIRC_RFC_RPL_INVITING)
        .Const(_SC("RPL_SUMMONING"),            LIBIRC_RFC_RPL_SUMMONING)
        .Const(_SC("RPL_INVITELIST"),           LIBIRC_RFC_RPL_INVITELIST)
        .Const(_SC("RPL_ENDOFINVITELIST"),      LIBIRC_RFC_RPL_ENDOFINVITELIST)
        .Const(_SC("RPL_EXCEPTLIST"),           LIBIRC_RFC_RPL_EXCEPTLIST)
        .Const(_SC("RPL_ENDOFEXCEPTLIST"),      LIBIRC_RFC_RPL_ENDOFEXCEPTLIST)
        .Const(_SC("RPL_VERSION"),              LIBIRC_RFC_RPL_VERSION)
        .Const(_SC("RPL_WHOREPLY"),             LIBIRC_RFC_RPL_WHOREPLY)
        .Const(_SC("RPL_ENDOFWHO"),             LIBIRC_RFC_RPL_ENDOFWHO)
        .Const(_SC("RPL_NAMREPLY"),             LIBIRC_RFC_RPL_NAMREPLY)
        .Const(_SC("RPL_ENDOFNAMES"),           LIBIRC_RFC_RPL_ENDOFNAMES)
        .Const(_SC("RPL_LINKS"),                LIBIRC_RFC_RPL_LINKS)
        .Const(_SC("RPL_ENDOFLINKS"),           LIBIRC_RFC_RPL_ENDOFLINKS)
        .Const(_SC("RPL_BANLIST"),              LIBIRC_RFC_RPL_BANLIST)
        .Const(_SC("RPL_ENDOFBANLIST"),         LIBIRC_RFC_RPL_ENDOFBANLIST)
        .Const(_SC("RPL_INFO"),                 LIBIRC_RFC_RPL_INFO)
        .Const(_SC("RPL_ENDOFINFO"),            LIBIRC_RFC_RPL_ENDOFINFO)
        .Const(_SC("RPL_MOTDSTART"),            LIBIRC_RFC_RPL_MOTDSTART)
        .Const(_SC("RPL_MOTD"),                 LIBIRC_RFC_RPL_MOTD)
        .Const(_SC("RPL_ENDOFMOTD"),            LIBIRC_RFC_RPL_ENDOFMOTD)
        .Const(_SC("RPL_YOUREOPER"),            LIBIRC_RFC_RPL_YOUREOPER)
        .Const(_SC("RPL_REHASHING"),            LIBIRC_RFC_RPL_REHASHING)
        .Const(_SC("RPL_YOURESERVICE"),         LIBIRC_RFC_RPL_YOURESERVICE)
        .Const(_SC("RPL_TIME"),                 LIBIRC_RFC_RPL_TIME)
        .Const(_SC("RPL_USERSSTART"),           LIBIRC_RFC_RPL_USERSSTART)
        .Const(_SC("RPL_USERS"),                LIBIRC_RFC_RPL_USERS)
        .Const(_SC("RPL_ENDOFUSERS"),           LIBIRC_RFC_RPL_ENDOFUSERS)
        .Const(_SC("RPL_NOUSERS"),              LIBIRC_RFC_RPL_NOUSERS)
        .Const(_SC("RPL_TRACELINK"),            LIBIRC_RFC_RPL_TRACELINK)
        .Const(_SC("RPL_TRACECONNECTING"),      LIBIRC_RFC_RPL_TRACECONNECTING)
        .Const(_SC("RPL_TRACEHANDSHAKE"),       LIBIRC_RFC_RPL_TRACEHANDSHAKE)
        .Const(_SC("RPL_TRACEUNKNOWN"),         LIBIRC_RFC_RPL_TRACEUNKNOWN)
        .Const(_SC("RPL_TRACEOPERATOR"),        LIBIRC_RFC_RPL_TRACEOPERATOR)
        .Const(_SC("RPL_TRACEUSER"),            LIBIRC_RFC_RPL_TRACEUSER)
        .Const(_SC("RPL_TRACESERVER"),          LIBIRC_RFC_RPL_TRACESERVER)
        .Const(_SC("RPL_TRACESERVICE"),         LIBIRC_RFC_RPL_TRACESERVICE)
        .Const(_SC("RPL_TRACENEWTYPE"),         LIBIRC_RFC_RPL_TRACENEWTYPE)
        .Const(_SC("RPL_TRACECLASS"),           LIBIRC_RFC_RPL_TRACECLASS)
        .Const(_SC("RPL_TRACELOG"),             LIBIRC_RFC_RPL_TRACELOG)
        .Const(_SC("RPL_TRACEEND"),             LIBIRC_RFC_RPL_TRACEEND)
        .Const(_SC("RPL_STATSLINKINFO"),        LIBIRC_RFC_RPL_STATSLINKINFO)
        .Const(_SC("RPL_STATSCOMMANDS"),        LIBIRC_RFC_RPL_STATSCOMMANDS)
        .Const(_SC("RPL_ENDOFSTATS"),           LIBIRC_RFC_RPL_ENDOFSTATS)
        .Const(_SC("RPL_STATSUPTIME"),          LIBIRC_RFC_RPL_STATSUPTIME)
        .Const(_SC("RPL_STATSOLINE"),           LIBIRC_RFC_RPL_STATSOLINE)
        .Const(_SC("RPL_UMODEIS"),              LIBIRC_RFC_RPL_UMODEIS)
        .Const(_SC("RPL_SERVLIST"),             LIBIRC_RFC_RPL_SERVLIST)
        .Const(_SC("RPL_SERVLISTEND"),          LIBIRC_RFC_RPL_SERVLISTEND)
        .Const(_SC("RPL_LUSERCLIENT"),          LIBIRC_RFC_RPL_LUSERCLIENT)
        .Const(_SC("RPL_LUSEROP"),              LIBIRC_RFC_RPL_LUSEROP)
        .Const(_SC("RPL_LUSERUNKNOWN"),         LIBIRC_RFC_RPL_LUSERUNKNOWN)
        .Const(_SC("RPL_LUSERCHANNELS"),        LIBIRC_RFC_RPL_LUSERCHANNELS)
        .Const(_SC("RPL_LUSERME"),              LIBIRC_RFC_RPL_LUSERME)
        .Const(_SC("RPL_ADMINME"),              LIBIRC_RFC_RPL_ADMINME)
        .Const(_SC("RPL_ADMINLOC1"),            LIBIRC_RFC_RPL_ADMINLOC1)
        .Const(_SC("RPL_ADMINLOC2"),            LIBIRC_RFC_RPL_ADMINLOC2)
        .Const(_SC("RPL_ADMINEMAIL"),           LIBIRC_RFC_RPL_ADMINEMAIL)
        .Const(_SC("RPL_TRYAGAIN"),             LIBIRC_RFC_RPL_TRYAGAIN)
        .Const(_SC("ERR_NOSUCHNICK"),           LIBIRC_RFC_ERR_NOSUCHNICK)
        .Const(_SC("ERR_NOSUCHSERVER"),         LIBIRC_RFC_ERR_NOSUCHSERVER)
        .Const(_SC("ERR_NOSUCHCHANNEL"),        LIBIRC_RFC_ERR_NOSUCHCHANNEL)
        .Const(_SC("ERR_CANNOTSENDTOCHAN"),     LIBIRC_RFC_ERR_CANNOTSENDTOCHAN)
        .Const(_SC("ERR_TOOMANYCHANNELS"),      LIBIRC_RFC_ERR_TOOMANYCHANNELS)
        .Const(_SC("ERR_WASNOSUCHNICK"),        LIBIRC_RFC_ERR_WASNOSUCHNICK)
        .Const(_SC("ERR_TOOMANYTARGETS"),       LIBIRC_RFC_ERR_TOOMANYTARGETS)
        .Const(_SC("ERR_NOSUCHSERVICE"),        LIBIRC_RFC_ERR_NOSUCHSERVICE)
        .Const(_SC("ERR_NOORIGIN"),             LIBIRC_RFC_ERR_NOORIGIN)
        .Const(_SC("ERR_NORECIPIENT"),          LIBIRC_RFC_ERR_NORECIPIENT)
        .Const(_SC("ERR_NOTEXTTOSEND"),         LIBIRC_RFC_ERR_NOTEXTTOSEND)
        .Const(_SC("ERR_NOTOPLEVEL"),           LIBIRC_RFC_ERR_NOTOPLEVEL)
        .Const(_SC("ERR_WILDTOPLEVEL"),         LIBIRC_RFC_ERR_WILDTOPLEVEL)
        .Const(_SC("ERR_BADMASK"),              LIBIRC_RFC_ERR_BADMASK)
        .Const(_SC("ERR_UNKNOWNCOMMAND"),       LIBIRC_RFC_ERR_UNKNOWNCOMMAND)
        .Const(_SC("ERR_NOMOTD"),               LIBIRC_RFC_ERR_NOMOTD)
        .Const(_SC("ERR_NOADMININFO"),          LIBIRC_RFC_ERR_NOADMININFO)
        .Const(_SC("ERR_FILEERROR"),            LIBIRC_RFC_ERR_FILEERROR)
        .Const(_SC("ERR_NONICKNAMEGIVEN"),      LIBIRC_RFC_ERR_NONICKNAMEGIVEN)
        .Const(_SC("ERR_ERRONEUSNICKNAME"),     LIBIRC_RFC_ERR_ERRONEUSNICKNAME)
        .Const(_SC("ERR_NICKNAMEINUSE"),        LIBIRC_RFC_ERR_NICKNAMEINUSE)
        .Const(_SC("ERR_NICKCOLLISION"),        LIBIRC_RFC_ERR_NICKCOLLISION)
        .Const(_SC("ERR_UNAVAILRESOURCE"),      LIBIRC_RFC_ERR_UNAVAILRESOURCE)
        .Const(_SC("ERR_USERNOTINCHANNEL"),     LIBIRC_RFC_ERR_USERNOTINCHANNEL)
        .Const(_SC("ERR_NOTONCHANNEL"),         LIBIRC_RFC_ERR_NOTONCHANNEL)
        .Const(_SC("ERR_USERONCHANNEL"),        LIBIRC_RFC_ERR_USERONCHANNEL)
        .Const(_SC("ERR_NOLOGIN"),              LIBIRC_RFC_ERR_NOLOGIN)
        .Const(_SC("ERR_SUMMONDISABLED"),       LIBIRC_RFC_ERR_SUMMONDISABLED)
        .Const(_SC("ERR_USERSDISABLED"),        LIBIRC_RFC_ERR_USERSDISABLED)
        .Const(_SC("ERR_NOTREGISTERED"),        LIBIRC_RFC_ERR_NOTREGISTERED)
        .Const(_SC("ERR_NEEDMOREPARAMS"),       LIBIRC_RFC_ERR_NEEDMOREPARAMS)
        .Const(_SC("ERR_ALREADYREGISTRED"),     LIBIRC_RFC_ERR_ALREADYREGISTRED)
        .Const(_SC("ERR_NOPERMFORHOST"),        LIBIRC_RFC_ERR_NOPERMFORHOST)
        .Const(_SC("ERR_PASSWDMISMATCH"),       LIBIRC_RFC_ERR_PASSWDMISMATCH)
        .Const(_SC("ERR_YOUREBANNEDCREEP"),     LIBIRC_RFC_ERR_YOUREBANNEDCREEP)
        .Const(_SC("ERR_YOUWILLBEBANNED"),      LIBIRC_RFC_ERR_YOUWILLBEBANNED)
        .Const(_SC("ERR_KEYSET"),               LIBIRC_RFC_ERR_KEYSET)
        .Const(_SC("ERR_CHANNELISFULL"),        LIBIRC_RFC_ERR_CHANNELISFULL)
        .Const(_SC("ERR_UNKNOWNMODE"),          LIBIRC_RFC_ERR_UNKNOWNMODE)
        .Const(_SC("ERR_INVITEONLYCHAN"),       LIBIRC_RFC_ERR_INVITEONLYCHAN)
        .Const(_SC("ERR_BANNEDFROMCHAN"),       LIBIRC_RFC_ERR_BANNEDFROMCHAN)
        .Const(_SC("ERR_BADCHANNELKEY"),        LIBIRC_RFC_ERR_BADCHANNELKEY)
        .Const(_SC("ERR_BADCHANMASK"),          LIBIRC_RFC_ERR_BADCHANMASK)
        .Const(_SC("ERR_NOCHANMODES"),          LIBIRC_RFC_ERR_NOCHANMODES)
        .Const(_SC("ERR_BANLISTFULL"),          LIBIRC_RFC_ERR_BANLISTFULL)
        .Const(_SC("ERR_NOPRIVILEGES"),         LIBIRC_RFC_ERR_NOPRIVILEGES)
        .Const(_SC("ERR_CHANOPRIVSNEEDED"),     LIBIRC_RFC_ERR_CHANOPRIVSNEEDED)
        .Const(_SC("ERR_CANTKILLSERVER"),       LIBIRC_RFC_ERR_CANTKILLSERVER)
        .Const(_SC("ERR_RESTRICTED"),           LIBIRC_RFC_ERR_RESTRICTED)
        .Const(_SC("ERR_UNIQOPPRIVSNEEDED"),    LIBIRC_RFC_ERR_UNIQOPPRIVSNEEDED)
        .Const(_SC("ERR_NOOPERHOST"),           LIBIRC_RFC_ERR_NOOPERHOST)
        .Const(_SC("ERR_UMODEUNKNOWNFLAG"),     LIBIRC_RFC_ERR_UMODEUNKNOWNFLAG)
        .Const(_SC("ERR_USERSDONTMATCH"),       LIBIRC_RFC_ERR_USERSDONTMATCH)
    );
}

} // Namespace:: SqMod

// ------------------------------------------------------------------------------------------------
SQMOD_API_EXPORT unsigned int VcmpPluginInit(PluginFuncs * functions, PluginCallbacks * callbacks, PluginInfo * info)
{
    using namespace SqMod;
    // Output plug-in header
    puts("");
    OutputMessage("--------------------------------------------------------------------");
    OutputMessage("Plug-in: %s", SQIRC_NAME);
    OutputMessage("Author: %s", SQIRC_AUTHOR);
    OutputMessage("Legal: %s", SQIRC_COPYRIGHT);
    OutputMessage("--------------------------------------------------------------------");
    puts("");
    // Attempt to find the host plug-in ID
    const int host_plugin_id = functions->FindPlugin(SQMOD_HOST_NAME);
    // See if our plug-in was loaded after the host plug-in
    if (host_plugin_id < 0)
    {
        OutputError("%s could find the host plug-in", SQIRC_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
    // Should never reach this point but just in case
    else if (static_cast< Uint32 >(host_plugin_id) > info->pluginId)
    {
        OutputError("%s loaded after the host plug-in", SQIRC_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
#ifdef SQMOD_OS_WINDOWS
    WSADATA wsa_data;
    // Initialize the sockets on windows
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        OutputError("Unable to initialize the windows sockets");
        // Don't load!
        return SQMOD_FAILURE;
    }
#endif // SQMOD_OS_WINDOWS
    // Store server proxies
    _Func = functions;
    _Clbk = callbacks;
    _Info = info;
    // Assign plug-in version
    _Info->pluginVersion = SQIRC_VERSION;
    _Info->apiMajorVersion = PLUGIN_API_MAJOR;
    _Info->apiMinorVersion = PLUGIN_API_MINOR;
    // Assign the plug-in name
    std::snprintf(_Info->name, sizeof(_Info->name), "%s", SQIRC_HOST_NAME);
    // Bind callbacks
    BindCallbacks();
    // Notify that the plug-in was successfully loaded
    OutputMessage("Successfully loaded %s", SQIRC_NAME);
    // Dummy spacing
    puts("");
    // Done!
    return SQMOD_SUCCESS;
}
