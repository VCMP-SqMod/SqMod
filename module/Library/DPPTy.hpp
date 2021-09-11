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
 * Represents a user on discord. May or may not be a member of a DpGuild.
*/
struct DpUser
{
    using Ptr = std::unique_ptr< dpp::user >;
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
    DpUser() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpUser(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit DpUser(const Ptr::element_type & o) noexcept
        : DpUser(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpUser(Ptr::element_type && o) noexcept
        : DpUser(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpUser(const DpUser & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpUser(DpUser && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpUser() noexcept
    {
        // Do we own this to try delete it?
        if (!mOwned && mPtr) [[maybe_unused]] auto p = mPtr.release();
    }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpUser & operator = (const DpUser & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpUser & operator = (DpUser && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord user handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check wether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve user discord username.
    */
    SQMOD_NODISCARD const std::string & GetUsername() const { return Valid().username; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve user discriminator (aka tag), 4 digits usually displayed with leading zeroes.
    */
    SQMOD_NODISCARD SQInteger GetDiscriminator() const { return static_cast< SQInteger >(Valid().discriminator); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve user avatar hash.
    */
    SQMOD_NODISCARD const dpp::utility::iconhash & GetAvatar() const { return Valid().avatar; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve user flags built from a bitmask of values in dpp::user_flags.
    */
    SQMOD_NODISCARD SQInteger GetFlags() const { return static_cast< SQInteger >(Valid().flags); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve user feference count of how many guilds this user is in.
    */
    SQMOD_NODISCARD SQInteger GetRefCount() const { return static_cast< SQInteger >(Valid().refcount); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the avatar url of the user object.
    */
    SQMOD_NODISCARD std::string GetAvatarURL() const { return Valid().get_avatar_url(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user is a bot.
    */
    SQMOD_NODISCARD bool IsBot() const { return Valid().is_bot(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user is a system user (Clyde).
    */
    SQMOD_NODISCARD bool IsSystem() const { return Valid().is_system(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user has multi-factor authentication enabled.
    */
    SQMOD_NODISCARD bool IsMfaEnabled() const { return Valid().is_mfa_enabled(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user has verified account.
    */
    SQMOD_NODISCARD bool IsVerified() const { return Valid().is_verified(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user has full nitro.
    */
    SQMOD_NODISCARD bool HasNitroFull() const { return Valid().has_nitro_full(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user has nitro classic.
    */
    SQMOD_NODISCARD bool HasNitroClassic() const { return Valid().has_nitro_classic(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user is a discord employee.
    */
    SQMOD_NODISCARD bool IsDiscordEmployee() const { return Valid().is_discord_employee(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user owns a partnered server.
    */
    SQMOD_NODISCARD bool IsPartneredOwner() const { return Valid().is_partnered_owner(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user has hype-squad events.
    */
    SQMOD_NODISCARD bool HasHypesquadEvents() const { return Valid().has_hypesquad_events(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user has the bug-hunter level 1 badge.
    */
    SQMOD_NODISCARD bool IsBughunter1() const { return Valid().is_bughunter_1(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user is in house bravery.
    */
    SQMOD_NODISCARD bool IsHouseBravery() const { return Valid().is_house_bravery(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user is in house brilliance.
    */
    SQMOD_NODISCARD bool IsHouseBrilliance() const { return Valid().is_house_brilliance(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user is in house balance.
    */
    SQMOD_NODISCARD bool IsHouseBalanace() const { return Valid().is_house_balanace(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user is an early supporter.
    */
    SQMOD_NODISCARD bool IsEarlySupporter() const { return Valid().is_early_supporter(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user is a team user.
    */
    SQMOD_NODISCARD bool IsTeamUser() const { return Valid().is_team_user(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user has the bug-hunter level 2 badge.
    */
    SQMOD_NODISCARD bool IsBughunter2() const { return Valid().is_bughunter_2(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user has the verified bot badge.
    */
    SQMOD_NODISCARD bool IsVerifiedBot() const { return Valid().is_verified_bot(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user is an early verified bot developer.
    */
    SQMOD_NODISCARD bool IsVerifiedBotDev() const { return Valid().is_verified_bot_dev(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user is a certified moderator.
    */
    SQMOD_NODISCARD bool IsCertifiedDoderator() const { return Valid().is_certified_moderator(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user has an animated icon.
    */
    SQMOD_NODISCARD bool HasAnimatedIcon() const { return Valid().has_animated_icon(); }
};

/* ------------------------------------------------------------------------------------------------
 * Represents a guild on Discord (AKA a server).
*/
struct DpGuildMember
{
    using Ptr = std::unique_ptr< dpp::guild_member >;
    /* --------------------------------------------------------------------------------------------
     * Referenced guild member instance.
    */
    Ptr mPtr{nullptr};
    /* --------------------------------------------------------------------------------------------
     * Wether the referenced pointer is owned.
    */
    bool mOwned{false};
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpGuildMember() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuildMember(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit DpGuildMember(const Ptr::element_type & o) noexcept
        : DpGuildMember(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpGuildMember(Ptr::element_type && o) noexcept
        : DpGuildMember(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpGuildMember(const DpGuildMember & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpGuildMember(DpGuildMember && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpGuildMember() noexcept
    {
        // Do we own this to try delete it?
        if (!mOwned && mPtr) [[maybe_unused]] auto p = mPtr.release();
    }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpGuildMember & operator = (const DpGuildMember & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpGuildMember & operator = (DpGuildMember && o) noexcept
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
     * Retrieve the gickname, or empty string if they don't have a nickname on this guild.
    */
    SQMOD_NODISCARD const std::string & GetNickname() const { return Valid().nickname; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild id.
    */
    SQMOD_NODISCARD dpp::snowflake GetGuildID() const { return Valid().guild_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the user id.
    */
    SQMOD_NODISCARD dpp::snowflake GetUserID() const { return Valid().user_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the list of roles this user has on this guild.
    */
    SQMOD_NODISCARD Array GetRoles() const
    {
        return Array(SqVM()).Reserve(static_cast< SQInteger >(Valid().roles.size()))
                            .AppendFromVector(Valid().roles);
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the date and time since the user joined the guild.
    */
    SQMOD_NODISCARD SQInteger GetJoinedAt() const
    {
        return std::chrono::time_point_cast< std::chrono::seconds >(
                std::chrono::system_clock::from_time_t(Valid().joined_at)
            ).time_since_epoch().count();
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the date and time since the user has boosted the guild guild.
    */
    SQMOD_NODISCARD SQInteger GetPremiumSince() const
    {
        return std::chrono::time_point_cast< std::chrono::seconds >(
                std::chrono::system_clock::from_time_t(Valid().premium_since)
            ).time_since_epoch().count();
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the set of flags built from the bitmask defined by dpp::guild_member_flags.
    */
    SQMOD_NODISCARD SQInteger GetFlags() const { return static_cast< SQInteger >(Valid().flags); }
    /* --------------------------------------------------------------------------------------------
     * Build JSON string for the member object.
    */
    SQMOD_NODISCARD std::string BuildJSON() const { return Valid().build_json(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user is deafened.
    */
    SQMOD_NODISCARD bool IsDeaf() const { return Valid().is_deaf(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user is muted.
    */
    SQMOD_NODISCARD bool IsMuted() const { return Valid().is_muted(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the user is pending verification by membership screening.
    */
    SQMOD_NODISCARD bool IsPending() const { return Valid().is_pending(); }
};

/* ------------------------------------------------------------------------------------------------
 * Represents a guild on Discord (AKA a server)
*/
struct DpGuild
{
    using Ptr = std::unique_ptr< dpp::guild >;
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
    DpGuild() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpGuild(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit DpGuild(const Ptr::element_type & o) noexcept
        : DpGuild(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpGuild(Ptr::element_type && o) noexcept
        : DpGuild(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpGuild(const DpGuild & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpGuild(DpGuild && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpGuild() noexcept
    {
        // Do we own this to try delete it?
        if (!mOwned && mPtr) [[maybe_unused]] auto p = mPtr.release();
    }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpGuild & operator = (const DpGuild & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpGuild & operator = (DpGuild && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord guild handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check wether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the shard ID of the guild.
    */
    SQMOD_NODISCARD SQInteger GetShardID() const { return static_cast< SQInteger >(Valid().shard_id); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the flags bitmask as defined by values within dpp::guild_flags.
    */
    SQMOD_NODISCARD SQInteger GetFlags() const { return static_cast< SQInteger >(Valid().flags); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild name.
    */
    SQMOD_NODISCARD const std::string & GetName() const { return Valid().name; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the server description for communities.
    */
    SQMOD_NODISCARD const std::string & GetDescription() const { return Valid().description; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the vanity url code for verified or partnered servers and boost level 3.
    */
    SQMOD_NODISCARD const std::string & GetVanityUrlCode() const { return Valid().vanity_url_code; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild icon hash.
    */
    SQMOD_NODISCARD const dpp::utility::iconhash & GetIcon() const { return Valid().icon; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild splash hash.
    */
    SQMOD_NODISCARD const dpp::utility::iconhash & GetSplash() const { return Valid().splash; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild discovery splash hash.
    */
    SQMOD_NODISCARD const dpp::utility::iconhash & GetDiscoverySplash() const { return Valid().discovery_splash; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the snowflake id of guild owner.
    */
    SQMOD_NODISCARD dpp::snowflake GetOwnerID() const { return Valid().owner_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild voice region.
    */
    SQMOD_NODISCARD SQInteger GetVoiceRegion() const { return static_cast< SQInteger >(Valid().voice_region); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the snowflake ID of AFK voice channel or 0.
    */
    SQMOD_NODISCARD dpp::snowflake GetAfkChannelID() const { return Valid().afk_channel_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the voice AFK timeout before moving users to AFK channel.
    */
    SQMOD_NODISCARD SQInteger GetAfkTimeout() const { return static_cast< SQInteger >(Valid().afk_timeout); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the snowflake ID of widget channel, or 0.
    */
    SQMOD_NODISCARD dpp::snowflake GetWidgetChannelID() const { return Valid().widget_channel_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the verification level of server.
    */
    SQMOD_NODISCARD SQInteger GetVerificationLevel() const { return static_cast< SQInteger >(Valid().verification_level); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the setting for how notifications are to be delivered to users.
    */
    SQMOD_NODISCARD SQInteger GetDefaultMessageNotifications() const { return static_cast< SQInteger >(Valid().default_message_notifications); }
    /* --------------------------------------------------------------------------------------------
     * Check wether or not explicit content filtering is enable and what setting it is.
    */
    SQMOD_NODISCARD SQInteger GetExplicitContentFilter() const { return static_cast< SQInteger >(Valid().explicit_content_filter); }
    /* --------------------------------------------------------------------------------------------
     * Check wether multi factor authentication is required for moderators or not.
    */
    SQMOD_NODISCARD SQInteger GetMfaLevel() const { return static_cast< SQInteger >(Valid().mfa_level); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the ID of creating application, if any, or 0.
    */
    SQMOD_NODISCARD dpp::snowflake GetApplicationID() const { return Valid().application_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the ID of system channel where discord update messages are sent.
    */
    SQMOD_NODISCARD dpp::snowflake GetSystemChannelID() const { return Valid().system_channel_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the ID of rules channel for communities.
    */
    SQMOD_NODISCARD dpp::snowflake GetRulesChannelID() const { return Valid().rules_channel_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the approximate member count. May be sent as zero.
    */
    SQMOD_NODISCARD SQInteger GetMemberCount() const { return static_cast< SQInteger >(Valid().member_count); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the server banner hash.
    */
    SQMOD_NODISCARD const dpp::utility::iconhash & GetBanner() const { return Valid().banner; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the boost level.
    */
    SQMOD_NODISCARD SQInteger GetPremiumTier() const { return static_cast< SQInteger >(Valid().premium_tier); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of boosters.
    */
    SQMOD_NODISCARD SQInteger GetPremiumSubscriptionCount() const { return static_cast< SQInteger >(Valid().premium_subscription_count); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve public updates channel ID or 0.
    */
    SQMOD_NODISCARD dpp::snowflake GetPublicUpdatesChannelID() const { return Valid().public_updates_channel_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the maximum users in a video channel, or 0.
    */
    SQMOD_NODISCARD SQInteger GetMaxVideoChannelUsers() const { return static_cast< SQInteger >(Valid().max_video_channel_users); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a roles defined on this server.
    */
    SQMOD_NODISCARD Array GetRoles() const { return Array(SqVM()).Reserve(static_cast< SQInteger >(Valid().roles.size())).AppendFromVector(Valid().roles); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a list of channels on this server.
    */
    SQMOD_NODISCARD Array GetChannels() const { return Array(SqVM()).Reserve(static_cast< SQInteger >(Valid().channels.size())).AppendFromVector(Valid().channels); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a list of threads on this server.
    */
    SQMOD_NODISCARD Array GetThreads() const { return Array(SqVM()).Reserve(static_cast< SQInteger >(Valid().threads.size())).AppendFromVector(Valid().threads); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a list of guild members. Note that when you first receive the guild create event,
     * this may be empty or near empty. This depends upon your dpp::intents and the size of your bot.
     * It will be filled by guild member chunk requests.
    */
    SQMOD_NODISCARD Table GetMembers() const
    {
        Table t(SqVM(), static_cast< SQInteger >(Valid().members.size()));
        // Attempt to convert the [members] associative container into a script table
        const auto r = t.InsertFromMapWith(mPtr->members, [](HSQUIRRELVM vm, auto id, auto & m) -> SQRESULT {
            // The [id] is a dpp::snowflake which is basically a uint64_t so let's leave that as is
            sq_pushinteger(vm, static_cast< SQInteger >(id));
            // Wrap the dpp::guild_member type into a DpGuildMember instance
            ClassType< DpGuildMember >::PushInstance(vm, new DpGuildMember(m));
            // The elements are now on the stack and can be inserted in the table
            return SQ_OK;
        });
        // Did anything fail?
        if (SQ_FAILED(r))
        {
            STHROWF("Unable to convert [members] container");
        }
        // Return the resulted table
        return t;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a list of members in voice channels in the guild.
    */
    SQMOD_NODISCARD Table GetVoiceMembers() const
    {
        Table t(SqVM(), static_cast< SQInteger >(Valid().voice_members.size()));
        // Attempt to convert the [voice_members] associative container into a script table
        const auto r = t.InsertFromMapWith(mPtr->voice_members, [](HSQUIRRELVM vm, auto id, auto & m) -> SQRESULT {
            // The [id] is a dpp::snowflake which is basically a uint64_t so let's leave that as is
            sq_pushinteger(vm, static_cast< SQInteger >(id));
            // Wrap the dpp::guild_member type into a DpGuildMember instance
            ClassType< DpVoiceState >::PushInstance(vm, new DpVoiceState(m));
            // The elements are now on the stack and can be inserted in the table
            return SQ_OK;
        });
        // Did anything fail?
        if (SQ_FAILED(r))
        {
            STHROWF("Unable to convert [voice_members] container");
        }
        // Return the resulted table
        return t;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a list of emojis.
    */
    SQMOD_NODISCARD Array GetEmojis() const { return Array(SqVM()).Reserve(static_cast< SQInteger >(Valid().emojis.size())).AppendFromVector(Valid().emojis); }
    /* --------------------------------------------------------------------------------------------
     * Build a JSON string from this object.
    */
    SQMOD_NODISCARD std::string BuildJSON() const { return Valid().build_json(); }
    /* --------------------------------------------------------------------------------------------
     * Build a JSON string from this object. If [with_id] is True then ID is to be included in the JSON.
    */
    SQMOD_NODISCARD std::string BuildJSON_(bool with_id) const { return Valid().build_json(with_id); }
    /* --------------------------------------------------------------------------------------------
     * Rehash members map.
    */
    SQMOD_NODISCARD DpGuild & RehashMembers() { Valid().rehash_members(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Connect to a voice channel another guild member is in.
     * Returns true if the user specified is in a voice channel, false if they aren't.
    */
    SQMOD_NODISCARD bool ConnectMemberVoice(SQInteger user_id) const { return Valid().connect_member_voice(user_id); }
    /* --------------------------------------------------------------------------------------------
     * Is a large server (>250 users).
    */
    SQMOD_NODISCARD bool IsLarge() const { return Valid().is_large(); }
    /* --------------------------------------------------------------------------------------------
     * Is unavailable due to outage (most other fields will be blank or outdated).
    */
    SQMOD_NODISCARD bool IsUnavailable() const { return Valid().is_unavailable(); }
    /* --------------------------------------------------------------------------------------------
     * Widget is enabled for this server.
    */
    SQMOD_NODISCARD bool WidgetEnabled() const { return Valid().widget_enabled(); }
    /* --------------------------------------------------------------------------------------------
     * Guild has an invite splash.
    */
    SQMOD_NODISCARD bool HasInviteSplash() const { return Valid().has_invite_splash(); }
    /* --------------------------------------------------------------------------------------------
     * Guild has VIP regions.
    */
    SQMOD_NODISCARD bool HasVipRegions() const { return Valid().has_vip_regions(); }
    /* --------------------------------------------------------------------------------------------
     * Guild can have a vanity URL.
    */
    SQMOD_NODISCARD bool HasVanityURL() const { return Valid().has_vanity_url(); }
    /* --------------------------------------------------------------------------------------------
     * Guild is a verified server.
    */
    SQMOD_NODISCARD bool IsVerified() const { return Valid().is_verified(); }
    /* --------------------------------------------------------------------------------------------
     * Guild is a discord partner server.
    */
    SQMOD_NODISCARD bool IsPartnered() const { return Valid().is_partnered(); }
    /* --------------------------------------------------------------------------------------------
     * Guild has enabled community.
    */
    SQMOD_NODISCARD bool IsCommunity() const { return Valid().is_community(); }
    /* --------------------------------------------------------------------------------------------
     * Guild has enabled commerce channels.
    */
    SQMOD_NODISCARD bool HasCommerce() const { return Valid().has_commerce(); }
    /* --------------------------------------------------------------------------------------------
     * Guild has news channel.
    */
    SQMOD_NODISCARD bool HasNews() const { return Valid().has_news(); }
    /* --------------------------------------------------------------------------------------------
     * Guild is discoverable.
    */
    SQMOD_NODISCARD bool IsDiscoverable() const { return Valid().is_discoverable(); }
    /* --------------------------------------------------------------------------------------------
     * Guild is featureable.
    */
    SQMOD_NODISCARD bool IsFeatureable() const { return Valid().is_featureable(); }
    /* --------------------------------------------------------------------------------------------
     * Guild is allowed an animated icon.
    */
    SQMOD_NODISCARD bool HasAnimatedIcon() const { return Valid().has_animated_icon(); }
    /* --------------------------------------------------------------------------------------------
     * Guild has a banner image.
    */
    SQMOD_NODISCARD bool BasBanner() const { return Valid().has_banner(); }
    /* --------------------------------------------------------------------------------------------
     * Guild has enabled welcome screen.
    */
    SQMOD_NODISCARD bool WelcomeScreenEnabled() const { return Valid().is_welcome_screen_enabled(); }
    /* --------------------------------------------------------------------------------------------
     * Guild has enabled membership screening.
    */
    SQMOD_NODISCARD bool HasMemberVerificationGate() const { return Valid().has_member_verification_gate(); }
    /* --------------------------------------------------------------------------------------------
     * Guild has preview enabled.
    */
    SQMOD_NODISCARD bool IsPreviewEnabled() const { return Valid().is_preview_enabled(); }
    /* --------------------------------------------------------------------------------------------
     * Server icon is actually an animated gif.
    */
    SQMOD_NODISCARD bool HasAnimatedIconHash() const { return Valid().has_animated_icon_hash(); }
};

} // Namespace:: SqMod
