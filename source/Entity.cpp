#include "Entity.hpp"
#include "Register.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
EBlipCreated & GBlipCreated() noexcept
{
    return _Core->BlipCreated;
}

ECheckpointCreated & GCheckpointCreated() noexcept
{
    return _Core->CheckpointCreated;
}

EKeybindCreated & GKeybindCreated() noexcept
{
    return _Core->KeybindCreated;
}

EObjectCreated & GObjectCreated() noexcept
{
    return _Core->ObjectCreated;
}

EPickupCreated & GPickupCreated() noexcept
{
    return _Core->PickupCreated;
}

EPlayerCreated & GPlayerCreated() noexcept
{
    return _Core->PlayerCreated;
}

ESphereCreated & GSphereCreated() noexcept
{
    return _Core->SphereCreated;
}

ESpriteCreated & GSpriteCreated() noexcept
{
    return _Core->SpriteCreated;
}

ETextdrawCreated & GTextdrawCreated() noexcept
{
    return _Core->TextdrawCreated;
}

EVehicleCreated & GVehicleCreated() noexcept
{
    return _Core->VehicleCreated;
}

// ------------------------------------------------------------------------------------------------
EBlipDestroyed & GBlipDestroyed() noexcept
{
    return _Core->BlipDestroyed;
}

ECheckpointDestroyed & GCheckpointDestroyed() noexcept
{
    return _Core->CheckpointDestroyed;
}

EKeybindDestroyed & GKeybindDestroyed() noexcept
{
    return _Core->KeybindDestroyed;
}

EObjectDestroyed & GObjectDestroyed() noexcept
{
    return _Core->ObjectDestroyed;
}

EPickupDestroyed & GPickupDestroyed() noexcept
{
    return _Core->PickupDestroyed;
}

EPlayerDestroyed & GPlayerDestroyed() noexcept
{
    return _Core->PlayerDestroyed;
}

ESphereDestroyed & GSphereDestroyed() noexcept
{
    return _Core->SphereDestroyed;
}

ESpriteDestroyed & GSpriteDestroyed() noexcept
{
    return _Core->SpriteDestroyed;
}

ETextdrawDestroyed & GTextdrawDestroyed() noexcept
{
    return _Core->TextdrawDestroyed;
}

EVehicleDestroyed & GVehicleDestroyed() noexcept
{
    return _Core->VehicleDestroyed;
}

// ------------------------------------------------------------------------------------------------
EBlipCustom & GBlipCustom() noexcept
{
    return _Core->BlipCustom;
}

ECheckpointCustom & GCheckpointCustom() noexcept
{
    return _Core->CheckpointCustom;
}

EKeybindCustom & GKeybindCustom() noexcept
{
    return _Core->KeybindCustom;
}

EObjectCustom & GObjectCustom() noexcept
{
    return _Core->ObjectCustom;
}

EPickupCustom & GPickupCustom() noexcept
{
    return _Core->PickupCustom;
}

EPlayerCustom & GPlayerCustom() noexcept
{
    return _Core->PlayerCustom;
}

ESphereCustom & GSphereCustom() noexcept
{
    return _Core->SphereCustom;
}

ESpriteCustom & GSpriteCustom() noexcept
{
    return _Core->SpriteCustom;
}

ETextdrawCustom & GTextdrawCustom() noexcept
{
    return _Core->TextdrawCustom;
}

EVehicleCustom & GVehicleCustom() noexcept
{
    return _Core->VehicleCustom;
}

// ------------------------------------------------------------------------------------------------
bool Register_Entity(HSQUIRRELVM vm)
{
    return true;
}

} // Namespace:: SqMod
