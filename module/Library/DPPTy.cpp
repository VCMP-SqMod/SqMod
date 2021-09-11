// ------------------------------------------------------------------------------------------------
#include "Library/DPPTy.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDppCachePolicy, _SC("SqDppCachePolicy"))
SQMOD_DECL_TYPENAME(SqDppIconHash, _SC("SqDppIconHash"))
SQMOD_DECL_TYPENAME(SqDppUptime, _SC("SqDppUptime"))
SQMOD_DECL_TYPENAME(SqDppActivity, _SC("SqDppActivity"))
SQMOD_DECL_TYPENAME(SqDppPresence, _SC("SqDppPresence"))
SQMOD_DECL_TYPENAME(SqDppVoiceState, _SC("SqDppVoiceState"))
SQMOD_DECL_TYPENAME(SqDppUser, _SC("SqDppUser"))
SQMOD_DECL_TYPENAME(SqDppGuildMember, _SC("SqDppGuildMember"))
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
        Class< DpActivity, NoCopy< DpActivity > >(vm, SqDppActivity::Str)
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
        Class< DpPresence, NoCopy< DpPresence > >(vm, SqDppPresence::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppPresence::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpPresence::IsValid)
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
        .Prop(_SC("JSON"), &DpVoiceState::BuildJSON)
        .Prop(_SC("GuildID"), &DpVoiceState::GetGuildID)
        .Prop(_SC("ChannelID"), &DpVoiceState::GetChannelID)
        .Prop(_SC("UserID"), &DpVoiceState::GetUserID)
        .Prop(_SC("SessionID"), &DpVoiceState::GetSessionID)
        .Prop(_SC("Flags"), &DpVoiceState::GetFlags, &DpVoiceState::SetFlags)
        .Prop(_SC("IsDeaf"), &DpVoiceState::IsDeaf)
        .Prop(_SC("IsMute"), &DpVoiceState::IsMute)
        .Prop(_SC("IsSelfMute"), &DpVoiceState::IsSelfMute)
        .Prop(_SC("IsSelfDeaf"), &DpVoiceState::IsSelfDeaf)
        .Prop(_SC("SelfStream"), &DpVoiceState::SelfStream)
        .Prop(_SC("SelfVideo"), &DpVoiceState::SelfVideo)
        .Prop(_SC("IsSupressed"), &DpVoiceState::IsSupressed)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("User"),
        Class< DpUser, NoConstructor< DpUser > >(vm, SqDppUser::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppUser::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpUser::IsValid)
        .Prop(_SC("Username"), &DpUser::GetUsername)
        .Prop(_SC("Discriminator"), &DpUser::GetDiscriminator)
        .Prop(_SC("Avatar"), &DpUser::GetAvatar)
        .Prop(_SC("Flags"), &DpUser::GetFlags)
        .Prop(_SC("RefCount"), &DpUser::GetRefCount)
        .Prop(_SC("AvatarURL"), &DpUser::GetAvatarURL)
        .Prop(_SC("IsBot"), &DpUser::IsBot)
        .Prop(_SC("IsSystem"), &DpUser::IsSystem)
        .Prop(_SC("IsMfaEnabled"), &DpUser::IsMfaEnabled)
        .Prop(_SC("IsVerified"), &DpUser::IsVerified)
        .Prop(_SC("HasNitroFull"), &DpUser::HasNitroFull)
        .Prop(_SC("HasNitroClassic"), &DpUser::HasNitroClassic)
        .Prop(_SC("IsDiscordEmployee"), &DpUser::IsDiscordEmployee)
        .Prop(_SC("IsPartneredOwner"), &DpUser::IsPartneredOwner)
        .Prop(_SC("HasHypesquadEvents"), &DpUser::HasHypesquadEvents)
        .Prop(_SC("IsBughunter1"), &DpUser::IsBughunter1)
        .Prop(_SC("IsHouseBravery"), &DpUser::IsHouseBravery)
        .Prop(_SC("IsHouseBrilliance"), &DpUser::IsHouseBrilliance)
        .Prop(_SC("IsHouseBalanace"), &DpUser::IsHouseBalanace)
        .Prop(_SC("IsEarlySupporter"), &DpUser::IsEarlySupporter)
        .Prop(_SC("IsTeamUser"), &DpUser::IsTeamUser)
        .Prop(_SC("IsBughunter2"), &DpUser::IsBughunter2)
        .Prop(_SC("IsVerifiedBot"), &DpUser::IsVerifiedBot)
        .Prop(_SC("IsVerifiedBotDev"), &DpUser::IsVerifiedBotDev)
        .Prop(_SC("IsCertifiedDoderator"), &DpUser::IsCertifiedDoderator)
        .Prop(_SC("HasAnimatedIcon"), &DpUser::HasAnimatedIcon)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildMember"),
        Class< DpGuildMember, NoConstructor< DpGuildMember > >(vm, SqDppGuildMember::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildMember::Fn)
        .Func(_SC("_tojson"), &DpGuildMember::BuildJSON)
        // Member Properties
        .Prop(_SC("Valid"), &DpGuildMember::IsValid)
        .Prop(_SC("JSON"), &DpGuildMember::BuildJSON)
        .Prop(_SC("Nickname"), &DpGuildMember::GetNickname)
        .Prop(_SC("GuildID"), &DpGuildMember::GetGuildID)
        .Prop(_SC("UserID"), &DpGuildMember::GetUserID)
        .Prop(_SC("JoinedAt"), &DpGuildMember::GetJoinedAt)
        .Prop(_SC("PremiumSince"), &DpGuildMember::GetPremiumSince)
        .Prop(_SC("Flags"), &DpGuildMember::GetFlags)
        .Prop(_SC("IsDeaf"), &DpGuildMember::IsDeaf)
        .Prop(_SC("IsMuted"), &DpGuildMember::IsMuted)
        .Prop(_SC("IsPending"), &DpGuildMember::IsPending)
        // Member Methods
        .Func(_SC("GetRoles"), &DpGuildMember::GetRoles)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Guild"),
        Class< DpGuild, NoConstructor< DpGuild > >(vm, SqDppGuild::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuild::Fn)
        .Func(_SC("_tojson"), &DpGuild::BuildJSON)
        // Member Properties
        .Prop(_SC("Valid"), &DpGuild::IsValid)
        .Prop(_SC("JSON"), &DpGuild::BuildJSON)
        .Prop(_SC("ShardID"), &DpGuild::GetShardID)
        .Prop(_SC("Flags"), &DpGuild::GetFlags)
        .Prop(_SC("Name"), &DpGuild::GetName)
        .Prop(_SC("Description"), &DpGuild::GetDescription)
        .Prop(_SC("VanityUrlCode"), &DpGuild::GetVanityUrlCode)
        .Prop(_SC("Icon"), &DpGuild::GetIcon)
        .Prop(_SC("Splash"), &DpGuild::GetSplash)
        .Prop(_SC("DiscoverySplash"), &DpGuild::GetDiscoverySplash)
        .Prop(_SC("OwnerID"), &DpGuild::GetOwnerID)
        .Prop(_SC("VoiceRegion"), &DpGuild::GetVoiceRegion)
        .Prop(_SC("AfkChannelID"), &DpGuild::GetAfkChannelID)
        .Prop(_SC("AfkTimeout"), &DpGuild::GetAfkTimeout)
        .Prop(_SC("WidgetChannelID"), &DpGuild::GetWidgetChannelID)
        .Prop(_SC("VerificationLevel"), &DpGuild::GetVerificationLevel)
        .Prop(_SC("DefaultMessageNotifications"), &DpGuild::GetDefaultMessageNotifications)
        .Prop(_SC("ExplicitContentFilter"), &DpGuild::GetExplicitContentFilter)
        .Prop(_SC("MfaLevel"), &DpGuild::GetMfaLevel)
        .Prop(_SC("ApplicationID"), &DpGuild::GetApplicationID)
        .Prop(_SC("SystemChannelID"), &DpGuild::GetSystemChannelID)
        .Prop(_SC("RulesChannelID"), &DpGuild::GetRulesChannelID)
        .Prop(_SC("MemberCount"), &DpGuild::GetMemberCount)
        .Prop(_SC("Banner"), &DpGuild::GetBanner)
        .Prop(_SC("PremiumTier"), &DpGuild::GetPremiumTier)
        .Prop(_SC("PremiumSubscriptionCount"), &DpGuild::GetPremiumSubscriptionCount)
        .Prop(_SC("PublicUpdatesChannelID"), &DpGuild::GetPublicUpdatesChannelID)
        .Prop(_SC("MaxVideoChannelUsers"), &DpGuild::GetMaxVideoChannelUsers)
        .Prop(_SC("IsLarge"), &DpGuild::IsLarge)
        .Prop(_SC("IsUnavailable"), &DpGuild::IsUnavailable)
        .Prop(_SC("WidgetEnabled"), &DpGuild::WidgetEnabled)
        .Prop(_SC("HasInviteSplash"), &DpGuild::HasInviteSplash)
        .Prop(_SC("HasVipRegions"), &DpGuild::HasVipRegions)
        .Prop(_SC("HasVanityURL"), &DpGuild::HasVanityURL)
        .Prop(_SC("IsVerified"), &DpGuild::IsVerified)
        .Prop(_SC("IsPartnered"), &DpGuild::IsPartnered)
        .Prop(_SC("IsCommunity"), &DpGuild::IsCommunity)
        .Prop(_SC("HasCommerce"), &DpGuild::HasCommerce)
        .Prop(_SC("HasNews"), &DpGuild::HasNews)
        .Prop(_SC("IsDiscoverable"), &DpGuild::IsDiscoverable)
        .Prop(_SC("IsFeatureable"), &DpGuild::IsFeatureable)
        .Prop(_SC("HasAnimatedIcon"), &DpGuild::HasAnimatedIcon)
        .Prop(_SC("BasBanner"), &DpGuild::BasBanner)
        .Prop(_SC("WelcomeScreenEnabled"), &DpGuild::WelcomeScreenEnabled)
        .Prop(_SC("HasMemberVerificationGate"), &DpGuild::HasMemberVerificationGate)
        .Prop(_SC("IsPreviewEnabled"), &DpGuild::IsPreviewEnabled)
        .Prop(_SC("HasAnimatedIconHash"), &DpGuild::HasAnimatedIconHash)
        // Member Methods
        .Func(_SC("BuildJSON"), &DpGuild::BuildJSON_)
        .Func(_SC("GetRoles"), &DpGuild::GetRoles)
        .Func(_SC("GetChannels"), &DpGuild::GetChannels)
        .Func(_SC("GetThreads"), &DpGuild::GetThreads)
        .Func(_SC("GetMembers"), &DpGuild::GetMembers)
        .Func(_SC("GetVoiceMembers"), &DpGuild::GetVoiceMembers)
        .Func(_SC("GetEmojis"), &DpGuild::GetEmojis)
        .Func(_SC("RehashMembers"), &DpGuild::RehashMembers)
        .Func(_SC("ConnectMemberVoice"), &DpGuild::ConnectMemberVoice)
    );
}

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpLogLevelEnum[] = {
    {_SC("Trace"),      static_cast< SQInteger >(dpp::ll_trace)},
    {_SC("Debug"),      static_cast< SQInteger >(dpp::ll_debug)},
    {_SC("Info"),       static_cast< SQInteger >(dpp::ll_info)},
    {_SC("Warning"),    static_cast< SQInteger >(dpp::ll_warning)},
    {_SC("Error"),      static_cast< SQInteger >(dpp::ll_error)},
    {_SC("Critical"),   static_cast< SQInteger >(dpp::ll_critical)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpImageTypeEnum[] = {
    {_SC("PNG"),    static_cast< SQInteger >(dpp::i_png)},
    {_SC("JPG"),    static_cast< SQInteger >(dpp::i_jpg)},
    {_SC("GIF"),    static_cast< SQInteger >(dpp::i_gif)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpCachePolicyEnum[] = {
    {_SC("Aggressive"),   static_cast< SQInteger >(dpp::cp_aggressive)},
    {_SC("Lazy"),         static_cast< SQInteger >(dpp::cp_lazy)},
    {_SC("None"),         static_cast< SQInteger >(dpp::cp_none)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpClusterIntentsEnum[] = {
    {_SC("Guilds"),                   static_cast< SQInteger >(dpp::i_guilds)},
    {_SC("GuildMembers"),             static_cast< SQInteger >(dpp::i_guild_members)},
    {_SC("GuildBans"),                static_cast< SQInteger >(dpp::i_guild_bans)},
    {_SC("GuildEmojis"),              static_cast< SQInteger >(dpp::i_guild_emojis)},
    {_SC("GuildIntegrations"),        static_cast< SQInteger >(dpp::i_guild_integrations)},
    {_SC("GuildWebhooks"),            static_cast< SQInteger >(dpp::i_guild_webhooks)},
    {_SC("GuildInvites"),             static_cast< SQInteger >(dpp::i_guild_invites)},
    {_SC("GuildVoiceStates"),         static_cast< SQInteger >(dpp::i_guild_voice_states)},
    {_SC("GuildPresences"),           static_cast< SQInteger >(dpp::i_guild_presences)},
    {_SC("GuildMessages"),            static_cast< SQInteger >(dpp::i_guild_messages)},
    {_SC("GuildMessageReactions"),    static_cast< SQInteger >(dpp::i_guild_message_reactions)},
    {_SC("GuildMessageTyping"),       static_cast< SQInteger >(dpp::i_guild_message_typing)},
    {_SC("DirectMessages"),           static_cast< SQInteger >(dpp::i_direct_messages)},
    {_SC("DirectMessageReactions"),   static_cast< SQInteger >(dpp::i_direct_message_reactions)},
    {_SC("DirectMessageTyping"),      static_cast< SQInteger >(dpp::i_direct_message_typing)},
    {_SC("Default"),                  static_cast< SQInteger >(dpp::i_default_intents)},
    {_SC("Privileged"),               static_cast< SQInteger >(dpp::i_privileged_intents)},
    {_SC("All"),                      static_cast< SQInteger >(dpp::i_all_intents)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpPresenceFlagsEnum[] = {
    {_SC("DesktopOnline"),  static_cast< SQInteger >(dpp::p_desktop_online)},
    {_SC("DesktopDND"),     static_cast< SQInteger >(dpp::p_desktop_dnd)},
    {_SC("DesktopIdle"),    static_cast< SQInteger >(dpp::p_desktop_idle)},
    {_SC("WebWnline"),      static_cast< SQInteger >(dpp::p_web_online)},
    {_SC("WebDND"),         static_cast< SQInteger >(dpp::p_web_dnd)},
    {_SC("WebIdle"),        static_cast< SQInteger >(dpp::p_web_idle)},
    {_SC("MobileOnline"),   static_cast< SQInteger >(dpp::p_mobile_online)},
    {_SC("MobileDND"),      static_cast< SQInteger >(dpp::p_mobile_dnd)},
    {_SC("MobileIdle"),     static_cast< SQInteger >(dpp::p_mobile_idle)},
    {_SC("StatusOnline"),   static_cast< SQInteger >(dpp::p_status_online)},
    {_SC("StatusDND"),      static_cast< SQInteger >(dpp::p_status_dnd)},
    {_SC("StatusIdle"),     static_cast< SQInteger >(dpp::p_status_idle)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpPresenceStatusEnum[] = {
    {_SC("Offline"),    static_cast< SQInteger >(dpp::ps_offline)},
    {_SC("Online"),     static_cast< SQInteger >(dpp::ps_online)},
    {_SC("DND"),        static_cast< SQInteger >(dpp::ps_dnd)},
    {_SC("Idle"),       static_cast< SQInteger >(dpp::ps_idle)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpDesktopStatusBitsEnum[] = {
    {_SC("ShiftDesktop"),   static_cast< SQInteger >(PF_SHIFT_DESKTOP)},
    {_SC("ShiftWeb"),       static_cast< SQInteger >(PF_SHIFT_WEB)},
    {_SC("ShiftMobile"),    static_cast< SQInteger >(PF_SHIFT_MOBILE)},
    {_SC("ShiftMain"),      static_cast< SQInteger >(PF_SHIFT_MAIN)},
    {_SC("StatusMask"),     static_cast< SQInteger >(PF_STATUS_MASK)},
    {_SC("ClearDesktop"),   static_cast< SQInteger >(PF_CLEAR_DESKTOP)},
    {_SC("ClearWeb"),       static_cast< SQInteger >(PF_CLEAR_WEB)},
    {_SC("ClearMobile"),    static_cast< SQInteger >(PF_CLEAR_MOBILE)},
    {_SC("ClearStatus"),    static_cast< SQInteger >(PF_CLEAR_STATUS)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpActivityTypeEnum[] = {
    {_SC("Game"),       static_cast< SQInteger >(dpp::at_game)},
    {_SC("Streaming"),  static_cast< SQInteger >(dpp::at_streaming)},
    {_SC("Listening"),  static_cast< SQInteger >(dpp::at_listening)},
    {_SC("Custom"),     static_cast< SQInteger >(dpp::at_custom)},
    {_SC("Competing"),  static_cast< SQInteger >(dpp::at_competing)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpActivityFlagsEnum[] = {
    {_SC("Instance"),       static_cast< SQInteger >(dpp::af_instance)},
    {_SC("Join"),           static_cast< SQInteger >(dpp::af_join)},
    {_SC("Spectate"),       static_cast< SQInteger >(dpp::af_spectate)},
    {_SC("JoinRequest"),    static_cast< SQInteger >(dpp::af_join_request)},
    {_SC("Sync"),           static_cast< SQInteger >(dpp::af_sync)},
    {_SC("Play"),           static_cast< SQInteger >(dpp::af_play)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpVoiceStateFlagsEnum[] = {
    {_SC("Deaf"),       static_cast< SQInteger >(dpp::vs_deaf)},
    {_SC("Mute"),       static_cast< SQInteger >(dpp::vs_mute)},
    {_SC("SelfMute"),   static_cast< SQInteger >(dpp::vs_self_mute)},
    {_SC("SelfDeaf"),   static_cast< SQInteger >(dpp::vs_self_deaf)},
    {_SC("SelfStream"), static_cast< SQInteger >(dpp::vs_self_stream)},
    {_SC("SelfVideo"),  static_cast< SQInteger >(dpp::vs_self_video)},
    {_SC("Supress"),    static_cast< SQInteger >(dpp::vs_supress)},
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpUserFlagsEnum[] = {
    {_SC("Bot"),                static_cast< SQInteger >(dpp::u_bot)},
    {_SC("System"),             static_cast< SQInteger >(dpp::u_system)},
    {_SC("MfaEnabled"),         static_cast< SQInteger >(dpp::u_mfa_enabled)},
    {_SC("Verified"),           static_cast< SQInteger >(dpp::u_verified)},
    {_SC("NitroFull"),          static_cast< SQInteger >(dpp::u_nitro_full)},
    {_SC("NitroClassic"),       static_cast< SQInteger >(dpp::u_nitro_classic)},
    {_SC("DiscordEmployee"),    static_cast< SQInteger >(dpp::u_discord_employee)},
    {_SC("PartneredOwner"),     static_cast< SQInteger >(dpp::u_partnered_owner)},
    {_SC("HypesquadEvents"),    static_cast< SQInteger >(dpp::u_hypesquad_events)},
    {_SC("Bughunter1"),         static_cast< SQInteger >(dpp::u_bughunter_1)},
    {_SC("HouseBravery"),       static_cast< SQInteger >(dpp::u_house_bravery)},
    {_SC("HouseBrilliance"),    static_cast< SQInteger >(dpp::u_house_brilliance)},
    {_SC("HouseBalanace"),      static_cast< SQInteger >(dpp::u_house_balanace)},
    {_SC("EarlySupporter"),     static_cast< SQInteger >(dpp::u_early_supporter)},
    {_SC("TeamUser"),           static_cast< SQInteger >(dpp::u_team_user)},
    {_SC("Bughunter2"),         static_cast< SQInteger >(dpp::u_bughunter_2)},
    {_SC("VerifiedBot"),        static_cast< SQInteger >(dpp::u_verified_bot)},
    {_SC("VerifiedBotDev"),     static_cast< SQInteger >(dpp::u_verified_bot_dev)},
    {_SC("AnimatedIcon"),       static_cast< SQInteger >(dpp::u_animated_icon)},
    {_SC("CertifiedModerator"), static_cast< SQInteger >(dpp::u_certified_moderator)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpRegionEnum[] = {
    {_SC("Brazil"),         static_cast< SQInteger >(dpp::r_brazil)},
    {_SC("CentralEurope"),  static_cast< SQInteger >(dpp::r_central_europe)},
    {_SC("HongKong"),       static_cast< SQInteger >(dpp::r_hong_kong)},
    {_SC("India"),          static_cast< SQInteger >(dpp::r_india)},
    {_SC("Japan"),          static_cast< SQInteger >(dpp::r_japan)},
    {_SC("Russia"),         static_cast< SQInteger >(dpp::r_russia)},
    {_SC("Singapore"),      static_cast< SQInteger >(dpp::r_singapore)},
    {_SC("SouthAfrica"),    static_cast< SQInteger >(dpp::r_south_africa)},
    {_SC("Sydney"),         static_cast< SQInteger >(dpp::r_sydney)},
    {_SC("UsCentral"),      static_cast< SQInteger >(dpp::r_us_central)},
    {_SC("UsEast"),         static_cast< SQInteger >(dpp::r_us_east)},
    {_SC("UsSouth"),        static_cast< SQInteger >(dpp::r_us_south)},
    {_SC("UsWest"),         static_cast< SQInteger >(dpp::r_us_west)},
    {_SC("WesternEurope"),  static_cast< SQInteger >(dpp::r_western_europe)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpGuildFlagsEnum[] = {
    {_SC("Large"),                  static_cast< SQInteger >(dpp::g_large)},
    {_SC("Unavailable"),            static_cast< SQInteger >(dpp::g_unavailable)},
    {_SC("WidgetEnabled"),          static_cast< SQInteger >(dpp::g_widget_enabled)},
    {_SC("InviteSplash"),           static_cast< SQInteger >(dpp::g_invite_splash)},
    {_SC("VipRegions"),             static_cast< SQInteger >(dpp::g_vip_regions)},
    {_SC("VanityURL"),              static_cast< SQInteger >(dpp::g_vanity_url)},
    {_SC("Verified"),               static_cast< SQInteger >(dpp::g_verified)},
    {_SC("Partnered"),              static_cast< SQInteger >(dpp::g_partnered)},
    {_SC("Community"),              static_cast< SQInteger >(dpp::g_community)},
    {_SC("Commerce"),               static_cast< SQInteger >(dpp::g_commerce)},
    {_SC("News"),                   static_cast< SQInteger >(dpp::g_news)},
    {_SC("Discoverable"),           static_cast< SQInteger >(dpp::g_discoverable)},
    {_SC("Featureable"),            static_cast< SQInteger >(dpp::g_featureable)},
    {_SC("AnimatedIcon"),           static_cast< SQInteger >(dpp::g_animated_icon)},
    {_SC("Banner"),                 static_cast< SQInteger >(dpp::g_banner)},
    {_SC("WelcomeScreenEnabled"),   static_cast< SQInteger >(dpp::g_welcome_screen_enabled)},
    {_SC("MemberVerificationGate"), static_cast< SQInteger >(dpp::g_member_verification_gate)},
    {_SC("PreviewEnabled"),         static_cast< SQInteger >(dpp::g_preview_enabled)},
    {_SC("NoJoinNotifications"),    static_cast< SQInteger >(dpp::g_no_join_notifications)},
    {_SC("NoBoostNotifications"),   static_cast< SQInteger >(dpp::g_no_boost_notifications)},
    {_SC("HasAnimatedIcon"),        static_cast< SQInteger >(dpp::g_has_animated_icon)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpGuildMemberFlagsEnum[] = {
    {_SC("Deaf"),       static_cast< SQInteger >(dpp::gm_deaf)},
    {_SC("Mute"),       static_cast< SQInteger >(dpp::gm_mute)},
    {_SC("Pending"),    static_cast< SQInteger >(dpp::gm_pending)}
};
/*
// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpEnum[] = {

};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpEnum[] = {

};
*/
// ------------------------------------------------------------------------------------------------
static const EnumElements g_EnumList[] = {
    {_SC("SqDiscordLogLevel"),              g_DpLogLevelEnum},
    {_SC("SqDiscordImageType"),             g_DpImageTypeEnum},
    {_SC("SqDiscordCachePolicy"),           g_DpCachePolicyEnum},
    {_SC("SqDiscordClusterIntents"),        g_DpClusterIntentsEnum},
    {_SC("SqDiscordPresenceFlags"),         g_DpPresenceFlagsEnum},
    {_SC("SqDiscordPresenceStatus"),        g_DpPresenceStatusEnum},
    {_SC("SqDiscordDesktopStatusBits"),     g_DpDesktopStatusBitsEnum},
    {_SC("SqDiscordActivityType"),          g_DpActivityTypeEnum},
    {_SC("SqDiscordActivityFlags"),         g_DpActivityFlagsEnum},
    {_SC("SqDiscordVoiceStateFlags"),       g_DpVoiceStateFlagsEnum},
    {_SC("SqDiscordUserFlags"),             g_DpUserFlagsEnum},
    {_SC("SqDiscordRegion"),                g_DpRegionEnum},
    {_SC("SqDiscordGuildFlags"),            g_DpGuildFlagsEnum},
    {_SC("SqDiscordGuildMemberFlags"),      g_DpGuildMemberFlagsEnum}
};

// ------------------------------------------------------------------------------------------------
void Register_DPPConst(HSQUIRRELVM vm, Table &)
{
    RegisterEnumerations(vm, g_EnumList);
}

} // Namespace:: SqMod
