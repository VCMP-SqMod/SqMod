#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"
#include "Core/Signal.hpp"

// ------------------------------------------------------------------------------------------------
#include "Library/Discord/Constants.hpp"
#include "Library/Discord/Misc.hpp"

// ------------------------------------------------------------------------------------------------
#include <chrono>
#include <memory>
#include <functional>
#include <string_view>

// ------------------------------------------------------------------------------------------------
#include <concurrentqueue.h>
#include <dpp/dpp.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
struct DpEventBase;

/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpClusterOptions
{
    // The bot token to use for all HTTP commands and websocket connections.
    std::string         mToken{};
    // A bitmask of dpd::intents values for all shards on this cluster. This is required to be sent for all bots with over 100 servers.
    uint32_t            mIntents{dpp::i_default_intents};
    // The total number of shards on this bot. If there are multiple clusters, then (shards / clusters) actual shards will run on this cluster.
    // If you omit this value, the library will attempt to query the Discord API for the correct number of shards to start.
    uint32_t            mShards{0};
    // The ID of this cluster, should be between 0 and MAXCLUSTERS-1
    uint32_t            mClusterID{0};
    // The total number of clusters that are active, which may be on separate processes or even separate machines.
    uint32_t            mMaxClusters{1};
    // Whether or not to use compression for shards on this cluster. Saves a ton of bandwidth at the cost of some CPU
    bool                mCompressed{true};
    // Set the user caching policy for the cluster, either lazy (only cache users/members when they message the bot) or aggressive (request whole member lists on seeing new guilds too)
    dpp::cache_policy_t mPolicy{dpp::cp_aggressive, dpp::cp_aggressive, dpp::cp_aggressive};
    // The number of threads to allocate for making HTTP requests to Discord. This defaults to 12. You can increase this at runtime via the object returned from get_rest().
    uint32_t            mRequestThreads{12};
    // The number of threads to allocate for making HTTP requests to sites outside of Discord. This defaults to 1. You can increase this at runtime via the object returned from get_raw_rest().
    uint32_t            mRequestThreadsRaw{1};
    // Disable automatic forwarding of logged messages to internal logging. Allows to handle logging manually without risking duplicate output (at the cost of some performance, ofc).
    bool                mCustomLogging{false};

    /* --------------------------------------------------------------------------------------------
     * Base constructors.
    */
    DpClusterOptions(StackStrF & token)
        : mToken(token.ToStr())
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy/Move constructors.
    */
    DpClusterOptions(const DpClusterOptions &) noexcept = default;
    DpClusterOptions(DpClusterOptions &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Copy/Move assignment operators.
    */
    DpClusterOptions & operator = (const DpClusterOptions &) noexcept = default;
    DpClusterOptions & operator = (DpClusterOptions &&) noexcept = default;
};

/* ------------------------------------------------------------------------------------------------
 * Primitive implementation of a discord client with basic functionality.
*/
struct DpCluster : public SqChainedInstances< DpCluster >
{
    using Base = SqChainedInstances< DpCluster >;

    /* --------------------------------------------------------------------------------------------
     * Queue of events generated from other threads.
    */
    using EventItem = std::unique_ptr< DpEventBase >;
    using EventQueue = moodycamel::ConcurrentQueue< EventItem >;

    /* --------------------------------------------------------------------------------------------
     * Type of container for cluster signals.
    */
    using Signals = std::array< SignalPair, static_cast< size_t >(DpEventID::Max) >;

    /* --------------------------------------------------------------------------------------------
     * Type of container for event handles.
    */
    using EventHandle = std::array< dpp::event_handle, static_cast< size_t >(DpEventID::Max) >;

    /* --------------------------------------------------------------------------------------------
     * Event queue.
    */
    EventQueue mQueue{4096};

    /* --------------------------------------------------------------------------------------------
     * Managed cluster instance.
    */
    std::unique_ptr< dpp::cluster > mC;

    /* --------------------------------------------------------------------------------------------
     * Table containing the emitted cluster events.
    */
    LightObj mSqEvents{};

    /* --------------------------------------------------------------------------------------------
     * Cluster signals.
    */
    Signals mEvents{};

    /* --------------------------------------------------------------------------------------------
     * Event handles for bound event event handlers so they can be stopped at any time.
    */
    EventHandle mEventsHandle{};

    /* --------------------------------------------------------------------------------------------
     * Base constructors.
    */
    DpCluster(DpClusterOptions & o);

    /* --------------------------------------------------------------------------------------------
     * Copy/Move constructors (disabled).
    */
    DpCluster(const DpCluster &) noexcept = delete;
    DpCluster(DpCluster &&) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpCluster()
    {
        Stop();
        // Forget about this instance
        UnchainInstance();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy/Move assignment operators (disabled).
    */
    DpCluster & operator = (const DpCluster &) noexcept = delete;
    DpCluster & operator = (DpCluster &&) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Check if the managed cluster instance is valid and throw an exception otherwise.
    */
    void Validate() const
    {
        if (!mC)
        {
            STHROWF("Discord cluster instance is not valid anymore.");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Check if the managed cluster instance is valid and throw an exception otherwise.
    */
    void Validate(const char * m) const
    {
        if (!mC)
        {
            STHROWF("Cannot {} a cluster instance that is not valid anymore.", fmt::to_string_view(m));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Check if the managed cluster instance is valid and throw an exception otherwise.
    */
    dpp::cluster & Valid() const { Validate(); return *mC; }

    /* --------------------------------------------------------------------------------------------
     * Check if the managed cluster instance is valid and throw an exception otherwise.
    */
    dpp::cluster & Valid(const char * m) const { Validate(m); return *mC; }

    /* --------------------------------------------------------------------------------------------
     * Process the cluster. This is used internally on each server frame.
    */
    void Process(bool force = false);

    /* --------------------------------------------------------------------------------------------
     * Terminate the cluster. This is used internally when the VM is shutting down.
    */
    void Terminate();

    /* --------------------------------------------------------------------------------------------
     * Start the cluster.
    */
    DpCluster & Start() { Valid("start").start(dpp::st_return); return *this; }

    /* --------------------------------------------------------------------------------------------
     * Stop the cluster.
    */
    void Stop() { Valid("stop").shutdown(); }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the events table of this cluster.
    */
    SQMOD_NODISCARD LightObj & GetEvents()
    {
        return mSqEvents;
    }

    /* --------------------------------------------------------------------------------------------
     * Enable a certain event for the cluster.
    */
    DpCluster & EnableEvent(SQInteger id);

    /* --------------------------------------------------------------------------------------------
     * Disable a certain event for the cluster.
    */
    DpCluster & DisableEvent(SQInteger id);

private:

    /* --------------------------------------------------------------------------------------------
     * Signal initialization.
    */
    void InitEvents()
    {
        // Ignore the call if already initialized
        if (!mSqEvents.IsNull())
        {
            return;
        }
        // Create a new table on the stack with enough space pre-allocated
        sq_newtableex(SqVM(), static_cast< SQInteger >(DpEventID::Max + 1));
        // Grab the table object from the stack
        mSqEvents = LightObj(-1, SqVM());
        // Pop the table object from the stack
        sq_pop(SqVM(), 1);
        // Proceed to initializing the events
        for (size_t i = 0; i < mEvents.size(); ++i)
        {
            InitSignalPair(mEvents[i], mSqEvents, DpEventID::NAME[i]);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Signal termination.
    */
    void DropEvents()
    {
        for (auto & e : mEvents)
        {
            ResetSignalPair(e);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Event handlers.
    */
    void OnVoiceStateUpdate(const dpp::voice_state_update_t & ev);
    void OnVoiceClientDisconnect(const dpp::voice_client_disconnect_t & ev);
    void OnVoiceClientSpeaking(const dpp::voice_client_speaking_t & ev);
    void OnLog(const dpp::log_t & ev);
    void OnGuildJoinRequestDelete(const dpp::guild_join_request_delete_t & ev);
    void OnInteractionCreate(const dpp::interaction_create_t & ev);
    void OnSlashCommand(const dpp::slashcommand_t & ev);
    void OnButtonClick(const dpp::button_click_t & ev);
    void OnAutoComplete(const dpp::autocomplete_t & ev);
    void OnSelectClick(const dpp::select_click_t & ev);
    void OnMessageContextMenu(const dpp::message_context_menu_t & ev);
    void OnUserContextMenu(const dpp::user_context_menu_t & ev);
    void OnFormSubmit(const dpp::form_submit_t & ev);
    void OnGuildDelete(const dpp::guild_delete_t & ev);
    void OnChannelDelete(const dpp::channel_delete_t & ev);
    void OnChannelUpdate(const dpp::channel_update_t & ev);
    void OnReady(const dpp::ready_t & ev);
    void OnMessageDelete(const dpp::message_delete_t & ev);
    void OnGuildMemberRemove(const dpp::guild_member_remove_t & ev);
    void OnResumed(const dpp::resumed_t & ev);
    void OnGuildRoleCreate(const dpp::guild_role_create_t & ev);
    void OnTypingStart(const dpp::typing_start_t & ev);
    void OnMessageReactionAdd(const dpp::message_reaction_add_t & ev);
    void OnGuildMembersChunk(const dpp::guild_members_chunk_t & ev);
    void OnMessageReactionRemove(const dpp::message_reaction_remove_t & ev);
    void OnGuildCreate(const dpp::guild_create_t & ev);
    void OnChannelCreate(const dpp::channel_create_t & ev);
    void OnMessageReactionRemoveEmoji(const dpp::message_reaction_remove_emoji_t & ev);
    void OnMessageDeleteDulk(const dpp::message_delete_bulk_t & ev);
    void OnGuildRoleUpdate(const dpp::guild_role_update_t & ev);
    void OnGuildRoleDelete(const dpp::guild_role_delete_t & ev);
    void OnChannelPinsUpdate(const dpp::channel_pins_update_t & ev);
    void OnMessageReactionRemoveAll(const dpp::message_reaction_remove_all_t & ev);
    void OnVoiceServerUpdate(const dpp::voice_server_update_t & ev);
    void OnGuildEmojisUpdate(const dpp::guild_emojis_update_t & ev);
    void OnGuildStickersUpdate(const dpp::guild_stickers_update_t & ev);
    void OnPresenceUpdate(const dpp::presence_update_t & ev);
    void OnWebhooksUpdate(const dpp::webhooks_update_t & ev);
    void OnAutomodRuleCreate(const dpp::automod_rule_create_t & ev);
    void OnAutomodRuleUpdate(const dpp::automod_rule_update_t & ev);
    void OnAutomodRuleDelete(const dpp::automod_rule_delete_t & ev);
    void OnAutomodRuleExecute(const dpp::automod_rule_execute_t & ev);
    void OnGuildMemberAdd(const dpp::guild_member_add_t & ev);
    void OnInviteDelete(const dpp::invite_delete_t & ev);
    void OnGuildUpdate(const dpp::guild_update_t & ev);
    void OnGuildIntegrationsUpdate(const dpp::guild_integrations_update_t & ev);
    void OnGuildMemberUpdate(const dpp::guild_member_update_t & ev);
    void OnInviteCreate(const dpp::invite_create_t & ev);
    void OnMessageUpdate(const dpp::message_update_t & ev);
    void OnUserUpdate(const dpp::user_update_t & ev);
    void OnMessageCreate(const dpp::message_create_t & ev);
    void OnGuildBanAdd(const dpp::guild_ban_add_t & ev);
    void OnGuildBanRemove(const dpp::guild_ban_remove_t & ev);
    void OnIntegrationCreate(const dpp::integration_create_t & ev);
    void OnIntegrationUpdate(const dpp::integration_update_t & ev);
    void OnIntegrationDelete(const dpp::integration_delete_t & ev);
    void OnThreadCreate(const dpp::thread_create_t & ev);
    void OnThreadUpdate(const dpp::thread_update_t & ev);
    void OnThreadDelete(const dpp::thread_delete_t & ev);
    void OnThreadListSync(const dpp::thread_list_sync_t & ev);
    void OnThreadMemberUpdate(const dpp::thread_member_update_t & ev);
    void OnThreadMembersUpdate(const dpp::thread_members_update_t & ev);
    void OnGuildScheduledEventCreate(const dpp::guild_scheduled_event_create_t & ev);
    void OnGuildScheduledEventUpdate(const dpp::guild_scheduled_event_update_t & ev);
    void OnGuildScheduledEventDelete(const dpp::guild_scheduled_event_delete_t & ev);
    void OnGuildScheduledEventUserAdd(const dpp::guild_scheduled_event_user_add_t & ev);
    void OnGuildScheduledEventUserRemove(const dpp::guild_scheduled_event_user_remove_t & ev);
    void OnVoiceBufferSend(const dpp::voice_buffer_send_t & ev);
    void OnVoiceUserTalking(const dpp::voice_user_talking_t & ev);
    void OnVoiceReady(const dpp::voice_ready_t & ev);
    void OnVoiceReceive(const dpp::voice_receive_t & ev);
    void OnVoiceReceiveCombined(const dpp::voice_receive_t & ev);
    void OnVoiceTrackMarker(const dpp::voice_track_marker_t & ev);
    void OnStageInstanceCreate(const dpp::stage_instance_create_t & ev);
    void OnStageInstanceUpdate(const dpp::stage_instance_update_t & ev);
    void OnStageInstanceDelete(const dpp::stage_instance_delete_t & ev);
};

} // Namespace:: SqMod
