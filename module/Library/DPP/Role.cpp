// ------------------------------------------------------------------------------------------------
#include "Library/DPP/Role.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDppRole, _SC("SqDppRole"))

// ------------------------------------------------------------------------------------------------
void Register_DPP_Role(HSQUIRRELVM vm, Table & ns)
{
    ns.Bind(_SC("Role"),
        Class< DpRole, NoConstructor< DpRole > >(vm, SqDppRole::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppRole::Fn)
        .Func(_SC("_tojson"), &DpRole::BuildJSON)
        // Member Properties
        .Prop(_SC("Valid"), &DpRole::IsValid)
        .Prop(_SC("JSON"), &DpRole::BuildJSON)
        .Prop(_SC("Name"), &DpRole::GetName)
        .Prop(_SC("GuildID"), &DpRole::GetGuildID)
        .Prop(_SC("Color"), &DpRole::GetColour)
        .Prop(_SC("Colour"), &DpRole::GetColour)
        .Prop(_SC("Position"), &DpRole::GetPosition)
        .Prop(_SC("Permissions"), &DpRole::GetPermissions)
        .Prop(_SC("Flags"), &DpRole::GetFlags)
        .Prop(_SC("IntegrationID"), &DpRole::GetIntegrationID)
        .Prop(_SC("BotID"), &DpRole::GetBotID)
        .Prop(_SC("IsHoisted"), &DpRole::IsHoisted)
        .Prop(_SC("IsMentionable"), &DpRole::IsMentionable)
        .Prop(_SC("IsManaged"), &DpRole::IsManaged)
        .Prop(_SC("CanCreateInstantInvite"), &DpRole::CanCreateInstantInvite)
        .Prop(_SC("CanKickMembers"), &DpRole::CanKickMembers)
        .Prop(_SC("CanBanMembers"), &DpRole::CanBanMembers)
        .Prop(_SC("IsAdministrator"), &DpRole::IsAdministrator)
        .Prop(_SC("CanManageChannels"), &DpRole::CanManageChannels)
        .Prop(_SC("CanManageGuild"), &DpRole::CanManageGuild)
        .Prop(_SC("CanAddReactions"), &DpRole::CanAddReactions)
        .Prop(_SC("CanViewAuditLog"), &DpRole::CanViewAuditLog)
        .Prop(_SC("IsPrioritySpeaker"), &DpRole::IsPrioritySpeaker)
        .Prop(_SC("CanStream"), &DpRole::CanStream)
        .Prop(_SC("CanViewChannel"), &DpRole::CanViewChannel)
        .Prop(_SC("CanSendMessages"), &DpRole::CanSendMessages)
        .Prop(_SC("CanSendTtsMessages"), &DpRole::CanSendTtsMessages)
        .Prop(_SC("CanManageMessages"), &DpRole::CanManageMessages)
        .Prop(_SC("CanEmbedLinks"), &DpRole::CanEmbedLinks)
        .Prop(_SC("CanAttachFiles"), &DpRole::CanAttachFiles)
        .Prop(_SC("CanReadMessageHistory"), &DpRole::CanReadMessageHistory)
        .Prop(_SC("CanMentionEveryone"), &DpRole::CanMentionEveryone)
        .Prop(_SC("CanUseExternalEmojis"), &DpRole::CanUseExternalEmojis)
        .Prop(_SC("CanViewGuildInsights"), &DpRole::CanViewGuildInsights)
        .Prop(_SC("CanConnect"), &DpRole::CanConnect)
        .Prop(_SC("CanSpeak"), &DpRole::CanSpeak)
        .Prop(_SC("CanMuteMembers"), &DpRole::CanMuteMembers)
        .Prop(_SC("CanDeafenMembers"), &DpRole::CanDeafenMembers)
        .Prop(_SC("CanMoveMembers"), &DpRole::CanMoveMembers)
        .Prop(_SC("CanUseVAT"), &DpRole::CanUseVAT)
        .Prop(_SC("CanChangeNickname"), &DpRole::CanChangeNickname)
        .Prop(_SC("CanManageNicknames"), &DpRole::CanManageNicknames)
        .Prop(_SC("CanManageRoles"), &DpRole::CanManageRoles)
        .Prop(_SC("CanManageWebhooks"), &DpRole::CanManageWebhooks)
        .Prop(_SC("CanManageEmojis"), &DpRole::CanManageEmojis)
        .Prop(_SC("CanUseSlashCommands"), &DpRole::CanUseSlashCommands)
        .Prop(_SC("HasRequestToSpeak"), &DpRole::HasRequestToSpeak)
        .Prop(_SC("CanManageThreads"), &DpRole::CanManageThreads)
        .Prop(_SC("HasUsePublicThreads"), &DpRole::HasUsePublicThreads)
        .Prop(_SC("HasUsePrivateThreads"), &DpRole::HasUsePrivateThreads)
        // Member Methods
        .Func(_SC("BuildJSON"), &DpRole::BuildJSON_)
    );
}

} // Namespace:: SqMod
