// ------------------------------------------------------------------------------------------------
#include "Library/Worker/Job.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(SqBaseJob, _SC("SqBaseJob"))
SQMODE_DECL_TYPENAME(SqEvaluateJob, _SC("SqEvaluateJob"))
SQMODE_DECL_TYPENAME(SqExecuteJob, _SC("SqExecuteJob"))

// ================================================================================================
void Register_Job(HSQUIRRELVM vm)
{
    Table jbns(vm);

    {
        using Type = JobWrapperBase;

        jbns.Bind(_SC("Base"),
            Class< Type, NoConstructor< Type > >(vm, SqBaseJob::Str)
            // Meta-methods
            .SquirrelFunc(_SC("_typename"), &SqBaseJob::Fn)
            // Properties
            .Prop(_SC("Callback"), &Type::GetCallback, &Type::SetCallback)
            // Core Methods
            .CbFunc(_SC("SetCallback"), &Type::SetCallback)
        );
    }

    {
        using Type = JobWrapper< EvaluateJob >;

        jbns.Bind(_SC("Evaluate"),
            DerivedClass< Type, JobWrapperBase, NoCopy< Type > >(vm, SqEvaluateJob::Str)
            // Meta-methods
            .SquirrelFunc(_SC("_typename"), &SqEvaluateJob::Fn)
            // Properties
            .Prop(_SC("Code"), &Type::GetCode, &Type::SetCode)
            .Prop(_SC("Name"), &Type::GetName, &Type::SetName)
            // Core Methods
            .CbFunc(_SC("SetCode"), &Type::SetCode)
            .FmtFunc(_SC("SetName"), &Type::ApplyName)
        );
    }

    {
        using Type = JobWrapper< ExecuteJob >;

        jbns.Bind(_SC("Execute"),
            DerivedClass< Type, JobWrapperBase, NoCopy< Type > >(vm, SqExecuteJob::Str)
            // Meta-methods
            .SquirrelFunc(_SC("_typename"), &SqExecuteJob::Fn)
        );
    }

    RootTable(vm).Bind(_SC("SqJob"), jbns);
}

} // Namespace:: SqMod
