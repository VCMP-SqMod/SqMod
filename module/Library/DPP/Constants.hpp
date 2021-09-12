#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <dpp/dpp.h>

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
        Log,
        GuildJoinRequestDelete,
        InteractionCreate,
        ButtonClick,
        SelectClick,
        GuildDelete,
        ChannelDelete,
        ChannelUpdate,
        Ready,
        MessageDelete,
        ApplicationCommandDelete,
        GuildMemberRemove,
        ApplicationCommandCreate,
        Resumed,
        GuildRoleCreate,
        TypingStart,
        MessageReactionAdd,
        GuildMembersChunk,
        MessageReactionRemove,
        GuildCreate,
        ChannelCreate,
        MessageReactionRemoveEmoji,
        MessageDeleteBulk,
        GuildRoleUpdate,
        GuildRoleDelete,
        ChannelPinsUpdate,
        MessageReactionRemoveAll,
        VoiceServerUpdate,
        GuildEmojisUpdate,
        GuildStickersUpdate,
        PresenceUpdate,
        WebhooksUpdate,
        GuildMemberAdd,
        InviteDelete,
        GuildUpdate,
        GuildIntegrationsUpdate,
        GuildMemberUpdate,
        ApplicationCommandUpdate,
        InviteCreate,
        MessageUpdate,
        UserUpdate,
        MessageCreate,
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
        VoiceBufferSend,
        VoiceUserTalking,
        VoiceReady,
        VoiceReceive,
        VoiceTrackMarker,
        StageInstanceCreate,
        StageInstanceDelete,
        Max
    };
    /* --------------------------------------------------------------------------------------------
     * String identification for each event ID.
    */
    static const std::array< const char *, static_cast< size_t >(Max) > NAME;
};

} // Namespace:: SqMod
