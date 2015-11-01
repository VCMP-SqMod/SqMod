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
#include <bitset>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Helper enum to have a numeric way of identifying entity types.
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
 * Helper functions used by the entity interfaces to obtain global signals from the core instance.
*/
EBlipCreated & GBlipCreated() noexcept;
ECheckpointCreated & GCheckpointCreated() noexcept;
EKeybindCreated & GKeybindCreated() noexcept;
EObjectCreated & GObjectCreated() noexcept;
EPickupCreated & GPickupCreated() noexcept;
EPlayerCreated & GPlayerCreated() noexcept;
ESphereCreated & GSphereCreated() noexcept;
ESpriteCreated & GSpriteCreated() noexcept;
ETextdrawCreated & GTextdrawCreated() noexcept;
EVehicleCreated & GVehicleCreated() noexcept;

/* ------------------------------------------------------------------------------------------------
 * Helper functions used by the entity interfaces to obtain global signals from the core instance.
*/
EBlipDestroyed & GBlipDestroyed() noexcept;
ECheckpointDestroyed & GCheckpointDestroyed() noexcept;
EKeybindDestroyed & GKeybindDestroyed() noexcept;
EObjectDestroyed & GObjectDestroyed() noexcept;
EPickupDestroyed & GPickupDestroyed() noexcept;
EPlayerDestroyed & GPlayerDestroyed() noexcept;
ESphereDestroyed & GSphereDestroyed() noexcept;
ESpriteDestroyed & GSpriteDestroyed() noexcept;
ETextdrawDestroyed & GTextdrawDestroyed() noexcept;
EVehicleDestroyed & GVehicleDestroyed() noexcept;

/* ------------------------------------------------------------------------------------------------
 * Helper functions used by the entity interfaces to obtain global signals from the core instance.
*/
EBlipCustom & GBlipCustom() noexcept;
ECheckpointCustom & GCheckpointCustom() noexcept;
EKeybindCustom & GKeybindCustom() noexcept;
EObjectCustom & GObjectCustom() noexcept;
EPickupCustom & GPickupCustom() noexcept;
EPlayerCustom & GPlayerCustom() noexcept;
ESphereCustom & GSphereCustom() noexcept;
ESpriteCustom & GSpriteCustom() noexcept;
ETextdrawCustom & GTextdrawCustom() noexcept;
EVehicleCustom & GVehicleCustom() noexcept;

/* ------------------------------------------------------------------------------------------------
 * Forward declaration of an entity interface.
*/
template < class T > class Ent;

/* ------------------------------------------------------------------------------------------------
 * Specialized entity interface for the blip entity type.
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
     * Default destructor (disabled)
    */
    Ent() = delete;

    /* --------------------------------------------------------------------------------------------
     * Helper structure that holds information about each entity instance of this type.
    */
    typedef struct Blip
    {
        // ----------------------------------------------------------------------------------------
        Blip() noexcept
            : ID(-1), Root(0), Owned(false), Fresh(true)
        {
            /* ... */
        }

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

        // ----------------------------------------------------------------------------------------
        EBlipCreated & Created() noexcept
        {
            return BlipCreated;
        }

        EBlipDestroyed & Destroyed() noexcept
        {
            return BlipDestroyed;
        }

        EBlipCustom & Custom() noexcept
        {
            return BlipCustom;
        }

        // ----------------------------------------------------------------------------------------
        EBlipCreated & GCreated() noexcept
        {
            return GBlipCreated();
        }

        EBlipDestroyed & GDestroyed() noexcept
        {
            return GBlipDestroyed();
        }

        EBlipCustom & GCustom() noexcept
        {
            return GBlipCustom();
        }

    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst) noexcept
    {
        inst.World = SQMOD_UNKNOWN;
        inst.Scale = SQMOD_UNKNOWN;
        inst.SprID = SQMOD_UNKNOWN;
        inst.Position.Clear();
        inst.Color.Clear();
    }

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst, SQInt32 index, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 scale, SQUint32 color, SQInt32 sprid) noexcept
    {
        inst.World = world;
        inst.Scale = scale;
        inst.SprID = sprid;
        inst.Position.x = x;
        inst.Position.y = y;
        inst.Position.z = z;
        inst.Color.SetRGBA(color);
        SQMOD_UNUSED_VAR(index);
    }

    // --------------------------------------------------------------------------------------------
    static void Clear(Instance & inst) noexcept
    {
        inst.BlipCreated.Clear();
        inst.BlipDestroyed.Clear();
        inst.BlipCustom.Clear();
    }

    // --------------------------------------------------------------------------------------------
    static SQInt32 Create(SQInt32 index, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                            SQInt32 scale, SQUint32 color, SQInt32 sprid) noexcept
    {
        return _Func->CreateCoordBlip(index, world, x, y, z, scale, color, sprid);
    }

    // --------------------------------------------------------------------------------------------
    static void Destroy(SQInt32 id) noexcept
    {
        _Func->DestroyCoordBlip(id);
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Blip";
    static constexpr const SQChar* CName    = "CBlip";

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID         = ENT_BLIP;
    static constexpr SQInt32 Limit          = SQMOD_BLIP_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 CreateEvID     = EVT_BLIPCREATED;
    static constexpr SQInt32 DestroyEvID    = EVT_BLIPDESTROYED;
    static constexpr SQInt32 CustomEvID     = EVT_BLIPCUSTOM;

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit >   Instances;

    // --------------------------------------------------------------------------------------------
    typedef std::bitset< Limit >            Set;

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type) noexcept
    {
        switch (type)
        {
            case EVT_BLIPCREATED:
            case EVT_BLIPDESTROYED:
            case EVT_BLIPCUSTOM:
                return true;
            default:
                return false;
        }
    }
    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type, bool /* inversed */) noexcept
    {
        switch (type)
        {
            case EVT_BLIPCREATED:
            case EVT_BLIPDESTROYED:
            case EVT_BLIPCUSTOM:
                return true;
            default:
                return false;
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialized entity interface for the checkpoint entity type.
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
     * Default destructor (disabled)
    */
    Ent() = delete;

    /* --------------------------------------------------------------------------------------------
     * Helper structure that holds information about each entity instance of this type.
    */
    typedef struct Checkpoint
    {
        // ----------------------------------------------------------------------------------------
        Checkpoint() noexcept
            : ID(-1), Root(0), Owned(false), Fresh(true)
        {
            /* ... */
        }

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

        // ----------------------------------------------------------------------------------------
        ECheckpointCreated & Created() noexcept
        {
            return CheckpointCreated;
        }

        ECheckpointDestroyed & Destroyed() noexcept
        {
            return CheckpointDestroyed;
        }

        ECheckpointCustom & Custom() noexcept
        {
            return CheckpointCustom;
        }

        // ----------------------------------------------------------------------------------------
        ECheckpointCreated & GCreated() noexcept
        {
            return GCheckpointCreated();
        }

        ECheckpointDestroyed & GDestroyed() noexcept
        {
            return GCheckpointDestroyed();
        }

        ECheckpointCustom & GCustom() noexcept
        {
            return GCheckpointCustom();
        }

    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst) noexcept
    {
        SQMOD_UNUSED_VAR(inst);
    }

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst, SQInt32 player, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                        SQUint32 r, SQUint32 g, SQUint32 b, SQUint32 a, SQFloat radius) noexcept
    {
        SQMOD_UNUSED_VAR(inst);
        SQMOD_UNUSED_VAR(player);
        SQMOD_UNUSED_VAR(world);
        SQMOD_UNUSED_VAR(x);
        SQMOD_UNUSED_VAR(y);
        SQMOD_UNUSED_VAR(z);
        SQMOD_UNUSED_VAR(r);
        SQMOD_UNUSED_VAR(g);
        SQMOD_UNUSED_VAR(b);
        SQMOD_UNUSED_VAR(a);
        SQMOD_UNUSED_VAR(radius);
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

    // --------------------------------------------------------------------------------------------
    static SQInt32 Create(SQInt32 player, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                            SQUint32 r, SQUint32 g, SQUint32 b, SQUint32 a, SQFloat radius) noexcept
    {
        return _Func->CreateCheckpoint(player, world, x, y, z, r, g, b, a, radius);
    }

    // --------------------------------------------------------------------------------------------
    static void Destroy(SQInt32 id) noexcept
    {
        _Func->DeleteCheckpoint(id);
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Checkpoint";
    static constexpr const SQChar* CName    = "CCheckpoint";

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID         = ENT_CHECKPOINT;
    static constexpr SQInt32 Limit          = SQMOD_CHECKPOINT_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 CreateEvID     = EVT_CHECKPOINTCREATED;
    static constexpr SQInt32 DestroyEvID    = EVT_CHECKPOINTDESTROYED;
    static constexpr SQInt32 CustomEvID     = EVT_CHECKPOINTCUSTOM;

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit >   Instances;

    // --------------------------------------------------------------------------------------------
    typedef std::bitset< Limit >            Set;

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type) noexcept
    {
        switch (type)
        {
            case EVT_CHECKPOINTCREATED:
            case EVT_CHECKPOINTDESTROYED:
            case EVT_CHECKPOINTCUSTOM:
            case EVT_CHECKPOINTENTERED:
            case EVT_CHECKPOINTEXITED:
                return true;
            default:
                return false;
        }
    }

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type, bool inversed) noexcept
    {
        switch (type)
        {
            case EVT_CHECKPOINTCREATED:
            case EVT_CHECKPOINTDESTROYED:
            case EVT_CHECKPOINTCUSTOM:
                return true;
            case EVT_CHECKPOINTENTERED:
            case EVT_CHECKPOINTEXITED:
                return !inversed;
            default:
                return false;
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialized entity interface for the keybind entity type.
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
     * Default destructor (disabled)
    */
    Ent() = delete;

    /* --------------------------------------------------------------------------------------------
     * Helper structure that holds information about each entity instance of this type.
    */
    typedef struct Keybind
    {
        // ----------------------------------------------------------------------------------------
        Keybind() noexcept
            : ID(-1), Root(0), Owned(false), Fresh(true)
        {
            /* ... */
        }

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

        // ----------------------------------------------------------------------------------------
        EKeybindCreated & Created() noexcept
        {
            return KeybindCreated;
        }

        EKeybindDestroyed & Destroyed() noexcept
        {
            return KeybindDestroyed;
        }

        EKeybindCustom & Custom() noexcept
        {
            return KeybindCustom;
        }

        // ----------------------------------------------------------------------------------------
        EKeybindCreated & GCreated() noexcept
        {
            return GKeybindCreated();
        }

        EKeybindDestroyed & GDestroyed() noexcept
        {
            return GKeybindDestroyed();
        }

        EKeybindCustom & GCustom() noexcept
        {
            return GKeybindCustom();
        }

    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst) noexcept
    {
        inst.Primary = SQMOD_UNKNOWN;
        inst.Secondary = SQMOD_UNKNOWN;
        inst.Alternative = SQMOD_UNKNOWN;
        inst.Release = false;
    }

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst, SQInt32 slot, bool release, SQInt32 primary, SQInt32 secondary,
                        SQInt32 alternative) noexcept
    {
        inst.Primary = primary;
        inst.Secondary = secondary;
        inst.Alternative = alternative;
        inst.Release = release;
        SQMOD_UNUSED_VAR(slot);
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

    // --------------------------------------------------------------------------------------------
    static SQInt32 Create(SQInt32 slot, bool release, SQInt32 primary, SQInt32 secondary,
                            SQInt32 alternative) noexcept
    {
        return _Func->RegisterKeyBind(slot, release, primary, secondary, alternative);
    }

    // --------------------------------------------------------------------------------------------
    static void Destroy(SQInt32 id) noexcept
    {
        _Func->RemoveKeyBind(id);
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Keybind";
    static constexpr const SQChar* CName    = "CKeybind";

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID         = ENT_KEYBIND;
    static constexpr SQInt32 Limit          = SQMOD_KEYBIND_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 CreateEvID     = EVT_KEYBINDCREATED;
    static constexpr SQInt32 DestroyEvID    = EVT_KEYBINDDESTROYED;
    static constexpr SQInt32 CustomEvID     = EVT_KEYBINDCUSTOM;

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit >   Instances;

    // --------------------------------------------------------------------------------------------
    typedef std::bitset< Limit >            Set;

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type) noexcept
    {
        switch (type)
        {
            case EVT_KEYBINDCREATED:
            case EVT_KEYBINDDESTROYED:
            case EVT_KEYBINDCUSTOM:
            case EVT_KEYBINDKEYPRESS:
            case EVT_KEYBINDKEYRELEASE:
                return true;
            default:
                return false;
        }
    }

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type, bool inversed) noexcept
    {
        switch (type)
        {
            case EVT_KEYBINDCREATED:
            case EVT_KEYBINDDESTROYED:
            case EVT_KEYBINDCUSTOM:
                return true;
            case EVT_KEYBINDKEYPRESS:
            case EVT_KEYBINDKEYRELEASE:
                return !inversed;
            default:
                return false;
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialized entity interface for the object entity type.
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
     * Default destructor (disabled)
    */
    Ent() = delete;

    /* --------------------------------------------------------------------------------------------
     * Helper structure that holds information about each entity instance of this type.
    */
    typedef struct Object
    {
        // ----------------------------------------------------------------------------------------
        Object() noexcept
            : ID(-1), Root(0), Owned(false), Fresh(true)
        {
            /* ... */
        }

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

        // ----------------------------------------------------------------------------------------
        EObjectCreated & Created() noexcept
        {
            return ObjectCreated;
        }

        EObjectDestroyed & Destroyed() noexcept
        {
            return ObjectDestroyed;
        }

        EObjectCustom & Custom() noexcept
        {
            return ObjectCustom;
        }

        // ----------------------------------------------------------------------------------------
        EObjectCreated & GCreated() noexcept
        {
            return GObjectCreated();
        }

        EObjectDestroyed & GDestroyed() noexcept
        {
            return GObjectDestroyed();
        }

        EObjectCustom & GCustom() noexcept
        {
            return GObjectCustom();
        }

    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst) noexcept
    {
        SQMOD_UNUSED_VAR(inst);
    }

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst, SQInt32 model, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha) noexcept
    {
        SQMOD_UNUSED_VAR(inst);
        SQMOD_UNUSED_VAR(model);
        SQMOD_UNUSED_VAR(world);
        SQMOD_UNUSED_VAR(x);
        SQMOD_UNUSED_VAR(y);
        SQMOD_UNUSED_VAR(z);
        SQMOD_UNUSED_VAR(alpha);
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

    // --------------------------------------------------------------------------------------------
    static SQInt32 Create(SQInt32 model, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                            SQInt32 alpha) noexcept
    {
        return _Func->CreateObject(model, world, x, y, z, alpha);
    }

    // --------------------------------------------------------------------------------------------
    static void Destroy(SQInt32 id) noexcept
    {
        _Func->DeleteObject(id);
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Object";
    static constexpr const SQChar* CName    = "CObject";

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID         = ENT_OBJECT;
    static constexpr SQInt32 Limit          = SQMOD_OBJECT_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 CreateEvID     = EVT_OBJECTCREATED;
    static constexpr SQInt32 DestroyEvID    = EVT_OBJECTDESTROYED;
    static constexpr SQInt32 CustomEvID     = EVT_OBJECTCUSTOM;

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit >   Instances;

    // --------------------------------------------------------------------------------------------
    typedef std::bitset< Limit >            Set;

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type) noexcept
    {
        switch (type)
        {
            case EVT_OBJECTCREATED:
            case EVT_OBJECTDESTROYED:
            case EVT_OBJECTCUSTOM:
            case EVT_OBJECTSHOT:
            case EVT_OBJECTBUMP:
                return true;
            default:
                return false;
        }
    }

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type, bool inversed) noexcept
    {
        switch (type)
        {
            case EVT_OBJECTCREATED:
            case EVT_OBJECTDESTROYED:
            case EVT_OBJECTCUSTOM:
                return true;
            case EVT_OBJECTSHOT:
            case EVT_OBJECTBUMP:
                return !inversed;
            default:
                return false;
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialized entity interface for the pickup entity type.
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
     * Default destructor (disabled)
    */
    Ent() = delete;

    /* --------------------------------------------------------------------------------------------
     * Helper structure that holds information about each entity instance of this type.
    */
    typedef struct Pickup
    {
        // ----------------------------------------------------------------------------------------
        Pickup() noexcept
            : ID(-1), Root(0), Owned(false), Fresh(true)
        {
            /* ... */
        }

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

        // ----------------------------------------------------------------------------------------
        EPickupCreated & Created() noexcept
        {
            return PickupCreated;
        }

        EPickupDestroyed & Destroyed() noexcept
        {
            return PickupDestroyed;
        }

        EPickupCustom & Custom() noexcept
        {
            return PickupCustom;
        }

        // ----------------------------------------------------------------------------------------
        EPickupCreated & GCreated() noexcept
        {
            return GPickupCreated();
        }

        EPickupDestroyed & GDestroyed() noexcept
        {
            return GPickupDestroyed();
        }

        EPickupCustom & GCustom() noexcept
        {
            return GPickupCustom();
        }

    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst) noexcept
    {
        SQMOD_UNUSED_VAR(inst);
    }

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst, SQInt32 model, SQInt32 world, SQInt32 quantity,
                        SQFloat x,  SQFloat y,  SQFloat z, SQInt32 alpha, bool automatic) noexcept
    {
        SQMOD_UNUSED_VAR(inst);
        SQMOD_UNUSED_VAR(model);
        SQMOD_UNUSED_VAR(world);
        SQMOD_UNUSED_VAR(quantity);
        SQMOD_UNUSED_VAR(x);
        SQMOD_UNUSED_VAR(y);
        SQMOD_UNUSED_VAR(z);
        SQMOD_UNUSED_VAR(alpha);
        SQMOD_UNUSED_VAR(automatic);
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

    // --------------------------------------------------------------------------------------------
    static SQInt32 Create(SQInt32 model, SQInt32 world, SQInt32 quantity,
                            SQFloat x,  SQFloat y,  SQFloat z, SQInt32 alpha, bool automatic) noexcept
    {
        return _Func->CreatePickup(model, world, quantity, x, y, z, alpha, automatic);
    }

    // --------------------------------------------------------------------------------------------
    static void Destroy(SQInt32 id) noexcept
    {
        _Func->DeletePickup(id);
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Pickup";
    static constexpr const SQChar* CName    = "CPickup";

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID         = ENT_PICKUP;
    static constexpr SQInt32 Limit          = SQMOD_PICKUP_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 CreateEvID     = EVT_PICKUPCREATED;
    static constexpr SQInt32 DestroyEvID    = EVT_PICKUPDESTROYED;
    static constexpr SQInt32 CustomEvID     = EVT_PICKUPCUSTOM;

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit >   Instances;

    // --------------------------------------------------------------------------------------------
    typedef std::bitset< Limit >            Set;

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type) noexcept
    {
        switch (type)
        {
            case EVT_PICKUPCREATED:
            case EVT_PICKUPDESTROYED:
            case EVT_PICKUPCUSTOM:
            case EVT_PICKUPRESPAWN:
            case EVT_PICKUPCLAIMED:
            case EVT_PICKUPCOLLECTED:
                return true;
            default:
                return false;
        }
    }

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type, bool inversed) noexcept
    {
        switch (type)
        {
            case EVT_PICKUPCREATED:
            case EVT_PICKUPDESTROYED:
            case EVT_PICKUPCUSTOM:
            case EVT_PICKUPRESPAWN:
                return true;
            case EVT_PICKUPCLAIMED:
            case EVT_PICKUPCOLLECTED:
                return !inversed;
            default:
                return false;
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialized entity interface for the player entity type.
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
     * Default destructor (disabled)
    */
    Ent() = delete;

    /* --------------------------------------------------------------------------------------------
     * Helper structure that holds information about each entity instance of this type.
    */
    typedef struct Player
    {
        // ----------------------------------------------------------------------------------------
        Player() noexcept
            : ID(-1), Root(0), Owned(false), Fresh(true)
        {
            /* ... */
        }

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
        EKeybindKeyPress        KeybindKeyPress;
        EKeybindKeyRelease      KeybindKeyRelease;
        EVehicleEmbarking       VehicleEmbarking;
        EVehicleEmbarked        VehicleEmbarked;
        EVehicleDisembark       VehicleDisembark;
        EPickupClaimed          PickupClaimed;
        EPickupCollected        PickupCollected;
        EObjectShot             ObjectShot;
        EObjectBump             ObjectBump;
        ECheckpointEntered      CheckpointEntered;
        ECheckpointExited       CheckpointExited;
        ESphereEntered          SphereEntered;
        ESphereExited           SphereExited;

        // ----------------------------------------------------------------------------------------
        EPlayerCreated & Created() noexcept
        {
            return PlayerCreated;
        }

        EPlayerDestroyed & Destroyed() noexcept
        {
            return PlayerDestroyed;
        }

        EPlayerCustom & Custom() noexcept
        {
            return PlayerCustom;
        }

        // ----------------------------------------------------------------------------------------
        EPlayerCreated & GCreated() noexcept
        {
            return GPlayerCreated();
        }

        EPlayerDestroyed & GDestroyed() noexcept
        {
            return GPlayerDestroyed();
        }

        EPlayerCustom & GCustom() noexcept
        {
            return GPlayerCustom();
        }

    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst) noexcept
    {
        SQMOD_UNUSED_VAR(inst);
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
        inst.KeybindKeyPress.Clear();
        inst.KeybindKeyRelease.Clear();
        inst.VehicleEmbarking.Clear();
        inst.VehicleEmbarked.Clear();
        inst.VehicleDisembark.Clear();
        inst.PickupClaimed.Clear();
        inst.PickupCollected.Clear();
        inst.ObjectShot.Clear();
        inst.ObjectBump.Clear();
        inst.CheckpointEntered.Clear();
        inst.CheckpointExited.Clear();
        inst.SphereEntered.Clear();
        inst.SphereExited.Clear();
    }

    // --------------------------------------------------------------------------------------------
    static SQInt32 Create() noexcept
    {
        return SQMOD_UNKNOWN;
    }

    // --------------------------------------------------------------------------------------------
    static void Destroy(SQInt32 id) noexcept
    {
        /* @TODO: Implement as kick. */
        SQMOD_UNUSED_VAR(id);
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Player";
    static constexpr const SQChar* CName    = "CPlayer";

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID         = ENT_PLAYER;
    static constexpr SQInt32 Limit          = SQMOD_PLAYER_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 CreateEvID     = EVT_PLAYERCREATED;
    static constexpr SQInt32 DestroyEvID    = EVT_PLAYERDESTROYED;
    static constexpr SQInt32 CustomEvID     = EVT_PLAYERCUSTOM;

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit >   Instances;

    // --------------------------------------------------------------------------------------------
    typedef std::bitset< Limit >            Set;

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type) noexcept
    {
        switch (type)
        {
            case EVT_PLAYERCREATED:
            case EVT_PLAYERDESTROYED:
            case EVT_PLAYERCUSTOM:
            case EVT_PLAYERAWAY:
            case EVT_PLAYERGAMEKEYS:
            case EVT_PLAYERRENAME:
            case EVT_PLAYERREQUESTCLASS:
            case EVT_PLAYERREQUESTSPAWN:
            case EVT_PLAYERSPAWN:
            case EVT_PLAYERSTARTTYPING:
            case EVT_PLAYERSTOPTYPING:
            case EVT_PLAYERCHAT:
            case EVT_PLAYERCOMMAND:
            case EVT_PLAYERMESSAGE:
            case EVT_PLAYERHEALTH:
            case EVT_PLAYERARMOUR:
            case EVT_PLAYERWEAPON:
            case EVT_PLAYERMOVE:
            case EVT_PLAYERWASTED:
            case EVT_PLAYERKILLED:
            case EVT_PLAYERTEAMKILL:
            case EVT_PLAYERSPECTATE:
            case EVT_PLAYERCRASHREPORT:
            case EVT_PLAYERBURNING:
            case EVT_PLAYERCROUCHING:
            case EVT_PLAYERSTATE:
            case EVT_PLAYERACTION:
            case EVT_STATENONE:
            case EVT_STATENORMAL:
            case EVT_STATESHOOTING:
            case EVT_STATEDRIVER:
            case EVT_STATEPASSENGER:
            case EVT_STATEENTERDRIVER:
            case EVT_STATEENTERPASSENGER:
            case EVT_STATEEXITVEHICLE:
            case EVT_STATEUNSPAWNED:
            case EVT_ACTIONNONE:
            case EVT_ACTIONNORMAL:
            case EVT_ACTIONAIMING:
            case EVT_ACTIONSHOOTING:
            case EVT_ACTIONJUMPING:
            case EVT_ACTIONLIEDOWN:
            case EVT_ACTIONGETTINGUP:
            case EVT_ACTIONJUMPVEHICLE:
            case EVT_ACTIONDRIVING:
            case EVT_ACTIONDYING:
            case EVT_ACTIONWASTED:
            case EVT_ACTIONEMBARKING:
            case EVT_ACTIONDISEMBARKING:
            case EVT_KEYBINDKEYPRESS:
            case EVT_KEYBINDKEYRELEASE:
            case EVT_VEHICLEEMBARKING:
            case EVT_VEHICLEEMBARKED:
            case EVT_VEHICLEDISEMBARK:
            case EVT_PICKUPCLAIMED:
            case EVT_PICKUPCOLLECTED:
            case EVT_OBJECTSHOT:
            case EVT_OBJECTBUMP:
            case EVT_CHECKPOINTENTERED:
            case EVT_CHECKPOINTEXITED:
            case EVT_SPHEREENTERED:
            case EVT_SPHEREEXITED:
                return true;
            default:
                return false;
        }
    }

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type, bool inversed) noexcept
    {
        switch (type)
        {
            case EVT_PLAYERCREATED:
            case EVT_PLAYERDESTROYED:
            case EVT_PLAYERCUSTOM:
            case EVT_PLAYERAWAY:
            case EVT_PLAYERGAMEKEYS:
            case EVT_PLAYERRENAME:
            case EVT_PLAYERREQUESTCLASS:
            case EVT_PLAYERREQUESTSPAWN:
            case EVT_PLAYERSPAWN:
            case EVT_PLAYERSTARTTYPING:
            case EVT_PLAYERSTOPTYPING:
            case EVT_PLAYERCHAT:
            case EVT_PLAYERCOMMAND:
            case EVT_PLAYERMESSAGE:
            case EVT_PLAYERHEALTH:
            case EVT_PLAYERARMOUR:
            case EVT_PLAYERWEAPON:
            case EVT_PLAYERMOVE:
            case EVT_PLAYERWASTED:
            case EVT_PLAYERKILLED:
            case EVT_PLAYERTEAMKILL:
            case EVT_PLAYERSPECTATE:
            case EVT_PLAYERCRASHREPORT:
            case EVT_PLAYERBURNING:
            case EVT_PLAYERCROUCHING:
            case EVT_PLAYERSTATE:
            case EVT_PLAYERACTION:
            case EVT_STATENONE:
            case EVT_STATENORMAL:
            case EVT_STATESHOOTING:
            case EVT_STATEDRIVER:
            case EVT_STATEPASSENGER:
            case EVT_STATEENTERDRIVER:
            case EVT_STATEENTERPASSENGER:
            case EVT_STATEEXITVEHICLE:
            case EVT_STATEUNSPAWNED:
            case EVT_ACTIONNONE:
            case EVT_ACTIONNORMAL:
            case EVT_ACTIONAIMING:
            case EVT_ACTIONSHOOTING:
            case EVT_ACTIONJUMPING:
            case EVT_ACTIONLIEDOWN:
            case EVT_ACTIONGETTINGUP:
            case EVT_ACTIONJUMPVEHICLE:
            case EVT_ACTIONDRIVING:
            case EVT_ACTIONDYING:
            case EVT_ACTIONWASTED:
            case EVT_ACTIONEMBARKING:
            case EVT_ACTIONDISEMBARKING:
                return true;
            case EVT_KEYBINDKEYPRESS:
            case EVT_KEYBINDKEYRELEASE:
            case EVT_VEHICLEEMBARKING:
            case EVT_VEHICLEEMBARKED:
            case EVT_VEHICLEDISEMBARK:
            case EVT_PICKUPCLAIMED:
            case EVT_PICKUPCOLLECTED:
            case EVT_OBJECTSHOT:
            case EVT_OBJECTBUMP:
            case EVT_CHECKPOINTENTERED:
            case EVT_CHECKPOINTEXITED:
            case EVT_SPHEREENTERED:
            case EVT_SPHEREEXITED:
                return inversed;
            default:
                return false;
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialized entity interface for the sphere entity type.
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
     * Default destructor (disabled)
    */
    Ent() = delete;

    /* --------------------------------------------------------------------------------------------
     * Helper structure that holds information about each entity instance of this type.
    */
    typedef struct Sphere
    {
        // ----------------------------------------------------------------------------------------
        Sphere() noexcept
            : ID(-1), Root(0), Owned(false), Fresh(true)
        {
            /* ... */
        }

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

        // ----------------------------------------------------------------------------------------
        ESphereCreated & Created() noexcept
        {
            return SphereCreated;
        }

        ESphereDestroyed & Destroyed() noexcept
        {
            return SphereDestroyed;
        }

        ESphereCustom & Custom() noexcept
        {
            return SphereCustom;
        }

        // ----------------------------------------------------------------------------------------
        ESphereCreated & GCreated() noexcept
        {
            return GSphereCreated();
        }

        ESphereDestroyed & GDestroyed() noexcept
        {
            return GSphereDestroyed();
        }

        ESphereCustom & GCustom() noexcept
        {
            return GSphereCustom();
        }

    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst) noexcept
    {
        SQMOD_UNUSED_VAR(inst);
    }

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst, SQInt32 player, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                        SQUint32 r, SQUint32 g, SQUint32 b, SQFloat radius) noexcept
    {
        SQMOD_UNUSED_VAR(inst);
        SQMOD_UNUSED_VAR(player);
        SQMOD_UNUSED_VAR(world);
        SQMOD_UNUSED_VAR(x);
        SQMOD_UNUSED_VAR(y);
        SQMOD_UNUSED_VAR(z);
        SQMOD_UNUSED_VAR(r);
        SQMOD_UNUSED_VAR(g);
        SQMOD_UNUSED_VAR(b);
        SQMOD_UNUSED_VAR(radius);
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

    // --------------------------------------------------------------------------------------------
    static SQInt32 Create(SQInt32 player, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                            SQUint32 r, SQUint32 g, SQUint32 b, SQFloat radius) noexcept
    {
        return _Func->CreateSphere(_SCI32(player), world, x, y, z, r, g, b, radius);
    }

    // --------------------------------------------------------------------------------------------
    static void Destroy(SQInt32 id) noexcept
    {
        _Func->DeleteSphere(id);
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Sphere";
    static constexpr const SQChar* CName    = "CSphere";

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID         = ENT_SPHERE;
    static constexpr SQInt32 Limit          = SQMOD_SPHERE_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 CreateEvID     = EVT_SPHERECREATED;
    static constexpr SQInt32 DestroyEvID    = EVT_SPHEREDESTROYED;
    static constexpr SQInt32 CustomEvID     = EVT_SPHERECUSTOM;

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit >   Instances;

    // --------------------------------------------------------------------------------------------
    typedef std::bitset< Limit >            Set;

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type) noexcept
    {
        switch (type)
        {
            case EVT_SPHERECREATED:
            case EVT_SPHEREDESTROYED:
            case EVT_SPHERECUSTOM:
            case EVT_SPHEREENTERED:
            case EVT_SPHEREEXITED:
                return true;
            default:
                return false;
        }
    }

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type, bool inversed) noexcept
    {
        switch (type)
        {
            case EVT_SPHERECREATED:
            case EVT_SPHEREDESTROYED:
            case EVT_SPHERECUSTOM:
                return true;
            case EVT_SPHEREENTERED:
            case EVT_SPHEREEXITED:
                return !inversed;
            default:
                return false;
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialized entity interface for the sprite entity type.
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
     * Default destructor (disabled)
    */
    Ent() = delete;

    /* --------------------------------------------------------------------------------------------
     * Helper structure that holds information about each entity instance of this type.
    */
    typedef struct Sprite
    {
        // ----------------------------------------------------------------------------------------
        Sprite() noexcept
            : ID(-1), Root(0), Owned(false), Fresh(true)
        {
            /* ... */
        }

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

        // ----------------------------------------------------------------------------------------
        ESpriteCreated & Created() noexcept
        {
            return SpriteCreated;
        }

        ESpriteDestroyed & Destroyed() noexcept
        {
            return SpriteDestroyed;
        }

        ESpriteCustom & Custom() noexcept
        {
            return SpriteCustom;
        }

        // ----------------------------------------------------------------------------------------
        ESpriteCreated & GCreated() noexcept
        {
            return GSpriteCreated();
        }

        ESpriteDestroyed & GDestroyed() noexcept
        {
            return GSpriteDestroyed();
        }

        ESpriteCustom & GCustom() noexcept
        {
            return GSpriteCustom();
        }

    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst) noexcept
    {
        inst.Path.clear();
    }

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst, SQInt32 index, const SQChar * file, SQInt32 xp, SQInt32 yp,
                        SQInt32 xr, SQInt32 yr, SQFloat angle, SQInt32 alpha, bool rel) noexcept
    {
        inst.Path.assign(file);
        SQMOD_UNUSED_VAR(index);
        SQMOD_UNUSED_VAR(xp);
        SQMOD_UNUSED_VAR(yp);
        SQMOD_UNUSED_VAR(xr);
        SQMOD_UNUSED_VAR(yr);
        SQMOD_UNUSED_VAR(angle);
        SQMOD_UNUSED_VAR(alpha);
        SQMOD_UNUSED_VAR(rel);
    }

    // --------------------------------------------------------------------------------------------
    static void Clear(Instance & inst) noexcept
    {
        inst.SpriteCreated.Clear();
        inst.SpriteDestroyed.Clear();
        inst.SpriteCustom.Clear();
    }

    // --------------------------------------------------------------------------------------------
    static SQInt32 Create(SQInt32 index, const SQChar * file, SQInt32 xp, SQInt32 yp,
                            SQInt32 xr, SQInt32 yr, SQFloat angle, SQInt32 alpha, bool rel) noexcept
    {
        return _Func->CreateSprite(index, file, xp, yp, xr, yr, angle, alpha, rel);
    }

    // --------------------------------------------------------------------------------------------
    static void Destroy(SQInt32 id) noexcept
    {
        _Func->DestroySprite(id);
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Sprite";
    static constexpr const SQChar* CName    = "CSprite";

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID         = ENT_SPRITE;
    static constexpr SQInt32 Limit          = SQMOD_SPRITE_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 CreateEvID     = EVT_SPRITECREATED;
    static constexpr SQInt32 DestroyEvID    = EVT_SPRITEDESTROYED;
    static constexpr SQInt32 CustomEvID     = EVT_SPRITECUSTOM;

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit >   Instances;

    // --------------------------------------------------------------------------------------------
    typedef std::bitset< Limit >            Set;

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type) noexcept
    {
        switch (type)
        {
            case EVT_SPRITECREATED:
            case EVT_SPRITEDESTROYED:
            case EVT_SPRITECUSTOM:
                return true;
            default:
                return false;
        }
    }

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type, bool /* inversed */) noexcept
    {
        switch (type)
        {
            case EVT_SPRITECREATED:
            case EVT_SPRITEDESTROYED:
            case EVT_SPRITECUSTOM:
                return true;
            default:
                return false;
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialized entity interface for the textdraw entity type.
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
     * Default destructor (disabled)
    */
    Ent() = delete;

    /* --------------------------------------------------------------------------------------------
     * Helper structure that holds information about each entity instance of this type.
    */
    typedef struct Textdraw
    {
        // ----------------------------------------------------------------------------------------
        Textdraw() noexcept
            : ID(-1), Root(0), Owned(false), Fresh(true)
        {
            /* ... */
        }

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

        // ----------------------------------------------------------------------------------------
        ETextdrawCreated & Created() noexcept
        {
            return TextdrawCreated;
        }

        ETextdrawDestroyed & Destroyed() noexcept
        {
            return TextdrawDestroyed;
        }

        ETextdrawCustom & Custom() noexcept
        {
            return TextdrawCustom;
        }

        // ----------------------------------------------------------------------------------------
        ETextdrawCreated & GCreated() noexcept
        {
            return GTextdrawCreated();
        }

        ETextdrawDestroyed & GDestroyed() noexcept
        {
            return GTextdrawDestroyed();
        }

        ETextdrawCustom & GCustom() noexcept
        {
            return GTextdrawCustom();
        }

    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst) noexcept
    {
        inst.Text.clear();
    }

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst, SQInt32 index, const SQChar * text, SQInt32 xp, SQInt32 yp,
                        SQUint32 color, bool rel) noexcept
    {
        inst.Text.assign(text);
        SQMOD_UNUSED_VAR(index);
        SQMOD_UNUSED_VAR(xp);
        SQMOD_UNUSED_VAR(yp);
        SQMOD_UNUSED_VAR(color);
        SQMOD_UNUSED_VAR(rel);
    }

    // --------------------------------------------------------------------------------------------
    static void Clear(Instance & inst) noexcept
    {
        inst.TextdrawCreated.Clear();
        inst.TextdrawDestroyed.Clear();
        inst.TextdrawCustom.Clear();
    }

    // --------------------------------------------------------------------------------------------
    static SQInt32 Create(SQInt32 index, const SQChar * text, SQInt32 xp, SQInt32 yp,
                            SQUint32 color, bool rel) noexcept
    {
        return _Func->CreateTextdraw(index, text, xp, yp, color, rel);
    }

    // --------------------------------------------------------------------------------------------
    static void Destroy(SQInt32 id) noexcept
    {
        _Func->DestroyTextdraw(id);
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Textdraw";
    static constexpr const SQChar* CName    = "CTextdraw";

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID         = ENT_TEXTDRAW;
    static constexpr SQInt32 Limit          = SQMOD_TEXTDRAW_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 CreateEvID     = EVT_TEXTDRAWCREATED;
    static constexpr SQInt32 DestroyEvID    = EVT_TEXTDRAWDESTROYED;
    static constexpr SQInt32 CustomEvID     = EVT_TEXTDRAWCUSTOM;

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit >   Instances;

    // --------------------------------------------------------------------------------------------
    typedef std::bitset< Limit >            Set;

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type) noexcept
    {
        switch (type)
        {
            case EVT_TEXTDRAWCREATED:
            case EVT_TEXTDRAWDESTROYED:
            case EVT_TEXTDRAWCUSTOM:
                return true;
            default:
                return false;
        }
    }

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type, bool /* inversed */) noexcept
    {
        switch (type)
        {
            case EVT_TEXTDRAWCREATED:
            case EVT_TEXTDRAWDESTROYED:
            case EVT_TEXTDRAWCUSTOM:
                return true;
            default:
                return false;
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialized entity interface for the vehicle entity type.
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
     * Default destructor (disabled)
    */
    Ent() = delete;

    /* --------------------------------------------------------------------------------------------
     * Helper structure that holds information about each entity instance of this type.
    */
    typedef struct Vehicle
    {
        // ----------------------------------------------------------------------------------------
        Vehicle() noexcept
            : ID(-1), Root(0), Owned(false), Fresh(true)
        {
            /* ... */
        }

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

        // ----------------------------------------------------------------------------------------
        EVehicleCreated & Created() noexcept
        {
            return VehicleCreated;
        }

        EVehicleDestroyed & Destroyed() noexcept
        {
            return VehicleDestroyed;
        }

        EVehicleCustom & Custom() noexcept
        {
            return VehicleCustom;
        }

        // ----------------------------------------------------------------------------------------
        EVehicleCreated & GCreated() noexcept
        {
            return GVehicleCreated();
        }

        EVehicleDestroyed & GDestroyed() noexcept
        {
            return GVehicleDestroyed();
        }

        EVehicleCustom & GCustom() noexcept
        {
            return GVehicleCustom();
        }

    } Instance;

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst) noexcept
    {
        SQMOD_UNUSED_VAR(inst);
    }

    // --------------------------------------------------------------------------------------------
    static void Store(Instance & inst, SQInt32 model, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                        SQFloat angle, SQInt32 primary, SQInt32 secondary) noexcept
    {
        SQMOD_UNUSED_VAR(inst);
        SQMOD_UNUSED_VAR(model);
        SQMOD_UNUSED_VAR(world);
        SQMOD_UNUSED_VAR(x);
        SQMOD_UNUSED_VAR(y);
        SQMOD_UNUSED_VAR(z);
        SQMOD_UNUSED_VAR(angle);
        SQMOD_UNUSED_VAR(primary);
        SQMOD_UNUSED_VAR(secondary);
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

    // --------------------------------------------------------------------------------------------
    static SQInt32 Create(SQInt32 model, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                            SQFloat angle, SQInt32 primary, SQInt32 secondary) noexcept
    {
        return _Func->CreateVehicle(model, world, x, y, z, angle, primary, secondary);
    }

    // --------------------------------------------------------------------------------------------
    static void Destroy(SQInt32 id) noexcept
    {
        _Func->DeleteVehicle(id);
    }

public:

    // --------------------------------------------------------------------------------------------
    static constexpr const SQChar* Name     = "Vehicle";
    static constexpr const SQChar* CName    = "CVehicle";

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 TypeID         = ENT_VEHICLE;
    static constexpr SQInt32 Limit          = SQMOD_VEHICLE_POOL;

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32 CreateEvID     = EVT_VEHICLECREATED;
    static constexpr SQInt32 DestroyEvID    = EVT_VEHICLEDESTROYED;
    static constexpr SQInt32 CustomEvID     = EVT_VEHICLECUSTOM;

    // --------------------------------------------------------------------------------------------
    typedef std::array< Instance, Limit >   Instances;

    // --------------------------------------------------------------------------------------------
    typedef std::bitset< Limit >            Set;

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type) noexcept
    {
        switch (type)
        {
            case EVT_VEHICLECREATED:
            case EVT_VEHICLEDESTROYED:
            case EVT_VEHICLECUSTOM:
            case EVT_VEHICLERESPAWN:
            case EVT_VEHICLEEXPLODE:
            case EVT_VEHICLEHEALTH:
            case EVT_VEHICLEMOVE:
            case EVT_VEHICLEEMBARKING:
            case EVT_VEHICLEEMBARKED:
            case EVT_VEHICLEDISEMBARK:
                return true;
            default:
                return false;
        }
    }

    // --------------------------------------------------------------------------------------------
    static bool InEvent(SQInt32 type, bool inversed) noexcept
    {
        switch (type)
        {
            case EVT_VEHICLECREATED:
            case EVT_VEHICLEDESTROYED:
            case EVT_VEHICLECUSTOM:
            case EVT_VEHICLERESPAWN:
            case EVT_VEHICLEEXPLODE:
            case EVT_VEHICLEHEALTH:
            case EVT_VEHICLEMOVE:
                return true;
            case EVT_VEHICLEEMBARKING:
            case EVT_VEHICLEEMBARKED:
            case EVT_VEHICLEDISEMBARK:
                return !inversed;
            default:
                return false;
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * A basic reference to an entity instance.
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

    // --------------------------------------------------------------------------------------------
    static constexpr SQInt32    TypeID = Ent< T >::TypeID;

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
     * Insert this instance into the reference chain.
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
     * Remove this instance from the reference chain.
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
     * Verify that the specified entity instance is active
    */
    static bool Verify(SQInt32 id) noexcept
    {
        return (VALID_ENTITYEX(id, Max) && VALID_ENTITY(s_Instances[id].ID));
    }

    /* --------------------------------------------------------------------------------------------
     * Returns a reference to the specified entity instance
    */
    static typename Ent< T >::Instance & Get(SQInt32 id) noexcept
    {
        return s_Instances[id]; /* Expects the callee to deal with range checks! */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    Reference() noexcept
        : Reference(SQMOD_UNKNOWN)
    {

    }

    /* --------------------------------------------------------------------------------------------
     * Construct a reference of this type and attach it to the referenced instance.
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
     * Copy constructor.
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
     * Move constructor.
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
     * Destructor.
    */
    ~Reference()
    {
        RemoveFromChain();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
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
     * Move assignment operator. (disabled)
    */
    Reference< T > & operator = (Reference< T > &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to an entity identifier.
    */
    operator SQInt32 () const noexcept
    {
        return m_ID;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to an entity identifier.
    */
    operator Int64 () const noexcept
    {
        return _SCI64(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to an entity identifier.
    */
    operator SQUint32 () const noexcept
    {
        return _SCU32(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to an entity identifier.
    */
    operator Uint64 () const noexcept
    {
        return _SCU64(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean.
    */
    operator bool () const noexcept
    {
        return VALID_ENTITY(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Negation operator.
    */
    bool operator ! () const noexcept
    {
        return INVALID_ENTITY(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script VM to compare two references against eachother.
    */
    SQInteger Cmp(const Reference< T > & r) const noexcept
    {
        if (m_ID == r.m_ID)
        {
            return 0;
        }
        else if (m_ID > r.m_ID)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert this reference to a string.
    */
    const SQChar * ToString() const noexcept
    {
        return ToStringF("%d", m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier of the referenced instance.
    */
    SQInteger GetID() const noexcept
    {
        return m_ID;
    }

    /* --------------------------------------------------------------------------------------------
     * Point ths reference to another entity instance.
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
     * See whether this reference is persistent.
    */
    bool GetPersistent() const noexcept
    {
        return m_Persistent;
    }

    /* --------------------------------------------------------------------------------------------
     * Set this reference to be persistent.
    */
    void SetPersistent(bool toggle) noexcept
    {
        m_Persistent = toggle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the global tag.
    */
    const SQChar * GetGlobalTag() const noexcept
    {
        if (VALID_ENTITY(m_ID))
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
     * Change the global tag.
    */
    void SetGlobalTag(const SQChar * tag) const noexcept
    {
        if (VALID_ENTITY(m_ID))
        {
            s_Instances[m_ID].Tag.assign(tag);
        }
        else
        {
            LogWrn(_SC("Attempting to <set global tag> using an invalid reference: %d"), m_ID);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the global data.
    */
    SqObj & GetGlobalData() noexcept
    {
        if (VALID_ENTITY(m_ID))
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
     * Change the global data.
    */
    void SetGlobalData(SqObj & data) const noexcept
    {
        if (VALID_ENTITY(m_ID))
        {
            s_Instances[m_ID].Data = data;
        }
        else
        {
            LogWrn(_SC("Attempting to <set global tag> using an invalid reference: %d"), m_ID);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the local tag
    */
    const SQChar * GetLocalTag() const noexcept
    {
        return m_Tag.c_str();
    }

    /* --------------------------------------------------------------------------------------------
     * Change the local tag.
    */
    void SetLocalTag(const SQChar * tag) noexcept
    {
        m_Tag.assign(tag);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the local data.
    */
    SqObj & GetLocalData() noexcept
    {
        return m_Data;
    }

    /* --------------------------------------------------------------------------------------------
     * Change the local data.
    */
    void SetLocalData(SqObj & data) noexcept
    {
        m_Data = data;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the maximum allowed identifier for this entity type.
    */
    SQUint32 GetMax() const noexcept
    {
        return Max;
    }

    /* --------------------------------------------------------------------------------------------
     * Return a new reference from the base reference.
    */
    T GetReference() const noexcept
    {
        return T(m_ID);
    }

    /* --------------------------------------------------------------------------------------------
     * Returns whether this entity reference points to an active entity.
    */
    bool IsActive() const noexcept
    {
        return VALID_ENTITYEX(m_ID, Max);
    }

    /* --------------------------------------------------------------------------------------------
     * Counts the number of active references for this entity.
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
     * Counts the number of persistent references for this entity.
    */
    SQUint32 CountPersistentRefs() const noexcept
    {
        SQUint32 refs = 0;
        // Make sure the reference is valid
        if (VALID_ENTITYEX(m_ID, Max))
        {
            // Count this instance if persistent
            if (m_Persistent)
            {
                ++refs;
            }
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

    /* --------------------------------------------------------------------------------------------
     * Destroy the referenced entity.
    */
    bool Destroy() noexcept
    {
        return Destroy(0, NullData());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the referenced entity.
    */
    bool Destroy(SQInt32 header) noexcept
    {
        return Destroy(header, NullData());
    }

    /* --------------------------------------------------------------------------------------------
     * Destroy the referenced entity.
    */
    bool Destroy(SQInt32 header, SqObj & payload) noexcept
    {
        if (VALID_ENTITY(m_ID))
        {
            return EntMan< T >::Deactivate(m_ID, header, payload, true);
        }
        else
        {
            LogWrn(_SC("Attempting to <destroy %d entity> using an invalid reference: %d"), Ent< T >::Name, m_ID);
        }

        return false;
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
    friend class Reference< T >;
    friend class Core;

public:

    // --------------------------------------------------------------------------------------------
    typedef Reference< T >  RefType;
    typedef Ent< T >        EntType;

private:

    // --------------------------------------------------------------------------------------------
    static RefType    NullRef;

    /* --------------------------------------------------------------------------------------------
     * Deactivates the specified entity instance.
    */
    static bool Deactivate(SQInt32 id, SQInt32 header, SqObj & payload, bool notify) noexcept
    {
        // Make sure this entity even exists
        if (RefType::Verify(id))
        {
            // Trigger this associated events if required
            if (notify)
            {
                RefType::s_Instances[id].Destroyed().Emit(id, header, payload);
                RefType::s_Instances[id].GDestroyed().Emit(id, header, payload);
            }

            RefType * ref = 0, * bkp = 0;
            // Get the pointer to the first backward reference
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
            // Get the pointer to the first forward reference
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
            EntType::Destroy(id);

            // Disable this entity instance
            RefType::s_Instances[id].ID = SQMOD_UNKNOWN;

            // Clear any events that are still listening
            EntType::Clear(RefType::s_Instances[id]);

            // Successfully deactivated the instance
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
     * Activates the specified entity instance.
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
                // @TODO Remove the reference from the chain, if somehow
                //  is present there without being persistent

                // Resurrect backward references
                for (RefType * ref = RefType::s_Instances[id].Root->m_Prev; ref; ref = ref->m_Prev)
                {
                    if (ref->m_Persistent)
                    {
                        ref->m_ID = id;
                    }
                }

                // Resurrect forward references
                for (RefType * ref = RefType::s_Instances[id].Root->m_Next; ref; ref = ref->m_Next)
                {
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

    /* --------------------------------------------------------------------------------------------
     * Creates a new entity instance of the specified type.
    */
    template < typename... Args > static SQInt32 Create(SQInt32 header, SqObj & payload, bool notify, Args&&... args) noexcept
    {
        // Attempt to create an instance on the server and obtain it's identifier
        SQInt32 id = EntType::Create(std::forward< Args >(args)...);
        // Attempt to activate the instance in the plugin at the received identifier
        if (!Activate(id, true, std::forward< Args >(args)...))
        {
            LogErr("Unable to <create a new %s> instance", EntType::Name);
            // Destroy this entity instance
            EntType::Destroy(id);
            // Replace the id with an invalid one
            id = SQMOD_UNKNOWN;
        }
        // Trigger this associated events if required
        else if (notify)
        {
            RefType::s_Instances[id].Created().Emit(id, header, payload);
            RefType::s_Instances[id].GCreated().Emit(id, header, payload);
        }
        // Return the obtained entity identifier
        return id;
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
    Sqrat::RootTable(vm).Bind(cname, Sqrat::Class< Ref >(vm, cname)
        /* Constructors */
        .Ctor()
        .template Ctor< SQInt32 >()
        /* Metamethods */
        .Func(_SC("_cmp"), &Ref::Cmp)
        .Func(_SC("_tostring"), &Ref::ToString)
        /* Functions */
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
        /* Overloads */
        .template Overload< bool (Ref::*)(void) >(_SC("destroy"), &Ref::Destroy)
        .template Overload< bool (Ref::*)(SQInt32) >(_SC("destroy"), &Ref::Destroy)
        .template Overload< bool (Ref::*)(SQInt32, SqObj &) >(_SC("destroy"), &Ref::Destroy)
    );
    // Output debugging information
    LogDbg("Registration of <%s> type was successful", cname);
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod

#endif // _ENTITY_HPP_
