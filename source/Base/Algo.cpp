// ------------------------------------------------------------------------------------------------
#include "Base/Algo.hpp"

// ------------------------------------------------------------------------------------------------
#include "Entity/Blip.hpp"
#include "Entity/Checkpoint.hpp"
#include "Entity/Keybind.hpp"
#include "Entity/Object.hpp"
#include "Entity/Pickup.hpp"
#include "Entity/Player.hpp"
#include "Entity/Vehicle.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace Algo {

// ================================================================================================
void Register(HSQUIRRELVM vm)
{
    Table fns(vm);

    fns.Bind(_SC("Blip"), Table(vm)
        .Func(_SC("WithID"), &Entity< CBlip >::FindByID)
        .Func(_SC("IfTagEquals"), &Entity< CBlip >::FirstWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CBlip >::FirstWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CBlip >::FirstWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CBlip >::FirstWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CBlip >::FirstWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CBlip >::FirstWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CBlip >::FirstWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CBlip >::FirstWhereTagNotContains)
    );

    fns.Bind(_SC("Checkpoint"), Table(vm)
        .Func(_SC("WithID"), &Entity< CCheckpoint >::FindByID)
        .Func(_SC("IfTagEquals"), &Entity< CCheckpoint >::FirstWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CCheckpoint >::FirstWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CCheckpoint >::FirstWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CCheckpoint >::FirstWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CCheckpoint >::FirstWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CCheckpoint >::FirstWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CCheckpoint >::FirstWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CCheckpoint >::FirstWhereTagNotContains)
    );

    fns.Bind(_SC("Keybind"), Table(vm)
        .Func(_SC("WithID"), &Entity< CKeybind >::FindByID)
        .Func(_SC("IfTagEquals"), &Entity< CKeybind >::FirstWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CKeybind >::FirstWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CKeybind >::FirstWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CKeybind >::FirstWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CKeybind >::FirstWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CKeybind >::FirstWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CKeybind >::FirstWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CKeybind >::FirstWhereTagNotContains)
    );

    fns.Bind(_SC("Object"), Table(vm)
        .Func(_SC("WithID"), &Entity< CObject >::FindByID)
        .Func(_SC("IfTagEquals"), &Entity< CObject >::FirstWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CObject >::FirstWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CObject >::FirstWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CObject >::FirstWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CObject >::FirstWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CObject >::FirstWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CObject >::FirstWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CObject >::FirstWhereTagNotContains)
    );

    fns.Bind(_SC("Pickup"), Table(vm)
        .Func(_SC("WithID"), &Entity< CPickup >::FindByID)
        .Func(_SC("IfTagEquals"), &Entity< CPickup >::FirstWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CPickup >::FirstWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CPickup >::FirstWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CPickup >::FirstWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CPickup >::FirstWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CPickup >::FirstWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CPickup >::FirstWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CPickup >::FirstWhereTagNotContains)
    );

    fns.Bind(_SC("Player"), Table(vm)
        .Func(_SC("WithID"), &Entity< CPlayer >::FindByID)
        .Func(_SC("IfTagEquals"), &Entity< CPlayer >::FirstWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CPlayer >::FirstWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CPlayer >::FirstWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CPlayer >::FirstWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CPlayer >::FirstWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CPlayer >::FirstWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CPlayer >::FirstWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CPlayer >::FirstWhereTagNotContains)
    );

    fns.Bind(_SC("Vehicle"), Table(vm)
        .Func(_SC("WithID"), &Entity< CVehicle >::FindByID)
        .Func(_SC("IfTagEquals"), &Entity< CVehicle >::FirstWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CVehicle >::FirstWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CVehicle >::FirstWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CVehicle >::FirstWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CVehicle >::FirstWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CVehicle >::FirstWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CVehicle >::FirstWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CVehicle >::FirstWhereTagNotContains)
    );

    RootTable(vm).Bind(_SC("SqFind"), fns);

    Table cns(vm);

    cns.Bind(_SC("Blip"), Table(vm)
        .Func(_SC("IfActive"), &Entity< CBlip >::AllActive)
        .Func(_SC("IfTagEquals"), &Entity< CBlip >::AllWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CBlip >::AllWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CBlip >::AllWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CBlip >::AllWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CBlip >::AllWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CBlip >::AllWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CBlip >::AllWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CBlip >::AllWhereTagNotContains)
    );

    cns.Bind(_SC("Checkpoint"), Table(vm)
        .Func(_SC("IfActive"), &Entity< CCheckpoint >::AllActive)
        .Func(_SC("IfTagEquals"), &Entity< CCheckpoint >::AllWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CCheckpoint >::AllWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CCheckpoint >::AllWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CCheckpoint >::AllWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CCheckpoint >::AllWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CCheckpoint >::AllWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CCheckpoint >::AllWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CCheckpoint >::AllWhereTagNotContains)
    );

    cns.Bind(_SC("Keybind"), Table(vm)
        .Func(_SC("IfActive"), &Entity< CKeybind >::AllActive)
        .Func(_SC("IfTagEquals"), &Entity< CKeybind >::AllWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CKeybind >::AllWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CKeybind >::AllWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CKeybind >::AllWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CKeybind >::AllWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CKeybind >::AllWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CKeybind >::AllWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CKeybind >::AllWhereTagNotContains)
    );

    cns.Bind(_SC("Object"), Table(vm)
        .Func(_SC("IfActive"), &Entity< CObject >::AllActive)
        .Func(_SC("IfTagEquals"), &Entity< CObject >::AllWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CObject >::AllWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CObject >::AllWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CObject >::AllWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CObject >::AllWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CObject >::AllWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CObject >::AllWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CObject >::AllWhereTagNotContains)
    );

    cns.Bind(_SC("Pickup"), Table(vm)
        .Func(_SC("IfActive"), &Entity< CPickup >::AllActive)
        .Func(_SC("IfTagEquals"), &Entity< CPickup >::AllWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CPickup >::AllWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CPickup >::AllWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CPickup >::AllWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CPickup >::AllWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CPickup >::AllWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CPickup >::AllWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CPickup >::AllWhereTagNotContains)
    );

    cns.Bind(_SC("Player"), Table(vm)
        .Func(_SC("IfActive"), &Entity< CPlayer >::AllActive)
        .Func(_SC("IfTagEquals"), &Entity< CPlayer >::AllWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CPlayer >::AllWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CPlayer >::AllWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CPlayer >::AllWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CPlayer >::AllWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CPlayer >::AllWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CPlayer >::AllWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CPlayer >::AllWhereTagNotContains)
    );

    cns.Bind(_SC("Vehicle"), Table(vm)
        .Func(_SC("IfActive"), &Entity< CVehicle >::AllActive)
        .Func(_SC("IfTagEquals"), &Entity< CVehicle >::AllWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CVehicle >::AllWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CVehicle >::AllWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CVehicle >::AllWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CVehicle >::AllWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CVehicle >::AllWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CVehicle >::AllWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CVehicle >::AllWhereTagNotContains)
    );

    RootTable(vm).Bind(_SC("SqCollect"), cns);

    Table ens(vm);

    ens.Bind(_SC("Blip"), Table(vm)
        .Func(_SC("IfActive"), &Entity< CBlip >::EachActive)
        .Func(_SC("IfTagEquals"), &Entity< CBlip >::EachWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CBlip >::EachWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CBlip >::EachWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CBlip >::EachWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CBlip >::EachWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CBlip >::EachWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CBlip >::EachWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CBlip >::EachWhereTagNotContains)
    );

    ens.Bind(_SC("Checkpoint"), Table(vm)
        .Func(_SC("IfActive"), &Entity< CCheckpoint >::EachActive)
        .Func(_SC("IfTagEquals"), &Entity< CCheckpoint >::EachWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CCheckpoint >::EachWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CCheckpoint >::EachWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CCheckpoint >::EachWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CCheckpoint >::EachWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CCheckpoint >::EachWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CCheckpoint >::EachWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CCheckpoint >::EachWhereTagNotContains)
    );

    ens.Bind(_SC("Keybind"), Table(vm)
        .Func(_SC("IfActive"), &Entity< CKeybind >::EachActive)
        .Func(_SC("IfTagEquals"), &Entity< CKeybind >::EachWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CKeybind >::EachWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CKeybind >::EachWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CKeybind >::EachWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CKeybind >::EachWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CKeybind >::EachWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CKeybind >::EachWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CKeybind >::EachWhereTagNotContains)
    );

    ens.Bind(_SC("Object"), Table(vm)
        .Func(_SC("IfActive"), &Entity< CObject >::EachActive)
        .Func(_SC("IfTagEquals"), &Entity< CObject >::EachWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CObject >::EachWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CObject >::EachWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CObject >::EachWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CObject >::EachWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CObject >::EachWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CObject >::EachWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CObject >::EachWhereTagNotContains)
    );

    ens.Bind(_SC("Pickup"), Table(vm)
        .Func(_SC("IfActive"), &Entity< CPickup >::EachActive)
        .Func(_SC("IfTagEquals"), &Entity< CPickup >::EachWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CPickup >::EachWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CPickup >::EachWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CPickup >::EachWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CPickup >::EachWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CPickup >::EachWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CPickup >::EachWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CPickup >::EachWhereTagNotContains)
    );

    ens.Bind(_SC("Player"), Table(vm)
        .Func(_SC("IfActive"), &Entity< CPlayer >::EachActive)
        .Func(_SC("IfTagEquals"), &Entity< CPlayer >::EachWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CPlayer >::EachWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CPlayer >::EachWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CPlayer >::EachWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CPlayer >::EachWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CPlayer >::EachWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CPlayer >::EachWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CPlayer >::EachWhereTagNotContains)
    );

    ens.Bind(_SC("Vehicle"), Table(vm)
        .Func(_SC("IfActive"), &Entity< CVehicle >::EachActive)
        .Func(_SC("IfTagEquals"), &Entity< CVehicle >::EachWhereTagEquals)
        .Func(_SC("IfTagNotEquals"), &Entity< CVehicle >::EachWhereTagNotEquals)
        .Func(_SC("IfTagBegins"), &Entity< CVehicle >::EachWhereTagBegins)
        .Func(_SC("IfTagNotBegins"), &Entity< CVehicle >::EachWhereTagNotBegins)
        .Func(_SC("IfTagEnds"), &Entity< CVehicle >::EachWhereTagEnds)
        .Func(_SC("IfTagNotEnds"), &Entity< CVehicle >::EachWhereTagNotEnds)
        .Func(_SC("IfTagContains"), &Entity< CVehicle >::EachWhereTagContains)
        .Func(_SC("IfTagNotContains"), &Entity< CVehicle >::EachWhereTagNotContains)
    );

    RootTable(vm).Bind(_SC("SqForeach"), ens);
}

} // Namespace:: Algo

// ================================================================================================
void Register_Algo(HSQUIRRELVM vm)
{
    Algo::Register(vm);
}

} // Namespace:: SqMod
