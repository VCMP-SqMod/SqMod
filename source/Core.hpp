#ifndef _CORE_HPP_
#define _CORE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Signal.hpp"

// ------------------------------------------------------------------------------------------------
#include "Base/Vector3.hpp"

// ------------------------------------------------------------------------------------------------
#include <queue>
#include <vector>
#include <utility>
#include <unordered_map>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
class Core
{
protected:

    // --------------------------------------------------------------------------------------------
    friend class std::unique_ptr<Core, void(*)(Core *)>;

    // --------------------------------------------------------------------------------------------
    struct TPlayer
    {
        SQInt32     Weapon;
        SQFloat     Health;
        SQFloat     Armour;
        Vector3     Position;
        bool        Fresh;
    };

    // --------------------------------------------------------------------------------------------
    struct TVehicle
    {
        SQFloat     Health;
        Vector3     Position;
        bool        Fresh;
    };

    // --------------------------------------------------------------------------------------------
    typedef std::array<TPlayer, SQMOD_PLAYER_POOL>          TPlayerInstPool;
    typedef std::array<TVehicle, SQMOD_VEHICLE_POOL>        TVehicleInstPool;

    // --------------------------------------------------------------------------------------------
    typedef std::unique_ptr<RootTable>                      SqRootTable;
    typedef std::unordered_map<String, Script>              SqScriptPool;

    // --------------------------------------------------------------------------------------------
    typedef std::unordered_map<String, String>              OptionPool;

public:

    // --------------------------------------------------------------------------------------------
    typedef std::vector< SQChar >                           Buffer;
    typedef std::queue< Buffer >                            BufferPool;

private:

    // --------------------------------------------------------------------------------------------
    SQInteger                           m_State;

    // --------------------------------------------------------------------------------------------
    OptionPool                          m_Options;

    // --------------------------------------------------------------------------------------------
    HSQUIRRELVM                         m_VM;

    // --------------------------------------------------------------------------------------------
    SqRootTable                         m_RootTable;
    SqScriptPool                        m_Scripts;

    // --------------------------------------------------------------------------------------------
    String                              m_ErrorMsg;

    // --------------------------------------------------------------------------------------------
    TPlayerInstPool                     m_PlayerTrack;
    TVehicleInstPool                    m_VehicleTrack;

    // --------------------------------------------------------------------------------------------
    BufferPool                          m_BufferPool;

protected:

    // --------------------------------------------------------------------------------------------
    Core();

    // --------------------------------------------------------------------------------------------
    ~Core();

    // --------------------------------------------------------------------------------------------
    Core(Core const &)              = delete;
    Core(Core &&)                   = delete;

    // --------------------------------------------------------------------------------------------
    Core & operator=(Core const &)  = delete;
    Core & operator=(Core &&)       = delete;

    // --------------------------------------------------------------------------------------------
    static void _Finalizer(Core * ptr);

public:

    // --------------------------------------------------------------------------------------------
    typedef std::unique_ptr<Core, void(*)(Core *)>  Pointer;

    // --------------------------------------------------------------------------------------------
    static Pointer Inst();

    // --------------------------------------------------------------------------------------------
    bool Init();
    bool Load();

    // --------------------------------------------------------------------------------------------
    void Deinit();
    void Unload();

    // --------------------------------------------------------------------------------------------
    void Terminate();

    // --------------------------------------------------------------------------------------------
    void SetState(SQInteger val);
    SQInteger GetState() const;

    // --------------------------------------------------------------------------------------------
    String GetOption(const String & name) const;
    void SetOption(const String & name, const String & value);

    // --------------------------------------------------------------------------------------------
    Buffer PullBuffer(unsigned sz = 4096);
    void PushBuffer(Buffer && buf);
    void MakeBuffer(unsigned num, unsigned sz = 4096);

    // --------------------------------------------------------------------------------------------
    void ConnectPlayer(SQInt32 id, SQInt32 header, SqObj & payload);
    void DisconnectPlayer(SQInt32 id, SQInt32 header, SqObj & payload);

protected:

    // --------------------------------------------------------------------------------------------
    bool Configure();

    // --------------------------------------------------------------------------------------------
    bool CreateVM();
    void DestroyVM();

    // --------------------------------------------------------------------------------------------
    bool LoadScripts();

    // --------------------------------------------------------------------------------------------
    bool Compile(const String & name);
    bool Execute();

    // --------------------------------------------------------------------------------------------
    void PrintCallstack();

public:

    // --------------------------------------------------------------------------------------------
    static void PrintFunc(HSQUIRRELVM vm, const SQChar * str, ...);
    static void ErrorFunc(HSQUIRRELVM vm, const SQChar * str, ...);

    // --------------------------------------------------------------------------------------------
    static SQInteger RuntimeErrorHandler(HSQUIRRELVM vm);
    static void CompilerErrorHandler(HSQUIRRELVM vm, const SQChar * desc, const SQChar * src, SQInteger line, SQInteger column);

protected:

    /* --------------------------------------------------------------------------------------------
     * Destroys a Player created by the server
    */
    bool DestroyPlayer(SQInt32 id, SQInt32 header, SqObj & payload);

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

    // --------------------------------------------------------------------------------------------
    void OnBlipCreated(SQInt32 blip, SQInt32 header, SqObj & payload);
    void OnCheckpointCreated(SQInt32 checkpoint, SQInt32 header, SqObj & payload);
    void OnKeybindCreated(SQInt32 keybind, SQInt32 header, SqObj & payload);
    void OnObjectCreated(SQInt32 object, SQInt32 header, SqObj & payload);
    void OnPickupCreated(SQInt32 pickup, SQInt32 header, SqObj & payload);
    void OnPlayerCreated(SQInt32 player, SQInt32 header, SqObj & payload);
    void OnSphereCreated(SQInt32 sphere, SQInt32 header, SqObj & payload);
    void OnSpriteCreated(SQInt32 sprite, SQInt32 header, SqObj & payload);
    void OnTextdrawCreated(SQInt32 textdraw, SQInt32 header, SqObj & payload);
    void OnVehicleCreated(SQInt32 vehicle, SQInt32 header, SqObj & payload);

    // --------------------------------------------------------------------------------------------
    void OnBlipDestroyed(SQInt32 blip, SQInt32 header, SqObj & payload);
    void OnCheckpointDestroyed(SQInt32 checkpoint, SQInt32 header, SqObj & payload);
    void OnKeybindDestroyed(SQInt32 keybind, SQInt32 header, SqObj & payload);
    void OnObjectDestroyed(SQInt32 object, SQInt32 header, SqObj & payload);
    void OnPickupDestroyed(SQInt32 pickup, SQInt32 header, SqObj & payload);
    void OnPlayerDestroyed(SQInt32 player, SQInt32 header, SqObj & payload);
    void OnSphereDestroyed(SQInt32 sphere, SQInt32 header, SqObj & payload);
    void OnSpriteDestroyed(SQInt32 sprite, SQInt32 header, SqObj & payload);
    void OnTextdrawDestroyed(SQInt32 textdraw, SQInt32 header, SqObj & payload);
    void OnVehicleDestroyed(SQInt32 vehicle, SQInt32 header, SqObj & payload);

    // --------------------------------------------------------------------------------------------
    void OnBlipCustom(SQInt32 blip, SQInt32 header, SqObj & payload);
    void OnCheckpointCustom(SQInt32 checkpoint, SQInt32 header, SqObj & payload);
    void OnKeybindCustom(SQInt32 keybind, SQInt32 header, SqObj & payload);
    void OnObjectCustom(SQInt32 object, SQInt32 header, SqObj & payload);
    void OnPickupCustom(SQInt32 pickup, SQInt32 header, SqObj & payload);
    void OnPlayerCustom(SQInt32 player, SQInt32 header, SqObj & payload);
    void OnSphereCustom(SQInt32 sphere, SQInt32 header, SqObj & payload);
    void OnSpriteCustom(SQInt32 sprite, SQInt32 header, SqObj & payload);
    void OnTextdrawCustom(SQInt32 textdraw, SQInt32 header, SqObj & payload);
    void OnVehicleCustom(SQInt32 vehicle, SQInt32 header, SqObj & payload);

    // --------------------------------------------------------------------------------------------
    void OnPlayerAway(SQInt32 player, bool status);

    // --------------------------------------------------------------------------------------------
    void OnPlayerGameKeys(SQInt32 player, SQInt32 previous, SQInt32 current);
    void OnPlayerName(SQInt32 player, const SQChar * previous, const SQChar * current);

    // --------------------------------------------------------------------------------------------
    void OnPlayerRequestClass(SQInt32 player, SQInt32 offset);
    void OnPlayerRequestSpawn(SQInt32 player);

    // --------------------------------------------------------------------------------------------
    void OnPlayerSpawn(SQInt32 player);

    // --------------------------------------------------------------------------------------------
    void OnPlayerStartTyping(SQInt32 player);
    void OnPlayerStopTyping(SQInt32 player);

    // --------------------------------------------------------------------------------------------
    void OnPlayerChat(SQInt32 player, const SQChar * message);
    void OnPlayerCommand(SQInt32 player, const SQChar * command);
    void OnPlayerMessage(SQInt32 player, SQInt32 receiver, const SQChar * message);

    // --------------------------------------------------------------------------------------------
    void OnPlayerHealth(SQInt32 player, SQFloat previous, SQFloat current);
    void OnPlayerArmour(SQInt32 player, SQFloat previous, SQFloat current);
    void OnPlayerWeapon(SQInt32 player, SQInt32 previous, SQInt32 current);
    void OnPlayerMove(SQInt32 player, const Vector3 & previous, const Vector3 & current);

    // --------------------------------------------------------------------------------------------
    void OnPlayerWasted(SQInt32 player, SQInt32 reason);
    void OnPlayerKilled(SQInt32 player, SQInt32 killer, SQInt32 reason, SQInt32 body_part);

    // --------------------------------------------------------------------------------------------
    void OnPlayerSpectate(SQInt32 player, SQInt32 target);
    void OnPlayerCrashreport(SQInt32 player, const SQChar * report);

    // --------------------------------------------------------------------------------------------
    void OnPlayerBurning(SQInt32 player, bool state);
    void OnPlayerCrouching(SQInt32 player, bool state);

    // --------------------------------------------------------------------------------------------
    void OnPlayerState(SQInt32 player, SQInt32 previous, SQInt32 current);
    void OnPlayerAction(SQInt32 player, SQInt32 previous, SQInt32 current);

    // --------------------------------------------------------------------------------------------
    void OnStateNone(SQInt32 player, SQInt32 previous);
    void OnStateNormal(SQInt32 player, SQInt32 previous);
    void OnStateShooting(SQInt32 player, SQInt32 previous);
    void OnStateDriver(SQInt32 player, SQInt32 previous);
    void OnStatePassenger(SQInt32 player, SQInt32 previous);
    void OnStateEnterDriver(SQInt32 player, SQInt32 previous);
    void OnStateEnterPassenger(SQInt32 player, SQInt32 previous);
    void OnStateExitVehicle(SQInt32 player, SQInt32 previous);
    void OnStateUnspawned(SQInt32 player, SQInt32 previous);

    // --------------------------------------------------------------------------------------------
    void OnActionNone(SQInt32 player, SQInt32 previous);
    void OnActionNormal(SQInt32 player, SQInt32 previous);
    void OnActionAiming(SQInt32 player, SQInt32 previous);
    void OnActionShooting(SQInt32 player, SQInt32 previous);
    void OnActionJumping(SQInt32 player, SQInt32 previous);
    void OnActionLieDown(SQInt32 player, SQInt32 previous);
    void OnActionGettingUp(SQInt32 player, SQInt32 previous);
    void OnActionJumpVehicle(SQInt32 player, SQInt32 previous);
    void OnActionDriving(SQInt32 player, SQInt32 previous);
    void OnActionDying(SQInt32 player, SQInt32 previous);
    void OnActionWasted(SQInt32 player, SQInt32 previous);
    void OnActionEmbarking(SQInt32 player, SQInt32 previous);
    void OnActionDisembarking(SQInt32 player, SQInt32 previous);

    // --------------------------------------------------------------------------------------------
    void OnVehicleRespawn(SQInt32 vehicle);
    void OnVehicleExplode(SQInt32 vehicle);

    // --------------------------------------------------------------------------------------------
    void OnVehicleHealth(SQInt32 vehicle, SQFloat previous, SQFloat current);
    void OnVehicleMove(SQInt32 vehicle, const Vector3 & previous, const Vector3 & current);

    // --------------------------------------------------------------------------------------------
    void OnPickupRespawn(SQInt32 pickup);

    // --------------------------------------------------------------------------------------------
    void OnPlayerKeyPress(SQInt32 player, SQInt32 keybind);
    void OnPlayerKeyRelease(SQInt32 player, SQInt32 keybind);

    // --------------------------------------------------------------------------------------------
    void OnPlayerEmbarking(SQInt32 player, SQInt32 vehicle, SQInt32 slot);
    void OnPlayerEmbarked(SQInt32 player, SQInt32 vehicle, SQInt32 slot);
    void OnPlayerDisembark(SQInt32 player, SQInt32 vehicle);

    // --------------------------------------------------------------------------------------------
    void OnPickupClaimed(SQInt32 player, SQInt32 pickup);
    void OnPickupCollected(SQInt32 player, SQInt32 pickup);

    // --------------------------------------------------------------------------------------------
    void OnObjectShot(SQInt32 player, SQInt32 object, SQInt32 weapon);
    void OnObjectBump(SQInt32 player, SQInt32 object);

    // --------------------------------------------------------------------------------------------
    void OnCheckpointEntered(SQInt32 player, SQInt32 checkpoint);
    void OnCheckpointExited(SQInt32 player, SQInt32 checkpoint);

    // --------------------------------------------------------------------------------------------
    void OnSphereEntered(SQInt32 player, SQInt32 sphere);
    void OnSphereExited(SQInt32 player, SQInt32 sphere);

    // --------------------------------------------------------------------------------------------
    void OnServerFrame(SQFloat delta);

    // --------------------------------------------------------------------------------------------
    void OnServerStartup();
    void OnServerShutdown();

    // --------------------------------------------------------------------------------------------
    void OnInternalCommand(SQInt32 type, const SQChar * text);
    void OnLoginAttempt(const SQChar * name, const SQChar * passwd, const SQChar * ip);

    // --------------------------------------------------------------------------------------------
    void OnCustomEvent(SQInt32 group, SQInt32 header, SqObj & payload);

    // --------------------------------------------------------------------------------------------
    void OnWorldOption(SQInt32 option, SqObj & value);
    void OnWorldToggle(SQInt32 option, bool value);

    // --------------------------------------------------------------------------------------------
    void OnScriptReload(SQInt32 header, SqObj & payload);

    // --------------------------------------------------------------------------------------------
    void OnLogMessage(SQInt32 type, const SQChar * message);

    // --------------------------------------------------------------------------------------------
    void OnPlayerUpdate(SQInt32 player, SQInt32 type);
    void OnVehicleUpdate(SQInt32 vehicle, SQInt32 type);
    void OnEntityPool(SQInt32 type, SQInt32 id, bool deleted);

public:

    // --------------------------------------------------------------------------------------------
    EBlipCreated                BlipCreated;
    ECheckpointCreated          CheckpointCreated;
    EKeybindCreated             KeybindCreated;
    EObjectCreated              ObjectCreated;
    EPickupCreated              PickupCreated;
    EPlayerCreated              PlayerCreated;
    ESphereCreated              SphereCreated;
    ESpriteCreated              SpriteCreated;
    ETextdrawCreated            TextdrawCreated;
    EVehicleCreated             VehicleCreated;

    // --------------------------------------------------------------------------------------------
    EBlipDestroyed              BlipDestroyed;
    ECheckpointDestroyed        CheckpointDestroyed;
    EKeybindDestroyed           KeybindDestroyed;
    EObjectDestroyed            ObjectDestroyed;
    EPickupDestroyed            PickupDestroyed;
    EPlayerDestroyed            PlayerDestroyed;
    ESphereDestroyed            SphereDestroyed;
    ESpriteDestroyed            SpriteDestroyed;
    ETextdrawDestroyed          TextdrawDestroyed;
    EVehicleDestroyed           VehicleDestroyed;

    // --------------------------------------------------------------------------------------------
    EBlipCustom                 BlipCustom;
    ECheckpointCustom           CheckpointCustom;
    EKeybindCustom              KeybindCustom;
    EObjectCustom               ObjectCustom;
    EPickupCustom               PickupCustom;
    EPlayerCustom               PlayerCustom;
    ESphereCustom               SphereCustom;
    ESpriteCustom               SpriteCustom;
    ETextdrawCustom             TextdrawCustom;
    EVehicleCustom              VehicleCustom;

    // --------------------------------------------------------------------------------------------
    EPlayerAway                 PlayerAway;

    // --------------------------------------------------------------------------------------------
    EPlayerGameKeys             PlayerGameKeys;
    EPlayerRename               PlayerRename;

    // --------------------------------------------------------------------------------------------
    EPlayerRequestClass         PlayerRequestClass;
    EPlayerRequestSpawn         PlayerRequestSpawn;

    // --------------------------------------------------------------------------------------------
    EPlayerSpawn                PlayerSpawn;

    // --------------------------------------------------------------------------------------------
    EPlayerStartTyping          PlayerStartTyping;
    EPlayerStopTyping           PlayerStopTyping;

    // --------------------------------------------------------------------------------------------
    EPlayerChat                 PlayerChat;
    EPlayerCommand              PlayerCommand;
    EPlayerMessage              PlayerMessage;

    // --------------------------------------------------------------------------------------------
    EPlayerHealth               PlayerHealth;
    EPlayerArmour               PlayerArmour;
    EPlayerWeapon               PlayerWeapon;
    EPlayerMove                 PlayerMove;

    // --------------------------------------------------------------------------------------------
    EPlayerWasted               PlayerWasted;
    EPlayerKilled               PlayerKilled;
    EPlayerTeamKill             PlayerTeamKill;

    // --------------------------------------------------------------------------------------------
    EPlayerSpectate             PlayerSpectate;
    EPlayerCrashreport          PlayerCrashreport;

    // --------------------------------------------------------------------------------------------
    EPlayerBurning              PlayerBurning;
    EPlayerCrouching            PlayerCrouching;

    // --------------------------------------------------------------------------------------------
    EPlayerState                PlayerState;
    EPlayerAction               PlayerAction;

    // --------------------------------------------------------------------------------------------
    EStateNone                  StateNone;
    EStateNormal                StateNormal;
    EStateShooting              StateShooting;
    EStateDriver                StateDriver;
    EStatePassenger             StatePassenger;
    EStateEnterDriver           StateEnterDriver;
    EStateEnterPassenger        StateEnterPassenger;
    EStateExitVehicle           StateExitVehicle;
    EStateUnspawned             StateUnspawned;

    // --------------------------------------------------------------------------------------------
    EActionNone                 ActionNone;
    EActionNormal               ActionNormal;
    EActionAiming               ActionAiming;
    EActionShooting             ActionShooting;
    EActionJumping              ActionJumping;
    EActionLieDown              ActionLieDown;
    EActionGettingUp            ActionGettingUp;
    EActionJumpVehicle          ActionJumpVehicle;
    EActionDriving              ActionDriving;
    EActionDying                ActionDying;
    EActionWasted               ActionWasted;
    EActionEmbarking            ActionEmbarking;
    EActionDisembarking         ActionDisembarking;

    // --------------------------------------------------------------------------------------------
    EVehicleRespawn             VehicleRespawn;
    EVehicleExplode             VehicleExplode;

    // --------------------------------------------------------------------------------------------
    EVehicleHealth              VehicleHealth;
    EVehicleMove                VehicleMove;

    // --------------------------------------------------------------------------------------------
    EPickupRespawn              PickupRespawn;

    // --------------------------------------------------------------------------------------------
    EKeybindKeyPress            KeybindKeyPress;
    EKeybindKeyRelease          KeybindKeyRelease;

    // --------------------------------------------------------------------------------------------
    EVehicleEmbarking           VehicleEmbarking;
    EVehicleEmbarked            VehicleEmbarked;
    EVehicleDisembark           VehicleDisembark;

    // --------------------------------------------------------------------------------------------
    EPickupClaimed              PickupClaimed;
    EPickupCollected            PickupCollected;

    // --------------------------------------------------------------------------------------------
    EObjectShot                 ObjectShot;
    EObjectBump                 ObjectBump;

    // --------------------------------------------------------------------------------------------
    ECheckpointEntered          CheckpointEntered;
    ECheckpointExited           CheckpointExited;

    // --------------------------------------------------------------------------------------------
    ESphereEntered              SphereEntered;
    ESphereExited               SphereExited;

    // --------------------------------------------------------------------------------------------
    EServerFrame                ServerFrame;

    // --------------------------------------------------------------------------------------------
    EServerStartup              ServerStartup;
    EServerShutdown             ServerShutdown;

    // --------------------------------------------------------------------------------------------
    EInternalCommand            InternalCommand;
    ELoginAttempt               LoginAttempt;

    // --------------------------------------------------------------------------------------------
    ECustomEvent                CustomEvent;

    // --------------------------------------------------------------------------------------------
    EWorldOption                WorldOption;
    EWorldToggle                WorldToggle;

    // --------------------------------------------------------------------------------------------
    EScriptReload               ScriptReload;

    // --------------------------------------------------------------------------------------------
    ELogMessage                 LogMessage;

    // --------------------------------------------------------------------------------------------
    EVMClose                    VMClose;
};

// ------------------------------------------------------------------------------------------------
extern const Core::Pointer _Core;

} // Namespace:: SqMod

#endif // _CORE_HPP_
