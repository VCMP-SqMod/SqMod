#ifndef _CORE_HPP_
#define _CORE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"
#include "Base/Vector3.hpp"
#include "Base/Color4.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>
#include <unordered_map>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern SQMOD_MANAGEDPTR_TYPE(Core) _Core;

/* ------------------------------------------------------------------------------------------------
 * Core module class responsible for managing resources.
*/
class Core
{
protected:

    /* --------------------------------------------------------------------------------------------
     * Helper structure meant to track changes in player instances.
    */
    struct PlayerTrack
    {
        PlayerTrack() : mWeapon(-1), mHealth(0), mArmour(0), mPosition()
        { /* ... */ }

        Int32       mWeapon; /* Last used player weapon. */
        Float32     mHealth; /* Last known player health. */
        Float32     mArmour; /* Last known player armour */
        Vector3     mPosition; /* Last known player position. */
    };

    /* --------------------------------------------------------------------------------------------
     * Helper structure meant to track changes in vehicle instances.
    */
    struct VehicleTrack
    {
        VehicleTrack() : mHealth(0), mPosition()
        { /* ... */ }

        Float32     mHealth; /* Last known vehicle health. */
        Vector3     mPosition; /* Last known vehicle position. */
    };

    // --------------------------------------------------------------------------------------------
    typedef String MsgPrefix[SQMOD_PLAYER_MSG_PREFIXES];

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    struct BlipInst
    {
        BlipInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(NULL)
        { /* ... */ }

        ~BlipInst();

        // ----------------------------------------------------------------------------------------
        Int32           mID;
        Uint16          mFlags;
        CBlip *         mInst;
        Object          mObj;

        // ----------------------------------------------------------------------------------------
        Int32           mWorld;
        Int32           mScale;

        // ----------------------------------------------------------------------------------------
        Int32           mSprID;

        // ----------------------------------------------------------------------------------------
        Vector3         mPosition;
        Color4          mColor;

        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;

        // ----------------------------------------------------------------------------------------
    };

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    struct CheckpointInst
    {
        CheckpointInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(NULL)
        { /* ... */ }

        ~CheckpointInst();

        // ----------------------------------------------------------------------------------------
        Int32           mID;
        Uint16          mFlags;
        CCheckpoint *   mInst;
        Object          mObj;

        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;

        // ----------------------------------------------------------------------------------------
        Function        mOnEntered;
        Function        mOnExited;
    };

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    struct ForcefieldInst
    {
        ForcefieldInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(NULL)
        { /* ... */ }

        ~ForcefieldInst();

        // ----------------------------------------------------------------------------------------
        Int32           mID;
        Uint16          mFlags;
        CForcefield *   mInst;
        Object          mObj;
        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;

        // ----------------------------------------------------------------------------------------
        Function        mOnEntered;
        Function        mOnExited;
    };

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    struct KeybindInst
    {
        KeybindInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(NULL)
        { /* ... */ }

        ~KeybindInst();

        // ----------------------------------------------------------------------------------------
        Int32           mID;
        Uint16          mFlags;
        CKeybind *      mInst;
        Object          mObj;

        // ----------------------------------------------------------------------------------------
        Int32           mFirst;
        Int32           mSecond;
        Int32           mThird;
        Int32           mRelease;

        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;

        // ----------------------------------------------------------------------------------------
        Function        mOnKeyPress;
        Function        mOnKeyRelease;
    };

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    struct ObjectInst
    {
        ObjectInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(NULL)
        { /* ... */ }

        ~ObjectInst();

        // ----------------------------------------------------------------------------------------
        Int32           mID;
        Uint16          mFlags;
        CObject *       mInst;
        Object          mObj;

        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;

        // ----------------------------------------------------------------------------------------
        Function        mOnShot;
        Function        mOnBump;
    };

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    struct PickupInst
    {
        PickupInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(NULL)
        { /* ... */ }

        ~PickupInst();

        // ----------------------------------------------------------------------------------------
        Int32           mID;
        Uint16          mFlags;
        CPickup *       mInst;
        Object          mObj;

        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;

        // ----------------------------------------------------------------------------------------
        Function        mOnRespawn;
        Function        mOnClaimed;
        Function        mOnCollected;
    };

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    struct PlayerInst
    {
        PlayerInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(NULL)
        { /* ... */ }

        ~PlayerInst();

        // ----------------------------------------------------------------------------------------
        Int32           mID;
        Uint16          mFlags;
        CPlayer *       mInst;
        Object          mObj;

        // ----------------------------------------------------------------------------------------
        Int32           mAuthority;

        // ----------------------------------------------------------------------------------------
        MsgPrefix       mPrefixes;

        // ----------------------------------------------------------------------------------------
        Uint32          mMessageColor;
        Int32           mAnnounceStyle;

        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;

        // ----------------------------------------------------------------------------------------
        Function        mOnAway;
        Function        mOnGameKeys;
        Function        mOnRename;
        Function        mOnRequestClass;
        Function        mOnRequestSpawn;
        Function        mOnSpawn;
        Function        mOnStartTyping;
        Function        mOnStopTyping;
        Function        mOnChat;
        Function        mOnCommand;
        Function        mOnMessage;
        Function        mOnHealth;
        Function        mOnArmour;
        Function        mOnWeapon;
        Function        mOnMove;
        Function        mOnWasted;
        Function        mOnKilled;
        Function        mOnTeamKill;
        Function        mOnSpectate;
        Function        mOnCrashreport;
        Function        mOnBurning;
        Function        mOnCrouching;
        Function        mOnState;
        Function        mOnAction;
        Function        mOnStateNone;
        Function        mOnStateNormal;
        Function        mOnStateShooting;
        Function        mOnStateDriver;
        Function        mOnStatePassenger;
        Function        mOnStateEnterDriver;
        Function        mOnStateEnterPassenger;
        Function        mOnStateExitVehicle;
        Function        mOnStateUnspawned;
        Function        mOnActionNone;
        Function        mOnActionNormal;
        Function        mOnActionAiming;
        Function        mOnActionShooting;
        Function        mOnActionJumping;
        Function        mOnActionLieDown;
        Function        mOnActionGettingUp;
        Function        mOnActionJumpVehicle;
        Function        mOnActionDriving;
        Function        mOnActionDying;
        Function        mOnActionWasted;
        Function        mOnActionEmbarking;
        Function        mOnActionDisembarking;
        Function        mOnKeyPress;
        Function        mOnKeyRelease;
        Function        mOnEmbarking;
        Function        mOnEmbarked;
        Function        mOnDisembark;
        Function        mOnPickupClaimed;
        Function        mOnPickupCollected;
        Function        mOnObjectShot;
        Function        mOnObjectBump;
        Function        mOnCheckpointEntered;
        Function        mOnCheckpointExited;
        Function        mOnForcefieldEntered;
        Function        mOnForcefieldExited;
    };

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    struct SpriteInst
    {
        SpriteInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(NULL)
        { /* ... */ }

        ~SpriteInst();

        // ----------------------------------------------------------------------------------------
        Int32           mID;
        Uint16          mFlags;
        CSprite *       mInst;
        Object          mObj;

        // ----------------------------------------------------------------------------------------
        String          mPath;

        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;

        // ----------------------------------------------------------------------------------------
    };

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    struct TextdrawInst
    {
        TextdrawInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(NULL)
        { /* ... */ }

        ~TextdrawInst();

        // ----------------------------------------------------------------------------------------
        Int32           mID;
        Uint16          mFlags;
        CTextdraw *     mInst;
        Object          mObj;

        // ----------------------------------------------------------------------------------------
        String          mText;

        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;

        // ----------------------------------------------------------------------------------------
    };

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    struct VehicleInst
    {
        VehicleInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(NULL)
        { /* ... */ }

        ~VehicleInst();

        // ----------------------------------------------------------------------------------------
        Int32           mID;
        Uint16          mFlags;
        CVehicle *      mInst;
        Object          mObj;

        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;

        // ----------------------------------------------------------------------------------------
        Function        mOnRespawn;
        Function        mOnExplode;
        Function        mOnHealth;
        Function        mOnMove;
        Function        mOnEmbarking;
        Function        mOnEmbarked;
        Function        mOnDisembark;
    };

public:

    // --------------------------------------------------------------------------------------------
    typedef std::vector< BlipInst >             Blips;
    typedef std::vector< CheckpointInst >       Checkpoints;
    typedef std::vector< ForcefieldInst >       Forcefields;
    typedef std::vector< KeybindInst >          Keybinds;
    typedef std::vector< ObjectInst >           Objects;
    typedef std::vector< PickupInst >           Pickups;
    typedef std::vector< PlayerInst >           Players;
    typedef std::vector< SpriteInst >           Sprites;
    typedef std::vector< TextdrawInst >         Textdraws;
    typedef std::vector< VehicleInst >          Vehicles;

    // --------------------------------------------------------------------------------------------
    typedef PlayerTrack                         PlayerInstTrack[SQMOD_PLAYER_POOL];
    typedef VehicleTrack                        VehicleInstTrack[SQMOD_VEHICLE_POOL];

    // --------------------------------------------------------------------------------------------
    typedef std::unordered_map< String, Script >    Scripts;
    typedef std::unordered_map< String, String >    Options;

private:

    // --------------------------------------------------------------------------------------------
    Int32                           m_State;
    HSQUIRRELVM                     m_VM;
    Scripts                         m_Scripts;
    Options                         m_Options;

    // --------------------------------------------------------------------------------------------
    Blips                           m_Blips;
    Checkpoints                     m_Checkpoints;
    Forcefields                     m_Forcefields;
    Keybinds                        m_Keybinds;
    Objects                         m_Objects;
    Pickups                         m_Pickups;
    Players                         m_Players;
    Sprites                         m_Sprites;
    Textdraws                       m_Textdraws;
    Vehicles                        m_Vehicles;

    // --------------------------------------------------------------------------------------------
    PlayerInstTrack                 m_PlayerTrack;
    VehicleInstTrack                m_VehicleTrack;

protected:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Core();

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Core(const Core &);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Core & operator = (const Core &);

public:

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Core();

    /* --------------------------------------------------------------------------------------------
     * Singleton retriever.
    */
    static Core * Get()
    {
        if (!_Core)
        {
            _Core = SQMOD_MANAGEDPTR_MAKE(Core, new Core());
        }

        return SQMOD_MANAGEDPTR_GET(_Core);
    }

    /* --------------------------------------------------------------------------------------------
     * Lifetime managers.
    */
    bool Init();
    bool Load();
    void Terminate();

    /* --------------------------------------------------------------------------------------------
     * State mutators.
    */
    void SetState(Int32 val)
    {
        m_State = val;
    }

    Int32 GetState() const
    {
        return m_State;
    }

    /* --------------------------------------------------------------------------------------------
     * Option mutators.
    */
    CSStr GetOption(CSStr name) const;
    CSStr GetOption(CSStr name, CSStr value) const;
    void SetOption(CSStr name, CSStr value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the virtual machine.
    */
    HSQUIRRELVM GetVM() const
    {
        return m_VM;
    }

    /* --------------------------------------------------------------------------------------------
     * Adds a script to the load queue.
    */
    bool LoadScript(CSStr filepath);

protected:

    /* --------------------------------------------------------------------------------------------
     * Script output handlers.
    */
    static void PrintFunc(HSQUIRRELVM vm, CSStr msg, ...);
    static void ErrorFunc(HSQUIRRELVM vm, CSStr msg, ...);

    /* --------------------------------------------------------------------------------------------
     * Script error handlers.
    */
    static SQInteger RuntimeErrorHandler(HSQUIRRELVM vm);
    static void CompilerErrorHandler(HSQUIRRELVM vm, CSStr desc, CSStr src,
                                        SQInteger line, SQInteger column);

    /* --------------------------------------------------------------------------------------------
     * Entity instance scaners.
    */
    void ImportBlips();
    void ImportCheckpoints();
    void ImportForcefields();
    void ImportKeybinds();
    void ImportObjects();
    void ImportPickups();
    void ImportPlayers();
    void ImportSprites();
    void ImportTextdraws();
    void ImportVehicles();

    /* --------------------------------------------------------------------------------------------
     * Instance allocators.
    */
    Object & AllocBlip(Int32 id, bool owned, Int32 header, Object & payload);
    Object & AllocCheckpoint(Int32 id, bool owned, Int32 header, Object & payload);
    Object & AllocForcefield(Int32 id, bool owned, Int32 header, Object & payload);
    Object & AllocKeybind(Int32 id, bool owned, Int32 header, Object & payload);
    Object & AllocObject(Int32 id, bool owned, Int32 header, Object & payload);
    Object & AllocPickup(Int32 id, bool owned, Int32 header, Object & payload);
    Object & AllocSprite(Int32 id, bool owned, Int32 header, Object & payload);
    Object & AllocTextdraw(Int32 id, bool owned, Int32 header, Object & payload);
    Object & AllocVehicle(Int32 id, bool owned, Int32 header, Object & payload);

    /* --------------------------------------------------------------------------------------------
     * Instance deallocators.
    */
    void DeallocBlip(Int32 id, bool destroy, Int32 header, Object & payload);
    void DeallocCheckpoint(Int32 id, bool destroy, Int32 header, Object & payload);
    void DeallocForcefield(Int32 id, bool destroy, Int32 header, Object & payload);
    void DeallocKeybind(Int32 id, bool destroy, Int32 header, Object & payload);
    void DeallocObject(Int32 id, bool destroy, Int32 header, Object & payload);
    void DeallocPickup(Int32 id, bool destroy, Int32 header, Object & payload);
    void DeallocSprite(Int32 id, bool destroy, Int32 header, Object & payload);
    void DeallocTextdraw(Int32 id, bool destroy, Int32 header, Object & payload);
    void DeallocVehicle(Int32 id, bool destroy, Int32 header, Object & payload);

public:

    /* --------------------------------------------------------------------------------------------
     * Instance creators.
    */
    Object & NewBlip(Int32 index, Int32 world, Float32 x, Float32 y, Float32 z,
                        Int32 scale, Uint32 color, Int32 sprid,
                        Int32 header, Object & payload);

    Object & NewCheckpoint(Int32 player, Int32 world, Float32 x, Float32 y, Float32 z,
                        Uint8 r, Uint8 g, Uint8 b, Uint8 a, Float32 radius,
                        Int32 header, Object & payload);

    Object & NewForcefield(Int32 player, Int32 world, Float32 x, Float32 y, Float32 z,
                        Uint8 r, Uint8 g, Uint8 b, Float32 radius,
                        Int32 header, Object & payload);

    Object & NewKeybind(Int32 slot, bool release,
                        Int32 primary, Int32 secondary, Int32 alternative,
                        Int32 header, Object & payload);

    Object & NewObject(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z,
                        Int32 alpha, Int32 header, Object & payload);

    Object & NewPickup(Int32 model, Int32 world, Int32 quantity,
                        Float32 x, Float32 y, Float32 z, Int32 alpha, bool automatic,
                        Int32 header, Object & payload);

    Object & NewSprite(Int32 index, CSStr file, Int32 xp, Int32 yp,
                        Int32 xr, Int32 yr, Float32 angle, Int32 alpha, bool rel,
                        Int32 header, Object & payload);

    Object & NewTextdraw(Int32 index, CSStr text, Int32 xp, Int32 yp,
                        Uint32 color, bool rel, Int32 header, Object & payload);

    Object & NewVehicle(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z,
                        Float32 angle, Int32 primary, Int32 secondary,
                        Int32 header, Object & payload);

    /* --------------------------------------------------------------------------------------------
     * Instance destroyers.
    */
    bool DelBlip(Int32 id, Int32 header, Object & payload);
    bool DelCheckpoint(Int32 id, Int32 header, Object & payload);
    bool DelForcefield(Int32 id, Int32 header, Object & payload);
    bool DelKeybind(Int32 id, Int32 header, Object & payload);
    bool DelObject(Int32 id, Int32 header, Object & payload);
    bool DelPickup(Int32 id, Int32 header, Object & payload);
    bool DelSprite(Int32 id, Int32 header, Object & payload);
    bool DelTextdraw(Int32 id, Int32 header, Object & payload);
    bool DelVehicle(Int32 id, Int32 header, Object & payload);

    /* --------------------------------------------------------------------------------------------
     * Instance retrievers.
    */
    BlipInst & GetBlip(Int32 id) { return m_Blips.at(id); }
    CheckpointInst & GetCheckpoint(Int32 id) { return m_Checkpoints.at(id); }
    ForcefieldInst & GetForcefield(Int32 id) { return m_Forcefields.at(id); }
    KeybindInst & GetKeybind(Int32 id) { return m_Keybinds.at(id); }
    ObjectInst & GetObject(Int32 id) { return m_Objects.at(id); }
    PickupInst & GetPickup(Int32 id) { return m_Pickups.at(id); }
    PlayerInst & GetPlayer(Int32 id) { return m_Players.at(id); }
    SpriteInst & GetSprite(Int32 id) { return m_Sprites.at(id); }
    TextdrawInst & GetTextdraw(Int32 id) { return m_Textdraws.at(id); }
    VehicleInst & GetVehicle(Int32 id) { return m_Vehicles.at(id); }

    /* --------------------------------------------------------------------------------------------
     * Pool retrievers.
    */
    const Blips & GetBlips() const { return m_Blips; }
    const Checkpoints & GetCheckpoints() const { return m_Checkpoints; }
    const Forcefields & GetForcefields() const { return m_Forcefields; }
    const Keybinds & GetKeybinds() const { return m_Keybinds; }
    const Objects & GetObjects() const { return m_Objects; }
    const Pickups & GetPickups() const { return m_Pickups; }
    const Players & GetPlayers() const { return m_Players; }
    const Sprites & GetSprites() const { return m_Sprites; }
    const Textdraws & GetTextdraws() const { return m_Textdraws; }
    const Vehicles & GetVehicles() const { return m_Vehicles; }

protected:

    /* --------------------------------------------------------------------------------------------
     * Instance cleaners.
    */
    void ResetInst(BlipInst & inst);
    void ResetInst(CheckpointInst & inst);
    void ResetInst(ForcefieldInst & inst);
    void ResetInst(KeybindInst & inst);
    void ResetInst(ObjectInst & inst);
    void ResetInst(PickupInst & inst);
    void ResetInst(PlayerInst & inst);
    void ResetInst(SpriteInst & inst);
    void ResetInst(TextdrawInst & inst);
    void ResetInst(VehicleInst & inst);

    /* --------------------------------------------------------------------------------------------
     * Bindings cleaners.
    */
    void ResetFunc(BlipInst & inst);
    void ResetFunc(CheckpointInst & inst);
    void ResetFunc(ForcefieldInst & inst);
    void ResetFunc(KeybindInst & inst);
    void ResetFunc(ObjectInst & inst);
    void ResetFunc(PickupInst & inst);
    void ResetFunc(PlayerInst & inst);
    void ResetFunc(SpriteInst & inst);
    void ResetFunc(TextdrawInst & inst);
    void ResetFunc(VehicleInst & inst);
    void ResetFunc();

    // --------------------------------------------------------------------------------------------
    static void Emit(Function & func)
    {
        if (!func.IsNull())
            func.Execute();
    }

    // --------------------------------------------------------------------------------------------
    template < typename A1 >
    static void Emit(Function & func, A1 a1)
    {
        if (!func.IsNull())
            func.Execute(a1);
    }

    // --------------------------------------------------------------------------------------------
    template < typename A1, typename A2 >
    static void Emit(Function & func, A1 a1, A2 a2)
    {
        if (!func.IsNull())
            func.Execute(a1, a2);
    }

    // --------------------------------------------------------------------------------------------
    template < typename A1, typename A2, typename A3 >
    static void Emit(Function & func, A1 a1, A2 a2, A3 a3)
    {
        if (!func.IsNull())
            func.Execute(a1, a2, a3);
    }

    // --------------------------------------------------------------------------------------------
    template < typename A1, typename A2, typename A3, typename A4 >
    static void Emit(Function & func, A1 a1, A2 a2, A3 a3, A4 a4)
    {
        if (!func.IsNull())
            func.Execute(a1, a2, a3, a4);
    }

    // --------------------------------------------------------------------------------------------
    template < typename A1, typename A2, typename A3, typename A4, typename A5 >
    static void Emit(Function & func, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
    {
        if (!func.IsNull())
            func.Execute(a1, a2, a3, a4, a5);
    }

    // --------------------------------------------------------------------------------------------
    template < typename A1, typename A2, typename A3, typename A4, typename A5, typename A6 >
    static void Emit(Function & func, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
    {
        if (!func.IsNull())
            func.Execute(a1, a2, a3, a4, a5, a6);
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Global event binder.
    */
    void BindEvent(Int32 id, Object & env, Function & func);

    /* --------------------------------------------------------------------------------------------
     * Player lifetime management.
    */
    void ConnectPlayer(Int32 id, Int32 header, Object & payload);
    void DisconnectPlayer(Int32 id, Int32 header, Object & payload);

    /* --------------------------------------------------------------------------------------------
     * Event propagators.
    */
    void EmitBlipCreated(Int32 blip, Int32 header, Object & payload);
    void EmitCheckpointCreated(Int32 checkpoint, Int32 header, Object & payload);
    void EmitForcefieldCreated(Int32 forcefield, Int32 header, Object & payload);
    void EmitKeybindCreated(Int32 keybind, Int32 header, Object & payload);
    void EmitObjectCreated(Int32 object, Int32 header, Object & payload);
    void EmitPickupCreated(Int32 pickup, Int32 header, Object & payload);
    void EmitPlayerCreated(Int32 player, Int32 header, Object & payload);
    void EmitSpriteCreated(Int32 sprite, Int32 header, Object & payload);
    void EmitTextdrawCreated(Int32 textdraw, Int32 header, Object & payload);
    void EmitVehicleCreated(Int32 vehicle, Int32 header, Object & payload);
    void EmitBlipDestroyed(Int32 blip, Int32 header, Object & payload);
    void EmitCheckpointDestroyed(Int32 checkpoint, Int32 header, Object & payload);
    void EmitForcefieldDestroyed(Int32 forcefield, Int32 header, Object & payload);
    void EmitKeybindDestroyed(Int32 keybind, Int32 header, Object & payload);
    void EmitObjectDestroyed(Int32 object, Int32 header, Object & payload);
    void EmitPickupDestroyed(Int32 pickup, Int32 header, Object & payload);
    void EmitPlayerDestroyed(Int32 player, Int32 header, Object & payload);
    void EmitSpriteDestroyed(Int32 sprite, Int32 header, Object & payload);
    void EmitTextdrawDestroyed(Int32 textdraw, Int32 header, Object & payload);
    void EmitVehicleDestroyed(Int32 vehicle, Int32 header, Object & payload);
    void EmitBlipCustom(Int32 blip, Int32 header, Object & payload);
    void EmitCheckpointCustom(Int32 checkpoint, Int32 header, Object & payload);
    void EmitForcefieldCustom(Int32 forcefield, Int32 header, Object & payload);
    void EmitKeybindCustom(Int32 keybind, Int32 header, Object & payload);
    void EmitObjectCustom(Int32 object, Int32 header, Object & payload);
    void EmitPickupCustom(Int32 pickup, Int32 header, Object & payload);
    void EmitPlayerCustom(Int32 player, Int32 header, Object & payload);
    void EmitSpriteCustom(Int32 sprite, Int32 header, Object & payload);
    void EmitTextdrawCustom(Int32 textdraw, Int32 header, Object & payload);
    void EmitVehicleCustom(Int32 vehicle, Int32 header, Object & payload);
    void EmitPlayerAway(Int32 player, bool status);
    void EmitPlayerGameKeys(Int32 player, Int32 previous, Int32 current);
    void EmitPlayerRename(Int32 player, CCStr previous, CCStr current);
    void EmitPlayerRequestClass(Int32 player, Int32 offset);
    void EmitPlayerRequestSpawn(Int32 player);
    void EmitPlayerSpawn(Int32 player);
    void EmitPlayerStartTyping(Int32 player);
    void EmitPlayerStopTyping(Int32 player);
    void EmitPlayerChat(Int32 player, CCStr message);
    void EmitPlayerCommand(Int32 player, CCStr command);
    void EmitPlayerMessage(Int32 player, Int32 receiver, CCStr message);
    void EmitPlayerHealth(Int32 player, Float32 previous, Float32 current);
    void EmitPlayerArmour(Int32 player, Float32 previous, Float32 current);
    void EmitPlayerWeapon(Int32 player, Int32 previous, Int32 current);
    void EmitPlayerMove(Int32 player, const Vector3 & previous, const Vector3 & current);
    void EmitPlayerWasted(Int32 player, Int32 reason);
    void EmitPlayerKilled(Int32 player, Int32 killer, Int32 reason, Int32 body_part);
    void EmitPlayerTeamKill(Int32 player, Int32 killer, Int32 reason, Int32 body_part);
    void EmitPlayerSpectate(Int32 player, Int32 target);
    void EmitPlayerCrashreport(Int32 player, CCStr report);
    void EmitPlayerBurning(Int32 player, bool state);
    void EmitPlayerCrouching(Int32 player, bool state);
    void EmitPlayerState(Int32 player, Int32 previous, Int32 current);
    void EmitPlayerAction(Int32 player, Int32 previous, Int32 current);
    void EmitStateNone(Int32 player, Int32 previous);
    void EmitStateNormal(Int32 player, Int32 previous);
    void EmitStateShooting(Int32 player, Int32 previous);
    void EmitStateDriver(Int32 player, Int32 previous);
    void EmitStatePassenger(Int32 player, Int32 previous);
    void EmitStateEnterDriver(Int32 player, Int32 previous);
    void EmitStateEnterPassenger(Int32 player, Int32 previous);
    void EmitStateExitVehicle(Int32 player, Int32 previous);
    void EmitStateUnspawned(Int32 player, Int32 previous);
    void EmitActionNone(Int32 player, Int32 previous);
    void EmitActionNormal(Int32 player, Int32 previous);
    void EmitActionAiming(Int32 player, Int32 previous);
    void EmitActionShooting(Int32 player, Int32 previous);
    void EmitActionJumping(Int32 player, Int32 previous);
    void EmitActionLieDown(Int32 player, Int32 previous);
    void EmitActionGettingUp(Int32 player, Int32 previous);
    void EmitActionJumpVehicle(Int32 player, Int32 previous);
    void EmitActionDriving(Int32 player, Int32 previous);
    void EmitActionDying(Int32 player, Int32 previous);
    void EmitActionWasted(Int32 player, Int32 previous);
    void EmitActionEmbarking(Int32 player, Int32 previous);
    void EmitActionDisembarking(Int32 player, Int32 previous);
    void EmitVehicleRespawn(Int32 vehicle);
    void EmitVehicleExplode(Int32 vehicle);
    void EmitVehicleHealth(Int32 vehicle, Float32 previous, Float32 current);
    void EmitVehicleMove(Int32 vehicle, const Vector3 & previous, const Vector3 & current);
    void EmitPickupRespawn(Int32 pickup);
    void EmitPlayerKeyPress(Int32 player, Int32 keybind);
    void EmitPlayerKeyRelease(Int32 player, Int32 keybind);
    void EmitPlayerEmbarking(Int32 player, Int32 vehicle, Int32 slot);
    void EmitPlayerEmbarked(Int32 player, Int32 vehicle, Int32 slot);
    void EmitPlayerDisembark(Int32 player, Int32 vehicle);
    void EmitPickupClaimed(Int32 player, Int32 pickup);
    void EmitPickupCollected(Int32 player, Int32 pickup);
    void EmitObjectShot(Int32 player, Int32 object, Int32 weapon);
    void EmitObjectBump(Int32 player, Int32 object);
    void EmitCheckpointEntered(Int32 player, Int32 checkpoint);
    void EmitCheckpointExited(Int32 player, Int32 checkpoint);
    void EmitForcefieldEntered(Int32 player, Int32 forcefield);
    void EmitForcefieldExited(Int32 player, Int32 forcefield);
    void EmitServerFrame(Float32 delta);
    void EmitServerStartup();
    void EmitServerShutdown();
    void EmitInternalCommand(Int32 type, CCStr text);
    void EmitLoginAttempt(CCStr name, CCStr passwd, CCStr ip);
    void EmitCustomEvent(Int32 group, Int32 header, Object & payload);
    void EmitWorldOption(Int32 option, Object & value);
    void EmitWorldToggle(Int32 option, bool value);
    void EmitScriptReload(Int32 header, Object & payload);
    void EmitScriptLoaded();
    void EmitPlayerUpdate(Int32 player, Int32 type);
    void EmitVehicleUpdate(Int32 vehicle, Int32 type);
    void EmitEntityPool(Int32 type, Int32 id, bool deleted);

    /* --------------------------------------------------------------------------------------------
     * Retrie global event bidings.
    */
    Function & GetEvent(Int32 evid);

    /* --------------------------------------------------------------------------------------------
     * Retrie local event bidings.
    */
    Function & GetBlipEvent(Int32 id, Int32 evid);
    Function & GetCheckpointEvent(Int32 id, Int32 evid);
    Function & GetForcefieldEvent(Int32 id, Int32 evid);
    Function & GetKeybindEvent(Int32 id, Int32 evid);
    Function & GetObjectEvent(Int32 id, Int32 evid);
    Function & GetPickupEvent(Int32 id, Int32 evid);
    Function & GetPlayerEvent(Int32 id, Int32 evid);
    Function & GetSpriteEvent(Int32 id, Int32 evid);
    Function & GetTextdrawEvent(Int32 id, Int32 evid);
    Function & GetVehicleEvent(Int32 id, Int32 evid);

private:

    /* --------------------------------------------------------------------------------------------
     * Global event bindings.
    */
    Function    mOnBlipCreated;
    Function    mOnCheckpointCreated;
    Function    mOnForcefieldCreated;
    Function    mOnKeybindCreated;
    Function    mOnObjectCreated;
    Function    mOnPickupCreated;
    Function    mOnPlayerCreated;
    Function    mOnSpriteCreated;
    Function    mOnTextdrawCreated;
    Function    mOnVehicleCreated;
    Function    mOnBlipDestroyed;
    Function    mOnCheckpointDestroyed;
    Function    mOnForcefieldDestroyed;
    Function    mOnKeybindDestroyed;
    Function    mOnObjectDestroyed;
    Function    mOnPickupDestroyed;
    Function    mOnPlayerDestroyed;
    Function    mOnSpriteDestroyed;
    Function    mOnTextdrawDestroyed;
    Function    mOnVehicleDestroyed;
    Function    mOnBlipCustom;
    Function    mOnCheckpointCustom;
    Function    mOnForcefieldCustom;
    Function    mOnKeybindCustom;
    Function    mOnObjectCustom;
    Function    mOnPickupCustom;
    Function    mOnPlayerCustom;
    Function    mOnSpriteCustom;
    Function    mOnTextdrawCustom;
    Function    mOnVehicleCustom;
    Function    mOnPlayerAway;
    Function    mOnPlayerGameKeys;
    Function    mOnPlayerRename;
    Function    mOnPlayerRequestClass;
    Function    mOnPlayerRequestSpawn;
    Function    mOnPlayerSpawn;
    Function    mOnPlayerStartTyping;
    Function    mOnPlayerStopTyping;
    Function    mOnPlayerChat;
    Function    mOnPlayerCommand;
    Function    mOnPlayerMessage;
    Function    mOnPlayerHealth;
    Function    mOnPlayerArmour;
    Function    mOnPlayerWeapon;
    Function    mOnPlayerMove;
    Function    mOnPlayerWasted;
    Function    mOnPlayerKilled;
    Function    mOnPlayerTeamKill;
    Function    mOnPlayerSpectate;
    Function    mOnPlayerCrashreport;
    Function    mOnPlayerBurning;
    Function    mOnPlayerCrouching;
    Function    mOnPlayerState;
    Function    mOnPlayerAction;
    Function    mOnStateNone;
    Function    mOnStateNormal;
    Function    mOnStateShooting;
    Function    mOnStateDriver;
    Function    mOnStatePassenger;
    Function    mOnStateEnterDriver;
    Function    mOnStateEnterPassenger;
    Function    mOnStateExitVehicle;
    Function    mOnStateUnspawned;
    Function    mOnActionNone;
    Function    mOnActionNormal;
    Function    mOnActionAiming;
    Function    mOnActionShooting;
    Function    mOnActionJumping;
    Function    mOnActionLieDown;
    Function    mOnActionGettingUp;
    Function    mOnActionJumpVehicle;
    Function    mOnActionDriving;
    Function    mOnActionDying;
    Function    mOnActionWasted;
    Function    mOnActionEmbarking;
    Function    mOnActionDisembarking;
    Function    mOnVehicleRespawn;
    Function    mOnVehicleExplode;
    Function    mOnVehicleHealth;
    Function    mOnVehicleMove;
    Function    mOnPickupRespawn;
    Function    mOnKeybindKeyPress;
    Function    mOnKeybindKeyRelease;
    Function    mOnVehicleEmbarking;
    Function    mOnVehicleEmbarked;
    Function    mOnVehicleDisembark;
    Function    mOnPickupClaimed;
    Function    mOnPickupCollected;
    Function    mOnObjectShot;
    Function    mOnObjectBump;
    Function    mOnCheckpointEntered;
    Function    mOnCheckpointExited;
    Function    mOnForcefieldEntered;
    Function    mOnForcefieldExited;
    Function    mOnServerFrame;
    Function    mOnServerStartup;
    Function    mOnServerShutdown;
    Function    mOnInternalCommand;
    Function    mOnLoginAttempt;
    Function    mOnCustomEvent;
    Function    mOnWorldOption;
    Function    mOnWorldToggle;
    Function    mOnScriptReload;
    Function    mOnScriptLoaded;
};

} // Namespace:: SqMod

#endif // _CORE_HPP_
