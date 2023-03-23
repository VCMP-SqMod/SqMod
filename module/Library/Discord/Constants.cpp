// ------------------------------------------------------------------------------------------------
#include "Library/Discord/Constants.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const std::array< const char *, static_cast< size_t >(DpEventID::Max) > DpEventID::NAME{
  "VoiceStateUpdate",
  "VoiceClientDisconnect",
  "VoiceClientSpeaking",
  "Log",
  "GuildJoinRequestDelete",
  "InteractionCreate",
  "SlashCommand",
  "ButtonClick",
  "AutoComplete",
  "SelectClick",
  "MessageContextMenu",
  "UserContextMenu",
  "FormSubmit",
  "GuildDelete",
  "ChannelDelete",
  "ChannelUpdate",
  "Ready",
  "MessageDelete",
  "GuildMemberRemove",
  "Resumed",
  "GuildRoleCreate",
  "TypingStart",
  "MessageReactionAdd",
  "GuildMembersChunk",
  "MessageReactionRemove",
  "GuildCreate",
  "ChannelCreate",
  "MessageReactionRemoveEmoji",
  "MessageDeleteDulk",
  "GuildRoleUpdate",
  "GuildRoleDelete",
  "ChannelPinsUpdate",
  "MessageReactionRemoveAll",
  "VoiceServerUpdate",
  "GuildEmojisUpdate",
  "GuildStickersUpdate",
  "PresenceUpdate",
  "WebhooksUpdate",
  "AutomodRuleCreate",
  "AutomodRuleUpdate",
  "AutomodRuleDelete",
  "AutomodRuleExecute",
  "GuildMemberAdd",
  "InviteDelete",
  "GuildUpdate",
  "GuildIntegrationsUpdate",
  "GuildMemberUpdate",
  "InviteCreate",
  "MessageUpdate",
  "UserUpdate",
  "MessageCreate",
  "GuildBanAdd",
  "GuildBanRemove",
  "IntegrationCreate",
  "IntegrationUpdate",
  "IntegrationDelete",
  "ThreadCreate",
  "ThreadUpdate",
  "ThreadDelete",
  "ThreadListSync",
  "ThreadMemberUpdate",
  "ThreadMembersUpdate",
  "GuildScheduledEventCreate",
  "GuildScheduledEventUpdate",
  "GuildScheduledEventDelete",
  "GuildScheduledEventUserAdd",
  "GuildScheduledEventUserRemove",
  "VoiceBufferSend",
  "VoiceUserTalking",
  "VoiceReady",
  "VoiceReceive",
  "VoiceReceiveCombined",
  "VoiceTrackMarker",
  "StageInstanceCreate",
  "StageInstanceUpdate",
  "StageInstanceDelete",
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpLogLevelEnum[] = {
    {_SC("Trace"),      static_cast< SQInteger >(dpp::ll_trace)},
    {_SC("Debug"),      static_cast< SQInteger >(dpp::ll_debug)},
    {_SC("Info"),       static_cast< SQInteger >(dpp::ll_info)},
    {_SC("Warning"),    static_cast< SQInteger >(dpp::ll_warning)},
    {_SC("Error"),      static_cast< SQInteger >(dpp::ll_error)},
    {_SC("Critical"),   static_cast< SQInteger >(dpp::ll_critical)},
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpVoiceStateFlagsEnum[] = {
    {_SC("Deaf"),       static_cast< SQInteger >(dpp::vs_deaf)},
    {_SC("Mute"),       static_cast< SQInteger >(dpp::vs_mute)},
    {_SC("SelfMute"),   static_cast< SQInteger >(dpp::vs_self_mute)},
    {_SC("SelfDeaf"),   static_cast< SQInteger >(dpp::vs_self_deaf)},
    {_SC("SelfStream"), static_cast< SQInteger >(dpp::vs_self_stream)},
    {_SC("SelfVideo"),  static_cast< SQInteger >(dpp::vs_self_video)},
    {_SC("Suppress"),   static_cast< SQInteger >(dpp::vs_suppress)},
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpEmojiFlagsEnum[] = {
    {_SC("RequireColons"),      static_cast< SQInteger >(dpp::e_require_colons)},
    {_SC("Managed"),            static_cast< SQInteger >(dpp::e_managed)},
    {_SC("Animated"),           static_cast< SQInteger >(dpp::e_animated)},
    {_SC("Available"),          static_cast< SQInteger >(dpp::e_available)},
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpPresenceFlagsEnum[] = {
    {_SC("DesktopOnline"),      static_cast< SQInteger >(dpp::p_desktop_online)},
    {_SC("DesktopDND"),         static_cast< SQInteger >(dpp::p_desktop_dnd)},
    {_SC("DesktopIdle"),        static_cast< SQInteger >(dpp::p_desktop_idle)},
    {_SC("WebOnline"),          static_cast< SQInteger >(dpp::p_web_online)},
    {_SC("WebDND"),             static_cast< SQInteger >(dpp::p_web_dnd)},
    {_SC("WebIdle"),            static_cast< SQInteger >(dpp::p_web_idle)},
    {_SC("MobileOnline"),       static_cast< SQInteger >(dpp::p_mobile_online)},
    {_SC("MobileDND"),          static_cast< SQInteger >(dpp::p_mobile_dnd)},
    {_SC("MobileIdle"),         static_cast< SQInteger >(dpp::p_mobile_idle)},
    {_SC("StatusOnline"),       static_cast< SQInteger >(dpp::p_status_online)},
    {_SC("StatusDND"),          static_cast< SQInteger >(dpp::p_status_dnd)},
    {_SC("StatusIdle"),         static_cast< SQInteger >(dpp::p_status_idle)},
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpPresenceStatusEnum[] = {
    {_SC("Offline"),            static_cast< SQInteger >(dpp::ps_offline)},
    {_SC("Online"),             static_cast< SQInteger >(dpp::ps_online)},
    {_SC("DND"),                static_cast< SQInteger >(dpp::ps_dnd)},
    {_SC("Idle"),               static_cast< SQInteger >(dpp::ps_idle)},
    // Bit shift for desktop status
    {_SC("ShiftDesktop"),       static_cast< SQInteger >(PF_SHIFT_DESKTOP)},
    {_SC("ShiftWeb"),           static_cast< SQInteger >(PF_SHIFT_WEB)},
    {_SC("ShiftMobile"),        static_cast< SQInteger >(PF_SHIFT_MOBILE)},
    {_SC("ShiftMain"),          static_cast< SQInteger >(PF_SHIFT_MAIN)},
    {_SC("StatusMask"),         static_cast< SQInteger >(PF_STATUS_MASK)},
    {_SC("ClearDesktop"),       static_cast< SQInteger >(PF_CLEAR_DESKTOP)},
    {_SC("ClearWeb"),           static_cast< SQInteger >(PF_CLEAR_WEB)},
    {_SC("ClearMobile"),        static_cast< SQInteger >(PF_CLEAR_MOBILE)},
    {_SC("ClearStatus"),        static_cast< SQInteger >(PF_CLEAR_STATUS)},
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpActivityTypeEnum[] = {
    {_SC("Game"),               static_cast< SQInteger >(dpp::at_game)},
    {_SC("Streaming"),          static_cast< SQInteger >(dpp::at_streaming)},
    {_SC("Listening"),          static_cast< SQInteger >(dpp::at_listening)},
    {_SC("Watching"),           static_cast< SQInteger >(dpp::at_watching)},
    {_SC("Custom"),             static_cast< SQInteger >(dpp::at_custom)},
    {_SC("Competing"),          static_cast< SQInteger >(dpp::at_competing)},
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpActivityFlagsEnum[] = {
    {_SC("Instance"),                   static_cast< SQInteger >(dpp::af_instance)},
    {_SC("Join"),                       static_cast< SQInteger >(dpp::af_join)},
    {_SC("Spectate"),                   static_cast< SQInteger >(dpp::af_spectate)},
    {_SC("JoinRequest"),                static_cast< SQInteger >(dpp::af_join_request)},
    {_SC("Sync"),                       static_cast< SQInteger >(dpp::af_sync)},
    {_SC("Play"),                       static_cast< SQInteger >(dpp::af_play)},
    {_SC("PartyPrivacyFriends"),        static_cast< SQInteger >(dpp::af_party_privacy_friends)},
    {_SC("PartyPrivacyVoiceChannel"),   static_cast< SQInteger >(dpp::af_party_privacy_voice_channel)},
    {_SC("Embedded"),                   static_cast< SQInteger >(dpp::af_embedded)},
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpRegionEnum[] = {
    {_SC("Brazil"),                     static_cast< SQInteger >(dpp::r_brazil)},
    {_SC("CentralEurope"),              static_cast< SQInteger >(dpp::r_central_europe)},
    {_SC("HongKong"),                   static_cast< SQInteger >(dpp::r_hong_kong)},
    {_SC("India"),                      static_cast< SQInteger >(dpp::r_india)},
    {_SC("Japan"),                      static_cast< SQInteger >(dpp::r_japan)},
    {_SC("Russia"),                     static_cast< SQInteger >(dpp::r_russia)},
    {_SC("Singapore"),                  static_cast< SQInteger >(dpp::r_singapore)},
    {_SC("SouthAfrica"),                static_cast< SQInteger >(dpp::r_south_africa)},
    {_SC("Sydney"),                     static_cast< SQInteger >(dpp::r_sydney)},
    {_SC("UsCentral"),                  static_cast< SQInteger >(dpp::r_us_central)},
    {_SC("UsEast"),                     static_cast< SQInteger >(dpp::r_us_east)},
    {_SC("UsSouth"),                    static_cast< SQInteger >(dpp::r_us_south)},
    {_SC("UsWest"),                     static_cast< SQInteger >(dpp::r_us_west)},
    {_SC("WesternEurope"),              static_cast< SQInteger >(dpp::r_western_europe)},
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpGuildFlagsEnum[] = {
    {_SC("Large"),                      static_cast< SQInteger >(dpp::g_large)},
    {_SC("Unavailable"),                static_cast< SQInteger >(dpp::g_unavailable)},
    {_SC("WidgetEnabled"),              static_cast< SQInteger >(dpp::g_widget_enabled)},
    {_SC("InviteSplash"),               static_cast< SQInteger >(dpp::g_invite_splash)},
    {_SC("VipRegions"),                 static_cast< SQInteger >(dpp::g_vip_regions)},
    {_SC("VanityURL"),                  static_cast< SQInteger >(dpp::g_vanity_url)},
    {_SC("Verified"),                   static_cast< SQInteger >(dpp::g_verified)},
    {_SC("Partnered"),                  static_cast< SQInteger >(dpp::g_partnered)},
    {_SC("Community"),                  static_cast< SQInteger >(dpp::g_community)},
    {_SC("RoleSubscriptionEnabled"),    static_cast< SQInteger >(dpp::g_role_subscription_enabled)},
    {_SC("News"),                       static_cast< SQInteger >(dpp::g_news)},
    {_SC("Discoverable"),               static_cast< SQInteger >(dpp::g_discoverable)},
    {_SC("Featureable"),                static_cast< SQInteger >(dpp::g_featureable)},
    {_SC("AnimatedIcon"),               static_cast< SQInteger >(dpp::g_animated_icon)},
    {_SC("Banner"),                     static_cast< SQInteger >(dpp::g_banner)},
    {_SC("WelcomeScreenEnabled"),       static_cast< SQInteger >(dpp::g_welcome_screen_enabled)},
    {_SC("MemberVerificationGate"),     static_cast< SQInteger >(dpp::g_member_verification_gate)},
    {_SC("PreviewEnabled"),             static_cast< SQInteger >(dpp::g_preview_enabled)},
    {_SC("NoJoinNotifications"),        static_cast< SQInteger >(dpp::g_no_join_notifications)},
    {_SC("NoBoostNotifications"),       static_cast< SQInteger >(dpp::g_no_boost_notifications)},
    {_SC("HasAnimatedIcon"),            static_cast< SQInteger >(dpp::g_has_animated_icon)},
    {_SC("HasAnimatedBanner"),          static_cast< SQInteger >(dpp::g_has_animated_banner)},
    {_SC("NoSetupTips"),                static_cast< SQInteger >(dpp::g_no_setup_tips)},
    {_SC("NoStickerGreeting"),          static_cast< SQInteger >(dpp::g_no_sticker_greeting)},
    {_SC("MonetizationEnabled"),        static_cast< SQInteger >(dpp::g_monetization_enabled)},
    {_SC("MoreStickers"),               static_cast< SQInteger >(dpp::g_more_stickers)},
    {_SC("CreatorStorePageEnabled"),    static_cast< SQInteger >(dpp::g_creator_store_page_enabled)},
    {_SC("RoleIcons"),                  static_cast< SQInteger >(dpp::g_role_icons)},
    {_SC("SevenDayThreadArchive"),      static_cast< SQInteger >(dpp::g_seven_day_thread_archive)},
    {_SC("ThreeDayThreadArchive"),      static_cast< SQInteger >(dpp::g_three_day_thread_archive)},
    {_SC("TicketedEvents"),             static_cast< SQInteger >(dpp::g_ticketed_events)},
    {_SC("ChannelBanners"),             static_cast< SQInteger >(dpp::g_channel_banners)},
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpGuildFlagsExtraEnum[] = {
    {_SC("PremiumProgressBarEnabled"),              static_cast< SQInteger >(dpp::g_premium_progress_bar_enabled)},
    {_SC("AnimatedBanner"),                         static_cast< SQInteger >(dpp::g_animated_banner)},
    {_SC("AutoModeration"),                         static_cast< SQInteger >(dpp::g_auto_moderation)},
    {_SC("InvitesDisabled"),                        static_cast< SQInteger >(dpp::g_invites_disabled)},
    {_SC("DeveloperSupportServer"),                 static_cast< SQInteger >(dpp::g_developer_support_server)},
    {_SC("NoRoleSubscriptionNotifications"),        static_cast< SQInteger >(dpp::g_no_role_subscription_notifications)},
    {_SC("NoRoleSubscriptionNotificationReplies"),  static_cast< SQInteger >(dpp::g_no_role_subscription_notification_replies)},
    {_SC("RoleSubscriptionsAvailableForPurchase"),  static_cast< SQInteger >(dpp::g_role_subscriptions_available_for_purchase)},

};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpGuildMemberFlagsEnum[] = {
    {_SC("Deaf"),               static_cast< SQInteger >(dpp::gm_deaf)},
    {_SC("Mute"),               static_cast< SQInteger >(dpp::gm_mute)},
    {_SC("Pending"),            static_cast< SQInteger >(dpp::gm_pending)},
    {_SC("AnimatedAvatar"),     static_cast< SQInteger >(dpp::gm_animated_avatar)},
    {_SC("VoiceAction"),        static_cast< SQInteger >(dpp::gm_voice_action)},
};

// ------------------------------------------------------------------------------------------------
static const EnumElements g_EnumList[] = {
    {_SC("SqDiscordLogLevel"),              g_DpLogLevelEnum},
    {_SC("SqDiscordVoiceStateFlags"),       g_DpVoiceStateFlagsEnum},
    {_SC("SqDiscordEmojiFlags"),            g_DpEmojiFlagsEnum},
    {_SC("SqDiscordPresenceFlags"),         g_DpPresenceFlagsEnum},
    {_SC("SqDiscordPresenceStatus"),        g_DpPresenceStatusEnum},
    {_SC("SqDiscordActivityType"),          g_DpActivityTypeEnum},
    {_SC("SqDiscordActivityFlags"),         g_DpActivityFlagsEnum},
    {_SC("SqDiscordRegion"),                g_DpRegionEnum},
    {_SC("SqDiscordGuildFlags"),            g_DpGuildFlagsEnum},
    {_SC("SqDiscordGuildFlagsExtra"),       g_DpGuildFlagsExtraEnum},
    {_SC("SqDiscordGuildMemberFlags"),      g_DpGuildMemberFlagsEnum},
};

// ------------------------------------------------------------------------------------------------
void Register_Discord_Constants(HSQUIRRELVM vm, Table & ns)
{
    RegisterEnumerations(vm, g_EnumList);
    // --------------------------------------------------------------------------------------------
    Enumeration e(vm);
    // Bind all events using their associated name
    for (SQInteger i = 0; i < static_cast< SQInteger >(DpEventID::Max); ++i)
    {
        e.Const(DpEventID::NAME[i], i);
    }
    // Expose the constants
    ConstTable(vm).Enum(_SC("SqDiscordEvent"), e);
}

} // Namespace:: SqMod
