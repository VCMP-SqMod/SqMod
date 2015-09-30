#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Signal.hpp"

// ------------------------------------------------------------------------------------------------
#include "Base/Vector3.hpp"
#include "Base/Color4.hpp"

// ------------------------------------------------------------------------------------------------
#include <array>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
enum EntityType
{
    ENT_UNKNOWN = 0,
    ENT_BLIP,
    ENT_CHECKPOINT,
    ENT_KEYBIND,
    ENT_OBJECT,
    ENT_PICKUP,
    ENT_PLAYER,
    ENT_SPHERE,
    ENT_SPRITE,
    ENT_TEXTDRAW,
    ENT_VEHICLE,
    ENT_COUNT
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template < class T > class Ent;

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> class Ent< CBlip >
{
private:

    // --------------------------------------------------------------------------------------------
    friend class EntMan< CBlip >;
    friend class Reference< CBlip >;

    // --------------------------------------------------------------------------------------------
    typedef Reference< CBlip >  RefType;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    typedef struct Blip
    {
        // ----------------------------------------------------------------------------------------
        Blip() : ID(-1), Root(0), Owned(false), Fresh(true) { /* ... */ }

        // ----------------------------------------------------------------------------------------
        SQInt32                 ID;

        // ----------------------------------------------------------------------------------------
        RefType*                Root;

        // ----------------------------------------------------------------------------------------
        bool                    Owned;
        bool                    Fresh;

        // ----------------------------------------------------------------------------------------
        SqTag                   Tag;
        SqObj                   Data;

        // ----------------------------------------------------------------------------------------
        SQInt32                 World;
        SQInt32                 Scale;

        // ----------------------------------------------------------------------------------------
        SQInt32                 SprID;

        // ----------------------------------------------------------------------------------------
        Vector3                 Position;
        Color4                  Color;

        // ----------------------------------------------------------------------------------------
        EBlipCreated            BlipCreated;
        EBlipDestroyed          BlipDestroyed;
        EBlipCustom             BlipCustom;

    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst, SQInt32 world, SQInt32 scale, SQInt32 sprite, \
                    const Vector3 & pos, const Color4 & color) noexcept
    {
        inst.World = world;
        inst.Scale = scale;
        inst.SprID = sprite;
        inst.Position = pos;
        inst.Color = color;
    }

    // --------------------------------------------------------------------------------------------
    static void Clear(Instance & inst) noexcept
    {
        inst.BlipCreated.Clear();
        inst.BlipDestroyed.Clear();
        inst.BlipCustom.Clear();
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID = ENT_BLIP;
    static constexpr SQInt32 Limit  = SQMOD_BLIP_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Blip";
    static constexpr const SQChar* CName    = "CBlip";

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit > Instances;
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> class Ent< CCheckpoint >
{
private:

    // --------------------------------------------------------------------------------------------
    friend class EntMan< CCheckpoint >;
    friend class Reference< CCheckpoint >;

    // --------------------------------------------------------------------------------------------
    typedef Reference< CCheckpoint >    RefType;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    typedef struct Checkpoint
    {
        // ----------------------------------------------------------------------------------------
        Checkpoint() : ID(-1), Root(0), Owned(false), Fresh(true) { /* ... */ }

        // ----------------------------------------------------------------------------------------
        SQInt32                 ID;

        // ----------------------------------------------------------------------------------------
        RefType*                Root;

        // ----------------------------------------------------------------------------------------
        bool                    Owned;
        bool                    Fresh;

        // ----------------------------------------------------------------------------------------
        SqTag                   Tag;
        SqObj                   Data;

        // ----------------------------------------------------------------------------------------
        ECheckpointCreated      CheckpointCreated;
        ECheckpointDestroyed    CheckpointDestroyed;
        ECheckpointCustom       CheckpointCustom;
        ECheckpointEntered      CheckpointEntered;
        ECheckpointExited       CheckpointExited;
    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst) noexcept
    {
        /* ... */
    }

    // --------------------------------------------------------------------------------------------
    static void Clear(Instance & inst) noexcept
    {
        inst.CheckpointCreated.Clear();
        inst.CheckpointDestroyed.Clear();
        inst.CheckpointCustom.Clear();
        inst.CheckpointEntered.Clear();
        inst.CheckpointExited.Clear();
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID = ENT_CHECKPOINT;
    static constexpr SQInt32 Limit  = SQMOD_CHECKPOINT_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Checkpoint";
    static constexpr const SQChar* CName    = "CCheckpoint";

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit > Instances;
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> class Ent< CKeybind >
{
private:

    // --------------------------------------------------------------------------------------------
    friend class EntMan< CKeybind >;
    friend class Reference< CKeybind >;

    // --------------------------------------------------------------------------------------------
    typedef Reference< CKeybind >   RefType;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    typedef struct Keybind
    {
        // ----------------------------------------------------------------------------------------
        Keybind() : ID(-1), Root(0), Owned(false), Fresh(true) { /* ... */ }

        // ----------------------------------------------------------------------------------------
        SQInt32                 ID;

        // ----------------------------------------------------------------------------------------
        RefType*                Root;

        // ----------------------------------------------------------------------------------------
        bool                    Owned;
        bool                    Fresh;

        // ----------------------------------------------------------------------------------------
        SqTag                   Tag;
        SqObj                   Data;

        // ----------------------------------------------------------------------------------------
        SQInt32                 Primary;
        SQInt32                 Secondary;
        SQInt32                 Alternative;

        // ----------------------------------------------------------------------------------------
        bool                    Release;

        // ----------------------------------------------------------------------------------------
        EKeybindCreated         KeybindCreated;
        EKeybindDestroyed       KeybindDestroyed;
        EKeybindCustom          KeybindCustom;
        EKeybindKeyPress        KeybindKeyPress;
        EKeybindKeyRelease      KeybindKeyRelease;
    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst, SQInt32 primary, SQInt32 secondary, SQInt32 alternative, \
                        bool release) noexcept
    {
        inst.Primary = primary;
        inst.Secondary = secondary;
        inst.Alternative = alternative;
        inst.Release = release;
    }

    // --------------------------------------------------------------------------------------------
    static void Clear(Instance & inst) noexcept
    {
        inst.KeybindCreated.Clear();
        inst.KeybindDestroyed.Clear();
        inst.KeybindCustom.Clear();
        inst.KeybindKeyPress.Clear();
        inst.KeybindKeyRelease.Clear();
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID = ENT_KEYBIND;
    static constexpr SQInt32 Limit  = SQMOD_KEYBIND_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Keybind";
    static constexpr const SQChar* CName    = "CKeybind";

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit > Instances;
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> class Ent< CObject >
{
private:

    // --------------------------------------------------------------------------------------------
    friend class EntMan< CObject >;
    friend class Reference< CObject >;

    // --------------------------------------------------------------------------------------------
    typedef Reference< CObject >    RefType;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    typedef struct Object
    {
        // ----------------------------------------------------------------------------------------
        Object() : ID(-1), Root(0), Owned(false), Fresh(true) { /* ... */ }

        // ----------------------------------------------------------------------------------------
        SQInt32                 ID;

        // ----------------------------------------------------------------------------------------
        RefType*                Root;

        // ----------------------------------------------------------------------------------------
        bool                    Owned;
        bool                    Fresh;

        // ----------------------------------------------------------------------------------------
        SqTag                   Tag;
        SqObj                   Data;

        // ----------------------------------------------------------------------------------------
        EObjectCreated          ObjectCreated;
        EObjectDestroyed        ObjectDestroyed;
        EObjectCustom           ObjectCustom;
        EObjectShot             ObjectShot;
        EObjectBump             ObjectBump;
    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst) noexcept
    {
        /* ... */
    }

    // --------------------------------------------------------------------------------------------
    static void Clear(Instance & inst) noexcept
    {
        inst.ObjectCreated.Clear();
        inst.ObjectDestroyed.Clear();
        inst.ObjectCustom.Clear();
        inst.ObjectShot.Clear();
        inst.ObjectBump.Clear();
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID = ENT_OBJECT;
    static constexpr SQInt32 Limit  = SQMOD_OBJECT_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Object";
    static constexpr const SQChar* CName    = "CObject";

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit > Instances;
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> class Ent< CPickup >
{
private:

    // --------------------------------------------------------------------------------------------
    friend class EntMan< CPickup >;
    friend class Reference< CPickup >;

    // --------------------------------------------------------------------------------------------
    typedef Reference< CPickup >    RefType;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    typedef struct Pickup
    {
        // ----------------------------------------------------------------------------------------
        Pickup() : ID(-1), Root(0), Owned(false), Fresh(true) { /* ... */ }

        // ----------------------------------------------------------------------------------------
        SQInt32                 ID;

        // ----------------------------------------------------------------------------------------
        RefType*                Root;

        // ----------------------------------------------------------------------------------------
        bool                    Owned;
        bool                    Fresh;

        // ----------------------------------------------------------------------------------------
        SqTag                   Tag;
        SqObj                   Data;

        // ----------------------------------------------------------------------------------------
        EPickupCreated          PickupCreated;
        EPickupDestroyed        PickupDestroyed;
        EPickupCustom           PickupCustom;
        EPickupRespawn          PickupRespawn;
        EPickupClaimed          PickupClaimed;
        EPickupCollected        PickupCollected;
    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst) noexcept
    {
        /* ... */
    }

    // --------------------------------------------------------------------------------------------
    static void Clear(Instance & inst) noexcept
    {
        inst.PickupCreated.Clear();
        inst.PickupDestroyed.Clear();
        inst.PickupCustom.Clear();
        inst.PickupRespawn.Clear();
        inst.PickupClaimed.Clear();
        inst.PickupCollected.Clear();
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID = ENT_PICKUP;
    static constexpr SQInt32 Limit  = SQMOD_PICKUP_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Pickup";
    static constexpr const SQChar* CName    = "CPickup";

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit > Instances;
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> class Ent< CPlayer >
{
private:

    // --------------------------------------------------------------------------------------------
    friend class EntMan< CPlayer >;
    friend class Reference< CPlayer >;

    // --------------------------------------------------------------------------------------------
    typedef Reference< CPlayer >    RefType;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    typedef struct Player
    {
        // ----------------------------------------------------------------------------------------
        Player() : ID(-1), Root(0), Owned(false), Fresh(true) { /* ... */ }

        // ----------------------------------------------------------------------------------------
        SQInt32                 ID;

        // ----------------------------------------------------------------------------------------
        RefType*                Root;

        // ----------------------------------------------------------------------------------------
        bool                    Owned; /* Useless but required by the instance activation system */
        bool                    Fresh;

        // ----------------------------------------------------------------------------------------
        SqTag                   Tag;
        SqObj                   Data;

        // ----------------------------------------------------------------------------------------
        EPlayerCreated          PlayerCreated;
        EPlayerDestroyed        PlayerDestroyed;
        EPlayerCustom           PlayerCustom;
        EPlayerAway             PlayerAway;
        EPlayerGameKeys         PlayerGameKeys;
        EPlayerRename           PlayerRename;
        EPlayerRequestClass     PlayerRequestClass;
        EPlayerRequestSpawn     PlayerRequestSpawn;
        EPlayerSpawn            PlayerSpawn;
        EPlayerStartTyping      PlayerStartTyping;
        EPlayerStopTyping       PlayerStopTyping;
        EPlayerChat             PlayerChat;
        EPlayerCommand          PlayerCommand;
        EPlayerMessage          PlayerMessage;
        EPlayerHealth           PlayerHealth;
        EPlayerArmour           PlayerArmour;
        EPlayerWeapon           PlayerWeapon;
        EPlayerMove             PlayerMove;
        EPlayerWasted           PlayerWasted;
        EPlayerKilled           PlayerKilled;
        EPlayerTeamKill         PlayerTeamKill;
        EPlayerSpectate         PlayerSpectate;
        EPlayerCrashreport      PlayerCrashreport;
        EPlayerBurning          PlayerBurning;
        EPlayerCrouching        PlayerCrouching;
        EPlayerState            PlayerState;
        EPlayerAction           PlayerAction;
        EStateNone              StateNone;
        EStateNormal            StateNormal;
        EStateShooting          StateShooting;
        EStateDriver            StateDriver;
        EStatePassenger         StatePassenger;
        EStateEnterDriver       StateEnterDriver;
        EStateEnterPassenger    StateEnterPassenger;
        EStateExitVehicle       StateExitVehicle;
        EStateUnspawned         StateUnspawned;
        EActionNone             ActionNone;
        EActionNormal           ActionNormal;
        EActionAiming           ActionAiming;
        EActionShooting         ActionShooting;
        EActionJumping          ActionJumping;
        EActionLieDown          ActionLieDown;
        EActionGettingUp        ActionGettingUp;
        EActionJumpVehicle      ActionJumpVehicle;
        EActionDriving          ActionDriving;
        EActionDying            ActionDying;
        EActionWasted           ActionWasted;
        EActionEmbarking        ActionEmbarking;
        EActionDisembarking     ActionDisembarking;
    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst) noexcept
    {
        /* ... */
    }

    // --------------------------------------------------------------------------------------------
    static void Clear(Instance & inst) noexcept
    {
        inst.PlayerCreated.Clear();
        inst.PlayerDestroyed.Clear();
        inst.PlayerCustom.Clear();
        inst.PlayerAway.Clear();
        inst.PlayerGameKeys.Clear();
        inst.PlayerRename.Clear();
        inst.PlayerRequestClass.Clear();
        inst.PlayerRequestSpawn.Clear();
        inst.PlayerSpawn.Clear();
        inst.PlayerStartTyping.Clear();
        inst.PlayerStopTyping.Clear();
        inst.PlayerChat.Clear();
        inst.PlayerCommand.Clear();
        inst.PlayerMessage.Clear();
        inst.PlayerHealth.Clear();
        inst.PlayerArmour.Clear();
        inst.PlayerWeapon.Clear();
        inst.PlayerMove.Clear();
        inst.PlayerWasted.Clear();
        inst.PlayerKilled.Clear();
        inst.PlayerTeamKill.Clear();
        inst.PlayerSpectate.Clear();
        inst.PlayerCrashreport.Clear();
        inst.PlayerBurning.Clear();
        inst.PlayerCrouching.Clear();
        inst.PlayerState.Clear();
        inst.PlayerAction.Clear();
        inst.StateNone.Clear();
        inst.StateNormal.Clear();
        inst.StateShooting.Clear();
        inst.StateDriver.Clear();
        inst.StatePassenger.Clear();
        inst.StateEnterDriver.Clear();
        inst.StateEnterPassenger.Clear();
        inst.StateExitVehicle.Clear();
        inst.StateUnspawned.Clear();
        inst.ActionNone.Clear();
        inst.ActionNormal.Clear();
        inst.ActionAiming.Clear();
        inst.ActionShooting.Clear();
        inst.ActionJumping.Clear();
        inst.ActionLieDown.Clear();
        inst.ActionGettingUp.Clear();
        inst.ActionJumpVehicle.Clear();
        inst.ActionDriving.Clear();
        inst.ActionDying.Clear();
        inst.ActionWasted.Clear();
        inst.ActionEmbarking.Clear();
        inst.ActionDisembarking.Clear();
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID = ENT_PLAYER;
    static constexpr SQInt32 Limit  = SQMOD_PLAYER_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Player";
    static constexpr const SQChar* CName    = "CPlayer";

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit > Instances;
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> class Ent< CSphere >
{
private:

    // --------------------------------------------------------------------------------------------
    friend class EntMan< CSphere >;
    friend class Reference< CSphere >;

    // --------------------------------------------------------------------------------------------
    typedef Reference< CSphere >    RefType;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    typedef struct Sphere
    {
        // ----------------------------------------------------------------------------------------
        Sphere() : ID(-1), Root(0), Owned(false), Fresh(true) { /* ... */ }

        // ----------------------------------------------------------------------------------------
        SQInt32                 ID;

        // ----------------------------------------------------------------------------------------
        RefType*                Root;

        // ----------------------------------------------------------------------------------------
        bool                    Owned;
        bool                    Fresh;

        // ----------------------------------------------------------------------------------------
        SqTag                   Tag;
        SqObj                   Data;

        // ----------------------------------------------------------------------------------------
        ESphereCreated          SphereCreated;
        ESphereDestroyed        SphereDestroyed;
        ESphereCustom           SphereCustom;
        ESphereEntered          SphereEntered;
        ESphereExited           SphereExited;
    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst) noexcept
    {
        /* ... */
    }

    // --------------------------------------------------------------------------------------------
    static void Clear(Instance & inst) noexcept
    {
        inst.SphereCreated.Clear();
        inst.SphereDestroyed.Clear();
        inst.SphereCustom.Clear();
        inst.SphereEntered.Clear();
        inst.SphereExited.Clear();
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID = ENT_SPHERE;
    static constexpr SQInt32 Limit  = SQMOD_SPHERE_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Sphere";
    static constexpr const SQChar* CName    = "CSphere";

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit > Instances;
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> class Ent< CSprite >
{
private:

    // --------------------------------------------------------------------------------------------
    friend class EntMan< CSprite >;
    friend class Reference< CSprite >;

    // --------------------------------------------------------------------------------------------
    typedef Reference< CSprite >    RefType;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    typedef struct Sprite
    {
        // ----------------------------------------------------------------------------------------
        Sprite() : ID(-1), Root(0), Owned(false), Fresh(true) { /* ... */ }

        // ----------------------------------------------------------------------------------------
        SQInt32                 ID;

        // ----------------------------------------------------------------------------------------
        RefType*                Root;

        // ----------------------------------------------------------------------------------------
        bool                    Owned;
        bool                    Fresh;

        // ----------------------------------------------------------------------------------------
        SqTag                   Tag;
        SqObj                   Data;

        // ----------------------------------------------------------------------------------------
        String                  Path;

        // ----------------------------------------------------------------------------------------
        ESpriteCreated          SpriteCreated;
        ESpriteDestroyed        SpriteDestroyed;
        ESpriteCustom           SpriteCustom;
    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst, const String & path) noexcept
    {
        inst.Path = path;
    }

    // --------------------------------------------------------------------------------------------
    static void Clear(Instance & inst) noexcept
    {
        inst.SpriteCreated.Clear();
        inst.SpriteDestroyed.Clear();
        inst.SpriteCustom.Clear();
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID = ENT_SPRITE;
    static constexpr SQInt32 Limit  = SQMOD_SPRITE_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Sprite";
    static constexpr const SQChar* CName    = "CSprite";

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit > Instances;
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> class Ent< CTextdraw >
{
private:

    // --------------------------------------------------------------------------------------------
    friend class EntMan< CTextdraw >;
    friend class Reference< CTextdraw >;

    // --------------------------------------------------------------------------------------------
    typedef Reference< CTextdraw >  RefType;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    typedef struct Textdraw
    {
        // ----------------------------------------------------------------------------------------
        Textdraw() : ID(-1), Root(0), Owned(false), Fresh(true) { /* ... */ }

        // ----------------------------------------------------------------------------------------
        SQInt32                 ID;

        // ----------------------------------------------------------------------------------------
        RefType*                Root;

        // ----------------------------------------------------------------------------------------
        bool                    Owned;
        bool                    Fresh;

        // ----------------------------------------------------------------------------------------
        SqTag                   Tag;
        SqObj                   Data;

        // ----------------------------------------------------------------------------------------
        String                  Text;

        // ----------------------------------------------------------------------------------------
        ETextdrawCreated        TextdrawCreated;
        ETextdrawDestroyed      TextdrawDestroyed;
        ETextdrawCustom         TextdrawCustom;
    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst, const String & text) noexcept
    {
        inst.Text = text;
    }

    // --------------------------------------------------------------------------------------------
    static void Clear(Instance & inst) noexcept
    {
        inst.TextdrawCreated.Clear();
        inst.TextdrawDestroyed.Clear();
        inst.TextdrawCustom.Clear();
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID = ENT_TEXTDRAW;
    static constexpr SQInt32 Limit  = SQMOD_TEXTDRAW_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Textdraw";
    static constexpr const SQChar* CName    = "CTextdraw";

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit > Instances;
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> class Ent< CVehicle >
{
private:

    // --------------------------------------------------------------------------------------------
    friend class EntMan< CVehicle >;
    friend class Reference< CVehicle >;

    // --------------------------------------------------------------------------------------------
    typedef Reference< CVehicle >   RefType;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    typedef struct Vehicle
    {
        // ----------------------------------------------------------------------------------------
        Vehicle() : ID(-1), Root(0), Owned(false), Fresh(true) { /* ... */ }

        // ----------------------------------------------------------------------------------------
        SQInt32                 ID;

        // ----------------------------------------------------------------------------------------
        RefType*                Root;

        // ----------------------------------------------------------------------------------------
        bool                    Owned;
        bool                    Fresh;

        // ----------------------------------------------------------------------------------------
        SqTag                   Tag;
        SqObj                   Data;

        // ----------------------------------------------------------------------------------------
        EVehicleCreated         VehicleCreated;
        EVehicleDestroyed       VehicleDestroyed;
        EVehicleCustom          VehicleCustom;
        EVehicleRespawn         VehicleRespawn;
        EVehicleExplode         VehicleExplode;
        EVehicleHealth          VehicleHealth;
        EVehicleMove            VehicleMove;
        EVehicleEmbarking       VehicleEmbarking;
        EVehicleEmbarked        VehicleEmbarked;
        EVehicleDisembark       VehicleDisembark;
    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst) noexcept
    {

    }

    // --------------------------------------------------------------------------------------------
    static void Clear(Instance & inst) noexcept
    {
        inst.VehicleCreated.Clear();
        inst.VehicleDestroyed.Clear();
        inst.VehicleCustom.Clear();
        inst.VehicleRespawn.Clear();
        inst.VehicleExplode.Clear();
        inst.VehicleHealth.Clear();
        inst.VehicleMove.Clear();
        inst.VehicleEmbarking.Clear();
        inst.VehicleEmbarked.Clear();
        inst.VehicleDisembark.Clear();
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID = ENT_VEHICLE;
    static constexpr SQInt32 Limit  = SQMOD_VEHICLE_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Vehicle";
    static constexpr const SQChar* CName    = "CVehicle";

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit > Instances;
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template < class T > class Reference
{
    // --------------------------------------------------------------------------------------------
    friend class EntMan< T >;

public:

    // --------------------------------------------------------------------------------------------
    typedef T                   Type;

    // --------------------------------------------------------------------------------------------
    typedef Reference< T >      RefType;

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32    Max = Ent< T >::Limit;

protected:

    // --------------------------------------------------------------------------------------------
    static typename Ent< T >::Instances s_Instances;

protected:

    // --------------------------------------------------------------------------------------------
    SQInt32                 m_ID;

    // --------------------------------------------------------------------------------------------
    SqTag                   m_Tag;
    SqObj                   m_Data;

    // --------------------------------------------------------------------------------------------
    Reference< T > *        m_Prev;
    Reference< T > *        m_Next;

    // --------------------------------------------------------------------------------------------
    bool                    m_Persistent;

protected:

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void InsertIntoChain() noexcept
    {
        if (VALID_ENTITY(m_ID))
        {
            if (s_Instances[m_ID].Root)
            {
                m_Next = s_Instances[m_ID].Root;
                m_Prev = m_Next->m_Prev;
                m_Next->m_Prev = this;
            }

            s_Instances[m_ID].Root = this;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void RemoveFromChain() noexcept
    {
        if (VALID_ENTITY(m_ID))
        {
            if (m_Next)
            {
                m_Next->m_Prev = m_Prev;
            }

            if (m_Prev)
            {
                m_Prev->m_Next = m_Next;
            }

            if (s_Instances[m_ID].Root == this)
            {
                s_Instances[m_ID].Root = (m_Next ? m_Next : (m_Prev ? m_Prev : 0));
            }

            m_Next = 0;
            m_Prev = 0;
        }
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Verify that the specified entity instacne is active
    */
    static bool Verify(SQInt32 id) noexcept
    {
        return (VALID_ENTITYEX(id, Max) && VALID_ENTITY(s_Instances[id].ID));
    }

public:

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Reference() noexcept
        : Reference(SQMOD_UNKNOWN)
    {

    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Reference(SQInt32 id) noexcept
        : m_ID(Verify(id) ? id : SQMOD_UNKNOWN)
        , m_Tag()
        , m_Data()
        , m_Prev(0)
        , m_Next(0)
        , m_Persistent(false)
    {
        InsertIntoChain();
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Reference(const Reference< T > & r) noexcept
        : m_ID(r.m_ID)
        , m_Tag(r.m_Tag)
        , m_Data(r.m_Data)
        , m_Prev(0)
        , m_Next(0)
        , m_Persistent(r.m_Persistent)
    {
        InsertIntoChain();
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Reference(Reference< T > && r) noexcept
        : m_ID(r.m_ID)
        , m_Tag(r.m_Tag)
        , m_Data(r.m_Data)
        , m_Prev(0)
        , m_Next(0)
        , m_Persistent(r.m_Persistent)
    {
        InsertIntoChain();
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    ~Reference()
    {
        RemoveFromChain();
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Reference< T > & operator = (const Reference< T > & r) noexcept
    {
        if (this != &r)
        {
            RemoveFromChain();

            m_ID = r.m_ID;
            m_Tag = r.m_Tag;
            m_Data = r.m_Data;
            m_Persistent = r.m_Persistent;

            InsertIntoChain();
        }

        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Reference< T > & operator = (Reference< T > &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    operator SQInt32 () const noexcept
    {
        return m_ID;
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    operator SQUint32 () const noexcept
    {
        return static_cast< SQUint32 >(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    operator bool () const noexcept
    {
        return VALID_ENTITYEX(m_ID, Max);
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    bool operator ! () const noexcept
    {
        return INVALID_ENTITYEX(m_ID, Max);
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInteger Cmp(const Reference< T > & r) const noexcept
    {
        return m_ID == r.m_ID ? 0 : (m_ID > r.m_ID ? 1 : -1);
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    const SQChar * ToString() const noexcept
    {
        return ToStringF("%d", m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQInteger GetID() const noexcept
    {
        return m_ID;
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetID(SQInt32 id) noexcept
    {
        if (id != m_ID)
        {
            RemoveFromChain();
            m_ID = id;
            InsertIntoChain();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    bool GetPersistent() const noexcept
    {
        return m_Persistent;
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetPersistent(bool toggle) noexcept
    {
        m_Persistent = toggle;
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    const SQChar * GetGlobalTag() const noexcept
    {
        if (VALID_ENTITYEX(m_ID, Max))
        {
            return s_Instances[m_ID].Tag.c_str();
        }
        else
        {
            LogWrn(_SC("Attempting to <get global tag> using an invalid reference: %d"), m_ID);
        }

        return _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetGlobalTag(const SQChar * tag) const noexcept
    {
        if (VALID_ENTITYEX(m_ID, Max))
        {
            s_Instances[m_ID].Tag.assign(tag);
        }
        else
        {
            LogWrn(_SC("Attempting to <set global tag> using an invalid reference: %d"), m_ID);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SqObj & GetGlobalData() noexcept
    {
        if (VALID_ENTITYEX(m_ID, Max))
        {
            return s_Instances[m_ID].Data;
        }
        else
        {
            LogWrn(_SC("Attempting to <get global data> using an invalid reference: %d"), m_ID);
        }

        return NullData();
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetGlobalData(SqObj & data) const noexcept
    {
        if (VALID_ENTITYEX(m_ID, Max))
        {
            s_Instances[m_ID].Data = data;
        }
        else
        {
            LogWrn(_SC("Attempting to <set global tag> using an invalid reference: %d"), m_ID);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    const SQChar * GetLocalTag() const noexcept
    {
        return m_Tag.c_str();
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetLocalTag(const SQChar * tag) noexcept
    {
        m_Tag.assign(tag);
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SqObj & GetLocalData() noexcept
    {
        return m_Data;
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetLocalData(SqObj & data) noexcept
    {
        m_Data = data;
    }

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    SQUint32 GetMax() const noexcept
    {
        return Max;
    }

    /* --------------------------------------------------------------------------------------------
     * Return a new reference from the base reference
    */
    T GetReference() const noexcept
    {
        return T(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Returns whether this entity reference points to an active entity
    */
    bool IsActive() const noexcept
    {
        return VALID_ENTITYEX(m_ID, Max);
    }

    /* --------------------------------------------------------------------------------------------
     * Counts the number of active references for this entity
    */
    SQUint32 CountRefs() const noexcept
    {
        SQUint32 refs = 0;
        // Make sure the reference is valid
        if (VALID_ENTITYEX(m_ID, Max))
        {
            // Count this instance
            ++refs;
            // Count backward references
            for (RefType * ref = m_Prev; ref; ref = ref->m_Prev)
            {
                ++refs;
            }
            // Count forward references
            for (RefType * ref = m_Next; ref; ref = ref->m_Next)
            {
                ++refs;
            }
        }
        return refs;
    }

    /* --------------------------------------------------------------------------------------------
     * Counts the number of persistent references for this entity
    */    SQUint32 CountPersistentRefs() const noexcept
    {
        SQUint32 refs = 0;
        // Make sure the reference is valid
        if (VALID_ENTITYEX(m_ID, Max))
        {
            // Count this instance if persistent
            if (m_Persistent) ++refs;
            // Count backward references
            for (RefType * ref = m_Prev; ref; ref = ref->m_Prev)
            {
                if (ref->m_Persistent)
                {
                    ++refs;
                }
            }
            // Count forward references
            for (RefType * ref = m_Next; ref; ref = ref->m_Next)
            {
                if (ref->m_Persistent)
                {
                    ++refs;
                }
            }
        }
        return refs;
    }

};

// ------------------------------------------------------------------------------------------------
template < class T > typename Ent< T >::Instances Reference< T >::s_Instances;

/* ------------------------------------------------------------------------------------------------
 * Utility meant to reduce the code duplication for entity management required by the Core class
*/

template < class T > class EntMan
{
    // --------------------------------------------------------------------------------------------
    friend class Core;

public:

    // --------------------------------------------------------------------------------------------
    typedef Reference< T >  RefType;
    typedef Ent< T >        EntType;

private:

    // --------------------------------------------------------------------------------------------
    static RefType    NullRef;

    /* --------------------------------------------------------------------------------------------
     * Deactivates the specified entity instance
    */
    static bool Deactivate(SQInt32 id, SqObj & payload) noexcept
    {
        // Make sure this entity even exists
        if (RefType::Verify(id))
        {
            RefType * ref = 0, bkp = 0;

            ref = RefType::s_Instances[id].Root->m_Prev;

            // Deactivate backward references
            while (ref)
            {
                if (ref->m_Persistent)
                {
                    // Just disable the entity if persistent
                    ref->m_ID = SQMOD_UNKNOWN;
                    // Move to the previous reference
                    ref = ref->m_Prev;
                }
                else
                {
                    // Backup the current reference
                    bkp = ref;
                    // Move to the previous reference before unchaining
                    ref = ref->m_Prev;
                    // Now it's safe to unchain the reference
                    bkp->RemoveFromChain();
                }
            }

            ref = RefType::s_Instances[id].Root->m_Next;

            // Deactivate forward references
            while (ref)
            {
                if (ref->m_Persistent)
                {
                    // Just disable the entity if persistent
                    ref->m_ID = SQMOD_UNKNOWN;
                    // Move to the next reference
                    ref = ref->m_Next;
                }
                else
                {
                    // Backup the current reference
                    bkp = ref;
                    // Move to the next reference before unchaining
                    ref = ref->m_Next;
                    // Now it's safe to unchain the reference
                    bkp->RemoveFromChain();
                }
            }

            // Disable this entity instance
            RefType::s_Instances[id].ID = SQMOD_UNKNOWN;

            // Clear any events that are still listening
            EntType::Clear(RefType::s_Instances[id]);

            // Sucessfully deactivated the instance
            return true;
        }
        else
        {
            LogErr("Cannot deactivate an already deactivated <%s> instance with identifier (%d)", EntType::Name, id);
        }
        // Failed to destroy the entity
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Activates the specified entity instance
    */
    template < typename... Args > static bool Activate(SQInt32 id, bool owned, Args&&... args) noexcept
    {
        // Validate the specified entity identifier
        if (INVALID_ENTITYEX(id, EntType::Limit))
        {
            LogErr("Cannot initialize <%s> instance (%d) with invalid identifier", EntType::Name, id);
        }
        // Make sure this entity instance isn't already activated
        else if (!RefType::Verify(id))
        {
            // See if there's any persistent references to activate
            if (RefType::s_Instances[id].Root)
            {
                // Resurect backward references
                for (RefType * ref = RefType::s_Instances[id].Root->m_Prev; ref; ref = ref->m_Prev)
                {
                    // Unnecessary check, but just to be sure
                    if (ref->m_Persistent)
                    {
                        ref->m_ID = id;
                    }
                }
                // Resurect forward references
                for (RefType * ref = RefType::s_Instances[id].Root->m_Next; ref; ref = ref->m_Next)
                {
                    // Unnecessary check, but just to be sure
                    if (ref->m_Persistent)
                    {
                        ref->m_ID = id;
                    }
                }
            }

            // Enable this entity instance
            RefType::s_Instances[id].ID = id;
            // Set the instance ownership
            RefType::s_Instances[id].Owned = owned;

            // See if the user data needs purging
            if (RefType::s_Instances[id].Fresh)
            {
                RefType::s_Instances[id].Tag.clear();
                RefType::s_Instances[id].Data.Release();
            }

            // Store any specified values on the instance
            EntType::Store(RefType::s_Instances[id], std::forward< Args >(args)...);

            // Initialization successful
            return true;
        }
        else
        {
            LogErr("Cannot activate an already activated <%s> instance with identifier (%d)", EntType::Name, id);
        }
        // Failed to activate the entity
        return false;
    }
};

// ------------------------------------------------------------------------------------------------
template < class T > typename EntMan< T >::RefType EntMan< T >::NullRef;

// ------------------------------------------------------------------------------------------------
template < class T > bool Register_Reference(HSQUIRRELVM vm, const SQChar * cname)
{
    // Typedef the reference type to simplify code
    typedef Reference< T > Ref;
    // Output debugging information
    LogDbg("Beginning registration of <%s> type", cname);
    // Attempt to register the specified type
    Sqrat::RootTable(vm).Bind(cname, Sqrat::Class< Reference< T > >(vm, cname)
        .Ctor()
        .template Ctor< SQInt32 >()

        .Func(_SC("_cmp"), &Ref::Cmp)
        .Func(_SC("_tostring"), &Ref::ToString)

        .Prop(_SC("id"), &Ref::GetID, &Ref::SetID)
        .Prop(_SC("persistent"), &Ref::GetPersistent, &Ref::SetPersistent)
        .Prop(_SC("gtag"), &Ref::GetGlobalTag, &Ref::SetGlobalTag)
        .Prop(_SC("gdata"), &Ref::GetGlobalData, &Ref::SetGlobalData)
        .Prop(_SC("ltag"), &Ref::GetLocalTag, &Ref::SetLocalTag)
        .Prop(_SC("ldata"), &Ref::GetLocalData, &Ref::SetLocalData)

        .Prop(_SC("max"), &Ref::GetMax)
        .Prop(_SC("sref"), &Ref::GetReference)
        .Prop(_SC("active"), &Ref::IsActive)
        .Prop(_SC("refs"), &Ref::CountRefs)
        .Prop(_SC("prefs"), &Ref::CountPersistentRefs)
    );
    // Output debugging information
    LogDbg("Registration of <%s> type was successful", cname);
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod

#endif // _ENTITY_HPP_
