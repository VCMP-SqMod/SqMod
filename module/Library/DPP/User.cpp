// ------------------------------------------------------------------------------------------------
#include "Library/DPP/User.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDppUser, _SC("SqDppUser"))

// ------------------------------------------------------------------------------------------------
void Register_DPP_User(HSQUIRRELVM vm, Table & ns)
{
    ns.Bind(_SC("User"),
        Class< DpUser, NoConstructor< DpUser > >(vm, SqDppUser::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppUser::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpUser::IsValid)
        .Prop(_SC("Username"), &DpUser::GetUsername)
        .Prop(_SC("Discriminator"), &DpUser::GetDiscriminator)
        .Prop(_SC("Avatar"), &DpUser::GetAvatar)
        .Prop(_SC("Flags"), &DpUser::GetFlags)
        .Prop(_SC("RefCount"), &DpUser::GetRefCount)
        .Prop(_SC("AvatarURL"), &DpUser::GetAvatarURL)
        .Prop(_SC("IsBot"), &DpUser::IsBot)
        .Prop(_SC("IsSystem"), &DpUser::IsSystem)
        .Prop(_SC("IsMfaEnabled"), &DpUser::IsMfaEnabled)
        .Prop(_SC("IsVerified"), &DpUser::IsVerified)
        .Prop(_SC("HasNitroFull"), &DpUser::HasNitroFull)
        .Prop(_SC("HasNitroClassic"), &DpUser::HasNitroClassic)
        .Prop(_SC("IsDiscordEmployee"), &DpUser::IsDiscordEmployee)
        .Prop(_SC("IsPartneredOwner"), &DpUser::IsPartneredOwner)
        .Prop(_SC("HasHypesquadEvents"), &DpUser::HasHypesquadEvents)
        .Prop(_SC("IsBughunter1"), &DpUser::IsBughunter1)
        .Prop(_SC("IsHouseBravery"), &DpUser::IsHouseBravery)
        .Prop(_SC("IsHouseBrilliance"), &DpUser::IsHouseBrilliance)
        .Prop(_SC("IsHouseBalanace"), &DpUser::IsHouseBalanace)
        .Prop(_SC("IsEarlySupporter"), &DpUser::IsEarlySupporter)
        .Prop(_SC("IsTeamUser"), &DpUser::IsTeamUser)
        .Prop(_SC("IsBughunter2"), &DpUser::IsBughunter2)
        .Prop(_SC("IsVerifiedBot"), &DpUser::IsVerifiedBot)
        .Prop(_SC("IsVerifiedBotDev"), &DpUser::IsVerifiedBotDev)
        .Prop(_SC("IsCertifiedDoderator"), &DpUser::IsCertifiedDoderator)
        .Prop(_SC("HasAnimatedIcon"), &DpUser::HasAnimatedIcon)
    );
}

} // Namespace:: SqMod
