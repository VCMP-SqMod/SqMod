// ------------------------------------------------------------------------------------------------
#include "Library/Discord/Guild.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDpGuildMember, _SC("SqDiscordGuildMember"))
SQMOD_DECL_TYPENAME(SqDpGuild, _SC("SqDiscordGuild"))

// ------------------------------------------------------------------------------------------------
void Register_Discord_Guild(HSQUIRRELVM vm, Table & ns)
{
    ns.Bind(_SC("GuildMember"),
        Class< DpGuildMember, NoConstructor< DpGuildMember > >(vm, SqDpGuildMember::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuildMember::Fn)
        .Func(_SC("_cmp"), &DpGuildMember::SqCmp)
        // Member Properties
        .Prop(_SC("Valid"), &DpGuildMember::IsValid)
        .Prop(_SC("JSON"), &DpGuildMember::BuildJSON)
        .Prop(_SC("Nickname"), &DpGuildMember::GetNickname, &DpGuildMember::SetNickname)
        .Prop(_SC("Roles"), &DpGuildMember::GetRoles)
        .Prop(_SC("RolesCount"), &DpGuildMember::RolesCount)
        .Prop(_SC("GuildID"), &DpGuildMember::GetGuildID, &DpGuildMember::SetGuildID)
        .Prop(_SC("UserID"), &DpGuildMember::GetUserID, &DpGuildMember::SetUserID)
        .Prop(_SC("Avatar"), &DpGuildMember::GetAvatar, &DpGuildMember::SetAvatar)
        .Prop(_SC("CommunicationDisabledUntil"), &DpGuildMember::GetCommunicationDisabledUntil, &DpGuildMember::SetCommunicationDisabledUntil)
        .Prop(_SC("JoinedAt"), &DpGuildMember::GetJoinedAt)
        .Prop(_SC("PremiumSince"), &DpGuildMember::GetPremiumSince)
        .Prop(_SC("Flags"), &DpGuildMember::GetFlags, &DpGuildMember::SetFlags)
        .Prop(_SC("CommunicationDisabled"), &DpGuildMember::IsCommunicationDisabled)
        .Prop(_SC("Deaf"), &DpGuildMember::GetDeaf, &DpGuildMember::SetDeaf)
        .Prop(_SC("Muted"), &DpGuildMember::GetMuted, &DpGuildMember::SetMuted)
        .Prop(_SC("IsPending"), &DpGuildMember::IsPending)
        .Prop(_SC("HasAnimatedGuildAvatar"), &DpGuildMember::HasAnimatedGuildAvatar)
        .Prop(_SC("Mention"), &DpGuildMember::GetMention)
        // Member Methods
        .Func(_SC("BuildJSON"), &DpGuildMember::BuildJSON_)
        .Func(_SC("GetNickname"), &DpGuildMember::ApplyNickname)
        .Func(_SC("AddRole"), &DpGuildMember::AddRole)
        .Func(_SC("EachRole"), &DpGuildMember::EachRole)
        .Func(_SC("ClearRoles"), &DpGuildMember::ClearRoles)
        .Func(_SC("FilterRoles"), &DpGuildMember::FilterRoles)
        .Func(_SC("GetFlags"), &DpGuildMember::ApplyFlags)
        .Func(_SC("GetAvatarURL"), &DpGuildMember::GetAvatarURL)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Guild"),
        Class< DpGuild, NoConstructor< DpGuild > >(vm, SqDpGuild::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpGuild::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpGuild::IsValid)
        .Prop(_SC("JSON"), &DpGuild::BuildJSON)
        .Prop(_SC("Name"), &DpGuild::GetName, &DpGuild::SetName)
        .Prop(_SC("Description"), &DpGuild::GetDescription, &DpGuild::SetDescription)
        // Member Methods
        .Func(_SC("BuildJSON"), &DpGuild::BuildJSON_)
        .Func(_SC("GetName"), &DpGuild::ApplyName)
        .Func(_SC("GetDescription"), &DpGuild::ApplyDescription)
    );
}

} // Namespace:: SqMod
