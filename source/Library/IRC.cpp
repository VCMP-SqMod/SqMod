// ------------------------------------------------------------------------------------------------
#include "Library/IRC.hpp"

// ------------------------------------------------------------------------------------------------
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace IRC {

// ------------------------------------------------------------------------------------------------
static SQChar       g_Buffer[512];

// ------------------------------------------------------------------------------------------------
irc_callbacks_t     Session::s_Callbacks;

// ------------------------------------------------------------------------------------------------
bool                Session::s_Initialized = false;

// ------------------------------------------------------------------------------------------------
Session*            Session::s_Session = NULL;

// ------------------------------------------------------------------------------------------------
Session::Sessions   Session::s_Sessions;

// ------------------------------------------------------------------------------------------------
void Session::Process()
{
    // Do we only have one IRC session?
    if (s_Session)
        s_Session->Update();
    // Do we have multiple sessions?
    else if (!s_Sessions.empty())
    {
        for (Sessions::iterator itr = s_Sessions.begin(); itr != s_Sessions.end(); ++itr)
            (*itr)->Update();
    }
}

// ------------------------------------------------------------------------------------------------
void Session::Terminate()
{
    // Do we only have one IRC session?
    if (s_Session)
        s_Session->Destroy(); /* This should do the job. */
    // Do we have multiple sessions?
    else if (!s_Sessions.empty())
    {
        for (Sessions::iterator itr = s_Sessions.begin(); itr != s_Sessions.end(); ++itr)
            (*itr)->Destroy();
    }
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
        s_Callbacks.event_privmsg           = &OnPrivMsg;
        s_Callbacks.event_notice            = &OnNotice;
        s_Callbacks.event_channel_notice    = &OnChannelNotice;
        s_Callbacks.event_invite            = &OnInvite;
        s_Callbacks.event_ctcp_req          = &OnCtcpReq;
        s_Callbacks.event_ctcp_rep          = &OnCtcpRep;
        s_Callbacks.event_ctcp_action       = &OnCtcpAction;
        s_Callbacks.event_unknown           = &OnUnknown;
        s_Callbacks.event_numeric           = &OnNumeric;
        s_Callbacks.event_dcc_chat_req      = &OnDccChatReq;
        s_Callbacks.event_dcc_send_req      = &OnDccSendReq;
        // Specify that the callbacks were initialized
        s_Initialized = true;
    }
    // Return the callbacks structure
    return &s_Callbacks;
}

// ------------------------------------------------------------------------------------------------
void Session::Update()
{
    // Make sure we even have a session
    if (!m_Session)
        return;
    // Make sure that the IRC session is connected
    else if (!irc_is_connected(m_Session))
    {
        // Do we meet the condition to attempt to reconnect?
        if (m_Reconnect && (m_LeftTries != 0) && (m_NextTry <= GetEpochTimeMicro()))
        {
            // Take out one try
            --m_LeftTries;
            // Update the time-point for the next try
            m_NextTry = (GetEpochTimeMicro() + (m_Wait * 1000LL));
            // Attempt to reconnect
            if (m_IPv6)
            {
                m_LastCode = irc_connect6(m_Session, m_Server.c_str(), m_Port,
                            m_Passwd.empty() ? NULL : m_Passwd.c_str(),
                            m_Nick.c_str(),
                            m_User.empty() ? NULL : m_User.c_str(),
                            m_Name.empty() ? NULL : m_Name.c_str()
                );
            }
            else
            {
                m_LastCode = irc_connect(m_Session, m_Server.c_str(), m_Port,
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
    // Pool for the specified amount of time
    tv.tv_usec = (m_PoolTime * 1000L);
    tv.tv_sec = 0;
    // Initialize the sets
    memset(&in_set, 0, sizeof(fd_set));
    memset(&out_set, 0, sizeof(fd_set));
    // Add the IRC session descriptors
    irc_add_select_descriptors(m_Session, &in_set, &out_set, &maxfd);
    // Call select()
    if (select(maxfd + 1, &in_set, &out_set, 0, &tv) < 0)
    {
        LogErr("Unable to select() on IRC session");
    }
    // Call irc_process_select_descriptors() for the session
    if (irc_process_select_descriptors (m_Session, &in_set, &out_set))
    {
        // @TODO: The connection failed, or the server disconnected. Handle it!
        LogWrn("The IRC connection failed, or the server disconnected.");
    }
}

// ------------------------------------------------------------------------------------------------
void Session::Release()
{
    // Release the reference to the specified callbacks
    m_OnConnect.ReleaseGently();
    m_OnNick.ReleaseGently();
    m_OnQuit.ReleaseGently();
    m_OnJoin.ReleaseGently();
    m_OnPart.ReleaseGently();
    m_OnMode.ReleaseGently();
    m_OnUmode.ReleaseGently();
    m_OnTopic.ReleaseGently();
    m_OnKick.ReleaseGently();
    m_OnChannel.ReleaseGently();
    m_OnPrivMsg.ReleaseGently();
    m_OnNotice.ReleaseGently();
    m_OnChannelNotice.ReleaseGently();
    m_OnInvite.ReleaseGently();
    m_OnCtcpReq.ReleaseGently();
    m_OnCtcpRep.ReleaseGently();
    m_OnCtcpAction.ReleaseGently();
    m_OnUnknown.ReleaseGently();
    m_OnNumeric.ReleaseGently();
    m_OnDccChatReq.ReleaseGently();
    m_OnDccSendReq.ReleaseGently();
    // Release the reference to the specified user data
    m_Data.Release();
}

// ------------------------------------------------------------------------------------------------
void Session::Destroy()
{
    // Make sure there's even a session to release
    if (!m_Session)
        return;
    // Disconnect the session
    Disconnect();
    // Break the association with this instance (paranoia)
    irc_set_ctx(m_Session, NULL);
    // Destroy the IRC session structure
    irc_destroy_session(m_Session);
    // Explicitly make sure no further calls can be made to this session (again... paranoia)
    m_Session = NULL;
    // Release resources
    Release();
}

// ------------------------------------------------------------------------------------------------
Session::Session()
    : m_Session(irc_create_session(GetCallbacks()))
    , m_Server(_SC(""))
    , m_Passwd(_SC(""))
    , m_Nick(_SC(""))
    , m_User(_SC(""))
    , m_Name(_SC(""))
    , m_Port(6667)
    , m_LastCode(0)
    , m_PoolTime(4)
    , m_Tries(3)
    , m_Wait(5000)
    , m_LeftTries(0)
    , m_NextTry(0)
    , m_SessionTime(0)
    , m_Reconnect(false)
    , m_IPv6(false)
{
    if (!m_Session)
    {
        SqThrow("Unable to create an IRC session");
        // Explicitly make sure no further calls can be made to this session
        m_Session = NULL;
    }
    else
    {
        // Associate this wrapper instance with the session structure
        irc_set_ctx(m_Session, this);
        // Is this the only session instance?
        if (!s_Session && s_Sessions.empty())
        {
            s_Session = this;
        }
        else if (s_Sessions.empty())
        {
            s_Sessions.push_back(s_Session);
            s_Session = NULL;
            s_Sessions.push_back(this);
        }
        else
        {
            s_Sessions.push_back(this);
        }
    }
}

// ------------------------------------------------------------------------------------------------
Session::~Session()
{
    Destroy();
    // Attempt to find our self in the session pool
    Sessions::iterator itr = std::find(s_Sessions.begin(), s_Sessions.end(), this);
    // Are we in the pool?
    if (itr != s_Sessions.end())
        s_Sessions.erase(itr); /* Remove our self from the pool. */
    // Is there a single session and that's us?
    if (s_Session == this)
        s_Session = NULL;
}

// ------------------------------------------------------------------------------------------------
Int32 Session::Connect()
{
    // Are we even allowed to try to connect?
    if (!Validate() || !NotConnected())
        return -1; /* No point in going forward */
    // Did we already try to connect?
    else if (m_Reconnect)
        SqThrow("Attempting to connect IRC while connection was already issued");
    // Validate the specified server
    else if (!m_Server.empty())
        SqThrow("Attempting to connect IRC without specifying a server");
    // Validate the specified nickname
    else if (!m_Nick.empty())
        SqThrow("Attempting to connect IRC without specifying a nickname");
    else
    {
        // Enable the reconnection system
        m_Reconnect = true;
        // Reset the number of tries
        m_LeftTries = m_Tries;
        // Set the time-point for the next try
        m_NextTry = (GetEpochTimeMicro() + (m_Wait * 1000LL));
        // This is not an IPv6 connection
        m_IPv6 = false;
        // Attempt to connect the session and return the result
        return irc_connect(m_Session, m_Server.c_str(), m_Port,
                            m_Passwd.empty() ? NULL : m_Passwd.c_str(),
                            m_Nick.c_str(),
                            m_User.empty() ? NULL : m_User.c_str(),
                            m_Name.empty() ? NULL : m_Name.c_str()
        );
    }
    // Connection denied before even attempted
    return -1;
}

// ------------------------------------------------------------------------------------------------
Int32 Session::Connect(CSStr server, Uint32 port, CSStr nick, CSStr passwd, CSStr user, CSStr name)
{
    // Are we even allowed to try to connect?
    if (!Validate() || !NotConnected())
        return -1; /* No point in going forward */
    // Did we already try to connect?
    else if (m_Reconnect)
        SqThrow("Attempting to connect IRC while connection was already issued");
    // Validate the specified port
    else if (port > NumLimit< Uint16 >::Max)
        SqThrow("Port number is out of range: %u > %u", port, NumLimit< Uint16 >::Max);
    // Validate the specified server
    else if (!server || strlen(server) <= 0)
        SqThrow("Attempting to connect IRC without specifying a server");
    // Validate the specified nickname
    else if (!nick || strlen(nick) <= 0)
        SqThrow("Attempting to connect IRC without specifying a nickname");
    else
    {
        // Save the specified port
        m_Port = port;
        // Save the specified server
        m_Server.assign(server);
        // Save the specified nickname
        m_Nick.assign(nick);
        // Save the specified password
        m_Passwd.assign(!passwd ? _SC("") : passwd);
        // Save the specified user
        m_User.assign(!user ? _SC("") : user);
        // Save the specified name
        m_Name.assign(!name ? _SC("") : name);
        // Enable the reconnection system
        m_Reconnect = true;
        // Reset the number of tries
        m_LeftTries = m_Tries;
        // Set the time-point for the next connection try
        m_NextTry = (GetEpochTimeMicro() + (m_Wait * 1000LL));
        // This is not an IPv6 connection
        m_IPv6 = false;
        // Attempt to connect the session and return the result
        return irc_connect(m_Session, m_Server.c_str(), m_Port,
                            m_Passwd.empty() ? NULL : m_Passwd.c_str(),
                            m_Nick.c_str(),
                            m_User.empty() ? NULL : m_User.c_str(),
                            m_Name.empty() ? NULL : m_Name.c_str()
        );
    }
    // Connection denied before even attempted
    return -1;
}

// ------------------------------------------------------------------------------------------------
Int32 Session::Connect6()
{
    // Are we even allowed to try to connect?
    if (!Validate() || !NotConnected())
        return -1; /* No point in going forward */
    // Did we already try to connect?
    else if (m_Reconnect)
        SqThrow("Attempting to connect IRC while connection was already issued");
    // Validate the specified server
    else if (!m_Server.empty())
        SqThrow("Attempting to connect IRC without specifying a server");
    // Validate the specified nickname
    else if (!m_Nick.empty())
        SqThrow("Attempting to connect IRC without specifying a nickname");
    else
    {
        // Enable the reconnection system
        m_Reconnect = true;
        // Reset the number of tries
        m_LeftTries = m_Tries;
        // Set the time-point for the next try
        m_NextTry = (GetEpochTimeMicro() + (m_Wait * 1000LL));
        // This is an IPv6 connection
        m_IPv6 = true;
        // Attempt to connect the session and return the result
        return irc_connect6(m_Session, m_Server.c_str(), m_Port,
                            m_Passwd.empty() ? NULL : m_Passwd.c_str(),
                            m_Nick.c_str(),
                            m_User.empty() ? NULL : m_User.c_str(),
                            m_Name.empty() ? NULL : m_Name.c_str()
        );
    }
    // Connection denied before even attempted
    return -1;
}

// ------------------------------------------------------------------------------------------------
Int32 Session::Connect6(CSStr server, Uint32 port, CSStr nick, CSStr passwd, CSStr user, CSStr name)
{
    // Are we even allowed to try to connect?
    if (!Validate() || !NotConnected())
        return -1; /* No point in going forward */
    // Did we already try to connect?
    else if (m_Reconnect)
        SqThrow("Attempting to connect IRC while connection was already issued");
    // Validate the specified port
    else if (port > NumLimit< Uint16 >::Max)
        SqThrow("Port number is out of range: %u > %u", port, NumLimit< Uint16 >::Max);
    // Validate the specified server
    else if (!server || strlen(server) <= 0)
        SqThrow("Attempting to connect IRC without specifying a server");
    // Validate the specified nickname
    else if (!nick || strlen(nick) <= 0)
        SqThrow("Attempting to connect IRC without specifying a nickname");
    else
    {
        // Save the specified port
        m_Port = port;
        // Save the specified server
        m_Server.assign(server);
        // Save the specified nickname
        m_Nick.assign(nick);
        // Save the specified password
        m_Passwd.assign(!passwd ? _SC("") : passwd);
        // Save the specified user
        m_User.assign(!user ? _SC("") : user);
        // Save the specified name
        m_Name.assign(!name ? _SC("") : name);
        // Enable the reconnection system
        m_Reconnect = true;
        // Reset the number of tries
        m_LeftTries = m_Tries;
        // Set the time-point for the next connection try
        m_NextTry = (GetEpochTimeMicro() + (m_Wait * 1000LL));
        // This is an IPv6 connection
        m_IPv6 = true;
        // Attempt to connect the session and return the result
        return irc_connect6(m_Session, m_Server.c_str(), m_Port,
                            m_Passwd.empty() ? NULL : m_Passwd.c_str(),
                            m_Nick.c_str(),
                            m_User.empty() ? NULL : m_User.c_str(),
                            m_Name.empty() ? NULL : m_Name.c_str()
        );
    }
    // Connection denied before even attempted
    return -1;
}

// ------------------------------------------------------------------------------------------------
void Session::Disconnect()
{
    if (Connected())
    {
        // Update one last time to catch remaining events
        Update();
        // Disable the reconnection system
        m_Reconnect = false;
        // Attempt to disconnect
        irc_disconnect(m_Session);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::ForwardEvent(Session * session, Function & listener, CCStr event,
                            CCStr origin, CCStr * params, Uint32 count)
{
    // Is there anyone even listening to this event?
    if (listener.IsNull())
    {
        return; /* No point in going forward */
    }
    // Make sure that the origin can't be a null pointer
    else if (!origin)
    {
        origin = _SC("");
    }
    // Each event must have an array of parameters (empty or not)
    Array parameters(DefaultVM::Get(), count);
    // Are the any parameters?
    if (params && count > 0)
    {
        // Transform the parameters into a squirrel array
        for (Uint32 i = 0; i < count; ++i)
            parameters.SetValue(i, params[i]);
    }
    // Obtain an object to this session instance without creating a new reference counter!
    ClassType< Session >::PushInstance(DefaultVM::Get(), session);
    // Obtain the pushed object from the stack
    Var< Object > var(DefaultVM::Get(), -1);
    // Call the event with the obtained values
    listener.Execute< Object &, CSStr, CSStr, Array & >(var.value, event, origin, parameters);
}

// ------------------------------------------------------------------------------------------------
void Session::ForwardEvent(Session * session, Function & listener, Uint32 event,
                            CCStr origin, CCStr * params, Uint32 count)
{
    // Is there anyone even listening to this event?
    if (listener.IsNull())
    {
        return; /* No point in going forward */
    }
    // Make sure that the origin can't be a null pointer
    else if (!origin)
    {
        origin = _SC("");
    }
    // Each event must have an array of parameters (empty or not)
    Array parameters(DefaultVM::Get(), count);
    // Are the any parameters?
    if (params && count > 0)
    {
        // Transform the parameters into a squirrel array
        for (unsigned int i = 0; i < count; ++i)
        {
            parameters.SetValue(i, params[i]);
        }
    }
    // Obtain an object to this session instance without creating a new reference counter!
    ClassType< Session >::PushInstance(DefaultVM::Get(), session);
    // Obtain the pushed object from the stack
    Var< Object > var(DefaultVM::Get(), -1);
    // Call the event with the obtained values
    listener.Execute< Object &, Uint32, CSStr, Array & >(var.value, event, origin, parameters);
}

// ------------------------------------------------------------------------------------------------
void Session::ForwardEvent(Session * /*session*/, Function & /*listener*/, CCStr /*nick*/,
                            CCStr /*addr*/, irc_dcc_t /*dccid*/)
{
    /* TODO! */
}

// ------------------------------------------------------------------------------------------------
void Session::ForwardEvent(Session * session, Function & /*listener*/, CCStr /*nick*/,
                            CCStr /*addr*/, CCStr /*filename*/, Ulong /*size*/, irc_dcc_t /*dccid*/)
{
    /* TODO! */
}

// ------------------------------------------------------------------------------------------------
void Session::OnConnect(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
    {
        // Prevent any attempts to reconnect now
        inst->m_Reconnect = false;
        // Save the connection time-stamp to calculate session uptime
        inst->m_SessionTime = GetEpochTimeMicro();
        // Now forward event
        ForwardEvent(inst, inst->m_OnConnect, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnNick(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
    {
        if (params && params[0])
            inst->m_Nick.assign(params[0]);
        ForwardEvent(inst, inst->m_OnNick, event, origin, params, count);
    }
}

// ------------------------------------------------------------------------------------------------
void Session::OnQuit(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnQuit, event, origin, params, count);
}

// ------------------------------------------------------------------------------------------------
void Session::OnJoin(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnJoin, event, origin, params, count);
}

// ------------------------------------------------------------------------------------------------
void Session::OnPart(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnPart, event, origin, params, count);
}

// ------------------------------------------------------------------------------------------------
void Session::OnMode(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnMode, event, origin, params, count);
}

// ------------------------------------------------------------------------------------------------
void Session::OnUmode(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnUmode, event, origin, params, count);
}

// ------------------------------------------------------------------------------------------------
void Session::OnTopic(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnTopic, event, origin, params, count);
}

// ------------------------------------------------------------------------------------------------
void Session::OnKick(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnKick, event, origin, params, count);
}

// ------------------------------------------------------------------------------------------------
void Session::OnChannel(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnChannel, event, origin, params, count);
}

// ------------------------------------------------------------------------------------------------
void Session::OnPrivMsg(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnPrivMsg, event, origin, params, count);
}

// ------------------------------------------------------------------------------------------------
void Session::OnNotice(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnNotice, event, origin, params, count);
}

// ------------------------------------------------------------------------------------------------
void Session::OnChannelNotice(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
       ForwardEvent(inst, inst->m_OnChannelNotice, event, origin, params, count);
}

// ------------------------------------------------------------------------------------------------
void Session::OnInvite(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnInvite, event, origin, params, count);
}

// ------------------------------------------------------------------------------------------------
void Session::OnCtcpReq(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnCtcpReq, event, origin, params, count);

}

// ------------------------------------------------------------------------------------------------
void Session::OnCtcpRep(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnCtcpRep, event, origin, params, count);
}

// ------------------------------------------------------------------------------------------------
void Session::OnCtcpAction(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnCtcpAction, event, origin, params, count);
}

// ------------------------------------------------------------------------------------------------
void Session::OnUnknown(irc_session_t * session, CCStr event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnUnknown, event, origin, params, count);
}

// ------------------------------------------------------------------------------------------------
void Session::OnNumeric(irc_session_t * session, Uint32 event, CCStr origin, CCStr * params, Uint32 count)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnNumeric, event, origin, params, count);
}

// ------------------------------------------------------------------------------------------------
void Session::OnDccChatReq(irc_session_t * session, CCStr nick, CCStr addr, irc_dcc_t dccid)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnDccChatReq, nick, addr, dccid);
}

// ------------------------------------------------------------------------------------------------
void Session::OnDccSendReq(irc_session_t * session, CCStr nick, CCStr addr, CCStr filename, Ulong size, irc_dcc_t dccid)
{
    Session * inst = (Session *)irc_get_ctx(session);
    if (ValidateEventSession(inst))
        ForwardEvent(inst, inst->m_OnDccSendReq, nick, addr, filename, size, dccid);
}

// ------------------------------------------------------------------------------------------------
CSStr GetNick(CSStr origin)
{
    // Attempt to retrieve the nickname
    irc_target_get_nick(origin, g_Buffer, sizeof(g_Buffer));
    // Return the nickname that could be retrieved
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
CSStr GetHost(CSStr target)
{
    // Attempt to retrieve the host
    irc_target_get_host(target, g_Buffer, sizeof(g_Buffer));
    // Return the host that could be retrieved
    return g_Buffer;
}

} // Namespace:: IRC

/* ------------------------------------------------------------------------------------------------
 * Update IRC sessions and process events.
*/
void ProcessIrc()
{
    IRC::Session::Process();
}

/* ------------------------------------------------------------------------------------------------
 * Terminate IRC sessions and release their resources.
*/
void TerminateIrc()
{
    IRC::Session::Terminate();
}

// ================================================================================================
void Register_IRC(HSQUIRRELVM vm)
{
    using namespace IRC;

    Table ircns(vm);

    ircns.Bind(_SC("Session"), Class< Session, NoCopy< Session > >(vm, _SC("SqIrcSession"))
        /* Constructors */
        .Ctor()
        /* Core Metamethods */
        .Func(_SC("_tostring"), &Session::ToString)
        .Func(_SC("_cmp"), &Session::Cmp)
        /* Properties */
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
        .Prop(_SC("Server"), &Session::GetServer, &Session::SetServer)
        .Prop(_SC("ErrNo"), &Session::GetErrNo)
        .Prop(_SC("ErrStr"), &Session::GetErrStr)
        .Prop(_SC("OnConnect"), &Session::GetOnConnect)
        .Prop(_SC("OnNick"), &Session::GetOnNick)
        .Prop(_SC("OnQuit"), &Session::GetOnQuit)
        .Prop(_SC("OnJoin"), &Session::GetOnJoin)
        .Prop(_SC("OnPart"), &Session::GetOnPart)
        .Prop(_SC("OnMode"), &Session::GetOnMode)
        .Prop(_SC("OnUmode"), &Session::GetOnUmode)
        .Prop(_SC("OnTopic"), &Session::GetOnTopic)
        .Prop(_SC("OnKick"), &Session::GetOnKick)
        .Prop(_SC("OnChannel"), &Session::GetOnChannel)
        .Prop(_SC("OnPrivMsg"), &Session::GetOnPrivMsg)
        .Prop(_SC("OnNotice"), &Session::GetOnNotice)
        .Prop(_SC("OnChannelNotice"), &Session::GetOnChannelNotice)
        .Prop(_SC("OnInvite"), &Session::GetOnInvite)
        .Prop(_SC("OnCtcpReq"), &Session::GetOnCtcpReq)
        .Prop(_SC("OnCtcpRep"), &Session::GetOnCtcpRep)
        .Prop(_SC("OnCtcpAction"), &Session::GetOnCtcpAction)
        .Prop(_SC("OnUnknown"), &Session::GetOnUnknown)
        .Prop(_SC("OnNumeric"), &Session::GetOnNumeric)
        .Prop(_SC("OnDccChatReq"), &Session::GetOnDccChatReq)
        .Prop(_SC("OnDccSendReq"), &Session::GetOnDccSendReq)
        /* Functions */
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
        .Func(_SC("Disconnect"), &Session::Disconnect)
        .Overload< Int32 (Session::*)(CSStr) >(_SC("CmdJoin"), &Session::CmdJoin)
        .Overload< Int32 (Session::*)(CSStr, CSStr) >(_SC("CmdJoin"), &Session::CmdJoin)
        .Func(_SC("CmdPart"), &Session::CmdPart)
        .Func(_SC("CmdInvite"), &Session::CmdInvite)
        .Func(_SC("CmdNames"), &Session::CmdNames)
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
        .Func(_SC("CmdMsg"), &Session::CmdMsg)
        .Func(_SC("CmdMe"), &Session::CmdMe)
        .Func(_SC("CmdNotice"), &Session::CmdNotice)
        .Func(_SC("CmdCtcpRequest"), &Session::CmdCtcpRequest)
        .Func(_SC("CmdCtcpReply"), &Session::CmdCtcpReply)
        .Func(_SC("CmdNick"), &Session::CmdNick)
        .Func(_SC("CmdWhois"), &Session::CmdWhois)
        .Overload< Int32 (Session::*)(void) >(_SC("CmdQuit"), &Session::CmdQuit)
        .Overload< Int32 (Session::*)(CSStr) >(_SC("CmdQuit"), &Session::CmdQuit)
        .Func(_SC("SendRaw"), &Session::SendRaw)
        .Func(_SC("DestroyDcc"), &Session::DestroyDcc)
        .Func(_SC("SetCtcpVersion"), &Session::SetCtcpVersion)
        .Func(_SC("SetOption"), &Session::SetOption)
        .Func(_SC("ResetOption"), &Session::ResetOption)
        .Func(_SC("BindConnect"), &Session::BindOnConnect)
        .Func(_SC("BindNick"), &Session::BindOnNick)
        .Func(_SC("BindQuit"), &Session::BindOnQuit)
        .Func(_SC("BindJoin"), &Session::BindOnJoin)
        .Func(_SC("BindPart"), &Session::BindOnPart)
        .Func(_SC("BindMode"), &Session::BindOnMode)
        .Func(_SC("BindUmode"), &Session::BindOnUmode)
        .Func(_SC("BindTopic"), &Session::BindOnTopic)
        .Func(_SC("BindKick"), &Session::BindOnKick)
        .Func(_SC("BindChannel"), &Session::BindOnChannel)
        .Func(_SC("BindPrivMsg"), &Session::BindOnPrivMsg)
        .Func(_SC("BindNotice"), &Session::BindOnNotice)
        .Func(_SC("BindChannelNotice"), &Session::BindOnChannelNotice)
        .Func(_SC("BindInvite"), &Session::BindOnInvite)
        .Func(_SC("BindCtcpReq"), &Session::BindOnCtcpReq)
        .Func(_SC("BindCtcpRep"), &Session::BindOnCtcpRep)
        .Func(_SC("BindCtcpAction"), &Session::BindOnCtcpAction)
        .Func(_SC("BindUnknown"), &Session::BindOnUnknown)
        .Func(_SC("BindNumeric"), &Session::BindOnNumeric)
        .Func(_SC("BindDccChatReq"), &Session::BindOnDccChatReq)
        .Func(_SC("BindDccSendReq"), &Session::BindOnDccSendReq)
    );

    ircns.Func(_SC("GetNick"), &GetNick);
    ircns.Func(_SC("GetHost"), &GetHost);
    ircns.Func(_SC("GetErrStr"), &irc_strerror);

    RootTable(vm).Bind(_SC("SqIrc"), ircns);

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
