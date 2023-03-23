#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/Discord/Constants.hpp"
#include "Library/Discord/Channel.hpp"
#include "Library/Discord/Client.hpp"
#include "Library/Discord/Command.hpp"
#include "Library/Discord/Guild.hpp"
#include "Library/Discord/Integration.hpp"
#include "Library/Discord/Message.hpp"
#include "Library/Discord/Misc.hpp"
#include "Library/Discord/Role.hpp"
#include "Library/Discord/User.hpp"

// ------------------------------------------------------------------------------------------------
#include <chrono>
#include <functional>

// ------------------------------------------------------------------------------------------------
#include <dpp/dpp.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

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
    virtual ~DpEventBase() { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpEventBase & operator = (const DpEventBase &) noexcept = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    DpEventBase & operator = (DpEventBase &&) noexcept = delete;
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated event ID.
    */
    SQMOD_NODISCARD virtual int GetEventID() const = 0;
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated event name.
    */
    SQMOD_NODISCARD virtual std::string_view GetEventName() const = 0;
    /* --------------------------------------------------------------------------------------------
     * Transform the script object itself to a script object.
    */
    SQMOD_NODISCARD virtual LightObj ToScriptObject() = 0;
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed. Release associated resources.
    */
    virtual void Cleanup()
    {
        mFrom = nullptr;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Middleman between event implementation and event base where common functionality that depends
 * on the implementation type information can be added once.
*/
template < class T > struct DpEvent : public DpEventBase
{
    using Info = T;
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpEvent() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpEvent(const typename T::Type & d) noexcept
        : DpEventBase(d)
    { }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const
    {
        if (!DpEvent::mFrom)
        {
            const auto & name = T::Name;
            STHROWF("Invalid discord [{}] event handle", name);
        }
    }
    // --------------------------------------------------------------------------------------------
    using DpEventBase::Cleanup;
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated event ID.
    */
    SQMOD_NODISCARD int GetEventID() const noexcept override
    {
        return T::ID;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated event name.
    */
    SQMOD_NODISCARD std::string_view GetEventName() const noexcept override
    {
        return T::Name;
    }
};

/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpVoiceStateUpdateEvent : public DpEvent< DpVoiceStateUpdateEventInfo >
{
    using Type = dpp::voice_state_update_t;
    using Base = DpEvent< DpVoiceStateUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------
    dpp::voicestate mState{};
    // --------------------------------------------------------------------------------------------
    LightObj        mSqState{};
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceStateUpdateEvent(const Type & d) noexcept
        : Base(d), mState(d.state), mSqState()
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a reference to it.
    */
    SQMOD_NODISCARD DpVoiceStateUpdateEvent & Valid() { Base::Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD const DpVoiceStateUpdateEvent & Valid() const { Base::Validate(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        if (!mSqState.IsNull())
        {
            mSqState.CastI< DpVoiceState >()->Cleanup();
            // Release script resources
            mSqState.Release();
        }
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the voice state.
    */
    SQMOD_NODISCARD LightObj & GetState()
    {
        // Is the script object already cached?
        if (Valid().mSqState.IsNull())
        {
            mSqState = LightObj{SqTypeIdentity< DpVoiceState >{}, SqVM(), &mState, false};
        }
        // Return the associated script object
        return mSqState;
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpVoiceClientDisconnectEvent : public DpEvent< DpVoiceClientDisconnectEventInfo >
{
    using Type = dpp::voice_client_disconnect_t;
    using Base = DpEvent< DpVoiceClientDisconnectEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceClientDisconnectEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpVoiceClientSpeakingEvent : public DpEvent< DpVoiceClientSpeakingEventInfo >
{
    using Type = dpp::voice_client_speaking_t;
    using Base = DpEvent< DpVoiceClientSpeakingEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceClientSpeakingEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpLogEvent : public DpEvent< DpLogEventInfo >
{
    using Type = dpp::log_t;
    using Base = DpEvent< DpLogEventInfo >;
    // --------------------------------------------------------------------------------------------
    SQInteger   mSeverity{0};
    std::string mMessage{};
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpLogEvent(const Type & d) noexcept
        : Base(d), mSeverity(d.severity), mMessage(d.message)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
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
 *
*/
struct DpGuildJoinRequestDeleteEvent : public DpEvent< DpGuildJoinRequestDeleteEventInfo >
{
    using Type = dpp::guild_join_request_delete_t;
    using Base = DpEvent< DpGuildJoinRequestDeleteEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildJoinRequestDeleteEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpInteractionCreateEvent : public DpEvent< DpInteractionCreateEventInfo >
{
    using Type = dpp::interaction_create_t;
    using Base = DpEvent< DpInteractionCreateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpInteractionCreateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpSlashCommandEvent : public DpEvent< DpSlashCommandEventInfo >
{
    using Type = dpp::slashcommand_t;
    using Base = DpEvent< DpSlashCommandEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpSlashCommandEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpButtonClickEvent : public DpEvent< DpButtonClickEventInfo >
{
    using Type = dpp::button_click_t;
    using Base = DpEvent< DpButtonClickEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpButtonClickEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpAutoCompleteEvent : public DpEvent< DpAutoCompleteEventInfo >
{
    using Type = dpp::autocomplete_t;
    using Base = DpEvent< DpAutoCompleteEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpAutoCompleteEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpSelectClickEvent : public DpEvent< DpSelectClickEventInfo >
{
    using Type = dpp::select_click_t;
    using Base = DpEvent< DpSelectClickEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpSelectClickEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpMessageContextMenuEvent : public DpEvent< DpMessageContextMenuEventInfo >
{
    using Type = dpp::message_context_menu_t;
    using Base = DpEvent< DpMessageContextMenuEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageContextMenuEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpUserContextMenuEvent : public DpEvent< DpUserContextMenuEventInfo >
{
    using Type = dpp::user_context_menu_t;
    using Base = DpEvent< DpUserContextMenuEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpUserContextMenuEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpFormSubmitEvent : public DpEvent< DpFormSubmitEventInfo >
{
    using Type = dpp::form_submit_t;
    using Base = DpEvent< DpFormSubmitEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpFormSubmitEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildDeleteEvent : public DpEvent< DpGuildDeleteEventInfo >
{
    using Type = dpp::guild_delete_t;
    using Base = DpEvent< DpGuildDeleteEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildDeleteEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpChannelDeleteEvent : public DpEvent< DpChannelDeleteEventInfo >
{
    using Type = dpp::channel_delete_t;
    using Base = DpEvent< DpChannelDeleteEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpChannelDeleteEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpChannelUpdateEvent : public DpEvent< DpChannelUpdateEventInfo >
{
    using Type = dpp::channel_update_t;
    using Base = DpEvent< DpChannelUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpChannelUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpReadyEvent : public DpEvent< DpReadyEventInfo >
{
    using Type = dpp::ready_t;
    using Base = DpEvent< DpReadyEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpReadyEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpMessageDeleteEvent : public DpEvent< DpMessageDeleteEventInfo >
{
    using Type = dpp::message_delete_t;
    using Base = DpEvent< DpMessageDeleteEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageDeleteEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildMemberRemoveEvent : public DpEvent< DpGuildMemberRemoveEventInfo >
{
    using Type = dpp::guild_member_remove_t;
    using Base = DpEvent< DpGuildMemberRemoveEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildMemberRemoveEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpResumedEvent : public DpEvent< DpResumedEventInfo >
{
    using Type = dpp::resumed_t;
    using Base = DpEvent< DpResumedEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpResumedEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildRoleCreateEvent : public DpEvent< DpGuildRoleCreateEventInfo >
{
    using Type = dpp::guild_role_create_t;
    using Base = DpEvent< DpGuildRoleCreateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildRoleCreateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpTypingStartEvent : public DpEvent< DpTypingStartEventInfo >
{
    using Type = dpp::typing_start_t;
    using Base = DpEvent< DpTypingStartEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpTypingStartEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpMessageReactionAddEvent : public DpEvent< DpMessageReactionAddEventInfo >
{
    using Type = dpp::message_reaction_add_t;
    using Base = DpEvent< DpMessageReactionAddEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageReactionAddEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildMembersChunkEvent : public DpEvent< DpGuildMembersChunkEventInfo >
{
    using Type = dpp::guild_members_chunk_t;
    using Base = DpEvent< DpGuildMembersChunkEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildMembersChunkEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpMessageReactionRemoveEvent : public DpEvent< DpMessageReactionRemoveEventInfo >
{
    using Type = dpp::message_reaction_remove_t;
    using Base = DpEvent< DpMessageReactionRemoveEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageReactionRemoveEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildCreateEvent : public DpEvent< DpGuildCreateEventInfo >
{
    using Type = dpp::guild_create_t;
    using Base = DpEvent< DpGuildCreateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildCreateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpChannelCreateEvent : public DpEvent< DpChannelCreateEventInfo >
{
    using Type = dpp::channel_create_t;
    using Base = DpEvent< DpChannelCreateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpChannelCreateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpMessageReactionRemoveEmojiEvent : public DpEvent< DpMessageReactionRemoveEmojiEventInfo >
{
    using Type = dpp::message_reaction_remove_emoji_t;
    using Base = DpEvent< DpMessageReactionRemoveEmojiEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageReactionRemoveEmojiEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpMessageDeleteDulkEvent : public DpEvent< DpMessageDeleteDulkEventInfo >
{
    using Type = dpp::message_delete_bulk_t;
    using Base = DpEvent< DpMessageDeleteDulkEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageDeleteDulkEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildRoleUpdateEvent : public DpEvent< DpGuildRoleUpdateEventInfo >
{
    using Type = dpp::guild_role_update_t;
    using Base = DpEvent< DpGuildRoleUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildRoleUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildRoleDeleteEvent : public DpEvent< DpGuildRoleDeleteEventInfo >
{
    using Type = dpp::guild_role_delete_t;
    using Base = DpEvent< DpGuildRoleDeleteEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildRoleDeleteEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpChannelPinsUpdateEvent : public DpEvent< DpChannelPinsUpdateEventInfo >
{
    using Type = dpp::channel_pins_update_t;
    using Base = DpEvent< DpChannelPinsUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpChannelPinsUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpMessageReactionRemoveAllEvent : public DpEvent< DpMessageReactionRemoveAllEventInfo >
{
    using Type = dpp::message_reaction_remove_all_t;
    using Base = DpEvent< DpMessageReactionRemoveAllEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageReactionRemoveAllEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpVoiceServerUpdateEvent : public DpEvent< DpVoiceServerUpdateEventInfo >
{
    using Type = dpp::voice_server_update_t;
    using Base = DpEvent< DpVoiceServerUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceServerUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildEmojisUpdateEvent : public DpEvent< DpGuildEmojisUpdateEventInfo >
{
    using Type = dpp::guild_emojis_update_t;
    using Base = DpEvent< DpGuildEmojisUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildEmojisUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildStickersUpdateEvent : public DpEvent< DpGuildStickersUpdateEventInfo >
{
    using Type = dpp::guild_stickers_update_t;
    using Base = DpEvent< DpGuildStickersUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildStickersUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpPresenceUpdateEvent : public DpEvent< DpPresenceUpdateEventInfo >
{
    using Type = dpp::presence_update_t;
    using Base = DpEvent< DpPresenceUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpPresenceUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpWebhooksUpdateEvent : public DpEvent< DpWebhooksUpdateEventInfo >
{
    using Type = dpp::webhooks_update_t;
    using Base = DpEvent< DpWebhooksUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpWebhooksUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpAutomodRuleCreateEvent : public DpEvent< DpAutomodRuleCreateEventInfo >
{
    using Type = dpp::automod_rule_create_t;
    using Base = DpEvent< DpAutomodRuleCreateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpAutomodRuleCreateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpAutomodRuleUpdateEvent : public DpEvent< DpAutomodRuleUpdateEventInfo >
{
    using Type = dpp::automod_rule_update_t;
    using Base = DpEvent< DpAutomodRuleUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpAutomodRuleUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpAutomodRuleDeleteEvent : public DpEvent< DpAutomodRuleDeleteEventInfo >
{
    using Type = dpp::automod_rule_delete_t;
    using Base = DpEvent< DpAutomodRuleDeleteEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpAutomodRuleDeleteEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpAutomodRuleExecuteEvent : public DpEvent< DpAutomodRuleExecuteEventInfo >
{
    using Type = dpp::automod_rule_execute_t;
    using Base = DpEvent< DpAutomodRuleExecuteEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpAutomodRuleExecuteEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildMemberAddEvent : public DpEvent< DpGuildMemberAddEventInfo >
{
    using Type = dpp::guild_member_add_t;
    using Base = DpEvent< DpGuildMemberAddEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildMemberAddEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpInviteDeleteEvent : public DpEvent< DpInviteDeleteEventInfo >
{
    using Type = dpp::invite_delete_t;
    using Base = DpEvent< DpInviteDeleteEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpInviteDeleteEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildUpdateEvent : public DpEvent< DpGuildUpdateEventInfo >
{
    using Type = dpp::guild_update_t;
    using Base = DpEvent< DpGuildUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildIntegrationsUpdateEvent : public DpEvent< DpGuildIntegrationsUpdateEventInfo >
{
    using Type = dpp::guild_integrations_update_t;
    using Base = DpEvent< DpGuildIntegrationsUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildIntegrationsUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildMemberUpdateEvent : public DpEvent< DpGuildMemberUpdateEventInfo >
{
    using Type = dpp::guild_member_update_t;
    using Base = DpEvent< DpGuildMemberUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildMemberUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpInviteCreateEvent : public DpEvent< DpInviteCreateEventInfo >
{
    using Type = dpp::invite_create_t;
    using Base = DpEvent< DpInviteCreateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpInviteCreateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpMessageUpdateEvent : public DpEvent< DpMessageUpdateEventInfo >
{
    using Type = dpp::message_update_t;
    using Base = DpEvent< DpMessageUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpUserUpdateEvent : public DpEvent< DpUserUpdateEventInfo >
{
    using Type = dpp::user_update_t;
    using Base = DpEvent< DpUserUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpUserUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpMessageCreateEvent : public DpEvent< DpMessageCreateEventInfo >
{
    using Type = dpp::message_create_t;
    using Base = DpEvent< DpMessageCreateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessageCreateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildBanAddEvent : public DpEvent< DpGuildBanAddEventInfo >
{
    using Type = dpp::guild_ban_add_t;
    using Base = DpEvent< DpGuildBanAddEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildBanAddEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildBanRemoveEvent : public DpEvent< DpGuildBanRemoveEventInfo >
{
    using Type = dpp::guild_ban_remove_t;
    using Base = DpEvent< DpGuildBanRemoveEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildBanRemoveEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpIntegrationCreateEvent : public DpEvent< DpIntegrationCreateEventInfo >
{
    using Type = dpp::integration_create_t;
    using Base = DpEvent< DpIntegrationCreateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpIntegrationCreateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpIntegrationUpdateEvent : public DpEvent< DpIntegrationUpdateEventInfo >
{
    using Type = dpp::integration_update_t;
    using Base = DpEvent< DpIntegrationUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpIntegrationUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpIntegrationDeleteEvent : public DpEvent< DpIntegrationDeleteEventInfo >
{
    using Type = dpp::integration_delete_t;
    using Base = DpEvent< DpIntegrationDeleteEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpIntegrationDeleteEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpThreadCreateEvent : public DpEvent< DpThreadCreateEventInfo >
{
    using Type = dpp::thread_create_t;
    using Base = DpEvent< DpThreadCreateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpThreadCreateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpThreadUpdateEvent : public DpEvent< DpThreadUpdateEventInfo >
{
    using Type = dpp::thread_update_t;
    using Base = DpEvent< DpThreadUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpThreadUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpThreadDeleteEvent : public DpEvent< DpThreadDeleteEventInfo >
{
    using Type = dpp::thread_delete_t;
    using Base = DpEvent< DpThreadDeleteEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpThreadDeleteEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpThreadListSyncEvent : public DpEvent< DpThreadListSyncEventInfo >
{
    using Type = dpp::thread_list_sync_t;
    using Base = DpEvent< DpThreadListSyncEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpThreadListSyncEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpThreadMemberUpdateEvent : public DpEvent< DpThreadMemberUpdateEventInfo >
{
    using Type = dpp::thread_member_update_t;
    using Base = DpEvent< DpThreadMemberUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpThreadMemberUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpThreadMembersUpdateEvent : public DpEvent< DpThreadMembersUpdateEventInfo >
{
    using Type = dpp::thread_members_update_t;
    using Base = DpEvent< DpThreadMembersUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpThreadMembersUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildScheduledEventCreateEvent : public DpEvent< DpGuildScheduledEventCreateEventInfo >
{
    using Type = dpp::guild_scheduled_event_create_t;
    using Base = DpEvent< DpGuildScheduledEventCreateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildScheduledEventCreateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildScheduledEventUpdateEvent : public DpEvent< DpGuildScheduledEventUpdateEventInfo >
{
    using Type = dpp::guild_scheduled_event_update_t;
    using Base = DpEvent< DpGuildScheduledEventUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildScheduledEventUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildScheduledEventDeleteEvent : public DpEvent< DpGuildScheduledEventDeleteEventInfo >
{
    using Type = dpp::guild_scheduled_event_delete_t;
    using Base = DpEvent< DpGuildScheduledEventDeleteEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildScheduledEventDeleteEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildScheduledEventUserAddEvent : public DpEvent< DpGuildScheduledEventUserAddEventInfo >
{
    using Type = dpp::guild_scheduled_event_user_add_t;
    using Base = DpEvent< DpGuildScheduledEventUserAddEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildScheduledEventUserAddEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpGuildScheduledEventUserRemoveEvent : public DpEvent< DpGuildScheduledEventUserRemoveEventInfo >
{
    using Type = dpp::guild_scheduled_event_user_remove_t;
    using Base = DpEvent< DpGuildScheduledEventUserRemoveEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildScheduledEventUserRemoveEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpVoiceBufferSendEvent : public DpEvent< DpVoiceBufferSendEventInfo >
{
    using Type = dpp::voice_buffer_send_t;
    using Base = DpEvent< DpVoiceBufferSendEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceBufferSendEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpVoiceUserTalkingEvent : public DpEvent< DpVoiceUserTalkingEventInfo >
{
    using Type = dpp::voice_user_talking_t;
    using Base = DpEvent< DpVoiceUserTalkingEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceUserTalkingEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpVoiceReadyEvent : public DpEvent< DpVoiceReadyEventInfo >
{
    using Type = dpp::voice_ready_t;
    using Base = DpEvent< DpVoiceReadyEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceReadyEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpVoiceReceiveEvent : public DpEvent< DpVoiceReceiveEventInfo >
{
    using Type = dpp::voice_receive_t;
    using Base = DpEvent< DpVoiceReceiveEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceReceiveEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpVoiceReceiveCombinedEvent : public DpEvent< DpVoiceReceiveCombinedEventInfo >
{
    using Type = dpp::voice_receive_t;
    using Base = DpEvent< DpVoiceReceiveCombinedEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceReceiveCombinedEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpVoiceTrackMarkerEvent : public DpEvent< DpVoiceTrackMarkerEventInfo >
{
    using Type = dpp::voice_track_marker_t;
    using Base = DpEvent< DpVoiceTrackMarkerEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceTrackMarkerEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpStageInstanceCreateEvent : public DpEvent< DpStageInstanceCreateEventInfo >
{
    using Type = dpp::stage_instance_create_t;
    using Base = DpEvent< DpStageInstanceCreateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpStageInstanceCreateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpStageInstanceUpdateEvent : public DpEvent< DpStageInstanceUpdateEventInfo >
{
    using Type = dpp::stage_instance_update_t;
    using Base = DpEvent< DpStageInstanceUpdateEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpStageInstanceUpdateEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};
/* ------------------------------------------------------------------------------------------------
 *
*/
struct DpStageInstanceDeleteEvent : public DpEvent< DpStageInstanceDeleteEventInfo >
{
    using Type = dpp::stage_instance_delete_t;
    using Base = DpEvent< DpStageInstanceDeleteEventInfo >;
    // --------------------------------------------------------------------------------------------

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpStageInstanceDeleteEvent(const Type & d) noexcept
        : Base(d)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Transform the event object itself to a script object. Used internally.
    */
    SQMOD_NODISCARD LightObj ToScriptObject() override { return LightObj{this}; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw event.
    */
    SQMOD_NODISCARD const std::string & GetRawEvent() const { return Base::mRaw; }
    /* --------------------------------------------------------------------------------------------
     * Cleanup after the event was processed.
    */
    void Cleanup() override
    {
        // Allow the base to cleanup as well
        Base::Cleanup();
    }
};

} // Namespace:: SqMod
