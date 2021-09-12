// ------------------------------------------------------------------------------------------------
#include "Library/DPP.hpp"
#include "Library/DPP/Cluster.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void TerminateDPP()
{
    // Go over all clusters and try to terminate them
    for (DpCluster * inst = DpCluster::sHead; inst && inst->mNext != DpCluster::sHead; inst = inst->mNext)
    {
        inst->Terminate(); // Terminate() the cluster
    }
}

// ------------------------------------------------------------------------------------------------
void ProcessDPP()
{
    // Go over all clusters and allow them to process data
    for (DpCluster * inst = DpCluster::sHead; inst && inst->mNext != DpCluster::sHead; inst = inst->mNext)
    {
        inst->Process();
    }
}

// ------------------------------------------------------------------------------------------------
extern void Register_DPP_Channel(HSQUIRRELVM vm, Table & ns);
extern void Register_DPP_Client(HSQUIRRELVM vm, Table & ns);
extern void Register_DPP_Cluster(HSQUIRRELVM vm, Table & ns);
extern void Register_DPP_Command(HSQUIRRELVM vm, Table & ns);
extern void Register_DPP_Constants(HSQUIRRELVM vm, Table & ns);
extern void Register_DPP_Events(HSQUIRRELVM vm, Table & ns);
extern void Register_DPP_Guild(HSQUIRRELVM vm, Table & ns);
extern void Register_DPP_Integration(HSQUIRRELVM vm, Table & ns);
extern void Register_DPP_Message(HSQUIRRELVM vm, Table & ns);
extern void Register_DPP_Other(HSQUIRRELVM vm, Table & ns);
extern void Register_DPP_Role(HSQUIRRELVM vm, Table & ns);
extern void Register_DPP_User(HSQUIRRELVM vm, Table & ns);

// ================================================================================================
void Register_DPP(HSQUIRRELVM vm)
{
    Table ns(vm);
    // --------------------------------------------------------------------------------------------
    Register_DPP_Constants(vm, ns);
    Register_DPP_Client(vm, ns);
    Register_DPP_Integration(vm, ns);
    Register_DPP_Command(vm, ns);
    Register_DPP_Message(vm, ns);
    Register_DPP_Channel(vm, ns);
    Register_DPP_Other(vm, ns);
    Register_DPP_Role(vm, ns);
    Register_DPP_User(vm, ns);
    Register_DPP_Guild(vm, ns);
    {
        Table ens(vm);
        Register_DPP_Events(vm, ens);
        ns.Bind(_SC("Events"), ens);
    }
    Register_DPP_Cluster(vm, ns);
    // --------------------------------------------------------------------------------------------
    ns.Func(_SC("HasVoice"), dpp::utility::has_voice);
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(_SC("SqDiscord"), ns);
}

} // Namespace:: SqMod
