// ------------------------------------------------------------------------------------------------
#include "Library/Discord/Presence.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDpActivityButton, _SC("SqDiscordActivityButton"))
SQMOD_DECL_TYPENAME(SqDpActivityButtons, _SC("SqDiscordActivityButtons"))
SQMOD_DECL_TYPENAME(SqDpActivityAssets, _SC("SqDiscordActivityAssets"))
SQMOD_DECL_TYPENAME(SqDpActivitySecrets, _SC("SqDiscordActivitySecrets"))
SQMOD_DECL_TYPENAME(SqDpActivity, _SC("SqDiscordActivity"))
SQMOD_DECL_TYPENAME(SqDpActivities, _SC("SqDiscordActivities"))
SQMOD_DECL_TYPENAME(SqDpPresence, _SC("SqDiscordPresence"))

// ------------------------------------------------------------------------------------------------
void Register_Discord_Presence(HSQUIRRELVM vm, Table & ns)
{
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ActivityButton"),
        Class< DpActivityButton, NoCopy< DpActivityButton > >(vm, SqDpActivityButton::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpActivityButton::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpActivityButton::IsValid)
        .Prop(_SC("Label"), &DpActivityButton::GetLabel, &DpActivityButton::SetLabel)
        .Prop(_SC("URL"), &DpActivityButton::GetURL, &DpActivityButton::SetURL)
        // Member Methods
        .Func(_SC("SetLabel"), &DpActivityButton::ApplyLabel)
        .Func(_SC("SetURL"), &DpActivityButton::ApplyURL)
    );
    // --------------------------------------------------------------------------------------------
    Register_Discord_VectorProxy< dpp::activity_button, DpActivityButton, SqDpActivityButton >(vm, ns, _SC("ActivityButtons"));
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ActivityAssets"),
        Class< DpActivityAssets, NoCopy< DpActivityAssets > >(vm, SqDpActivityAssets::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpActivityAssets::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpActivityAssets::IsValid)
        .Prop(_SC("LargeImage"), &DpActivityAssets::GetLargeImage, &DpActivityAssets::SetLargeImage)
        .Prop(_SC("LargeText"), &DpActivityAssets::GetLargeText, &DpActivityAssets::SetLargeText)
        .Prop(_SC("SmallImage"), &DpActivityAssets::GetSmallImage, &DpActivityAssets::SetSmallImage)
        .Prop(_SC("SmallText"), &DpActivityAssets::GetSmallText, &DpActivityAssets::SetSmallText)
        // Member Methods
        .Func(_SC("SetLargeImage"), &DpActivityAssets::ApplyLargeImage)
        .Func(_SC("SetLargeText"), &DpActivityAssets::ApplyLargeText)
        .Func(_SC("SetSmallImage"), &DpActivityAssets::ApplySmallImage)
        .Func(_SC("SetSmallText"), &DpActivityAssets::ApplySmallText)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("ActivitySecrets"),
        Class< DpActivitySecrets, NoCopy< DpActivitySecrets > >(vm, SqDpActivitySecrets::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpActivitySecrets::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpActivitySecrets::IsValid)
        .Prop(_SC("Join"), &DpActivitySecrets::GetJoin, &DpActivitySecrets::SetJoin)
        .Prop(_SC("Spectate"), &DpActivitySecrets::GetSpectate, &DpActivitySecrets::SetSpectate)
        .Prop(_SC("Match"), &DpActivitySecrets::GetMatch, &DpActivitySecrets::SetMatch)
        // Member Methods
        .Func(_SC("SetJoin"), &DpActivitySecrets::ApplyJoin)
        .Func(_SC("SetSpectate"), &DpActivitySecrets::ApplySpectate)
        .Func(_SC("SetMatch"), &DpActivitySecrets::ApplyMatch)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Activity"),
        Class< DpActivity, NoCopy< DpActivity > >(vm, SqDpActivity::Str)
        // Constructors
        .Ctor()
        .Ctor< SQInteger, StackStrF &, StackStrF &, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpActivity::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpActivity::IsValid)
        .Prop(_SC("Name"), &DpActivity::GetName, &DpActivity::SetName)
        .Prop(_SC("State"), &DpActivity::GetState, &DpActivity::SetState)
        .Prop(_SC("Details"), &DpActivity::GetDetails, &DpActivity::SetDetails)
        .Prop(_SC("Assets"), &DpActivity::GetAssets)
        .Prop(_SC("Buttons"), &DpActivity::GetButtons)
        .Prop(_SC("Emoji"), &DpActivity::GetEmoji)
        .Prop(_SC("PartyID"), &DpActivity::GetPartyID)
        .Prop(_SC("CurrentPartySize"), &DpActivity::GetCurrentPartySize)
        .Prop(_SC("MaxPartySize"), &DpActivity::GetMaxPartySize)
        .Prop(_SC("Secrets"), &DpActivity::GetSecrets)
        .Prop(_SC("URL"), &DpActivity::GetURL, &DpActivity::SetURL)
        .Prop(_SC("Type"), &DpActivity::GetType, &DpActivity::SetType)
        .Prop(_SC("CreatedAt"), &DpActivity::GetCreatedAt, &DpActivity::SetCreatedAt)
        .Prop(_SC("Start"), &DpActivity::GetStart, &DpActivity::SetStart)
        .Prop(_SC("End"), &DpActivity::GetEnd, &DpActivity::SetEnd)
        .Prop(_SC("ApplicationID"), &DpActivity::GetApplicationID)
        .Prop(_SC("Flags"), &DpActivity::GetFlags, &DpActivity::SetFlags)
        .Prop(_SC("Instance"), &DpActivity::IsInstance)
        // Member Methods
        .Func(_SC("SetName"), &DpActivity::ApplyName)
        .Func(_SC("SetState"), &DpActivity::ApplyState)
        .Func(_SC("SetDetails"), &DpActivity::ApplyDetails)
        .Func(_SC("SetURL"), &DpActivity::ApplyURL)
        .Func(_SC("SetType"), &DpActivity::ApplyType)
        .Func(_SC("SetCreatedAt"), &DpActivity::ApplyCreatedAt)
        .Func(_SC("SetStart"), &DpActivity::ApplyStart)
        .Func(_SC("SetEnd"), &DpActivity::ApplyEnd)
        .Func(_SC("SetFlags"), &DpActivity::ApplyFlags)
        .Func(_SC("GetLargeAssetURL"), &DpActivity::GetLargeAssetURL)
        .Func(_SC("GetSmallAssetURL"), &DpActivity::GetSmallAssetURL)
    );
    // --------------------------------------------------------------------------------------------
    Register_Discord_VectorProxy< dpp::activity, DpActivity, SqDpActivities >(vm, ns, _SC("Activities"));
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Presence"),
        Class< DpPresence, NoCopy< DpPresence > >(vm, SqDpPresence::Str)
        // Constructors
        .Ctor()
        .Ctor< SQInteger, DpActivity & >()
        .Ctor< SQInteger, SQInteger, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpPresence::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpPresence::IsValid)
        .Prop(_SC("JSON"), &DpPresence::BuildJSON)
        .Prop(_SC("UserID"), &DpPresence::GetUserID, &DpPresence::SetUserID)
        .Prop(_SC("GuildID"), &DpPresence::GetGuildID, &DpPresence::SetGuildID)
        .Prop(_SC("Flags"), &DpPresence::GetFlags, &DpPresence::SetFlags)
        .Prop(_SC("Activities"), &DpPresence::GetActivities)
        .Prop(_SC("DesktopStatus"), &DpPresence::GetDesktopStatus)
        .Prop(_SC("WebStatus"), &DpPresence::GetWebStatus)
        .Prop(_SC("MobileStatus"), &DpPresence::GetMobileStatus)
        .Prop(_SC("Status"), &DpPresence::GetStatus)
        // Member Methods
        .Func(_SC("SetUserID"), &DpPresence::ApplyUserID)
        .Func(_SC("SetGuildID"), &DpPresence::ApplyGuildID)
        .Func(_SC("SetFlags"), &DpPresence::ApplyFlags)
        .Func(_SC("BuildJSON"), &DpVoiceState::BuildJSON_)
    );
}

} // Namespace:: SqMod
