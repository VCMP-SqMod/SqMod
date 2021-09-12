// ------------------------------------------------------------------------------------------------
#include "Library/DPP/Cluster.hpp"
#include "Library/DPP/Events.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDppCluster, _SC("SqDppCluster"))

// ------------------------------------------------------------------------------------------------
void Register_DPP_Cluster(HSQUIRRELVM vm, Table & ns)
{
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Cluster"),
        Class< DpCluster, NoCopy< DpCluster > >(vm, SqDppCluster::Str)
        // Constructors
        .Ctor< StackStrF & >()
        .Ctor< StackStrF &, SQInteger >()
        .Ctor< StackStrF &, SQInteger, SQInteger >()
        .Ctor< StackStrF &, SQInteger, SQInteger, SQInteger >()
        .Ctor< StackStrF &, SQInteger, SQInteger, SQInteger, SQInteger >()
        .Ctor< StackStrF &, SQInteger, SQInteger, SQInteger, SQInteger, bool >()
        .Ctor< StackStrF &, SQInteger, SQInteger, SQInteger, SQInteger, bool, const DpCachePolicy & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppCluster::Fn)
        // Member Properties
        .Prop(_SC("On"), &DpCluster::GetEvents)
        .Prop(_SC("UpTime"), &DpCluster::UpTime)
        // Member Methods
        .Func(_SC("Start"), &DpCluster::Start)
        .Func(_SC("Log"), &DpCluster::Log)
        .Func(_SC("GetDmChannel"), &DpCluster::GetDmChannel)
        .Func(_SC("SetDmChannel"), &DpCluster::SetDmChannel)
        .Func(_SC("SetPresence"), &DpCluster::SetPresence)
        .Func(_SC("EnableEvent"), &DpCluster::EnableEvent)
        .Func(_SC("DisableEvent"), &DpCluster::DisableEvent)
    );
}

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD LightObj EventToScriptObject(uint8_t type, uintptr_t data);
void EventInvokeCleanup(uint8_t type, uintptr_t data);

// ------------------------------------------------------------------------------------------------
void DpCluster::Process(bool force)
{
    // Is there a valid connection?
    if (!mC && !force)
    {
        return; // No point in going forward
    }
    DpInternalEvent event;
    // Retrieve each event individually and process it
    for (size_t count = mQueue.size_approx(), n = 0; n <= count; ++n)
    {
        // Try to get an event from the queue
        if (mQueue.try_dequeue(event))
        {
            // Fetch the type of event
            const auto type = event.GetType();
            // Fetch the event itself
            const auto data = event.GetData();
            // Is this a valid event and is anyone listening to it?
            if (event.mData == 0 || mEvents[type].first == nullptr || mEvents[type].first->IsEmpty())
            {
                continue; // Move on
            }
            // Transform the event instance into a script object
            LightObj obj = EventToScriptObject(type, data);
            // Allow the script to take ownership of the event instance now
            event.Reset();
            // Forward the call to the associated signal
            (*mEvents[type].first)(obj);
            // Allow the event instance to clean itself
            EventInvokeCleanup(type, data);
        }
    }
}

/* ================================================================================================
 * Event handlers.
*/
void DpCluster::OnVoiceStateUpdate(const dpp::voice_state_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::VoiceStateUpdate, new DpVoiceStateUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnLog(const dpp::log_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::Log, new DpLogEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildJoinRequestDelete(const dpp::guild_join_request_delete_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::GuildJoinRequestDelete, new DpGuildJoinRequestDeleteEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnInteractionCreate(const dpp::interaction_create_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::InteractionCreate, new DpInteractionCreateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnButtonClick(const dpp::button_click_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::ButtonClick, new DpButtonClickEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnSelectClick(const dpp::select_click_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::SelectClick, new DpSelectClickEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildDelete(const dpp::guild_delete_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::GuildDelete, new DpGuildDeleteEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnChannelDelete(const dpp::channel_delete_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::ChannelDelete, new DpChannelDeleteEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnChannelUpdate(const dpp::channel_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::ChannelUpdate, new DpChannelUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnReady(const dpp::ready_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::Ready, new DpReadyEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnMessageDelete(const dpp::message_delete_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::MessageDelete, new DpMessageDeleteEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnApplicationCommandDelete(const dpp::application_command_delete_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::ApplicationCommandDelete, new DpApplicationCommandDeleteEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildMemberRemove(const dpp::guild_member_remove_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::GuildMemberRemove, new DpGuildMemberRemoveEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnApplicationCommandCreate(const dpp::application_command_create_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::ApplicationCommandCreate, new DpApplicationCommandCreateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnResumed(const dpp::resumed_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::Resumed, new DpResumedEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildRoleCreate(const dpp::guild_role_create_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::GuildRoleCreate, new DpGuildRoleCreateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnTypingStart(const dpp::typing_start_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::TypingStart, new DpTypingStartEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnMessageReactionAdd(const dpp::message_reaction_add_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::MessageReactionAdd, new DpMessageReactionAddEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildMembersChunk(const dpp::guild_members_chunk_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::GuildMembersChunk, new DpGuildMembersChunkEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnMessageReactionRemove(const dpp::message_reaction_remove_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::MessageReactionRemove, new DpMessageReactionRemoveEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildCreate(const dpp::guild_create_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::GuildCreate, new DpGuildCreateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnChannelCreate(const dpp::channel_create_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::ChannelCreate, new DpChannelCreateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnMessageReactionRemoveEmoji(const dpp::message_reaction_remove_emoji_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::MessageReactionRemoveEmoji, new DpMessageReactionRemoveEmojiEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnMessageDeleteBulk(const dpp::message_delete_bulk_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::MessageDeleteBulk, new DpMessageDeleteBulkEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildRoleUpdate(const dpp::guild_role_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::GuildRoleUpdate, new DpGuildRoleUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildRoleDelete(const dpp::guild_role_delete_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::GuildRoleDelete, new DpGuildRoleDeleteEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnChannelPinsUpdate(const dpp::channel_pins_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::ChannelPinsUpdate, new DpChannelPinsUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnMessageReactionRemoveAll(const dpp::message_reaction_remove_all_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::MessageReactionRemoveAll, new DpMessageReactionRemoveAllEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnVoiceServerUpdate(const dpp::voice_server_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::VoiceServerUpdate, new DpVoiceServerUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildEmojisUpdate(const dpp::guild_emojis_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::GuildEmojisUpdate, new DpGuildEmojisUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildStickersUpdate(const dpp::guild_stickers_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::GuildStickersUpdate, new DpGuildStickersUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnPresenceUpdate(const dpp::presence_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::PresenceUpdate, new DpPresenceUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnWebhooksUpdate(const dpp::webhooks_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::WebhooksUpdate, new DpWebhooksUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildMemberAdd(const dpp::guild_member_add_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::GuildMemberAdd, new DpGuildMemberAddEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnInviteDelete(const dpp::invite_delete_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::InviteDelete, new DpInviteDeleteEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildUpdate(const dpp::guild_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::GuildUpdate, new DpGuildUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildIntegrationsUpdate(const dpp::guild_integrations_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::GuildIntegrationsUpdate, new DpGuildIntegrationsUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildMemberUpdate(const dpp::guild_member_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::GuildMemberUpdate, new DpGuildMemberUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnApplicationCommandUpdate(const dpp::application_command_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::ApplicationCommandUpdate, new DpApplicationCommandUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnInviteCreate(const dpp::invite_create_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::InviteCreate, new DpInviteCreateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnMessageUpdate(const dpp::message_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::MessageUpdate, new DpMessageUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnUserUpdate(const dpp::user_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::UserUpdate, new DpUserUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnMessageCreate(const dpp::message_create_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::MessageCreate, new DpMessageCreateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildBanAdd(const dpp::guild_ban_add_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::GuildBanAdd, new DpGuildBanAddEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildBanRemove(const dpp::guild_ban_remove_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::GuildBanRemove, new DpGuildBanRemoveEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnIntegrationCreate(const dpp::integration_create_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::IntegrationCreate, new DpIntegrationCreateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnIntegrationUpdate(const dpp::integration_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::IntegrationUpdate, new DpIntegrationUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnIntegrationDelete(const dpp::integration_delete_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::IntegrationDelete, new DpIntegrationDeleteEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnThreadCreate(const dpp::thread_create_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::ThreadCreate, new DpThreadCreateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnThreadUpdate(const dpp::thread_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::ThreadUpdate, new DpThreadUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnThreadDelete(const dpp::thread_delete_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::ThreadDelete, new DpThreadDeleteEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnThreadListSync(const dpp::thread_list_sync_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::ThreadListSync, new DpThreadListSyncEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnThreadMemberUpdate(const dpp::thread_member_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::ThreadMemberUpdate, new DpThreadMemberUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnThreadMembersUpdate(const dpp::thread_members_update_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::ThreadMembersUpdate, new DpThreadMembersUpdateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnVoiceBufferSend(const dpp::voice_buffer_send_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::VoiceBufferSend, new DpVoiceBufferSendEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnVoiceUserTalking(const dpp::voice_user_talking_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::VoiceUserTalking, new DpVoiceUserTalkingEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnVoiceReady(const dpp::voice_ready_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::VoiceReady, new DpVoiceReadyEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnVoiceReceive(const dpp::voice_receive_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::VoiceReceive, new DpVoiceReceiveEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnVoiceTrackMarker(const dpp::voice_track_marker_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::VoiceTrackMarker, new DpVoiceTrackMarkerEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnStageInstanceCreate(const dpp::stage_instance_create_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::StageInstanceCreate, new DpStageInstanceCreateEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnStageInstanceDelete(const dpp::stage_instance_delete_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::StageInstanceDelete, new DpStageInstanceDeleteEvent(ev)));
}

// ------------------------------------------------------------------------------------------------
DpCluster & DpCluster::EnableEvent(SQInteger id)
{
    switch (id)
    {
        case DpEventID::VoiceStateUpdate:           mC->on_voice_state_update([this](auto && e) { OnVoiceStateUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::Log:                        mC->on_log([this](auto && e) { OnLog(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::GuildJoinRequestDelete:     mC->on_guild_join_request_delete([this](auto && e) { OnGuildJoinRequestDelete(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::InteractionCreate:          mC->on_interaction_create([this](auto && e) { OnInteractionCreate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::ButtonClick:                mC->on_button_click([this](auto && e) { OnButtonClick(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::SelectClick:                mC->on_select_click([this](auto && e) { OnSelectClick(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::GuildDelete:                mC->on_guild_delete([this](auto && e) { OnGuildDelete(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::ChannelDelete:              mC->on_channel_delete([this](auto && e) { OnChannelDelete(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::ChannelUpdate:              mC->on_channel_update([this](auto && e) { OnChannelUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::Ready:                      mC->on_ready([this](auto && e) { OnReady(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::MessageDelete:              mC->on_message_delete([this](auto && e) { OnMessageDelete(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::ApplicationCommandDelete:   mC->on_application_command_delete([this](auto && e) { OnApplicationCommandDelete(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::GuildMemberRemove:          mC->on_guild_member_remove([this](auto && e) { OnGuildMemberRemove(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::ApplicationCommandCreate:   mC->on_application_command_create([this](auto && e) { OnApplicationCommandCreate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::Resumed:                    mC->on_resumed([this](auto && e) { OnResumed(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::GuildRoleCreate:            mC->on_guild_role_create([this](auto && e) { OnGuildRoleCreate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::TypingStart:                mC->on_typing_start([this](auto && e) { OnTypingStart(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::MessageReactionAdd:         mC->on_message_reaction_add([this](auto && e) { OnMessageReactionAdd(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::GuildMembersChunk:          mC->on_guild_members_chunk([this](auto && e) { OnGuildMembersChunk(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::MessageReactionRemove:      mC->on_message_reaction_remove([this](auto && e) { OnMessageReactionRemove(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::GuildCreate:                mC->on_guild_create([this](auto && e) { OnGuildCreate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::ChannelCreate:              mC->on_channel_create([this](auto && e) { OnChannelCreate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::MessageReactionRemoveEmoji: mC->on_message_reaction_remove_emoji([this](auto && e) { OnMessageReactionRemoveEmoji(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::MessageDeleteBulk:          mC->on_message_delete_bulk([this](auto && e) { OnMessageDeleteBulk(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::GuildRoleUpdate:            mC->on_guild_role_update([this](auto && e) { OnGuildRoleUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::GuildRoleDelete:            mC->on_guild_role_delete([this](auto && e) { OnGuildRoleDelete(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::ChannelPinsUpdate:          mC->on_channel_pins_update([this](auto && e) { OnChannelPinsUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::MessageReactionRemoveAll:   mC->on_message_reaction_remove_all([this](auto && e) { OnMessageReactionRemoveAll(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::VoiceServerUpdate:          mC->on_voice_server_update([this](auto && e) { OnVoiceServerUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::GuildEmojisUpdate:          mC->on_guild_emojis_update([this](auto && e) { OnGuildEmojisUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::GuildStickersUpdate:        mC->on_guild_stickers_update([this](auto && e) { OnGuildStickersUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::PresenceUpdate:             mC->on_presence_update([this](auto && e) { OnPresenceUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::WebhooksUpdate:             mC->on_webhooks_update([this](auto && e) { OnWebhooksUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::GuildMemberAdd:             mC->on_guild_member_add([this](auto && e) { OnGuildMemberAdd(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::InviteDelete:               mC->on_invite_delete([this](auto && e) { OnInviteDelete(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::GuildUpdate:                mC->on_guild_update([this](auto && e) { OnGuildUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::GuildIntegrationsUpdate:    mC->on_guild_integrations_update([this](auto && e) { OnGuildIntegrationsUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::GuildMemberUpdate:          mC->on_guild_member_update([this](auto && e) { OnGuildMemberUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::ApplicationCommandUpdate:   mC->on_application_command_update([this](auto && e) { OnApplicationCommandUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::InviteCreate:               mC->on_invite_create([this](auto && e) { OnInviteCreate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::MessageUpdate:              mC->on_message_update([this](auto && e) { OnMessageUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::UserUpdate:                 mC->on_user_update([this](auto && e) { OnUserUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::MessageCreate:              mC->on_message_create([this](auto && e) { OnMessageCreate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::GuildBanAdd:                mC->on_guild_ban_add([this](auto && e) { OnGuildBanAdd(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::GuildBanRemove:             mC->on_guild_ban_remove([this](auto && e) { OnGuildBanRemove(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::IntegrationCreate:          mC->on_integration_create([this](auto && e) { OnIntegrationCreate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::IntegrationUpdate:          mC->on_integration_update([this](auto && e) { OnIntegrationUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::IntegrationDelete:          mC->on_integration_delete([this](auto && e) { OnIntegrationDelete(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::ThreadCreate:               mC->on_thread_create([this](auto && e) { OnThreadCreate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::ThreadUpdate:               mC->on_thread_update([this](auto && e) { OnThreadUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::ThreadDelete:               mC->on_thread_delete([this](auto && e) { OnThreadDelete(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::ThreadListSync:             mC->on_thread_list_sync([this](auto && e) { OnThreadListSync(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::ThreadMemberUpdate:         mC->on_thread_member_update([this](auto && e) { OnThreadMemberUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::ThreadMembersUpdate:        mC->on_thread_members_update([this](auto && e) { OnThreadMembersUpdate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::VoiceBufferSend:            mC->on_voice_buffer_send([this](auto && e) { OnVoiceBufferSend(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::VoiceUserTalking:           mC->on_voice_user_talking([this](auto && e) { OnVoiceUserTalking(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::VoiceReady:                 mC->on_voice_ready([this](auto && e) { OnVoiceReady(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::VoiceReceive:               mC->on_voice_receive([this](auto && e) { OnVoiceReceive(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::VoiceTrackMarker:           mC->on_voice_track_marker([this](auto && e) { OnVoiceTrackMarker(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::StageInstanceCreate:        mC->on_stage_instance_create([this](auto && e) { OnStageInstanceCreate(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::StageInstanceDelete:        mC->on_stage_instance_delete([this](auto && e) { OnStageInstanceDelete(std::forward< decltype(e) >(e)); }); break;
        case DpEventID::Max: // Fall through
        default: STHROWF("Invalid discord event identifier {}", id);
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
DpCluster & DpCluster::DisableEvent(SQInteger id)
{
    switch (id)
    {
        case DpEventID::VoiceStateUpdate:           mC->on_voice_state_update(std::function<void(const dpp::voice_state_update_t&)>{}); break;
        case DpEventID::Log:                        mC->on_log(std::function<void(const dpp::log_t&)>{}); break;
        case DpEventID::GuildJoinRequestDelete:     mC->on_guild_join_request_delete(std::function<void(const dpp::guild_join_request_delete_t&)>{}); break;
        case DpEventID::InteractionCreate:          mC->on_interaction_create(std::function<void(const dpp::interaction_create_t&)>{}); break;
        case DpEventID::ButtonClick:                mC->on_button_click(std::function<void(const dpp::button_click_t&)>{}); break;
        case DpEventID::SelectClick:                mC->on_select_click(std::function<void(const dpp::select_click_t&)>{}); break;
        case DpEventID::GuildDelete:                mC->on_guild_delete(std::function<void(const dpp::guild_delete_t&)>{}); break;
        case DpEventID::ChannelDelete:              mC->on_channel_delete(std::function<void(const dpp::channel_delete_t&)>{}); break;
        case DpEventID::ChannelUpdate:              mC->on_channel_update(std::function<void(const dpp::channel_update_t&)>{}); break;
        case DpEventID::Ready:                      mC->on_ready(std::function<void(const dpp::ready_t&)>{}); break;
        case DpEventID::MessageDelete:              mC->on_message_delete(std::function<void(const dpp::message_delete_t&)>{}); break;
        case DpEventID::ApplicationCommandDelete:   mC->on_application_command_delete(std::function<void(const dpp::application_command_delete_t&)>{}); break;
        case DpEventID::GuildMemberRemove:          mC->on_guild_member_remove(std::function<void(const dpp::guild_member_remove_t&)>{}); break;
        case DpEventID::ApplicationCommandCreate:   mC->on_application_command_create(std::function<void(const dpp::application_command_create_t&)>{}); break;
        case DpEventID::Resumed:                    mC->on_resumed(std::function<void(const dpp::resumed_t&)>{}); break;
        case DpEventID::GuildRoleCreate:            mC->on_guild_role_create(std::function<void(const dpp::guild_role_create_t&)>{}); break;
        case DpEventID::TypingStart:                mC->on_typing_start(std::function<void(const dpp::typing_start_t&)>{}); break;
        case DpEventID::MessageReactionAdd:         mC->on_message_reaction_add(std::function<void(const dpp::message_reaction_add_t&)>{}); break;
        case DpEventID::GuildMembersChunk:          mC->on_guild_members_chunk(std::function<void(const dpp::guild_members_chunk_t&)>{}); break;
        case DpEventID::MessageReactionRemove:      mC->on_message_reaction_remove(std::function<void(const dpp::message_reaction_remove_t&)>{}); break;
        case DpEventID::GuildCreate:                mC->on_guild_create(std::function<void(const dpp::guild_create_t&)>{}); break;
        case DpEventID::ChannelCreate:              mC->on_channel_create(std::function<void(const dpp::channel_create_t&)>{}); break;
        case DpEventID::MessageReactionRemoveEmoji: mC->on_message_reaction_remove_emoji(std::function<void(const dpp::message_reaction_remove_emoji_t&)>{}); break;
        case DpEventID::MessageDeleteBulk:          mC->on_message_delete_bulk(std::function<void(const dpp::message_delete_bulk_t&)>{}); break;
        case DpEventID::GuildRoleUpdate:            mC->on_guild_role_update(std::function<void(const dpp::guild_role_update_t&)>{}); break;
        case DpEventID::GuildRoleDelete:            mC->on_guild_role_delete(std::function<void(const dpp::guild_role_delete_t&)>{}); break;
        case DpEventID::ChannelPinsUpdate:          mC->on_channel_pins_update(std::function<void(const dpp::channel_pins_update_t&)>{}); break;
        case DpEventID::MessageReactionRemoveAll:   mC->on_message_reaction_remove_all(std::function<void(const dpp::message_reaction_remove_all_t&)>{}); break;
        case DpEventID::VoiceServerUpdate:          mC->on_voice_server_update(std::function<void(const dpp::voice_server_update_t&)>{}); break;
        case DpEventID::GuildEmojisUpdate:          mC->on_guild_emojis_update(std::function<void(const dpp::guild_emojis_update_t&)>{}); break;
        case DpEventID::GuildStickersUpdate:        mC->on_guild_stickers_update(std::function<void(const dpp::guild_stickers_update_t&)>{}); break;
        case DpEventID::PresenceUpdate:             mC->on_presence_update(std::function<void(const dpp::presence_update_t&)>{}); break;
        case DpEventID::WebhooksUpdate:             mC->on_webhooks_update(std::function<void(const dpp::webhooks_update_t&)>{}); break;
        case DpEventID::GuildMemberAdd:             mC->on_guild_member_add(std::function<void(const dpp::guild_member_add_t&)>{}); break;
        case DpEventID::InviteDelete:               mC->on_invite_delete(std::function<void(const dpp::invite_delete_t&)>{}); break;
        case DpEventID::GuildUpdate:                mC->on_guild_update(std::function<void(const dpp::guild_update_t&)>{}); break;
        case DpEventID::GuildIntegrationsUpdate:    mC->on_guild_integrations_update(std::function<void(const dpp::guild_integrations_update_t&)>{}); break;
        case DpEventID::GuildMemberUpdate:          mC->on_guild_member_update(std::function<void(const dpp::guild_member_update_t&)>{}); break;
        case DpEventID::ApplicationCommandUpdate:   mC->on_application_command_update(std::function<void(const dpp::application_command_update_t&)>{}); break;
        case DpEventID::InviteCreate:               mC->on_invite_create(std::function<void(const dpp::invite_create_t&)>{}); break;
        case DpEventID::MessageUpdate:              mC->on_message_update(std::function<void(const dpp::message_update_t&)>{}); break;
        case DpEventID::UserUpdate:                 mC->on_user_update(std::function<void(const dpp::user_update_t&)>{}); break;
        case DpEventID::MessageCreate:              mC->on_message_create(std::function<void(const dpp::message_create_t&)>{}); break;
        case DpEventID::GuildBanAdd:                mC->on_guild_ban_add(std::function<void(const dpp::guild_ban_add_t&)>{}); break;
        case DpEventID::GuildBanRemove:             mC->on_guild_ban_remove(std::function<void(const dpp::guild_ban_remove_t&)>{}); break;
        case DpEventID::IntegrationCreate:          mC->on_integration_create(std::function<void(const dpp::integration_create_t&)>{}); break;
        case DpEventID::IntegrationUpdate:          mC->on_integration_update(std::function<void(const dpp::integration_update_t&)>{}); break;
        case DpEventID::IntegrationDelete:          mC->on_integration_delete(std::function<void(const dpp::integration_delete_t&)>{}); break;
        case DpEventID::ThreadCreate:               mC->on_thread_create(std::function<void(const dpp::thread_create_t&)>{}); break;
        case DpEventID::ThreadUpdate:               mC->on_thread_update(std::function<void(const dpp::thread_update_t&)>{}); break;
        case DpEventID::ThreadDelete:               mC->on_thread_delete(std::function<void(const dpp::thread_delete_t&)>{}); break;
        case DpEventID::ThreadListSync:             mC->on_thread_list_sync(std::function<void(const dpp::thread_list_sync_t&)>{}); break;
        case DpEventID::ThreadMemberUpdate:         mC->on_thread_member_update(std::function<void(const dpp::thread_member_update_t&)>{}); break;
        case DpEventID::ThreadMembersUpdate:        mC->on_thread_members_update(std::function<void(const dpp::thread_members_update_t&)>{}); break;
        case DpEventID::VoiceBufferSend:            mC->on_voice_buffer_send(std::function<void(const dpp::voice_buffer_send_t&)>{}); break;
        case DpEventID::VoiceUserTalking:           mC->on_voice_user_talking(std::function<void(const dpp::voice_user_talking_t&)>{}); break;
        case DpEventID::VoiceReady:                 mC->on_voice_ready(std::function<void(const dpp::voice_ready_t&)>{}); break;
        case DpEventID::VoiceReceive:               mC->on_voice_receive(std::function<void(const dpp::voice_receive_t&)>{}); break;
        case DpEventID::VoiceTrackMarker:           mC->on_voice_track_marker(std::function<void(const dpp::voice_track_marker_t&)>{}); break;
        case DpEventID::StageInstanceCreate:        mC->on_stage_instance_create(std::function<void(const dpp::stage_instance_create_t&)>{}); break;
        case DpEventID::StageInstanceDelete:        mC->on_stage_instance_delete(std::function<void(const dpp::stage_instance_delete_t&)>{}); break;
        case DpEventID::Max: // Fall through
        default: STHROWF("Invalid discord event identifier {}", id);
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
void DpInternalEvent::Release()
{
    // Make sure we actually manage something
    if (mData == 0) return;
    // Fetch the type of data
    const auto type = GetType();
    // Fetch the data itself
    const auto data = GetData();
    // Identify data type
    switch (type)
    {
        case DpEventID::VoiceStateUpdate:           delete reinterpret_cast< DpVoiceStateUpdateEvent * >(data); break;
        case DpEventID::Log:                        delete reinterpret_cast< DpLogEvent * >(data); break;
        case DpEventID::GuildJoinRequestDelete:     delete reinterpret_cast< DpGuildJoinRequestDeleteEvent * >(data); break;
        case DpEventID::InteractionCreate:          delete reinterpret_cast< DpInteractionCreateEvent * >(data); break;
        case DpEventID::ButtonClick:                delete reinterpret_cast< DpButtonClickEvent * >(data); break;
        case DpEventID::SelectClick:                delete reinterpret_cast< DpSelectClickEvent * >(data); break;
        case DpEventID::GuildDelete:                delete reinterpret_cast< DpGuildDeleteEvent * >(data); break;
        case DpEventID::ChannelDelete:              delete reinterpret_cast< DpChannelDeleteEvent * >(data); break;
        case DpEventID::ChannelUpdate:              delete reinterpret_cast< DpChannelUpdateEvent * >(data); break;
        case DpEventID::Ready:                      delete reinterpret_cast< DpReadyEvent * >(data); break;
        case DpEventID::MessageDelete:              delete reinterpret_cast< DpMessageDeleteEvent * >(data); break;
        case DpEventID::ApplicationCommandDelete:   delete reinterpret_cast< DpApplicationCommandDeleteEvent * >(data); break;
        case DpEventID::GuildMemberRemove:          delete reinterpret_cast< DpGuildMemberRemoveEvent * >(data); break;
        case DpEventID::ApplicationCommandCreate:   delete reinterpret_cast< DpApplicationCommandCreateEvent * >(data); break;
        case DpEventID::Resumed:                    delete reinterpret_cast< DpResumedEvent * >(data); break;
        case DpEventID::GuildRoleCreate:            delete reinterpret_cast< DpGuildRoleCreateEvent * >(data); break;
        case DpEventID::TypingStart:                delete reinterpret_cast< DpTypingStartEvent * >(data); break;
        case DpEventID::MessageReactionAdd:         delete reinterpret_cast< DpMessageReactionAddEvent * >(data); break;
        case DpEventID::GuildMembersChunk:          delete reinterpret_cast< DpGuildMembersChunkEvent * >(data); break;
        case DpEventID::MessageReactionRemove:      delete reinterpret_cast< DpMessageReactionRemoveEvent * >(data); break;
        case DpEventID::GuildCreate:                delete reinterpret_cast< DpGuildCreateEvent * >(data); break;
        case DpEventID::ChannelCreate:              delete reinterpret_cast< DpChannelCreateEvent * >(data); break;
        case DpEventID::MessageReactionRemoveEmoji: delete reinterpret_cast< DpMessageReactionRemoveEmojiEvent * >(data); break;
        case DpEventID::MessageDeleteBulk:          delete reinterpret_cast< DpMessageDeleteBulkEvent * >(data); break;
        case DpEventID::GuildRoleUpdate:            delete reinterpret_cast< DpGuildRoleUpdateEvent * >(data); break;
        case DpEventID::GuildRoleDelete:            delete reinterpret_cast< DpGuildRoleDeleteEvent * >(data); break;
        case DpEventID::ChannelPinsUpdate:          delete reinterpret_cast< DpChannelPinsUpdateEvent * >(data); break;
        case DpEventID::MessageReactionRemoveAll:   delete reinterpret_cast< DpMessageReactionRemoveAllEvent * >(data); break;
        case DpEventID::VoiceServerUpdate:          delete reinterpret_cast< DpVoiceServerUpdateEvent * >(data); break;
        case DpEventID::GuildEmojisUpdate:          delete reinterpret_cast< DpGuildEmojisUpdateEvent * >(data); break;
        case DpEventID::GuildStickersUpdate:        delete reinterpret_cast< DpGuildStickersUpdateEvent * >(data); break;
        case DpEventID::PresenceUpdate:             delete reinterpret_cast< DpPresenceUpdateEvent * >(data); break;
        case DpEventID::WebhooksUpdate:             delete reinterpret_cast< DpWebhooksUpdateEvent * >(data); break;
        case DpEventID::GuildMemberAdd:             delete reinterpret_cast< DpGuildMemberAddEvent * >(data); break;
        case DpEventID::InviteDelete:               delete reinterpret_cast< DpInviteDeleteEvent * >(data); break;
        case DpEventID::GuildUpdate:                delete reinterpret_cast< DpGuildUpdateEvent * >(data); break;
        case DpEventID::GuildIntegrationsUpdate:    delete reinterpret_cast< DpGuildIntegrationsUpdateEvent * >(data); break;
        case DpEventID::GuildMemberUpdate:          delete reinterpret_cast< DpGuildMemberUpdateEvent * >(data); break;
        case DpEventID::ApplicationCommandUpdate:   delete reinterpret_cast< DpApplicationCommandUpdateEvent * >(data); break;
        case DpEventID::InviteCreate:               delete reinterpret_cast< DpInviteCreateEvent * >(data); break;
        case DpEventID::MessageUpdate:              delete reinterpret_cast< DpMessageUpdateEvent * >(data); break;
        case DpEventID::UserUpdate:                 delete reinterpret_cast< DpUserUpdateEvent * >(data); break;
        case DpEventID::MessageCreate:              delete reinterpret_cast< DpMessageCreateEvent * >(data); break;
        case DpEventID::GuildBanAdd:                delete reinterpret_cast< DpGuildBanAddEvent * >(data); break;
        case DpEventID::GuildBanRemove:             delete reinterpret_cast< DpGuildBanRemoveEvent * >(data); break;
        case DpEventID::IntegrationCreate:          delete reinterpret_cast< DpIntegrationCreateEvent * >(data); break;
        case DpEventID::IntegrationUpdate:          delete reinterpret_cast< DpIntegrationUpdateEvent * >(data); break;
        case DpEventID::IntegrationDelete:          delete reinterpret_cast< DpIntegrationDeleteEvent * >(data); break;
        case DpEventID::ThreadCreate:               delete reinterpret_cast< DpThreadCreateEvent * >(data); break;
        case DpEventID::ThreadUpdate:               delete reinterpret_cast< DpThreadUpdateEvent * >(data); break;
        case DpEventID::ThreadDelete:               delete reinterpret_cast< DpThreadDeleteEvent * >(data); break;
        case DpEventID::ThreadListSync:             delete reinterpret_cast< DpThreadListSyncEvent * >(data); break;
        case DpEventID::ThreadMemberUpdate:         delete reinterpret_cast< DpThreadMemberUpdateEvent * >(data); break;
        case DpEventID::ThreadMembersUpdate:        delete reinterpret_cast< DpThreadMembersUpdateEvent * >(data); break;
        case DpEventID::VoiceBufferSend:            delete reinterpret_cast< DpVoiceBufferSendEvent * >(data); break;
        case DpEventID::VoiceUserTalking:           delete reinterpret_cast< DpVoiceUserTalkingEvent * >(data); break;
        case DpEventID::VoiceReady:                 delete reinterpret_cast< DpVoiceReadyEvent * >(data); break;
        case DpEventID::VoiceReceive:               delete reinterpret_cast< DpVoiceReceiveEvent * >(data); break;
        case DpEventID::VoiceTrackMarker:           delete reinterpret_cast< DpVoiceTrackMarkerEvent * >(data); break;
        case DpEventID::StageInstanceCreate:        delete reinterpret_cast< DpStageInstanceCreateEvent * >(data); break;
        case DpEventID::StageInstanceDelete:        delete reinterpret_cast< DpStageInstanceDeleteEvent * >(data); break;
        case DpEventID::Max: // Fall through
        default: LogFtl("Unrecognized discord event instance type"); assert(0); break;
    }
    // Forget about it
    Reset();
}

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD LightObj EventToScriptObject(uint8_t type, uintptr_t data)
{
    switch (type)
    {
        case DpEventID::VoiceStateUpdate:           return LightObj(reinterpret_cast< DpVoiceStateUpdateEvent * >(data));
        case DpEventID::Log:                        return LightObj(reinterpret_cast< DpLogEvent * >(data));
        case DpEventID::GuildJoinRequestDelete:     return LightObj(reinterpret_cast< DpGuildJoinRequestDeleteEvent * >(data));
        case DpEventID::InteractionCreate:          return LightObj(reinterpret_cast< DpInteractionCreateEvent * >(data));
        case DpEventID::ButtonClick:                return LightObj(reinterpret_cast< DpButtonClickEvent * >(data));
        case DpEventID::SelectClick:                return LightObj(reinterpret_cast< DpSelectClickEvent * >(data));
        case DpEventID::GuildDelete:                return LightObj(reinterpret_cast< DpGuildDeleteEvent * >(data));
        case DpEventID::ChannelDelete:              return LightObj(reinterpret_cast< DpChannelDeleteEvent * >(data));
        case DpEventID::ChannelUpdate:              return LightObj(reinterpret_cast< DpChannelUpdateEvent * >(data));
        case DpEventID::Ready:                      return LightObj(reinterpret_cast< DpReadyEvent * >(data));
        case DpEventID::MessageDelete:              return LightObj(reinterpret_cast< DpMessageDeleteEvent * >(data));
        case DpEventID::ApplicationCommandDelete:   return LightObj(reinterpret_cast< DpApplicationCommandDeleteEvent * >(data));
        case DpEventID::GuildMemberRemove:          return LightObj(reinterpret_cast< DpGuildMemberRemoveEvent * >(data));
        case DpEventID::ApplicationCommandCreate:   return LightObj(reinterpret_cast< DpApplicationCommandCreateEvent * >(data));
        case DpEventID::Resumed:                    return LightObj(reinterpret_cast< DpResumedEvent * >(data));
        case DpEventID::GuildRoleCreate:            return LightObj(reinterpret_cast< DpGuildRoleCreateEvent * >(data));
        case DpEventID::TypingStart:                return LightObj(reinterpret_cast< DpTypingStartEvent * >(data));
        case DpEventID::MessageReactionAdd:         return LightObj(reinterpret_cast< DpMessageReactionAddEvent * >(data));
        case DpEventID::GuildMembersChunk:          return LightObj(reinterpret_cast< DpGuildMembersChunkEvent * >(data));
        case DpEventID::MessageReactionRemove:      return LightObj(reinterpret_cast< DpMessageReactionRemoveEvent * >(data));
        case DpEventID::GuildCreate:                return LightObj(reinterpret_cast< DpGuildCreateEvent * >(data));
        case DpEventID::ChannelCreate:              return LightObj(reinterpret_cast< DpChannelCreateEvent * >(data));
        case DpEventID::MessageReactionRemoveEmoji: return LightObj(reinterpret_cast< DpMessageReactionRemoveEmojiEvent * >(data));
        case DpEventID::MessageDeleteBulk:          return LightObj(reinterpret_cast< DpMessageDeleteBulkEvent * >(data));
        case DpEventID::GuildRoleUpdate:            return LightObj(reinterpret_cast< DpGuildRoleUpdateEvent * >(data));
        case DpEventID::GuildRoleDelete:            return LightObj(reinterpret_cast< DpGuildRoleDeleteEvent * >(data));
        case DpEventID::ChannelPinsUpdate:          return LightObj(reinterpret_cast< DpChannelPinsUpdateEvent * >(data));
        case DpEventID::MessageReactionRemoveAll:   return LightObj(reinterpret_cast< DpMessageReactionRemoveAllEvent * >(data));
        case DpEventID::VoiceServerUpdate:          return LightObj(reinterpret_cast< DpVoiceServerUpdateEvent * >(data));
        case DpEventID::GuildEmojisUpdate:          return LightObj(reinterpret_cast< DpGuildEmojisUpdateEvent * >(data));
        case DpEventID::GuildStickersUpdate:        return LightObj(reinterpret_cast< DpGuildStickersUpdateEvent * >(data));
        case DpEventID::PresenceUpdate:             return LightObj(reinterpret_cast< DpPresenceUpdateEvent * >(data));
        case DpEventID::WebhooksUpdate:             return LightObj(reinterpret_cast< DpWebhooksUpdateEvent * >(data));
        case DpEventID::GuildMemberAdd:             return LightObj(reinterpret_cast< DpGuildMemberAddEvent * >(data));
        case DpEventID::InviteDelete:               return LightObj(reinterpret_cast< DpInviteDeleteEvent * >(data));
        case DpEventID::GuildUpdate:                return LightObj(reinterpret_cast< DpGuildUpdateEvent * >(data));
        case DpEventID::GuildIntegrationsUpdate:    return LightObj(reinterpret_cast< DpGuildIntegrationsUpdateEvent * >(data));
        case DpEventID::GuildMemberUpdate:          return LightObj(reinterpret_cast< DpGuildMemberUpdateEvent * >(data));
        case DpEventID::ApplicationCommandUpdate:   return LightObj(reinterpret_cast< DpApplicationCommandUpdateEvent * >(data));
        case DpEventID::InviteCreate:               return LightObj(reinterpret_cast< DpInviteCreateEvent * >(data));
        case DpEventID::MessageUpdate:              return LightObj(reinterpret_cast< DpMessageUpdateEvent * >(data));
        case DpEventID::UserUpdate:                 return LightObj(reinterpret_cast< DpUserUpdateEvent * >(data));
        case DpEventID::MessageCreate:              return LightObj(reinterpret_cast< DpMessageCreateEvent * >(data));
        case DpEventID::GuildBanAdd:                return LightObj(reinterpret_cast< DpGuildBanAddEvent * >(data));
        case DpEventID::GuildBanRemove:             return LightObj(reinterpret_cast< DpGuildBanRemoveEvent * >(data));
        case DpEventID::IntegrationCreate:          return LightObj(reinterpret_cast< DpIntegrationCreateEvent * >(data));
        case DpEventID::IntegrationUpdate:          return LightObj(reinterpret_cast< DpIntegrationUpdateEvent * >(data));
        case DpEventID::IntegrationDelete:          return LightObj(reinterpret_cast< DpIntegrationDeleteEvent * >(data));
        case DpEventID::ThreadCreate:               return LightObj(reinterpret_cast< DpThreadCreateEvent * >(data));
        case DpEventID::ThreadUpdate:               return LightObj(reinterpret_cast< DpThreadUpdateEvent * >(data));
        case DpEventID::ThreadDelete:               return LightObj(reinterpret_cast< DpThreadDeleteEvent * >(data));
        case DpEventID::ThreadListSync:             return LightObj(reinterpret_cast< DpThreadListSyncEvent * >(data));
        case DpEventID::ThreadMemberUpdate:         return LightObj(reinterpret_cast< DpThreadMemberUpdateEvent * >(data));
        case DpEventID::ThreadMembersUpdate:        return LightObj(reinterpret_cast< DpThreadMembersUpdateEvent * >(data));
        case DpEventID::VoiceBufferSend:            return LightObj(reinterpret_cast< DpVoiceBufferSendEvent * >(data));
        case DpEventID::VoiceUserTalking:           return LightObj(reinterpret_cast< DpVoiceUserTalkingEvent * >(data));
        case DpEventID::VoiceReady:                 return LightObj(reinterpret_cast< DpVoiceReadyEvent * >(data));
        case DpEventID::VoiceReceive:               return LightObj(reinterpret_cast< DpVoiceReceiveEvent * >(data));
        case DpEventID::VoiceTrackMarker:           return LightObj(reinterpret_cast< DpVoiceTrackMarkerEvent * >(data));
        case DpEventID::StageInstanceCreate:        return LightObj(reinterpret_cast< DpStageInstanceCreateEvent * >(data));
        case DpEventID::StageInstanceDelete:        return LightObj(reinterpret_cast< DpStageInstanceDeleteEvent * >(data));
        case DpEventID::Max: // Fall through
        default: assert(0); return LightObj{};
    }
}

// ------------------------------------------------------------------------------------------------
void EventInvokeCleanup(uint8_t type, uintptr_t data)
{
    switch (type)
    {
        case DpEventID::VoiceStateUpdate:           reinterpret_cast< DpVoiceStateUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::Log:                        reinterpret_cast< DpLogEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildJoinRequestDelete:     reinterpret_cast< DpGuildJoinRequestDeleteEvent * >(data)->Cleanup(); break;
        case DpEventID::InteractionCreate:          reinterpret_cast< DpInteractionCreateEvent * >(data)->Cleanup(); break;
        case DpEventID::ButtonClick:                reinterpret_cast< DpButtonClickEvent * >(data)->Cleanup(); break;
        case DpEventID::SelectClick:                reinterpret_cast< DpSelectClickEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildDelete:                reinterpret_cast< DpGuildDeleteEvent * >(data)->Cleanup(); break;
        case DpEventID::ChannelDelete:              reinterpret_cast< DpChannelDeleteEvent * >(data)->Cleanup(); break;
        case DpEventID::ChannelUpdate:              reinterpret_cast< DpChannelUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::Ready:                      reinterpret_cast< DpReadyEvent * >(data)->Cleanup(); break;
        case DpEventID::MessageDelete:              reinterpret_cast< DpMessageDeleteEvent * >(data)->Cleanup(); break;
        case DpEventID::ApplicationCommandDelete:   reinterpret_cast< DpApplicationCommandDeleteEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildMemberRemove:          reinterpret_cast< DpGuildMemberRemoveEvent * >(data)->Cleanup(); break;
        case DpEventID::ApplicationCommandCreate:   reinterpret_cast< DpApplicationCommandCreateEvent * >(data)->Cleanup(); break;
        case DpEventID::Resumed:                    reinterpret_cast< DpResumedEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildRoleCreate:            reinterpret_cast< DpGuildRoleCreateEvent * >(data)->Cleanup(); break;
        case DpEventID::TypingStart:                reinterpret_cast< DpTypingStartEvent * >(data)->Cleanup(); break;
        case DpEventID::MessageReactionAdd:         reinterpret_cast< DpMessageReactionAddEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildMembersChunk:          reinterpret_cast< DpGuildMembersChunkEvent * >(data)->Cleanup(); break;
        case DpEventID::MessageReactionRemove:      reinterpret_cast< DpMessageReactionRemoveEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildCreate:                reinterpret_cast< DpGuildCreateEvent * >(data)->Cleanup(); break;
        case DpEventID::ChannelCreate:              reinterpret_cast< DpChannelCreateEvent * >(data)->Cleanup(); break;
        case DpEventID::MessageReactionRemoveEmoji: reinterpret_cast< DpMessageReactionRemoveEmojiEvent * >(data)->Cleanup(); break;
        case DpEventID::MessageDeleteBulk:          reinterpret_cast< DpMessageDeleteBulkEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildRoleUpdate:            reinterpret_cast< DpGuildRoleUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildRoleDelete:            reinterpret_cast< DpGuildRoleDeleteEvent * >(data)->Cleanup(); break;
        case DpEventID::ChannelPinsUpdate:          reinterpret_cast< DpChannelPinsUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::MessageReactionRemoveAll:   reinterpret_cast< DpMessageReactionRemoveAllEvent * >(data)->Cleanup(); break;
        case DpEventID::VoiceServerUpdate:          reinterpret_cast< DpVoiceServerUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildEmojisUpdate:          reinterpret_cast< DpGuildEmojisUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildStickersUpdate:        reinterpret_cast< DpGuildStickersUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::PresenceUpdate:             reinterpret_cast< DpPresenceUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::WebhooksUpdate:             reinterpret_cast< DpWebhooksUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildMemberAdd:             reinterpret_cast< DpGuildMemberAddEvent * >(data)->Cleanup(); break;
        case DpEventID::InviteDelete:               reinterpret_cast< DpInviteDeleteEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildUpdate:                reinterpret_cast< DpGuildUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildIntegrationsUpdate:    reinterpret_cast< DpGuildIntegrationsUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildMemberUpdate:          reinterpret_cast< DpGuildMemberUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::ApplicationCommandUpdate:   reinterpret_cast< DpApplicationCommandUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::InviteCreate:               reinterpret_cast< DpInviteCreateEvent * >(data)->Cleanup(); break;
        case DpEventID::MessageUpdate:              reinterpret_cast< DpMessageUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::UserUpdate:                 reinterpret_cast< DpUserUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::MessageCreate:              reinterpret_cast< DpMessageCreateEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildBanAdd:                reinterpret_cast< DpGuildBanAddEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildBanRemove:             reinterpret_cast< DpGuildBanRemoveEvent * >(data)->Cleanup(); break;
        case DpEventID::IntegrationCreate:          reinterpret_cast< DpIntegrationCreateEvent * >(data)->Cleanup(); break;
        case DpEventID::IntegrationUpdate:          reinterpret_cast< DpIntegrationUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::IntegrationDelete:          reinterpret_cast< DpIntegrationDeleteEvent * >(data)->Cleanup(); break;
        case DpEventID::ThreadCreate:               reinterpret_cast< DpThreadCreateEvent * >(data)->Cleanup(); break;
        case DpEventID::ThreadUpdate:               reinterpret_cast< DpThreadUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::ThreadDelete:               reinterpret_cast< DpThreadDeleteEvent * >(data)->Cleanup(); break;
        case DpEventID::ThreadListSync:             reinterpret_cast< DpThreadListSyncEvent * >(data)->Cleanup(); break;
        case DpEventID::ThreadMemberUpdate:         reinterpret_cast< DpThreadMemberUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::ThreadMembersUpdate:        reinterpret_cast< DpThreadMembersUpdateEvent * >(data)->Cleanup(); break;
        case DpEventID::VoiceBufferSend:            reinterpret_cast< DpVoiceBufferSendEvent * >(data)->Cleanup(); break;
        case DpEventID::VoiceUserTalking:           reinterpret_cast< DpVoiceUserTalkingEvent * >(data)->Cleanup(); break;
        case DpEventID::VoiceReady:                 reinterpret_cast< DpVoiceReadyEvent * >(data)->Cleanup(); break;
        case DpEventID::VoiceReceive:               reinterpret_cast< DpVoiceReceiveEvent * >(data)->Cleanup(); break;
        case DpEventID::VoiceTrackMarker:           reinterpret_cast< DpVoiceTrackMarkerEvent * >(data)->Cleanup(); break;
        case DpEventID::StageInstanceCreate:        reinterpret_cast< DpStageInstanceCreateEvent * >(data)->Cleanup(); break;
        case DpEventID::StageInstanceDelete:        reinterpret_cast< DpStageInstanceDeleteEvent * >(data)->Cleanup(); break;
        case DpEventID::Max: // Fall through
        default: assert(0); return;
    }
}

} // Namespace:: SqMod
