#include "Library/IRC/Session.hpp"
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
Session::Session() noexcept
    : m_Session(irc_create_session(GetCallbacks()))
{
    if (!m_Session)
    {
        LogErr("Unable to create an IRC session");
        m_Session = nullptr;
    }
    else
    {
        irc_set_ctx(m_Session, this);
    }
}

// ------------------------------------------------------------------------------------------------
Session::~Session()
{
    if (m_Session != nullptr)
    {
        irc_destroy_session(m_Session);
    }
}

// ------------------------------------------------------------------------------------------------
irc_callbacks_t * Session::GetCallbacks() noexcept
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
void Session::ForwardEvent(Function & listener, const char * event, const char * origin, const char ** params, unsigned int count) noexcept
{
    if (listener.IsNull())
    {
        return;
    }

    Array parameters(DefaultVM::Get(), count);

    if (params && count > 0)
    {
        for (unsigned int i = 0; i < count; ++i)
        {
            parameters.SetValue(i, params[i]);
        }
    }

    listener.Execute< const SQChar *, const SQChar *, Array >(event, origin ? origin : _SC(""), parameters);
}

// ------------------------------------------------------------------------------------------------
void Session::ForwardEvent(Function & listener, unsigned int event, const char * origin, const char ** params, unsigned int count) noexcept
{
    if (listener.IsNull())
    {
        return;
    }

    Array parameters(DefaultVM::Get(), count);

    if (params && count > 0)
    {
        for (unsigned int i = 0; i < count; ++i)
        {
            parameters.SetValue(i, params[i]);
        }
    }

    listener.Execute< unsigned int, const SQChar *, Array >(event, origin ? origin : _SC(""), parameters);
}

// ------------------------------------------------------------------------------------------------
void Session::ForwardEvent(Function & listener, const char * nick, const char * addr, irc_dcc_t dccid) noexcept
{
    /* @TODO: Implement! */
}

// ------------------------------------------------------------------------------------------------
void Session::ForwardEvent(Function & listener, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid) noexcept
{
    /* @TODO: Implement! */
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Cmp(const Session & o) const noexcept
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
const SQChar * Session::ToString() const noexcept
{
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
const SQChar * Session::GetTag() const noexcept
{
    return m_Tag.c_str();
}

// ------------------------------------------------------------------------------------------------
void Session::SetTag(const SQChar * tag) noexcept
{
    m_Tag.assign(tag);
}

// ------------------------------------------------------------------------------------------------
SqObj & Session::GetData() noexcept
{
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void Session::SetData(SqObj & data) noexcept
{
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnConnect() noexcept
{
    return m_OnConnect;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnConnect(Function & func) noexcept
{
    m_OnConnect = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnNick() noexcept
{
    return m_OnNick;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnNick(Function & func) noexcept
{
    m_OnNick = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnQuit() noexcept
{
    return m_OnQuit;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnQuit(Function & func) noexcept
{
    m_OnQuit = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnJoin() noexcept
{
    return m_OnJoin;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnJoin(Function & func) noexcept
{
    m_OnJoin = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnPart() noexcept
{
    return m_OnPart;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnPart(Function & func) noexcept
{
    m_OnPart = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnMode() noexcept
{
    return m_OnMode;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnMode(Function & func) noexcept
{
    m_OnMode = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnUmode() noexcept
{
    return m_OnUmode;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnUmode(Function & func) noexcept
{
    m_OnUmode = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnTopic() noexcept
{
    return m_OnTopic;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnTopic(Function & func) noexcept
{
    m_OnTopic = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnKick() noexcept
{
    return m_OnKick;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnKick(Function & func) noexcept
{
    m_OnKick = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnChannel() noexcept
{
    return m_OnChannel;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnChannel(Function & func) noexcept
{
    m_OnChannel = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnPrivMSG() noexcept
{
    return m_OnPrivMSG;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnPrivMSG(Function & func) noexcept
{
    m_OnPrivMSG = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnNotice() noexcept
{
    return m_OnNotice;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnNotice(Function & func) noexcept
{
    m_OnNotice = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnChannel_Notice() noexcept
{
    return m_OnChannel_Notice;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnChannel_Notice(Function & func) noexcept
{
    m_OnChannel_Notice = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnInvite() noexcept
{
    return m_OnInvite;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnInvite(Function & func) noexcept
{
    m_OnInvite = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnCTCP_Req() noexcept
{
    return m_OnCTCP_Req;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnCTCP_Req(Function & func) noexcept
{
    m_OnCTCP_Req = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnCTCP_Rep() noexcept
{
    return m_OnCTCP_Rep;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnCTCP_Rep(Function & func) noexcept
{
    m_OnCTCP_Rep = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnCTCP_Action() noexcept
{
    return m_OnCTCP_Action;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnCTCP_Action(Function & func) noexcept
{
    m_OnCTCP_Action = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnUnknown() noexcept
{
    return m_OnUnknown;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnUnknown(Function & func) noexcept
{
    m_OnUnknown = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnNumeric() noexcept
{
    return m_OnNumeric;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnNumeric(Function & func) noexcept
{
    m_OnNumeric = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnDcc_Chat_Req() noexcept
{
    return m_OnDcc_Chat_Req;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnDcc_Chat_Req(Function & func) noexcept
{
    m_OnDcc_Chat_Req = func;
}

// ------------------------------------------------------------------------------------------------
Function & Session::GetOnDcc_Send_Req() noexcept
{
    return m_OnDcc_Send_Req;
}

// ------------------------------------------------------------------------------------------------
void Session::SetOnDcc_Send_Req(Function & func) noexcept
{
    m_OnDcc_Send_Req = func;
}

// ------------------------------------------------------------------------------------------------
bool Session::IsValid() const noexcept
{
    return (m_Session != nullptr);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect(const SQChar * server, SQUint32 port, const SQChar * nick) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_connect(m_Session, server, port, NULL, nick, NULL, NULL);
    }
    else
    {
        LogWrn("Attempting to <connect session to server> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_connect(m_Session, server, port, passwd, nick, NULL, NULL);
    }
    else
    {
        LogWrn("Attempting to <connect session to server> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd,
                const SQChar * username) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_connect(m_Session, server, port, passwd, nick, username, NULL);
    }
    else
    {
        LogWrn("Attempting to <connect session to server> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd,
                const SQChar * username, const SQChar * realname) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_connect(m_Session, server, port, passwd, nick, username, realname);
    }
    else
    {
        LogWrn("Attempting to <connect session to server> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect6(const SQChar * server, SQUint32 port, const SQChar * nick) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_connect6(m_Session, server, port, NULL, nick, NULL, NULL);
    }
    else
    {
        LogWrn("Attempting to <connect session to server> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect6(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_connect6(m_Session, server, port, passwd, nick, NULL, NULL);
    }
    else
    {
        LogWrn("Attempting to <connect session to server> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect6(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd,
                const SQChar * username) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_connect6(m_Session, server, port, passwd, nick, username, NULL);
    }
    else
    {
        LogWrn("Attempting to <connect session to server> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::Connect6(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd,
                const SQChar * username, const SQChar * realname) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_connect6(m_Session, server, port, passwd, nick, username, realname);
    }
    else
    {
        LogWrn("Attempting to <connect session to server> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void Session::Disconnect() noexcept
{
    if (m_Session != nullptr)
    {
        irc_disconnect(m_Session);
    }
    else
    {
        LogWrn("Attempting to <disconnect session to server> using an invalid session: null");
    }
}

// ------------------------------------------------------------------------------------------------
bool Session::IsConnected() noexcept
{
    if (m_Session != nullptr)
    {
        return (irc_is_connected(m_Session) > 0);
    }
    else
    {
        LogWrn("Attempting to <see if session is connected> using an invalid session: null");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdJoin(const SQChar * channel) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_join(m_Session, channel, NULL);
    }
    else
    {
        LogWrn("Attempting to <join channel> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdJoin(const SQChar * channel, const SQChar * key) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_join(m_Session, channel, key);
    }
    else
    {
        LogWrn("Attempting to <join channel> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdPart(const SQChar * channel) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_part(m_Session, channel);
    }
    else
    {
        LogWrn("Attempting to <part channel> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdInvite(const SQChar * nick, const SQChar * channel) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_invite(m_Session, nick, channel);
    }
    else
    {
        LogWrn("Attempting to <invite onto channel> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdNames(const SQChar * channel) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_names(m_Session, channel);
    }
    else
    {
        LogWrn("Attempting to <get channel names list> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdList() noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_list(m_Session, NULL);
    }
    else
    {
        LogWrn("Attempting to <get active channel list> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdList(const SQChar * channel) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_list(m_Session, channel);
    }
    else
    {
        LogWrn("Attempting to <get active channel list> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdTopic(const SQChar * channel) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_topic(m_Session, channel, NULL);
    }
    else
    {
        LogWrn("Attempting to <get channel topic> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdTopic(const SQChar * channel, const SQChar * topic) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_topic(m_Session, channel, topic);
    }
    else
    {
        LogWrn("Attempting to <set channel topic> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdChannelMode(const SQChar * channel) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_channel_mode(m_Session, channel, NULL);
    }
    else
    {
        LogWrn("Attempting to <get channel mode> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdChannelMode(const SQChar * channel, const SQChar * mode) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_channel_mode(m_Session, channel, mode);
    }
    else
    {
        LogWrn("Attempting to <set channel mode> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdUserMode() noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_user_mode(m_Session, NULL);
    }
    else
    {
        LogWrn("Attempting to <get user mode> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdUserMode(const SQChar * mode) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_user_mode(m_Session, mode);
    }
    else
    {
        LogWrn("Attempting to <set user mode> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdKick(const SQChar * nick, const SQChar * channel) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_kick(m_Session, nick, channel, NULL);
    }
    else
    {
        LogWrn("Attempting to <kick from channel> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdKick(const SQChar * nick, const SQChar * channel, const SQChar * reason) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_kick(m_Session, nick, channel, reason);
    }
    else
    {
        LogWrn("Attempting to <kick from channel> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdMsg(const SQChar * nch, const SQChar * text) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_msg(m_Session, nch, text);
    }
    else
    {
        LogWrn("Attempting to <send message> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdMe(const SQChar * nch, const SQChar * text) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_me(m_Session, nch, text);
    }
    else
    {
        LogWrn("Attempting to <send me message> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdNotice(const SQChar * nch, const SQChar * text) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_notice(m_Session, nch, text);
    }
    else
    {
        LogWrn("Attempting to <send notice message> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdCtcpRequest(const SQChar * nick, const SQChar * request) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_notice(m_Session, nick, request);
    }
    else
    {
        LogWrn("Attempting to <send ctcp request> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdCtcpReply(const SQChar * nick, const SQChar * reply) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_ctcp_reply(m_Session, nick, reply);
    }
    else
    {
        LogWrn("Attempting to <send ctcp reply> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdNick(const SQChar * nick) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_nick(m_Session, nick);
    }
    else
    {
        LogWrn("Attempting to <change nick> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdWhois(const SQChar * nick) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_whois(m_Session, nick);
    }
    else
    {
        LogWrn("Attempting to <request whois> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdQuit() noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_quit(m_Session, NULL);
    }
    else
    {
        LogWrn("Attempting to <quit server> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::CmdQuit(const SQChar * reason) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_cmd_quit(m_Session, reason);
    }
    else
    {
        LogWrn("Attempting to <quit server> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Session::SendRaw(const SQChar * str) noexcept
{
    if (m_Session != nullptr)
    {
        return irc_send_raw(m_Session, str);
    }
    else
    {
        LogWrn("Attempting to <send raw data> using an invalid session: null");
    }

    return SQMOD_UNKNOWN;
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
        ForwardEvent(inst->m_OnConnect, event, origin, params, count);
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
        ForwardEvent(inst->m_OnNick, event, origin, params, count);
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
        ForwardEvent(inst->m_OnQuit, event, origin, params, count);
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
        ForwardEvent(inst->m_OnJoin, event, origin, params, count);
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
        ForwardEvent(inst->m_OnPart, event, origin, params, count);
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
        ForwardEvent(inst->m_OnMode, event, origin, params, count);
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
        ForwardEvent(inst->m_OnUmode, event, origin, params, count);
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
        ForwardEvent(inst->m_OnTopic, event, origin, params, count);
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
        ForwardEvent(inst->m_OnKick, event, origin, params, count);
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
        ForwardEvent(inst->m_OnChannel, event, origin, params, count);
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
        ForwardEvent(inst->m_OnPrivMSG, event, origin, params, count);
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
        ForwardEvent(inst->m_OnNotice, event, origin, params, count);
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
        ForwardEvent(inst->m_OnChannel_Notice, event, origin, params, count);
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
        ForwardEvent(inst->m_OnInvite, event, origin, params, count);
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
        ForwardEvent(inst->m_OnCTCP_Req, event, origin, params, count);
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
        ForwardEvent(inst->m_OnCTCP_Rep, event, origin, params, count);
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
        ForwardEvent(inst->m_OnCTCP_Action, event, origin, params, count);
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
        ForwardEvent(inst->m_OnUnknown, event, origin, params, count);
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
        ForwardEvent(inst->m_OnNumeric, event, origin, params, count);
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
        ForwardEvent(inst->m_OnConnect, nick, addr, dccid);
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
        ForwardEvent(inst->m_OnConnect, nick, addr, filename, size, dccid);
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * GetNick(const SQChar * origin) noexcept
{
    // Attempt to retrieve the nickname
    irc_target_get_nick(origin, g_Buffer, std::extent< decltype(g_Buffer) >::value * sizeof(SQChar));
    // Return the nickname that could be retrieved
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
const SQChar * GetHost(const SQChar * target) noexcept
{
    // Attempt to retrieve the nickname
    irc_target_get_host(target, g_Buffer, std::extent< decltype(g_Buffer) >::value * sizeof(SQChar));
    // Return the nickname that could be retrieved
    return g_Buffer;
}

} // Namespace:: IRC

// ================================================================================================
bool Register_IRC(HSQUIRRELVM vm)
{
    using namespace IRC;
    // Output debugging information
    LogDbg("Beginning registration of <IRCSession> type");
    // IRC sessions should not be copied for the sake of simplicity
    typedef NoCopy< Session > Allocator;
    // Attempt to register the specified type
    Sqrat::RootTable(vm).Bind(_SC("IRCSession"), Sqrat::Class< Session, Allocator >(vm, _SC("IRCSession"))
        /* Constructors */
        .Ctor()
        /* Metamethods */
        .Func(_SC("_cmp"), &Session::Cmp)
        .Func(_SC("_tostring"), &Session::ToString)
        /* Properties */
        .Prop(_SC("ltag"), &Session::GetTag, &Session::SetTag)
        .Prop(_SC("ldata"), &Session::GetData, &Session::SetData)
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
        /* Functions */
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
        .Func(_SC("cmd_send_raw"), &Session::SendRaw)
        /* Overloads */
        .Overload< SQInt32 (Session::*)(const SQChar *, SQUint32, const SQChar *) >
            (_SC("connect"), &Session::Connect)
        .Overload< SQInt32 (Session::*)(const SQChar *, SQUint32, const SQChar *, const SQChar *) >
            (_SC("connect"), &Session::Connect)
        .Overload< SQInt32 (Session::*)(const SQChar *, SQUint32, const SQChar *, const SQChar *, const SQChar *) >
            (_SC("connect"), &Session::Connect)
        .Overload< SQInt32 (Session::*)(const SQChar *, SQUint32, const SQChar *, const SQChar *, const SQChar *, const SQChar *) >
            (_SC("connect"), &Session::Connect)
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
    LogDbg("Registration of <IRCSession> type was successful");
    // Output debugging information
    LogDbg("Beginning registration of <IRC Constants> type");
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
    LogDbg("Registration of <IRC Constants> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
