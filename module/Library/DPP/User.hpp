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
     * Referenced user instance.
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
     * Check whether a valid instance is managed.
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
     * Check whether the user is a bot.
    */
    SQMOD_NODISCARD bool IsBot() const { return Valid().is_bot(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is a system user (Clyde).
    */
    SQMOD_NODISCARD bool IsSystem() const { return Valid().is_system(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user has multi-factor authentication enabled.
    */
    SQMOD_NODISCARD bool IsMfaEnabled() const { return Valid().is_mfa_enabled(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user has verified account.
    */
    SQMOD_NODISCARD bool IsVerified() const { return Valid().is_verified(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user has full nitro.
    */
    SQMOD_NODISCARD bool HasNitroFull() const { return Valid().has_nitro_full(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user has nitro classic.
    */
    SQMOD_NODISCARD bool HasNitroClassic() const { return Valid().has_nitro_classic(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is a discord employee.
    */
    SQMOD_NODISCARD bool IsDiscordEmployee() const { return Valid().is_discord_employee(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user owns a partnered server.
    */
    SQMOD_NODISCARD bool IsPartneredOwner() const { return Valid().is_partnered_owner(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user has hype-squad events.
    */
    SQMOD_NODISCARD bool HasHypesquadEvents() const { return Valid().has_hypesquad_events(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user has the bug-hunter level 1 badge.
    */
    SQMOD_NODISCARD bool IsBughunter1() const { return Valid().is_bughunter_1(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is in house bravery.
    */
    SQMOD_NODISCARD bool IsHouseBravery() const { return Valid().is_house_bravery(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is in house brilliance.
    */
    SQMOD_NODISCARD bool IsHouseBrilliance() const { return Valid().is_house_brilliance(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is in house balance.
    */
    SQMOD_NODISCARD bool IsHouseBalanace() const { return Valid().is_house_balanace(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is an early supporter.
    */
    SQMOD_NODISCARD bool IsEarlySupporter() const { return Valid().is_early_supporter(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is a team user.
    */
    SQMOD_NODISCARD bool IsTeamUser() const { return Valid().is_team_user(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user has the bug-hunter level 2 badge.
    */
    SQMOD_NODISCARD bool IsBughunter2() const { return Valid().is_bughunter_2(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user has the verified bot badge.
    */
    SQMOD_NODISCARD bool IsVerifiedBot() const { return Valid().is_verified_bot(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is an early verified bot developer.
    */
    SQMOD_NODISCARD bool IsVerifiedBotDev() const { return Valid().is_verified_bot_dev(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user is a certified moderator.
    */
    SQMOD_NODISCARD bool IsCertifiedDoderator() const { return Valid().is_certified_moderator(); }
    /* --------------------------------------------------------------------------------------------
     * Check whether the user has an animated icon.
    */
    SQMOD_NODISCARD bool HasAnimatedIcon() const { return Valid().has_animated_icon(); }
};

} // Namespace:: SqMod
