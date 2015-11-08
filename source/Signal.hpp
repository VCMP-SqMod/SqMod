#ifndef _SIGNALS_HPP_
#define _SIGNALS_HPP_

// ------------------------------------------------------------------------------------------------
#include <sqconfig.h>

// ------------------------------------------------------------------------------------------------
#include <tuple>

// ------------------------------------------------------------------------------------------------
namespace Sqrat
{
    class Object;
}

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
using namespace Sqrat;

// ------------------------------------------------------------------------------------------------
struct Vector3;

// ------------------------------------------------------------------------------------------------
template < typename Ret > class SignalImpl;

// ------------------------------------------------------------------------------------------------
template < typename Ret, typename... Args > class SignalImpl< Ret (Args...) >
{
protected:
    // --------------------------------------------------------------------------------------------
    using Executor = Ret (*) (void *, Args...);

    // --------------------------------------------------------------------------------------------
    struct Node
    {
        // ----------------------------------------------------------------------------------------
        Executor        m_Exec;

        // ----------------------------------------------------------------------------------------
        void *          m_This;

        // ----------------------------------------------------------------------------------------
        Node *          m_Next;

        // ----------------------------------------------------------------------------------------
        Node(Executor e, void * t, Node * n)
            : m_Exec(e), m_This(t), m_Next(n)
        {
            /* ... */
        }

        // ----------------------------------------------------------------------------------------
        Node(const Node &) = default;
        Node(Node &&) = default;

        // ----------------------------------------------------------------------------------------
        Node & operator = (const Node &) = default;
        Node & operator = (Node &&) = default;
    };

    // --------------------------------------------------------------------------------------------
    struct Container
    {
        // ----------------------------------------------------------------------------------------
        Node * m_Head;

        // ----------------------------------------------------------------------------------------
        Container() : m_Head(0) { /* ... */ }

        Container(Node * n) : m_Head(n) { /* ... */ }

        // ----------------------------------------------------------------------------------------
        Container(const Container &) = default;
        Container(Container &&) = default;

        // ----------------------------------------------------------------------------------------
        ~Container()
        {
            Clear();
        }

        // ----------------------------------------------------------------------------------------
        Container & operator = (const Container &) = default;
        Container & operator = (Container &&) = default;

        // ----------------------------------------------------------------------------------------
        void Remove(Executor e, void * t)
        {
            for (Node * node = m_Head, * prev = 0; node; prev = node, node = node->m_Next)
            {
                if (node->m_This == t && node->m_Exec == e)
                {
                    if (prev)
                    {
                        prev->m_Next = node->m_Next;
                    }
                    else
                    {
                        m_Head = m_Head->m_Next;
                    }
                    delete node;
                    break;
                }
            }
        }

        // ----------------------------------------------------------------------------------------
        void Clear()
        {
            for (Node * node = m_Head, * next = 0; node; node = next)
            {
                next = node->m_Next;
                delete node;
            }
            m_Head = 0;
        }
    };

    // --------------------------------------------------------------------------------------------
    template < Ret (* FPtr) (Args...) > static inline Executor Free()
    {
        return [](void * /* NULL */, Args... args) -> Ret {
                 return (*FPtr)(std::forward<Args>(args)...);
            };
    }

    // --------------------------------------------------------------------------------------------
    template < typename T, Ret (T::* MPtr) (Args...) > static inline Executor Member()
    {
        return [](void * thisptr, Args... args) -> Ret {
                return (static_cast<T *>(thisptr)->*MPtr)(std::forward<Args>(args)...);
            };
    }

    // --------------------------------------------------------------------------------------------
    template < typename T, Ret (T::* MPtr) (Args...) const> static inline Executor Member()
    {
        return [](void * thisptr, Args... args) -> Ret {
                return (static_cast<T *>(thisptr)->*MPtr)(std::forward<Args>(args)...);
            };
    }

    // --------------------------------------------------------------------------------------------
    template < typename L > static inline Executor Lambda()
    {
        return [](void * thisptr, Args... args) -> Ret {
                return (static_cast<L *>(thisptr)->operator()(std::forward<Args>(args)...));
            };
    }

public:

    // --------------------------------------------------------------------------------------------
    typedef Ret Return;

    // --------------------------------------------------------------------------------------------
    template < Ret (* FPtr) (Args...) > void Connect()
    {
        m_Nodes.m_Head = new Node(Free< FPtr >(), 0, m_Nodes.m_Head);
    }

    // --------------------------------------------------------------------------------------------
    template < typename T, Ret (T::* MPtr) (Args...) > void Connect(T * ptr)
    {
        m_Nodes.m_Head = new Node(Member< T, MPtr >(), ptr, m_Nodes.m_Head);
    }

    template < typename T, Ret (T::* MPtr) (Args...) > void Connect(T & ptr)
    {
        m_Nodes.m_Head = new Node(Member< T, MPtr >(), std::addressof(ptr), m_Nodes.m_Head);
    }

    // --------------------------------------------------------------------------------------------
    template < typename T, Ret (T::* MPtr) (Args...) const > void Connect(T * ptr)
    {
        m_Nodes.m_Head = new Node(Member< T, MPtr >(), ptr, m_Nodes.m_Head);
    }

    template < typename T, Ret (T::* MPtr) (Args...) const > void Connect(T & ptr)
    {
        m_Nodes.m_Head = new Node(Member< T, MPtr >(), std::addressof(ptr), m_Nodes.m_Head);
    }

    // --------------------------------------------------------------------------------------------
    template < typename L > void Connect(L * ptr)
    {
        m_Nodes.m_Head = new Node(Lambda< L >(), ptr, m_Nodes.m_Head);
    }

    template < typename L > void Connect(L & ptr)
    {
        m_Nodes.m_Head = new Node(Lambda< L >(), std::addressof(ptr), m_Nodes.m_Head);
    }

    // --------------------------------------------------------------------------------------------
    template < Ret (* FPtr) (Args...) > void Disconnect()
    {
        m_Nodes.Remove(Free< FPtr >(), 0);
    }

    // --------------------------------------------------------------------------------------------
    template < typename T, Ret (T::* MPtr) (Args...) > void Disconnect(T * ptr)
    {
        m_Nodes.Remove(Member< T, MPtr >(), ptr);
    }

    template < typename T, Ret (T::* MPtr) (Args...) > void Disconnect(T & ptr)
    {
        m_Nodes.Remove(Member< T, MPtr >(), std::addressof(ptr));
    }

    // --------------------------------------------------------------------------------------------
    template < typename T, Ret (T::* MPtr) (Args...) const > void Disconnect(T * ptr)
    {
        m_Nodes.Remove(Member< T, MPtr >(), ptr);
    }

    template < typename T, Ret (T::* MPtr) (Args...) const > void Disconnect(T & ptr)
    {
        m_Nodes.Remove(Member< T, MPtr >(), std::addressof(ptr));
    }

    // --------------------------------------------------------------------------------------------
    template < typename L > void Disconnect(L * ptr)
    {
        m_Nodes.Remove(Lambda< L >(), ptr);
    }

    template < typename L > void Disconnect(L & ptr)
    {
        m_Nodes.Remove(Lambda< L >(), std::addressof(ptr));
    }

    // --------------------------------------------------------------------------------------------
    void Clear()
    {
        m_Nodes.Clear();
    }

    // --------------------------------------------------------------------------------------------
    template <typename... Uref> void Emit(Uref &&... args)
    {
        for (Node * node = m_Nodes.m_Head; node; node = node->m_Next)
        {
            (*node->m_Exec)(node->m_This, std::forward<Uref>(args)...);
        }
    }

    // --------------------------------------------------------------------------------------------
    template <typename T, typename... Uref> void Query(T && collecter, Uref &&... args)
    {
        for (Node * node = m_Nodes.m_Head; node; node = node->m_Next)
        {
            collecter( ( (*node->m_Exec) (node->m_This, std::forward<Uref>(args)...) ) );
        }
    }

    // --------------------------------------------------------------------------------------------
    template <typename... Uref> void operator () (Uref &&... args)
    {
        for (Node * node = m_Nodes.m_Head; node; node = node->m_Next)
        {
            (*node->m_Exec)(node->m_This, std::forward<Uref>(args)...);
        }
    }

private:

    // --------------------------------------------------------------------------------------------
    Container   m_Nodes;
};

// ------------------------------------------------------------------------------------------------
template < typename Sig, int I = 0 > class Signal : public SignalImpl< Sig >
{
    static constexpr int SignalID = I;
};

// ------------------------------------------------------------------------------------------------
enum EventType
{
    EVT_UNKNOWN = 0,
    EVT_BLIPCREATED,
    EVT_CHECKPOINTCREATED,
    EVT_KEYBINDCREATED,
    EVT_OBJECTCREATED,
    EVT_PICKUPCREATED,
    EVT_PLAYERCREATED,
    EVT_SPHERECREATED,
    EVT_SPRITECREATED,
    EVT_TEXTDRAWCREATED,
    EVT_VEHICLECREATED,
    EVT_BLIPDESTROYED,
    EVT_CHECKPOINTDESTROYED,
    EVT_KEYBINDDESTROYED,
    EVT_OBJECTDESTROYED,
    EVT_PICKUPDESTROYED,
    EVT_PLAYERDESTROYED,
    EVT_SPHEREDESTROYED,
    EVT_SPRITEDESTROYED,
    EVT_TEXTDRAWDESTROYED,
    EVT_VEHICLEDESTROYED,
    EVT_BLIPCUSTOM,
    EVT_CHECKPOINTCUSTOM,
    EVT_KEYBINDCUSTOM,
    EVT_OBJECTCUSTOM,
    EVT_PICKUPCUSTOM,
    EVT_PLAYERCUSTOM,
    EVT_SPHERECUSTOM,
    EVT_SPRITECUSTOM,
    EVT_TEXTDRAWCUSTOM,
    EVT_VEHICLECUSTOM,
    EVT_PLAYERAWAY,
    EVT_PLAYERGAMEKEYS,
    EVT_PLAYERRENAME,
    EVT_PLAYERREQUESTCLASS,
    EVT_PLAYERREQUESTSPAWN,
    EVT_PLAYERSPAWN,
    EVT_PLAYERSTARTTYPING,
    EVT_PLAYERSTOPTYPING,
    EVT_PLAYERCHAT,
    EVT_PLAYERCOMMAND,
    EVT_PLAYERMESSAGE,
    EVT_PLAYERHEALTH,
    EVT_PLAYERARMOUR,
    EVT_PLAYERWEAPON,
    EVT_PLAYERMOVE,
    EVT_PLAYERWASTED,
    EVT_PLAYERKILLED,
    EVT_PLAYERTEAMKILL,
    EVT_PLAYERSPECTATE,
    EVT_PLAYERCRASHREPORT,
    EVT_PLAYERBURNING,
    EVT_PLAYERCROUCHING,
    EVT_PLAYERSTATE,
    EVT_PLAYERACTION,
    EVT_STATENONE,
    EVT_STATENORMAL,
    EVT_STATESHOOTING,
    EVT_STATEDRIVER,
    EVT_STATEPASSENGER,
    EVT_STATEENTERDRIVER,
    EVT_STATEENTERPASSENGER,
    EVT_STATEEXITVEHICLE,
    EVT_STATEUNSPAWNED,
    EVT_ACTIONNONE,
    EVT_ACTIONNORMAL,
    EVT_ACTIONAIMING,
    EVT_ACTIONSHOOTING,
    EVT_ACTIONJUMPING,
    EVT_ACTIONLIEDOWN,
    EVT_ACTIONGETTINGUP,
    EVT_ACTIONJUMPVEHICLE,
    EVT_ACTIONDRIVING,
    EVT_ACTIONDYING,
    EVT_ACTIONWASTED,
    EVT_ACTIONEMBARKING,
    EVT_ACTIONDISEMBARKING,
    EVT_VEHICLERESPAWN,
    EVT_VEHICLEEXPLODE,
    EVT_VEHICLEHEALTH,
    EVT_VEHICLEMOVE,
    EVT_PICKUPRESPAWN,
    EVT_KEYBINDKEYPRESS,
    EVT_KEYBINDKEYRELEASE,
    EVT_VEHICLEEMBARKING,
    EVT_VEHICLEEMBARKED,
    EVT_VEHICLEDISEMBARK,
    EVT_PICKUPCLAIMED,
    EVT_PICKUPCOLLECTED,
    EVT_OBJECTSHOT,
    EVT_OBJECTBUMP,
    EVT_CHECKPOINTENTERED,
    EVT_CHECKPOINTEXITED,
    EVT_SPHEREENTERED,
    EVT_SPHEREEXITED,
    EVT_SERVERFRAME,
    EVT_SERVERSTARTUP,
    EVT_SERVERSHUTDOWN,
    EVT_INTERNALCOMMAND,
    EVT_LOGINATTEMPT,
    EVT_CUSTOMEVENT,
    EVT_WORLDOPTION,
    EVT_WORLDTOGGLE,
    EVT_SCRIPTRELOAD,
    EVT_COUNT
};

// ------------------------------------------------------------------------------------------------
using EBlipCreated          = Signal< void (SQInt32 /* blip */, SQInt32 /* header */, Object & /* payload */) >;
using ECheckpointCreated    = Signal< void (SQInt32 /* checkpoint */, SQInt32 /* header */, Object & /* payload */) >;
using EKeybindCreated       = Signal< void (SQInt32 /* keybind */, SQInt32 /* header */, Object & /* payload */) >;
using EObjectCreated        = Signal< void (SQInt32 /* object */, SQInt32 /* header */, Object & /* payload */) >;
using EPickupCreated        = Signal< void (SQInt32 /* pickup */, SQInt32 /* header */, Object & /* payload */) >;
using EPlayerCreated        = Signal< void (SQInt32 /* player */, SQInt32 /* header */, Object & /* payload */) >;
using ESphereCreated        = Signal< void (SQInt32 /* sphere */, SQInt32 /* header */, Object & /* payload */) >;
using ESpriteCreated        = Signal< void (SQInt32 /* sprite */, SQInt32 /* header */, Object & /* payload */) >;
using ETextdrawCreated      = Signal< void (SQInt32 /* textdraw */, SQInt32 /* header */, Object & /* payload */) >;
using EVehicleCreated       = Signal< void (SQInt32 /* vehicle */, SQInt32 /* header */, Object & /* payload */) >;

// ------------------------------------------------------------------------------------------------
using EBlipDestroyed        = Signal< void (SQInt32 /* blip */, SQInt32 /* header */, Object & /* payload */) >;
using ECheckpointDestroyed  = Signal< void (SQInt32 /* checkpoint */, SQInt32 /* header */, Object & /* payload */) >;
using EKeybindDestroyed     = Signal< void (SQInt32 /* keybind */, SQInt32 /* header */, Object & /* payload */) >;
using EObjectDestroyed      = Signal< void (SQInt32 /* object */, SQInt32 /* header */, Object & /* payload */) >;
using EPickupDestroyed      = Signal< void (SQInt32 /* pickup */, SQInt32 /* header */, Object & /* payload */) >;
using EPlayerDestroyed      = Signal< void (SQInt32 /* player */, SQInt32 /* header */, Object & /* payload */) >;
using ESphereDestroyed      = Signal< void (SQInt32 /* sphere */, SQInt32 /* header */, Object & /* payload */) >;
using ESpriteDestroyed      = Signal< void (SQInt32 /* sprite */, SQInt32 /* header */, Object & /* payload */) >;
using ETextdrawDestroyed    = Signal< void (SQInt32 /* textdraw */, SQInt32 /* header */, Object & /* payload */) >;
using EVehicleDestroyed     = Signal< void (SQInt32 /* vehicle */, SQInt32 /* header */, Object & /* payload */) >;

// ------------------------------------------------------------------------------------------------
using EBlipCustom           = Signal< void (SQInt32 /* blip */, SQInt32 /* header */, Object & /* payload */) >;
using ECheckpointCustom     = Signal< void (SQInt32 /* checkpoint */, SQInt32 /* header */, Object & /* payload */) >;
using EKeybindCustom        = Signal< void (SQInt32 /* keybind */, SQInt32 /* header */, Object & /* payload */) >;
using EObjectCustom         = Signal< void (SQInt32 /* object */, SQInt32 /* header */, Object & /* payload */) >;
using EPickupCustom         = Signal< void (SQInt32 /* pickup */, SQInt32 /* header */, Object & /* payload */) >;
using EPlayerCustom         = Signal< void (SQInt32 /* player */, SQInt32 /* header */, Object & /* payload */) >;
using ESphereCustom         = Signal< void (SQInt32 /* sphere */, SQInt32 /* header */, Object & /* payload */) >;
using ESpriteCustom         = Signal< void (SQInt32 /* sprite */, SQInt32 /* header */, Object & /* payload */) >;
using ETextdrawCustom       = Signal< void (SQInt32 /* textdraw */, SQInt32 /* header */, Object & /* payload */) >;
using EVehicleCustom        = Signal< void (SQInt32 /* vehicle */, SQInt32 /* header */, Object & /* payload */) >;

// ------------------------------------------------------------------------------------------------
using EPlayerAway           = Signal< void (SQInt32 /* player */, bool /* status */) >;

// ------------------------------------------------------------------------------------------------
using EPlayerGameKeys       = Signal< void (SQInt32 /* player */, SQInt32 /* previous */, SQInt32 /* current */) >;
using EPlayerRename         = Signal< void (SQInt32 /* player */, const SQChar * /* previous */, const SQChar * /* current */) >;

// ------------------------------------------------------------------------------------------------
using EPlayerRequestClass   = Signal< void (SQInt32 /* player */, SQInt32 /* offset */) >;
using EPlayerRequestSpawn   = Signal< void (SQInt32 /* player */) >;

// ------------------------------------------------------------------------------------------------
using EPlayerSpawn          = Signal< void (SQInt32 /* player */) >;

// ------------------------------------------------------------------------------------------------
using EPlayerStartTyping    = Signal< void (SQInt32 /* player */) >;
using EPlayerStopTyping     = Signal< void (SQInt32 /* player */) >;

// ------------------------------------------------------------------------------------------------
using EPlayerChat           = Signal< void (SQInt32 /* player */, const SQChar * /* message */) >;
using EPlayerCommand        = Signal< void (SQInt32 /* player */, const SQChar * /* command */) >;
using EPlayerMessage        = Signal< void (SQInt32 /* player */, SQInt32 /* receiver */, const SQChar * /* message */) >;

// ------------------------------------------------------------------------------------------------
using EPlayerHealth         = Signal< void (SQInt32 /* player */, SQFloat /* previous */, SQFloat /* current */) >;
using EPlayerArmour         = Signal< void (SQInt32 /* player */, SQFloat /* previous */, SQFloat /* current */) >;
using EPlayerWeapon         = Signal< void (SQInt32 /* player */, SQInt32 /* previous */, SQInt32 /* current */) >;
using EPlayerMove           = Signal< void (SQInt32 /* player */, const Vector3 & /* previous */, const Vector3 & /* current */) >;

// ------------------------------------------------------------------------------------------------
using EPlayerWasted         = Signal< void (SQInt32 /* player */, SQInt32 /* reason */) >;
using EPlayerKilled         = Signal< void (SQInt32 /* player */, SQInt32 /* killer */, SQInt32 /* reason */, SQInt32 /* body_part */) >;
using EPlayerTeamKill       = Signal< void (SQInt32 /* player */, SQInt32 /* killer */, SQInt32 /* reason */, SQInt32 /* body_part */) >;

// ------------------------------------------------------------------------------------------------
using EPlayerSpectate       = Signal< void (SQInt32 /* player */, SQInt32 /* target */) >;
using EPlayerCrashreport    = Signal< void (SQInt32 /* player */, const SQChar * /* report */) >;

// ------------------------------------------------------------------------------------------------
using EPlayerBurning        = Signal< void (SQInt32 /* player */, bool /* state */) >;
using EPlayerCrouching      = Signal< void (SQInt32 /* player */, bool /* state */) >;

// ------------------------------------------------------------------------------------------------
using EPlayerState          = Signal< void (SQInt32 /* player */, SQInt32 /* previous */, SQInt32 /* current */) >;
using EPlayerAction         = Signal< void (SQInt32 /* player */, SQInt32 /* previous */, SQInt32 /* current */) >;

// ------------------------------------------------------------------------------------------------
using EStateNone            = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EStateNormal          = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EStateShooting        = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EStateDriver          = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EStatePassenger       = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EStateEnterDriver     = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EStateEnterPassenger  = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EStateExitVehicle     = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EStateUnspawned       = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;

// ------------------------------------------------------------------------------------------------
using EActionNone           = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EActionNormal         = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EActionAiming         = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EActionShooting       = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EActionJumping        = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EActionLieDown        = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EActionGettingUp      = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EActionJumpVehicle    = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EActionDriving        = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EActionDying          = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EActionWasted         = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EActionEmbarking      = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;
using EActionDisembarking   = Signal< void (SQInt32 /* player */, SQInt32 /* previous */) >;

// ------------------------------------------------------------------------------------------------
using EVehicleRespawn       = Signal< void (SQInt32 /* vehicle */) >;
using EVehicleExplode       = Signal< void (SQInt32 /* vehicle */) >;

// ------------------------------------------------------------------------------------------------
using EVehicleHealth        = Signal< void (SQInt32 /* vehicle */, SQFloat /* previous */, SQFloat /* current */) >;
using EVehicleMove          = Signal< void (SQInt32 /* vehicle */, const Vector3 & /* previous */, const Vector3 &/* current */) >;

// ------------------------------------------------------------------------------------------------
using EPickupRespawn        = Signal< void (SQInt32 /* pickup */) >;

// ------------------------------------------------------------------------------------------------
using EKeybindKeyPress      = Signal< void (SQInt32 /* player */, SQInt32 /* keybind */) >;
using EKeybindKeyRelease    = Signal< void (SQInt32 /* player */, SQInt32 /* keybind */) >;

// ------------------------------------------------------------------------------------------------
using EVehicleEmbarking     = Signal< void (SQInt32 /* player */, SQInt32 /* vehicle */, SQInt32 /* slot */) >;
using EVehicleEmbarked      = Signal< void (SQInt32 /* player */, SQInt32 /* vehicle */, SQInt32 /* slot */) >;
using EVehicleDisembark     = Signal< void (SQInt32 /* player */, SQInt32 /* vehicle */) >;

// ------------------------------------------------------------------------------------------------
using EPickupClaimed        = Signal< void (SQInt32 /* player */, SQInt32 /* pickup */) >;
using EPickupCollected      = Signal< void (SQInt32 /* player */, SQInt32 /* pickup */) >;

// ------------------------------------------------------------------------------------------------
using EObjectShot           = Signal< void (SQInt32 /* player */, SQInt32 /* object */, SQInt32 /* weapon */) >;
using EObjectBump           = Signal< void (SQInt32 /* player */, SQInt32 /* object */) >;

// ------------------------------------------------------------------------------------------------
using ECheckpointEntered    = Signal< void (SQInt32 /* player */, SQInt32 /* checkpoint */) >;
using ECheckpointExited     = Signal< void (SQInt32 /* player */, SQInt32 /* checkpoint */) >;

// ------------------------------------------------------------------------------------------------
using ESphereEntered        = Signal< void (SQInt32 /* player */, SQInt32 /* sphere */) >;
using ESphereExited         = Signal< void (SQInt32 /* player */, SQInt32 /* sphere */) >;

// ------------------------------------------------------------------------------------------------
using EServerFrame          = Signal< void (SQFloat /* delta */) >;

// ------------------------------------------------------------------------------------------------
using EServerStartup        = Signal< void (void) >;
using EServerShutdown       = Signal< void (void) >;

// ------------------------------------------------------------------------------------------------
using EInternalCommand      = Signal< void (SQInt32 /* type */, const SQChar * /* text */) >;
using ELoginAttempt         = Signal< void (const SQChar * /* name */, const SQChar * /* pass */, const SQChar * /* addr */) >;

// ------------------------------------------------------------------------------------------------
using ECustomEvent          = Signal< void (SQInt32 /* group */, SQInt32 /* header */, Object & /* payload */) >;

// ------------------------------------------------------------------------------------------------
using EWorldOption          = Signal< void (SQInt32 /* option */, Object & /* value */) >;
using EWorldToggle          = Signal< void (SQInt32 /* option */, bool /* value */) >;

// ------------------------------------------------------------------------------------------------
using EScriptReload         = Signal< void (SQInt32 /* header */, Object & /* payload */) >;

// ------------------------------------------------------------------------------------------------
using EVMClose              = Signal< void (void) >;

} // Namespace:: SqMod

#endif // _SIGNALS_HPP_
