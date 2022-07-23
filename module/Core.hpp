#pragma once


// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"
#include "Core/Entity.hpp"
#include "Core/Script.hpp"

// ------------------------------------------------------------------------------------------------
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

public:

    // --------------------------------------------------------------------------------------------
    typedef std::vector< BlipInst >         Blips; // Blips entity instances container.
    typedef std::vector< CheckpointInst >   Checkpoints; // Checkpoints entity instances container.
    typedef std::vector< KeyBindInst >      KeyBinds; // Key-binds entity instances container.
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
    int32_t                         m_State; // Current plug-in state.
#ifdef VCMP_ENABLE_OFFICIAL
    bool                            m_Official; // Whether official support is enabled.
#endif
    HSQUIRRELVM                     m_VM; // Script virtual machine.
    Scripts                         m_Scripts; // Loaded scripts objects.
    Scripts                         m_PendingScripts; // Pending scripts objects.
    Options                         m_Options; // Custom configuration options.

    // --------------------------------------------------------------------------------------------
    Blips                           m_Blips; // Blips pool.
    Checkpoints                     m_Checkpoints; // Checkpoints pool.
    KeyBinds                        m_KeyBinds; // Key-binds pool.
    Objects                         m_Objects; // Objects pool.
    Pickups                         m_Pickups; // Pickups pool.
    Players                         m_Players; // Players pool.
    Vehicles                        m_Vehicles; // Vehicles pool.

    // --------------------------------------------------------------------------------------------
    LightObj                        m_Events; // Table containing the emitted module events.

    // --------------------------------------------------------------------------------------------
    uint32_t                        m_CircularLocks; // Prevent events from triggering themselves.

    // --------------------------------------------------------------------------------------------
    int32_t                         m_ReloadHeader; // The specified reload header.
    LightObj                        m_ReloadPayload; // The specified reload payload.

    // --------------------------------------------------------------------------------------------
    char *                          m_IncomingNameBuffer; // Name of an incoming connection.
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
    int32_t                         m_Verbosity; // Restrict the amount of outputted information.

    // --------------------------------------------------------------------------------------------
    LightObj                        m_ClientData; // Currently processed client data buffer.

    // --------------------------------------------------------------------------------------------
    LightObj                        m_NullBlip; // Null Blips instance.
    LightObj                        m_NullCheckpoint; // Null Checkpoints instance.
    LightObj                        m_NullKeyBind; // Null Key-instance pool.
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
    void SetState(int32_t val)
    {
        m_State = val;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current plug-in state.
    */
    SQMOD_NODISCARD int32_t GetState() const
    {
        return m_State;
    }

#ifdef VCMP_ENABLE_OFFICIAL
    /* --------------------------------------------------------------------------------------------
     * See whether official support option was enabled in the plug-in.
    */
    SQMOD_NODISCARD bool IsOfficial() const
    {
        return m_Official;
    }
#endif

    /* --------------------------------------------------------------------------------------------
     * See whether debugging option was enabled in the plug-in.
    */
    SQMOD_NODISCARD bool IsDebugging() const
    {
        return m_Debugging;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether all queued scripts were executed and the plug-in fully started.
    */
    SQMOD_NODISCARD bool IsExecuted() const
    {
        return m_Executed;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the server is currently in the process of shutting down.
    */
    SQMOD_NODISCARD bool ShuttingDown() const
    {
        return m_Shutdown;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether area tracking should be enabled on newly created entities.
    */
    SQMOD_NODISCARD bool AreasEnabled() const
    {
        return m_AreasEnabled;
    }

    /* --------------------------------------------------------------------------------------------
     * Toggle whether area tracking should be enabled on newly created entities.
    */
    void AreasEnabled(bool toggle)
    {
        m_AreasEnabled = toggle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value of the specified option.
    */
    SQMOD_NODISCARD const String & GetOption(const String & name) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value of the specified option or the fall back value if it doesn't exist.
    */
    SQMOD_NODISCARD const String & GetOption(const String & name, const String & value) const;

    /* --------------------------------------------------------------------------------------------
     * Modify the value of the specified option.
    */
    void SetOption(const String & name, const String & value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the script source associated with a certain path in the scripts list.
    */
    SQMOD_NODISCARD Scripts::iterator FindScript(const SQChar * src);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the script source associated with a certain path in the pending scripts list.
    */
    SQMOD_NODISCARD Scripts::iterator FindPendingScript(const SQChar * src);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the scripts list. Should not be modified directly! Information purposes only.
    */
    SQMOD_NODISCARD Scripts & GetScripts()
    {
        return m_Scripts;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the pending scripts list. Should not be modified directly! Information purposes only.
    */
    SQMOD_NODISCARD Scripts & GetPendingScripts()
    {
        return m_PendingScripts;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the virtual machine.
    */
    SQMOD_NODISCARD HSQUIRRELVM GetVM() const
    {
        return m_VM;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the global events table.
    */
    SQMOD_NODISCARD LightObj & GetEvents()
    {
        return m_Events;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the circular locks.
    */
    SQMOD_NODISCARD uint32_t & GetCircularLock()
    {
        return m_CircularLocks;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the preload signal if not complete.
    */
   SQMOD_NODISCARD  LightObj & GetPreLoadEvent()
    {
        return m_LockPreLoadSignal ? NullLightObj() : mOnPreLoad.second;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the post-load signal if not complete.
    */
    SQMOD_NODISCARD LightObj & GetPostLoadEvent()
    {
        return m_LockPostLoadSignal ? NullLightObj() : mOnPostLoad.second;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unload signal if not complete.
    */
    SQMOD_NODISCARD LightObj & GetUnloadEvent()
    {
        return m_LockUnloadSignal ? NullLightObj() : mOnUnload.second;
    }

    /* --------------------------------------------------------------------------------------------
     * See if certain circular locks are enabled.
    */
    SQMOD_NODISCARD bool IsCircularLock(uint32_t lock) const
    {
        return static_cast< bool >(m_CircularLocks & lock);
    }

    /* --------------------------------------------------------------------------------------------
     * Set the header and payload for the reload.
    */
    void SetReloadInfo(int32_t header, LightObj & payload)
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
    SQMOD_NODISCARD int32_t GetReloadHeader() const
    {
        return m_ReloadHeader;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the specified reload header.
    */
    SQMOD_NODISCARD LightObj & GetReloadPayload()
    {
        return m_ReloadPayload;
    }

    /* --------------------------------------------------------------------------------------------
     * Adds a script to the load queue.
    */
    bool LoadScript(const SQChar * filepath, Function & cb, LightObj & ctx, bool delay = false);

    /* --------------------------------------------------------------------------------------------
     * Modify the name for the currently assigned incoming connection.
    */
    void SetIncomingName(const SQChar * name);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name for the currently assigned incoming connection.
    */
    SQMOD_NODISCARD const SQChar * GetIncomingName()
    {
        return (!m_IncomingNameBuffer) ? _SC("") : m_IncomingNameBuffer;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current client data buffer, if any.
    */
    SQMOD_NODISCARD LightObj & GetClientDataBuffer()
    {
        return m_ClientData;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieves a line of code from a certain source.
    */
    SQMOD_NODISCARD String FetchCodeLine(const SQChar * src, SQInteger line, bool trim = true);

protected:

    /* --------------------------------------------------------------------------------------------
     * Script execution process.
    */
    static bool DoScripts(Scripts::iterator itr, Scripts::iterator end);

    /* --------------------------------------------------------------------------------------------
     * Script output handlers.
    */
    static void PrintFunc(HSQUIRRELVM vm, const SQChar * msg, ...);
    static void ErrorFunc(HSQUIRRELVM vm, const SQChar * msg, ...);

    /* --------------------------------------------------------------------------------------------
     * Script error handlers.
    */
    static SQInteger RuntimeErrorHandler(HSQUIRRELVM vm);
    static void CompilerErrorHandler(HSQUIRRELVM vm, const SQChar * desc, const SQChar * src,
                                        SQInteger line, SQInteger column);
    bool CompilerErrorHandlerEx(const SQChar * desc, const SQChar * src, SQInteger line, SQInteger column);

    /* --------------------------------------------------------------------------------------------
     * Entity scanners.
    */
    void ImportBlips();
    void ImportCheckpoints();
    void ImportKeyBinds();
    void ImportObjects();
    void ImportPickups();
    void ImportPlayers();
    void ImportVehicles();

    /* --------------------------------------------------------------------------------------------
     * Entity allocators.
    */
    BlipInst & AllocBlip(int32_t id, bool owned, int32_t header, LightObj & payload);
    CheckpointInst & AllocCheckpoint(int32_t id, bool owned, int32_t header, LightObj & payload);
    KeyBindInst & AllocKeyBind(int32_t id, bool owned, int32_t header, LightObj & payload);
    ObjectInst & AllocObject(int32_t id, bool owned, int32_t header, LightObj & payload);
    PickupInst & AllocPickup(int32_t id, bool owned, int32_t header, LightObj & payload);
    VehicleInst & AllocVehicle(int32_t id, bool owned, int32_t header, LightObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Entity de-allocator.
    */
    void DeallocBlip(int32_t id, bool destroy, int32_t header, LightObj & payload);
    void DeallocCheckpoint(int32_t id, bool destroy, int32_t header, LightObj & payload);
    void DeallocKeyBind(int32_t id, bool destroy, int32_t header, LightObj & payload);
    void DeallocObject(int32_t id, bool destroy, int32_t header, LightObj & payload);
    void DeallocPickup(int32_t id, bool destroy, int32_t header, LightObj & payload);
    void DeallocVehicle(int32_t id, bool destroy, int32_t header, LightObj & payload);

public:

    /* --------------------------------------------------------------------------------------------
     * Entity creators.
    */
    BlipInst & NewBlip(int32_t index, int32_t world, float x, float y, float z,
                        int32_t scale, uint32_t color, int32_t spr_id,
                        int32_t header, LightObj & payload);

    CheckpointInst & NewCheckpoint(int32_t player, int32_t world, bool sphere, float x, float y, float z,
                        uint8_t r, uint8_t g, uint8_t b, uint8_t a, float radius,
                        int32_t header, LightObj & payload);

    KeyBindInst & NewKeyBind(int32_t slot, bool release,
                          int32_t primary, int32_t secondary, int32_t alternative,
                          int32_t header, LightObj & payload);

    ObjectInst & NewObject(int32_t model, int32_t world, float x, float y, float z,
                        int32_t alpha, int32_t header, LightObj & payload);

    PickupInst & NewPickup(int32_t model, int32_t world, int32_t quantity,
                        float x, float y, float z, int32_t alpha, bool automatic,
                        int32_t header, LightObj & payload);

    VehicleInst & NewVehicle(int32_t model, int32_t world, float x, float y, float z,
                        float angle, int32_t primary, int32_t secondary,
                        int32_t header, LightObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Entity destroyers.
    */
    bool DelBlip(int32_t id, int32_t header, LightObj & payload);
    bool DelCheckpoint(int32_t id, int32_t header, LightObj & payload);
    bool DelKeyBind(int32_t id, int32_t header, LightObj & payload);
    bool DelObject(int32_t id, int32_t header, LightObj & payload);
    bool DelPickup(int32_t id, int32_t header, LightObj & payload);
    bool DelVehicle(int32_t id, int32_t header, LightObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Entity retrievers.
    */
    SQMOD_NODISCARD BlipInst & GetBlip(int32_t id) { return m_Blips.at(static_cast< size_t >(id)); }
    SQMOD_NODISCARD CheckpointInst & GetCheckpoint(int32_t id) { return m_Checkpoints.at(static_cast< size_t >(id)); }
    SQMOD_NODISCARD KeyBindInst & GetKeyBind(int32_t id) { return m_KeyBinds.at(static_cast< size_t >(id)); }
    // Windows API is retarded. That's why this isn't named `GetObject` like the others
    SQMOD_NODISCARD ObjectInst & GetObj(int32_t id) { return m_Objects.at(static_cast< size_t >(id)); }
    SQMOD_NODISCARD PickupInst & GetPickup(int32_t id) { return m_Pickups.at(static_cast< size_t >(id)); }
    SQMOD_NODISCARD PlayerInst & GetPlayer(int32_t id) { return m_Players.at(static_cast< size_t >(id)); }
    SQMOD_NODISCARD VehicleInst & GetVehicle(int32_t id) { return m_Vehicles.at(static_cast< size_t >(id)); }

    /* --------------------------------------------------------------------------------------------
     * Pool retrievers.
    */
    SQMOD_NODISCARD const Blips & GetBlips() const { return m_Blips; }
    SQMOD_NODISCARD const Checkpoints & GetCheckpoints() const { return m_Checkpoints; }
    SQMOD_NODISCARD const KeyBinds & GetKeyBinds() const { return m_KeyBinds; }
    SQMOD_NODISCARD const Objects & GetObjs() const { return m_Objects; }
    SQMOD_NODISCARD const Pickups & GetPickups() const { return m_Pickups; }
    SQMOD_NODISCARD const Players & GetPlayers() const { return m_Players; }
    SQMOD_NODISCARD const Vehicles & GetVehicles() const { return m_Vehicles; }

    /* --------------------------------------------------------------------------------------------
     * Null instance retrievers.
    */
    SQMOD_NODISCARD LightObj & GetNullBlip() { return m_NullBlip; }
    SQMOD_NODISCARD LightObj & GetNullCheckpoint() { return m_NullCheckpoint; }
    SQMOD_NODISCARD LightObj & GetNullKeyBind() { return m_NullKeyBind; }
    SQMOD_NODISCARD LightObj & GetNullObject() { return m_NullObject; }
    SQMOD_NODISCARD LightObj & GetNullPickup() { return m_NullPickup; }
    SQMOD_NODISCARD LightObj & GetNullPlayer() { return m_NullPlayer; }
    SQMOD_NODISCARD LightObj & GetNullVehicle() { return m_NullVehicle; }

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
    void ConnectPlayer(int32_t id, int32_t header, LightObj & payload);
    void DisconnectPlayer(int32_t id, int32_t header, LightObj & payload);

    /* --------------------------------------------------------------------------------------------
     * Emit a custom event.
    */
    void EmitCustomEvent(int32_t group, int32_t header, LightObj & payload) const;

    /* --------------------------------------------------------------------------------------------
     * Server events.
    */
    void EmitBlipCreated(int32_t blip, int32_t header, LightObj & payload);
    void EmitCheckpointCreated(int32_t checkpoint, int32_t header, LightObj & payload);
    void EmitKeyBindCreated(int32_t key_bind, int32_t header, LightObj & payload);
    void EmitObjectCreated(int32_t object, int32_t header, LightObj & payload);
    void EmitPickupCreated(int32_t pickup, int32_t header, LightObj & payload);
    void EmitPlayerCreated(int32_t player, int32_t header, LightObj & payload);
    void EmitVehicleCreated(int32_t vehicle, int32_t header, LightObj & payload);
    void EmitBlipDestroyed(int32_t blip, int32_t header, LightObj & payload);
    void EmitCheckpointDestroyed(int32_t checkpoint, int32_t header, LightObj & payload);
    void EmitKeyBindDestroyed(int32_t key_bind, int32_t header, LightObj & payload);
    void EmitObjectDestroyed(int32_t object, int32_t header, LightObj & payload);
    void EmitPickupDestroyed(int32_t pickup, int32_t header, LightObj & payload);
    void EmitPlayerDestroyed(int32_t player, int32_t header, LightObj & payload);
    void EmitVehicleDestroyed(int32_t vehicle, int32_t header, LightObj & payload);
    void EmitBlipCustom(int32_t blip, int32_t header, LightObj & payload);
    void EmitCheckpointCustom(int32_t checkpoint, int32_t header, LightObj & payload);
    void EmitKeyBindCustom(int32_t key_bind, int32_t header, LightObj & payload);
    void EmitObjectCustom(int32_t object, int32_t header, LightObj & payload);
    void EmitPickupCustom(int32_t pickup, int32_t header, LightObj & payload);
    void EmitPlayerCustom(int32_t player, int32_t header, LightObj & payload);
    void EmitVehicleCustom(int32_t vehicle, int32_t header, LightObj & payload);
    void EmitServerStartup() const;
    void EmitServerShutdown() const;
    void EmitServerFrame(float elapsed_time) const;
    void EmitPluginCommand(uint32_t command_identifier, const char * message);
    void EmitIncomingConnection(char * player_name, size_t name_buffer_size, const char * user_password, const char * ip_address);
    void EmitPlayerRequestClass(int32_t player_id, int32_t offset);
    void EmitPlayerRequestSpawn(int32_t player_id);
    void EmitPlayerSpawn(int32_t player_id);
    void EmitPlayerWasted(int32_t player_id, int32_t reason, vcmpBodyPart body_part);
    void EmitPlayerKilled(int32_t player_id, int32_t killer_id, int32_t reason, vcmpBodyPart body_part, bool team_kill);
    void EmitPlayerEmbarking(int32_t player_id, int32_t vehicle_id, int32_t slot_index);
    void EmitPlayerEmbarked(int32_t player_id, int32_t vehicle_id, int32_t slot_index);
    void EmitPlayerDisembark(int32_t player_id, int32_t vehicle_id);
    void EmitPlayerRename(int32_t player_id, const char * old_name, const char * new_name);
    void EmitPlayerState(int32_t player_id, int32_t old_state, int32_t new_state);
    void EmitStateNone(int32_t player_id, int32_t old_state);
    void EmitStateNormal(int32_t player_id, int32_t old_state);
    void EmitStateAim(int32_t player_id, int32_t old_state);
    void EmitStateDriver(int32_t player_id, int32_t old_state);
    void EmitStatePassenger(int32_t player_id, int32_t old_state);
    void EmitStateEnterDriver(int32_t player_id, int32_t old_state);
    void EmitStateEnterPassenger(int32_t player_id, int32_t old_state);
    void EmitStateExit(int32_t player_id, int32_t old_state);
    void EmitStateUnspawned(int32_t player_id, int32_t old_state);
    void EmitPlayerAction(int32_t player_id, int32_t old_action, int32_t new_action);
    void EmitActionNone(int32_t player_id, int32_t old_action);
    void EmitActionNormal(int32_t player_id, int32_t old_action);
    void EmitActionAiming(int32_t player_id, int32_t old_action);
    void EmitActionShooting(int32_t player_id, int32_t old_action);
    void EmitActionJumping(int32_t player_id, int32_t old_action);
    void EmitActionLieDown(int32_t player_id, int32_t old_action);
    void EmitActionGettingUp(int32_t player_id, int32_t old_action);
    void EmitActionJumpVehicle(int32_t player_id, int32_t old_action);
    void EmitActionDriving(int32_t player_id, int32_t old_action);
    void EmitActionDying(int32_t player_id, int32_t old_action);
    void EmitActionWasted(int32_t player_id, int32_t old_action);
    void EmitActionEmbarking(int32_t player_id, int32_t old_action);
    void EmitActionDisembarking(int32_t player_id, int32_t old_action);
    void EmitPlayerBurning(int32_t player_id, bool is_on_fire);
    void EmitPlayerCrouching(int32_t player_id, bool is_crouching);
    void EmitPlayerGameKeys(int32_t player_id, uint32_t old_keys, uint32_t new_keys);
    void EmitPlayerStartTyping(int32_t player_id);
    void EmitPlayerStopTyping(int32_t player_id);
    void EmitPlayerAway(int32_t player_id, bool is_away);
    void EmitPlayerMessage(int32_t player_id, const char * message);
    void EmitPlayerCommand(int32_t player_id, const char * message);
    void EmitPlayerPrivateMessage(int32_t player_id, int32_t target_player_id, const char * message);
    void EmitPlayerKeyPress(int32_t player_id, int32_t bind_id);
    void EmitPlayerKeyRelease(int32_t player_id, int32_t bind_id);
    void EmitPlayerSpectate(int32_t player_id, int32_t target_player_id);
    void EmitPlayerUnspectate(int32_t player_id);
    void EmitPlayerCrashReport(int32_t player_id, const char * report);
    void EmitPlayerModuleList(int32_t player_id, const char * list);
    void EmitVehicleExplode(int32_t vehicle_id);
    void EmitVehicleRespawn(int32_t vehicle_id);
    void EmitObjectShot(int32_t object_id, int32_t player_id, int32_t weapon_id);
    void EmitObjectTouched(int32_t object_id, int32_t player_id);
    void EmitPickupClaimed(int32_t pickup_id, int32_t player_id);
    void EmitPickupCollected(int32_t pickup_id, int32_t player_id);
    void EmitPickupRespawn(int32_t pickup_id);
    void EmitCheckpointEntered(int32_t checkpoint_id, int32_t player_id);
    void EmitCheckpointExited(int32_t checkpoint_id, int32_t player_id);

    /* --------------------------------------------------------------------------------------------
     * Miscellaneous events.
    */
    void EmitCheckpointWorld(int32_t checkpoint_id, int32_t old_world, int32_t new_world);
    void EmitCheckpointRadius(int32_t checkpoint_id, float old_radius, float new_radius);
    void EmitObjectWorld(int32_t object_id, int32_t old_world, int32_t new_world);
    void EmitObjectAlpha(int32_t object_id, int32_t old_alpha, int32_t new_alpha, int32_t time);
    void EmitPickupWorld(int32_t pickup_id, int32_t old_world, int32_t new_world);
    void EmitPickupAlpha(int32_t pickup_id, int32_t old_alpha, int32_t new_alpha);
    void EmitPickupAutomatic(int32_t pickup_id, bool old_status, bool new_status);
    void EmitPickupAutoTimer(int32_t pickup_id, int32_t old_timer, int32_t new_timer);
    void EmitPickupOption(int32_t pickup_id, int32_t option_id, bool value, int32_t header, LightObj & payload);
    void EmitObjectReport(int32_t object_id, bool old_status, bool new_status, bool touched);
    void EmitPlayerHealth(int32_t player_id, float old_health, float new_health);
    void EmitPlayerArmour(int32_t player_id, float old_armour, float new_armour);
    void EmitPlayerWeapon(int32_t player_id, int32_t old_weapon, int32_t new_weapon);
    void EmitPlayerHeading(int32_t player_id, float old_heading, float new_heading);
    void EmitPlayerPosition(int32_t player_id);
    void EmitPlayerOption(int32_t player_id, int32_t option_id, bool value, int32_t header, LightObj & payload);
    void EmitPlayerAdmin(int32_t player_id, bool old_status, bool new_status);
    void EmitPlayerWorld(int32_t player_id, int32_t old_world, int32_t new_world, bool secondary);
    void EmitPlayerTeam(int32_t player_id, int32_t old_team, int32_t new_team);
    void EmitPlayerSkin(int32_t player_id, int32_t old_skin, int32_t new_skin);
    void EmitPlayerMoney(int32_t player_id, int32_t old_money, int32_t new_money);
    void EmitPlayerScore(int32_t player_id, int32_t old_score, int32_t new_score);
    void EmitPlayerWantedLevel(int32_t player_id, int32_t old_level, int32_t new_level);
    void EmitPlayerImmunity(int32_t player_id, int32_t old_immunity, int32_t new_immunity);
    void EmitPlayerAlpha(int32_t player_id, int32_t old_alpha, int32_t new_alpha, int32_t fade);
    void EmitPlayerEnterArea(int32_t player_id, LightObj & area_obj);
    void EmitPlayerLeaveArea(int32_t player_id, LightObj & area_obj);
    void EmitVehicleColor(int32_t vehicle_id, int32_t changed);
    void EmitVehicleHealth(int32_t vehicle_id, float old_health, float new_health);
    void EmitVehiclePosition(int32_t vehicle_id);
    void EmitVehicleRotation(int32_t vehicle_id);
    void EmitVehicleOption(int32_t vehicle_id, int32_t option_id, bool value, int32_t header, LightObj & payload);
    void EmitVehicleWorld(int32_t vehicle_id, int32_t old_world, int32_t new_world);
    void EmitVehicleImmunity(int32_t vehicle_id, int32_t old_immunity, int32_t new_immunity);
    void EmitVehiclePartStatus(int32_t vehicle_id, int32_t part, int32_t old_status, int32_t new_status);
    void EmitVehicleTyreStatus(int32_t vehicle_id, int32_t tyre, int32_t old_status, int32_t new_status);
    void EmitVehicleDamageData(int32_t vehicle_id, uint32_t old_data, uint32_t new_data);
    void EmitVehicleRadio(int32_t vehicle_id, int32_t old_radio, int32_t new_radio);
    void EmitVehicleHandlingRule(int32_t vehicle_id, int32_t rule, SQFloat old_data, SQFloat new_data);
    void EmitVehicleEnterArea(int32_t player_id, LightObj & area_obj);
    void EmitVehicleLeaveArea(int32_t player_id, LightObj & area_obj);
    void EmitServerOption(int32_t option, bool value, int32_t header, LightObj & payload);
    void EmitScriptReload(int32_t header, LightObj & payload) const;
    void EmitScriptLoaded() const;

    /* --------------------------------------------------------------------------------------------
     * Entity pool changes events.
    */
    void EmitEntityPool(vcmpEntityPool entity_type, int32_t entity_id, bool is_deleted);
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
    void EmitPlayerUpdate(int32_t player_id, vcmpPlayerUpdate update_type);
    void EmitVehicleUpdate(int32_t vehicle_id, vcmpVehicleUpdate update_type);

    /* --------------------------------------------------------------------------------------------
     * Client data streams event.
    */
    void EmitClientScriptData(int32_t player_id, const uint8_t * data, size_t size);

public:

    /* --------------------------------------------------------------------------------------------
     * Module signals.
    */
    SignalPair  mOnPreLoad{};
    SignalPair  mOnPostLoad{};
    SignalPair  mOnUnload{};
    SignalPair  mOnScript{}; // Callback to be invoked for each loaded script.

    /* --------------------------------------------------------------------------------------------
     * Server signals.
    */
    SignalPair  mOnCustomEvent{};
    SignalPair  mOnBlipCreated{};
    SignalPair  mOnCheckpointCreated{};
    SignalPair  mOnKeyBindCreated{};
    SignalPair  mOnObjectCreated{};
    SignalPair  mOnPickupCreated{};
    SignalPair  mOnPlayerCreated{};
    SignalPair  mOnVehicleCreated{};
    SignalPair  mOnBlipDestroyed{};
    SignalPair  mOnCheckpointDestroyed{};
    SignalPair  mOnKeyBindDestroyed{};
    SignalPair  mOnObjectDestroyed{};
    SignalPair  mOnPickupDestroyed{};
    SignalPair  mOnPlayerDestroyed{};
    SignalPair  mOnVehicleDestroyed{};
    SignalPair  mOnBlipCustom{};
    SignalPair  mOnCheckpointCustom{};
    SignalPair  mOnKeyBindCustom{};
    SignalPair  mOnObjectCustom{};
    SignalPair  mOnPickupCustom{};
    SignalPair  mOnPlayerCustom{};
    SignalPair  mOnVehicleCustom{};
#if SQMOD_SDK_LEAST(2, 1)
    SignalPair  mOnCheckpointStream{};
    SignalPair  mOnObjectStream{};
    SignalPair  mOnPickupStream{};
    SignalPair  mOnPlayerStream{};
    SignalPair  mOnVehicleStream{};
#endif
    SignalPair  mOnServerStartup{};
    SignalPair  mOnServerShutdown{};
    SignalPair  mOnServerFrame{};
    SignalPair  mOnIncomingConnection{};
    SignalPair  mOnPlayerRequestClass{};
    SignalPair  mOnPlayerRequestSpawn{};
    SignalPair  mOnPlayerSpawn{};
    SignalPair  mOnPlayerWasted{};
    SignalPair  mOnPlayerKilled{};
    SignalPair  mOnPlayerEmbarking{};
    SignalPair  mOnPlayerEmbarked{};
    SignalPair  mOnPlayerDisembark{};
    SignalPair  mOnPlayerRename{};
    SignalPair  mOnPlayerState{};
    SignalPair  mOnStateNone{};
    SignalPair  mOnStateNormal{};
    SignalPair  mOnStateAim{};
    SignalPair  mOnStateDriver{};
    SignalPair  mOnStatePassenger{};
    SignalPair  mOnStateEnterDriver{};
    SignalPair  mOnStateEnterPassenger{};
    SignalPair  mOnStateExit{};
    SignalPair  mOnStateUnspawned{};
    SignalPair  mOnPlayerAction{};
    SignalPair  mOnActionNone{};
    SignalPair  mOnActionNormal{};
    SignalPair  mOnActionAiming{};
    SignalPair  mOnActionShooting{};
    SignalPair  mOnActionJumping{};
    SignalPair  mOnActionLieDown{};
    SignalPair  mOnActionGettingUp{};
    SignalPair  mOnActionJumpVehicle{};
    SignalPair  mOnActionDriving{};
    SignalPair  mOnActionDying{};
    SignalPair  mOnActionWasted{};
    SignalPair  mOnActionEmbarking{};
    SignalPair  mOnActionDisembarking{};
    SignalPair  mOnPlayerBurning{};
    SignalPair  mOnPlayerCrouching{};
    SignalPair  mOnPlayerGameKeys{};
    SignalPair  mOnPlayerStartTyping{};
    SignalPair  mOnPlayerStopTyping{};
    SignalPair  mOnPlayerAway{};
    SignalPair  mOnPlayerMessage{};
    SignalPair  mOnPlayerCommand{};
    SignalPair  mOnPlayerPrivateMessage{};
    SignalPair  mOnPlayerKeyPress{};
    SignalPair  mOnPlayerKeyRelease{};
    SignalPair  mOnPlayerSpectate{};
    SignalPair  mOnPlayerUnspectate{};
    SignalPair  mOnPlayerCrashReport{};
    SignalPair  mOnPlayerModuleList{};
    SignalPair  mOnVehicleExplode{};
    SignalPair  mOnVehicleRespawn{};
    SignalPair  mOnObjectShot{};
    SignalPair  mOnObjectTouched{};
    SignalPair  mOnObjectWorld{};
    SignalPair  mOnObjectAlpha{};
    SignalPair  mOnObjectReport{};
    SignalPair  mOnPickupClaimed{};
    SignalPair  mOnPickupCollected{};
    SignalPair  mOnPickupRespawn{};
    SignalPair  mOnPickupWorld{};
    SignalPair  mOnPickupAlpha{};
    SignalPair  mOnPickupAutomatic{};
    SignalPair  mOnPickupAutoTimer{};
    SignalPair  mOnPickupOption{};
    SignalPair  mOnCheckpointEntered{};
    SignalPair  mOnCheckpointExited{};
    SignalPair  mOnCheckpointWorld{};
    SignalPair  mOnCheckpointRadius{};
    SignalPair  mOnEntityPool{};
    SignalPair  mOnClientScriptData{};
    SignalPair  mOnPlayerUpdate{};
    SignalPair  mOnVehicleUpdate{};
    SignalPair  mOnPlayerHealth{};
    SignalPair  mOnPlayerArmour{};
    SignalPair  mOnPlayerWeapon{};
    SignalPair  mOnPlayerHeading{};
    SignalPair  mOnPlayerPosition{};
    SignalPair  mOnPlayerOption{};
    SignalPair  mOnPlayerAdmin{};
    SignalPair  mOnPlayerWorld{};
    SignalPair  mOnPlayerTeam{};
    SignalPair  mOnPlayerSkin{};
    SignalPair  mOnPlayerMoney{};
    SignalPair  mOnPlayerScore{};
    SignalPair  mOnPlayerWantedLevel{};
    SignalPair  mOnPlayerImmunity{};
    SignalPair  mOnPlayerAlpha{};
    SignalPair  mOnPlayerEnterArea{};
    SignalPair  mOnPlayerLeaveArea{};
    SignalPair  mOnVehicleColor{};
    SignalPair  mOnVehicleHealth{};
    SignalPair  mOnVehiclePosition{};
    SignalPair  mOnVehicleRotation{};
    SignalPair  mOnVehicleOption{};
    SignalPair  mOnVehicleWorld{};
    SignalPair  mOnVehicleImmunity{};
    SignalPair  mOnVehiclePartStatus{};
    SignalPair  mOnVehicleTyreStatus{};
    SignalPair  mOnVehicleDamageData{};
    SignalPair  mOnVehicleRadio{};
    SignalPair  mOnVehicleHandlingRule{};
    SignalPair  mOnVehicleEnterArea{};
    SignalPair  mOnVehicleLeaveArea{};
#if SQMOD_SDK_LEAST(2, 1)
    SignalPair  mOnEntityStream{};
#endif
    SignalPair  mOnServerOption{};
    SignalPair  mOnScriptReload{};
    SignalPair  mOnScriptLoaded{};
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
        : m_State(Core::Get().GetState()), m_Start(m_State)
    {
        //...
    }

    /* --------------------------------------------------------------------------------------------
     * Backup the current core state and set the given state.
    */
    explicit CoreState(int s)
        : CoreState()
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
     * Retrieve the initial state.
    */
    SQMOD_NODISCARD int GetStart() const
    {
        return m_Start;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the guarded state.
    */
    SQMOD_NODISCARD int GetState() const
    {
        return m_State;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the guarded state.
    */
    CoreState & SetState(int s)
    {
        m_State = s;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Restore the guarded state. I.e the original state value at the time of construction.
    */
    CoreState & Regress()
    {
        m_State = m_Start;
        return *this;
    }

protected:

    int m_State; // The core state to be applied by destructor.
    int m_Start; // The core state at the time when this instance was created.
};

/* ------------------------------------------------------------------------------------------------
 * Generic implementation of entity iteration.
*/
template < class T, class F > inline void ForeachEntity(std::vector< T > & vec, F cb)
{
    for (auto & e : vec)
    {
        cb(e);
    }
}
// Entity iteration.
template < class F > inline void ForeachBlip(F f) { ForeachEntity(Core::Get().GetBlips(), std::forward< F >(f)); }
template < class F > inline void ForeachCheckpoint(F f) { ForeachEntity(Core::Get().GetCheckpoints(), std::forward< F >(f)); }
template < class F > inline void ForeachObject(F f) { ForeachEntity(Core::Get().GetObjs(), std::forward< F >(f)); }
template < class F > inline void ForeachPickup(F f) { ForeachEntity(Core::Get().GetPickups(), std::forward< F >(f)); }
template < class F > inline void ForeachPlayer(F f) { ForeachEntity(Core::Get().GetPlayers(), std::forward< F >(f)); }
template < class F > inline void ForeachVehicle(F f) { ForeachEntity(Core::Get().GetVehicles(), std::forward< F >(f)); }

/* ------------------------------------------------------------------------------------------------
 * Generic implementation of active entity iteration.
*/
template < class T, class F > inline void ForeachActiveEntity(const std::vector< T > & vec, F cb)
{
    for (const auto & e : vec)
    {
        if (VALID_ENTITY(e.mID)) cb(e);
    }
}
// Entity iteration.
template < class F > inline void ForeachActiveBlip(F f) { ForeachActiveEntity(Core::Get().GetBlips(), std::forward< F >(f)); }
template < class F > inline void ForeachActiveCheckpoint(F f) { ForeachActiveEntity(Core::Get().GetCheckpoints(), std::forward< F >(f)); }
template < class F > inline void ForeachActiveObject(F f) { ForeachActiveEntity(Core::Get().GetObjs(), std::forward< F >(f)); }
template < class F > inline void ForeachActivePickup(F f) { ForeachActiveEntity(Core::Get().GetPickups(), std::forward< F >(f)); }
template < class F > inline void ForeachActivePlayer(F f) { ForeachActiveEntity(Core::Get().GetPlayers(), std::forward< F >(f)); }
template < class F > inline void ForeachActiveVehicle(F f) { ForeachActiveEntity(Core::Get().GetVehicles(), std::forward< F >(f)); }

/* ------------------------------------------------------------------------------------------------
 * Process the identifier of each player slot.
*/
template < class F > inline void ForeachPlayerSlot(F f) {
    for (int32_t i = 0, n = static_cast< int32_t >(_Func->GetMaxPlayers()); i < n; ++i) {
        f(i);
    }
}
/* ------------------------------------------------------------------------------------------------
 * Process the identifier of each player slot and count processed players.
*/
template < class F > SQMOD_NODISCARD inline int32_t ForeachPlayerSlotCount(F f) {
    int32_t c = 0;
    for (int32_t i = 0, n = static_cast< int32_t >(_Func->GetMaxPlayers()); i < n; ++i) {
        if (f(i)) ++c;
    }
    return c;
}
/* ------------------------------------------------------------------------------------------------
 * Process the identifier of each player slot until a certain criteria is met.
*/
template < class F > SQMOD_NODISCARD inline int32_t ForeachPlayerSlotUntil(F f) {
    for (int32_t i = 0, n = static_cast< int32_t >(_Func->GetMaxPlayers()); i < n; ++i) {
        if (f(i)) return i;
    }
    return -1;
}

/* ------------------------------------------------------------------------------------------------
 * Process the identifier of each connected player.
*/
template < class F > inline void ForeachConnectedPlayer(F f) {
    for (int32_t i = 0, n = static_cast< int32_t >(_Func->GetMaxPlayers()); i < n; ++i) {
        if (_Func->IsPlayerConnected(i) != 0) f(i);
    }
}
/* ------------------------------------------------------------------------------------------------
 * Process the identifier of each connected player and count processed players.
*/
template < class F > SQMOD_NODISCARD inline int32_t ForeachConnectedPlayerCount(F f) {
    int32_t c = 0;
    for (int32_t i = 0, n = static_cast< int32_t >(_Func->GetMaxPlayers()); i < n; ++i) {
        if (_Func->IsPlayerConnected(i) != 0 && f(i)) ++c;
    }
    return c;
}
/* ------------------------------------------------------------------------------------------------
 * Process the identifier of each connected player until a certain criteria is met.
*/
template < class F > SQMOD_NODISCARD inline int32_t ForeachConnectedPlayerUntil(F f) {
    for (int32_t i = 0, n = static_cast< int32_t >(_Func->GetMaxPlayers()); i < n; ++i) {
        if (_Func->IsPlayerConnected(i) != 0 && f(i)) return i;
    }
    return -1;
}

/* ------------------------------------------------------------------------------------------------
 * Used to select entity instances based on type.
*/
template < class > struct EntityInstSelect;
// Specialization for blips.
template < > struct EntityInstSelect< CBlip > {
    static BlipInst & Get(int32_t id) { return Core::Get().GetBlip(id); }
};
// Specialization for checkpoints.
template < > struct EntityInstSelect< CCheckpoint > {
    static CheckpointInst & Get(int32_t id) { return Core::Get().GetCheckpoint(id); }
};
// Specialization for keybinds.
template < > struct EntityInstSelect< CKeyBind > {
    static KeyBindInst & Get(int32_t id) { return Core::Get().GetKeyBind(id); }
};
// Specialization for objects.
template < > struct EntityInstSelect< CObject > {
    static ObjectInst & Get(int32_t id) { return Core::Get().GetObj(id); }
};
// Specialization for pickups.
template < > struct EntityInstSelect< CPickup > {
    static PickupInst & Get(int32_t id) { return Core::Get().GetPickup(id); }
};
// Specialization for players.
template < > struct EntityInstSelect< CPlayer > {
    static PlayerInst & Get(int32_t id) { return Core::Get().GetPlayer(id); }
};
// Specialization for vehicles.
template < > struct EntityInstSelect< CVehicle > {
    static VehicleInst & Get(int32_t id) { return Core::Get().GetVehicle(id); }
};

} // Namespace:: SqMod
