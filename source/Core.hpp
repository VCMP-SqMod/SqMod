#ifndef _CORE_HPP_
#define _CORE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Signal.hpp"

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
        //Vector3     Position;
        bool        Fresh;
    };

    // --------------------------------------------------------------------------------------------
    struct TVehicle
    {
        SQFloat     Health;
        //Vector3     Position;
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
    Core() noexcept;

    // --------------------------------------------------------------------------------------------
    ~Core() noexcept;

    // --------------------------------------------------------------------------------------------
    Core(Core const &) noexcept              = delete;
    Core(Core &&) noexcept                   = delete;

    // --------------------------------------------------------------------------------------------
    Core & operator=(Core const &) noexcept  = delete;
    Core & operator=(Core &&) noexcept       = delete;

    // --------------------------------------------------------------------------------------------
    static void _Finalizer(Core * ptr) noexcept;

public:

    // --------------------------------------------------------------------------------------------
    typedef std::unique_ptr<Core, void(*)(Core *)>  Pointer;

    // --------------------------------------------------------------------------------------------
    static Pointer Inst() noexcept;

    // --------------------------------------------------------------------------------------------
    bool Init() noexcept;
    bool Load() noexcept;

    // --------------------------------------------------------------------------------------------
    void Deinit() noexcept;
    void Unload() noexcept;

    // --------------------------------------------------------------------------------------------
    void Terminate() noexcept;

    // --------------------------------------------------------------------------------------------
    void SetState(SQInteger val) noexcept;
    SQInteger GetState() const noexcept;

    // --------------------------------------------------------------------------------------------
    String GetOption(const String & name) const noexcept;
    void SetOption(const String & name, const String & value) noexcept;

    // --------------------------------------------------------------------------------------------
    Buffer PullBuffer(unsigned sz = 4096) noexcept;
    void PushBuffer(Buffer && buf) noexcept;
    void MakeBuffer(unsigned num, unsigned sz = 4096) noexcept;

    // --------------------------------------------------------------------------------------------
    void ConnectPlayer(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void DisconnectPlayer(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;

protected:

    // --------------------------------------------------------------------------------------------
    bool Configure() noexcept;

    // --------------------------------------------------------------------------------------------
    bool CreateVM() noexcept;
    void DestroyVM() noexcept;

    // --------------------------------------------------------------------------------------------
    bool LoadScripts() noexcept;

    // --------------------------------------------------------------------------------------------
    bool Compile(const String & name) noexcept;
    bool Execute() noexcept;

    // --------------------------------------------------------------------------------------------
    void PrintCallstack() noexcept;

public:

    // --------------------------------------------------------------------------------------------
    static void PrintFunc(HSQUIRRELVM vm, const SQChar * str, ...) noexcept;
    static void ErrorFunc(HSQUIRRELVM vm, const SQChar * str, ...) noexcept;

    // --------------------------------------------------------------------------------------------
    static SQInteger RuntimeErrorHandler(HSQUIRRELVM vm) noexcept;
    static void CompilerErrorHandler(HSQUIRRELVM vm, const SQChar * desc, const SQChar * src, SQInteger line, SQInteger column) noexcept;

protected:

    /* --------------------------------------------------------------------------------------------
     * Destroys a Player created by the server
    */
    bool DestroyPlayer(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;

public:

    /* --------------------------------------------------------------------------------------------
    * Creates a new Blip on the server
    */
    Reference< CBlip > CreateBlip(SQInt32 index, SQInt32 world, const Vector3 & pos, SQInt32 scale, \
                    const Color4 & color, SQInt32 sprite, SQInt32 header, const SqObj & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
    * Creates a new Checkpoint on the server
    */
    Reference< CCheckpoint > CreateCheckpoint(const  Reference< CPlayer > & player, SQInt32 world, const Vector3 & pos, \
                                const Color4 & color, SQFloat radius, SQInt32 header, const SqObj & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
    * Creates a new Keybind on the server
    */
    Reference< CKeybind > CreateKeybind(SQInt32 slot, bool release, SQInt32 primary, SQInt32 secondary, \
                        SQInt32 alternative, SQInt32 header, const SqObj & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
    * Creates a new Object on the server
    */
    Reference< CObject > CreateObject(const CModel & model, SQInt32 world, const Vector3 & pos, SQInt32 alpha, \
                        SQInt32 header, const SqObj & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
    * Creates a new Pickup on the server
    */
    Reference< CPickup > CreatePickup(const CModel & model, SQInt32 world, SQInt32 quantity, const Vector3 & pos, \
                        SQInt32 alpha, bool automatic, SQInt32 header, const SqObj & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
    * Creates a new Sphere on the server
    */
    Reference< CSphere > CreateSphere(const  Reference< CPlayer > & player, SQInt32 world, const Vector3 & pos, \
                        const Color3 & color, SQFloat radius, SQInt32 header, const SqObj & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
    * Creates a new Sprite on the server
    */
    Reference< CSprite > CreateSprite(SQInt32 index, const String & file, const Vector2i & pos, const Vector2i & rot, \
                        SQFloat angle, SQInt32 alpha, bool rel, SQInt32 header, const SqObj & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
    * Creates a new Textdraw on the server
    */
    Reference< CTextdraw > CreateTextdraw(SQInt32 index, const String & text, const Vector2i & pos, \
                            const Color4 & color, bool rel, SQInt32 header, const SqObj & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
    * Creates a new Vehicle on the server
    */
    Reference< CVehicle > CreateVehicle(const CAutomobile & model, SQInt32 world, const Vector3 & pos, SQFloat angle, \
                        SQInt32 primary, SQInt32 secondary, SQInt32 header, const SqObj & payload) noexcept;

public:

    /* --------------------------------------------------------------------------------------------
     * Destroys a Blip created by the server
    */
    bool DestroyBlip(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Destroys a Checkpoint created by the server
    */
    bool DestroyCheckpoint(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Destroys a Keybind created by the server
    */
    bool DestroyKeybind(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Destroys a Object created by the server
    */
    bool DestroyObject(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Destroys a Pickup created by the server
    */
    bool DestroyPickup(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Destroys a Sphere created by the server
    */
    bool DestroySphere(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Destroys a Sprite created by the server
    */
    bool DestroySprite(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Destroys a Textdraw created by the server
    */
    bool DestroyTextdraw(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Destroys a Vehicle created by the server
    */
    bool DestroyVehicle(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;

public:

    // --------------------------------------------------------------------------------------------
    void OnBlipCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnCheckpointCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnKeybindCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnObjectCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnPickupCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnPlayerCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnSphereCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnSpriteCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnTextdrawCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnVehicleCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnBlipDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnCheckpointDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnKeybindDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnObjectDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnPickupDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnPlayerDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnSphereDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnSpriteDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnTextdrawDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnVehicleDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnBlipCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnCheckpointCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnKeybindCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnObjectCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnPickupCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnPlayerCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnSphereCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnSpriteCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnTextdrawCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;
    void OnVehicleCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnPlayerAway(SQInt32 player, bool status) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnPlayerGameKeys(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept;
    void OnPlayerName(SQInt32 player, const SQChar * previous, const SQChar * current) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnPlayerRequestClass(SQInt32 player, SQInt32 offset) noexcept;
    void OnPlayerRequestSpawn(SQInt32 player) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnPlayerSpawn(SQInt32 player) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnPlayerStartTyping(SQInt32 player) noexcept;
    void OnPlayerStopTyping(SQInt32 player) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnPlayerChat(SQInt32 player, const SQChar * message) noexcept;
    void OnPlayerCommand(SQInt32 player, const SQChar * command) noexcept;
    void OnPlayerMessage(SQInt32 player, SQInt32 receiver, const SQChar * message) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnPlayerHealth(SQInt32 player, SQFloat previous, SQFloat current) noexcept;
    void OnPlayerArmour(SQInt32 player, SQFloat previous, SQFloat current) noexcept;
    void OnPlayerWeapon(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept;
    void OnPlayerMove(SQInt32 player, const Vector3 & previous, const Vector3 & current) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnPlayerWasted(SQInt32 player, SQInt32 reason) noexcept;
    void OnPlayerKilled(SQInt32 player, SQInt32 killer, SQInt32 reason, SQInt32 body_part) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnPlayerSpectate(SQInt32 player, SQInt32 target) noexcept;
    void OnPlayerCrashreport(SQInt32 player, const SQChar * report) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnPlayerBurning(SQInt32 player, bool state) noexcept;
    void OnPlayerCrouching(SQInt32 player, bool state) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnPlayerState(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept;
    void OnPlayerAction(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnStateNone(SQInt32 player, SQInt32 previous) noexcept;
    void OnStateNormal(SQInt32 player, SQInt32 previous) noexcept;
    void OnStateShooting(SQInt32 player, SQInt32 previous) noexcept;
    void OnStateDriver(SQInt32 player, SQInt32 previous) noexcept;
    void OnStatePassenger(SQInt32 player, SQInt32 previous) noexcept;
    void OnStateEnterDriver(SQInt32 player, SQInt32 previous) noexcept;
    void OnStateEnterPassenger(SQInt32 player, SQInt32 previous) noexcept;
    void OnStateExitVehicle(SQInt32 player, SQInt32 previous) noexcept;
    void OnStateUnspawned(SQInt32 player, SQInt32 previous) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnActionNone(SQInt32 player, SQInt32 previous) noexcept;
    void OnActionNormal(SQInt32 player, SQInt32 previous) noexcept;
    void OnActionAiming(SQInt32 player, SQInt32 previous) noexcept;
    void OnActionShooting(SQInt32 player, SQInt32 previous) noexcept;
    void OnActionJumping(SQInt32 player, SQInt32 previous) noexcept;
    void OnActionLieDown(SQInt32 player, SQInt32 previous) noexcept;
    void OnActionGettingUp(SQInt32 player, SQInt32 previous) noexcept;
    void OnActionJumpVehicle(SQInt32 player, SQInt32 previous) noexcept;
    void OnActionDriving(SQInt32 player, SQInt32 previous) noexcept;
    void OnActionDying(SQInt32 player, SQInt32 previous) noexcept;
    void OnActionWasted(SQInt32 player, SQInt32 previous) noexcept;
    void OnActionEmbarking(SQInt32 player, SQInt32 previous) noexcept;
    void OnActionDisembarking(SQInt32 player, SQInt32 previous) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnVehicleRespawn(SQInt32 vehicle) noexcept;
    void OnVehicleExplode(SQInt32 vehicle) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnVehicleHealth(SQInt32 vehicle, SQFloat previous, SQFloat current) noexcept;
    void OnVehicleMove(SQInt32 vehicle, const Vector3 & previous, const Vector3 & current) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnPickupRespawn(SQInt32 pickup) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnPlayerKeyPress(SQInt32 player, SQInt32 keybind) noexcept;
    void OnPlayerKeyRelease(SQInt32 player, SQInt32 keybind) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnPlayerEmbarking(SQInt32 player, SQInt32 vehicle, SQInt32 slot) noexcept;
    void OnPlayerEmbarked(SQInt32 player, SQInt32 vehicle, SQInt32 slot) noexcept;
    void OnPlayerDisembark(SQInt32 player, SQInt32 vehicle) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnPickupClaimed(SQInt32 pickup, SQInt32 player) noexcept;
    void OnPickupCollected(SQInt32 pickup, SQInt32 player) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnObjectShot(SQInt32 object, SQInt32 player, SQInt32 weapon) noexcept;
    void OnObjectBump(SQInt32 object, SQInt32 player) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnCheckpointEntered(SQInt32 checkpoint, SQInt32 player) noexcept;
    void OnCheckpointExited(SQInt32 checkpoint, SQInt32 player) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnSphereEntered(SQInt32 sphere, SQInt32 player) noexcept;
    void OnSphereExited(SQInt32 sphere, SQInt32 player) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnServerFrame(SQFloat delta) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnServerStartup() noexcept;
    void OnServerShutdown() noexcept;

    // --------------------------------------------------------------------------------------------
    void OnInternalCommand(SQInt32 type, const SQChar * text) noexcept;
    void OnLoginAttempt(const SQChar * name, const SQChar * passwd, const SQChar * ip) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnCustomEvent(SQInt32 group, SQInt32 header, const SqObj & payload) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnWorldOption(SQInt32 option, const SqObj & value) noexcept;
    void OnWorldToggle(SQInt32 option, bool value) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnScriptReload(SQInt32 header, const SqObj & payload) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnLogMessage(SQInt32 type, const SQChar * message) noexcept;

    // --------------------------------------------------------------------------------------------
    void OnPlayerUpdate(SQInt32 player, SQInt32 type) noexcept;
    void OnVehicleUpdate(SQInt32 vehicle, SQInt32 type) noexcept;
    void OnEntityPool(SQInt32 type, SQInt32 id, bool deleted) noexcept;

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
};

// ------------------------------------------------------------------------------------------------
extern const Core::Pointer _Core;

} // Namespace:: SqMod

#endif // _CORE_HPP_
