#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/Discord/Utilities.hpp"

// ------------------------------------------------------------------------------------------------
#include <dpp/dpp.h>

// ------------------------------------------------------------------------------------------------
#include <chrono>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Represents the caching policy of the cluster.
*/
struct DpCachePolicy
{
    SQInteger mUserPolicy{dpp::cp_aggressive};
    SQInteger mEmojiPolicy{dpp::cp_aggressive};
    SQInteger mRolePolicy{dpp::cp_aggressive};
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpCachePolicy() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpCachePolicy(SQInteger user) noexcept
        : mUserPolicy(user), mEmojiPolicy(dpp::cp_aggressive), mRolePolicy(dpp::cp_aggressive)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    DpCachePolicy(SQInteger user, SQInteger emoji) noexcept
        : mUserPolicy(user), mEmojiPolicy(emoji), mRolePolicy(dpp::cp_aggressive)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    DpCachePolicy(SQInteger user, SQInteger emoji, SQInteger role) noexcept
        : mUserPolicy(user), mEmojiPolicy(emoji), mRolePolicy(role)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    DpCachePolicy(const DpCachePolicy &) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Convert to native cache policy type.
    */
    SQMOD_NODISCARD dpp::cache_policy_t ToNative() const noexcept
    {
        return dpp::cache_policy_t{
            static_cast< dpp::cache_policy_setting_t >(mUserPolicy),
            static_cast< dpp::cache_policy_setting_t >(mEmojiPolicy),
            static_cast< dpp::cache_policy_setting_t >(mRolePolicy)
        };
    }
};

/* ------------------------------------------------------------------------------------------------
 * Represents the voice state of a user on a guild.
 * These are stored in the DpGuild object, and accessible there, or via DpChannel::GetVoiceMembers.
*/
struct DpVoiceState
{
    using Ptr = std::unique_ptr< dpp::voicestate >;
    /* --------------------------------------------------------------------------------------------
     * Referenced voice state instance.
    */
    Ptr mPtr{nullptr};
    /* --------------------------------------------------------------------------------------------
     * Whether the referenced pointer is owned.
    */
    bool mOwned{false};
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpVoiceState() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceState(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVoiceState(const Ptr::element_type & o) noexcept
        : DpVoiceState(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpVoiceState(Ptr::element_type && o) noexcept
        : DpVoiceState(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpVoiceState(const DpVoiceState & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpVoiceState(DpVoiceState && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpVoiceState() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpVoiceState & operator = (const DpVoiceState & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpVoiceState & operator = (DpVoiceState && o) noexcept
    {
        if (this != &o) {
            Cleanup();
            // Transfer members values
            mPtr = std::move(o.mPtr);
            mOwned = o.mOwned;
        }
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Release any referenced resources and default to an empty/invalid state.
    */
    void Cleanup()
    {
        // Do we own this to try delete it?
        if (!mOwned && mPtr) {
            // Not our job, simply forget about it
            [[maybe_unused]] auto p = mPtr.release();
        } else mPtr.reset(); // We own this so delete the instance
    }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mPtr) STHROWF("Invalid discord voice state handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild id this voice state is for (optional).
    */
    SQMOD_NODISCARD dpp::snowflake GetGuildID() const { return Valid().guild_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the channel id this user is connected to (may be empty).
    */
    SQMOD_NODISCARD dpp::snowflake GetChannelID() const { return Valid().channel_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the user id this voice state is for.
    */
    SQMOD_NODISCARD dpp::snowflake GetUserID() const { return Valid().user_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the session id for this voice state.
    */
    SQMOD_NODISCARD const std::string & GetSessionID() const { return Valid().session_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the voice state flags (see SqDiscordVoiceStateFlags).
    */
    SQMOD_NODISCARD SQInteger GetFlags() const { return Valid().flags; }
    /* --------------------------------------------------------------------------------------------
     * Modify the voice state flags (see SqDiscordVoiceStateFlags).
    */
    void SetFlags(SQInteger flags) const { Valid().flags = flags; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the time at which the user requested to speak, or 0.
    */
    SQMOD_NODISCARD SQInteger GetRequestToSpeak() const {
        return static_cast< SQInteger >(std::chrono::duration_cast< std::chrono::seconds >(
            std::chrono::system_clock::from_time_t(Valid().request_to_speak).time_since_epoch()
        ).count());
    }
    /* --------------------------------------------------------------------------------------------
     * Build json representation of the object.
    */
    SQMOD_NODISCARD std::string BuildJSON() const { return Valid().build_json(); }
    SQMOD_NODISCARD std::string BuildJSON_(bool with_id) const { return Valid().build_json(with_id); }
    /* --------------------------------------------------------------------------------------------
     * Check if the user is deafened by the server.
    */
    SQMOD_NODISCARD bool IsDeaf() const { return Valid().is_deaf(); }
    /* --------------------------------------------------------------------------------------------
     * Check if the user is muted by the server.
    */
    SQMOD_NODISCARD bool IsMute() const { return Valid().is_mute(); }
    /* --------------------------------------------------------------------------------------------
     * Check if the user muted themselves.
    */
    SQMOD_NODISCARD bool IsSelfMute() const { return Valid().is_self_mute(); }
    /* --------------------------------------------------------------------------------------------
     * Check if the user deafened themselves.
    */
    SQMOD_NODISCARD bool IsSelfDeaf() const { return Valid().is_self_deaf(); }
    /* --------------------------------------------------------------------------------------------
     * Check if the user is streaming using "Go Live".
    */
    SQMOD_NODISCARD bool SelfStream() const { return Valid().self_stream(); }
    /* --------------------------------------------------------------------------------------------
     * Check if the user's camera is enabled.
    */
    SQMOD_NODISCARD bool SelfVideo() const { return Valid().self_video(); }
    /* --------------------------------------------------------------------------------------------
     * Check if user is suppressed. "HELP HELP I'M BEING SUPPRESSED!"
    */
    SQMOD_NODISCARD bool IsSuppressed() const { return Valid().is_suppressed(); }
};


/* ------------------------------------------------------------------------------------------------
 * Represents the voice state of a user on a guild.
*/
struct DpEmoji
{
    using Ptr = std::unique_ptr< dpp::emoji >;
    /* --------------------------------------------------------------------------------------------
     * Referenced voice state instance.
    */
    Ptr mPtr{nullptr};
    /* --------------------------------------------------------------------------------------------
     * Whether the referenced pointer is owned.
    */
    bool mOwned{false};
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpEmoji() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpEmoji(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpEmoji(const Ptr::element_type & o) noexcept
        : DpEmoji(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpEmoji(Ptr::element_type && o) noexcept
        : DpEmoji(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    DpEmoji(StackStrF & name, dpp::snowflake id, SQInteger flags)
        : DpEmoji(new Ptr::element_type(name.ToStr(), id, static_cast< uint8_t >(flags)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpEmoji(const DpEmoji & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpEmoji(DpEmoji && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpEmoji() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpEmoji & operator = (const DpEmoji & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpEmoji & operator = (DpEmoji && o) noexcept
    {
        if (this != &o) {
            Cleanup();
            // Transfer members values
            mPtr = std::move(o.mPtr);
            mOwned = o.mOwned;
        }
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Release any referenced resources and default to an empty/invalid state.
    */
    void Cleanup()
    {
        // Do we own this to try delete it?
        if (!mOwned && mPtr) {
            // Not our job, simply forget about it
            [[maybe_unused]] auto p = mPtr.release();
        } else mPtr.reset(); // We own this so delete the instance
    }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mPtr) STHROWF("Invalid discord voice state handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the emoji.
    */
    SQMOD_NODISCARD const std::string & GetName() const { return Valid().name; }
    /* --------------------------------------------------------------------------------------------
     * Modify the name of the emoji.
    */
    void SetName(StackStrF & name) const { Valid().name = name.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the name of the emoji.
    */
    DpEmoji & ApplyName(StackStrF & name) { SetName(name); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the user who uploaded the emoji.
    */
    SQMOD_NODISCARD dpp::snowflake GetUserID() const { return Valid().user_id; }
    /* --------------------------------------------------------------------------------------------
     * Modify the user who uploaded the emoji.
    */
    void SetUserID(dpp::snowflake id) const { Valid().user_id = id; }
    /* --------------------------------------------------------------------------------------------
     * Modify the user who uploaded the emoji.
    */
    DpEmoji & ApplyUserID(dpp::snowflake id) { SetUserID(id); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the flags for the emoji from SqDiscordEmojiFlags.
    */
    SQMOD_NODISCARD SQInteger GetFlags() const { return Valid().flags; }
    /* --------------------------------------------------------------------------------------------
     * Modify the flags for the emoji from SqDiscordEmojiFlags.
    */
    void SetFlags(SQInteger flags) const { Valid().flags = static_cast< uint8_t >(flags); }
    /* --------------------------------------------------------------------------------------------
     * Modify the flags for the emoji from SqDiscordEmojiFlags.
    */
    DpEmoji & ApplyFlags(SQInteger flags) { SetFlags(flags); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the image data for the emoji if uploading.
    */
    SQMOD_NODISCARD LightObj GetImageData() const { return !Valid().image_data ? LightObj{} : LightObj(*Valid().image_data); }
    /* --------------------------------------------------------------------------------------------
     * Create a mentionable emoji.
    */
    SQMOD_NODISCARD std::string GetMention_(StackStrF & name, dpp::snowflake id, bool is_animated = false) const
    { return Valid().get_mention(name.ToStr(), id, is_animated); }
    /* --------------------------------------------------------------------------------------------
     * Build json representation of the object.
    */
    SQMOD_NODISCARD std::string BuildJSON() const { return Valid().build_json(); }
    SQMOD_NODISCARD std::string BuildJSON_(bool with_id) const { return Valid().build_json(with_id); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve whether the Emoji requires colons.
    */
    SQMOD_NODISCARD bool GetRequiresColons() const { return Valid().requires_colons(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve whether the Emoji is managed.
    */
    SQMOD_NODISCARD bool GetIsManaged() const { return Valid().is_managed(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve whether the Emoji is animated.
    */
    SQMOD_NODISCARD bool GetIsAnimated() const { return Valid().is_animated(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve whether the Emoji is available.
    */
    SQMOD_NODISCARD bool GetIsAvailable() const { return Valid().is_available(); }
    /* --------------------------------------------------------------------------------------------
     * Load an image into the object as base64.
    */
    DpEmoji & LoadImage(StackStrF & data, SQInteger type) { Valid().load_image(data.ToStr(), static_cast< dpp::image_type >(type)); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Format to name if unicode, name:id if has id or a:name:id if animated.
    */
    SQMOD_NODISCARD std::string Format() const { return Valid().format(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the mention/ping for the emoji.
    */
    SQMOD_NODISCARD std::string GetMention() const { return Valid().get_mention(); }
};

/* ------------------------------------------------------------------------------------------------
 * The results of a REST call wrapped in a convenient struct.
*/
struct DpCommandConfirmation
{
    using Ptr = std::unique_ptr< dpp::confirmation_callback_t >;
    /* --------------------------------------------------------------------------------------------
     * Referenced confirmation callback instance.
    */
    Ptr mPtr{nullptr};
    /* --------------------------------------------------------------------------------------------
     * Whether the referenced pointer is owned.
    */
    bool mOwned{false};
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpCommandConfirmation() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpCommandConfirmation(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpCommandConfirmation(Ptr && ptr) noexcept
        : mPtr(std::forward< Ptr >(ptr)), mOwned(true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpCommandConfirmation(const Ptr::element_type & o) noexcept
        : DpCommandConfirmation(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpCommandConfirmation(Ptr::element_type && o) noexcept
        : DpCommandConfirmation(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpCommandConfirmation(const DpCommandConfirmation & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpCommandConfirmation(DpCommandConfirmation && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpCommandConfirmation() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpCommandConfirmation & operator = (const DpCommandConfirmation & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpCommandConfirmation & operator = (DpCommandConfirmation && o) noexcept
    {
        if (this != &o) {
            Cleanup();
            // Transfer members values
            mPtr = std::move(o.mPtr);
            mOwned = o.mOwned;
        }
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Release any referenced resources and default to an empty/invalid state.
    */
    void Cleanup()
    {
        // Do we own this to try delete it?
        if (!mOwned && mPtr) {
            // Not our job, simply forget about it
            [[maybe_unused]] auto p = mPtr.release();
        } else mPtr.reset(); // We own this so delete the instance
    }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mPtr) STHROWF("Invalid discord confirmation callback handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw body string of the HTTP result.
    */
    SQMOD_NODISCARD std::string & GetHttpBody() const { return Valid().http_info.body; }
};

} // Namespace:: SqMod
