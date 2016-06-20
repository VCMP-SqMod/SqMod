// ------------------------------------------------------------------------------------------------
#include "Core.hpp"

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

// --------------------------------------------------------------------------------------------
void Core::ImportBlips()
{
    // Information about the blip entity
    Int32 world = -1, scale = -1, sprid = -1;
    Uint32 color = 0;
    Float32 x = 0.0, y = 0.0, z = 0.0;

    for (Int32 i = 0; i < SQMOD_BLIP_POOL; ++i)
    {
        // See if this entity exists on the server and whether was not allocated already
        if (_Func->CheckEntityExists(vcmpEntityPoolBlip, i) && INVALID_ENTITY(m_Blips[i].mID))
        {
            _Func->GetCoordBlipInfo(i, &world, &x, &y, &z, &scale, &color, &sprid);
            // Make the properties available before triggering the event
            m_Blips[i].mWorld = world;
            m_Blips[i].mScale = scale;
            m_Blips[i].mSprID = sprid;
            m_Blips[i].mPosition.Set(x, y, z);
            m_Blips[i].mColor.SetRGBA(color);
            // Attempt to allocate the instance
            AllocBlip(i, false, SQMOD_CREATE_IMPORT, NullObject());
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Core::ImportCheckpoints()
{
    for (Int32 i = 0; i < SQMOD_CHECKPOINT_POOL; ++i)
    {
        // See if this entity exists on the server and whether was not allocated already
        if (_Func->CheckEntityExists(vcmpEntityPoolCheckPoint, i) && INVALID_ENTITY(m_Checkpoints[i].mID))
        {
            AllocCheckpoint(i, false, SQMOD_CREATE_IMPORT, NullObject());
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Core::ImportKeybinds()
{
    // Information about the key-bind entity
    Uint8 release = 0;
    Int32 first = -1, second = -1, third = -1;

    for (Int32 i = 0; i < SQMOD_KEYBIND_POOL; ++i)
    {
        // See if this entity exists on the server and whether was not allocated already
        if ((_Func->GetKeyBindData(i, &release, &first, &second, &third) != vcmpErrorNoSuchEntity)
            && (INVALID_ENTITY(m_Keybinds[i].mID)))
        {
            // Make the properties available before triggering the event
            m_Keybinds[i].mFirst = first;
            m_Keybinds[i].mSecond = second;
            m_Keybinds[i].mThird = third;
            m_Keybinds[i].mRelease = release;
            // Attempt to allocate the instance
            AllocKeybind(i, false, SQMOD_CREATE_IMPORT, NullObject());
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Core::ImportObjects()
{
    for (Int32 i = 0; i < SQMOD_OBJECT_POOL; ++i)
    {
        // See if this entity exists on the server and whether was not allocated already
        if (_Func->CheckEntityExists(vcmpEntityPoolObject, i) && INVALID_ENTITY(m_Objects[i].mID))
        {
            AllocObject(i, false, SQMOD_CREATE_IMPORT, NullObject());
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Core::ImportPickups()
{
    for (Int32 i = 0; i < SQMOD_PICKUP_POOL; ++i)
    {
        // See if this entity exists on the server and whether was not allocated already
        if (_Func->CheckEntityExists(vcmpEntityPoolPickup, i) && (INVALID_ENTITY(m_Pickups[i].mID)))
        {
            AllocPickup(i, false, SQMOD_CREATE_IMPORT, NullObject());
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Core::ImportPlayers()
{
    for (Int32 i = 0; i < SQMOD_PLAYER_POOL; ++i)
    {
        // See if this entity exists on the server and whether was not allocated already
        if (_Func->IsPlayerConnected(i) && (INVALID_ENTITY(m_Players[i].mID)))
        {
            ConnectPlayer(i, SQMOD_CREATE_IMPORT, NullObject());
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Core::ImportVehicles()
{
    for (Int32 i = 0; i < SQMOD_VEHICLE_POOL; ++i)
    {
        // See if this entity exists on the server and whether was not allocated already
        if (_Func->CheckEntityExists(vcmpEntityPoolVehicle, i) && INVALID_ENTITY(m_Vehicles[i].mID))
        {
            AllocVehicle(i, false, SQMOD_CREATE_IMPORT, NullObject());
        }
    }
}

// --------------------------------------------------------------------------------------------
Object & Core::AllocBlip(Int32 id, bool owned, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_BLIP_POOL))
    {
        STHROWF("Cannot allocate blip with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    BlipInst & inst = m_Blips[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst.mObj; // Return the existing instance
    }
    // Instantiate the entity manager
    inst.mInst = new CBlip(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a blip instance for: %d", id);
    }
    // Assign the specified entity identifier
    inst.mID = id;
    // Specify whether the entity is owned by this plug-in
    if (owned)
    {
        inst.mFlags |= ENF_OWNED;
    }
    else if (inst.mFlags & ENF_OWNED)
    {
        inst.mFlags ^= ENF_OWNED;
    }
    // Let the script callbacks know about this entity
    EmitBlipCreated(id, header, payload);
    // Return the script object
    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::AllocCheckpoint(Int32 id, bool owned, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_CHECKPOINT_POOL))
    {
        STHROWF("Cannot allocate checkpoint with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    CheckpointInst & inst = m_Checkpoints[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst.mObj; // Return the existing instance
    }
    // Instantiate the entity manager
    inst.mInst = new CCheckpoint(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a checkpoint instance for: %d", id);
    }
    // Assign the specified entity identifier
    inst.mID = id;
    // Specify whether the entity is owned by this plug-in
    if (owned)
    {
        inst.mFlags |= ENF_OWNED;
    }
    else if (inst.mFlags & ENF_OWNED)
    {
        inst.mFlags ^= ENF_OWNED;
    }
    // Let the script callbacks know about this entity
    EmitCheckpointCreated(id, header, payload);
    // Return the script object
    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::AllocKeybind(Int32 id, bool owned, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_KEYBIND_POOL))
    {
        STHROWF("Cannot allocate keybind with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    KeybindInst & inst = m_Keybinds[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst.mObj; // Return the existing instance
    }
    // Instantiate the entity manager
    inst.mInst = new CKeybind(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a keybind instance for: %d", id);
    }
    // Assign the specified entity identifier
    inst.mID = id;
    // Specify whether the entity is owned by this plug-in
    if (owned)
    {
        inst.mFlags |= ENF_OWNED;
    }
    else if (inst.mFlags & ENF_OWNED)
    {
        inst.mFlags ^= ENF_OWNED;
    }
    // Let the script callbacks know about this entity
    EmitKeybindCreated(id, header, payload);
    // Return the script object
    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::AllocObject(Int32 id, bool owned, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_OBJECT_POOL))
    {
        STHROWF("Cannot allocate object with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    ObjectInst & inst = m_Objects[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst.mObj; // Return the existing instance
    }
    // Instantiate the entity manager
    inst.mInst = new CObject(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a object instance for: %d", id);
    }
    // Assign the specified entity identifier
    inst.mID = id;
    // Specify whether the entity is owned by this plug-in
    if (owned)
    {
        inst.mFlags |= ENF_OWNED;
    }
    else if (inst.mFlags & ENF_OWNED)
    {
        inst.mFlags ^= ENF_OWNED;
    }
    // Let the script callbacks know about this entity
    EmitObjectCreated(id, header, payload);
    // Return the script object
    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::AllocPickup(Int32 id, bool owned, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_PICKUP_POOL))
    {
        STHROWF("Cannot allocate pickup with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    PickupInst & inst = m_Pickups[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst.mObj; // Return the existing instance
    }
    // Instantiate the entity manager
    inst.mInst = new CPickup(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a pickup instance for: %d", id);
    }
    // Assign the specified entity identifier
    inst.mID = id;
    // Specify whether the entity is owned by this plug-in
    if (owned)
    {
        inst.mFlags |= ENF_OWNED;
    }
    else if (inst.mFlags & ENF_OWNED)
    {
        inst.mFlags ^= ENF_OWNED;
    }
    // Let the script callbacks know about this entity
    EmitPickupCreated(id, header, payload);
    // Return the script object
    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::AllocVehicle(Int32 id, bool owned, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_VEHICLE_POOL))
    {
        STHROWF("Cannot allocate vehicle with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    VehicleInst & inst = m_Vehicles[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst.mObj; // Return the existing instance
    }
    // Instantiate the entity manager
    inst.mInst = new CVehicle(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a vehicle instance for: %d", id);
    }
    // Assign the specified entity identifier
    inst.mID = id;
    // Specify whether the entity is owned by this plug-in
    if (owned)
    {
        inst.mFlags |= ENF_OWNED;
    }
    else if (inst.mFlags & ENF_OWNED)
    {
        inst.mFlags ^= ENF_OWNED;
    }
    // Let the script callbacks know about this entity
    EmitVehicleCreated(id, header, payload);
    // Return the script object
    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
void Core::DeallocBlip(Int32 id, bool destroy, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_BLIP_POOL))
    {
        STHROWF("Cannot deallocate blip with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    BlipInst & inst = m_Blips[id];
    // Make sure that the instance is even allocated
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const BitGuardU16 bg(inst.mFlags, static_cast< Uint16 >(ENF_LOCKED));
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitBlipDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was probably dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
    }
    // Should we delete this entity from the server as well?
    if (destroy || (inst.mFlags & ENF_OWNED))
    {
        _Func->DestroyCoordBlip(inst.mID);
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

// --------------------------------------------------------------------------------------------
void Core::DeallocCheckpoint(Int32 id, bool destroy, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_CHECKPOINT_POOL))
    {
        STHROWF("Cannot deallocate checkpoint with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    CheckpointInst & inst = m_Checkpoints[id];
    // Make sure that the instance is even allocated
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const BitGuardU16 bg(inst.mFlags, static_cast< Uint16 >(ENF_LOCKED));
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitCheckpointDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was probably dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
    }
    // Should we delete this entity from the server as well?
    if (destroy || (inst.mFlags & ENF_OWNED))
    {
        _Func->DeleteCheckPoint(inst.mID);
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

// --------------------------------------------------------------------------------------------
void Core::DeallocKeybind(Int32 id, bool destroy, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_KEYBIND_POOL))
    {
        STHROWF("Cannot deallocate keybind with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    KeybindInst & inst = m_Keybinds[id];
    // Make sure that the instance is even allocated
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const BitGuardU16 bg(inst.mFlags, static_cast< Uint16 >(ENF_LOCKED));
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitBlipDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
    }
    // Should we delete this entity from the server as well?
    if (destroy || (inst.mFlags & ENF_OWNED))
    {
        _Func->RemoveKeyBind(inst.mID);
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

// --------------------------------------------------------------------------------------------
void Core::DeallocObject(Int32 id, bool destroy, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_OBJECT_POOL))
    {
        STHROWF("Cannot deallocate object with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    ObjectInst & inst = m_Objects[id];
    // Make sure that the instance is even allocated
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const BitGuardU16 bg(inst.mFlags, static_cast< Uint16 >(ENF_LOCKED));
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitObjectDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was probably dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
    }
    // Should we delete this entity from the server as well?
    if (destroy || (inst.mFlags & ENF_OWNED))
    {
        _Func->DeleteObject(inst.mID);
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

// --------------------------------------------------------------------------------------------
void Core::DeallocPickup(Int32 id, bool destroy, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_PICKUP_POOL))
    {
        STHROWF("Cannot deallocate pickup with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    PickupInst & inst = m_Pickups[id];
    // Make sure that the instance is even allocated
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const BitGuardU16 bg(inst.mFlags, static_cast< Uint16 >(ENF_LOCKED));
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitPickupDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was probably dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
    }
    // Should we delete this entity from the server as well?
    if (destroy || (inst.mFlags & ENF_OWNED))
    {
        _Func->DeletePickup(inst.mID);
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

// --------------------------------------------------------------------------------------------
void Core::DeallocVehicle(Int32 id, bool destroy, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_VEHICLE_POOL))
    {
        STHROWF("Cannot deallocate vehicle with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    VehicleInst & inst = m_Vehicles[id];
    // Make sure that the instance is even allocated
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const BitGuardU16 bg(inst.mFlags, static_cast< Uint16 >(ENF_LOCKED));
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitVehicleDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was probably dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
    }
    // Should we delete this entity from the server as well?
    if (destroy || (inst.mFlags & ENF_OWNED))
    {
        _Func->DeleteVehicle(inst.mID);
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

// --------------------------------------------------------------------------------------------
Object & Core::NewBlip(Int32 index, Int32 world, Float32 x, Float32 y, Float32 z,
                            Int32 scale, Uint32 color, Int32 sprid,
                            Int32 header, Object & payload)
{
    // Request the server to create this entity
    const Int32 id = _Func->CreateCoordBlip(index, world, x, y, z, scale, color, sprid);
    // See if the entity creation failed on the server
    if (_Func->GetLastError() == vcmpErrorPoolExhausted)
    {
        STHROWF("Blip pool was exhausted: %d", id);
    }
    // Validate the identifier returned by the server
    else if (INVALID_ENTITYEX(id, SQMOD_BLIP_POOL))
    {
        STHROWF("Server returned invalid blip: %d", id);
    }
    // Attempt to allocate this entity and return the result
    return AllocBlip(id, true, header, payload);
}

// --------------------------------------------------------------------------------------------
Object & Core::NewCheckpoint(Int32 player, Int32 world, bool sphere, Float32 x, Float32 y, Float32 z,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a, Float32 radius,
                                Int32 header, Object & payload)
{
    // Request the server to create this entity
    const Int32 id = _Func->CreateCheckPoint(player, world, sphere, x, y, z, r, g, b, a, radius);
    // See if the entity creation failed on the server
    if (_Func->GetLastError() == vcmpErrorNoSuchEntity)
    {
        STHROWF("Invalid player reference: %d", player);
    }
    else if (_Func->GetLastError() == vcmpErrorPoolExhausted)
    {
        STHROWF("Checkpoint pool was exhausted: %d", id);
    }
    // Validate the identifier returned by the server
    else if (INVALID_ENTITYEX(id, SQMOD_CHECKPOINT_POOL))
    {
        STHROWF("Server returned invalid checkpoint: %d", id);
    }
    // Attempt to allocate this entity and return the result
    return AllocCheckpoint(id, true, header, payload);
}

// --------------------------------------------------------------------------------------------
Object & Core::NewKeybind(Int32 slot, bool release, Int32 primary, Int32 secondary, Int32 alternative,
                            Int32 header, Object & payload)
{
    // Should we obtain a new keybind slot automatically?
    if (slot < 0)
    {
        slot = _Func->GetKeyBindUnusedSlot();
    }
    // Validate the keybind slot
    if (slot < 0)
    {
        STHROWF("Out of keybind slots");
    }
    // Request the server to create this entity
    const Int32 id = _Func->RegisterKeyBind(slot, release, primary, secondary, alternative);
    // See if the entity creation failed on the server
    if (_Func->GetLastError() == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Out of bounds keybind argument: %d", id);
    }
    // Validate the identifier returned by the server
    else if (INVALID_ENTITYEX(id, SQMOD_KEYBIND_POOL))
    {
        STHROWF("Server returned invalid keybind: %d", id);
    }
    // Attempt to allocate this entity and return the result
    return AllocKeybind(id, true, header, payload);
}

// --------------------------------------------------------------------------------------------
Object & Core::NewObject(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z,  Int32 alpha,
                            Int32 header, Object & payload)
{
    // Request the server to create this entity
    const Int32 id = _Func->CreateObject(model, world, x, y, z, alpha);
    // See if the entity creation failed on the server
    if (_Func->GetLastError() == vcmpErrorPoolExhausted)
    {
        STHROWF("Object pool was exhausted: %d", id);
    }
    // Validate the identifier returned by the server
    else if (INVALID_ENTITYEX(id, SQMOD_OBJECT_POOL))
    {
        STHROWF("Server returned invalid object: %d", id);
    }
    // Attempt to allocate this entity and return the result
    return AllocObject(id, true, header, payload);
}

// --------------------------------------------------------------------------------------------
Object & Core::NewPickup(Int32 model, Int32 world, Int32 quantity,
                            Float32 x, Float32 y, Float32 z, Int32 alpha, bool automatic,
                            Int32 header, Object & payload)
{
    // Request the server to create this entity
    const Int32 id = _Func->CreatePickup(model, world, quantity, x, y, z, alpha, automatic);
    // See if the entity creation failed on the server
    if (_Func->GetLastError() == vcmpErrorPoolExhausted)
    {
        STHROWF("Pickup pool was exhausted: %d", id);
    }
    // Validate the identifier returned by the server
    else if (INVALID_ENTITYEX(id, SQMOD_PICKUP_POOL))
    {
        STHROWF("Server returned invalid pickup: %d", id);
    }
    // Attempt to allocate this entity and return the result
    return AllocPickup(id, true, header, payload);
}

// --------------------------------------------------------------------------------------------
Object & Core::NewVehicle(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z,
                            Float32 angle, Int32 primary, Int32 secondary,
                            Int32 header, Object & payload)
{
    // Request the server to create this entity
    const Int32 id = _Func->CreateVehicle(model, world, x, y, z, angle, primary, secondary);
    // See if the entity creation failed on the server
    if (_Func->GetLastError() == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Out of bounds vehicle argument: %d", id);
    }
    else if (_Func->GetLastError() == vcmpErrorPoolExhausted)
    {
        STHROWF("Vehicle pool was exhausted: %d", id);
    }
    // Validate the identifier returned by the server
    else if (INVALID_ENTITYEX(id, SQMOD_VEHICLE_POOL))
    {
        STHROWF("Server returned invalid vehicle: %d", id);
    }
    // Attempt to allocate this entity and return the result
    return AllocVehicle(id, true, header, payload);
}

// --------------------------------------------------------------------------------------------
bool Core::DelBlip(Int32 id, Int32 header, Object & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocBlip(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DelCheckpoint(Int32 id, Int32 header, Object & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocCheckpoint(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DelKeybind(Int32 id, Int32 header, Object & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocKeybind(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DelObject(Int32 id, Int32 header, Object & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocObject(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DelPickup(Int32 id, Int32 header, Object & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocPickup(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DelVehicle(Int32 id, Int32 header, Object & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocVehicle(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

// --------------------------------------------------------------------------------------------
void Core::ConnectPlayer(Int32 id, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_PLAYER_POOL))
    {
        STHROWF("Cannot allocate player with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    PlayerInst & inst = m_Players[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return; // Nothing to allocate!
    }
    // Instantiate the entity manager
    inst.mInst = new CPlayer(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a player instance for: %d", id);
    }
    // Assign the specified entity identifier
    inst.mID = id;
    // Initialize the position
    _Func->GetPlayerPosition(id, &inst.mLastPosition.x, &inst.mLastPosition.y, &inst.mLastPosition.z);
    // Initialize the remaining attributes
    inst.mLastWeapon = _Func->GetPlayerWeapon(id);
    inst.mLastHealth = _Func->GetPlayerHealth(id);
    inst.mLastArmour = _Func->GetPlayerArmour(id);
    inst.mLastHeading = _Func->GetPlayerHeading(id);
    // Let the script callbacks know about this entity
    EmitPlayerCreated(id, header, payload);
}

void Core::DisconnectPlayer(Int32 id, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_PLAYER_POOL))
    {
        STHROWF("Cannot deallocate player with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    PlayerInst & inst = m_Players[id];
    // Make sure that the instance is even allocated and we're allowed to deallocate it
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const BitGuardU16 bg(inst.mFlags, static_cast< Uint16 >(ENF_LOCKED));
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitPlayerDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was probably dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
        // Release the used memory buffer
        inst.mInst->m_Buffer.ResetAll();
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

} // Namespace:: SqMod
