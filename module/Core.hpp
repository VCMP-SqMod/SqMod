#pragma once


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
    CCL_RELOAD_SCRIPTS      = (1u << 0u),
    CCL_EMIT_SERVER_OPTION  = (1u << 1u)
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
    Core() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Core();

protected:

    // --------------------------------------------------------------------------------------------
    typedef std::vector< std::pair< Area *, LightObj > > AreaList; // List of colided areas.

    /* --------------------------------------------------------------------------------------------
     * Helper structure used to identify a blip entity instance on the server.
    */
    struct BlipInst
    {
        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        BlipInst() = default;
        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~BlipInst()
        {
            if (VALID_ENTITY(mID))
            {
                Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullLightObj());
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Destroy the entity instance from the server, if necessary.
        */
        void Destroy(bool destroy, Int32 header, LightObj & payload);

        /* ----------------------------------------------------------------------------------------
         * Reset the instance to the default values.
        */
        void ResetInstance();

        /* ----------------------------------------------------------------------------------------
         * Create the associated signals.
        */
        void InitEvents();

        /* ----------------------------------------------------------------------------------------
         * Clear the associated signals.
        */
        void DropEvents();

        // ----------------------------------------------------------------------------------------
        Int32           mID{-1}; // The unique number that identifies this entity on the server.
        Uint32          mFlags{ENF_DEFAULT}; // Various options and states that can be toggled on the instance.
        CBlip *         mInst{nullptr}; // Pointer to the actual instance used to interact this entity.
        LightObj        mObj{}; // Script object of the instance used to interact this entity.

        // ----------------------------------------------------------------------------------------
        Int32           mWorld{-1}; // The identifier of the world in which this blip was created.
        Int32           mScale{-1}; // The scale of the blip.

        // ----------------------------------------------------------------------------------------
        Int32           mSprID{-1};

        // ----------------------------------------------------------------------------------------
        LightObj        mEvents{}; // Table containing the emitted entity events.

        // ----------------------------------------------------------------------------------------
        Vector3         mPosition{};
        Color4          mColor{};

        // ----------------------------------------------------------------------------------------
        SignalPair      mOnDestroyed{};
        SignalPair      mOnCustom{};
    };

    /* --------------------------------------------------------------------------------------------
     * Helper structure used to identify a checkpoint entity instance on the server.
    */
    struct CheckpointInst
    {
        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        CheckpointInst() = default;

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~CheckpointInst()
        {
            if (VALID_ENTITY(mID))
            {
                Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullLightObj());
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Destroy the entity instance from the server, if necessary.
        */
        void Destroy(bool destroy, Int32 header, LightObj & payload);

        /* ----------------------------------------------------------------------------------------
         * Reset the instance to the default values.
        */
        void ResetInstance();

        /* ----------------------------------------------------------------------------------------
         * Create the associated signals.
        */
        void InitEvents();

        /* ----------------------------------------------------------------------------------------
         * Clear the associated signals.
        */
        void DropEvents();

        // ----------------------------------------------------------------------------------------
        Int32           mID{-1}; // The unique number that identifies this entity on the server.
        Uint32          mFlags{ENF_DEFAULT}; // Various options and states that can be toggled on the instance.
        CCheckpoint *   mInst{nullptr}; // Pointer to the actual instance used to interact this entity.
        LightObj        mObj{}; // Script object of the instance used to interact this entity.

        // ----------------------------------------------------------------------------------------
        LightObj        mEvents{}; // Table containing the emitted entity events.

        // ----------------------------------------------------------------------------------------
        SignalPair      mOnDestroyed{};
        SignalPair      mOnCustom{};
#if SQMOD_SDK_LEAST(2, 1)
        SignalPair      mOnStream{};
#endif
        // ----------------------------------------------------------------------------------------
        SignalPair      mOnEntered{};
        SignalPair      mOnExited{};
        SignalPair      mOnWorld{};
        SignalPair      mOnRadius{};
    };

    /* --------------------------------------------------------------------------------------------
     * Helper structure used to identify a key-bind entity instance on the server.
    */
    struct KeybindInst
    {
        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        KeybindInst() = default;

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~KeybindInst()
        {
            if (VALID_ENTITY(mID))
            {
                Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullLightObj());
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Destroy the entity instance from the server, if necessary.
        */
        void Destroy(bool destroy, Int32 header, LightObj & payload);

        /* ----------------------------------------------------------------------------------------
         * Reset the instance to the default values.
        */
        void ResetInstance();

        /* ----------------------------------------------------------------------------------------
         * Create the associated signals.
        */
        void InitEvents();

        /* ----------------------------------------------------------------------------------------
         * Clear the associated signals.
        */
        void DropEvents();

        // ----------------------------------------------------------------------------------------
        Int32           mID{-1}; // The unique number that identifies this entity on the server.
        Uint32          mFlags{ENF_DEFAULT}; // Various options and states that can be toggled on the instance.
        CKeybind *      mInst{nullptr}; // Pointer to the actual instance used to interact this entity.
        LightObj        mObj{}; // Script object of the instance used to interact this entity.

        // ----------------------------------------------------------------------------------------
        Int32           mFirst{-1}; // Key-code of the first button from the triggering combination.
        Int32           mSecond{-1}; // Key-code of the second button from the triggering combination.
        Int32           mThird{-1}; // Key-code of the third button from the triggering combination.
        Int32           mRelease{-1}; // Whether the key-bind reacts to button press or release.

        // ----------------------------------------------------------------------------------------
        LightObj        mEvents{}; // Table containing the emitted entity events.

        // ----------------------------------------------------------------------------------------
        SignalPair      mOnDestroyed{};
        SignalPair      mOnCustom{};

        // ----------------------------------------------------------------------------------------
        SignalPair      mOnKeyPress{};
        SignalPair      mOnKeyRelease{};
    };

    /* --------------------------------------------------------------------------------------------
     * Helper structure used to identify an object entity instance on the server.
    */
    struct ObjectInst
    {
        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        ObjectInst() = default;

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~ObjectInst()
        {
            if (VALID_ENTITY(mID))
            {
                Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullLightObj());
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Destroy the entity instance from the server, if necessary.
        */
        void Destroy(bool destroy, Int32 header, LightObj & payload);

        /* ----------------------------------------------------------------------------------------
         * Reset the instance to the default values.
        */
        void ResetInstance();

        /* ----------------------------------------------------------------------------------------
         * Create the associated signals.
        */
        void InitEvents();

        /* ----------------------------------------------------------------------------------------
         * Clear the associated signals.
        */
        void DropEvents();

        // ----------------------------------------------------------------------------------------
        Int32           mID{-1}; // The unique number that identifies this entity on the server.
        Uint32          mFlags{ENF_DEFAULT}; // Various options and states that can be toggled on the instance.
        CObject *       mInst{nullptr}; // Pointer to the actual instance used to interact this entity.
        LightObj        mObj{}; // Script object of the instance used to interact this entity.

        // ----------------------------------------------------------------------------------------
        LightObj        mEvents{}; // Table containing the emitted entity events.

        // ----------------------------------------------------------------------------------------
        SignalPair      mOnDestroyed{};
        SignalPair      mOnCustom{};
#if SQMOD_SDK_LEAST(2, 1)
        SignalPair      mOnStream{};
#endif
        // ----------------------------------------------------------------------------------------
        SignalPair      mOnShot{};
        SignalPair      mOnTouched{};
        SignalPair      mOnWorld{};
        SignalPair      mOnAlpha{};
        SignalPair      mOnReport{};
    };

    /* --------------------------------------------------------------------------------------------
     * Helper structure used to identify a pickup entity instance on the server.
    */
    struct PickupInst
    {
        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        PickupInst() = default;

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~PickupInst()
        {
            if (VALID_ENTITY(mID))
            {
                Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullLightObj());
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Destroy the entity instance from the server, if necessary.
        */
        void Destroy(bool destroy, Int32 header, LightObj & payload);

        /* ----------------------------------------------------------------------------------------
         * Reset the instance to the default values.
        */
        void ResetInstance();

        /* ----------------------------------------------------------------------------------------
         * Create the associated signals.
        */
        void InitEvents();

        /* ----------------------------------------------------------------------------------------
         * Clear the associated signals.
        */
        void DropEvents();

        // ----------------------------------------------------------------------------------------
        Int32           mID{-1}; // The unique number that identifies this entity on the server.
        Uint32          mFlags{ENF_DEFAULT}; // Various options and states that can be toggled on the instance.
        CPickup *       mInst{nullptr}; // Pointer to the actual instance used to interact this entity.
        LightObj        mObj{}; // Script object of the instance used to interact this entity.

        // ----------------------------------------------------------------------------------------
        LightObj        mEvents{}; // Table containing the emitted entity events.

        // ----------------------------------------------------------------------------------------
        SignalPair      mOnDestroyed{};
        SignalPair      mOnCustom{};
#if SQMOD_SDK_LEAST(2, 1)
        SignalPair      mOnStream{};
#endif
        // ----------------------------------------------------------------------------------------
        SignalPair      mOnRespawn{};
        SignalPair      mOnClaimed{};
        SignalPair      mOnCollected{};
        SignalPair      mOnWorld{};
        SignalPair      mOnAlpha{};
        SignalPair      mOnAutomatic{};
        SignalPair      mOnAutoTimer{};
        SignalPair      mOnOption{};
    };

    /* --------------------------------------------------------------------------------------------
     * Helper structure used to identify a player entity instance on the server.
    */
    struct PlayerInst
    {
        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        PlayerInst() = default;

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~PlayerInst()
        {
            if (VALID_ENTITY(mID))
            {
                Destroy(false, SQMOD_DESTROY_CLEANUP, NullLightObj());
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Destroy the entity instance from the server, if necessary.
        */
        void Destroy(bool destroy, Int32 header, LightObj & payload);

        /* ----------------------------------------------------------------------------------------
         * Reset the instance to the default values.
        */
        void ResetInstance();

        /* ----------------------------------------------------------------------------------------
         * Create the associated signals.
        */
        void InitEvents();

        /* ----------------------------------------------------------------------------------------
         * Clear the associated signals.
        */
        void DropEvents();

        // ----------------------------------------------------------------------------------------
        Int32           mID{-1}; // The unique number that identifies this entity on the server.
        Uint32          mFlags{ENF_DEFAULT}; // Various options and states that can be toggled on the instance.
        CPlayer *       mInst{nullptr}; // Pointer to the actual instance used to interact this entity.
        LightObj        mObj{}; // Script object of the instance used to interact this entity.

        // ----------------------------------------------------------------------------------------
        AreaList        mAreas{}; // Areas the player is currently in.
        Float64         mDistance{0}; // Distance traveled while tracking was enabled.

        // ----------------------------------------------------------------------------------------
        SQInteger       mTrackPosition{0}; // The number of times to track position changes.
        SQInteger       mTrackHeading{0}; // The number of times to track heading changes.

        // ----------------------------------------------------------------------------------------
        Int32           mTrackPositionHeader{0}; // Header to send when triggering position callback.
        LightObj        mTrackPositionPayload{}; // Payload to send when triggering position callback.

        // ----------------------------------------------------------------------------------------
        Int32           mKickBanHeader{0}; // Header to send when triggering kick/ban callback.
        LightObj        mKickBanPayload{}; // Payload to send when triggering kick/ban callback.

        // ----------------------------------------------------------------------------------------
        Int32           mLastWeapon{-1}; // Last known weapon of the player entity.
        Float32         mLastHealth{0}; // Last known health of the player entity.
        Float32         mLastArmour{0}; // Last known armor of the player entity.
        Float32         mLastHeading{0}; // Last known heading of the player entity.
        Vector3         mLastPosition{}; // Last known position of the player entity.

        // ----------------------------------------------------------------------------------------
        Int32           mAuthority{0}; // The authority level of the managed player.

        // ----------------------------------------------------------------------------------------
        LightObj        mEvents{}; // Table containing the emitted entity events.

        // ----------------------------------------------------------------------------------------
        SignalPair      mOnDestroyed{};
        SignalPair      mOnCustom{};
#if SQMOD_SDK_LEAST(2, 1)
        SignalPair      mOnStream{};
#endif
        // ----------------------------------------------------------------------------------------
        SignalPair      mOnRequestClass{};
        SignalPair      mOnRequestSpawn{};
        SignalPair      mOnSpawn{};
        SignalPair      mOnWasted{};
        SignalPair      mOnKilled{};
        SignalPair      mOnEmbarking{};
        SignalPair      mOnEmbarked{};
        SignalPair      mOnDisembark{};
        SignalPair      mOnRename{};
        SignalPair      mOnState{};
        SignalPair      mOnStateNone{};
        SignalPair      mOnStateNormal{};
        SignalPair      mOnStateAim{};
        SignalPair      mOnStateDriver{};
        SignalPair      mOnStatePassenger{};
        SignalPair      mOnStateEnterDriver{};
        SignalPair      mOnStateEnterPassenger{};
        SignalPair      mOnStateExit{};
        SignalPair      mOnStateUnspawned{};
        SignalPair      mOnAction{};
        SignalPair      mOnActionNone{};
        SignalPair      mOnActionNormal{};
        SignalPair      mOnActionAiming{};
        SignalPair      mOnActionShooting{};
        SignalPair      mOnActionJumping{};
        SignalPair      mOnActionLieDown{};
        SignalPair      mOnActionGettingUp{};
        SignalPair      mOnActionJumpVehicle{};
        SignalPair      mOnActionDriving{};
        SignalPair      mOnActionDying{};
        SignalPair      mOnActionWasted{};
        SignalPair      mOnActionEmbarking{};
        SignalPair      mOnActionDisembarking{};
        SignalPair      mOnBurning{};
        SignalPair      mOnCrouching{};
        SignalPair      mOnGameKeys{};
        SignalPair      mOnStartTyping{};
        SignalPair      mOnStopTyping{};
        SignalPair      mOnAway{};
        SignalPair      mOnMessage{};
        SignalPair      mOnCommand{};
        SignalPair      mOnPrivateMessage{};
        SignalPair      mOnKeyPress{};
        SignalPair      mOnKeyRelease{};
        SignalPair      mOnSpectate{};
        SignalPair      mOnUnspectate{};
        SignalPair      mOnCrashreport{};
        SignalPair      mOnModuleList{};
        SignalPair      mOnObjectShot{};
        SignalPair      mOnObjectTouched{};
        SignalPair      mOnPickupClaimed{};
        SignalPair      mOnPickupCollected{};
        SignalPair      mOnCheckpointEntered{};
        SignalPair      mOnCheckpointExited{};
        SignalPair      mOnClientScriptData{};
#if SQMOD_SDK_LEAST(2, 1)
        SignalPair      mOnEntityStream{};
#endif
        SignalPair      mOnUpdate{};
        SignalPair      mOnHealth{};
        SignalPair      mOnArmour{};
        SignalPair      mOnWeapon{};
        SignalPair      mOnHeading{};
        SignalPair      mOnPosition{};
        SignalPair      mOnOption{};
        SignalPair      mOnAdmin{};
        SignalPair      mOnWorld{};
        SignalPair      mOnTeam{};
        SignalPair      mOnSkin{};
        SignalPair      mOnMoney{};
        SignalPair      mOnScore{};
        SignalPair      mOnWantedLevel{};
        SignalPair      mOnImmunity{};
        SignalPair      mOnAlpha{};
        SignalPair      mOnEnterArea{};
        SignalPair      mOnLeaveArea{};
    };

    /* --------------------------------------------------------------------------------------------
     * Helper structure used to identify a vehicle entity instance on the server.
    */
    struct VehicleInst
    {
        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        VehicleInst() = default;

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~VehicleInst()
        {
            if (VALID_ENTITY(mID))
            {
                Destroy(!Core::Get().ShuttingDown(), SQMOD_DESTROY_CLEANUP, NullLightObj());
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Destroy the entity instance from the server, if necessary.
        */
        void Destroy(bool destroy, Int32 header, LightObj & payload);

        /* ----------------------------------------------------------------------------------------
         * Reset the instance to the default values.
        */
        void ResetInstance();

        /* ----------------------------------------------------------------------------------------
         * Create the associated signals.
        */
        void InitEvents();

        /* ----------------------------------------------------------------------------------------
         * Clear the associated signals.
        */
        void DropEvents();

        // ----------------------------------------------------------------------------------------
        Int32           mID{-1}; // The unique number that identifies this entity on the server.
        Uint32          mFlags{ENF_DEFAULT}; // Various options and states that can be toggled on the instance.
        CVehicle *      mInst{nullptr}; // Pointer to the actual instance used to interact this entity.
        LightObj        mObj{}; // Script object of the instance used to interact this entity.

        // ----------------------------------------------------------------------------------------
        AreaList        mAreas{}; // Areas the vehicle is currently in.
        Float64         mDistance{0}; // Distance traveled while tracking was enabled.

        // ----------------------------------------------------------------------------------------
        SQInteger       mTrackPosition{0}; // The number of times to track position changes.
        SQInteger       mTrackRotation{0}; // The number of times to track rotation changes.

        // ----------------------------------------------------------------------------------------
        Int32           mLastPrimaryColor{-1}; // Last known secondary-color of the player entity.
        Int32           mLastSecondaryColor{-1}; // Last known primary-color of the player entity.
        Float32         mLastHealth{0}; // Last known health of the player entity.
        Vector3         mLastPosition{}; // Last known position of the player entity.
        Quaternion      mLastRotation{}; // Last known rotation of the player entity.

        // ----------------------------------------------------------------------------------------
        LightObj        mEvents{}; // Table containing the emitted entity events.

        // ----------------------------------------------------------------------------------------
        SignalPair      mOnDestroyed{};
        SignalPair      mOnCustom{};
#if SQMOD_SDK_LEAST(2, 1)
        SignalPair      mOnStream{};
#endif
        // ----------------------------------------------------------------------------------------
        SignalPair      mOnEmbarking{};
        SignalPair      mOnEmbarked{};
        SignalPair      mOnDisembark{};
        SignalPair      mOnExplode{};
        SignalPair      mOnRespawn{};
        SignalPair      mOnUpdate{};
        SignalPair      mOnColor{};
        SignalPair      mOnHealth{};
        SignalPair      mOnPosition{};
        SignalPair      mOnRotation{};
        SignalPair      mOnOption{};
        SignalPair      mOnWorld{};
        SignalPair      mOnImmunity{};
        SignalPair      mOnPartStatus{};
        SignalPair      mOnTyreStatus{};
        SignalPair      mOnDamageData{};
        SignalPair      mOnRadio{};
        SignalPair      mOnHandlingRule{};
        SignalPair      mOnEnterArea{};
        SignalPair      mOnLeaveArea{};
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
    Scripts                         m_PendingScripts; // Pending scripts objects.
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
    LightObj                        m_Events; // Table containing the emitted module events.

    // --------------------------------------------------------------------------------------------
    Uint32                          m_CircularLocks; // Prevent events from triggering themselves.

    // --------------------------------------------------------------------------------------------
    Int32                           m_ReloadHeader; // The specified reload header.
    LightObj                        m_ReloadPayload; // The specified reload payload.

    // --------------------------------------------------------------------------------------------
    CStr                            m_IncomingNameBuffer; // Name of an incoming connection.
    size_t                          m_IncomingNameCapacity; // Incoming connection name size.

    // --------------------------------------------------------------------------------------------
    bool                            m_AreasEnabled; // Whether area tracking is enabled.
    bool                            m_Debugging; // Enable debugging features, if any.
    bool                            m_Executed; // Whether the scripts were executed.
    bool                            m_Shutdown; // Whether the server currently shutting down.
    bool                            m_LockPreLoadSignal; // Lock pre load signal container.
    bool                            m_LockPostLoadSignal; // Lock post load signal container.
    bool                            m_LockUnloadSignal; // Lock unload signal container.
    bool                            m_EmptyInit; // Whether to initialize without any scripts.
    // --------------------------------------------------------------------------------------------
    Int32                           m_Verbosity; // Restrict the amount of outputted information.

    // --------------------------------------------------------------------------------------------
    LightObj                        m_NullBlip; // Null Blips instance.
    LightObj                        m_NullCheckpoint; // Null Checkpoints instance.
    LightObj                        m_NullKeybind; // Null Key-instance pool.
    LightObj                        m_NullObject; // Null Objects instance.
    LightObj                        m_NullPickup; // Null Pickups instance.
    LightObj                        m_NullPlayer; // Null Players instance.
    LightObj                        m_NullVehicle; // Null Vehicles instance.

public:

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Core(const Core & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    Core(Core && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Core & operator = (const Core & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Core & operator = (Core && o) = delete;

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
     * Create the null instances of entity classes. Thus, locking them from further changes.
    */
    void EnableNullEntities();

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
     * See whether debugging option was enabled in the plug-in.
    */
    bool IsDebugging() const
    {
        return m_Debugging;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether all queued scripts were executed and the plug-in fully started.
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
     * See whether area tracking should be enabled on newlly created entities.
    */
    bool AreasEnabled() const
    {
        return m_AreasEnabled;
    }

    /* --------------------------------------------------------------------------------------------
     * Toggle whether area tracking should be enabled on newlly created entities.
    */
    void AreasEnabled(bool toggle)
    {
        m_AreasEnabled = toggle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value of the specified option.
    */
    const String & GetOption(const String & name) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value of the specified option or the fall back value if it doesn't exist.
    */
    const String & GetOption(const String & name, const String & value) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the value of the specified option.
    */
    void SetOption(const String & name, const String & value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the script source associated with a certain path in the scripts list.
    */
    Scripts::iterator FindScript(CSStr src);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the script source associated with a certain path in the pending scripts list.
    */
    Scripts::iterator FindPendingScript(CSStr src);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the scripts list. Should not be modified directly! Information purposes only.
    */
    Scripts & GetScripts()
    {
        return m_Scripts;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the pending scripts list. Should not be modified directly! Information purposes only.
    */
    Scripts & GetPendingScripts()
    {
        return m_PendingScripts;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the virtual machine.
    */
    HSQUIRRELVM GetVM() const
    {
        return m_VM;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the global events table.
    */
    LightObj & GetEvents()
    {
        return m_Events;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the circular locks.
    */
    Uint32 & GetCircularLock()
    {
        return m_CircularLocks;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the preload signal if not complete.
    */
    LightObj & GetPreLoadEvent()
    {
        return m_LockPreLoadSignal ? NullLightObj() : mOnPreLoad.second;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the postload signal if not complete.
    */
    LightObj & GetPostLoadEvent()
    {
        return m_LockPostLoadSignal ? NullLightObj() : mOnPostLoad.second;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unload signal if not complete.
    */
    LightObj & GetUnloadEvent()
    {
        return m_LockUnloadSignal ? NullLightObj() : mOnUnload.second;
    }

    /* --------------------------------------------------------------------------------------------
     * See if certain circular locks are enabled.
    */
    bool IsCircularLock(Uint32 lock) const
    {
        return static_cast< bool >(m_CircularLocks & lock);
    }

    /* --------------------------------------------------------------------------------------------
     * Set the header and payload for the reload.
    */
    void SetReloadInfo(Int32 header, LightObj & payload)
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
    LightObj & GetReloadPayload()
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
     * Retrieve the name for the currently assigned incoming connection.
    */
    CSStr GetIncomingName()
    {
        return (!m_IncomingNameBuffer) ? _SC("") : m_IncomingNameBuffer;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieves a line of code from a certain source.
    */
    String FetchCodeLine(CSStr src, SQInteger line, bool trim = true);

protected:

    /* --------------------------------------------------------------------------------------------
     * Script execution process.
    */
    static bool DoScripts(Scripts::iterator itr, Scripts::iterator end);

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
    bool CompilerErrorHandlerEx(CSStr desc, CSStr src, SQInteger line, SQInteger column);

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
    BlipInst & AllocBlip(Int32 id, bool owned, Int32 header, LightObj & payload);
    CheckpointInst & AllocCheckpoint(Int32 id, bool owned, Int32 header, LightObj & payload);
    KeybindInst & AllocKeybind(Int32 id, bool owned, Int32 header, LightObj & payload);
    ObjectInst & AllocObject(Int32 id, bool owned, Int32 header, LightObj & payload);
    PickupInst & AllocPickup(Int32 id, bool owned, Int32 header, LightObj & payload);
    VehicleInst & AllocVehicle(Int32 id, bool owned, Int32 header, LightObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Entity deallocator.
    */
    void DeallocBlip(Int32 id, bool destroy, Int32 header, LightObj & payload);
    void DeallocCheckpoint(Int32 id, bool destroy, Int32 header, LightObj & payload);
    void DeallocKeybind(Int32 id, bool destroy, Int32 header, LightObj & payload);
    void DeallocObject(Int32 id, bool destroy, Int32 header, LightObj & payload);
    void DeallocPickup(Int32 id, bool destroy, Int32 header, LightObj & payload);
    void DeallocVehicle(Int32 id, bool destroy, Int32 header, LightObj & payload);

public:

    /* --------------------------------------------------------------------------------------------
     * Entity creators.
    */
    LightObj & NewBlip(Int32 index, Int32 world, Float32 x, Float32 y, Float32 z,
                        Int32 scale, Uint32 color, Int32 sprid,
                        Int32 header, LightObj & payload);

    LightObj & NewCheckpoint(Int32 player, Int32 world, bool sphere, Float32 x, Float32 y, Float32 z,
                        Uint8 r, Uint8 g, Uint8 b, Uint8 a, Float32 radius,
                        Int32 header, LightObj & payload);

    LightObj & NewKeybind(Int32 slot, bool release,
                        Int32 primary, Int32 secondary, Int32 alternative,
                        Int32 header, LightObj & payload);

    LightObj & NewObject(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z,
                        Int32 alpha, Int32 header, LightObj & payload);

    LightObj & NewPickup(Int32 model, Int32 world, Int32 quantity,
                        Float32 x, Float32 y, Float32 z, Int32 alpha, bool automatic,
                        Int32 header, LightObj & payload);

    LightObj & NewVehicle(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z,
                        Float32 angle, Int32 primary, Int32 secondary,
                        Int32 header, LightObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Entity destroyers.
    */
    bool DelBlip(Int32 id, Int32 header, LightObj & payload);
    bool DelCheckpoint(Int32 id, Int32 header, LightObj & payload);
    bool DelKeybind(Int32 id, Int32 header, LightObj & payload);
    bool DelObject(Int32 id, Int32 header, LightObj & payload);
    bool DelPickup(Int32 id, Int32 header, LightObj & payload);
    bool DelVehicle(Int32 id, Int32 header, LightObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Entity retrievers.
    */
    BlipInst & GetBlip(Int32 id) { return m_Blips.at(static_cast< size_t >(id)); }
    CheckpointInst & GetCheckpoint(Int32 id) { return m_Checkpoints.at(static_cast< size_t >(id)); }
    KeybindInst & GetKeybind(Int32 id) { return m_Keybinds.at(static_cast< size_t >(id)); }
    ObjectInst & GetObj(Int32 id) { return m_Objects.at(static_cast< size_t >(id)); }
    PickupInst & GetPickup(Int32 id) { return m_Pickups.at(static_cast< size_t >(id)); }
    PlayerInst & GetPlayer(Int32 id) { return m_Players.at(static_cast< size_t >(id)); }
    VehicleInst & GetVehicle(Int32 id) { return m_Vehicles.at(static_cast< size_t >(id)); }

    /* --------------------------------------------------------------------------------------------
     * Pool retrievers.
    */
    const Blips & GetBlips() const { return m_Blips; }
    const Checkpoints & GetCheckpoints() const { return m_Checkpoints; }
    const Keybinds & GetKeybinds() const { return m_Keybinds; }
    const Objects & GetObjs() const { return m_Objects; }
    const Pickups & GetPickups() const { return m_Pickups; }
    const Players & GetPlayers() const { return m_Players; }
    const Vehicles & GetVehicles() const { return m_Vehicles; }

    /* --------------------------------------------------------------------------------------------
     * Null instance retrievers.
    */
    LightObj & GetNullBlip() { return m_NullBlip; }
    LightObj & GetNullCheckpoint() { return m_NullCheckpoint; }
    LightObj & GetNullKeybind() { return m_NullKeybind; }
    LightObj & GetNullObject() { return m_NullObject; }
    LightObj & GetNullPickup() { return m_NullPickup; }
    LightObj & GetNullPlayer() { return m_NullPlayer; }
    LightObj & GetNullVehicle() { return m_NullVehicle; }

    /* --------------------------------------------------------------------------------------------
     * Container cleaner.
    */
    void ClearContainer(EntityType type);

protected:

    /* --------------------------------------------------------------------------------------------
     * Signal initialization and termination.
    */
    void InitEvents();
    void DropEvents();

public:

    /* --------------------------------------------------------------------------------------------
     * Player lifetime management.
    */
    void ConnectPlayer(Int32 id, Int32 header, LightObj & payload);
    void DisconnectPlayer(Int32 id, Int32 header, LightObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Emit a custom event.
    */
    void EmitCustomEvent(Int32 group, Int32 header, LightObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Server events.
    */
    void EmitBlipCreated(Int32 blip, Int32 header, LightObj & payload);
    void EmitCheckpointCreated(Int32 forcefield, Int32 header, LightObj & payload);
    void EmitKeybindCreated(Int32 keybind, Int32 header, LightObj & payload);
    void EmitObjectCreated(Int32 object, Int32 header, LightObj & payload);
    void EmitPickupCreated(Int32 pickup, Int32 header, LightObj & payload);
    void EmitPlayerCreated(Int32 player, Int32 header, LightObj & payload);
    void EmitVehicleCreated(Int32 vehicle, Int32 header, LightObj & payload);
    void EmitBlipDestroyed(Int32 blip, Int32 header, LightObj & payload);
    void EmitCheckpointDestroyed(Int32 forcefield, Int32 header, LightObj & payload);
    void EmitKeybindDestroyed(Int32 keybind, Int32 header, LightObj & payload);
    void EmitObjectDestroyed(Int32 object, Int32 header, LightObj & payload);
    void EmitPickupDestroyed(Int32 pickup, Int32 header, LightObj & payload);
    void EmitPlayerDestroyed(Int32 player, Int32 header, LightObj & payload);
    void EmitVehicleDestroyed(Int32 vehicle, Int32 header, LightObj & payload);
    void EmitBlipCustom(Int32 blip, Int32 header, LightObj & payload);
    void EmitCheckpointCustom(Int32 forcefield, Int32 header, LightObj & payload);
    void EmitKeybindCustom(Int32 keybind, Int32 header, LightObj & payload);
    void EmitObjectCustom(Int32 object, Int32 header, LightObj & payload);
    void EmitPickupCustom(Int32 pickup, Int32 header, LightObj & payload);
    void EmitPlayerCustom(Int32 player, Int32 header, LightObj & payload);
    void EmitVehicleCustom(Int32 vehicle, Int32 header, LightObj & payload);
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
    void EmitPlayerUnspectate(Int32 player_id);
    void EmitPlayerCrashreport(Int32 player_id, CCStr report);
    void EmitPlayerModuleList(Int32 player_id, CCStr list);
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
    void EmitPickupOption(Int32 pickup_id, Int32 option_id, bool value, Int32 header, LightObj & payload);
    void EmitObjectReport(Int32 object_id, bool old_status, bool new_status, bool touched);
    void EmitPlayerHealth(Int32 player_id, Float32 old_health, Float32 new_health);
    void EmitPlayerArmour(Int32 player_id, Float32 old_armour, Float32 new_armour);
    void EmitPlayerWeapon(Int32 player_id, Int32 old_weapon, Int32 new_weapon);
    void EmitPlayerHeading(Int32 player_id, Float32 old_heading, Float32 new_heading);
    void EmitPlayerPosition(Int32 player_id);
    void EmitPlayerOption(Int32 player_id, Int32 option_id, bool value, Int32 header, LightObj & payload);
    void EmitPlayerAdmin(Int32 player_id, bool old_status, bool new_status);
    void EmitPlayerWorld(Int32 player_id, Int32 old_world, Int32 new_world, bool secondary);
    void EmitPlayerTeam(Int32 player_id, Int32 old_team, Int32 new_team);
    void EmitPlayerSkin(Int32 player_id, Int32 old_skin, Int32 new_skin);
    void EmitPlayerMoney(Int32 player_id, Int32 old_money, Int32 new_money);
    void EmitPlayerScore(Int32 player_id, Int32 old_score, Int32 new_score);
    void EmitPlayerWantedLevel(Int32 player_id, Int32 old_level, Int32 new_level);
    void EmitPlayerImmunity(Int32 player_id, Int32 old_immunity, Int32 new_immunity);
    void EmitPlayerAlpha(Int32 player_id, Int32 old_alpha, Int32 new_alpha, Int32 fade);
    void EmitPlayerEnterArea(Int32 player_id, LightObj & area_obj);
    void EmitPlayerLeaveArea(Int32 player_id, LightObj & area_obj);
    void EmitVehicleColor(Int32 vehicle_id, Int32 changed);
    void EmitVehicleHealth(Int32 vehicle_id, Float32 old_health, Float32 new_health);
    void EmitVehiclePosition(Int32 vehicle_id);
    void EmitVehicleRotation(Int32 vehicle_id);
    void EmitVehicleOption(Int32 vehicle_id, Int32 option_id, bool value, Int32 header, LightObj & payload);
    void EmitVehicleWorld(Int32 vehicle_id, Int32 old_world, Int32 new_world);
    void EmitVehicleImmunity(Int32 vehicle_id, Int32 old_immunity, Int32 new_immunity);
    void EmitVehiclePartStatus(Int32 vehicle_id, Int32 part, Int32 old_status, Int32 new_status);
    void EmitVehicleTyreStatus(Int32 vehicle_id, Int32 tyre, Int32 old_status, Int32 new_status);
    void EmitVehicleDamageData(Int32 vehicle_id, Uint32 old_data, Uint32 new_data);
    void EmitVehicleRadio(Int32 vehicle_id, Int32 old_radio, Int32 new_radio);
    void EmitVehicleHandlingRule(Int32 vehicle_id, Int32 rule, SQFloat old_data, SQFloat new_data);
    void EmitVehicleEnterArea(Int32 player_id, LightObj & area_obj);
    void EmitVehicleLeaveArea(Int32 player_id, LightObj & area_obj);
    void EmitServerOption(Int32 option, bool value, Int32 header, LightObj & payload);
    void EmitScriptReload(Int32 header, LightObj & payload);
    void EmitScriptLoaded();

    /* --------------------------------------------------------------------------------------------
     * Entity pool changes events.
    */
    void EmitEntityPool(vcmpEntityPool entity_type, Int32 entity_id, bool is_deleted);
#if SQMOD_SDK_LEAST(2, 1)
    /* --------------------------------------------------------------------------------------------
     * Entity streaming changes events.
    */
    void EmitCheckpointStream(int32_t player_id, int32_t entity_id, bool is_deleted);
    void EmitObjectStream(int32_t player_id, int32_t entity_id, bool is_deleted);
    void EmitPickupStream(int32_t player_id, int32_t entity_id, bool is_deleted);
    void EmitPlayerStream(int32_t player_id, int32_t entity_id, bool is_deleted);
    void EmitVehicleStream(int32_t player_id, int32_t entity_id, bool is_deleted);
    void EmitEntityStreaming(int32_t player_id, int32_t entity_id, vcmpEntityPool entity_type, bool is_deleted);
#endif
    /* --------------------------------------------------------------------------------------------
     * Entity update events.
    */
    void EmitPlayerUpdate(Int32 player_id, vcmpPlayerUpdate update_type);
    void EmitVehicleUpdate(Int32 vehicle_id, vcmpVehicleUpdate update_type);

    /* --------------------------------------------------------------------------------------------
     * Client data streams event.
    */
    void EmitClientScriptData(Int32 player_id, const uint8_t * data, size_t size);

public:

    /* --------------------------------------------------------------------------------------------
     * Module signals.
    */
    SignalPair  mOnPreLoad;
    SignalPair  mOnPostLoad;
    SignalPair  mOnUnload;

    /* --------------------------------------------------------------------------------------------
     * Server signals.
    */
    SignalPair  mOnCustomEvent;
    SignalPair  mOnBlipCreated;
    SignalPair  mOnCheckpointCreated;
    SignalPair  mOnKeybindCreated;
    SignalPair  mOnObjectCreated;
    SignalPair  mOnPickupCreated;
    SignalPair  mOnPlayerCreated;
    SignalPair  mOnVehicleCreated;
    SignalPair  mOnBlipDestroyed;
    SignalPair  mOnCheckpointDestroyed;
    SignalPair  mOnKeybindDestroyed;
    SignalPair  mOnObjectDestroyed;
    SignalPair  mOnPickupDestroyed;
    SignalPair  mOnPlayerDestroyed;
    SignalPair  mOnVehicleDestroyed;
    SignalPair  mOnBlipCustom;
    SignalPair  mOnCheckpointCustom;
    SignalPair  mOnKeybindCustom;
    SignalPair  mOnObjectCustom;
    SignalPair  mOnPickupCustom;
    SignalPair  mOnPlayerCustom;
    SignalPair  mOnVehicleCustom;
#if SQMOD_SDK_LEAST(2, 1)
    SignalPair  mOnCheckpointStream;
    SignalPair  mOnObjectStream;
    SignalPair  mOnPickupStream;
    SignalPair  mOnPlayerStream;
    SignalPair  mOnVehicleStream;
#endif
    SignalPair  mOnServerStartup;
    SignalPair  mOnServerShutdown;
    SignalPair  mOnServerFrame;
    SignalPair  mOnIncomingConnection;
    SignalPair  mOnPlayerRequestClass;
    SignalPair  mOnPlayerRequestSpawn;
    SignalPair  mOnPlayerSpawn;
    SignalPair  mOnPlayerWasted;
    SignalPair  mOnPlayerKilled;
    SignalPair  mOnPlayerEmbarking;
    SignalPair  mOnPlayerEmbarked;
    SignalPair  mOnPlayerDisembark;
    SignalPair  mOnPlayerRename;
    SignalPair  mOnPlayerState;
    SignalPair  mOnStateNone;
    SignalPair  mOnStateNormal;
    SignalPair  mOnStateAim;
    SignalPair  mOnStateDriver;
    SignalPair  mOnStatePassenger;
    SignalPair  mOnStateEnterDriver;
    SignalPair  mOnStateEnterPassenger;
    SignalPair  mOnStateExit;
    SignalPair  mOnStateUnspawned;
    SignalPair  mOnPlayerAction;
    SignalPair  mOnActionNone;
    SignalPair  mOnActionNormal;
    SignalPair  mOnActionAiming;
    SignalPair  mOnActionShooting;
    SignalPair  mOnActionJumping;
    SignalPair  mOnActionLieDown;
    SignalPair  mOnActionGettingUp;
    SignalPair  mOnActionJumpVehicle;
    SignalPair  mOnActionDriving;
    SignalPair  mOnActionDying;
    SignalPair  mOnActionWasted;
    SignalPair  mOnActionEmbarking;
    SignalPair  mOnActionDisembarking;
    SignalPair  mOnPlayerBurning;
    SignalPair  mOnPlayerCrouching;
    SignalPair  mOnPlayerGameKeys;
    SignalPair  mOnPlayerStartTyping;
    SignalPair  mOnPlayerStopTyping;
    SignalPair  mOnPlayerAway;
    SignalPair  mOnPlayerMessage;
    SignalPair  mOnPlayerCommand;
    SignalPair  mOnPlayerPrivateMessage;
    SignalPair  mOnPlayerKeyPress;
    SignalPair  mOnPlayerKeyRelease;
    SignalPair  mOnPlayerSpectate;
    SignalPair  mOnPlayerUnspectate;
    SignalPair  mOnPlayerCrashreport;
    SignalPair  mOnPlayerModuleList;
    SignalPair  mOnVehicleExplode;
    SignalPair  mOnVehicleRespawn;
    SignalPair  mOnObjectShot;
    SignalPair  mOnObjectTouched;
    SignalPair  mOnObjectWorld;
    SignalPair  mOnObjectAlpha;
    SignalPair  mOnObjectReport;
    SignalPair  mOnPickupClaimed;
    SignalPair  mOnPickupCollected;
    SignalPair  mOnPickupRespawn;
    SignalPair  mOnPickupWorld;
    SignalPair  mOnPickupAlpha;
    SignalPair  mOnPickupAutomatic;
    SignalPair  mOnPickupAutoTimer;
    SignalPair  mOnPickupOption;
    SignalPair  mOnCheckpointEntered;
    SignalPair  mOnCheckpointExited;
    SignalPair  mOnCheckpointWorld;
    SignalPair  mOnCheckpointRadius;
    SignalPair  mOnEntityPool;
    SignalPair  mOnClientScriptData;
    SignalPair  mOnPlayerUpdate;
    SignalPair  mOnVehicleUpdate;
    SignalPair  mOnPlayerHealth;
    SignalPair  mOnPlayerArmour;
    SignalPair  mOnPlayerWeapon;
    SignalPair  mOnPlayerHeading;
    SignalPair  mOnPlayerPosition;
    SignalPair  mOnPlayerOption;
    SignalPair  mOnPlayerAdmin;
    SignalPair  mOnPlayerWorld;
    SignalPair  mOnPlayerTeam;
    SignalPair  mOnPlayerSkin;
    SignalPair  mOnPlayerMoney;
    SignalPair  mOnPlayerScore;
    SignalPair  mOnPlayerWantedLevel;
    SignalPair  mOnPlayerImmunity;
    SignalPair  mOnPlayerAlpha;
    SignalPair  mOnPlayerEnterArea;
    SignalPair  mOnPlayerLeaveArea;
    SignalPair  mOnVehicleColor;
    SignalPair  mOnVehicleHealth;
    SignalPair  mOnVehiclePosition;
    SignalPair  mOnVehicleRotation;
    SignalPair  mOnVehicleOption;
    SignalPair  mOnVehicleWorld;
    SignalPair  mOnVehicleImmunity;
    SignalPair  mOnVehiclePartStatus;
    SignalPair  mOnVehicleTyreStatus;
    SignalPair  mOnVehicleDamageData;
    SignalPair  mOnVehicleRadio;
    SignalPair  mOnVehicleHandlingRule;
    SignalPair  mOnVehicleEnterArea;
    SignalPair  mOnVehicleLeaveArea;
#if SQMOD_SDK_LEAST(2, 1)
    SignalPair  mOnEntityStream;
#endif
    SignalPair  mOnServerOption;
    SignalPair  mOnScriptReload;
    SignalPair  mOnScriptLoaded;
};

/* ------------------------------------------------------------------------------------------------
 * Structure used to preserve the core state across recursive event calls from the server.
*/
struct CoreState
{
    /* --------------------------------------------------------------------------------------------
     * Backup the current core state.
    */
    CoreState()
        : m_State(Core::Get().GetState())
    {
        //...
    }

    /* --------------------------------------------------------------------------------------------
     * Backup the current core state and set the given state.
    */
    explicit CoreState(int s)
        : m_State(Core::Get().GetState())
    {
        Core::Get().SetState(s);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    CoreState(const CoreState & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    CoreState(CoreState && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor. Restore the grabbed core state.
    */
    ~CoreState()
    {
        Core::Get().SetState(m_State);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    CoreState & operator = (const CoreState & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    CoreState & operator = (CoreState && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the guarded state.
    */
    int GetValue() const
    {
        return m_State;
    }

protected:

    int m_State; // The core state at the time when this instance was created.
};

} // Namespace:: SqMod
