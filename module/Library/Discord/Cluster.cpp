// ------------------------------------------------------------------------------------------------
#include "Library/Discord/Cluster.hpp"
#include "Library/Discord/Events.hpp"
#include "Logger.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDpClusterTypename, _SC("SqDiscordCluster"))
SQMOD_DECL_TYPENAME(SqDpClusterOptionsTypename, _SC("SqDiscordClusterOptions"))

// ------------------------------------------------------------------------------------------------
DpCluster::DpCluster(DpClusterOptions & o)
    : Base(), mQueue(4096)
    , mC(std::make_unique< dpp::cluster >(o.mToken, o.mIntents, o.mShards, o.mClusterID, o.mMaxClusters, o.mCompressed, o.mPolicy, o.mRequestThreads, o.mRequestThreadsRaw))
    , mSqEvents(), mEvents(), mEventsHandle()
    , mCCList(), mCCResults(std::make_shared< CCResultQueue >(4096))
{
    // Make sure all event handles are not valid
    mEventsHandle.fill(0);
    // Initialize event signals
    InitEvents();
    // Proxy library logging to our logger
    if (!o.mCustomLogging)
    {
        mC->on_log([](const dpp::log_t& event) {
            switch (event.severity)
            {
                case dpp::ll_trace: Logger::Get().Send(LOGL_DBG, true, event.message.c_str(), event.message.size()); break;
                case dpp::ll_debug: Logger::Get().Send(LOGL_DBG, true, event.message.c_str(), event.message.size()); break;
                case dpp::ll_info: Logger::Get().Send(LOGL_INF, true, event.message.c_str(), event.message.size()); break;
                case dpp::ll_warning: Logger::Get().Send(LOGL_WRN, true, event.message.c_str(), event.message.size()); break;
                case dpp::ll_error: Logger::Get().Send(LOGL_ERR, true, event.message.c_str(), event.message.size()); break;
                case dpp::ll_critical: Logger::Get().Send(LOGL_FTL, true, event.message.c_str(), event.message.size()); break;
                default: break;
            }
        });
    }
    // Remember this instance
    ChainInstance();
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
    EventItem event;
    // Retrieve each event individually and process it
    for (size_t count = mQueue.size_approx(), n = 0; n <= count; ++n)
    {
        // Try to get an event from the queue
        if (mQueue.try_dequeue(event))
        {
            // Fetch the type of event
            const auto id = static_cast< size_t >(event->GetEventID());
            // Is this a valid event and is anyone listening to it?
            if (!(event->mFrom) || !(mEvents[id].first) || mEvents[id].first->IsEmpty())
            {
                continue; // Move on
            }
            // Transform the event instance into a script object
            LightObj obj = event->ToScriptObject();
            // Allow the script to take ownership of the event instance now
            [[maybe_unused]] auto p = event.release();
            // Don't abort everything down the line for an error caused by a discord event handler
            try {
                (*mEvents[id].first)(obj); // Forward the call to the associated signal
            } catch (const std::exception & e) {
                LogErr("Squirrel exception caught in (%s) discord event", p->GetEventName().data());
                LogSInf("Message: %s", e.what());
            }
            // Allow the event instance to clean itself (i.e. invalidate itself)
            // User should not keep this event object for later use!
            // Event data is accessible only during the event signal
            p->Cleanup();
        }
    }
    CCResultItem cc_item;
    // Retrieve each command completion result individually and process it
    for (size_t count = mCCResults->size_approx(), n = 0; n <= count; ++n)
    {
        // Try to get a result from the queue
        if (mCCResults->try_dequeue(cc_item))
        {
            CCResult & r = *cc_item;
            // Get the script callback
            Function & cb = *(r.first);
            // Is there still a valid callback to invoke?
            if (!cb.IsNull())
            {
                // Don't abort everything down the line for an error caused by a script callback
                try {
                    cb.Execute(LightObj(SqTypeIdentity< DpCommandConfirmation >{}, cb.GetVM(), std::move(r.second)));
                } catch (const std::exception & e) {
                    LogErr("Squirrel exception caught in discord command completion event");
                    LogSInf("Message: %s", e.what());
                }
            }
            // Release the callback from the list
            mCCList.erase(r.first);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void DpCluster::Terminate()
{
    // Stop the cluster connection
    if (mC) mC->shutdown();
    // Release associated script objects
    mSqEvents.Release();
    // Release event signal objects
    DropEvents();
    // Delete the cluster instance
    mC.reset();
}

// ================================================================================================
void Register_Discord_Cluster(HSQUIRRELVM vm, Table & ns)
{
    ns.Bind(_SC("ClusterOptions"),
        Class< DpClusterOptions >(vm, SqDpClusterOptionsTypename::Str)
        // Constructors
        .Ctor< StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpClusterOptionsTypename::Fn)
    );

    ns.Bind(_SC("Cluster"),
        Class< DpCluster, NoCopy< DpCluster > >(vm, SqDpClusterTypename::Str)
        // Constructors
        .Ctor< DpClusterOptions & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpClusterTypename::Fn)
        // Member Properties
        .Prop(_SC("On"), &DpCluster::GetEvents)
        // Member Methods
        .Func(_SC("Start"), &DpCluster::Start)
        .Func(_SC("Stop"), &DpCluster::Stop)
        .Func(_SC("EnableEvent"), &DpCluster::EnableEvent)
        .Func(_SC("DisableEvent"), &DpCluster::DisableEvent)
        .CbFunc(_SC("CurrentUserGetGuilds"), &DpCluster::CurrentUserGetGuilds)
    );
}

// ------------------------------------------------------------------------------------------------
DpCluster & DpCluster::EnableEvent(SQInteger id)
{
    // Retrieve managed cluster instance
    auto & c = Valid("enable event on a");
    // Assigned event handle
    dpp::event_handle eh = 0;
    // Make sure the specified event handle is valid
    if (id >= 0 && id < static_cast< SQInteger >(DpEventID::Max))
    {
        eh = mEventsHandle[static_cast< size_t >(id)]; // Get the real handle
    }
    // Is this event already enabled?
    if (eh != 0)
    {
        return *this; // Job already done
    }
    // Identify event type
    switch (id)
    {
        case DpEventID::VoiceStateUpdate:               eh = c.on_voice_state_update.attach([this](const dpp::voice_state_update_t & e) { OnVoiceStateUpdate(e); }); break;
        case DpEventID::VoiceClientDisconnect:          eh = c.on_voice_client_disconnect.attach([this](const dpp::voice_client_disconnect_t & e) { OnVoiceClientDisconnect(e); }); break;
        case DpEventID::VoiceClientSpeaking:            eh = c.on_voice_client_speaking.attach([this](const dpp::voice_client_speaking_t & e) { OnVoiceClientSpeaking(e); }); break;
        case DpEventID::Log:                            eh = c.on_log.attach([this](const dpp::log_t & e) { OnLog(e); }); break;
        case DpEventID::GuildJoinRequestDelete:         eh = c.on_guild_join_request_delete.attach([this](const dpp::guild_join_request_delete_t & e) { OnGuildJoinRequestDelete(e); }); break;
        case DpEventID::InteractionCreate:              eh = c.on_interaction_create.attach([this](const dpp::interaction_create_t & e) { OnInteractionCreate(e); }); break;
        case DpEventID::SlashCommand:                   eh = c.on_slashcommand.attach([this](const dpp::slashcommand_t & e) { OnSlashCommand(e); }); break;
        case DpEventID::ButtonClick:                    eh = c.on_button_click.attach([this](const dpp::button_click_t & e) { OnButtonClick(e); }); break;
        case DpEventID::AutoComplete:                   eh = c.on_autocomplete.attach([this](const dpp::autocomplete_t & e) { OnAutoComplete(e); }); break;
        case DpEventID::SelectClick:                    eh = c.on_select_click.attach([this](const dpp::select_click_t & e) { OnSelectClick(e); }); break;
        case DpEventID::MessageContextMenu:             eh = c.on_message_context_menu.attach([this](const dpp::message_context_menu_t & e) { OnMessageContextMenu(e); }); break;
        case DpEventID::UserContextMenu:                eh = c.on_user_context_menu.attach([this](const dpp::user_context_menu_t & e) { OnUserContextMenu(e); }); break;
        case DpEventID::FormSubmit:                     eh = c.on_form_submit.attach([this](const dpp::form_submit_t & e) { OnFormSubmit(e); }); break;
        case DpEventID::GuildDelete:                    eh = c.on_guild_delete.attach([this](const dpp::guild_delete_t & e) { OnGuildDelete(e); }); break;
        case DpEventID::ChannelDelete:                  eh = c.on_channel_delete.attach([this](const dpp::channel_delete_t & e) { OnChannelDelete(e); }); break;
        case DpEventID::ChannelUpdate:                  eh = c.on_channel_update.attach([this](const dpp::channel_update_t & e) { OnChannelUpdate(e); }); break;
        case DpEventID::Ready:                          eh = c.on_ready.attach([this](const dpp::ready_t & e) { OnReady(e); }); break;
        case DpEventID::MessageDelete:                  eh = c.on_message_delete.attach([this](const dpp::message_delete_t & e) { OnMessageDelete(e); }); break;
        case DpEventID::GuildMemberRemove:              eh = c.on_guild_member_remove.attach([this](const dpp::guild_member_remove_t & e) { OnGuildMemberRemove(e); }); break;
        case DpEventID::Resumed:                        eh = c.on_resumed.attach([this](const dpp::resumed_t & e) { OnResumed(e); }); break;
        case DpEventID::GuildRoleCreate:                eh = c.on_guild_role_create.attach([this](const dpp::guild_role_create_t & e) { OnGuildRoleCreate(e); }); break;
        case DpEventID::TypingStart:                    eh = c.on_typing_start.attach([this](const dpp::typing_start_t & e) { OnTypingStart(e); }); break;
        case DpEventID::MessageReactionAdd:             eh = c.on_message_reaction_add.attach([this](const dpp::message_reaction_add_t & e) { OnMessageReactionAdd(e); }); break;
        case DpEventID::GuildMembersChunk:              eh = c.on_guild_members_chunk.attach([this](const dpp::guild_members_chunk_t & e) { OnGuildMembersChunk(e); }); break;
        case DpEventID::MessageReactionRemove:          eh = c.on_message_reaction_remove.attach([this](const dpp::message_reaction_remove_t & e) { OnMessageReactionRemove(e); }); break;
        case DpEventID::GuildCreate:                    eh = c.on_guild_create.attach([this](const dpp::guild_create_t & e) { OnGuildCreate(e); }); break;
        case DpEventID::ChannelCreate:                  eh = c.on_channel_create.attach([this](const dpp::channel_create_t & e) { OnChannelCreate(e); }); break;
        case DpEventID::MessageReactionRemoveEmoji:     eh = c.on_message_reaction_remove_emoji.attach([this](const dpp::message_reaction_remove_emoji_t & e) { OnMessageReactionRemoveEmoji(e); }); break;
        case DpEventID::MessageDeleteDulk:              eh = c.on_message_delete_bulk.attach([this](const dpp::message_delete_bulk_t & e) { OnMessageDeleteDulk(e); }); break;
        case DpEventID::GuildRoleUpdate:                eh = c.on_guild_role_update.attach([this](const dpp::guild_role_update_t & e) { OnGuildRoleUpdate(e); }); break;
        case DpEventID::GuildRoleDelete:                eh = c.on_guild_role_delete.attach([this](const dpp::guild_role_delete_t & e) { OnGuildRoleDelete(e); }); break;
        case DpEventID::ChannelPinsUpdate:              eh = c.on_channel_pins_update.attach([this](const dpp::channel_pins_update_t & e) { OnChannelPinsUpdate(e); }); break;
        case DpEventID::MessageReactionRemoveAll:       eh = c.on_message_reaction_remove_all.attach([this](const dpp::message_reaction_remove_all_t & e) { OnMessageReactionRemoveAll(e); }); break;
        case DpEventID::VoiceServerUpdate:              eh = c.on_voice_server_update.attach([this](const dpp::voice_server_update_t & e) { OnVoiceServerUpdate(e); }); break;
        case DpEventID::GuildEmojisUpdate:              eh = c.on_guild_emojis_update.attach([this](const dpp::guild_emojis_update_t & e) { OnGuildEmojisUpdate(e); }); break;
        case DpEventID::GuildStickersUpdate:            eh = c.on_guild_stickers_update.attach([this](const dpp::guild_stickers_update_t & e) { OnGuildStickersUpdate(e); }); break;
        case DpEventID::PresenceUpdate:                 eh = c.on_presence_update.attach([this](const dpp::presence_update_t & e) { OnPresenceUpdate(e); }); break;
        case DpEventID::WebhooksUpdate:                 eh = c.on_webhooks_update.attach([this](const dpp::webhooks_update_t & e) { OnWebhooksUpdate(e); }); break;
        case DpEventID::AutomodRuleCreate:              eh = c.on_automod_rule_create.attach([this](const dpp::automod_rule_create_t & e) { OnAutomodRuleCreate(e); }); break;
        case DpEventID::AutomodRuleUpdate:              eh = c.on_automod_rule_update.attach([this](const dpp::automod_rule_update_t & e) { OnAutomodRuleUpdate(e); }); break;
        case DpEventID::AutomodRuleDelete:              eh = c.on_automod_rule_delete.attach([this](const dpp::automod_rule_delete_t & e) { OnAutomodRuleDelete(e); }); break;
        case DpEventID::AutomodRuleExecute:             eh = c.on_automod_rule_execute.attach([this](const dpp::automod_rule_execute_t & e) { OnAutomodRuleExecute(e); }); break;
        case DpEventID::GuildMemberAdd:                 eh = c.on_guild_member_add.attach([this](const dpp::guild_member_add_t & e) { OnGuildMemberAdd(e); }); break;
        case DpEventID::InviteDelete:                   eh = c.on_invite_delete.attach([this](const dpp::invite_delete_t & e) { OnInviteDelete(e); }); break;
        case DpEventID::GuildUpdate:                    eh = c.on_guild_update.attach([this](const dpp::guild_update_t & e) { OnGuildUpdate(e); }); break;
        case DpEventID::GuildIntegrationsUpdate:        eh = c.on_guild_integrations_update.attach([this](const dpp::guild_integrations_update_t & e) { OnGuildIntegrationsUpdate(e); }); break;
        case DpEventID::GuildMemberUpdate:              eh = c.on_guild_member_update.attach([this](const dpp::guild_member_update_t & e) { OnGuildMemberUpdate(e); }); break;
        case DpEventID::InviteCreate:                   eh = c.on_invite_create.attach([this](const dpp::invite_create_t & e) { OnInviteCreate(e); }); break;
        case DpEventID::MessageUpdate:                  eh = c.on_message_update.attach([this](const dpp::message_update_t & e) { OnMessageUpdate(e); }); break;
        case DpEventID::UserUpdate:                     eh = c.on_user_update.attach([this](const dpp::user_update_t & e) { OnUserUpdate(e); }); break;
        case DpEventID::MessageCreate:                  eh = c.on_message_create.attach([this](const dpp::message_create_t & e) { OnMessageCreate(e); }); break;
        case DpEventID::GuildAuditLogEntryCreate:       eh = c.on_guild_audit_log_entry_create.attach([this](const dpp::guild_audit_log_entry_create_t & e) { OnGuildAuditLogEntryCreate(e); }); break;
        case DpEventID::GuildBanAdd:                    eh = c.on_guild_ban_add.attach([this](const dpp::guild_ban_add_t & e) { OnGuildBanAdd(e); }); break;
        case DpEventID::GuildBanRemove:                 eh = c.on_guild_ban_remove.attach([this](const dpp::guild_ban_remove_t & e) { OnGuildBanRemove(e); }); break;
        case DpEventID::IntegrationCreate:              eh = c.on_integration_create.attach([this](const dpp::integration_create_t & e) { OnIntegrationCreate(e); }); break;
        case DpEventID::IntegrationUpdate:              eh = c.on_integration_update.attach([this](const dpp::integration_update_t & e) { OnIntegrationUpdate(e); }); break;
        case DpEventID::IntegrationDelete:              eh = c.on_integration_delete.attach([this](const dpp::integration_delete_t & e) { OnIntegrationDelete(e); }); break;
        case DpEventID::ThreadCreate:                   eh = c.on_thread_create.attach([this](const dpp::thread_create_t & e) { OnThreadCreate(e); }); break;
        case DpEventID::ThreadUpdate:                   eh = c.on_thread_update.attach([this](const dpp::thread_update_t & e) { OnThreadUpdate(e); }); break;
        case DpEventID::ThreadDelete:                   eh = c.on_thread_delete.attach([this](const dpp::thread_delete_t & e) { OnThreadDelete(e); }); break;
        case DpEventID::ThreadListSync:                 eh = c.on_thread_list_sync.attach([this](const dpp::thread_list_sync_t & e) { OnThreadListSync(e); }); break;
        case DpEventID::ThreadMemberUpdate:             eh = c.on_thread_member_update.attach([this](const dpp::thread_member_update_t & e) { OnThreadMemberUpdate(e); }); break;
        case DpEventID::ThreadMembersUpdate:            eh = c.on_thread_members_update.attach([this](const dpp::thread_members_update_t & e) { OnThreadMembersUpdate(e); }); break;
        case DpEventID::GuildScheduledEventCreate:      eh = c.on_guild_scheduled_event_create.attach([this](const dpp::guild_scheduled_event_create_t & e) { OnGuildScheduledEventCreate(e); }); break;
        case DpEventID::GuildScheduledEventUpdate:      eh = c.on_guild_scheduled_event_update.attach([this](const dpp::guild_scheduled_event_update_t & e) { OnGuildScheduledEventUpdate(e); }); break;
        case DpEventID::GuildScheduledEventDelete:      eh = c.on_guild_scheduled_event_delete.attach([this](const dpp::guild_scheduled_event_delete_t & e) { OnGuildScheduledEventDelete(e); }); break;
        case DpEventID::GuildScheduledEventUserAdd:     eh = c.on_guild_scheduled_event_user_add.attach([this](const dpp::guild_scheduled_event_user_add_t & e) { OnGuildScheduledEventUserAdd(e); }); break;
        case DpEventID::GuildScheduledEventUserRemove:  eh = c.on_guild_scheduled_event_user_remove.attach([this](const dpp::guild_scheduled_event_user_remove_t & e) { OnGuildScheduledEventUserRemove(e); }); break;
        case DpEventID::VoiceBufferSend:                eh = c.on_voice_buffer_send.attach([this](const dpp::voice_buffer_send_t & e) { OnVoiceBufferSend(e); }); break;
        case DpEventID::VoiceUserTalking:               eh = c.on_voice_user_talking.attach([this](const dpp::voice_user_talking_t & e) { OnVoiceUserTalking(e); }); break;
        case DpEventID::VoiceReady:                     eh = c.on_voice_ready.attach([this](const dpp::voice_ready_t & e) { OnVoiceReady(e); }); break;
        case DpEventID::VoiceReceive:                   eh = c.on_voice_receive.attach([this](const dpp::voice_receive_t & e) { OnVoiceReceive(e); }); break;
        case DpEventID::VoiceReceiveCombined:           eh = c.on_voice_receive_combined.attach([this](const dpp::voice_receive_t & e) { OnVoiceReceiveCombined(e); }); break;
        case DpEventID::VoiceTrackMarker:               eh = c.on_voice_track_marker.attach([this](const dpp::voice_track_marker_t & e) { OnVoiceTrackMarker(e); }); break;
        case DpEventID::StageInstanceCreate:            eh = c.on_stage_instance_create.attach([this](const dpp::stage_instance_create_t & e) { OnStageInstanceCreate(e); }); break;
        case DpEventID::StageInstanceUpdate:            eh = c.on_stage_instance_update.attach([this](const dpp::stage_instance_update_t & e) { OnStageInstanceUpdate(e); }); break;
        case DpEventID::StageInstanceDelete:            eh = c.on_stage_instance_delete.attach([this](const dpp::stage_instance_delete_t & e) { OnStageInstanceDelete(e); }); break;
        case DpEventID::Max: // Fall through
        default: STHROWF("Invalid discord event identifier {}", id);
    }
    // Remember the designated event handle
    mEventsHandle[static_cast< size_t >(id)] = eh;
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
DpCluster & DpCluster::DisableEvent(SQInteger id)
{
    // Retrieve managed cluster instance
    auto & c = Valid("disable event on a");
    // Assigned event handle
    dpp::event_handle eh = 0;
    // Make sure the specified event handle is valid
    if (id >= 0 && id < static_cast< SQInteger >(DpEventID::Max))
    {
        eh = mEventsHandle[static_cast< size_t >(id)]; // Get the real handle
    }
    // Is there anything attached to event dispatched?
    if (eh == 0)
    {
        return *this; // Nothing to detach
    }
    // Identify event type
    switch (id)
    {
        case DpEventID::VoiceStateUpdate:               c.on_voice_state_update.detach(eh); break;
        case DpEventID::VoiceClientDisconnect:          c.on_voice_client_disconnect.detach(eh); break;
        case DpEventID::VoiceClientSpeaking:            c.on_voice_client_speaking.detach(eh); break;
        case DpEventID::Log:                            c.on_log.detach(eh); break;
        case DpEventID::GuildJoinRequestDelete:         c.on_guild_join_request_delete.detach(eh); break;
        case DpEventID::InteractionCreate:              c.on_interaction_create.detach(eh); break;
        case DpEventID::SlashCommand:                   c.on_slashcommand.detach(eh); break;
        case DpEventID::ButtonClick:                    c.on_button_click.detach(eh); break;
        case DpEventID::AutoComplete:                   c.on_autocomplete.detach(eh); break;
        case DpEventID::SelectClick:                    c.on_select_click.detach(eh); break;
        case DpEventID::MessageContextMenu:             c.on_message_context_menu.detach(eh); break;
        case DpEventID::UserContextMenu:                c.on_user_context_menu.detach(eh); break;
        case DpEventID::FormSubmit:                     c.on_form_submit.detach(eh); break;
        case DpEventID::GuildDelete:                    c.on_guild_delete.detach(eh); break;
        case DpEventID::ChannelDelete:                  c.on_channel_delete.detach(eh); break;
        case DpEventID::ChannelUpdate:                  c.on_channel_update.detach(eh); break;
        case DpEventID::Ready:                          c.on_ready.detach(eh); break;
        case DpEventID::MessageDelete:                  c.on_message_delete.detach(eh); break;
        case DpEventID::GuildMemberRemove:              c.on_guild_member_remove.detach(eh); break;
        case DpEventID::Resumed:                        c.on_resumed.detach(eh); break;
        case DpEventID::GuildRoleCreate:                c.on_guild_role_create.detach(eh); break;
        case DpEventID::TypingStart:                    c.on_typing_start.detach(eh); break;
        case DpEventID::MessageReactionAdd:             c.on_message_reaction_add.detach(eh); break;
        case DpEventID::GuildMembersChunk:              c.on_guild_members_chunk.detach(eh); break;
        case DpEventID::MessageReactionRemove:          c.on_message_reaction_remove.detach(eh); break;
        case DpEventID::GuildCreate:                    c.on_guild_create.detach(eh); break;
        case DpEventID::ChannelCreate:                  c.on_channel_create.detach(eh); break;
        case DpEventID::MessageReactionRemoveEmoji:     c.on_message_reaction_remove_emoji.detach(eh); break;
        case DpEventID::MessageDeleteDulk:              c.on_message_delete_bulk.detach(eh); break;
        case DpEventID::GuildRoleUpdate:                c.on_guild_role_update.detach(eh); break;
        case DpEventID::GuildRoleDelete:                c.on_guild_role_delete.detach(eh); break;
        case DpEventID::ChannelPinsUpdate:              c.on_channel_pins_update.detach(eh); break;
        case DpEventID::MessageReactionRemoveAll:       c.on_message_reaction_remove_all.detach(eh); break;
        case DpEventID::VoiceServerUpdate:              c.on_voice_server_update.detach(eh); break;
        case DpEventID::GuildEmojisUpdate:              c.on_guild_emojis_update.detach(eh); break;
        case DpEventID::GuildStickersUpdate:            c.on_guild_stickers_update.detach(eh); break;
        case DpEventID::PresenceUpdate:                 c.on_presence_update.detach(eh); break;
        case DpEventID::WebhooksUpdate:                 c.on_webhooks_update.detach(eh); break;
        case DpEventID::AutomodRuleCreate:              c.on_automod_rule_create.detach(eh); break;
        case DpEventID::AutomodRuleUpdate:              c.on_automod_rule_update.detach(eh); break;
        case DpEventID::AutomodRuleDelete:              c.on_automod_rule_delete.detach(eh); break;
        case DpEventID::AutomodRuleExecute:             c.on_automod_rule_execute.detach(eh); break;
        case DpEventID::GuildMemberAdd:                 c.on_guild_member_add.detach(eh); break;
        case DpEventID::InviteDelete:                   c.on_invite_delete.detach(eh); break;
        case DpEventID::GuildUpdate:                    c.on_guild_update.detach(eh); break;
        case DpEventID::GuildIntegrationsUpdate:        c.on_guild_integrations_update.detach(eh); break;
        case DpEventID::GuildMemberUpdate:              c.on_guild_member_update.detach(eh); break;
        case DpEventID::InviteCreate:                   c.on_invite_create.detach(eh); break;
        case DpEventID::MessageUpdate:                  c.on_message_update.detach(eh); break;
        case DpEventID::UserUpdate:                     c.on_user_update.detach(eh); break;
        case DpEventID::MessageCreate:                  c.on_message_create.detach(eh); break;
        case DpEventID::GuildAuditLogEntryCreate:       c.on_guild_audit_log_entry_create.detach(eh); break;
        case DpEventID::GuildBanAdd:                    c.on_guild_ban_add.detach(eh); break;
        case DpEventID::GuildBanRemove:                 c.on_guild_ban_remove.detach(eh); break;
        case DpEventID::IntegrationCreate:              c.on_integration_create.detach(eh); break;
        case DpEventID::IntegrationUpdate:              c.on_integration_update.detach(eh); break;
        case DpEventID::IntegrationDelete:              c.on_integration_delete.detach(eh); break;
        case DpEventID::ThreadCreate:                   c.on_thread_create.detach(eh); break;
        case DpEventID::ThreadUpdate:                   c.on_thread_update.detach(eh); break;
        case DpEventID::ThreadDelete:                   c.on_thread_delete.detach(eh); break;
        case DpEventID::ThreadListSync:                 c.on_thread_list_sync.detach(eh); break;
        case DpEventID::ThreadMemberUpdate:             c.on_thread_member_update.detach(eh); break;
        case DpEventID::ThreadMembersUpdate:            c.on_thread_members_update.detach(eh); break;
        case DpEventID::GuildScheduledEventCreate:      c.on_guild_scheduled_event_create.detach(eh); break;
        case DpEventID::GuildScheduledEventUpdate:      c.on_guild_scheduled_event_update.detach(eh); break;
        case DpEventID::GuildScheduledEventDelete:      c.on_guild_scheduled_event_delete.detach(eh); break;
        case DpEventID::GuildScheduledEventUserAdd:     c.on_guild_scheduled_event_user_add.detach(eh); break;
        case DpEventID::GuildScheduledEventUserRemove:  c.on_guild_scheduled_event_user_remove.detach(eh); break;
        case DpEventID::VoiceBufferSend:                c.on_voice_buffer_send.detach(eh); break;
        case DpEventID::VoiceUserTalking:               c.on_voice_user_talking.detach(eh); break;
        case DpEventID::VoiceReady:                     c.on_voice_ready.detach(eh); break;
        case DpEventID::VoiceReceive:                   c.on_voice_receive.detach(eh); break;
        case DpEventID::VoiceReceiveCombined:           c.on_voice_receive_combined.detach(eh); break;
        case DpEventID::VoiceTrackMarker:               c.on_voice_track_marker.detach(eh); break;
        case DpEventID::StageInstanceCreate:            c.on_stage_instance_create.detach(eh); break;
        case DpEventID::StageInstanceUpdate:            c.on_stage_instance_update.detach(eh); break;
        case DpEventID::StageInstanceDelete:            c.on_stage_instance_delete.detach(eh); break;
        case DpEventID::Max: // Fall through
        default: STHROWF("Invalid discord event identifier {}", id);
    }
    // Forget about this event handler
    mEventsHandle[static_cast< size_t >(id)] = 0;
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
void DpCluster::OnVoiceStateUpdate(const dpp::voice_state_update_t & ev)
{ mQueue.enqueue(EventItem(new DpVoiceStateUpdateEvent(ev))); }
void DpCluster::OnVoiceClientDisconnect(const dpp::voice_client_disconnect_t & ev)
{ mQueue.enqueue(EventItem(new DpVoiceClientDisconnectEvent(ev))); }
void DpCluster::OnVoiceClientSpeaking(const dpp::voice_client_speaking_t & ev)
{ mQueue.enqueue(EventItem(new DpVoiceClientSpeakingEvent(ev))); }
void DpCluster::OnLog(const dpp::log_t & ev)
{ mQueue.enqueue(EventItem(new DpLogEvent(ev))); }
void DpCluster::OnGuildJoinRequestDelete(const dpp::guild_join_request_delete_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildJoinRequestDeleteEvent(ev))); }
void DpCluster::OnInteractionCreate(const dpp::interaction_create_t & ev)
{ mQueue.enqueue(EventItem(new DpInteractionCreateEvent(ev))); }
void DpCluster::OnSlashCommand(const dpp::slashcommand_t & ev)
{ mQueue.enqueue(EventItem(new DpSlashCommandEvent(ev))); }
void DpCluster::OnButtonClick(const dpp::button_click_t & ev)
{ mQueue.enqueue(EventItem(new DpButtonClickEvent(ev))); }
void DpCluster::OnAutoComplete(const dpp::autocomplete_t & ev)
{ mQueue.enqueue(EventItem(new DpAutoCompleteEvent(ev))); }
void DpCluster::OnSelectClick(const dpp::select_click_t & ev)
{ mQueue.enqueue(EventItem(new DpSelectClickEvent(ev))); }
void DpCluster::OnMessageContextMenu(const dpp::message_context_menu_t & ev)
{ mQueue.enqueue(EventItem(new DpMessageContextMenuEvent(ev))); }
void DpCluster::OnUserContextMenu(const dpp::user_context_menu_t & ev)
{ mQueue.enqueue(EventItem(new DpUserContextMenuEvent(ev))); }
void DpCluster::OnFormSubmit(const dpp::form_submit_t & ev)
{ mQueue.enqueue(EventItem(new DpFormSubmitEvent(ev))); }
void DpCluster::OnGuildDelete(const dpp::guild_delete_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildDeleteEvent(ev))); }
void DpCluster::OnChannelDelete(const dpp::channel_delete_t & ev)
{ mQueue.enqueue(EventItem(new DpChannelDeleteEvent(ev))); }
void DpCluster::OnChannelUpdate(const dpp::channel_update_t & ev)
{ mQueue.enqueue(EventItem(new DpChannelUpdateEvent(ev))); }
void DpCluster::OnReady(const dpp::ready_t & ev)
{ mQueue.enqueue(EventItem(new DpReadyEvent(ev))); }
void DpCluster::OnMessageDelete(const dpp::message_delete_t & ev)
{ mQueue.enqueue(EventItem(new DpMessageDeleteEvent(ev))); }
void DpCluster::OnGuildMemberRemove(const dpp::guild_member_remove_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildMemberRemoveEvent(ev))); }
void DpCluster::OnResumed(const dpp::resumed_t & ev)
{ mQueue.enqueue(EventItem(new DpResumedEvent(ev))); }
void DpCluster::OnGuildRoleCreate(const dpp::guild_role_create_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildRoleCreateEvent(ev))); }
void DpCluster::OnTypingStart(const dpp::typing_start_t & ev)
{ mQueue.enqueue(EventItem(new DpTypingStartEvent(ev))); }
void DpCluster::OnMessageReactionAdd(const dpp::message_reaction_add_t & ev)
{ mQueue.enqueue(EventItem(new DpMessageReactionAddEvent(ev))); }
void DpCluster::OnGuildMembersChunk(const dpp::guild_members_chunk_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildMembersChunkEvent(ev))); }
void DpCluster::OnMessageReactionRemove(const dpp::message_reaction_remove_t & ev)
{ mQueue.enqueue(EventItem(new DpMessageReactionRemoveEvent(ev))); }
void DpCluster::OnGuildCreate(const dpp::guild_create_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildCreateEvent(ev))); }
void DpCluster::OnChannelCreate(const dpp::channel_create_t & ev)
{ mQueue.enqueue(EventItem(new DpChannelCreateEvent(ev))); }
void DpCluster::OnMessageReactionRemoveEmoji(const dpp::message_reaction_remove_emoji_t & ev)
{ mQueue.enqueue(EventItem(new DpMessageReactionRemoveEmojiEvent(ev))); }
void DpCluster::OnMessageDeleteDulk(const dpp::message_delete_bulk_t & ev)
{ mQueue.enqueue(EventItem(new DpMessageDeleteDulkEvent(ev))); }
void DpCluster::OnGuildRoleUpdate(const dpp::guild_role_update_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildRoleUpdateEvent(ev))); }
void DpCluster::OnGuildRoleDelete(const dpp::guild_role_delete_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildRoleDeleteEvent(ev))); }
void DpCluster::OnChannelPinsUpdate(const dpp::channel_pins_update_t & ev)
{ mQueue.enqueue(EventItem(new DpChannelPinsUpdateEvent(ev))); }
void DpCluster::OnMessageReactionRemoveAll(const dpp::message_reaction_remove_all_t & ev)
{ mQueue.enqueue(EventItem(new DpMessageReactionRemoveAllEvent(ev))); }
void DpCluster::OnVoiceServerUpdate(const dpp::voice_server_update_t & ev)
{ mQueue.enqueue(EventItem(new DpVoiceServerUpdateEvent(ev))); }
void DpCluster::OnGuildEmojisUpdate(const dpp::guild_emojis_update_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildEmojisUpdateEvent(ev))); }
void DpCluster::OnGuildStickersUpdate(const dpp::guild_stickers_update_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildStickersUpdateEvent(ev))); }
void DpCluster::OnPresenceUpdate(const dpp::presence_update_t & ev)
{ mQueue.enqueue(EventItem(new DpPresenceUpdateEvent(ev))); }
void DpCluster::OnWebhooksUpdate(const dpp::webhooks_update_t & ev)
{ mQueue.enqueue(EventItem(new DpWebhooksUpdateEvent(ev))); }
void DpCluster::OnAutomodRuleCreate(const dpp::automod_rule_create_t & ev)
{ mQueue.enqueue(EventItem(new DpAutomodRuleCreateEvent(ev))); }
void DpCluster::OnAutomodRuleUpdate(const dpp::automod_rule_update_t & ev)
{ mQueue.enqueue(EventItem(new DpAutomodRuleUpdateEvent(ev))); }
void DpCluster::OnAutomodRuleDelete(const dpp::automod_rule_delete_t & ev)
{ mQueue.enqueue(EventItem(new DpAutomodRuleDeleteEvent(ev))); }
void DpCluster::OnAutomodRuleExecute(const dpp::automod_rule_execute_t & ev)
{ mQueue.enqueue(EventItem(new DpAutomodRuleExecuteEvent(ev))); }
void DpCluster::OnGuildMemberAdd(const dpp::guild_member_add_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildMemberAddEvent(ev))); }
void DpCluster::OnInviteDelete(const dpp::invite_delete_t & ev)
{ mQueue.enqueue(EventItem(new DpInviteDeleteEvent(ev))); }
void DpCluster::OnGuildUpdate(const dpp::guild_update_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildUpdateEvent(ev))); }
void DpCluster::OnGuildIntegrationsUpdate(const dpp::guild_integrations_update_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildIntegrationsUpdateEvent(ev))); }
void DpCluster::OnGuildMemberUpdate(const dpp::guild_member_update_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildMemberUpdateEvent(ev))); }
void DpCluster::OnInviteCreate(const dpp::invite_create_t & ev)
{ mQueue.enqueue(EventItem(new DpInviteCreateEvent(ev))); }
void DpCluster::OnMessageUpdate(const dpp::message_update_t & ev)
{ mQueue.enqueue(EventItem(new DpMessageUpdateEvent(ev))); }
void DpCluster::OnUserUpdate(const dpp::user_update_t & ev)
{ mQueue.enqueue(EventItem(new DpUserUpdateEvent(ev))); }
void DpCluster::OnMessageCreate(const dpp::message_create_t & ev)
{ mQueue.enqueue(EventItem(new DpMessageCreateEvent(ev))); }
void DpCluster::OnGuildAuditLogEntryCreate(const dpp::guild_audit_log_entry_create_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildAuditLogEntryCreateEvent(ev))); }
void DpCluster::OnGuildBanAdd(const dpp::guild_ban_add_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildBanAddEvent(ev))); }
void DpCluster::OnGuildBanRemove(const dpp::guild_ban_remove_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildBanRemoveEvent(ev))); }
void DpCluster::OnIntegrationCreate(const dpp::integration_create_t & ev)
{ mQueue.enqueue(EventItem(new DpIntegrationCreateEvent(ev))); }
void DpCluster::OnIntegrationUpdate(const dpp::integration_update_t & ev)
{ mQueue.enqueue(EventItem(new DpIntegrationUpdateEvent(ev))); }
void DpCluster::OnIntegrationDelete(const dpp::integration_delete_t & ev)
{ mQueue.enqueue(EventItem(new DpIntegrationDeleteEvent(ev))); }
void DpCluster::OnThreadCreate(const dpp::thread_create_t & ev)
{ mQueue.enqueue(EventItem(new DpThreadCreateEvent(ev))); }
void DpCluster::OnThreadUpdate(const dpp::thread_update_t & ev)
{ mQueue.enqueue(EventItem(new DpThreadUpdateEvent(ev))); }
void DpCluster::OnThreadDelete(const dpp::thread_delete_t & ev)
{ mQueue.enqueue(EventItem(new DpThreadDeleteEvent(ev))); }
void DpCluster::OnThreadListSync(const dpp::thread_list_sync_t & ev)
{ mQueue.enqueue(EventItem(new DpThreadListSyncEvent(ev))); }
void DpCluster::OnThreadMemberUpdate(const dpp::thread_member_update_t & ev)
{ mQueue.enqueue(EventItem(new DpThreadMemberUpdateEvent(ev))); }
void DpCluster::OnThreadMembersUpdate(const dpp::thread_members_update_t & ev)
{ mQueue.enqueue(EventItem(new DpThreadMembersUpdateEvent(ev))); }
void DpCluster::OnGuildScheduledEventCreate(const dpp::guild_scheduled_event_create_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildScheduledEventCreateEvent(ev))); }
void DpCluster::OnGuildScheduledEventUpdate(const dpp::guild_scheduled_event_update_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildScheduledEventUpdateEvent(ev))); }
void DpCluster::OnGuildScheduledEventDelete(const dpp::guild_scheduled_event_delete_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildScheduledEventDeleteEvent(ev))); }
void DpCluster::OnGuildScheduledEventUserAdd(const dpp::guild_scheduled_event_user_add_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildScheduledEventUserAddEvent(ev))); }
void DpCluster::OnGuildScheduledEventUserRemove(const dpp::guild_scheduled_event_user_remove_t & ev)
{ mQueue.enqueue(EventItem(new DpGuildScheduledEventUserRemoveEvent(ev))); }
void DpCluster::OnVoiceBufferSend(const dpp::voice_buffer_send_t & ev)
{ mQueue.enqueue(EventItem(new DpVoiceBufferSendEvent(ev))); }
void DpCluster::OnVoiceUserTalking(const dpp::voice_user_talking_t & ev)
{ mQueue.enqueue(EventItem(new DpVoiceUserTalkingEvent(ev))); }
void DpCluster::OnVoiceReady(const dpp::voice_ready_t & ev)
{ mQueue.enqueue(EventItem(new DpVoiceReadyEvent(ev))); }
void DpCluster::OnVoiceReceive(const dpp::voice_receive_t & ev)
{ mQueue.enqueue(EventItem(new DpVoiceReceiveEvent(ev))); }
void DpCluster::OnVoiceReceiveCombined(const dpp::voice_receive_t & ev)
{ mQueue.enqueue(EventItem(new DpVoiceReceiveCombinedEvent(ev))); }
void DpCluster::OnVoiceTrackMarker(const dpp::voice_track_marker_t & ev)
{ mQueue.enqueue(EventItem(new DpVoiceTrackMarkerEvent(ev))); }
void DpCluster::OnStageInstanceCreate(const dpp::stage_instance_create_t & ev)
{ mQueue.enqueue(EventItem(new DpStageInstanceCreateEvent(ev))); }
void DpCluster::OnStageInstanceUpdate(const dpp::stage_instance_update_t & ev)
{ mQueue.enqueue(EventItem(new DpStageInstanceUpdateEvent(ev))); }
void DpCluster::OnStageInstanceDelete(const dpp::stage_instance_delete_t & ev)
{ mQueue.enqueue(EventItem(new DpStageInstanceDeleteEvent(ev))); }

} // Namespace:: SqMod
