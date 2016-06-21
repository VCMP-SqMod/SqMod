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

// ------------------------------------------------------------------------------------------------
static const Object & Blip_FindBySprID(Int32 sprid)
{
    // Perform a range check on the specified identifier
    if (sprid < 0)
    {
        STHROWF("The specified sprite identifier is invalid: %d", sprid);
    }
    // Obtain the ends of the entity pool
    Core::Blips::const_iterator itr = Core::Get().GetBlips().cbegin();
    Core::Blips::const_iterator end = Core::Get().GetBlips().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does the identifier match the specified one?
        if (itr->mSprID == sprid)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a blip matching the specified identifier
    return NullObject();
}

// ------------------------------------------------------------------------------------------------

// ================================================================================================
void Register(HSQUIRRELVM vm)
{
    Table fns(vm);

    fns.Bind(_SC("Blip"), Table(vm)
        .Func(_SC("WithID"), &Entity< CBlip >::FindByID)
        .Func(_SC("TagEquals"), &Entity< CBlip >::FirstWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CBlip >::FirstWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CBlip >::FirstWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CBlip >::FirstWhereTagContains)
        .Func(_SC("WithSprID"), &Blip_FindBySprID)
    );

    fns.Bind(_SC("Checkpoint"), Table(vm)
        .Func(_SC("WithID"), &Entity< CCheckpoint >::FindByID)
        .Func(_SC("TagEquals"), &Entity< CCheckpoint >::FirstWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CCheckpoint >::FirstWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CCheckpoint >::FirstWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CCheckpoint >::FirstWhereTagContains)
    );

    fns.Bind(_SC("Keybind"), Table(vm)
        .Func(_SC("WithID"), &Entity< CKeybind >::FindByID)
        .Func(_SC("TagEquals"), &Entity< CKeybind >::FirstWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CKeybind >::FirstWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CKeybind >::FirstWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CKeybind >::FirstWhereTagContains)
    );

    fns.Bind(_SC("Object"), Table(vm)
        .Func(_SC("WithID"), &Entity< CObject >::FindByID)
        .Func(_SC("TagEquals"), &Entity< CObject >::FirstWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CObject >::FirstWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CObject >::FirstWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CObject >::FirstWhereTagContains)
    );

    fns.Bind(_SC("Pickup"), Table(vm)
        .Func(_SC("WithID"), &Entity< CPickup >::FindByID)
        .Func(_SC("TagEquals"), &Entity< CPickup >::FirstWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CPickup >::FirstWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CPickup >::FirstWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CPickup >::FirstWhereTagContains)
    );

    fns.Bind(_SC("Player"), Table(vm)
        .Func(_SC("WithID"), &Entity< CPlayer >::FindByID)
        .Func(_SC("TagEquals"), &Entity< CPlayer >::FirstWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CPlayer >::FirstWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CPlayer >::FirstWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CPlayer >::FirstWhereTagContains)
    );

    fns.Bind(_SC("Vehicle"), Table(vm)
        .Func(_SC("WithID"), &Entity< CVehicle >::FindByID)
        .Func(_SC("TagEquals"), &Entity< CVehicle >::FirstWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CVehicle >::FirstWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CVehicle >::FirstWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CVehicle >::FirstWhereTagContains)
    );

    RootTable(vm).Bind(_SC("SqFind"), fns);

    Table cns(vm);

    cns.Bind(_SC("Blip"), Table(vm)
        .Func(_SC("Active"), &Entity< CBlip >::AllActive)
        .Func(_SC("TagEquals"), &Entity< CBlip >::AllWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CBlip >::AllWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CBlip >::AllWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CBlip >::AllWhereTagContains)
    );

    cns.Bind(_SC("Checkpoint"), Table(vm)
        .Func(_SC("Active"), &Entity< CCheckpoint >::AllActive)
        .Func(_SC("TagEquals"), &Entity< CCheckpoint >::AllWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CCheckpoint >::AllWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CCheckpoint >::AllWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CCheckpoint >::AllWhereTagContains)
    );

    cns.Bind(_SC("Keybind"), Table(vm)
        .Func(_SC("Active"), &Entity< CKeybind >::AllActive)
        .Func(_SC("TagEquals"), &Entity< CKeybind >::AllWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CKeybind >::AllWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CKeybind >::AllWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CKeybind >::AllWhereTagContains)
    );

    cns.Bind(_SC("Object"), Table(vm)
        .Func(_SC("Active"), &Entity< CObject >::AllActive)
        .Func(_SC("TagEquals"), &Entity< CObject >::AllWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CObject >::AllWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CObject >::AllWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CObject >::AllWhereTagContains)
    );

    cns.Bind(_SC("Pickup"), Table(vm)
        .Func(_SC("Active"), &Entity< CPickup >::AllActive)
        .Func(_SC("TagEquals"), &Entity< CPickup >::AllWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CPickup >::AllWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CPickup >::AllWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CPickup >::AllWhereTagContains)
    );

    cns.Bind(_SC("Player"), Table(vm)
        .Func(_SC("Active"), &Entity< CPlayer >::AllActive)
        .Func(_SC("TagEquals"), &Entity< CPlayer >::AllWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CPlayer >::AllWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CPlayer >::AllWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CPlayer >::AllWhereTagContains)
    );

    cns.Bind(_SC("Vehicle"), Table(vm)
        .Func(_SC("Active"), &Entity< CVehicle >::AllActive)
        .Func(_SC("TagEquals"), &Entity< CVehicle >::AllWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CVehicle >::AllWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CVehicle >::AllWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CVehicle >::AllWhereTagContains)
    );

    RootTable(vm).Bind(_SC("SqCollect"), cns);

    Table ens(vm);

    ens.Bind(_SC("Blip"), Table(vm)
        .Func(_SC("Active"), &Entity< CBlip >::EachActive)
        .Func(_SC("TagEquals"), &Entity< CBlip >::EachWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CBlip >::EachWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CBlip >::EachWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CBlip >::EachWhereTagContains)
    );

    ens.Bind(_SC("Checkpoint"), Table(vm)
        .Func(_SC("Active"), &Entity< CCheckpoint >::EachActive)
        .Func(_SC("TagEquals"), &Entity< CCheckpoint >::EachWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CCheckpoint >::EachWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CCheckpoint >::EachWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CCheckpoint >::EachWhereTagContains)
    );

    ens.Bind(_SC("Keybind"), Table(vm)
        .Func(_SC("Active"), &Entity< CKeybind >::EachActive)
        .Func(_SC("TagEquals"), &Entity< CKeybind >::EachWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CKeybind >::EachWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CKeybind >::EachWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CKeybind >::EachWhereTagContains)
    );

    ens.Bind(_SC("Object"), Table(vm)
        .Func(_SC("Active"), &Entity< CObject >::EachActive)
        .Func(_SC("TagEquals"), &Entity< CObject >::EachWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CObject >::EachWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CObject >::EachWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CObject >::EachWhereTagContains)
    );

    ens.Bind(_SC("Pickup"), Table(vm)
        .Func(_SC("Active"), &Entity< CPickup >::EachActive)
        .Func(_SC("TagEquals"), &Entity< CPickup >::EachWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CPickup >::EachWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CPickup >::EachWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CPickup >::EachWhereTagContains)
    );

    ens.Bind(_SC("Player"), Table(vm)
        .Func(_SC("Active"), &Entity< CPlayer >::EachActive)
        .Func(_SC("TagEquals"), &Entity< CPlayer >::EachWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CPlayer >::EachWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CPlayer >::EachWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CPlayer >::EachWhereTagContains)
    );

    ens.Bind(_SC("Vehicle"), Table(vm)
        .Func(_SC("Active"), &Entity< CVehicle >::EachActive)
        .Func(_SC("TagEquals"), &Entity< CVehicle >::EachWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CVehicle >::EachWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CVehicle >::EachWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CVehicle >::EachWhereTagContains)
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
