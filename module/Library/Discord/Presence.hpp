#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/Discord/Misc.hpp"

// ------------------------------------------------------------------------------------------------
#include <dpp/dpp.h>

// ------------------------------------------------------------------------------------------------
#include <chrono>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * An activity button is a custom button shown in the rich presence. Can be to join a game or whatever.
*/
struct DpActivityButton
{
    using Ptr = std::unique_ptr< dpp::activity_button >;
    /* --------------------------------------------------------------------------------------------
     * Referenced activity instance.
    */
    Ptr mPtr{nullptr};
    /* --------------------------------------------------------------------------------------------
     * Whether the referenced pointer is owned.
    */
    bool mOwned{false};
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpActivityButton() noexcept
        : DpActivityButton(new Ptr::element_type(), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpActivityButton(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit DpActivityButton(const Ptr::element_type & o) noexcept
        : DpActivityButton(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpActivityButton(Ptr::element_type && o) noexcept
        : DpActivityButton(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpActivityButton(const DpActivityButton & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpActivityButton(DpActivityButton && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpActivityButton() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpActivityButton & operator = (const DpActivityButton & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpActivityButton & operator = (DpActivityButton && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord activity button handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the text shown on the button (1-32 characters).
    */
    SQMOD_NODISCARD const std::string & GetLabel() const { return Valid().label; }
    /* --------------------------------------------------------------------------------------------
     * Modify the text shown on the button (1-32 characters).
    */
    void SetLabel(StackStrF & label) const { Valid().label = label.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the text shown on the button (1-32 characters).
    */
    DpActivityButton & ApplyLabel(StackStrF & label) { SetLabel(label); return *this; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the url opened when clicking the button (1-512 characters). It's may be empty.
    */
    SQMOD_NODISCARD const std::string & GetURL() const { return Valid().url; }
    /* --------------------------------------------------------------------------------------------
     * Modify the url opened when clicking the button (1-512 characters).
    */
    void SetURL(StackStrF & url) const { Valid().url = url.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the url opened when clicking the button (1-512 characters).
    */
    DpActivityButton & ApplyURL(StackStrF & url) { SetURL(url); return *this; }
};

/* ------------------------------------------------------------------------------------------------
 * An activity asset are the images and the hover text displayed in the rich presence.
*/
struct DpActivityAssets
{
    using Ptr = std::unique_ptr< dpp::activity_assets >;
    /* --------------------------------------------------------------------------------------------
     * Referenced activity instance.
    */
    Ptr mPtr{nullptr};
    /* --------------------------------------------------------------------------------------------
     * Whether the referenced pointer is owned.
    */
    bool mOwned{false};
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpActivityAssets() noexcept
        : DpActivityAssets(new Ptr::element_type(), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpActivityAssets(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit DpActivityAssets(const Ptr::element_type & o) noexcept
        : DpActivityAssets(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpActivityAssets(Ptr::element_type && o) noexcept
        : DpActivityAssets(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpActivityAssets(const DpActivityAssets & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpActivityAssets(DpActivityAssets && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpActivityAssets() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpActivityAssets & operator = (const DpActivityAssets & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpActivityAssets & operator = (DpActivityAssets && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord activity assets handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the large asset image which usually contain snowflake ID or prefixed image ID.
    */
    SQMOD_NODISCARD const std::string & GetLargeImage() const { return Valid().large_image; }
    /* --------------------------------------------------------------------------------------------
     * Modify the large asset image.
    */
    void SetLargeImage(StackStrF & large_image) const { Valid().large_image = large_image.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the large asset image.
    */
    DpActivityAssets & ApplyLargeImage(StackStrF & large_image) { SetLargeImage(large_image); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the text displayed when hovering over the large image of the activity.
    */
    SQMOD_NODISCARD const std::string & GetLargeText() const { return Valid().large_text; }
    /* --------------------------------------------------------------------------------------------
     * Modify the text displayed when hovering over the large image of the activity.
    */
    void SetLargeText(StackStrF & large_text) const { Valid().large_text = large_text.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the text displayed when hovering over the large image of the activity.
    */
    DpActivityAssets & ApplyLargeText(StackStrF & large_text) { SetLargeText(large_text); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the small asset image which usually contain snowflake ID or prefixed image ID.
    */
    SQMOD_NODISCARD const std::string & GetSmallImage() const { return Valid().small_image; }
    /* --------------------------------------------------------------------------------------------
     * Modify the small asset image.
    */
    void SetSmallImage(StackStrF & small_image) const { Valid().small_image = small_image.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the small asset image.
    */
    DpActivityAssets & ApplySmallImage(StackStrF & small_image) { SetSmallImage(small_image); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the text displayed when hovering over the small image of the activity.
    */
    SQMOD_NODISCARD const std::string & GetSmallText() const { return Valid().small_text; }
    /* --------------------------------------------------------------------------------------------
     * Modify the text displayed when hovering over the small image of the activity.
    */
    void SetSmallText(StackStrF & small_text) const { Valid().small_text = small_text.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the text displayed when hovering over the small image of the activity.
    */
    DpActivityAssets & ApplySmallText(StackStrF & small_text) { SetSmallText(small_text); return *this; }
};

/* ------------------------------------------------------------------------------------------------
 * Secrets for Rich Presence joining and spectating.
*/
struct DpActivitySecrets
{
    using Ptr = std::unique_ptr< dpp::activity_secrets >;
    /* --------------------------------------------------------------------------------------------
     * Referenced activity instance.
    */
    Ptr mPtr{nullptr};
    /* --------------------------------------------------------------------------------------------
     * Whether the referenced pointer is owned.
    */
    bool mOwned{false};
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpActivitySecrets() noexcept
        : DpActivitySecrets(new Ptr::element_type(), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpActivitySecrets(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit DpActivitySecrets(const Ptr::element_type & o) noexcept
        : DpActivitySecrets(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpActivitySecrets(Ptr::element_type && o) noexcept
        : DpActivitySecrets(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpActivitySecrets(const DpActivitySecrets & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpActivitySecrets(DpActivitySecrets && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpActivitySecrets() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpActivitySecrets & operator = (const DpActivitySecrets & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpActivitySecrets & operator = (DpActivitySecrets && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord activity secrets handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the secret for joining a party.
    */
    SQMOD_NODISCARD const std::string & GetJoin() const { return Valid().join; }
    /* --------------------------------------------------------------------------------------------
     * Modify the secret for joining a party.
    */
    void SetJoin(StackStrF & join) const { Valid().join = join.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the secret for joining a party.
    */
    DpActivitySecrets & ApplyJoin(StackStrF & join) { SetJoin(join); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the secret for spectating a game.
    */
    SQMOD_NODISCARD const std::string & GetSpectate() const { return Valid().spectate; }
    /* --------------------------------------------------------------------------------------------
     * Modify the secret for spectating a game.
    */
    void SetSpectate(StackStrF & spectate) const { Valid().spectate = spectate.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the secret for spectating a game.
    */
    DpActivitySecrets & ApplySpectate(StackStrF & spectate) { SetSpectate(spectate); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the secret for a specific instanced match.
    */
    SQMOD_NODISCARD const std::string & GetMatch() const { return Valid().match; }
    /* --------------------------------------------------------------------------------------------
     * Modify the secret for a specific instanced match.
    */
    void SetMatch(StackStrF & match) const { Valid().match = match.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the secret for a specific instanced match.
    */
    DpActivitySecrets & ApplyMatch(StackStrF & match) { SetMatch(match); return *this; }
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
     * Whether the referenced pointer is owned.
    */
    bool mOwned{false};
    // --------------------------------------------------------------------------------------------
    using Buttons = DpVectorProxy< dpp::activity_button, DpActivityButton >;
    // --------------------------------------------------------------------------------------------
    LightObj mSqAssets{};
    LightObj mSqButtons{};
    LightObj mSqEmoji{};
    LightObj mSqSecrets{};
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
    ~DpActivity() noexcept { Cleanup(); }
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
        // Cleanup activity assets, if any
        if (!mSqAssets.IsNull())
        {
            mSqAssets.CastI< DpActivityAssets >()->Cleanup();
            // Release script resources
            mSqAssets.Release();
        }
        // Cleanup activity buttons, if any
        if (!mSqButtons.IsNull())
        {
            mSqButtons.CastI< Buttons >()->Cleanup();
            // Release script resources
            mSqButtons.Release();
        }
        // Cleanup activity emoji, if any
        if (!mSqEmoji.IsNull())
        {
            mSqEmoji.CastI< DpEmoji >()->Cleanup();
            // Release script resources
            mSqEmoji.Release();
        }
        // Cleanup activity secrets, if any
        if (!mSqSecrets.IsNull())
        {
            mSqSecrets.CastI< DpActivitySecrets >()->Cleanup();
            // Release script resources
            mSqSecrets.Release();
        }
        // Do we own this to try delete it?
        if (!mOwned && mPtr) {
            // Not our job, simply forget about it
            [[maybe_unused]] auto p = mPtr.release();
        } else mPtr.reset(); // We own this so delete the instance
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
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the activity. e.g. "Vice City"
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
     * Retrieve the state of the activity. e.g. "Waiting in lobby"
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
     * Retrieve the details of the activity. What the player is currently doing.
    */
    SQMOD_NODISCARD const std::string & GetDetails() const { return Valid().details; }
    /* --------------------------------------------------------------------------------------------
     * Modify the details of the activity.
    */
    void SetDetails(StackStrF & details) const { Valid().details = details.ToStr(); }
    /* --------------------------------------------------------------------------------------------
     * Modify the details of the activity.
    */
    DpActivity & ApplyDetails(StackStrF & details) { SetDetails(details); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the images for the presence and their hover texts.
    */
    SQMOD_NODISCARD LightObj & GetAssets()
    {
        if (mSqAssets.IsNull())
        {
            mSqAssets = LightObj{SqTypeIdentity< DpActivityAssets >{}, SqVM(), &Valid().assets, false};
        }
        // Return the associated script object
        return mSqAssets;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the url of the activity. Only applicable for certain sites such a YouTube.
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
     * Retrieve the custom buttons shown in the Rich Presence (max 2).
    */
    SQMOD_NODISCARD LightObj & GetButtons()
    {
        if (mSqButtons.IsNull())
        {
            mSqButtons = LightObj{SqTypeIdentity< Buttons >{}, SqVM(), &Valid().buttons, false};
        }
        // Return the associated script object
        return mSqButtons;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the emoji used for the custom status.
    */
    SQMOD_NODISCARD LightObj & GetEmoji()
    {
        if (mSqEmoji.IsNull())
        {
            mSqEmoji = LightObj{SqTypeIdentity< DpEmoji >{}, SqVM(), &Valid().emoji, false};
        }
        // Return the associated script object
        return mSqEmoji;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the ID of the party.
    */
    SQMOD_NODISCARD dpp::snowflake GetPartyID() const { return Valid().party.id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the party's current size. Used to show the party's current size.
    */
    SQMOD_NODISCARD SQInteger GetCurrentPartySize() const { return Valid().party.current_size; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the party's maximum size. Used to show the party's maximum size.
    */
    SQMOD_NODISCARD SQInteger GetMaxPartySize() const { return Valid().party.maximum_size; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the secrets for rich presence joining and spectating.
    */
    SQMOD_NODISCARD LightObj & GetSecrets()
    {
        if (mSqSecrets.IsNull())
        {
            mSqSecrets = LightObj{SqTypeIdentity< DpActivitySecrets >{}, SqVM(), &Valid().secrets, false};
        }
        // Return the associated script object
        return mSqSecrets;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the type for the activity from SqDiscordActivityType.
    */
    SQMOD_NODISCARD SQInteger GetType() const { return Valid().type; }
    /* --------------------------------------------------------------------------------------------
     * Modify the type for the activity from SqDiscordActivityType.
    */
    void SetType(SQInteger type) const { Valid().type = static_cast< dpp::activity_type >(type); }
    /* --------------------------------------------------------------------------------------------
     * Modify the type for the activity from SqDiscordActivityType.
    */
    DpActivity & ApplyType(SQInteger type) { SetType(type); return *this; }
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
    /* --------------------------------------------------------------------------------------------
     * Retrieve the creating application (e.g. a linked account on the user's client)
    */
    SQMOD_NODISCARD dpp::snowflake GetApplicationID() const { return Valid().application_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the flags for the activity from SqDiscordActivityFlags.
    */
    SQMOD_NODISCARD SQInteger GetFlags() const { return Valid().flags; }
    /* --------------------------------------------------------------------------------------------
     * Modify the flags for the activity from SqDiscordActivityFlags.
    */
    void SetFlags(SQInteger flags) const { Valid().flags = static_cast< uint8_t >(flags); }
    /* --------------------------------------------------------------------------------------------
     * Modify the flags for the activity from SqDiscordActivityFlags.
    */
    DpActivity & ApplyFlags(SQInteger flags) { SetFlags(flags); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve whether or not the activity is an instanced game session.
    */
    SQMOD_NODISCARD bool IsInstance() const { return Valid().is_instance; }
    /* --------------------------------------------------------------------------------------------
     * Get the assets large image url if they have one, otherwise returns an empty string.
     * In case of prefixed image IDs (mp:{image_id}) it returns an empty string.
     * See: https://discord.com/developers/docs/topics/gateway-events#activity-object-activity-asset-image
    */
    SQMOD_NODISCARD std::string GetLargeAssetURL(SQInteger size, SQInteger format) const
    { return Valid().get_large_asset_url(static_cast< uint16_t >(size), static_cast< dpp::image_type >(format)); }
    /* --------------------------------------------------------------------------------------------
     * Get the assets small image url if they have one, otherwise returns an empty string.
     * In case of prefixed image IDs (mp:{image_id}) it returns an empty string.
     * See: https://discord.com/developers/docs/topics/gateway-events#activity-object-activity-asset-image
    */
    SQMOD_NODISCARD std::string GetSmallAssetURL(SQInteger size, SQInteger format) const
    { return Valid().get_small_asset_url(static_cast< uint16_t >(size), static_cast< dpp::image_type >(format)); }
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
     * Whether the referenced pointer is owned.
    */
    bool mOwned{false};
    // --------------------------------------------------------------------------------------------
    using Activities = DpVectorProxy< dpp::activity, DpActivity >;
    // --------------------------------------------------------------------------------------------
    LightObj mSqActivities{};
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
     * Explicit constructor.
    */
    DpPresence(SQInteger status, DpActivity & activity)
        : DpPresence(new Ptr::element_type(static_cast< dpp::presence_status >(status), activity.Valid()), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    DpPresence(SQInteger status, SQInteger type, StackStrF & description)
        : DpPresence(new Ptr::element_type(static_cast< dpp::presence_status >(status), static_cast< dpp::activity_type >(status), description.ToStr()), true)
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
    ~DpPresence() noexcept { Cleanup(); }
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
        // Cleanup presence activities, if any
        if (!mSqActivities.IsNull())
        {
            mSqActivities.CastI< Activities >()->Cleanup();
            // Release script resources
            mSqActivities.Release();
        }
        // Do we own this to try delete it?
        if (!mOwned && mPtr) {
            // Not our job, simply forget about it
            [[maybe_unused]] auto p = mPtr.release();
        } else mPtr.reset(); // We own this so delete the instance
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
     * Check whether a valid instance is managed.
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
     * Retrieve the list of activities.
    */
    SQMOD_NODISCARD LightObj & GetActivities()
    {
        if (mSqActivities.IsNull())
        {
            mSqActivities = LightObj{SqTypeIdentity< Activities >{}, SqVM(), &Valid().activities, false};
        }
        // Return the associated script object
        return mSqActivities;
    }
    /* --------------------------------------------------------------------------------------------
     * Build JSON string from this object.
    */
    SQMOD_NODISCARD std::string BuildJSON() const { return Valid().build_json(); }
    SQMOD_NODISCARD std::string BuildJSON_(bool with_id) const { return Valid().build_json(with_id); }
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


} // Namespace:: SqMod
