// ------------------------------------------------------------------------------------------------
#include "Library/DPP/Guild.hpp"
#include "Library/DPP/Other.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDppGuildMember, _SC("SqDppGuildMember"))
SQMOD_DECL_TYPENAME(SqDppGuild, _SC("SqDppGuild"))

// ------------------------------------------------------------------------------------------------
Table DpGuild::GetMembers() const
{
    Table t(SqVM(), static_cast< SQInteger >(Valid().members.size()));
    // Attempt to convert the [members] associative container into a script table
    const auto r = t.InsertFromMapWith(mPtr->members, [](HSQUIRRELVM vm, auto id, auto & m) -> SQRESULT {
        // The [id] is a dpp::snowflake which is basically a uint64_t so let's leave that as is
        sq_pushinteger(vm, static_cast< SQInteger >(id));
        // Wrap the dpp::guild_member type into a DpGuildMember instance
        ClassType< DpGuildMember >::PushInstance(vm, new DpGuildMember(m));
        // The elements are now on the stack and can be inserted in the table
        return SQ_OK;
    });
    // Did anything fail?
    if (SQ_FAILED(r))
    {
        STHROWF("Unable to convert [members] container");
    }
    // Return the resulted table
    return t;
}

// ------------------------------------------------------------------------------------------------
Table DpGuild::GetVoiceMembers() const
{
    Table t(SqVM(), static_cast< SQInteger >(Valid().voice_members.size()));
    // Attempt to convert the [voice_members] associative container into a script table
    const auto r = t.InsertFromMapWith(mPtr->voice_members, [](HSQUIRRELVM vm, auto id, auto & m) -> SQRESULT {
        // The [id] is a dpp::snowflake which is basically a uint64_t so let's leave that as is
        sq_pushinteger(vm, static_cast< SQInteger >(id));
        // Wrap the dpp::guild_member type into a DpGuildMember instance
        ClassType< DpVoiceState >::PushInstance(vm, new DpVoiceState(m));
        // The elements are now on the stack and can be inserted in the table
        return SQ_OK;
    });
    // Did anything fail?
    if (SQ_FAILED(r))
    {
        STHROWF("Unable to convert [voice_members] container");
    }
    // Return the resulted table
    return t;
}

// ------------------------------------------------------------------------------------------------
void Register_DPP_Guild(HSQUIRRELVM vm, Table & ns)
{
    ns.Bind(_SC("GuildMember"),
        Class< DpGuildMember, NoConstructor< DpGuildMember > >(vm, SqDppGuildMember::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuildMember::Fn)
        .Func(_SC("_tojson"), &DpGuildMember::BuildJSON)
        // Member Properties
        .Prop(_SC("Valid"), &DpGuildMember::IsValid)
        .Prop(_SC("JSON"), &DpGuildMember::BuildJSON)
        .Prop(_SC("Nickname"), &DpGuildMember::GetNickname)
        .Prop(_SC("GuildID"), &DpGuildMember::GetGuildID)
        .Prop(_SC("UserID"), &DpGuildMember::GetUserID)
        .Prop(_SC("JoinedAt"), &DpGuildMember::GetJoinedAt)
        .Prop(_SC("PremiumSince"), &DpGuildMember::GetPremiumSince)
        .Prop(_SC("Flags"), &DpGuildMember::GetFlags)
        .Prop(_SC("IsDeaf"), &DpGuildMember::IsDeaf)
        .Prop(_SC("IsMuted"), &DpGuildMember::IsMuted)
        .Prop(_SC("IsPending"), &DpGuildMember::IsPending)
        // Member Methods
        .Func(_SC("GetRoles"), &DpGuildMember::GetRoles)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Guild"),
        Class< DpGuild, NoConstructor< DpGuild > >(vm, SqDppGuild::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppGuild::Fn)
        .Func(_SC("_tojson"), &DpGuild::BuildJSON)
        // Member Properties
        .Prop(_SC("Valid"), &DpGuild::IsValid)
        .Prop(_SC("JSON"), &DpGuild::BuildJSON)
        .Prop(_SC("ShardID"), &DpGuild::GetShardID)
        .Prop(_SC("Flags"), &DpGuild::GetFlags)
        .Prop(_SC("Name"), &DpGuild::GetName)
        .Prop(_SC("Description"), &DpGuild::GetDescription)
        .Prop(_SC("VanityUrlCode"), &DpGuild::GetVanityUrlCode)
        .Prop(_SC("Icon"), &DpGuild::GetIcon)
        .Prop(_SC("Splash"), &DpGuild::GetSplash)
        .Prop(_SC("DiscoverySplash"), &DpGuild::GetDiscoverySplash)
        .Prop(_SC("OwnerID"), &DpGuild::GetOwnerID)
        .Prop(_SC("VoiceRegion"), &DpGuild::GetVoiceRegion)
        .Prop(_SC("AfkChannelID"), &DpGuild::GetAfkChannelID)
        .Prop(_SC("AfkTimeout"), &DpGuild::GetAfkTimeout)
        .Prop(_SC("WidgetChannelID"), &DpGuild::GetWidgetChannelID)
        .Prop(_SC("VerificationLevel"), &DpGuild::GetVerificationLevel)
        .Prop(_SC("DefaultMessageNotifications"), &DpGuild::GetDefaultMessageNotifications)
        .Prop(_SC("ExplicitContentFilter"), &DpGuild::GetExplicitContentFilter)
        .Prop(_SC("MfaLevel"), &DpGuild::GetMfaLevel)
        .Prop(_SC("ApplicationID"), &DpGuild::GetApplicationID)
        .Prop(_SC("SystemChannelID"), &DpGuild::GetSystemChannelID)
        .Prop(_SC("RulesChannelID"), &DpGuild::GetRulesChannelID)
        .Prop(_SC("MemberCount"), &DpGuild::GetMemberCount)
        .Prop(_SC("Banner"), &DpGuild::GetBanner)
        .Prop(_SC("PremiumTier"), &DpGuild::GetPremiumTier)
        .Prop(_SC("PremiumSubscriptionCount"), &DpGuild::GetPremiumSubscriptionCount)
        .Prop(_SC("PublicUpdatesChannelID"), &DpGuild::GetPublicUpdatesChannelID)
        .Prop(_SC("MaxVideoChannelUsers"), &DpGuild::GetMaxVideoChannelUsers)
        .Prop(_SC("IsLarge"), &DpGuild::IsLarge)
        .Prop(_SC("IsUnavailable"), &DpGuild::IsUnavailable)
        .Prop(_SC("WidgetEnabled"), &DpGuild::WidgetEnabled)
        .Prop(_SC("HasInviteSplash"), &DpGuild::HasInviteSplash)
        .Prop(_SC("HasVipRegions"), &DpGuild::HasVipRegions)
        .Prop(_SC("HasVanityURL"), &DpGuild::HasVanityURL)
        .Prop(_SC("IsVerified"), &DpGuild::IsVerified)
        .Prop(_SC("IsPartnered"), &DpGuild::IsPartnered)
        .Prop(_SC("IsCommunity"), &DpGuild::IsCommunity)
        .Prop(_SC("HasCommerce"), &DpGuild::HasCommerce)
        .Prop(_SC("HasNews"), &DpGuild::HasNews)
        .Prop(_SC("IsDiscoverable"), &DpGuild::IsDiscoverable)
        .Prop(_SC("IsFeatureable"), &DpGuild::IsFeatureable)
        .Prop(_SC("HasAnimatedIcon"), &DpGuild::HasAnimatedIcon)
        .Prop(_SC("BasBanner"), &DpGuild::BasBanner)
        .Prop(_SC("WelcomeScreenEnabled"), &DpGuild::WelcomeScreenEnabled)
        .Prop(_SC("HasMemberVerificationGate"), &DpGuild::HasMemberVerificationGate)
        .Prop(_SC("IsPreviewEnabled"), &DpGuild::IsPreviewEnabled)
        .Prop(_SC("HasAnimatedIconHash"), &DpGuild::HasAnimatedIconHash)
        // Member Methods
        .Func(_SC("BuildJSON"), &DpGuild::BuildJSON_)
        .Func(_SC("GetRoles"), &DpGuild::GetRoles)
        .Func(_SC("GetChannels"), &DpGuild::GetChannels)
        .Func(_SC("GetThreads"), &DpGuild::GetThreads)
        .Func(_SC("GetMembers"), &DpGuild::GetMembers)
        .Func(_SC("GetVoiceMembers"), &DpGuild::GetVoiceMembers)
        .Func(_SC("GetEmojis"), &DpGuild::GetEmojis)
        .Func(_SC("RehashMembers"), &DpGuild::RehashMembers)
        .Func(_SC("ConnectMemberVoice"), &DpGuild::ConnectMemberVoice)
    );
}

} // Namespace:: SqMod
