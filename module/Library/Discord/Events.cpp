// ------------------------------------------------------------------------------------------------
#include "Library/Discord/Events.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDpVoiceStateUpdateEvent, _SC("SqDiscordVoiceStateUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpVoiceClientDisconnectEvent, _SC("SqDiscordVoiceClientDisconnectEvent"))
SQMOD_DECL_TYPENAME(SqDpVoiceClientSpeakingEvent, _SC("SqDiscordVoiceClientSpeakingEvent"))
SQMOD_DECL_TYPENAME(SqDpLogEvent, _SC("SqDiscordLogEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildJoinRequestDeleteEvent, _SC("SqDiscordGuildJoinRequestDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDpInteractionCreateEvent, _SC("SqDiscordInteractionCreateEvent"))
SQMOD_DECL_TYPENAME(SqDpSlashCommandEvent, _SC("SqDiscordSlashCommandEvent"))
SQMOD_DECL_TYPENAME(SqDpButtonClickEvent, _SC("SqDiscordButtonClickEvent"))
SQMOD_DECL_TYPENAME(SqDpAutoCompleteEvent, _SC("SqDiscordAutoCompleteEvent"))
SQMOD_DECL_TYPENAME(SqDpSelectClickEvent, _SC("SqDiscordSelectClickEvent"))
SQMOD_DECL_TYPENAME(SqDpMessageContextMenuEvent, _SC("SqDiscordMessageContextMenuEvent"))
SQMOD_DECL_TYPENAME(SqDpUserContextMenuEvent, _SC("SqDiscordUserContextMenuEvent"))
SQMOD_DECL_TYPENAME(SqDpFormSubmitEvent, _SC("SqDiscordFormSubmitEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildDeleteEvent, _SC("SqDiscordGuildDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDpChannelDeleteEvent, _SC("SqDiscordChannelDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDpChannelUpdateEvent, _SC("SqDiscordChannelUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpReadyEvent, _SC("SqDiscordReadyEvent"))
SQMOD_DECL_TYPENAME(SqDpMessageDeleteEvent, _SC("SqDiscordMessageDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildMemberRemoveEvent, _SC("SqDiscordGuildMemberRemoveEvent"))
SQMOD_DECL_TYPENAME(SqDpResumedEvent, _SC("SqDiscordResumedEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildRoleCreateEvent, _SC("SqDiscordGuildRoleCreateEvent"))
SQMOD_DECL_TYPENAME(SqDpTypingStartEvent, _SC("SqDiscordTypingStartEvent"))
SQMOD_DECL_TYPENAME(SqDpMessageReactionAddEvent, _SC("SqDiscordMessageReactionAddEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildMembersChunkEvent, _SC("SqDiscordGuildMembersChunkEvent"))
SQMOD_DECL_TYPENAME(SqDpMessageReactionRemoveEvent, _SC("SqDiscordMessageReactionRemoveEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildCreateEvent, _SC("SqDiscordGuildCreateEvent"))
SQMOD_DECL_TYPENAME(SqDpChannelCreateEvent, _SC("SqDiscordChannelCreateEvent"))
SQMOD_DECL_TYPENAME(SqDpMessageReactionRemoveEmojiEvent, _SC("SqDiscordMessageReactionRemoveEmojiEvent"))
SQMOD_DECL_TYPENAME(SqDpMessageDeleteDulkEvent, _SC("SqDiscordMessageDeleteDulkEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildRoleUpdateEvent, _SC("SqDiscordGuildRoleUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildRoleDeleteEvent, _SC("SqDiscordGuildRoleDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDpChannelPinsUpdateEvent, _SC("SqDiscordChannelPinsUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpMessageReactionRemoveAllEvent, _SC("SqDiscordMessageReactionRemoveAllEvent"))
SQMOD_DECL_TYPENAME(SqDpVoiceServerUpdateEvent, _SC("SqDiscordVoiceServerUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildEmojisUpdateEvent, _SC("SqDiscordGuildEmojisUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildStickersUpdateEvent, _SC("SqDiscordGuildStickersUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpPresenceUpdateEvent, _SC("SqDiscordPresenceUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpWebhooksUpdateEvent, _SC("SqDiscordWebhooksUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpAutomodRuleCreateEvent, _SC("SqDiscordAutomodRuleCreateEvent"))
SQMOD_DECL_TYPENAME(SqDpAutomodRuleUpdateEvent, _SC("SqDiscordAutomodRuleUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpAutomodRuleDeleteEvent, _SC("SqDiscordAutomodRuleDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDpAutomodRuleExecuteEvent, _SC("SqDiscordAutomodRuleExecuteEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildMemberAddEvent, _SC("SqDiscordGuildMemberAddEvent"))
SQMOD_DECL_TYPENAME(SqDpInviteDeleteEvent, _SC("SqDiscordInviteDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildUpdateEvent, _SC("SqDiscordGuildUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildIntegrationsUpdateEvent, _SC("SqDiscordGuildIntegrationsUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildMemberUpdateEvent, _SC("SqDiscordGuildMemberUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpInviteCreateEvent, _SC("SqDiscordInviteCreateEvent"))
SQMOD_DECL_TYPENAME(SqDpMessageUpdateEvent, _SC("SqDiscordMessageUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpUserUpdateEvent, _SC("SqDiscordUserUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpMessageCreateEvent, _SC("SqDiscordMessageCreateEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildAuditLogEntryCreateEvent, _SC("SqDiscordGuildAuditLogEntryCreateEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildBanAddEvent, _SC("SqDiscordGuildBanAddEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildBanRemoveEvent, _SC("SqDiscordGuildBanRemoveEvent"))
SQMOD_DECL_TYPENAME(SqDpIntegrationCreateEvent, _SC("SqDiscordIntegrationCreateEvent"))
SQMOD_DECL_TYPENAME(SqDpIntegrationUpdateEvent, _SC("SqDiscordIntegrationUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpIntegrationDeleteEvent, _SC("SqDiscordIntegrationDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDpThreadCreateEvent, _SC("SqDiscordThreadCreateEvent"))
SQMOD_DECL_TYPENAME(SqDpThreadUpdateEvent, _SC("SqDiscordThreadUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpThreadDeleteEvent, _SC("SqDiscordThreadDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDpThreadListSyncEvent, _SC("SqDiscordThreadListSyncEvent"))
SQMOD_DECL_TYPENAME(SqDpThreadMemberUpdateEvent, _SC("SqDiscordThreadMemberUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpThreadMembersUpdateEvent, _SC("SqDiscordThreadMembersUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildScheduledEventCreateEvent, _SC("SqDiscordGuildScheduledEventCreateEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildScheduledEventUpdateEvent, _SC("SqDiscordGuildScheduledEventUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildScheduledEventDeleteEvent, _SC("SqDiscordGuildScheduledEventDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildScheduledEventUserAddEvent, _SC("SqDiscordGuildScheduledEventUserAddEvent"))
SQMOD_DECL_TYPENAME(SqDpGuildScheduledEventUserRemoveEvent, _SC("SqDiscordGuildScheduledEventUserRemoveEvent"))
SQMOD_DECL_TYPENAME(SqDpVoiceBufferSendEvent, _SC("SqDiscordVoiceBufferSendEvent"))
SQMOD_DECL_TYPENAME(SqDpVoiceUserTalkingEvent, _SC("SqDiscordVoiceUserTalkingEvent"))
SQMOD_DECL_TYPENAME(SqDpVoiceReadyEvent, _SC("SqDiscordVoiceReadyEvent"))
SQMOD_DECL_TYPENAME(SqDpVoiceReceiveEvent, _SC("SqDiscordVoiceReceiveEvent"))
SQMOD_DECL_TYPENAME(SqDpVoiceReceiveCombinedEvent, _SC("SqDiscordVoiceReceiveCombinedEvent"))
SQMOD_DECL_TYPENAME(SqDpVoiceTrackMarkerEvent, _SC("SqDiscordVoiceTrackMarkerEvent"))
SQMOD_DECL_TYPENAME(SqDpStageInstanceCreateEvent, _SC("SqDiscordStageInstanceCreateEvent"))
SQMOD_DECL_TYPENAME(SqDpStageInstanceUpdateEvent, _SC("SqDiscordStageInstanceUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDpStageInstanceDeleteEvent, _SC("SqDiscordStageInstanceDeleteEvent"))

// ------------------------------------------------------------------------------------------------
void Register_Discord_Events(HSQUIRRELVM vm, Table & ns)
{
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceStateUpdate"),
        Class< DpVoiceStateUpdateEvent, NoConstructor< DpVoiceStateUpdateEvent > >(vm, SqDpVoiceStateUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpVoiceStateUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceStateUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpVoiceStateUpdateEvent::GetRawEvent)
        .Prop(_SC("State"), &DpVoiceStateUpdateEvent::GetState)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceClientDisconnect"),
        Class< DpVoiceClientDisconnectEvent, NoConstructor< DpVoiceClientDisconnectEvent > >(vm, SqDpVoiceClientDisconnectEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpVoiceClientDisconnectEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceClientDisconnectEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpVoiceClientDisconnectEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceClientSpeaking"),
        Class< DpVoiceClientSpeakingEvent, NoConstructor< DpVoiceClientSpeakingEvent > >(vm, SqDpVoiceClientSpeakingEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpVoiceClientSpeakingEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceClientSpeakingEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpVoiceClientSpeakingEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Log"),
        Class< DpLogEvent, NoConstructor< DpLogEvent > >(vm, SqDpLogEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpLogEvent::Fn)
        .Func(_SC("_tostring"), &DpLogEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpLogEvent::GetRawEvent)
        .Prop(_SC("Severity"), &DpLogEvent::GetSeverity)
        .Prop(_SC("Message"), &DpLogEvent::GetMessage)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildJoinRequestDelete"),
        Class< DpGuildJoinRequestDeleteEvent, NoConstructor< DpGuildJoinRequestDeleteEvent > >(vm, SqDpGuildJoinRequestDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildJoinRequestDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildJoinRequestDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildJoinRequestDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("InteractionCreate"),
        Class< DpInteractionCreateEvent, NoConstructor< DpInteractionCreateEvent > >(vm, SqDpInteractionCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpInteractionCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpInteractionCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpInteractionCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("SlashCommand"),
        Class< DpSlashCommandEvent, NoConstructor< DpSlashCommandEvent > >(vm, SqDpSlashCommandEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpSlashCommandEvent::Fn)
        .Func(_SC("_tostring"), &DpSlashCommandEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpSlashCommandEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ButtonClick"),
        Class< DpButtonClickEvent, NoConstructor< DpButtonClickEvent > >(vm, SqDpButtonClickEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpButtonClickEvent::Fn)
        .Func(_SC("_tostring"), &DpButtonClickEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpButtonClickEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("AutoComplete"),
        Class< DpAutoCompleteEvent, NoConstructor< DpAutoCompleteEvent > >(vm, SqDpAutoCompleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpAutoCompleteEvent::Fn)
        .Func(_SC("_tostring"), &DpAutoCompleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpAutoCompleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("SelectClick"),
        Class< DpSelectClickEvent, NoConstructor< DpSelectClickEvent > >(vm, SqDpSelectClickEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpSelectClickEvent::Fn)
        .Func(_SC("_tostring"), &DpSelectClickEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpSelectClickEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageContextMenu"),
        Class< DpMessageContextMenuEvent, NoConstructor< DpMessageContextMenuEvent > >(vm, SqDpMessageContextMenuEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpMessageContextMenuEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageContextMenuEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpMessageContextMenuEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("UserContextMenu"),
        Class< DpUserContextMenuEvent, NoConstructor< DpUserContextMenuEvent > >(vm, SqDpUserContextMenuEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpUserContextMenuEvent::Fn)
        .Func(_SC("_tostring"), &DpUserContextMenuEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpUserContextMenuEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("FormSubmit"),
        Class< DpFormSubmitEvent, NoConstructor< DpFormSubmitEvent > >(vm, SqDpFormSubmitEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpFormSubmitEvent::Fn)
        .Func(_SC("_tostring"), &DpFormSubmitEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpFormSubmitEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildDelete"),
        Class< DpGuildDeleteEvent, NoConstructor< DpGuildDeleteEvent > >(vm, SqDpGuildDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ChannelDelete"),
        Class< DpChannelDeleteEvent, NoConstructor< DpChannelDeleteEvent > >(vm, SqDpChannelDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpChannelDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpChannelDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpChannelDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ChannelUpdate"),
        Class< DpChannelUpdateEvent, NoConstructor< DpChannelUpdateEvent > >(vm, SqDpChannelUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpChannelUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpChannelUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpChannelUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Ready"),
        Class< DpReadyEvent, NoConstructor< DpReadyEvent > >(vm, SqDpReadyEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpReadyEvent::Fn)
        .Func(_SC("_tostring"), &DpReadyEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpReadyEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageDelete"),
        Class< DpMessageDeleteEvent, NoConstructor< DpMessageDeleteEvent > >(vm, SqDpMessageDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpMessageDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpMessageDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildMemberRemove"),
        Class< DpGuildMemberRemoveEvent, NoConstructor< DpGuildMemberRemoveEvent > >(vm, SqDpGuildMemberRemoveEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildMemberRemoveEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildMemberRemoveEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildMemberRemoveEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Resumed"),
        Class< DpResumedEvent, NoConstructor< DpResumedEvent > >(vm, SqDpResumedEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpResumedEvent::Fn)
        .Func(_SC("_tostring"), &DpResumedEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpResumedEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildRoleCreate"),
        Class< DpGuildRoleCreateEvent, NoConstructor< DpGuildRoleCreateEvent > >(vm, SqDpGuildRoleCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildRoleCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildRoleCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildRoleCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("TypingStart"),
        Class< DpTypingStartEvent, NoConstructor< DpTypingStartEvent > >(vm, SqDpTypingStartEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpTypingStartEvent::Fn)
        .Func(_SC("_tostring"), &DpTypingStartEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpTypingStartEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageReactionAdd"),
        Class< DpMessageReactionAddEvent, NoConstructor< DpMessageReactionAddEvent > >(vm, SqDpMessageReactionAddEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpMessageReactionAddEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageReactionAddEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpMessageReactionAddEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildMembersChunk"),
        Class< DpGuildMembersChunkEvent, NoConstructor< DpGuildMembersChunkEvent > >(vm, SqDpGuildMembersChunkEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildMembersChunkEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildMembersChunkEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildMembersChunkEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageReactionRemove"),
        Class< DpMessageReactionRemoveEvent, NoConstructor< DpMessageReactionRemoveEvent > >(vm, SqDpMessageReactionRemoveEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpMessageReactionRemoveEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageReactionRemoveEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpMessageReactionRemoveEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildCreate"),
        Class< DpGuildCreateEvent, NoConstructor< DpGuildCreateEvent > >(vm, SqDpGuildCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ChannelCreate"),
        Class< DpChannelCreateEvent, NoConstructor< DpChannelCreateEvent > >(vm, SqDpChannelCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpChannelCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpChannelCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpChannelCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageReactionRemoveEmoji"),
        Class< DpMessageReactionRemoveEmojiEvent, NoConstructor< DpMessageReactionRemoveEmojiEvent > >(vm, SqDpMessageReactionRemoveEmojiEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpMessageReactionRemoveEmojiEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageReactionRemoveEmojiEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpMessageReactionRemoveEmojiEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageDeleteDulk"),
        Class< DpMessageDeleteDulkEvent, NoConstructor< DpMessageDeleteDulkEvent > >(vm, SqDpMessageDeleteDulkEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpMessageDeleteDulkEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageDeleteDulkEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpMessageDeleteDulkEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildRoleUpdate"),
        Class< DpGuildRoleUpdateEvent, NoConstructor< DpGuildRoleUpdateEvent > >(vm, SqDpGuildRoleUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildRoleUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildRoleUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildRoleUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildRoleDelete"),
        Class< DpGuildRoleDeleteEvent, NoConstructor< DpGuildRoleDeleteEvent > >(vm, SqDpGuildRoleDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildRoleDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildRoleDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildRoleDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ChannelPinsUpdate"),
        Class< DpChannelPinsUpdateEvent, NoConstructor< DpChannelPinsUpdateEvent > >(vm, SqDpChannelPinsUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpChannelPinsUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpChannelPinsUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpChannelPinsUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageReactionRemoveAll"),
        Class< DpMessageReactionRemoveAllEvent, NoConstructor< DpMessageReactionRemoveAllEvent > >(vm, SqDpMessageReactionRemoveAllEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpMessageReactionRemoveAllEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageReactionRemoveAllEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpMessageReactionRemoveAllEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceServerUpdate"),
        Class< DpVoiceServerUpdateEvent, NoConstructor< DpVoiceServerUpdateEvent > >(vm, SqDpVoiceServerUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpVoiceServerUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceServerUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpVoiceServerUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildEmojisUpdate"),
        Class< DpGuildEmojisUpdateEvent, NoConstructor< DpGuildEmojisUpdateEvent > >(vm, SqDpGuildEmojisUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildEmojisUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildEmojisUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildEmojisUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildStickersUpdate"),
        Class< DpGuildStickersUpdateEvent, NoConstructor< DpGuildStickersUpdateEvent > >(vm, SqDpGuildStickersUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildStickersUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildStickersUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildStickersUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("PresenceUpdate"),
        Class< DpPresenceUpdateEvent, NoConstructor< DpPresenceUpdateEvent > >(vm, SqDpPresenceUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpPresenceUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpPresenceUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpPresenceUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("WebhooksUpdate"),
        Class< DpWebhooksUpdateEvent, NoConstructor< DpWebhooksUpdateEvent > >(vm, SqDpWebhooksUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpWebhooksUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpWebhooksUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpWebhooksUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("AutomodRuleCreate"),
        Class< DpAutomodRuleCreateEvent, NoConstructor< DpAutomodRuleCreateEvent > >(vm, SqDpAutomodRuleCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpAutomodRuleCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpAutomodRuleCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpAutomodRuleCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("AutomodRuleUpdate"),
        Class< DpAutomodRuleUpdateEvent, NoConstructor< DpAutomodRuleUpdateEvent > >(vm, SqDpAutomodRuleUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpAutomodRuleUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpAutomodRuleUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpAutomodRuleUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("AutomodRuleDelete"),
        Class< DpAutomodRuleDeleteEvent, NoConstructor< DpAutomodRuleDeleteEvent > >(vm, SqDpAutomodRuleDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpAutomodRuleDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpAutomodRuleDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpAutomodRuleDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("AutomodRuleExecute"),
        Class< DpAutomodRuleExecuteEvent, NoConstructor< DpAutomodRuleExecuteEvent > >(vm, SqDpAutomodRuleExecuteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpAutomodRuleExecuteEvent::Fn)
        .Func(_SC("_tostring"), &DpAutomodRuleExecuteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpAutomodRuleExecuteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildMemberAdd"),
        Class< DpGuildMemberAddEvent, NoConstructor< DpGuildMemberAddEvent > >(vm, SqDpGuildMemberAddEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildMemberAddEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildMemberAddEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildMemberAddEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("InviteDelete"),
        Class< DpInviteDeleteEvent, NoConstructor< DpInviteDeleteEvent > >(vm, SqDpInviteDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpInviteDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpInviteDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpInviteDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildUpdate"),
        Class< DpGuildUpdateEvent, NoConstructor< DpGuildUpdateEvent > >(vm, SqDpGuildUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildIntegrationsUpdate"),
        Class< DpGuildIntegrationsUpdateEvent, NoConstructor< DpGuildIntegrationsUpdateEvent > >(vm, SqDpGuildIntegrationsUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildIntegrationsUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildIntegrationsUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildIntegrationsUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildMemberUpdate"),
        Class< DpGuildMemberUpdateEvent, NoConstructor< DpGuildMemberUpdateEvent > >(vm, SqDpGuildMemberUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildMemberUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildMemberUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildMemberUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("InviteCreate"),
        Class< DpInviteCreateEvent, NoConstructor< DpInviteCreateEvent > >(vm, SqDpInviteCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpInviteCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpInviteCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpInviteCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageUpdate"),
        Class< DpMessageUpdateEvent, NoConstructor< DpMessageUpdateEvent > >(vm, SqDpMessageUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpMessageUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpMessageUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("UserUpdate"),
        Class< DpUserUpdateEvent, NoConstructor< DpUserUpdateEvent > >(vm, SqDpUserUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpUserUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpUserUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpUserUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageCreate"),
        Class< DpMessageCreateEvent, NoConstructor< DpMessageCreateEvent > >(vm, SqDpMessageCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpMessageCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpMessageCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildAuditLogEntryCreate"),
        Class< DpGuildAuditLogEntryCreateEvent, NoConstructor< DpGuildAuditLogEntryCreateEvent > >(vm, SqDpGuildAuditLogEntryCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildAuditLogEntryCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildAuditLogEntryCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildAuditLogEntryCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildBanAdd"),
        Class< DpGuildBanAddEvent, NoConstructor< DpGuildBanAddEvent > >(vm, SqDpGuildBanAddEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildBanAddEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildBanAddEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildBanAddEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildBanRemove"),
        Class< DpGuildBanRemoveEvent, NoConstructor< DpGuildBanRemoveEvent > >(vm, SqDpGuildBanRemoveEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildBanRemoveEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildBanRemoveEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildBanRemoveEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("IntegrationCreate"),
        Class< DpIntegrationCreateEvent, NoConstructor< DpIntegrationCreateEvent > >(vm, SqDpIntegrationCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpIntegrationCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpIntegrationCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpIntegrationCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("IntegrationUpdate"),
        Class< DpIntegrationUpdateEvent, NoConstructor< DpIntegrationUpdateEvent > >(vm, SqDpIntegrationUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpIntegrationUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpIntegrationUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpIntegrationUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("IntegrationDelete"),
        Class< DpIntegrationDeleteEvent, NoConstructor< DpIntegrationDeleteEvent > >(vm, SqDpIntegrationDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpIntegrationDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpIntegrationDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpIntegrationDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ThreadCreate"),
        Class< DpThreadCreateEvent, NoConstructor< DpThreadCreateEvent > >(vm, SqDpThreadCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpThreadCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpThreadCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpThreadCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ThreadUpdate"),
        Class< DpThreadUpdateEvent, NoConstructor< DpThreadUpdateEvent > >(vm, SqDpThreadUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpThreadUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpThreadUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpThreadUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ThreadDelete"),
        Class< DpThreadDeleteEvent, NoConstructor< DpThreadDeleteEvent > >(vm, SqDpThreadDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpThreadDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpThreadDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpThreadDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ThreadListSync"),
        Class< DpThreadListSyncEvent, NoConstructor< DpThreadListSyncEvent > >(vm, SqDpThreadListSyncEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpThreadListSyncEvent::Fn)
        .Func(_SC("_tostring"), &DpThreadListSyncEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpThreadListSyncEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ThreadMemberUpdate"),
        Class< DpThreadMemberUpdateEvent, NoConstructor< DpThreadMemberUpdateEvent > >(vm, SqDpThreadMemberUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpThreadMemberUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpThreadMemberUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpThreadMemberUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ThreadMembersUpdate"),
        Class< DpThreadMembersUpdateEvent, NoConstructor< DpThreadMembersUpdateEvent > >(vm, SqDpThreadMembersUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpThreadMembersUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpThreadMembersUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpThreadMembersUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildScheduledEventCreate"),
        Class< DpGuildScheduledEventCreateEvent, NoConstructor< DpGuildScheduledEventCreateEvent > >(vm, SqDpGuildScheduledEventCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildScheduledEventCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildScheduledEventCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildScheduledEventCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildScheduledEventUpdate"),
        Class< DpGuildScheduledEventUpdateEvent, NoConstructor< DpGuildScheduledEventUpdateEvent > >(vm, SqDpGuildScheduledEventUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildScheduledEventUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildScheduledEventUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildScheduledEventUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildScheduledEventDelete"),
        Class< DpGuildScheduledEventDeleteEvent, NoConstructor< DpGuildScheduledEventDeleteEvent > >(vm, SqDpGuildScheduledEventDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildScheduledEventDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildScheduledEventDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildScheduledEventDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildScheduledEventUserAdd"),
        Class< DpGuildScheduledEventUserAddEvent, NoConstructor< DpGuildScheduledEventUserAddEvent > >(vm, SqDpGuildScheduledEventUserAddEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildScheduledEventUserAddEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildScheduledEventUserAddEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildScheduledEventUserAddEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildScheduledEventUserRemove"),
        Class< DpGuildScheduledEventUserRemoveEvent, NoConstructor< DpGuildScheduledEventUserRemoveEvent > >(vm, SqDpGuildScheduledEventUserRemoveEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildScheduledEventUserRemoveEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildScheduledEventUserRemoveEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpGuildScheduledEventUserRemoveEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceBufferSend"),
        Class< DpVoiceBufferSendEvent, NoConstructor< DpVoiceBufferSendEvent > >(vm, SqDpVoiceBufferSendEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpVoiceBufferSendEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceBufferSendEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpVoiceBufferSendEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceUserTalking"),
        Class< DpVoiceUserTalkingEvent, NoConstructor< DpVoiceUserTalkingEvent > >(vm, SqDpVoiceUserTalkingEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpVoiceUserTalkingEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceUserTalkingEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpVoiceUserTalkingEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceReady"),
        Class< DpVoiceReadyEvent, NoConstructor< DpVoiceReadyEvent > >(vm, SqDpVoiceReadyEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpVoiceReadyEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceReadyEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpVoiceReadyEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceReceive"),
        Class< DpVoiceReceiveEvent, NoConstructor< DpVoiceReceiveEvent > >(vm, SqDpVoiceReceiveEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpVoiceReceiveEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceReceiveEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpVoiceReceiveEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceReceive"),
        Class< DpVoiceReceiveCombinedEvent, NoConstructor< DpVoiceReceiveCombinedEvent > >(vm, SqDpVoiceReceiveCombinedEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpVoiceReceiveCombinedEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceReceiveCombinedEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpVoiceReceiveCombinedEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceTrackMarker"),
        Class< DpVoiceTrackMarkerEvent, NoConstructor< DpVoiceTrackMarkerEvent > >(vm, SqDpVoiceTrackMarkerEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpVoiceTrackMarkerEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceTrackMarkerEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpVoiceTrackMarkerEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("StageInstanceCreate"),
        Class< DpStageInstanceCreateEvent, NoConstructor< DpStageInstanceCreateEvent > >(vm, SqDpStageInstanceCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpStageInstanceCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpStageInstanceCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpStageInstanceCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("StageInstanceUpdate"),
        Class< DpStageInstanceUpdateEvent, NoConstructor< DpStageInstanceUpdateEvent > >(vm, SqDpStageInstanceUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpStageInstanceUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpStageInstanceUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpStageInstanceUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("StageInstanceDelete"),
        Class< DpStageInstanceDeleteEvent, NoConstructor< DpStageInstanceDeleteEvent > >(vm, SqDpStageInstanceDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpStageInstanceDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpStageInstanceDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("Raw"), &DpStageInstanceDeleteEvent::GetRawEvent)
    );
}

} // Namespace:: SqMod
