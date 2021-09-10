#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/DPPTy.hpp"

// ------------------------------------------------------------------------------------------------
#include <chrono>
#include <functional>

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
     * String identification.
    */
    static const std::array< const char *, static_cast< size_t >(Max) > NAME;
};

/* ------------------------------------------------------------------------------------------------
 * Base class of an event handler.
*/
struct DpEventBase
{
    /* --------------------------------------------------------------------------------------------
     * Raw event text.
    */
    std::string mRaw{};

    /* --------------------------------------------------------------------------------------------
     * Shard the event came from.
    */
    dpp::discord_client * mFrom{nullptr}; 

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpEventBase() noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpEventBase(const dpp::event_dispatch_t & d) noexcept
        : mRaw(d.raw_event), mFrom(d.from)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpEventBase(const DpEventBase &) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    DpEventBase(DpEventBase &&) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    virtual ~DpEventBase() noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpEventBase & operator = (const DpEventBase &) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    DpEventBase & operator = (DpEventBase &&) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    virtual void Cleanup()
    {
        mFrom = nullptr;
    }
};

/* ------------------------------------------------------------------------------------------------
 *  Voice state update event.
*/
struct DpVoiceStateUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------
    dpp::voicestate mState{};

    // --------------------------------------------------------------------------------------------
    LightObj        mSqState{};

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceStateUpdateEvent(const dpp::voice_state_update_t & d) noexcept
        : DpEventBase(d), mState(d.state), mSqState()
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [Ready] event handle"); }

    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpVoiceStateUpdateEvent & Valid() { Validate(); return *this; }

    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpVoiceStateUpdateEvent & Valid() const { Validate(); return *this; }

    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        if (!mSqState.IsNull())
        {
            [[maybe_unused]] auto p = mSqState.CastI< DpVoiceState >()->mPtr.release();
            // Release script resources
            mSqState.Release();
        }
        // Allow the base to cleanup as well
        DpEventBase::Cleanup();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return mRaw; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the voice state.
    */
    SQMOD_NODISCARD LightObj & GetState()
    {
        if (Valid().mSqState.IsNull())
        {
            mSqState = LightObj{SqTypeIdentity< DpVoiceState >{}, SqVM(), &mState, false};
        }
        // Return the associated script object
        return mSqState;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Guild join request delete (user declined membership screening) event.
*/
struct DpGuildJoinRequestDeleteEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------
    dpp::snowflake mGuildID{};
    dpp::snowflake mUserID{};

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildJoinRequestDeleteEvent(const dpp::guild_join_request_delete_t & d) noexcept
        : DpEventBase(d), mGuildID(d.guild_id), mUserID(d.user_id)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [GuildJoinRequestDelete] event handle"); }

    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpGuildJoinRequestDeleteEvent & Valid() { Validate(); return *this; }

    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpGuildJoinRequestDeleteEvent & Valid() const { Validate(); return *this; }

    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        DpEventBase::Cleanup();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Valid().mRaw; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild id.
    */
    SQMOD_NODISCARD dpp::snowflake GetGuildID() const { return Valid().mGuildID; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the user id.
    */
    SQMOD_NODISCARD dpp::snowflake GetUserID() const { return Valid().mUserID; }
};

/* ------------------------------------------------------------------------------------------------
 * Log message event.
*/
struct DpLogEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------
    SQInteger   mSeverity{0};
    std::string mMessage{};

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpLogEvent(const dpp::log_t & d) noexcept
        : DpEventBase(d), mSeverity(d.severity), mMessage(d.message)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        DpEventBase::Cleanup();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return mRaw; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve log severity.
    */
    SQMOD_NODISCARD SQInteger GetSeverity() const { return mSeverity; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve log message.
    */
    SQMOD_NODISCARD const std::string & GetMessage() const { return mMessage; }
};

/* ------------------------------------------------------------------------------------------------
 * Session ready event.
*/
struct DpReadyEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------
    std::string mSessionID{};
    uint32_t    mShardID{};

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpReadyEvent(const dpp::ready_t & d) noexcept
        : DpEventBase(d)
        , mSessionID(d.session_id)
        , mShardID(d.shard_id)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [Ready] event handle"); }

    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpReadyEvent & Valid() { Validate(); return *this; }

    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpReadyEvent & Valid() const { Validate(); return *this; }

    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        DpEventBase::Cleanup();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Valid().mRaw; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the session id.
    */
    SQMOD_NODISCARD const std::string & GetSessionID() const { return Valid().mSessionID; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the shard id.
    */
    SQMOD_NODISCARD SQInteger GetShardID() const { return static_cast< SQInteger >(Valid().mShardID); }
};

} // Namespace:: SqMod
