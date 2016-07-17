// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Helper class that represents an integral enumeration value. Used to reduce compilation times.
*/
struct EnumElement
{
    CSStr Name;
    Int32 Value;
};

// ------------------------------------------------------------------------------------------------
#define SQENUMCOUNT(arr) (sizeof(arr) / sizeof(EnumElement))

// ------------------------------------------------------------------------------------------------
static const EnumElement g_IrcEvent[] = {
    {_SC("Connect"),                                SET_CONNECT},
    {_SC("Nick"),                                   SET_NICK},
    {_SC("Quit"),                                   SET_QUIT},
    {_SC("Join"),                                   SET_JOIN},
    {_SC("Part"),                                   SET_PART},
    {_SC("Mode"),                                   SET_MODE},
    {_SC("Umode"),                                  SET_UMODE},
    {_SC("Topic"),                                  SET_TOPIC},
    {_SC("Kick"),                                   SET_KICK},
    {_SC("Channel"),                                SET_CHANNEL},
    {_SC("PrivMsg"),                                SET_PRIVMSG},
    {_SC("Notice"),                                 SET_NOTICE},
    {_SC("ChannelNotice"),                          SET_CHANNELNOTICE},
    {_SC("Invite"),                                 SET_INVITE},
    {_SC("CtcpReq"),                                SET_CTCPREQ},
    {_SC("CtcpRep"),                                SET_CTCPREP},
    {_SC("CtcpAction"),                             SET_CTCPACTION},
    {_SC("Unknown"),                                SET_UNKNOWN},
    {_SC("Numeric"),                                SET_NUMERIC},
    {_SC("DccChatReq"),                             SET_DCCCHATREQ},
    {_SC("DccSendReq"),                             SET_DCCSENDREQ}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_IrcErr[] = {
    {_SC("Ok"),                                     LIBIRC_ERR_OK},
    {_SC("InVal"),                                  LIBIRC_ERR_INVAL},
    {_SC("Resolv"),                                 LIBIRC_ERR_RESOLV},
    {_SC("Socket"),                                 LIBIRC_ERR_SOCKET},
    {_SC("Connect"),                                LIBIRC_ERR_CONNECT},
    {_SC("Closed"),                                 LIBIRC_ERR_CLOSED},
    {_SC("NoMem"),                                  LIBIRC_ERR_NOMEM},
    {_SC("Accept"),                                 LIBIRC_ERR_ACCEPT},
    {_SC("NoDccSend"),                              LIBIRC_ERR_NODCCSEND},
    {_SC("Read"),                                   LIBIRC_ERR_READ},
    {_SC("Write"),                                  LIBIRC_ERR_WRITE},
    {_SC("State"),                                  LIBIRC_ERR_STATE},
    {_SC("Timeout"),                                LIBIRC_ERR_TIMEOUT},
    {_SC("OpenFile"),                               LIBIRC_ERR_OPENFILE},
    {_SC("Terminated"),                             LIBIRC_ERR_TERMINATED},
    {_SC("NoIPv6"),                                 LIBIRC_ERR_NOIPV6},
    {_SC("SSLNotSupported"),                        LIBIRC_ERR_SSL_NOT_SUPPORTED},
    {_SC("SSLInitFailed"),                          LIBIRC_ERR_SSL_INIT_FAILED},
    {_SC("ConnectSSLFailed"),                       LIBIRC_ERR_CONNECT_SSL_FAILED},
    {_SC("SSLCertVerifyFailed"),                    LIBIRC_ERR_SSL_CERT_VERIFY_FAILED},
    {_SC("Max"),                                    LIBIRC_ERR_MAX}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_IrcOpt[] = {
    {_SC("Debug"),                                  LIBIRC_OPTION_DEBUG},
    {_SC("StripNicks"),                             LIBIRC_OPTION_STRIPNICKS},
    {_SC("SSLNoVerify"),                            LIBIRC_OPTION_SSL_NO_VERIFY}

};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_IrcRFC[] = {

    {_SC("RPL_WELCOME"),                            LIBIRC_RFC_RPL_WELCOME},
    {_SC("RPL_YOURHOST"),                           LIBIRC_RFC_RPL_YOURHOST},
    {_SC("RPL_CREATED"),                            LIBIRC_RFC_RPL_CREATED},
    {_SC("RPL_MYINFO"),                             LIBIRC_RFC_RPL_MYINFO},
    {_SC("RPL_BOUNCE"),                             LIBIRC_RFC_RPL_BOUNCE},
    {_SC("RPL_NONE"),                               LIBIRC_RFC_RPL_NONE},
    {_SC("RPL_USERHOST"),                           LIBIRC_RFC_RPL_USERHOST},
    {_SC("RPL_ISON"),                               LIBIRC_RFC_RPL_ISON},
    {_SC("RPL_AWAY"),                               LIBIRC_RFC_RPL_AWAY},
    {_SC("RPL_UNAWAY"),                             LIBIRC_RFC_RPL_UNAWAY},
    {_SC("RPL_NOWAWAY"),                            LIBIRC_RFC_RPL_NOWAWAY},
    {_SC("RPL_WHOISUSER"),                          LIBIRC_RFC_RPL_WHOISUSER},
    {_SC("RPL_WHOISSERVER"),                        LIBIRC_RFC_RPL_WHOISSERVER},
    {_SC("RPL_WHOISOPERATOR"),                      LIBIRC_RFC_RPL_WHOISOPERATOR},
    {_SC("RPL_WHOISIDLE"),                          LIBIRC_RFC_RPL_WHOISIDLE},
    {_SC("RPL_ENDOFWHOIS"),                         LIBIRC_RFC_RPL_ENDOFWHOIS},
    {_SC("RPL_WHOISCHANNELS"),                      LIBIRC_RFC_RPL_WHOISCHANNELS},
    {_SC("RPL_WHOWASUSER"),                         LIBIRC_RFC_RPL_WHOWASUSER},
    {_SC("RPL_ENDOFWHOWAS"),                        LIBIRC_RFC_RPL_ENDOFWHOWAS},
    {_SC("RPL_LISTSTART"),                          LIBIRC_RFC_RPL_LISTSTART},
    {_SC("RPL_LIST"),                               LIBIRC_RFC_RPL_LIST},
    {_SC("RPL_LISTEND"),                            LIBIRC_RFC_RPL_LISTEND},
    {_SC("RPL_UNIQOPIS"),                           LIBIRC_RFC_RPL_UNIQOPIS},
    {_SC("RPL_CHANNELMODEIS"),                      LIBIRC_RFC_RPL_CHANNELMODEIS},
    {_SC("RPL_NOTOPIC"),                            LIBIRC_RFC_RPL_NOTOPIC},
    {_SC("RPL_TOPIC"),                              LIBIRC_RFC_RPL_TOPIC},
    {_SC("RPL_INVITING"),                           LIBIRC_RFC_RPL_INVITING},
    {_SC("RPL_SUMMONING"),                          LIBIRC_RFC_RPL_SUMMONING},
    {_SC("RPL_INVITELIST"),                         LIBIRC_RFC_RPL_INVITELIST},
    {_SC("RPL_ENDOFINVITELIST"),                    LIBIRC_RFC_RPL_ENDOFINVITELIST},
    {_SC("RPL_EXCEPTLIST"),                         LIBIRC_RFC_RPL_EXCEPTLIST},
    {_SC("RPL_ENDOFEXCEPTLIST"),                    LIBIRC_RFC_RPL_ENDOFEXCEPTLIST},
    {_SC("RPL_VERSION"),                            LIBIRC_RFC_RPL_VERSION},
    {_SC("RPL_WHOREPLY"),                           LIBIRC_RFC_RPL_WHOREPLY},
    {_SC("RPL_ENDOFWHO"),                           LIBIRC_RFC_RPL_ENDOFWHO},
    {_SC("RPL_NAMREPLY"),                           LIBIRC_RFC_RPL_NAMREPLY},
    {_SC("RPL_ENDOFNAMES"),                         LIBIRC_RFC_RPL_ENDOFNAMES},
    {_SC("RPL_LINKS"),                              LIBIRC_RFC_RPL_LINKS},
    {_SC("RPL_ENDOFLINKS"),                         LIBIRC_RFC_RPL_ENDOFLINKS},
    {_SC("RPL_BANLIST"),                            LIBIRC_RFC_RPL_BANLIST},
    {_SC("RPL_ENDOFBANLIST"),                       LIBIRC_RFC_RPL_ENDOFBANLIST},
    {_SC("RPL_INFO"),                               LIBIRC_RFC_RPL_INFO},
    {_SC("RPL_ENDOFINFO"),                          LIBIRC_RFC_RPL_ENDOFINFO},
    {_SC("RPL_MOTDSTART"),                          LIBIRC_RFC_RPL_MOTDSTART},
    {_SC("RPL_MOTD"),                               LIBIRC_RFC_RPL_MOTD},
    {_SC("RPL_ENDOFMOTD"),                          LIBIRC_RFC_RPL_ENDOFMOTD},
    {_SC("RPL_YOUREOPER"),                          LIBIRC_RFC_RPL_YOUREOPER},
    {_SC("RPL_REHASHING"),                          LIBIRC_RFC_RPL_REHASHING},
    {_SC("RPL_YOURESERVICE"),                       LIBIRC_RFC_RPL_YOURESERVICE},
    {_SC("RPL_TIME"),                               LIBIRC_RFC_RPL_TIME},
    {_SC("RPL_USERSSTART"),                         LIBIRC_RFC_RPL_USERSSTART},
    {_SC("RPL_USERS"),                              LIBIRC_RFC_RPL_USERS},
    {_SC("RPL_ENDOFUSERS"),                         LIBIRC_RFC_RPL_ENDOFUSERS},
    {_SC("RPL_NOUSERS"),                            LIBIRC_RFC_RPL_NOUSERS},
    {_SC("RPL_TRACELINK"),                          LIBIRC_RFC_RPL_TRACELINK},
    {_SC("RPL_TRACECONNECTING"),                    LIBIRC_RFC_RPL_TRACECONNECTING},
    {_SC("RPL_TRACEHANDSHAKE"),                     LIBIRC_RFC_RPL_TRACEHANDSHAKE},
    {_SC("RPL_TRACEUNKNOWN"),                       LIBIRC_RFC_RPL_TRACEUNKNOWN},
    {_SC("RPL_TRACEOPERATOR"),                      LIBIRC_RFC_RPL_TRACEOPERATOR},
    {_SC("RPL_TRACEUSER"),                          LIBIRC_RFC_RPL_TRACEUSER},
    {_SC("RPL_TRACESERVER"),                        LIBIRC_RFC_RPL_TRACESERVER},
    {_SC("RPL_TRACESERVICE"),                       LIBIRC_RFC_RPL_TRACESERVICE},
    {_SC("RPL_TRACENEWTYPE"),                       LIBIRC_RFC_RPL_TRACENEWTYPE},
    {_SC("RPL_TRACECLASS"),                         LIBIRC_RFC_RPL_TRACECLASS},
    {_SC("RPL_TRACELOG"),                           LIBIRC_RFC_RPL_TRACELOG},
    {_SC("RPL_TRACEEND"),                           LIBIRC_RFC_RPL_TRACEEND},
    {_SC("RPL_STATSLINKINFO"),                      LIBIRC_RFC_RPL_STATSLINKINFO},
    {_SC("RPL_STATSCOMMANDS"),                      LIBIRC_RFC_RPL_STATSCOMMANDS},
    {_SC("RPL_ENDOFSTATS"),                         LIBIRC_RFC_RPL_ENDOFSTATS},
    {_SC("RPL_STATSUPTIME"),                        LIBIRC_RFC_RPL_STATSUPTIME},
    {_SC("RPL_STATSOLINE"),                         LIBIRC_RFC_RPL_STATSOLINE},
    {_SC("RPL_UMODEIS"),                            LIBIRC_RFC_RPL_UMODEIS},
    {_SC("RPL_SERVLIST"),                           LIBIRC_RFC_RPL_SERVLIST},
    {_SC("RPL_SERVLISTEND"),                        LIBIRC_RFC_RPL_SERVLISTEND},
    {_SC("RPL_LUSERCLIENT"),                        LIBIRC_RFC_RPL_LUSERCLIENT},
    {_SC("RPL_LUSEROP"),                            LIBIRC_RFC_RPL_LUSEROP},
    {_SC("RPL_LUSERUNKNOWN"),                       LIBIRC_RFC_RPL_LUSERUNKNOWN},
    {_SC("RPL_LUSERCHANNELS"),                      LIBIRC_RFC_RPL_LUSERCHANNELS},
    {_SC("RPL_LUSERME"),                            LIBIRC_RFC_RPL_LUSERME},
    {_SC("RPL_ADMINME"),                            LIBIRC_RFC_RPL_ADMINME},
    {_SC("RPL_ADMINLOC1"),                          LIBIRC_RFC_RPL_ADMINLOC1},
    {_SC("RPL_ADMINLOC2"),                          LIBIRC_RFC_RPL_ADMINLOC2},
    {_SC("RPL_ADMINEMAIL"),                         LIBIRC_RFC_RPL_ADMINEMAIL},
    {_SC("RPL_TRYAGAIN"),                           LIBIRC_RFC_RPL_TRYAGAIN},
    {_SC("ERR_NOSUCHNICK"),                         LIBIRC_RFC_ERR_NOSUCHNICK},
    {_SC("ERR_NOSUCHSERVER"),                       LIBIRC_RFC_ERR_NOSUCHSERVER},
    {_SC("ERR_NOSUCHCHANNEL"),                      LIBIRC_RFC_ERR_NOSUCHCHANNEL},
    {_SC("ERR_CANNOTSENDTOCHAN"),                   LIBIRC_RFC_ERR_CANNOTSENDTOCHAN},
    {_SC("ERR_TOOMANYCHANNELS"),                    LIBIRC_RFC_ERR_TOOMANYCHANNELS},
    {_SC("ERR_WASNOSUCHNICK"),                      LIBIRC_RFC_ERR_WASNOSUCHNICK},
    {_SC("ERR_TOOMANYTARGETS"),                     LIBIRC_RFC_ERR_TOOMANYTARGETS},
    {_SC("ERR_NOSUCHSERVICE"),                      LIBIRC_RFC_ERR_NOSUCHSERVICE},
    {_SC("ERR_NOORIGIN"),                           LIBIRC_RFC_ERR_NOORIGIN},
    {_SC("ERR_NORECIPIENT"),                        LIBIRC_RFC_ERR_NORECIPIENT},
    {_SC("ERR_NOTEXTTOSEND"),                       LIBIRC_RFC_ERR_NOTEXTTOSEND},
    {_SC("ERR_NOTOPLEVEL"),                         LIBIRC_RFC_ERR_NOTOPLEVEL},
    {_SC("ERR_WILDTOPLEVEL"),                       LIBIRC_RFC_ERR_WILDTOPLEVEL},
    {_SC("ERR_BADMASK"),                            LIBIRC_RFC_ERR_BADMASK},
    {_SC("ERR_UNKNOWNCOMMAND"),                     LIBIRC_RFC_ERR_UNKNOWNCOMMAND},
    {_SC("ERR_NOMOTD"),                             LIBIRC_RFC_ERR_NOMOTD},
    {_SC("ERR_NOADMININFO"),                        LIBIRC_RFC_ERR_NOADMININFO},
    {_SC("ERR_FILEERROR"),                          LIBIRC_RFC_ERR_FILEERROR},
    {_SC("ERR_NONICKNAMEGIVEN"),                    LIBIRC_RFC_ERR_NONICKNAMEGIVEN},
    {_SC("ERR_ERRONEUSNICKNAME"),                   LIBIRC_RFC_ERR_ERRONEUSNICKNAME},
    {_SC("ERR_NICKNAMEINUSE"),                      LIBIRC_RFC_ERR_NICKNAMEINUSE},
    {_SC("ERR_NICKCOLLISION"),                      LIBIRC_RFC_ERR_NICKCOLLISION},
    {_SC("ERR_UNAVAILRESOURCE"),                    LIBIRC_RFC_ERR_UNAVAILRESOURCE},
    {_SC("ERR_USERNOTINCHANNEL"),                   LIBIRC_RFC_ERR_USERNOTINCHANNEL},
    {_SC("ERR_NOTONCHANNEL"),                       LIBIRC_RFC_ERR_NOTONCHANNEL},
    {_SC("ERR_USERONCHANNEL"),                      LIBIRC_RFC_ERR_USERONCHANNEL},
    {_SC("ERR_NOLOGIN"),                            LIBIRC_RFC_ERR_NOLOGIN},
    {_SC("ERR_SUMMONDISABLED"),                     LIBIRC_RFC_ERR_SUMMONDISABLED},
    {_SC("ERR_USERSDISABLED"),                      LIBIRC_RFC_ERR_USERSDISABLED},
    {_SC("ERR_NOTREGISTERED"),                      LIBIRC_RFC_ERR_NOTREGISTERED},
    {_SC("ERR_NEEDMOREPARAMS"),                     LIBIRC_RFC_ERR_NEEDMOREPARAMS},
    {_SC("ERR_ALREADYREGISTRED"),                   LIBIRC_RFC_ERR_ALREADYREGISTRED},
    {_SC("ERR_NOPERMFORHOST"),                      LIBIRC_RFC_ERR_NOPERMFORHOST},
    {_SC("ERR_PASSWDMISMATCH"),                     LIBIRC_RFC_ERR_PASSWDMISMATCH},
    {_SC("ERR_YOUREBANNEDCREEP"),                   LIBIRC_RFC_ERR_YOUREBANNEDCREEP},
    {_SC("ERR_YOUWILLBEBANNED"),                    LIBIRC_RFC_ERR_YOUWILLBEBANNED},
    {_SC("ERR_KEYSET"),                             LIBIRC_RFC_ERR_KEYSET},
    {_SC("ERR_CHANNELISFULL"),                      LIBIRC_RFC_ERR_CHANNELISFULL},
    {_SC("ERR_UNKNOWNMODE"),                        LIBIRC_RFC_ERR_UNKNOWNMODE},
    {_SC("ERR_INVITEONLYCHAN"),                     LIBIRC_RFC_ERR_INVITEONLYCHAN},
    {_SC("ERR_BANNEDFROMCHAN"),                     LIBIRC_RFC_ERR_BANNEDFROMCHAN},
    {_SC("ERR_BADCHANNELKEY"),                      LIBIRC_RFC_ERR_BADCHANNELKEY},
    {_SC("ERR_BADCHANMASK"),                        LIBIRC_RFC_ERR_BADCHANMASK},
    {_SC("ERR_NOCHANMODES"),                        LIBIRC_RFC_ERR_NOCHANMODES},
    {_SC("ERR_BANLISTFULL"),                        LIBIRC_RFC_ERR_BANLISTFULL},
    {_SC("ERR_NOPRIVILEGES"),                       LIBIRC_RFC_ERR_NOPRIVILEGES},
    {_SC("ERR_CHANOPRIVSNEEDED"),                   LIBIRC_RFC_ERR_CHANOPRIVSNEEDED},
    {_SC("ERR_CANTKILLSERVER"),                     LIBIRC_RFC_ERR_CANTKILLSERVER},
    {_SC("ERR_RESTRICTED"),                         LIBIRC_RFC_ERR_RESTRICTED},
    {_SC("ERR_UNIQOPPRIVSNEEDED"),                  LIBIRC_RFC_ERR_UNIQOPPRIVSNEEDED},
    {_SC("ERR_NOOPERHOST"),                         LIBIRC_RFC_ERR_NOOPERHOST},
    {_SC("ERR_UMODEUNKNOWNFLAG"),                   LIBIRC_RFC_ERR_UMODEUNKNOWNFLAG},
    {_SC("ERR_USERSDONTMATCH"),                     LIBIRC_RFC_ERR_USERSDONTMATCH}
};

// ------------------------------------------------------------------------------------------------
static Enumeration RegisterEnum(HSQUIRRELVM vm, CSStr name, const EnumElement * data, Uint32 count)
{
    // Allocate an empty enumeration
    Enumeration e(vm);
    // Register the values from the received data
    for (Uint32 n = 0; n < count; ++n, ++data)
    {
        e.Const(data->Name, data->Value);
    }
    // Bind the enumeration to the constant table
    ConstTable(vm).Enum(name, e);
    // Return the enumeration for further changes if necessary
    return e;
}

// ================================================================================================
void Register_Constants(Table & ircns)
{
    RegisterEnum(ircns.GetVM(), _SC("SqIrcEvent"),      g_IrcEvent,     SQENUMCOUNT(g_IrcEvent));
    RegisterEnum(ircns.GetVM(), _SC("SqIrcErr"),        g_IrcErr,       SQENUMCOUNT(g_IrcErr));
    RegisterEnum(ircns.GetVM(), _SC("SqIrcOpt"),        g_IrcOpt,       SQENUMCOUNT(g_IrcOpt));
    RegisterEnum(ircns.GetVM(), _SC("SqIrcRFC"),        g_IrcRFC,       SQENUMCOUNT(g_IrcRFC));
}

} // Namespace:: SqMod