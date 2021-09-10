// ------------------------------------------------------------------------------------------------
#include "Library/DPPEv.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDppVoiceStateUpdateEventEvent, _SC("SqDppVoiceStateUpdateEventEvent"))
SQMOD_DECL_TYPENAME(SqDppGuildJoinRequestDeleteEvent, _SC("SqDppGuildJoinRequestDeleteEvent"))
SQMOD_DECL_TYPENAME(SqDppLogEvent, _SC("SqDppLogEvent"))
SQMOD_DECL_TYPENAME(SqDppReadyEvent, _SC("SqDppReadyEvent"))

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
void Register_DPPEv(HSQUIRRELVM vm, Table & ns)
{
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceStateUpdate"),
        Class< DpVoiceStateUpdateEvent, NoConstructor< DpVoiceStateUpdateEvent > >(vm, SqDppVoiceStateUpdateEventEvent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppVoiceStateUpdateEventEvent::Fn)
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
    ConstTable(vm).Enum(_SC("SqDiscordEvent"), Enumeration(vm)
        .Const(_SC("VoiceStateUpdate"),             static_cast< SQInteger >(DpEventID::VoiceStateUpdate))
        .Const(_SC("Log"),                          static_cast< SQInteger >(DpEventID::Log))
        .Const(_SC("GuildJoinRequestDelete"),       static_cast< SQInteger >(DpEventID::GuildJoinRequestDelete))
        .Const(_SC("InteractionCreate"),            static_cast< SQInteger >(DpEventID::InteractionCreate))
        .Const(_SC("ButtonClick"),                  static_cast< SQInteger >(DpEventID::ButtonClick))
        .Const(_SC("SelectClick"),                  static_cast< SQInteger >(DpEventID::SelectClick))
        .Const(_SC("GuildDelete"),                  static_cast< SQInteger >(DpEventID::GuildDelete))
        .Const(_SC("ChannelDelete"),                static_cast< SQInteger >(DpEventID::ChannelDelete))
        .Const(_SC("ChannelUpdate"),                static_cast< SQInteger >(DpEventID::ChannelUpdate))
        .Const(_SC("Ready"),                        static_cast< SQInteger >(DpEventID::Ready))
        .Const(_SC("MessageDelete"),                static_cast< SQInteger >(DpEventID::MessageDelete))
        .Const(_SC("ApplicationCommandDelete"),     static_cast< SQInteger >(DpEventID::ApplicationCommandDelete))
        .Const(_SC("GuildMemberRemove"),            static_cast< SQInteger >(DpEventID::GuildMemberRemove))
        .Const(_SC("ApplicationCommandCreate"),     static_cast< SQInteger >(DpEventID::ApplicationCommandCreate))
        .Const(_SC("Resumed"),                      static_cast< SQInteger >(DpEventID::Resumed))
        .Const(_SC("GuildRoleCreate"),              static_cast< SQInteger >(DpEventID::GuildRoleCreate))
        .Const(_SC("TypingStart"),                  static_cast< SQInteger >(DpEventID::TypingStart))
        .Const(_SC("MessageReactionAdd"),           static_cast< SQInteger >(DpEventID::MessageReactionAdd))
        .Const(_SC("GuildMembersChunk"),            static_cast< SQInteger >(DpEventID::GuildMembersChunk))
        .Const(_SC("MessageReactionRemove"),        static_cast< SQInteger >(DpEventID::MessageReactionRemove))
        .Const(_SC("GuildCreate"),                  static_cast< SQInteger >(DpEventID::GuildCreate))
        .Const(_SC("ChannelCreate"),                static_cast< SQInteger >(DpEventID::ChannelCreate))
        .Const(_SC("MessageReactionRemoveEmoji"),   static_cast< SQInteger >(DpEventID::MessageReactionRemoveEmoji))
        .Const(_SC("MessageDeleteBulk"),            static_cast< SQInteger >(DpEventID::MessageDeleteBulk))
        .Const(_SC("GuildRoleUpdate"),              static_cast< SQInteger >(DpEventID::GuildRoleUpdate))
        .Const(_SC("GuildRoleDelete"),              static_cast< SQInteger >(DpEventID::GuildRoleDelete))
        .Const(_SC("ChannelPinsUpdate"),            static_cast< SQInteger >(DpEventID::ChannelPinsUpdate))
        .Const(_SC("MessageReactionRemoveAll"),     static_cast< SQInteger >(DpEventID::MessageReactionRemoveAll))
        .Const(_SC("VoiceServerUpdate"),            static_cast< SQInteger >(DpEventID::VoiceServerUpdate))
        .Const(_SC("GuildEmojisUpdate"),            static_cast< SQInteger >(DpEventID::GuildEmojisUpdate))
        .Const(_SC("GuildStickersUpdate"),          static_cast< SQInteger >(DpEventID::GuildStickersUpdate))
        .Const(_SC("PresenceUpdate"),               static_cast< SQInteger >(DpEventID::PresenceUpdate))
        .Const(_SC("WebhooksUpdate"),               static_cast< SQInteger >(DpEventID::WebhooksUpdate))
        .Const(_SC("GuildMemberAdd"),               static_cast< SQInteger >(DpEventID::GuildMemberAdd))
        .Const(_SC("InviteDelete"),                 static_cast< SQInteger >(DpEventID::InviteDelete))
        .Const(_SC("GuildUpdate"),                  static_cast< SQInteger >(DpEventID::GuildUpdate))
        .Const(_SC("GuildIntegrationsUpdate"),      static_cast< SQInteger >(DpEventID::GuildIntegrationsUpdate))
        .Const(_SC("GuildMemberUpdate"),            static_cast< SQInteger >(DpEventID::GuildMemberUpdate))
        .Const(_SC("ApplicationCommandUpdate"),     static_cast< SQInteger >(DpEventID::ApplicationCommandUpdate))
        .Const(_SC("InviteCreate"),                 static_cast< SQInteger >(DpEventID::InviteCreate))
        .Const(_SC("MessageUpdate"),                static_cast< SQInteger >(DpEventID::MessageUpdate))
        .Const(_SC("UserUpdate"),                   static_cast< SQInteger >(DpEventID::UserUpdate))
        .Const(_SC("MessageCreate"),                static_cast< SQInteger >(DpEventID::MessageCreate))
        .Const(_SC("GuildBanAdd"),                  static_cast< SQInteger >(DpEventID::GuildBanAdd))
        .Const(_SC("GuildBanRemove"),               static_cast< SQInteger >(DpEventID::GuildBanRemove))
        .Const(_SC("IntegrationCreate"),            static_cast< SQInteger >(DpEventID::IntegrationCreate))
        .Const(_SC("IntegrationUpdate"),            static_cast< SQInteger >(DpEventID::IntegrationUpdate))
        .Const(_SC("IntegrationDelete"),            static_cast< SQInteger >(DpEventID::IntegrationDelete))
        .Const(_SC("ThreadCreate"),                 static_cast< SQInteger >(DpEventID::ThreadCreate))
        .Const(_SC("ThreadUpdate"),                 static_cast< SQInteger >(DpEventID::ThreadUpdate))
        .Const(_SC("ThreadDelete"),                 static_cast< SQInteger >(DpEventID::ThreadDelete))
        .Const(_SC("ThreadListSync"),               static_cast< SQInteger >(DpEventID::ThreadListSync))
        .Const(_SC("ThreadMemberUpdate"),           static_cast< SQInteger >(DpEventID::ThreadMemberUpdate))
        .Const(_SC("ThreadMembersUpdate"),          static_cast< SQInteger >(DpEventID::ThreadMembersUpdate))
        .Const(_SC("VoiceBufferSend"),              static_cast< SQInteger >(DpEventID::VoiceBufferSend))
        .Const(_SC("VoiceUserTalking"),             static_cast< SQInteger >(DpEventID::VoiceUserTalking))
        .Const(_SC("VoiceReady"),                   static_cast< SQInteger >(DpEventID::VoiceReady))
        .Const(_SC("VoiceReceive"),                 static_cast< SQInteger >(DpEventID::VoiceReceive))
        .Const(_SC("VoiceTrackMarker"),             static_cast< SQInteger >(DpEventID::VoiceTrackMarker))
        .Const(_SC("StageInstanceCreate"),          static_cast< SQInteger >(DpEventID::StageInstanceCreate))
        .Const(_SC("StageInstanceDelete"),          static_cast< SQInteger >(DpEventID::StageInstanceDelete))
        .Const(_SC("Max"),                          static_cast< SQInteger >(DpEventID::Max))
    );
}

} // Namespace:: SqMod
