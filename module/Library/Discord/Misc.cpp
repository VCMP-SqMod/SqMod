// ------------------------------------------------------------------------------------------------
#include "Library/Discord/Misc.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDpCachePolicy, _SC("SqDiscordCachePolicy"))
SQMOD_DECL_TYPENAME(SqDpUptime, _SC("SqDiscordUptime"))
SQMOD_DECL_TYPENAME(SqDpIconHash, _SC("SqDiscordIconHash"))
SQMOD_DECL_TYPENAME(SqDpVoiceState, _SC("SqDiscordVoiceState"))
SQMOD_DECL_TYPENAME(SqDpEmoji, _SC("SqDiscordEmoji"))

// ------------------------------------------------------------------------------------------------
void Register_Discord_Misc(HSQUIRRELVM vm, Table & ns)
{
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Uptime"),
        Class< dpp::utility::uptime >(vm, SqDpUptime::Str)
        // Constructors
        .Ctor()
        .Ctor< double >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpUptime::Fn)
        .Func(_SC("_tostring"), &dpp::utility::uptime::to_string)
        // Member Variables
        .Var(_SC("Days"), &dpp::utility::uptime::days)
        .Var(_SC("Hours"), &dpp::utility::uptime::hours)
        .Var(_SC("Minutes"), &dpp::utility::uptime::mins)
        .Var(_SC("Seconds"), &dpp::utility::uptime::secs)
        // Member Methods
        .Func(_SC("ToSeconds"), &dpp::utility::uptime::to_secs)
        .Func(_SC("ToMilliseconds"), &dpp::utility::uptime::to_msecs)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("IconHash"),
        Class< dpp::utility::iconhash >(vm, SqDpIconHash::Str)
        // Constructors
        .Ctor()
        .Ctor< const std::string & >()
        .Ctor< uint64_t, uint64_t >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpIconHash::Fn)
        .Func(_SC("_tostring"), &dpp::utility::iconhash::to_string)
        // Member Variables
        .Var(_SC("High"), &dpp::utility::iconhash::first)
        .Var(_SC("Low"), &dpp::utility::iconhash::second)
        // Member Methods
        .Func(_SC("Set"), &dpp::utility::iconhash::set)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("CachePolicy"),
        Class< DpCachePolicy >(vm, SqDpCachePolicy::Str)
        // Constructors
        .Ctor()
        .Ctor< SQInteger >()
        .Ctor< SQInteger, SQInteger >()
        .Ctor< SQInteger, SQInteger, SQInteger >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpCachePolicy::Fn)
        // Member Variables
        .Var(_SC("UserPolicy"), &DpCachePolicy::mUserPolicy)
        .Var(_SC("EmojiPolicy"), &DpCachePolicy::mEmojiPolicy)
        .Var(_SC("RolePolicy"), &DpCachePolicy::mRolePolicy)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceState"),
        Class< DpVoiceState, NoConstructor< DpVoiceState > >(vm, SqDpVoiceState::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpVoiceState::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpVoiceState::IsValid)
        .Prop(_SC("JSON"), &DpVoiceState::BuildJSON)
        .Prop(_SC("GuildID"), &DpVoiceState::GetGuildID)
        .Prop(_SC("ChannelID"), &DpVoiceState::GetChannelID)
        .Prop(_SC("UserID"), &DpVoiceState::GetUserID)
        .Prop(_SC("SessionID"), &DpVoiceState::GetSessionID)
        .Prop(_SC("Flags"), &DpVoiceState::GetFlags, &DpVoiceState::SetFlags)
        .Prop(_SC("RequestToSpeak"), &DpVoiceState::GetRequestToSpeak)
        .Prop(_SC("IsDeaf"), &DpVoiceState::IsDeaf)
        .Prop(_SC("IsMute"), &DpVoiceState::IsMute)
        .Prop(_SC("IsSelfMute"), &DpVoiceState::IsSelfMute)
        .Prop(_SC("IsSelfDeaf"), &DpVoiceState::IsSelfDeaf)
        .Prop(_SC("SelfStream"), &DpVoiceState::SelfStream)
        .Prop(_SC("SelfVideo"), &DpVoiceState::SelfVideo)
        .Prop(_SC("IsSuppressed"), &DpVoiceState::IsSuppressed)
        // Member Methods
        .Func(_SC("BuildJSON"), &DpVoiceState::BuildJSON_)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Emoji"),
        Class< DpEmoji, NoCopy< DpEmoji > >(vm, SqDpEmoji::Str)
        // Constructors
        .Ctor()
        .Ctor< StackStrF &, dpp::snowflake, SQInteger >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDpEmoji::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpEmoji::IsValid)
        .Prop(_SC("JSON"), &DpEmoji::BuildJSON)
        .Prop(_SC("Name"), &DpEmoji::GetName, &DpEmoji::SetName)
        .Prop(_SC("ImageData"), &DpEmoji::GetImageData)
        .Prop(_SC("RequiresColons"), &DpEmoji::GetRequiresColons)
        .Prop(_SC("IsManaged"), &DpEmoji::GetIsManaged)
        .Prop(_SC("IsAnimated"), &DpEmoji::GetIsAnimated)
        .Prop(_SC("IsAvailable"), &DpEmoji::GetIsAvailable)
        .Prop(_SC("Format"), &DpEmoji::Format)
        .Prop(_SC("Mention"), &DpEmoji::GetMention)
        // Member Methods
        .Func(_SC("SetName"), &DpEmoji::ApplyName)
        .Func(_SC("GetMention"), &DpEmoji::GetMention_)
        .Func(_SC("BuildJSON"), &DpEmoji::BuildJSON_)
        .Func(_SC("LoadImage"), &DpEmoji::LoadImage)
    );
}

} // Namespace:: SqMod
