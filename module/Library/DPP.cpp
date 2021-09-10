// ------------------------------------------------------------------------------------------------
#include "Library/DPP.hpp"
#include "Core/Signal.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDppCluster, _SC("SqDppCluster"))

// ------------------------------------------------------------------------------------------------
void TerminateDPP()
{
    // Go over all clusters and try to terminate them
    for (DpCluster * inst = DpCluster::sHead; inst && inst->mNext != DpCluster::sHead; inst = inst->mNext)
    {
        inst->Terminate(); // Terminate() the cluster
    }
}

// ------------------------------------------------------------------------------------------------
void ProcessDPP()
{
    // Go over all clusters and allow them to process data
    for (DpCluster * inst = DpCluster::sHead; inst && inst->mNext != DpCluster::sHead; inst = inst->mNext)
    {
        inst->Process();
    }
}

// ------------------------------------------------------------------------------------------------
extern void Register_DPPTy(HSQUIRRELVM vm, Table & ns);
extern void Register_DPPEv(HSQUIRRELVM vm, Table & ns);

// ================================================================================================
void Register_DPP(HSQUIRRELVM vm)
{
    Table ns(vm);
    // --------------------------------------------------------------------------------------------
    {
        Table ens(vm);
        Register_DPPEv(vm, ens);
        ns.Bind(_SC("Events"), ens);
    }
    // Register base types
    Register_DPPTy(vm, ns);
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
    // --------------------------------------------------------------------------------------------
    ns.Func(_SC("HasVoice"), dpp::utility::has_voice);

    RootTable(vm).Bind(_SC("SqDiscord"), ns);
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
    //mQueue.enqueue(DpInternalEvent(DpEventID::InteractionCreate, new dpp::interaction_create_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnButtonClick(const dpp::button_click_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::ButtonClick, new dpp::button_click_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnSelectClick(const dpp::select_click_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::SelectClick, new dpp::select_click_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildDelete(const dpp::guild_delete_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::GuildDelete, new dpp::guild_delete_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnChannelDelete(const dpp::channel_delete_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::ChannelDelete, new dpp::channel_delete_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnChannelUpdate(const dpp::channel_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::ChannelUpdate, new dpp::channel_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnReady(const dpp::ready_t & ev)
{
    mQueue.enqueue(DpInternalEvent(DpEventID::Ready, new DpReadyEvent(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnMessageDelete(const dpp::message_delete_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::MessageDelete, new dpp::message_delete_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnApplicationCommandDelete(const dpp::application_command_delete_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::ApplicationCommandDelete, new dpp::application_command_delete_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildMemberRemove(const dpp::guild_member_remove_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::GuildMemberRemove, new dpp::guild_member_remove_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnApplicationCommandCreate(const dpp::application_command_create_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::ApplicationCommandCreate, new dpp::application_command_create_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnResumed(const dpp::resumed_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::Resumed, new dpp::resumed_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildRoleCreate(const dpp::guild_role_create_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::GuildRoleCreate, new dpp::guild_role_create_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnTypingStart(const dpp::typing_start_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::TypingStart, new dpp::typing_start_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnMessageReactionAdd(const dpp::message_reaction_add_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::MessageReactionAdd, new dpp::message_reaction_add_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildMembersChunk(const dpp::guild_members_chunk_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::GuildMembersChunk, new dpp::guild_members_chunk_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnMessageReactionRemove(const dpp::message_reaction_remove_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::MessageReactionRemove, new dpp::message_reaction_remove_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildCreate(const dpp::guild_create_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::GuildCreate, new dpp::guild_create_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnChannelCreate(const dpp::channel_create_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::ChannelCreate, new dpp::channel_create_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnMessageReactionRemoveEmoji(const dpp::message_reaction_remove_emoji_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::MessageReactionRemoveEmoji, new dpp::message_reaction_remove_emoji_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnMessageDeleteBulk(const dpp::message_delete_bulk_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::MessageDeleteBulk, new dpp::message_delete_bulk_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildRoleUpdate(const dpp::guild_role_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::GuildRoleUpdate, new dpp::guild_role_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildRoleDelete(const dpp::guild_role_delete_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::GuildRoleDelete, new dpp::guild_role_delete_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnChannelPinsUpdate(const dpp::channel_pins_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::ChannelPinsUpdate, new dpp::channel_pins_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnMessageReactionRemoveAll(const dpp::message_reaction_remove_all_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::MessageReactionRemoveAll, new dpp::message_reaction_remove_all_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnVoiceServerUpdate(const dpp::voice_server_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::VoiceServerUpdate, new dpp::voice_server_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildEmojisUpdate(const dpp::guild_emojis_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::GuildEmojisUpdate, new dpp::guild_emojis_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildStickersUpdate(const dpp::guild_stickers_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::GuildStickersUpdate, new dpp::guild_stickers_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnPresenceUpdate(const dpp::presence_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::PresenceUpdate, new dpp::presence_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnWebhooksUpdate(const dpp::webhooks_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::WebhooksUpdate, new dpp::webhooks_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildMemberAdd(const dpp::guild_member_add_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::GuildMemberAdd, new dpp::guild_member_add_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnInviteDelete(const dpp::invite_delete_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::InviteDelete, new dpp::invite_delete_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildUpdate(const dpp::guild_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::GuildUpdate, new dpp::guild_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildIntegrationsUpdate(const dpp::guild_integrations_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::GuildIntegrationsUpdate, new dpp::guild_integrations_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildMemberUpdate(const dpp::guild_member_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::GuildMemberUpdate, new dpp::guild_member_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnApplicationCommandUpdate(const dpp::application_command_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::ApplicationCommandUpdate, new dpp::application_command_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnInviteCreate(const dpp::invite_create_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::InviteCreate, new dpp::invite_create_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnMessageUpdate(const dpp::message_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::MessageUpdate, new dpp::message_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnUserUpdate(const dpp::user_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::UserUpdate, new dpp::user_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnMessageCreate(const dpp::message_create_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::MessageCreate, new dpp::message_create_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildBanAdd(const dpp::guild_ban_add_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::GuildBanAdd, new dpp::guild_ban_add_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnGuildBanRemove(const dpp::guild_ban_remove_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::GuildBanRemove, new dpp::guild_ban_remove_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnIntegrationCreate(const dpp::integration_create_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::IntegrationCreate, new dpp::integration_create_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnIntegrationUpdate(const dpp::integration_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::IntegrationUpdate, new dpp::integration_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnIntegrationDelete(const dpp::integration_delete_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::IntegrationDelete, new dpp::integration_delete_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnThreadCreate(const dpp::thread_create_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::ThreadCreate, new dpp::thread_create_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnThreadUpdate(const dpp::thread_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::ThreadUpdate, new dpp::thread_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnThreadDelete(const dpp::thread_delete_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::ThreadDelete, new dpp::thread_delete_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnThreadListSync(const dpp::thread_list_sync_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::ThreadListSync, new dpp::thread_list_sync_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnThreadMemberUpdate(const dpp::thread_member_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::ThreadMemberUpdate, new dpp::thread_member_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnThreadMembersUpdate(const dpp::thread_members_update_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::ThreadMembersUpdate, new dpp::thread_members_update_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnVoiceBufferSend(const dpp::voice_buffer_send_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::VoiceBufferSend, new dpp::voice_buffer_send_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnVoiceUserTalking(const dpp::voice_user_talking_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::VoiceUserTalking, new dpp::voice_user_talking_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnVoiceReady(const dpp::voice_ready_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::VoiceReady, new dpp::voice_ready_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnVoiceReceive(const dpp::voice_receive_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::VoiceReceive, new dpp::voice_receive_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnVoiceTrackMarker(const dpp::voice_track_marker_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::VoiceTrackMarker, new dpp::voice_track_marker_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnStageInstanceCreate(const dpp::stage_instance_create_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::StageInstanceCreate, new dpp::stage_instance_create_t(ev)));
}
// ------------------------------------------------------------------------------------------------
void DpCluster::OnStageInstanceDelete(const dpp::stage_instance_delete_t & ev)
{
    //mQueue.enqueue(DpInternalEvent(DpEventID::StageInstanceDelete, new dpp::stage_instance_delete_t(ev)));
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
        case DpEventID::VoiceStateUpdate:           delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::Log:                        delete reinterpret_cast< DpLogEvent * >(data); break;
        case DpEventID::GuildJoinRequestDelete:     delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::InteractionCreate:          delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ButtonClick:                delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::SelectClick:                delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildDelete:                delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ChannelDelete:              delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ChannelUpdate:              delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::Ready:                      delete reinterpret_cast< DpReadyEvent * >(data); break;
        case DpEventID::MessageDelete:              delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ApplicationCommandDelete:   delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildMemberRemove:          delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ApplicationCommandCreate:   delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::Resumed:                    delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildRoleCreate:            delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::TypingStart:                delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::MessageReactionAdd:         delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildMembersChunk:          delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::MessageReactionRemove:      delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildCreate:                delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ChannelCreate:              delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::MessageReactionRemoveEmoji: delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::MessageDeleteBulk:          delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildRoleUpdate:            delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildRoleDelete:            delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ChannelPinsUpdate:          delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::MessageReactionRemoveAll:   delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::VoiceServerUpdate:          delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildEmojisUpdate:          delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildStickersUpdate:        delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::PresenceUpdate:             delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::WebhooksUpdate:             delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildMemberAdd:             delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::InviteDelete:               delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildUpdate:                delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildIntegrationsUpdate:    delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildMemberUpdate:          delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ApplicationCommandUpdate:   delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::InviteCreate:               delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::MessageUpdate:              delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::UserUpdate:                 delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::MessageCreate:              delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildBanAdd:                delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildBanRemove:             delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::IntegrationCreate:          delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::IntegrationUpdate:          delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::IntegrationDelete:          delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ThreadCreate:               delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ThreadUpdate:               delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ThreadDelete:               delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ThreadListSync:             delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ThreadMemberUpdate:         delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ThreadMembersUpdate:        delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::VoiceBufferSend:            delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::VoiceUserTalking:           delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::VoiceReady:                 delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::VoiceReceive:               delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::VoiceTrackMarker:           delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::StageInstanceCreate:        delete reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::StageInstanceDelete:        delete reinterpret_cast< uint8_t * >(data); break;
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
        case DpEventID::VoiceStateUpdate:           return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::Log:                        return LightObj(reinterpret_cast< DpLogEvent * >(data));
        case DpEventID::GuildJoinRequestDelete:     return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::InteractionCreate:          return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::ButtonClick:                return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::SelectClick:                return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::GuildDelete:                return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::ChannelDelete:              return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::ChannelUpdate:              return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::Ready:                      return LightObj(reinterpret_cast< DpReadyEvent * >(data));
        case DpEventID::MessageDelete:              return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::ApplicationCommandDelete:   return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::GuildMemberRemove:          return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::ApplicationCommandCreate:   return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::Resumed:                    return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::GuildRoleCreate:            return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::TypingStart:                return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::MessageReactionAdd:         return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::GuildMembersChunk:          return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::MessageReactionRemove:      return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::GuildCreate:                return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::ChannelCreate:              return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::MessageReactionRemoveEmoji: return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::MessageDeleteBulk:          return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::GuildRoleUpdate:            return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::GuildRoleDelete:            return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::ChannelPinsUpdate:          return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::MessageReactionRemoveAll:   return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::VoiceServerUpdate:          return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::GuildEmojisUpdate:          return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::GuildStickersUpdate:        return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::PresenceUpdate:             return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::WebhooksUpdate:             return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::GuildMemberAdd:             return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::InviteDelete:               return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::GuildUpdate:                return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::GuildIntegrationsUpdate:    return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::GuildMemberUpdate:          return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::ApplicationCommandUpdate:   return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::InviteCreate:               return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::MessageUpdate:              return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::UserUpdate:                 return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::MessageCreate:              return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::GuildBanAdd:                return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::GuildBanRemove:             return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::IntegrationCreate:          return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::IntegrationUpdate:          return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::IntegrationDelete:          return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::ThreadCreate:               return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::ThreadUpdate:               return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::ThreadDelete:               return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::ThreadListSync:             return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::ThreadMemberUpdate:         return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::ThreadMembersUpdate:        return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::VoiceBufferSend:            return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::VoiceUserTalking:           return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::VoiceReady:                 return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::VoiceReceive:               return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::VoiceTrackMarker:           return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::StageInstanceCreate:        return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::StageInstanceDelete:        return LightObj(reinterpret_cast< uint8_t * >(data));
        case DpEventID::Max: // Fall through
        default: assert(0); return LightObj{};
    }
}

// ------------------------------------------------------------------------------------------------
void EventInvokeCleanup(uint8_t type, uintptr_t data)
{
    switch (type)
    {
        case DpEventID::VoiceStateUpdate:           reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::Log:                        reinterpret_cast< DpLogEvent * >(data)->Cleanup(); break;
        case DpEventID::GuildJoinRequestDelete:     reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::InteractionCreate:          reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ButtonClick:                reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::SelectClick:                reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildDelete:                reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ChannelDelete:              reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ChannelUpdate:              reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::Ready:                      reinterpret_cast< DpReadyEvent * >(data)->Cleanup(); break;
        case DpEventID::MessageDelete:              reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ApplicationCommandDelete:   reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildMemberRemove:          reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ApplicationCommandCreate:   reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::Resumed:                    reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildRoleCreate:            reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::TypingStart:                reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::MessageReactionAdd:         reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildMembersChunk:          reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::MessageReactionRemove:      reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildCreate:                reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ChannelCreate:              reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::MessageReactionRemoveEmoji: reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::MessageDeleteBulk:          reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildRoleUpdate:            reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildRoleDelete:            reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ChannelPinsUpdate:          reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::MessageReactionRemoveAll:   reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::VoiceServerUpdate:          reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildEmojisUpdate:          reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildStickersUpdate:        reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::PresenceUpdate:             reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::WebhooksUpdate:             reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildMemberAdd:             reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::InviteDelete:               reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildUpdate:                reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildIntegrationsUpdate:    reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildMemberUpdate:          reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ApplicationCommandUpdate:   reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::InviteCreate:               reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::MessageUpdate:              reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::UserUpdate:                 reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::MessageCreate:              reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildBanAdd:                reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::GuildBanRemove:             reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::IntegrationCreate:          reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::IntegrationUpdate:          reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::IntegrationDelete:          reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ThreadCreate:               reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ThreadUpdate:               reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ThreadDelete:               reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ThreadListSync:             reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ThreadMemberUpdate:         reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::ThreadMembersUpdate:        reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::VoiceBufferSend:            reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::VoiceUserTalking:           reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::VoiceReady:                 reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::VoiceReceive:               reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::VoiceTrackMarker:           reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::StageInstanceCreate:        reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::StageInstanceDelete:        reinterpret_cast< uint8_t * >(data); break;
        case DpEventID::Max: // Fall through
        default: assert(0); return;
    }
}

} // Namespace:: SqMod
