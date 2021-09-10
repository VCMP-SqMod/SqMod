// ------------------------------------------------------------------------------------------------
#include "Library/DPPTy.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDppCachePolicy, _SC("SqDppCachePolicy"))
SQMOD_DECL_TYPENAME(SqDppUptime, _SC("SqDppUptime"))
SQMOD_DECL_TYPENAME(SqDppIconHash, _SC("SqDppIconHash"))
SQMOD_DECL_TYPENAME(SqDppActivity, _SC("SqDppActivity"))
SQMOD_DECL_TYPENAME(SqDppPresence, _SC("SqDppPresence"))
SQMOD_DECL_TYPENAME(SqDppVoiceState, _SC("SqDppVoiceState"))
SQMOD_DECL_TYPENAME(SqDppGuild, _SC("SqDppGuild"))

// ------------------------------------------------------------------------------------------------
void Register_DPPConst(HSQUIRRELVM vm, Table & ns);

// ------------------------------------------------------------------------------------------------
void Register_DPPTy(HSQUIRRELVM vm, Table & ns)
{
	Register_DPPConst(vm, ns);
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Uptime"),
        Class< dpp::utility::uptime >(vm, SqDppUptime::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppUptime::Fn)
        .Func(_SC("_tostring"), &dpp::utility::uptime::to_string)
        // Member Variables
        .Var(_SC("Days"), &dpp::utility::uptime::days)
        .Var(_SC("Hours"), &dpp::utility::uptime::hours)
        .Var(_SC("Minutes"), &dpp::utility::uptime::mins)
        .Var(_SC("Seconds"), &dpp::utility::uptime::secs)
        // Member Methods
        .Func(_SC("ToSeconds"), &dpp::utility::uptime::to_secs)
        .Func(_SC("ToMilliseconds"), &dpp::utility::uptime::to_msecs)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("IconHash"),
        Class< dpp::utility::iconhash >(vm, SqDppIconHash::Str)
        // Constructors
        .Ctor()
        .Ctor< const std::string & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppIconHash::Fn)
        .Func(_SC("_tostring"), &dpp::utility::iconhash::to_string)
        // Member Variables
        .Var(_SC("High"), &dpp::utility::iconhash::first)
        .Var(_SC("Low"), &dpp::utility::iconhash::second)
        // Member Methods
        .Func(_SC("Set"), &dpp::utility::iconhash::set)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("CachePolicy"),
        Class< DpCachePolicy >(vm, SqDppCachePolicy::Str)
        // Constructors
        .Ctor()
        .Ctor< SQInteger >()
        .Ctor< SQInteger, SQInteger >()
        .Ctor< SQInteger, SQInteger, SQInteger >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppCachePolicy::Fn)
        // Member Variables
        .Var(_SC("UserPolicy"), &DpCachePolicy::mUserPolicy)
        .Var(_SC("EmojiPolicy"), &DpCachePolicy::mEmojiPolicy)
        .Var(_SC("RolePolicy"), &DpCachePolicy::mRolePolicy)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Activity"),
        Class< DpActivity >(vm, SqDppActivity::Str)
        // Constructors
        .Ctor()
        .Ctor< SQInteger, StackStrF &, StackStrF &, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppActivity::Fn)
        // Member Properties
        .Prop(_SC("Name"), &DpActivity::GetName, &DpActivity::SetName)
        .Prop(_SC("State"), &DpActivity::GetState, &DpActivity::SetState)
        .Prop(_SC("URL"), &DpActivity::GetURL, &DpActivity::SetURL)
        .Prop(_SC("Type"), &DpActivity::GetType, &DpActivity::SetType)
        .Prop(_SC("CreatedAt"), &DpActivity::GetCreatedAt, &DpActivity::SetCreatedAt)
        .Prop(_SC("Start"), &DpActivity::GetStart, &DpActivity::SetStart)
        .Prop(_SC("End"), &DpActivity::GetEnd, &DpActivity::SetEnd)
        // Member Methods
        .Func(_SC("SetName"), &DpActivity::ApplyName)
        .Func(_SC("SetState"), &DpActivity::ApplyState)
        .Func(_SC("SetURL"), &DpActivity::ApplyURL)
        .Func(_SC("SetType"), &DpActivity::ApplyType)
        .Func(_SC("SetCreatedAt"), &DpActivity::ApplyCreatedAt)
        .Func(_SC("SetStart"), &DpActivity::ApplyStart)
        .Func(_SC("SetEnd"), &DpActivity::ApplyEnd)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Presence"),
        Class< DpPresence >(vm, SqDppPresence::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppPresence::Fn)
        // Member Properties
        .Prop(_SC("UserID"), &DpPresence::GetUserID, &DpPresence::SetUserID)
        .Prop(_SC("GuildID"), &DpPresence::GetGuildID, &DpPresence::SetGuildID)
        .Prop(_SC("Flags"), &DpPresence::GetFlags, &DpPresence::SetFlags)
        .Prop(_SC("ActivityCount"), &DpPresence::ActivityCount)
        .Prop(_SC("DesktopStatus"), &DpPresence::GetDesktopStatus)
        .Prop(_SC("WebStatus"), &DpPresence::GetWebStatus)
        .Prop(_SC("MobileStatus"), &DpPresence::GetMobileStatus)
        .Prop(_SC("Status"), &DpPresence::GetStatus)
        // Member Methods
        .Func(_SC("SetUserID"), &DpPresence::ApplyUserID)
        .Func(_SC("SetGuildID"), &DpPresence::ApplyGuildID)
        .Func(_SC("SetFlags"), &DpPresence::ApplyFlags)
        .Func(_SC("AddActivity"), &DpPresence::AddActivity)
        .Func(_SC("EachActivity"), &DpPresence::EachActivity)
        .Func(_SC("ClearActivities"), &DpPresence::ClearActivities)
        .Func(_SC("FilterActivities"), &DpPresence::FilterActivities)
        .Func(_SC("BuildJSON"), &DpPresence::BuildJSON)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceState"),
        Class< DpVoiceState, NoConstructor< DpVoiceState > >(vm, SqDppVoiceState::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppVoiceState::Fn)
        .Func(_SC("_tojson"), &DpVoiceState::BuildJSON)
        // Member Properties
        .Prop(_SC("Valid"), &DpVoiceState::IsValid)
        .Prop(_SC("GuildID"), &DpVoiceState::GetGuildID)
        .Prop(_SC("ChannelID"), &DpVoiceState::GetChannelID)
        .Prop(_SC("UserID"), &DpVoiceState::GetUserID)
        .Prop(_SC("SessionID"), &DpVoiceState::GetSessionID)
        .Prop(_SC("Flags"), &DpVoiceState::GetFlags, &DpVoiceState::SetFlags)
        .Prop(_SC("JSON"), &DpVoiceState::BuildJSON)
        .Prop(_SC("Deaf"), &DpVoiceState::IsDeaf)
        .Prop(_SC("Mute"), &DpVoiceState::IsMute)
        .Prop(_SC("SelfMute"), &DpVoiceState::IsSelfMute)
        .Prop(_SC("SelfDeaf"), &DpVoiceState::IsSelfDeaf)
        .Prop(_SC("SelfStream"), &DpVoiceState::SelfStream)
        .Prop(_SC("SelfVideo"), &DpVoiceState::SelfVideo)
        .Prop(_SC("Supressed"), &DpVoiceState::IsSupressed)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Guild"),
        Class< DpGuild, NoConstructor< DpGuild > >(vm, SqDppGuild::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuild::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpGuild::IsValid)
    );
}

// ------------------------------------------------------------------------------------------------
void Register_DPPConst(HSQUIRRELVM vm, Table & ns)
{
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqDiscordLogLevel"), Enumeration(vm)
        .Const(_SC("Trace"),        static_cast< SQInteger >(dpp::ll_trace))
        .Const(_SC("Debug"),        static_cast< SQInteger >(dpp::ll_debug))
        .Const(_SC("Info"),         static_cast< SQInteger >(dpp::ll_info))
        .Const(_SC("Warning"),      static_cast< SQInteger >(dpp::ll_warning))
        .Const(_SC("Error"),        static_cast< SQInteger >(dpp::ll_error))
        .Const(_SC("Critical"),     static_cast< SQInteger >(dpp::ll_critical))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqDiscordImageType"), Enumeration(vm)
        .Const(_SC("PNG"),          static_cast< SQInteger >(dpp::i_png))
        .Const(_SC("JPG"),          static_cast< SQInteger >(dpp::i_jpg))
        .Const(_SC("GIF"),          static_cast< SQInteger >(dpp::i_gif))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqDiscordCachePolicy"), Enumeration(vm)
        .Const(_SC("Aggressive"),   static_cast< SQInteger >(dpp::cp_aggressive))
        .Const(_SC("Lazy"),         static_cast< SQInteger >(dpp::cp_lazy))
        .Const(_SC("None"),         static_cast< SQInteger >(dpp::cp_none))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqDiscordClusterIntents"), Enumeration(vm)
        .Const(_SC("Guilds"),                   static_cast< SQInteger >(dpp::i_guilds))
        .Const(_SC("GuildMembers"),             static_cast< SQInteger >(dpp::i_guild_members))
        .Const(_SC("GuildBans"),                static_cast< SQInteger >(dpp::i_guild_bans))
        .Const(_SC("GuildEmojis"),              static_cast< SQInteger >(dpp::i_guild_emojis))
        .Const(_SC("GuildIntegrations"),        static_cast< SQInteger >(dpp::i_guild_integrations))
        .Const(_SC("GuildWebhooks"),            static_cast< SQInteger >(dpp::i_guild_webhooks))
        .Const(_SC("GuildInvites"),             static_cast< SQInteger >(dpp::i_guild_invites))
        .Const(_SC("GuildVoiceStates"),         static_cast< SQInteger >(dpp::i_guild_voice_states))
        .Const(_SC("GuildPresences"),           static_cast< SQInteger >(dpp::i_guild_presences))
        .Const(_SC("GuildMessages"),            static_cast< SQInteger >(dpp::i_guild_messages))
        .Const(_SC("GuildMessageReactions"),    static_cast< SQInteger >(dpp::i_guild_message_reactions))
        .Const(_SC("GuildMessageTyping"),       static_cast< SQInteger >(dpp::i_guild_message_typing))
        .Const(_SC("DirectMessages"),           static_cast< SQInteger >(dpp::i_direct_messages))
        .Const(_SC("DirectMessageReactions"),   static_cast< SQInteger >(dpp::i_direct_message_reactions))
        .Const(_SC("DirectMessageTyping"),      static_cast< SQInteger >(dpp::i_direct_message_typing))
        .Const(_SC("Default"),                  static_cast< SQInteger >(dpp::i_default_intents))
        .Const(_SC("Privileged"),               static_cast< SQInteger >(dpp::i_privileged_intents))
        .Const(_SC("All"),                      static_cast< SQInteger >(dpp::i_all_intents))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqDiscordPresenceFlags"), Enumeration(vm)
        .Const(_SC("DesktopOnline"),            static_cast< SQInteger >(dpp::p_desktop_online))
        .Const(_SC("DesktopDND"),               static_cast< SQInteger >(dpp::p_desktop_dnd))
        .Const(_SC("DesktopIdle"),              static_cast< SQInteger >(dpp::p_desktop_idle))
        .Const(_SC("WebWnline"),                static_cast< SQInteger >(dpp::p_web_online))
        .Const(_SC("WebDND"),                   static_cast< SQInteger >(dpp::p_web_dnd))
        .Const(_SC("WebIdle"),                  static_cast< SQInteger >(dpp::p_web_idle))
        .Const(_SC("MobileOnline"),             static_cast< SQInteger >(dpp::p_mobile_online))
        .Const(_SC("MobileDND"),                static_cast< SQInteger >(dpp::p_mobile_dnd))
        .Const(_SC("MobileIdle"),               static_cast< SQInteger >(dpp::p_mobile_idle))
        .Const(_SC("StatusOnline"),             static_cast< SQInteger >(dpp::p_status_online))
        .Const(_SC("StatusDND"),                static_cast< SQInteger >(dpp::p_status_dnd))
        .Const(_SC("StatusIdle"),               static_cast< SQInteger >(dpp::p_status_idle))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqDiscordPresenceStatus"), Enumeration(vm)
        .Const(_SC("Offline"),                  static_cast< SQInteger >(dpp::ps_offline))
        .Const(_SC("Online"),                   static_cast< SQInteger >(dpp::ps_online))
        .Const(_SC("DND"),                      static_cast< SQInteger >(dpp::ps_dnd))
        .Const(_SC("Idle"),                     static_cast< SQInteger >(dpp::ps_idle))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqDiscordDesktopStatusBits"), Enumeration(vm)
        .Const(_SC("ShiftDesktop"),             static_cast< SQInteger >(PF_SHIFT_DESKTOP))
        .Const(_SC("ShiftWeb"),                 static_cast< SQInteger >(PF_SHIFT_WEB))
        .Const(_SC("ShiftMobile"),              static_cast< SQInteger >(PF_SHIFT_MOBILE))
        .Const(_SC("ShiftMain"),                static_cast< SQInteger >(PF_SHIFT_MAIN))
        .Const(_SC("StatusMask"),               static_cast< SQInteger >(PF_STATUS_MASK))
        .Const(_SC("ClearDesktop"),             static_cast< SQInteger >(PF_CLEAR_DESKTOP))
        .Const(_SC("ClearWeb"),                 static_cast< SQInteger >(PF_CLEAR_WEB))
        .Const(_SC("ClearMobile"),              static_cast< SQInteger >(PF_CLEAR_MOBILE))
        .Const(_SC("ClearStatus"),              static_cast< SQInteger >(PF_CLEAR_STATUS))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqDiscordActivityType"), Enumeration(vm)
        .Const(_SC("Game"),                     static_cast< SQInteger >(dpp::at_game))
        .Const(_SC("Streaming"),                static_cast< SQInteger >(dpp::at_streaming))
        .Const(_SC("Listening"),                static_cast< SQInteger >(dpp::at_listening))
        .Const(_SC("Custom"),                   static_cast< SQInteger >(dpp::at_custom))
        .Const(_SC("Competing"),                static_cast< SQInteger >(dpp::at_competing))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqDiscordActivityFlags"), Enumeration(vm)
        .Const(_SC("Instance"),                 static_cast< SQInteger >(dpp::af_instance))
        .Const(_SC("Join"),                     static_cast< SQInteger >(dpp::af_join))
        .Const(_SC("Spectate"),                 static_cast< SQInteger >(dpp::af_spectate))
        .Const(_SC("JoinRequest"),              static_cast< SQInteger >(dpp::af_join_request))
        .Const(_SC("Sync"),                     static_cast< SQInteger >(dpp::af_sync))
        .Const(_SC("Play"),                     static_cast< SQInteger >(dpp::af_play))
    );
}

} // Namespace:: SqMod
