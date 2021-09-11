#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/DPPEv.hpp"
#include "Library/DPPTy.hpp"
#include "Core/Signal.hpp"

// ------------------------------------------------------------------------------------------------
#include <chrono>
#include <memory>
#include <functional>

// ------------------------------------------------------------------------------------------------
#include <concurrentqueue.h>
#include <dpp/dpp.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * WebSocket frame.
*/
struct DpInternalEvent
{
    /* --------------------------------------------------------------------------------------------
     * Event data.
    */
    uint64_t mData{0llu};

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpInternalEvent() noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    DpInternalEvent(uint64_t type, void * data) noexcept
        : mData((type << 56u) | reinterpret_cast< uint64_t >(data))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpInternalEvent(const DpInternalEvent & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpInternalEvent(DpInternalEvent && o) noexcept
        : mData(o.mData)
    {
        o.mData = 0llu; // Take ownership
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpInternalEvent()
    {
        Release();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpInternalEvent & operator = (const DpInternalEvent & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpInternalEvent & operator = (DpInternalEvent && o) noexcept
    {
        if (mData != o.mData)
        {
            // Release current information
            Release();
            // Replicate members
            mData = o.mData;
            // Take ownership
            o.mData = 0llu;
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Forget about the managed event data.
    */
    void Reset() noexcept
    {
        mData = 0llu;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the event type.
    */
    SQMOD_NODISCARD uint8_t GetType() const noexcept
    {
        return static_cast< uint8_t >((mData >> 56u) & 0xFFllu);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the event data.
    */
    SQMOD_NODISCARD uintptr_t GetData() const noexcept
    {
        return static_cast< uintptr_t >((~(0xFFllu << 56u)) & mData);
    }

    /* --------------------------------------------------------------------------------------------
     * Release associated event data, if any.
    */
    void Release();
};

/* ------------------------------------------------------------------------------------------------
 * The cluster class represents a group of shards and a command queue for sending and receiving
 * commands from discord via HTTP.
*/
struct DpCluster : public SqChainedInstances< DpCluster >
{
    /* --------------------------------------------------------------------------------------------
     * Queue of events generated from other threads.
    */
    using EventQueue = moodycamel::ConcurrentQueue< DpInternalEvent >;

    /* --------------------------------------------------------------------------------------------
     * Managed cluster instance.
    */
    std::unique_ptr< dpp::cluster > mC{nullptr};

    /* --------------------------------------------------------------------------------------------
     * Event queue.
    */
    EventQueue mQueue{4096};

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpCluster(StackStrF & token)
        : mC(std::make_unique< dpp::cluster >(token.ToStr()))
        , mQueue(4096)
    {
        Initialize();
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    DpCluster(StackStrF & token, SQInteger intents)
        : mC(std::make_unique< dpp::cluster >(token.ToStr(), static_cast< uint32_t >(intents)))
        , mQueue(4096)
    {
        Initialize();
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    DpCluster(StackStrF & token, SQInteger intents, SQInteger shards)
        : mC(std::make_unique< dpp::cluster >(token.ToStr(), static_cast< uint32_t >(intents), static_cast< uint32_t >(shards)))
        , mQueue(4096)
    {
        Initialize();
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    DpCluster(StackStrF & token, SQInteger intents, SQInteger shards, SQInteger cluster_id)
        : mC(std::make_unique< dpp::cluster >(token.ToStr(), static_cast< uint32_t >(intents), static_cast< uint32_t >(shards), static_cast< uint32_t >(cluster_id)))
        , mQueue(4096)
    {
        Initialize();
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    DpCluster(StackStrF & token, SQInteger intents, SQInteger shards, SQInteger cluster_id, SQInteger max_clusters)
        : mC(std::make_unique< dpp::cluster >(token.ToStr(), static_cast< uint32_t >(intents), static_cast< uint32_t >(shards), static_cast< uint32_t >(cluster_id), static_cast< uint32_t >(max_clusters)))
        , mQueue(4096)
    {
        Initialize();
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    DpCluster(StackStrF & token, SQInteger intents, SQInteger shards, SQInteger cluster_id, SQInteger max_clusters, bool compressed)
        : mC(std::make_unique< dpp::cluster >(token.ToStr(), static_cast< uint32_t >(intents), static_cast< uint32_t >(shards), static_cast< uint32_t >(cluster_id), static_cast< uint32_t >(max_clusters), compressed))
        , mQueue(4096)
    {
        Initialize();
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    DpCluster(StackStrF & token, SQInteger intents, SQInteger shards, SQInteger cluster_id, SQInteger max_clusters, bool compressed, const DpCachePolicy & cp)
        : mC(std::make_unique< dpp::cluster >(token.ToStr(), static_cast< uint32_t >(intents), static_cast< uint32_t >(shards), static_cast< uint32_t >(cluster_id), static_cast< uint32_t >(max_clusters), compressed, cp.ToNative()))
        , mQueue(4096)
    {
        Initialize();
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpCluster()
    {
        DropEvents();
        // Forget about this instance
        UnchainInstance();
    }

    /* --------------------------------------------------------------------------------------------
     * Start the cluster, connecting all its shards. Returns once all shards are connected.
    */
    DpCluster & Start()
    {
        LogInf("Before start...");
        mC->start(true);
        LogInf("After start...");
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Log a message to whatever log the user is using.
    */
    DpCluster & Log(SQInteger severity, StackStrF & message)
    {
        mC->log(static_cast< dpp::loglevel >(severity), message.ToStr());
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Get the dm channel for a user id.
    */
    SQMOD_NODISCARD dpp::snowflake GetDmChannel(dpp::snowflake user_id) const
    {
        return mC->get_dm_channel(static_cast< dpp::snowflake >(user_id));
    }

    /* --------------------------------------------------------------------------------------------
     * Set the dm channel id for a user id.
    */
    DpCluster & SetDmChannel(dpp::snowflake user_id, dpp::snowflake channel_id)
    {
        mC->set_dm_channel(user_id, channel_id);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the uptime of the cluster.
    */
    SQMOD_NODISCARD dpp::utility::uptime UpTime() const
    {
        return mC->uptime();
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the uptime of the cluster.
    */
    DpCluster & SetPresence(const DpPresence & p)
    {
        mC->set_presence(p.Valid());
        return *this;
    }

    // --------------------------------------------------------------------------------------------
    LightObj mSqEvents{}; // Table containing the emitted cluster events.

    /* --------------------------------------------------------------------------------------------
     * Retrieve the events table of this cluster.
    */
    SQMOD_NODISCARD LightObj & GetEvents()
    {
        return mSqEvents;
    }

    /* --------------------------------------------------------------------------------------------
     * Cluster signals.
    */
    std::array< SignalPair, static_cast< size_t >(DpEventID::Max) > mEvents{};

    /* --------------------------------------------------------------------------------------------
     * Process the cluster.
    */
    void Process(bool force = false);

    /* --------------------------------------------------------------------------------------------
     * Terminate the cluster.
    */
    void Terminate()
    {
        // Delete the cluster instance
        mC.reset();
        // Release associated script objects
        mSqEvents.Release();
        // Release event signal objects
        DropEvents();
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
     * Initialize the cluster.
    */
    void Initialize()
    {
        InitEvents();
        // Remember this instance
        ChainInstance();
    }

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
        // Create a new table on the stack
        sq_newtableex(SqVM(), 64);
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
    void OnLog(const dpp::log_t & ev);
    void OnGuildJoinRequestDelete(const dpp::guild_join_request_delete_t & ev);
    void OnInteractionCreate(const dpp::interaction_create_t & ev);
    void OnButtonClick(const dpp::button_click_t & ev);
    void OnSelectClick(const dpp::select_click_t & ev);
    void OnGuildDelete(const dpp::guild_delete_t & ev);
    void OnChannelDelete(const dpp::channel_delete_t & ev);
    void OnChannelUpdate(const dpp::channel_update_t & ev);
    void OnReady(const dpp::ready_t & ev);
    void OnMessageDelete(const dpp::message_delete_t & ev);
    void OnApplicationCommandDelete(const dpp::application_command_delete_t & ev);
    void OnGuildMemberRemove(const dpp::guild_member_remove_t & ev);
    void OnApplicationCommandCreate(const dpp::application_command_create_t & ev);
    void OnResumed(const dpp::resumed_t & ev);
    void OnGuildRoleCreate(const dpp::guild_role_create_t & ev);
    void OnTypingStart(const dpp::typing_start_t & ev);
    void OnMessageReactionAdd(const dpp::message_reaction_add_t & ev);
    void OnGuildMembersChunk(const dpp::guild_members_chunk_t & ev);
    void OnMessageReactionRemove(const dpp::message_reaction_remove_t & ev);
    void OnGuildCreate(const dpp::guild_create_t & ev);
    void OnChannelCreate(const dpp::channel_create_t & ev);
    void OnMessageReactionRemoveEmoji(const dpp::message_reaction_remove_emoji_t & ev);
    void OnMessageDeleteBulk(const dpp::message_delete_bulk_t & ev);
    void OnGuildRoleUpdate(const dpp::guild_role_update_t & ev);
    void OnGuildRoleDelete(const dpp::guild_role_delete_t & ev);
    void OnChannelPinsUpdate(const dpp::channel_pins_update_t & ev);
    void OnMessageReactionRemoveAll(const dpp::message_reaction_remove_all_t & ev);
    void OnVoiceServerUpdate(const dpp::voice_server_update_t & ev);
    void OnGuildEmojisUpdate(const dpp::guild_emojis_update_t & ev);
    void OnGuildStickersUpdate(const dpp::guild_stickers_update_t & ev);
    void OnPresenceUpdate(const dpp::presence_update_t & ev);
    void OnWebhooksUpdate(const dpp::webhooks_update_t & ev);
    void OnGuildMemberAdd(const dpp::guild_member_add_t & ev);
    void OnInviteDelete(const dpp::invite_delete_t & ev);
    void OnGuildUpdate(const dpp::guild_update_t & ev);
    void OnGuildIntegrationsUpdate(const dpp::guild_integrations_update_t & ev);
    void OnGuildMemberUpdate(const dpp::guild_member_update_t & ev);
    void OnApplicationCommandUpdate(const dpp::application_command_update_t & ev);
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
    void OnVoiceBufferSend(const dpp::voice_buffer_send_t & ev);
    void OnVoiceUserTalking(const dpp::voice_user_talking_t & ev);
    void OnVoiceReady(const dpp::voice_ready_t & ev);
    void OnVoiceReceive(const dpp::voice_receive_t & ev);
    void OnVoiceTrackMarker(const dpp::voice_track_marker_t & ev);
    void OnStageInstanceCreate(const dpp::stage_instance_create_t & ev);
    void OnStageInstanceDelete(const dpp::stage_instance_delete_t & ev);
};

} // Namespace:: SqMod
