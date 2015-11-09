#include "Entity.hpp"
#include "Register.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Ent< CPlayer >::MsgPrefix Ent< CPlayer >::Prefixes;

// ------------------------------------------------------------------------------------------------
SQUint32 Ent< CPlayer >::MessageColor;
SQInt32 Ent< CPlayer >::AnnounceStyle;

// ------------------------------------------------------------------------------------------------
EBlipCreated & GBlipCreated()
{
    return _Core->BlipCreated;
}

ECheckpointCreated & GCheckpointCreated()
{
    return _Core->CheckpointCreated;
}

EKeybindCreated & GKeybindCreated()
{
    return _Core->KeybindCreated;
}

EObjectCreated & GObjectCreated()
{
    return _Core->ObjectCreated;
}

EPickupCreated & GPickupCreated()
{
    return _Core->PickupCreated;
}

EPlayerCreated & GPlayerCreated()
{
    return _Core->PlayerCreated;
}

ESphereCreated & GSphereCreated()
{
    return _Core->SphereCreated;
}

ESpriteCreated & GSpriteCreated()
{
    return _Core->SpriteCreated;
}

ETextdrawCreated & GTextdrawCreated()
{
    return _Core->TextdrawCreated;
}

EVehicleCreated & GVehicleCreated()
{
    return _Core->VehicleCreated;
}

// ------------------------------------------------------------------------------------------------
EBlipDestroyed & GBlipDestroyed()
{
    return _Core->BlipDestroyed;
}

ECheckpointDestroyed & GCheckpointDestroyed()
{
    return _Core->CheckpointDestroyed;
}

EKeybindDestroyed & GKeybindDestroyed()
{
    return _Core->KeybindDestroyed;
}

EObjectDestroyed & GObjectDestroyed()
{
    return _Core->ObjectDestroyed;
}

EPickupDestroyed & GPickupDestroyed()
{
    return _Core->PickupDestroyed;
}

EPlayerDestroyed & GPlayerDestroyed()
{
    return _Core->PlayerDestroyed;
}

ESphereDestroyed & GSphereDestroyed()
{
    return _Core->SphereDestroyed;
}

ESpriteDestroyed & GSpriteDestroyed()
{
    return _Core->SpriteDestroyed;
}

ETextdrawDestroyed & GTextdrawDestroyed()
{
    return _Core->TextdrawDestroyed;
}

EVehicleDestroyed & GVehicleDestroyed()
{
    return _Core->VehicleDestroyed;
}

// ------------------------------------------------------------------------------------------------
EBlipCustom & GBlipCustom()
{
    return _Core->BlipCustom;
}

ECheckpointCustom & GCheckpointCustom()
{
    return _Core->CheckpointCustom;
}

EKeybindCustom & GKeybindCustom()
{
    return _Core->KeybindCustom;
}

EObjectCustom & GObjectCustom()
{
    return _Core->ObjectCustom;
}

EPickupCustom & GPickupCustom()
{
    return _Core->PickupCustom;
}

EPlayerCustom & GPlayerCustom()
{
    return _Core->PlayerCustom;
}

ESphereCustom & GSphereCustom()
{
    return _Core->SphereCustom;
}

ESpriteCustom & GSpriteCustom()
{
    return _Core->SpriteCustom;
}

ETextdrawCustom & GTextdrawCustom()
{
    return _Core->TextdrawCustom;
}

EVehicleCustom & GVehicleCustom()
{
    return _Core->VehicleCustom;
}

// ------------------------------------------------------------------------------------------------
static SQInt32 GetPlayerLevel(SQInt32 id)
{
    return Reference< CPlayer >::Get(id).Level;
} 

// ------------------------------------------------------------------------------------------------
bool Register_Entity(HSQUIRRELVM vm)
{
    // Attempt to bind the namespace to the root table
    Sqrat::RootTable(vm).Func(_SC("GetPlayerLevel"), GetPlayerLevel);
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
