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
    { }
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
    { }
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
    { }
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
 * Create interaction event.
*/
struct DpInteractionCreateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpInteractionCreateEvent(const dpp::interaction_create_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [InteractionCreate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpInteractionCreateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpInteractionCreateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Click on button event.
*/
struct DpButtonClickEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpButtonClickEvent(const dpp::button_click_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [ButtonClick] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpButtonClickEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpButtonClickEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 *  event.
*/
struct DpSelectClickEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpSelectClickEvent(const dpp::select_click_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [SelectClick] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpSelectClickEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpSelectClickEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Delete guild event.
*/
struct DpGuildDeleteEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildDeleteEvent(const dpp::guild_delete_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [GuildDelete] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpGuildDeleteEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpGuildDeleteEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Delete channel event.
*/
struct DpChannelDeleteEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpChannelDeleteEvent(const dpp::channel_delete_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [ChannelDelete] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpChannelDeleteEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpChannelDeleteEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Update channel event.
*/
struct DpChannelUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpChannelUpdateEvent(const dpp::channel_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [ChannelUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpChannelUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpChannelUpdateEvent & Valid() const { Validate(); return *this; }
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
        : DpEventBase(d), mSessionID(d.session_id), mShardID(d.shard_id)
    { }
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

/* ------------------------------------------------------------------------------------------------
 * Message deleted event.
*/
struct DpMessageDeleteEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageDeleteEvent(const dpp::message_delete_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [MessageDelete] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpMessageDeleteEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpMessageDeleteEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Delete application slash command event.
*/
struct DpApplicationCommandDeleteEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpApplicationCommandDeleteEvent(const dpp::application_command_delete_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [ApplicationCommandDelete] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpApplicationCommandDeleteEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpApplicationCommandDeleteEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Guild member remove event.
*/
struct DpGuildMemberRemoveEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildMemberRemoveEvent(const dpp::guild_member_remove_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [GuildMemberRemove] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpGuildMemberRemoveEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpGuildMemberRemoveEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Create application slash command event.
*/
struct DpApplicationCommandCreateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpApplicationCommandCreateEvent(const dpp::application_command_create_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [ApplicationCommandCreate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpApplicationCommandCreateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpApplicationCommandCreateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Session resumed event.
*/
struct DpResumedEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpResumedEvent(const dpp::resumed_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [Resumed] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpResumedEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpResumedEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Guild role create event.
*/
struct DpGuildRoleCreateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildRoleCreateEvent(const dpp::guild_role_create_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [GuildRoleCreate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpGuildRoleCreateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpGuildRoleCreateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Typing start event.
*/
struct DpTypingStartEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpTypingStartEvent(const dpp::typing_start_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [TypingStart] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpTypingStartEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpTypingStartEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Message reaction add event.
*/
struct DpMessageReactionAddEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageReactionAddEvent(const dpp::message_reaction_add_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [MessageReactionAdd] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpMessageReactionAddEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpMessageReactionAddEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Guild members chunk event.
*/
struct DpGuildMembersChunkEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildMembersChunkEvent(const dpp::guild_members_chunk_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [GuildMembersChunk] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpGuildMembersChunkEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpGuildMembersChunkEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Message reaction remove event.
*/
struct DpMessageReactionRemoveEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageReactionRemoveEvent(const dpp::message_reaction_remove_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [MessageReactionRemove] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpMessageReactionRemoveEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpMessageReactionRemoveEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Create guild event.
*/
struct DpGuildCreateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildCreateEvent(const dpp::guild_create_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [GuildCreate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpGuildCreateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpGuildCreateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Create channel event.
*/
struct DpChannelCreateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpChannelCreateEvent(const dpp::channel_create_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [ChannelCreate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpChannelCreateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpChannelCreateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Message remove emoji event.
*/
struct DpMessageReactionRemoveEmojiEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageReactionRemoveEmojiEvent(const dpp::message_reaction_remove_emoji_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [MessageReactionRemoveEmoji] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpMessageReactionRemoveEmojiEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpMessageReactionRemoveEmojiEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Message delete bulk event.
*/
struct DpMessageDeleteBulkEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageDeleteBulkEvent(const dpp::message_delete_bulk_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [MessageDeleteBulk] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpMessageDeleteBulkEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpMessageDeleteBulkEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Guild role update event.
*/
struct DpGuildRoleUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildRoleUpdateEvent(const dpp::guild_role_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [GuildRoleUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpGuildRoleUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpGuildRoleUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Guild role delete event.
*/
struct DpGuildRoleDeleteEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildRoleDeleteEvent(const dpp::guild_role_delete_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [GuildRoleDelete] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpGuildRoleDeleteEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpGuildRoleDeleteEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Channel pins update event.
*/
struct DpChannelPinsUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpChannelPinsUpdateEvent(const dpp::channel_pins_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [ChannelPinsUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpChannelPinsUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpChannelPinsUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Message remove all reactions event.
*/
struct DpMessageReactionRemoveAllEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageReactionRemoveAllEvent(const dpp::message_reaction_remove_all_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [MessageReactionRemoveAll] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpMessageReactionRemoveAllEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpMessageReactionRemoveAllEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Voice server update event.
*/
struct DpVoiceServerUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceServerUpdateEvent(const dpp::voice_server_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [VoiceServerUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpVoiceServerUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpVoiceServerUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Guild emojis update event.
*/
struct DpGuildEmojisUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildEmojisUpdateEvent(const dpp::guild_emojis_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [GuildEmojisUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpGuildEmojisUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpGuildEmojisUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Update guild stickers event.
*/
struct DpGuildStickersUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildStickersUpdateEvent(const dpp::guild_stickers_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [GuildStickersUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpGuildStickersUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpGuildStickersUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Presence update event.
*/
struct DpPresenceUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpPresenceUpdateEvent(const dpp::presence_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [PresenceUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpPresenceUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpPresenceUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Webhooks update event.
*/
struct DpWebhooksUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpWebhooksUpdateEvent(const dpp::webhooks_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [WebhooksUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpWebhooksUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpWebhooksUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Guild member add event.
*/
struct DpGuildMemberAddEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildMemberAddEvent(const dpp::guild_member_add_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [GuildMemberAdd] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpGuildMemberAddEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpGuildMemberAddEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Invite delete event.
*/
struct DpInviteDeleteEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpInviteDeleteEvent(const dpp::invite_delete_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [InviteDelete] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpInviteDeleteEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpInviteDeleteEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Guild update event.
*/
struct DpGuildUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildUpdateEvent(const dpp::guild_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [GuildUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpGuildUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpGuildUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Guild integrations update event.
*/
struct DpGuildIntegrationsUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildIntegrationsUpdateEvent(const dpp::guild_integrations_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [GuildIntegrationsUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpGuildIntegrationsUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpGuildIntegrationsUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Guild member update event.
*/
struct DpGuildMemberUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildMemberUpdateEvent(const dpp::guild_member_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [GuildMemberUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpGuildMemberUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpGuildMemberUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Update application slash command event.
*/
struct DpApplicationCommandUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpApplicationCommandUpdateEvent(const dpp::application_command_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [ApplicationCommandUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpApplicationCommandUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpApplicationCommandUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Invite create event.
*/
struct DpInviteCreateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpInviteCreateEvent(const dpp::invite_create_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [InviteCreate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpInviteCreateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpInviteCreateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Message update event.
*/
struct DpMessageUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageUpdateEvent(const dpp::message_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [MessageUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpMessageUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpMessageUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * User update event.
*/
struct DpUserUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpUserUpdateEvent(const dpp::user_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [UserUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpUserUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpUserUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Create message event.
*/
struct DpMessageCreateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageCreateEvent(const dpp::message_create_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [MessageCreate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpMessageCreateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpMessageCreateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Guild ban add event.
*/
struct DpGuildBanAddEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildBanAddEvent(const dpp::guild_ban_add_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [GuildBanAdd] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpGuildBanAddEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpGuildBanAddEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Guild ban remove event.
*/
struct DpGuildBanRemoveEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildBanRemoveEvent(const dpp::guild_ban_remove_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [GuildBanRemove] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpGuildBanRemoveEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpGuildBanRemoveEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Integration create event.
*/
struct DpIntegrationCreateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpIntegrationCreateEvent(const dpp::integration_create_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [IntegrationCreate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpIntegrationCreateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpIntegrationCreateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Integration update event.
*/
struct DpIntegrationUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpIntegrationUpdateEvent(const dpp::integration_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [IntegrationUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpIntegrationUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpIntegrationUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Integration delete event.
*/
struct DpIntegrationDeleteEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpIntegrationDeleteEvent(const dpp::integration_delete_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [IntegrationDelete] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpIntegrationDeleteEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpIntegrationDeleteEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Thread create event.
*/
struct DpThreadCreateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpThreadCreateEvent(const dpp::thread_create_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [ThreadCreate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpThreadCreateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpThreadCreateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Thread update event.
*/
struct DpThreadUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpThreadUpdateEvent(const dpp::thread_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [ThreadUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpThreadUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpThreadUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Thread delete event.
*/
struct DpThreadDeleteEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpThreadDeleteEvent(const dpp::thread_delete_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [ThreadDelete] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpThreadDeleteEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpThreadDeleteEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Thread list sync event.
*/
struct DpThreadListSyncEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpThreadListSyncEvent(const dpp::thread_list_sync_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [ThreadListSync] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpThreadListSyncEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpThreadListSyncEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Thread member update event.
*/
struct DpThreadMemberUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpThreadMemberUpdateEvent(const dpp::thread_member_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [ThreadMemberUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpThreadMemberUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpThreadMemberUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Thread members update event.
*/
struct DpThreadMembersUpdateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpThreadMembersUpdateEvent(const dpp::thread_members_update_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [ThreadMembersUpdate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpThreadMembersUpdateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpThreadMembersUpdateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Voice buffer send event.
*/
struct DpVoiceBufferSendEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceBufferSendEvent(const dpp::voice_buffer_send_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [VoiceBufferSend] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpVoiceBufferSendEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpVoiceBufferSendEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Voice user talking event.
*/
struct DpVoiceUserTalkingEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceUserTalkingEvent(const dpp::voice_user_talking_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [VoiceUserTalking] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpVoiceUserTalkingEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpVoiceUserTalkingEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Voice user talking event.
*/
struct DpVoiceReadyEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceReadyEvent(const dpp::voice_ready_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [VoiceReady] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpVoiceReadyEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpVoiceReadyEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Voice receive packet event.
*/
struct DpVoiceReceiveEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceReceiveEvent(const dpp::voice_receive_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [VoiceReceive] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpVoiceReceiveEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpVoiceReceiveEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Voice state update event.
*/
struct DpVoiceTrackMarkerEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceTrackMarkerEvent(const dpp::voice_track_marker_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [VoiceTrackMarker] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpVoiceTrackMarkerEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpVoiceTrackMarkerEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Create stage instance event.
*/
struct DpStageInstanceCreateEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpStageInstanceCreateEvent(const dpp::stage_instance_create_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [StageInstanceCreate] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpStageInstanceCreateEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpStageInstanceCreateEvent & Valid() const { Validate(); return *this; }
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

};

/* ------------------------------------------------------------------------------------------------
 * Delete stage instance event.
*/
struct DpStageInstanceDeleteEvent : public DpEventBase
{
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpStageInstanceDeleteEvent(const dpp::stage_instance_delete_t & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mFrom) STHROWF("Invalid discord [StageInstanceDelete] event handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpStageInstanceDeleteEvent & Valid() { Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpStageInstanceDeleteEvent & Valid() const { Validate(); return *this; }
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

};

} // Namespace:: SqMod
