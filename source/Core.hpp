#ifndef _CORE_HPP_
#define _CORE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Signal.hpp"

// ------------------------------------------------------------------------------------------------
#include "Base/Buffer.hpp"
#include "Base/Vector3.hpp"

// ------------------------------------------------------------------------------------------------
#include <list>
#include <vector>
#include <utility>
#include <unordered_map>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * The central core class is supposed to manage the life time of the polugin and it's resources.
*/
class Core
{
    /* --------------------------------------------------------------------------------------------
     * Allow only the smart pointer to delete this class instance as soon as it's not needed.
    */
    friend class std::unique_ptr< Core, void(*)(Core *) >;

protected:

    /* --------------------------------------------------------------------------------------------
     * Helper structure meant to track changes in player instances.
    */
    struct TPlayer
    {
        /* Last used player weapon. */
        SQInt32     Weapon;
        /* Last known player health. */
        SQFloat     Health;
        /* Last known player armour */
        SQFloat     Armour;
        /* Last known player position. */
        Vector3     Position;
        /* Whether this entity is new and must not be check on first update. */
        bool        Fresh;
    };

    /* --------------------------------------------------------------------------------------------
     * Helper structure meant to track changes in vehicle instances.
    */
    struct TVehicle
    {
        /* Last known vehicle health. */
        SQFloat     Health;
        /* Last known vehicle position. */
        Vector3     Position;
        /* Whether this entity is new and must not be check on first update. */
        bool        Fresh;
    };

    /* --------------------------------------------------------------------------------------------
     * An array of player tracking structures for each player instance.
    */
    typedef std::array< TPlayer, SQMOD_PLAYER_POOL >        TPlayerInstPool;

    /* --------------------------------------------------------------------------------------------
     * An array of vehicle tracking structures for each vehicle instance.
    */
    typedef std::array< TVehicle, SQMOD_VEHICLE_POOL >      TVehicleInstPool;

    /* --------------------------------------------------------------------------------------------
     * Reference to all compiled scripts specified in the configuration file. 
    */
    typedef std::unordered_map< String, Script >            SqScriptPool;

    /* --------------------------------------------------------------------------------------------
     * A key->value pair container with arbitrary configuration values.
    */
    typedef std::unordered_map< String, String >            OptionPool;

    /* --------------------------------------------------------------------------------------------
     * A list of available buffers that are shared across the script to avoid frequent allocations.
    */
    typedef std::list< Buffer >                             BufferPool;

private:

    /* --------------------------------------------------------------------------------------------
     * Last known state of the plugin that is to be returned by the event callbacks.
    */
    SQInteger                           m_State;

    /* --------------------------------------------------------------------------------------------
     * All the user defined options in the general section of the configuration file.
    */
    OptionPool                          m_Options;

    /* --------------------------------------------------------------------------------------------
     * The main Squirrel virtual machine instance associated with this class instance.
    */
    HSQUIRRELVM                         m_VM;

    /* --------------------------------------------------------------------------------------------
     * Reference to the root table to be used whenever interaction with the root table is required.
    */
    RootTable                           m_RootTable;

    /* --------------------------------------------------------------------------------------------
     * All the scripts specified in the configuration file in their compiled form.
    */
    SqScriptPool                        m_Scripts;

    /* --------------------------------------------------------------------------------------------
     * Last known error message in the plugin throwing an error at certain stages is not an option.
    */
    String                              m_ErrorMsg;

    /* --------------------------------------------------------------------------------------------
     * An array of instances of the tracking structure for each posible player on the server.
    */
    TPlayerInstPool                     m_PlayerTrack;

    /* --------------------------------------------------------------------------------------------
     * An array of instances of the tracking structure for each posible vehicle on the server.
    */
    TVehicleInstPool                    m_VehicleTrack;

    /* --------------------------------------------------------------------------------------------
     * A pool of shared buffers shared throught the plugin through move semantics.
    */
    BufferPool                          m_BufferPool;

    /* --------------------------------------------------------------------------------------------
     * Server uptime calculated as the sum of delta time between server frames.
    */
    SQFloat                             m_Uptime;

protected:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Core();

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    Core(Core const &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    Core(Core &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Core();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    Core & operator=(Core const &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    Core & operator=(Core &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Called by the smart pointer to delete the instance of this class.
    */
    static void _Finalizer(Core * ptr);

public:

    // --------------------------------------------------------------------------------------------
    typedef std::unique_ptr< Core, void(*)(Core *) >  Pointer;

    /* --------------------------------------------------------------------------------------------
     * Creates an instance of this type if one doesn't already exist and returns it.
    */
    static Pointer Inst();

    /* --------------------------------------------------------------------------------------------
     * Attempt to initialize the plugin subsystems and prepare it for the loading stage.
    */
    bool Init();

    /* --------------------------------------------------------------------------------------------
     * Attempt load the plugin resources and finally startup the plugin.
    */
    bool Load();

    /* --------------------------------------------------------------------------------------------
     * Attempt to deinitialize the plugin subsystems and prepare the plugin for propper shutdown.
    */
    void Deinit();

    /* --------------------------------------------------------------------------------------------
     * Attempt to unload the plugin resouces and release everything from the load stage.
    */
    void Unload();

    /* --------------------------------------------------------------------------------------------
     * Attempt to completely terminate the plugin instance with no intention of starting up again.
    */
    void Terminate();

    /* --------------------------------------------------------------------------------------------
     * Set the current plugin state to the specified value.
    */
    void SetState(SQInteger val);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current plugin state.
    */
    SQInteger GetState() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value associated with the specified option name.
    */
    String GetOption(const String & name) const;

    /* --------------------------------------------------------------------------------------------
     * Change the value associated with the specified name.
    */
    void SetOption(const String & name, const String & value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the plugin/server cumulated uptime.
    */
    SQFloat GetUptime() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a buffer of at least the specified size.
    */
    Buffer PullBuffer(unsigned sz = 4096);

    /* --------------------------------------------------------------------------------------------
     * Return a previously borrwoed buffer back to the pool of buffers.
    */
    void PushBuffer(Buffer && buf);

    /* --------------------------------------------------------------------------------------------
     * Create a collection of buffer with the specified size and add them to the pool.
    */
    void MakeBuffer(unsigned num, unsigned sz = 4096);

    /* --------------------------------------------------------------------------------------------
     * Attempt to activate a speciffic player within the plugin.
    */
    void ConnectPlayer(SQInt32 id, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Attempt to deactivate a specific player withing the plugin.
    */
    void DisconnectPlayer(SQInt32 id, SQInt32 header, SqObj & payload);

protected:

    /* --------------------------------------------------------------------------------------------
     * Attempt to retrieve the values from the configuration file.
    */
    bool Configure();

    /* --------------------------------------------------------------------------------------------
     * Attempt to create a Squirrel Virtual Machine.
    */
    bool CreateVM();

    /* --------------------------------------------------------------------------------------------
     * Attempt to close a Squirrel Virtual Machine and destroy it's resources allong with it.
    */
    void DestroyVM();

    /* --------------------------------------------------------------------------------------------
     * Attempt to load the scripts specified in the configuration file.
    */
    bool LoadScripts();

    /* --------------------------------------------------------------------------------------------
     * Attempt to compile the loaded scripts and store the resulted object.
    */
    bool Compile(const String & name);

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute the previously compiled scripts.
    */
    bool Execute();

    /* --------------------------------------------------------------------------------------------
     * Print debugging information about the current callstack.
    */
    void PrintCallstack();

public:

    /* --------------------------------------------------------------------------------------------
     * Used by the Squirrel VM to outpute text messages to a stream defined by the plugin.
    */
    static void PrintFunc(HSQUIRRELVM vm, const SQChar * str, ...);

    /* --------------------------------------------------------------------------------------------
     * Used by the Squirrel VM to outpute error messages to a stream defined by the plugin.
    */
    static void ErrorFunc(HSQUIRRELVM vm, const SQChar * str, ...);

    /* --------------------------------------------------------------------------------------------
     * A custom error handler defined by the plugin to be invoked when runtime errors occur.
    */
    static SQInteger RuntimeErrorHandler(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * A custom error handler defined by the plugin to be invoked when compile time errors occur.
    */
    static void CompilerErrorHandler(HSQUIRRELVM vm, const SQChar * desc, const SQChar * src,
                                        SQInteger line, SQInteger column);

public:

    /* --------------------------------------------------------------------------------------------
    * Creates a new Blip on the server
    */
    Reference< CBlip > NewBlip(SQInt32 index, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                                SQInt32 scale, SQUint32 color, SQInt32 sprid,
                                SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
    * Creates a new Checkpoint on the server
    */
    Reference< CCheckpoint > NewCheckpoint(SQInt32 player, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                                            Uint8 r, Uint8 g, Uint8 b, Uint8 a, SQFloat radius,
                                            SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
    * Creates a new Keybind on the server
    */
    Reference< CKeybind > NewKeybind(SQInt32 slot, bool release,
                                        SQInt32 primary, SQInt32 secondary, SQInt32 alternative,
                                        SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
    * Creates a new Object on the server
    */
    Reference< CObject > NewObject(SQInt32 model, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                                    SQInt32 alpha,
                                    SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
    * Creates a new Pickup on the server
    */
    Reference< CPickup > NewPickup(SQInt32 model, SQInt32 world, SQInt32 quantity,
                                    SQFloat x, SQFloat y, SQFloat z, SQInt32 alpha, bool automatic,
                                    SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
    * Creates a new Sphere on the server
    */
    Reference< CSphere > NewSphere(SQInt32 player, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                                    Uint8 r, Uint8 g, Uint8 b, SQFloat radius,
                                    SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
    * Creates a new Sprite on the server
    */
    Reference< CSprite > NewSprite(SQInt32 index, const SQChar * file, SQInt32 xp, SQInt32 yp,
                                    SQInt32 xr, SQInt32 yr, SQFloat angle, SQInt32 alpha, bool rel,
                                    SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
    * Creates a new Textdraw on the server
    */
    Reference< CTextdraw > NewTextdraw(SQInt32 index, const SQChar * text, SQInt32 xp, SQInt32 yp,
                                        SQUint32 color, bool rel,
                                        SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
    * Creates a new Vehicle on the server
    */
    Reference< CVehicle > NewVehicle(SQInt32 model, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                                        SQFloat angle, SQInt32 primary, SQInt32 secondary,
                                        SQInt32 header, SqObj & payload);

public:

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the Squirrel Virtual machine is about to be closed.
    */
    void OnVMClose();

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a blip entity instance was created.
    */
    void OnBlipCreated(SQInt32 blip, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a checkpoint entity instance was created.
    */
    void OnCheckpointCreated(SQInt32 checkpoint, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a keybind entity instance was created.
    */
    void OnKeybindCreated(SQInt32 keybind, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a object entity instance was created.
    */
    void OnObjectCreated(SQInt32 object, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a pickup entity instance was created.
    */
    void OnPickupCreated(SQInt32 pickup, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player entity instance was created.
    */
    void OnPlayerCreated(SQInt32 player, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a sphere entity instance was created.
    */
    void OnSphereCreated(SQInt32 sphere, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a sprite entity instance was created.
    */
    void OnSpriteCreated(SQInt32 sprite, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a textdraw entity instance was created.
    */
    void OnTextdrawCreated(SQInt32 textdraw, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a vehicle entity instance was created.
    */
    void OnVehicleCreated(SQInt32 vehicle, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a blip entity instance was destroyed.
    */
    void OnBlipDestroyed(SQInt32 blip, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a checkpoint entity instance was destroyed.
    */
    void OnCheckpointDestroyed(SQInt32 checkpoint, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a keybind entity instance was destroyed.
    */
    void OnKeybindDestroyed(SQInt32 keybind, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a object entity instance was destroyed.
    */
    void OnObjectDestroyed(SQInt32 object, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a pickup entity instance was destroyed.
    */
    void OnPickupDestroyed(SQInt32 pickup, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player entity instance was destroyed.
    */
    void OnPlayerDestroyed(SQInt32 player, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a sphere entity instance was destroyed.
    */
    void OnSphereDestroyed(SQInt32 sphere, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a sprite entity instance was destroyed.
    */
    void OnSpriteDestroyed(SQInt32 sprite, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a textdraw entity instance was destroyed.
    */
    void OnTextdrawDestroyed(SQInt32 textdraw, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a vehicle entity instance was destroyed.
    */
    void OnVehicleDestroyed(SQInt32 vehicle, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a blip entity instance triggered a custom event.
    */
    void OnBlipCustom(SQInt32 blip, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a checkpoint entity instance triggered a custom event.
    */
    void OnCheckpointCustom(SQInt32 checkpoint, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a keybind entity instance triggered a custom event.
    */
    void OnKeybindCustom(SQInt32 keybind, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a object entity instance triggered a custom event.
    */
    void OnObjectCustom(SQInt32 object, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a pickup entity instance triggered a custom event.
    */
    void OnPickupCustom(SQInt32 pickup, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player entity instance triggered a custom event.
    */
    void OnPlayerCustom(SQInt32 player, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a sphere entity instance triggered a custom event.
    */
    void OnSphereCustom(SQInt32 sphere, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a sprite entity instance triggered a custom event.
    */
    void OnSpriteCustom(SQInt32 sprite, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a textdraw entity instance triggered a custom event.
    */
    void OnTextdrawCustom(SQInt32 textdraw, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a vehicle entity instance triggered a custom event.
    */
    void OnVehicleCustom(SQInt32 vehicle, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player changed his away status.
    */
    void OnPlayerAway(SQInt32 player, bool status);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player pressed some game keys.
    */
    void OnPlayerGameKeys(SQInt32 player, SQInt32 previous, SQInt32 current);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player changed his nick name.
    */
    void OnPlayerName(SQInt32 player, const SQChar * previous, const SQChar * current);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player is requesting a speciffic class to be assigned to.
    */
    void OnPlayerRequestClass(SQInt32 player, SQInt32 offset);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player requests to spawn in the game.
    */
    void OnPlayerRequestSpawn(SQInt32 player);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player has spawned in the game.
    */
    void OnPlayerSpawn(SQInt32 player);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player began typing in the chat/console.
    */
    void OnPlayerStartTyping(SQInt32 player);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player stopped typing in the chat/console.
    */
    void OnPlayerStopTyping(SQInt32 player);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player sent a public chat message.
    */
    void OnPlayerChat(SQInt32 player, const SQChar * message);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player sent a server command.
    */
    void OnPlayerCommand(SQInt32 player, const SQChar * command);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player sent a private chat mesage.
    */
    void OnPlayerMessage(SQInt32 player, SQInt32 receiver, const SQChar * message);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the health changed for a speciffic player.
    */
    void OnPlayerHealth(SQInt32 player, SQFloat previous, SQFloat current);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the armour changed for a speciffic player.
    */
    void OnPlayerArmour(SQInt32 player, SQFloat previous, SQFloat current);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player changed his current weapon.
    */
    void OnPlayerWeapon(SQInt32 player, SQInt32 previous, SQInt32 current);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player changed his current location/position.
    */
    void OnPlayerMove(SQInt32 player, const Vector3 & previous, const Vector3 & current);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player died from a natural cause or self inflicted injury.
    */
    void OnPlayerWasted(SQInt32 player, SQInt32 reason);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player was killed by another player.
    */
    void OnPlayerKilled(SQInt32 player, SQInt32 killer, SQInt32 reason, SQInt32 body_part);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player began to spectate another player.
    */
    void OnPlayerSpectate(SQInt32 player, SQInt32 target);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player sent a crash report to the server.
    */
    void OnPlayerCrashreport(SQInt32 player, const SQChar * report);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player is on fire or stopped being on fire.
    */
    void OnPlayerBurning(SQInt32 player, bool state);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player is crouching or stopped crouching.
    */
    void OnPlayerCrouching(SQInt32 player, bool state);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current state of a player has changed.
    */
    void OnPlayerState(SQInt32 player, SQInt32 previous, SQInt32 current);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current action of a player has changed.
    */
    void OnPlayerAction(SQInt32 player, SQInt32 previous, SQInt32 current);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current state of a player has changed to nothing.
    */
    void OnStateNone(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current state of a player has changed to normal.
    */
    void OnStateNormal(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current state of a player has changed to shooting.
    */
    void OnStateShooting(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current state of a player has changed to entered a vehicle
     * as the driver.
    */
    void OnStateDriver(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current state of a player has changed to entered a vehicle
     * as the passenger.
    */
    void OnStatePassenger(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current state of a player has changed to preparing to
     * enter a vehicle as the driver.
    */
    void OnStateEnterDriver(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current state of a player has changed to preparing to
     * enter a vehicle as a passenger.
    */
    void OnStateEnterPassenger(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current state of a player has changed to exiting a vehicle.
    */
    void OnStateExitVehicle(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current state of a player has changed to being unspawned.
    */
    void OnStateUnspawned(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current action of a player has changed to nothing.
    */
    void OnActionNone(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current action of a player has changed to normal.
    */
    void OnActionNormal(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current action of a player has changed to aiming at
     * something.
    */
    void OnActionAiming(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current action of a player has changed to shooting.
    */
    void OnActionShooting(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current action of a player has changed to jumping.
    */
    void OnActionJumping(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current action of a player has changed to lying down.
    */
    void OnActionLieDown(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current action of a player has changed to getting up.
    */
    void OnActionGettingUp(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current action of a player has changed to jumping out
     * of a vehicle.
    */
    void OnActionJumpVehicle(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current action of a player has changed to driving a vehicle.
    */
    void OnActionDriving(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current action of a player has changed to dying after
     * being killed by someone else.
    */
    void OnActionDying(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current action of a player has changed to dying by
     * natural causes or self inflicted injuries.
    */
    void OnActionWasted(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current action of a player has changed to embarking
     * a vehicle.
    */
    void OnActionEmbarking(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the current action of a player has changed to dissembarking
     * a vehicle.
    */
    void OnActionDisembarking(SQInt32 player, SQInt32 previous);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the a vehicle instance has respawned.
    */
    void OnVehicleRespawn(SQInt32 vehicle);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the a vehicle has exploded.
    */
    void OnVehicleExplode(SQInt32 vehicle);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the health changed for a speciffic vehicle.
    */
    void OnVehicleHealth(SQInt32 vehicle, SQFloat previous, SQFloat current);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a vehicle changed its current location/position.
    */
    void OnVehicleMove(SQInt32 vehicle, const Vector3 & previous, const Vector3 & current);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the a pickup instance has respawned.
    */
    void OnPickupRespawn(SQInt32 pickup);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player pressed a listened key combination.
    */
    void OnPlayerKeyPress(SQInt32 player, SQInt32 keybind);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player released a listened key combination.
    */
    void OnPlayerKeyRelease(SQInt32 player, SQInt32 keybind);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player is about to embark in a vehicle.
    */
    void OnPlayerEmbarking(SQInt32 player, SQInt32 vehicle, SQInt32 slot);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player just embarked in a vehicle.
    */
    void OnPlayerEmbarked(SQInt32 player, SQInt32 vehicle, SQInt32 slot);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player just dissembarked from a vehicle.
    */
    void OnPlayerDisembark(SQInt32 player, SQInt32 vehicle);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player claimed owenership over a pickup.
    */
    void OnPickupClaimed(SQInt32 player, SQInt32 pickup);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player colected a pickup.
    */
    void OnPickupCollected(SQInt32 player, SQInt32 pickup);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player just shot a object.
    */
    void OnObjectShot(SQInt32 player, SQInt32 object, SQInt32 weapon);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player just bumped in a object.
    */
    void OnObjectBump(SQInt32 player, SQInt32 object);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player just entered a checkpoint.
    */
    void OnCheckpointEntered(SQInt32 player, SQInt32 checkpoint);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player just exited a checkpoint.
    */
    void OnCheckpointExited(SQInt32 player, SQInt32 checkpoint);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player just entered a sphere.
    */
    void OnSphereEntered(SQInt32 player, SQInt32 sphere);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a player just exited a sphere.
    */
    void OnSphereExited(SQInt32 player, SQInt32 sphere);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the server just finished one cycle of it's main loop.
    */
    void OnServerFrame(SQFloat delta);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the server just started up.
    */
    void OnServerStartup();

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the server is about to shut down.
    */
    void OnServerShutdown();

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the server received an internal command.
    */
    void OnInternalCommand(SQInt32 type, const SQChar * text);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that somone is trying to log into the server.
    */
    void OnLoginAttempt(const SQChar * name, const SQChar * passwd, const SQChar * ip);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that a custom event was just triggered.
    */
    void OnCustomEvent(SQInt32 group, SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that an option just changed in the game world.
    */
    void OnWorldOption(SQInt32 option, SqObj & value);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that something was just toggeled in the game world.
    */
    void OnWorldToggle(SQInt32 option, bool value);

    /* --------------------------------------------------------------------------------------------
     * Notify event listeners that the plugin is about to reload it's resources.
    */
    void OnScriptReload(SQInt32 header, SqObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Process updates for a player instance.
    */
    void OnPlayerUpdate(SQInt32 player, SQInt32 type);

    /* --------------------------------------------------------------------------------------------
     * Process updates for a vehicle instance.
    */
    void OnVehicleUpdate(SQInt32 vehicle, SQInt32 type);

    /* --------------------------------------------------------------------------------------------
     * Process external creation and destruction of entity instances.
    */
    void OnEntityPool(SQInt32 type, SQInt32 id, bool deleted);

public:

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when the Squirrel VM is closed.
    */
    EVMClose                    VMClose;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a blip entity instance was
     * created on the server. Either by this plugin or external ones.
    */
    EBlipCreated                BlipCreated;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a checkpoint entity instance was
     * created on the server. Either by this plugin or external ones.
    */
    ECheckpointCreated          CheckpointCreated;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a keybind entity instance was
     * created on the server. Either by this plugin or external ones.
    */
    EKeybindCreated             KeybindCreated;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a object entity instance was
     * created on the server. Either by this plugin or external ones.
    */
    EObjectCreated              ObjectCreated;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a pickup entity instance was
     * created on the server. Either by this plugin or external ones.
    */
    EPickupCreated              PickupCreated;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a Player entity instance was
     * created on the server. Either by this plugin or external ones.
    */
    EPlayerCreated              PlayerCreated;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a sphere entity instance was
     * created on the server. Either by this plugin or external ones.
    */
    ESphereCreated              SphereCreated;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a sprite entity instance was
     * created on the server. Either by this plugin or external ones.
    */
    ESpriteCreated              SpriteCreated;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a textdraw entity instance was
     * created on the server. Either by this plugin or external ones.
    */
    ETextdrawCreated            TextdrawCreated;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a vehicle entity instance was
     * created on the server. Either by this plugin or external ones.
    */
    EVehicleCreated             VehicleCreated;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a blip entity instance was
     * destroyed from the server. Either by this plugin or external ones.
    */
    EBlipDestroyed              BlipDestroyed;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a checkpoint entity instance was
     * destroyed from the server. Either by this plugin or external ones.
    */
    ECheckpointDestroyed        CheckpointDestroyed;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a keybind entity instance was
     * destroyed from the server. Either by this plugin or external ones.
    */
    EKeybindDestroyed           KeybindDestroyed;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a object entity instance was
     * destroyed from the server. Either by this plugin or external ones.
    */
    EObjectDestroyed            ObjectDestroyed;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a pickup entity instance was
     * destroyed from the server. Either by this plugin or external ones.
    */
    EPickupDestroyed            PickupDestroyed;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player entity instance was
     * destroyed from the server. Either by this plugin or external ones.
    */
    EPlayerDestroyed            PlayerDestroyed;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a sphere entity instance was
     * destroyed from the server. Either by this plugin or external ones.
    */
    ESphereDestroyed            SphereDestroyed;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a sprite entity instance was
     * destroyed from the server. Either by this plugin or external ones.
    */
    ESpriteDestroyed            SpriteDestroyed;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a textdraw entity instance was
     * destroyed from the server. Either by this plugin or external ones.
    */
    ETextdrawDestroyed          TextdrawDestroyed;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a vehicle entity instance was
     * destroyed from the server. Either by this plugin or external ones.
    */
    EVehicleDestroyed           VehicleDestroyed;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a blip entity instance emitted
     * a custom event type.
    */
    EBlipCustom                 BlipCustom;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a checkpoint entity instance emitted
     * a custom event type.
    */
    ECheckpointCustom           CheckpointCustom;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a keybind entity instance emitted
     * a custom event type.
    */
    EKeybindCustom              KeybindCustom;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a object entity instance emitted
     * a custom event type.
    */
    EObjectCustom               ObjectCustom;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a pickup entity instance emitted
     * a custom event type.
    */
    EPickupCustom               PickupCustom;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player entity instance emitted
     * a custom event type.
    */
    EPlayerCustom               PlayerCustom;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a sphere entity instance emitted
     * a custom event type.
    */
    ESphereCustom               SphereCustom;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a sprite entity instance emitted
     * a custom event type.
    */
    ESpriteCustom               SpriteCustom;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a textdraw entity instance emitted
     * a custom event type.
    */
    ETextdrawCustom             TextdrawCustom;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a vehicle entity instance emitted
     * a custom event type.
    */
    EVehicleCustom              VehicleCustom;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player changed his away status.
    */
    EPlayerAway                 PlayerAway;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player pressed any of the game keys.
    */
    EPlayerGameKeys             PlayerGameKeys;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player changed his name.
    */
    EPlayerRename               PlayerRename;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when requested to be assigned a class.
    */
    EPlayerRequestClass         PlayerRequestClass;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player requested to spawn.
    */
    EPlayerRequestSpawn         PlayerRequestSpawn;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player just spaned in the game.
    */
    EPlayerSpawn                PlayerSpawn;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player began typing in the
     * chat/console.
    */
    EPlayerStartTyping          PlayerStartTyping;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player stopped typing in the
     * chat/console.
    */
    EPlayerStopTyping           PlayerStopTyping;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player sent a public chat message.
    */
    EPlayerChat                 PlayerChat;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player sent a server command.
    */
    EPlayerCommand              PlayerCommand;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player sent a private chat mesage.
    */
    EPlayerMessage              PlayerMessage;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when health changed for a speciffic player.
    */
    EPlayerHealth               PlayerHealth;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when the armour changed for a speciffic
     * player.
    */
    EPlayerArmour               PlayerArmour;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player changed his current weapon.
    */
    EPlayerWeapon               PlayerWeapon;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player changed his current
     * location/position.
    */
    EPlayerMove                 PlayerMove;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player died from a natural cause
     * or self inflicted injury.
    */
    EPlayerWasted               PlayerWasted;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player was killed by another player.
    */
    EPlayerKilled               PlayerKilled;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player is killed by one of his
     * teammates.
    */
    EPlayerTeamKill             PlayerTeamKill;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player began to spectate another
     * player.
    */
    EPlayerSpectate             PlayerSpectate;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player sent a crash report to
     * the server.
    */
    EPlayerCrashreport          PlayerCrashreport;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player is on fire or stopped
     * being on fire.
    */
    EPlayerBurning              PlayerBurning;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player is crouching or stopped
     * crouching.
    */
    EPlayerCrouching            PlayerCrouching;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when the current state of a player has
     * changed.
    */
    EPlayerState                PlayerState;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current action of a player has
     * changed.
    */
    EPlayerAction               PlayerAction;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to nothing.
    */
    EStateNone                  StateNone;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to normal.
    */
    EStateNormal                StateNormal;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to shooting.
    */
    EStateShooting              StateShooting;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to entered a vehicle as the driver.
    */
    EStateDriver                StateDriver;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to entered a vehicle as the passenger.
    */
    EStatePassenger             StatePassenger;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to preparing to enter a vehicle as the driver.
    */
    EStateEnterDriver           StateEnterDriver;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to preparing to enter a vehicle as a passenger.
    */
    EStateEnterPassenger        StateEnterPassenger;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to exiting a vehicle.
    */
    EStateExitVehicle           StateExitVehicle;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to being unspawned.
    */
    EStateUnspawned             StateUnspawned;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has action
     * to nothing.
    */
    EActionNone                 ActionNone;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to normal.
    */
    EActionNormal               ActionNormal;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to aiming at something.
    */
    EActionAiming               ActionAiming;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to shooting.
    */
    EActionShooting             ActionShooting;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to jumping.
    */
    EActionJumping              ActionJumping;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to lying down.
    */
    EActionLieDown              ActionLieDown;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to getting up.
    */
    EActionGettingUp            ActionGettingUp;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to jumping out of a vehicle.
    */
    EActionJumpVehicle          ActionJumpVehicle;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to driving a vehicle.
    */
    EActionDriving              ActionDriving;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to dying after being killed by someone else.
    */
    EActionDying                ActionDying;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to dying by natural causes or self inflicted injuries.
    */
    EActionWasted               ActionWasted;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to embarking a vehicle.
    */
    EActionEmbarking            ActionEmbarking;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when current state of a player has changed
     * to dissembarking a vehicle.
    */
    EActionDisembarking         ActionDisembarking;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a vehicle instance has respawned.
    */
    EVehicleRespawn             VehicleRespawn;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a vehicle has exploded.
    */
    EVehicleExplode             VehicleExplode;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when the health changed for a speciffic
     * vehicle.
    */
    EVehicleHealth              VehicleHealth;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a vehicle changed its current
     * location/position.
    */
    EVehicleMove                VehicleMove;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a pickup instance has respawned.
    */
    EPickupRespawn              PickupRespawn;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player pressed a listened
     * key combination.
    */
    EKeybindKeyPress            KeybindKeyPress;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player released a listened
     * key combination.
    */
    EKeybindKeyRelease          KeybindKeyRelease;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player is about to embark in a
     * vehicle.
    */
    EVehicleEmbarking           VehicleEmbarking;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player just embarked in a vehicle.
    */
    EVehicleEmbarked            VehicleEmbarked;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player just dissembarked from a
     * vehicle.
    */
    EVehicleDisembark           VehicleDisembark;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player claimed owenership over a
     * pickup.
    */
    EPickupClaimed              PickupClaimed;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player colected a pickup.
    */
    EPickupCollected            PickupCollected;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player just shot a object.
    */
    EObjectShot                 ObjectShot;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player just bumped in a object.
    */
    EObjectBump                 ObjectBump;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player just entered a checkpoint.
    */
    ECheckpointEntered          CheckpointEntered;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player just exited a checkpoint.
    */
    ECheckpointExited           CheckpointExited;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player just entered a sphere.
    */
    ESphereEntered              SphereEntered;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a player just exited a sphere.
    */
    ESphereExited               SphereExited;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when the server just finished one cycle
     * of it's main loop.
    */
    EServerFrame                ServerFrame;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when the server just started up.
    */
    EServerStartup              ServerStartup;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when the server is about to shut down.
    */
    EServerShutdown             ServerShutdown;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when the server received an internal
     * command.
    */
    EInternalCommand            InternalCommand;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when somone is trying to log into the
     * server.
    */
    ELoginAttempt               LoginAttempt;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when a custom event was just triggered.
    */
    ECustomEvent                CustomEvent;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when an option just changed in the
     * game world.
    */
    EWorldOption                WorldOption;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when something was just toggeled in the
     * game world.
    */
    EWorldToggle                WorldToggle;

    /* --------------------------------------------------------------------------------------------
     * A collection of listeners waiting to be notified when the plugin is about to reload it's
     * resources.
    */
    EScriptReload               ScriptReload;
};

// ------------------------------------------------------------------------------------------------
extern const Core::Pointer _Core;

} // Namespace:: SqMod

#endif // _CORE_HPP_
