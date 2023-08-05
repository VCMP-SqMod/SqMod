// ------------------------------------------------------------------------------------------------
#include "Library/Discord.hpp"
#include "Library/Discord/Cluster.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void TerminateDiscord()
{
    // Go over all clusters and try to terminate them
    for (DpCluster * inst = DpCluster::sHead; inst && inst->mNext != DpCluster::sHead; inst = inst->mNext)
    {
        inst->Terminate(); // Terminate the cluster
    }
}

// ------------------------------------------------------------------------------------------------
void ProcessDiscord()
{
    // Go over all clusters and allow them to process data
    for (DpCluster * inst = DpCluster::sHead; inst && inst->mNext != DpCluster::sHead; inst = inst->mNext)
    {
        inst->Process();
    }
}

// ------------------------------------------------------------------------------------------------
extern void Register_Discord_Constants(HSQUIRRELVM vm, Table & ns);
extern void Register_Discord_Events(HSQUIRRELVM vm, Table & ns);
extern void Register_Discord_Misc(HSQUIRRELVM vm, Table & ns);
extern void Register_Discord_Cluster(HSQUIRRELVM vm, Table & ns);

// ================================================================================================
void Register_Discord(HSQUIRRELVM vm)
{
    Table ns(vm);
    // --------------------------------------------------------------------------------------------
    Register_Discord_Constants(vm, ns);
    {
        Table ens(vm);
        Register_Discord_Events(vm, ens);
        ns.Bind(_SC("Event"), ens);
    }
    Register_Discord_Misc(vm, ns);
    Register_Discord_Cluster(vm, ns);
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(_SC("SqDiscord"), ns);
}

} // Namespace:: SqMod
