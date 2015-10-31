#ifndef _LIBRARY_IRC_SESSION_HPP_
#define _LIBRARY_IRC_SESSION_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <libircclient.h>
#include <libirc_rfcnumeric.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace IRC {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
class Session
{
protected:

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static irc_callbacks_t  s_Callbacks;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static bool             s_Initialized;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static irc_callbacks_t * GetCallbacks() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void ForwardEvent(Function & listener, const char * event, const char * origin, const char ** params, unsigned int count) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void ForwardEvent(Function & listener, unsigned int event, const char * origin, const char ** params, unsigned int count) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void ForwardEvent(Function & listener, const char * nick, const char * addr, irc_dcc_t dccid) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void ForwardEvent(Function & listener, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid) noexcept;

public:

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Session() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Session(const Session & o) = default;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Session(Session && o) = default;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    ~Session();

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Session & operator = (const Session & o) = default;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Session & operator = (Session && o) = default;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void Process(SQFloat delta) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 Cmp(const Session & o) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    const SQChar * ToString() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the local tag.
    */
    const SQChar * GetTag() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the local tag.
    */
    void SetTag(const SQChar * tag) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the local data.
    */
    SqObj & GetData() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the local data.
    */
    void SetData(SqObj & data) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnConnect() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnConnect(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnConnect_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnNick() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnNick(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnNick_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnQuit() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnQuit(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnQuit_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnJoin() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnJoin(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnJoin_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnPart() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnPart(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnPart_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnMode() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnMode(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnMode_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnUmode() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnUmode(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnUmode_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnTopic() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnTopic(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnTopic_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnKick() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnKick(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnKick_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnChannel() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnChannel(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnChannel_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnPrivMSG() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnPrivMSG(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnPrivMSG_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnNotice() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnNotice(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnNotice_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnChannel_Notice() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnChannel_Notice(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnChannel_Notice_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnInvite() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnInvite(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnInvite_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnCTCP_Req() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnCTCP_Req(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnCTCP_Req_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnCTCP_Rep() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnCTCP_Rep(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnCTCP_Rep_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnCTCP_Action() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnCTCP_Action(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnCTCP_Action_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnUnknown() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnUnknown(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnUnknown_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnNumeric() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnNumeric(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnNumeric_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnDcc_Chat_Req() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnDcc_Chat_Req(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnDcc_Chat_Req_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function GetOnDcc_Send_Req() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnDcc_Send_Req(Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOnDcc_Send_Req_Env(SqObj & env, Function & func) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    bool IsValid() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 Connect(const SQChar * server, SQUint32 port, const SQChar * nick) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 Connect(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 Connect(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd,
                    const SQChar * username) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 Connect(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd,
                    const SQChar * username, const SQChar * realname) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 Connect6(const SQChar * server, SQUint32 port, const SQChar * nick) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 Connect6(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 Connect6(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd,
                    const SQChar * username) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 Connect6(const SQChar * server, SQUint32 port, const SQChar * nick, const SQChar * passwd,
                    const SQChar * username, const SQChar * realname) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void Disconnect() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    bool IsConnected() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdJoin(const SQChar * channel) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdJoin(const SQChar * channel, const SQChar * key) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdPart(const SQChar * channel) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdInvite(const SQChar * nick, const SQChar * channel) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdNames(const SQChar * channel) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdList() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdList(const SQChar * channel) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdTopic(const SQChar * channel) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdTopic(const SQChar * channel, const SQChar * topic) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdChannelMode(const SQChar * channel) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdChannelMode(const SQChar * channel, const SQChar * mode) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdUserMode() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdUserMode(const SQChar * mode) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdKick(const SQChar * nick, const SQChar * channel) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdKick(const SQChar * nick, const SQChar * channel, const SQChar * reason) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdMsg(const SQChar * nch, const SQChar * text) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdMe(const SQChar * nch, const SQChar * text) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdNotice(const SQChar * nch, const SQChar * text) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdCtcpRequest(const SQChar * nick, const SQChar * request) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdCtcpReply(const SQChar * nick, const SQChar * reply) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdNick(const SQChar * nick) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdWhois(const SQChar * nick) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdQuit() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 CmdQuit(const SQChar * reason) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 SendRaw(const SQChar * str) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 DestroyDcc(SQUint32 dccid) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetCtcpVersion(const SQChar * version) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInt32 GetErrNo() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    const SQChar * GetErrStr() noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetOption(SQUint32 option) noexcept;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void ResetOption(SQUint32 option) noexcept;

private:

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    irc_session_t*  m_Session;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Function        m_OnConnect;
    Function        m_OnNick;
    Function        m_OnQuit;
    Function        m_OnJoin;
    Function        m_OnPart;
    Function        m_OnMode;
    Function        m_OnUmode;
    Function        m_OnTopic;
    Function        m_OnKick;
    Function        m_OnChannel;
    Function        m_OnPrivMSG;
    Function        m_OnNotice;
    Function        m_OnChannel_Notice;
    Function        m_OnInvite;
    Function        m_OnCTCP_Req;
    Function        m_OnCTCP_Rep;
    Function        m_OnCTCP_Action;
    Function        m_OnUnknown;
    Function        m_OnNumeric;
    Function        m_OnDcc_Chat_Req;
    Function        m_OnDcc_Send_Req;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    String          m_Tag;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SqObj           m_Data;

public:

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnConnect(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnNick(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnQuit(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnJoin(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnPart(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnMode(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnUmode(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnTopic(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnKick(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnChannel(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnPrivMSG(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnNotice(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnChannel_Notice(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnInvite(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnCTCP_Req(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnCTCP_Rep(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnCTCP_Action(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnUnknown(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnNumeric(irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnDcc_Chat_Req(irc_session_t * session, const char * nick, const char * addr, irc_dcc_t dccid);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static void OnDcc_Send_Req(irc_session_t * session, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid);

};

} // Namespace:: IRC
} // Namespace:: SqMod

#endif // _LIBRARY_IRC_SESSION_HPP_
