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
        Node(Executor e, void * t, Node * n) noexcept
            : m_Exec(e), m_This(t), m_Next(n)
        {
            /* ... */
        }

        // ----------------------------------------------------------------------------------------
        Node(const Node &) noexcept = default;
        Node(Node &&) noexcept = default;

        // ----------------------------------------------------------------------------------------
        Node & operator = (const Node &) noexcept = default;
        Node & operator = (Node &&) noexcept = default;
    };

    // --------------------------------------------------------------------------------------------
    struct Container
    {
        // ----------------------------------------------------------------------------------------
        Node * m_Head;

        // ----------------------------------------------------------------------------------------
        Container() noexcept : m_Head(0) { /* ... */ }

        Container(Node * n) noexcept : m_Head(n) { /* ... */ }

        // ----------------------------------------------------------------------------------------
        Container(const Container &) noexcept = default;
        Container(Container &&) noexcept = default;

        // ----------------------------------------------------------------------------------------
        ~Container()
        {
            Clear();
        }

        // ----------------------------------------------------------------------------------------
        Container & operator = (const Container &) noexcept = default;
        Container & operator = (Container &&) noexcept = default;

        // ----------------------------------------------------------------------------------------
        void Remove(Executor e, void * t) noexcept
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
    template < Ret (* FPtr) (Args...) > static inline Executor Free() noexcept
    {
        return [](void * /* NULL */, Args... args) -> Ret {
                 return (*FPtr)(std::forward<Args>(args)...);
            };
    }

    // --------------------------------------------------------------------------------------------
    template < typename T, Ret (T::* MPtr) (Args...) > static inline Executor Member() noexcept
    {
        return [](void * thisptr, Args... args) -> Ret {
                return (static_cast<T *>(thisptr)->*MPtr)(std::forward<Args>(args)...);
            };
    }

    // --------------------------------------------------------------------------------------------
    template < typename T, Ret (T::* MPtr) (Args...) const> static inline Executor Member() noexcept
    {
        return [](void * thisptr, Args... args) -> Ret {
                return (static_cast<T *>(thisptr)->*MPtr)(std::forward<Args>(args)...);
            };
    }

    // --------------------------------------------------------------------------------------------
    template < typename L > static inline Executor Lambda() noexcept
    {
        return [](void * thisptr, Args... args) -> Ret {
                return (static_cast<L *>(thisptr)->operator()(std::forward<Args>(args)...));
            };
    }

public:

    // --------------------------------------------------------------------------------------------
    typedef Ret Return;

    // --------------------------------------------------------------------------------------------
    template < Ret (* FPtr) (Args...) > void Connect() noexcept
    {
        m_Nodes.m_Head = new Node(Free< FPtr >(), 0, m_Nodes.m_Head);
    }

    // --------------------------------------------------------------------------------------------
    template < typename T, Ret (T::* MPtr) (Args...) > void Connect(T * ptr) noexcept
    {
        m_Nodes.m_Head = new Node(Member< T, MPtr >(), ptr, m_Nodes.m_Head);
    }

    template < typename T, Ret (T::* MPtr) (Args...) > void Connect(T & ptr) noexcept
    {
        m_Nodes.m_Head = new Node(Member< T, MPtr >(), std::addressof(ptr), m_Nodes.m_Head);
    }

    // --------------------------------------------------------------------------------------------
    template < typename T, Ret (T::* MPtr) (Args...) const > void Connect(T * ptr) noexcept
    {
        m_Nodes.m_Head = new Node(Member< T, MPtr >(), ptr, m_Nodes.m_Head);
    }

    template < typename T, Ret (T::* MPtr) (Args...) const > void Connect(T & ptr) noexcept
    {
        m_Nodes.m_Head = new Node(Member< T, MPtr >(), std::addressof(ptr), m_Nodes.m_Head);
    }

    // --------------------------------------------------------------------------------------------
    template < typename L > void Connect(L * ptr) noexcept
    {
        m_Nodes.m_Head = new Node(Lambda< L >(), ptr, m_Nodes.m_Head);
    }

    template < typename L > void Connect(L & ptr) noexcept
    {
        m_Nodes.m_Head = new Node(Lambda< L >(), std::addressof(ptr), m_Nodes.m_Head);
    }

    // --------------------------------------------------------------------------------------------
    template < Ret (* FPtr) (Args...) > void Disconnect() noexcept
    {
        m_Nodes.Remove(Free< FPtr >(), 0);
    }

    // --------------------------------------------------------------------------------------------
    template < typename T, Ret (T::* MPtr) (Args...) > void Disconnect(T * ptr) noexcept
    {
        m_Nodes.Remove(Member< T, MPtr >(), ptr);
    }

    template < typename T, Ret (T::* MPtr) (Args...) > void Disconnect(T & ptr) noexcept
    {
        m_Nodes.Remove(Member< T, MPtr >(), std::addressof(ptr));
    }

    // --------------------------------------------------------------------------------------------
    template < typename T, Ret (T::* MPtr) (Args...) const > void Disconnect(T * ptr) noexcept
    {
        m_Nodes.Remove(Member< T, MPtr >(), ptr);
    }

    template < typename T, Ret (T::* MPtr) (Args...) const > void Disconnect(T & ptr) noexcept
    {
        m_Nodes.Remove(Member< T, MPtr >(), std::addressof(ptr));
    }

    // --------------------------------------------------------------------------------------------
    template < typename L > void Disconnect(L * ptr) noexcept
    {
        m_Nodes.Remove(Lambda< L >(), ptr);
    }

    template < typename L > void Disconnect(L & ptr) noexcept
    {
        m_Nodes.Remove(Lambda< L >(), std::addressof(ptr));
    }

    // --------------------------------------------------------------------------------------------
    void Clear() noexcept
    {
        m_Nodes.Clear();
    }

    // --------------------------------------------------------------------------------------------
    void Emit(Args &&... args)
    {
        for (Node * node = m_Nodes.m_Head; node; node = node->m_Next)
        {
            (*node->m_Exec)(node->m_This, std::forward<Args>(args)...);
        }
    }

    // --------------------------------------------------------------------------------------------
    template <typename T> void Query(T && collecter, Args &&... args)
    {
        for (Node * node = m_Nodes.m_Head; node; node = node->m_Next)
        {
            collecter( ( (*node->m_Exec) (node->m_This, std::forward<Args>(args)...) ) );
        }
    }

    // --------------------------------------------------------------------------------------------
    void operator () (Args &&... args)
    {
        for (Node * node = m_Nodes.m_Head; node; node = node->m_Next)
        {
            (*node->m_Exec)(node->m_This, std::forward<Args>(args)...);
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
    EVT_LOGMESSAGE,
    EVT_COUNT
};

// ------------------------------------------------------------------------------------------------
using EBlipCreated          = Signal< void (SQInt32 /* blip */, SQInt32 /* header */, Object & /* payload */), EVT_BLIPCREATED >;
using ECheckpointCreated    = Signal< void (SQInt32 /* checkpoint */, SQInt32 /* header */, Object & /* payload */), EVT_CHECKPOINTCREATED >;
using EKeybindCreated       = Signal< void (SQInt32 /* keybind */, SQInt32 /* header */, Object & /* payload */), EVT_KEYBINDCREATED >;
using EObjectCreated        = Signal< void (SQInt32 /* object */, SQInt32 /* header */, Object & /* payload */), EVT_OBJECTCREATED >;
using EPickupCreated        = Signal< void (SQInt32 /* pickup */, SQInt32 /* header */, Object & /* payload */), EVT_PICKUPCREATED >;
using EPlayerCreated        = Signal< void (SQInt32 /* player */, SQInt32 /* header */, Object & /* payload */), EVT_PLAYERCREATED >;
using ESphereCreated        = Signal< void (SQInt32 /* sphere */, SQInt32 /* header */, Object & /* payload */), EVT_SPHERECREATED >;
using ESpriteCreated        = Signal< void (SQInt32 /* sprite */, SQInt32 /* header */, Object & /* payload */), EVT_SPRITECREATED >;
using ETextdrawCreated      = Signal< void (SQInt32 /* textdraw */, SQInt32 /* header */, Object & /* payload */), EVT_TEXTDRAWCREATED >;
using EVehicleCreated       = Signal< void (SQInt32 /* vehicle */, SQInt32 /* header */, Object & /* payload */), EVT_VEHICLECREATED >;

// ------------------------------------------------------------------------------------------------
using EBlipDestroyed        = Signal< void (SQInt32 /* blip */, SQInt32 /* header */, Object & /* payload */), EVT_BLIPDESTROYED >;
using ECheckpointDestroyed  = Signal< void (SQInt32 /* checkpoint */, SQInt32 /* header */, Object & /* payload */), EVT_CHECKPOINTDESTROYED >;
using EKeybindDestroyed     = Signal< void (SQInt32 /* keybind */, SQInt32 /* header */, Object & /* payload */), EVT_KEYBINDDESTROYED >;
using EObjectDestroyed      = Signal< void (SQInt32 /* object */, SQInt32 /* header */, Object & /* payload */), EVT_OBJECTDESTROYED >;
using EPickupDestroyed      = Signal< void (SQInt32 /* pickup */, SQInt32 /* header */, Object & /* payload */), EVT_PICKUPDESTROYED >;
using EPlayerDestroyed      = Signal< void (SQInt32 /* player */, SQInt32 /* header */, Object & /* payload */), EVT_PLAYERDESTROYED >;
using ESphereDestroyed      = Signal< void (SQInt32 /* sphere */, SQInt32 /* header */, Object & /* payload */), EVT_SPHEREDESTROYED >;
using ESpriteDestroyed      = Signal< void (SQInt32 /* sprite */, SQInt32 /* header */, Object & /* payload */), EVT_SPRITEDESTROYED >;
using ETextdrawDestroyed    = Signal< void (SQInt32 /* textdraw */, SQInt32 /* header */, Object & /* payload */), EVT_TEXTDRAWDESTROYED >;
using EVehicleDestroyed     = Signal< void (SQInt32 /* vehicle */, SQInt32 /* header */, Object & /* payload */), EVT_VEHICLEDESTROYED >;

// ------------------------------------------------------------------------------------------------
using EBlipCustom           = Signal< void (SQInt32 /* blip */, SQInt32 /* header */, Object & /* payload */), EVT_BLIPCUSTOM >;
using ECheckpointCustom     = Signal< void (SQInt32 /* checkpoint */, SQInt32 /* header */, Object & /* payload */), EVT_CHECKPOINTCUSTOM >;
using EKeybindCustom        = Signal< void (SQInt32 /* keybind */, SQInt32 /* header */, Object & /* payload */), EVT_KEYBINDCUSTOM >;
using EObjectCustom         = Signal< void (SQInt32 /* object */, SQInt32 /* header */, Object & /* payload */), EVT_OBJECTCUSTOM >;
using EPickupCustom         = Signal< void (SQInt32 /* pickup */, SQInt32 /* header */, Object & /* payload */), EVT_PICKUPCUSTOM >;
using EPlayerCustom         = Signal< void (SQInt32 /* player */, SQInt32 /* header */, Object & /* payload */), EVT_PLAYERCUSTOM >;
using ESphereCustom         = Signal< void (SQInt32 /* sphere */, SQInt32 /* header */, Object & /* payload */), EVT_SPHERECUSTOM >;
using ESpriteCustom         = Signal< void (SQInt32 /* sprite */, SQInt32 /* header */, Object & /* payload */), EVT_SPRITECUSTOM >;
using ETextdrawCustom       = Signal< void (SQInt32 /* textdraw */, SQInt32 /* header */, Object & /* payload */), EVT_TEXTDRAWCUSTOM >;
using EVehicleCustom        = Signal< void (SQInt32 /* vehicle */, SQInt32 /* header */, Object & /* payload */), EVT_VEHICLECUSTOM >;

// ------------------------------------------------------------------------------------------------
using EPlayerAway           = Signal< void (SQInt32 /* player */, bool /* status */), EVT_PLAYERAWAY >;

// ------------------------------------------------------------------------------------------------
using EPlayerGameKeys       = Signal< void (SQInt32 /* player */, SQInt32 /* previous */, SQInt32 /* current */), EVT_PLAYERGAMEKEYS >;
using EPlayerRename         = Signal< void (SQInt32 /* player */, const SQChar * /* previous */, const SQChar * /* current */), EVT_PLAYERRENAME >;

// ------------------------------------------------------------------------------------------------
using EPlayerRequestClass   = Signal< void (SQInt32 /* player */, SQInt32 /* offset */), EVT_PLAYERREQUESTCLASS >;
using EPlayerRequestSpawn   = Signal< void (SQInt32 /* player */), EVT_PLAYERREQUESTSPAWN >;

// ------------------------------------------------------------------------------------------------
using EPlayerSpawn          = Signal< void (SQInt32 /* player */), EVT_PLAYERSPAWN >;

// ------------------------------------------------------------------------------------------------
using EPlayerStartTyping    = Signal< void (SQInt32 /* player */), EVT_PLAYERSTARTTYPING >;
using EPlayerStopTyping     = Signal< void (SQInt32 /* player */), EVT_PLAYERSTOPTYPING >;

// ------------------------------------------------------------------------------------------------
using EPlayerChat           = Signal< void (SQInt32 /* player */, const SQChar * /* message */), EVT_PLAYERCHAT >;
using EPlayerCommand        = Signal< void (SQInt32 /* player */, const SQChar * /* command */), EVT_PLAYERCOMMAND >;
using EPlayerMessage        = Signal< void (SQInt32 /* player */, SQInt32 /* receiver */, const SQChar * /* message */), EVT_PLAYERMESSAGE >;

// ------------------------------------------------------------------------------------------------
using EPlayerHealth         = Signal< void (SQInt32 /* player */, SQFloat /* previous */, SQFloat /* current */), EVT_PLAYERHEALTH >;
using EPlayerArmour         = Signal< void (SQInt32 /* player */, SQFloat /* previous */, SQFloat /* current */), EVT_PLAYERARMOUR >;
using EPlayerWeapon         = Signal< void (SQInt32 /* player */, SQInt32 /* previous */, SQInt32 /* current */), EVT_PLAYERWEAPON >;
using EPlayerMove           = Signal< void (SQInt32 /* player */, const Vector3 & /* previous */, const Vector3 & /* current */), EVT_PLAYERMOVE >;

// ------------------------------------------------------------------------------------------------
using EPlayerWasted         = Signal< void (SQInt32 /* player */, SQInt32 /* reason */), EVT_PLAYERWASTED >;
using EPlayerKilled         = Signal< void (SQInt32 /* player */, SQInt32 /* killer */, SQInt32 /* reason */, SQInt32 /* body_part */), EVT_PLAYERKILLED >;
using EPlayerTeamKill       = Signal< void (SQInt32 /* player */, SQInt32 /* killer */, SQInt32 /* reason */, SQInt32 /* body_part */), EVT_PLAYERTEAMKILL >;

// ------------------------------------------------------------------------------------------------
using EPlayerSpectate       = Signal< void (SQInt32 /* player */, SQInt32 /* target */), EVT_PLAYERSPECTATE >;
using EPlayerCrashreport    = Signal< void (SQInt32 /* player */, const SQChar * /* report */), EVT_PLAYERCRASHREPORT >;

// ------------------------------------------------------------------------------------------------
using EPlayerBurning        = Signal< void (SQInt32 /* player */, bool /* state */), EVT_PLAYERBURNING >;
using EPlayerCrouching      = Signal< void (SQInt32 /* player */, bool /* state */), EVT_PLAYERCROUCHING >;

// ------------------------------------------------------------------------------------------------
using EPlayerState          = Signal< void (SQInt32 /* player */, SQInt32 /* previous */, SQInt32 /* current */), EVT_PLAYERSTATE >;
using EPlayerAction         = Signal< void (SQInt32 /* player */, SQInt32 /* previous */, SQInt32 /* current */), EVT_PLAYERACTION >;

// ------------------------------------------------------------------------------------------------
using EStateNone            = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_STATENONE >;
using EStateNormal          = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_STATENORMAL >;
using EStateShooting        = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_STATESHOOTING >;
using EStateDriver          = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_STATEDRIVER >;
using EStatePassenger       = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_STATEPASSENGER >;
using EStateEnterDriver     = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_STATEENTERDRIVER >;
using EStateEnterPassenger  = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_STATEENTERPASSENGER >;
using EStateExitVehicle     = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_STATEEXITVEHICLE >;
using EStateUnspawned       = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_STATEUNSPAWNED >;

// ------------------------------------------------------------------------------------------------
using EActionNone           = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_ACTIONNONE >;
using EActionNormal         = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_ACTIONNORMAL >;
using EActionAiming         = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_ACTIONAIMING >;
using EActionShooting       = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_ACTIONSHOOTING >;
using EActionJumping        = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_ACTIONJUMPING >;
using EActionLieDown        = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_ACTIONLIEDOWN >;
using EActionGettingUp      = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_ACTIONGETTINGUP >;
using EActionJumpVehicle    = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_ACTIONJUMPVEHICLE >;
using EActionDriving        = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_ACTIONDRIVING >;
using EActionDying          = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_ACTIONDYING >;
using EActionWasted         = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_ACTIONWASTED >;
using EActionEmbarking      = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_ACTIONEMBARKING >;
using EActionDisembarking   = Signal< void (SQInt32 /* player */, SQInt32 /* previous */), EVT_ACTIONDISEMBARKING >;

// ------------------------------------------------------------------------------------------------
using EVehicleRespawn       = Signal< void (SQInt32 /* vehicle */), EVT_VEHICLERESPAWN >;
using EVehicleExplode       = Signal< void (SQInt32 /* vehicle */), EVT_VEHICLEEXPLODE >;

// ------------------------------------------------------------------------------------------------
using EVehicleHealth        = Signal< void (SQInt32 /* vehicle */, SQFloat /* previous */, SQFloat /* current */), EVT_VEHICLEHEALTH >;
using EVehicleMove          = Signal< void (SQInt32 /* vehicle */, const Vector3 & /* previous */, const Vector3 &/* current */), EVT_VEHICLEMOVE >;

// ------------------------------------------------------------------------------------------------
using EPickupRespawn        = Signal< void (SQInt32 /* pickup */), EVT_PICKUPRESPAWN >;

// ------------------------------------------------------------------------------------------------
using EKeybindKeyPress      = Signal< void (SQInt32 /* player */, SQInt32 /* keybind */), EVT_KEYBINDKEYPRESS >;
using EKeybindKeyRelease    = Signal< void (SQInt32 /* player */, SQInt32 /* keybind */), EVT_KEYBINDKEYRELEASE >;

// ------------------------------------------------------------------------------------------------
using EVehicleEmbarking     = Signal< void (SQInt32 /* player */, SQInt32 /* vehicle */, SQInt32 /* slot */), EVT_VEHICLEEMBARKING >;
using EVehicleEmbarked      = Signal< void (SQInt32 /* player */, SQInt32 /* vehicle */, SQInt32 /* slot */), EVT_VEHICLEEMBARKED >;
using EVehicleDisembark     = Signal< void (SQInt32 /* player */, SQInt32 /* vehicle */), EVT_VEHICLEDISEMBARK >;

// ------------------------------------------------------------------------------------------------
using EPickupClaimed        = Signal< void (SQInt32 /* player */, SQInt32 /* pickup */), EVT_PICKUPCLAIMED >;
using EPickupCollected      = Signal< void (SQInt32 /* player */, SQInt32 /* pickup */), EVT_PICKUPCOLLECTED >;

// ------------------------------------------------------------------------------------------------
using EObjectShot           = Signal< void (SQInt32 /* player */, SQInt32 /* object */, SQInt32 /* weapon */), EVT_OBJECTSHOT >;
using EObjectBump           = Signal< void (SQInt32 /* player */, SQInt32 /* object */), EVT_OBJECTBUMP >;

// ------------------------------------------------------------------------------------------------
using ECheckpointEntered    = Signal< void (SQInt32 /* player */, SQInt32 /* checkpoint */), EVT_CHECKPOINTENTERED >;
using ECheckpointExited     = Signal< void (SQInt32 /* player */, SQInt32 /* checkpoint */), EVT_CHECKPOINTEXITED >;

// ------------------------------------------------------------------------------------------------
using ESphereEntered        = Signal< void (SQInt32 /* player */, SQInt32 /* sphere */), EVT_SPHEREENTERED >;
using ESphereExited         = Signal< void (SQInt32 /* player */, SQInt32 /* sphere */), EVT_SPHEREEXITED >;

// ------------------------------------------------------------------------------------------------
using EServerFrame          = Signal< void (SQFloat /* delta */), EVT_SERVERFRAME >;

// ------------------------------------------------------------------------------------------------
using EServerStartup        = Signal< void (void), EVT_SERVERSTARTUP >;
using EServerShutdown       = Signal< void (void), EVT_SERVERSHUTDOWN >;

// ------------------------------------------------------------------------------------------------
using EInternalCommand      = Signal< void (SQInt32 /* type */, const SQChar * /* text */), EVT_INTERNALCOMMAND >;
using ELoginAttempt         = Signal< void (const SQChar * /* name */, const SQChar * /* pass */, const SQChar * /* addr */), EVT_LOGINATTEMPT >;

// ------------------------------------------------------------------------------------------------
using ECustomEvent          = Signal< void (SQInt32 /* group */, SQInt32 /* header */, Object & /* payload */), EVT_CUSTOMEVENT >;

// ------------------------------------------------------------------------------------------------
using EWorldOption          = Signal< void (SQInt32 /* option */, Object & /* value */), EVT_WORLDOPTION >;
using EWorldToggle          = Signal< void (SQInt32 /* option */, bool /* value */), EVT_WORLDTOGGLE >;

// ------------------------------------------------------------------------------------------------
using EScriptReload           = Signal< void (SQInt32 /* header */, Object & /* payload */), EVT_SCRIPTRELOAD >;

// ------------------------------------------------------------------------------------------------
using ELogMessage           = Signal< void (SQInt32 /* type */, const SQChar * /* message */), EVT_LOGMESSAGE >;

} // Namespace:: SqMod

#endif // _SIGNALS_HPP_
