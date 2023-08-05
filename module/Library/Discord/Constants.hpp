#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <dpp/dpp.h>

// ------------------------------------------------------------------------------------------------
#include <array>
#include <string_view>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Unique ID for each event.
*/
struct DpEventID
{
    /* --------------------------------------------------------------------------------------------
     * ID enumeration.
    */
    enum Type
    {
        VoiceStateUpdate=0,
        VoiceClientDisconnect,
        VoiceClientSpeaking,
        Log,
        GuildJoinRequestDelete,
        InteractionCreate,
        SlashCommand,
        ButtonClick,
        AutoComplete,
        SelectClick,
        MessageContextMenu,
        UserContextMenu,
        FormSubmit,
        GuildDelete,
        ChannelDelete,
        ChannelUpdate,
        Ready,
        MessageDelete,
        GuildMemberRemove,
        Resumed,
        GuildRoleCreate,
        TypingStart,
        MessageReactionAdd,
        GuildMembersChunk,
        MessageReactionRemove,
        GuildCreate,
        ChannelCreate,
        MessageReactionRemoveEmoji,
        MessageDeleteDulk,
        GuildRoleUpdate,
        GuildRoleDelete,
        ChannelPinsUpdate,
        MessageReactionRemoveAll,
        VoiceServerUpdate,
        GuildEmojisUpdate,
        GuildStickersUpdate,
        PresenceUpdate,
        WebhooksUpdate,
        AutomodRuleCreate,
        AutomodRuleUpdate,
        AutomodRuleDelete,
        AutomodRuleExecute,
        GuildMemberAdd,
        InviteDelete,
        GuildUpdate,
        GuildIntegrationsUpdate,
        GuildMemberUpdate,
        InviteCreate,
        MessageUpdate,
        UserUpdate,
        MessageCreate,
        GuildAuditLogEntryCreate,
        GuildBanAdd,
        GuildBanRemove,
        IntegrationCreate,
        IntegrationUpdate,
        IntegrationDelete,
        ThreadCreate,
        ThreadUpdate,
        ThreadDelete,
        ThreadListSync,
        ThreadMemberUpdate,
        ThreadMembersUpdate,
        GuildScheduledEventCreate,
        GuildScheduledEventUpdate,
        GuildScheduledEventDelete,
        GuildScheduledEventUserAdd,
        GuildScheduledEventUserRemove,
        VoiceBufferSend,
        VoiceUserTalking,
        VoiceReady,
        VoiceReceive,
        VoiceReceiveCombined,
        VoiceTrackMarker,
        StageInstanceCreate,
        StageInstanceUpdate,
        StageInstanceDelete,
        Max
    };
    /* --------------------------------------------------------------------------------------------
     * String identification for each event ID.
    */
    static const std::array< const char *, static_cast< size_t >(Max) > NAME;
};

/* ------------------------------------------------------------------------------------------------
 * Structures that hold compile-time type information for events.
*/
struct DpVoiceStateUpdateEventInfo
{
    using Type = dpp::voice_state_update_t;
    static inline constexpr size_t ID = DpEventID::VoiceStateUpdate;
    static inline constexpr std::string_view Name = "VoiceStateUpdate";
};
struct DpVoiceClientDisconnectEventInfo
{
    using Type = dpp::voice_client_disconnect_t;
    static inline constexpr size_t ID = DpEventID::VoiceClientDisconnect;
    static inline constexpr std::string_view Name = "VoiceClientDisconnect";
};
struct DpVoiceClientSpeakingEventInfo
{
    using Type = dpp::voice_client_speaking_t;
    static inline constexpr size_t ID = DpEventID::VoiceClientSpeaking;
    static inline constexpr std::string_view Name = "VoiceClientSpeaking";
};
struct DpLogEventInfo
{
    using Type = dpp::log_t;
    static inline constexpr size_t ID = DpEventID::Log;
    static inline constexpr std::string_view Name = "Log";
};
struct DpGuildJoinRequestDeleteEventInfo
{
    using Type = dpp::guild_join_request_delete_t;
    static inline constexpr size_t ID = DpEventID::GuildJoinRequestDelete;
    static inline constexpr std::string_view Name = "GuildJoinRequestDelete";
};
struct DpInteractionCreateEventInfo
{
    using Type = dpp::interaction_create_t;
    static inline constexpr size_t ID = DpEventID::InteractionCreate;
    static inline constexpr std::string_view Name = "InteractionCreate";
};
struct DpSlashCommandEventInfo
{
    using Type = dpp::slashcommand_t;
    static inline constexpr size_t ID = DpEventID::SlashCommand;
    static inline constexpr std::string_view Name = "SlashCommand";
};
struct DpButtonClickEventInfo
{
    using Type = dpp::button_click_t;
    static inline constexpr size_t ID = DpEventID::ButtonClick;
    static inline constexpr std::string_view Name = "ButtonClick";
};
struct DpAutoCompleteEventInfo
{
    using Type = dpp::autocomplete_t;
    static inline constexpr size_t ID = DpEventID::AutoComplete;
    static inline constexpr std::string_view Name = "AutoComplete";
};
struct DpSelectClickEventInfo
{
    using Type = dpp::select_click_t;
    static inline constexpr size_t ID = DpEventID::SelectClick;
    static inline constexpr std::string_view Name = "SelectClick";
};
struct DpMessageContextMenuEventInfo
{
    using Type = dpp::message_context_menu_t;
    static inline constexpr size_t ID = DpEventID::MessageContextMenu;
    static inline constexpr std::string_view Name = "MessageContextMenu";
};
struct DpUserContextMenuEventInfo
{
    using Type = dpp::user_context_menu_t;
    static inline constexpr size_t ID = DpEventID::UserContextMenu;
    static inline constexpr std::string_view Name = "UserContextMenu";
};
struct DpFormSubmitEventInfo
{
    using Type = dpp::form_submit_t;
    static inline constexpr size_t ID = DpEventID::FormSubmit;
    static inline constexpr std::string_view Name = "FormSubmit";
};
struct DpGuildDeleteEventInfo
{
    using Type = dpp::guild_delete_t;
    static inline constexpr size_t ID = DpEventID::GuildDelete;
    static inline constexpr std::string_view Name = "GuildDelete";
};
struct DpChannelDeleteEventInfo
{
    using Type = dpp::channel_delete_t;
    static inline constexpr size_t ID = DpEventID::ChannelDelete;
    static inline constexpr std::string_view Name = "ChannelDelete";
};
struct DpChannelUpdateEventInfo
{
    using Type = dpp::channel_update_t;
    static inline constexpr size_t ID = DpEventID::ChannelUpdate;
    static inline constexpr std::string_view Name = "ChannelUpdate";
};
struct DpReadyEventInfo
{
    using Type = dpp::ready_t;
    static inline constexpr size_t ID = DpEventID::Ready;
    static inline constexpr std::string_view Name = "Ready";
};
struct DpMessageDeleteEventInfo
{
    using Type = dpp::message_delete_t;
    static inline constexpr size_t ID = DpEventID::MessageDelete;
    static inline constexpr std::string_view Name = "MessageDelete";
};
struct DpGuildMemberRemoveEventInfo
{
    using Type = dpp::guild_member_remove_t;
    static inline constexpr size_t ID = DpEventID::GuildMemberRemove;
    static inline constexpr std::string_view Name = "GuildMemberRemove";
};
struct DpResumedEventInfo
{
    using Type = dpp::resumed_t;
    static inline constexpr size_t ID = DpEventID::Resumed;
    static inline constexpr std::string_view Name = "Resumed";
};
struct DpGuildRoleCreateEventInfo
{
    using Type = dpp::guild_role_create_t;
    static inline constexpr size_t ID = DpEventID::GuildRoleCreate;
    static inline constexpr std::string_view Name = "GuildRoleCreate";
};
struct DpTypingStartEventInfo
{
    using Type = dpp::typing_start_t;
    static inline constexpr size_t ID = DpEventID::TypingStart;
    static inline constexpr std::string_view Name = "TypingStart";
};
struct DpMessageReactionAddEventInfo
{
    using Type = dpp::message_reaction_add_t;
    static inline constexpr size_t ID = DpEventID::MessageReactionAdd;
    static inline constexpr std::string_view Name = "MessageReactionAdd";
};
struct DpGuildMembersChunkEventInfo
{
    using Type = dpp::guild_members_chunk_t;
    static inline constexpr size_t ID = DpEventID::GuildMembersChunk;
    static inline constexpr std::string_view Name = "GuildMembersChunk";
};
struct DpMessageReactionRemoveEventInfo
{
    using Type = dpp::message_reaction_remove_t;
    static inline constexpr size_t ID = DpEventID::MessageReactionRemove;
    static inline constexpr std::string_view Name = "MessageReactionRemove";
};
struct DpGuildCreateEventInfo
{
    using Type = dpp::guild_create_t;
    static inline constexpr size_t ID = DpEventID::GuildCreate;
    static inline constexpr std::string_view Name = "GuildCreate";
};
struct DpChannelCreateEventInfo
{
    using Type = dpp::channel_create_t;
    static inline constexpr size_t ID = DpEventID::ChannelCreate;
    static inline constexpr std::string_view Name = "ChannelCreate";
};
struct DpMessageReactionRemoveEmojiEventInfo
{
    using Type = dpp::message_reaction_remove_emoji_t;
    static inline constexpr size_t ID = DpEventID::MessageReactionRemoveEmoji;
    static inline constexpr std::string_view Name = "MessageReactionRemoveEmoji";
};
struct DpMessageDeleteDulkEventInfo
{
    using Type = dpp::message_delete_bulk_t;
    static inline constexpr size_t ID = DpEventID::MessageDeleteDulk;
    static inline constexpr std::string_view Name = "MessageDeleteDulk";
};
struct DpGuildRoleUpdateEventInfo
{
    using Type = dpp::guild_role_update_t;
    static inline constexpr size_t ID = DpEventID::GuildRoleUpdate;
    static inline constexpr std::string_view Name = "GuildRoleUpdate";
};
struct DpGuildRoleDeleteEventInfo
{
    using Type = dpp::guild_role_delete_t;
    static inline constexpr size_t ID = DpEventID::GuildRoleDelete;
    static inline constexpr std::string_view Name = "GuildRoleDelete";
};
struct DpChannelPinsUpdateEventInfo
{
    using Type = dpp::channel_pins_update_t;
    static inline constexpr size_t ID = DpEventID::ChannelPinsUpdate;
    static inline constexpr std::string_view Name = "ChannelPinsUpdate";
};
struct DpMessageReactionRemoveAllEventInfo
{
    using Type = dpp::message_reaction_remove_all_t;
    static inline constexpr size_t ID = DpEventID::MessageReactionRemoveAll;
    static inline constexpr std::string_view Name = "MessageReactionRemoveAll";
};
struct DpVoiceServerUpdateEventInfo
{
    using Type = dpp::voice_server_update_t;
    static inline constexpr size_t ID = DpEventID::VoiceServerUpdate;
    static inline constexpr std::string_view Name = "VoiceServerUpdate";
};
struct DpGuildEmojisUpdateEventInfo
{
    using Type = dpp::guild_emojis_update_t;
    static inline constexpr size_t ID = DpEventID::GuildEmojisUpdate;
    static inline constexpr std::string_view Name = "GuildEmojisUpdate";
};
struct DpGuildStickersUpdateEventInfo
{
    using Type = dpp::guild_stickers_update_t;
    static inline constexpr size_t ID = DpEventID::GuildStickersUpdate;
    static inline constexpr std::string_view Name = "GuildStickersUpdate";
};
struct DpPresenceUpdateEventInfo
{
    using Type = dpp::presence_update_t;
    static inline constexpr size_t ID = DpEventID::PresenceUpdate;
    static inline constexpr std::string_view Name = "PresenceUpdate";
};
struct DpWebhooksUpdateEventInfo
{
    using Type = dpp::webhooks_update_t;
    static inline constexpr size_t ID = DpEventID::WebhooksUpdate;
    static inline constexpr std::string_view Name = "WebhooksUpdate";
};
struct DpAutomodRuleCreateEventInfo
{
    using Type = dpp::automod_rule_create_t;
    static inline constexpr size_t ID = DpEventID::AutomodRuleCreate;
    static inline constexpr std::string_view Name = "AutomodRuleCreate";
};
struct DpAutomodRuleUpdateEventInfo
{
    using Type = dpp::automod_rule_update_t;
    static inline constexpr size_t ID = DpEventID::AutomodRuleUpdate;
    static inline constexpr std::string_view Name = "AutomodRuleUpdate";
};
struct DpAutomodRuleDeleteEventInfo
{
    using Type = dpp::automod_rule_delete_t;
    static inline constexpr size_t ID = DpEventID::AutomodRuleDelete;
    static inline constexpr std::string_view Name = "AutomodRuleDelete";
};
struct DpAutomodRuleExecuteEventInfo
{
    using Type = dpp::automod_rule_execute_t;
    static inline constexpr size_t ID = DpEventID::AutomodRuleExecute;
    static inline constexpr std::string_view Name = "AutomodRuleExecute";
};
struct DpGuildMemberAddEventInfo
{
    using Type = dpp::guild_member_add_t;
    static inline constexpr size_t ID = DpEventID::GuildMemberAdd;
    static inline constexpr std::string_view Name = "GuildMemberAdd";
};
struct DpInviteDeleteEventInfo
{
    using Type = dpp::invite_delete_t;
    static inline constexpr size_t ID = DpEventID::InviteDelete;
    static inline constexpr std::string_view Name = "InviteDelete";
};
struct DpGuildUpdateEventInfo
{
    using Type = dpp::guild_update_t;
    static inline constexpr size_t ID = DpEventID::GuildUpdate;
    static inline constexpr std::string_view Name = "GuildUpdate";
};
struct DpGuildIntegrationsUpdateEventInfo
{
    using Type = dpp::guild_integrations_update_t;
    static inline constexpr size_t ID = DpEventID::GuildIntegrationsUpdate;
    static inline constexpr std::string_view Name = "GuildIntegrationsUpdate";
};
struct DpGuildMemberUpdateEventInfo
{
    using Type = dpp::guild_member_update_t;
    static inline constexpr size_t ID = DpEventID::GuildMemberUpdate;
    static inline constexpr std::string_view Name = "GuildMemberUpdate";
};
struct DpInviteCreateEventInfo
{
    using Type = dpp::invite_create_t;
    static inline constexpr size_t ID = DpEventID::InviteCreate;
    static inline constexpr std::string_view Name = "InviteCreate";
};
struct DpMessageUpdateEventInfo
{
    using Type = dpp::message_update_t;
    static inline constexpr size_t ID = DpEventID::MessageUpdate;
    static inline constexpr std::string_view Name = "MessageUpdate";
};
struct DpUserUpdateEventInfo
{
    using Type = dpp::user_update_t;
    static inline constexpr size_t ID = DpEventID::UserUpdate;
    static inline constexpr std::string_view Name = "UserUpdate";
};
struct DpMessageCreateEventInfo
{
    using Type = dpp::message_create_t;
    static inline constexpr size_t ID = DpEventID::MessageCreate;
    static inline constexpr std::string_view Name = "MessageCreate";
};
struct DpGuildAuditLogEntryCreateEventInfo
{
    using Type = dpp::guild_audit_log_entry_create_t;
    static inline constexpr size_t ID = DpEventID::GuildAuditLogEntryCreate;
    static inline constexpr std::string_view Name = "GuildAuditLogEntryCreate";
};
struct DpGuildBanAddEventInfo
{
    using Type = dpp::guild_ban_add_t;
    static inline constexpr size_t ID = DpEventID::GuildBanAdd;
    static inline constexpr std::string_view Name = "GuildBanAdd";
};
struct DpGuildBanRemoveEventInfo
{
    using Type = dpp::guild_ban_remove_t;
    static inline constexpr size_t ID = DpEventID::GuildBanRemove;
    static inline constexpr std::string_view Name = "GuildBanRemove";
};
struct DpIntegrationCreateEventInfo
{
    using Type = dpp::integration_create_t;
    static inline constexpr size_t ID = DpEventID::IntegrationCreate;
    static inline constexpr std::string_view Name = "IntegrationCreate";
};
struct DpIntegrationUpdateEventInfo
{
    using Type = dpp::integration_update_t;
    static inline constexpr size_t ID = DpEventID::IntegrationUpdate;
    static inline constexpr std::string_view Name = "IntegrationUpdate";
};
struct DpIntegrationDeleteEventInfo
{
    using Type = dpp::integration_delete_t;
    static inline constexpr size_t ID = DpEventID::IntegrationDelete;
    static inline constexpr std::string_view Name = "IntegrationDelete";
};
struct DpThreadCreateEventInfo
{
    using Type = dpp::thread_create_t;
    static inline constexpr size_t ID = DpEventID::ThreadCreate;
    static inline constexpr std::string_view Name = "ThreadCreate";
};
struct DpThreadUpdateEventInfo
{
    using Type = dpp::thread_update_t;
    static inline constexpr size_t ID = DpEventID::ThreadUpdate;
    static inline constexpr std::string_view Name = "ThreadUpdate";
};
struct DpThreadDeleteEventInfo
{
    using Type = dpp::thread_delete_t;
    static inline constexpr size_t ID = DpEventID::ThreadDelete;
    static inline constexpr std::string_view Name = "ThreadDelete";
};
struct DpThreadListSyncEventInfo
{
    using Type = dpp::thread_list_sync_t;
    static inline constexpr size_t ID = DpEventID::ThreadListSync;
    static inline constexpr std::string_view Name = "ThreadListSync";
};
struct DpThreadMemberUpdateEventInfo
{
    using Type = dpp::thread_member_update_t;
    static inline constexpr size_t ID = DpEventID::ThreadMemberUpdate;
    static inline constexpr std::string_view Name = "ThreadMemberUpdate";
};
struct DpThreadMembersUpdateEventInfo
{
    using Type = dpp::thread_members_update_t;
    static inline constexpr size_t ID = DpEventID::ThreadMembersUpdate;
    static inline constexpr std::string_view Name = "ThreadMembersUpdate";
};
struct DpGuildScheduledEventCreateEventInfo
{
    using Type = dpp::guild_scheduled_event_create_t;
    static inline constexpr size_t ID = DpEventID::GuildScheduledEventCreate;
    static inline constexpr std::string_view Name = "GuildScheduledEventCreate";
};
struct DpGuildScheduledEventUpdateEventInfo
{
    using Type = dpp::guild_scheduled_event_update_t;
    static inline constexpr size_t ID = DpEventID::GuildScheduledEventUpdate;
    static inline constexpr std::string_view Name = "GuildScheduledEventUpdate";
};
struct DpGuildScheduledEventDeleteEventInfo
{
    using Type = dpp::guild_scheduled_event_delete_t;
    static inline constexpr size_t ID = DpEventID::GuildScheduledEventDelete;
    static inline constexpr std::string_view Name = "GuildScheduledEventDelete";
};
struct DpGuildScheduledEventUserAddEventInfo
{
    using Type = dpp::guild_scheduled_event_user_add_t;
    static inline constexpr size_t ID = DpEventID::GuildScheduledEventUserAdd;
    static inline constexpr std::string_view Name = "GuildScheduledEventUserAdd";
};
struct DpGuildScheduledEventUserRemoveEventInfo
{
    using Type = dpp::guild_scheduled_event_user_remove_t;
    static inline constexpr size_t ID = DpEventID::GuildScheduledEventUserRemove;
    static inline constexpr std::string_view Name = "GuildScheduledEventUserRemove";
};
struct DpVoiceBufferSendEventInfo
{
    using Type = dpp::voice_buffer_send_t;
    static inline constexpr size_t ID = DpEventID::VoiceBufferSend;
    static inline constexpr std::string_view Name = "VoiceBufferSend";
};
struct DpVoiceUserTalkingEventInfo
{
    using Type = dpp::voice_user_talking_t;
    static inline constexpr size_t ID = DpEventID::VoiceUserTalking;
    static inline constexpr std::string_view Name = "VoiceUserTalking";
};
struct DpVoiceReadyEventInfo
{
    using Type = dpp::voice_ready_t;
    static inline constexpr size_t ID = DpEventID::VoiceReady;
    static inline constexpr std::string_view Name = "VoiceReady";
};
struct DpVoiceReceiveEventInfo
{
    using Type = dpp::voice_receive_t;
    static inline constexpr size_t ID = DpEventID::VoiceReceive;
    static inline constexpr std::string_view Name = "VoiceReceive";
};
struct DpVoiceReceiveCombinedEventInfo
{
    using Type = dpp::voice_receive_t;
    static inline constexpr size_t ID = DpEventID::VoiceReceiveCombined;
    static inline constexpr std::string_view Name = "VoiceReceiveCombined";
};
struct DpVoiceTrackMarkerEventInfo
{
    using Type = dpp::voice_track_marker_t;
    static inline constexpr size_t ID = DpEventID::VoiceTrackMarker;
    static inline constexpr std::string_view Name = "VoiceTrackMarker";
};
struct DpStageInstanceCreateEventInfo
{
    using Type = dpp::stage_instance_create_t;
    static inline constexpr size_t ID = DpEventID::StageInstanceCreate;
    static inline constexpr std::string_view Name = "StageInstanceCreate";
};
struct DpStageInstanceUpdateEventInfo
{
    using Type = dpp::stage_instance_update_t;
    static inline constexpr size_t ID = DpEventID::StageInstanceUpdate;
    static inline constexpr std::string_view Name = "StageInstanceUpdate";
};
struct DpStageInstanceDeleteEventInfo
{
    using Type = dpp::stage_instance_delete_t;
    static inline constexpr size_t ID = DpEventID::StageInstanceDelete;
    static inline constexpr std::string_view Name = "StageInstanceDelete";
};

/* ------------------------------------------------------------------------------------------------
 * Utility used to acquire event information at compile time.
*/
template < int > struct EventInfoID;

/* ------------------------------------------------------------------------------------------------
 * Implementation.
*/
template < > struct EventInfoID< DpEventID::VoiceStateUpdate > : public DpVoiceStateUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::VoiceClientDisconnect > : public DpVoiceClientDisconnectEventInfo { };
template < > struct EventInfoID< DpEventID::VoiceClientSpeaking > : public DpVoiceClientSpeakingEventInfo { };
template < > struct EventInfoID< DpEventID::Log > : public DpLogEventInfo { };
template < > struct EventInfoID< DpEventID::GuildJoinRequestDelete > : public DpGuildJoinRequestDeleteEventInfo { };
template < > struct EventInfoID< DpEventID::InteractionCreate > : public DpInteractionCreateEventInfo { };
template < > struct EventInfoID< DpEventID::SlashCommand > : public DpSlashCommandEventInfo { };
template < > struct EventInfoID< DpEventID::ButtonClick > : public DpButtonClickEventInfo { };
template < > struct EventInfoID< DpEventID::AutoComplete > : public DpAutoCompleteEventInfo { };
template < > struct EventInfoID< DpEventID::SelectClick > : public DpSelectClickEventInfo { };
template < > struct EventInfoID< DpEventID::MessageContextMenu > : public DpMessageContextMenuEventInfo { };
template < > struct EventInfoID< DpEventID::UserContextMenu > : public DpUserContextMenuEventInfo { };
template < > struct EventInfoID< DpEventID::FormSubmit > : public DpFormSubmitEventInfo { };
template < > struct EventInfoID< DpEventID::GuildDelete > : public DpGuildDeleteEventInfo { };
template < > struct EventInfoID< DpEventID::ChannelDelete > : public DpChannelDeleteEventInfo { };
template < > struct EventInfoID< DpEventID::ChannelUpdate > : public DpChannelUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::Ready > : public DpReadyEventInfo { };
template < > struct EventInfoID< DpEventID::MessageDelete > : public DpMessageDeleteEventInfo { };
template < > struct EventInfoID< DpEventID::GuildMemberRemove > : public DpGuildMemberRemoveEventInfo { };
template < > struct EventInfoID< DpEventID::Resumed > : public DpResumedEventInfo { };
template < > struct EventInfoID< DpEventID::GuildRoleCreate > : public DpGuildRoleCreateEventInfo { };
template < > struct EventInfoID< DpEventID::TypingStart > : public DpTypingStartEventInfo { };
template < > struct EventInfoID< DpEventID::MessageReactionAdd > : public DpMessageReactionAddEventInfo { };
template < > struct EventInfoID< DpEventID::GuildMembersChunk > : public DpGuildMembersChunkEventInfo { };
template < > struct EventInfoID< DpEventID::MessageReactionRemove > : public DpMessageReactionRemoveEventInfo { };
template < > struct EventInfoID< DpEventID::GuildCreate > : public DpGuildCreateEventInfo { };
template < > struct EventInfoID< DpEventID::ChannelCreate > : public DpChannelCreateEventInfo { };
template < > struct EventInfoID< DpEventID::MessageReactionRemoveEmoji > : public DpMessageReactionRemoveEmojiEventInfo { };
template < > struct EventInfoID< DpEventID::MessageDeleteDulk > : public DpMessageDeleteDulkEventInfo { };
template < > struct EventInfoID< DpEventID::GuildRoleUpdate > : public DpGuildRoleUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::GuildRoleDelete > : public DpGuildRoleDeleteEventInfo { };
template < > struct EventInfoID< DpEventID::ChannelPinsUpdate > : public DpChannelPinsUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::MessageReactionRemoveAll > : public DpMessageReactionRemoveAllEventInfo { };
template < > struct EventInfoID< DpEventID::VoiceServerUpdate > : public DpVoiceServerUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::GuildEmojisUpdate > : public DpGuildEmojisUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::GuildStickersUpdate > : public DpGuildStickersUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::PresenceUpdate > : public DpPresenceUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::WebhooksUpdate > : public DpWebhooksUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::AutomodRuleCreate > : public DpAutomodRuleCreateEventInfo { };
template < > struct EventInfoID< DpEventID::AutomodRuleUpdate > : public DpAutomodRuleUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::AutomodRuleDelete > : public DpAutomodRuleDeleteEventInfo { };
template < > struct EventInfoID< DpEventID::AutomodRuleExecute > : public DpAutomodRuleExecuteEventInfo { };
template < > struct EventInfoID< DpEventID::GuildMemberAdd > : public DpGuildMemberAddEventInfo { };
template < > struct EventInfoID< DpEventID::InviteDelete > : public DpInviteDeleteEventInfo { };
template < > struct EventInfoID< DpEventID::GuildUpdate > : public DpGuildUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::GuildIntegrationsUpdate > : public DpGuildIntegrationsUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::GuildMemberUpdate > : public DpGuildMemberUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::InviteCreate > : public DpInviteCreateEventInfo { };
template < > struct EventInfoID< DpEventID::MessageUpdate > : public DpMessageUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::UserUpdate > : public DpUserUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::MessageCreate > : public DpMessageCreateEventInfo { };
template < > struct EventInfoID< DpEventID::GuildAuditLogEntryCreate > : public DpGuildAuditLogEntryCreateEventInfo { };
template < > struct EventInfoID< DpEventID::GuildBanAdd > : public DpGuildBanAddEventInfo { };
template < > struct EventInfoID< DpEventID::GuildBanRemove > : public DpGuildBanRemoveEventInfo { };
template < > struct EventInfoID< DpEventID::IntegrationCreate > : public DpIntegrationCreateEventInfo { };
template < > struct EventInfoID< DpEventID::IntegrationUpdate > : public DpIntegrationUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::IntegrationDelete > : public DpIntegrationDeleteEventInfo { };
template < > struct EventInfoID< DpEventID::ThreadCreate > : public DpThreadCreateEventInfo { };
template < > struct EventInfoID< DpEventID::ThreadUpdate > : public DpThreadUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::ThreadDelete > : public DpThreadDeleteEventInfo { };
template < > struct EventInfoID< DpEventID::ThreadListSync > : public DpThreadListSyncEventInfo { };
template < > struct EventInfoID< DpEventID::ThreadMemberUpdate > : public DpThreadMemberUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::ThreadMembersUpdate > : public DpThreadMembersUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::GuildScheduledEventCreate > : public DpGuildScheduledEventCreateEventInfo { };
template < > struct EventInfoID< DpEventID::GuildScheduledEventUpdate > : public DpGuildScheduledEventUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::GuildScheduledEventDelete > : public DpGuildScheduledEventDeleteEventInfo { };
template < > struct EventInfoID< DpEventID::GuildScheduledEventUserAdd > : public DpGuildScheduledEventUserAddEventInfo { };
template < > struct EventInfoID< DpEventID::GuildScheduledEventUserRemove > : public DpGuildScheduledEventUserRemoveEventInfo { };
template < > struct EventInfoID< DpEventID::VoiceBufferSend > : public DpVoiceBufferSendEventInfo { };
template < > struct EventInfoID< DpEventID::VoiceUserTalking > : public DpVoiceUserTalkingEventInfo { };
template < > struct EventInfoID< DpEventID::VoiceReady > : public DpVoiceReadyEventInfo { };
template < > struct EventInfoID< DpEventID::VoiceReceive > : public DpVoiceReceiveEventInfo { };
template < > struct EventInfoID< DpEventID::VoiceReceiveCombined > : public DpVoiceReceiveCombinedEventInfo { };
template < > struct EventInfoID< DpEventID::VoiceTrackMarker > : public DpVoiceTrackMarkerEventInfo { };
template < > struct EventInfoID< DpEventID::StageInstanceCreate > : public DpStageInstanceCreateEventInfo { };
template < > struct EventInfoID< DpEventID::StageInstanceUpdate > : public DpStageInstanceUpdateEventInfo { };
template < > struct EventInfoID< DpEventID::StageInstanceDelete > : public DpStageInstanceDeleteEventInfo { };

} // Namespace:: SqMod
