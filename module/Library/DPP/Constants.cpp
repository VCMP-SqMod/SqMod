// ------------------------------------------------------------------------------------------------
#include "Library/DPP/Constants.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const std::array< const char *, static_cast< size_t >(DpEventID::Max) > DpEventID::NAME{
    "VoiceStateUpdate",
    "Log",
    "GuildJoinRequestDelete",
    "InteractionCreate",
    "ButtonClick",
    "SelectClick",
    "GuildDelete",
    "ChannelDelete",
    "ChannelUpdate",
    "Ready",
    "MessageDelete",
    "ApplicationCommandDelete",
    "GuildMemberRemove",
    "ApplicationCommandCreate",
    "Resumed",
    "GuildRoleCreate",
    "TypingStart",
    "MessageReactionAdd",
    "GuildMembersChunk",
    "MessageReactionRemove",
    "GuildCreate",
    "ChannelCreate",
    "MessageReactionRemoveEmoji",
    "MessageDeleteBulk",
    "GuildRoleUpdate",
    "GuildRoleDelete",
    "ChannelPinsUpdate",
    "MessageReactionRemoveAll",
    "VoiceServerUpdate",
    "GuildEmojisUpdate",
    "GuildStickersUpdate",
    "PresenceUpdate",
    "WebhooksUpdate",
    "GuildMemberAdd",
    "InviteDelete",
    "GuildUpdate",
    "GuildIntegrationsUpdate",
    "GuildMemberUpdate",
    "ApplicationCommandUpdate",
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
    "VoiceBufferSend",
    "VoiceUserTalking",
    "VoiceReady",
    "VoiceReceive",
    "VoiceTrackMarker",
    "StageInstanceCreate",
    "StageInstanceDelete"
};

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
    {_SC("StatusIdle"),     static_cast< SQInteger >(dpp::p_status_idle)},
    // Helper bit-shift flags
    {_SC("BitShiftDesktop"),    static_cast< SQInteger >(PF_SHIFT_DESKTOP)},
    {_SC("BitShiftWeb"),        static_cast< SQInteger >(PF_SHIFT_WEB)},
    {_SC("BitShiftMobile"),     static_cast< SQInteger >(PF_SHIFT_MOBILE)},
    {_SC("BitShiftMain"),       static_cast< SQInteger >(PF_SHIFT_MAIN)},
    {_SC("BitStatusMask"),      static_cast< SQInteger >(PF_STATUS_MASK)},
    {_SC("BitClearDesktop"),    static_cast< SQInteger >(PF_CLEAR_DESKTOP)},
    {_SC("BitClearWeb"),        static_cast< SQInteger >(PF_CLEAR_WEB)},
    {_SC("BitClearMobile"),     static_cast< SQInteger >(PF_CLEAR_MOBILE)},
    {_SC("BitClearStatus"),     static_cast< SQInteger >(PF_CLEAR_STATUS)},
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
    {_SC("Suppress"),   static_cast< SQInteger >(dpp::vs_suppress)},
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpRoleFlagsEnum[] = {
    {_SC("Hoist"),              static_cast< SQInteger >(dpp::r_hoist)},
    {_SC("Managed"),            static_cast< SQInteger >(dpp::r_managed)},
    {_SC("Mentionable"),        static_cast< SQInteger >(dpp::r_mentionable)},
    {_SC("PremiumSubscriber"),  static_cast< SQInteger >(dpp::r_premium_subscriber)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpRolePermissionsEnum[] = {
    {_SC("CreateInstantInvite"),  static_cast< SQInteger >(dpp::p_create_instant_invite)},
    {_SC("KickMembers"),           static_cast< SQInteger >(dpp::p_kick_members)},
    {_SC("BanMembers"),            static_cast< SQInteger >(dpp::p_ban_members)},
    {_SC("Administrator"),          static_cast< SQInteger >(dpp::p_administrator)},
    {_SC("ManageChannels"),        static_cast< SQInteger >(dpp::p_manage_channels)},
    {_SC("ManageGuild"),           static_cast< SQInteger >(dpp::p_manage_guild)},
    {_SC("AddReactions"),          static_cast< SQInteger >(dpp::p_add_reactions)},
    {_SC("ViewAuditLog"),         static_cast< SQInteger >(dpp::p_view_audit_log)},
    {_SC("PrioritySpeaker"),       static_cast< SQInteger >(dpp::p_priority_speaker)},
    {_SC("Stream"),                 static_cast< SQInteger >(dpp::p_stream)},
    {_SC("ViewChannel"),           static_cast< SQInteger >(dpp::p_view_channel)},
    {_SC("SendMessages"),          static_cast< SQInteger >(dpp::p_send_messages)},
    {_SC("SendTtsMessages"),      static_cast< SQInteger >(dpp::p_send_tts_messages)},
    {_SC("ManageMessages"),        static_cast< SQInteger >(dpp::p_manage_messages)},
    {_SC("EmbedLinks"),            static_cast< SQInteger >(dpp::p_embed_links)},
    {_SC("AttachFiles"),           static_cast< SQInteger >(dpp::p_attach_files)},
    {_SC("ReadMessageHistory"),   static_cast< SQInteger >(dpp::p_read_message_history)},
    {_SC("MentionEveryone"),       static_cast< SQInteger >(dpp::p_mention_everyone)},
    {_SC("UseExternalEmojis"),    static_cast< SQInteger >(dpp::p_use_external_emojis)},
    {_SC("ViewGuildInsights"),    static_cast< SQInteger >(dpp::p_view_guild_insights)},
    {_SC("Connect"),                static_cast< SQInteger >(dpp::p_connect)},
    {_SC("Speak"),                  static_cast< SQInteger >(dpp::p_speak)},
    {_SC("MuteMembers"),           static_cast< SQInteger >(dpp::p_mute_members)},
    {_SC("DeafenMembers"),         static_cast< SQInteger >(dpp::p_deafen_members)},
    {_SC("MoveMembers"),           static_cast< SQInteger >(dpp::p_move_members)},
    {_SC("UseVAD"),                static_cast< SQInteger >(dpp::p_use_vad)},
    {_SC("ChangeNickname"),        static_cast< SQInteger >(dpp::p_change_nickname)},
    {_SC("ManageNicknames"),       static_cast< SQInteger >(dpp::p_manage_nicknames)},
    {_SC("ManageRoles"),           static_cast< SQInteger >(dpp::p_manage_roles)},
    {_SC("ManageWebHooks"),        static_cast< SQInteger >(dpp::p_manage_webhooks)},
    {_SC("ManageEmojis"),          static_cast< SQInteger >(dpp::p_manage_emojis)},
    {_SC("UseSlashCommands"),     static_cast< SQInteger >(dpp::p_use_slash_commands)},
    {_SC("RequestToSpeak"),       static_cast< SQInteger >(dpp::p_request_to_speak)},
    {_SC("ManageThreads"),         static_cast< SQInteger >(dpp::p_manage_threads)},
    {_SC("UsePublicThreads"),     static_cast< SQInteger >(dpp::p_use_public_threads)},
    {_SC("UsePrivateThreads"),    static_cast< SQInteger >(dpp::p_use_private_threads)}
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

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpComponentTypeEnum[] = {
    {_SC("ActionRow"),      static_cast< SQInteger >(dpp::cot_action_row)},
    {_SC("Button"),         static_cast< SQInteger >(dpp::cot_button)},
    {_SC("SelectMenu"),     static_cast< SQInteger >(dpp::cot_selectmenu)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpComponentStyleEnum[] = {
    {_SC("Primary"),        static_cast< SQInteger >(dpp::cos_primary)},
    {_SC("Secondary"),      static_cast< SQInteger >(dpp::cos_secondary)},
    {_SC("Success"),        static_cast< SQInteger >(dpp::cos_success)},
    {_SC("Danger"),         static_cast< SQInteger >(dpp::cos_danger)},
    {_SC("Link"),           static_cast< SQInteger >(dpp::cos_link)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpStickerTypeEnum[] = {
    {_SC("Standard"),      static_cast< SQInteger >(dpp::st_standard)},
    {_SC("Guild"),         static_cast< SQInteger >(dpp::st_guild)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpStickerFormatEnum[] = {
    {_SC("PNG"),        static_cast< SQInteger >(dpp::sf_png)},
    {_SC("APNG"),       static_cast< SQInteger >(dpp::sf_apng)},
    {_SC("Lottie"),     static_cast< SQInteger >(dpp::sf_lottie)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpMessageFlagsEnum[] = {
    {_SC("Crossposted"),            static_cast< SQInteger >(dpp::m_crossposted)},
    {_SC("IsCrosspost"),            static_cast< SQInteger >(dpp::m_is_crosspost)},
    {_SC("SuppressEmbeds"),         static_cast< SQInteger >(dpp::m_suppress_embeds)},
    {_SC("SourceMessageDeleted"),   static_cast< SQInteger >(dpp::m_source_message_deleted)},
    {_SC("Urgent"),                 static_cast< SQInteger >(dpp::m_urgent)},
    {_SC("Ephemeral"),              static_cast< SQInteger >(dpp::m_ephemeral)},
    {_SC("Loading"),                static_cast< SQInteger >(dpp::m_loading)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_DpMessageTypeEnum[] = {
    {_SC("Default"),                                    static_cast< SQInteger >(dpp::mt_default)},
    {_SC("RecipientAdd"),                               static_cast< SQInteger >(dpp::mt_recipient_add)},
    {_SC("RecipientRemove"),                            static_cast< SQInteger >(dpp::mt_recipient_remove)},
    {_SC("Call"),                                       static_cast< SQInteger >(dpp::mt_call)},
    {_SC("ChannelNameChange"),                          static_cast< SQInteger >(dpp::mt_channel_name_change)},
    {_SC("ChannelIconChange"),                          static_cast< SQInteger >(dpp::mt_channel_icon_change)},
    {_SC("ChannelPinnedMessage"),                       static_cast< SQInteger >(dpp::mt_channel_pinned_message)},
    {_SC("GuildMemberJoin"),                            static_cast< SQInteger >(dpp::mt_guild_member_join)},
    {_SC("UserPremiumGuildSubscription"),               static_cast< SQInteger >(dpp::mt_user_premium_guild_subscription)},
    {_SC("UserPremiumGuildSubscriptionTier1"),          static_cast< SQInteger >(dpp::mt_user_premium_guild_subscription_tier_1)},
    {_SC("UserPremiumGuildSubscriptionTier2"),          static_cast< SQInteger >(dpp::mt_user_premium_guild_subscription_tier_2)},
    {_SC("UserPremiumGuildSubscriptionTier3"),          static_cast< SQInteger >(dpp::mt_user_premium_guild_subscription_tier_3)},
    {_SC("ChannelFollowAdd"),                           static_cast< SQInteger >(dpp::mt_channel_follow_add)},
    {_SC("GuildDiscoveryDisqualified"),                 static_cast< SQInteger >(dpp::mt_guild_discovery_disqualified)},
    {_SC("GuildDiscoveryRequalified"),                  static_cast< SQInteger >(dpp::mt_guild_discovery_requalified)},
    {_SC("GuildDiscoveryGracePeriodInitialWarning"),    static_cast< SQInteger >(dpp::mt_guild_discovery_grace_period_initial_warning)},
    {_SC("GuildDiscoveryGracePeriodFinalWarning"),      static_cast< SQInteger >(dpp::mt_guild_discovery_grace_period_final_warning)},
    {_SC("ThreadCreated"),                              static_cast< SQInteger >(dpp::mt_thread_created)},
    {_SC("Reply"),                                      static_cast< SQInteger >(dpp::mt_reply)},
    {_SC("ApplicationCommand"),                         static_cast< SQInteger >(dpp::mt_application_command)},
    {_SC("ThreadStarterMessage"),                       static_cast< SQInteger >(dpp::mt_thread_starter_message)},
    {_SC("GuildInviteReminder"),                        static_cast< SQInteger >(dpp::mt_guild_invite_reminder)}
};

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
    {_SC("SqDiscordRoleFlags"),             g_DpRoleFlagsEnum},
    {_SC("SqDiscordRolePermissions"),       g_DpRolePermissionsEnum},
    {_SC("SqDiscordUserFlags"),             g_DpUserFlagsEnum},
    {_SC("SqDiscordRegion"),                g_DpRegionEnum},
    {_SC("SqDiscordGuildFlags"),            g_DpGuildFlagsEnum},
    {_SC("SqDiscordGuildMemberFlags"),      g_DpGuildMemberFlagsEnum},
    {_SC("SqDiscordComponentType"),         g_DpComponentTypeEnum},
    {_SC("SqDiscordComponentStyle"),        g_DpComponentStyleEnum},
    {_SC("SqDiscordStickerType"),           g_DpStickerTypeEnum},
    {_SC("SqDiscordStickerFormat"),         g_DpStickerFormatEnum},
    {_SC("SqDiscordMessageFlags"),          g_DpMessageFlagsEnum},
    {_SC("SqDiscordMessageType"),           g_DpMessageTypeEnum}
};

// ------------------------------------------------------------------------------------------------
void Register_DPP_Constants(HSQUIRRELVM vm, Table & ns)
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
