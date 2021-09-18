#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/DPP/Utilities.hpp"

// ------------------------------------------------------------------------------------------------
#include <chrono>

// ------------------------------------------------------------------------------------------------
#include <dpp/dpp.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Select option.
*/
struct DpSelectOption
{
    using Ptr = std::unique_ptr< dpp::select_option >;
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
    DpSelectOption() noexcept
        : DpSelectOption(new Ptr::element_type(), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Null constructor.
    */
    explicit DpSelectOption(std::nullptr_t) noexcept
        : mPtr(nullptr), mOwned(false)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpSelectOption(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    DpSelectOption(StackStrF & label, StackStrF & value, StackStrF & description) noexcept
        : DpSelectOption(new Ptr::element_type(label.ToStr(), value.ToStr(), description.ToStr()), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpSelectOption(const Ptr::element_type & o) noexcept
        : DpSelectOption(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpSelectOption(Ptr::element_type && o) noexcept
        : DpSelectOption(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpSelectOption(const DpSelectOption & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpSelectOption(DpSelectOption && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpSelectOption() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpSelectOption & operator = (const DpSelectOption & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpSelectOption & operator = (DpSelectOption && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord select option handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated label.
    */
    SQMOD_NODISCARD const String & GetLabel() const { return Valid().label; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated label.
    */
    void SetLabel(StackStrF & label) const { Valid().set_label(label.ToStr()); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated label.
    */
    DpSelectOption & ApplyLabel(StackStrF & label) { SetLabel(label); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated value.
    */
    SQMOD_NODISCARD const String & GetValue() const { return Valid().value; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated value.
    */
    void SetValue(StackStrF & value) const { Valid().set_value(value.ToStr()); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated value.
    */
    DpSelectOption & ApplyValue(StackStrF & value) { SetValue(value); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated description.
    */
    SQMOD_NODISCARD const String & GetDescription() const { return Valid().description; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated description.
    */
    void SetDescription(StackStrF & description) const { Valid().set_description(description.ToStr()); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated description.
    */
    DpSelectOption & ApplyDescription(StackStrF & description) { SetDescription(description); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated emoji.
    */
    DpSelectOption & SetEmoji(StackStrF & name, dpp::snowflake id, bool animated) { Valid().set_emoji(name.ToStr(), id, animated); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Check whether this is the default option.
    */
    SQMOD_NODISCARD bool IsDefault() const { return Valid().is_default; }
    /* --------------------------------------------------------------------------------------------
     * Modify whether this is the default option.
    */
    DpSelectOption & SetDefault(bool def) { Valid().set_default(def); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Check whether the emoji is animated.
    */
    SQMOD_NODISCARD bool IsAnimated() const { return Valid().emoji.animated; }
    /* --------------------------------------------------------------------------------------------
     * Modify whether the emoji is animated.
    */
    DpSelectOption & SetAnimated(bool anim) { Valid().set_animated(anim); return *this; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated emoji name.
    */
    SQMOD_NODISCARD const String & GetEmojiName() const { return Valid().emoji.name; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated emoji name.
    */
    void SetEmojiName(StackStrF & name) const
    {
        if (name.mLen > 0)
        {
            Valid().emoji.name.assign(name.mPtr, static_cast< size_t >(name.mLen));
        }
        else
        {
            Valid().emoji.name.clear();
        } 
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated emoji name.
    */
    DpSelectOption & ApplyEmojiName(StackStrF & name) { SetEmojiName(name); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated emoji id.
    */
    SQMOD_NODISCARD dpp::snowflake GetEmojiID() const { return Valid().emoji.id; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated emoji id.
    */
    void SetEmojiID(dpp::snowflake id) const { Valid().emoji.id = id; }
};

/* ------------------------------------------------------------------------------------------------
 * Represents the component object.
 * A component is a clickable button or drop down list within a discord message,
 * where the buttons emit ButtonClick events when the user interacts with them.
*/
struct DpComponent
{
    using Ptr = std::unique_ptr< dpp::component >;
    /* --------------------------------------------------------------------------------------------
     * Referenced voice state instance.
    */
    Ptr mPtr{nullptr};
    /* --------------------------------------------------------------------------------------------
     * Whether the referenced pointer is owned.
    */
    bool mOwned{false};
    // --------------------------------------------------------------------------------------------
    using Components = DpVectorProxy< dpp::component, DpComponent >;
    using Options = DpVectorProxy< dpp::select_option, DpSelectOption >;
    // --------------------------------------------------------------------------------------------
    LightObj mSqComponents{};
    LightObj mSqOptions{};
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpComponent() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpComponent(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpComponent(const Ptr::element_type & o) noexcept
        : DpComponent(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpComponent(Ptr::element_type && o) noexcept
        : DpComponent(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpComponent(const DpComponent & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpComponent(DpComponent && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpComponent() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpComponent & operator = (const DpComponent & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpComponent & operator = (DpComponent && o) noexcept
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
        // Cleanup components, if any
        if (!mSqComponents.IsNull())
        {
            mSqComponents.CastI< Components >()->Cleanup();
            // Release script resources
            mSqComponents.Release();
        }
        // Cleanup options, if any
        if (!mSqOptions.IsNull())
        {
            mSqOptions.CastI< Components >()->Cleanup();
            // Release script resources
            mSqOptions.Release();
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord component handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Build JSON string from this object.
    */
    SQMOD_NODISCARD std::string BuildJSON() const { return Valid().build_json(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated type.
    */
    SQMOD_NODISCARD SQInteger GetType() const { return static_cast< SQInteger >(Valid().type); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated type.
    */
    void SetType(SQInteger type) const { Valid().set_type(static_cast< dpp::component_type >(type)); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated label.
    */
    SQMOD_NODISCARD const String & GetLabel() const { return Valid().label; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated label.
    */
    void SetLabel(StackStrF & label) const { Valid().set_label(label.ToStr()); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated label.
    */
    DpComponent & ApplyLabel(StackStrF & label) { SetLabel(label); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated style.
    */
    SQMOD_NODISCARD SQInteger GetStyle() const { return static_cast< SQInteger >(Valid().style); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated style.
    */
    void SetStyle(SQInteger style) const { Valid().set_style(static_cast< dpp::component_style >(style)); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated custom id.
    */
    SQMOD_NODISCARD const String & GetCustomID() const { return Valid().custom_id; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated custom id.
    */
    void SetCustomID(StackStrF & custom_id) const { Valid().set_label(custom_id.ToStr()); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated custom id.
    */
    DpComponent & ApplyCustomID(StackStrF & custom_id) { SetCustomID(custom_id); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated URL.
    */
    SQMOD_NODISCARD const String & GetURL() const { return Valid().url; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated URL.
    */
    void SetURL(StackStrF & url) const { Valid().set_label(url.ToStr()); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated URL.
    */
    DpComponent & ApplyURL(StackStrF & url) { SetURL(url); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated placeholder.
    */
    SQMOD_NODISCARD const String & GetPlaceholder() const { return Valid().placeholder; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated placeholder.
    */
    void SetPlaceholder(StackStrF & placeholder) const { Valid().set_label(placeholder.ToStr()); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated placeholder.
    */
    DpComponent & ApplyPlaceholder(StackStrF & placeholder) { SetPlaceholder(placeholder); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated minimum values.
    */
    SQMOD_NODISCARD SQInteger GetMinValues() const { return static_cast< SQInteger >(Valid().min_values); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated minimum values.
    */
    void SetMinValues(SQInteger value) const { Valid().set_min_values(static_cast< uint32_t >(value)); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated maximum values.
    */
    SQMOD_NODISCARD SQInteger GetMaxValues() const { return static_cast< SQInteger >(Valid().max_values); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated maximum values.
    */
    void SetMaxValues(SQInteger value) const { Valid().set_max_values(static_cast< uint32_t >(value)); }
    /* --------------------------------------------------------------------------------------------
     * Check whether this component is disabled.
    */
    SQMOD_NODISCARD bool IsDisabled() const { return Valid().disabled; }
    /* --------------------------------------------------------------------------------------------
     * Modify whether this component is disabled.
    */
    DpComponent & SetDisabled(bool toggle) { Valid().set_disabled(toggle); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated emoji.
    */
    DpComponent & SetEmoji(StackStrF & name, dpp::snowflake id, bool animated) { Valid().set_emoji(name.ToStr(), id, animated); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Check whether the emoji is animated.
    */
    SQMOD_NODISCARD bool IsAnimated() const { return Valid().emoji.animated; }
    /* --------------------------------------------------------------------------------------------
     * Modify whether the emoji is animated.
    */
    DpComponent & SetAnimated(bool anim) { Valid().emoji.animated = anim; return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated emoji name.
    */
    SQMOD_NODISCARD const String & GetEmojiName() const { return Valid().emoji.name; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated emoji name.
    */
    void SetEmojiName(StackStrF & name) const
    {
        if (name.mLen > 0)
        {
            Valid().emoji.name.assign(name.mPtr, static_cast< size_t >(name.mLen));
        }
        else
        {
            Valid().emoji.name.clear();
        } 
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated emoji name.
    */
    DpComponent & ApplyEmojiName(StackStrF & name) { SetEmojiName(name); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated emoji id.
    */
    SQMOD_NODISCARD dpp::snowflake GetEmojiID() const { return Valid().emoji.id; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated emoji id.
    */
    void SetEmojiID(dpp::snowflake id) const { Valid().emoji.id = id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated components.
    */
    SQMOD_NODISCARD LightObj & GetComponents()
    {
        if (mSqComponents.IsNull())
        {
            mSqComponents = LightObj{SqTypeIdentity< Components >{}, SqVM(), &Valid().components, false};
        }
        // Return the associated script object
        return mSqComponents;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated components.
    */
    SQMOD_NODISCARD LightObj & GetOptions()
    {
        if (mSqOptions.IsNull())
        {
            mSqOptions = LightObj{SqTypeIdentity< Options >{}, SqVM(), &Valid().options, false};
        }
        // Return the associated script object
        return mSqOptions;
    }
};

/* ------------------------------------------------------------------------------------------------
 * A footer in a DpEmbed.
*/
struct DpEmbedFooter
{
    using Ptr = std::unique_ptr< dpp::embed_footer >;
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
    DpEmbedFooter() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpEmbedFooter(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpEmbedFooter(const Ptr::element_type & o) noexcept
        : DpEmbedFooter(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpEmbedFooter(Ptr::element_type && o) noexcept
        : DpEmbedFooter(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpEmbedFooter(const DpEmbedFooter & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpEmbedFooter(DpEmbedFooter && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpEmbedFooter() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpEmbedFooter & operator = (const DpEmbedFooter & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpEmbedFooter & operator = (DpEmbedFooter && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord embed footer handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }

};

/* ------------------------------------------------------------------------------------------------
 * An video, image or thumbnail in a DpEmbed.
*/
struct DpEmbedImage
{
    using Ptr = std::unique_ptr< dpp::embed_image >;
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
    DpEmbedImage() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpEmbedImage(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpEmbedImage(const Ptr::element_type & o) noexcept
        : DpEmbedImage(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpEmbedImage(Ptr::element_type && o) noexcept
        : DpEmbedImage(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpEmbedImage(const DpEmbedImage & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpEmbedImage(DpEmbedImage && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpEmbedImage() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpEmbedImage & operator = (const DpEmbedImage & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpEmbedImage & operator = (DpEmbedImage && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord embed image handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }

};

/* ------------------------------------------------------------------------------------------------
 * Embed provider in a DpEmbed. Received from discord but cannot be sent.
*/
struct DpEmbedProvider
{
    using Ptr = std::unique_ptr< dpp::embed_provider >;
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
    DpEmbedProvider() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpEmbedProvider(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpEmbedProvider(const Ptr::element_type & o) noexcept
        : DpEmbedProvider(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpEmbedProvider(Ptr::element_type && o) noexcept
        : DpEmbedProvider(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpEmbedProvider(const DpEmbedProvider & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpEmbedProvider(DpEmbedProvider && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpEmbedProvider() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpEmbedProvider & operator = (const DpEmbedProvider & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpEmbedProvider & operator = (DpEmbedProvider && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord embed provider handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }

};

/* ------------------------------------------------------------------------------------------------
 * Author within a DpEmbed.
*/
struct DpEmbedAuthor
{
    using Ptr = std::unique_ptr< dpp::embed_author >;
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
    DpEmbedAuthor() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpEmbedAuthor(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpEmbedAuthor(const Ptr::element_type & o) noexcept
        : DpEmbedAuthor(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpEmbedAuthor(Ptr::element_type && o) noexcept
        : DpEmbedAuthor(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpEmbedAuthor(const DpEmbedAuthor & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpEmbedAuthor(DpEmbedAuthor && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpEmbedAuthor() noexcept { Cleanup();  }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpEmbedAuthor & operator = (const DpEmbedAuthor & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpEmbedAuthor & operator = (DpEmbedAuthor && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord embed author handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }

};

/* ------------------------------------------------------------------------------------------------
 * A DpEmbed may contain zero or more fields.
*/
struct DpEmbedField
{
    using Ptr = std::unique_ptr< dpp::embed_field >;
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
    DpEmbedField() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpEmbedField(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpEmbedField(const Ptr::element_type & o) noexcept
        : DpEmbedField(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpEmbedField(Ptr::element_type && o) noexcept
        : DpEmbedField(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpEmbedField(const DpEmbedField & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpEmbedField(DpEmbedField && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpEmbedField() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpEmbedField & operator = (const DpEmbedField & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpEmbedField & operator = (DpEmbedField && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord embed field handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }

};

/* ------------------------------------------------------------------------------------------------
 * A rich embed for display within a DpMessage.
*/
struct DpEmbed
{
    using Ptr = std::unique_ptr< dpp::embed >;
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
    DpEmbed() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpEmbed(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpEmbed(const Ptr::element_type & o) noexcept
        : DpEmbed(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpEmbed(Ptr::element_type && o) noexcept
        : DpEmbed(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpEmbed(const DpEmbed & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpEmbed(DpEmbed && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpEmbed() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpEmbed & operator = (const DpEmbed & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpEmbed & operator = (DpEmbed && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord embed handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }

};

/* ------------------------------------------------------------------------------------------------
 * Represets a reaction to a DpMessage.
*/
struct DpReaction
{
    using Ptr = std::unique_ptr< dpp::reaction >;
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
    DpReaction() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpReaction(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpReaction(const Ptr::element_type & o) noexcept
        : DpReaction(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpReaction(Ptr::element_type && o) noexcept
        : DpReaction(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpReaction(const DpReaction & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpReaction(DpReaction && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpReaction() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpReaction & operator = (const DpReaction & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpReaction & operator = (DpReaction && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord reaction handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }

};

/* ------------------------------------------------------------------------------------------------
 * Represents an attachment in a DpMessage.
*/
struct DpAttachment
{
    using Ptr = std::unique_ptr< dpp::attachment >;
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
    DpAttachment() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpAttachment(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpAttachment(const Ptr::element_type & o) noexcept
        : DpAttachment(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpAttachment(Ptr::element_type && o) noexcept
        : DpAttachment(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpAttachment(const DpAttachment & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpAttachment(DpAttachment && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpAttachment() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpAttachment & operator = (const DpAttachment & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpAttachment & operator = (DpAttachment && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord attachment handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }

};

/* ------------------------------------------------------------------------------------------------
 * Represents stickers received in a DpMessage.
*/
struct DpSticker
{
    using Ptr = std::unique_ptr< dpp::sticker >;
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
    DpSticker() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpSticker(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpSticker(const Ptr::element_type & o) noexcept
        : DpSticker(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpSticker(Ptr::element_type && o) noexcept
        : DpSticker(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpSticker(const DpSticker & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpSticker(DpSticker && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpSticker() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpSticker & operator = (const DpSticker & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpSticker & operator = (DpSticker && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord sticker handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }

};

/* ------------------------------------------------------------------------------------------------
 * Represents a sticker pack received in a DpMessage.
*/
struct DpStickerPack
{
    using Ptr = std::unique_ptr< dpp::sticker_pack >;
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
    DpStickerPack() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpStickerPack(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpStickerPack(const Ptr::element_type & o) noexcept
        : DpStickerPack(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpStickerPack(Ptr::element_type && o) noexcept
        : DpStickerPack(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpStickerPack(const DpStickerPack & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpStickerPack(DpStickerPack && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpStickerPack() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpStickerPack & operator = (const DpStickerPack & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpStickerPack & operator = (DpStickerPack && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord sticker pack handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }

};

/* ------------------------------------------------------------------------------------------------
 * Represents messages sent and received on Discord.
*/
struct DpMessage
{
    using Ptr = std::unique_ptr< dpp::message >;
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
    DpMessage() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessage(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpMessage(const Ptr::element_type & o) noexcept
        : DpMessage(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpMessage(Ptr::element_type && o) noexcept
        : DpMessage(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpMessage(const DpMessage & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpMessage(DpMessage && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpMessage() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpMessage & operator = (const DpMessage & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpMessage & operator = (DpMessage && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord message handle"); }
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
