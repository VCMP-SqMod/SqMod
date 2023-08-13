// ------------------------------------------------------------------------------------------------
#include "Library/Discord/User.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDpUser, _SC("SqDiscordUser"))
SQMOD_DECL_TYPENAME(SqDpUserIdentifier, _SC("SqDiscordUser"))

// ------------------------------------------------------------------------------------------------
void Register_Discord_User(HSQUIRRELVM vm, Table & ns)
{
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("User"),
        Class< DpUserIdentifier, NoCopy< DpUser > >(vm, SqDpUser::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpUser::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpUser::IsValid)
        .Prop(_SC("Identified"), &DpUser::IsIdentified)
        .Prop(_SC("JSON"), &DpUser::BuildJSON)
        .Prop(_SC("CreationTime"), &DpUser::GetCreationTime)
        .Prop(_SC("Name"), &DpUser::GetName, &DpUser::SetName)
        .Prop(_SC("Avatar"), &DpUser::GetAvatar, &DpUser::SetAvatar)
        .Prop(_SC("Flags"), &DpUser::GetFlags, &DpUser::SetFlags)
        .Prop(_SC("Discriminator"), &DpUser::GetDiscriminator, &DpUser::SetDiscriminator)
        .Prop(_SC("RefCount"), &DpUser::GetRefCount, &DpUser::SetRefCount)
        .Prop(_SC("AvatarUrl"), &DpUser::GetAvatarUrl)
        .Prop(_SC("DefaultAvatarUrl"), &DpUser::GetDefaultAvatarUrl)
        .Prop(_SC("Mention"), &DpUser::GetMention)
        .Prop(_SC("IsActiveDeveloper"), &DpUser::IsActiveDeveloper)
        .Prop(_SC("IsBot"), &DpUser::IsBot)
        .Prop(_SC("IsSystem"), &DpUser::IsSystem)
        .Prop(_SC("IsMfaEnabled"), &DpUser::IsMfaEnabled)
        .Prop(_SC("IsVerified"), &DpUser::IsVerified)
        .Prop(_SC("HasNitroFull"), &DpUser::HasNitroFull)
        .Prop(_SC("HasNitroClassic"), &DpUser::HasNitroClassic)
        .Prop(_SC("HasNitroBasic"), &DpUser::HasNitroBasic)
        .Prop(_SC("IsDiscordEmployee"), &DpUser::IsDiscordEmployee)
        .Prop(_SC("IsPartneredOwner"), &DpUser::IsPartneredOwner)
        .Prop(_SC("HasHypesquadEvents"), &DpUser::HasHypesquadEvents)
        .Prop(_SC("IsBughunter1"), &DpUser::IsBughunter1)
        .Prop(_SC("IsHouseBravery"), &DpUser::IsHouseBravery)
        .Prop(_SC("IsHouseBrilliance"), &DpUser::IsHouseBrilliance)
        .Prop(_SC("IsHouseBalance"), &DpUser::IsHouseBalance)
        .Prop(_SC("IsEarlySupporter"), &DpUser::IsEarlySupporter)
        .Prop(_SC("IsTeamUser"), &DpUser::IsTeamUser)
        .Prop(_SC("IsBughunter2"), &DpUser::IsBughunter2)
        .Prop(_SC("IsVerifiedBot"), &DpUser::IsVerifiedBot)
        .Prop(_SC("IsVerifiedBotDev"), &DpUser::IsVerifiedBotDev)
        .Prop(_SC("IsCertifiedModerator"), &DpUser::IsCertifiedModerator)
        .Prop(_SC("IsBotHttpInteractions"), &DpUser::IsBotHttpInteractions)
        .Prop(_SC("HasAnimatedIcon"), &DpUser::HasAnimatedIcon)
        // Member Methods
        .Func(_SC("SetName"), &DpUser::ApplyName)
        .Func(_SC("SetFlags"), &DpUser::ApplyFlags)
        .Func(_SC("SetDiscriminator"), &DpUser::ApplyDiscriminator)
        .Func(_SC("BuildJSON"), &DpUser::BuildJSON_)
        .Func(_SC("GetMention"), &DpUser::GetMention)
        .Func(_SC("FormatUsername"), &DpUser::FormatUsername)
        // Member Overloads
        .Overload(_SC("GetAvatarUrl"), &DpUser::GetAvatarUrl)
        .Overload(_SC("GetAvatarUrl"), &DpUser::GetAvatarUrl_1)
        .Overload(_SC("GetAvatarUrl"), &DpUser::GetAvatarUrl_2)
        .Overload(_SC("GetAvatarUrl"), &DpUser::GetAvatarUrl_3)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("UserIdentifier"),
        Class< DpUserIdentifier, NoCopy< DpUserIdentifier > >(vm, SqDpUserIdentifier::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpUserIdentifier::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpUserIdentifier::IsValid)
        .Prop(_SC("Identified"), &DpUserIdentifier::IsIdentified)
        .Prop(_SC("JSON"), &DpUserIdentifier::BuildJSON)
        .Prop(_SC("CreationTime"), &DpUserIdentifier::GetCreationTime)
        .Prop(_SC("Name"), &DpUserIdentifier::GetName, &DpUserIdentifier::SetName)
        .Prop(_SC("Avatar"), &DpUserIdentifier::GetAvatar, &DpUserIdentifier::SetAvatar)
        .Prop(_SC("Flags"), &DpUserIdentifier::GetFlags, &DpUserIdentifier::SetFlags)
        .Prop(_SC("Discriminator"), &DpUserIdentifier::GetDiscriminator, &DpUserIdentifier::SetDiscriminator)
        .Prop(_SC("RefCount"), &DpUserIdentifier::GetRefCount, &DpUserIdentifier::SetRefCount)
        .Prop(_SC("AvatarUrl"), &DpUserIdentifier::GetAvatarUrl)
        .Prop(_SC("DefaultAvatarUrl"), &DpUserIdentifier::GetDefaultAvatarUrl)
        .Prop(_SC("Mention"), &DpUserIdentifier::GetMention)
        .Prop(_SC("IsActiveDeveloper"), &DpUserIdentifier::IsActiveDeveloper)
        .Prop(_SC("IsBot"), &DpUserIdentifier::IsBot)
        .Prop(_SC("IsSystem"), &DpUserIdentifier::IsSystem)
        .Prop(_SC("IsMfaEnabled"), &DpUserIdentifier::IsMfaEnabled)
        .Prop(_SC("IsVerified"), &DpUserIdentifier::IsVerified)
        .Prop(_SC("HasNitroFull"), &DpUserIdentifier::HasNitroFull)
        .Prop(_SC("HasNitroClassic"), &DpUserIdentifier::HasNitroClassic)
        .Prop(_SC("HasNitroBasic"), &DpUserIdentifier::HasNitroBasic)
        .Prop(_SC("IsDiscordEmployee"), &DpUserIdentifier::IsDiscordEmployee)
        .Prop(_SC("IsPartneredOwner"), &DpUserIdentifier::IsPartneredOwner)
        .Prop(_SC("HasHypesquadEvents"), &DpUserIdentifier::HasHypesquadEvents)
        .Prop(_SC("IsBughunter1"), &DpUserIdentifier::IsBughunter1)
        .Prop(_SC("IsHouseBravery"), &DpUserIdentifier::IsHouseBravery)
        .Prop(_SC("IsHouseBrilliance"), &DpUserIdentifier::IsHouseBrilliance)
        .Prop(_SC("IsHouseBalance"), &DpUserIdentifier::IsHouseBalance)
        .Prop(_SC("IsEarlySupporter"), &DpUserIdentifier::IsEarlySupporter)
        .Prop(_SC("IsTeamUser"), &DpUserIdentifier::IsTeamUser)
        .Prop(_SC("IsBughunter2"), &DpUserIdentifier::IsBughunter2)
        .Prop(_SC("IsVerifiedBot"), &DpUserIdentifier::IsVerifiedBot)
        .Prop(_SC("IsVerifiedBotDev"), &DpUserIdentifier::IsVerifiedBotDev)
        .Prop(_SC("IsCertifiedModerator"), &DpUserIdentifier::IsCertifiedModerator)
        .Prop(_SC("IsBotHttpInteractions"), &DpUserIdentifier::IsBotHttpInteractions)
        .Prop(_SC("HasAnimatedIcon"), &DpUserIdentifier::HasAnimatedIcon)
        .Prop(_SC("HasAnimatedBanner"), &DpUserIdentifier::HasAnimatedBanner)
        .Prop(_SC("Banner"), &DpUserIdentifier::GetBanner, &DpUserIdentifier::SetBanner)
        .Prop(_SC("Locale"), &DpUserIdentifier::GetLocale, &DpUserIdentifier::SetLocale)
        .Prop(_SC("Email"), &DpUserIdentifier::GetEmail, &DpUserIdentifier::SetEmail)
        .Prop(_SC("AccentColor"), &DpUserIdentifier::GetAccentColor, &DpUserIdentifier::SetAccentColor)
        .Prop(_SC("Verified"), &DpUserIdentifier::GetVerified, &DpUserIdentifier::SetVerified)
        .Prop(_SC("BannerUrl"), &DpUserIdentifier::GetBannerUrl)
        // Member Methods
        .Func(_SC("SetName"), &DpUserIdentifier::ApplyName)
        .Func(_SC("SetFlags"), &DpUserIdentifier::ApplyFlags)
        .Func(_SC("SetDiscriminator"), &DpUserIdentifier::ApplyDiscriminator)
        .Func(_SC("BuildJSON"), &DpUserIdentifier::BuildJSON_)
        .Func(_SC("GetMention"), &DpUserIdentifier::GetMention)
        .Func(_SC("FormatUsername"), &DpUserIdentifier::FormatUsername)
        .Func(_SC("SetLocale"), &DpUserIdentifier::ApplyLocale)
        .Func(_SC("SetEmail"), &DpUserIdentifier::ApplyEmail)
        .Func(_SC("SetAccentColor"), &DpUserIdentifier::ApplyAccentColor)
        // Member Overloads
        .Overload(_SC("GetAvatarUrl"), &DpUserIdentifier::GetAvatarUrl)
        .Overload(_SC("GetAvatarUrl"), &DpUserIdentifier::GetAvatarUrl_1)
        .Overload(_SC("GetAvatarUrl"), &DpUserIdentifier::GetAvatarUrl_2)
        .Overload(_SC("GetAvatarUrl"), &DpUserIdentifier::GetAvatarUrl_3)
        .Overload(_SC("GetBannerUrl"), &DpUserIdentifier::GetBannerUrl)
        .Overload(_SC("GetBannerUrl"), &DpUserIdentifier::GetBannerUrl_1)
        .Overload(_SC("GetBannerUrl"), &DpUserIdentifier::GetBannerUrl_2)
        .Overload(_SC("GetBannerUrl"), &DpUserIdentifier::GetBannerUrl_3)
    );
}

} // Namespace:: SqMod
