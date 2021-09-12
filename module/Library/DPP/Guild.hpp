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
     * Check whether a valid instance is managed.
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
     * Check whether the user is deafened.
    */
    SQMOD_NODISCARD bool IsDeaf() const { return Valid().is_deaf(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is muted.
    */
    SQMOD_NODISCARD bool IsMuted() const { return Valid().is_muted(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is pending verification by membership screening.
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
     * Check whether a valid instance is managed.
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
     * Check whether or not explicit content filtering is enable and what setting it is.
    */
    SQMOD_NODISCARD SQInteger GetExplicitContentFilter() const { return static_cast< SQInteger >(Valid().explicit_content_filter); }
    /* --------------------------------------------------------------------------------------------
     * Check whether multi factor authentication is required for moderators or not.
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
    SQMOD_NODISCARD Table GetMembers() const;
    /* --------------------------------------------------------------------------------------------
     * Retrieve a list of members in voice channels in the guild.
    */
    SQMOD_NODISCARD Table GetVoiceMembers() const;
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
