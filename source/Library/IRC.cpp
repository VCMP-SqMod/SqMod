#include "Library/IRC.hpp"
#include "Core.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace IRC {

// ------------------------------------------------------------------------------------------------
static SQChar   g_Buffer[512]{ /* ... */ };

// ------------------------------------------------------------------------------------------------
irc_callbacks_t Session::s_Callbacks;

// ------------------------------------------------------------------------------------------------
bool            Session::s_Initialized = false;

// ------------------------------------------------------------------------------------------------
Session::Session()
    : m_Session(irc_create_session(GetCallbacks()))
    , m_Server(_SC(""))
    , m_Passwd(_SC(""))
    , m_Nick(_SC(""))
    , m_User(_SC(""))
    , m_Name(_SC(""))
    , m_Port(6667)
    , m_Tries(3)
    , m_Wait(5.0f)
    , m_LeftTries(0)
    , m_NextTry(0.0f)
    , m_SessionTime(0.0f)
    , m_Reconnect(false)
    , m_IPv6(false)
{
    if (!m_Session)
    {
        DbgErr("IRC.Session", "constructor", "Unable to create an IRC session");
        // Explicitly make sure no further calls can be made to this session
        m_Session = nullptr;
    }
    else
    {
        // Associate this wrapper instance with the session structure
        irc_set_ctx(m_Session, this);
        // Connect to the on frame event so we can process callbacks
        _Core->ServerFrame.Connect< Session, &Session::Process >(this);
    }
    // Receive notification when the VM is about to be closed to release object references
    _Core->VMClose.Connect< Session, &Session::VMClose >(this);
}

// ------------------------------------------------------------------------------------------------
Session::~Session()
{
    // Make sure there's even a session to release
    if (m_Session != nullptr)
    {
        irc_set_ctx(m_Session, NULL);
        irc_destroy_session(m_Session);
        m_Session = nullptr;
    }
    // Disconnect from the on frame event
    _Core->ServerFrame.Disconnect< Session, &Session::Process >(this);
    // Stop receiving notification when the VM is about to be closed
    _Core->VMClose.Disconnect< Session, &Session::VMClose >(this);
}

// ------------------------------------------------------------------------------------------------
void Session::Process(SQFloat delta)
{
    m_SessionTime += delta;
    // Make sure that the IRC session is connected
    if (!irc_is_connected(m_Session))
    {
        // Do we meet the condition to attempt to reconnect?
        if (m_Reconnect && (m_LeftTries != 0) && (m_NextTry <= m_SessionTime))
        {
            // Take out one try
            --m_LeftTries;
            // Update the timepoint for the next try 
            m_NextTry = (m_SessionTime + m_Wait);
            // Attempt to reconnect
            if (m_IPv6)
            {
                irc_connect6(m_Session, m_Server.c_str(), m_Port,
                            m_Passwd.empty() ? NULL : m_Passwd.c_str(),
                            m_Nick.c_str(),
                            m_User.empty() ? NULL : m_User.c_str(),
                            m_Name.empty() ? NULL : m_Name.c_str()
                );
            }
            else
            {
                irc_connect(m_Session, m_Server.c_str(), m_Port,
                            m_Passwd.empty() ? NULL : m_Passwd.c_str(),
                            m_Nick.c_str(),
                            m_User.empty() ? NULL : m_User.c_str(),
                            m_Name.empty() ? NULL : m_Name.c_str()
                );
            }
        }
        // We're done for now
        return;
    }
    // Create the structures for select()
    struct timeval tv;
    fd_set in_set, out_set;
    int maxfd = 0;
    // Wait 4 millisecond for events. Meaning each session instance adds 4ms to the frame delta
    tv.tv_usec = 4000;
    tv.tv_sec = 0;
    // Initialize the sets
    memset(&in_set, 0, sizeof(fd_set));
    memset(&out_set, 0, sizeof(fd_set));
    // Add the IRC session descriptors
    irc_add_select_descriptors(m_Session, &in_set, &out_set, &maxfd);
    // Call select()
    if (select(maxfd + 1, &in_set, &out_set, 0, &tv) < 0)
    {
        // @TODO: Error
        LogWrn("Unable to select() on session");
    }
    // Call irc_process_select_descriptors() for the session
    if (irc_process_select_descriptors (m_Session, &in_set, &out_set))
    {
        // @TODO: The connection failed, or the server disconnected. Handle it
        LogWrn("The connection failed, or the server disconnected.");
    }
}

// ------------------------------------------------------------------------------------------------
void Session::VMClose()
{
    // Release the reference to the specified callback
    m_OnConnect.Release2();
    m_OnNick.Release2();
    m_OnQuit.Release2();
    m_OnJoin.Release2();
    m_OnPart.Release2();
    m_OnMode.Release2();
    m_OnUmode.Release2();
    m_OnTopic.Release2();
    m_OnKick.Release2();
    m_OnChannel.Release2();
    m_OnPrivMSG.Release2();
    m_OnNotice.Release2();
    m_OnChannel_Notice.Release2();
    m_OnInvite.Release2();
    m_OnCTCP_Req.Release2();
    m_OnCTCP_Rep.Release2();
    m_OnCTCP_Action.Release2();
    m_OnUnknown.Release2();
    m_OnNumeric.Release2();
    m_OnDcc_Chat_Req.Release2();
    m_OnDcc_Send_Req.Release2();
    // Release the reference to the specified user data
    m_Data.Release();
}

// ------------------------------------------------------------------------------------------------
irc_callbacks_t * Session::GetCallbacks()
{
    // See if the callbacks structure was initialized before
    if (!s_Initialized)
    {
        // Initialize the callbacks
        s_Callbacks.event_connect           = &OnConnect;
        s_Callbacks.event_nick              = &OnNick;
        s_Callbacks.event_quit              = &OnQuit;
        s_Callbacks.event_join              = &OnJoin;
        s_Callbacks.event_part              = &OnPart;
        s_Callbacks.event_mode              = &OnMode;
        s_Callbacks.event_umode             = &OnUmode;
        s_Callbacks.event_topic             = &OnTopic;
        s_Callbacks.event_kick              = &OnKick;
        s_Callbacks.event_channel           = &OnChannel;
        s_Callbacks.event_privmsg           = &OnPrivMSG;
        s_Callbacks.event_notice            = &OnNotice;
        s_Callbacks.event_channel_notice    = &OnChannel_Notice;
        s_Callbacks.event_invite            = &OnInvite;
        s_Callbacks.event_ctcp_req          = &OnCTCP_Req;
        s_Callbacks.event_ctcp_rep          = &OnCTCP_Rep;
        s_Callbacks.event_ctcp_action       = &OnCTCP_Action;
        s_Callbacks.event_unknown           = &OnUnknown;
        s_Callbacks.event_numeric           = &OnNumeric;
        s_Callbacks.event_dcc_chat_req      = &OnDcc_Chat_Req;
        s_Callbacks.event_dcc_send_req      = &OnDcc_Send_Req;
        // Specify that the callbacks were initialized
        s_Initialized = true;
    }
    // Return the callbacks structure
    return &s_Callbacks;
}

// ------------------------------------------------------------------------------------------------
void Session::ForwardEvent(Session * session, Function & listener, const char * event,
                            const char * origin, const char ** params, unsigned int count)
{
    // Is there anyone even listening to this event?
    if (listener.IsNull())
    {
        return;
    }
    // Each event must have an array of parameters (empty or not)
    Array parameters(DefaultVM::Get(), count);
    // Are the any parameters?
    if (params && count > 0)
    {
    // Transofrm the parameters into a squirrel array
        for (unsigned int i = 0; i < count; ++i)
        {
            parameters.SetValue(i, params[i]);
        }
    }
    // Obtain an object to this session instance without creating a new reference counter!
    ClassType< Session >::PushInstance(DefaultVM::Get(), session);
    // Obtain the pushed object from the stack
    Var< Object > var(DefaultVM::Get(), -1);
    // Call the event with the obtaine values
    listener.Execute< Object &, const SQChar *, const SQChar *, Array >(var.value, event,
        origin ? origin : _SC(""), parameters);
}

// ------------------------------------------------------------------------------------------------
void Session::ForwardEvent(Session * session, Function & listener, unsigned int event,
                            const char * origin, const char ** params, unsigned int count)
{
    // Is there anyone even listening to this event?
    if (listener.IsNull())
    {
        return;
    }
    // Each event must have an array of parameters (empty or not)
    Array parameters(DefaultVM::Get(), count);
    // Are the any parameters?
    if (params && count > 0)
    {
        // Transofrm the parameters into a squirrel array
        for (unsigned int i = 0; i < count; ++i)
        {
            parameters.SetValue(i, params[i]);
        }
    }

    // Obtain an object to this session instance without creating a new reference counter!
    ClassType< Session >::PushInstance(DefaultVM::Get(), session);
    // Obtain the pushed object from the stack
    Var< Object > var(DefaultVM::Get(), -1);
    // Call the event with the obtaine values
    listener.Execute< Object &, unsigned int, const SQChar *, Array >(var.value, event,
        origin ? origin : _SC(""), parameters);
}

// ------------------------------------------------------------------------------------------------
void Session::ForwardEvent(Session * session, Function & listener, const char * nick,
                            const char * addr, irc_dcc_t dccid)
{
    SQMOD_UNUSED_VAR(session);
    SQMOD_UNUSED_VAR(listener);
    SQMOD_UNUSED_VAR(nick);
    SQMOD_UNUSED_VAR(addr);
    SQMOD_UNUSED_VAR(dccid);
    /* @TODO: Implement! */
}

// ------------------------------------------------------------------------------------------------
void Session::ForwardEvent(Session * session, Function & listener, const char * nick, const char * addr,
                            const char * filename, unsigned long size, irc_dcc_t dccid)
{
    SQMOD_UNUSED_VAR(session);
    SQMOD_UNUSED_VAR(listener);
    SQMOD_UNUSED_VAR(nick);
    SQMOD_UNUSED_VAR(addr);
    SQMOD_UNUSED_VAR(filename);
    SQMOD_UNUSED_VAR(size);
    SQMOD_UNUSED_VAR(dccid);
    /* @TODO: Implement! */
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Cmp(const Session & o) const
{
    if (m_Session == o.m_Session)
    {
        return 0;
    }
    else if (m_Session > o.m_Session)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * Session::ToString() const
{
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
const SQChar * Session::GetTag() const
{
    return m_Tag.c_str();
}

// ------------------------------------------------------------------------------------------------
void Session::SetTag(const SQChar * tag)
{
    m_Tag.assign(tag);
}

// ------------------------------------------------------------------------------------------------
SqObj & Session::GetData()
{
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void Session::SetData(SqObj & data)
{
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
const SQChar * Session::GetServer() const
{
    return m_Server.c_str();
}

// ------------------------------------------------------------------------------------------------
void Session::SetServer(const SQChar * server)
{
    m_Server.assign(server);
}

// ------------------------------------------------------------------------------------------------
const SQChar * Session::GetPassword() const
{
    return m_Passwd.c_str();
}

// ------------------------------------------------------------------------------------------------
void Session::SetPassword(const SQChar * passwd)
{
    m_Passwd.assign(passwd);
}

// ------------------------------------------------------------------------------------------------
const SQChar * Session::GetNick() const
{
    return m_Nick.c_str();
}

// ------------------------------------------------------------------------------------------------
void Session::SetNick(const SQChar * nick)
{
    m_Nick.assign(nick);
}

// ------------------------------------------------------------------------------------------------
const SQChar * Session::GetUser() const
{
    return m_User.c_str();
}

// ------------------------------------------------------------------------------------------------
void Session::SetUser(const SQChar * user)
{
    m_User.assign(user);
}

// ------------------------------------------------------------------------------------------------
const SQChar * Session::GetName() const
{
    return m_Name.c_str();
}

// ------------------------------------------------------------------------------------------------
void Session::SetName(const SQChar * name)
{
    m_Name.assign(name);
}

// ------------------------------------------------------------------------------------------------
SQUint32 Session::GetPort() const
{
    return m_Port;
}

// ------------------------------------------------------------------------------------------------
void Session::SetPort(SQUint32 port)
{
    m_Port = port > _NLMAX(Uint16) ? _NLMAX(Uint16) : port;
}

// ------------------------------------------------------------------------------------------------
SQUint32 Session::GetTries() const
{
    return m_Tries;
}

// ------------------------------------------------------------------------------------------------
void Session::SetTries(SQUint32 num)
{
    if (m_Tries < num)
    {
        m_LeftTries += (num - m_Tries);
    }
    else
    {
        m_LeftTries -= (m_Tries - num);
    }

    m_Tries = num;
}

// ------------------------------------------------------------------------------------------------
SQFloat Session::GetWait() const
{
    return m_Wait;
}

// ------------------------------------------------------------------------------------------------
void Session::SetWait(SQFloat time)
{
    m_Wait = time;
}

// ------------------------------------------------------------------------------------------------
SQUint32 Session::GetLeftTries() const
{
    return m_LeftTries;
}

// ------------------------------------------------------------------------------------------------
void Session::SetLeftTries(SQUint32 num)
{
    m_LeftTries = num;
}

// ------------------------------------------------------------------------------------------------
SQFloat Session::GetNextTry() const
{
    return m_NextTry;
}

// ------------------------------------------------------------------------------------------------
void Session::SetNextTry(SQFloat time)
{
    m_NextTry = time;
}

// ------------------------------------------------------------------------------------------------
SQFloat Session::GetSessionTime() const
{
    return m_SessionTime;
}

// ------------------------------------------------------------------------------------------------
bool Session::GetReconnect() const
{
    return m_Reconnect;
}

// ------------------------------------------------------------------------------------------------
bool Session::GetIPv6() const
{
    return m_IPv6;
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnConnect()
{
    return m_OnConnect;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnConnect(Function & func)
{
    m_OnConnect = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnConnect_Env(SqObj & env, Function & func)
{
    m_OnConnect = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnNick()
{
    return m_OnNick;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnNick(Function & func)
{
    m_OnNick = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnNick_Env(SqObj & env, Function & func)
{
    m_OnNick = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnQuit()
{
    return m_OnQuit;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnQuit(Function & func)
{
    m_OnQuit = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnQuit_Env(SqObj & env, Function & func)
{
    m_OnQuit = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnJoin()
{
    return m_OnJoin;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnJoin(Function & func)
{
    m_OnJoin = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnJoin_Env(SqObj & env, Function & func)
{
    m_OnJoin = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnPart()
{
    return m_OnPart;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnPart(Function & func)
{
    m_OnPart = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnPart_Env(SqObj & env, Function & func)
{
    m_OnPart = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnMode()
{
    return m_OnMode;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnMode(Function & func)
{
    m_OnMode = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnMode_Env(SqObj & env, Function & func)
{
    m_OnMode = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnUmode()
{
    return m_OnUmode;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnUmode(Function & func)
{
    m_OnUmode = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnUmode_Env(SqObj & env, Function & func)
{
    m_OnUmode = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnTopic()
{
    return m_OnTopic;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnTopic(Function & func)
{
    m_OnTopic = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnTopic_Env(SqObj & env, Function & func)
{
    m_OnTopic = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnKick()
{
    return m_OnKick;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnKick(Function & func)
{
    m_OnKick = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnKick_Env(SqObj & env, Function & func)
{
    m_OnKick = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnChannel()
{
    return m_OnChannel;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnChannel(Function & func)
{
    m_OnChannel = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnChannel_Env(SqObj & env, Function & func)
{
    m_OnChannel = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnPrivMSG()
{
    return m_OnPrivMSG;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnPrivMSG(Function & func)
{
    m_OnPrivMSG = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnPrivMSG_Env(SqObj & env, Function & func)
{
    m_OnPrivMSG = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnNotice()
{
    return m_OnNotice;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnNotice(Function & func)
{
    m_OnNotice = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnNotice_Env(SqObj & env, Function & func)
{
    m_OnNotice = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnChannel_Notice()
{
    return m_OnChannel_Notice;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnChannel_Notice(Function & func)
{
    m_OnChannel_Notice = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnChannel_Notice_Env(SqObj & env, Function & func)
{
    m_OnChannel_Notice = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnInvite()
{
    return m_OnInvite;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnInvite(Function & func)
{
    m_OnInvite = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnInvite_Env(SqObj & env, Function & func)
{
    m_OnInvite = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnCTCP_Req()
{
    return m_OnCTCP_Req;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnCTCP_Req(Function & func)
{
    m_OnCTCP_Req = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnCTCP_Req_Env(SqObj & env, Function & func)
{
    m_OnCTCP_Req = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnCTCP_Rep()
{
    return m_OnCTCP_Rep;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnCTCP_Rep(Function & func)
{
    m_OnCTCP_Rep = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnCTCP_Rep_Env(SqObj & env, Function & func)
{
    m_OnCTCP_Rep = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnCTCP_Action()
{
    return m_OnCTCP_Action;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnCTCP_Action(Function & func)
{
    m_OnCTCP_Action = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnCTCP_Action_Env(SqObj & env, Function & func)
{
    m_OnCTCP_Action = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnUnknown()
{
    return m_OnUnknown;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnUnknown(Function & func)
{
    m_OnUnknown = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnUnknown_Env(SqObj & env, Function & func)
{
    m_OnUnknown = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnNumeric()
{
    return m_OnNumeric;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnNumeric(Function & func)
{
    m_OnNumeric = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnNumeric_Env(SqObj & env, Function & func)
{
    m_OnNumeric = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnDcc_Chat_Req()
{
    return m_OnDcc_Chat_Req;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnDcc_Chat_Req(Function & func)
{
    m_OnDcc_Chat_Req = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnDcc_Chat_Req_Env(SqObj & env, Function & func)
{
    m_OnDcc_Chat_Req = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function Session::GetOnDcc_Send_Req()
{
    return m_OnDcc_Send_Req;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnDcc_Send_Req(Function & func)
{
    m_OnDcc_Send_Req = func;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnDcc_Send_Req_Env(SqObj & env, Function & func)
{
    m_OnDcc_Send_Req = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
bool Session::IsValid() const
{
    return (m_Session != nullptr);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect()
{
    if (m_Session != nullptr && !irc_is_connected(m_Session) && !m_Server.empty() && !m_Nick.empty())
    {
        // Enable the reconnection system
        m_Reconnect = true;
        m_LeftTries = m_Tries;
        m_NextTry = (m_SessionTime + m_Wait);
        // This is not an IPv6 connection
        m_IPv6 = false;
        // Attempt to connect
        return irc_connect(m_Session, m_Server.c_str(), m_Port,
                            m_Passwd.empty() ? NULL : m_Passwd.c_str(),
                            m_Nick.c_str(),
                            m_User.empty() ? NULL : m_User.c_str(),
                            m_Name.empty() ? NULL : m_Name.c_str()
                );
    }
    else if (irc_is_connected(m_Session))
    {
        Error("connect", "connect to server", "while already connected to another server");
    }
    else if (m_Server.empty())
    {
        Error("connect", "connect to server", "without specifying a server first");
    }
    else if (m_Nick.empty())
    {
        Error("connect", "connect to server", "without specifying a nickname first");
    }
    else
    {
        BadRef("connect", "connect to server");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect(const SQChar * server, SQUint32 port, const SQChar * nick)
{
    return Connect(server, port, NULL, nick, NULL, NULL);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd)
{
    return Connect(server, port, passwd, nick, NULL, NULL);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd,
                const SQChar * username)
{
    return Connect(server, port, passwd, nick, username, NULL);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd,
                const SQChar * username, const SQChar * realname)
{
    if (m_Session != nullptr && !irc_is_connected(m_Session))
    {
        // Save information
        SetServer(server == NULL ? _SC("") : server);
        SetPort(port);
        SetPassword(passwd == NULL ? _SC("") : passwd);
        SetNick(nick == NULL ? _SC("") : nick);
        SetUser(username == NULL ? _SC("") : username);
        SetName(realname == NULL ? _SC("") : realname);
        // Enable the reconnection system
        m_Reconnect = true;
        m_LeftTries = m_Tries;
        m_NextTry = (m_SessionTime + m_Wait);
        // This is not an IPv6 connection
        m_IPv6 = false;
        // Attempt to connect
        return irc_connect(m_Session, server, m_Port, passwd, nick, username, realname);
    }
    else if (irc_is_connected(m_Session))
    {
        Error("connect", "connect to server", "while already connected to another server");
    }
    else
    {
        BadRef("connect", "connect to server");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect6()
{
    if (m_Session != nullptr && !irc_is_connected(m_Session) && !m_Server.empty() && !m_Nick.empty())
    {
        // Enable the reconnection system
        m_Reconnect = true;
        m_LeftTries = m_Tries;
        m_NextTry = (m_SessionTime + m_Wait);
        // This is an IPv6 connection
        m_IPv6 = true;
        // Attempt to connect
        return irc_connect6(m_Session, m_Server.c_str(), m_Port,
                            m_Passwd.empty() ? NULL : m_Passwd.c_str(),
                            m_Nick.c_str(),
                            m_User.empty() ? NULL : m_User.c_str(),
                            m_Name.empty() ? NULL : m_Name.c_str()
                );
    }
    else if (irc_is_connected(m_Session))
    {
        Error("connect6", "connect to server", "while already connected to another server");
    }
    else if (m_Server.empty())
    {
        Error("connect6", "connect to server", "without specifying a server first");
    }
    else if (m_Nick.empty())
    {
        Error("connect6", "connect to server", "without specifying a nickname first");
    }
    else
    {
        BadRef("connect6", "connect to server");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect6(const SQChar * server, SQUint32 port, const SQChar * nick)
{
    return Connect6(server, port, NULL, nick, NULL, NULL);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect6(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd)
{
    return Connect6(server, port, passwd, nick, NULL, NULL);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect6(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd,
                const SQChar * username)
{
    return Connect6(server, port, passwd, nick, username, NULL);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect6(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd,
                const SQChar * username, const SQChar * realname)
{
    if (m_Session != nullptr && !irc_is_connected(m_Session))
    {
        // Save infomation
        SetServer(server == NULL ? _SC("") : server);
        SetPort(port);
        SetPassword(passwd == NULL ? _SC("") : passwd);
        SetNick(nick == NULL ? _SC("") : nick);
        SetUser(username == NULL ? _SC("") : username);
        SetName(realname == NULL ? _SC("") : realname);
        // Enable the reconnection system
        m_Reconnect = true;
        m_LeftTries = m_Tries;
        m_NextTry = (m_SessionTime + m_Wait);
        // This is an IPv6 connection
        m_IPv6 = true;
        // Attempt to connect
        return irc_connect6(m_Session, server, m_Port, passwd, nick, username, realname);
    }
    else if (irc_is_connected(m_Session))
    {
        Error("connect6", "connect to server", "while already connected to another server");
    }
    else
    {
        BadRef("connect6", "connect to server");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void Session::Disconnect()
{
    if (m_Session != nullptr)
    {
        // Disable the reconnection system
        m_Reconnect = false;
        // Attempt to disconnect
        irc_disconnect(m_Session);
    }
    else
    {
        BadRef("disconnect", "disconnect from server");
    }
}

// ------------------------------------------------------------------------------------------------
bool Session::IsConnected()
{
    if (m_Session != nullptr)
    {
        return (irc_is_connected(m_Session) > 0);
    }
    else
    {
        BadRef("@connected", "see whether is connected");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdJoin(const SQChar * channel)
{
    return CmdJoin(channel, NULL);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdJoin(const SQChar * channel, const SQChar * key)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_join(m_Session, channel, key);
    }
    else
    {
        BadRef("cmd_join", "join channel");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdPart(const SQChar * channel)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_part(m_Session, channel);
    }
    else
    {
        BadRef("cmd_part", "part channel");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdInvite(const SQChar * nick, const SQChar * channel)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_invite(m_Session, nick, channel);
    }
    else
    {
        BadRef("cmd_invite", "invite onto channel");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdNames(const SQChar * channel)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_names(m_Session, channel);
    }
    else
    {
        BadRef("cmd_names", "get channel names list");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdList()
{
    return CmdList(NULL);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdList(const SQChar * channel)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_list(m_Session, channel);
    }
    else
    {
        BadRef("cmd_list", "get active channel list");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdTopic(const SQChar * channel)
{
    return (channel, NULL);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdTopic(const SQChar * channel, const SQChar * topic)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_topic(m_Session, channel, topic);
    }
    else
    {
        BadRef("cmd_topic", "set channel topic");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdChannelMode(const SQChar * channel)
{
    return CmdChannelMode(channel, NULL);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdChannelMode(const SQChar * channel, const SQChar * mode)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_channel_mode(m_Session, channel, mode);
    }
    else
    {
        BadRef("cmd_channel_mode", "set channel mode");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdUserMode()
{
    return CmdUserMode(NULL);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdUserMode(const SQChar * mode)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_user_mode(m_Session, mode);
    }
    else
    {
        BadRef("cmd_user_mode", "set user mode");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdKick(const SQChar * nick, const SQChar * channel)
{
    return CmdKick(nick, channel, NULL);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdKick(const SQChar * nick, const SQChar * channel, const SQChar * reason)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_kick(m_Session, nick, channel, reason);
    }
    else
    {
        BadRef("cmd_kick", "kick from channel");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdMsg(const SQChar * nch, const SQChar * text)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_msg(m_Session, nch, text);
    }
    else
    {
        BadRef("cmd_msg", "send message");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdMe(const SQChar * nch, const SQChar * text)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_me(m_Session, nch, text);
    }
    else
    {
        BadRef("cmd_me", "send me message");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdNotice(const SQChar * nch, const SQChar * text)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_notice(m_Session, nch, text);
    }
    else
    {
        BadRef("cmd_notice", "send notice message");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdCtcpRequest(const SQChar * nick, const SQChar * request)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_notice(m_Session, nick, request);
    }
    else
    {
        BadRef("cmd_ctcp_request", "send ctcp request");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdCtcpReply(const SQChar * nick, const SQChar * reply)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_ctcp_reply(m_Session, nick, reply);
    }
    else
    {
        BadRef("cmd_ctcp_reply", "send ctcp reply");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdNick(const SQChar * nick)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_nick(m_Session, nick);
    }
    else
    {
        BadRef("cmd_nick", "change nick");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdWhois(const SQChar * nick)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_whois(m_Session, nick);
    }
    else
    {
        BadRef("cmd_whois", "request whois");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdQuit()
{

    return CmdQuit(NULL);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdQuit(const SQChar * reason)
{
    if (m_Session != nullptr)
    {
        return irc_cmd_quit(m_Session, reason);
    }
    else
    {
        BadRef("cmd_quit", "quit server");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::SendRaw(const SQChar * str)
{
    if (m_Session != nullptr)
    {
        return irc_send_raw(m_Session, str);
    }
    else
    {
        BadRef("send_raw", "send raw data");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::DestroyDcc(SQUint32 dccid)
{
    if (m_Session != nullptr)
    {
        return irc_dcc_destroy(m_Session, dccid);
    }
    else
    {
        BadRef("destroy_dcc", "destroy dcc");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void Session::SetCtcpVersion(const SQChar * version)
{
    if (m_Session != nullptr)
    {
        irc_set_ctcp_version(m_Session, version);
    }
    else
    {
        BadRef("set_ctcp_version", "set ctcp version");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::GetErrNo()
{
    if (m_Session != nullptr)
    {
        return irc_errno(m_Session);
    }
    else
    {
        BadRef("@err_no", "get error number");
    }
    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
const SQChar * Session::GetErrStr()
{
    if (m_Session != nullptr)
    {
        return irc_strerror(irc_errno(m_Session));
    }
    else
    {
        BadRef("@err_str", "get error string");
    }
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
void Session::SetOption(SQUint32 option)
{
    if (m_Session != nullptr)
    {
        return irc_option_set(m_Session, option);
    }
    else
    {
        BadRef("set_option", "set option");
    }
}

// ------------------------------------------------------------------------------------------------
void Session::ResetOption(SQUint32 option)
{
    if (m_Session != nullptr)
    {
        return irc_option_set(m_Session, option);
    }
    else
    {
        BadRef("reset_option", "reset option");
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnConnect(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        // Prevent any attempts to reconnect now
        inst->m_Reconnect = false;
        // Now forward event
        ForwardEvent(inst, inst->m_OnConnect, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnNick(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnNick, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnQuit(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnQuit, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnJoin(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnJoin, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnPart(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnPart, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnMode(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnMode, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnUmode(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnUmode, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnTopic(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnTopic, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnKick(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnKick, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnChannel(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnChannel, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnPrivMSG(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnPrivMSG, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnNotice(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnNotice, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnChannel_Notice(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnChannel_Notice, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnInvite(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnInvite, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnCTCP_Req(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnCTCP_Req, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnCTCP_Rep(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnCTCP_Rep, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnCTCP_Action(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnCTCP_Action, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnUnknown(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnUnknown, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnNumeric(irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnNumeric, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnDcc_Chat_Req(irc_session_t * session, const char * nick, const char * addr, irc_dcc_t dccid)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnDcc_Chat_Req, nick, addr, dccid);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnDcc_Send_Req(irc_session_t * session, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid)
{
    Session * inst = reinterpret_cast< Session * >(irc_get_ctx(session));

    if (!inst)
    {
        LogErr("Cannot forward IRC event without a session container");
    }
    else
    {
        ForwardEvent(inst, inst->m_OnDcc_Send_Req, nick, addr, filename, size, dccid);
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * GetNick(const SQChar * origin)
{
    // Attempt to retrieve the nickname
    irc_target_get_nick(origin, g_Buffer, sizeof(g_Buffer));
    // Return the nickname that could be retrieved
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
const SQChar * GetHost(const SQChar * target)
{
    // Attempt to retrieve the nickname
    irc_target_get_host(target, g_Buffer, sizeof(g_Buffer));
    // Return the nickname that could be retrieved
    return g_Buffer;
}

} // Namespace:: IRC

// ================================================================================================
bool Register_IRC(HSQUIRRELVM vm)
{
    using namespace IRC;

    // // Attempt to create the IRC namespace
    Sqrat::Table ircns(vm);

    // Output debugging information
    LogDbg("Beginning registration of <IRC Session> type");
    // Attempt to register the specified type
    ircns.Bind(_SC("Session"), Sqrat::Class< Session, NoCopy< Session > >(vm, _SC("Session"))
        /* Constructors */
        .Ctor()
        /* Metamethods */
        .Func(_SC("_cmp"), &Session::Cmp)
        .Func(_SC("_tostring"), &Session::ToString)
        /* Properties */
        .Prop(_SC("ltag"), &Session::GetTag, &Session::SetTag)
        .Prop(_SC("ldata"), &Session::GetData, &Session::SetData)
        .Prop(_SC("server"), &Session::GetData, &Session::SetData)
        .Prop(_SC("server"), &Session::GetServer, &Session::SetServer)
        .Prop(_SC("passwd"), &Session::GetPassword, &Session::SetPassword)
        .Prop(_SC("nick"), &Session::GetNick, &Session::SetNick)
        .Prop(_SC("user"), &Session::GetUser, &Session::SetUser)
        .Prop(_SC("name"), &Session::GetName, &Session::SetName)
        .Prop(_SC("port"), &Session::GetPort, &Session::SetPort)
        .Prop(_SC("tries"), &Session::GetTries, &Session::SetTries)
        .Prop(_SC("wait"), &Session::GetWait, &Session::SetWait)
        .Prop(_SC("left_tries"), &Session::GetLeftTries, &Session::SetLeftTries)
        .Prop(_SC("next_try"), &Session::GetNextTry, &Session::SetNextTry)
        .Prop(_SC("session_time"), &Session::GetSessionTime)
        .Prop(_SC("reconnect"), &Session::GetReconnect)
        .Prop(_SC("ipv6"), &Session::GetIPv6)
        .Prop(_SC("on_connect"), &Session::GetOnConnect, &Session::SetOnConnect)
        .Prop(_SC("on_nick"), &Session::GetOnNick, &Session::SetOnNick)
        .Prop(_SC("on_quit"), &Session::GetOnQuit, &Session::SetOnQuit)
        .Prop(_SC("on_join"), &Session::GetOnJoin, &Session::SetOnJoin)
        .Prop(_SC("on_part"), &Session::GetOnPart, &Session::SetOnPart)
        .Prop(_SC("on_mode"), &Session::GetOnMode, &Session::SetOnMode)
        .Prop(_SC("on_umode"), &Session::GetOnUmode, &Session::SetOnUmode)
        .Prop(_SC("on_topic"), &Session::GetOnTopic, &Session::SetOnTopic)
        .Prop(_SC("on_kick"), &Session::GetOnKick, &Session::SetOnKick)
        .Prop(_SC("on_channel"), &Session::GetOnChannel, &Session::SetOnChannel)
        .Prop(_SC("on_priv_msg"), &Session::GetOnPrivMSG, &Session::SetOnPrivMSG)
        .Prop(_SC("on_notice"), &Session::GetOnNotice, &Session::SetOnNotice)
        .Prop(_SC("on_channel_notice"), &Session::GetOnChannel_Notice, &Session::SetOnChannel_Notice)
        .Prop(_SC("on_invite"), &Session::GetOnInvite, &Session::SetOnInvite)
        .Prop(_SC("on_ctcp_req"), &Session::GetOnCTCP_Req, &Session::SetOnCTCP_Req)
        .Prop(_SC("on_ctcp_rep"), &Session::GetOnCTCP_Rep, &Session::SetOnCTCP_Rep)
        .Prop(_SC("on_ctcp_action"), &Session::GetOnCTCP_Action, &Session::SetOnCTCP_Action)
        .Prop(_SC("on_unknown"), &Session::GetOnUnknown, &Session::SetOnUnknown)
        .Prop(_SC("on_numeric"), &Session::GetOnNumeric, &Session::SetOnNumeric)
        .Prop(_SC("on_dcc_chat_req"), &Session::GetOnDcc_Chat_Req, &Session::SetOnDcc_Chat_Req)
        .Prop(_SC("on_dcc_send_req"), &Session::GetOnDcc_Send_Req, &Session::SetOnDcc_Send_Req)
        .Prop(_SC("connected"), &Session::IsConnected)
        .Prop(_SC("err_no"), &Session::GetErrNo)
        .Prop(_SC("err_str"), &Session::GetErrStr)
        .Prop(_SC("ctcp_version"), &Session::SetCtcpVersion)
        /* Functions */
        .Func(_SC("get_on_connect"), &Session::GetOnConnect)
        .Func(_SC("get_on_nick"), &Session::GetOnNick)
        .Func(_SC("get_on_quit"), &Session::GetOnQuit)
        .Func(_SC("get_on_join"), &Session::GetOnJoin)
        .Func(_SC("get_on_part"), &Session::GetOnPart)
        .Func(_SC("get_on_mode"), &Session::GetOnMode)
        .Func(_SC("get_on_umode"), &Session::GetOnUmode)
        .Func(_SC("get_on_topic"), &Session::GetOnTopic)
        .Func(_SC("get_on_kick"), &Session::GetOnKick)
        .Func(_SC("get_on_channel"), &Session::GetOnChannel)
        .Func(_SC("get_on_priv_msg"), &Session::GetOnPrivMSG)
        .Func(_SC("get_on_notice"), &Session::GetOnNotice)
        .Func(_SC("get_on_channel_notice"), &Session::GetOnChannel_Notice)
        .Func(_SC("get_on_invite"), &Session::GetOnInvite)
        .Func(_SC("get_on_ctcp_req"), &Session::GetOnCTCP_Req)
        .Func(_SC("get_on_ctcp_rep"), &Session::GetOnCTCP_Rep)
        .Func(_SC("get_on_ctcp_action"), &Session::GetOnCTCP_Action)
        .Func(_SC("get_on_unknown"), &Session::GetOnUnknown)
        .Func(_SC("get_on_numeric"), &Session::GetOnNumeric)
        .Func(_SC("get_on_dcc_chat_req"), &Session::GetOnDcc_Chat_Req)
        .Func(_SC("get_on_dcc_send_req"), &Session::GetOnDcc_Send_Req)
        .Func(_SC("set_on_connect"), &Session::SetOnConnect_Env)
        .Func(_SC("set_on_nick"), &Session::SetOnNick_Env)
        .Func(_SC("set_on_quit"), &Session::SetOnQuit_Env)
        .Func(_SC("set_on_join"), &Session::SetOnJoin_Env)
        .Func(_SC("set_on_part"), &Session::SetOnPart_Env)
        .Func(_SC("set_on_mode"), &Session::SetOnMode_Env)
        .Func(_SC("set_on_umode"), &Session::SetOnUmode_Env)
        .Func(_SC("set_on_topic"), &Session::SetOnTopic_Env)
        .Func(_SC("set_on_kick"), &Session::SetOnKick_Env)
        .Func(_SC("set_on_channel"), &Session::SetOnChannel_Env)
        .Func(_SC("set_on_priv_msg"), &Session::SetOnPrivMSG_Env)
        .Func(_SC("set_on_notice"), &Session::SetOnNotice_Env)
        .Func(_SC("set_on_channel_notice"), &Session::SetOnChannel_Notice_Env)
        .Func(_SC("set_on_invite"), &Session::SetOnInvite_Env)
        .Func(_SC("set_on_ctcp_req"), &Session::SetOnCTCP_Req_Env)
        .Func(_SC("set_on_ctcp_rep"), &Session::SetOnCTCP_Rep_Env)
        .Func(_SC("set_on_ctcp_action"), &Session::SetOnCTCP_Action_Env)
        .Func(_SC("set_on_unknown"), &Session::SetOnUnknown_Env)
        .Func(_SC("set_on_numeric"), &Session::SetOnNumeric_Env)
        .Func(_SC("set_on_dcc_chat_req"), &Session::SetOnDcc_Chat_Req_Env)
        .Func(_SC("set_on_dcc_send_req"), &Session::SetOnDcc_Send_Req_Env)
        .Func(_SC("disconnect"), &Session::Disconnect)
        .Func(_SC("cmd_part"), &Session::CmdPart)
        .Func(_SC("cmd_invite"), &Session::CmdInvite)
        .Func(_SC("cmd_names"), &Session::CmdNames)
        .Func(_SC("cmd_msg"), &Session::CmdMsg)
        .Func(_SC("cmd_me"), &Session::CmdMe)
        .Func(_SC("cmd_notice"), &Session::CmdNotice)
        .Func(_SC("cmd_ctcp_request"), &Session::CmdCtcpRequest)
        .Func(_SC("cmd_ctcp_reply"), &Session::CmdCtcpReply)
        .Func(_SC("cmd_nick"), &Session::CmdNick)
        .Func(_SC("cmd_whois"), &Session::CmdWhois)
        .Func(_SC("send_raw"), &Session::SendRaw)
        .Func(_SC("destroy_dcc"), &Session::DestroyDcc)
        .Func(_SC("set_ctcp_version"), &Session::SetCtcpVersion)
        .Func(_SC("set_option"), &Session::SetOption)
        .Func(_SC("reset_option"), &Session::ResetOption)
        /* Overloads */
        .Overload< SQInt32 (Session::*)(void) >
            (_SC("connect"), &Session::Connect)
        .Overload< SQInt32 (Session::*)(const SQChar *, SQUint32, const SQChar *) >
            (_SC("connect"), &Session::Connect)
        .Overload< SQInt32 (Session::*)(const SQChar *, SQUint32, const SQChar *, const SQChar *) >
            (_SC("connect"), &Session::Connect)
        .Overload< SQInt32 (Session::*)(const SQChar *, SQUint32, const SQChar *, const SQChar *, const SQChar *) >
            (_SC("connect"), &Session::Connect)
        .Overload< SQInt32 (Session::*)(const SQChar *, SQUint32, const SQChar *, const SQChar *, const SQChar *, const SQChar *) >
            (_SC("connect"), &Session::Connect)
        .Overload< SQInt32 (Session::*)(void) >
            (_SC("connect6"), &Session::Connect6)
        .Overload< SQInt32 (Session::*)(const SQChar *, SQUint32, const SQChar *) >
            (_SC("connect6"), &Session::Connect6)
        .Overload< SQInt32 (Session::*)(const SQChar *, SQUint32, const SQChar *, const SQChar *) >
            (_SC("connect6"), &Session::Connect6)
        .Overload< SQInt32 (Session::*)(const SQChar *, SQUint32, const SQChar *, const SQChar *, const SQChar *) >
            (_SC("connect6"), &Session::Connect6)
        .Overload< SQInt32 (Session::*)(const SQChar *, SQUint32, const SQChar *, const SQChar *, const SQChar *, const SQChar *) >
            (_SC("connect6"), &Session::Connect6)
        .Overload< SQInt32 (Session::*)(const SQChar *) >
            (_SC("cmd_join"), &Session::CmdJoin)
        .Overload< SQInt32 (Session::*)(const SQChar *, const SQChar *) >
            (_SC("cmd_join"), &Session::CmdJoin)
        .Overload< SQInt32 (Session::*)(void) >
            (_SC("cmd_list"), &Session::CmdList)
        .Overload< SQInt32 (Session::*)(const SQChar *) >
            (_SC("cmd_list"), &Session::CmdList)
        .Overload< SQInt32 (Session::*)(const SQChar *) >
            (_SC("cmd_topic"), &Session::CmdTopic)
        .Overload< SQInt32 (Session::*)(const SQChar *, const SQChar *) >
            (_SC("cmd_topic"), &Session::CmdTopic)
        .Overload< SQInt32 (Session::*)(const SQChar *) >
            (_SC("cmd_channel_mode"), &Session::CmdChannelMode)
        .Overload< SQInt32 (Session::*)(const SQChar *, const SQChar *) >
            (_SC("cmd_channel_mode"), &Session::CmdChannelMode)
        .Overload< SQInt32 (Session::*)(void) >
            (_SC("cmd_user_mode"), &Session::CmdUserMode)
        .Overload< SQInt32 (Session::*)(const SQChar *) >
            (_SC("cmd_user_mode"), &Session::CmdUserMode)
        .Overload< SQInt32 (Session::*)(const SQChar *, const SQChar *) >
            (_SC("cmd_kick"), &Session::CmdKick)
        .Overload< SQInt32 (Session::*)(const SQChar *, const SQChar *, const SQChar *) >
            (_SC("cmd_kick"), &Session::CmdKick)
        .Overload< SQInt32 (Session::*)(void) >
            (_SC("cmd_quit"), &Session::CmdQuit)
        .Overload< SQInt32 (Session::*)(const SQChar *) >
            (_SC("cmd_quit"), &Session::CmdQuit)
    );

    // Output debugging information
    LogDbg("Registration of <IRC Session> type was successful");

    // Output debugging information
    LogDbg("Beginning registration of <IRC> functions");
    // Attempt to register the free functions
    ircns.Func(_SC("GetNick"), &GetNick);
    ircns.Func(_SC("GetHost"), &GetHost);
    ircns.Func(_SC("GetErrStr"), &irc_strerror);
    // Output debugging information
    LogDbg("Registration of <IRC> functions was successful");

    // Attempt to bind the namespace to the root table
    Sqrat::RootTable(vm).Bind(_SC("IRC"), ircns);

    // Output debugging information
    LogDbg("Beginning registration of <IRC> constants");
    // Attempt to register the error codes enumeration
    Sqrat::ConstTable(vm).Enum(_SC("EIRCERR"), Sqrat::Enumeration(vm)
        .Const(_SC("OK"),                       LIBIRC_ERR_OK)
        .Const(_SC("INVAL"),                    LIBIRC_ERR_INVAL)
        .Const(_SC("RESOLV"),                   LIBIRC_ERR_RESOLV)
        .Const(_SC("SOCKET"),                   LIBIRC_ERR_SOCKET)
        .Const(_SC("CONNECT"),                  LIBIRC_ERR_CONNECT)
        .Const(_SC("CLOSED"),                   LIBIRC_ERR_CLOSED)
        .Const(_SC("NOMEM"),                    LIBIRC_ERR_NOMEM)
        .Const(_SC("ACCEPT"),                   LIBIRC_ERR_ACCEPT)
        .Const(_SC("NODCCSEND"),                LIBIRC_ERR_NODCCSEND)
        .Const(_SC("READ"),                     LIBIRC_ERR_READ)
        .Const(_SC("WRITE"),                    LIBIRC_ERR_WRITE)
        .Const(_SC("STATE"),                    LIBIRC_ERR_STATE)
        .Const(_SC("TIMEOUT"),                  LIBIRC_ERR_TIMEOUT)
        .Const(_SC("OPENFILE"),                 LIBIRC_ERR_OPENFILE)
        .Const(_SC("TERMINATED"),               LIBIRC_ERR_TERMINATED)
        .Const(_SC("NOIPV6"),                   LIBIRC_ERR_NOIPV6)
        .Const(_SC("SSL_NOT_SUPPORTED"),        LIBIRC_ERR_SSL_NOT_SUPPORTED)
        .Const(_SC("SSL_INIT_FAILED"),          LIBIRC_ERR_SSL_INIT_FAILED)
        .Const(_SC("CONNECT_SSL_FAILED"),       LIBIRC_ERR_CONNECT_SSL_FAILED)
        .Const(_SC("SSL_CERT_VERIFY_FAILED"),   LIBIRC_ERR_SSL_CERT_VERIFY_FAILED)
        .Const(_SC("MAX"),                      LIBIRC_ERR_MAX)
    );
    // Attempt to register the options enumeration
    Sqrat::ConstTable(vm).Enum(_SC("EIRCOPT"), Sqrat::Enumeration(vm)
        .Const(_SC("DEBUG"),                    LIBIRC_OPTION_DEBUG)
        .Const(_SC("STRIPNICKS"),               LIBIRC_OPTION_STRIPNICKS)
        .Const(_SC("SSL_NO_VERIFY"),            LIBIRC_OPTION_SSL_NO_VERIFY)
    );
    // Attempt to register the irc rfc enumeration
    Sqrat::ConstTable(vm).Enum(_SC("EIRCRFC"), Sqrat::Enumeration(vm)
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
    // Output debugging information
    LogDbg("Registration of <IRC> constants was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
