// ------------------------------------------------------------------------------------------------
#include "Library/DPP/Other.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDppCachePolicy, _SC("SqDppCachePolicy"))
SQMOD_DECL_TYPENAME(SqDppIconHash, _SC("SqDppIconHash"))
SQMOD_DECL_TYPENAME(SqDppUptime, _SC("SqDppUptime"))
SQMOD_DECL_TYPENAME(SqDppActivity, _SC("SqDppActivity"))
SQMOD_DECL_TYPENAME(SqDppPresence, _SC("SqDppPresence"))
SQMOD_DECL_TYPENAME(SqDppVoiceState, _SC("SqDppVoiceState"))

// ------------------------------------------------------------------------------------------------
void Register_DPP_Other(HSQUIRRELVM vm, Table & ns)
{
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Uptime"),
        Class< dpp::utility::uptime >(vm, SqDppUptime::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppUptime::Fn)
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
        Class< dpp::utility::iconhash >(vm, SqDppIconHash::Str)
        // Constructors
        .Ctor()
        .Ctor< const std::string & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppIconHash::Fn)
        .Func(_SC("_tostring"), &dpp::utility::iconhash::to_string)
        // Member Variables
        .Var(_SC("High"), &dpp::utility::iconhash::first)
        .Var(_SC("Low"), &dpp::utility::iconhash::second)
        // Member Methods
        .Func(_SC("Set"), &dpp::utility::iconhash::set)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("CachePolicy"),
        Class< DpCachePolicy >(vm, SqDppCachePolicy::Str)
        // Constructors
        .Ctor()
        .Ctor< SQInteger >()
        .Ctor< SQInteger, SQInteger >()
        .Ctor< SQInteger, SQInteger, SQInteger >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppCachePolicy::Fn)
        // Member Variables
        .Var(_SC("UserPolicy"), &DpCachePolicy::mUserPolicy)
        .Var(_SC("EmojiPolicy"), &DpCachePolicy::mEmojiPolicy)
        .Var(_SC("RolePolicy"), &DpCachePolicy::mRolePolicy)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Activity"),
        Class< DpActivity, NoCopy< DpActivity > >(vm, SqDppActivity::Str)
        // Constructors
        .Ctor()
        .Ctor< SQInteger, StackStrF &, StackStrF &, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppActivity::Fn)
        // Member Properties
        .Prop(_SC("Name"), &DpActivity::GetName, &DpActivity::SetName)
        .Prop(_SC("State"), &DpActivity::GetState, &DpActivity::SetState)
        .Prop(_SC("URL"), &DpActivity::GetURL, &DpActivity::SetURL)
        .Prop(_SC("Type"), &DpActivity::GetType, &DpActivity::SetType)
        .Prop(_SC("CreatedAt"), &DpActivity::GetCreatedAt, &DpActivity::SetCreatedAt)
        .Prop(_SC("Start"), &DpActivity::GetStart, &DpActivity::SetStart)
        .Prop(_SC("End"), &DpActivity::GetEnd, &DpActivity::SetEnd)
        // Member Methods
        .Func(_SC("SetName"), &DpActivity::ApplyName)
        .Func(_SC("SetState"), &DpActivity::ApplyState)
        .Func(_SC("SetURL"), &DpActivity::ApplyURL)
        .Func(_SC("SetType"), &DpActivity::ApplyType)
        .Func(_SC("SetCreatedAt"), &DpActivity::ApplyCreatedAt)
        .Func(_SC("SetStart"), &DpActivity::ApplyStart)
        .Func(_SC("SetEnd"), &DpActivity::ApplyEnd)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Presence"),
        Class< DpPresence, NoCopy< DpPresence > >(vm, SqDppPresence::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppPresence::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpPresence::IsValid)
        .Prop(_SC("UserID"), &DpPresence::GetUserID, &DpPresence::SetUserID)
        .Prop(_SC("GuildID"), &DpPresence::GetGuildID, &DpPresence::SetGuildID)
        .Prop(_SC("Flags"), &DpPresence::GetFlags, &DpPresence::SetFlags)
        .Prop(_SC("ActivityCount"), &DpPresence::ActivityCount)
        .Prop(_SC("DesktopStatus"), &DpPresence::GetDesktopStatus)
        .Prop(_SC("WebStatus"), &DpPresence::GetWebStatus)
        .Prop(_SC("MobileStatus"), &DpPresence::GetMobileStatus)
        .Prop(_SC("Status"), &DpPresence::GetStatus)
        // Member Methods
        .Func(_SC("SetUserID"), &DpPresence::ApplyUserID)
        .Func(_SC("SetGuildID"), &DpPresence::ApplyGuildID)
        .Func(_SC("SetFlags"), &DpPresence::ApplyFlags)
        .Func(_SC("AddActivity"), &DpPresence::AddActivity)
        .Func(_SC("EachActivity"), &DpPresence::EachActivity)
        .Func(_SC("ClearActivities"), &DpPresence::ClearActivities)
        .Func(_SC("FilterActivities"), &DpPresence::FilterActivities)
        .Func(_SC("BuildJSON"), &DpPresence::BuildJSON)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("VoiceState"),
        Class< DpVoiceState, NoConstructor< DpVoiceState > >(vm, SqDppVoiceState::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppVoiceState::Fn)
        .Func(_SC("_tojson"), &DpVoiceState::BuildJSON)
        // Member Properties
        .Prop(_SC("Valid"), &DpVoiceState::IsValid)
        .Prop(_SC("JSON"), &DpVoiceState::BuildJSON)
        .Prop(_SC("GuildID"), &DpVoiceState::GetGuildID)
        .Prop(_SC("ChannelID"), &DpVoiceState::GetChannelID)
        .Prop(_SC("UserID"), &DpVoiceState::GetUserID)
        .Prop(_SC("SessionID"), &DpVoiceState::GetSessionID)
        .Prop(_SC("Flags"), &DpVoiceState::GetFlags, &DpVoiceState::SetFlags)
        .Prop(_SC("IsDeaf"), &DpVoiceState::IsDeaf)
        .Prop(_SC("IsMute"), &DpVoiceState::IsMute)
        .Prop(_SC("IsSelfMute"), &DpVoiceState::IsSelfMute)
        .Prop(_SC("IsSelfDeaf"), &DpVoiceState::IsSelfDeaf)
        .Prop(_SC("SelfStream"), &DpVoiceState::SelfStream)
        .Prop(_SC("SelfVideo"), &DpVoiceState::SelfVideo)
        .Prop(_SC("IsSupressed"), &DpVoiceState::IsSupressed)
    );
}

} // Namespace:: SqMod
