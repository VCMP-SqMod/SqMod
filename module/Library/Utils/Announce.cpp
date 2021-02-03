// ------------------------------------------------------------------------------------------------
#include "Library/Utils/Announce.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqServerAnnouncer, _SC("SqServerAnnouncer"))

// ------------------------------------------------------------------------------------------------
void AnnounceTerminate()
{
    // Go over all sockets and try to close them
    for (SqAnnouncer * inst = SqAnnouncer::sHead; inst && inst->mNext != SqAnnouncer::sHead; inst = inst->mNext)
    {
        inst->Stop(); // Stop the announcer
    }
}

// ================================================================================================
void Register_ServerAnnouncer(HSQUIRRELVM vm, Table & ns)
{
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Announcer"),
        Class< SqAnnouncer, NoCopy< SqAnnouncer > >(vm, SqServerAnnouncer::Str)
        // Constructors
        .Ctor< StackStrF & >()
        .Ctor< SQInteger, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqServerAnnouncer::Fn)
        // Properties
        .Prop(_SC("Running"), &SqAnnouncer::Running)
        .Prop(_SC("Log"), &SqAnnouncer::GetLog, &SqAnnouncer::SetLog)
        .Prop(_SC("Interval"), &SqAnnouncer::GetInterval, &SqAnnouncer::SetInterval)
        .Prop(_SC("Retries"), &SqAnnouncer::GetRetries, &SqAnnouncer::SetRetries)
        .Prop(_SC("Count"), &SqAnnouncer::GetCount)
        .Prop(_SC("Success"), &SqAnnouncer::GetSuccess)
        .Prop(_SC("Failures"), &SqAnnouncer::GetFailures)
        .Prop(_SC("URI"), &SqAnnouncer::GetURI, &SqAnnouncer::SetURI)
        // Member Methods
        .Func(_SC("Run"), &SqAnnouncer::Run)
        .Func(_SC("Stop"), &SqAnnouncer::Stop)
    );
}

} // Namespace:: SqMod
