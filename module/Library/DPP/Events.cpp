// ------------------------------------------------------------------------------------------------
#include "Library/DPP/Events.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDppVoiceStateUpdateEvent, _SC("SqDppVoiceStateUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppLogEvent, _SC("SqDppLogEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildJoinRequestDeleteEvent, _SC("SqDppGuildJoinRequestDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDppInteractionCreateEvent, _SC("SqDppInteractionCreateEvent"))
SQMOD_DECL_TYPENAME(SqDppButtonClickEvent, _SC("SqDppButtonClickEvent"))
SQMOD_DECL_TYPENAME(SqDppSelectClickEvent, _SC("SqDppSelectClickEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildDeleteEvent, _SC("SqDppGuildDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDppChannelDeleteEvent, _SC("SqDppChannelDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDppChannelUpdateEvent, _SC("SqDppChannelUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppReadyEvent, _SC("SqDppReadyEvent"))
SQMOD_DECL_TYPENAME(SqDppMessageDeleteEvent, _SC("SqDppMessageDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDppApplicationCommandDeleteEvent, _SC("SqDppApplicationCommandDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildMemberRemoveEvent, _SC("SqDppGuildMemberRemoveEvent"))
SQMOD_DECL_TYPENAME(SqDppApplicationCommandCreateEvent, _SC("SqDppApplicationCommandCreateEvent"))
SQMOD_DECL_TYPENAME(SqDppResumedEvent, _SC("SqDppResumedEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildRoleCreateEvent, _SC("SqDppGuildRoleCreateEvent"))
SQMOD_DECL_TYPENAME(SqDppTypingStartEvent, _SC("SqDppTypingStartEvent"))
SQMOD_DECL_TYPENAME(SqDppMessageReactionAddEvent, _SC("SqDppMessageReactionAddEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildMembersChunkEvent, _SC("SqDppGuildMembersChunkEvent"))
SQMOD_DECL_TYPENAME(SqDppMessageReactionRemoveEvent, _SC("SqDppMessageReactionRemoveEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildCreateEvent, _SC("SqDppGuildCreateEvent"))
SQMOD_DECL_TYPENAME(SqDppChannelCreateEvent, _SC("SqDppChannelCreateEvent"))
SQMOD_DECL_TYPENAME(SqDppMessageReactionRemoveEmojiEvent, _SC("SqDppMessageReactionRemoveEmojiEvent"))
SQMOD_DECL_TYPENAME(SqDppMessageDeleteBulkEvent, _SC("SqDppMessageDeleteBulkEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildRoleUpdateEvent, _SC("SqDppGuildRoleUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildRoleDeleteEvent, _SC("SqDppGuildRoleDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDppChannelPinsUpdateEvent, _SC("SqDppChannelPinsUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppMessageReactionRemoveAllEvent, _SC("SqDppMessageReactionRemoveAllEvent"))
SQMOD_DECL_TYPENAME(SqDppVoiceServerUpdateEvent, _SC("SqDppVoiceServerUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildEmojisUpdateEvent, _SC("SqDppGuildEmojisUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildStickersUpdateEvent, _SC("SqDppGuildStickersUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppPresenceUpdateEvent, _SC("SqDppPresenceUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppWebhooksUpdateEvent, _SC("SqDppWebhooksUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildMemberAddEvent, _SC("SqDppGuildMemberAddEvent"))
SQMOD_DECL_TYPENAME(SqDppInviteDeleteEvent, _SC("SqDppInviteDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildUpdateEvent, _SC("SqDppGuildUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildIntegrationsUpdateEvent, _SC("SqDppGuildIntegrationsUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildMemberUpdateEvent, _SC("SqDppGuildMemberUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppApplicationCommandUpdateEvent, _SC("SqDppApplicationCommandUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppInviteCreateEvent, _SC("SqDppInviteCreateEvent"))
SQMOD_DECL_TYPENAME(SqDppMessageUpdateEvent, _SC("SqDppMessageUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppUserUpdateEvent, _SC("SqDppUserUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppMessageCreateEvent, _SC("SqDppMessageCreateEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildBanAddEvent, _SC("SqDppGuildBanAddEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildBanRemoveEvent, _SC("SqDppGuildBanRemoveEvent"))
SQMOD_DECL_TYPENAME(SqDppIntegrationCreateEvent, _SC("SqDppIntegrationCreateEvent"))
SQMOD_DECL_TYPENAME(SqDppIntegrationUpdateEvent, _SC("SqDppIntegrationUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppIntegrationDeleteEvent, _SC("SqDppIntegrationDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDppThreadCreateEvent, _SC("SqDppThreadCreateEvent"))
SQMOD_DECL_TYPENAME(SqDppThreadUpdateEvent, _SC("SqDppThreadUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppThreadDeleteEvent, _SC("SqDppThreadDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDppThreadListSyncEvent, _SC("SqDppThreadListSyncEvent"))
SQMOD_DECL_TYPENAME(SqDppThreadMemberUpdateEvent, _SC("SqDppThreadMemberUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppThreadMembersUpdateEvent, _SC("SqDppThreadMembersUpdateEvent"))
SQMOD_DECL_TYPENAME(SqDppVoiceBufferSendEvent, _SC("SqDppVoiceBufferSendEvent"))
SQMOD_DECL_TYPENAME(SqDppVoiceUserTalkingEvent, _SC("SqDppVoiceUserTalkingEvent"))
SQMOD_DECL_TYPENAME(SqDppVoiceReadyEvent, _SC("SqDppVoiceReadyEvent"))
SQMOD_DECL_TYPENAME(SqDppVoiceReceiveEvent, _SC("SqDppVoiceReceiveEvent"))
SQMOD_DECL_TYPENAME(SqDppVoiceTrackMarkerEvent, _SC("SqDppVoiceTrackMarkerEvent"))
SQMOD_DECL_TYPENAME(SqDppStageInstanceCreateEvent, _SC("SqDppStageInstanceCreateEvent"))
SQMOD_DECL_TYPENAME(SqDppStageInstanceDeleteEvent, _SC("SqDppStageInstanceDeleteEvent"))

// ------------------------------------------------------------------------------------------------
void Register_DPP_Events(HSQUIRRELVM vm, Table & ns)
{
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceStateUpdate"),
        Class< DpVoiceStateUpdateEvent, NoConstructor< DpVoiceStateUpdateEvent > >(vm, SqDppVoiceStateUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppVoiceStateUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceStateUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("State"), &DpVoiceStateUpdateEvent::GetState)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Log"),
        Class< DpLogEvent, NoConstructor< DpLogEvent > >(vm, SqDppLogEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppLogEvent::Fn)
        .Func(_SC("_tostring"), &DpLogEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpLogEvent::GetRawEvent)
        .Prop(_SC("Severity"), &DpLogEvent::GetSeverity)
        .Prop(_SC("Message"), &DpLogEvent::GetMessage)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildJoinRequestDelete"),
        Class< DpGuildJoinRequestDeleteEvent, NoConstructor< DpGuildJoinRequestDeleteEvent > >(vm, SqDppGuildJoinRequestDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildJoinRequestDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildJoinRequestDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpGuildJoinRequestDeleteEvent::GetRawEvent)
        .Prop(_SC("GuildID"), &DpGuildJoinRequestDeleteEvent::GetGuildID)
        .Prop(_SC("UserID"), &DpGuildJoinRequestDeleteEvent::GetUserID)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("InteractionCreate"),
        Class< DpInteractionCreateEvent, NoConstructor< DpInteractionCreateEvent > >(vm, SqDppInteractionCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppInteractionCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpInteractionCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpInteractionCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ButtonClick"),
        Class< DpButtonClickEvent, NoConstructor< DpButtonClickEvent > >(vm, SqDppButtonClickEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppButtonClickEvent::Fn)
        .Func(_SC("_tostring"), &DpButtonClickEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpButtonClickEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("SelectClick"),
        Class< DpSelectClickEvent, NoConstructor< DpSelectClickEvent > >(vm, SqDppSelectClickEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppSelectClickEvent::Fn)
        .Func(_SC("_tostring"), &DpSelectClickEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpSelectClickEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildDelete"),
        Class< DpGuildDeleteEvent, NoConstructor< DpGuildDeleteEvent > >(vm, SqDppGuildDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpGuildDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ChannelDelete"),
        Class< DpChannelDeleteEvent, NoConstructor< DpChannelDeleteEvent > >(vm, SqDppChannelDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppChannelDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpChannelDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpChannelDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ChannelUpdate"),
        Class< DpChannelUpdateEvent, NoConstructor< DpChannelUpdateEvent > >(vm, SqDppChannelUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppChannelUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpChannelUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpChannelUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Ready"),
        Class< DpReadyEvent, NoConstructor< DpReadyEvent > >(vm, SqDppReadyEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppReadyEvent::Fn)
        .Func(_SC("_tostring"), &DpReadyEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpReadyEvent::GetRawEvent)
        .Prop(_SC("SessionID"), &DpReadyEvent::GetSessionID)
        .Prop(_SC("ShardID"), &DpReadyEvent::GetShardID)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageDelete"),
        Class< DpMessageDeleteEvent, NoConstructor< DpMessageDeleteEvent > >(vm, SqDppMessageDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppMessageDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpMessageDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ApplicationCommandDelete"),
        Class< DpApplicationCommandDeleteEvent, NoConstructor< DpApplicationCommandDeleteEvent > >(vm, SqDppApplicationCommandDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppApplicationCommandDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpApplicationCommandDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpApplicationCommandDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildMemberRemove"),
        Class< DpGuildMemberRemoveEvent, NoConstructor< DpGuildMemberRemoveEvent > >(vm, SqDppGuildMemberRemoveEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildMemberRemoveEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildMemberRemoveEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpGuildMemberRemoveEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ApplicationCommandCreate"),
        Class< DpApplicationCommandCreateEvent, NoConstructor< DpApplicationCommandCreateEvent > >(vm, SqDppApplicationCommandCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppApplicationCommandCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpApplicationCommandCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpApplicationCommandCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Resumed"),
        Class< DpResumedEvent, NoConstructor< DpResumedEvent > >(vm, SqDppResumedEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppResumedEvent::Fn)
        .Func(_SC("_tostring"), &DpResumedEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpResumedEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildRoleCreate"),
        Class< DpGuildRoleCreateEvent, NoConstructor< DpGuildRoleCreateEvent > >(vm, SqDppGuildRoleCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildRoleCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildRoleCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpGuildRoleCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("TypingStart"),
        Class< DpTypingStartEvent, NoConstructor< DpTypingStartEvent > >(vm, SqDppTypingStartEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppTypingStartEvent::Fn)
        .Func(_SC("_tostring"), &DpTypingStartEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpTypingStartEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageReactionAdd"),
        Class< DpMessageReactionAddEvent, NoConstructor< DpMessageReactionAddEvent > >(vm, SqDppMessageReactionAddEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppMessageReactionAddEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageReactionAddEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpMessageReactionAddEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildMembersChunk"),
        Class< DpGuildMembersChunkEvent, NoConstructor< DpGuildMembersChunkEvent > >(vm, SqDppGuildMembersChunkEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildMembersChunkEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildMembersChunkEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpGuildMembersChunkEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageReactionRemove"),
        Class< DpMessageReactionRemoveEvent, NoConstructor< DpMessageReactionRemoveEvent > >(vm, SqDppMessageReactionRemoveEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppMessageReactionRemoveEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageReactionRemoveEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpMessageReactionRemoveEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildCreate"),
        Class< DpGuildCreateEvent, NoConstructor< DpGuildCreateEvent > >(vm, SqDppGuildCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpGuildCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ChannelCreate"),
        Class< DpChannelCreateEvent, NoConstructor< DpChannelCreateEvent > >(vm, SqDppChannelCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppChannelCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpChannelCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpChannelCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageReactionRemoveEmoji"),
        Class< DpMessageReactionRemoveEmojiEvent, NoConstructor< DpMessageReactionRemoveEmojiEvent > >(vm, SqDppMessageReactionRemoveEmojiEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppMessageReactionRemoveEmojiEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageReactionRemoveEmojiEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpMessageReactionRemoveEmojiEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageDeleteBulk"),
        Class< DpMessageDeleteBulkEvent, NoConstructor< DpMessageDeleteBulkEvent > >(vm, SqDppMessageDeleteBulkEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppMessageDeleteBulkEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageDeleteBulkEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpMessageDeleteBulkEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildRoleUpdate"),
        Class< DpGuildRoleUpdateEvent, NoConstructor< DpGuildRoleUpdateEvent > >(vm, SqDppGuildRoleUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildRoleUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildRoleUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpGuildRoleUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildRoleDelete"),
        Class< DpGuildRoleDeleteEvent, NoConstructor< DpGuildRoleDeleteEvent > >(vm, SqDppGuildRoleDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildRoleDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildRoleDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpGuildRoleDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ChannelPinsUpdate"),
        Class< DpChannelPinsUpdateEvent, NoConstructor< DpChannelPinsUpdateEvent > >(vm, SqDppChannelPinsUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppChannelPinsUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpChannelPinsUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpChannelPinsUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageReactionRemoveAll"),
        Class< DpMessageReactionRemoveAllEvent, NoConstructor< DpMessageReactionRemoveAllEvent > >(vm, SqDppMessageReactionRemoveAllEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppMessageReactionRemoveAllEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageReactionRemoveAllEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpMessageReactionRemoveAllEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceServerUpdate"),
        Class< DpVoiceServerUpdateEvent, NoConstructor< DpVoiceServerUpdateEvent > >(vm, SqDppVoiceServerUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppVoiceServerUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceServerUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpVoiceServerUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildEmojisUpdate"),
        Class< DpGuildEmojisUpdateEvent, NoConstructor< DpGuildEmojisUpdateEvent > >(vm, SqDppGuildEmojisUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildEmojisUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildEmojisUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpGuildEmojisUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildStickersUpdate"),
        Class< DpGuildStickersUpdateEvent, NoConstructor< DpGuildStickersUpdateEvent > >(vm, SqDppGuildStickersUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildStickersUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildStickersUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpGuildStickersUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("PresenceUpdate"),
        Class< DpPresenceUpdateEvent, NoConstructor< DpPresenceUpdateEvent > >(vm, SqDppPresenceUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppPresenceUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpPresenceUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpPresenceUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("WebhooksUpdate"),
        Class< DpWebhooksUpdateEvent, NoConstructor< DpWebhooksUpdateEvent > >(vm, SqDppWebhooksUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppWebhooksUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpWebhooksUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpWebhooksUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildMemberAdd"),
        Class< DpGuildMemberAddEvent, NoConstructor< DpGuildMemberAddEvent > >(vm, SqDppGuildMemberAddEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildMemberAddEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildMemberAddEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpGuildMemberAddEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("InviteDelete"),
        Class< DpInviteDeleteEvent, NoConstructor< DpInviteDeleteEvent > >(vm, SqDppInviteDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppInviteDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpInviteDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpInviteDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildUpdate"),
        Class< DpGuildUpdateEvent, NoConstructor< DpGuildUpdateEvent > >(vm, SqDppGuildUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpGuildUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildIntegrationsUpdate"),
        Class< DpGuildIntegrationsUpdateEvent, NoConstructor< DpGuildIntegrationsUpdateEvent > >(vm, SqDppGuildIntegrationsUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildIntegrationsUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildIntegrationsUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpGuildIntegrationsUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildMemberUpdate"),
        Class< DpGuildMemberUpdateEvent, NoConstructor< DpGuildMemberUpdateEvent > >(vm, SqDppGuildMemberUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildMemberUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildMemberUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpGuildMemberUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ApplicationCommandUpdate"),
        Class< DpApplicationCommandUpdateEvent, NoConstructor< DpApplicationCommandUpdateEvent > >(vm, SqDppApplicationCommandUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppApplicationCommandUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpApplicationCommandUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpApplicationCommandUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("InviteCreate"),
        Class< DpInviteCreateEvent, NoConstructor< DpInviteCreateEvent > >(vm, SqDppInviteCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppInviteCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpInviteCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpInviteCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageUpdate"),
        Class< DpMessageUpdateEvent, NoConstructor< DpMessageUpdateEvent > >(vm, SqDppMessageUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppMessageUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpMessageUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("UserUpdate"),
        Class< DpUserUpdateEvent, NoConstructor< DpUserUpdateEvent > >(vm, SqDppUserUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppUserUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpUserUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpUserUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("MessageCreate"),
        Class< DpMessageCreateEvent, NoConstructor< DpMessageCreateEvent > >(vm, SqDppMessageCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppMessageCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpMessageCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpMessageCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildBanAdd"),
        Class< DpGuildBanAddEvent, NoConstructor< DpGuildBanAddEvent > >(vm, SqDppGuildBanAddEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildBanAddEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildBanAddEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpGuildBanAddEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("GuildBanRemove"),
        Class< DpGuildBanRemoveEvent, NoConstructor< DpGuildBanRemoveEvent > >(vm, SqDppGuildBanRemoveEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildBanRemoveEvent::Fn)
        .Func(_SC("_tostring"), &DpGuildBanRemoveEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpGuildBanRemoveEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("IntegrationCreate"),
        Class< DpIntegrationCreateEvent, NoConstructor< DpIntegrationCreateEvent > >(vm, SqDppIntegrationCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppIntegrationCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpIntegrationCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpIntegrationCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("IntegrationUpdate"),
        Class< DpIntegrationUpdateEvent, NoConstructor< DpIntegrationUpdateEvent > >(vm, SqDppIntegrationUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppIntegrationUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpIntegrationUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpIntegrationUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("IntegrationDelete"),
        Class< DpIntegrationDeleteEvent, NoConstructor< DpIntegrationDeleteEvent > >(vm, SqDppIntegrationDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppIntegrationDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpIntegrationDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpIntegrationDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ThreadCreate"),
        Class< DpThreadCreateEvent, NoConstructor< DpThreadCreateEvent > >(vm, SqDppThreadCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppThreadCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpThreadCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpThreadCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ThreadUpdate"),
        Class< DpThreadUpdateEvent, NoConstructor< DpThreadUpdateEvent > >(vm, SqDppThreadUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppThreadUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpThreadUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpThreadUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ThreadDelete"),
        Class< DpThreadDeleteEvent, NoConstructor< DpThreadDeleteEvent > >(vm, SqDppThreadDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppThreadDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpThreadDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpThreadDeleteEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ThreadListSync"),
        Class< DpThreadListSyncEvent, NoConstructor< DpThreadListSyncEvent > >(vm, SqDppThreadListSyncEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppThreadListSyncEvent::Fn)
        .Func(_SC("_tostring"), &DpThreadListSyncEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpThreadListSyncEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ThreadMemberUpdate"),
        Class< DpThreadMemberUpdateEvent, NoConstructor< DpThreadMemberUpdateEvent > >(vm, SqDppThreadMemberUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppThreadMemberUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpThreadMemberUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpThreadMemberUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ThreadMembersUpdate"),
        Class< DpThreadMembersUpdateEvent, NoConstructor< DpThreadMembersUpdateEvent > >(vm, SqDppThreadMembersUpdateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppThreadMembersUpdateEvent::Fn)
        .Func(_SC("_tostring"), &DpThreadMembersUpdateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpThreadMembersUpdateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceBufferSend"),
        Class< DpVoiceBufferSendEvent, NoConstructor< DpVoiceBufferSendEvent > >(vm, SqDppVoiceBufferSendEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppVoiceBufferSendEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceBufferSendEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpVoiceBufferSendEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceUserTalking"),
        Class< DpVoiceUserTalkingEvent, NoConstructor< DpVoiceUserTalkingEvent > >(vm, SqDppVoiceUserTalkingEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppVoiceUserTalkingEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceUserTalkingEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpVoiceUserTalkingEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceReady"),
        Class< DpVoiceReadyEvent, NoConstructor< DpVoiceReadyEvent > >(vm, SqDppVoiceReadyEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppVoiceReadyEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceReadyEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpVoiceReadyEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceReceive"),
        Class< DpVoiceReceiveEvent, NoConstructor< DpVoiceReceiveEvent > >(vm, SqDppVoiceReceiveEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppVoiceReceiveEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceReceiveEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpVoiceReceiveEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceTrackMarker"),
        Class< DpVoiceTrackMarkerEvent, NoConstructor< DpVoiceTrackMarkerEvent > >(vm, SqDppVoiceTrackMarkerEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppVoiceTrackMarkerEvent::Fn)
        .Func(_SC("_tostring"), &DpVoiceTrackMarkerEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpVoiceTrackMarkerEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("StageInstanceCreate"),
        Class< DpStageInstanceCreateEvent, NoConstructor< DpStageInstanceCreateEvent > >(vm, SqDppStageInstanceCreateEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppStageInstanceCreateEvent::Fn)
        .Func(_SC("_tostring"), &DpStageInstanceCreateEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpStageInstanceCreateEvent::GetRawEvent)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("StageInstanceDelete"),
        Class< DpStageInstanceDeleteEvent, NoConstructor< DpStageInstanceDeleteEvent > >(vm, SqDppStageInstanceDeleteEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppStageInstanceDeleteEvent::Fn)
        .Func(_SC("_tostring"), &DpStageInstanceDeleteEvent::GetRawEvent)
        // Member Properties
        .Prop(_SC("RawEvent"), &DpStageInstanceDeleteEvent::GetRawEvent)
    );
}

} // Namespace:: SqMod
