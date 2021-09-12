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
 * Represents a role within a DpGuild.
*/
struct DpRole
{
    using Ptr = std::unique_ptr< dpp::role >;
    /* --------------------------------------------------------------------------------------------
     * Referenced role instance.
    */
    Ptr mPtr{nullptr};
    /* --------------------------------------------------------------------------------------------
     * Wether the referenced pointer is owned.
    */
    bool mOwned{false};
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpRole() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpRole(Ptr::pointer ptr, bool owned = false) noexcept
        : mPtr(ptr), mOwned(owned)
    { }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpRole(const Ptr::element_type & o) noexcept
        : DpRole(new Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpRole(Ptr::element_type && o) noexcept
        : DpRole(new Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpRole(const DpRole & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpRole(DpRole && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpRole() noexcept
    {
        // Do we own this to try delete it?
        if (!mOwned && mPtr) [[maybe_unused]] auto p = mPtr.release();
    }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpRole & operator = (const DpRole & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpRole & operator = (DpRole && o) noexcept
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
    void Validate() const { if (!mPtr) STHROWF("Invalid discord role handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check wether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve role name.
    */
    SQMOD_NODISCARD const std::string & GetName() const { return Valid().name; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the role guild id.
    */
    SQMOD_NODISCARD dpp::snowflake GetGuildID() const { return Valid().guild_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the role colour.
    */
    SQMOD_NODISCARD SQInteger GetColour() const { return static_cast< SQInteger >(Valid().colour); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the role position.
    */
    SQMOD_NODISCARD SQInteger GetPosition() const { return static_cast< SQInteger >(Valid().position); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the role permissions bitmask values from SqDiscordRolePermissions.
    */
    SQMOD_NODISCARD SQInteger GetPermissions() const { return static_cast< SQInteger >(Valid().permissions); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the role flags from SqDiscordRoleFlags.
    */
    SQMOD_NODISCARD SQInteger GetFlags() const { return static_cast< SQInteger >(Valid().flags); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the role integration id if any (e.g. role is a bot's role created when it was invited).
    */
    SQMOD_NODISCARD dpp::snowflake GetIntegrationID() const { return Valid().integration_id; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the role bot id if any (e.g. role is a bot's role created when it was invited).
    */
    SQMOD_NODISCARD dpp::snowflake GetBotID() const { return Valid().bot_id; }
    /* --------------------------------------------------------------------------------------------
     * Build a JSON string from this object.
    */
    SQMOD_NODISCARD std::string BuildJSON() const { return Valid().build_json(); }
    /* --------------------------------------------------------------------------------------------
     * Build a JSON string from this object. If [with_id] is True then ID is to be included in the JSON.
    */
    SQMOD_NODISCARD std::string BuildJSON_(bool with_id) const { return Valid().build_json(with_id); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role is hoisted.
    */
    SQMOD_NODISCARD bool IsHoisted() const { return Valid().is_hoisted(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role is mentionable.
    */
    SQMOD_NODISCARD bool IsMentionable() const { return Valid().is_mentionable(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role is managed (belongs to a bot or application).
    */
    SQMOD_NODISCARD bool IsManaged() const { return Valid().is_managed(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has create instant invite permission.
    */
    SQMOD_NODISCARD bool CanCreateInstantInvite() const { return Valid().has_create_instant_invite(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the kick members permission.
    */
    SQMOD_NODISCARD bool CanKickMembers() const { return Valid().has_kick_members(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the ban members permission.
    */
    SQMOD_NODISCARD bool CanBanMembers() const { return Valid().has_ban_members(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the administrator permission.
    */
    SQMOD_NODISCARD bool IsAdministrator() const { return Valid().has_administrator(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the manage channels permission.
    */
    SQMOD_NODISCARD bool CanManageChannels() const { return Valid().has_manage_channels(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the manage guild permission.
    */
    SQMOD_NODISCARD bool CanManageGuild() const { return Valid().has_manage_guild(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the add reactions permission.
    */
    SQMOD_NODISCARD bool CanAddReactions() const { return Valid().has_add_reactions(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the view audit log permission.
    */
    SQMOD_NODISCARD bool CanViewAuditLog() const { return Valid().has_view_audit_log(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the priority speaker permission.
    */
    SQMOD_NODISCARD bool IsPrioritySpeaker() const { return Valid().has_priority_speaker(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the stream permission.
    */
    SQMOD_NODISCARD bool CanStream() const { return Valid().has_stream(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the view channel permission.
    */
    SQMOD_NODISCARD bool CanViewChannel() const { return Valid().has_view_channel(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the send messages permission.
    */
    SQMOD_NODISCARD bool CanSendMessages() const { return Valid().has_send_messages(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the send TTS messages permission.
    */
    SQMOD_NODISCARD bool CanSendTtsMessages() const { return Valid().has_send_tts_messages(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the manage messages permission.
    */
    SQMOD_NODISCARD bool CanManageMessages() const { return Valid().has_manage_messages(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the embed links permission.
    */
    SQMOD_NODISCARD bool CanEmbedLinks() const { return Valid().has_embed_links(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the attach files permission.
    */
    SQMOD_NODISCARD bool CanAttachFiles() const { return Valid().has_attach_files(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the read message history permission.
    */
    SQMOD_NODISCARD bool CanReadMessageHistory() const { return Valid().has_read_message_history(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the mention \@everyone and \@here permission.
    */
    SQMOD_NODISCARD bool CanMentionEveryone() const { return Valid().has_mention_everyone(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the use external emojis permission.
    */
    SQMOD_NODISCARD bool CanUseExternalEmojis() const { return Valid().has_use_external_emojis(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the view guild insights permission.
    */
    SQMOD_NODISCARD bool CanViewGuildInsights() const { return Valid().has_view_guild_insights(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the connect voice permission.
    */
    SQMOD_NODISCARD bool CanConnect() const { return Valid().has_connect(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the speak permission.
    */
    SQMOD_NODISCARD bool CanSpeak() const { return Valid().has_speak(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the mute members permission.
    */
    SQMOD_NODISCARD bool CanMuteMembers() const { return Valid().has_mute_members(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the deafen members permission.
    */
    SQMOD_NODISCARD bool CanDeafenMembers() const { return Valid().has_deafen_members(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the move members permission.
    */
    SQMOD_NODISCARD bool CanMoveMembers() const { return Valid().has_move_members(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has use voice activity detection permission.
    */
    SQMOD_NODISCARD bool CanUseVAT() const { return Valid().has_use_vad(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the change nickname permission.
    */
    SQMOD_NODISCARD bool CanChangeNickname() const { return Valid().has_change_nickname(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the manage nicknames permission.
    */
    SQMOD_NODISCARD bool CanManageNicknames() const { return Valid().has_manage_nicknames(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the manage roles permission.
    */
    SQMOD_NODISCARD bool CanManageRoles() const { return Valid().has_manage_roles(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the manage webhooks permission.
    */
    SQMOD_NODISCARD bool CanManageWebhooks() const { return Valid().has_manage_webhooks(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the manage emojis permission.
    */
    SQMOD_NODISCARD bool CanManageEmojis() const { return Valid().has_manage_emojis(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the use slash commands permission.
    */
    SQMOD_NODISCARD bool CanUseSlashCommands() const { return Valid().has_use_slash_commands(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the request to speak permission.
    */
    SQMOD_NODISCARD bool HasRequestToSpeak() const { return Valid().has_request_to_speak(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the manage threads permission.
    */
    SQMOD_NODISCARD bool CanManageThreads() const { return Valid().has_manage_threads(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the use public threads permission.
    */
    SQMOD_NODISCARD bool HasUsePublicThreads() const { return Valid().has_use_public_threads(); }
    /* --------------------------------------------------------------------------------------------
     * Check wether the role has the use private threads permission.
    */
    SQMOD_NODISCARD bool HasUsePrivateThreads() const { return Valid().has_use_private_threads(); }
};

} // Namespace:: SqMod
