#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

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
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    DpCachePolicy(SQInteger user, SQInteger emoji) noexcept
        : mUserPolicy(user), mEmojiPolicy(emoji), mRolePolicy(dpp::cp_aggressive)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    DpCachePolicy(SQInteger user, SQInteger emoji, SQInteger role) noexcept
        : mUserPolicy(user), mEmojiPolicy(emoji), mRolePolicy(role)
    {
    }

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
struct DpActivity : public dpp::activity
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpActivity()
        : dpp::activity()
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    DpActivity(SQInteger type, StackStrF & name, StackStrF & state, StackStrF & url)
        : dpp::activity(static_cast< dpp::activity_type >(type), name.ToStr(), state.ToStr(), url.ToStr())
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit DpActivity(const dpp::activity & o)
        : dpp::activity(o)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the activity.
    */
    SQMOD_NODISCARD const std::string & GetName() const noexcept
    {
        return dpp::activity::name;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the name of the activity.
    */
    void SetName(StackStrF & name)
    {
        dpp::activity::name = name.ToStr();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the name of the activity.
    */
    DpActivity & ApplyName(StackStrF & name)
    {
        SetName(name);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the state of the activity.
    */
    SQMOD_NODISCARD const std::string & GetState() const noexcept
    {
        return dpp::activity::state;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the state of the activity.
    */
    void SetState(StackStrF & state)
    {
        dpp::activity::state = state.ToStr();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the state of the activity.
    */
    DpActivity & ApplyState(StackStrF & state)
    {
        SetState(state);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the url of the activity.
    */
    SQMOD_NODISCARD const std::string & GetURL() const noexcept
    {
        return dpp::activity::url;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the url of the activity.
    */
    void SetURL(StackStrF & url)
    {
        dpp::activity::url = url.ToStr();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the url of the activity.
    */
    DpActivity & ApplyURL(StackStrF & url)
    {
        SetURL(url);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the type of the activity.
    */
    SQMOD_NODISCARD SQInteger GetType() const noexcept
    {
        return static_cast< SQInteger >(dpp::activity::type);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the type of the activity.
    */
    void SetType(SQInteger s)
    {
        dpp::activity::type = static_cast< dpp::activity_type >(s);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the type of the activity.
    */
    DpActivity & ApplyType(SQInteger s)
    {
        SetType(s);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve when the activity was created.
    */
    SQMOD_NODISCARD SQInteger GetCreatedAt() const noexcept
    {
        return static_cast< SQInteger >(std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::from_time_t(dpp::activity::created_at).time_since_epoch()).count());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify when the activity was created.
    */
    void SetCreatedAt(SQInteger s)
    {
        dpp::activity::created_at = std::chrono::system_clock::to_time_t(std::chrono::time_point< std::chrono::system_clock >{std::chrono::seconds{s}});
    }

    /* --------------------------------------------------------------------------------------------
     * Modify when the activity was created.
    */
    DpActivity & ApplyCreatedAt(SQInteger s)
    {
        SetCreatedAt(s);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve when the activity was started.
    */
    SQMOD_NODISCARD SQInteger GetStart() const noexcept
    {
        return static_cast< SQInteger >(std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::from_time_t(dpp::activity::start).time_since_epoch()).count());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify when the activity was started.
    */
    void SetStart(SQInteger s)
    {
        dpp::activity::start = std::chrono::system_clock::to_time_t(std::chrono::time_point< std::chrono::system_clock >{std::chrono::seconds{s}});
    }

    /* --------------------------------------------------------------------------------------------
     * Modify when the activity was started.
    */
    DpActivity & ApplyStart(SQInteger s)
    {
        SetStart(s);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve when the activity was stopped.
    */
    SQMOD_NODISCARD SQInteger GetEnd() const noexcept
    {
        return static_cast< SQInteger >(std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::from_time_t(dpp::activity::end).time_since_epoch()).count());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify when the activity was stopped.
    */
    void SetEnd(SQInteger s)
    {
        dpp::activity::end = std::chrono::system_clock::to_time_t(std::chrono::time_point< std::chrono::system_clock >{std::chrono::seconds{s}});
    }

    /* --------------------------------------------------------------------------------------------
     * Modify when the activity was stopped.
    */
    DpActivity & ApplyEnd(SQInteger s)
    {
        SetEnd(s);
        return *this;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Represents user presence, e.g. what game they are playing and if they are online.
*/
struct DpPresence : public dpp::presence
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpPresence()
        : dpp::presence()
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the user that the presence applies to.
    */
    SQMOD_NODISCARD dpp::snowflake GetUserID() const noexcept
    {
        return dpp::presence::user_id;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the user that the presence applies to.
    */
    void SetUserID(dpp::snowflake id)
    {
        dpp::presence::user_id = id;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the user that the presence applies to.
    */
    DpPresence & ApplyUserID(dpp::snowflake id)
    {
        SetUserID(id);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild that the presence applies to.
    */
    SQMOD_NODISCARD dpp::snowflake GetGuildID() const noexcept
    {
        return dpp::presence::guild_id;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the guild that the presence applies to.
    */
    void SetGuildID(dpp::snowflake id)
    {
        dpp::presence::guild_id = id;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the guild that the presence applies to.
    */
    DpPresence & ApplyGuildID(dpp::snowflake id)
    {
        SetGuildID(id);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the presence bit-mask.
    */
    SQMOD_NODISCARD SQInteger GetFlags() const noexcept
    {
        return static_cast< SQInteger >(dpp::presence::flags);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the presence bit-mask.
    */
    void SetFlags(SQInteger f)
    {
        dpp::presence::flags = static_cast< uint8_t >(f);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the presence bit-mask.
    */
    DpPresence & ApplyFlags(SQInteger f)
    {
        SetFlags(f);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of activities.
    */
    SQMOD_NODISCARD SQInteger ActivityCount() const
    {
        return static_cast< SQInteger >(dpp::presence::activities.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Add a new activity.
    */
    DpPresence & AddActivity(const DpActivity & a)
    {
        dpp::presence::activities.push_back(a);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all activities.
    */
    DpPresence & EachActivity(Function & fn)
    {
        for (const auto & a : dpp::presence::activities)
        {
            fn.Execute(a);
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of activities.
    */
    DpPresence & ClearActivities(const DpActivity & a)
    {
        dpp::presence::activities.clear();
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Filter activities.
    */
    DpPresence & FilterActivities(Function & fn)
    {
        std::vector< dpp::activity > list;
        list.reserve(dpp::presence::activities.size());
        for (const auto & a : dpp::presence::activities)
        {
            auto ret = fn.Eval(a);
            // (null || true) == keep & false == skip
            if (!ret.IsNull() || !ret.template Cast< bool >())
            {
                list.push_back(a);
            }
        }
        dpp::presence::activities.swap(list);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Build JSON string from this object.
    */
    SQMOD_NODISCARD std::string BuildJSON() const
    {
        return dpp::presence::build_json();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the users status on desktop.
    */
    SQMOD_NODISCARD SQInteger GetDesktopStatus() const noexcept
    {
        return static_cast< SQInteger >(dpp::presence::desktop_status());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the user's status on web.
    */
    SQMOD_NODISCARD SQInteger GetWebStatus() const noexcept
    {
        return static_cast< SQInteger >(dpp::presence::web_status());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the user's status on mobile.
    */
    SQMOD_NODISCARD SQInteger GetMobileStatus() const noexcept
    {
        return static_cast< SQInteger >(dpp::presence::mobile_status());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the user's status as shown to other users.
    */
    SQMOD_NODISCARD SQInteger GetStatus() const noexcept
    {
        return static_cast< SQInteger >(dpp::presence::status());
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
    {
    }

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
        if (this != &o)
        {
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
     * Check whether a valid instance is managed.
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
     * Whether the referenced pointer is owned.
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
    {
    }

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
        if (this != &o)
        {
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
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }

};

} // Namespace:: SqMod
