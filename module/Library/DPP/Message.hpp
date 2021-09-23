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
            mSqOptions.CastI< Options >()->Cleanup();
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
    /* --------------------------------------------------------------------------------------------
     * Add a sub-component, only valid for action rows.
     * Adding sub-components to a component will automatically set this component's type to SqDiscordComponentType.ActionRow.
    */
    DpComponent & AddComponent(DpComponent & comp)
    {
        Valid().add_component(comp.Valid());
        // Do we have to sync a script wrapper?
        if (!mSqComponents.IsNull())
        {
            mSqComponents.CastI< Components >()->mVec.emplace_back();
        }
        // Allow chaining
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Add a select-option. Limited to 25 options.
    */
    DpComponent & AddSelectOption(DpSelectOption & opt)
    {
        // Make sure we are within bounds
        if (Valid().options.size() >= 25)
        {
            STHROWF("Maximum of 25 select options was reached");
        }
        // Perform the request
        mPtr->add_select_option(opt.Valid());
        // Do we have to sync a script wrapper?
        if (!mSqOptions.IsNull())
        {
            mSqOptions.CastI< Options >()->mVec.emplace_back();
        }
        // Allow chaining
        return *this;
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
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated footer's text.
    */
    SQMOD_NODISCARD const String & GetText() const { return Valid().text; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated footer's text.
    */
    void SetText(StackStrF & text) const { Valid().set_text(text.ToStr()); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated footer's text.
    */
    DpEmbedFooter & ApplyText(StackStrF & text) { SetText(text); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated footer's icon url.
    */
    SQMOD_NODISCARD const String & GetIconURL() const { return Valid().icon_url; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated footer's icon url.
    */
    void SetIconURL(StackStrF & icon_url) const { Valid().set_icon(icon_url.ToStr()); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated footer's icon url.
    */
    DpEmbedFooter & ApplyIconURL(StackStrF & icon_url) { SetIconURL(icon_url); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated footer's proxied icon url.
    */
    SQMOD_NODISCARD const String & GetProxyURL() const { return Valid().proxy_url; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated footer's proxied icon url.
    */
    void SetProxyURL(StackStrF & proxy_url) const { Valid().set_proxy(proxy_url.ToStr()); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated footer's proxied icon url.
    */
    DpEmbedFooter & ApplyProxyURL(StackStrF & proxy_url) { SetProxyURL(proxy_url); return *this; }
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
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated URL to image or video.
    */
    SQMOD_NODISCARD const String & GetURL() const { return Valid().url; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated URL to image or video.
    */
    void SetURL(StackStrF & url) const
    {
        if (url.mLen > 0)
        {
            Valid().url.assign(url.mPtr, static_cast< size_t >(url.mLen));
        }
        else
        {
            Valid().url.clear();
        } 
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated URL to image or video.
    */
    DpEmbedImage & ApplyURL(StackStrF & url) { SetURL(url); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated proxied image url.
    */
    SQMOD_NODISCARD const String & GetProxyURL() const { return Valid().proxy_url; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated proxied image url.
    */
    void SetProxyURL(StackStrF & proxy_url) const
    {
        if (proxy_url.mLen > 0)
        {
            Valid().proxy_url.assign(proxy_url.mPtr, static_cast< size_t >(proxy_url.mLen));
        }
        else
        {
            Valid().proxy_url.clear();
        } 
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated proxied image url.
    */
    DpEmbedImage & ApplyProxyURL(StackStrF & proxy_url) { SetProxyURL(proxy_url); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated height (calculated by discord).
    */
    SQMOD_NODISCARD const String & GetHeight() const { return Valid().height; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated height (calculated by discord).
    */
    void SetHeight(StackStrF & height) const
    {
        if (height.mLen > 0)
        {
            Valid().height.assign(height.mPtr, static_cast< size_t >(height.mLen));
        }
        else
        {
            Valid().height.clear();
        } 
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated height (calculated by discord).
    */
    DpEmbedImage & ApplyHeight(StackStrF & height) { SetHeight(height); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated width (calculated by discord).
    */
    SQMOD_NODISCARD const String & GetWidth() const { return Valid().width; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated width (calculated by discord).
    */
    void SetWidth(StackStrF & width) const
    {
        if (width.mLen > 0)
        {
            Valid().width.assign(width.mPtr, static_cast< size_t >(width.mLen));
        }
        else
        {
            Valid().width.clear();
        } 
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated width (calculated by discord).
    */
    DpEmbedImage & ApplyWidth(StackStrF & width) { SetWidth(width); return *this; }
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
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated provider name.
    */
    SQMOD_NODISCARD const String & GetName() const { return Valid().name; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated provider name.
    */
    void SetName(StackStrF & name) const
    {
        if (name.mLen > 0)
        {
            Valid().name.assign(name.mPtr, static_cast< size_t >(name.mLen));
        }
        else
        {
            Valid().name.clear();
        } 
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated provider name.
    */
    DpEmbedProvider & ApplyName(StackStrF & name) { SetName(name); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated provider URL.
    */
    SQMOD_NODISCARD const String & GetURL() const { return Valid().url; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated provider URL.
    */
    void SetURL(StackStrF & url) const
    {
        if (url.mLen > 0)
        {
            Valid().url.assign(url.mPtr, static_cast< size_t >(url.mLen));
        }
        else
        {
            Valid().url.clear();
        } 
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated provider URL.
    */
    DpEmbedProvider & ApplyURL(StackStrF & url) { SetURL(url); return *this; }
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
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated author name.
    */
    SQMOD_NODISCARD const String & GetName() const { return Valid().name; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated author name.
    */
    void SetName(StackStrF & name) const
    {
        if (name.mLen > 0)
        {
            Valid().name.assign(name.mPtr, static_cast< size_t >(name.mLen));
        }
        else
        {
            Valid().name.clear();
        } 
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated author name.
    */
    DpEmbedAuthor & ApplyName(StackStrF & name) { SetName(name); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated author URL.
    */
    SQMOD_NODISCARD const String & GetURL() const { return Valid().url; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated author URL.
    */
    void SetURL(StackStrF & url) const
    {
        if (url.mLen > 0)
        {
            Valid().url.assign(url.mPtr, static_cast< size_t >(url.mLen));
        }
        else
        {
            Valid().url.clear();
        } 
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated author URL.
    */
    DpEmbedAuthor & ApplyURL(StackStrF & url) { SetURL(url); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated author icon URL.
    */
    SQMOD_NODISCARD const String & GetIconURL() const { return Valid().icon_url; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated author icon URL.
    */
    void SetIconURL(StackStrF & icon_url) const
    {
        if (icon_url.mLen > 0)
        {
            Valid().icon_url.assign(icon_url.mPtr, static_cast< size_t >(icon_url.mLen));
        }
        else
        {
            Valid().icon_url.clear();
        } 
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated author icon URL.
    */
    DpEmbedAuthor & ApplyIconURL(StackStrF & icon_url) { SetIconURL(icon_url); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated proxied icon URL.
    */
    SQMOD_NODISCARD const String & GetProxyIconURL() const { return Valid().proxy_icon_url; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated proxied icon URL.
    */
    void SetProxyIconURL(StackStrF & proxy_icon_url) const
    {
        if (proxy_icon_url.mLen > 0)
        {
            Valid().proxy_icon_url.assign(proxy_icon_url.mPtr, static_cast< size_t >(proxy_icon_url.mLen));
        }
        else
        {
            Valid().proxy_icon_url.clear();
        } 
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated proxied icon URL.
    */
    DpEmbedAuthor & ApplyProxyIconURL(StackStrF & proxy_icon_url) { SetProxyIconURL(proxy_icon_url); return *this; }
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
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated field name.
    */
    SQMOD_NODISCARD const String & GetName() const { return Valid().name; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated field name.
    */
    void SetName(StackStrF & name) const
    {
        if (name.mLen > 0)
        {
            Valid().name.assign(name.mPtr, static_cast< size_t >(name.mLen));
        }
        else
        {
            Valid().name.clear();
        } 
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated field name.
    */
    DpEmbedField & ApplyName(StackStrF & name) { SetName(name); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated field value (max length 1000).
    */
    SQMOD_NODISCARD const String & GetValue() const { return Valid().value; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated field value (max length 1000).
    */
    void SetValue(StackStrF & value) const
    {
        if (value.mLen > 0)
        {
            auto str = value.ToStr();
            auto len = dpp::utility::utf8len(str);
            // Make sure we are within bounds
            if (len > 1000)
            {
                STHROWF("Field value exceeds the 1000 character limit");
            }
            // Assign the specified string
            Valid().value = std::move(str);
        }
        else
        {
            Valid().value.clear();
        }
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated field value (max length 1000).
    */
    DpEmbedField & ApplyValue(StackStrF & value) { SetValue(value); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Check whether the field is to be displayed inline.
    */
    SQMOD_NODISCARD bool IsInline() const { return Valid().is_inline; }
    /* --------------------------------------------------------------------------------------------
     * Modify whether the field is to be displayed inline.
    */
    DpEmbedField & SetInline(bool toggle) { Valid().is_inline = toggle; return *this; }
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
    // --------------------------------------------------------------------------------------------
    using Fields = DpVectorProxy< dpp::embed_field, DpEmbedField >;
    // --------------------------------------------------------------------------------------------
    LightObj mSqFooter{};
    LightObj mSqImage{};
    LightObj mSqThumbnail{};
    LightObj mSqVideo{};
    LightObj mSqProvider{};
    LightObj mSqAuthor{};
    LightObj mSqFields{};
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
        DropObject< DpEmbedFooter >(mSqFooter);
        DropObject< DpEmbedImage >(mSqImage);
        DropObject< DpEmbedImage >(mSqThumbnail);
        DropObject< DpEmbedImage >(mSqVideo);
        DropObject< DpEmbedProvider >(mSqProvider);
        DropObject< DpEmbedAuthor >(mSqAuthor);
        // Cleanup fields, if any
        if (!mSqFields.IsNull())
        {
            mSqFields.CastI< Fields >()->Cleanup();
            // Release script resources
            mSqFields.Release();
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord embed handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated title of embed.
    */
    SQMOD_NODISCARD const String & GetTitle() const { return Valid().title; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated title of embed.
    */
    void SetTitle(StackStrF & title) const { Valid().set_title(title.ToStr()); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated title of embed.
    */
    DpEmbed & ApplyTitle(StackStrF & title) { SetTitle(title); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated type of embed (always "rich" for web-hook embeds).
    */
    SQMOD_NODISCARD const String & GetType() const { return Valid().type; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated type of embed.
    */
    void SetType(StackStrF & type) const
    {
        if (type.mLen > 0)
        {
            Valid().type.assign(type.mPtr, static_cast< size_t >(type.mLen));
        }
        else
        {
            Valid().type.clear();
        } 
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated type of embed.
    */
    DpEmbed & ApplyType(StackStrF & type) { SetType(type); return *this; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated description of embed.
    */
    SQMOD_NODISCARD const String & GetDescription() const { return Valid().description; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated description of embed.
    */
    void SetDescription(StackStrF & description) const { Valid().set_description(description.ToStr()); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated description of embed.
    */
    DpEmbed & ApplyDescription(StackStrF & description) { SetDescription(description); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated URL of embed.
    */
    SQMOD_NODISCARD const String & GetURL() const { return Valid().url; }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated URL of embed.
    */
    void SetURL(StackStrF & url) const { Valid().set_url(url.ToStr()); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated URL of embed.
    */
    DpEmbed & ApplyURL(StackStrF & url) { SetURL(url); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve time-stamp of embed content.
    */
    SQMOD_NODISCARD SQInteger GetTimeStamp() const
    {
        return static_cast< SQInteger >(std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::from_time_t(Valid().timestamp).time_since_epoch()).count());
    }
    /* --------------------------------------------------------------------------------------------
     * Modify time-stamp of embed content.
    */
    void SetTimeStamp(SQInteger s) const
    {
        Valid().timestamp = std::chrono::system_clock::to_time_t(std::chrono::time_point< std::chrono::system_clock >{std::chrono::seconds{s}});
    }
    /* --------------------------------------------------------------------------------------------
     * Modify time-stamp of embed content.
    */
    DpEmbed & ApplyTimeStamp(SQInteger s) { SetTimeStamp(s); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated color code of the embed.
    */
    SQMOD_NODISCARD SQInteger GetColor() const { return static_cast< SQInteger >(Valid().color); }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated color code of the embed.
    */
    void SetColor(SQInteger color) const { Valid().set_color(static_cast< dpp::component_style >(color)); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated footer information.
    */
    SQMOD_NODISCARD LightObj & GetFooter()
    {
        // Do we have a value assigned?
        if (Valid().footer.has_value())
        {
            // Do we have a script object associated with it?
            if (mSqFooter.IsNull())
            {
                // Create the script object with non-owning reference
                mSqFooter = LightObj(SqTypeIdentity< DpEmbedFooter >{}, SqVM(), &(mPtr->footer.value()), false);
            }
        } else mSqFooter.Release(); // Make sure this is null
        // Return the associated script object
        return mSqFooter;
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated footer information.
    */
    void SetFooter(LightObj & o)
    {
        Validate();
        // Is the specified object null?
        if (o.IsNull())
        {
            DropObject< DpEmbedFooter >(mSqFooter);
            // Finally release the instance
            mPtr->footer.reset();
        }
        else
        {
            auto * p = o.CastI< DpEmbedFooter >();
            // Is this the same instance?
            if (&p->Valid() == &(mPtr->footer.value()))
            {
                return; // Ignore (someone is playing dumb)
            }
            // Drop currently associated script object, if any
            DropObject< DpEmbedFooter >(mSqFooter);
            // Assign the new object
            mPtr->footer = *(p->mPtr);
        }
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated footer information.
    */
    DpEmbed & ApplyFooter(LightObj & o) { SetFooter(o); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated image information.
    */
    SQMOD_NODISCARD LightObj & GetImage()
    {
        // Do we have a value assigned?
        if (Valid().image.has_value())
        {
            // Do we have a script object associated with it?
            if (mSqImage.IsNull())
            {
                // Create the script object with non-owning reference
                mSqImage = LightObj(SqTypeIdentity< DpEmbedImage >{}, SqVM(), &(mPtr->image.value()), false);
            }
        } else mSqImage.Release(); // Make sure this is null
        // Return the associated script object
        return mSqImage;
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated image information.
    */
    void SetImage(LightObj & o)
    {
        Validate();
        // Is the specified object null?
        if (o.IsNull())
        {
            DropObject< DpEmbedImage >(mSqImage);
            // Finally release the instance
            mPtr->image.reset();
        }
        else
        {
            auto * p = o.CastI< DpEmbedImage >();
            // Is this the same instance?
            if (&p->Valid() == &(mPtr->image.value()))
            {
                return; // Ignore (someone is playing dumb)
            }
            // Drop currently associated script object, if any
            DropObject< DpEmbedImage >(mSqImage);
            // Assign the new object
            mPtr->image = *(p->mPtr);
        }
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated image information.
    */
    DpEmbed & ApplyImage(LightObj & o) { SetImage(o); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated thumbnail information.
    */
    SQMOD_NODISCARD LightObj & GetThumbnail()
    {
        // Do we have a value assigned?
        if (Valid().thumbnail.has_value())
        {
            // Do we have a script object associated with it?
            if (mSqThumbnail.IsNull())
            {
                // Create the script object with non-owning reference
                mSqThumbnail = LightObj(SqTypeIdentity< DpEmbedImage >{}, SqVM(), &(mPtr->thumbnail.value()), false);
            }
        } else mSqThumbnail.Release(); // Make sure this is null
        // Return the associated script object
        return mSqThumbnail;
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated thumbnail information.
    */
    void SetThumbnail(LightObj & o)
    {
        Validate();
        // Is the specified object null?
        if (o.IsNull())
        {
            DropObject< DpEmbedImage >(mSqThumbnail);
            // Finally release the instance
            mPtr->thumbnail.reset();
        }
        else
        {
            auto * p = o.CastI< DpEmbedImage >();
            // Is this the same instance?
            if (&p->Valid() == &(mPtr->thumbnail.value()))
            {
                return; // Ignore (someone is playing dumb)
            }
            // Drop currently associated script object, if any
            DropObject< DpEmbedImage >(mSqThumbnail);
            // Assign the new object
            mPtr->thumbnail = *(p->mPtr);
        }
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated thumbnail information.
    */
    DpEmbed & ApplyThumbnail(LightObj & o) { SetThumbnail(o); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated video information (can't send these).
    */
    SQMOD_NODISCARD LightObj & GetVideo()
    {
        // Do we have a value assigned?
        if (Valid().video.has_value())
        {
            // Do we have a script object associated with it?
            if (mSqVideo.IsNull())
            {
                // Create the script object with non-owning reference
                mSqVideo = LightObj(SqTypeIdentity< DpEmbedImage >{}, SqVM(), &(mPtr->video.value()), false);
            }
        } else mSqVideo.Release(); // Make sure this is null
        // Return the associated script object
        return mSqVideo;
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated video information (can't send these).
    */
    void SetVideo(LightObj & o)
    {
        Validate();
        // Is the specified object null?
        if (o.IsNull())
        {
            DropObject< DpEmbedImage >(mSqVideo);
            // Finally release the instance
            mPtr->video.reset();
        }
        else
        {
            auto * p = o.CastI< DpEmbedImage >();
            // Is this the same instance?
            if (&p->Valid() == &(mPtr->video.value()))
            {
                return; // Ignore (someone is playing dumb)
            }
            // Drop currently associated script object, if any
            DropObject< DpEmbedImage >(mSqVideo);
            // Assign the new object
            mPtr->video = *(p->mPtr);
        }
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated video information (can't send these).
    */
    DpEmbed & ApplyVideo(LightObj & o) { SetVideo(o); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated provider information (can't send these).
    */
    SQMOD_NODISCARD LightObj & GetProvider()
    {
        // Do we have a value assigned?
        if (Valid().provider.has_value())
        {
            // Do we have a script object associated with it?
            if (mSqProvider.IsNull())
            {
                // Create the script object with non-owning reference
                mSqProvider = LightObj(SqTypeIdentity< DpEmbedProvider >{}, SqVM(), &(mPtr->provider.value()), false);
            }
        } else mSqProvider.Release(); // Make sure this is null
        // Return the associated script object
        return mSqProvider;
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated provider information (can't send these).
    */
    void SetProvider(LightObj & o)
    {
        Validate();
        // Is the specified object null?
        if (o.IsNull())
        {
            DropObject< DpEmbedProvider >(mSqProvider);
            // Finally release the instance
            mPtr->provider.reset();
        }
        else
        {
            auto * p = o.CastI< DpEmbedProvider >();
            // Is this the same instance?
            if (&p->Valid() == &(mPtr->provider.value()))
            {
                return; // Ignore (someone is playing dumb)
            }
            // Drop currently associated script object, if any
            DropObject< DpEmbedProvider >(mSqProvider);
            // Assign the new object
            mPtr->provider = *(p->mPtr);
        }
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated provider information (can't send these).
    */
    DpEmbed & ApplyProvider(LightObj & o) { SetProvider(o); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated author information.
    */
    SQMOD_NODISCARD LightObj & GetAuthor()
    {
        // Do we have a value assigned?
        if (Valid().author.has_value())
        {
            // Do we have a script object associated with it?
            if (mSqAuthor.IsNull())
            {
                // Create the script object with non-owning reference
                mSqAuthor = LightObj(SqTypeIdentity< DpEmbedAuthor >{}, SqVM(), &(mPtr->author.value()), false);
            }
        } else mSqAuthor.Release(); // Make sure this is null
        // Return the associated script object
        return mSqAuthor;
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated author information.
    */
    void SetAuthor(LightObj & o)
    {
        Validate();
        // Is the specified object null?
        if (o.IsNull())
        {
            DropObject< DpEmbedAuthor >(mSqAuthor);
            // Finally release the instance
            mPtr->author.reset();
        }
        else
        {
            auto * p = o.CastI< DpEmbedAuthor >();
            // Is this the same instance?
            if (&p->Valid() == &(mPtr->author.value()))
            {
                return; // Ignore (someone is playing dumb)
            }
            // Drop currently associated script object, if any
            DropObject< DpEmbedAuthor >(mSqAuthor);
            // Assign the new object
            mPtr->author = *(p->mPtr);
        }
    }
    /* --------------------------------------------------------------------------------------------
     * Modify the associated author information.
    */
    DpEmbed & ApplyAuthor(LightObj & o) { SetAuthor(o); return *this; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated embed fields.
    */
    SQMOD_NODISCARD LightObj & GetFields()
    {
        if (mSqFields.IsNull())
        {
            mSqFields = LightObj{SqTypeIdentity< Fields >{}, SqVM(), &Valid().fields, false};
        }
        // Return the associated script object
        return mSqFields;
    }
    /* --------------------------------------------------------------------------------------------
     * Add an embed field.
    */
    DpEmbed & AddField(StackStrF & name, StackStrF & value)
    {
        return AddField_(name, value, false); *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Add an embed field.
    */
    DpEmbed & AddField_(StackStrF & name, StackStrF & value, bool inl)
    {
        // Perform the request
        mPtr->add_field(name.ToStr(), value.ToStr(), inl);
        // Do we have to sync a script wrapper?
        if (!mSqFields.IsNull())
        {
            mSqFields.CastI< Fields >()->mVec.emplace_back();
        }
        // Allow chaining
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Cleanup script object.
    */
    template < class T > static void DropObject(LightObj & o)
    {
        // Do we have a script object associated with it?
        if (!o.IsNull())
        {
            auto * p = o.CastI< T >();
            // Is this script object the owner of the instance?
            if (!p->mOwned)
            {
                p->Cleanup(); // Invalidate
            }
            // Release it and revert to null
            o.Release();
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Represents a reaction to a DpMessage.
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
