#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <dpp/dpp.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

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
     * Whether the referenced pointer is owned.
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
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpUser(const Ptr::element_type & o) noexcept
        : DpUser(new Ptr::element_type(o), true)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpUser(Ptr::element_type && o) noexcept
        : DpUser(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    {
    }
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
    ~DpUser() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpUser & operator=(const DpUser & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpUser & operator=(DpUser && o) noexcept
    {
        if (this != &o)
        {
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
        if (!mOwned && mPtr)
        {
            // Not our job, simply forget about it
            [[maybe_unused]] auto p = mPtr.release();
        }
        else
            mPtr.reset(); // We own this so delete the instance
    }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const
    {
        if (!mPtr)
            STHROWF("Invalid discord user handle");
    }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const
    {
        Validate();
        return *mPtr;
    }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast<bool>(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Check whether this represents an identified user.
    */
    SQMOD_NODISCARD bool IsIdentified() const { return false; }
    /* --------------------------------------------------------------------------------------------
     * Get the creation time of this object according to Discord.
    */
    SQMOD_NODISCARD SQFloat GetCreationTime() const { return Valid().get_creation_time(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the user.
     */
    SQMOD_NODISCARD const std::string & GetName() const { return Valid().username; }
    /* --------------------------------------------------------------------------------------------
     * Modify the name of the user.
     */
    void SetName(StackStrF & name) const { Valid().username = name.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the name of the user.
     */
    DpUser & ApplyName(StackStrF & name)
    {
        SetName(name);
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the avatar hash of the user.
     */
    SQMOD_NODISCARD const dpp::utility::iconhash & GetAvatar() const { return Valid().avatar; }
    /* --------------------------------------------------------------------------------------------
     * Modify the avatar hash of the user.
     */
    void SetAvatar(dpp::utility::iconhash & avatar) const { Valid().avatar = avatar; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the flags for the user from SqDiscordUserFlags.
     */
    SQMOD_NODISCARD SQInteger GetFlags() const { return Valid().flags; }
    /* --------------------------------------------------------------------------------------------
     * Modify the flags for the user from SqDiscordUserFlags.
     */
    void SetFlags(SQInteger flags) const { Valid().flags = static_cast< uint32_t >(flags); }
    /* --------------------------------------------------------------------------------------------
     * Modify the flags for the user from SqDiscordUserFlags.
     */
    DpUser & ApplyFlags(SQInteger flags)
    {
        SetFlags(flags);
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the discriminator (aka tag), 4 digits usually displayed with leading zeroes for the user.
     * To print the discriminator with leading zeroes, use FormatUsername()
     */
    SQMOD_NODISCARD SQInteger GetDiscriminator() const { return Valid().discriminator; }
    /* --------------------------------------------------------------------------------------------
     * Modify the discriminator for the user.
     */
    void SetDiscriminator(SQInteger discriminator) const { Valid().discriminator = static_cast< uint16_t >(discriminator); }
    /* --------------------------------------------------------------------------------------------
     * Modify the discriminator for the user.
     */
    DpUser & ApplyDiscriminator(SQInteger discriminator)
    {
        SetDiscriminator(discriminator);
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the reference count of how many guilds this user is in.
     */
    SQMOD_NODISCARD SQInteger GetRefCount() const { return Valid().refcount; }
    /* --------------------------------------------------------------------------------------------
     * Modify the reference count of how many guilds this user is in.
     */
    void SetRefCount(SQInteger refcount) const { Valid().refcount = static_cast< uint8_t >(refcount); }
    /* --------------------------------------------------------------------------------------------
     * Build JSON string from this user.
    */
    SQMOD_NODISCARD std::string BuildJSON() const { return Valid().build_json(); }
    SQMOD_NODISCARD std::string BuildJSON_(bool with_id) const { return Valid().build_json(with_id); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the avatar url of the user.
     * If the user doesn't have an avatar, the default user avatar url is returned which is always in `png` format!
     * Size of the avatar is in pixels. It can be any power of two between 16 and 4096.
     * Check SqDiscordImageType enum for available image formats.
    */
    SQMOD_NODISCARD std::string GetAvatarUrl() const { return Valid().get_avatar_url(); }
    SQMOD_NODISCARD std::string GetAvatarUrl_1(uint16_t size) const { return Valid().get_avatar_url(size); }
    SQMOD_NODISCARD std::string GetAvatarUrl_2(uint16_t size, SQInteger format) const { return Valid().get_avatar_url(size, static_cast< dpp::image_type >(format)); }
    SQMOD_NODISCARD std::string GetAvatarUrl_3(uint16_t size, SQInteger format, bool prefer_animated) const { return Valid().get_avatar_url(size, static_cast< dpp::image_type >(format), prefer_animated); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the default avatar url of the user. This is calculated by the discriminator.
    */
    SQMOD_NODISCARD std::string GetDefaultAvatarUrl() const { return Valid().get_default_avatar_url(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a ping/mention for the user.
    */
    SQMOD_NODISCARD std::string GetMention() const { return Valid().get_mention(); }
    /* --------------------------------------------------------------------------------------------
     * Format a username into user\#discriminator. For example Brain#0001
    */
    SQMOD_NODISCARD std::string FormatUsername() const { return Valid().format_username(); }
    /* --------------------------------------------------------------------------------------------
     * Helper methods for flags.
    */
    SQMOD_NODISCARD bool IsActiveDeveloper() const { return Valid().is_active_developer(); }
    SQMOD_NODISCARD bool IsBot() const { return Valid().is_bot(); }
    SQMOD_NODISCARD bool IsSystem() const { return Valid().is_system(); }
    SQMOD_NODISCARD bool IsMfaEnabled() const { return Valid().is_mfa_enabled(); }
    SQMOD_NODISCARD bool IsVerified() const { return Valid().is_verified(); }
    SQMOD_NODISCARD bool HasNitroFull() const { return Valid().has_nitro_full(); }
    SQMOD_NODISCARD bool HasNitroClassic() const { return Valid().has_nitro_classic(); }
    SQMOD_NODISCARD bool HasNitroBasic() const { return Valid().has_nitro_basic(); }
    SQMOD_NODISCARD bool IsDiscordEmployee() const { return Valid().is_discord_employee(); }
    SQMOD_NODISCARD bool IsPartneredOwner() const { return Valid().is_partnered_owner(); }
    SQMOD_NODISCARD bool HasHypesquadEvents() const { return Valid().has_hypesquad_events(); }
    SQMOD_NODISCARD bool IsBughunter1() const { return Valid().is_bughunter_1(); }
    SQMOD_NODISCARD bool IsHouseBravery() const { return Valid().is_house_bravery(); }
    SQMOD_NODISCARD bool IsHouseBrilliance() const { return Valid().is_house_brilliance(); }
    SQMOD_NODISCARD bool IsHouseBalance() const { return Valid().is_house_balance(); }
    SQMOD_NODISCARD bool IsEarlySupporter() const { return Valid().is_early_supporter(); }
    SQMOD_NODISCARD bool IsTeamUser() const { return Valid().is_team_user(); }
    SQMOD_NODISCARD bool IsBughunter2() const { return Valid().is_bughunter_2(); }
    SQMOD_NODISCARD bool IsVerifiedBot() const { return Valid().is_verified_bot(); }
    SQMOD_NODISCARD bool IsVerifiedBotDev() const { return Valid().is_verified_bot_dev(); }
    SQMOD_NODISCARD bool IsCertifiedModerator() const { return Valid().is_certified_moderator(); }
    SQMOD_NODISCARD bool IsBotHttpInteractions() const { return Valid().is_bot_http_interactions(); }
    SQMOD_NODISCARD bool HasAnimatedIcon() const { return Valid().has_animated_icon(); }
};

/* ------------------------------------------------------------------------------------------------
 * A user with additional fields only available via the oauth2 identify scope.
 * These are not included in DpUser as additional scopes are needed to fetch them which bots do not normally have.
*/
struct DpUserIdentifier
{
    using Ptr = std::unique_ptr< dpp::user_identified >;
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
    DpUserIdentifier() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpUserIdentifier(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpUserIdentifier(const Ptr::element_type & o) noexcept
        : DpUserIdentifier(new Ptr::element_type(o), true)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpUserIdentifier(Ptr::element_type && o) noexcept
        : DpUserIdentifier(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpUserIdentifier(const DpUserIdentifier & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpUserIdentifier(DpUserIdentifier && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpUserIdentifier() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpUserIdentifier & operator=(const DpUserIdentifier & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpUserIdentifier & operator=(DpUserIdentifier && o) noexcept
    {
        if (this != &o)
        {
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
        if (!mOwned && mPtr)
        {
            // Not our job, simply forget about it
            [[maybe_unused]] auto p = mPtr.release();
        }
        else
            mPtr.reset(); // We own this so delete the instance
    }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const
    {
        if (!mPtr)
            STHROWF("Invalid discord user handle");
    }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const
    {
        Validate();
        return *mPtr;
    }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast<bool>(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Check whether this represents an identified user.
    */
    SQMOD_NODISCARD bool IsIdentified() const { return false; }
    /* --------------------------------------------------------------------------------------------
     * Get the creation time of this object according to Discord.
    */
    SQMOD_NODISCARD SQFloat GetCreationTime() const { return Valid().get_creation_time(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the user.
     */
    SQMOD_NODISCARD const std::string & GetName() const { return Valid().username; }
    /* --------------------------------------------------------------------------------------------
     * Modify the name of the user.
     */
    void SetName(StackStrF & name) const { Valid().username = name.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the name of the user.
     */
    DpUserIdentifier & ApplyName(StackStrF & name)
    {
        SetName(name);
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the avatar hash of the user.
     */
    SQMOD_NODISCARD const dpp::utility::iconhash & GetAvatar() const { return Valid().avatar; }
    /* --------------------------------------------------------------------------------------------
     * Modify the avatar hash of the user.
     */
    void SetAvatar(dpp::utility::iconhash & avatar) const { Valid().avatar = avatar; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the flags for the user from SqDiscordUserFlags.
     */
    SQMOD_NODISCARD SQInteger GetFlags() const { return Valid().flags; }
    /* --------------------------------------------------------------------------------------------
     * Modify the flags for the user from SqDiscordUserFlags.
     */
    void SetFlags(SQInteger flags) const { Valid().flags = static_cast< uint32_t >(flags); }
    /* --------------------------------------------------------------------------------------------
     * Modify the flags for the user from SqDiscordUserFlags.
     */
    DpUserIdentifier & ApplyFlags(SQInteger flags)
    {
        SetFlags(flags);
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the discriminator (aka tag), 4 digits usually displayed with leading zeroes for the user.
     * To print the discriminator with leading zeroes, use FormatUsername()
     */
    SQMOD_NODISCARD SQInteger GetDiscriminator() const { return Valid().discriminator; }
    /* --------------------------------------------------------------------------------------------
     * Modify the discriminator for the user.
     */
    void SetDiscriminator(SQInteger discriminator) const { Valid().discriminator = static_cast< uint16_t >(discriminator); }
    /* --------------------------------------------------------------------------------------------
     * Modify the discriminator for the user.
     */
    DpUserIdentifier & ApplyDiscriminator(SQInteger discriminator)
    {
        SetDiscriminator(discriminator);
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the reference count of how many guilds this user is in.
     */
    SQMOD_NODISCARD SQInteger GetRefCount() const { return Valid().refcount; }
    /* --------------------------------------------------------------------------------------------
     * Modify the reference count of how many guilds this user is in.
     */
    void SetRefCount(SQInteger refcount) const { Valid().refcount = static_cast< uint8_t >(refcount); }
    /* --------------------------------------------------------------------------------------------
     * Build JSON string from this user.
    */
    SQMOD_NODISCARD std::string BuildJSON() const { return Valid().build_json(); }
    SQMOD_NODISCARD std::string BuildJSON_(bool with_id) const { return Valid().build_json(with_id); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the avatar url of the user.
     * If the user doesn't have an avatar, the default user avatar url is returned which is always in `png` format!
     * Size of the avatar is in pixels. It can be any power of two between 16 and 4096.
     * Check SqDiscordImageType enum for available image formats.
    */
    SQMOD_NODISCARD std::string GetAvatarUrl() const { return Valid().get_avatar_url(); }
    SQMOD_NODISCARD std::string GetAvatarUrl_1(uint16_t size) const { return Valid().get_avatar_url(size); }
    SQMOD_NODISCARD std::string GetAvatarUrl_2(uint16_t size, SQInteger format) const { return Valid().get_avatar_url(size, static_cast< dpp::image_type >(format)); }
    SQMOD_NODISCARD std::string GetAvatarUrl_3(uint16_t size, SQInteger format, bool prefer_animated) const { return Valid().get_avatar_url(size, static_cast< dpp::image_type >(format), prefer_animated); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the default avatar url of the user. This is calculated by the discriminator.
    */
    SQMOD_NODISCARD std::string GetDefaultAvatarUrl() const { return Valid().get_default_avatar_url(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a ping/mention for the user.
    */
    SQMOD_NODISCARD std::string GetMention() const { return Valid().get_mention(); }
    /* --------------------------------------------------------------------------------------------
     * Format a username into user\#discriminator. For example Brain#0001
    */
    SQMOD_NODISCARD std::string FormatUsername() const { return Valid().format_username(); }
    /* --------------------------------------------------------------------------------------------
     * Helper methods for flags.
    */
    SQMOD_NODISCARD bool IsActiveDeveloper() const { return Valid().is_active_developer(); }
    SQMOD_NODISCARD bool IsBot() const { return Valid().is_bot(); }
    SQMOD_NODISCARD bool IsSystem() const { return Valid().is_system(); }
    SQMOD_NODISCARD bool IsMfaEnabled() const { return Valid().is_mfa_enabled(); }
    SQMOD_NODISCARD bool IsVerified() const { return Valid().is_verified(); }
    SQMOD_NODISCARD bool HasNitroFull() const { return Valid().has_nitro_full(); }
    SQMOD_NODISCARD bool HasNitroClassic() const { return Valid().has_nitro_classic(); }
    SQMOD_NODISCARD bool HasNitroBasic() const { return Valid().has_nitro_basic(); }
    SQMOD_NODISCARD bool IsDiscordEmployee() const { return Valid().is_discord_employee(); }
    SQMOD_NODISCARD bool IsPartneredOwner() const { return Valid().is_partnered_owner(); }
    SQMOD_NODISCARD bool HasHypesquadEvents() const { return Valid().has_hypesquad_events(); }
    SQMOD_NODISCARD bool IsBughunter1() const { return Valid().is_bughunter_1(); }
    SQMOD_NODISCARD bool IsHouseBravery() const { return Valid().is_house_bravery(); }
    SQMOD_NODISCARD bool IsHouseBrilliance() const { return Valid().is_house_brilliance(); }
    SQMOD_NODISCARD bool IsHouseBalance() const { return Valid().is_house_balance(); }
    SQMOD_NODISCARD bool IsEarlySupporter() const { return Valid().is_early_supporter(); }
    SQMOD_NODISCARD bool IsTeamUser() const { return Valid().is_team_user(); }
    SQMOD_NODISCARD bool IsBughunter2() const { return Valid().is_bughunter_2(); }
    SQMOD_NODISCARD bool IsVerifiedBot() const { return Valid().is_verified_bot(); }
    SQMOD_NODISCARD bool IsVerifiedBotDev() const { return Valid().is_verified_bot_dev(); }
    SQMOD_NODISCARD bool IsCertifiedModerator() const { return Valid().is_certified_moderator(); }
    SQMOD_NODISCARD bool IsBotHttpInteractions() const { return Valid().is_bot_http_interactions(); }
    SQMOD_NODISCARD bool HasAnimatedIcon() const { return Valid().has_animated_icon(); }
    SQMOD_NODISCARD bool HasAnimatedBanner() const { return Valid().has_animated_banner(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the locale of the user.
     */
    SQMOD_NODISCARD const std::string & GetLocale() const { return Valid().locale; }
    /* --------------------------------------------------------------------------------------------
     * Modify the locale of the user.
     */
    void SetLocale(StackStrF & locale) const { Valid().locale = locale.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the locale of the user.
     */
    DpUserIdentifier & ApplyLocale(StackStrF & locale)
    {
        SetLocale(locale);
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the locale of the user.
     */
    SQMOD_NODISCARD const std::string & GetEmail() const { return Valid().email; }
    /* --------------------------------------------------------------------------------------------
     * Modify the email of the user.
     */
    void SetEmail(StackStrF & email) const { Valid().email = email.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the email of the user.
     */
    DpUserIdentifier & ApplyEmail(StackStrF & email)
    {
        SetEmail(email);
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the banner hash of the user.
     */
    SQMOD_NODISCARD const dpp::utility::iconhash & GetBanner() const { return Valid().banner; }
    /* --------------------------------------------------------------------------------------------
     * Modify the banner hash of the user.
     */
    void SetBanner(dpp::utility::iconhash & banner) const { Valid().banner = banner; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the banner color encoded as an integer representation of hexadecimal color code for the user.
     */
    SQMOD_NODISCARD SQInteger GetAccentColor() const { return Valid().accent_color; }
    /* --------------------------------------------------------------------------------------------
     * Modify the banner color encoded as an integer representation of hexadecimal color code for the user.
     */
    void SetAccentColor(SQInteger color) const { Valid().accent_color = static_cast< uint32_t >(color); }
    /* --------------------------------------------------------------------------------------------
     * Modify the banner color encoded as an integer representation of hexadecimal color code for the user.
     */
    DpUserIdentifier & ApplyAccentColor(SQInteger color)
    {
        SetAccentColor(color);
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve whether the email on this account has been verified.
     */
    SQMOD_NODISCARD bool GetVerified() const { return Valid().verified; }
    /* --------------------------------------------------------------------------------------------
     * Modify whether the email on this account has been verified.
     */
    void SetVerified(bool verified) const { Valid().verified = verified; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the banner url of the user, if they have one, otherwise returns an empty string.
     * Size of the avatar is in pixels. It can be any power of two between 16 and 4096.
     * Check SqDiscordImageType enum for available image formats.
    */
    SQMOD_NODISCARD std::string GetBannerUrl() const { return Valid().get_banner_url(); }
    SQMOD_NODISCARD std::string GetBannerUrl_1(uint16_t size) const { return Valid().get_banner_url(size); }
    SQMOD_NODISCARD std::string GetBannerUrl_2(uint16_t size, SQInteger format) const { return Valid().get_banner_url(size, static_cast< dpp::image_type >(format)); }
    SQMOD_NODISCARD std::string GetBannerUrl_3(uint16_t size, SQInteger format, bool prefer_animated) const { return Valid().get_banner_url(size, static_cast< dpp::image_type >(format), prefer_animated); }
};

} // Namespace:: SqMod
