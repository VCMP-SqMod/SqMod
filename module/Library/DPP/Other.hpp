#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <chrono>

// ------------------------------------------------------------------------------------------------
#include <dpp/dpp.h>

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
 * An activity is a representation of what a user is doing. It might be a game, or a website, or a movie. Whatever.
*/
struct DpActivity
{
    using Ptr = std::unique_ptr< dpp::activity >;
    /* --------------------------------------------------------------------------------------------
     * Referenced activity instance.
    */
    Ptr mPtr{nullptr};
    /* --------------------------------------------------------------------------------------------
     * Wether the referenced pointer is owned.
    */
    bool mOwned{false};
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpActivity() noexcept
        : DpActivity(new Ptr::element_type(), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpActivity(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit DpActivity(const Ptr::element_type & o) noexcept
        : DpActivity(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpActivity(Ptr::element_type && o) noexcept
        : DpActivity(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    DpActivity(SQInteger type, StackStrF & name, StackStrF & state, StackStrF & url)
        : DpActivity(new Ptr::element_type(static_cast< dpp::activity_type >(type), name.ToStr(), state.ToStr(), url.ToStr()), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpActivity(const DpActivity & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpActivity(DpActivity && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpActivity() noexcept
    {
        // Do we own this to try delete it?
        if (!mOwned && mPtr) [[maybe_unused]] auto p = mPtr.release();
    }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpActivity & operator = (const DpActivity & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpActivity & operator = (DpActivity && o) noexcept
    {
        if (this != &o) {
            // Do we own this to try delete it?
            if (!mOwned && mPtr) [[maybe_unused]] auto p = mPtr.release();
            // Transfer members values
            mPtr = std::move(o.mPtr);
            mOwned = o.mOwned;
        }
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mPtr) STHROWF("Invalid discord activity handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check wether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the activity.
    */
    SQMOD_NODISCARD const std::string & GetName() const { return Valid().name; }
    /* --------------------------------------------------------------------------------------------
     * Modify the name of the activity.
    */
    void SetName(StackStrF & name) const { Valid().name = name.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the name of the activity.
    */
    DpActivity & ApplyName(StackStrF & name) { SetName(name); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the state of the activity.
    */
    SQMOD_NODISCARD const std::string & GetState() const { return Valid().state; }
    /* --------------------------------------------------------------------------------------------
     * Modify the state of the activity.
    */
    void SetState(StackStrF & state) const { Valid().state = state.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the state of the activity.
    */
    DpActivity & ApplyState(StackStrF & state) { SetState(state); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the url of the activity.
    */
    SQMOD_NODISCARD const std::string & GetURL() const { return Valid().url; }
    /* --------------------------------------------------------------------------------------------
     * Modify the url of the activity.
    */
    void SetURL(StackStrF & url) const { Valid().url = url.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the url of the activity.
    */
    DpActivity & ApplyURL(StackStrF & url) { SetURL(url); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the type of the activity.
    */
    SQMOD_NODISCARD SQInteger GetType() const { return static_cast< SQInteger >(Valid().type); }
    /* --------------------------------------------------------------------------------------------
     * Modify the type of the activity.
    */
    void SetType(SQInteger s) const { Valid().type = static_cast< dpp::activity_type >(s); }
    /* --------------------------------------------------------------------------------------------
     * Modify the type of the activity.
    */
    DpActivity & ApplyType(SQInteger s) { SetType(s); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve when the activity was created.
    */
    SQMOD_NODISCARD SQInteger GetCreatedAt() const
    {
        return static_cast< SQInteger >(std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::from_time_t(Valid().created_at).time_since_epoch()).count());
    }
    /* --------------------------------------------------------------------------------------------
     * Modify when the activity was created.
    */
    void SetCreatedAt(SQInteger s) const
    {
        Valid().created_at = std::chrono::system_clock::to_time_t(std::chrono::time_point< std::chrono::system_clock >{std::chrono::seconds{s}});
    }
    /* --------------------------------------------------------------------------------------------
     * Modify when the activity was created.
    */
    DpActivity & ApplyCreatedAt(SQInteger s) { SetCreatedAt(s); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve when the activity was started.
    */
    SQMOD_NODISCARD SQInteger GetStart() const
    {
        return static_cast< SQInteger >(std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::from_time_t(Valid().start).time_since_epoch()).count());
    }
    /* --------------------------------------------------------------------------------------------
     * Modify when the activity was started.
    */
    void SetStart(SQInteger s) const
    {
        Valid().start = std::chrono::system_clock::to_time_t(std::chrono::time_point< std::chrono::system_clock >{std::chrono::seconds{s}});
    }
    /* --------------------------------------------------------------------------------------------
     * Modify when the activity was started.
    */
    DpActivity & ApplyStart(SQInteger s) { SetStart(s); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve when the activity was stopped.
    */
    SQMOD_NODISCARD SQInteger GetEnd() const
    {
        return static_cast< SQInteger >(std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::from_time_t(Valid().end).time_since_epoch()).count());
    }
    /* --------------------------------------------------------------------------------------------
     * Modify when the activity was stopped.
    */
    void SetEnd(SQInteger s) const
    {
        Valid().end = std::chrono::system_clock::to_time_t(std::chrono::time_point< std::chrono::system_clock >{std::chrono::seconds{s}});
    }
    /* --------------------------------------------------------------------------------------------
     * Modify when the activity was stopped.
    */
    DpActivity & ApplyEnd(SQInteger s) { SetEnd(s); return *this; }
};

/* ------------------------------------------------------------------------------------------------
 * Represents user presence, e.g. what game they are playing and if they are online.
*/
struct DpPresence
{
    using Ptr = std::unique_ptr< dpp::presence >;
    /* --------------------------------------------------------------------------------------------
     * Referenced presence instance.
    */
    Ptr mPtr{nullptr};
    /* --------------------------------------------------------------------------------------------
     * Wether the referenced pointer is owned.
    */
    bool mOwned{false};
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpPresence() noexcept
        : DpPresence(new Ptr::element_type(), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpPresence(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit DpPresence(const Ptr::element_type & o) noexcept
        : DpPresence(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpPresence(Ptr::element_type && o) noexcept
        : DpPresence(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpPresence(const DpPresence & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpPresence(DpPresence && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpPresence() noexcept
    {
        // Do we own this to try delete it?
        if (!mOwned && mPtr) [[maybe_unused]] auto p = mPtr.release();
    }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpPresence & operator = (const DpPresence & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpPresence & operator = (DpPresence && o) noexcept
    {
        if (this != &o) {
            // Do we own this to try delete it?
            if (!mOwned && mPtr) [[maybe_unused]] auto p = mPtr.release();
            // Transfer members values
            mPtr = std::move(o.mPtr);
            mOwned = o.mOwned;
        }
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mPtr) STHROWF("Invalid discord presence handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check wether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the user that the presence applies to.
    */
    SQMOD_NODISCARD dpp::snowflake GetUserID() const { return Valid().user_id; }
    /* --------------------------------------------------------------------------------------------
     * Modify the user that the presence applies to.
    */
    void SetUserID(dpp::snowflake id) const { Valid().user_id = id; }
    /* --------------------------------------------------------------------------------------------
     * Modify the user that the presence applies to.
    */
    DpPresence & ApplyUserID(dpp::snowflake id) { SetUserID(id); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild that the presence applies to.
    */
    SQMOD_NODISCARD dpp::snowflake GetGuildID() const { return Valid().guild_id; }
    /* --------------------------------------------------------------------------------------------
     * Modify the guild that the presence applies to.
    */
    void SetGuildID(dpp::snowflake id) const { Valid().guild_id = id; }
    /* --------------------------------------------------------------------------------------------
     * Modify the guild that the presence applies to.
    */
    DpPresence & ApplyGuildID(dpp::snowflake id) { SetGuildID(id); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the presence bit-mask.
    */
    SQMOD_NODISCARD SQInteger GetFlags() const { return static_cast< SQInteger >(Valid().flags); }
    /* --------------------------------------------------------------------------------------------
     * Modify the presence bit-mask.
    */
    void SetFlags(SQInteger f) const { Valid().flags = static_cast< uint8_t >(f); }
    /* --------------------------------------------------------------------------------------------
     * Modify the presence bit-mask.
    */
    DpPresence & ApplyFlags(SQInteger f) { SetFlags(f); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of activities.
    */
    SQMOD_NODISCARD SQInteger ActivityCount() const { return static_cast< SQInteger >(Valid().activities.size()); }
    /* --------------------------------------------------------------------------------------------
     * Add a new activity.
    */
    DpPresence & AddActivity(const DpActivity & a) { Valid().activities.push_back(a.Valid()); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Iterate all activities.
    */
    DpPresence & EachActivity(Function & fn)
    {
        for (const auto & a : Valid().activities)
        {
            fn.Execute(a);
        }
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of activities.
    */
    DpPresence & ClearActivities(const DpActivity & a) { Valid().activities.clear(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Filter activities.
    */
    DpPresence & FilterActivities(Function & fn)
    {
        std::vector< dpp::activity > list;
        // Reserve memory in advance
        list.reserve(Valid().activities.size());
        // Process each activity individually
        for (const auto & a : Valid().activities)
        {
            auto ret = fn.Eval(a);
            // (null || true) == keep & false == skip
            if (!ret.IsNull() || !ret.template Cast< bool >())
            {
                list.push_back(a); // Keep this activity
            }
        }
        // Use filtered activities
        Valid().activities.swap(list);
        // Allow chaining
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Build JSON string from this object.
    */
    SQMOD_NODISCARD std::string BuildJSON() const { return Valid().build_json(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the users status on desktop.
    */
    SQMOD_NODISCARD SQInteger GetDesktopStatus() const { return static_cast< SQInteger >(Valid().desktop_status()); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the user's status on web.
    */
    SQMOD_NODISCARD SQInteger GetWebStatus() const { return static_cast< SQInteger >(Valid().web_status()); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the user's status on mobile.
    */
    SQMOD_NODISCARD SQInteger GetMobileStatus() const { return static_cast< SQInteger >(Valid().mobile_status()); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the user's status as shown to other users.
    */
    SQMOD_NODISCARD SQInteger GetStatus() const { return static_cast< SQInteger >(Valid().status()); }
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
     * Wether the referenced pointer is owned.
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
    ~DpVoiceState() noexcept
    {
        // Do we own this to try delete it?
        if (!mOwned && mPtr) [[maybe_unused]] auto p = mPtr.release();
    }
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
            // Do we own this to try delete it?
            if (!mOwned && mPtr) [[maybe_unused]] auto p = mPtr.release();
            // Transfer members values
            mPtr = std::move(o.mPtr);
            mOwned = o.mOwned;
        }
        return *this;
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
     * Check wether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild id this voice state is for (optional).
    */
    SQMOD_NODISCARD dpp::snowflake GetGuildID() const { return Valid().guild_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild id this voice state is for (optional).
    */
    SQMOD_NODISCARD dpp::snowflake GetChannelID() const { return Valid().channel_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild id this voice state is for (optional).
    */
    SQMOD_NODISCARD dpp::snowflake GetUserID() const { return Valid().user_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild id this voice state is for (optional).
    */
    SQMOD_NODISCARD const std::string & GetSessionID() const { return Valid().session_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild id this voice state is for (optional).
    */
    SQMOD_NODISCARD SQInteger GetFlags() const { return Valid().flags; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild id this voice state is for (optional).
    */
    void SetFlags(SQInteger flags) const { Valid().flags = flags; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild id this voice state is for (optional).
    */
    SQMOD_NODISCARD std::string BuildJSON() const { return Valid().build_json(); }
    /* --------------------------------------------------------------------------------------------
     * Check if user is deafened.
    */
    SQMOD_NODISCARD bool IsDeaf() const { return Valid().is_deaf(); }
    /* --------------------------------------------------------------------------------------------
     * Check if user is muted.
    */
    SQMOD_NODISCARD bool IsMute() const { return Valid().is_mute(); }
    /* --------------------------------------------------------------------------------------------
     * Check if user muted themselves.
    */
    SQMOD_NODISCARD bool IsSelfMute() const { return Valid().is_self_mute(); }
    /* --------------------------------------------------------------------------------------------
     * Check if user deafened themselves.
    */
    SQMOD_NODISCARD bool IsSelfDeaf() const { return Valid().is_self_deaf(); }
    /* --------------------------------------------------------------------------------------------
     * Check if user is streamig.
    */
    SQMOD_NODISCARD bool SelfStream() const { return Valid().self_stream(); }
    /* --------------------------------------------------------------------------------------------
     * Check if user is in video.
    */
    SQMOD_NODISCARD bool SelfVideo() const { return Valid().self_video(); }
    /* --------------------------------------------------------------------------------------------
     * Check if user is surpressed.
    */
    SQMOD_NODISCARD bool IsSupressed() const { return Valid().is_supressed(); }
};

/* ------------------------------------------------------------------------------------------------
 * 
*/

} // Namespace:: SqMod
