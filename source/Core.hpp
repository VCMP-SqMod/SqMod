#ifndef _CORE_HPP_
#define _CORE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"
#include "Base/Vector3.hpp"
#include "Base/Quaternion.hpp"
#include "Base/Color4.hpp"
#include "Base/ScriptSrc.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>
#include <unordered_map>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Circular locks employed by the central core.
*/
enum CoreCircularLocks
{
    CCL_RELOAD_SCRIPTS      = (1 << 0),
    CCL_EMIT_SERVER_OPTION  = (2 << 0)
};

/* ------------------------------------------------------------------------------------------------
 * Core module class responsible for managing resources.
*/
class Core
{
private:

    // --------------------------------------------------------------------------------------------
    static Core s_Inst; // Core instance.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Core();

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Core(const Core & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    Core(Core && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Core();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Core & operator = (const Core & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Core & operator = (Core && o) = delete;

protected:

    /* --------------------------------------------------------------------------------------------
     * Helper structure used to identify a blip entity instance on the server.
    */
    struct BlipInst
    {
        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        BlipInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(nullptr)
        {
            Core::Get().ResetInst(*this);
        }

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~BlipInst()
        {
            if (VALID_ENTITY(mID))
            {
                Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullObject());
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Destroy the entity instance from the server, if necessary.
        */
        void Destroy(bool destroy, Int32 header, Object & payload);

        // ----------------------------------------------------------------------------------------
        Int32           mID; // The unique number that identifies this entity on the server.
        Uint16          mFlags; // Various options and states that can be toggled on the instance.
        CBlip *         mInst; // Pointer to the actual instance used to interact this entity.
        Object          mObj; // Script object of the instance used to interact this entity.

        // ----------------------------------------------------------------------------------------
        Int32           mWorld; // The identifier of the world in which this blip was created.
        Int32           mScale; // The scale of the blip.

        // ----------------------------------------------------------------------------------------
        Int32           mSprID;

        // ----------------------------------------------------------------------------------------
        Vector3         mPosition;
        Color4          mColor;

        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;
    };

    /* --------------------------------------------------------------------------------------------
     * Helper structure used to identify a checkpoint entity instance on the server.
    */
    struct CheckpointInst
    {
        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        CheckpointInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(nullptr)
        {
            Core::Get().ResetInst(*this);
        }

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~CheckpointInst()
        {
            if (VALID_ENTITY(mID))
            {
                Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullObject());
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Destroy the entity instance from the server, if necessary.
        */
        void Destroy(bool destroy, Int32 header, Object & payload);

        // ----------------------------------------------------------------------------------------
        Int32           mID; // The unique number that identifies this entity on the server.
        Uint16          mFlags; // Various options and states that can be toggled on the instance.
        CCheckpoint *   mInst; // Pointer to the actual instance used to interact this entity.
        Object          mObj; // Script object of the instance used to interact this entity.

        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;

        // ----------------------------------------------------------------------------------------
        Function        mOnEntered;
        Function        mOnExited;
        Function        mOnWorld;
        Function        mOnRadius;
    };

    /* --------------------------------------------------------------------------------------------
     * Helper structure used to identify a key-bind entity instance on the server.
    */
    struct KeybindInst
    {
        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        KeybindInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(nullptr)
        {
            Core::Get().ResetInst(*this);
        }

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~KeybindInst()
        {
            if (VALID_ENTITY(mID))
            {
                Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullObject());
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Destroy the entity instance from the server, if necessary.
        */
        void Destroy(bool destroy, Int32 header, Object & payload);

        // ----------------------------------------------------------------------------------------
        Int32           mID; // The unique number that identifies this entity on the server.
        Uint16          mFlags; // Various options and states that can be toggled on the instance.
        CKeybind *      mInst; // Pointer to the actual instance used to interact this entity.
        Object          mObj; // Script object of the instance used to interact this entity.

        // ----------------------------------------------------------------------------------------
        Int32           mFirst; // Key-code of the first button from the triggering combination.
        Int32           mSecond; // Key-code of the second button from the triggering combination.
        Int32           mThird; // Key-code of the third button from the triggering combination.
        Int32           mRelease; // Whether the key-bind reacts to button press or release.

        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;

        // ----------------------------------------------------------------------------------------
        Function        mOnKeyPress;
        Function        mOnKeyRelease;
    };

    /* --------------------------------------------------------------------------------------------
     * Helper structure used to identify an object entity instance on the server.
    */
    struct ObjectInst
    {
        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        ObjectInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(nullptr)
        {
            Core::Get().ResetInst(*this);
        }

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~ObjectInst()
        {
            if (VALID_ENTITY(mID))
            {
                Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullObject());
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Destroy the entity instance from the server, if necessary.
        */
        void Destroy(bool destroy, Int32 header, Object & payload);

        // ----------------------------------------------------------------------------------------
        Int32           mID; // The unique number that identifies this entity on the server.
        Uint16          mFlags; // Various options and states that can be toggled on the instance.
        CObject *       mInst; // Pointer to the actual instance used to interact this entity.
        Object          mObj; // Script object of the instance used to interact this entity.

        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;

        // ----------------------------------------------------------------------------------------
        Function        mOnShot;
        Function        mOnTouched;
        Function        mOnWorld;
        Function        mOnAlpha;
        Function        mOnReport;
    };

    /* --------------------------------------------------------------------------------------------
     * Helper structure used to identify a pickup entity instance on the server.
    */
    struct PickupInst
    {
        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        PickupInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(nullptr)
        {
            Core::Get().ResetInst(*this);
        }

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~PickupInst()
        {
            if (VALID_ENTITY(mID))
            {
                Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullObject());
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Destroy the entity instance from the server, if necessary.
        */
        void Destroy(bool destroy, Int32 header, Object & payload);

        // ----------------------------------------------------------------------------------------
        Int32           mID; // The unique number that identifies this entity on the server.
        Uint16          mFlags; // Various options and states that can be toggled on the instance.
        CPickup *       mInst; // Pointer to the actual instance used to interact this entity.
        Object          mObj; // Script object of the instance used to interact this entity.

        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;

        // ----------------------------------------------------------------------------------------
        Function        mOnRespawn;
        Function        mOnClaimed;
        Function        mOnCollected;
        Function        mOnWorld;
        Function        mOnAlpha;
        Function        mOnAutomatic;
        Function        mOnAutoTimer;
    };

    /* --------------------------------------------------------------------------------------------
     * Helper structure used to identify a player entity instance on the server.
    */
    struct PlayerInst
    {
        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        PlayerInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(nullptr)
        {
            Core::Get().ResetInst(*this);
        }

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~PlayerInst()
        {
            if (VALID_ENTITY(mID))
            {
                Destroy(false, SQMOD_DESTROY_CLEANUP, NullObject());
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Destroy the entity instance from the server, if necessary.
        */
        void Destroy(bool destroy, Int32 header, Object & payload);

        // ----------------------------------------------------------------------------------------
        Int32           mID; // The unique number that identifies this entity on the server.
        Uint16          mFlags; // Various options and states that can be toggled on the instance.
        CPlayer *       mInst; // Pointer to the actual instance used to interact this entity.
        Object          mObj; // Script object of the instance used to interact this entity.

        // ----------------------------------------------------------------------------------------
        SQInteger       mTrackPosition; // The number of times to track position changes.
        SQInteger       mTrackHeading; // The number of times to track heading changes.

        // ----------------------------------------------------------------------------------------
        Int32           mTrackPositionHeader; // Header to send when triggering position callback.
        Object          mTrackPositionPayload; // Payload to send when triggering position callback.

        // ----------------------------------------------------------------------------------------
        Int32           mKickBanHeader; // Header to send when triggering kick/ban callback.
        Object          mKickBanPayload; // Payload to send when triggering kick/ban callback.

        // ----------------------------------------------------------------------------------------
        Int32           mLastWeapon; // Last known weapon of the player entity.
        Float32         mLastHealth; // Last known health of the player entity.
        Float32         mLastArmour; // Last known armor of the player entity.
        Float32         mLastHeading; // Last known heading of the player entity.
        Vector3         mLastPosition; // Last known position of the player entity.

        // ----------------------------------------------------------------------------------------
        Int32           mAuthority; // The authority level of the managed player.

        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;

        // ----------------------------------------------------------------------------------------
        Function        mOnRequestClass;
        Function        mOnRequestSpawn;
        Function        mOnSpawn;
        Function        mOnWasted;
        Function        mOnKilled;
        Function        mOnEmbarking;
        Function        mOnEmbarked;
        Function        mOnDisembark;
        Function        mOnRename;
        Function        mOnState;
        Function        mOnStateNone;
        Function        mOnStateNormal;
        Function        mOnStateAim;
        Function        mOnStateDriver;
        Function        mOnStatePassenger;
        Function        mOnStateEnterDriver;
        Function        mOnStateEnterPassenger;
        Function        mOnStateExit;
        Function        mOnStateUnspawned;
        Function        mOnAction;
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
        Function        mOnBurning;
        Function        mOnCrouching;
        Function        mOnGameKeys;
        Function        mOnStartTyping;
        Function        mOnStopTyping;
        Function        mOnAway;
        Function        mOnMessage;
        Function        mOnCommand;
        Function        mOnPrivateMessage;
        Function        mOnKeyPress;
        Function        mOnKeyRelease;
        Function        mOnSpectate;
        Function        mOnCrashreport;
        Function        mOnObjectShot;
        Function        mOnObjectTouched;
        Function        mOnPickupClaimed;
        Function        mOnPickupCollected;
        Function        mOnCheckpointEntered;
        Function        mOnCheckpointExited;
        Function        mOnClientScriptData;
        Function        mOnUpdate;
        Function        mOnHealth;
        Function        mOnArmour;
        Function        mOnWeapon;
        Function        mOnHeading;
        Function        mOnPosition;
        Function        mOnOption;
        Function        mOnAdmin;
        Function        mOnWorld;
        Function        mOnTeam;
        Function        mOnSkin;
        Function        mOnMoney;
        Function        mOnScore;
        Function        mOnWantedLevel;
        Function        mOnImmunity;
        Function        mOnAlpha;
    };

    /* --------------------------------------------------------------------------------------------
     * Helper structure used to identify a vehicle entity instance on the server.
    */
    struct VehicleInst
    {
        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        VehicleInst() : mID(-1), mFlags(ENF_DEFAULT), mInst(nullptr)
        {
            Core::Get().ResetInst(*this);
        }

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~VehicleInst()
        {
            if (VALID_ENTITY(mID))
            {
                Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullObject());
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Destroy the entity instance from the server, if necessary.
        */
        void Destroy(bool destroy, Int32 header, Object & payload);

        // ----------------------------------------------------------------------------------------
        Int32           mID; // The unique number that identifies this entity on the server.
        Uint16          mFlags; // Various options and states that can be toggled on the instance.
        CVehicle *      mInst; // Pointer to the actual instance used to interact this entity.
        Object          mObj; // Script object of the instance used to interact this entity.

        // ----------------------------------------------------------------------------------------
        SQInteger       mTrackPosition; // The number of times to track position changes.
        SQInteger       mTrackRotation; // The number of times to track rotation changes.

        // ----------------------------------------------------------------------------------------
        Int32           mLastPrimaryColour; // Last known secondary-color of the player entity.
        Int32           mLastSecondaryColour; // Last known primary-color of the player entity.
        Float32         mLastHealth; // Last known health of the player entity.
        Vector3         mLastPosition; // Last known position of the player entity.
        Quaternion      mLastRotation; // Last known rotation of the player entity.

        // ----------------------------------------------------------------------------------------
        Function        mOnDestroyed;
        Function        mOnCustom;

        // ----------------------------------------------------------------------------------------
        Function        mOnEmbarking;
        Function        mOnEmbarked;
        Function        mOnDisembark;
        Function        mOnExplode;
        Function        mOnRespawn;
        Function        mOnUpdate;
        Function        mOnColour;
        Function        mOnHealth;
        Function        mOnPosition;
        Function        mOnRotation;
        Function        mOnOption;
        Function        mOnWorld;
        Function        mOnImmunity;
        Function        mOnPartStatus;
        Function        mOnTyreStatus;
        Function        mOnDamageData;
        Function        mOnRadio;
        Function        mOnHandlingRule;
    };

public:

    // --------------------------------------------------------------------------------------------
    typedef std::vector< BlipInst >         Blips; // Blips entity instances container.
    typedef std::vector< CheckpointInst >   Checkpoints; // Checkpoints entity instances container.
    typedef std::vector< KeybindInst >      Keybinds; // Key-binds entity instances container.
    typedef std::vector< ObjectInst >       Objects; // Objects entity instances container.
    typedef std::vector< PickupInst >       Pickups; // Pickups entity instances container.
    typedef std::vector< PlayerInst >       Players; // Players entity instances container.
    typedef std::vector< VehicleInst >      Vehicles; // Vehicles entity instances container.

    // --------------------------------------------------------------------------------------------
    typedef std::vector< ScriptSrc >                Scripts; // List of loaded scripts.

    // --------------------------------------------------------------------------------------------
    typedef std::unordered_map< String, String >    Options; // List of custom options.

private:

    // --------------------------------------------------------------------------------------------
    Int32                           m_State; // Current plug-in state.
    HSQUIRRELVM                     m_VM; // Script virtual machine.
    Scripts                         m_Scripts; // Loaded scripts objects.
    Options                         m_Options; // Custom configuration options.

    // --------------------------------------------------------------------------------------------
    Blips                           m_Blips; // Blips pool.
    Checkpoints                     m_Checkpoints; // Checkpoints pool.
    Keybinds                        m_Keybinds; // Key-binds pool.
    Objects                         m_Objects; // Objects pool.
    Pickups                         m_Pickups; // Pickups pool.
    Players                         m_Players; // Players pool.
    Vehicles                        m_Vehicles; // Vehicles pool.

    // --------------------------------------------------------------------------------------------
    Uint32                          m_CircularLocks; // Prevent events from triggering themselves.

    // --------------------------------------------------------------------------------------------
    Int32                           m_ReloadHeader; // The specified reload header.
    Object                          m_ReloadPayload; // The specified reload payload.

    // --------------------------------------------------------------------------------------------
    CStr                            m_IncomingNameBuffer; // Name of an incoming connection.
    size_t                          m_IncomingNameCapacity; // Incoming connection name size.

    // --------------------------------------------------------------------------------------------
    bool                            m_Debugging; // Enable debugging features, if any.
    bool                            m_Executed; // Whether the scripts were executed.
    bool                            m_Shutdown; // Whether the server currently shutting down.

    // --------------------------------------------------------------------------------------------
    Object                          m_NullBlip; // Null Blips instance.
    Object                          m_NullCheckpoint; // Null Checkpoints instance.
    Object                          m_NullKeybind; // Null Key-instance pool.
    Object                          m_NullObject; // Null Objects instance.
    Object                          m_NullPickup; // Null Pickups instance.
    Object                          m_NullPlayer; // Null Players instance.
    Object                          m_NullVehicle; // Null Vehicles instance.

public:

    /* --------------------------------------------------------------------------------------------
     * Retrieve the core instance.
    */
    static Core & Get()
    {
        return s_Inst;
    }

    /* --------------------------------------------------------------------------------------------
     * Initialize the plug-in core.
    */
    bool Initialize();

    /* --------------------------------------------------------------------------------------------
     * Load and execute plug-in resources.
    */
    bool Execute();

    /* --------------------------------------------------------------------------------------------
     * Terminate the plug-in core.
    */
    void Terminate(bool shutdown = false);

    /* --------------------------------------------------------------------------------------------
     * Reload the plug-in core.
    */
    bool Reload();

    /* --------------------------------------------------------------------------------------------
     * Modify the current plug-in state.
    */
    void SetState(Int32 val)
    {
        m_State = val;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current plug-in state.
    */
    Int32 GetState() const
    {
        return m_State;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether debugging option was enabled in the plugin.
    */
    bool IsDebugging() const
    {
        return m_Debugging;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether all queued scripts were executed and the plugin fully started.
    */
    bool IsExecuted() const
    {
        return m_Executed;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the server is currently in the process of shutting down.
    */
    bool ShuttingDown() const
    {
        return m_Shutdown;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value of the specified option.
    */
    CSStr GetOption(CSStr name) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value of the specified option or the fall back value if it doesn't exist.
    */
    CSStr GetOption(CSStr name, CSStr value) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the value of the specified option.
    */
    void SetOption(CSStr name, CSStr value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the virtual machine.
    */
    HSQUIRRELVM GetVM() const
    {
        return m_VM;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the circular locks.
    */
    Uint32 & GetCircularLock()
    {
        return m_CircularLocks;
    }

    /* --------------------------------------------------------------------------------------------
     * See if certain circular locks are enabled.
    */
    bool IsCircularLock(Uint32 lock) const
    {
        return (m_CircularLocks & lock);
    }

    /* --------------------------------------------------------------------------------------------
     * Set the header and payload for the reload.
    */
    void SetReloadInfo(Int32 header, Object & payload)
    {
        m_ReloadHeader = header;
        m_ReloadPayload = payload;
    }

    /* --------------------------------------------------------------------------------------------
     * Reset the header and payload for the reload.
    */
    void ResetReloadInfo()
    {
        m_ReloadHeader = -1;
        m_ReloadPayload.Release();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the specified reload header.
    */
    Int32 GetReloadHeader() const
    {
        return m_ReloadHeader;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the specified reload header.
    */
    Object & GetReloadPayload()
    {
        return m_ReloadPayload;
    }

    /* --------------------------------------------------------------------------------------------
     * Adds a script to the load queue.
    */
    bool LoadScript(CSStr filepath, bool delay);

    /* --------------------------------------------------------------------------------------------
     * Modify the name for the currently assigned incoming connection.
    */
    void SetIncomingName(CSStr name);

    /* --------------------------------------------------------------------------------------------
     * retrieve the name for the currently assigned incoming connection..
    */
    CSStr GetIncomingName()
    {
        return (!m_IncomingNameBuffer) ? _SC("") : m_IncomingNameBuffer;
    }

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
     * Entity scanners.
    */
    void ImportBlips();
    void ImportCheckpoints();
    void ImportKeybinds();
    void ImportObjects();
    void ImportPickups();
    void ImportPlayers();
    void ImportVehicles();

    /* --------------------------------------------------------------------------------------------
     * Entity allocators.
    */
    BlipInst & AllocBlip(Int32 id, bool owned, Int32 header, Object & payload);
    CheckpointInst & AllocCheckpoint(Int32 id, bool owned, Int32 header, Object & payload);
    KeybindInst & AllocKeybind(Int32 id, bool owned, Int32 header, Object & payload);
    ObjectInst & AllocObject(Int32 id, bool owned, Int32 header, Object & payload);
    PickupInst & AllocPickup(Int32 id, bool owned, Int32 header, Object & payload);
    VehicleInst & AllocVehicle(Int32 id, bool owned, Int32 header, Object & payload);

    /* --------------------------------------------------------------------------------------------
     * Entity deallocator.
    */
    void DeallocBlip(Int32 id, bool destroy, Int32 header, Object & payload);
    void DeallocCheckpoint(Int32 id, bool destroy, Int32 header, Object & payload);
    void DeallocKeybind(Int32 id, bool destroy, Int32 header, Object & payload);
    void DeallocObject(Int32 id, bool destroy, Int32 header, Object & payload);
    void DeallocPickup(Int32 id, bool destroy, Int32 header, Object & payload);
    void DeallocVehicle(Int32 id, bool destroy, Int32 header, Object & payload);

public:

    /* --------------------------------------------------------------------------------------------
     * Entity creators.
    */
    Object & NewBlip(Int32 index, Int32 world, Float32 x, Float32 y, Float32 z,
                        Int32 scale, Uint32 color, Int32 sprid,
                        Int32 header, Object & payload);

    Object & NewCheckpoint(Int32 player, Int32 world, bool sphere, Float32 x, Float32 y, Float32 z,
                        Uint8 r, Uint8 g, Uint8 b, Uint8 a, Float32 radius,
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
     * Entity destroyers.
    */
    bool DelBlip(Int32 id, Int32 header, Object & payload);
    bool DelCheckpoint(Int32 id, Int32 header, Object & payload);
    bool DelKeybind(Int32 id, Int32 header, Object & payload);
    bool DelObject(Int32 id, Int32 header, Object & payload);
    bool DelPickup(Int32 id, Int32 header, Object & payload);
    bool DelVehicle(Int32 id, Int32 header, Object & payload);

    /* --------------------------------------------------------------------------------------------
     * Entity retrievers.
    */
    BlipInst & GetBlip(Int32 id) { return m_Blips.at(id); }
    CheckpointInst & GetCheckpoint(Int32 id) { return m_Checkpoints.at(id); }
    KeybindInst & GetKeybind(Int32 id) { return m_Keybinds.at(id); }
    ObjectInst & GetObject(Int32 id) { return m_Objects.at(id); }
    PickupInst & GetPickup(Int32 id) { return m_Pickups.at(id); }
    PlayerInst & GetPlayer(Int32 id) { return m_Players.at(id); }
    VehicleInst & GetVehicle(Int32 id) { return m_Vehicles.at(id); }

    /* --------------------------------------------------------------------------------------------
     * Pool retrievers.
    */
    const Blips & GetBlips() const { return m_Blips; }
    const Checkpoints & GetCheckpoints() const { return m_Checkpoints; }
    const Keybinds & GetKeybinds() const { return m_Keybinds; }
    const Objects & GetObjects() const { return m_Objects; }
    const Pickups & GetPickups() const { return m_Pickups; }
    const Players & GetPlayers() const { return m_Players; }
    const Vehicles & GetVehicles() const { return m_Vehicles; }

    /* --------------------------------------------------------------------------------------------
     * Null instance retrievers.
    */
    Object & GetNullBlip() { return m_NullBlip; }
    Object & GetNullCheckpoint() { return m_NullCheckpoint; }
    Object & GetNullKeybind() { return m_NullKeybind; }
    Object & GetNullObject() { return m_NullObject; }
    Object & GetNullPickup() { return m_NullPickup; }
    Object & GetNullPlayer() { return m_NullPlayer; }
    Object & GetNullVehicle() { return m_NullVehicle; }

    /* --------------------------------------------------------------------------------------------
     * Container cleaner.
    */
    void ClearContainer(EntityType type);

protected:

    /* --------------------------------------------------------------------------------------------
     * Instance cleaners.
    */
    static void ResetInst(BlipInst & inst);
    static void ResetInst(CheckpointInst & inst);
    static void ResetInst(KeybindInst & inst);
    static void ResetInst(ObjectInst & inst);
    static void ResetInst(PickupInst & inst);
    static void ResetInst(PlayerInst & inst);
    static void ResetInst(VehicleInst & inst);

    /* --------------------------------------------------------------------------------------------
     * Bindings cleaners.
    */
    static void ResetFunc(BlipInst & inst);
    static void ResetFunc(CheckpointInst & inst);
    static void ResetFunc(KeybindInst & inst);
    static void ResetFunc(ObjectInst & inst);
    static void ResetFunc(PickupInst & inst);
    static void ResetFunc(PlayerInst & inst);
    static void ResetFunc(VehicleInst & inst);
    static void ResetFunc();

    // --------------------------------------------------------------------------------------------
    static void Emit(Function & func)
    {
        if (!func.IsNull())
        {
            func.Execute();
        }
    }

    // --------------------------------------------------------------------------------------------
    template < typename... Args > static void Emit(Function & func, Args&&... args)
    {
        if (!func.IsNull())
        {
            func.Execute(std::forward< Args >(args)...);
        }
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
     * Emit a custom event.
    */
    void EmitCustomEvent(Int32 group, Int32 header, Object & payload);

    /* --------------------------------------------------------------------------------------------
     * Server events.
    */
    void EmitBlipCreated(Int32 blip, Int32 header, Object & payload);
    void EmitCheckpointCreated(Int32 forcefield, Int32 header, Object & payload);
    void EmitKeybindCreated(Int32 keybind, Int32 header, Object & payload);
    void EmitObjectCreated(Int32 object, Int32 header, Object & payload);
    void EmitPickupCreated(Int32 pickup, Int32 header, Object & payload);
    void EmitPlayerCreated(Int32 player, Int32 header, Object & payload);
    void EmitVehicleCreated(Int32 vehicle, Int32 header, Object & payload);
    void EmitBlipDestroyed(Int32 blip, Int32 header, Object & payload);
    void EmitCheckpointDestroyed(Int32 forcefield, Int32 header, Object & payload);
    void EmitKeybindDestroyed(Int32 keybind, Int32 header, Object & payload);
    void EmitObjectDestroyed(Int32 object, Int32 header, Object & payload);
    void EmitPickupDestroyed(Int32 pickup, Int32 header, Object & payload);
    void EmitPlayerDestroyed(Int32 player, Int32 header, Object & payload);
    void EmitVehicleDestroyed(Int32 vehicle, Int32 header, Object & payload);
    void EmitBlipCustom(Int32 blip, Int32 header, Object & payload);
    void EmitCheckpointCustom(Int32 forcefield, Int32 header, Object & payload);
    void EmitKeybindCustom(Int32 keybind, Int32 header, Object & payload);
    void EmitObjectCustom(Int32 object, Int32 header, Object & payload);
    void EmitPickupCustom(Int32 pickup, Int32 header, Object & payload);
    void EmitPlayerCustom(Int32 player, Int32 header, Object & payload);
    void EmitVehicleCustom(Int32 vehicle, Int32 header, Object & payload);
    void EmitServerStartup();
    void EmitServerShutdown();
    void EmitServerFrame(Float32 elapsed_time);
    void EmitPluginCommand(Uint32 command_identifier, CCStr message);
    void EmitIncomingConnection(CStr player_name, size_t name_buffer_size, CCStr user_password, CCStr ip_address);
    void EmitPlayerRequestClass(Int32 player_id, Int32 offset);
    void EmitPlayerRequestSpawn(Int32 player_id);
    void EmitPlayerSpawn(Int32 player_id);
    void EmitPlayerWasted(Int32 player_id, Int32 reason);
    void EmitPlayerKilled(Int32 player_id, Int32 killer_id, Int32 reason, vcmpBodyPart body_part, bool team_kill);
    void EmitPlayerEmbarking(Int32 player_id, Int32 vehicle_id, Int32 slot_index);
    void EmitPlayerEmbarked(Int32 player_id, Int32 vehicle_id, Int32 slot_index);
    void EmitPlayerDisembark(Int32 player_id, Int32 vehicle_id);
    void EmitPlayerRename(Int32 player_id, CCStr old_name, CCStr new_name);
    void EmitPlayerState(Int32 player_id, Int32 old_state, Int32 new_state);
    void EmitStateNone(Int32 player_id, Int32 old_state);
    void EmitStateNormal(Int32 player_id, Int32 old_state);
    void EmitStateAim(Int32 player_id, Int32 old_state);
    void EmitStateDriver(Int32 player_id, Int32 old_state);
    void EmitStatePassenger(Int32 player_id, Int32 old_state);
    void EmitStateEnterDriver(Int32 player_id, Int32 old_state);
    void EmitStateEnterPassenger(Int32 player_id, Int32 old_state);
    void EmitStateExit(Int32 player_id, Int32 old_state);
    void EmitStateUnspawned(Int32 player_id, Int32 old_state);
    void EmitPlayerAction(Int32 player_id, Int32 old_action, Int32 new_action);
    void EmitActionNone(Int32 player_id, Int32 old_action);
    void EmitActionNormal(Int32 player_id, Int32 old_action);
    void EmitActionAiming(Int32 player_id, Int32 old_action);
    void EmitActionShooting(Int32 player_id, Int32 old_action);
    void EmitActionJumping(Int32 player_id, Int32 old_action);
    void EmitActionLieDown(Int32 player_id, Int32 old_action);
    void EmitActionGettingUp(Int32 player_id, Int32 old_action);
    void EmitActionJumpVehicle(Int32 player_id, Int32 old_action);
    void EmitActionDriving(Int32 player_id, Int32 old_action);
    void EmitActionDying(Int32 player_id, Int32 old_action);
    void EmitActionWasted(Int32 player_id, Int32 old_action);
    void EmitActionEmbarking(Int32 player_id, Int32 old_action);
    void EmitActionDisembarking(Int32 player_id, Int32 old_action);
    void EmitPlayerBurning(Int32 player_id, bool is_on_fire);
    void EmitPlayerCrouching(Int32 player_id, bool is_crouching);
    void EmitPlayerGameKeys(Int32 player_id, Uint32 old_keys, Uint32 new_keys);
    void EmitPlayerStartTyping(Int32 player_id);
    void EmitPlayerStopTyping(Int32 player_id);
    void EmitPlayerAway(Int32 player_id, bool is_away);
    void EmitPlayerMessage(Int32 player_id, CCStr message);
    void EmitPlayerCommand(Int32 player_id, CCStr message);
    void EmitPlayerPrivateMessage(Int32 player_id, Int32 target_player_id, CCStr message);
    void EmitPlayerKeyPress(Int32 player_id, Int32 bind_id);
    void EmitPlayerKeyRelease(Int32 player_id, Int32 bind_id);
    void EmitPlayerSpectate(Int32 player_id, Int32 target_player_id);
    void EmitPlayerCrashreport(Int32 player_id, CCStr report);
    void EmitVehicleExplode(Int32 vehicle_id);
    void EmitVehicleRespawn(Int32 vehicle_id);
    void EmitObjectShot(Int32 object_id, Int32 player_id, Int32 weapon_id);
    void EmitObjectTouched(Int32 object_id, Int32 player_id);
    void EmitPickupClaimed(Int32 pickup_id, Int32 player_id);
    void EmitPickupCollected(Int32 pickup_id, Int32 player_id);
    void EmitPickupRespawn(Int32 pickup_id);
    void EmitCheckpointEntered(Int32 checkpoint_id, Int32 player_id);
    void EmitCheckpointExited(Int32 checkpoint_id, Int32 player_id);

    /* --------------------------------------------------------------------------------------------
     * Miscellaneous events.
    */
    void EmitCheckpointWorld(Int32 checkpoint_id, Int32 old_world, Int32 new_world);
    void EmitCheckpointRadius(Int32 checkpoint_id, Float32 old_radius, Float32 new_radius);
    void EmitObjectWorld(Int32 object_id, Int32 old_world, Int32 new_world);
    void EmitObjectAlpha(Int32 object_id, Int32 old_alpha, Int32 new_alpha, Int32 time);
    void EmitPickupWorld(Int32 pickup_id, Int32 old_world, Int32 new_world);
    void EmitPickupAlpha(Int32 pickup_id, Int32 old_alpha, Int32 new_alpha);
    void EmitPickupAutomatic(Int32 pickup_id, bool old_status, bool new_status);
    void EmitPickupAutoTimer(Int32 pickup_id, Int32 old_timer, Int32 new_timer);
    void EmitObjectReport(Int32 object_id, bool old_status, bool new_status, bool touched);
    void EmitPlayerHealth(Int32 player_id, Float32 old_health, Float32 new_health);
    void EmitPlayerArmour(Int32 player_id, Float32 old_armour, Float32 new_armour);
    void EmitPlayerWeapon(Int32 player_id, Int32 old_weapon, Int32 new_weapon);
    void EmitPlayerHeading(Int32 player_id, Float32 old_heading, Float32 new_heading);
    void EmitPlayerPosition(Int32 player_id);
    void EmitPlayerOption(Int32 player_id, Int32 option_id, bool value, Int32 header, Object & payload);
    void EmitPlayerAdmin(Int32 player_id, bool old_status, bool new_status);
    void EmitPlayerWorld(Int32 player_id, Int32 old_world, Int32 new_world, bool secondary);
    void EmitPlayerTeam(Int32 player_id, Int32 old_team, Int32 new_team);
    void EmitPlayerSkin(Int32 player_id, Int32 old_skin, Int32 new_skin);
    void EmitPlayerMoney(Int32 player_id, Int32 old_money, Int32 new_money);
    void EmitPlayerScore(Int32 player_id, Int32 old_score, Int32 new_score);
    void EmitPlayerWantedLevel(Int32 player_id, Int32 old_level, Int32 new_level);
    void EmitPlayerImmunity(Int32 player_id, Int32 old_immunity, Int32 new_immunity);
    void EmitPlayerAlpha(Int32 player_id, Int32 old_alpha, Int32 new_alpha, Int32 fade);
    void EmitVehicleColour(Int32 vehicle_id, Int32 changed);
    void EmitVehicleHealth(Int32 vehicle_id, Float32 old_health, Float32 new_health);
    void EmitVehiclePosition(Int32 vehicle_id);
    void EmitVehicleRotation(Int32 vehicle_id);
    void EmitVehicleOption(Int32 vehicle_id, Int32 option_id, bool value, Int32 header, Object & payload);
    void EmitVehicleWorld(Int32 vehicle_id, Int32 old_world, Int32 new_world);
    void EmitVehicleImmunity(Int32 vehicle_id, Int32 old_immunity, Int32 new_immunity);
    void EmitVehiclePartStatus(Int32 vehicle_id, Int32 part, Int32 old_status, Int32 new_status);
    void EmitVehicleTyreStatus(Int32 vehicle_id, Int32 tyre, Int32 old_status, Int32 new_status);
    void EmitVehicleDamageData(Int32 vehicle_id, Uint32 old_data, Uint32 new_data);
    void EmitVehicleRadio(Int32 vehicle_id, Int32 old_radio, Int32 new_radio);
    void EmitVehicleHandlingRule(Int32 vehicle_id, Int32 rule, Float32 old_data, Float32 new_data);
    void EmitServerOption(Int32 option, bool value, Int32 header, Object & payload);
    void EmitScriptReload(Int32 header, Object & payload);
    void EmitScriptLoaded();

    /* --------------------------------------------------------------------------------------------
     * Entity pool changes events.
    */
    void EmitEntityPool(vcmpEntityPool entity_type, Int32 entity_id, bool is_deleted);

    /* --------------------------------------------------------------------------------------------
     * Entity update events.
    */
    void EmitPlayerUpdate(Int32 player_id, vcmpPlayerUpdate update_type);
    void EmitVehicleUpdate(Int32 vehicle_id, vcmpVehicleUpdate update_type);

    /* --------------------------------------------------------------------------------------------
     * Client data streams event.
    */
    void EmitClientScriptData(Int32 player_id, const uint8_t * data, size_t size);

    /* --------------------------------------------------------------------------------------------
     * Retrieve global event bindings.
    */
    Function & GetEvent(Int32 evid);

    /* --------------------------------------------------------------------------------------------
     * Retrieve local event bindings.
    */
    Function & GetBlipEvent(Int32 id, Int32 evid);
    Function & GetCheckpointEvent(Int32 id, Int32 evid);
    Function & GetKeybindEvent(Int32 id, Int32 evid);
    Function & GetObjectEvent(Int32 id, Int32 evid);
    Function & GetPickupEvent(Int32 id, Int32 evid);
    Function & GetPlayerEvent(Int32 id, Int32 evid);
    Function & GetVehicleEvent(Int32 id, Int32 evid);

private:

    /* --------------------------------------------------------------------------------------------
     * Global event bindings.
    */
    Function    mOnCustomEvent;
    Function    mOnBlipCreated;
    Function    mOnCheckpointCreated;
    Function    mOnKeybindCreated;
    Function    mOnObjectCreated;
    Function    mOnPickupCreated;
    Function    mOnPlayerCreated;
    Function    mOnVehicleCreated;
    Function    mOnBlipDestroyed;
    Function    mOnCheckpointDestroyed;
    Function    mOnKeybindDestroyed;
    Function    mOnObjectDestroyed;
    Function    mOnPickupDestroyed;
    Function    mOnPlayerDestroyed;
    Function    mOnVehicleDestroyed;
    Function    mOnBlipCustom;
    Function    mOnCheckpointCustom;
    Function    mOnKeybindCustom;
    Function    mOnObjectCustom;
    Function    mOnPickupCustom;
    Function    mOnPlayerCustom;
    Function    mOnVehicleCustom;
    Function    mOnServerStartup;
    Function    mOnServerShutdown;
    Function    mOnServerFrame;
    Function    mOnIncomingConnection;
    Function    mOnPlayerRequestClass;
    Function    mOnPlayerRequestSpawn;
    Function    mOnPlayerSpawn;
    Function    mOnPlayerWasted;
    Function    mOnPlayerKilled;
    Function    mOnPlayerEmbarking;
    Function    mOnPlayerEmbarked;
    Function    mOnPlayerDisembark;
    Function    mOnPlayerRename;
    Function    mOnPlayerState;
    Function    mOnStateNone;
    Function    mOnStateNormal;
    Function    mOnStateAim;
    Function    mOnStateDriver;
    Function    mOnStatePassenger;
    Function    mOnStateEnterDriver;
    Function    mOnStateEnterPassenger;
    Function    mOnStateExit;
    Function    mOnStateUnspawned;
    Function    mOnPlayerAction;
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
    Function    mOnPlayerBurning;
    Function    mOnPlayerCrouching;
    Function    mOnPlayerGameKeys;
    Function    mOnPlayerStartTyping;
    Function    mOnPlayerStopTyping;
    Function    mOnPlayerAway;
    Function    mOnPlayerMessage;
    Function    mOnPlayerCommand;
    Function    mOnPlayerPrivateMessage;
    Function    mOnPlayerKeyPress;
    Function    mOnPlayerKeyRelease;
    Function    mOnPlayerSpectate;
    Function    mOnPlayerCrashreport;
    Function    mOnVehicleExplode;
    Function    mOnVehicleRespawn;
    Function    mOnObjectShot;
    Function    mOnObjectTouched;
    Function    mOnObjectWorld;
    Function    mOnObjectAlpha;
    Function    mOnObjectReport;
    Function    mOnPickupClaimed;
    Function    mOnPickupCollected;
    Function    mOnPickupRespawn;
    Function    mOnPickupWorld;
    Function    mOnPickupAlpha;
    Function    mOnPickupAutomatic;
    Function    mOnPickupAutoTimer;
    Function    mOnCheckpointEntered;
    Function    mOnCheckpointExited;
    Function    mOnCheckpointWorld;
    Function    mOnCheckpointRadius;
    Function    mOnEntityPool;
    Function    mOnClientScriptData;
    Function    mOnPlayerUpdate;
    Function    mOnVehicleUpdate;
    Function    mOnPlayerHealth;
    Function    mOnPlayerArmour;
    Function    mOnPlayerWeapon;
    Function    mOnPlayerHeading;
    Function    mOnPlayerPosition;
    Function    mOnPlayerOption;
    Function    mOnPlayerAdmin;
    Function    mOnPlayerWorld;
    Function    mOnPlayerTeam;
    Function    mOnPlayerSkin;
    Function    mOnPlayerMoney;
    Function    mOnPlayerScore;
    Function    mOnPlayerWantedLevel;
    Function    mOnPlayerImmunity;
    Function    mOnPlayerAlpha;
    Function    mOnVehicleColour;
    Function    mOnVehicleHealth;
    Function    mOnVehiclePosition;
    Function    mOnVehicleRotation;
    Function    mOnVehicleOption;
    Function    mOnVehicleWorld;
    Function    mOnVehicleImmunity;
    Function    mOnVehiclePartStatus;
    Function    mOnVehicleTyreStatus;
    Function    mOnVehicleDamageData;
    Function    mOnVehicleRadio;
    Function    mOnVehicleHandlingRule;
    Function    mOnServerOption;
    Function    mOnScriptReload;
    Function    mOnScriptLoaded;
};

} // Namespace:: SqMod

#endif // _CORE_HPP_
