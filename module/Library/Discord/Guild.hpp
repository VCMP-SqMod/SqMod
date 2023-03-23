#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <dpp/dpp.h>

// ------------------------------------------------------------------------------------------------
#include <chrono>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

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
     * Whether the referenced pointer is owned.
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
    ~DpGuildMember() noexcept { Cleanup(); }
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
     * Build json representation of the object.
    */
    SQMOD_NODISCARD std::string BuildJSON() const { return Valid().build_json(); }
    SQMOD_NODISCARD std::string BuildJSON_(bool with_id) const { return Valid().build_json(with_id); }
    /* --------------------------------------------------------------------------------------------
     * Check if this member is equal to another member object.
    */
    SQMOD_NODISCARD SQInteger SqCmp(const DpGuildMember & o) const
    {
        if (Valid() == o.Valid()) {
            return 0;
        // Nonsense...
        } else if (Valid().user_id > o.Valid().user_id) {
            return 1;
        } else {
            return -1;
        }
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the nickname, or empty string if they don't have a nickname on this guild.
    */
    SQMOD_NODISCARD const std::string & GetNickname() const { return Valid().nickname; }
    /* --------------------------------------------------------------------------------------------
     * Modify the nickname.
    */
    void SetNickname(StackStrF & name) const { Valid().set_nickname(name.ToStr()); }
    /* --------------------------------------------------------------------------------------------
     * Modify the nickname.
    */
    DpGuildMember & ApplyNickname(StackStrF & name) { SetNickname(name); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the list of roles this user has on this guild.
    */
    SQMOD_NODISCARD Array GetRoles() const
    {
        return Array(SqVM()).Reserve(static_cast< SQInteger >(Valid().roles.size()))
                            .AppendFromVector(Valid().roles);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of roles.
    */
    SQMOD_NODISCARD SQInteger RolesCount() const { return static_cast< SQInteger >(Valid().roles.size()); }
    /* --------------------------------------------------------------------------------------------
     * Add a new role.
    */
    DpGuildMember & AddRole(dpp::snowflake role) { Valid().roles.push_back(role); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Iterate all roles.
    */
    DpGuildMember & EachRole(Function & fn)
    {
        for (const auto & a : Valid().roles)
        {
            fn.Execute(a);
        }
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of roles.
    */
    DpGuildMember & ClearRoles(dpp::snowflake role) { Valid().roles.clear(); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Filter roles.
    */
    DpGuildMember & FilterRoles(Function & fn)
    {
        std::vector< dpp::snowflake > list;
        // Reserve memory in advance
        list.reserve(Valid().roles.size());
        // Process each role individually
        for (const auto & role : Valid().roles)
        {
            auto ret = fn.Eval(role);
            // (null || true) == keep & false == skip
            if (!ret.IsNull() || !ret.template Cast< bool >())
            {
                list.push_back(role); // Keep this role
            }
        }
        // Use filtered roles
        Valid().roles.swap(list);
        // Allow chaining
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild id.
    */
    SQMOD_NODISCARD dpp::snowflake GetGuildID() const { return Valid().guild_id; }
    /* --------------------------------------------------------------------------------------------
     * Modify the guild id.
    */
    void SetGuildID(dpp::snowflake id) { Valid().guild_id = id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the user id.
    */
    SQMOD_NODISCARD dpp::snowflake GetUserID() const { return Valid().user_id; }
    /* --------------------------------------------------------------------------------------------
     * Modify the guild id.
    */
    void SetUserID(dpp::snowflake id) { Valid().guild_id = id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the user avatar (per-server avatar is a nitro only feature).
    */
    SQMOD_NODISCARD const dpp::utility::iconhash & GetAvatar() const { return Valid().avatar; }
    /* --------------------------------------------------------------------------------------------
     * Modify the user avatar (per-server avatar is a nitro only feature).
    */
    void SetAvatar(const dpp::utility::iconhash & a) const { Valid().avatar = a; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the date and time when the time out will be removed; until then, they cannot interact with the guild.
    */
    SQMOD_NODISCARD SQInteger GetCommunicationDisabledUntil() const
    {
        return std::chrono::time_point_cast< std::chrono::seconds >(
                std::chrono::system_clock::from_time_t(Valid().communication_disabled_until)
            ).time_since_epoch().count();
    }
    /* --------------------------------------------------------------------------------------------
     * Assign a timestamp until communication is disabled.
    */
    void SetCommunicationDisabledUntil(SQInteger ts) const
    {
        Valid().set_communication_disabled_until(
            std::chrono::system_clock::to_time_t(std::chrono::time_point< std::chrono::system_clock >(std::chrono::seconds(ts)))
        );
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the date and time the user joined the guild.
    */
    SQMOD_NODISCARD SQInteger GetJoinedAt() const
    {
        return std::chrono::time_point_cast< std::chrono::seconds >(
                std::chrono::system_clock::from_time_t(Valid().joined_at)
            ).time_since_epoch().count();
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the date and time since the user is boosting.
    */
    SQMOD_NODISCARD SQInteger GetPremiumSince() const
    {
        return std::chrono::time_point_cast< std::chrono::seconds >(
                std::chrono::system_clock::from_time_t(Valid().premium_since)
            ).time_since_epoch().count();
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild flags. Built from the bitmask defined by SqDiscordGuildMemberFlags.
    */
    SQMOD_NODISCARD SQInteger GetFlags() const { return static_cast< SQInteger >(Valid().flags); }
    /* --------------------------------------------------------------------------------------------
     * Modify the guild flags.
    */
    void SetFlags(SQInteger f) const { Valid().flags = static_cast< uint8_t >(f); }
    /* --------------------------------------------------------------------------------------------
     * Modify the guild flags.
    */
    DpGuildMember & ApplyFlags(SQInteger f) { SetFlags(f); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is in time-out (communication disabled).
    */
    SQMOD_NODISCARD bool IsCommunicationDisabled() const { return Valid().is_communication_disabled(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is deafened.
    */
    SQMOD_NODISCARD bool GetDeaf() const { return Valid().is_deaf(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is deafened.
    */
    void SetDeaf(bool is_deafened) const { Valid().set_deaf(is_deafened); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is muted.
    */
    SQMOD_NODISCARD bool GetMuted() const { return Valid().is_muted(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is muted.
    */
    void SetMuted(bool is_muted) const { Valid().set_mute(is_muted); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is pending verification by membership screening.
    */
    SQMOD_NODISCARD bool IsPending() const { return Valid().is_pending(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the the user's per-guild custom avatar is animated.
    */
    SQMOD_NODISCARD bool HasAnimatedGuildAvatar() const { return Valid().has_animated_guild_avatar(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the the user's per-guild custom avatar is animated.
    */
    SQMOD_NODISCARD std::string GetAvatarURL(SQInteger size, SQInteger format, bool animated) const
    { return Valid().get_avatar_url(static_cast< uint16_t >(size), static_cast< dpp::image_type >(format), animated); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a ping/mention for the user by nickname.
    */
    SQMOD_NODISCARD std::string GetMention() const { return Valid().get_mention(); }
};

/* ------------------------------------------------------------------------------------------------
 * Represents a guild on Discord (AKA a server)
*/
struct DpGuild
{
    using Ptr = std::unique_ptr< dpp::guild >;
    /* --------------------------------------------------------------------------------------------
     * Referenced guild instance.
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
    ~DpGuild() noexcept { Cleanup(); }
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord guild handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Build json representation of the object.
    */
    SQMOD_NODISCARD std::string BuildJSON() const { return Valid().build_json(); }
    SQMOD_NODISCARD std::string BuildJSON_(bool with_id) const { return Valid().build_json(with_id); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the guild name. Min length: 2, Max length: 100 (not including leading/trailing spaces)
    */
    SQMOD_NODISCARD const std::string & GetName() const { return Valid().name; }
    /* --------------------------------------------------------------------------------------------
     * Modify the guild name. Min length: 2, Max length: 100 (not including leading/trailing spaces)
    */
    void SetName(StackStrF & name) const { Valid().set_name(name.ToStr()); }
    /* --------------------------------------------------------------------------------------------
     * Modify the guild name. Min length: 2, Max length: 100 (not including leading/trailing spaces)
    */
    DpGuild & ApplyName(StackStrF & name) { SetName(name); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the server description.
    */
    SQMOD_NODISCARD const std::string & GetDescription() const { return Valid().description; }
    /* --------------------------------------------------------------------------------------------
     * Modify the server description.
    */
    void SetDescription(StackStrF & description) const { Valid().description = description.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the server description.
    */
    DpGuild & ApplyDescription(StackStrF & description) { SetDescription(description); return *this; }
};

} // Namespace:: SqMod
